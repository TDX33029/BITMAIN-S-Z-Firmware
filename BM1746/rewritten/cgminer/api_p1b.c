/* ============================================================================
 * cgminer/api_p1b.c — cgminer API 层转写 第 1b 段 (batch api_b1b, 分类 30_api_status, 共 4 函数)
 *
 * 内容:
 *   1) checkcommand (FUN_00033c34): CHECK 命令响应构造 (Exists/Access Y/N + print_data)
 *   2) devdetails   (FUN_00033dcc): DEVDETAILS 命令响应构造 (驱动/内核/型号/设备路径)
 *   3) mcast_init   (FUN_0003669c): API 多播线程创建 (失败 quit)
 *   4) api          (FUN_00036764): API 主线程 — ipaccess/组解析, bind 4028 重试,
 *                                    cmds 命令表分发+权限检查, mcast_init, 退出路径
 *
 * 转写约定 (rewritten/CONVENTIONS.md):
 *   - 命名事实源 rewritten/name_map/cgminer.tsv (阶段 1 冻结)。
 *   - API 响应字段名/命令代码表等字符串字面量原样保留 (协议证据链)。
 *   - 严格语义等价; 结构布局不可知处保留指针算术并注释字节偏移。
 * ============================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <strings.h>     /* strcasecmp */
#include <ctype.h>       /* toupper/isalpha (原码经 __ctype_toupper_loc/__ctype_b_loc 表实现, 等价) */
#include <time.h>
#include <errno.h>       /* 原码为 __errno_location() 显式读写, 等价 */
#include <unistd.h>      /* close */
#include <pthread.h>
#include <sys/socket.h>  /* socket/accept/recv/setsockopt: SOCK_STREAM=1, SOL_SOCKET=1, SO_REUSEADDR=2 */
#include <netdb.h>       /* getaddrinfo/addrinfo: AI_PASSIVE=1 */
#include <arpa/inet.h>   /* inet_pton: AF_INET6=10 */

/* ================= 本层结构布局 (均由构造写序/ELF 数据核实) ================= */

/* API 响应输出缓冲 — 与 api_p2.c 的 io_data_t 同布局 (io_add/tail_join 用法证实) */
typedef struct io_data {
	uint32_t siz;    /* +0x00: 已分配容量 (初值 0x10000 = SOCKBUFALLOCSIZ) */
	char    *ptr;    /* +0x04: 缓冲基址 */
	char    *cur;    /* +0x08: 当前写位置 */
	uint8_t  sock;   /* +0x0C: api() 构造时置 1 (p2 段未读写; 语义待定) */
	uint8_t  close;  /* +0x0D: 已输出 JSON 段待闭合标志 (tail_join 消费后清零) */
} io_data_t;

/* io_list 环形双向链表节点 (DAT_00078cd0 为表头, 尾插; p2 亦见 {子结构+0, next+8}) */
typedef struct io_list_node {
	io_data_t           *io;    /* +0x00 */
	struct io_list_node *prev;  /* +0x04 */
	struct io_list_node *next;  /* +0x08 */
} io_list_node_t;

/* ipaccess 数组项 (0x24 字节, DAT_00078cb4) — 布局由解析写入序与
 * check_connect 的 16B 掩码匹配证实 */
typedef struct {
	uint8_t addr[16];  /* +0x00: 匹配地址 (IPv4 → ::ffff: 映射; "0/0" → 全零通配) */
	uint8_t mask[16];  /* +0x10: 掩码 (高 bits 位 / 无前缀全 1 / "0/0" 全 0) */
	uint8_t group;     /* +0x20: 该网段映射的组字母 (缺省 'R') */
	uint8_t pad[3];
} api_ipaccess_t;

/* cmds[] 命令表 (DAT_0007652c, 每项 12B, ELF 数据核实 33 项 + name==NULL 结尾) */
typedef struct {
	const char *name;    /* +0x00 */
	void (*fn)(void *io_data, int sock, char *params, int isjson, char group); /* +0x04 */
	uint8_t iswpriv;     /* +0x08: 写特权命令 (1 = 不入只读组 'R'/noprivgroup) */
	uint8_t multi_ok;    /* +0x09: 多命令 '+' 串允许标志 (0 → 直接拒绝, 代码 45);
	                          表数据: 只读命令=1, 写命令与 check=0, lockstats=1 (语义待定) */
} api_cmd_t;

/* api_add_* 链表节点 (布局见 api_p2.c / api_p1a.c) */
typedef struct api_data api_data_t;

/* jansson 2.x json_t 头 (与 api_p2.c / json.c 一致) */
typedef struct json_t {
	int type;       /* +0x00: json_type 枚举 (0=OBJECT 2=STRING 3=INTEGER 4=REAL) */
	int refcount;   /* +0x04: -1 = 永久对象 */
} json_t;

/* jansson 解析错误描述 (252 字节, 与 api_p2.c / json.c 布局一致) */
typedef struct json_error_t {
	int  line;        /* +0x000 */
	int  column;      /* +0x004 */
	int  position;    /* +0x008 */
	char source[80];  /* +0x00C */
	char text[160];   /* +0x05C */
} json_error_t;

/* ================= message() 代码 (ELF codes 表 DAT_000760fc 核实) ================= */

#define MSG_NODEVS      10   /* "No devices" */
#define MSG_INVALID_CMD 14   /* "Invalid command" */
#define MSG_BADJSON     23   /* "Invalid JSON" */
#define MSG_NOJSONFIELD 24   /* "Missing JSON '%s'" */
#define MSG_ACCDENY     45   /* "Access denied to '%s' command" */
#define MSG_DEVDETAILS  69   /* "Device Details" (0x45) */
#define MSG_MISSCHECK   71   /* "Missing check cmd" (0x47) */
#define MSG_CHECKCMD    72   /* "Check command" (0x48) */

/* ================= 反编译统一日志门控 (同 log.c 头注释) =================
 * A=DAT_0007ad48 (非 0 → syslog 输出), B=DAT_00078e08 (verbose 联动),
 * C=DAT_000766c4 (int 日志级别), E=DAT_00078e09 (opt_debug)。
 * 门控: (A || B || C > prio-1); 7 级(调试)日志另需 E。 */

#define APPLOG_GATE(prio) \
	(DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || DAT_000766c4 > ((prio) - 1))
#define APPLOG_GATE_DBG(prio) \
	(DAT_00078e09 != '\0' && APPLOG_GATE(prio))

/* ================= 外部全局 (DAT_ 原名保留, 注释为已核实语义) ================= */

