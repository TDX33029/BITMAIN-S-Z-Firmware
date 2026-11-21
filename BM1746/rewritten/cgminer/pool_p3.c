// =============================================================================
// cgminer 矿池网络层 第 3 段 (pool_p3.c)
// 批次: pool_b3  |  分类: 21_pool_net  |  47 个函数 (分配见 rewritten/batch_plan.tsv)
// 命名事实源: rewritten/name_map/cgminer.tsv (阶段 1 冻结)
// 严格语义等价转写; 结构布局不可知处保留指针算术并注释字节偏移。
//
// 主题分组 (报告用):
//   [配置/URL/代理 12]  priority_pool detect_stratum remove_pool setup_url
//                       add_url set_quota extract_sockaddr get_proxy proxytype
//                       copyadvanceafter pooldetails switchpool
//   [池状态/调度 10]    adjust_quota_gcd pool_tset pool_tclear current_pool
//                       pool_died pool_died_silent cp_prio pool_resus
//                       stratum_resumed cnx_needed
//   [socket/stratum 11] clear_sockbuf socket_full block_socket recv_byte
//                       http_negotiate __stratum_send clear_sock sock_full
//                       suspend_stratum init_stratum_threads restart_stratum
//   [线程/队列/curses 4] input_thread tq_pop rename_thread wlogprint
//   [jansson 底层 10]   json_dump_callback json_dumps hashtable_find_pair
//                       hashtable_iter_at json_loads jsonp_strdup
//                       strbuffer_init strbuffer_close json_object_get
//                       __umodsi3
//
// 关键佐证 (ELF: analysis/cgminer_z15_bm1746.elf):
//   - 代理前缀表 @0x64e3c: {名称指针,类型} 对 6 项 + NULL 结束
//     (http:0 http0:1 socks4:2 socks5:3 socks4a:4 socks5h:5), proxytype/get_proxy 共用。
//   - jansson prime_list @0x6cb74: 29 项质数 (5..1610612741), num_buckets 为其索引。
//   - setup_url 的栈构串字面量为 "Failed to malloc httpinput" (rodata @0x58960)。
//   - pair_t 布局: hash@0 list@4 value@0xc serial@0x10 key@0x14 (与 jansson 2.4 一致)。
// =============================================================================
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>      /* snprintf/strncpy/strncat/strncmp/strcmp */
#include <stdlib.h>     /* malloc/free/strtol/strdup? (strdup 在 string.h) */
#include <string.h>     /* strdup/memcpy/strlen/strchr/strstr */
#include <strings.h>    /* strncasecmp */
#include <errno.h>      /* errno (原码 __errno_location() 显式读写, 等价) */
#include <stdbool.h>
#include <pthread.h>
#include <sys/types.h>  /* ssize_t */
#include <sys/time.h>   /* struct timeval */
#include <sys/select.h> /* fd_set/select */
#include <time.h>       /* struct timespec */

/* ================= struct pool 关键偏移 (全部反编译核实, 32 位 ARM) =================
 * +0x00 pool_no   +0x04 prio      +0x1c 统计字段 (pool_resus 清零, 语义待定)
 * +0x38 quota     +0x3c quota_gcd(=quota/gcd)  +0x40 quota_used(轮转累计)
 * +0x81 idle      +0x84 enabled (POOL_ENABLED=1)  +0x89 removed
 * +0xa8 tv_idle (struct timeval)  +0xc4 rpc_url  +0xcc/0xd0 rpc_user/rpc_pass
 * +0xd4 rpc_proxytype  +0xd8 rpc_proxy (代理 URL 串, get_proxy 填)
 * +0xdc pool_lock (pool_tset/pool_tclear)
 * +0x25c extranonce_subscribe (parse_xnsub 置位)  +0x260 sockaddr_port
 * +0x264 sock     +0x268 sockbuf  +0x270 sockaddr_url
 * +0x274/0x278 代理 sockaddr url/port (get_proxy 经 extract_sockaddr 填)
 * +0x298 has_stratum  +0x299 stratum_active  +0x29b stratum 标志 (语义待定)
 * +0x2d8 stratum_lock  +0x2f4 连接需求相关计数 (语义待定)
 * +0x208/0x20c 行计数 64 位低/高字   +0x210/+0x218 两个 64 位发送字节累计
 */

/* ================= 外部函数 (定义在其余分段, 名取 name_map) ================= */

extern void   applog(uint32_t prio, char *buf, ...);            /* 00038730 (第 3 参按反编译保留) */
extern void   quit(int status);                                 /* 00016ca8, 不返回 */
extern void   parse_xnsub(int *pool, char *url);                /* 0002aa68 */
extern void  *add_pool(void);                                   /* 00017a3c (pool_p2; 返回值本调用点未用) */
extern void   switch_pools(void *preferred);                    /* 00019bf8 (NULL=自动选择) */
extern void   cgtime(struct timeval *tv);                       /* 0002a84c */
extern int    initiate_stratum(void *pool);                     /* 0002b8e8 */
extern int    extranonce_subscribe(void *pool);                 /* 0002e69c */
extern int    auth_stratum(void *pool);                         /* 0002ea38 (json.c) */
extern char *  opt_set_charp(const char *arg, char **ptr);        /* 0005b1d4 (cfg.c; 恒返回 NULL=无错) */
extern void   kill_work(void);                                  /* 000178f8 */
extern void   display_options(void);                            /* 000260fc */
extern void   set_options(void);                                /* 000267b0 */
extern void   unlock_curses(void);                              /* 00019adc */
extern void   display_pools(void);                              /* 00026f50 (pool_b1) */
extern void   message(void *io_data, int code, int p0, int p1); /* 0003305c (api.c codes 表) */

/* 锁失败处理包装 (logstring 类) */
extern void   mutex_lock_fail_cgminer(const char *caller, int line);    /* 0000b414 */
extern void   mutex_unlock_fail_cgminer(const char *caller, int line);  /* 0000b5c4 */
extern void   rd_lock_error(const char *caller, int line);              /* 0000b69c */
extern void   rw_unlock_error(const char *caller, int line);            /* 0000b630 */
extern void   mutex_lock_fail_util(const char *caller, int line);       /* 0000b708 */
extern void   mutex_unlock_util(pthread_mutex_t *lock, const char *caller, int line); /* 00029388 */
extern void   _mutex_lock(pthread_mutex_t *lock, const char *caller, int line);    /* 0000b774 (api.c) */
extern void   _mutex_unlock(pthread_mutex_t *lock, const char *caller, int line);  /* 0000b84c (api.c) */
extern void   api_rd_lock(pthread_rwlock_t *lock, const char *caller, int line);   /* 00032190 (api.c) */
extern void   api_rw_unlock(pthread_rwlock_t *lock, const char *caller, int line); /* 0000b7e0 (api.c) */

/* ---- jansson 内部 (0005xxxx, 布局经反编译偏移核实) ---- */
typedef struct json_t { int type; int refcount; } json_t;       /* 头两字段, 见 json.c */

typedef struct strbuffer_t {
	char  *value;                                               /* +0x00 */
	size_t length;                                              /* +0x04 */
	size_t size;                                                /* +0x08 */
} strbuffer_t;

typedef struct hashtable_list {
	struct hashtable_list *prev;                                /* +0x00 */
	struct hashtable_list *next;                                /* +0x04 */
} hashtable_list_t;

typedef struct hashtable_pair {
	size_t hash;                                                /* +0x00 */
	hashtable_list_t list;                                      /* +0x04 */
	json_t *value;                                              /* +0x0c */
	size_t serial;                                              /* +0x10 */
	char key[1];                                                /* +0x14 (变长尾随) */
} hashtable_pair_t;

typedef struct hashtable_bucket {
	hashtable_list_t *first;                                    /* +0x00 */
	hashtable_list_t *last;                                     /* +0x04 */
} hashtable_bucket_t;

typedef struct hashtable_t {
	size_t size;                                                /* +0x00 */
	hashtable_bucket_t *buckets;                                /* +0x04 */
	size_t num_buckets;                                         /* +0x08: prime_list 索引 */
	hashtable_list_t list;                                      /* +0x0c: 内嵌哨兵链节点 */
} hashtable_t;

/* json_error_t 252 字节: line@0 column@4 position@8 source[80]@0xc text[160]@0x5c (json.c 核实) */
typedef struct json_error_t { char pad[252]; } json_error_t;

extern int    do_dump(json_t *json, size_t flags, int indent,
                      int (*dump)(const char *, size_t, void *), void *data); /* 00057178 */
extern int    strbuffer_value(strbuffer_t *strbuff);            /* 00059694 */
extern void  *jsonp_malloc(size_t size);                        /* 0005956c */
extern void   jsonp_free(void *ptr);                            /* 00059594 */
extern void   jsonp_error_init(json_error_t *error, const char *source);  /* 0005b014 */
extern void   error_set(json_error_t *error, void *lex, const char *msg); /* 00058034 */
extern int    lex_init(void *lex, void *get, void *data);       /* 00058154 (本版 3 参形态, 两次调用点一致) */
extern json_t *parse_json(void *lex, size_t flags, json_error_t *error);  /* 00059154 */
extern void   lex_cleanup(void *lex);                           /* 000581a0 */
extern int    fun_000570bc(const char *buf, size_t size, void *data); /* dump_to_strbuffer (原地址 0x000570bc) 命名待补 */
extern int    fun_00057f94(void *lex);                          /* string_get (原地址 0x00057f94) 命名待补 */
extern uint32_t __udivsi3(uint32_t dividend, uint32_t divisor); /* 0005c648 (商在 r0; 余 r1) */

