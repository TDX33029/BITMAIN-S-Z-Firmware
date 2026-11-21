/* ============================================================================
 * rewritten/cgminer/cfg.c — cgminer 配置/选项层转写 (batch cfg_b1, 分类 40_config, 共 17 函数)
 *
 * 内容:
 *   1) 配置文件解析: parse_config (JSON 选项分发 + include 递归入口) + json_array_size
 *   2) 默认配置文件名与 curses 设置菜单: default_save_file / set_options
 *   3) 选项注册与命令行解析 (cgminer ccan/opt 层): opt_set_charp / check_opt /
 *      next_name / first_opt / next_opt / first_lopt / next_lopt / first_sopt /
 *      next_sopt / consume_option / parse_one
 *   4) bitmain 桩: bm1740_makeup_set_config_cmd (恒报错返回 -1) / set_logfile_openflag
 *
 * 事实还原 (全部以反编译偏移 / ELF 数据段核实):
 *   - 双选项表机制: 静态命令表 opt_config_table 位于 .data 0x00075600 (70 个实选项 +
 *     末项 OPT_END 共 71 项, 项距 28 字节, axi.c 批已逐项解出核实), parse_config/
 *     write_config 遍历它; 动态表 opt_table (DAT_0009d77c) 由 check_opt/add_opt
 *     (opt_register_table, FUN_0005bbb8/0005b9a8, 他批) realloc 逐项追加,
 *     parse_one 与 first_* / next_* 系列遍历它。
 *   - struct opt_table 共 7 字 (28 字节), 布局由访问点锁定:
 *     names+0x00 / type+0x04 / cb+0x08 / cb_arg+0x0C / show+0x10 / u+0x14 / desc+0x18,
 *     与上游 cgminer ccan/opt/opt.h 完全一致。
 *   - 选项类型枚举 (OPT_NOARG=1 / OPT_HASARG=2 / OPT_PROCESSARG=4 / OPT_SUBTABLE=8 /
 *     OPT_END=16) 由表项与比较指令核实: ELF 表中 --sched-start/--sched-stop type=4,
 *     --api-allow 等 cb_arg=0x5b1d4(opt_set_charp), 末项 type=0x10。
 *   - 选项名段格式: 竖线分隔, 首段长选项形如 "--foo|-f"。next_name 返回"分隔符后
 *     跳过 1 字符"的名字段 (上游 ccan next_name 语义): 后续段 "-f" 变 "f" (len==1
 *     → 短选项), "--bar" 变 "-bar" (p[0]=='-' → 长选项, 由 first_lopt 再剥 '-')。
 *   - JSON 类型常量 (jansson 2.x): 0 OBJECT / 1 ARRAY / 2 STRING / 5 TRUE,
 *     parse_config 按其分发 cb (true) / cb_arg (string) / 递归 (array 内 object)。
 *   - 日志门控 (与 uart.c/log.c 统一模式): LOG_ERR(3) 调用点仅当
 *     DAT_0007ad48!=0 || DAT_00078e08!=0 || 2 < DAT_000766c4 才真正调 applog。
 *   - 本文件 17 函数中 11 个与上游 cgminer (ccan/opt) 逐语义一致 (命名/结构对齐),
 *     parse_config/default_save_file/set_options 为 cgminer.c 配置层 (set_options
 *     的 [Q]ueue/[S]cantime/[E]xpiry 菜单为 bitmain 定制)。
 *   - name_map 冻结名与实际选项对照: set_logfile_openflag (FUN_0000f9a8) 实为静态表
 *     第 0 项 "--version-file" (desc "Set miner version file") 的 cb_arg, 把参数存入
 *     DAT_00076bf0; 命名沿用 name_map, 行为以横幅注释为准。
 *
 * 注: extern 声明均取自 name_map 语义名, 定义分布在其余批次文件 (json.c/axi.c/
 *     log.c/fan.c 等); 共享 DAT_ 全局的定义归属与本文件所有权见各声明处注释,
 *     最终应上移至 rewritten/include/*.h。
 */

#include <stdio.h>      /* snprintf/fopen/fclose/FILE */
#include <stdlib.h>     /* getenv/strdup/strtok/free/realloc/malloc */
#include <string.h>     /* strcmp/strcpy/strncmp/strlen/memmove/stpcpy */
#include <strings.h>    /* strncasecmp */
#include <stdint.h>     /* uint8_t/uint32_t */
#include <stddef.h>     /* size_t */
#include <assert.h>     /* __assert_fail */
#include <err.h>        /* errx (check_opt 校验失败即退出, 上游 ccan/opt 同) */
#include <sys/stat.h>   /* struct stat/mkdir/__xstat */
#include <curses.h>     /* WINDOW/stdscr/wgetch/immedok */

#define LOG_ERR 3       /* syslog LOG_ERR (反编译调用点恒为数值 3) */

/* ---- ccan/opt 选项类型 (值由 ELF 表项与 parse_one/check_opt 比较指令核实) ---- */
#define OPT_NOARG       1   /* 选项不接参数: 触发 cb */
#define OPT_HASARG      2   /* 选项接参数: 触发 cb_arg */
#define OPT_PROCESSARG  4   /* 选项接参数且写回 u 变量: cb_arg 前先 opt_set_charp */
#define OPT_SUBTABLE    8   /* 子表项 (动态注册时展开; 静态表遍历遇到即 assert) */
#define OPT_END         16  /* 静态命令表终止项 */