extern api_cmd_t       DAT_0007652c[];   /* cmds[] 命令表首项 (0x7652C, ELF 核实) */
extern char           *DAT_0007bd24[];   /* 组访问串数组, 索引 = 组字母-'A' ('R'=只读组) */
extern int             DAT_0007b720;     /* total_devices 设备总数 */
extern uint8_t         DAT_0007ad48;     /* applog 门控 A: 非 0 → applog 走 syslog(prio|0x80) */
extern uint8_t         DAT_00078e08;     /* applog 门控 B = opt_verbose (curses 'v' 联动) */
extern uint8_t         DAT_00078e09;     /* opt_debug (门控 E) */
extern int             DAT_000766c4;     /* applog 门控 C: int 日志级别 */
extern int             DAT_00075ed0;     /* opt_api_port (ELF 初值 0xfbc = 4028; 原码读入后截 short) */
extern int             DAT_00075dcc;     /* API 延迟启动秒数 (ELF 初值 5, ×1000 → cgsleep_ms) */
extern char           *DAT_00075ecc;     /* API 监听地址 (ELF 初值 "0.0.0.0") */
extern void          (*DAT_00075ed8)(void); /* 空钩子函数指针 (初值 noop_hook FUN_00028ae0) */
extern char            DAT_00066a40[];   /* "api" — 本模块函数名串 (日志/分配器包装用) */
extern char           *DAT_00078c98;     /* opt_api_allow: ipaccess 配置串 (NULL=未配置) */
extern int             DAT_00078cb0;     /* ipaccess 有效项数 */
extern void           *DAT_00078cb4;     /* ipaccess 数组基址 (calloc(n,0x24)) */
extern pthread_mutex_t DAT_00078cb8;     /* quit_restart_lock */
extern void           *DAT_00078cd0;     /* io_data 环形链表头 (NULL=空; 本文件经 io_list_node_t* 视图访问) */
extern void           *DAT_00078cd4;     /* StrBufs 分配池句柄 (_k_new_list 返回) */
extern time_t          DAT_00078cd8;     /* 最后一次命令接收时刻 */
extern void           *DAT_00078cdc;     /* api 线程 userdata 存档 */
extern uint8_t         DAT_00078cac;     /* BYE 标志 (tidyup 亦置 1) */
extern uint8_t         DAT_00078cad;     /* RESTART 标志 */
extern uint8_t         DAT_00078cae;     /* QUIT 标志 */
extern uint8_t         DAT_0007b7d1;     /* API 启用标志 (0 → 线程直接退出) */
extern char           *DAT_0007b944;     /* ipgroups 配置串 "组字母:cmd:cmd,..." */
extern char            DAT_0007b7c4;     /* 非 0 = UNRESTRICTED 读访问模式标志 */
extern uint8_t         DAT_0007b624;     /* 多播(mcast)启用标志 */
extern char            DAT_000670e8[];   /* "ID" (ELF rodata 核实) */

/* ================= 外部函数 (name_map 语义名; 签名与定义文件一致) ================= */

/* 30_api_status 同层 (定义于 api_p1a.c / api_p2.c) */
extern void        message(void *io_data, int code, int param0, int param1, int isjson);      /* FUN_0003305c */
extern int         io_add(io_data_t *io, const char *str);                                    /* FUN_000307c8 */
extern api_data_t *api_add_int(api_data_t *root, char *name, int *data, int copy);            /* FUN_00032ed0 */
extern api_data_t *api_add_string(api_data_t *root, char *name, char *data, int copy);        /* FUN_00032e1c */
extern api_data_t *api_add_const(api_data_t *root, char *name, const char *data, int copy);   /* FUN_00032e40 */
extern int         print_data(void *io_data, api_data_t *root, int isjson, int prepend_comma); /* FUN_00032534 */
extern void        head_join(io_data_t *io, char *cmd, int isjson, char *first);              /* FUN_0003085c */
extern void        tail_join(io_data_t *io, int isjson);                                      /* FUN_00031f64 */
extern void        send_result(void *io_data, int sock, int isjson);                          /* FUN_00030b88 */
extern bool        check_connect(struct sockaddr *saddr, char **ipaddr, char *group);         /* FUN_000309e8 */

/* 其它批次 */
extern void *get_devices(int index);                    /* FUN_0001b83c: 按索引返回设备 (cgpu) */
extern int   thr_info_create(void *thr, const pthread_attr_t *attr, void *(*fn)(void *), void *arg); /* FUN_0002f174 */
extern void *mcast_thread(void *arg);                   /* FUN_00031ca0 */
extern void *quit_thread(void *arg);                    /* FUN_00032340: "API: killing cgminer" (线程体约定签名) */
extern void *api_restart_thread(void *arg);             /* FUN_00032248: "API: restarting cgminer" (同上) */
extern void  quit_restart_unlock(pthread_mutex_t *lock, const char *fn, int line); /* FUN_0003220c */
extern void  _mutex_lock(const char *fn, int line);     /* FUN_0000b774: "WTF MUTEX ERROR ON LOCK!..." */
extern void  _mutex_unlock(const char *fn, int line);   /* FUN_0000b84c */
extern void  tidyup(int *soc);                          /* FUN_0003243c: 加锁置 BYE + shutdown/close fd + 释放 ipaccess/io_list */
extern unsigned int cgsleep_ms(unsigned int ms);        /* FUN_0002a92c (r0 直通) */
extern void *_k_new_list(const char *name, int size, int a3, int a4, int a5,
                         const char *file, const char *fn, int line); /* FUN_00038f3c */
extern void  applog(uint32_t prio, char *buf, ...); /* FUN_00038730: 第 3 参非 0 → 强制 curses 刷新 */
extern void  quit(int status);                          /* FUN_00016ca8: 不返回 */

/* jansson (静态链接) */
extern void  *json_loadb(const char *buf, size_t len, int flags, void *error); /* FUN_000592c4 */
extern void  *json_object_get(void *json, const char *key);                    /* FUN_00059e20 */
extern char  *json_string_value(void *json);   /* 原调用点无参 (r0 承接 json_object_get 返回) */ /* FUN_0005a178 */
extern int    json_integer_value(void *json);   /* 原型 int64, 调用点仅取 r0 作 int */ /* FUN_0005a248 */
extern double json_real_value(void *json);                                     /* FUN_0005a2f8 */
extern void   json_delete(void *json);                                         /* FUN_0005a3e4 */

/* glibc 扩展 (原二进制调用) */
extern char  *stpcpy(char *dest, const char *src);

