/*
 * rewritten/cgminer/json.c — cgminer JSON-RPC / JSON 配置层 (语义化转写)
 *
 * 来源: reversed/cgminer/22_json_rpc/FUN_00016d98.c ~ FUN_000597e8.c
 * 范围: 6 个函数 — jansson 底层 (json_load_file / jsonp_strtod) +
 *       cgminer 配置加载 (load_json_file) + stratum 授权 (auth_stratum) +
 *       API "notify" 命令 (api_notify / notify_dev_status)
 *
 * 事实还原 (全部以反编译偏移 / 反汇编 / ELF 数据为证):
 *   - 本固件静态链接 jansson 2.x。json_t 头两字段: +0x00 type (json_type 枚举),
 *     +0x04 refcount; refcount == -1 ((size_t)-1) 表示永久对象, decref 跳过。
 *     auth_stratum 中的 json_decref 为编译器内联展开 (判 NULL → 判 -1 → 自减 →
 *     归零则 json_delete), 转写保持内联形态。
 *   - json_type 枚举对照 (jansson 2.x): 0 OBJECT / 1 ARRAY / 2 STRING /
 *     3 INTEGER / 4 REAL / 5 TRUE / 6 FALSE / 7 NULL。
 *     auth_stratum 显式比较 6 (json_is_false) 与 7 (json_is_null)。
 *   - json_error_t 共 252 字节: {int line; int column; int position;
 *     char source[80]; char text[160];}; text 位于 +0x5C, 由 jsonp_error_init
 *     的写偏移与 load_json_file 取 err.text 的栈布局共同证实。
 *   - applog 优先级为本固件 syslog 级别 (applog 体内 syslog(prio|0x80) 佐证):
 *     LOG_ERR = 3, LOG_DEBUG = 7。调用点形如
 *     "DAT_00078e09 && (DAT_0007ad48 || DAT_00078e08 || N < DAT_000766c4)"
 *     的日志守卫为全库统一的内联模式 (N 随级别升高, 值越大越详细);
 *     各 DAT 精确语义待定, 保留原名。
 *   - API 输出走 io_data 增长缓冲 {int siz; char *ptr; char *cur; ...}:
 *     message(io_data, code, p0, p1, isjson) 前置 {"STATUS":[ (JSON 模式) 并
 *     以 print_data 输出条目; codes 表 (DAT_000760fc, 16 字节/项:
 *     {severity, code, 参数类型, 消息串}) 实测 10 = MSG_NODEVS "No devices"(E)、
 *     60 = MSG_NOTIFY "Notify"(S)。
 *   - print_data(io_data, root, isjson, prepend_comma): isjson 决定 JSON("{"…"}")
 *     与文本("|" 分隔) 两种输出; 第 4 参在条目前置 ","。notify_dev_status 的
 *     调用点反汇编 (0x36540~0x36568) 证实第 4 参 = (isjson > 0 ? 1 : 0),
 *     而 Ghidra 反编译仅显示 3 参 — 转写以反汇编为准。
 *   - notify_dev_status 的 notwell 原因表 @0x66a1c 共 9 项 (第 10 项缺失,
 *     与反编译的 "< 9" 边界一致); 9 条字符串已从 ELF 数据段逐一核对。
 *   - api_notify 成功追加 ,"NOTIFY":[ 段后置 io_data+0x0D 字节 = 1
 *     (疑为响应 JSON 模式标志, 供后续响应封装读取; 语义待定)。
 *   - 全局: DAT_00076f24 = 配置文件路径缓存 (主流程 malloc/strdup 写入并打印
 *     "Loaded configuration file %s"), DAT_00076f20 = 配置已加载标志,
 *     DAT_00076f28 = include 递归深度; DAT_0007b0f8 = pool_id 分配计数器
 *     (取值后自增); DAT_0007bb24 / DAT_00075eb4 分别为 mining.suggest_difficulty
 *     与 mining.multi_version 的参数值; DAT_0007b720 = total_devices。
 *
 * 注: 下方 extern 声明均取自 name_map 语义名, 定义分布在其余批次文件;
 *     最终应上移至 rewritten/include/*.h 共享。
 */
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>      /* fopen/fclose/FILE/snprintf/sprintf/fprintf */
#include <stdlib.h>     /* malloc/free/strtod/strerror */
#include <string.h>     /* strlen/strchr/strncpy/strdup */
#include <locale.h>     /* localeconv/struct lconv */
#include <errno.h>      /* errno (原码为 __errno_location() 显式读写, 等价) */
#include <time.h>       /* time_t (本平台 32 位) */

