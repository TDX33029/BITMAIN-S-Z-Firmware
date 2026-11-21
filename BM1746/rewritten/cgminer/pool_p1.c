// =============================================================================
// cgminer 矿池网络层 第 1 段 (pool_p1.c)
// 批次: pool_b1  |  分类: 21_pool_net
// 函数: stratum_rthread (FUN_00020088) / display_pools (FUN_00026f50) /
//       initiate_stratum (FUN_0002b8e8)
// 命名事实源: rewritten/name_map/cgminer.tsv (阶段 1 冻结)
// 严格语义等价转写; 结构布局不可知处保留指针算术并注释字节偏移。
// =============================================================================

/*
 * rewritten/cgminer/pool_p1.c — cgminer 矿池网络层 第 1 段 (批次 pool_b1)
 * ===========================================================================
 * 来源: reversed/cgminer/21_pool_net/
 * 范围: 3 个大函数 —
 *   stratum_rthread  (原 FUN_00020088, 742 行) — stratum 接收线程主循环
 *   display_pools    (原 FUN_00026f50, 585 行) — curses 矿池管理界面
 *   initiate_stratum (原 FUN_0002b8e8, 843 行) — 建立 stratum 连接并发送 subscribe
 *
 * 事实还原 (以反编译偏移 / name_map / 上游 cgminer 字段名为证):
 *   - pool 结构仅还原本批实证偏移, 以 POOL_OFF_* 宏 + 字节偏移注释表达;
 *     完整布局不可知, 统一保留 int *pool 指针算术 (与 axi.c 批一致)。
 *   - 池状态枚举 (pool+0x84): 0=Disabled, 1=Enabled, 2=Rejecting
 *     (display_pools 打印三态、enable/disable 路径增减全局启用计数所证)。
 *   - 池管理策略全局 DAT_00076efc: 0=Failover, 1=RoundRobin, 2=Rotate,
 *     3=LoadBalance, 4=Balance (display_pools 菜单 5 项 + cnx_needed 特判 3/4)。
 *   - stratum 应答份额表为 uthash: 全局头 DAT_00077048 (首个 stratum_share*,
 *     NULL=空); share 结构内联 UT_hash_handle (+0x00..0x1F), id@+0x20,
 *     work*@+0x24, 提交时间@+0x30。查找/删除为编译器内联展开的
 *     HASH_FIND_INT + HASH_DELETE (Jenkins mix 常量折叠形态, 原样保留)。
 *   - "PROOF OF WORK RESULT: ..." 等日志串原码为编译器对常量串的逐字写
 *     (字面量池化), 转写还原为等价字面量。
 *   - applog 门控为全库统一内联模式 (见 json.c):
 *     外层 opt_debug(DAT_00078e09) && (DAT_0007ad48 || DAT_00078e08 ||
 *     N < DAT_000766c4), N 随日志级别取 6(级别7)/4(级别4)/3/2。
 *   - 浮点比较在反编译中表现为 fpscr 位域运算 (ARM VCMP), 均已还原为
 *     直接的双精度比较 (语义逐位核对: N=bit31 a<b, Z=bit30 a==b,
 *     C=bit29 a>=b)。
 *   - initiate_stratum 中 setup_stratum_socket 已内联 (上游 util.c 同名函数),
 *     SOCKS4/4a/5/HTTP 协商按 pool->proxytype 分派 (0..5)。
 *
 * 注: extern 声明取自 name_map 语义名, 定义分布在其余批次文件;
 *     最终应上移至 rewritten/include 目录共享。
 */
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>      /* snprintf/sprintf/strlen/memcpy 等 */
#include <stdlib.h>     /* malloc/free/calloc/strtol */
#include <string.h>     /* memset/memcmp/strncpy */
#include <stdbool.h>
#include <time.h>       /* time_t/time */
#include <pthread.h>
#include <sys/select.h> /* fd_set/select */
#include <sys/socket.h> /* socket/connect/setsockopt/getsockopt/send/close */
#include <sys/time.h>   /* struct timeval */
#include <fcntl.h>      /* fcntl */
#include <netdb.h>      /* addrinfo/getaddrinfo/freeaddrinfo */
#include <errno.h>      /* __errno_location */
#include <unistd.h>     /* close */
#include <math.h>       /* round */
#include <strings.h>    /* strncasecmp */

/* ================= 本固件日志级别 (syslog 值, 见 json.c) ================= */

#define LOG_ERR    3
#define LOG_WARNING 4
#define LOG_INFO   6
#define LOG_DEBUG  7

/* ================= pool 结构实证偏移 (字节) =================
 * 完整布局不可知; 仅列本批三函数访问到的字段。int *pool 指针,
 * int 槽位寻址 pool[N] == 字节偏移 4N, 字节字段用 (char*)pool + off。 */

#define POOL_OFF_POOL_NO      0x00   /* pool_no (int) */
#define POOL_OFF_PRIO         0x04   /* prio (int) */
#define POOL_OFF_ACCEPTED     0x08   /* accepted (int64: 低 0x08/高 0x0C) */
#define POOL_OFF_REJECTED     0x10   /* rejected (int64: 低 0x10/高 0x14) */
#define POOL_OFF_SEQ_REJECTS  0x18   /* seq_rejects (int): 连续拒绝计数 */
#define POOL_OFF_UNS_1C       0x1C   /* int, pool_resus 时清零 (语义待定) */
#define POOL_OFF_SOLVED       0x20   /* solved (int): 已解出区块数 */
#define POOL_OFF_IDLE         0x81   /* idle (字节): pool_tset/tclear 管理 */
#define POOL_OFF_STRATUM_AUTH 0x83   /* 字节: 授权成功置 1 (json.c auth_stratum
				       * 同一偏移); initiate_stratum 中首次
				       * resolve 失败亦置 1 以抑制重复告警 */
#define POOL_OFF_STATE        0x84   /* state (int): 0=Disabled 1=Enabled 2=Rejecting */
#define POOL_OFF_UNS_89       0x89   /* 字节: 置位时 stratum 线程挂起退出 (语义待定) */
#define POOL_OFF_HAS_LONGPOLL 0x8C   /* has_longpoll (int) */
#define POOL_OFF_QUEUED       0x94   /* queued (int): 排队工作请求数 */
#define POOL_OFF_STRATUM_PORT 0x260  /* stratum_port (char*: 端口串) */
#define POOL_OFF_SOCK         0x264  /* sock (int) */
#define POOL_OFF_SOCKBUF      0x268  /* sockbuf (char*: 接收缓冲) */
#define POOL_OFF_SOCKBUF_SIZE 0x26C  /* sockbuf_size (int) */
#define POOL_OFF_SOCKADDR_URL 0x270  /* sockaddr_url (char*: stratum 主机) */
#define POOL_OFF_PROXY_URL    0x274  /* 代理 sockaddr_url (char*) */
#define POOL_OFF_PROXY_PORT   0x278  /* 代理 stratum_port (char*) */
#define POOL_OFF_NONCE1       0x27C  /* nonce1 (char*) */
#define POOL_OFF_GETFAIL      0xA0   /* getfail_occasions (int) */
#define POOL_OFF_REMOTEFAIL   0xA4   /* remotefail_occasions (int) */
#define POOL_OFF_RPC_URL      0xC4   /* rpc_url (axi.c 实证) */
#define POOL_OFF_RPC_USER     0xCC   /* rpc_user (axi.c 实证) */
#define POOL_OFF_RPC_PASS     0xD0   /* rpc_pass (axi.c 实证) */
#define POOL_OFF_PROXYTYPE    0xD4   /* proxytype (axi.c 实证): 0..5 */
#define POOL_OFF_RPC_PROXY    0xD8   /* rpc_proxy (axi.c 实证, char*) */
#define POOL_OFF_QUOTA        0x38   /* quota (axi.c 实证) */
#define POOL_OFF_DIFF         0x730  /* 当前池难度 (double, 初始 1.0) */
#define POOL_OFF_SESSION_ID   0x294  /* session_id (char*, data rwlock 保护) */
#define POOL_OFF_EXTRAN2SIZE  0x290  /* extranonce2 长度 (int) */
#define POOL_OFF_UNS_258      0x258  /* char*: 订阅成功且为零时记 sockaddr_url (语义待定) */
#define POOL_OFF_UNS_25C      0x25C  /* 字节: restart_stratum 中触发 extranonce_subscribe (语义待定) */
#define POOL_OFF_UNS_298      0x298  /* 字节: display_pools/cnx_needed 读 (stratum 活动标志?) */
#define POOL_OFF_UNS_299      0x299  /* 字节: 订阅成功置 1, suspend_stratum 清 0 (stratum_active) */
#define POOL_OFF_UNS_29B      0x29B  /* 字节: suspend_stratum 清 0 (语义待定) */
#define POOL_OFF_UNS_2F4      0x2F4  /* int: 非零时 cnx_needed 判需保持连接 (语义待定) */
#define POOL_OFF_UNS_2FC      0x2FC  /* int: 收到份额应答时减 1 (语义待定) */
#define POOL_OFF_RESTART      0x2B8  /* restart (字节): 重启后补发一份 stratum work */
#define POOL_OFF_ACC_DIFF     0x68   /* accepted_diff (double) */
#define POOL_OFF_REJ_DIFF     0x70   /* rejected_diff (double) */
#define POOL_OFF_LAST_SH_TIME 0x180  /* 最近接受份额时间 (int) */
#define POOL_OFF_LAST_SH_DIFF 0x188  /* 最近接受份额难度 (double, 按半字写) */
#define POOL_OFF_DATA_LOCK_M  0xF4   /* pthread_mutex_t (data_lock) */
#define POOL_OFF_DATA_LOCK_RW 0x10C  /* pthread_rwlock_t (data_lock) */
#define POOL_OFF_STRATUM_LOCK 0x2D8  /* pthread_mutex_t (stratum_lock) */
#define POOL_OFF_UNS_370      0x370  /* pthread_mutex_t (pool_tset/tclear 使用) */

/* work 结构实证偏移 (字节; void *work) */
#define WORK_OFF_POOL         0x6D4  /* work->pool (struct pool *) */
#define WORK_OFF_THR_ID       0x6D0  /* work->thr_id (int) */
#define WORK_OFF_LONGPOLL     0x6E8  /* 字节: 重启补发路径置 1 */
#define WORK_OFF_UNS_6E9      0x6E9  /* 字节: 拒绝禁用判定读 (语义待定) */
#define WORK_OFF_BLOCK        0x6EB  /* 字节: 区块标志, 显示 " BLOCK!" 并触发 restart_threads */
#define WORK_OFF_UNS_6EC      0x6EC  /* 字节: 允许从 error 字段取拒绝原因 */
#define WORK_OFF_UNS_714      0x714  /* 字节: result==NULL 也算接受的例外标志 (语义待定) */
#define WORK_OFF_UNS_6B8      0x6B8  /* double: hashshow "%s" 后缀串输入 (语义待定) */
#define WORK_OFF_DIFFICULTY   0x748  /* work_difficulty (double) */

/* cgpu_info 实证偏移 (字节; int *cgpu = thr+0x24 所指) */
#define CGPU_OFF_ACCEPTED     0x24   /* accepted (int) */
#define CGPU_OFF_REJECTED     0x28   /* rejected (int) */
#define CGPU_OFF_ACC_DIFF     0xC8   /* diff_accepted (double) */
#define CGPU_OFF_REJ_DIFF     0xD0   /* diff_rejected (double) */
#define CGPU_OFF_LAST_POOL    0xD8   /* 最近接受份额所在池的 pool_no (int) */
#define CGPU_OFF_LAST_VALID   0xDC   /* 最近有效工作时刻 (time_t) */
#define CGPU_OFF_LAST_DIFF    0xE0   /* 最近接受份额难度 (double, 按两个半字写) */
#define CGPU_OFF_DRV          0x04   /* 驱动描述指针 (其 +0x08 为名称串) */
#define CGPU_OFF_DEV_ID       0x08   /* 设备编号 (int) */

/* thr_info 实证偏移 */
#define THR_OFF_CGPU          0x24   /* thr->cgpu (struct cgpu_info *) */