/* ---- jansson 2.x 类型 (与 json.c 保持一致, 待上移 include/) ---- */
typedef enum json_type {
	JSON_OBJECT  = 0,
	JSON_ARRAY   = 1,
	JSON_STRING  = 2,
	JSON_INTEGER = 3,
	JSON_REAL    = 4,
	JSON_TRUE    = 5,
	JSON_FALSE   = 6,
	JSON_NULL    = 7
} json_type;

typedef struct json_t {
	int type;       /* +0x00: json_type 枚举值 */
	int refcount;   /* +0x04: 引用计数 (本批不访问) */
} json_t;

/* ---- struct opt_table (7 字/28 字节; 偏移由 parse_one/parse_config 访问点锁定) ---- */
struct opt_table {
	const char *names;      /* +0x00: 竖线分隔选项名, 形如 "--long|-s" */
	unsigned int type;      /* +0x04: OPT_* 枚举 */
	char *(*cb)(void *arg); /* +0x08: OPT_NOARG 回调, 返回错误串 (NULL=无错) */
	char *(*cb_arg)(const char *optarg, void *arg); /* +0x0C: 带参回调, 返回错误串 */
	void (*show)(char *buf, const void *arg);       /* +0x10: 显示当前值 (本批不调用) */
	void *u;                /* +0x14: 回调参数 (通常指向选项变量) */
	const char *desc;       /* +0x18: 描述 */
};

/* ================= 外部函数 (name_map 语义名) ================= */

extern void  applog(uint32_t prio, char *buf, ...); /* FUN_00038730: 第 3 参非 0 → 强制 curses 刷新 */
extern void  _wlogprint(const char *str);                          /* FUN_00018bc0 */
extern void  clear_logwin(void);                                   /* FUN_00018ad8 (41_logging/log.c 定义) */
extern void  logwin_update(void);                                  /* FUN_00018b44 (41_logging/log.c 定义) */
extern char *curses_input(const char *prompt);                     /* FUN_00025fdc */
extern long  curses_int(const char *prompt);                       /* FUN_000260d0 (41_logging/log.c 定义) */
extern void  restart_mine(void);                                   /* FUN_0001d7c8 (14_temp_fan/fan.c 定义) */
extern void  write_config(FILE *fcfg);                             /* FUN_00015b64 (15_fpga_axi/axi.c 定义) */
extern char *load_json_file(const char *filename);                 /* FUN_00016d98 (22_json_rpc/json.c 定义) */

/* --- jansson 内部 (B3/D 分批) --- */
extern json_t *json_object_get(json_t *object, const char *key);   /* FUN_00059e20 */
extern char   *json_string_value(const json_t *value);             /* FUN_0005a178 */
extern json_t *json_array_get(const json_t *array, size_t index);  /* FUN_0005a008 */

/* ================= 外部全局 (DAT_ 原名保留, 注释为已核实语义) ================= */

extern char    *DAT_00076bdc;   /* default_config: "config" 命令记录的默认配置路径
                                 * (default_save_file 优先使用; 写方在他批 set_default_config) */
extern WINDOW  *DAT_0007b430;   /* logwin: curses 日志窗口 (immedok 即时刷新) */
extern uint8_t  DAT_0007bac8;   /* opt_loginput: curses 交互会话标志 (1=菜单输入中) */
extern uint8_t  DAT_0007ad48;   /* applog 门控 A: 非 0 → applog 走 syslog 输出 */
extern uint8_t  DAT_00078e08;   /* applog 门控 B = opt_verbose (curses 'v' 联动) */
extern int      DAT_000766c4;   /* applog 门控 C: int 日志级别 (LOG_ERR=3 阈值) */
extern int      DAT_00075ddc;   /* opt_queue: 排队工作数上限 (--queue, ELF .data 初值 1) */
extern int      DAT_00075dd8;   /* 队列动态目标计数: 主循环按需向 opt_queue 自增靠拢
                                 * (FUN_0000b928; ELF .data 初值 1; set_options 调低 queue 时同步压低) */
extern int      DAT_00075df0;   /* opt_scantime: 当前工作扫描时间上限秒 (--scan-time, ELF .data 初值 -1) */
extern int      DAT_00075df4;   /* opt_expiry: 工作过期秒数 (--expiry, ELF .data 初值 120) */

/* opt_config_table: 静态选项命令表 (.data 0x00075600, 70 实选项 + 末项 OPT_END;
 * 全部表项已由 axi.c 批从 ELF 解出核实; parse_config 直接遍历它)。 */
extern uint32_t DAT_00075600[];

/* 本文件内定义、前方调用的函数前置声明 */
unsigned int json_array_size(const json_t *array);      /* 定义见下 (FUN_00059fec) */
char *opt_set_charp(const char *arg, char **p);         /* 定义见下 (FUN_0005b1d4) */

/* ================= 本模块全局 (其他批次经 extern 引用) ================= */

/* --- ccan/opt 动态选项表状态 (opt_register_table/add_opt/opt_parse 等他批共享) --- */
struct opt_table *DAT_0009d77c = NULL;  /* opt_table: 动态选项表 (check_opt/add_opt realloc 逐项追加) */
unsigned int      DAT_0009d774 = 0;     /* opt_count: 动态表项数 */
unsigned int      DAT_0009d76c = 0;     /* opt_num_long: 已注册长选项数 (check_opt 统计) */
unsigned int      DAT_0009d770 = 0;     /* opt_num_short: 已注册短选项数 */
unsigned int      DAT_0009d778 = 0;     /* opt_num_short_arg: 带参短选项数 */

