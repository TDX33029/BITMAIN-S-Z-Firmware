/*
 * rewritten/cgminer/api_p2.c — cgminer API 层第 2 段 (语义化转写)
 *
 * 来源: reversed/cgminer/30_api_status/FUN_0000b774.c ~ FUN_0005a2f8.c (34 个函数)
 * 范围 (按主题分组):
 *   1. 锁错误上报包装 (4):  _mutex_lock / _mutex_unlock / api_rw_unlock / api_rd_lock
 *   2. 主流程回调 (3):      zero_bestshare / kill_work / inc_dev_status
 *   3. 线程与生命周期 (7):  thr_info_create / mcast_thread / quit_restart_unlock /
 *                           api_restart_thread / quit_thread / tidyup /
 *                           create_bitmain_check_miner_status_pthread
 *   4. io 缓冲与响应拼装 (4): add_item_buf / io_add / head_join / tail_join
 *   5. api_add_* 单行包装 (9): escape/string/const/int/uint32/elapsed/timeval/time/mhs
 *   6. 命令与统计处理 (4):  asccount / dosave / itemstats / bm1740_makeup_get_status_cmd
 *   7. jansson 底层 (3):    json_loadb / json_integer_value / json_real_value
 *
 * 事实还原 (全部以反编译偏移 / 反汇编 / ELF 数据为证):
 *   - api_data 链表节点布局由 api_add_data_full (FUN_00031cdc) 反编译证实:
 *     0x18 字节节点 {int type(+0); char *name(+4); char *data(+8); bool copy(+0xC);
 *     prev(+0x10); next(+0x14)}, 且 root 为环链表头: 尾节点 prev 回指头,
 *     头节点 prev 恒指向最新尾 (root->prev = tail)。
 *   - io_data 增长缓冲: io_add (FUN_000307c8) 证实 {siz(+0); ptr(+4); cur(+8)},
 *     tail_join/api_notify/asccount 证实 +0xD 处 1 字节 close 标志 (已输出 JSON
 *     段需闭合); +0xC 为 sock 字节 (对照上游 api.c struct io_data, 本段未读写)。
 *   - print_data 的条目暂存缓冲: print_data (FUN_00032534) 上下文 +0xC 指向
 *     {char *buf(+0); int used(+4); int tot(+8)} 三元组 (add_item_buf 操作对象)。
 *   - 响应拼装字符串常量逐一自 ELF .rodata 核对 (地址见下方宏), 名称对照上游
 *     cgminer api.c: JSON0="{" JSON1="\"" JSON2="\":[" JSON_END=",\"id\":1}"
 *     COMSTR="," JOIN_CMD="CMD=" SEPSTR="|"。
 *   - SOCKBUFALLOCSIZ = 0x10000 (上游 api.c 同名宏); io_add 溢出分支的
 *     "(int)(tot/0x10000)" 在原码中经 ARM VCVT 定点转换指令实现, 语义为截断除法
 *     (Ghidra 伪函数 VectorFloatToUnsignedFixed/VectorFloatToUnsigned)。
 *   - head_join 参数序 (io, cmd, isjson, &first 标志字节) 由调用点 api()
 *     (FUN_00036764 @0x787/0x822) 与 tail_join 共用 bVar34 作 isjson 证实。
 *   - itemstats 第 4 参 print_data 的 prepend_comma 反编译缺参, 反汇编
 *     (0x35250~0x3526c) 证实 = (isjson != 0 && i > 0) ? 1 : 0。
 *   - kill_work 的 cg_completion_timeout 回调地址 0x0001b4c0 在 name_map 未收录,
 *     按 fan.c 既有约定以 fun_0001b4c0 占位 (注释标 命名待补)。
 *   - jansson: json_t {int type(+0); int refcount(+4); 值(+8)};
 *     json_type: 3=INTEGER / 4=REAL (与 json.c 枚举一致)。
 *   - 全局 (name_map 未收录, 保留原名并注明已知用途, 其余语义待定):
 *     DAT_00075ed8 = noop_hook 全局函数指针 (初值 FUN_00028ae0);
 *     DAT_00078cb8 = quit_restart_lock; DAT_00078cac = api() 主循环退出标志
 *     (api() 循环条件检查, tidyup 置 1); DAT_0007b198/DAT_0007b438 = total_pools/
 *     pools; DAT_00075dc4(8B) = best share 格式化串缓冲 (suffix_string 输出,
 *     share_diff "New best share: %s" 读取); DAT_00076f18(8B) = best share/diff
 *     值 (share_diff 写入, stats "Best Share" 读取); DAT_000755f8(double) = 运行
 *     秒数; DAT_0007acc0(double) = GHS av 分子; DAT_00078b84 = GHS 5s 格式化串
 *     (api_stats sprintf 写入)。
 *
 * 注: 下方 extern 声明均取自 name_map 语义名, 定义分布在其余批次文件;
 *     最终应上移至 rewritten/include/*.h 共享。
 */
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>      /* FILE/fopen/fclose/snprintf */
#include <stdlib.h>     /* malloc/free/calloc/realloc/getenv */
#include <string.h>     /* strlen/memcpy */
#include <errno.h>      /* errno (原码为 __errno_location() 显式读取, 等价) */
#include <time.h>       /* time_t (本平台 32 位) */
#include <sys/time.h>   /* struct timeval (api_add_timeval / itemstats) */
#include <sys/socket.h> /* shutdown (tidyup) */
#include <unistd.h>     /* close */
#include <pthread.h>

/* glibc: errno 线程局部取址 (原码锁包装直调 __errno_location, 显式声明) */
extern int *__errno_location(void);

/* ================= API 响应拼装字符串常量 =================
 * (地址经 ELF .rodata 实测; 名称对照上游 cgminer api.c) */

#define COMSTR       ","          /* DAT_00060764: JSON 段间分隔 */
#define JSON0        "{"          /* DAT_00066aa8: 首个 JSON 段前导 */
#define JSON1        "\""         /* DAT_00060810: 段名前引号 */
#define JSON2        "\":["       /* DAT_0006712c: 段名后缀 */
#define JSON_CLOSE   "]"          /* tail_join 内联字面量; io_add 长度预留成分 */
#define JSON_END     ",\"id\":1}" /* DAT_00066afc */
#define JOIN_CMD     "CMD="       /* DAT_00066aac: 文本模式前导 */
#define SEPSTR       "|"          /* DAT_000606fc: 文本模式段分隔 */