/* work->data 头 32 字节区 (0xC0..0xDF) 大端展示起点 */
#define WORK_OFF_DATA_C0      0xC0

/* ================= 外部函数 (name_map 语义名) ================= */

/* --- 21_pool_net / util --- */
extern void   rename_thread(char *name);                       /* 0002f0b4 */
extern int    sock_full(int *pool);                            /* 0002ae10: sockbuf 有数据即真 */
extern int    cnx_needed(int *pool);                           /* 0001ffec: 返回 0/1 */
extern int   *current_pool(void);                              /* 0001da58 */
extern void   suspend_stratum(int *pool);                      /* 0002aee0 */
extern void   stratum_resumed(int *pool);                      /* 0001fd64 */
extern char  *recv_line(int *pool);                            /* 0002b040: 无整行返回 NULL */
extern int    restart_stratum(int *pool);                      /* 0002cec8 */
extern int    __stratum_send(int *pool, char *buf, size_t len);/* 00029404: 0=成功, 1=写选择失败, 2=发送失败, 3=链路未激活 */
extern int    stratum_send(int *pool, char *s, int total_stratum_json); /* 0002b5c8 */
extern int    socket_full(int *pool, int wait);                /* 00028afc: 等待 sockbuf 数据 */
extern void   clear_sockbuf(int *pool);                        /* 00028ae8 */
extern void   clear_sock(int *pool);                           /* 000295a8: 读净 socket 并清 sockbuf */
extern void   block_socket(int sock);                          /* 00028b9c */
extern int    recv_byte(int sock);                             /* 00028bc4 */
extern int    http_negotiate(int *pool, int sock, int);        /* 00028bec */
extern int    socks4_negotiate(int *pool, int sock, int);      /* 00028fc8 */
extern void   extract_sockaddr(char *url, char **host, char **port); /* 0002ab54 */
extern unsigned int parse_notify_equihash(int *pool, char *buf); /* 0002cf40: 非 0=方法消息已消化 */
extern int    extranonce_subscribe(int *pool);                 /* 0002e69c */
extern int    parse_extranonce_equihash(int *pool, void *res_val, int); /* 000299ac */
extern void   clear_stratum_shares(int *pool);                 /* 000182f4 */
extern void   clear_pool_work(int *pool);                      /* 000185c8 */
extern void   switch_pools(int *preferred);                    /* 00019bf8 (NULL=自动) */
extern void   remove_pool(int *pool);                          /* 00015ac8 */
extern void   adjust_quota_gcd(void);                          /* 000152e0 */
extern int    pool_tclear(int *pool, uint8_t *bvar);           /* 00018c2c: 返回旧值并清零 */
extern int    pool_tset(int *pool, uint8_t *bvar);             /* 00018834: 置 1 返回旧值 */
extern void   cgtime(struct timeval *tv);                      /* 0002a84c */
extern void   cgsleep_ms(uint32_t ms);                         /* 0002a92c */

/* --- 20_job_mining / 线程 --- */
extern void  *make_work(void);                                 /* 0001a3d4 */
extern void   gen_stratum_work(int *pool, void *work);         /* 0001acc4 */
extern int    test_work_current(void *work);                   /* 0001db0c */
extern void   free_work(void *work);                           /* 00015780 */
extern void   restart_threads(void);                           /* 00017210 */
extern void   kill_work(void);                                 /* 000178f8 */
extern int   *get_thread(int id);                              /* 0001b124 */

/* --- 41_logging / 60_util --- */
extern void   applog(uint32_t prio, char *buf, ...);           /* 00038730 (第 3 参按反编译保留) */
extern void   sharelog(char *sharetype, void *work);           /* 0001b600 (fork: 标签含拒绝原因) */
extern void   suffix_string(double value, char *buf, size_t bufsiz, int sigfigs); /* 00011580 */

/* --- 错误包装 (锁失败即打印并 quit) --- */
extern void   mutex_lock_fail_cgminer(const char *caller, int line);   /* 0000b414 */
extern void   mutex_unlock_fail_cgminer(const char *caller, int line); /* 0000b5c4 */
extern void   rd_lock_error(const char *caller, int line);     /* 0000b69c */
extern void   rw_unlock_error(const char *caller, int line);   /* 0000b630 */
extern void   mutex_lock_fail_util(const char *caller, int line);   /* 0000b708 */
extern void   quit(int status);                                /* 00016ca8 (不返回) */

/* --- jansson (B3/D 分片) --- */
typedef struct json_t { int type; int refcount; } json_t;      /* 头两字段, 见 json.c */
extern json_t *json_loads(char *string, size_t flags, void *error); /* 00059230 (error=json_error_t*) */
extern json_t *json_object_get(json_t *object, const char *key);    /* 00059e20 */
extern char   *json_dumps(json_t *value, unsigned int flags);       /* 00057854 */
extern void    json_delete(json_t *value);                          /* 0005a3e4 */
extern int     json_integer_value(json_t *value);                   /* 0005a248 */
extern char   *json_string_value(json_t *value);                    /* 0005a178 */
extern json_t *json_array_get(json_t *array, int index);            /* 0005a008 */

/* --- curses (display_pools) --- */
extern void   immedok(void *win, bool flag);
extern int    wgetch(void *win);
extern void   wattr_on(void *win, uint32_t attrs, void *unused);
extern void   wattr_off(void *win, uint32_t attrs, void *unused);
extern void  *stdscr;                                             /* 主屏 */
extern void   clear_logwin(void);                              /* 00018ad8 */
extern void   logwin_update(void);                             /* 00018b44 */
extern void   _wlogprint(char *str);                           /* 00018bc0: 原样打印一行 */
extern void   wlogprint(const char *fmt, ...);                 /* 00015890: 格式化打印 */
extern int    curses_active_locked(void);                      /* 00017fe4: 抢锁成功非 0 */
extern int    curses_int(const char *prompt);                  /* 000260d0: curses 数字输入 */
extern int    input_pool(int flag);                            /* 00026d10 */

/* --- 编译器辅助 (libgcc, 软浮点 ABI: 64 位经 r0/r1 传递) --- */
extern double __floatdidf(unsigned long long value);            /* 0005cb64 */
extern unsigned long long __fixunsdfdi(double value);           /* 0005cc48 */

/* ================= 外部全局 (语义见文件头/行内注释) ================= */

extern void   (*DAT_00075ed8)(void);   /* 空操作函数指针 (初值 noop_hook, 恒返回 0) */
extern uint8_t DAT_00078e09;   /* 日志守卫: 外层总开关 (opt_debug, 语义待定) */
extern uint8_t DAT_0007ad48;   /* 日志守卫: 次级开关 A */
extern uint8_t DAT_00078e08;   /* 日志守卫: 次级开关 B */
extern int     DAT_000766c4;   /* 日志守卫: 级别阈值 (值越大越详细) */

extern int     DAT_0007b198;   /* total_pools: 池总数 */
extern int    *DAT_0007b438;   /* pools: 池指针数组 */
extern int     DAT_0007b5e4;   /* 启用态 (state==1) 池计数 */
extern int     DAT_00076efc;   /* 池管理策略: 0=Failover 1=RoundRobin 2=Rotate 3=LoadBalance 4=Balance */
extern int     DAT_0007b558;   /* 轮换间隔 (分钟, 策略 2 用) */
extern uint8_t DAT_0007b625;   /* failover-only 开关 */
extern char   *DAT_00075e10[]; /* 策略名字符串指针表 (5 项) */
extern char   *DAT_00078b78;   /* 全局 SOCKS5 代理地址串 (赋给 pool->rpc_proxy) */
extern uint8_t DAT_0007b380;   /* extranonce 调试打印开关 */
extern uint8_t DAT_0007bd20;   /* 为 0 时对 socket 置 FD_CLOEXEC (语义待定) */
extern int     DAT_0007b0f8;   /* pool_id 分配计数器 (取值后自增) */

extern pthread_mutex_t DAT_00077030;  /* stratum 应答份额表锁 */
extern pthread_cond_t  DAT_00077078;  /* lp 条件变量 (wait_lpcurrent) */
extern pthread_mutex_t DAT_00077060;  /* lp 锁 (wait_lpcurrent) */
extern int    *DAT_00077048;          /* stratum 应答份额表头 (首个 stratum_share*, NULL=空) */
extern pthread_mutex_t DAT_0007ad4c;  /* stats_lock: 总份额计数锁 */
extern uint32_t DAT_0007b420;         /* 总接受份额 (uint64 低字) */
extern uint32_t DAT_0007b424;         /* 总接受份额 (uint64 高字) */
extern double   DAT_0007b5e8;         /* 总接受份额难度累计 */
extern uint32_t DAT_0007b6f8;         /* 总拒绝份额 (uint64 低字) */
extern uint32_t DAT_0007b6fc;         /* 总拒绝份额 (uint64 高字) */
extern double   DAT_0007b670;         /* 总拒绝份额难度累计 */
extern int      DAT_0007b43c;         /* 全局取工作失败计数 (与池 getfail 同步自增, 语义待定) */
extern uint8_t  DAT_0007b644;         /* 连接丢失时也清空 stratum 份额的全局条件 (语义待定) */
extern uint8_t  DAT_0007b250;         /* 允许按连续拒绝禁用池 (语义待定) */
extern uint8_t  DAT_0007bc78;         /* 静默标志 A (禁用 sharelog/打印) */
extern uint8_t  DAT_0007bb18;         /* 静默标志 B (禁用 sharelog/打印) */
extern int      DAT_00077288;         /* 目标接受份额数, 达到后退出 (语义待定) */
extern double   DAT_000755f8;         /* 连续拒绝率阈值换算除数 (语义待定) */
extern int      DAT_0007b4c8;         /* cnx_needed 兜底返回布尔 (语义待定) */

extern void  *DAT_0007b430;           /* curses 主窗口 (WINDOW*) */
extern int    DAT_0007bac8;           /* curses 活动标志 */
extern pthread_mutex_t DAT_0007b7a8;  /* curses 锁 */

/* 键名 "method" 原数据地址 (json_object_get 第 2 参) */
extern char DAT_00061ca0[];           /* 内容为 "method" */

/* uthash 字段偏移 (share 内联 UT_hash_handle, 32 字节):
 * +0x00 tbl +0x04 prev +0x08 next +0x0C hh_prev +0x10 hh_next
 * +0x14 key +0x18 keylen +0x1C hashv;  UT_hash_table:
 * +0x00 buckets +0x04 num_buckets +0x0C num_items +0x10 tail +0x14 hho */

/* stratum_share 结构 (uthash 项, 与上游 cgminer.c 同名结构一致) */
typedef struct stratum_share {
	uint32_t hh_tbl;      /* +0x00: UT_hash_handle 首字段 (hh.tbl) */
	uint32_t hh_prev;     /* +0x04: 前一 share */
	uint32_t hh_next;     /* +0x08: 后一 share */
	uint32_t hh_hhprev;   /* +0x0C: 桶内前驱 */
	uint32_t hh_hhnext;   /* +0x10: 桶内后继 */
	uint32_t hh_key;      /* +0x14: 指向 +0x20 id 的指针 */
	uint32_t hh_keylen;   /* +0x18: == 4 */
	uint32_t hh_hashv;    /* +0x1C */
	int32_t  id;          /* +0x20: stratum 应答 id (hh.key 指向此) */
	void    *work;        /* +0x24 */
	uint32_t pad28[2];    /* +0x28..0x2F: 本批未访问 */
	time_t   stratum_time;/* +0x30: 提交时刻 */
} stratum_share_t;

/* 日志门控宏 (全库统一内联模式, N 随级别升高) */
#define LOG_GATE7 (DAT_0007ad48 != 0 || DAT_00078e08 != 0 || 6 < DAT_000766c4)
#define LOG_GATE4 (DAT_0007ad48 != 0 || DAT_00078e08 != 0 || 3 < DAT_000766c4)
#define LOG_GATE2 (DAT_0007ad48 != 0 || DAT_00078e08 != 0 || 2 < DAT_000766c4)