/* --- cgminer.c 配置层状态 (均为 .bss, 初值 0) --- */
char *DAT_00076bf0;             /* miner version 文件路径 (--version-file 参数;
                                 * FUN_0000b928 以 fopen(...,"rb") 读取并报
                                 * "Open miner version file %s error") */
int  DAT_00076ff4;              /* fileconf_load: 配置文件解析状态
                                 * 0=未解析, 1=解析中, -1=曾遇非法值 (主流程据此告警) */
char DAT_00076f2c[200];         /* err_buf: parse_config 非文件配置 (JSON 串/递归) 路径的
                                 * 错误消息缓冲, 随返回值交调用方 */

/* ================= 模块内部函数 (static 前向声明) ================= */

static const char *first_name(const char *names, unsigned *len);
static const char *next_name(const char *names, unsigned *len);
static const char *first_opt(unsigned *i, unsigned *len);
static const char *next_opt(const char *p, unsigned *i, unsigned *len);
static void consume_option(int *argc, char **argv, unsigned optnum);

/* ============================================================================
 * 函数转写 (转写顺序: json/配置组 → opt 表组 → 命令行解析组)
 * ============================================================================ */

// === set_logfile_openflag === 原 FUN_0000f9a8 @ 0x0000f9a8
// 来源: reversed/cgminer/40_config/FUN_0000f9a8.c | 分类: 40_config
// 语义: 静态选项表第 0 项 "--version-file" (desc "Set miner version file") 的
//       cb_arg 回调: 把参数串存入 DAT_00076bf0 (miner version 文件路径, 供
//       FUN_0000b928 以 fopen(...,"rb") 读取), 恒返回 NULL (= 无错误)。
//       命名沿用 name_map 冻结名。
char *set_logfile_openflag(const char *arg, void *argp)
{
	opt_set_charp(arg, &DAT_00076bf0);  /* 第 2 参 (表项 u.arg = DAT_0009d784) 原码未使用 */
	return NULL;
}

// === default_save_file === 原 FUN_00016150 @ 0x00016150
// 来源: reversed/cgminer/40_config/FUN_00016150.c | 分类: 40_config
// 语义: 填默认配置文件路径到 filename: default_config (DAT_00076bdc) 非空则直接
//       拷贝返回; 否则 $HOME + "/.cgminer/" (mkdir 0777) + "cgminer.conf",
//       无 HOME 时以空串起始拼接。上游 cgminer.c default_save_file 逐语义对应
//       (stpcpy 后尾写 '/'/'\0' 即 strcat "/", builtin_strncpy 为内联 strcat)。
void default_save_file(char *filename)
{
	char *home;
	char *end;

	if (DAT_00076bdc != NULL && *DAT_00076bdc != '\0') {
		strcpy(filename, DAT_00076bdc);
		return;
	}

	home = getenv("HOME");      /* 原码按用途连续调 getenv("HOME") 三次, 等价合并为一次 */
	if (home == NULL || *home == '\0') {
		*filename = '\0';
	} else {
		end = stpcpy(filename, home);
		end[0] = '/';
		end[1] = '\0';
	}
	strcat(filename, ".cgminer/");
	mkdir(filename, 0777);      /* 原 0x1ff */
	strcat(filename, "cgminer.conf");
}