#define SOCKBUFALLOCSIZ 0x10000   /* 上游 api.c: 65536, io_add 扩容基粒度 */

/* ================= 类型 ================= */

/* api_add_* 链表节点 (布局由 api_add_data_full 反编译证实, 见文件头) */
typedef struct api_data api_data_t;
struct api_data {
	int       type;    /* +0x00: api_data_type 枚举 (0=ESCAPE 1=STRING 2=CONST
		          * 3=UINT8 4=UINT16 5=SHORT 6=INT 7=UINT 8=UINT32 9=INT32
		          * 10=UINT64 11=INT64 12=DOUBLE 13=ELAPSED 14=UTILITY
		          * 15=TIMEVAL 16=TIME 17=MHS 18=MHTOTAL 19=TEMP 20=UTILITY2
		          * 24=DIFF 25=PERCENT …) */
	const char *name;  /* +0x04 */
	char      *data;   /* +0x08 (按 type 拷贝/借用) */
	int       copy;    /* +0x0C: bool, data 是否已拷贝 */
	api_data_t *prev;  /* +0x10: 环链表 — 头节点 prev 恒指向尾 */
	api_data_t *next;  /* +0x14 */
};

/* API 响应输出缓冲 (io_add/tail_join/asccount 用法证实, 见文件头) */
typedef struct io_data {
	uint32_t siz;    /* +0x00: 已分配容量 */
	char    *ptr;    /* +0x04: 缓冲基址 */
	char    *cur;    /* +0x08: 当前写位置 (恒停在最后写入的 NUL 上) */
	uint8_t  sock;   /* +0x0C (本段未读写; 对照上游 struct io_data) */
	uint8_t  close;  /* +0x0D: 已输出 JSON 段待闭合标志 */
} io_data_t;

/* print_data 的条目暂存缓冲 (print_data 上下文 +0xC 指向, add_item_buf 操作对象) */
typedef struct {
	char *buf;   /* +0: 缓冲基址 */
	int   used;  /* +4: 已写字节数 (写入位置) */
	int   tot;   /* +8: 已分配容量 */
} datasb_t;

/* jansson 2.x (与 json.c 一致) */
typedef struct json_t {
	int type;       /* +0x00: json_type 枚举 (3=INTEGER / 4=REAL) */
	int refcount;   /* +0x04 */
} json_t;

/* 解析错误描述 (252 字节, 与 json.c 布局一致) */
typedef struct json_error_t {
	int  line;        /* +0x000 */
	int  column;      /* +0x004 */
	int  position;    /* +0x008 */
	char source[80];  /* +0x00C */
	char text[160];   /* +0x05C */
} json_error_t;

/* ================= 外部函数 (name_map 语义名) ================= */

extern void    applog(uint32_t prio, char *buf, ...);      /* FUN_00038730: 第 3 参语义待定 (1/0 两见) */
extern void    quit(int status);                                /* FUN_00016ca8 */
extern void    suffix_string(double value, char *buf, size_t bufsiz, int sigfigs); /* 00011580 */
extern void    mutex_lock_fail_cgminer(char *file, int line);   /* 0000b414: 锁失败上报+退出 */
extern void    mutex_unlock_fail_cgminer(char *file, int line); /* 0000b5c4 */
extern void    cg_completion_timeout(void (*fn)(void *), void *arg, uint32_t timeout_ms); /* 0002f438 */
extern void    fun_0001b4c0(void *arg);                         /* 原 0x0001b4c0 命名待补: kill_work 收尾回调 */
extern void    rename_thread(const char *name);                 /* 0002f0b4 */
extern void    mcast(void);                                     /* 0003107c: 组播监听 (api.c mcast) */
extern void    _cgsem_init(void *sem, const char *file, const char *fn, int line); /* 0002f0f4 */
extern void    restart_mine(void);                              /* 0001d7c8 */
extern void   *check_miner_status(void *arg);                   /* 00044b5c: miner_status 检查线程体
			                                          * (反编译签名 void(void) 为线程体推断噪声) */
extern unsigned int cgsleep_ms(unsigned int ms);                /* 0002a92c (r0 直通 FUN_0002a8c4 返回值) */
extern char   *escape_string(char *str, int isjson);            /* 00030500 */
extern api_data_t *api_add_data_full(api_data_t *root, const char *name, int type,
                                     void *data, int copy);     /* 00031cdc: 环链表构造核心 */
extern void    message(void *io_data, int code, int param0, int param1, int isjson); /* 0003305c */
extern int     print_data(void *io_data, api_data_t *root, int isjson, int prepend_comma); /* 00032534 */
extern void    default_save_file(char *buf);                    /* 00016150: 填充默认配置文件路径 */
extern void    write_config(FILE *fp);                          /* 00015b64 */
extern void    jsonp_error_init(json_error_t *error, const char *source);   /* 0005b014 */
extern void    error_set(json_error_t *error, int line, const char *fmt, ...); /* 00058034 */
extern int     lex_init(void *lex, int (*reader)(void *), void *data);      /* 00058154: 成功返回 0 */
extern int     fun_00057fb4(void *data);                        /* 原 0x00057fb4 命名待补: jansson buffer_get 流读取回调 */
extern json_t *parse_json(void *lex, uint32_t flags, json_error_t *error); /* 00059154 (原反编译多一未用第 4 参) */
extern void    lex_cleanup(void *lex);                          /* 000581a0 */

/* ================= 外部全局 (语义见文件头说明) ================= */