/* ---- curses ---- */
extern int    wprintw(void *win, const char *fmt, ...);
extern int    wgetch(void *win);
extern void  *stdscr;                                           /* 主屏 */

/* ---- 系统调用 (按反编译原形声明) ---- */
extern int    fcntl(int fd, int cmd, ...);
extern int    close(int fd);
extern ssize_t recv(int fd, void *buf, size_t n, int flags);
extern ssize_t send(int fd, const void *buf, size_t n, int flags);
extern int    select(int nfds, fd_set *rdevents, fd_set *wdevents, fd_set *exevents, struct timeval *tv);
extern int    raise(int sig);
extern int    prctl(int option, ...);

/* ================= 外部全局 (DAT_ 原名保留, 注释为已核实语义) ================= */

extern int      DAT_0007b198;     /* total_pools: 已配置矿池数 (axi.c) */
extern void   **DAT_0007b438;     /* pools: 矿池指针数组 (axi.c/freq_p2) */
extern int      DAT_00077014;     /* add_url 游标: 1-based 待配置矿池序号 */
extern void    *DAT_0007705c;     /* 当前池指针 (current_pool/cp_prio 读) */
extern int      DAT_000755f0;     /* 全局 quota gcd 运行值 (adjust_quota_gcd 维护) */
extern int      DAT_00076efc;     /* 挖矿策略: 0=Failover 1=RoundRobin 2=Rotate 3=LoadBalance 4=Balance */
extern int      DAT_0007b4c8;     /* cnx_needed 兜底返回布尔 (pool_p1 注, 语义待定) */
extern uint8_t  DAT_00077018;     /* curses_active 标志 (fan.c) */
extern uint8_t  DAT_0007bb18;     /* 静默退出标志 (log.c: curses 's' 置位) */
extern void    *DAT_0007b430;     /* curses 日志窗口 logwin (pool_p1/cfg.c) */
extern void   (*DAT_00075ed8)(void);      /* noop_hook 函数指针 (util.c, 尾部惯例调用) */
extern void  *(*DAT_00076ba0)(size_t);   /* jansson 分配器函数指针 (jsonp_strdup 经此分配)
                                          * [存证互引] freq_p4.c jsonp_malloc (FUN_0005956c) 同经此指针分配,
                                          * 与其 DAT_00076ba0/00076ba4 分配钩子声明互为存证 */
extern uint32_t DAT_0006cb74[];   /* jansson prime_list @0x6cb74 (ELF 核实: 29 项 5..1610612741) */
extern uint8_t  DAT_00078e09;     /* 日志守卫: 外层总开关 */
extern uint8_t  DAT_0007ad48;     /* 日志守卫 A */
extern uint8_t  DAT_00078e08;     /* 日志守卫 B */
extern int      DAT_000766c4;     /* 日志守卫: 级别阈值 (值越大越详细) */
extern pthread_mutex_t  DAT_0007ba10; /* 池管理全局互斥锁 */
extern pthread_rwlock_t DAT_0007ba28; /* 池管理全局读写锁 */

/* 反编译统一日志门控: 调用点形态 (A||B||N<C) ↔ applog(N+1);
 * LOG_DEBUG(7) 调用点额外带外层总开关 DAT_00078e09。 */
#define GATE_E3   (DAT_0007ad48 != 0 || DAT_00078e08 != 0 || 2 < DAT_000766c4)  /* applog(3)=LOG_ERR */
#define GATE_W4   (DAT_0007ad48 != 0 || DAT_00078e08 != 0 || 3 < DAT_000766c4)  /* applog(4)=LOG_WARNING */
#define GATE_DBG7 (DAT_00078e09 != 0 && (DAT_0007ad48 != 0 || DAT_00078e08 != 0 || 6 < DAT_000766c4))  /* applog(7)=LOG_DEBUG */

/* ================= 模块内前向声明 (本文件后段定义) ================= */

int   extract_sockaddr(char *url, char **sockaddr_url, char **sockaddr_port);   /* 0002ab54 */
char *get_proxy(char *url, void *pool);                                         /* 0002ace0 */
void  rename_thread(char *name);                                                /* 0002f0b4 */
void  clear_sockbuf(void *pool);                                                /* 00028ae8 */
bool  socket_full(void *pool, unsigned int wait);                               /* 00028afc */
uint32_t __umodsi3(uint32_t dividend, uint32_t divisor);                        /* 0005c6f0 */
char *jsonp_strdup(const char *str);                                            /* 000595b4 */
int   strbuffer_init(strbuffer_t *strbuff);                                     /* 0005961c */
void  strbuffer_close(strbuffer_t *strbuff);                                    /* 00059654 */

/* ================= 代理前缀表 ================= */
/* @0x64e3c (ELF 数据核实): {名称指针, 类型} 对, 名称 NULL 结束。
 * proxytype()/get_proxy() 共用; proxytype(0) 快路径返回首项 "http:"。 */
static const struct proxy_name {
	const char *name;
	int         type;
} DAT_00064e3c[] = {                                            /* 语义: ELF @0x64e3c 原表 */
	{ "http:",   0 },
	{ "http0:",  1 },
	{ "socks4:", 2 },
	{ "socks5:", 3 },
	{ "socks4a:", 4 },
	{ "socks5h:", 5 },
	{ NULL,      0 },
};

// === priority_pool === 原 FUN_00012038 @ 0x00012038
// 来源: reversed/cgminer/21_pool_net/FUN_00012038.c | 分类: 21_pool_net
// 语义: 按 pool->prio (+0x04) 查找目标矿池; 池表为空或找不到时告警
//       "WTF No pool %d found!" 并越界返回 pools[pool_no] (原样兜底)。
void *priority_pool(int pool_no)
{
	void  *pool;
	int    i;
	char   buf[4096];

	if (DAT_0007b198 >= 1) {
		for (i = 0; i < DAT_0007b198; i++) {
			pool = DAT_0007b438[i];
			if (*(int *)((char *)pool + 4) == pool_no)      /* pool->prio */
				return pool;
		}
	}
	if (GATE_E3) {
		snprintf(buf, 0x1000, "WTF No pool %d found!", pool_no);
		applog(3, buf, 0);
	}
	return DAT_0007b438[pool_no];                           /* 原样: 未命中时按下标直取 */
}

// === adjust_quota_gcd === 原 FUN_000152e0 @ 0x000152e0
// 来源: reversed/cgminer/21_pool_net/FUN_000152e0.c | 分类: 21_pool_net
// 语义: 取各池非零 quota (+0x38) 的最小值向下试除求最大公约数 (整除全部非零
//       quota), 写全局 DAT_000755f0; 按新旧 gcd 重标定 quota_used (+0x40 =
//       旧值×旧gcd/新gcd) 并回写 quota_gcd (+0x3c = quota/新gcd)。DEBUG 日志。
void adjust_quota_gcd(void)
{
	void   **pool_p;
	void   **pools_end;
	uint32_t gcd;
	uint32_t quota;
	uint32_t old_gcd;
	int      count;
	int      i;
	char     buf[4096];

	count = DAT_0007b198;
	if (count < 1) {
		gcd = 1;
	} else {
		gcd = 0xffffffff;
		pools_end = DAT_0007b438 + count;
		for (pool_p = DAT_0007b438; pool_p != pools_end; pool_p++) {
			quota = *(uint32_t *)((char *)*pool_p + 0x38);      /* pool->quota */
			if (quota != 0 && quota <= gcd)
				gcd = quota;
		}
		if (gcd == 0xffffffff) {
			gcd = 1;                                            /* 全部 quota 为 0 */
		} else {
			/* 试除: 对每个非零 quota 循环 gcd-- 直到整除 (0x00015360 前段) */
			for (pool_p = DAT_0007b438; pool_p != pools_end; pool_p++) {
				quota = *(uint32_t *)((char *)*pool_p + 0x38);
				if (quota == 0)
					continue;
				while (__umodsi3(quota, gcd) != 0)              /* 余数在 r1 (0005c6f0) */
					gcd--;
			}
		}
		/* 重标定 (0x00015360): 所有池无差别处理 */
		old_gcd = DAT_000755f0;                                 /* 旧全局 gcd */
		pool_p = DAT_0007b438;
		i = 0;
		do {
			uint32_t *pool = (uint32_t *)*pool_p;
			/* +0x40 quota_used = quota_used * 旧gcd / 新gcd (0005c648 商 r0) */
			*(uint32_t *)((char *)pool + 0x40) =
			        __udivsi3(*(uint32_t *)((char *)pool + 0x40) * old_gcd, gcd);
			/* +0x3c quota_gcd = quota / 新gcd */
			*(uint32_t *)((char *)pool + 0x3c) =
			        __udivsi3(*(uint32_t *)((char *)pool + 0x38), gcd);
			pool_p++;
			i++;
		} while (i < count);
	}
	DAT_000755f0 = gcd;
	if (GATE_DBG7) {
		snprintf(buf, 0x1000, "Global quota greatest common denominator set to %lu",
		         (unsigned long)gcd);
		applog(7, buf, 0);
	}
}