/* ================= jansson 2.x 类型 ================= */

/* json_type 枚举 (与 name_map 中 json_object_get "type==0" 判对象等证据一致) */
typedef enum json_type {
	JSON_OBJECT  = 0,
	JSON_ARRAY   = 1,   /* parse_config 对 "include" 值的合法类型 */
	JSON_STRING  = 2,
	JSON_INTEGER = 3,
	JSON_REAL    = 4,
	JSON_TRUE    = 5,
	JSON_FALSE   = 6,   /* auth_stratum: result == FALSE → 授权失败 */
	JSON_NULL    = 7    /* auth_stratum: error == NULL 类型 → 非错误 */
} json_type;

/* json_t 公共头 (各节点首两字段; refcount == -1 = 永久对象) */
typedef struct json_t {
	int type;       /* +0x00: json_type 枚举值 */
	int refcount;   /* +0x04: 引用计数; -1 表示永久对象, decref 跳过 */
} json_t;

/* 解析错误描述 (252 字节; 偏移由 jsonp_error_init 与 err.text 访问证实) */
typedef struct json_error_t {
	int  line;        /* +0x000 */
	int  column;      /* +0x004 */
	int  position;    /* +0x008 */
	char source[80];  /* +0x00C */
	char text[160];   /* +0x05C */
} json_error_t;

/* ================= 本固件日志级别 (syslog 值) ================= */

#define LOG_ERR    3   /* load_json_file 分配失败等致命错误 */
#define LOG_DEBUG  7   /* auth_stratum 成败均按 debug 级打印 */

/* ================= API 层类型 ================= */

typedef struct api_data api_data_t;   /* api_add_* 链表节点 (定义于 API 批次, 布局未用) */

/* io_data 增长缓冲为不透明指针: {int siz(+0); char *ptr(+4); char *cur(+8); …},
   本文件仅访问 +0x0D 处的 1 字节标志 (见 api_notify 尾部) */

/* ================= 外部函数 (name_map 语义名) ================= */

/* --- jansson 内部 (B3/D 分片) --- */
extern json_t *json_loadf(FILE *stream, size_t flags, json_error_t *error);
extern void    jsonp_error_init(json_error_t *error, const char *source);
extern void    error_set(json_error_t *error, int line, const char *fmt, ...);
extern json_t *json_loads(char *string, size_t flags, json_error_t *error);
extern json_t *json_object_get(json_t *object, const char *key);
extern char   *json_dumps(json_t *value, unsigned int flags);
extern void    json_delete(json_t *value);

/* --- 配置 / stratum / API / 设备 / 日志 (各批次) --- */
extern char   *parse_config(json_t *config, int flag);       /* 40_config: 返回错误串, NULL=无错 */
extern int     stratum_send(int *pool, char *buf, size_t len);
extern char   *recv_line(int *pool);                          /* 读一行 stratum 数据, 无数据返回 NULL */
extern unsigned int parse_notify_equihash(int *pool, char *buf); /* cgminer parse_method: 非 0=方法消息已消化 */
extern void    suspend_stratum(int *pool);
extern void   *get_devices(int index);                        /* 按索引返回设备 (cgpu) */
extern void    applog(uint32_t prio, char *buf, ...);    /* 第 3 参语义待定 (两处调用分别传 1/0) */
extern void    quit(int status);                              /* 包装 __quit, 不返回 */
extern void    message(void *io_data, int msg, int param0, int param1, int isjson);
extern int     io_add(void *io_data, const char *str);        /* 成功返回 1 */
extern int     print_data(void *io_data, api_data_t *root, int isjson, int prepend_comma);
extern api_data_t *api_add_int(api_data_t *root, const char *name, int *data, int copy);
extern api_data_t *api_add_string(api_data_t *root, const char *name, char *data, int copy);
extern api_data_t *api_add_time(api_data_t *root, const char *name, time_t *data, int copy);

/* ================= 外部全局 (语义见文件头说明) ================= */