extern void (*DAT_00075ed8)(void);          /* noop_hook 全局函数指针 (初值 FUN_00028ae0) */
extern pthread_mutex_t DAT_0007ad4c;        /* inc_dev_status 互斥锁 (语义待定) */
extern int      DAT_0007b8a8;               /* 设备状态计数 (仅本段 inc_dev_status 写入, 语义待定) */
extern int      DAT_0007bb20;               /* 设备状态计数 (同上, 语义待定) */
extern int      DAT_0007b198;               /* total_pools */
extern int    **DAT_0007b438;               /* pools[] (pool* 数组基址) */
extern uint32_t DAT_00075dc4;               /* 8B 串缓冲: best share 格式化串 (suffix_string 输出) */
extern uint32_t DAT_00075dc8;               /* 上行缓冲后半字 (0x75dc4..0x75dcb) */
extern uint32_t DAT_00076f18;               /* 8B best share/diff 值 (share_diff 写入) */
extern uint32_t DAT_00076f1c;               /* 上行值后半字 */
extern pthread_mutex_t DAT_00078cb8;        /* quit_restart_lock */
extern uint8_t  DAT_00078cac;               /* api() 主循环退出标志 (tidyup 置 1) */
extern void    *DAT_00078cb4;               /* api() 分配的连接条目数组 (0x24 字节/项) */
extern int     *DAT_00078cd0;               /* 访问条目环链表头 {子结构(+0), next(+8)}, 子结构+4 为串 */
extern uint8_t  DAT_00078e09;               /* 日志守卫: 外层总开关 (疑 opt_log_output, 语义待定) */
extern uint8_t  DAT_0007ad48;               /* 日志守卫: 次级开关 (语义待定) */
extern uint8_t  DAT_00078e08;               /* 日志守卫: 次级开关 (语义待定) */
extern int      DAT_000766c4;               /* 日志守卫: 级别阈值 (值越大越详细) */
extern double   DAT_0007acc0;               /* GHS av 分子 (全局, 语义待定) */
extern double   DAT_000755f8;               /* 运行秒数 (elapsed; itemstats "Elapsed" 数据源) */
extern char     DAT_00078b84;               /* GHS 5s 格式化串缓冲首字节 (api_stats sprintf 写入) */
extern void    *DAT_0007c060;               /* miner_status 检查线程 thr_info (0x40 字节) */

/* 日志守卫 (与 json.c 同型的全库统一内联模式) */
#define API_LOG_GUARD() \
	(DAT_00078e09 != 0 && (DAT_0007ad48 != 0 || DAT_00078e08 != 0 || 6 < DAT_000766c4))

/* ============================================================
 * 1. 锁错误上报包装 (api.c 内部锁封装的错误分支)
 * ============================================================ */

// === _mutex_lock === 原 FUN_0000b774 @ 0x0000b774
// 来源: reversed/cgminer/30_api_status/FUN_0000b774.c | 分类: 30_api_status
// 语义: api.c 互斥锁加锁失败上报: 取 errno 打印 "WTF MUTEX ERROR ON LOCK!" 并
//       quit(1) 终止 (调用点内联完成加锁, 本函数仅错误分支)。
void _mutex_lock(char *file, int line)
{
	int *err;

	err = __errno_location();
	char msg[4100];
	snprintf(msg, 0x1000, "WTF MUTEX ERROR ON LOCK! errno=%d in %s %s():%d", *err, "api.c", file, line);
	applog(3 /*LOG_ERR*/, msg, 1);
	quit(1);        /* 不返回 */
}

// === _mutex_unlock === 原 FUN_0000b84c @ 0x0000b84c
// 来源: reversed/cgminer/30_api_status/FUN_0000b84c.c | 分类: 30_api_status
// 语义: api.c 互斥锁解锁失败上报: 打印 "WTF MUTEX ERROR ON UNLOCK!" 并 quit(1)。
void _mutex_unlock(char *file, int line)
{
	int *err;

	err = __errno_location();
	char msg[4100];
	snprintf(msg, 0x1000, "WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d", *err, "api.c", file, line);
	applog(3 /*LOG_ERR*/, msg, 1);
	quit(1);        /* 不返回 */
}

// === api_rw_unlock === 原 FUN_0000b7e0 @ 0x0000b7e0
// 来源: reversed/cgminer/30_api_status/FUN_0000b7e0.c | 分类: 30_api_status
// 语义: api.c 读写锁解锁失败上报: 打印 "WTF RWLOCK ERROR ON UNLOCK!" 并 quit(1)
//       (与 _mutex_unlock 区分; 调用点内联完成解锁)。
void api_rw_unlock(char *file, int line)
{
	int *err;

	err = __errno_location();
	char msg[4100];
	snprintf(msg, 0x1000, "WTF RWLOCK ERROR ON UNLOCK! errno=%d in %s %s():%d", *err, "api.c", file, line);
	applog(3 /*LOG_ERR*/, msg, 1);
	quit(1);        /* 不返回 */
}

// === api_rd_lock === 原 FUN_00032190 @ 0x00032190
// 来源: reversed/cgminer/30_api_status/FUN_00032190.c | 分类: 30_api_status
// 语义: 对读写锁执行 pthread_rwlock_rdlock; 失败时打印 "WTF RDLOCK ERROR ON
//       LOCK!" 并 quit(1)。成功返回 0 (不入错误分支)。
void api_rd_lock(pthread_rwlock_t *lock, char *file, int line)
{
	int ret;

	ret = pthread_rwlock_rdlock(lock);
	if (ret != 0) {
		int *err = __errno_location();
		char msg[4100];
		snprintf(msg, 0x1000, "WTF RDLOCK ERROR ON LOCK! errno=%d in %s %s():%d", *err, "api.c", file, line);
		applog(3 /*LOG_ERR*/, msg, 1);
		quit(1);    /* 不返回 */
	}
}

/* ============================================================
 * 2. 主流程回调 (与 cgminer.c 主体共享)
 * ============================================================ */

// === zero_bestshare === 原 FUN_000160b4 @ 0x000160b4
// 来源: reversed/cgminer/30_api_status/FUN_000160b4.c | 分类: 30_api_status
// 语义: 清零全局最优份额记录 (两处 8 字节全局, 见文件头) 并把 suffix_string(0)
//       结果 "0" 写回 8 字节串缓冲; 随后遍历所有池把 pool->best_diff (+400 起
//       8 字节) 清零。由 zero_stats (FUN_0001b8b4) 调用。
void zero_bestshare(void)
{
	/* 两处 8 字节全局清零: 0x75dc4 (串缓冲) 与 0x76f18 (best share/diff 值) */
	DAT_00075dc4 = 0;
	DAT_00075dc8 = 0;
	DAT_00076f18 = 0;
	DAT_00076f1c = 0;
	suffix_string(0.0, (char *)&DAT_00075dc4, 8, 0);

	if (0 < DAT_0007b198) {
		int **pl = DAT_0007b438;                    /* pools[] */
		int **pl_end = DAT_0007b438 + DAT_0007b198;
		do {
			char *pool = (char *)*pl;
			*(uint32_t *)(pool + 400) = 0;          /* pool->best_diff 低 32 位 */
			*(uint32_t *)(pool + 404) = 0;          /* pool->best_diff 高 32 位 (u64) */
			pl++;
		} while (pl != pl_end);
	}
}