// === detect_stratum === 原 FUN_0001557c @ 0x0001557c
// 来源: reversed/cgminer/21_pool_net/FUN_0001557c.c | 分类: 21_pool_net
// 语义: 先 parse_xnsub(pool,url) 处理 "#xnsub" 后缀 (Ghidra 显示无参, r0/r1 即
//       pool/url), 再 extract_sockaddr 解析 sockaddr (+0x270/+0x260); URL 前缀
//       stratum+tcp:// 命中则置 has_stratum (+0x298)=1、rpc_url (+0xc4)=strdup,
//       返回 1; 否则 0。
int detect_stratum(int *pool, char *url)
{
	int ret;

	parse_xnsub(pool, url);
	ret = extract_sockaddr(url, (char **)((char *)pool + 0x270), (char **)((char *)pool + 0x260));
	if (ret == 0 || strncasecmp(url, "stratum+tcp://", 0xe) != 0)
		return 0;
	*(uint8_t *)((char *)pool + 0x298) = 1;                 /* pool->has_stratum */
	*(char **)((char *)pool + 0xc4) = strdup(url);          /* pool->rpc_url */
	return 1;
}

// === wlogprint === 原 FUN_00015890 @ 0x00015890
// 来源: reversed/cgminer/21_pool_net/FUN_00015890.c | 分类: 21_pool_net
// 语义: curses 日志窗口 (DAT_0007b430 logwin) 原样打印一行字符串。
void wlogprint(const char *str)
{
	wprintw(DAT_0007b430, "%s", str);
}

// === remove_pool === 原 FUN_00015ac8 @ 0x00015ac8
// 来源: reversed/cgminer/21_pool_net/FUN_00015ac8.c | 分类: 21_pool_net
// 语义: 删除矿池: 所有 prio 高于本池的池 prio-- 重新压实; 若本池不在末位则与
//       尾池交换槽位 (尾池接管本池 pool_no); 本池 pool_no = 递减前的
//       total_pools, removed (+0x89)=1, total_pools--。
void remove_pool(int *pool)
{
	int    i;
	int    count;       /* total_pools 进入时快照 */
	int    last;        /* 尾池下标 */
	int   *other;

	count = DAT_0007b198;
	last = count - 1;
	if (0 < count) {
		for (i = 0; i < count; i++) {
			other = (int *)DAT_0007b438[i];
			if (pool[1] < other[1])                         /* other->prio > pool->prio */
				other[1]--;
		}
	}
	if (*pool < last) {
		*(int *)DAT_0007b438[last] = *pool;                 /* 尾池接管本池序号 */
		DAT_0007b438[*pool] = DAT_0007b438[last];           /* 尾池换入本池槽位 */
	}
	*pool = count;                                          /* pool_no = 递减前的 total_pools */
	*(uint8_t *)((char *)pool + 0x89) = 1;                  /* pool->removed */
	DAT_0007b198 = count - 1;
}

// === setup_url === 原 FUN_00017100 @ 0x00017100
// 来源: reversed/cgminer/21_pool_net/FUN_00017100.c | 分类: 21_pool_net
// 语义: 配置矿池 URL: 先 get_proxy 剥代理前缀 (返回推进后的 URL); 未探测到
//       stratum 则 opt_set_charp 写 rpc_url (+0xc4); 非 http(s) 前缀时拼
//       "stratum+tcp://" (0x100 缓冲, 上限 0xf2) 再探测一次。
void setup_url(int *pool, char *url)
{
	char *str;
	char *dest;

	str = get_proxy(url, pool);
	if (detect_stratum(pool, str) == 0) {
		opt_set_charp(str, (char **)((char *)pool + 0xc4)); /* pool->rpc_url */
		if (strncmp(str, "http://", 7) != 0 &&
		    strncmp(str, "https://", 8) != 0) {
			dest = malloc(0x100);
			if (dest == NULL) {
				/* 字面量 ELF rodata @0x58960: "Failed to malloc httpinput" */
				applog(3, "Failed to malloc httpinput", 1);
				quit(1);
			}
			strncpy(dest, "stratum+tcp://", 0xf);
			strncat(dest, str, 0xf2);
			detect_stratum(pool, dest);
		}
	}
}

// === add_url === 原 FUN_00017db8 @ 0x00017db8
// 来源: reversed/cgminer/21_pool_net/FUN_00017db8.c | 分类: 21_pool_net
// 语义: 取下一个待配置矿池: 游标 DAT_00077014 (1-based) 递增, 池数不足时调
//       add_pool (00017a3c, pool_b2) 扩充, 返回 pools[游标-1]。
void *add_url(void)
{
	DAT_00077014 = DAT_00077014 + 1;
	if (DAT_0007b198 < DAT_00077014)
		add_pool();
	return DAT_0007b438[DAT_00077014 - 1];
}

// === set_quota === 原 FUN_00017e20 @ 0x00017e20
// 来源: reversed/cgminer/21_pool_net/FUN_00017e20.c | 分类: 21_pool_net
// 语义: 解析 "quota;URL" 参数: ';' 拆分, 前段 strtol 为 quota (负数报错),
//       后段长度不足报错; add_url 取池、setup_url 设 URL, 写 quota (+0x38),
//       DEBUG 日志后 adjust_quota_gcd。成功返回 NULL, 否则返回错误消息串。
char *set_quota(char *arg)
{
	char   *ret;
	char   *semicolon;
	size_t  total_len;      /* 原串长度 */
	size_t  quota_len;      /* ';' 前段长度 */
	int     quota;
	void   *pool;
	char    buf[4096];

	semicolon = strchr(arg, ';');
	if (semicolon == NULL)
		return "No semicolon separated quota;URL pair found";
	total_len = strlen(arg);
	*semicolon = '\0';
	quota_len = strlen(arg);
	if (quota_len == 0)
		return "No parameter for quota found";
	if ((int)(total_len - quota_len - 1) < 1)               /* 反编译 ~quota_len+total_len */
		return "No parameter for URL found";
	quota = (int)strtol(arg, NULL, 10);
	if (quota < 0)
		return "Invalid negative parameter for quota set";
	pool = add_url();                                       /* 反编译 r0 带 quota, add_url 未读取 */
	setup_url(pool, arg + quota_len + 1);
	*(int *)((char *)pool + 0x38) = quota;                  /* pool->quota */
	if (GATE_DBG7) {
		snprintf(buf, 0x1000, "Setting pool %d to quota %d", *(int *)pool, quota);
		applog(7, buf, 0);
	}
	adjust_quota_gcd();
	return NULL;
}

// === pool_tset === 原 FUN_00018834 @ 0x00018834
// 来源: reversed/cgminer/21_pool_net/FUN_00018834.c | 分类: 21_pool_net
// 语义: 在 pool->pool_lock (+0xdc) 保护下置 *flag = 1 并返回旧值; 锁失败走
//       0000b414/0000b5c4 报错包装; 尾部惯例调 noop_hook。
uint8_t pool_tset(int *pool, uint8_t *flag)
{
	uint8_t old;
	int     ret;

	ret = pthread_mutex_lock((pthread_mutex_t *)((char *)pool + 0xdc));
	if (ret != 0)
		mutex_lock_fail_cgminer("pool_tset", 0x304);
	old = *flag;
	*flag = 1;
	ret = pthread_mutex_unlock((pthread_mutex_t *)((char *)pool + 0xdc));
	if (ret != 0)
		mutex_unlock_fail_cgminer("pool_tset", 0x307);
	(*DAT_00075ed8)();
	return old;
}

// === pool_tclear === 原 FUN_00018c2c @ 0x00018c2c
// 来源: reversed/cgminer/21_pool_net/FUN_00018c2c.c | 分类: 21_pool_net
// 语义: pool_tset 的清除版: 持 pool_lock (+0xdc) 置 *flag = 0 并返回旧值。
uint8_t pool_tclear(int *pool, uint8_t *flag)
{
	uint8_t old;
	int     ret;

	ret = pthread_mutex_lock((pthread_mutex_t *)((char *)pool + 0xdc));
	if (ret != 0)
		mutex_lock_fail_cgminer("pool_tclear", 0x310);
	old = *flag;
	*flag = 0;
	ret = pthread_mutex_unlock((pthread_mutex_t *)((char *)pool + 0xdc));
	if (ret != 0)
		mutex_unlock_fail_cgminer("pool_tclear", 0x313);
	(*DAT_00075ed8)();
	return old;
}

// === current_pool === 原 FUN_0001da58 @ 0x0001da58
// 来源: reversed/cgminer/21_pool_net/FUN_0001da58.c | 分类: 21_pool_net
// 语义: 持全局互斥锁 DAT_0007ba10 + 读锁 DAT_0007ba28 读当前池指针 DAT_0007705c。
void *current_pool(void)
{
	void *pool;
	int   ret;

	ret = pthread_mutex_lock(&DAT_0007ba10);
	if (ret != 0)
		mutex_lock_fail_cgminer("current_pool", 0x31c);
	ret = pthread_rwlock_rdlock(&DAT_0007ba28);
	if (ret != 0)
		rd_lock_error("current_pool", 0x31c);
	ret = pthread_mutex_unlock(&DAT_0007ba10);
	if (ret != 0)
		mutex_unlock_fail_cgminer("current_pool", 0x31c);
	pool = DAT_0007705c;
	ret = pthread_rwlock_unlock(&DAT_0007ba28);
	if (ret != 0)
		rw_unlock_error("current_pool", 0x31e);
	(*DAT_00075ed8)();
	return pool;
}