extern uint8_t DAT_00076f20;   /* 配置已加载标志 */
extern char   *DAT_00076f24;   /* 配置文件路径缓存 */
extern int     DAT_00076f28;   /* include 递归深度计数 */
extern int     DAT_0007b0f8;   /* pool_id 分配计数器 */
extern int     DAT_0007bb24;   /* mining.suggest_difficulty 参数值 */
extern int     DAT_00075eb4;   /* mining.multi_version 参数值 */
extern uint8_t DAT_00078ca9;   /* 全局标志: main 初始化与授权成功时置 1 (语义待定) */
extern uint8_t DAT_00078e09;   /* 日志守卫: 外层总开关 (疑 opt_log_output, 语义待定) */
extern uint8_t DAT_0007ad48;   /* 日志守卫: 次级开关 (语义待定) */
extern uint8_t DAT_00078e08;   /* 日志守卫: 次级开关 (语义待定) */
extern int     DAT_000766c4;   /* 日志守卫: 级别阈值 (值越大越详细) */
extern int     DAT_0007b720;   /* total_devices 设备总数 */

/* device_not_well 原因字符串表 (原数据 @0x00066a1c, 9 项, 第 10 项缺失) */
static const char *notwell_reasons[9] = {
	"Thread failed to init",            /* 序号 0 */
	"Thread got zero hashes",           /* 1 */
	"Thread failed to queue work",      /* 2 */
	"Device idle for 60s",              /* 3 */
	"Device dead - idle for 600s",      /* 4 */
	"Device failed to start",           /* 5 */
	"Device over heated",               /* 6 */
	"Device reached thermal cutoff",    /* 7 */
	"Device comms error",               /* 8 */
};

// === json_load_file === 原 FUN_0005940c @ 0x0005940c
// 来源: reversed/cgminer/22_json_rpc/FUN_0005940c.c | 分类: 22_json_rpc
// 语义: jansson: 从文件读入并解析 JSON。先以文件路径初始化错误结构;
//       路径为空或 fopen 失败时写错误并返回 NULL, 否则转交 json_loadf 后关闭文件。
json_t *json_load_file(const char *path, size_t flags, json_error_t *error)
{
	FILE *stream;
	json_t *result;

	jsonp_error_init(error, path);          /* 错误源 = 文件路径 */
	if (path == NULL) {
		error_set(error, 0, "wrong arguments");
		return NULL;
	}
	stream = fopen(path, "rb");
	if (stream == NULL) {
		error_set(error, 0, "unable to open %s: %s", path, strerror(errno));
		return NULL;
	}
	result = json_loadf(stream, flags, error);
	fclose(stream);
	return result;
}

// === jsonp_strtod === 原 FUN_000597e8 @ 0x000597e8
// 来源: reversed/cgminer/22_json_rpc/FUN_000597e8.c | 分类: 22_json_rpc
// 语义: jansson 数字解析底层: 当 locale 小数点非 '.' 时, 将缓冲中的 '.' 就地替换为
//       locale 小数点后 strtod; 要求端点恰好耗尽整个缓冲 (断言); errno==ERANGE
//       且值非 0 时返回 -1, 否则 *out = 值并返回 0。
int jsonp_strtod(int *strbuffer, double *out)
{
	/* strbuffer_t*: +0x00 = value (char*), +0x04 = length (size_t) — 由断言比较式证实 */
	struct lconv *lc;
	char *str;
	char *dot;
	char *end;
	double value;

	lc = localeconv();
	str = *(char **)strbuffer;
	if (lc->decimal_point[0] != '.') {
		dot = strchr(str, '.');
		if (dot != NULL) {
			*dot = lc->decimal_point[0];   /* 就地适配 locale 小数点 */
			str = *(char **)strbuffer;     /* 原码重新读取 (值不变) */
		}
	}
	errno = 0;
	value = strtod(str, &end);
	if (end != *(char **)strbuffer + strbuffer[1]) {
		/* 端点未耗尽整个缓冲 → jansson 内部断言失败 */
		__assert_fail("end == strbuffer->value + strbuffer->length", "strconv.c", 0x46,
		              "jsonp_strtod");
	}
	if (errno == 0x22 /* ERANGE */ && value != 0.0)
		return -1;
	*out = value;
	return 0;
}