// === kill_work === 原 FUN_000178f8 @ 0x000178f8
// 来源: reversed/cgminer/30_api_status/FUN_000178f8.c | 分类: 30_api_status
// 语义: 关停入口: 以 5000ms 超时异步执行 0x0001b4c0 收尾回调 (__kill_work,
//       命名待补), 然后打印 "Shutdown signal received." 并 quit(0)。
void kill_work(void)
{
	cg_completion_timeout(fun_0001b4c0, NULL, 5000);
	char msg[] = "Shutdown signal received.";   /* 原码 strncpy 栈串 (0x1a 字节) */
	applog(3, msg, 1);
	quit(0);        /* 不返回 */
}

// === inc_dev_status === 原 FUN_00018924 @ 0x00018924
// 来源: reversed/cgminer/30_api_status/FUN_00018924.c | 分类: 30_api_status
// 语义: 在互斥锁保护下把两个设备状态计数写为入参值, 解锁后调用 noop_hook
//       (经全局函数指针 DAT_00075ed8); 锁失败走 cgminer 主体上报并退出。
void inc_dev_status(int status_a, int status_b)
{
	int ret;

	ret = pthread_mutex_lock(&DAT_0007ad4c);
	if (ret != 0) {
		mutex_lock_fail_cgminer("inc_dev_status", 0x2081);
	}
	DAT_0007b8a8 = status_a;
	DAT_0007bb20 = status_b;
	ret = pthread_mutex_unlock(&DAT_0007ad4c);
	if (ret != 0) {
		mutex_unlock_fail_cgminer("inc_dev_status", 0x2084);
	}
	(*DAT_00075ed8)();                          /* noop_hook (全局函数指针直调) */
}

/* ============================================================
 * 3. 线程与生命周期
 * ============================================================ */

// === thr_info_create === 原 FUN_0002f174 @ 0x0002f174
// 来源: reversed/cgminer/30_api_status/FUN_0002f174.c | 分类: 30_api_status
// 语义: 初始化 thr_info 的 cgsem (+0x10) 后创建线程, 句柄写入 thr+0xC;
//       返回 pthread_create 的结果 (r0 直通, 调用点按 0=成功判读)。
int thr_info_create(void *thr, const pthread_attr_t *attr, void *(*fn)(void *), void *arg)
{
	_cgsem_init((void *)((char *)thr + 0x10), "util.c", "thr_info_create", 0x520);
	return pthread_create((pthread_t *)((char *)thr + 0xc), attr, fn, arg);
}

// === mcast_thread === 原 FUN_00031ca0 @ 0x00031ca0
// 来源: reversed/cgminer/30_api_status/FUN_00031ca0.c | 分类: 30_api_status
// 语义: 组播监听线程体: 分离自身、设为可异步取消、改线程名 "APIMCast"、
//       进入 mcast() (不返回直至线程退出), 退出前把 PTH(mythr) (thr+0xC) 清 0。
void *mcast_thread(void *mythr)
{
	pthread_t self;

	self = pthread_self();
	pthread_detach(self);
	pthread_setcanceltype(1 /*PTHREAD_CANCEL_ASYNCHRONOUS*/, NULL);
	rename_thread("APIMCast");
	mcast();
	*(pthread_t *)((char *)mythr + 0xc) = 0;    /* PTH(mythr) = 0 */
	return 0;
}

// === quit_restart_unlock === 原 FUN_0003220c @ 0x0003220c
// 来源: reversed/cgminer/30_api_status/FUN_0003220c.c | 分类: 30_api_status
// 语义: 解锁 quit_restart_lock (DAT_00078cb8) 失败走 _mutex_unlock 上报, 然后
//       调用 noop_hook (DAT_00075ed8); api() 退出路径 (0x15f9/0x1602) 专用。
void quit_restart_unlock(pthread_mutex_t *lock, char *file, int line)
{
	int ret;

	ret = pthread_mutex_unlock(lock);
	if (ret != 0) {
		_mutex_unlock(file, line);
	}
	(*DAT_00075ed8)();                          /* noop_hook (全局函数指针直调) */
}

// === api_restart_thread === 原 FUN_00032248 @ 0x00032248
// 来源: reversed/cgminer/30_api_status/FUN_00032248.c | 分类: 30_api_status
// 语义: API 侧重启线程体 (与 cgminer.c restart_thread 区分): 加/解锁
//       quit_restart_lock, noop_hook, debug 守卫下打印 "API: restarting
//       cgminer", 最后转投 restart_mine()。原码的栈串逐字构造在此合并为字面量。
void *api_restart_thread(void)
{
	int ret;

	ret = pthread_mutex_lock(&DAT_00078cb8);    /* quit_restart_lock */
	if (ret != 0) {
		_mutex_lock("restart_thread", 0x1357);
	}
	ret = pthread_mutex_unlock(&DAT_00078cb8);
	if (ret != 0) {
		_mutex_unlock("restart_thread", 0x1358);
	}
	(*DAT_00075ed8)();                          /* noop_hook */
	if (API_LOG_GUARD()) {
		char msg[] = "API: restarting cgminer"; /* 原码 24 字节栈串逐字构造 */
		applog(7 /*LOG_DEBUG*/, msg, 0);
	}
	restart_mine();
	return 0;
}

// === quit_thread === 原 FUN_00032340 @ 0x00032340
// 来源: reversed/cgminer/30_api_status/FUN_00032340.c | 分类: 30_api_status
// 语义: API 杀进程线程体: 加/解锁 quit_restart_lock, noop_hook, debug 守卫下
//       打印 "API: killing cgminer", 最后转投 kill_work()。
void *quit_thread(void)
{
	int ret;

	ret = pthread_mutex_lock(&DAT_00078cb8);    /* quit_restart_lock */
	if (ret != 0) {
		_mutex_lock("quit_thread", 0x1349);
	}
	ret = pthread_mutex_unlock(&DAT_00078cb8);
	if (ret != 0) {
		_mutex_unlock("quit_thread", 0x134a);
	}
	(*DAT_00075ed8)();                          /* noop_hook */
	if (API_LOG_GUARD()) {
		char msg[] = "API: killing cgminer";    /* 原码 22 字节栈串逐字构造 */
		applog(7 /*LOG_DEBUG*/, msg, 0);
	}
	kill_work();
	return 0;
}