// === pool_died === 原 FUN_0001fa14 @ 0x0001fa14
// 来源: reversed/cgminer/21_pool_net/FUN_0001fa14.c | 分类: 21_pool_net
// 语义: 池失联处理: 首次置 idle (+0x81) 并记 tv_idle (+0xa8); 若为当前池则
//       WARNING "not responding!" 并 switch_pools(NULL), 否则 DEBUG 日志。
void pool_died(int *pool)
{
	void *cur;
	char  buf[4096];

	if (pool_tset(pool, (uint8_t *)((char *)pool + 0x81)) == 0) {   /* pool->idle */
		cgtime((struct timeval *)((char *)pool + 0xa8));            /* pool->tv_idle */
		cur = current_pool();
		if (cur == pool) {
			if (GATE_W4) {
				snprintf(buf, 0x1000, "Pool %d %s not responding!",
				         *(int *)cur, *(char **)((char *)cur + 0xc4));
				applog(4, buf, 0);
			}
			switch_pools(NULL);
		} else if (GATE_DBG7) {
			snprintf(buf, 0x1000, "Pool %d %s failed to return work",
			         *pool, *(char **)((char *)pool + 0xc4));
			applog(7, buf, 0);
		}
	}
}

// === pool_died_silent === 原 FUN_0001fb44 @ 0x0001fb44
// 来源: reversed/cgminer/21_pool_net/FUN_0001fb44.c | 分类: 21_pool_net
// 语义: pool_died 的静默版: 仅置 idle + 记 tv_idle; 若为当前池则
//       switch_pools(NULL), 无任何日志 (Ghidra 显示的 64 位实参为寄存器噪音)。
void pool_died_silent(int *pool)
{
	if (pool_tset(pool, (uint8_t *)((char *)pool + 0x81)) == 0) {
		cgtime((struct timeval *)((char *)pool + 0xa8));
		if ((int *)current_pool() == pool)
			switch_pools(NULL);
	}
}

// === cp_prio === 原 FUN_0001fb7c @ 0x0001fb7c
// 来源: reversed/cgminer/21_pool_net/FUN_0001fb7c.c | 分类: 21_pool_net
// 语义: 持全局锁读当前池的 prio (+0x04)。
int cp_prio(void)
{
	int prio;
	int ret;

	ret = pthread_mutex_lock(&DAT_0007ba10);
	if (ret != 0)
		mutex_lock_fail_cgminer("cp_prio", 0x1b7c);
	ret = pthread_rwlock_rdlock(&DAT_0007ba28);
	if (ret != 0)
		rd_lock_error("cp_prio", 0x1b7c);
	ret = pthread_mutex_unlock(&DAT_0007ba10);
	if (ret != 0)
		mutex_unlock_fail_cgminer("cp_prio", 0x1b7c);
	prio = *(int *)((char *)DAT_0007705c + 4);
	ret = pthread_rwlock_unlock(&DAT_0007ba28);
	if (ret != 0)
		rw_unlock_error("cp_prio", 0x1b7e);
	(*DAT_00075ed8)();
	return prio;
}

// === pool_resus === 原 FUN_0001fc34 @ 0x0001fc34
// 来源: reversed/cgminer/21_pool_net/FUN_0001fc34.c | 分类: 21_pool_net
// 语义: 池复活处理: 清统计字段 (+0x1c); Failover 策略且 prio 低于当前池时
//       WARNING "alive, testing stability", 否则 DEBUG "alive"。
void pool_resus(int *pool)
{
	int strategy = DAT_00076efc;
	int prio;
	char buf[4096];

	*(int *)((char *)pool + 0x1c) = 0;                      /* 统计字段清零 (语义待定) */
	if (strategy == 0 && (prio = pool[1], prio < cp_prio())) {
		if (GATE_W4) {
			snprintf(buf, 0x1000, "Pool %d %s alive, testing stability",
			         *pool, *(char **)((char *)pool + 0xc4));
			applog(4, buf, 0);
		}
	} else if (GATE_DBG7) {
		snprintf(buf, 0x1000, "Pool %d %s alive",
		         *pool, *(char **)((char *)pool + 0xc4));
		applog(7, buf, 0);
	}
}

// === stratum_resumed === 原 FUN_0001fd64 @ 0x0001fd64
// 来源: reversed/cgminer/21_pool_net/FUN_0001fd64.c | 分类: 21_pool_net
// 语义: stratum 恢复处理: 清 stratum_active (+0x81); 原为激活态 (清除返回 1)
//       则 DEBUG 日志 "resumed" 并调 pool_resus。
void stratum_resumed(int *pool)
{
	char buf[4096];

	if (pool_tclear(pool, (uint8_t *)((char *)pool + 0x81)) != 0) {
		if (GATE_DBG7) {
			snprintf(buf, 0x1000, "Stratum connection to pool %d resumed", *pool);
			applog(7, buf, 0);
		}
		pool_resus(pool);
	}
}

// === cnx_needed === 原 FUN_0001ffec @ 0x0001ffec
// 来源: reversed/cgminer/21_pool_net/FUN_0001ffec.c | 分类: 21_pool_net
// 语义: 判断是否仍需保持该池连接: 仅 enabled (+0x84)==1 的池参与; 命中
//       Balance/LoadBalance 策略、has_stratum 且 stratum_active、当前池、
//       +0x2f4 计数非零、或 Failover 下 prio 低于当前池 → 需要(1);
//       否则返回兜底布尔 DAT_0007b4c8。
int cnx_needed(int *pool)
{
	int prio;

	if (*(int *)((char *)pool + 0x84) != 1)                 /* pool->enabled != POOL_ENABLED */
		return 0;
	if (DAT_00076efc == 4 || DAT_00076efc == 3)             /* Balance / LoadBalance */
		return 1;
	if (*(uint8_t *)((char *)pool + 0x298) != 0 &&          /* pool->has_stratum */
	    *(uint8_t *)((char *)pool + 0x81) != 0)             /* pool->stratum_active */
		return 1;
	if ((int *)current_pool() == pool ||
	    *(int *)((char *)pool + 0x2f4) != 0)                /* 连接需求计数 (语义待定) */
		return 1;
	if (DAT_00076efc == 0 && (prio = pool[1], prio < cp_prio()))   /* Failover 且非最高优先级 */
		return 1;
	return DAT_0007b4c8;
}

// === input_thread === 原 FUN_00027a98 @ 0x00027a98
// 来源: reversed/cgminer/21_pool_net/FUN_00027a98.c | 分类: 21_pool_net
// 语义: curses 输入线程: 异步可取消 (PTHREAD_CANCEL_ASYNCHRONOUS=1), 命名
//       "Input"; curses 激活 (DAT_00077018) 时循环读键: q=kill_work 退出,
//       d=display_options (DAT_0007bb18 置位时跳出), p=display_pools,
//       s=set_options; 退出前 unlock_curses。
void *input_thread(void *unused)
{
	char ch;

	pthread_setcanceltype(1 /*PTHREAD_CANCEL_ASYNCHRONOUS*/, NULL);
	rename_thread("Input");                                 /* 0002f0b4: "cg@%s" + prctl */

	if (DAT_00077018 != 0) {
		do {
			for (;;) {
				ch = (char)wgetch(stdscr);
				if (strncasecmp(&ch, "q", 1) == 0) {
					kill_work();
					return 0;
				}
				if (strncasecmp(&ch, "d", 1) != 0)
					break;
				display_options();
				if (DAT_0007bb18 != 0)
					goto out;                           /* 原标签 0x00027b58 */
			}
			if (strncasecmp(&ch, "p", 1) == 0)
				display_pools();
			else if (strncasecmp(&ch, "s", 1) == 0)
				set_options();
		} while (DAT_0007bb18 == 0);
out:
		unlock_curses();
	}
	return 0;
}

// === clear_sockbuf === 原 FUN_00028ae8 @ 0x00028ae8
// 来源: reversed/cgminer/21_pool_net/FUN_00028ae8.c | 分类: 21_pool_net
// 语义: 清空池接收缓冲: sockbuf (+0x268) 非空则 sockbuf[0] = '\0'。
void clear_sockbuf(void *pool)
{
	char **sockbuf = (char **)((char *)pool + 0x268);

	if (*sockbuf != NULL)
		**sockbuf = '\0';
}

// === socket_full === 原 FUN_00028afc @ 0x00028afc
// 来源: reversed/cgminer/21_pool_net/FUN_00028afc.c | 分类: 21_pool_net
// 语义: select 读就绪探测: 等待 wait 秒 (wait<0 视为 0, 原式
//       wait & ~(int)wait>>31), 池 sock (+0x264) 可读返回 true。
bool socket_full(void *pool, unsigned int wait)
{
	fd_set rdevents;
	struct timeval tv;
	int sock = *(int *)((char *)pool + 0x264);
	int ret;

	tv.tv_sec = wait & (~(int)wait >> 31);                  /* wait<0 → 0 */
	tv.tv_usec = 0;
	FD_ZERO(&rdevents);                                     /* 反编译为 32 字循环清零, 等价 */
	FD_SET(sock, &rdevents);
	ret = select(sock + 1, &rdevents, NULL, NULL, &tv);
	return ret > 0;
}

// === block_socket === 原 FUN_00028b9c @ 0x00028b9c
// 来源: reversed/cgminer/21_pool_net/FUN_00028b9c.c | 分类: 21_pool_net
// 语义: 清 O_NONBLOCK (0x800) 恢复 sock 阻塞模式。
void block_socket(int sock)
{
	int flags = fcntl(sock, 3 /*F_GETFL*/, 0);

	fcntl(sock, 4 /*F_SETFL*/, flags & 0xfffff7ff);
}

// === recv_byte === 原 FUN_00028bc4 @ 0x00028bc4
// 来源: reversed/cgminer/21_pool_net/FUN_00028bc4.c | 分类: 21_pool_net
// 语义: 从 sock 收 1 字节; 失败原样返回 -1 (0xffffffff)。
int recv_byte(int sock)
{
	unsigned char buf;
	int ret = recv(sock, &buf, 1, 0);

	if (ret != -1)
		return buf;
	return -1;
}