// === checkcommand === 原 FUN_00033c34 @ 0x00033c34
// 来源: reversed/cgminer/30_api_status/FUN_00033c34.c | 分类: 30_api_status
// 语义: API "check" 命令 — 在 cmds 命令表中查 cmd 是否存在; 存在时再查本连接
//       所在组 (group, 单字母) 的访问串是否含 "|cmd|" ('W' 组直通全权)。
//       输出 Exists/Access = "Y"/"N" (api_add_const, 不拷贝); cmd 为空 → message(71)。
//       第 2 参 sock 未用 (统一命令分发表签名)。

void checkcommand(void *io_data, int sock, char *cmd, uint32_t isjson, uint8_t group)
{
	char namebracket[100];    /* "|<cmd>|" 查找串 */
	bool exists, access;
	const char *str;
	uint32_t preflag;
	api_data_t *root;
	int i;

	(void)sock;

	if (cmd == NULL || *cmd == '\0') {
		message(io_data, MSG_MISSCHECK, 0, 0, (int)isjson);
		return;
	}

	exists = false;
	access = false;
	if (DAT_0007652c[0].name != NULL) {
		for (i = 0; DAT_0007652c[i].name != NULL; i++) {
			if (strcmp(DAT_0007652c[i].name, cmd) == 0) {
				sprintf(namebracket, "|%s|", cmd);
				if (toupper(group) == 'W' ||
				    strstr(DAT_0007bd24[toupper(group) - 'A'], namebracket) != NULL) {
					exists = true;
					access = true;
				} else {
					exists = true;
				}
				break;
			}
		}
	}

	message(io_data, MSG_CHECKCMD, 0, 0, (int)isjson);
	str = ",\"CHECK\":[";
	if (isjson == 0)
		str = "CHECK,";
	preflag = (uint32_t)io_add(io_data, str);
	root = api_add_const(NULL, "Exists", exists ? "Y" : "N", 0);   /* "Y"@0x670ac / "N"@0x670b0 (ELF 核实) */
	root = api_add_const(root, "Access", access ? "Y" : "N", 0);
	print_data(io_data, root, (int)isjson, 0);
	if ((preflag & isjson) != 0)
		((io_data_t *)io_data)->close = 1;   /* io_data+0xD: JSON 闭合标志 (tail_join 读取) */
}

// === devdetails === 原 FUN_00033dcc @ 0x00033dcc
// 来源: reversed/cgminer/30_api_status/FUN_00033dcc.c | 分类: 30_api_status
// 语义: API "devdetails" 命令 — total_devices(DAT_0007b720)==0 → message(10 "No devices");
//       否则 message(69 "Device Details") 后逐设备输出 DEVDETAILS(索引)/Name/ID/
//       Driver/Kernel/Model/Device Path; Kernel/Model/Device Path 为 NULL 时输出
//       空串并经 api_add_const (不拷贝)。
//       cgpu 布局未还原, 保留偏移: +4=drv 指针 (drv+8=Name/dname, drv+4=Driver/name),
//       +8=ID(dev_id), +0xa8=Kernel, +0xc=Model, +0x10=Device Path。
//       第 2/3 参 sock/params 与第 5 参 group 未用 (反编译仅示 4 参, 分发按 5 参调用)。

void devdetails(void *io_data, int sock, char *params, int isjson, uint8_t group)
{
	char *cgpu;
	char *s;
	api_data_t *root;
	int prepend;
	int i;

	(void)sock;
	(void)params;
	(void)group;

	if (DAT_0007b720 == 0) {
		message(io_data, MSG_NODEVS, 0, 0, isjson);
		return;
	}
	message(io_data, MSG_DEVDETAILS, 0, 0, isjson);
	prepend = 0;
	if (isjson != 0)
		prepend = io_add(io_data, ",\"DEVDETAILS\":[");

	root = NULL;
	i = 0;
	while (i < DAT_0007b720) {
		cgpu = (char *)get_devices(i);
		root = api_add_int(root, "DEVDETAILS", &i, 0);
		root = api_add_string(root, "Name", *(char **)(*(char **)(cgpu + 4) + 8), 0);
		root = api_add_int(root, DAT_000670e8, (int *)(cgpu + 8), 0);   /* "ID" */
		root = api_add_string(root, "Driver", *(char **)(*(char **)(cgpu + 4) + 4), 0);
		s = *(char **)(cgpu + 0xa8);
		if (s == NULL)
			s = "";
		root = api_add_const(root, "Kernel", s, 0);
		s = *(char **)(cgpu + 0xc);
		if (s == NULL)
			s = "";
		root = api_add_const(root, "Model", s, 0);
		s = *(char **)(cgpu + 0x10);
		if (s == NULL)
			s = "";
		root = api_add_const(root, "Device Path", s, 0);
		/* 第 4 参: JSON 模式且非首设备时条目前置 ","; print_data 返回值
		 * (int, 实测恒 0) 作为下一设备链根 — 原码 r0 回填 */
		root = (api_data_t *)(intptr_t)print_data(io_data, root, isjson,
		                                          (isjson != 0 && i >= 1) ? 1 : 0);
		i++;
	}
	if (prepend != 0)
		((io_data_t *)io_data)->close = 1;
}

// === mcast_init === 原 FUN_0003669c @ 0x0003669c
// 来源: reversed/cgminer/30_api_status/FUN_0003669c.c | 分类: 30_api_status
// 语义: 创建 API 多播(mcast)线程 — calloc(1,0x40) 线程参数块, thr_info_create
//       (mcast_thread, 以参数块自身作为线程参数); calloc 或 thr_info_create
//       任一失败 → applog(LOG_ERR, 消息, 1) + quit(1)。
//       (原码失败消息为栈上逐字节拼装, 转写还原为等价字面量。)

void mcast_init(void)
{
	void *thr;

	thr = calloc(1, 0x40);
	if (thr == NULL) {
		applog(3, "Failed to calloc mcast thr", 1);
		quit(1);
	}
	if (thr_info_create(thr, NULL, mcast_thread, thr) != 0) {
		applog(3, "API mcast thread create failed", 1);
		quit(1);
	}
}