// === tidyup === 原 FUN_0003243c @ 0x0003243c
// 来源: reversed/cgminer/30_api_status/FUN_0003243c.c | 分类: 30_api_status
// 语义: API 退出清理: 置 api() 主循环退出标志 (DAT_00078cac=1), 关闭监听
//       socket, 释放连接条目数组 (DAT_00078cb4) 与访问条目环链表 (DAT_00078cd0,
//       节点+0=子结构/子结构+4=串/节点+8=next), 全程持 quit_restart_lock。
void tidyup(int *soc)
{
	int ret;

	ret = pthread_mutex_lock(&DAT_00078cb8);    /* quit_restart_lock */
	if (ret != 0) {
		_mutex_lock("tidyup", 0x120c);
	}
	DAT_00078cac = 1;                           /* 通知 api() 主循环退出 */
	if (*soc != -1) {
		shutdown(*soc, 2 /*SHUT_RDWR*/);
		close(*soc);
		*soc = -1;
	}
	if (DAT_00078cb4 != NULL) {
		free(DAT_00078cb4);
		DAT_00078cb4 = NULL;
	}
	if (DAT_00078cd0 != NULL) {
		int *head = DAT_00078cd0;
		int *node = head;
		int *next;
		do {
			int *entry = (int *)node[0];
			next = (int *)node[2];              /* 节点+8: next */
			free(*(void **)((char *)entry + 4));/* 子结构+4: 字符串 */
			free(entry);
			free(node);
			node = next;
		} while (head != next);                 /* 环链表: 回到表头结束 */
		DAT_00078cd0 = NULL;
	}
	ret = pthread_mutex_unlock(&DAT_00078cb8);
	if (ret != 0) {
		_mutex_unlock("tidyup", 0x1221);
	}
	(*DAT_00075ed8)();                          /* noop_hook */
}

// === create_bitmain_check_miner_status_pthread === 原 FUN_000499d4 @ 0x000499d4
// 来源: reversed/cgminer/30_api_status/FUN_000499d4.c | 分类: 30_api_status
// 语义: 创建 miner_status 检查线程: 为全局 thr_info (DAT_0007c060) 分配 0x40
//       字节并 thr_info_create(check_miner_status); 成功则分离线程并 cgsleep
//       500ms (r0 直通其返回值), 失败在 debug 守卫下打印提示并返回 -5
//       (0xfffffffb)。
int create_bitmain_check_miner_status_pthread(void *arg)
{
	char msg[4100];
	int ret;

	DAT_0007c060 = calloc(1, 0x40);
	ret = thr_info_create(DAT_0007c060, NULL, check_miner_status, arg);
	if (ret == 0) {
		pthread_detach(*(pthread_t *)((char *)DAT_0007c060 + 0xc));
		return (int)cgsleep_ms(500);
	}
	if (API_LOG_GUARD()) {
		snprintf(msg, 0x1000, "%s: create thread for check miner_status", __func__);
		applog(7 /*LOG_DEBUG*/, msg, 0);
	}
	return -5;                                  /* 原码 0xfffffffb */
}

/* ============================================================
 * 4. io 缓冲与响应拼装
 * ============================================================ */

// === add_item_buf === 原 FUN_000306b4 @ 0x000306b4
// 来源: reversed/cgminer/30_api_status/FUN_000306b4.c | 分类: 30_api_status
// 语义: 把一条目串追加到 print_data 的暂存缓冲 (上下文 +0xC 指向
//       {buf, used, tot}): 容量不足时按 4096 对齐粒度 realloc 扩容 (原码对齐式
//       ext = len+0x1001-((len+1)&0xfff)), realloc 失败打印 "OOM buf siz=%d
//       tot=%d ext=%d …" 并 quit(1); 尾部连同 NUL 拷入, used 前进 len。
void add_item_buf(void *item_ctx, const char *str)
{
	datasb_t *sb = *(datasb_t **)((char *)item_ctx + 0xc);
	char msg[4100];
	char *base;
	size_t len;
	int used;
	int ext;

	len = strlen(str);
	used = sb->used;                            /* +4: 已写字节数 (写入位置) */
	base = sb->buf;                             /* +0: 缓冲基址 */
	if ((uint32_t)sb->tot < (uint32_t)(used + 1 + (int)len)) {
		ext = (int)len + 0x1001 - ((int)(len + 1) & 0xfff);   /* 4096 对齐扩容 */
		base = realloc(base, ext + sb->tot);
		sb->buf = base;
		if (base == NULL) {
			snprintf(msg, 0x1000, "OOM buf siz=%d tot=%d ext=%d in %s %s():%d",
			         (int)len, sb->tot, ext, "api.c", "add_item_buf", 0x49b);
			applog(3 /*LOG_ERR*/, msg, 1);
			quit(1);                            /* 不返回 */
		}
		sb->tot += ext;                         /* +8: 已分配容量 */
	}
	memcpy(base + used, str, len + 1);          /* 连同 NUL 一并拷入 */
	sb->used = used + (int)len;
}

// === io_add === 原 FUN_000307c8 @ 0x000307c8
// 来源: reversed/cgminer/30_api_status/FUN_000307c8.c | 分类: 30_api_status
// 语义: 把串 (含 NUL) 追加到 io_data 增长缓冲并让 cur 停在末尾 NUL 上。需求
//       tot = (cur-ptr) + 12 + len (12 = 预留 JSON 尾部 JSON_CLOSE+JSON_END 字节);
//       容量不足时先按 siz+0x20000 (2*SOCKBUFALLOCSIZ) 扩容, 仍不足则按
//       (2+(int)(tot/SOCKBUFALLOCSIZ))*SOCKBUFALLOCSIZ 扩 (原码经 VCVT 定点除)。
//       realloc 后以原偏移恢复 cur。恒返回 1。
int io_add(io_data_t *io, const char *str)
{
	char *cur;
	size_t len;
	int offset;
	uint32_t tot;

	len = strlen(str);
	cur = io->cur;
	offset = (int)(cur - io->ptr);
	tot = (uint32_t)offset + 0xc + (uint32_t)len;   /* 0xC = sizeof(JSON_CLOSE)+sizeof(JSON_END) */
	if (io->siz < tot) {
		uint32_t newsize = io->siz + 2 * SOCKBUFALLOCSIZ;   /* siz + 0x20000 */
		char *base;
		if (newsize < tot) {
			/* 原码 VectorFloatToUnsignedFixed/VectorFloatToUnsigned = 截断除以 0x10000 */
			newsize = ((int)(tot / SOCKBUFALLOCSIZ) + 2) * SOCKBUFALLOCSIZ;
		}
		base = realloc(io->ptr, newsize);
		io->siz = newsize;
		io->ptr = base;
		cur = base + offset;
		io->cur = cur;
	}
	memcpy(cur, str, len + 1);
	io->cur = cur + len;
	return 1;
}