// === http_negotiate === 原 FUN_00028bec @ 0x00028bec
// 来源: reversed/cgminer/21_pool_net/FUN_00028bec.c | 分类: 21_pool_net
// 语义: HTTP 代理 CONNECT 协商: sending_version==0 发 HTTP/1.1 带 Host 形态,
//       否则 HTTP/1.0 简形态 (与上游 cgminer 相反, 以二进制为准); 收 12 字节
//       校验 "HTTP/1.1 200"/"HTTP/1.0 200", 再逐字节滑动窗口吞响应头至
//       "\r\n\r\n"; recv_byte 返回 0xff (-1) 视为读失败。CONNECT 地址用池
//       sockaddr (+0x270/+0x260), 日志用代理 sockaddr (+0x274/+0x278)。
int http_negotiate(int *pool, int sock, int sending_version)
{
	char          buf[1024];
	char          errbuf[4096];
	char         *url = *(char **)((char *)pool + 0x270);      /* sockaddr_url */
	char         *port = *(char **)((char *)pool + 0x260);     /* sockaddr_port */
	char         *px_url = *(char **)((char *)pool + 0x274);   /* 代理 sockaddr_url */
	char         *px_port = *(char **)((char *)pool + 0x278);  /* 代理 sockaddr_port */
	ssize_t       ret;
	int           i;

	if (sending_version == 0)
		snprintf(buf, 0x400, "CONNECT %s:%s HTTP/1.1\r\nHost: %s:%s\r\n\r\n",
		         url, port, url, port);
	else
		snprintf(buf, 0x400, "CONNECT %s:%s HTTP/1.0\r\n\r\n", url, port);

	if (GATE_DBG7) {
		snprintf(errbuf, 0x1000, "Sending proxy %s:%s - %s", px_url, px_port, buf);
		applog(7, errbuf, 0);
	}
	send(sock, buf, strlen(buf), 0);
	ret = recv(sock, buf, 0xc, 0);                          /* 12 字节状态行前缀 */
	if (ret < 1) {
		if (!GATE_W4)
			return 0;
		snprintf(errbuf, 0x1000, "Couldn't read from proxy %s:%s after sending CONNECT",
		         px_url, px_port);
		applog(4, errbuf, 0);
		return 0;
	}
	buf[ret] = '\0';
	if (GATE_DBG7) {
		snprintf(errbuf, 0x1000, "Received from proxy %s:%s - %s", px_url, px_port, buf);
		applog(7, errbuf, 0);
	}
	if (strcmp(buf, "HTTP/1.1 200") != 0 && strcmp(buf, "HTTP/1.0 200") != 0) {
		if (!GATE_W4)
			return 0;
		snprintf(errbuf, 0x1000, "HTTP Error from proxy %s:%s - %s", px_url, px_port, buf);
		applog(4, errbuf, 0);
		return 0;
	}
	/* 再读 4 字节起滑动窗口; 0xff = recv_byte 的 -1 → 读头失败 (0x00028db8) */
	for (i = 0; i < 4; i++) {
		buf[i] = (char)recv_byte(sock);
		if ((unsigned char)buf[i] == 0xff)
			goto read_err;
	}
	while (strncmp(buf, "\r\n\r\n", 4) != 0) {
		buf[0] = buf[1];
		buf[1] = buf[2];
		buf[2] = buf[3];
		buf[3] = (char)recv_byte(sock);
		if ((unsigned char)buf[3] == 0xff)
			goto read_err;
	}
	if (GATE_DBG7) {
		snprintf(errbuf, 0x1000, "Success negotiating with %s:%s HTTP proxy", px_url, px_port);
		applog(7, errbuf, 0);
	}
	return 1;
read_err:
	if (!GATE_W4)
		return 0;
	snprintf(errbuf, 0x1000, "Couldn't read HTTP byte from proxy %s:%s", px_url, px_port);
	applog(4, errbuf, 0);
	return 0;
}

// === __stratum_send === 原 FUN_00029404 @ 0x00029404
// 来源: reversed/cgminer/21_pool_net/FUN_00029404.c | 分类: 21_pool_net
// 语义: 持锁状态下 (stratum_send 外层持 stratum_lock) 向池 sock 发送 stratum
//       行: 末尾补 '\n'+'\0'; select 可写 (1s 超时, EINTR 重试清 fd_set 重选)
//       后以 MSG_NOSIGNAL(0x4000) 发送, EAGAIN(0xb) 视为 0 发送重选; 其他错误
//       返回 2, select 失败返回 1; 发完记账: +0x210/+0x218 两个 64 位累计
//       += 发送字节数, +0x208/0x20c 行计数 64 位自增 (低字 0xffffffff 回绕进位),
//       返回 0。len+1<=0 (不发生) 直接记账 0 字节。
int __stratum_send(void *pool, char *s, int len)
{
	fd_set wdevents;
	struct timeval tv;
	int sock = *(int *)((char *)pool + 0x264);
	size_t remaining = (size_t)(len + 1);
	int sent = 0;
	int ret;
	int sel;
	uint32_t line_lo;
	size_t sent_total;

	s[strlen(s)] = '\n';
	s[strlen(s) + 1] = '\0';

	for (;;) {
		if ((int)remaining > 0) {
			FD_ZERO(&wdevents);                             /* 反编译 32 字清零循环, 等价 */
			FD_SET(sock, &wdevents);                        /* sock>=0 时等价原置位算式 */
			tv.tv_sec = 1;
			tv.tv_usec = 0;
			sel = select(sock + 1, NULL, &wdevents, NULL, &tv);
			if (sel <= 0) {
				if (errno != EINTR)
					return 1;
				continue;                                   /* EINTR: 重清 fd_set 重选 */
			}
			ret = send(sock, s + sent, remaining, 0x4000 /*MSG_NOSIGNAL*/);
			if (ret < 0) {
				if (errno != 0xb /*EAGAIN*/)
					return 2;
				ret = 0;
			}
			remaining -= ret;
			sent += ret;
			if ((int)remaining >= 1)
				continue;                                   /* 未发完 → 重新 select */
		}
		break;
	}
	/* 0x00029528 记账: 64 位移位指令取低 32 位, 即发送总字节数 sent */
	sent_total = (size_t)sent;
	*(uint64_t *)((char *)pool + 0x210) += (uint64_t)sent_total;
	line_lo = *(uint32_t *)((char *)pool + 0x208);
	*(uint64_t *)((char *)pool + 0x218) += (uint64_t)sent_total;
	*(uint32_t *)((char *)pool + 0x208) = line_lo + 1;
	*(int32_t *)((char *)pool + 0x20c) += (line_lo > 0xfffffffe);   /* 低字回绕进位 */
	return 0;
}

// === clear_sock === 原 FUN_000295a8 @ 0x000295a8
// 来源: reversed/cgminer/21_pool_net/FUN_000295a8.c | 分类: 21_pool_net
// 语义: 持 stratum_lock (+0x2d8) 循环 recv (单次至多 0x1ffc 字节入 sockbuf)
//       直到 sock 关闭或无数据; 经 util 解锁包装后清 sockbuf。
void clear_sock(void *pool)
{
	pthread_mutex_t *lock = (pthread_mutex_t *)((char *)pool + 0x2d8);
	ssize_t ret;

	ret = pthread_mutex_lock(lock);
	if (ret != 0)
		mutex_lock_fail_util("clear_sock", 0x7a3);
	do {
		if (*(int *)((char *)pool + 0x264) == 0)
			break;
		ret = recv(*(int *)((char *)pool + 0x264),
		           *(char **)((char *)pool + 0x268), 0x1ffc, 0);
	} while (ret > 0);
	mutex_unlock_util(lock, "clear_sock", 0x7ac);
	(*DAT_00075ed8)();
	clear_sockbuf(pool);
}

// === proxytype === 原 FUN_00029690 @ 0x00029690
// 来源: reversed/cgminer/21_pool_net/FUN_00029690.c | 分类: 21_pool_net
// 语义: 代理类型码转前缀名: 0 → "http:" 快路径; 其余遍历代理前缀表
//       (0x64e3c 第 1 项起) 按类型匹配返回名称; 无匹配/表尽返回 "invalid"。
const char *proxytype(int type)
{
	const struct proxy_name *p;

	if (type == 0)
		return "http:";
	for (p = &DAT_00064e3c[1]; p->name != NULL; p++) {
		if (p->type == type)
			return p->name;
	}
	return "invalid";
}