// === load_json_file === 原 FUN_00016d98 @ 0x00016d98
// 来源: reversed/cgminer/22_json_rpc/FUN_00016d98.c | 分类: 22_json_rpc
// 语义: 加载 JSON 配置文件 (含 include 递归): 首次调用记录文件路径, include 深度
//       超过 10 直接返回循环错误串; json_load_file 解析且顶层必须是 JSON 对象,
//       失败则构造 "JSON decode of file …" 错误消息返回 (分配失败时 applog+quit);
//       成功置已加载标志并把对象交给 parse_config, 返回其错误串 (NULL=无错)。
//       注: 原反编译签名为 4 参, 反汇编证实仅 r0 (filename) 被使用, 其余为签名推断噪声。
char *load_json_file(const char *filename)
{
	json_error_t err;
	json_t *config;
	char *msg;
	size_t size;

	if (DAT_00076f24 == NULL)
		DAT_00076f24 = strdup(filename);    /* 记录首个配置文件路径 */
	DAT_00076f28 = DAT_00076f28 + 1;        /* include 深度 +1 */
	if (10 < DAT_00076f28)
		return "Too many levels of JSON includes (limit 10) or a loop";

	config = json_load_file(filename, 0, &err);
	if (config == NULL || config->type != JSON_OBJECT) {
		/* 解析失败或顶层不是对象 → 拼错误消息 */
		size = strlen(filename) + strlen(err.text) + 0x23;
		msg = malloc(size);
		if (msg == NULL) {
			char errmsg[] = "Malloc failure in json error";   /* 28 字节栈串 (原为字面量数组) */
			applog(LOG_ERR, errmsg, 1);
			quit(1);                        /* 不返回 */
		}
		snprintf(msg, size, "JSON decode of file '%s' failed\n %s", filename, err.text);
	}
	else {
		DAT_00076f20 = 1;                   /* 配置已加载 */
		msg = parse_config(config, 1);
	}
	return msg;
}

// === auth_stratum === 原 FUN_0002ea38 @ 0x0002ea38
// 来源: reversed/cgminer/22_json_rpc/FUN_0002ea38.c | 分类: 22_json_rpc
// 语义: stratum 授权: 以 pool_id 发送 mining.authorize (用户/密码取 pool+0xCC/+0xD0),
//       随后循环读行并经 parse_notify_equihash 过滤方法消息 (非 0 = 已消化, 继续读);
//       非方法行视为授权应答: result 缺失/为 false, 或 error 非 null 时判失败
//       (error 对象 json_dumps 后打印) 并 suspend_stratum; 成功置 pool+0x83 授权
//       字节与全局标志 DAT_00078ca9, 并按配置追发 suggest_difficulty / multi_version。
//       返回 1 = 授权成功, 0 = 发送失败/应答失败。应答对象经内联 json_decref 释放。
int auth_stratum(int *pool)
{
	char sendbuf[8196];                 /* 原栈缓冲 0x2004 字节 (三条 JSON 请求共用) */
	char msg[4096];                     /* applog 格式化缓冲 */
	json_error_t err;
	int id;                             /* 多用途临时量: 当前消息 id / 通用返回值 */
	size_t len;
	json_t *val;                        /* 应答 JSON 对象 */
	json_t *res_val;                    /* 应答 "result" (借引用) */
	json_t *err_val;                    /* 应答 "error" (借引用) */
	char *reason;                       /* 失败原因串 (需 free) */
	char *line;
	int ret;

	id = DAT_0007b0f8;
	DAT_0007b0f8 = DAT_0007b0f8 + 1;    /* pool_id 取值后自增 */
	sprintf(sendbuf,
	        "{\"id\": %d, \"method\": \"mining.authorize\", \"params\": [\"%s\", \"%s\"]}",
	        id, (char *)pool[0x33], (char *)pool[0x34]);
	len = strlen(sendbuf);
	if (stratum_send(pool, sendbuf, len) == 0)
		return 0;

	while ((line = recv_line(pool)) != NULL) {
		if (parse_notify_equihash(pool, line) != 0) {
			free(line);                 /* 方法消息 (notify/难度/重连等) 已消化, 继续读 */
			continue;
		}

		/* 非方法行 → 授权应答 */
		val = json_loads(line, 0, &err);
		free(line);
		res_val = json_object_get(val, "result");
		err_val = json_object_get(val, "error");

		if (res_val == NULL || res_val->type == JSON_FALSE /*6: json_is_false*/) {
			if (err_val != NULL)
				goto dump_error;        /* 原标签 0002eb3c */
			reason = malloc(0x11);
			if (reason != NULL)
				strncpy(reason, "(unknown reason)", 0x11);
		}
		else if (err_val == NULL || err_val->type == JSON_NULL /*7: json_is_null*/) {
			/* 授权成功 */
			if (DAT_00078e09 != 0 &&
			    (DAT_0007ad48 != 0 || DAT_00078e08 != 0 || 6 < DAT_000766c4)) {
				snprintf(msg, 0x1000, "Stratum authorisation success for pool %d", *pool);
				applog(LOG_DEBUG, msg, 0);
			}
			*(uint8_t *)((char *)pool + 0x83) = 1;    /* pool->authorised (字节字段) */
			id = DAT_0007b0f8;
			DAT_00078ca9 = 1;
			if (DAT_0007bb24 != 0) {
				DAT_0007b0f8 = DAT_0007b0f8 + 1;
				sprintf(sendbuf,
				        "{\"id\": %d, \"method\": \"mining.suggest_difficulty\", \"params\": [%d]}",
				        id, DAT_0007bb24);
				stratum_send(pool, sendbuf, strlen(sendbuf));
			}
			id = DAT_0007b0f8;
			if (DAT_00075eb4 == 0) {
				ret = 1;
			}
			else {
				DAT_0007b0f8 = DAT_0007b0f8 + 1;
				sprintf(sendbuf,
				        "{\"id\": %d, \"method\": \"mining.multi_version\", \"params\": [%d]}",
				        id, DAT_00075eb4);
				ret = 1;
				stratum_send(pool, sendbuf, strlen(sendbuf));
			}
			goto out;                   /* 原标签 0002ebb4 */
		}
		else {
		dump_error:                     /* 原标签 0002eb3c: 序列化 error 对象作为原因 */
			reason = json_dumps(err_val, 3);   /* flags=3 (与 parse_notify_equihash 等处一致) */
		}

		/* 授权失败 */
		if (DAT_00078e09 != 0 &&
		    (DAT_0007ad48 != 0 || DAT_00078e08 != 0 || 6 < DAT_000766c4)) {
			snprintf(msg, 0x1000, "pool %d JSON stratum auth failed: %s", *pool, reason);
			applog(LOG_DEBUG, msg, 0);
		}
		ret = 0;
		free(reason);
		suspend_stratum(pool);

	out:                                /* 原标签 0002ebb4: 内联 json_decref(val) 后返回 */
		if (val == NULL)
			return ret;
		if (val->refcount == -1)        /* 永久对象不回收 */
			return ret;
		val->refcount--;
		if (val->refcount == 0)
			json_delete(val);
		return ret;
	}
	return 0;                           /* 连接断开 / 发送失败 */
}