// === head_join === 原 FUN_0003085c @ 0x0003085c
// 来源: reversed/cgminer/30_api_status/FUN_0003085c.c | 分类: 30_api_status
// 语义: JOIN (多命令串) 回复段头: JSON 模式下首个段输出 {"段名":[ 并把
//       *first 标志清 0, 后续段输出 ,段名:[; 文本模式输出 CMD=段名| (段名经
//       escape_string, isjson 决定转义方式; 返回的串非原串时 free)。
//       参数序 (io, cmd, isjson, &first) 由 api() 调用点与 tail_join 共参证实。
void head_join(io_data_t *io, char *cmd, int isjson, char *first)
{
	char *esc;

	if (*first == '\0') {
		if (isjson == 0) {
			/* 文本模式 (原标签 00030884): first 已为 0, 直接走 CMD= 分支 */
			goto text_join;
		}
		io_add(io, COMSTR);                     /* ",": 非首个 JSON 段前导 */
		esc = escape_string(cmd, 1);
	}
	else {
		if (isjson == 0) {
			*first = '\0';                      /* 文本模式消费 first 标志 */
		text_join:                              /* 原标签 00030884 */
			esc = escape_string(cmd, isjson);   /* isjson=0: 不转义引号 */
			io_add(io, JOIN_CMD);               /* "CMD=" */
			io_add(io, esc);
			io_add(io, SEPSTR);                 /* "|" */
			goto done;
		}
		io_add(io, JSON0);                      /* "{": 首个 JSON 段前导 */
		*first = '\0';                          /* 消费 first 标志 */
		esc = escape_string(cmd, 1);
	}
	io_add(io, JSON1);                          /* "\"" */
	io_add(io, esc);
	io_add(io, JSON2);                          /* "\":[" */
done:
	if (esc != cmd) {
		free(esc);                              /* escape_string 返回原串则免释放 */
	}
}

// === tail_join === 原 FUN_00031f64 @ 0x00031f64
// 来源: reversed/cgminer/30_api_status/FUN_00031f64.c | 分类: 30_api_status
// 语义: JOIN 回复段尾: 若 io_data close 标志 (+0xD) 已置, 先补 "]" 并清标志;
//       isjson 模式再补 ",\"id\":1}" 与 "]"。
void tail_join(io_data_t *io, int isjson)
{
	if (io->close == 0) {
		if (isjson == 0) {
			return;
		}
	}
	else {
		io_add(io, JSON_CLOSE);                 /* "]" */
		io->close = 0;
		if (isjson == 0) {
			return;
		}
	}
	io_add(io, JSON_END);                       /* DAT_00066afc: ",\"id\":1}" */
	io_add(io, JSON_CLOSE);                     /* "]" */
}

/* ============================================================
 * 5. api_add_* 单行包装 (转投 api_add_data_full 按 type 入链)
 * ============================================================ */

// === api_add_escape === 原 FUN_00032df8 @ 0x00032df8
// 来源: reversed/cgminer/30_api_status/FUN_00032df8.c | 分类: 30_api_status
// 语义: 以 type=0 (API_ESCAPE) 把条目加入 api_data 环链表 (一行包装)。
api_data_t *api_add_escape(api_data_t *root, char *name, char *data, int copy)
{
	return api_add_data_full(root, name, 0 /*API_ESCAPE*/, data, copy);
}

// === api_add_string === 原 FUN_00032e1c @ 0x00032e1c
// 来源: reversed/cgminer/30_api_status/FUN_00032e1c.c | 分类: 30_api_status
// 语义: 以 type=1 (API_STRING) 把条目加入 api_data 环链表 (一行包装)。
api_data_t *api_add_string(api_data_t *root, char *name, char *data, int copy)
{
	return api_add_data_full(root, name, 1 /*API_STRING*/, data, copy);
}

// === api_add_const === 原 FUN_00032e40 @ 0x00032e40
// 来源: reversed/cgminer/30_api_status/FUN_00032e40.c | 分类: 30_api_status
// 语义: 以 type=2 (API_CONST) 把条目加入 api_data 环链表 (一行包装)。
api_data_t *api_add_const(api_data_t *root, char *name, const char *data, int copy)
{
	return api_add_data_full(root, name, 2 /*API_CONST*/, (void *)data, copy);
}

// === api_add_int === 原 FUN_00032ed0 @ 0x00032ed0
// 来源: reversed/cgminer/30_api_status/FUN_00032ed0.c | 分类: 30_api_status
// 语义: 以 type=6 (API_INT) 把条目加入 api_data 环链表 (一行包装)。
api_data_t *api_add_int(api_data_t *root, char *name, int *data, int copy)
{
	return api_add_data_full(root, name, 6 /*API_INT*/, data, copy);
}

// === api_add_uint32 === 原 FUN_00032f18 @ 0x00032f18
// 来源: reversed/cgminer/30_api_status/FUN_00032f18.c | 分类: 30_api_status
// 语义: 以 type=8 (API_UINT32) 把条目加入 api_data 环链表 (一行包装)。
api_data_t *api_add_uint32(api_data_t *root, char *name, uint32_t *data, int copy)
{
	return api_add_data_full(root, name, 8 /*API_UINT32*/, data, copy);
}

// === api_add_elapsed === 原 FUN_00032fcc @ 0x00032fcc
// 来源: reversed/cgminer/30_api_status/FUN_00032fcc.c | 分类: 30_api_status
// 语义: 以 type=13 (API_ELAPSED, double 秒) 把条目加入 api_data 环链表 (一行包装)。
api_data_t *api_add_elapsed(api_data_t *root, char *name, double *data, int copy)
{
	return api_add_data_full(root, name, 0xd /*API_ELAPSED*/, data, copy);
}

// === api_add_timeval === 原 FUN_00033014 @ 0x00033014
// 来源: reversed/cgminer/30_api_status/FUN_00033014.c | 分类: 30_api_status
// 语义: 以 type=15 (API_TIMEVAL, 8 字节整拷贝) 把条目加入 api_data 环链表。
api_data_t *api_add_timeval(api_data_t *root, char *name, struct timeval *data, int copy)
{
	return api_add_data_full(root, name, 0xf /*API_TIMEVAL*/, data, copy);
}