// === set_options === 原 FUN_000267b0 @ 0x000267b0
// 来源: reversed/cgminer/40_config/FUN_000267b0.c | 分类: 40_config
// 语义: curses 选项菜单 (进入置 opt_loginput=1 并对 logwin 开即时刷新):
//       [Q] 改排队工作数 (<10000, 写 opt_queue; 若调低则同步压低队列动态目标
//           DAT_00075dd8)、[S] 改 scantime (<=9999)、[E] 改 expiry (<=9999),
//           越界一律 "Invalid selection"; [W] 写配置文件 (默认名来自
//           default_save_file, 目标已存在需 'y' 确认, 经 write_config 落盘,
//           打不开报 "Cannot open or create file"); [C] 重启 ('y' 确认后
//           restart_mine, 跳过清屏直接收尾)。其余按键/未确认 → clear_logwin。
//       收尾统一 immedok 关闭 + opt_loginput 清 0。"Invalid selection"/提示串
//       原码为栈上逐字节构造, 此处还原为等价字面量。
void set_options(void)
{
	char input;
	char prompt[0x1032];        /* 4146 字节 (上游 PATH_MAX+50; 菜单 snprintf 以 0x1000 为界) */
	char filename[4096];
	struct stat statbuf;
	FILE *fcfg;
	char *str;
	unsigned int val;

	DAT_0007bac8 = 1;
	immedok(DAT_0007b430, 1);
	clear_logwin();

	/* 原标签 LAB_000267f4: retry 菜单循环头 */
	for (;;) {
		snprintf(prompt, 0x1000,
			 "[Q]ueue: %d\n[S]cantime: %d\n[E]xpiry: %d\n[W]rite config file\n[C]gminer restart\n",
			 DAT_00075ddc, DAT_00075df0, DAT_00075df4);
		_wlogprint(prompt);
		_wlogprint("Select an option or any other key to return\n");
		logwin_update();
		input = wgetch(stdscr);

		if (!strncasecmp(&input, "q", 1)) {
			val = (unsigned int)curses_int("Extra work items to queue");
			if (val < 10000) {
				DAT_00075ddc = val;
				if ((int)val < (int)DAT_00075dd8)   /* 有符号比较: 调低 queue 时压低动态目标 */
					DAT_00075dd8 = val;
			} else {
				_wlogprint("Invalid selection\n");
			}
			continue;                   /* 回菜单 */
		}
		if (!strncasecmp(&input, "s", 1)) {
			val = (unsigned int)curses_int("Set scantime in seconds");
			if (9999 < val)
				_wlogprint("Invalid selection\n");
			else
				DAT_00075df0 = val;
			continue;
		}
		if (!strncasecmp(&input, "e", 1)) {
			val = (unsigned int)curses_int("Set expiry time in seconds");
			if (9999 < val)
				_wlogprint("Invalid selection\n");
			else
				DAT_00075df4 = val;
			continue;
		}
		if (!strncasecmp(&input, "w", 1)) {
			default_save_file(filename);
			snprintf(prompt, 0x1032, "Config filename to write (Enter for default) [%s]", filename);
			str = curses_input(prompt);
			if (strcmp(str, "-1") == 0) {       /* 取消输入 (原码为 3 字节内联比较) */
				free(str);
				goto writeout;                  /* 原标签 LAB_00026928 */
			}
			strcpy(filename, str);
			free(str);
			if (__xstat(3, filename, &statbuf) == 0) {  /* glibc stat 包装 (_STAT_VER=3) */
				_wlogprint("File exists, overwrite?\n");
				input = wgetch(stdscr);
				if (strncasecmp(&input, "y", 1) != 0)
					continue;                   /* 不覆盖 → 回菜单 */
			}
writeout:
			fcfg = fopen(filename, "w");
			if (fcfg == NULL) {
				_wlogprint("Cannot open or create file\n");
			} else {
				write_config(fcfg);
				fclose(fcfg);
			}
			continue;
		}
		if (!strncasecmp(&input, "c", 1)) {
			_wlogprint("Are you sure?\n");
			input = wgetch(stdscr);
			if (!strncasecmp(&input, "y", 1)) {
				restart_mine();
				goto cleanup;           /* 原标签 LAB_00026b54: 确认重启则跳过清屏 */
			}
		}
		clear_logwin();
		break;
	}

cleanup:
	immedok(DAT_0007b430, 0);
	DAT_0007bac8 = 0;
}