// === tq_pop === 原 FUN_0002a300 @ 0x0002a300
// 来源: reversed/cgminer/21_pool_net/FUN_0002a300.c | 分类: 21_pool_net
// 语义: 从线程队列取一条: 持 mutex (tq+0xc); 队空则 cond_wait/timedwait
//       (abstime==NULL 无限等), 等待失败或仍空返回 NULL; 否则摘链首项返回
//       其 data。thread_q 布局: +0x00 链头 list_head, +0x0c mutex, +0x28 cond;
//       tq_ent = {data; list 节点} (data 在节点前 4 字节)。
void *tq_pop(void *tq, const struct timespec *abstime)
{
	pthread_mutex_t *lock = (pthread_mutex_t *)((char *)tq + 0x0c);
	pthread_cond_t  *cond = (pthread_cond_t *)((char *)tq + 0x28);
	int   *ent;
	int   *next;
	int   *prev;
	void  *data;
	int    ret;

	ret = pthread_mutex_lock(lock);
	if (ret != 0)
		mutex_lock_fail_util("tq_pop", 0x506);
	ent = *(int **)tq;                                      /* tq->q.next */
	if ((int *)tq == ent) {                                 /* 队空 */
		if (abstime == NULL)
			ret = pthread_cond_wait(cond, lock);
		else
			ret = pthread_cond_timedwait(cond, lock, abstime);
		if (ret != 0) {
			data = NULL;
			goto out;
		}
		ent = *(int **)tq;
		if ((int *)tq == ent) {
			data = NULL;
			goto out;
		}
	}
	next = *(int **)ent;                                    /* ent->next */
	prev = *(int **)(ent + 1);                              /* ent->prev */
	data = *(void **)(ent - 1);                             /* tq_ent.data */
	*(int **)(next + 1) = prev;                             /* next->prev = prev */
	*(int **)prev = next;                                   /* prev->next = next */
	*(int **)ent = NULL;                                    /* 摘除后清链指针 */
	*(int **)(ent + 1) = NULL;
	free(ent - 1);                                          /* 释放 tq_ent 基址 */
out:
	mutex_unlock_util(lock, "tq_pop", 0x519);
	(*DAT_00075ed8)();
	return data;
}

// === extract_sockaddr === 原 FUN_0002ab54 @ 0x0002ab54
// 来源: reversed/cgminer/21_pool_net/FUN_0002ab54.c | 分类: 21_pool_net
// 语义: 从 URL 提取地址与端口: 跳过 "//" 前缀; 支持 IPv6 "[...]" (冒号从 ']'
//       后找); 无端口默认 "80"; 端口段在 '/' 处截断。结果 strdup 后写入
//       *sockaddr_url/*sockaddr_port (先 port 后 url); 地址或端口为空返回 0。
int extract_sockaddr(char *url, char **sockaddr_url, char **sockaddr_port)
{
	char address[256];      /* 栈 -0x128, snprintf 上限 0xfe */
	char port_str[6];       /* 栈 -0x130 区, snprintf 上限 6 */
	char *slash;
	char *colon;
	char *br_open;
	char *br_close;
	char *probe;
	size_t addr_len;
	size_t rest_len;        /* ':' 之后剩余长度 */
	int    brackets;

	*sockaddr_url = url;
	probe = strstr(url, "//");
	if (probe != NULL)
		url = probe + 2;

	br_open = strchr(url, '[');
	br_close = strchr(url, ']');
	brackets = (br_close != NULL && br_open != NULL) ? 1 : 0;

	/* 端口冒号: IPv6 有效时从 ']' 之后找, 否则从头找 */
	colon = strchr((brackets && br_open < br_close) ? br_close : url, ':');
	if (colon == NULL) {
		addr_len = strlen(url);
		rest_len = 0;
	} else {
		addr_len = colon - url;
		rest_len = strlen(url) - addr_len;                  /* 反编译 ~addr_len+strlen */
		if (rest_len < 1)
			return 0;                                       /* 冒号后无内容 */
		colon = colon + 1;
	}
	if ((int)addr_len < 1)
		return 0;                                           /* 地址为空 */
	if (brackets && br_open < br_close) {
		addr_len -= 2;                                      /* 去掉 [ ] */
		url = url + 1;
	}
	snprintf(address, 0xfe, "%.*s", (int)addr_len, url);
	if (rest_len == 0) {
		port_str[0] = '8';                                  /* 反编译栈字 0x3038 = "80" */
		port_str[1] = '0';
		port_str[2] = '\0';
	} else {
		snprintf(port_str, 6, "%.*s", (int)rest_len, colon);
		slash = strchr(port_str, '/');
		if (slash != NULL)
			*slash = '\0';
	}
	*sockaddr_port = strdup(port_str);
	*sockaddr_url = strdup(address);
	return 1;
}

// === get_proxy === 原 FUN_0002ace0 @ 0x0002ace0
// 来源: reversed/cgminer/21_pool_net/FUN_0002ace0.c | 分类: 21_pool_net
// 语义: 解析代理前缀: 先清 rpc_proxy (+0xd8)=0; 顺序比对代理前缀表 (0x64e3c,
//       首项硬编码 "http:"/长度 5); 命中且含 '|' 时: '|' 截断, 分配
//       strlen(前缀后段)+1 存 rpc_proxy (+0xd8, 失败报错 quit), 对代理串再跑
//       extract_sockaddr 填代理 sockaddr (+0x274/+0x278), rpc_proxytype (+0xd4)
//       = 表项类型; 返回 '|' 之后的剩余 URL (未命中/无 '|' 原样返回)。
char *get_proxy(char *url, void *pool)
{
	const struct proxy_name *entry;
	const char *probe;
	size_t prefix_len;
	char *ptr;
	char *proxy_url;
	char *after_prefix;
	char *src;
	char  buf[4096];

	*(char **)((char *)pool + 0xd8) = NULL;                 /* pool->rpc_proxy 先清零 */
	entry = &DAT_00064e3c[0];
	probe = "http:";                                        /* 首项硬编码, __n = 5 */
	prefix_len = 5;
	for (;;) {
		if (strncmp(url, probe, prefix_len) == 0)
			break;
		entry++;
		probe = entry->name;                                /* 0x64e3c + i*8 */
		if (probe == NULL)
			return url;                                     /* 表尽未命中 */
		prefix_len = strlen(probe);
	}
	ptr = strchr(url, '|');
	if (ptr != NULL) {
		*ptr = '\0';
		/* malloc(ptr+1 - url - prefix_len) = strlen(前缀后段)+1 */
		proxy_url = malloc((size_t)(ptr + 1 - url - (int)prefix_len));
		*(char **)((char *)pool + 0xd8) = proxy_url;
		if (proxy_url == NULL) {
			snprintf(buf, 0x1000, "Failed to malloc rpc_proxy in %s %s():%d",
			         "util.c", "get_proxy", 0x371);
			applog(3, buf, 1);
			quit(1);
		}
		src = url + prefix_len;                             /* strcpy 源 (先于 url 重赋值取好) */
		after_prefix = ptr + 1;
		strcpy(proxy_url, src);
		extract_sockaddr(proxy_url,
		                 (char **)((char *)pool + 0x274),
		                 (char **)((char *)pool + 0x278));  /* 代理 sockaddr url/port */
		*(int *)((char *)pool + 0xd4) = entry->type;        /* pool->rpc_proxytype */
		url = after_prefix;
	}
	return url;
}

// === sock_full === 原 FUN_0002ae10 @ 0x0002ae10
// 来源: reversed/cgminer/21_pool_net/FUN_0002ae10.c | 分类: 21_pool_net
// 语义: sockbuf 有残留数据 (首字节非 '\0') 视为缓冲满返回 1, 否则
//       socket_full(pool, 0) 探测。
int sock_full(void *pool)
{
	if (**(char **)((char *)pool + 0x268) == '\0')
		return socket_full(pool, 0);
	return 1;
}

// === suspend_stratum === 原 FUN_0002aee0 @ 0x0002aee0
// 来源: reversed/cgminer/21_pool_net/FUN_0002aee0.c | 分类: 21_pool_net
// 语义: 关闭 stratum 连接: DEBUG 日志; 持 stratum_lock (+0x2d8) 清 sockbuf、
//       清 +0x29b 与 stratum_active (+0x299), close sock (+0x264) 并置 0;
//       解锁失败则 WTF 报错 (util.c suspend_stratum:0xd1c) 并 quit(1)。
void suspend_stratum(void *pool)
{
	pthread_mutex_t *lock = (pthread_mutex_t *)((char *)pool + 0x2d8);
	int   ret;
	char  buf[4096];

	if (GATE_DBG7) {
		snprintf(buf, 0x1000, "Closing socket for stratum pool %d", *(int *)pool);
		applog(7, buf, 0);
	}
	ret = pthread_mutex_lock(lock);
	if (ret != 0)
		mutex_lock_fail_util("suspend_stratum", 0xd1a);
	clear_sockbuf(pool);
	*(uint8_t *)((char *)pool + 0x29b) = 0;                 /* stratum 标志 (语义待定) */
	*(uint8_t *)((char *)pool + 0x299) = 0;                 /* pool->stratum_active */
	if (*(int *)((char *)pool + 0x264) != 0)
		close(*(int *)((char *)pool + 0x264));
	*(int *)((char *)pool + 0x264) = 0;
	ret = pthread_mutex_unlock(lock);
	if (ret != 0) {
		snprintf(buf, 0x1000, "WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d",
		         errno, "util.c", "suspend_stratum", 0xd1c);
		applog(3, buf, 1);
		quit(1);
	}
	(*DAT_00075ed8)();
}

// === init_stratum_threads === 原 FUN_0002b8e4 @ 0x0002b8e4
// 来源: reversed/cgminer/21_pool_net/FUN_0002b8e4.c | 分类: 21_pool_net
// 语义: 空函数 (stratum 线程创建在本固件被裁剪, 由其他路径完成)。
//       调用点 (pool_p2) 传入 pool 实参 (寄存器 r0 被函数忽略), 按签名保留。
void init_stratum_threads(void *pool)
{
    (void)pool;
}