// === api === 原 FUN_00036764 @ 0x00036764
// 来源: reversed/cgminer/30_api_status/FUN_00036764.c | 分类: 30_api_status
// 语义: API 主线程 — 1) 解析 ipgroups ("组字母:cmd:cmd,..." → DAT_0007bd24[字母-'A']
//       访问串 "|cmd|...|", '*' 补全全部只读命令) 并构造只读组 'R' (noprivgroup);
//       2) 解析 ipaccess ("[组:]IPv6/位长,..." → 0x24B 项 addr/mask/group 数组,
//       IPv4 存 ::ffff: 映射, "0/0" = 全零通配, 无有效项 → 线程退出);
//       3) getaddrinfo(DAT_00075ecc:4028) + SO_REUSEADDR + bind (失败 30s 重试,
//       >61s 放弃) + listen(100); 4) mcast_init + StrBufs 分配池;
//       5) accept 循环: check_connect 鉴权 → recv → 文本 "cmd[|参数]" 或 JSON
//       {"command","parameter"} ('+' 连多命令, 已处理集去重) → cmds 表分发 +
//       组权限检查 ('W' 直通, 其余查组访问串) → send_result;
//       6) 退出: 按 QUIT/RESTART/BYE 标志收尾, QUIT/RESTART 各创建对应线程并
//       pthread_detach, 最后尾调 noop_hook (DAT_00075ed8)。
//       pthread_cleanup_push(tidyup, sock) 覆盖组解析至 accept 循环 (线程取消时
//       关 fd/释放 ipaccess/io_list); 注意 accept 失败/标志退出之外的提前 return
//       (解析错误等) 不弹栈 — 与原二进制一致。原码在弹栈后把 cleanup 栈槽复用为
//       quit/restart 线程的 thr_info — 转写以独立栈缓冲呈现 (生命周期不重叠)。