// === api_add_time === 原 FUN_00033038 @ 0x00033038
// 来源: reversed/cgminer/30_api_status/FUN_00033038.c | 分类: 30_api_status
// 语义: 以 type=16 (API_TIME) 把条目加入 api_data 环链表 (本平台 32 位 time_t,
//       4 字节拷贝, 与 api_add_data_full 的 case 0x10 分支一致)。
api_data_t *api_add_time(api_data_t *root, char *name, time_t *data, int copy)
{
	return api_add_data_full(root, name, 0x10 /*API_TIME*/, data, copy);
}

// === api_add_mhs === 原 FUN_00034eb0 @ 0x00034eb0
// 来源: reversed/cgminer/30_api_status/FUN_00034eb0.c | 分类: 30_api_status
// 语义: 以 type=17 (API_MHS, double) 把条目加入 api_data 环链表; 被 lcd
//       (GHS5s/GHSavg) 与 itemstats (GHS 5s/GHS av) 使用。
api_data_t *api_add_mhs(api_data_t *root, char *name, double *data, int copy)
{
	return api_add_data_full(root, name, 0x11 /*API_MHS*/, data, copy);
}

/* ============================================================
 * 6. 命令与统计处理
 * ============================================================ */

// === asccount === 原 FUN_00033488 @ 0x00033488
// 来源: reversed/cgminer/30_api_status/FUN_00033488.c | 分类: 30_api_status
// 语义: API 命令 "ASCS" 处理: 回 MSG_NUMASC (0x68) 头, 随后文本模式输出
//       "ASCS," 前缀, JSON 模式输出 ,"ASCS":[ 并置 close 标志 (+0xD); 两种模式
//       都输出 Count 条目 (本固件恒 0, 无 ASC 设备) 并 print_data。
void asccount(void *io_data, int sock, int issocket, int isjson)
{
	int count = 0;                              /* 原码计数局部量: 恒 0 */
	int io_new;

	message(io_data, 0x68 /*MSG_NUMASC*/, 0, 0, isjson);
	if (isjson == 0) {
		io_add(io_data, "ASCS,");
		print_data(io_data, api_add_int(NULL, "Count", &count, 0), 0, 0);
	}
	else {
		io_new = io_add(io_data, ",\"ASCS\":[");
		print_data(io_data, api_add_int(NULL, "Count", &count, 0), 1, 0);
		if (io_new != 0) {
			*(uint8_t *)((char *)io_data + 0xd) = 1;   /* close 标志: 待 tail_join 闭合 */
		}
	}
}

// === dosave === 原 FUN_00034dc0 @ 0x00034dc0
// 来源: reversed/cgminer/30_api_status/FUN_00034dc0.c | 分类: 30_api_status
// 语义: API 命令 "SAVE" 处理: 文件名缺省时取 default_save_file() 填充 4096 字节
//       栈缓冲; fopen(w) 失败回 MSG_BADFN (0x2b), 成功 write_config 后回
//       MSG_SAVED (0x2c); 回复中的文件名经 escape_string, 非原串则释放。
void dosave(void *io_data, int sock, char *fn, int isjson)
{
	char filename[4096];                        /* 原码 4096 字节栈缓冲 */
	char msg[4100];
	char *esc;
	FILE *fp;

	if (fn == NULL || *fn == '\0') {
		default_save_file(filename);            /* 原反编译未显参数, 按被调签名 (char *buf) 还原 */
		fn = filename;
	}
	fp = fopen(fn, "w");
	if (fp == NULL) {
		esc = escape_string(fn, isjson);
		message(io_data, 0x2b /*MSG_BADFN*/, 0, (int)(intptr_t)esc, isjson);
		if (esc != fn) {
			free(esc);
		}
	}
	else {
		write_config(fp);
		fclose(fp);
		esc = escape_string(fn, isjson);
		message(io_data, 0x2c /*MSG_SAVED*/, 0, (int)(intptr_t)esc, isjson);
		if (esc != fn) {
			free(esc);
		}
	}
}

// === itemstats === 原 FUN_00035154 @ 0x00035154
// 来源: reversed/cgminer/30_api_status/FUN_00035154.c | 分类: 30_api_status
// 语义: STATS 单条统计条目: 依次入链 STATS(索引)/ID/Elapsed(全局运行秒数)/
//       Calls/Wait/Max/Min(统计结构 +0/+4/+0xC/+0x14)/GHS 5s(全局串缓冲)/
//       GHS av(= DAT_0007acc0/DAT_000755f8); 若传入 extra 链则整体接到环链表
//       尾 (prev/next 互指), 最后 print_data 输出 (JSON 且 i>0 时前置逗号,
//       反汇编 0x35250~0x3526c 证实), 返回 i+1。
int itemstats(void *io_data, int i, char *id, void *stats, api_data_t *extra, int isjson)
{
	int index = i;
	double ghs_av;
	api_data_t *root;

	ghs_av = DAT_0007acc0 / DAT_000755f8;       /* GHS av = 全局分子 / 运行秒数 */
	root = api_add_int(NULL, "STATS", &index, 0);
	root = api_add_string(root, "ID", id, 0);
	root = api_add_elapsed(root, "Elapsed", &DAT_000755f8, 0);
	root = api_add_uint32(root, "Calls", (uint32_t *)stats, 0);
	root = api_add_timeval(root, "Wait", (struct timeval *)((char *)stats + 4), 0);
	root = api_add_timeval(root, "Max", (struct timeval *)((char *)stats + 0xc), 0);
	root = api_add_timeval(root, "Min", (struct timeval *)((char *)stats + 0x14), 0);
	root = api_add_string(root, "GHS 5s", (char *)&DAT_00078b84, 0);
	root = api_add_mhs(root, "GHS av", &ghs_av, 0);
	if (extra != NULL) {
		if (root == NULL) {
			root = extra;
		}
		else {
			/* 环链表拼接 (+0x10=prev, +0x14=next; 原反汇编 0x3522c~0x3524c) */
			api_data_t *main_tail = root->prev;     /* 头节点 prev = 主链尾 */
			api_data_t *extra_tail = extra->prev;   /* extra 头的 prev = extra 链尾 */
			extra->prev = main_tail;
			main_tail->next = extra;
			extra_tail->next = root;
			root->prev = extra_tail;
		}
	}
	print_data(io_data, root, isjson, (isjson != 0 && index > 0) ? 1 : 0);
	return index + 1;
}