// === notify_dev_status === 原 FUN_000363fc @ 0x000363fc
// 来源: reversed/cgminer/22_json_rpc/FUN_000363fc.c | 分类: 22_json_rpc
// 语义: 为 API "NOTIFY" 段构造单个设备条目: 按设备"最后异常时间"(dev+0xF4) 为 0
//       判健康 ("None"), 否则以异常序号 (dev+0xF8, <9) 查 notwell_reasons 表选原因;
//       依次追加 NOTIFY(索引)/Name/ID/Last Well/Last Not Well/Reason Not Well 及
//       10 项异常计数, 最后 print_data 输出 (JSON 模式下前置逗号)。
//       第 4/5 参在函数体内未使用 (按调用点路由传参)。
void notify_dev_status(void *io_data, int id, void *dev, int isjson, uint8_t arg5)
{
	api_data_t *root;
	char *reason;
	int *devlink;                       /* dev+0x04 的指针字段 (所指结构 +0x08 = 设备名) */
	uint32_t seq;

	if (*(uint32_t *)((char *)dev + 0xf4) == 0) {         /* device_last_not_well == 0 */
		reason = "None";
	}
	else {
		seq = *(uint32_t *)((char *)dev + 0xf8);          /* device_not_well_seq */
		if (seq < 9)
			reason = notwell_reasons[seq];
		else
			reason = "Unknown reason - code bug";
	}

	root = api_add_int(NULL, "NOTIFY", &id, 0);
	devlink = *(int **)((char *)dev + 4);
	root = api_add_string(root, "Name", *(char **)((char *)devlink + 8), 0);
	root = api_add_int(root, "ID", (int *)((char *)dev + 8), 0);
	root = api_add_time(root, "Last Well", (time_t *)((char *)dev + 0xf0), 0);      /* device_last_well */
	root = api_add_time(root, "Last Not Well", (time_t *)((char *)dev + 0xf4), 0);  /* device_last_not_well */
	root = api_add_string(root, "Reason Not Well", reason, 0);
	root = api_add_int(root, "*Thread Fail Init", (int *)((char *)dev + 0xfc), 0);      /* thread_fail_init_count */
	root = api_add_int(root, "*Thread Zero Hash", (int *)((char *)dev + 0x100), 0);     /* thread_zero_hash_count */
	root = api_add_int(root, "*Thread Fail Queue", (int *)((char *)dev + 0x104), 0);    /* thread_queue_fail_count */
	root = api_add_int(root, "*Dev Sick Idle 60s", (int *)((char *)dev + 0x108), 0);    /* dev_sick_idle_60s_count */
	root = api_add_int(root, "*Dev Dead Idle 600s", (int *)((char *)dev + 0x10c), 0);   /* dev_dead_idle_600s_count */
	root = api_add_int(root, "*Dev Nostart", (int *)((char *)dev + 0x110), 0);          /* dev_nostart_count */
	root = api_add_int(root, "*Dev Over Heat", (int *)((char *)dev + 0x114), 0);        /* dev_over_heat_count */
	root = api_add_int(root, "*Dev Thermal Cutoff", (int *)((char *)dev + 0x118), 0);   /* dev_thermal_cutoff_count */
	root = api_add_int(root, "*Dev Comms Error", (int *)((char *)dev + 0x11c), 0);      /* dev_comms_error_count */
	root = api_add_int(root, "*Dev Throttle", (int *)((char *)dev + 0x120), 0);         /* dev_throttle_count */

	/* 原反编译仅显 3 参; 反汇编 0x36540~0x36568 证实第 4 参 = (isjson > 0 ? 1 : 0) */
	print_data(io_data, root, isjson, isjson > 0 ? 1 : 0);
}