// === restart_stratum === 原 FUN_0002cec8 @ 0x0002cec8
// 来源: reversed/cgminer/21_pool_net/FUN_0002cec8.c | 分类: 21_pool_net
// 语义: 重启 stratum 会话: stratum_active (+0x299) 时先 suspend_stratum;
//       initiate_stratum 失败, 或池要求 extranonce_subscribe (+0x25c) 而
//       extranonce_subscribe 失败, 或 auth_stratum 失败 → pool_died 并返回 0;
//       全部成功 → stratum_resumed 并返回 1。
int restart_stratum(void *pool)
{
	int ret;

	if (*(uint8_t *)((char *)pool + 0x299) != 0)            /* pool->stratum_active */
		suspend_stratum(pool);
	ret = initiate_stratum(pool);
	if (ret == 0 ||
	    ((*(uint8_t *)((char *)pool + 0x25c) != 0 &&        /* pool->extranonce_subscribe */
	      (ret = extranonce_subscribe(pool)) == 0) ||
	     (ret = auth_stratum(pool)) == 0)) {
		pool_died(pool);
		return 0;
	}
	stratum_resumed(pool);
	return 1;
}

// === rename_thread === 原 FUN_0002f0b4 @ 0x0002f0b4
// 来源: reversed/cgminer/21_pool_net/FUN_0002f0b4.c | 分类: 21_pool_net
// 语义: 线程命名: snprintf "cg@%s" (0x10 缓冲) 后 prctl(PR_SET_NAME=0xf)。
void rename_thread(char *name)
{
	char buf[16];

	snprintf(buf, 0x10, "cg@%s", name);
	prctl(0xf /*PR_SET_NAME*/, buf, 0, 0, 0);
}

// === copyadvanceafter === 原 FUN_00031fc4 @ 0x00031fc4
// 来源: reversed/cgminer/21_pool_net/FUN_00031fc4.c | 分类: 21_pool_net
// 语义: 逐字符拷贝至 ',' 或串尾: 遇 ',' 前移源指针越过逗号; '\\' 转义时跳过
//       反斜杠复制其后字符 (后随 NUL 则原样复制反斜杠); 终止时在目的处补
//       '\0' 并前移目的指针。pooldetails/api 字段拆分用 (与 api.c 同名一致)。
void copyadvanceafter(char **src, char **dest)
{
	for (;;) {
		char c = **src;

		if (c == '\0') {
			**dest = '\0';
			(*dest)++;
			return;
		}
		if (c == ',') {
			(*src)++;                                       /* 越过 ',' */
			**dest = '\0';
			(*dest)++;
			return;
		}
		if (c == '\\' && (*src)[1] != '\0')
			(*src)++;                                       /* 跳过转义反斜杠, 复制其后字符 */
		**dest = **src;
		(*dest)++;
		(*src)++;
	}
}

// === pooldetails === 原 FUN_0003204c @ 0x0003204c
// 来源: reversed/cgminer/21_pool_net/FUN_0003204c.c | 分类: 21_pool_net
// 语义: 拆分矿池参数串 "url[,user[,pass]]": malloc(len+1) 复制缓冲 (失败报
//       "Failed to malloc pooldetails buf" 并 quit), 连续三次 copyadvanceafter
//       拆出 url/user/pass 写回出参; 任一后续字段为空则 free 并返回 0。
int pooldetails(char *arg, char **url, char **user, char **pass)
{
	char *buf;
	char *src;
	char *cursor;
	size_t len;

	len = strlen(arg);
	buf = malloc(len + 1);
	if (buf == NULL) {
		applog(3, "Failed to malloc pooldetails buf", 1);
		quit(1);
	}
	cursor = buf;
	*url = buf;
	src = arg;
	copyadvanceafter(&src, &cursor);
	if (*src != '\0') {
		*user = cursor;
		copyadvanceafter(&src, &cursor);
		if (*src != '\0') {
			*pass = cursor;
			copyadvanceafter(&src, &cursor);
			return 1;
		}
	}
	free(buf);                                              /* 反编译释放 malloc 基址 */
	return 0;
}

// === switchpool === 原 FUN_00034830 @ 0x00034830
// 来源: reversed/cgminer/21_pool_net/FUN_00034830.c | 分类: 21_pool_net
// 语义: API "switchpool" 命令: 无池→MSG_NOPOOL(8); 缺参→MSG_MISPID(0x19);
//       持 api 锁组校验序号 (strtol), 越界→解锁后 MSG_INVPOOL(0x1a);
//       有效→pool->enabled (+0x84)=1 解锁后 switch_pools(pool),
//       MSG_SWITCHSUCCESS(0x1b)。
void switchpool(void *io_data, void *unused, char *indata)
{
	long poolnum;
	int  ret;
	int  *pool;

	if (DAT_0007b198 == 0) {
		message(io_data, 8 /*MSG_NOPOOL*/, 0, 0);
		return;
	}
	if (indata == NULL || *indata == '\0') {
		message(io_data, 0x19 /*MSG_MISPID*/, 0, 0);
		return;
	}
	poolnum = strtol(indata, NULL, 10);
	ret = pthread_mutex_lock(&DAT_0007ba10);
	if (ret != 0)
		_mutex_lock(&DAT_0007ba10, "switchpool", 0xb5b);
	api_rd_lock(&DAT_0007ba28, "switchpool", 0xb5b);
	ret = pthread_mutex_unlock(&DAT_0007ba10);
	if (ret != 0)
		_mutex_unlock(&DAT_0007ba10, "switchpool", 0xb5b);
	if (poolnum < 0 || DAT_0007b198 <= poolnum) {
		ret = pthread_rwlock_unlock(&DAT_0007ba28);
		if (ret != 0)
			api_rw_unlock(&DAT_0007ba28, "switchpool", 0xb5e);
		(*DAT_00075ed8)();
		message(io_data, 0x1a /*MSG_INVPOOL*/, (int)poolnum, 0);
		return;
	}
	pool = (int *)DAT_0007b438[poolnum];
	*(int *)((char *)pool + 0x84) = 1;                      /* pool->enabled = POOL_ENABLED */
	ret = pthread_rwlock_unlock(&DAT_0007ba28);
	if (ret != 0)
		api_rw_unlock(&DAT_0007ba28, "switchpool", 0xb65);
	(*DAT_00075ed8)();
	switch_pools(pool);
	message(io_data, 0x1b /*MSG_SWITCHSUCCESS*/, (int)poolnum, 0);
}

// === json_dump_callback === 原 FUN_000577a4 @ 0x000577a4
// 来源: reversed/cgminer/21_pool_net/FUN_000577a4.c | 分类: 21_pool_net
// 语义: jansson json_dump_callback: 未开 JSON_ENCODE_ANY(0x200) 时仅允许
//       object/array (type<=1), 其余返回 -1; 否则 do_dump (00057178, indent 0)。
int json_dump_callback(const json_t *json,
                       int (*callback)(const char *, size_t, void *), void *data,
                       size_t flags)
{
	if ((flags & 0x200) == 0 && (json == NULL || json->type > 1))
		return -1;
	return do_dump((json_t *)json, flags, 0, callback, data);
}

// === json_dumps === 原 FUN_00057854 @ 0x00057854
// 来源: reversed/cgminer/21_pool_net/FUN_00057854.c | 分类: 21_pool_net
// 语义: jansson json_dumps: strbuffer_init + json_dump_callback(dump_to_strbuffer
//       =回调 0x000570bc) + jsonp_strdup(strbuffer_value) + strbuffer_close;
//       init 失败或 dump 失败返回 NULL (close 均执行)。
char *json_dumps(const json_t *json, size_t flags)
{
	strbuffer_t strbuff;
	char *result = NULL;

	if (strbuffer_init(&strbuff) == 0) {
		if (json_dump_callback(json, fun_000570bc, &strbuff, flags) == 0) {
			result = jsonp_strdup(strbuffer_value(&strbuff));
		}
		strbuffer_close(&strbuff);
	}
	return result;
}

// === hashtable_find_pair === 原 FUN_0005791c @ 0x0005791c
// 来源: reversed/cgminer/21_pool_net/FUN_0005791c.c | 分类: 21_pool_net
// 语义: jansson hashtable_find_pair: 遍历桶链 (first→…→last) 找 hash 与 key
//       都匹配的 pair (pair = 链节点-4, 因 list 位于 pair+4); 空桶 (first 指向
//       内嵌哨兵 &hashtable->list 或 first==last) 返回 NULL。
hashtable_pair_t *hashtable_find_pair(hashtable_t *hashtable,
                                      hashtable_bucket_t *bucket,
                                      const char *key, size_t hash)
{
	hashtable_list_t *node = bucket->first;
	hashtable_pair_t *pair;

	if (node != &hashtable->list || node != bucket->last) {
		while (1) {
			pair = (hashtable_pair_t *)((char *)node - 4);
			if (pair->hash == hash && strcmp(pair->key, key) == 0)
				return pair;
			if (bucket->last == node)                       /* 链尾 */
				break;
			node = node->next;
		}
	}
	return NULL;
}

// === hashtable_iter_at === 原 FUN_00057ea4 @ 0x00057ea4
// 来源: reversed/cgminer/21_pool_net/FUN_00057ea4.c | 分类: 21_pool_net
// 语义: jansson hashtable_iter_at: djb2 哈希 (0x1505=5381 起, ×33+c) 对
//       prime_list[num_buckets] 取模定桶, hashtable_find_pair 命中返回
//       &pair->list (即 pair+4, 迭代句柄), 未命中 NULL。
void *hashtable_iter_at(hashtable_t *hashtable, const char *key)
{
	size_t hash = 0x1505;
	const unsigned char *p = (const unsigned char *)key;
	unsigned char c = *p;
	hashtable_pair_t *pair;
	size_t bucket;

	while (c != 0) {                                        /* 反编译 do-while, 空串保持 0x1505 */
		hash = c + hash * 0x21;
		p++;
		c = *p;
	}
	bucket = (size_t)__umodsi3((uint32_t)hash,              /* 0005c6f0: 余数经 r1 返回 */
	                           DAT_0006cb74[hashtable->num_buckets]);
	pair = hashtable_find_pair(hashtable, &hashtable->buckets[bucket], key, hash);
	if (pair != NULL)
		return &pair->list;                                 /* pair + 4 */
	return NULL;
}