// === stratum_rthread === 原 FUN_00020088 @ 0x00020088
// 来源: reversed/cgminer/21_pool_net/FUN_00020088.c | 分类: 21_pool_net
// 语义: stratum 接收线程主循环: 无连接且不需要连接时挂起等待; select/recv_line
//       取行后先交 parse_notify_equihash 消化方法消息, 其余按 JSON 应答处理:
//       以 "id" 在 uthash 份额表中找到对应提交并删除, 统计接受/拒绝 (池/卡/全局
//       三级计数), 打印 hashshow 与 PROOF OF WORK RESULT; 连续拒绝超限禁池,
//       连接丢失走 supports_resume → 清份额/工作 → restart_stratum 重连循环。
//       pool+0x89 字节置位时 suspend_stratum 并退出线程。返回恒 0。
int stratum_rthread(int *pool)
{
	/* json_error_t 布局 (252 字节): line(+0) column(+4) position(+8)
	 * source[80](+0xC) text[160](+0x5C); 原栈上以 aiStack_1134 起始 */
	typedef struct { int line, column, position; char source[80]; char text[160]; } json_error_layout_t;
	char hashshow[60];              /* 原 acStack_11f4: "%08lx Diff %s/%llu%s" */
	char diffstr[16];               /* 原 local_1218 前 16 字节: suffix_string 输出 */
	char sharetype[36];             /* 原 local_1218..auStack_1211: "accept"/"reject:<原因>" */
	char reasonparen[32];           /* 原 acStack_1238: " (<原因>)" */
	char poolstr[20];               /* 原 acStack_124c: "pool %d" */
	char msg[4096];                 /* 原 local_1038 (0x1000): applog/snprintf 复用缓冲 */
	char proofbuf[40];              /* 常量串 "PROOF OF WORK RESULT: ..." 载体 */
	char threadname[16];            /* 原 acStack_125c: "%d/RStratum" */
	struct timeval timeout;         /* 原 tStack_1264 */
	/* json_error_t 布局 (252 字节): line(+0) column(+4) position(+8)
	 * source[80](+0xC) text[160](+0x5C); 原栈上以 aiStack_1134 起始 */
	json_error_layout_t err;        /* 原 aiStack_1134+acStack_10d8 */
	pthread_t self;
	char *line;                     /* recv_line 结果 */
	uint32_t id;                    /* 原 uStack_1268: 应答 id (哈希键) */
	fd_set fds;                     /* 原 local_11b4 */
	json_t *val;                    /* json_loads 结果 */
	json_t *res_val;                /* "result" */
	json_t *err_val;                /* "error" */
	json_t *meth_val;               /* "method" */
	json_t *reason_val;             /* "reject-reason" (原码复用 res_val 槽位) */
	char *reason;                   /* 需 free 的原因串 */
	char *rs;                       /* json_string_value 结果 */
	size_t n;                       /* 原因串长度 (上限 0x1c) */
	bool share_found;               /* 原 bVar19: 是否处理了份额应答 */
	stratum_share_t *share;         /* 原 __ptr_00 */
	uint32_t *tbl;                  /* 原 puVar17: UT_hash_table* */
	void *buckets;                  /* 原 __ptr: tbl->buckets */
	int sel;                        /* select 返回值 */
	int i;
	uint32_t carry;                 /* 原 bVar19 进位位 */
	time_t now;
	double diff;                    /* 份额难度 (= pool 当前难度) */
	double threshold;               /* 连续拒绝率阈值 */
	void *work;                     /* share->work */
	int *cgpu;                      /* thr->cgpu */
	int seq_rejects;                /* pool->seq_rejects 临时量 */

	self = pthread_self();
	pthread_detach(self);
	snprintf(threadname, 0x10, "%d/RStratum", *(int *)((char *)pool + POOL_OFF_POOL_NO));
	rename_thread(threadname);

	do {
		while (true) {
			if (*(uint8_t *)((char *)pool + POOL_OFF_UNS_89) != 0) {
				suspend_stratum(pool);
				return 0;
			}
			/* 无积压数据且连接不需要时: 挂起并等待重新启用 */
			if (sock_full(pool) == 0 && cnx_needed(pool) == 0) {
				suspend_stratum(pool);
				clear_stratum_shares(pool);
				clear_pool_work(pool);
				while (cnx_needed(pool) == 0 &&
				       (*(int *)((char *)pool + POOL_OFF_STATE) == 0 ||
				        (current_pool() != pool &&
				         DAT_00076efc != 3 && DAT_00076efc != 4))) {
					if (pthread_mutex_lock(&DAT_00077060) != 0)
						mutex_lock_fail_cgminer("wait_lpcurrent", 0x2420);
					pthread_cond_wait(&DAT_00077078, &DAT_00077060);
					if (pthread_mutex_unlock(&DAT_00077060) != 0)
						mutex_unlock_fail_cgminer("wait_lpcurrent", 0x2422);
					DAT_00075ed8();
				}
				while (true) {
					if (restart_stratum(pool) != 0)
						break;
					if (*(uint8_t *)((char *)pool + POOL_OFF_UNS_89) != 0)
						return 0;
					/* 仅剩一个启用池时用短间隔轮询 */
					cgsleep_ms(DAT_0007b5e4 < 2 ? 3000 : 30000);
				}
			}
			/* 等待 socket 数据 (最长 90 秒) */
			FD_ZERO(&fds);
			i = *(int *)((char *)pool + POOL_OFF_SOCK);
			timeout.tv_sec = 0x5a;
			timeout.tv_usec = 0;
			FD_SET(i, &fds);
			if (sock_full(pool) != 0)
				goto got_line;
			sel = select(i + 1, &fds, NULL, NULL, &timeout);
			if (sel > 0)
				goto got_line;
			/* 连接失败/超时 */
			if (DAT_00078e09 != 0 && LOG_GATE7) {
				snprintf(msg, 0x1000, "Stratum select failed on pool %d with value %d",
				         *(int *)((char *)pool + POOL_OFF_POOL_NO), sel);
				applog(LOG_DEBUG, msg, 0);
			}
			if (DAT_00078e09 != 0 && LOG_GATE7) {
				snprintf(msg, 0x1000, "Stratum connection to pool %d interrupted",
				         *(int *)((char *)pool + POOL_OFF_POOL_NO));
				applog(LOG_DEBUG, msg, 0);
			}
			goto connection_lost;
got_line:
			line = recv_line(pool);
			if (line == NULL) {
				if (DAT_00078e09 != 0 && LOG_GATE7) {
					snprintf(msg, 0x1000, "Stratum connection to pool %d interrupted",
					         *(int *)((char *)pool + POOL_OFF_POOL_NO));
					applog(LOG_DEBUG, msg, 0);
				}
				goto connection_lost;
			}
			stratum_resumed(pool);
			if (parse_notify_equihash(pool, line) != 0)
				goto after_line;            /* 方法消息 (notify/难度/重连) 已消化 */

			val = json_loads(line, 0, &err);
			if (val == NULL) {
				if (DAT_00078e09 != 0 && LOG_GATE7) {
					snprintf(msg, 0x1000, "JSON decode failed(%d): %s", err.line, err.text);
					applog(LOG_DEBUG, msg, 0);
				}
				goto unknown_msg;
			}
			res_val = json_object_get(val, "result");
			err_val = json_object_get(val, "error");
			meth_val = json_object_get(val, DAT_00061ca0 /* "method" */);
			if (meth_val == NULL || meth_val->type == 7 /* json_is_null */) {
				/* 非方法应答 (缺 method 字段) */
				if (err_val == NULL) {
					reason = malloc(0x11);
					if (reason != NULL)
						strncpy(reason, "(unknown reason)", 0x11);
				} else {
					reason = json_dumps(err_val, 3);
				}
				if (DAT_00078e09 != 0 && LOG_GATE7) {
					snprintf(msg, 0x1000, "JSON-RPC non method decode failed: %s", reason);
					applog(LOG_DEBUG, msg, 0);
				}
				share_found = false;
				free(reason);
			} else {
				id = (uint32_t)json_integer_value(meth_val);
				if (pthread_mutex_lock(&DAT_00077030) != 0)
					mutex_lock_fail_cgminer("parse_stratum_response", 0x1b09);
				share = NULL;
				if (DAT_00077048 != NULL) {
					/* uthash: tbl 经首个 share 的 hh.tbl 取得 */
					tbl = *(uint32_t **)DAT_00077048;
					/* HASH_FIND_INT 4 字节键 (HASH_JEN Jenkins mix, 编译器
					 * 常量折叠形态, 逐字保留) */
					{
						uint32_t a = ((id & 0xff) + 0x112410d + (id & 0xff000000) +
							     ((id >> 0x10) & 0xff) * 0x10000 +
							     ((id >> 8) & 0xff) * 0x100) ^ 0x7f76d;
						uint32_t b = (0x9f49bac6 - a) ^ (a << 8);
						uint32_t c = ((-0x112410d - a) - b) ^ (b >> 0xd);
						uint32_t hash;
						buckets = (void *)(uintptr_t)*(uint32_t *)tbl;
						a = ((a - b) - c) ^ (c >> 0xc);
						b = ((b - c) - a) ^ (a << 0x10);
						c = ((c - a) - b) ^ (b >> 5);
						a = ((a - b) - c) ^ (c >> 3);
						b = ((b - c) - a) ^ (a << 10);
						hash = ((c - a) - b) ^ (b >> 0xf);
						{
							uint32_t entry = *(uint32_t *)((char *)buckets +
								     (hash & (tbl[1] - 1)) * 0xc);
							share = (entry == 0) ? NULL
								: (stratum_share_t *)(entry - tbl[5]);
						}
					}
					for (; share != NULL;
					     share = (stratum_share_t *)(*(uint32_t *)((char *)share + 0x10) - tbl[5])) {
						if (*(uint32_t *)((char *)share + 0x18) == 4 &&
						    memcmp(*(void **)((char *)share + 0x14), &id, 4) == 0)
							break;              /* 键匹配 */
						if (*(int32_t *)((char *)share + 0x10) == 0) {
							share = NULL;       /* 桶链尽 */
							break;
						}
					}
				}
				if (share != NULL) {
					/* HASH_DELETE 内联 (uthash) */
					uint32_t prev = share->hh_prev;
					if (prev == 0 && share->hh_next == 0) {
						/* 表中最后一项: HASH_CLEAR */
						free(buckets);
						free(*(void **)DAT_00077048);
						DAT_00077048 = NULL;
					} else {
						uint32_t *next;
						if ((char *)share == (char *)tbl[4] - tbl[5]) {
							tbl[4] = prev + tbl[5];    /* tbl->tail = hh->prev */
							prev = share->hh_prev;
						}
						if (prev == 0) {
							/* 删除表头 */
							next = (uint32_t *)share->hh_next;
							DAT_00077048 = (int *)next;
						} else {
							/* hh->prev->hh_next = hh->next (next 项的 hh.tbl 取 hho) */
							*(uint32_t *)(prev + *(uint32_t *)(*(uint32_t *)DAT_00077048 + 0x14) + 8) =
								share->hh_next;
							next = (uint32_t *)share->hh_next;
						}
						if (next != NULL)
							*(uint32_t *)((char *)next + *(uint32_t *)(*(uint32_t *)DAT_00077048 + 0x14) + 4) =
								share->hh_prev;
						{
							/* 桶链摘除 + 计数维护 */
							uint32_t *tblp = *(uint32_t **)DAT_00077048;
							uint32_t bi = (share->hh_hashv) & (tblp[1] - 1);
							uint32_t *bucket = (uint32_t *)(*(uint32_t *)tblp + bi * 0xc);
							bucket[1]--;
							if ((void *)share == *(void **)bucket)
								*(uint32_t *)bucket = share->hh_hhnext;
							if (share->hh_hhprev != 0)
								*(uint32_t *)(share->hh_hhprev + 0x10) = share->hh_hhnext;
							if (share->hh_hhnext != 0)
								*(uint32_t *)(share->hh_hhnext + 0xc) = share->hh_hhprev;
							*(int32_t *)((char *)tblp + 0xc) -= 1;   /* tbl->num_items-- */
						}
					}
					*(int *)((char *)pool + POOL_OFF_UNS_2FC) -= 1;
				}
				if (pthread_mutex_unlock(&DAT_00077030) != 0)
					mutex_unlock_fail_cgminer("parse_stratum_response", 0x1b10);
				DAT_00075ed8();
				if (share == NULL) {
					if (res_val == NULL) {
						share_found = false;
					} else {
						/* 无对应提交记录的 "untracked" 应答: 仍按接受/拒绝统计 */
						if (pthread_mutex_lock((pthread_mutex_t *)((char *)pool + POOL_OFF_DATA_LOCK_M)) != 0)
							mutex_lock_fail_cgminer("parse_stratum_response", 0x1b1a);
						if (pthread_rwlock_rdlock((pthread_rwlock_t *)((char *)pool + POOL_OFF_DATA_LOCK_RW)) != 0)
							rd_lock_error("parse_stratum_response", 0x1b1a);
						if (pthread_mutex_unlock((pthread_mutex_t *)((char *)pool + POOL_OFF_DATA_LOCK_M)) != 0)
							mutex_unlock_fail_cgminer("parse_stratum_response", 0x1b1a);
						diff = *(double *)((char *)pool + POOL_OFF_DIFF);
						if (pthread_rwlock_unlock((pthread_rwlock_t *)((char *)pool + POOL_OFF_DATA_LOCK_RW)) != 0)
							rw_unlock_error("parse_stratum_response", 0x1b1c);
						DAT_00075ed8();
						if (res_val->type == 5 /* json_is_true */) {
							if (DAT_00078e09 != 0 && LOG_GATE7) {
								snprintf(msg, 0x1000, "Accepted untracked stratum share from pool %d",
								         *(int *)((char *)pool + POOL_OFF_POOL_NO));
								applog(LOG_DEBUG, msg, 0);
							}
							if (pthread_mutex_lock(&DAT_0007ad4c) != 0)
								mutex_lock_fail_cgminer("parse_stratum_response", 0x1b24);
							/* pool->accepted++ (uint64 含进位) */
							(*(uint32_t *)((char *)pool + POOL_OFF_ACCEPTED))++;
							if (*(uint32_t *)((char *)pool + POOL_OFF_ACCEPTED) == 0)
								(*(uint32_t *)((char *)pool + POOL_OFF_ACCEPTED + 4))++;
							/* 总接受份额 ++ (uint64) */
							carry = (0xfffffffe < DAT_0007b420) ? 1 : 0;
							DAT_0007b420++;
							DAT_0007b424 += carry;
							DAT_0007b5e8 += diff;
							*(double *)((char *)pool + POOL_OFF_ACC_DIFF) += diff;
							if (pthread_mutex_unlock(&DAT_0007ad4c) != 0)
								mutex_unlock_fail_cgminer("parse_stratum_response", 0x1b29);
						} else {
							if (DAT_00078e09 != 0 && LOG_GATE7) {
								snprintf(msg, 0x1000, "Rejected untracked stratum share from pool %d",
								         *(int *)((char *)pool + POOL_OFF_POOL_NO));
								applog(LOG_DEBUG, msg, 0);
							}
							if (pthread_mutex_lock(&DAT_0007ad4c) != 0)
								mutex_lock_fail_cgminer("parse_stratum_response", 0x1b2f);
							(*(uint32_t *)((char *)pool + POOL_OFF_REJECTED))++;
							if (*(uint32_t *)((char *)pool + POOL_OFF_REJECTED) == 0)
								(*(uint32_t *)((char *)pool + POOL_OFF_REJECTED + 4))++;
							carry = (0xfffffffe < DAT_0007b6f8) ? 1 : 0;
							DAT_0007b6f8++;
							DAT_0007b6fc += carry;
							DAT_0007b670 += diff;
							*(double *)((char *)pool + POOL_OFF_REJ_DIFF) += diff;
							if (pthread_mutex_unlock(&DAT_0007ad4c) != 0)
								mutex_unlock_fail_cgminer("parse_stratum_response", 0x1b34);
						}
						share_found = false;
						DAT_00075ed8();
					}
				} else {
					work = share->work;
					now = time(NULL);
					if (DAT_00078e09 != 0 && LOG_GATE7) {
						snprintf(msg, 0x1000, "Pool %d stratum share result lag time %d seconds",
						         *(int *)((char *)(*(void **)((char *)work + WORK_OFF_POOL)) + POOL_OFF_POOL_NO),
						         (int)(now - share->stratum_time));
						applog(LOG_DEBUG, msg, 0);
					}
					cgpu = *(int **)(*(int *)((char *)get_thread(*(int *)((char *)work + WORK_OFF_THR_ID)) + THR_OFF_CGPU));
					/* work->data 头 32 字节 (0xC0..0xDF) 以大端字序写入缓冲,
					 * 找到首个非零字节后取其后的字作为显示 nonce */
					{
						uint32_t w;
						w = *(uint32_t *)((char *)work + 0xdc);
						((uint32_t *)msg)[0] = __builtin_bswap32(w);
						w = *(uint32_t *)((char *)work + 0xd8);
						((uint32_t *)msg)[1] = __builtin_bswap32(w);
						w = *(uint32_t *)((char *)work + 0xd4);
						((uint32_t *)msg)[2] = __builtin_bswap32(w);
						w = *(uint32_t *)((char *)work + 0xd0);
						((uint32_t *)msg)[3] = __builtin_bswap32(w);
						w = *(uint32_t *)((char *)work + 0xcc);
						((uint32_t *)msg)[4] = __builtin_bswap32(w);
						w = *(uint32_t *)((char *)work + 0xc8);
						((uint32_t *)msg)[5] = __builtin_bswap32(w);
						w = *(uint32_t *)((char *)work + 0xc4);
						((uint32_t *)msg)[6] = __builtin_bswap32(w);
						w = *(uint32_t *)((char *)work + 0xc0);
						((uint32_t *)msg)[7] = __builtin_bswap32(w);
					}
					for (i = 0; i != 0x1d; i++) {
						if (msg[i] != '\0')
							break;
					}
					diff = *(double *)((char *)work + WORK_OFF_DIFFICULTY);
					{
						uint32_t shown = *(uint32_t *)(msg + i);
						shown = __builtin_bswap32(shown);
						suffix_string(*(double *)((char *)work + WORK_OFF_UNS_6B8), diffstr, 0x10, 0);
						snprintf(hashshow, 0x40, "%08lx Diff %s/%llu%s",
						         (unsigned long)shown, diffstr,
						         (unsigned long long)__fixunsdfdi(round(diff)),
						         *(uint8_t *)((char *)work + WORK_OFF_BLOCK) != 0 ? " BLOCK!" : "");
					}
					{
						int *shpool = *(int **)((char *)work + WORK_OFF_POOL);
						seq_rejects = *(int *)((char *)shpool + POOL_OFF_SEQ_REJECTS);
					}
					if (res_val == NULL ||
					    (res_val->type != 5 /* TRUE */ &&
					     (*(uint8_t *)((char *)work + WORK_OFF_UNS_714) == 0 ||
					      res_val->type != 7 /* NULL */))) {
						goto share_rejected;
					}
					goto share_accepted;
				}
			}
			goto decref_val;
share_rejected:
			/* --- 拒绝: 卡/池/全局三级计数 --- */
			if (pthread_mutex_lock(&DAT_0007ad4c) != 0)
				mutex_lock_fail_cgminer("share_result", 0xdcb);
			(*(int *)((char *)cgpu + CGPU_OFF_REJECTED))++;
			{
				int *shpool = *(int **)((char *)work + WORK_OFF_POOL);
				(*(uint32_t *)((char *)shpool + POOL_OFF_REJECTED))++;
				if (*(uint32_t *)((char *)shpool + POOL_OFF_REJECTED) == 0)
					(*(uint32_t *)((char *)shpool + POOL_OFF_REJECTED + 4))++;
				carry = (0xfffffffe < DAT_0007b6f8) ? 1 : 0;
				DAT_0007b6f8++;
				DAT_0007b6fc += carry;
				*(double *)((char *)cgpu + CGPU_OFF_REJ_DIFF) += diff;
				DAT_0007b670 += diff;
				*(int *)((char *)shpool + POOL_OFF_SEQ_REJECTS) += 1;
				*(double *)((char *)shpool + POOL_OFF_REJ_DIFF) += diff;
			}
			if (pthread_mutex_unlock(&DAT_0007ad4c) != 0)
				mutex_unlock_fail_cgminer("share_result", 0xdd3);
			DAT_00075ed8();
			if (DAT_00078e09 != 0 && LOG_GATE7) {
				/* 原码为编译器对常量串 "PROOF OF WORK RESULT: false (booooo)"
				 * 的逐字写, 此处还原为等价字面量 */
				snprintf(proofbuf, sizeof(proofbuf), "%s", "PROOF OF WORK RESULT: false (booooo)");
				applog(LOG_DEBUG, proofbuf, 0);
			}
			if (DAT_0007bc78 == 0 && DAT_0007bb18 == 0) {
				/* sharelog 标签: "reject" 或 "reject:<原因>" (fork 合并形态) */
				memcpy(sharetype, "reject", 7);
				memset(sharetype + 7, 0, 0x1d);
				reasonparen[0] = (char)DAT_0007bb18;    /* 恒 0 (进入本分支的条件) */
				if (DAT_0007b198 < 2) {
					poolstr[0] = '\0';
				} else {
					snprintf(poolstr, 0x14, "pool %d",
					         *(int *)((char *)*(void **)((char *)work + WORK_OFF_POOL) + POOL_OFF_POOL_NO));
				}
				/* 原码复用 res_val 变量: work+0x714 未置位时改取 "reject-reason" */
				reason_val = res_val;
				if (*(uint8_t *)((char *)work + WORK_OFF_UNS_714) == 0)
					reason_val = json_object_get(val, "reject-reason");
				if (reason_val == NULL) {
					if (*(uint8_t *)((char *)work + WORK_OFF_UNS_6EC) != 0 && err_val != NULL) {
						if (err_val->type == 1 /* json_is_array */) {
							reason_val = json_array_get(err_val, 0);
							if (reason_val != NULL && reason_val->type == 2 /* STRING */)
								goto have_reason_paren;
						} else if (err_val->type == 2 /* json_is_string */) {
have_reason_paren:
							rs = json_string_value(reason_val);
							snprintf(reasonparen, 0x1f, " (%s)", rs);
						}
					}
				} else {
					rs = json_string_value(reason_val);
					n = strlen(rs);
					reasonparen[0] = ' ';
					reasonparen[1] = '(';
					if (0x1b < n)
						n = 0x1c;
					memcpy(reasonparen + 2, rs, n);
					reasonparen[n + 3] = '\0';
					reasonparen[n + 2] = ')';
					memcpy(sharetype + 7, rs, n);
					sharetype[6] = ':';                 /* "reject" + ':' + 原因 */
					sharetype[7 + n] = '\0';
				}
				if (DAT_00078e09 != 0 && LOG_GATE7) {
					snprintf(msg, 0x1000, "Rejected %s %s %d %s%s %s%s", hashshow,
					         *(char **)(*(int *)((char *)cgpu + CGPU_OFF_DRV) + 8),
					         *(int *)((char *)cgpu + CGPU_OFF_DEV_ID), poolstr,
					         reasonparen, "", "");
					applog(LOG_DEBUG, msg, 0);
				}
				sharelog(sharetype, work);
			}
			/* 连续拒绝过多 → 禁用池 */
			{
				int *shpool = *(int **)((char *)work + WORK_OFF_POOL);
				seq_rejects = *(int *)((char *)shpool + POOL_OFF_SEQ_REJECTS);
				if (10 < seq_rejects &&
				    *(uint8_t *)((char *)work + WORK_OFF_UNS_6E9) == 0 &&
				    DAT_0007b250 != 0 && 1 < DAT_0007b5e4) {
					/* 原 fpscr 位域比较: !Z && N==V → seq_rejects > 阈值 */
					threshold = (double)__floatdidf(
					        (unsigned long long)DAT_0007b424 << 32 | DAT_0007b420);
					threshold = (threshold / DAT_000755f8) * 60.0 * 3.0;
					if ((double)seq_rejects > threshold) {
						if (LOG_GATE4) {
							snprintf(msg, 0x1000,
							         "Pool %d rejected %d sequential shares, disabling!",
							         *(int *)((char *)shpool + POOL_OFF_POOL_NO), seq_rejects);
							applog(LOG_WARNING, msg, 0);
						}
						if (*(int *)((char *)shpool + POOL_OFF_STATE) == 1)
							DAT_0007b5e4--;
						*(int *)((char *)shpool + POOL_OFF_STATE) = 2;
						if (current_pool() == shpool)
							switch_pools(NULL);
						*(int *)((char *)shpool + POOL_OFF_SEQ_REJECTS) = 0;
					}
				}
			}
			goto share_tail;
share_accepted:
			/* --- 接受: 卡/池/全局三级计数 --- */
			if (pthread_mutex_lock(&DAT_0007ad4c) != 0)
				mutex_lock_fail_cgminer("share_result", 0xd99);
			(*(int *)((char *)cgpu + CGPU_OFF_ACCEPTED))++;
			{
				int *shpool = *(int **)((char *)work + WORK_OFF_POOL);
				(*(uint32_t *)((char *)shpool + POOL_OFF_ACCEPTED))++;
				if (*(uint32_t *)((char *)shpool + POOL_OFF_ACCEPTED) == 0)
					(*(uint32_t *)((char *)shpool + POOL_OFF_ACCEPTED + 4))++;
				carry = (0xfffffffe < DAT_0007b420) ? 1 : 0;
				DAT_0007b420++;
				DAT_0007b424 += carry;
				*(double *)((char *)cgpu + CGPU_OFF_ACC_DIFF) += diff;
				DAT_0007b5e8 += diff;
				*(double *)((char *)shpool + POOL_OFF_ACC_DIFF) += diff;
			}
			if (pthread_mutex_unlock(&DAT_0007ad4c) != 0)
				mutex_unlock_fail_cgminer("share_result", 0xda0);
			DAT_00075ed8();
			{
				int *shpool = *(int **)((char *)work + WORK_OFF_POOL);
				*(int *)((char *)shpool + POOL_OFF_SEQ_REJECTS) = 0;
				*(int *)((char *)cgpu + CGPU_OFF_LAST_POOL) =
				        *(int *)((char *)shpool + POOL_OFF_POOL_NO);
				now = time(NULL);
				*(time_t *)((char *)cgpu + CGPU_OFF_LAST_VALID) = now;
				*(int *)((char *)cgpu + CGPU_OFF_LAST_DIFF) =
				        *(int *)((char *)work + WORK_OFF_DIFFICULTY);
				*(int *)((char *)cgpu + CGPU_OFF_LAST_DIFF + 4) =
				        *(int *)((char *)work + WORK_OFF_DIFFICULTY + 4);
				*(int *)((char *)shpool + POOL_OFF_LAST_SH_TIME) = (int)now;
				*(int *)((char *)shpool + POOL_OFF_LAST_SH_DIFF) =
				        *(int *)((char *)work + WORK_OFF_DIFFICULTY);
				*(int *)((char *)shpool + POOL_OFF_LAST_SH_DIFF + 4) =
				        *(int *)((char *)work + WORK_OFF_DIFFICULTY + 4);
				*(int *)((char *)shpool + 0x18C) =
				        *(int *)((char *)work + WORK_OFF_DIFFICULTY + 4);
			}
			if (DAT_00078e09 != 0 && LOG_GATE7) {
				snprintf(proofbuf, sizeof(proofbuf), "%s", "PROOF OF WORK RESULT: true (yay!!!)");
				applog(LOG_DEBUG, proofbuf, 0);
			}
			if (DAT_0007bc78 == 0 && DAT_0007bb18 == 0) {
				{
					int *shpool = *(int **)((char *)work + WORK_OFF_POOL);
					if (DAT_0007b198 < 2) {
						if (DAT_00078e09 != 0 && LOG_GATE7) {
							snprintf(msg, 0x1000, "Accepted %s %s %d %s%s", hashshow,
							         *(char **)(*(int *)((char *)cgpu + CGPU_OFF_DRV) + 8),
							         *(int *)((char *)cgpu + CGPU_OFF_DEV_ID), "", "");
							applog(LOG_DEBUG, msg, 0);
						}
					} else if (DAT_00078e09 != 0 && LOG_GATE7) {
						snprintf(msg, 0x1000, "Accepted %s %s %d pool %d %s%s", hashshow,
						         *(char **)(*(int *)((char *)cgpu + CGPU_OFF_DRV) + 8),
						         *(int *)((char *)cgpu + CGPU_OFF_DEV_ID),
						         *(int *)((char *)shpool + POOL_OFF_POOL_NO), "", "");
						applog(LOG_DEBUG, msg, 0);
					}
				}
				sharelog("accept", work);
				if (DAT_00077288 != 0) {
					/* 原 fpscr 位域比较: !(总累计 <= 目标) || 相等 → 达标退出 */
					if ((double)DAT_00077288 <= DAT_0007b5e8) {
						if (LOG_GATE4) {
							snprintf(msg, 0x1000,
							         "Successfully mined %d accepted shares as requested and exiting.",
							         DAT_00077288);
							applog(LOG_WARNING, msg, 0);
						}
						kill_work();
						goto share_tail;
					}
				}
				{
					int *shpool = *(int **)((char *)work + WORK_OFF_POOL);
					if (*(int *)((char *)shpool + POOL_OFF_STATE) == 2) {
						if (DAT_0007ad48 == 0 && DAT_00078e08 == 0 && DAT_000766c4 < 4) {
reenable_pool:                     /* 原标签 LAB_000216ac */
							*(int *)((char *)shpool + POOL_OFF_STATE) = 1;
							DAT_0007b5e4++;
						} else {
							snprintf(msg, 0x1000,
							         "Rejecting pool %d now accepting shares, re-enabling!",
							         *(int *)((char *)shpool + POOL_OFF_POOL_NO));
							applog(LOG_WARNING, msg, 0);
							if (*(int *)((char *)shpool + POOL_OFF_STATE) != 1)
								goto reenable_pool;
						}
						switch_pools(NULL);
					}
				}
				if (*(uint8_t *)((char *)work + WORK_OFF_BLOCK) != 0)
					restart_threads();
			}
share_tail:
			share_found = true;
			free_work(share->work);
			share->work = NULL;
			free(share);
decref_val:
			if (val->refcount != -1 && --val->refcount == 0)
				json_delete(val);
			if (share_found)
				goto after_line;
unknown_msg:
			if (DAT_00078e09 != 0 && LOG_GATE7) {
				snprintf(msg, 0x1000, "Unknown stratum msg: %s", line);
				applog(LOG_DEBUG, msg, 0);
			}
			free(line);
			continue;
after_line:
			/* 池重启标志: 补发一份 stratum work */
			if (*(uint8_t *)((char *)pool + POOL_OFF_RESTART) != 0) {
				void *w = make_work();
				*(uint8_t *)((char *)pool + POOL_OFF_RESTART) = 0;
				gen_stratum_work(pool, w);
				*(uint8_t *)((char *)w + WORK_OFF_LONGPOLL) = 1;
				test_work_current(w);
				free_work(w);
			}
			free(line);
		}
connection_lost:
		/* 连接丢失: 计数 + 判断是否支持会话恢复 */
		(*(int *)((char *)pool + POOL_OFF_GETFAIL))++;
		DAT_0007b43c++;
		if (pthread_mutex_lock((pthread_mutex_t *)((char *)pool + POOL_OFF_DATA_LOCK_M)) != 0)
			mutex_lock_fail_cgminer("supports_resume", 0x1bbd);
		if (pthread_rwlock_rdlock((pthread_rwlock_t *)((char *)pool + POOL_OFF_DATA_LOCK_RW)) != 0)
			rd_lock_error("supports_resume", 0x1bbd);
		if (pthread_mutex_unlock((pthread_mutex_t *)((char *)pool + POOL_OFF_DATA_LOCK_M)) != 0)
			mutex_unlock_fail_cgminer("supports_resume", 0x1bbd);
		{
			int resumed = *(int *)((char *)pool + POOL_OFF_SESSION_ID);
			if (pthread_rwlock_unlock((pthread_rwlock_t *)((char *)pool + POOL_OFF_DATA_LOCK_RW)) != 0)
				rw_unlock_error("supports_resume", 0x1bbf);
			DAT_00075ed8();
			if (resumed == 0 || DAT_0007b644 != 0)
				clear_stratum_shares(pool);
		}
		clear_pool_work(pool);
		if (current_pool() == pool)
			restart_threads();
		while (restart_stratum(pool) == 0) {
			if (*(uint8_t *)((char *)pool + POOL_OFF_UNS_89) != 0)
				return 0;
			cgsleep_ms(30000);
		}
	} while (true);
}