// === api_notify === 原 FUN_000365d0 @ 0x000365d0
// 来源: reversed/cgminer/22_json_rpc/FUN_000365d0.c | 分类: 22_json_rpc
// 语义: API 命令 "notify" 处理: 无设备 (total_devices == 0) 时回 MSG_NODEVS(10)
//       "No devices"; 否则回 MSG_NOTIFY(0x3C=60) "Notify", isjson 模式下追加
//       ,"NOTIFY":[ 段头, 并逐一遍历设备调用 notify_dev_status; 段头追加成功后
//       置 io_data+0x0D 字节 = 1 (疑为响应 JSON 模式标志, 语义待定)。
//       sock / issocket / arg5 三参在本函数体未被使用 (按调用点原样路由)。
void api_notify(void *io_data, int sock, int issocket, int isjson, uint8_t arg5)
{
	int io_new;
	int i;
	void *dev;

	if (DAT_0007b720 == 0) {            /* total_devices == 0 */
		message(io_data, 10 /* MSG_NODEVS "No devices" (codes 表实证) */, 0, 0, isjson);
		return;
	}
	message(io_data, 0x3c /* 60 = MSG_NOTIFY "Notify" (codes 表实证) */, 0, 0, isjson);
	if (isjson == 0)
		io_new = 0;
	else
		io_new = io_add(io_data, ",\"NOTIFY\":[");

	if (0 < DAT_0007b720) {
		i = 0;
		do {
			dev = get_devices(i);
			notify_dev_status(io_data, i, dev, isjson, arg5);
			i++;
		} while (i < DAT_0007b720);
	}

	if (io_new != 0)
		*(uint8_t *)((char *)io_data + 0xd) = 1;   /* 已追加 JSON 段标志 (反汇编 strb 证实) */
}

/*
 * ================= 覆盖率表 (6/6) =================
 * 原 FUN 地址    新函数名                  反编译行数
 * ---------------------------------------------
 * FUN_00016d98  load_json_file             77
 * FUN_0002ea38  auth_stratum              104
 * FUN_000363fc  notify_dev_status          39
 * FUN_000365d0  api_notify                 36
 * FUN_0005940c  json_load_file             31
 * FUN_000597e8  jsonp_strtod               38
 *
 * 外部依赖: json_loadf/jsonp_error_init/error_set/json_loads/json_object_get/
 *           json_dumps/json_delete (jansson 内部, B3/D 分片);
 *           parse_config (40_config), stratum_send/recv_line/parse_notify_equihash/
 *           suspend_stratum (21_pool_net/20_job_mining), message/io_add/print_data/
 *           api_add_* (30_api_status), get_devices (20_job_mining),
 *           applog/quit (41_logging/主流程)。
 */