// === parse_config === 原 FUN_00016dec @ 0x00016dec
// 来源: reversed/cgminer/40_config/FUN_00016dec.c | 分类: 40_config
// 语义: 遍历静态选项命令表 DAT_00075600 (opt_config_table) 把 JSON 配置对象分发
//       到各选项回调: 每项 names 以 '|' strtok 拆段, 仅长选项段 (p[1]=='-') 参与
//       匹配 (本固件编译无上游的 strlen<3 检查); 值类型分发: JSON 字符串 → cb_arg
//       (OPT_PROCESSARG 先 opt_set_charp 写回 u 变量), JSON 数组 → 逐元素同上、
//       对象元素递归 parse_config(elem, false), JSON true → cb (仅 OPT_NOARG),
//       其余 → "Invalid value"。出错时: 非文件配置 (fileconf==0, 即 JSON 串/递归
//       路径) 首错即经 err_buf (DAT_00076f2c) 返回; 配置文件路径仅 applog(LOG_ERR)
//       (受日志门控) 并置 fileconf_load (DAT_00076ff4) = -1 后继续。
//       表遍历到 OPT_END 结束后处理 "include" 键: 字符串值 → load_json_file;
//       遇 OPT_SUBTABLE 表项触发上游 assert (原 __assert_fail 文本保留)。
char *parse_config(json_t *config, int fileconf)
{
	char *name;
	char *p;
	char *err;
	const char *str;
	json_t *val;
	json_t *arr_val;
	unsigned int index;
	const struct opt_table *opt = (const struct opt_table *)DAT_00075600;

	if (fileconf != 0 && DAT_00076ff4 == 0)
		DAT_00076ff4 = 1;

	/* 原码循环旋转: 先判末项 OPT_END/include 出口; 遇 OPT_SUBTABLE 项落到表尾 assert */
	for (; opt->type != OPT_END; opt++) {
		if (opt->type & OPT_SUBTABLE)
			goto opt_subtable_assert;

		if (opt->names == NULL)
			continue;

		name = strdup(opt->names);
		for (p = strtok(name, "|"); p != NULL; p = strtok(NULL, "|")) {
			err = NULL;

			/* 只匹配长选项段: 第 2 字符必须为 '-' ("--foo" 形段) */
			if (p[1] != '-')
				continue;

			val = json_object_get(config, p + 2);
			if (val == NULL)
				continue;

			if ((opt->type & (OPT_HASARG | OPT_PROCESSARG)) != 0 && val->type == JSON_STRING) {
				str = json_string_value(val);
				err = opt->cb_arg(str, opt->u);
				if (opt->type == OPT_PROCESSARG)
					opt_set_charp(str, (char **)opt->u);
			} else if ((opt->type & (OPT_HASARG | OPT_PROCESSARG)) != 0 && val->type == JSON_ARRAY) {
				/* 数组值: 字符串元素按带参选项逐个下发; 对象元素递归解析 (fileconf=false)。
				 * 原 json_array_foreach: 每轮重读数组长度 (与反编译 while 条件一致) */
				for (index = 0; index < json_array_size(val) &&
				     (arr_val = json_array_get(val, index)) != NULL; index++) {
					if (arr_val->type == JSON_STRING) {
						str = json_string_value(arr_val);
						err = opt->cb_arg(str, opt->u);
						if (opt->type == OPT_PROCESSARG)
							opt_set_charp(str, (char **)opt->u);
					} else if (arr_val->type == JSON_OBJECT) {
						err = parse_config(arr_val, 0);
					}
					if (err != NULL)
						break;              /* 原 joined_r0x00016fd4: 带错退出遍历 */
				}
			} else if ((opt->type & OPT_NOARG) != 0 && val->type == JSON_TRUE) {
				err = opt->cb(opt->u);
			} else {
				err = "Invalid value";
			}

			if (err != NULL) {
				/* 配置文件里的非法值允许跳过 (前提 JSON 本身仍合法); 其余立即报错返回 */
				if (fileconf == 0) {
					snprintf(DAT_00076f2c, 200, "Parsing JSON option %s: %s", p, err);
					return DAT_00076f2c;
				}
				if ((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0') || (2 < DAT_000766c4)) {
					char logbuf[0x1000];

					snprintf(logbuf, 0x1000, "Invalid config option %s: %s", p, err);
					applog(LOG_ERR, logbuf, 0);
				}
				DAT_00076ff4 = -1;
			}
		}
		free(name);
	}

	/* 表尾: 处理 include 键 (JSON_INCLUDE_CONF = "include") */
	val = json_object_get(config, "include");
	if (val != NULL && val->type == JSON_STRING)
		return load_json_file(json_string_value(val));  /* 原调用附第 2 参 NULL, load_json_file 未使用 */
	return NULL;

opt_subtable_assert:
	/* 遍历遇 OPT_SUBTABLE 项 → 上游 assert(!(opt->type & OPT_SUBTABLE)) */
	__assert_fail("!(opt->type & OPT_SUBTABLE)", "cgminer.c", 0x7a0, "parse_config");
}

// === bm1740_makeup_set_config_cmd === 原 FUN_0003ae6c @ 0x0003ae6c
// 来源: reversed/cgminer/40_config/FUN_0003ae6c.c | 分类: 40_config
// 语义: bitmain 构帧辅助的错误桩: applog(LOG_ERR) 报
//       "bm1740_makeup_set_config_cmd input param error: str length = 13" 后恒
//       返回 -1 (0xffffffff)。该地址仅含此失败路径 (正常构帧在他处/内联)。
int bm1740_makeup_set_config_cmd(void)
{
	char logbuf[0x1000];

	snprintf(logbuf, 0x1000, "%s input param error: str length = %u\n", __func__, 0xd);
	applog(LOG_ERR, logbuf, 0);
	return -1;
}

// === json_array_size === 原 FUN_00059fec @ 0x00059fec
// 来源: reversed/cgminer/40_config/FUN_00059fec.c | 分类: 40_config
// 语义: jansson json_array_size: 入参 NULL 或非 JSON_ARRAY (+0x00 类型 != 1)
//       返回 0, 否则返回 +0x0C 处的数组长度字段。parse_config 用作数组遍历上界。
unsigned int json_array_size(const json_t *array)
{
	if (array == NULL)
		return 0;
	if (array->type != JSON_ARRAY)
		return 0;
	return ((const uint32_t *)array)[3];    /* +0x0C: json_array_t.size */
}

// === opt_set_charp === 原 FUN_0005b1d4 @ 0x0005b1d4
// 来源: reversed/cgminer/40_config/FUN_0005b1d4.c | 分类: 40_config
// 语义: 上游 opt_set_charp: 把字符串参数写入 *p (通常为选项变量)。在静态表中
//       以 cb_arg 身份直接使用 (--api-allow/--api-description/--logfile 等,
//       恒返回 NULL=无错), 亦被 parse_one/parse_config 在 OPT_PROCESSARG 时
//       显式调用以写回 u 变量。反编译显示恒返回 0。
char *opt_set_charp(const char *arg, char **p)
{
	*p = arg;
	return NULL;
}

/* first_name — 原二进制无此独立符号 (在 check_opt/first_opt/next_opt 调用点内联
 * 展开为逐字节扫描循环); 抽出为静态辅助消除三处重复, 语义与上游 ccan first_name
 * 一致: 跳过开头分隔符 ('-' 或 '|'), 返回名字段首地址 (names+1), *len = 该段
 * 长度。展开循环逐字节等价于 strcspn(names+1, "|= ")。 */
static const char *first_name(const char *names, unsigned *len)
{
	const char *p = names + 1;
	const char *q;
	unsigned int n = 0;

	if (*p != '\0' && *p != '|' && *p != '=' && *p != ' ') {
		q = p;
		do {
			q++;
			n++;
		} while (*q != '\0' && *q != '|' && *q != '=' && *q != ' ');
	}
	*len = n;
	return p;
}

// === next_name === 原 FUN_0005b544 @ 0x0005b544
// 来源: reversed/cgminer/40_config/FUN_0005b544.c | 分类: 40_config
// 语义: 上游 ccan next_name: names 指向当前名字段、*len 为其长度; 分隔符位置
//       names[*len] 为 ' '/'='/NUL → 返回 NULL (名字序列结束); 否则 ('|' 等)
//       返回下一段并更新 *len。下一段 = first_name(sep+1) 的返回: 跳过分隔符后
//       再跳 1 字符 (上游原语义): 后续段 "-f" 读出 "f" (len==1 → 短选项),
//       "--bar" 读出 "-bar" (p[0]=='-' → 长选项, first_lopt 再剥 '-')。
static const char *next_name(const char *names, unsigned *len)
{
	const char *sep = names + *len;
	char c;

	c = *sep;
	if (c == ' ' || c == '=')
		return NULL;
	if (c == '\0')
		return NULL;
	return first_name(sep + 1, len);
}

// === first_opt === 原 FUN_0005b5ec @ 0x0005b5ec
// 来源: reversed/cgminer/40_config/FUN_0005b5ec.c | 分类: 40_config
// 语义: 上游 ccan first_opt (模块内部): 从动态表 opt_table (DAT_0009d77c) 第 0 项
//       起扫描, *i 置为首个 type != OPT_SUBTABLE 的表项序号并返回其首个名字段
//       (first_name(names)), 段长经 *len 带回; 空表或全是子表 → NULL。
static const char *first_opt(unsigned *i, unsigned *len)
{
	const struct opt_table *opt;

	for (*i = 0; *i < DAT_0009d774; (*i)++) {
		opt = &((const struct opt_table *)DAT_0009d77c)[*i];
		if (opt->type == OPT_SUBTABLE)
			continue;
		return first_name(opt->names, len);
	}
	return NULL;
}

// === next_opt === 原 FUN_0005b6d4 @ 0x0005b6d4
// 来源: reversed/cgminer/40_config/FUN_0005b6d4.c | 分类: 40_config
// 语义: 上游 ccan next_opt (模块内部): 从 *i 起在动态表上继续遍历 (跳过
//       OPT_SUBTABLE 项): p 非空 → 先 next_name 取同表项下一段, 有则返回;
//       无 (NULL) → 前进到下一非子表表项取其首段 (first_name); *i 同步前进;
//       越界 (>= opt_count) 返回 NULL。p 为 NULL 直接取 *i 处表项首段。
static const char *next_opt(const char *p, unsigned *i, unsigned *len)
{
	const struct opt_table *opt;

	for (; *i < DAT_0009d774; (*i)++) {
		opt = &((const struct opt_table *)DAT_0009d77c)[*i];
		if (opt->type == OPT_SUBTABLE)
			continue;
		if (p == NULL)
			return first_name(opt->names, len);
		p = next_name(p, len);
		if (p != NULL)
			return p;
	}
	return NULL;
}

// === check_opt === 原 FUN_0005b7c8 @ 0x0005b7c8
// 来源: reversed/cgminer/40_config/FUN_0005b7c8.c | 分类: 40_config
// 语义: 上游 ccan/opt check_opt: 校验选项表项并统计名字段, 通过后把整项 (28 字)
//       追加进动态表 opt_table (realloc) 且 opt_count++。校验规则:
//       type 必须为 OPT_HASARG/OPT_NOARG/OPT_PROCESSARG (否则 errx "unknown
//       entry type"); desc 非空; names 以 '-' 开头; 逐段 (first_name/next_name):
//       '-' 段为长选项 (len==1 即 "--" → errx invalid long option) 计入
//       opt_num_long; 非 '-' 段为短选项 (len!=1 → errx invalid short option,
//       消息含前导字符) 计入 opt_num_short, 带参类型 (HASARG/PROCESSARG) 再计
//       opt_num_short_arg; OPT_NOARG 项名字段后不得再跟 ' '/'=' 参数段 (errx
//       does not take arguments)。errx 均不返回。
void check_opt(struct opt_table *entry)
{
	const char *p;
	unsigned int len;

	if (entry->type != OPT_HASARG && entry->type != OPT_NOARG && entry->type != OPT_PROCESSARG)
		errx(1, "Option %s: unknown entry type %u", entry->names, entry->type);

	if (entry->desc == NULL)
		errx(1, "Option %s: description cannot be NULL", entry->names);

	if (entry->names[0] != '-')
		errx(1, "Option %s: does not begin with '-'", entry->names);

	for (p = first_name(entry->names, &len); p != NULL; p = next_name(p, &len)) {
		if (*p == '-') {
			if (len == 1)
				errx(1, "Option %s: invalid long option '--'", entry->names);
			DAT_0009d76c++;                     /* opt_num_long++ */
		} else {
			if (len != 1)
				errx(1, "Option %s: invalid short option '%.*s'", entry->names, len + 1, p - 1);
			DAT_0009d770++;                     /* opt_num_short++ */
			if (entry->type == OPT_HASARG || entry->type == OPT_PROCESSARG)
				DAT_0009d778++;                 /* opt_num_short_arg++ */
		}
		/* OPT_NOARG 项的名字段后不得再挂参数段 */
		if (entry->type == OPT_NOARG) {
			if (p[len] == ' ' || p[len] == '=')
				errx(1, "Option %s: does not take arguments '%s'", entry->names, p + len + 1);
		}
	}

	DAT_0009d77c = realloc(DAT_0009d77c, (DAT_0009d774 + 1) * 0x1c);
	((struct opt_table *)DAT_0009d77c)[DAT_0009d774] = *entry;  /* 原 7 字逐字拷贝 */
	DAT_0009d774++;
}

// === first_lopt === 原 FUN_0005ba08 @ 0x0005ba08
// 来源: reversed/cgminer/40_config/FUN_0005ba08.c | 分类: 40_config
// 语义: 上游 ccan first_lopt: 自 first_opt 起遍历动态表名字段, 首个以 '-' 开头的
//       段即长选项: 剥掉该 '-' 返回 (段首 +1), *len 减 1; 无 → NULL。
const char *first_lopt(unsigned *i, unsigned *len)
{
	const char *p;

	for (p = first_opt(i, len); p != NULL; p = next_opt(p, i, len)) {
		if (p[0] == '-') {
			/* Skip leading "-" */
			(*len)--;
			p++;
			break;
		}
	}
	return p;
}

// === next_lopt === 原 FUN_0005ba60 @ 0x0005ba60
// 来源: reversed/cgminer/40_config/FUN_0005ba60.c | 分类: 40_config
// 语义: 上游 ccan next_lopt: 自当前长选项段继续 (next_opt), 首个以 '-' 开头的段
//       剥 '-' 返回; 无 → NULL。
const char *next_lopt(const char *p, unsigned *i, unsigned *len)
{
	for (p = next_opt(p, i, len); p != NULL; p = next_opt(p, i, len)) {
		if (p[0] == '-') {
			/* Skip leading "-" */
			(*len)--;
			p++;
			break;
		}
	}
	return p;
}

// === first_sopt === 原 FUN_0005bab8 @ 0x0005bab8
// 来源: reversed/cgminer/40_config/FUN_0005bab8.c | 分类: 40_config
// 语义: 上游 ccan first_sopt: 自 first_opt 起遍历名字段, 首个不以 '-' 开头的段
//       即短选项名 (len==1), 原样返回; 无 → NULL。
const char *first_sopt(unsigned *i)
{
	const char *p;
	unsigned int len = 0;   /* 上游占位: 抑制 GCC 假警告 */

	for (p = first_opt(i, &len); p != NULL; p = next_opt(p, i, &len)) {
		if (p[0] != '-')
			break;
	}
	return p;
}

// === next_sopt === 原 FUN_0005bb14 @ 0x0005bb14
// 来源: reversed/cgminer/40_config/FUN_0005bb14.c | 分类: 40_config
// 语义: 上游 ccan next_sopt: 自当前短选项段继续 (next_opt, len 初值 1), 首个
//       不以 '-' 开头的段返回; 无 → NULL。
const char *next_sopt(const char *p, unsigned *i)
{
	unsigned int len = 1;

	for (p = next_opt(p, i, &len); p != NULL; p = next_opt(p, i, &len)) {
		if (p[0] != '-')
			break;
	}
	return p;
}

// === consume_option === 原 FUN_0005bdb8 @ 0x0005bdb8
// 来源: reversed/cgminer/40_config/FUN_0005bdb8.c | 分类: 40_config
// 语义: 上游 ccan consume_option: 从 argv 中删除第 optnum 个参数 (memmove 把其后
//       *argc-optnum 个指针前移一格, 每指针 4 字节) 并 (*argc)--。
static void consume_option(int *argc, char **argv, unsigned optnum)
{
	memmove(&argv[optnum], &argv[optnum + 1], sizeof(argv[optnum]) * (*argc - optnum));
	(*argc)--;
}

// === parse_one === 原 FUN_0005bdec @ 0x0005bdec
// 来源: reversed/cgminer/40_config/FUN_0005bdec.c | 分类: 40_config
// 语义: 上游 ccan/opt parse_one (cgminer 版, 4 参): 从 argv[1] 起找第一个选项
//       参数并解析执行。POSIXLY_CORRECT 时仅认 argv[1] 且必须以 '-' 开头,
//       否则返回 0; 非该模式扫到首个 '-' 开头参数 (无则返回 0, 裸 "-" 落入短
//       选项路径报 unrecognized)。返回 1=已消费一个选项, 0=无更多选项, -1=出错。
//       "--" 终止符消费后返回 0; "--xxx" 走长选项遍历 (此时 assert *offset==0,
//       原 __assert_fail 文本保留), 支持 --opt=val; "-x"/连写 "-abc" 由 *offset
//       定位逐字母匹配短选项表。OPT_NOARG → cb(u); 其余 → 取参数 (连写余字/
//       下一 argv) → OPT_PROCESSARG 先 opt_set_charp 写回 u → cb_arg。回调返回
//       错误串时经 errlog 上报并 free 后返回 -1。短选项簇内尚有剩余字母时保持
//       *offset 直接返回 1; 否则复位 offset 并消费该 argv (参数取自下一 argv 时
//       一并消费)。
int parse_one(int *argc, char **argv, unsigned *offset, void (*errlog)(const char *fmt, ...))
{
	unsigned int arg;
	unsigned int i;
	unsigned int len;
	const char *o;
	const char *optarg = NULL;
	char *problem;
	const struct opt_table *ot;

	if (getenv("POSIXLY_CORRECT")) {
		/* 不在非选项参数之后找选项: 仅 argv[1] 且须以 '-' 开头 */
		if (argv[1] == NULL || argv[1][0] != '-')
			return 0;
		arg = 1;
	} else {
		for (arg = 1; argv[arg] != NULL; arg++) {
			if (argv[arg][0] == '-')
				break;
		}
		if (argv[arg] == NULL)
			return 0;
	}

	/* 特殊参数: "--" 选项终止符 (argv[arg][1]=='-' 且 [2]=='\0') */
	if (argv[arg][1] == '-' && argv[arg][2] == '\0') {
		consume_option(argc, argv, arg);
		return 0;
	}

	if (argv[arg][1] == '-') {
		/* 长选项: --xxx */
		if (*offset != 0)
			__assert_fail("*offset == 0", "opt/parse.c", 0x3b, "parse_one");

		len = 0;
		for (o = first_lopt(&i, &len); o != NULL; o = next_lopt(o, &i, &len)) {
			if (strncmp(argv[arg] + 2, o, len) != 0)
				continue;
			if (argv[arg][2 + len] == '=')
				optarg = argv[arg] + 2 + len + 1;       /* 形如 --opt=val */
			else if (argv[arg][2 + len] != '\0')
				continue;                               /* 仅前缀匹配, 非完整段 */
			break;
		}
		if (o == NULL) {
			errlog("%s: %.*s: %s", argv[0], strlen(argv[arg]), argv[arg], "unrecognized option");
			return -1;
		}
		/* For error messages, we include the leading '--' */
		o -= 2;
		len += 2;
	} else {
		/* 短选项: -x; offset 允许处理 -abc 连写 */
		for (o = first_sopt(&i); o != NULL; o = next_sopt(o, &i)) {
			if (argv[arg][*offset + 1] != *o)
				continue;
			(*offset)++;
			break;
		}
		if (o == NULL) {
			errlog("%s: %.*s: %s", argv[0], strlen(argv[arg]), argv[arg], "unrecognized option");
			return -1;
		}
		/* For error messages, we include the leading '-' */
		o--;
		len = 2;
	}

	ot = &((const struct opt_table *)DAT_0009d77c)[i];
	if (ot->type == OPT_NOARG) {
		if (optarg != NULL) {
			errlog("%s: %.*s: %s", argv[0], len, o, "doesn't allow an argument");
			return -1;
		}
		problem = ot->cb(ot->u);
	} else {
		if (optarg == NULL) {
			/* 短选项连写时吞掉余下字符作为参数, 如 -afile */
			if (*offset != 0 && argv[arg][*offset + 1] != '\0') {
				optarg = argv[arg] + *offset + 1;
				*offset = 0;
			} else {
				optarg = argv[arg + 1];
			}
		}
		if (optarg == NULL) {
			errlog("%s: %.*s: %s", argv[0], len, o, "requires an argument");
			return -1;
		}
		if (ot->type == OPT_PROCESSARG)
			opt_set_charp(optarg, (char **)ot->u);
		problem = ot->cb_arg(optarg, ot->u);
	}

	if (problem != NULL) {
		errlog("%s: %.*s: %s", argv[0], len, o, problem);
		free(problem);
		return -1;
	}

	/* 短选项簇中已无剩余字母则复位 offset */
	if (*offset != 0) {
		if (argv[arg][*offset + 1] != '\0')
			return 1;               /* 簇内还有字母: 保留 offset 供下一轮 */
		*offset = 0;
	}

	/* 该选项已完全处理完? 消费之 (参数取自下一 argv 时一并消费) */
	if (*offset == 0) {
		consume_option(argc, argv, arg);
		if (optarg != NULL && optarg == argv[arg])
			consume_option(argc, argv, arg);
	}
	return 1;
}

/*
 * ---- 覆盖率表 (batch=cfg_b1, 17/17) ----
 * 函数                         原地址       反编译行数  横幅行  一句话语义
 * set_logfile_openflag         0x0000f9a8  9           157     --version-file 的 cb_arg: 参数串存入 DAT_00076bf0 (version 文件路径), 恒返回 NULL
 * default_save_file            0x00016150  30          169     默认配置路径: default_config 非空直接用, 否则 $HOME/.cgminer/ (mkdir 0777) + cgminer.conf
 * set_options                  0x000267b0  252         198     curses 菜单: [Q]ueue/[S]cantime/[E]xpiry 修改、[W] 写配置 (write_config, 已存在需确认)、[C] 重启确认
 * parse_config                 0x00016dec  111         303     遍历静态选项表分发 JSON 配置到 cb/cb_arg (PROCESSARG 先写回), include 键转 load_json_file
 * bm1740_makeup_set_config_cmd 0x0003ae6c  13          405     bitmain 构帧错误桩: LOG_ERR 报 "input param error: str length = 13" 后恒返回 -1
 * json_array_size              0x00059fec  19          419     jansson 数组长度: NULL/非 JSON_ARRAY → 0, 否则 +0x0C 长度字段
 * opt_set_charp                0x0005b1d4  9           432     参数串写入 *p (静态表 cb_arg 身份与 PROCESSARG 写回共用), 恒返回 NULL
 * next_name                    0x0005b544  43          465     名字段迭代: ' '/'='/NUL 终止, '|' 分隔后跳 1 字符取下段并更新 *len
 * first_opt                    0x0005b5ec  57          485     动态表首个非 OPT_SUBTABLE 项的首名字段 (*i=项号, *len=段长)
 * next_opt                     0x0005b6d4  53          503     动态表名字段续遍历: 先同项下一段, 尽则跳到下一非子表项首段
 * check_opt                    0x0005b7c8  153         526     校验表项 type/desc/名字段合法性 (errx) 并 realloc 追加进动态表, 统计长/短/带参短选项数
 * first_lopt                   0x0005ba08  19          575     首个 '-' 开头名字段 (长选项), 剥 '-' 返回
 * next_lopt                    0x0005ba60  19          594     下一长选项段, 剥 '-' 返回
 * first_sopt                   0x0005bab8  20          611     首个非 '-' 名字段 (短选项), 原样返回
 * next_sopt                    0x0005bb14  20          627     下一短选项段, 原样返回
 * consume_option               0x0005bdb8  11          642     memmove 删除 argv[optnum] 并 (*argc)--
 * parse_one                    0x0005bdec  165         652     命令行单选项解析: 长/短/连写匹配 → cb/cb_arg, "--" 终止, 回调错误经 errlog 上报
 */