// === display_pools === 原 FUN_00026f50 @ 0x00026f50
// 来源: reversed/cgminer/21_pool_net/FUN_00026f50.c | 分类: 21_pool_net
// 语义: curses 矿池管理界面: 列出全部池 (当前池/禁用池加属性高亮, 逐行打印
//       编号/状态/存活/配额/优先级/URL/用户), 随后显示管理菜单并等待按键:
//       A 添加, R 移除, S 启用并切换, D 禁用, E 启用(优先级低则切换),
//       C 修改管理策略, I 池详情, Q 修改配额, F 切换 failover-only,
//       其他键退出界面。操作后重绘列表或仅重显菜单。
void display_pools(void)
{
	char display[4096];             /* 原 local_1030 (0x1000): 菜单/行打印缓冲 */
	int i;                          /* 原 iVar4: 已打印到第几个池 / 复用临时量 */
	int sel;                        /* 菜单数字选择 */
	char key;                       /* 原 cStack_1031: 按键 */
	char *status;                   /* 原 pcVar8: Alive/Dead 等 */
	int old_state;                  /* 原 iVar3 复用: 旧状态/优先级比较 */
	int pool_state;                 /* 原 iVar7 复用: 池状态 */
	int *target;                    /* 原 iVar7: 选中的池 */
	int *cur;                       /* current_pool() 结果 */
	long long submissions;          /* accepted + rejected (64 位加) */
	double rej_num, rej_den;        /* 拒绝率分子/分母 */
	uint32_t acc_lo, acc_hi, rej_lo, rej_hi;

	DAT_0007bac8 = 1;
	immedok(DAT_0007b430, 1);
	clear_logwin();
	i = 0;
	for (;;) {
redraw:                             /* 原标签 LAB_00026fa0 */
		if (DAT_0007b198 <= i) {
menu:                               /* 原标签 LAB_00026fac */
			snprintf(display, 0x1000, "\nCurrent pool management strategy: %s\n",
			         DAT_00075e10[DAT_00076efc]);
			_wlogprint(display);
			if (DAT_00076efc == 2) {
				snprintf(display, 0x1000, "Set to rotate every %d minutes\n", DAT_0007b558);
				_wlogprint(display);
			}
			status = "disabled";
			if (DAT_0007b625 != 0)
				status = "enabled";
			snprintf(display, 0x1000, "[F]ailover only %s\n", status);
			_wlogprint(display);
			/* 原码为编译器对常量串的逐字写, 还原为字面量 */
			_wlogprint("Pool [A]dd [R]emove [D]isable [E]nable [Q]uota change\n");
			_wlogprint("[C]hange management strategy [S]witch pool [I]nformation\n");
			_wlogprint("Or press any other key to continue\n");
			logwin_update();
			key = (char)wgetch(stdscr);
			if (strncasecmp(&key, "a", 1) == 0) {
				input_pool(1);
				i = 0;
				goto redraw;
			} else if (strncasecmp(&key, "r", 1) == 0) {
				if (DAT_0007b198 < 2) {
					_wlogprint("Cannot remove last pool");
					goto menu;
				}
				sel = curses_int("Select pool number");
				if (-1 < sel && sel < DAT_0007b198) {
					target = *(int **)((char *)DAT_0007b438 + sel * 4);
					cur = current_pool();
					if (target == cur) {
						switch_pools(NULL);
						cur = current_pool();
					} else {
						cur = current_pool();
					}
					if (target != cur) {
						old_state = *(int *)((char *)target + POOL_OFF_STATE);
						i = 0;
						*(int *)((char *)target + POOL_OFF_STATE) = 0;
						if (old_state == 1)
							DAT_0007b5e4--;
						remove_pool(target);
						goto redraw;
					}
					_wlogprint("Unable to remove pool due to activity\n");
					goto menu;
				}
			} else if (strncasecmp(&key, "s", 1) == 0) {
				sel = curses_int("Select pool number");
				if (sel < 0 || DAT_0007b198 <= sel)
					goto invalid;
				target = *(int **)((char *)DAT_0007b438 + sel * 4);
				if (*(int *)((char *)target + POOL_OFF_STATE) != 1) {
					*(int *)((char *)target + POOL_OFF_STATE) = 1;
					DAT_0007b5e4++;
				}
switch_enable:                      /* 原标签 LAB_00027470 */
				i = 0;
				switch_pools(target);
				goto redraw;
			} else if (strncasecmp(&key, "d", 1) == 0) {
				if (DAT_0007b5e4 < 2) {
					_wlogprint("Cannot disable last pool");
					goto menu;
				}
				sel = curses_int("Select pool number");
				if (sel < 0 || DAT_0007b198 <= sel)
					goto invalid;
				target = *(int **)((char *)DAT_0007b438 + sel * 4);
				if (*(int *)((char *)target + POOL_OFF_STATE) == 1)
					DAT_0007b5e4--;
				i = 0;
				*(int *)((char *)target + POOL_OFF_STATE) = 0;
				if (target == current_pool()) {
					switch_pools(NULL);
					goto redraw;
				}
				goto redraw;                /* 原标签 LAB_000271d8 路径 (等价重绘) */
			} else if (strncasecmp(&key, "e", 1) == 0) {
				sel = curses_int("Select pool number");
				if (-1 < sel && sel < DAT_0007b198) {
					target = *(int **)((char *)DAT_0007b438 + sel * 4);
					if (*(int *)((char *)target + POOL_OFF_STATE) != 1) {
						*(int *)((char *)target + POOL_OFF_STATE) = 1;
						DAT_0007b5e4++;
					}
					old_state = *(int *)((char *)target + POOL_OFF_PRIO);
					if (old_state < *(int *)((char *)current_pool() + POOL_OFF_PRIO))
						goto switch_enable;
					goto redraw;            /* 原标签 LAB_000271d8 路径 */
				}
			} else if (strncasecmp(&key, "c", 1) == 0) {
				for (i = 0; i != 5; i++) {
					snprintf(display, 0x1000, "%d: %s\n", i, DAT_00075e10[i]);
					_wlogprint(display);
				}
				sel = curses_int("Select strategy number type");
				if ((unsigned int)sel < 5) {
					if (sel != 2 ||
					    ((DAT_0007b558 = curses_int("Select interval in minutes")),
					     DAT_0007b558 < 10000)) {
						i = 0;
						DAT_00076efc = sel;
						switch_pools(NULL);
						goto redraw;
					}
					DAT_0007b558 = 0;       /* 轮换间隔非法: 清零且不修改策略 */
				}
			} else if (strncasecmp(&key, "i", 1) == 0) {
				sel = curses_int("Select pool number");
				if (-1 < sel && sel < DAT_0007b198) {
					target = *(int **)((char *)DAT_0007b438 + sel * 4);
					if (curses_active_locked() != 0) {
						wlogprint("Pool: %s\n", *(char **)((char *)target + POOL_OFF_RPC_URL));
						old_state = *(int *)((char *)target + POOL_OFF_SOLVED);
						if (old_state != 0) {
							status = "S";
							if (old_state < 2)
								status = "";
							wlogprint("SOLVED %d BLOCK%s!\n", old_state, status);
						}
						if (*(uint8_t *)((char *)target + POOL_OFF_UNS_298) == 0) {
							status = "Does not have";
							if (*(int *)((char *)target + POOL_OFF_HAS_LONGPOLL) != 0)
								status = "Has";
							wlogprint("%s own long-poll support\n", status);
						}
						wlogprint(" Queued work requests: %d\n",
						          *(int *)((char *)target + POOL_OFF_QUEUED));
						submissions = (long long)(*(unsigned long long *)((char *)target + POOL_OFF_ACCEPTED) +
						                          *(unsigned long long *)((char *)target + POOL_OFF_REJECTED));
						wlogprint(" Share submissions: %lld\n", submissions);
						wlogprint(" Accepted shares: %lld\n",
						          *(long long *)((char *)target + POOL_OFF_ACCEPTED));
						wlogprint(" Rejected shares: %lld\n",
						          *(long long *)((char *)target + POOL_OFF_REJECTED));
						wlogprint(" Accepted difficulty shares: %1.f\n",
						          *(double *)((char *)target + POOL_OFF_ACC_DIFF));
						wlogprint(" Rejected difficulty shares: %1.f\n",
						          *(double *)((char *)target + POOL_OFF_REJ_DIFF));
						acc_lo = *(uint32_t *)((char *)target + POOL_OFF_ACCEPTED);
						acc_hi = *(uint32_t *)((char *)target + POOL_OFF_ACCEPTED + 4);
						rej_lo = *(uint32_t *)((char *)target + POOL_OFF_REJECTED);
						rej_hi = *(uint32_t *)((char *)target + POOL_OFF_REJECTED + 4);
						if ((acc_lo != 0 || acc_hi != 0) || (rej_lo != 0 || rej_hi != 0)) {
							rej_num = (double)__floatdidf(
							        (uint64_t)rej_lo * 100 + ((uint64_t)rej_hi * 100 << 32));
							rej_den = (double)__floatdidf(
							        ((uint64_t)acc_hi << 32 | acc_lo) +
							        ((uint64_t)rej_hi << 32 | rej_lo));
							wlogprint(" Reject ratio: %.1f%%\n", rej_num / rej_den);
						}
						wlogprint(" Items worked on: %d\n", *(int *)((char *)target + 0x44));
						wlogprint(" Discarded work due to new blocks: %d\n",
						          *(int *)((char *)target + 0x9c));
						wlogprint(" Stale submissions discarded due to new blocks: %d\n",
						          *(int *)((char *)target + 0x98));
						wlogprint(" Unable to get work from server occasions: %d\n",
						          *(int *)((char *)target + POOL_OFF_GETFAIL));
						wlogprint(" Submitting work remotely delay occasions: %d\n\n",
						          *(int *)((char *)target + POOL_OFF_REMOTEFAIL));
						if (pthread_mutex_unlock(&DAT_0007b7a8) != 0)
							mutex_unlock_fail_cgminer("unlock_curses", 0xb72);
						DAT_00075ed8();
					}
					goto menu;
				}
			} else if (strncasecmp(&key, "q", 1) == 0) {
				sel = curses_int("Select pool number");
				if (sel < 0 || DAT_0007b198 <= sel)
					goto invalid;
				target = *(int **)((char *)DAT_0007b438 + sel * 4);
				sel = curses_int("Set quota");
				if (sel < 0) {
					_wlogprint("Invalid negative quota\n");
					goto menu;
				}
				*(int *)((char *)target + POOL_OFF_QUOTA) = sel;
				i = 0;
				adjust_quota_gcd();
				goto redraw;
			} else if (strncasecmp(&key, "f", 1) != 0) {
				/* 其他键: 退出管理界面 */
				clear_logwin();
				immedok(DAT_0007b430, 0);
				DAT_0007bac8 = 0;
				return;
			} else {
				DAT_0007b625 = DAT_0007b625 ^ 1;    /* 切换 failover-only */
				i = 0;
				goto redraw;
			}
invalid:                            /* 原标签 LAB_00027124 */
			_wlogprint("Invalid selection\n");
			goto menu;
		}
		/* 逐池打印 (原标签 LAB_000271e8) */
		{
			int *p = *(int **)((char *)DAT_0007b438 + i * 4);
			cur = current_pool();
			if (p == cur)
				wattr_on(DAT_0007b430, 0x200000, 0);
			if (*(int *)((char *)p + POOL_OFF_STATE) != 1)
				wattr_on(DAT_0007b430, 0x100000, 0);
			snprintf(display, 0x1000, "%d: ", *(int *)((char *)p + POOL_OFF_POOL_NO));
			_wlogprint(display);
			pool_state = *(int *)((char *)p + POOL_OFF_STATE);
			/* 原码为常量串逐字写 ("Enabled "/"Disabled "/"Rejecting ") */
			if (pool_state == 1)
				_wlogprint("Enabled ");
			else if (pool_state == 0)
				_wlogprint("Disabled ");
			else if (pool_state == 2)
				_wlogprint("Rejecting ");
			i++;
			status = "Alive";
			if (*(uint8_t *)((char *)p + POOL_OFF_IDLE) != 0)
				status = "Dead";
			snprintf(display, 0x1000, "%s Quota %d Prio %d: %s  User:%s\n", status,
			         *(int *)((char *)p + POOL_OFF_QUOTA),
			         *(int *)((char *)p + POOL_OFF_PRIO),
			         *(char **)((char *)p + POOL_OFF_RPC_URL),
			         *(char **)((char *)p + POOL_OFF_RPC_USER));
			_wlogprint(display);
			wattr_off(DAT_0007b430, 0x300000, 0);
		}
	}
}