// === json_loads === 原 FUN_00059230 @ 0x00059230
// 来源: reversed/cgminer/21_pool_net/FUN_00059230.c | 分类: 21_pool_net
// 语义: jansson json_loads: jsonp_error_init(error,"<string>") 先行;
//       string==NULL 报 "wrong arguments"; 否则 lex_init(lex, string_get
//       =回调 0x00057f94, 流数据{data,pos}) + parse_json + lex_cleanup。
json_t *json_loads(const char *string, size_t flags, json_error_t *error)
{
	char lex[64];                                           /* 栈 -0x50: lex_t 64 字节 */
	json_t *result = NULL;
	/* 字符串流游标 (栈 -0x58: {const char *data; int pos}) */
	struct { const char *data; int pos; } src;

	jsonp_error_init(error, "<string>");
	if (string == NULL) {
		error_set(error, NULL, "wrong arguments");
		return NULL;
	}
	src.data = string;
	src.pos = 0;
	if (lex_init(lex, fun_00057f94, &src) == 0) {
		result = parse_json(lex, flags, error);
		lex_cleanup(lex);
	}
	return result;
}

// === jsonp_strdup === 原 FUN_000595b4 @ 0x000595b4
// 来源: reversed/cgminer/21_pool_net/FUN_000595b4.c | 分类: 21_pool_net
// 语义: jansson jsonp_strdup: 经分配器函数指针 DAT_00076ba0 (jsonp_malloc) 分配
//       len+1 并 memcpy 含 NUL; strlen==-1、长度溢出为 0 或分配失败返回 NULL。
char *jsonp_strdup(const char *str)
{
	size_t len = strlen(str);
	char  *ret;

	if (len == (size_t)-1)
		return NULL;
	len = len + 1;
	if (len == 0)
		return NULL;
	ret = DAT_00076ba0(len);
	if (ret == NULL)
		return NULL;
	memcpy(ret, str, len);
	return ret;
}

// === strbuffer_init === 原 FUN_0005961c @ 0x0005961c
// 来源: reversed/cgminer/21_pool_net/FUN_0005961c.c | 分类: 21_pool_net
// 语义: jansson strbuffer_init: size=STRBUFFER_MIN_SIZE(0x10), length=0,
//       jsonp_malloc(0x10) 分配 value 并置 value[0]='\0'; 失败返回 -1。
int strbuffer_init(strbuffer_t *strbuff)
{
	strbuff->size = 0x10;
	strbuff->length = 0;
	strbuff->value = jsonp_malloc(0x10);
	if (strbuff->value == NULL)
		return -1;
	strbuff->value[0] = '\0';
	return 0;
}

// === strbuffer_close === 原 FUN_00059654 @ 0x00059654
// 来源: reversed/cgminer/21_pool_net/FUN_00059654.c | 分类: 21_pool_net
// 语义: jansson strbuffer_close: value 非空则 jsonp_free (00059594), 随后按
//       size/length/value 顺序清零。
void strbuffer_close(strbuffer_t *strbuff)
{
	if (strbuff->value != NULL)
		jsonp_free(strbuff->value);
	strbuff->size = 0;
	strbuff->length = 0;
	strbuff->value = NULL;
}

// === json_object_get === 原 FUN_00059e20 @ 0x00059e20
// 来源: reversed/cgminer/21_pool_net/FUN_00059e20.c | 分类: 21_pool_net
// 语义: jansson json_object_get: 仅对象 (type==0) 有效; djb2 哈希后对
//       prime_list[num_buckets] 取模定桶 (hashtable 位于 json+8, buckets 在
//       json+0xc), hashtable_find_pair 命中返回 pair->value (+0xc), 否则 NULL。
json_t *json_object_get(const json_t *object, const char *key)
{
	hashtable_t *ht;
	hashtable_pair_t *pair;
	size_t hash = 0x1505;
	const unsigned char *p = (const unsigned char *)key;
	unsigned char c = *p;
	size_t bucket;

	if (object == NULL || object->type != 0)                /* json_is_object */
		return NULL;
	while (c != 0) {                                        /* 反编译 do-while, 空串保持 0x1505 */
		hash = c + hash * 0x21;
		p++;
		c = *p;
	}
	ht = (hashtable_t *)((char *)object + 8);               /* json_object_t: 头部后即 hashtable */
	bucket = (size_t)__umodsi3((uint32_t)hash, DAT_0006cb74[ht->num_buckets]);
	pair = hashtable_find_pair(ht, &ht->buckets[bucket], key, hash);
	if (pair == NULL)
		return NULL;
	return pair->value;
}

// === __umodsi3 === 原 FUN_0005c6f0 @ 0x0005c6f0
// 来源: reversed/cgminer/21_pool_net/FUN_0005c6f0.c | 分类: 21_pool_net
// 语义: libgcc 无符号取模: 除数 0 时 raise(SIGFPE=8); 否则尾调 __udivsi3
//       (0005c648, ARM ABI 商/余成对返回 r0/r1) 取余数 r1 返回。
//       调用方: jansson 桶索引 hash%prime、adjust_quota_gcd 试除循环。
//       注: 原体 div0 后继续走 __udivsi3, 其 div0 分支再次 raise 后余数
//       寄存器为 0 — 故此处等价表达为 raise 后返回 0。
uint32_t __umodsi3(uint32_t dividend, uint32_t divisor)
{
	if (divisor == 0) {
		raise(8 /*SIGFPE*/);
		return 0;                                           /* 原体 div0 路径余数 r1=0 */
	}
	return dividend % divisor;                              /* 原体: 尾调 __udivsi3 取 r1 */
}

// =============================================================================
// 覆盖率表 (47 行) — pool_b3 分配全部转写完成
// 主题组: A=配置/URL/代理  B=池状态/调度  C=socket/stratum
//         D=线程/队列/curses  E=jansson 底层
// =============================================================================
// | #  | 语义名               | 原地址      | 组 |
// |----|----------------------|-------------|----|
// | 01 | priority_pool        | FUN_00012038 | A |
// | 02 | adjust_quota_gcd     | FUN_000152e0 | B |
// | 03 | detect_stratum       | FUN_0001557c | A |
// | 04 | wlogprint            | FUN_00015890 | D |
// | 05 | remove_pool          | FUN_00015ac8 | A |
// | 06 | setup_url            | FUN_00017100 | A |
// | 07 | add_url              | FUN_00017db8 | A |
// | 08 | set_quota            | FUN_00017e20 | A |
// | 09 | pool_tset            | FUN_00018834 | B |
// | 10 | pool_tclear          | FUN_00018c2c | B |
// | 11 | current_pool         | FUN_0001da58 | B |
// | 12 | pool_died            | FUN_0001fa14 | B |
// | 13 | pool_died_silent     | FUN_0001fb44 | B |
// | 14 | cp_prio              | FUN_0001fb7c | B |
// | 15 | pool_resus           | FUN_0001fc34 | B |
// | 16 | stratum_resumed      | FUN_0001fd64 | B |
// | 17 | cnx_needed           | FUN_0001ffec | B |
// | 18 | input_thread         | FUN_00027a98 | D |
// | 19 | clear_sockbuf        | FUN_00028ae8 | C |
// | 20 | socket_full          | FUN_00028afc | C |
// | 21 | block_socket         | FUN_00028b9c | C |
// | 22 | recv_byte            | FUN_00028bc4 | C |
// | 23 | http_negotiate       | FUN_00028bec | C |
// | 24 | __stratum_send       | FUN_00029404 | C |
// | 25 | clear_sock           | FUN_000295a8 | C |
// | 26 | proxytype            | FUN_00029690 | A |
// | 27 | tq_pop               | FUN_0002a300 | D |
// | 28 | extract_sockaddr     | FUN_0002ab54 | A |
// | 29 | get_proxy            | FUN_0002ace0 | A |
// | 30 | sock_full            | FUN_0002ae10 | C |
// | 31 | suspend_stratum      | FUN_0002aee0 | C |
// | 32 | init_stratum_threads | FUN_0002b8e4 | C |
// | 33 | restart_stratum      | FUN_0002cec8 | C |
// | 34 | rename_thread        | FUN_0002f0b4 | D |
// | 35 | copyadvanceafter     | FUN_00031fc4 | A |
// | 36 | pooldetails          | FUN_0003204c | A |
// | 37 | switchpool           | FUN_00034830 | A |
// | 38 | json_dump_callback   | FUN_000577a4 | E |
// | 39 | json_dumps           | FUN_00057854 | E |
// | 40 | hashtable_find_pair  | FUN_0005791c | E |
// | 41 | hashtable_iter_at    | FUN_00057ea4 | E |
// | 42 | json_loads           | FUN_00059230 | E |
// | 43 | jsonp_strdup         | FUN_000595b4 | E |
// | 44 | strbuffer_init       | FUN_0005961c | E |
// | 45 | strbuffer_close      | FUN_00059654 | E |
// | 46 | json_object_get      | FUN_00059e20 | E |
// | 47 | __umodsi3            | FUN_0005c6f0 | E |