void api(void *userdata)
{
	int *sock;            /* 监听 fd 容器 (tidyup 关闭/置 -1) */
	io_data_t *io;        /* 响应 io_data */
	io_list_node_t *node;
	io_list_node_t *head;
	char *gwork;          /* ipgroups 解析工作副本 (noprivgroup 存表后释放) */
	bool multi;           /* 原 bVar33: 本连接命令为 '+' 多命令 (跨 recv 保持 — 原样) */
	bool isjson;          /* 原 bVar34 */
	bool loopflag;        /* 原 bVar28 */
	char ismulti;         /* 原 bStack_535d: 传给 head_join 的多命令/first 标志 (字节) */
	char *donebuf;        /* 原 local_5394: 多命令已完成集 "|cmd|cmd|..." */
	char *cmd, *params, *next, *p;
	json_t *jroot, *jnode;
	char *conn_ip;        /* 原 pcStack_535c: 对端 IP 串 (check_connect 填写) */
	char conn_group;      /* 原 bStack_535e: 对端组字母 (check_connect 填写) */
	char recvbuf[0x2000]; /* 原 local_4028: 组解析错误消息 + recv 命令缓冲 */
	char gbuf[0x1000];    /* 原 local_2028: 组访问串构造 / 错误消息 / 参数临时 */
	char msgbuf[0x1000];  /* 原 local_5028: 运行日志 / "|cmd|" 查找串 */
	char portstr[12];     /* 原 acStack_534c */
	char tokbuf[100];     /* 原 acStack_5320: 分发用 "|cmd|" 查找串 */
	char cliaddr[128];    /* 原 asStack_52bc: accept 对端地址 (0x80) */
	json_error_t json_err; /* 原 auStack_523c: json_error_t (252B) */
	char thrbuf[0x40];    /* quit/restart 线程 thr_info (原复用 cleanup 栈槽) */
	socklen_t socklen;    /* 原 sStack_5358 */
	int conn, deny;
	int n, idx;
	int port, ret;
	time_t t0, t1;
	char *errstr;

	/* ---- 入口: API 启用检查 (端口读于函数入口, 原码截 short) ---- */
	port = (int)(short)DAT_00075ed0;
	sock = malloc(4);
	*sock = -1;
	if (DAT_0007b7d1 == '\0') {
		if (APPLOG_GATE_DBG(7)) {
			snprintf(gbuf, 0x1000, "API not running%s", " - API will not be available");
			applog(7, gbuf, 0);
		}
		free(sock);
		return;
	}

	/* ---- 响应 io_data 构造 + 挂入 io_list (环形双向链表, 尾插) ---- */
	io = malloc(0x10);
	node = malloc(0xc);
	io->ptr = malloc(0x10000);
	io->cur = io->ptr;
	((uint8_t *)io)[0xc] = 1;      /* +0x0C 构造置 1 (p2 名 sock, 语义待定) */
	io->siz = 0x10000;
	io->ptr[0] = '\0';
	io->close = 0;
	head = (io_list_node_t *)DAT_00078cd0;
	node->io = io;
	if (head == NULL) {
		node->prev = node;
		node->next = node;
		DAT_00078cd0 = node;
	} else {
		node->next = head;
		node->prev = head->prev;
		head->prev = node;
		node->prev->next = node;
	}

	/* ---- quit_restart_lock 初始化 ---- */
	ret = pthread_mutex_init(&DAT_00078cb8, NULL);
	if (ret != 0) {
		snprintf(gbuf, 0x1000, "Failed to pthread_mutex_init errno=%d in %s %s():%d",
		         errno, "api.c", "api", 0x14b1);
		applog(3, gbuf, 1);
		quit(1);
	}

	/* ---- pthread_cleanup_push(tidyup, sock) — glibc 展开为 __sigsetjmp +
	 * __pthread_register_cancel; 线程取消路径调用 tidyup(sock) 后继续展开
	 * (原码反编译显示为 sigsetjmp else 分支)。 ---- */
	pthread_cleanup_push((void (*)(void *))tidyup, sock);

	DAT_00078cdc = userdata;   /* 线程参数存档 */

	/* ---- ipgroups 解析 (DAT_0007b944): "组字母:cmd[:cmd...][,组...]" ----
	 * 每组构造访问串 "|cmd1|cmd2|...|" 存入 DAT_0007bd24[字母-'A']。 */
	{
		const char *src = DAT_0007b944;
		size_t sz;

		if (src == NULL) {
			sz = 1;
			src = "";
		} else {
			sz = strlen(src) + 1;
		}
		gwork = malloc(sz);
		if (gwork == NULL) {
			snprintf(gbuf, 0x1000, "Failed to malloc ipgroups buffer");
			applog(3, gbuf, 1);
			quit(1);
		}
		memcpy(gwork, src, sz);
		if (gwork[0] != '\0') {
			char *gp = gwork;
			char *gnext;
			char g;
			bool star;
			char *w;

			do {
				/* 以 ',' 分隔各组: 截断当前组, gnext 指向下一组首 */
				p = strchr(gp, ',');
				gnext = NULL;
				if (p != NULL) {
					gnext = p + 1;
					*p = '\0';
				}
				/* 组名必须为 "单字母:" */
				if (gp[1] != ':') {
					char *colon = strchr(gp, ':');

					if (colon != NULL)
						*colon = '\0';
					snprintf(recvbuf, 0x1000, "API invalid group name '%s'", gp);
					applog(3, recvbuf, 1);
					quit(1);
				}
				g = (char)toupper((unsigned char)gp[0]);
				if (g < 'A' || g > 'Z') {
					snprintf(recvbuf, 0x1000, "API invalid group name '%c'", gp[0]);
					applog(3, recvbuf, 1);
					quit(1);
				}
				if (g == 'W') {            /* 'W' 为内建全权组, 不可定义 */
					snprintf(recvbuf, 0x1000, "API group name can't be '%c'", g);
					applog(3, recvbuf, 1);
					quit(1);
				}
				if (g == 'R') {            /* 'R' 为内建只读组, 不可定义 */
					snprintf(recvbuf, 0x1000, "API group name can't be '%c'", g);
					applog(3, recvbuf, 1);
					quit(1);
				}
				if (DAT_0007bd24[g - 'A'] != NULL) {
					snprintf(recvbuf, 0x1000, "API duplicate group name '%c'", gp[0]);
					applog(3, recvbuf, 1);
					quit(1);
				}

				/* 构造本组访问串 "|cmd1|cmd2|...|" 于 gbuf */
				star = false;
				w = gbuf + 1;
				gbuf[0] = '|';
				gbuf[1] = '\0';
				if (gp[2] != '\0') {
					char *tp = gp + 2;

					for (;;) {
						char *colon2 = strchr(tp, ':');
						char *after = NULL;

						if (colon2 != NULL) {
							after = colon2 + 1;
							*colon2 = '\0';
						}
						if (tp[0] == '*' && tp[1] == '\0') {
							star = true;   /* '*' = 补全全部只读命令 */
						} else {
							bool hit = false;

							for (idx = 0; DAT_0007652c[idx].name != NULL; idx++) {
								if (strcasecmp(tp, DAT_0007652c[idx].name) == 0) {
									sprintf(msgbuf, "|%s|", DAT_0007652c[idx].name);
									if (strstr(gbuf, msgbuf) == NULL) {   /* 去重后追加 */
										size_t l;

										strcpy(w, DAT_0007652c[idx].name);
										l = strlen(DAT_0007652c[idx].name);
										w[l] = '|';
										w[l + 1] = '\0';
										w += l + 1;
									}
									hit = true;
									break;
								}
							}
							if (!hit) {
								snprintf(recvbuf, 0x1000,
								         "API unknown command '%s' in group '%c'", tp, g);
								applog(3, recvbuf, 1);
								quit(1);
							}
						}
						if (after == NULL || *after == '\0')
							break;              /* 本组命令处理完毕 */
						tp = after;
					}
				}
				/* '*' → 追加 cmds 表中全部 iswpriv==0 且未列入的命令 */
				if (star && DAT_0007652c[0].name != NULL) {
					for (idx = 0; DAT_0007652c[idx].name != NULL; idx++) {
						if (DAT_0007652c[idx].iswpriv == 0) {
							sprintf(msgbuf, "|%s|", DAT_0007652c[idx].name);
							if (strstr(gbuf, msgbuf) == NULL) {
								size_t l;

								strcpy(w, DAT_0007652c[idx].name);
								l = strlen(DAT_0007652c[idx].name);
								w[l] = '|';
								w[l + 1] = '\0';
								w += l + 1;
							}
						}
					}
				}

				/* 存入组表 */
				{
					char *lst = malloc(strlen(gbuf) + 1);

					DAT_0007bd24[g - 'A'] = lst;
					if (lst == NULL) {
						snprintf(recvbuf, 0x1000, "Failed to malloc group commands buf");
						applog(3, recvbuf, 1);
						quit(1);
					}
					strcpy(lst, gbuf);
				}
				if (gnext == NULL || *gnext == '\0')
					break;                      /* 全部组完成 */
				gp = gnext;
			} while (1);
		}
	}

	/* ---- 只读组 'R' (noprivgroup): cmds 表中全部 iswpriv==0 命令 (不去重) ---- */
	{
		char *w = gbuf + 1;
		size_t l;

		gbuf[0] = '|';
		gbuf[1] = '\0';
		if (DAT_0007652c[0].name != NULL) {
			for (idx = 0; DAT_0007652c[idx].name != NULL; idx++) {
				if (DAT_0007652c[idx].iswpriv == 0) {   /* 原: 入口 +8 字节非 0 → 跳过 */
					strcpy(w, DAT_0007652c[idx].name);
					l = strlen(DAT_0007652c[idx].name);
					w[l] = '|';
					w[l + 1] = '\0';
					w += l + 1;
				}
			}
		}
		{
			char *lst = malloc(strlen(gbuf) + 1);

			DAT_0007bd24['R' - 'A'] = lst;   /* 组 'R' (原码经 toupper('R') 两段大写表, 恒等) */
			if (lst == NULL) {
				snprintf(recvbuf, 0x1000, "Failed to malloc noprivgroup commands buffer");
				applog(3, recvbuf, 1);
				quit(1);
			}
			strcpy(lst, gbuf);
		}
	}
	free(gwork);

	/* ---- ipaccess 解析 (DAT_00078c98): "[组:]IPv6或IPv4[/位长][, ...]" ----
	 * 每项 0x24B 写入 DAT_00078cb4: addr[16]@+0 / mask[16]@+0x10 / 组字母@+0x20;
	 * IPv4 经 "::ffff:%s" 映射为 IPv6; "0/0" = addr/mask 全零通配; 组缺省 'R',
	 * 前缀组仅 'W' 或已定义组生效; 无位长 → 掩码全 1; 解析失败项不计入。 */
	if (DAT_00078c98 != NULL) {
		char *ipwork;
		int slots, nvalid;
		char *item, *s, c;

		ipwork = malloc(strlen(DAT_00078c98) + 1);
		if (ipwork == NULL) {
			snprintf(gbuf, 0x1000, "Failed to malloc ipaccess buffer");
			applog(3, gbuf, 1);
			quit(1);
		}
		strcpy(ipwork, DAT_00078c98);
		slots = 1;
		for (p = ipwork; *p != '\0'; p++) {
			if (*p == ',')
				slots++;               /* 项数上限 = 逗号数 + 1 */
		}
		DAT_00078cb4 = calloc(slots, 0x24);
		if (DAT_00078cb4 == NULL) {
			snprintf(gbuf, 0x1000, "Failed to calloc ipaccess");
			applog(3, gbuf, 1);
			quit(1);
		}
		DAT_00078cb0 = 0;
		nvalid = 0;
		item = ipwork;
		c = *item;
		if (*item != '\0') {
			for (;;) {
				char *nextitem;
				api_ipaccess_t *ent;
				uint8_t grp;

				while (c == ' ' || c == '\t') {
					item++;
					c = *item;
				}
				if (c == ',') {
					nextitem = item + 1;   /* 空项 → 跳过 */
					goto next_item;
				}
				p = strchr(item, ',');
				nextitem = NULL;
				if (p != NULL) {
					nextitem = p + 1;
					*p = '\0';
				}
				/* 组前缀 "G:" — 仅 'W' 或已定义组生效, 否则保持缺省 'R' */
				grp = 'R';
				if (isalpha((unsigned char)item[0]) && item[1] == ':') {
					uint8_t gg = (uint8_t)toupper((unsigned char)item[0]);

					if (gg == 'W' || DAT_0007bd24[gg - 'A'] != NULL)
						grp = gg;
					item += 2;
				}
				ent = (api_ipaccess_t *)((uint8_t *)DAT_00078cb4 + (size_t)nvalid * 0x24);
				/* 原码中途经 DAT_00078cb0 重取同一项 (处理期间 DAT_00078cb0 恒等于 nvalid) */
				ent->group = grp;
				s = item;
				c = s[0];
				if (c == '0' && s[1] == '/' && s[2] == '0' && s[3] == '\0') {
					memset(ent->addr, 0, 0x10);      /* "0/0": addr/mask 全零 */
					memset(ent->mask, 0, 0x10);
					nvalid++;
					DAT_00078cb0 = nvalid;
					goto next_item;
				}
				p = strchr(s, '/');
				{
					char v6 = 0;
					const char *addrstr;

					if (p == NULL) {
						memset(ent->mask, 0xff, 0x10);   /* 无前缀 → 掩码全 1 */
						p = cmd + strlen(cmd);
					}
					if (c == '[' && p[-1] == ']') {      /* "[IPv6]" */
						v6 = 1;
						addrstr = cmd + 1;
						cmd[0] = '\0';
						p[-1] = '\0';
					} else {
						addrstr = cmd;
					}
					if (*p == '\0')
						goto parse_addr;                 /* 原 LAB_0003722c 直入 */
					*p = '\0';
					{
						long bits = strtol(p + 1, NULL, 10);

						if (bits <= 0 || bits + (v6 ? 0 : 0x60) >= 0x81)
							goto next_item;              /* 位长无效 → 跳过 (不计入) */
						bits += v6 ? 0 : 0x60;           /* IPv4 → +96 (映射前缀) */
						memset(ent->mask, 0, 0x10);
						{
							int k;

							for (k = 0; k < bits; k++)   /* 高 bits 位掩码 (等价原逐位循环) */
								ent->mask[k / 8] |= (uint8_t)(0x80 >> (k % 8));
						}
					}
parse_addr:                  /* 原 LAB_0003722c: 地址清零 → 解析 → AND 掩码 → 计入 */
					memset(ent->addr, 0, 0x10);
					{
						int ok;

						if (v6) {
							ok = inet_pton(AF_INET6, addrstr, ent->addr);
						} else {
							sprintf(gbuf, "::ffff:%s", addrstr);
							ok = inet_pton(AF_INET6, gbuf, ent->addr);
						}
						if (ok == 1) {
							int k;

							for (k = 0; k < 0x10; k++)
								ent->addr[k] &= ent->mask[k];
							nvalid++;
							DAT_00078cb0 = nvalid;
						}
						/* 解析失败 → 不计入, 继续下一项 */
					}
				}
next_item:                   /* 原 LAB_000370cc */
				if (nextitem == NULL)
					break;
				item = nextitem;
				c = *item;
				if (c == '\0')
					break;
			}
		}
		free(ipwork);
		if (nvalid == 0) {
			/* 无有效 IP → API 不启动 */
			if (APPLOG_GATE(4)) {
				snprintf(gbuf, 0x1000, "API not running (no valid IPs specified)%s",
				         " - API will not be available");
				applog(4, gbuf, 0);
			}
			free(sock);
			return;
		}
	}

	/* ---- 监听 socket 建立 ---- */
	cgsleep_ms(DAT_00075dcc * 1000);   /* API 延迟启动 (ELF 初值 5 → 5000ms) */
	sprintf(portstr, "%d", port);
	{
		struct addrinfo hints;
		struct addrinfo *res = NULL;
		struct addrinfo *ai;           /* 迭代器兼 bind 目标 (原 paVar29) */

		hints.ai_flags = AI_PASSIVE;   /* 1 */
		hints.ai_family = AF_UNSPEC;   /* 0 */
		hints.ai_socktype = 0;         /* 原码置 0 (任意类型) */
		hints.ai_protocol = 0;
		hints.ai_addrlen = 0;
		hints.ai_addr = NULL;
		hints.ai_canonname = NULL;
		hints.ai_next = NULL;
		if (getaddrinfo(DAT_00075ecc, portstr, &hints, &res) != 0) {
			if (APPLOG_GATE(3)) {
				snprintf(gbuf, 0x1000, "API failed to resolve %s", DAT_00075ecc);
				applog(3, gbuf, 0);
			}
			free(sock);
			return;
		}
		if (res == NULL) {
			n = *sock;                 /* -1 */
		} else {
			ai = res;
			do {
				/* 原反编译: socket 家族取链表头 res->ai_family, 迭代器仅用于前进
				 * (与上游 socket(ai->ai_family) 惯例不一致, 按反编译保留) */
				n = socket(res->ai_family, SOCK_STREAM, 0);
				*sock = n;
				if (n > 0)
					goto have_sock;
				ai = ai->ai_next;
			} while (ai != NULL);
		}
		if (n == -1) {
			if (APPLOG_GATE(3)) {
				snprintf(gbuf, 0x1000, "API initialisation failed (%s)%s", strerror(errno),
				         " - API will not be available");
				applog(3, gbuf, 0);
			}
			freeaddrinfo(res);
			free(sock);
			return;
		}
		ai = NULL;   /* 原反编译: 仅全失败且 fd==0 (非 -1) 落空到此 — bind 将解引用 NULL, 按原样保留 */
have_sock:
		{
			int optval = 1;

			if (setsockopt(n, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0 &&
			    APPLOG_GATE_DBG(7)) {
				snprintf(gbuf, 0x1000, "API setsockopt SO_REUSEADDR failed (ignored): %s",
				         strerror(errno));
				applog(7, gbuf, 0);
			}
		}

		/* bind: 失败每 30s 重试, 超过 61s (0x3d) 放弃退出 */
		t0 = time(NULL);
		while (bind(*sock, ai->ai_addr, ai->ai_addrlen) < 0) {
			errstr = strerror(errno);
			t1 = time(NULL);
			if (t1 - t0 > 0x3d) {
				freeaddrinfo(res);
				if (!APPLOG_GATE(3)) {
					free(sock);
					return;
				}
				snprintf(gbuf, 0x1000, "API bind to port %d failed (%s)%s", port, errstr,
				         " - API will not be available");
				applog(3, gbuf, 0);
				free(sock);
				return;
			}
			if (APPLOG_GATE(4)) {
				snprintf(gbuf, 0x1000, "API bind to port %d failed - trying again in 30sec",
				         port);
				applog(4, gbuf, 0);
			}
			cgsleep_ms(30000);
		}
		freeaddrinfo(res);
		if (listen(*sock, 100) < 0) {
			if (APPLOG_GATE(3)) {
				snprintf(gbuf, 0x1000, "API3 initialisation failed (%s)%s", strerror(errno),
				         " - API will not be available");
				applog(3, gbuf, 0);
			}
			close(*sock);
			free(sock);
			return;
		}

		/* 启动模式日志 */
		if (DAT_00078c98 == NULL) {
			if (DAT_0007b7c4 == '\0') {
				if (APPLOG_GATE(4)) {
					snprintf(gbuf, 0x1000, "API running in local read access mode on port %d (%d)",
					         port, *sock);
					applog(4, gbuf, 0);
				}
			} else if (APPLOG_GATE(4)) {
				snprintf(gbuf, 0x1000, "API running in UNRESTRICTED read access mode on port %d (%d)",
				         port, *sock);
				applog(4, gbuf, 0);
			}
		} else if (APPLOG_GATE(4)) {
			snprintf(gbuf, 0x1000, "API running in IP access mode on port %d (%d)", port, *sock);
			applog(4, gbuf, 0);
		}
	}

	if (DAT_0007b624 != '\0')        /* 多播启用标志 */
		mcast_init();
	DAT_00078cd4 = _k_new_list("StrBufs", 0xc, 2, 0, 0, "api.c", DAT_00066a40, 0x1521);

	/* ---- accept 主循环 (BYE 标志置位后整体跳过/退出) ---- */
	if (DAT_00078cac == '\0') {
		multi = false;
		jroot = NULL;
		do {
			socklen = 0x80;
			conn = accept(*sock, (struct sockaddr *)cliaddr, &socklen);
			if (conn < 0) {
				if (APPLOG_GATE(3)) {
					snprintf(msgbuf, 0x1000, "API failed (%s)%s (%d)", strerror(errno),
					         " - API will not be available", *sock);
					applog(3, msgbuf, 0);
				}
				break;
			}
			deny = check_connect((struct sockaddr *)cliaddr, &conn_ip, &conn_group);
			if (APPLOG_GATE_DBG(7)) {
				snprintf(msgbuf, 0x1000, "API: connection from %s - %s", conn_ip,
				         deny == 0 ? "Ignored" : "Accepted");
				applog(7, msgbuf, 0);
			}
			if (deny != 0) {          /* 非零 = 接受 */
				n = recv(conn, recvbuf, 0x1fff, 0);
				if (n < 0) {
					recvbuf[0] = '\0';   /* 原: (uint32_t&)recvbuf &= 0xffffff00 */
					if (APPLOG_GATE_DBG(7)) {
						snprintf(msgbuf, 0x1000, "API: recv failed: %s", strerror(errno));
						applog(7, msgbuf, 0);
					}
				} else {
					recvbuf[n] = '\0';
					if (APPLOG_GATE_DBG(7)) {
						snprintf(msgbuf, 0x1000, "API: recv command: (%d) '%s'", n, recvbuf);
						applog(7, msgbuf, 0);
					}
					isjson = false;
					DAT_00078cd8 = time(NULL);   /* 最后活动时刻 */
					io->cur = io->ptr;           /* 复位响应缓冲 */
					io->ptr[0] = '\0';
					io->close = 0;

					if (recvbuf[0] != '{') {
						/* 文本命令: "cmd[|params]" */
						p = strchr(recvbuf, '|');
						if (p == NULL) {
							cmd = recvbuf;
							params = NULL;
						} else {
							params = p + 1;
							*p = '\0';
							cmd = recvbuf;
						}
						goto dispatch;
					}
					/* JSON 命令: {"command": "...", "parameter": ...} */
					jroot = json_loadb(recvbuf, n, 0, &json_err);
					if (jroot == NULL || jroot->type != 0) {      /* 解析失败或非对象 */
						message(io, MSG_BADJSON, 0, 0, 1);
						send_result(io, conn, 1);
						isjson = (jroot != NULL);
						loopflag = true;
						goto after_dispatch;
					}
					jnode = json_object_get(jroot, "command");
					if (jnode == NULL) {
						message(io, MSG_NOJSONFIELD, 0, 0, 1);
						send_result(io, conn, 1);
						isjson = true;
						loopflag = true;
						goto after_dispatch;
					}
					if (jnode->type != 2) {      /* command 非 JSON 字符串 */
						message(io, MSG_INVALID_CMD, 0, 0, 1);
						send_result(io, conn, 1);
						isjson = true;
						loopflag = true;
						goto after_dispatch;
					}
					cmd = json_string_value(jnode);   /* 原 r0 承接上一调返回 (Ghidra 无参显示) */
					jnode = json_object_get(jroot, "parameter");
					if (jnode == NULL) {
						params = NULL;
						isjson = true;
					} else {
						switch (jnode->type) {
						case 2:                  /* JSON_STRING */
							params = json_string_value(jnode);   /* 同上 (r0 承接) */
							isjson = true;
							break;
						case 3:                  /* JSON_INTEGER → 十进制串 */
							sprintf(gbuf, "%d", json_integer_value(jnode));
							params = gbuf;
							isjson = true;
							break;
						case 4:                  /* JSON_REAL → %f 串 */
							sprintf(gbuf, "%f", json_real_value(jnode));
							params = gbuf;
							isjson = true;
							break;
						default:
							params = NULL;
							isjson = true;
							break;
						}
					}
dispatch:                        /* 原 LAB_000378e0: 命令 (可 '+' 连接) 分发 */
					/* 多命令探测: cmd 含 '+' → 拆分依次执行, 结果合并为一次发送 */
					if (strchr(cmd, '+') == NULL) {
						multi = false;
						ismulti = false;
						donebuf = NULL;
					} else {
						ismulti = true;
						donebuf = malloc(strlen(cmd) + 3);
						if (donebuf == NULL) {
							snprintf(msgbuf, 0x1000, "OOM cmdsbuf in %s %s():%d",
							         "api.c", "api", 0x1590);
							applog(3, msgbuf, 1);
							quit(1);
						}
						multi = true;
						params = NULL;       /* 多命令模式清除参数 */
						donebuf[0] = '|';
						donebuf[1] = '\0';
					}
					next = cmd;
					do {
						if (multi) {
							p = strchr(cmd, '+');
							next = p;
							if (p != NULL) {
								next = p + 1;
								*p = '\0';
							}
							if (*cmd == '\0')
								goto advance;   /* 空段 → 下一命令 */
						}
						/* cmds 表精确查找 */
						idx = 0;
						while (DAT_0007652c[idx].name != NULL &&
						       strcmp(cmd, DAT_0007652c[idx].name) != 0)
							idx++;
						if (DAT_0007652c[idx].name != NULL) {
							sprintf(tokbuf, "|%s|", cmd);
							if (multi) {
								if (strstr(donebuf, tokbuf) != NULL)
									goto advance;   /* 本轮已处理 → 跳过 */
								p = stpcpy(donebuf + strlen(donebuf), cmd);   /* 记入已完成集 */
								p[0] = '|';
								p[1] = '\0';
								head_join(io, cmd, (int)isjson, &ismulti);
								if (DAT_0007652c[idx].multi_ok == 0) {   /* +0x09 == 0 → 直接拒绝 */
									message(io, MSG_ACCDENY, 0, (int)(intptr_t)DAT_0007652c[idx].name,
									        (int)isjson);
									tail_join(io, (int)isjson);
									goto advance;
								}
							}
							/* 组权限检查: 'W' 直通, 否则组访问串须含 "|cmd|" */
							{
								uint8_t g = (uint8_t)toupper((unsigned char)conn_group);

								if (g == 'W' ||
								    strstr(DAT_0007bd24[g - 'A'], tokbuf) != NULL) {
									(*DAT_0007652c[idx].fn)(io, conn, params, (int)isjson,
									                        conn_group);
								} else {
									message(io, MSG_ACCDENY, 0,
									        (int)(intptr_t)DAT_0007652c[idx].name, (int)isjson);
									if (APPLOG_GATE_DBG(7)) {
										snprintf(msgbuf, 0x1000,
										         "API: access denied to '%s' for '%s' command",
										         conn_ip, DAT_0007652c[idx].name);
										applog(7, msgbuf, 0);
									}
								}
							}
							if (!multi)
								goto send_now;
							goto tail_now;
						}
						/* 未识别命令 */
						if (multi) {
							head_join(io, cmd, (int)isjson, &ismulti);
							message(io, MSG_INVALID_CMD, 0, 0, (int)isjson);
tail_now:                        /* 原 LAB_00037d40 */
							tail_join(io, (int)isjson);
							goto advance;
						}
						message(io, MSG_INVALID_CMD, 0, 0, (int)isjson);
send_now:                        /* 原 LAB_00037ca8 */
						send_result(io, conn, (int)isjson);
advance:                     /* 原 LAB_00037a3c */
						if (multi)
							cmd = next;
					} while (multi && cmd != NULL);
					loopflag = isjson;
after_dispatch:
					if (multi)
						send_result(io, conn, (int)loopflag);   /* 多命令合并发送 */
					/* json root 引用释放 (仅对象且引用非永久 -1) */
					if (isjson && jroot != NULL && jroot->type == 0 &&
					    jroot->refcount != -1) {
						jroot->refcount--;
						if (jroot->refcount == 0)
							json_delete(jroot);
					}
				}
			}
			close(conn);
		} while (DAT_00078cac == '\0');
	}

	/* ---- pthread_cleanup_pop(1): 弹栈并调用 tidyup(sock) — 加锁置 BYE 标志 +
	 * shutdown/close 监听 fd + 释放 ipaccess 数组与 io_list 链 (含 io_data/缓冲) ---- */
	pthread_cleanup_pop(1);
	free(sock);

	if (APPLOG_GATE_DBG(7)) {
		const char *why;

		if (DAT_00078cae != '\0')        /* QUIT 优先 */
			why = "QUIT";
		else if (DAT_00078cad != '\0')   /* RESTART 次之 */
			why = "RESTART";
		else if (DAT_00078cac != '\0')   /* tidyup 已置 BYE — 实际其余分支不可达 */
			why = "BYE";
		else
			why = "UNKNOWN!";
		snprintf(msgbuf, 0x1000, "API: terminating due to: %s", why);
		applog(7, msgbuf, 0);
	}

	ret = pthread_mutex_lock(&DAT_00078cb8);
	if (ret != 0)
		_mutex_lock(DAT_00066a40, 0x15f3);
	if (DAT_00078cad == '\0') {
		if (DAT_00078cae == '\0')
			goto unlock_done;            /* BYE — 无善后 */
		/* QUIT — 创建 clean-quit 线程 (thr_info 以 thrbuf 为参数块与线程参数) */
		ret = thr_info_create(thrbuf, NULL, quit_thread, thrbuf);
		if (ret != 0) {
			quit_restart_unlock(&DAT_00078cb8, DAT_00066a40, 0x1602);
			snprintf(msgbuf, 0x1000, "API failed to initiate a clean quit - aborting");
			applog(3, msgbuf, 1);
			quit(1);
		}
	} else {
		/* RESTART — 创建重启线程 */
		ret = thr_info_create(thrbuf, NULL, api_restart_thread, thrbuf);
		if (ret != 0) {
			quit_restart_unlock(&DAT_00078cb8, DAT_00066a40, 0x15f9);
			snprintf(msgbuf, 0x1000, "API failed to initiate a restart - aborting");
			applog(3, msgbuf, 1);
			quit(1);
		}
	}
	pthread_detach(*(pthread_t *)(thrbuf + 0xc));   /* thr->pth (thr_info_create 写入 +0xC;
	                                                 * 原码复用 cleanup 栈槽, 转写为独立缓冲) */
unlock_done:
	ret = pthread_mutex_unlock(&DAT_00078cb8);
	if (ret != 0)
		_mutex_unlock(DAT_00066a40, 0x1608);

	(*DAT_00075ed8)();   /* 尾调空钩子 (noop_hook, 见 name_map FUN_00028ae0) */
}

/* ============================================================================
 * 覆盖率表 — batch api_b1b: cgminer/api_p1b.c (4/4 已转写)
 * ---------------------------------------------------------------------------
 * 语义名        | 原地址    | 来源 (reversed/cgminer/30_api_status/) | 源行数
 * --------------+-----------+----------------------------------------+-------
 * checkcommand  | 00033c34  | FUN_00033c34.c                         | 76
 * devdetails    | 00033dcc  | FUN_00033dcc.c                         | 69
 * mcast_init    | 0003669c  | FUN_0003669c.c                         | 86
 * api           | 00036764  | FUN_00036764.c                         | 953
 * ============================================================================ */