// === bm1740_makeup_get_status_cmd === 原 FUN_0003aebc @ 0x0003aebc
// 来源: reversed/cgminer/30_api_status/FUN_0003aebc.c | 分类: 30_api_status
// 语义: BM1740 状态查询构帧入口的参数错误分支: 打印 "%s input param error:
//       str length = 9" (applog 错误级) 并返回 -1 (0xffffffff)。
int bm1740_makeup_get_status_cmd(void)
{
	char msg[4100];

	snprintf(msg, 0x1000, "%s input param error: str length = %u\n", __func__, 9u);
	applog(3 /*LOG_ERR*/, msg, 0);
	return -1;                                  /* 原码 0xffffffff */
}

/* ============================================================
 * 7. jansson 底层 (静态链接 jansson 2.x)
 * ============================================================ */

// === json_loadb === 原 FUN_000592c4 @ 0x000592c4
// 来源: reversed/cgminer/30_api_status/FUN_000592c4.c | 分类: 30_api_status
// 语义: jansson json_loadb: 以 "<buffer>" 初始化错误结构; 缓冲为 NULL 时报
//       "wrong arguments"; 否则以流数据 {data(+0), len(+4), pos(+8)=0} (写序由
//       反编译证实) 初始化 lexer (68 字节, 布局不透明), reader 回调为
//       0x00057fb4 (jansson buffer_get, 命名待补); lex_init 成功则 parse_json
//       解析并 lex_cleanup 收尾, 失败返回 NULL。
json_t *json_loadb(const char *buffer, size_t size, uint32_t flags, json_error_t *error)
{
	uint32_t lex[17];                           /* jansson lexer 上下文, 68 字节 */
	uint32_t stream[3];                         /* buffer_get 流数据 */
	json_t *result;

	result = NULL;
	jsonp_error_init(error, "<buffer>");
	if (buffer == NULL) {
		error_set(error, 0, "wrong arguments");
		return NULL;
	}
	stream[0] = (uint32_t)(intptr_t)buffer;     /* data */
	stream[1] = (uint32_t)size;                 /* len */
	stream[2] = 0;                              /* pos */
	if (lex_init(lex, fun_00057fb4, stream) == 0) {
		result = parse_json(lex, flags, error);
		lex_cleanup(lex);
	}
	return result;
}

// === json_integer_value === 原 FUN_0005a248 @ 0x0005a248
// 来源: reversed/cgminer/30_api_status/FUN_0005a248.c | 分类: 30_api_status
// 语义: jansson json_integer_value: 对象为 NULL 或 type != 3 (JSON_INTEGER)
//       返回 0, 否则返回 +8 处的 64 位整数值 (json_int_t)。
int64_t json_integer_value(const json_t *json)
{
	if (json == NULL || json->type != 3 /*JSON_INTEGER*/) {
		return 0;
	}
	return *(int64_t *)((char *)json + 8);
}

// === json_real_value === 原 FUN_0005a2f8 @ 0x0005a2f8
// 来源: reversed/cgminer/30_api_status/FUN_0005a2f8.c | 分类: 30_api_status
// 语义: jansson json_real_value: 对象为 NULL 或 type != 4 (JSON_REAL) 返回 0,
//       否则返回 +8 处的 double (o32/软浮点 ABI 下经 r0:r1 成对返回)。
double json_real_value(const json_t *json)
{
	if (json == NULL || json->type != 4 /*JSON_REAL*/) {
		return 0;
	}
	return *(double *)((char *)json + 8);
}

/*
 * ================= 覆盖率表 (34/34) =================
 * 原 FUN 地址    新函数名                                  反编译行数
 * ------------------------------------------------------------
 * FUN_0000b774  _mutex_lock                               16
 * FUN_0000b7e0  api_rw_unlock                             16
 * FUN_0000b84c  _mutex_unlock                             16
 * FUN_000160b4  zero_bestshare                            29
 * FUN_000178f8  kill_work                                 14
 * FUN_00018924  inc_dev_status                            21
 * FUN_0002f174  thr_info_create                           10
 * FUN_000306b4  add_item_buf                              33
 * FUN_000307c8  io_add                                    36
 * FUN_0003085c  head_join                                 37
 * FUN_00031ca0  mcast_thread                              16
 * FUN_00031f64  tail_join                                 24
 * FUN_00032190  api_rd_lock                               20
 * FUN_0003220c  quit_restart_unlock                       15
 * FUN_00032248  api_restart_thread                        54
 * FUN_00032340  quit_thread                               51
 * FUN_0003243c  tidyup                                    44
 * FUN_00032df8  api_add_escape                             9
 * FUN_00032e1c  api_add_string                             9
 * FUN_00032e40  api_add_const                              9
 * FUN_00032ed0  api_add_int                                9
 * FUN_00032f18  api_add_uint32                             9
 * FUN_00032fcc  api_add_elapsed                            9
 * FUN_00033014  api_add_timeval                            9
 * FUN_00033038  api_add_time                               9
 * FUN_00033488  asccount                                  27
 * FUN_00034dc0  dosave                                    33
 * FUN_00034eb0  api_add_mhs                                9
 * FUN_00035154  itemstats                                 41
 * FUN_0003aebc  bm1740_makeup_get_status_cmd              13
 * FUN_000499d4  create_bitmain_check_miner_status_pthread 27
 * FUN_000592c4  json_loadb                                31
 * FUN_0005a248  json_integer_value                        11
 * FUN_0005a2f8  json_real_value                           11
 *
 * 主题分组: 锁错误上报 4 / 主流程回调 3 / 线程与生命周期 7 / io 缓冲与拼装 4 /
 *           api_add_* 包装 9 / 命令与统计 4 / jansson 3
 * 反编译总行数 727。
 *
 * 外部依赖: applog/quit (41_logging, 主流程); suffix_string/cg_completion_timeout/
 *           cgsleep_ms/_cgsem_init (20_job_mining/util); rename_thread/mcast/
 *           restart_mine/kill_work 相关 (util/主流程); mutex_lock_fail_cgminer/
 *           mutex_unlock_fail_cgminer (cgminer 主体锁上报); escape_string/
 *           api_add_data_full/message/print_data (30_api_status 其余批次);
 *           default_save_file/write_config (40_config); check_miner_status
 *           (bitmain 检查线程); jsonp_error_init/error_set/lex_init/parse_json/
 *           lex_cleanup (jansson B3/D 分片); fun_0001b4c0/fun_00057fb4
 *           (占位, 命名待补)。
 */