// === initiate_stratum === 原 FUN_0002b8e8 @ 0x0002b8e8
// 来源: reversed/cgminer/21_pool_net/FUN_0002b8e8.c | 分类: 21_pool_net
// 语义: 建立/恢复池的 stratum 连接 (内联 setup_stratum_socket): 挂起旧连接并关 socket,
//       解析地址 (无代理时可选启用全局 SOCKS5 代理), 非阻塞 connect 配 1 秒 select
//       等待 EINPROGRESS 完成; 按代理类型协商 (HTTP/SOCKS4 或内联 SOCKS5), 分配
//       0x2000 sockbuf 并设置 TCP keepalive 参数; 发送三形态 mining.subscribe, 等待并
//       解析应答: result 有效且无 error 时 parse_extranonce_equihash 成功 → 置
//       stratum_active (+0x299)、难度写 1.0, 按需补发 mining.extranonce.subscribe 并
//       返回其非 0 结果; 应答已到但解析失败时 (且未重试过) 清 session_id/nonce1 整体
//       重试一次; 其余失败路径打印 "Initiate stratum failed", socket 已建则
//       suspend_stratum, 返回 0。
int initiate_stratum(int *pool)
{
	/* json_error_t 布局 (252 字节): line(+0) column(+4) position(+8)
	 * source[80](+0xC) text[160](+0x5C); 原栈上以 aiStack_3328 起始 */
	typedef struct { int line, column, position; char source[80]; char text[160]; } json_error_layout_t;
	char sendbuf[8196];             /* 原 acStack_2028: subscribe 请求串 */
	char msg[4096];                 /* 原 local_3028 (0x1000): applog/snprintf 复用缓冲 */
	char socks5_req[263];           /* SOCKS5 请求暂存 (原复用 fd_set local_322c 及其后栈区) */
	json_error_layout_t err;        /* 原 aiStack_3328+acStack_32cc */
	fd_set wfds;                    /* 原 local_322c: 非阻塞 connect 等待写集 */
	struct timeval tv;              /* 原 tStack_3350 */
	struct addrinfo hints;          /* 原 local_3348 */
	struct addrinfo *res;           /* 原 paStack_3360 */
	struct addrinfo *ai;            /* 原 paVar18 */
	json_t *val;                    /* 原 local_3380: json_loads 应答对象 */
	json_t *res_val;                /* 原 piVar9: "result" */
	json_t *err_val;                /* 原 piVar12: "error" */
	char *reason;                   /* 原 pcVar13: 拒绝原因串 (需 free) */
	char *line;                     /* 原 pvVar11: recv_line 结果 */
	char *host;                     /* 原 local_337c: 解析主机 */
	char *port;                     /* 原 local_3378: 解析端口 */
	int sock;                       /* 原 iVar5 复用: socket fd */
	int ret;                        /* 原 iVar5/iVar8 复用: 各调用返回值 */
	int id;                         /* DAT_0007b0f8 快照 (原 iVar5/iVar8) */
	int so_error;                   /* 原 iStack_3354: SO_ERROR 结果 */
	socklen_t so_len;               /* 原 sStack_3358: getsockopt optlen */
	int keepalive;                  /* 原 uStack_335c: SO_KEEPALIVE/TCP_NODELAY/TCP_KEEPCNT 值 */
	int keepidle;                   /* 原 sStack_3358 复用: TCP_KEEPIDLE 值 */
	int keepintvl;                  /* 原 iStack_3354 复用: TCP_KEEPINTVL 值 */
	long portnum;                   /* 原 lVar14: strtol(stratum_port) */
	size_t len;                     /* 原 sVar10 */
	int i;                          /* SOCKS5 域名应答读入循环 (原 iVar8/iVar15) */
	char atyp;                      /* 原 cVar4: SOCKS5 应答地址类型 */
	bool sock_established;          /* 原 bVar1: socket 已建立 → 退出时 suspend_stratum */
	bool retried;                   /* 原 bVar2: 已做过一次恢复重试 */
	bool resumed;                   /* 原 bVar3: 已收到应答但解析失败 → 走重试分支 */

	sock_established = false;
	retried = false;
	resumed = false;
	val = NULL;
	do {
		/* ---- setup_stratum_socket 内联 ---- */
		if (pthread_mutex_lock((pthread_mutex_t *)((char *)pool + POOL_OFF_STRATUM_LOCK)) != 0) {
			mutex_lock_fail_util("setup_stratum_socket", 0xc5f);
		}
		*(uint8_t *)((char *)pool + POOL_OFF_UNS_299) = 0;   /* 清 stratum_active */
		if (*(int *)((char *)pool + POOL_OFF_SOCK) != 0) {
			close(*(int *)((char *)pool + POOL_OFF_SOCK));
		}
		*(int *)((char *)pool + POOL_OFF_SOCK) = 0;
		if (pthread_mutex_unlock((pthread_mutex_t *)((char *)pool + POOL_OFF_STRATUM_LOCK)) != 0) {
			snprintf(msg, 0x1000, "WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d",
			         errno, "util.c", "setup_stratum_socket", 0xc64);
			applog(3, msg, 1);
			quit(1);
		}
		(*DAT_00075ed8)();
		hints.ai_family = 0;            /* AF_UNSPEC */
		hints.ai_flags = 0;
		hints.ai_protocol = 0;
		hints.ai_addrlen = 0;
		hints.ai_addr = NULL;
		hints.ai_canonname = NULL;
		hints.ai_next = NULL;
		hints.ai_socktype = 1;          /* SOCK_STREAM */
		if (*(int *)((char *)pool + POOL_OFF_RPC_PROXY) != 0) {
use_proxy:                              /* 原 LAB_0002bb50 */
			host = *(char **)((char *)pool + POOL_OFF_PROXY_URL);
			port = *(char **)((char *)pool + POOL_OFF_PROXY_PORT);
		} else if (DAT_00078b78 != NULL) {
			/* 全局 SOCKS5 代理未在池上配置时, 采用全局代理 (类型固定 SOCKS5) */
			*(char **)((char *)pool + POOL_OFF_RPC_PROXY) = DAT_00078b78;
			extract_sockaddr(DAT_00078b78,
			                 (char **)((char *)pool + POOL_OFF_PROXY_URL),
			                 (char **)((char *)pool + POOL_OFF_PROXY_PORT));
			*(int *)((char *)pool + POOL_OFF_PROXYTYPE) = 3;
			if (*(int *)((char *)pool + POOL_OFF_RPC_PROXY) != 0)
				goto use_proxy;
			host = *(char **)((char *)pool + POOL_OFF_SOCKADDR_URL);
			port = *(char **)((char *)pool + POOL_OFF_STRATUM_PORT);
		} else {
			host = *(char **)((char *)pool + POOL_OFF_SOCKADDR_URL);
			port = *(char **)((char *)pool + POOL_OFF_STRATUM_PORT);
		}
		if (getaddrinfo(host, port, &hints, &res) != 0) {
			/* 解析失败: 授权字节为 0 (首次) 打印 WARNING 并记下, 之后只打 DEBUG */
			if (*(uint8_t *)((char *)pool + POOL_OFF_STRATUM_AUTH) == 0) {
				if (LOG_GATE4) {
					snprintf(msg, 0x1000, "Failed to resolve (?wrong URL) %s:%s", host, port);
					applog(4, msg, 0);
				}
				*(uint8_t *)((char *)pool + POOL_OFF_STRATUM_AUTH) = 1;
			} else if (DAT_00078e09 != 0 && LOG_GATE7) {
				snprintf(msg, 0x1000, "Failed to getaddrinfo for %s:%s", host, port);
				applog(7, msg, 0);
			}
			goto out_no_socket;                 /* 原 LAB_0002b994 */
		}
		for (ai = res; ai != NULL; ai = ai->ai_next) {
			sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
			if (sock == -1) {
				if (DAT_00078e09 != 0 && LOG_GATE7)
					applog(7, "Failed socket", 0);
				continue;                       /* 原 LAB_0002bc18 */
			}
			ret = fcntl(sock, 3 /* F_GETFL */, 0);
			fcntl(sock, 4 /* F_SETFL */, ret | 0x800 /* O_NONBLOCK (Linux 值) */);
			if (connect(sock, ai->ai_addr, ai->ai_addrlen) == -1) {
				tv.tv_sec = 1;
				tv.tv_usec = 0;
				if (errno == 0x73 /* EINPROGRESS */) {
					/* select 等待连接完成 (1 秒), 仅 EINTR 时重试 */
					do {
						FD_ZERO(&wfds);
						FD_SET(sock, &wfds);
						ret = select(sock + 1, NULL, &wfds, NULL, &tv);
					} while (ret < 1 && ret != 0 && errno == 4 /* EINTR */);
					if (ret >= 1 && FD_ISSET(sock, &wfds)) {
						so_len = 4;
						ret = getsockopt(sock, 1 /* SOL_SOCKET */, 4 /* SO_ERROR */,
						                 &so_error, &so_len);
						if (ret == 0 && so_error == 0) {
							if (DAT_00078e09 != 0 && LOG_GATE7)
								applog(7, "Succeeded delayed connect", 0);
							goto connected;     /* 原 LAB_0002bf8c */
						}
					}
					goto select_failed;         /* 原 LAB_0002bd38 */
				}
				close(sock);
				if (DAT_00078e09 != 0 && LOG_GATE7)
					applog(7, "Failed sock connect", 0);
				continue;                       /* 原 LAB_0002bc18 */
			}
			/* connect 立即成功 */
			if (LOG_GATE4)
				applog(4, "Succeeded immediate connect", 0);
			goto connected;                 /* 原 LAB_0002bf8c */
select_failed:                              /* 原 LAB_0002bd38 */
			close(sock);
			if (DAT_00078e09 != 0 && LOG_GATE7)
				applog(7, "Select timeout/failed connect", 0);
			continue;                       /* 原 LAB_0002bc18: 下一个 addrinfo */
		}
		/* 所有 addrinfo 均未连通 (原 LAB_0002be64) */
		if (DAT_00078e09 != 0 && LOG_GATE7) {
			snprintf(msg, 0x1000, "Failed to connect to stratum on %s:%s", host, port);
			applog(7, msg, 0);
		}
		freeaddrinfo(res);
		goto out_no_socket;                 /* 原 LAB_0002b994 */
connected:                                  /* 原 LAB_0002bf8c */
		block_socket(sock);
		freeaddrinfo(res);
		if (*(int *)((char *)pool + POOL_OFF_RPC_PROXY) == 0)
			goto alloc_sockbuf;             /* 原 LAB_0002c0d0: 无代理直接进 sockbuf 分配 */
		/* 代理协商 (enum proxytype: 0=HTTP 1=HTTP_1.0 2=SOCKS4 3=SOCKS5 4=SOCKS4A 5=SOCKS5H) */
		switch (*(int *)((char *)pool + POOL_OFF_PROXYTYPE)) {
		case 0:
			ret = http_negotiate(pool, sock, 0);
			if (ret == 0)
				goto out_no_socket;
			goto alloc_sockbuf;             /* joined_r0x0002c0d8 */
		case 1:
			ret = http_negotiate(pool, sock, 1);
			break;
		case 2:
			ret = socks4_negotiate(pool, sock, 0);
			break;
		case 4:
			ret = socks4_negotiate(pool, sock, 1);
			break;
		case 3:
		case 5:
			/* SOCKS5/SOCKS5H: 内联协商 (握手 05 01 00, CONNECT + 域名寻址) */
			socks5_req[0] = 5;
			socks5_req[1] = 1;
			socks5_req[2] = 0;
			if (DAT_00078e09 != 0 && LOG_GATE7) {
				snprintf(msg, 0x1000, "Attempting to negotiate with %s:%s SOCKS5 proxy",
				         *(char **)((char *)pool + POOL_OFF_PROXY_URL),
				         *(char **)((char *)pool + POOL_OFF_PROXY_PORT));
				applog(7, msg, 0);
			}
			send(sock, socks5_req, 3, 0);
			if (recv_byte(sock) != 5 || recv_byte(sock) != socks5_req[2])
				goto socks5_bad;
			host = *(char **)((char *)pool + POOL_OFF_SOCKADDR_URL);
			socks5_req[0] = 5;
			socks5_req[1] = 1;
			socks5_req[2] = 0;
			socks5_req[3] = 3;              /* CONNECT, 域名寻址 */
			len = strlen(host);
			if ((int)len > 0xfe)
				len = 0xff;
			socks5_req[4] = (uint8_t)len;
			memcpy(socks5_req + 5, host, len);
			portnum = strtol(*(char **)((char *)pool + POOL_OFF_STRATUM_PORT), NULL, 10);
			socks5_req[len + 6] = (char)portnum;          /* 端口低字节 */
			socks5_req[len + 5] = (char)(portnum >> 8);   /* 端口高字节 */
			send(sock, socks5_req, len + 7, 0);
			if (recv_byte(sock) != 5 || recv_byte(sock) != 0)
				goto socks5_bad;
			recv_byte(sock);                /* 保留字节 */
			atyp = (char)recv_byte(sock);
			if (atyp == 1) {
				/* IPv4: 跳过 4 字节地址 */
				recv_byte(sock);
				recv_byte(sock);
				recv_byte(sock);
				recv_byte(sock);
			} else if (atyp == 3) {
				/* 域名: 1 字节长度 + 域名 */
				ret = recv_byte(sock);
				for (i = 0; i < ret; i++)
					recv_byte(sock);
			} else {
				goto socks5_bad;
			}
			recv_byte(sock);                /* 端口高字节 */
			recv_byte(sock);                /* 端口低字节 */
			if (DAT_00078e09 != 0 && LOG_GATE7) {
				snprintf(msg, 0x1000, "Success negotiating with %s:%s SOCKS5 proxy",
				         *(char **)((char *)pool + POOL_OFF_PROXY_URL),
				         *(char **)((char *)pool + POOL_OFF_PROXY_PORT));
				applog(7, msg, 0);
			}
			goto alloc_sockbuf;
socks5_bad:                                 /* 原 LAB_0002c360 */
			if (LOG_GATE4) {
				snprintf(msg, 0x1000, "Bad response from %s:%s SOCKS5 server",
				         *(char **)((char *)pool + POOL_OFF_PROXY_URL),
				         *(char **)((char *)pool + POOL_OFF_PROXY_PORT));
				applog(4, msg, 0);
			}
			goto out_no_socket;
		default:
			if (LOG_GATE4) {
				snprintf(msg, 0x1000, "Unsupported proxy type for %s:%s",
				         *(char **)((char *)pool + POOL_OFF_PROXY_URL),
				         *(char **)((char *)pool + POOL_OFF_PROXY_PORT));
				applog(4, msg, 0);
			}
			goto out_no_socket;
		}
		if (ret == 0)
			goto out_no_socket;
alloc_sockbuf:                              /* 原 LAB_0002c0d0 / joined_r0x0002c0d8 */
		if (*(char **)((char *)pool + POOL_OFF_SOCKBUF) == NULL) {
			*(char **)((char *)pool + POOL_OFF_SOCKBUF) = calloc(0x2000, 1);
			if (*(char **)((char *)pool + POOL_OFF_SOCKBUF) == NULL) {
				snprintf(msg, 0x1000, "Failed to calloc pool sockbuf in %s %s():%d",
				         "util.c", "setup_stratum_socket", 0xcef);
				applog(3, msg, 1);
				quit(1);
			}
			*(int *)((char *)pool + POOL_OFF_SOCKBUF_SIZE) = 0x2000;
		}
		*(int *)((char *)pool + POOL_OFF_SOCK) = sock;
		keepalive = 1;
		keepidle = 0x2d;                    /* TCP_KEEPIDLE = 45 */
		keepintvl = 0x1e;                   /* TCP_KEEPINTVL = 30 */
		ret = fcntl(sock, 3 /* F_GETFL */, 0);
		fcntl(sock, 4 /* F_SETFL */, ret | 0x800 /* O_NONBLOCK (Linux 值) */);
		setsockopt(sock, 1 /* SOL_SOCKET */, 9 /* SO_KEEPALIVE */, &keepalive, 4);
		if (DAT_0007bd20 == 0)
			fcntl(sock, 2 /* F_SETFD */, 1 /* FD_CLOEXEC */);
		setsockopt(sock, 6 /* IPPROTO_TCP */, 1 /* TCP_NODELAY */, &keepalive, 4);
		setsockopt(sock, 6, 6 /* TCP_KEEPCNT */, &keepalive, 4);
		setsockopt(sock, 6, 4 /* TCP_KEEPIDLE */, &keepidle, 4);
		setsockopt(sock, 6, 5 /* TCP_KEEPINTVL */, &keepintvl, 4);
		/* 发送 mining.subscribe (恢复重试 / 无会话 / 带会话 三形态) */
		id = DAT_0007b0f8;
		if (resumed) {
			clear_sock(pool);
			id = DAT_0007b0f8;
			DAT_0007b0f8++;
			sprintf(sendbuf, "{\"id\": %d, \"method\": \"mining.subscribe\", \"params\": []}", id);
		} else if (*(char **)((char *)pool + POOL_OFF_SESSION_ID) == NULL) {
			DAT_0007b0f8++;
			sprintf(sendbuf,
			        "{\"id\":%d, \"method\":\"mining.subscribe\", \"params\":[\"cgminer/4.9.0\", null, \"%s\", \"%s\"]}",
			        id, *(char **)((char *)pool + POOL_OFF_SOCKADDR_URL),
			        *(char **)((char *)pool + POOL_OFF_STRATUM_PORT));
		} else {
			DAT_0007b0f8++;
			sprintf(sendbuf,
			        "{\"id\": %d, \"method\": \"mining.subscribe\", \"params\": [\"cgminer/4.9.0\", \"%s\"]}",
			        id, *(char **)((char *)pool + POOL_OFF_SESSION_ID));
		}
		len = strlen(sendbuf);
		if (__stratum_send(pool, sendbuf, len) != 0) {
			if (DAT_00078e09 != 0 && LOG_GATE7)
				applog(7, "Failed to send s in initiate_stratum", 0);
			sock_established = true;
			goto out_common;                /* 原 LAB_0002b998 */
		}
		if (socket_full(pool, 0x3c /* 60 秒 */) == 0) {
			if (DAT_00078e09 != 0 && LOG_GATE7)
				applog(7, "Timed out waiting for response in initiate_stratum", 0);
			sock_established = true;
			goto out_common;
		}
		line = recv_line(pool);
		if (line == NULL) {
			sock_established = true;
			goto out_common;
		}
		val = json_loads(line, 0, &err);
		free(line);
		if (val == NULL) {
			if (DAT_00078e09 != 0 && LOG_GATE7) {
				snprintf(msg, 0x1000, "JSON decode failed(%d): %s", err.line, err.text);
				applog(7, msg, 0);
			}
			sock_established = true;
			resumed = true;
			goto out_common;
		}
		res_val = json_object_get(val, "result");
		err_val = json_object_get(val, "error");
		if (res_val == NULL || res_val->type == 7 /* json_NULL */) {
			/* result 无效: 取 error 描述; 无 error 则填固定原因串 */
			if (err_val != NULL) {
				reason = json_dumps(err_val, 3);        /* 原 LAB_0002c750 */
			} else {
				reason = malloc(0x11);
				if (reason != NULL)
					strncpy(reason, "(unknown reason)", 0x11);
			}
		} else if (err_val == NULL || err_val->type == 7) {
			/* 无错误应答: 解析 extranonce, 其返回值即本函数返回值 */
			ret = parse_extranonce_equihash(pool, res_val, 1);
			if (ret == 0) {
				if (DAT_00078e09 != 0 && LOG_GATE7) {
					snprintf(msg, 0x1000, "%s: Failed to get parse extranonce.", "initiate_stratum");
					applog(7, msg, 0);
				}
				sock_established = true;
				resumed = true;
				goto out_common;
			}
			if (*(int *)((char *)pool + POOL_OFF_UNS_258) == 0) {
				*(char **)((char *)pool + POOL_OFF_UNS_258) =
				    *(char **)((char *)pool + POOL_OFF_SOCKADDR_URL);
			}
			*(uint8_t *)((char *)pool + POOL_OFF_UNS_299) = 1;  /* stratum_active = true */
			*(int *)((char *)pool + POOL_OFF_DIFF) = 0;         /* 难度 1.0 (两个半字写) */
			*(int *)((char *)pool + POOL_OFF_DIFF + 4) = 0x3ff00000;
			if (DAT_0007b380 != 0 && DAT_00078e09 != 0 && LOG_GATE7) {
				snprintf(msg, 0x1000,
				         "Pool %d confirmed mining.subscribe with extranonce1 %s extran2size %d",
				         *(int *)((char *)pool + POOL_OFF_POOL_NO),
				         *(char **)((char *)pool + POOL_OFF_NONCE1),
				         *(int *)((char *)pool + POOL_OFF_EXTRAN2SIZE));
				applog(7, msg, 0);
			}
			id = DAT_0007b0f8;
			if (*(uint8_t *)((char *)pool + POOL_OFF_UNS_25C) != 0) {
				/* 池要求 extranonce 订阅: 补发请求并读走应答 */
				DAT_0007b0f8++;
				sprintf(sendbuf,
				        "{\"id\": %d, \"method\": \"mining.extranonce.subscribe\", \"params\": []}", id);
				stratum_send(pool, sendbuf, strlen(sendbuf));
				line = recv_line(pool);
				if (line == NULL) {
					if (LOG_GATE2)
						applog(3, "recv mining.extranonce.subscribe failed\n", 0);
				} else {
					free(line);
				}
			}
			/* 原 LAB_0002c600: 释放应答对象, 以 parse_extranonce 结果返回 */
			if (val->refcount != -1 && --val->refcount == 0)
				json_delete(val);
			return ret;
		} else {
			reason = json_dumps(err_val, 3);
		}
		/* error 对象存在: JSON-RPC 解析失败 */
		if (DAT_00078e09 != 0 && LOG_GATE7) {
			snprintf(msg, 0x1000, "JSON-RPC decode failed: %s", reason);
			applog(7, msg, 0);
		}
		sock_established = true;
		free(reason);
		resumed = true;
		goto out_common;
out_no_socket:                          /* 原 LAB_0002b994: socket 未建立, 不挂起 */
		sock_established = false;
out_common:                             /* 原 LAB_0002b998 */
		if (!(resumed && !retried)) {
			if (DAT_00078e09 != 0 && LOG_GATE7)
				applog(7, "Initiate stratum failed", 0);
			if (sock_established)
				suspend_stratum(pool);
			if (val != NULL) {
				/* 内联 json_decref: refcount == -1 表示永久引用 */
				if (val->refcount != -1 && --val->refcount == 0)
					json_delete(val);
			}
			return 0;
		}
		/* 已收到应答但解析失败且未重试过: 清 session_id/nonce1 后整体重试一次 */
		if (pthread_mutex_lock((pthread_mutex_t *)((char *)pool + POOL_OFF_DATA_LOCK_M)) != 0) {
			mutex_lock_fail_util("initiate_stratum", 0xe18);
		}
		if (pthread_rwlock_wrlock((pthread_rwlock_t *)((char *)pool + POOL_OFF_DATA_LOCK_RW)) != 0) {
			snprintf(msg, 0x1000, "WTF WRLOCK ERROR ON LOCK! errno=%d in %s %s():%d",
			         errno, "util.c", "initiate_stratum", 0xe18);
			applog(3, msg, 1);
			quit(1);
		}
		free(*(char **)((char *)pool + POOL_OFF_SESSION_ID));
		free(*(char **)((char *)pool + POOL_OFF_NONCE1));
		*(char **)((char *)pool + POOL_OFF_NONCE1) = NULL;
		*(char **)((char *)pool + POOL_OFF_SESSION_ID) = NULL;
		if (pthread_rwlock_unlock((pthread_rwlock_t *)((char *)pool + POOL_OFF_DATA_LOCK_RW)) != 0) {
			snprintf(msg, 0x1000, "WTF RWLOCK ERROR ON UNLOCK! errno=%d in %s %s():%d",
			         errno, "util.c", "initiate_stratum", 0xe1c);
			applog(3, msg, 1);
			quit(1);
		}
		if (pthread_mutex_unlock((pthread_mutex_t *)((char *)pool + POOL_OFF_DATA_LOCK_M)) != 0) {
			snprintf(msg, 0x1000, "WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d",
			         errno, "util.c", "initiate_stratum", 0xe1c);
			applog(3, msg, 1);
			quit(1);
		}
		(*DAT_00075ed8)();
		if (DAT_00078e09 != 0 && LOG_GATE7)
			applog(7, "Failed to resume stratum, trying afresh", 0);
		if (val != NULL) {
			/* 内联 json_decref (原码此处不清空 val 指针, 按原样保留) */
			if (val->refcount != -1 && --val->refcount == 0)
				json_delete(val);
		}
		retried = true;
	} while (1);
}

/*
 * ================= 覆盖率表 (3/3) =================
 * 原 FUN 地址    新函数名                  反编译行数
 * ---------------------------------------------
 * FUN_00020088  stratum_rthread           742
 * FUN_00026f50  display_pools             585
 * FUN_0002b8e8  initiate_stratum          843
 *
 * 外部依赖: 接收/连接链 recv_line/parse_notify_equihash/parse_extranonce_equihash/
 *           __stratum_send/stratum_send/socket_full/clear_sock/clear_sockbuf/
 *           block_socket/recv_byte/http_negotiate/socks4_negotiate/extract_sockaddr/
 *           suspend_stratum/sock_full/cnx_needed/current_pool/restart_stratum/
 *           stratum_resumed/rename_thread (21_pool_net 其余批次);
 *           池管理 switch_pools/clear_stratum_shares/clear_pool_work/remove_pool/
 *           adjust_quota_gcd/input_pool (20_job_mining/40_config 批);
 *           工作链 make_work/gen_stratum_work/test_work_current/free_work/
 *           get_thread/restart_threads/kill_work/sharelog/suffix_string (20 批);
 *           线程辅助 pool_tset/pool_tclear/cgsleep_ms/cgtime (其余批次);
 *           json_loads/json_object_get/json_dumps/json_delete/json_string_value/
 *           json_integer_value/json_array_get (jansson, json.c 批);
 *           applog/quit (41_logging/主流程), mutex_lock_fail_cgminer/
 *           mutex_unlock_fail_cgminer/rd_lock_error/rw_unlock_error/mutex_lock_fail_util
 *           (01_main 锁错误辅助), curses 界面 clear_logwin/logwin_update/_wlogprint/
 *           wlogprint/curses_active_locked/curses_int/immedok/wgetch/wattr_on/wattr_off
 *           (41_logging curses 批)。
 */
