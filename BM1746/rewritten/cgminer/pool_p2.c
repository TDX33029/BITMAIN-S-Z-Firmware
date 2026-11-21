// =============================================================================
// cgminer 矿池网络层 第 2 段 (pool_p2.c)
// 批次: pool_b2  |  分类: 21_pool_net
// 函数: add_pool (FUN_00017a3c) / clear_stratum_shares (FUN_000182f4) /
//       pool_active (FUN_00018ca0) / switch_pools (FUN_00019bf8) /
//       watchpool_thread (FUN_00021850) / stratum_sthread (FUN_00021fe8) /
//       socks4_negotiate (FUN_00028fc8) / recv_line (FUN_0002b040) /
//       lcd (FUN_00034ed4) / pools (FUN_00035e88) /
//       http_listen_thread (FUN_0005664c)
// 命名事实源: rewritten/name_map/cgminer.tsv (阶段 1 冻结)
// 严格语义等价转写; 结构布局不可知处保留指针算术并注释字节偏移。
// =============================================================================

// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <jansson.h>

/* ==================== 外部全局 (DAT_ 原名保留, 语义见注释) ==================== */

extern int      DAT_0007b198;    /* total_pools: 已配置矿池数 (axi.c / freq_p2.c 同名) */
extern void   **DAT_0007b438;    /* pools: 矿池指针数组基址 (total_pools 项) */
extern void    *DAT_00077048;    /* stratum_shares: uthash 表头 (首个 stratum_share, 语义待定) */
extern pthread_mutex_t DAT_00077030;  /* stratum_lock: stratum 份额哈希表锁 (job_p1.c 同一全局) */
extern uint8_t  DAT_0007ad48;    /* applog 门控 A: 非 0 → applog 走 syslog (util.c 同一全局) */
extern uint8_t  DAT_00078e08;    /* applog 门控 B = opt_verbose (util.c 同一全局) */
extern int      DAT_000766c4;    /* applog 门控 C: int 日志级别 (util.c 同一全局) */
extern uint32_t DAT_0007bc80;    /* 全局 64 位统计计数低字 (与 DAT_0007bc84 成对), 语义待定 */
extern uint32_t DAT_0007bc84;    /* 全局 64 位统计计数高字, 语义待定 */
extern double   DAT_0007b428;    /* 全局 double 统计求和分量 (fan.c 同一全局), 语义待定 */
extern uint8_t  DAT_00078e09;    /* opt_debug (日志门控外层总开关, util.c 同一全局) */
extern int      DAT_00075dd4;    /* 全局, 语义待定: 非 0 才继续池测试 (疑似 curl 初始化完成标志) [低置信] */
extern uint8_t  DAT_00077054;    /* 全局字节标志, 语义待定: 非 0 时跳过 stratum_url 预解析路径 [低置信] */
extern uint8_t  DAT_0007bc79;    /* 全局字节标志 (20_job_mining 读), 置 1 后启动 stratum 线程 */
extern uint8_t  DAT_0007730d;    /* 全局字节标志, 语义待定: 与 DAT_00076ffc 任一非 0 时 watchpool 跳过 curl 回收/份额修剪 [低置信] */
extern int      DAT_00076ffc;    /* 全局, 语义待定: 同上 (20_job_mining 中与 benchmark 命名相关) [低置信] */
extern int      DAT_0007b558;    /* opt_rotate_period: rotate 策略轮换周期 (分钟, display_pools 设置) */
extern int      DAT_00077310;    /* 全局 timeval 的 tv_sec (rotate 上次执行时刻, cgtime 写入 8 字节) [低置信] */
extern void  (*DAT_00075ed8)(void);  /* 全局函数指针: ELF 初值 noop_hook (FUN_00028ae0, 恒 0) [低置信] */
extern void    *DAT_0007705c;    /* currentpool: 当前矿池指针 (current_pool(FUN_0001da58) 读取) [低置信] */
extern pthread_rwlock_t DAT_0007ba28;  /* control_lock 组合锁的读写锁部分 (job_p1.c 同一全局) */
extern pthread_mutex_t  DAT_00077060;  /* 全局互斥锁, 语义待定: 配合 DAT_00077078 广播更新 */
extern pthread_cond_t   DAT_00077078;  /* 全局条件变量, 语义待定: 矿池切换后广播唤醒 */
extern pthread_mutex_t  DAT_0007ba10;  /* control_lock 的 mutex 部分 (job_p1.c 同一全局) */
extern uint8_t  DAT_0007b625;    /* opt_failover_only: Failover-Only 模式开关 (API 串绑定) */
extern int      DAT_0007b0f8;    /* sshare_id: stratum_share 自增 id 计数器 (stratum_sthread 分配) */
extern int      DAT_0007ae68;    /* 全局 int: stratum 提交失败累计 (语义待定) [低置信] */
extern uint8_t  DAT_0007b644;    /* 全局字节标志: lowmem 选项, 非 0 阻止 stratum 份额重提交 [低置信] */
extern uint8_t  DAT_0007b380;    /* 全局字节标志: 疑似 opt_protocol, 非 0 时打印 "RECVD:" 报文 [低置信] */
extern double   DAT_0007acc0;    /* GHS av 分子 (api_p2.c 同一全局) */
extern double   DAT_000755f8;    /* 运行秒数 elapsed (api_p2.c 同一全局) */
extern double   DAT_00077158;    /* GHS 5s 值 (job_p3.c: DAT_0007bb10/1000.0 写入) */
extern int      DAT_0007b8a8;    /* 设备状态计数 (api_p1a/api_p2 同一全局, "fan" 数据源) */
extern int      DAT_0007bb20;    /* 设备状态计数 (同上, "temp" 数据源) */
extern char     DAT_000670ac[];  /* rodata: "Y" (ELF 0x000670ac 提取) */
extern char     UNK_000670b0[];  /* rodata: "N" (ELF 0x000670b0 提取) */
extern char     DAT_0006721c[];  /* rodata: "LCD" */
extern char     DAT_00067230[];  /* rodata: "fan" */
extern char     DAT_00067234[];  /* rodata: "temp" */
extern char     DAT_0006723c[];  /* rodata: "user" */
extern char     DAT_00067488[];  /* rodata: "POOL" */
extern char     DAT_000674b8[];  /* rodata: "User" */
extern char     DAT_000674d0[];  /* rodata: "Diff" */
extern char     DAT_0006753c[];  /* rodata: "Has GBT" */
extern int      DAT_00076b9c;    /* HTTP 监听 socket fd (6060) [低置信] */
extern uint8_t  DAT_0007acb8;    /* 全局停机标志字节 (http 监听等线程退出条件) [低置信] */
extern int   status_service_thread(int fd);                          /* FUN_000559d0: 处理已接受的客户端连接 */

/* ==================== 外部函数 (name_map 语义名) ==================== */

extern void  applog(uint32_t prio, char *buf, ...);   /* FUN_00038730: 第 3 参非 0 → 强制 curses 刷新 */
extern void  quit(int status);                                       /* FUN_00016ca8: 单参包装, 内部走 __quit(FUN_0001d8fc) */
extern void  mutex_lock_fail_cgminer(const char *fn, int line);      /* FUN_0000b414 */
extern void  mutex_init_fail_cgminer(const char *fn, int line);      /* FUN_0000b4ec */
extern void  mutex_unlock_fail_cgminer(const char *fn, int line);    /* FUN_0000b5c4 */
extern void  wr_lock_error(const char *fn, int line);                /* FUN_0000b480: pthread_rwlock_wrlock 失败处理 */
extern void  rd_lock_error(const char *fn, int line);                /* FUN_0000b69c: pthread_rwlock_rdlock 失败处理 */
extern void  rw_unlock_error(const char *fn, int line);              /* FUN_0000b630: pthread_rwlock_unlock 失败处理 */
extern void  rwlock_init_fail_cgminer(const char *fn, int line);     /* FUN_0000b558 */
extern void  free_work(void *work);                                  /* FUN_00015780: 清 work 内部引用 + free */
extern void  adjust_quota_gcd(void);                                 /* FUN_000152e0: 重算全池配额 gcd */
extern uint8_t pool_tset(void *pool, uint8_t *flag_addr);            /* FUN_00018834: pool_lock 下 *addr=1, 返回旧值 */
extern uint8_t pool_tclear(void *pool, uint8_t *flag_addr);          /* FUN_00018c2c: pool_lock 下 *addr=0, 返回旧值 */
extern int   initiate_stratum(void *pool);                           /* FUN_0002b8e8: stratum 握手/订阅 */
extern int   auth_stratum(void *pool);                               /* FUN_0002ea38: stratum 鉴权 */
extern void  init_stratum_threads(void *pool);                       /* FUN_0002b8e4: 反编译体为空桩 (忽略调用方传入的 pool) */
extern int   DAT_00076efc;   /* 挖矿策略选择 (0/3/4 = Failover/LoadBalance/Balance) */
extern void  parse_xnsub(void *pool, char *url);                     /* FUN_0002aa68: 解析 "#xnsub" 后缀 */
extern int   extract_sockaddr(char *url, void *sockaddr_url, void *sockaddr_port);  /* FUN_0002ab54 */
extern void  cgtime(struct timeval *tv);                             /* FUN_0002a84c: gettimeofday(tv, NULL) */
extern void *stratum_sthread(void *arg);                             /* FUN_00021fe8: stratum 发送线程体 */
extern void *stratum_rthread(void *arg);                             /* FUN_00020088: stratum 接收线程体 */
extern void *priority_pool(int idx);                                 /* FUN_00012038: 取优先级序为 idx 的矿池 */
extern void  clear_pool_work(void *pool);                            /* FUN_000185c8: 清空矿池工作队列 */
extern void  set_lowprio(void);                                      /* FUN_000120f4: 降低自身调度优先级 */
extern int   cp_prio(void);                                          /* FUN_0001fb7c: 取当前矿池优先级 (control_lock 下) */
extern void  pool_resus(void *pool);                                 /* FUN_0001fc34: 矿池恢复 (idle→活动) 处理 */
extern void *current_pool(void);                                     /* FUN_0001da58: 取当前矿池指针 */
extern unsigned int cgsleep_ms(unsigned int ms);                     /* FUN_0002a92c */
extern void  rename_thread(const char *name);                        /* FUN_0002f0b4: snprintf "cg@%s" + prctl */
extern double __floatdidf(int lo, int hi);                           /* FUN_0005cb64: int64→double (参数=低/高字) */
extern void *tq_new(void);                                           /* FUN_0002a168: 构造 tq 队列 (calloc 0x58 + 自环链表 + mutex/cond) */
extern void  tq_freezethaw(void *q);                                 /* FUN_0002a230: 置 q+8 冻结标志并 signal (锁 q+0xc) */
extern void *tq_pop(void *q, struct timespec *timeout);              /* FUN_0002a300: 取队首 work; timeout=NULL 永久阻塞 */
extern char *bin2hex(const void *src, int len);                      /* FUN_00029730: 二进制→十六进制串 (malloc) */
extern int   stratum_send(void *pool, const char *buf, int len);     /* FUN_0002b5c8: 经矿池连接发送 */
extern uint8_t cnx_needed(void *pool);                               /* FUN_0001ffec: 该矿池是否需要维持连接 */
extern bool  socket_full(void *pool, unsigned int timeout);          /* FUN_00028afc: pool sock (+0x264) 在 timeout 秒内是否有数据 */
extern unsigned int recv_byte(int fd);                               /* FUN_00028bc4: recv 1 字节 (失败返回 0xffffffff) */
extern void  suspend_stratum(void *pool);                            /* FUN_0002aee0: 关闭 stratum 连接并走重连链 */
extern void  clear_sock(void *pool);                                 /* FUN_000295a8: 清空 socket 接收缓冲 */
extern char *proxytype(int type);                                    /* FUN_00029690: 代理类型号 → "http:" 等串 */
extern void  message(void *io_data, int code, int param0, int param1, int isjson);   /* FUN_0003305c (api_p1b.c 定义) */
extern int   print_data(void *io_data, void *root, int isjson, int prepend_comma);   /* FUN_00032534 */
extern int   io_add(void *io, const char *str);                      /* FUN_000307c8 */
extern void *api_add_string(void *root, const char *name, char *data, int copy);     /* FUN_00032e1c */
extern void *api_add_escape(void *root, const char *name, char *data, int copy);     /* FUN_00032df8 */
extern void *api_add_const(void *root, const char *name, char *data, int copy);      /* FUN_00032e40 */
extern void *api_add_int(void *root, const char *name, int *data, int copy);         /* FUN_00032ed0 */
extern void *add_uint_buf(void *root, const char *name, uint32_t *data, int copy);   /* FUN_00032ef4 */
extern void *add_uint64_buf(void *root, const char *name, uint64_t *data, int copy); /* FUN_00032f60 */
extern void *add_int64_buf(void *root, const char *name, int64_t *data, int copy);   /* FUN_00032f84 */
extern void *add_diff_buf(void *root, const char *name, double *data, int copy);     /* FUN_000355f8 */
extern void *add_percent_buf(void *root, const char *name, double *data, int copy);  /* FUN_000357a4 */
extern void *add_bool_buf(void *root, const char *name, uint8_t *data, int copy);    /* FUN_00032ff0 */

/* ==================== pool 结构字节偏移 (机器码锁定, axi.c 同约定) ==================== */

#define POOL_OFF_POOL_NO    0x00   /* pool_no: 矿池序号 */
#define POOL_OFF_PRIO       0x04   /* prio: 优先级序号 */
#define POOL_OFF_QUOTA      0x38   /* quota (int) */
#define POOL_OFF_ACTIVE     0x83   /* active 标志 (uint8_t) */
#define POOL_OFF_0X78       0x78   /* double 统计分量 (语义待定) */
#define POOL_OFF_0X98       0x98   /* int 计数 (语义待定) */
#define POOL_OFF_0XA8       0xa8   /* int, 初始化为 -1 (语义待定) */
#define POOL_OFF_0X178      0x178  /* 双向链表头 (next=prev=自身, 语义待定) */
#define POOL_OFF_0X2F4      0x2f4  /* int 计数, 每 share 自减 (语义待定) */
#define POOL_OFF_RPC_URL    0xc4   /* rpc_url */
#define POOL_OFF_RPC_USER   0xcc   /* rpc_user */
#define POOL_OFF_RPC_PASS   0xd0   /* rpc_pass */
#define POOL_OFF_0XD8       0xd8   /* rpc_proxy (void*, add_pool 置 NULL) */
#define POOL_OFF_0X80       0x80   /* uint8_t 提交失败标志 (pool_tset/pool_tclear 操作) */
#define POOL_OFF_0X89       0x89   /* uint8_t 停机标志 (stratum_sthread 退出条件) */
#define POOL_OFF_0XA4       0xa4   /* int 计数: stratum 提交失败累计 (语义待定) */
#define POOL_OFF_NONCE1     0x27c  /* nonce1: stratum 会话 id 串 (char*) */
#define POOL_OFF_STRATUM_Q  0x2f0  /* stratum_q: tq 队列指针 (int 槽位) */

/* ==================== stratum_share / uthash 结构字节偏移 ==================== */
/* stratum_share 首成员为 UT_hash_handle (hho=0), 机器码锁定布局:
 *   +0x00 tbl(UT_hash_table*)  +0x04 prev(应用序前驱)  +0x08 next(应用序后继)
 *   +0x0c hh_prev(桶序前驱)    +0x10 hh_next(桶序后继)  +0x1c hashv
 *   +0x24 work(void*)
 * UT_hash_table: +0x00 buckets  +0x04 num_buckets  +0x0c num_items
 *                +0x10 tail     +0x14 hho
 * UT_hash_bucket (12 字节): +0x00 hh_head  +0x04 计数(int)  +0x08 hh_tail   */

#define SSHARE_OFF_TBL      0x00
#define SSHARE_OFF_PREV     0x04
#define SSHARE_OFF_NEXT     0x08
#define SSHARE_OFF_HH_PREV  0x0c
#define SSHARE_OFF_HH_NEXT  0x10
#define SSHARE_OFF_HASHV    0x1c
#define SSHARE_OFF_KEY      0x14   /* hh.key: 指向元素内 key 字段 */
#define SSHARE_OFF_KEYLEN   0x18   /* hh.keylen */
#define SSHARE_OFF_WORK     0x24
#define SSHARE_OFF_ID       0x28   /* sshare_id: int 自增 id (hash key) */
#define SSHARE_OFF_TV_SEC   0x2c   /* share 创建时刻 (time_t) */
#define SSHARE_OFF_TAILTV   0x30   /* 提交收尾时刻 (time_t) */

#define HTBL_OFF_BUCKETS    0x00
#define HTBL_OFF_NUMBUCKETS 0x04
#define HTBL_OFF_NUMITEMS   0x0c
#define HTBL_OFF_TAIL       0x10
#define HTBL_OFF_HHO        0x14
#define HTBL_OFF_LOG2NUM    0x08   /* log2_num_buckets */
#define HTBL_OFF_IDEALMAX   0x18   /* ideal_chain_maxlen (桶扩展时重算) */
#define HTBL_OFF_NONIDEAL   0x1c   /* nonideal_items */
#define HTBL_OFF_INEFF      0x20   /* ineff_expands */
#define HTBL_OFF_NOEXPAND   0x24   /* noexpand: 连续低效扩展后置 1 */
#define HTBL_OFF_SIGNATURE  0x28   /* bloom 签名 (0xa0111fe1) */

/* work 结构机器码锁定偏移: */
#define WORK_OFF_POOL       0x6d4  /* work->pool (void*) */
#define WORK_OFF_0X748      0x748  /* work 难度 (double, 语义待定) */
#define WORK_OFF_NONCE1BIN  0x14c  /* nonce1bin||nonce2 起始 (0x20 字节) */
#define WORK_OFF_0X16C      0x16c  /* 0x14c+0x20 起 0x543 字节 (stratum 参数 8, 语义待定) */
#define WORK_OFF_JOB_ID     0x6f0  /* work->job_id (char*) */
#define WORK_OFF_NTIME_STR  0x704  /* work->ntime 串 (char*) */
#define WORK_OFF_NONCE1_STR 0x710  /* work->nonce1 串 (char*) */

// === add_pool === 原 FUN_00017a3c @ 0x00017a3c
// 来源: reversed/cgminer/21_pool_net/FUN_00017a3c.c | 分类: 21_pool_net
// 语义: 分配一个 0x750 字节的矿池结构并入池表: pool_no/prio 先取自增前的
//       total_pools, 再把 pools 指针数组按 (total_pools+2) 项 realloc 并挂入,
//       初始化 5 把同步原语 (失败按原行号报错退出), 预置 getwork 请求串等
//       初值 (quota=1), 末尾触发全池配额 gcd 重算。返回新矿池指针。
void *add_pool(void)
{
	void *pool;
	char logbuf[0x1000];
	int ret;
	int old_total;

	pool = calloc(1, 0x750);                       /* pool 结构总长 0x750 字节 */
	if (pool == NULL) {
		applog(3, "Failed to malloc pool in add_pool", 1);
		quit(1);                                   /* 不返回 */
	}

	/* 入池表: pool+0x00 pool_no 与 pool+0x04 prio 均先取自增前的 total_pools */
	old_total = DAT_0007b198;
	*(int *)((uint8_t *)pool + POOL_OFF_PRIO) = old_total;
	*(int *)((uint8_t *)pool + POOL_OFF_POOL_NO) = old_total;
	DAT_0007b438 = realloc(DAT_0007b438, (old_total + 2) * 4);   /* 指针数组扩容 */
	*(void **)((uint8_t *)DAT_0007b438 + old_total * 4) = pool;  /* pools[old_total] = pool */
	DAT_0007b198 = old_total + 1;

	ret = pthread_mutex_init((pthread_mutex_t *)((uint8_t *)pool + 0xdc), NULL);   /* pool+0xdc */
	if (ret != 0) {
		mutex_init_fail_cgminer("add_pool", 0x2ec);
	}
	ret = pthread_cond_init((pthread_cond_t *)((uint8_t *)pool + 0x148), NULL);    /* pool+0x148 */
	if (ret != 0) {
		applog(3, "Failed to pthread_cond_init in add_pool", 1);
		quit(1);                                   /* 不返回 */
	}
	ret = pthread_mutex_init((pthread_mutex_t *)((uint8_t *)pool + 0xf4), NULL);   /* pool+0xf4 */
	if (ret != 0) {
		mutex_init_fail_cgminer("add_pool", 0x2ef);
	}
	ret = pthread_rwlock_init((pthread_rwlock_t *)((uint8_t *)pool + 0x10c), NULL);  /* pool+0x10c */
	if (ret != 0) {
		rwlock_init_fail_cgminer("add_pool", 0x2ef);
	}
	ret = pthread_mutex_init((pthread_mutex_t *)((uint8_t *)pool + 0x2d8), NULL);  /* pool+0x2d8 */
	if (ret != 0) {
		mutex_init_fail_cgminer("add_pool", 0x2f0);
	}
	ret = pthread_mutex_init((pthread_mutex_t *)((uint8_t *)pool + 0x2fc), NULL);  /* pool+0x2fc */
	if (ret != 0) {
		mutex_init_fail_cgminer("add_pool", 0x2f1);
	}
	ret = pthread_rwlock_init((pthread_rwlock_t *)((uint8_t *)pool + 0x314), NULL);  /* pool+0x314 */
	if (ret != 0) {
		rwlock_init_fail_cgminer("add_pool", 0x2f1);
	}

	/* pool+0x178 空双向链表头: next(+0x178)=prev(+0x17c)=指向头自身 */
	*(uint32_t *)((uint8_t *)pool + POOL_OFF_0X178) = (uint32_t)((uint8_t *)pool + POOL_OFF_0X178);
	*(uint32_t *)((uint8_t *)pool + POOL_OFF_0X178 + 4) = (uint32_t)((uint8_t *)pool + POOL_OFF_0X178);

	*(const char **)((uint8_t *)pool + 0xc0) = "{\"method\": \"getwork\", \"params\": [], \"id\":0}\n";
	*(int *)((uint8_t *)pool + POOL_OFF_0XA8) = -1;              /* 初始 -1 */
	*(void **)((uint8_t *)pool + POOL_OFF_0XD8) = NULL;          /* rpc_proxy 初值 NULL */
	*(int *)((uint8_t *)pool + POOL_OFF_QUOTA) = 1;              /* quota 默认 1 */
	adjust_quota_gcd();                                          /* 重算全池配额 gcd */
	return pool;
}

// === clear_stratum_shares === 原 FUN_000182f4 @ 0x000182f4
// 来源: reversed/cgminer/21_pool_net/FUN_000182f4.c | 分类: 21_pool_net
// 语义: 矿池断线时清理其全部在途 stratum 份额: sshare_lock (DAT_00077030) 下遍历
//       全局 stratum_shares 哈希表 (DAT_00077048, uthash), 对 work->pool==pool 的
//       表项执行 HASH_DEL (仅剩单元素时改为释放桶数组+表结构并清空表头), 统计丢失
//       份额数与难度和, 释放 work 与 sshare; 解锁后把统计回填矿池/全局并按日志门控告警。
void clear_stratum_shares(void *pool)
{
	char logbuf[0x1000];
	int *sshare;          /* 当前表项 (stratum_share) */
	int *tmpshare;        /* HASH_ITER 的临时后继 (先于函数体捕获) */
	void *head;           /* 表头元素 (HASH_DELETE 每次经它重读 tbl, 删除会改变表头) */
	void *tbl;            /* head->hh.tbl */
	uint32_t hho;         /* tbl->hho: hh 在元素内偏移 (本表为 0) */
	int prev;             /* sshare->hh.prev */
	uint32_t bucket_idx;
	void *bucket;
	uint32_t lost = 0;    /* 丢失份额数 */
	double lost_cps = 0.0;
	int ret;

	ret = pthread_mutex_lock(&DAT_00077030);       /* sshare_lock */
	if (ret != 0) {
		mutex_lock_fail_cgminer("clear_stratum_shares", 0x1b4a);
	}

	if (DAT_00077048 != NULL) {
		/* HASH_ITER(hh, stratum_shares, sshare, tmpshare) */
		sshare = (int *)DAT_00077048;
		tmpshare = *(int **)((uint8_t *)sshare + SSHARE_OFF_NEXT);
		while (true) {
			head = DAT_00077048;                   /* 每轮重读: 删除首元素会更新表头 */
			/* 命中条件: sshare->work(+0x24)->pool(work+0x6d4) == pool */
			if (*(void **)(*(void **)((uint8_t *)sshare + SSHARE_OFF_WORK) + WORK_OFF_POOL) == pool) {
				prev = *(int *)((uint8_t *)sshare + SSHARE_OFF_PREV);
				if ((prev == 0) && (*(int *)((uint8_t *)sshare + SSHARE_OFF_NEXT) == 0)) {
					/* 表中仅剩此元素: 释放桶数组与表结构, 表头清空 (固件定制, 上游 HASH_DEL 无此释放) */
					free(*(void **)*(void **)DAT_00077048);   /* free(head->hh.tbl->buckets) */
					free(*(void **)head);                     /* free(head->hh.tbl) */
					DAT_00077048 = NULL;
				} else {
					/* uthash HASH_DELETE 内联 (tbl 经表头读取, hho 运行时取自 tbl+0x14) */
					tbl = *(void **)head;                     /* head->hh.tbl */
					hho = *(uint32_t *)((uint8_t *)tbl + HTBL_OFF_HHO);
					/* 应用序表尾指向被删元素 → 表尾回退到 hh.prev */
					if (*(void **)((uint8_t *)tbl + HTBL_OFF_TAIL) == (uint8_t *)sshare + hho) {
						*(void **)((uint8_t *)tbl + HTBL_OFF_TAIL) =
						    (uint8_t *)*(void **)((uint8_t *)sshare + SSHARE_OFF_PREV) + hho;
						prev = *(int *)((uint8_t *)sshare + SSHARE_OFF_PREV);
					}
					if (prev == 0) {
						/* 删的是应用序表头 → 表头直接指向后继 */
						DAT_00077048 = *(void **)((uint8_t *)sshare + SSHARE_OFF_NEXT);
					} else {
						/* 前驱的应用序后继指向被删元素的后继 */
						*(void **)((uint8_t *)*(void **)((uint8_t *)sshare + SSHARE_OFF_PREV) + hho + 8) =
						    *(void **)((uint8_t *)sshare + SSHARE_OFF_NEXT);
					}
					if (*(int *)((uint8_t *)sshare + SSHARE_OFF_NEXT) != 0) {
						/* 后继的应用序前驱 = 被删元素的前驱 (按机器码: 不加 hho) */
						*(void **)((uint8_t *)*(void **)((uint8_t *)sshare + SSHARE_OFF_NEXT) + hho + 4) =
						    *(void **)((uint8_t *)sshare + SSHARE_OFF_PREV);
					}
					/* 桶链: 桶计数--, 若桶头即被删元素则桶头改挂其桶序后继 */
					bucket_idx = (*(uint32_t *)((uint8_t *)tbl + HTBL_OFF_NUMBUCKETS) - 1U) &
					             *(uint32_t *)((uint8_t *)sshare + SSHARE_OFF_HASHV);
					bucket = *(void **)((uint8_t *)tbl + HTBL_OFF_BUCKETS) + bucket_idx * 0xc;
					*(int *)((uint8_t *)bucket + 4) = *(int *)((uint8_t *)bucket + 4) - 1;
					if (*(void **)bucket == sshare) {
						*(void **)bucket = *(void **)((uint8_t *)sshare + SSHARE_OFF_HH_NEXT);
					}
					/* 桶序链表 (hh_prev/hh_next) 摘除 */
					if (*(void **)((uint8_t *)sshare + SSHARE_OFF_HH_PREV) != NULL) {
						*(void **)((uint8_t *)*(void **)((uint8_t *)sshare + SSHARE_OFF_HH_PREV) + SSHARE_OFF_HH_NEXT) =
						    *(void **)((uint8_t *)sshare + SSHARE_OFF_HH_NEXT);
					}
					if (*(void **)((uint8_t *)sshare + SSHARE_OFF_HH_NEXT) != NULL) {
						*(void **)((uint8_t *)*(void **)((uint8_t *)sshare + SSHARE_OFF_HH_NEXT) + SSHARE_OFF_HH_PREV) =
						    *(void **)((uint8_t *)sshare + SSHARE_OFF_HH_PREV);
					}
					*(int *)((uint8_t *)tbl + HTBL_OFF_NUMITEMS) = *(int *)((uint8_t *)tbl + HTBL_OFF_NUMITEMS) - 1;
				}
				lost = lost + 1;
				lost_cps = lost_cps + *(double *)(*(void **)((uint8_t *)sshare + SSHARE_OFF_WORK) + WORK_OFF_0X748);
				free_work(*(void **)((uint8_t *)sshare + SSHARE_OFF_WORK));
				*(void **)((uint8_t *)sshare + SSHARE_OFF_WORK) = NULL;
				*(int *)((uint8_t *)pool + POOL_OFF_0X2F4) = *(int *)((uint8_t *)pool + POOL_OFF_0X2F4) - 1;
				free(sshare);
			}
			if (tmpshare == NULL) {
				break;
			}
			sshare = tmpshare;
			tmpshare = *(int **)((uint8_t *)tmpshare + SSHARE_OFF_NEXT);
		}
	}

	ret = pthread_mutex_unlock(&DAT_00077030);
	if (ret != 0) {
		mutex_unlock_fail_cgminer("clear_stratum_shares", 6999);
	}
	(*DAT_00075ed8)();                             /* 空操作钩子 (按机器码保留) */

	if (lost != 0) {
		if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
			snprintf(logbuf, 0x1000, "Lost %d shares due to stratum disconnect on pool %d",
			         lost, *(int *)pool);   /* pool_no */
			applog(4, logbuf, 0);
		}
		*(int *)((uint8_t *)pool + POOL_OFF_0X98) = *(int *)((uint8_t *)pool + POOL_OFF_0X98) + lost;
		/* 全局 64 位计数 (低字 DAT_0007bc80 / 高字 DAT_0007bc84) += lost */
		{
			uint32_t carry = (DAT_0007bc80 + lost < DAT_0007bc80) ? 1U : 0U;   /* CARRY4 */
			DAT_0007bc80 = DAT_0007bc80 + lost;
			DAT_0007bc84 = DAT_0007bc84 + (uint32_t)((int32_t)lost >> 31) + carry;
		}
		DAT_0007b428 = DAT_0007b428 + lost_cps;
		*(double *)((uint8_t *)pool + POOL_OFF_0X78) = *(double *)((uint8_t *)pool + POOL_OFF_0X78) + lost_cps;
	}
}

// === pool_active === 原 FUN_00018ca0 @ 0x00018ca0
// 来源: reversed/cgminer/21_pool_net/FUN_00018ca0.c | 分类: 21_pool_net
// 语义: 测试矿池可用性并激活其 stratum 线程: 打印探测日志 → CURL 就绪检查 →
//       (stratum 未激活时) GBT 能力探测 → 按 stratum_url 有无走预解析/直连两条
//       initiate_stratum 路径 (解析失败会经 LAB_00018d38 重试一次握手) → testing
//       标志置位后鉴权并创建 stratum_sthread/stratum_rthread 双线程。返回 1=激活
//       成功, 0=失败; 已在测试中 (testing 已置位) 返回 pool+0x299 标志。
uint8_t pool_active(void *pool, int pinging)
{
	char logbuf[0x1000];
	char *url;            /* pool+0x258: stratum_url (疑似, 上游同名) */
	uint8_t has_gbt;      /* pool+0x2f8 字节: GBT coinbase+append 支持标志 (疑似 has_gbt) */
	uint8_t gbt_solo;     /* pool+0x39c 字节: GBT solo 标志 (语义待定) */
	uint8_t probe_debug;
	uint8_t ret;
	int res;
	struct timeval tv_start;
	struct timeval tv_end;

	has_gbt = *(uint8_t *)((uint8_t *)pool + 0x2f8);
	gbt_solo = *(uint8_t *)((uint8_t *)pool + 0x39c);

	if (has_gbt == '\0') {
		if ((DAT_00078e09 != '\0') &&
		    (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4))) {
			snprintf(logbuf, 0x1000, "Testing pool %s",
			         *(char **)((uint8_t *)pool + POOL_OFF_RPC_URL));
			applog(7, logbuf, 0);
		}
	} else if ((DAT_00078e09 != '\0') &&
	         (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4))) {
		snprintf(logbuf, 0x1000, "Retrieving block template from pool %s",
		         *(char **)((uint8_t *)pool + POOL_OFF_RPC_URL));
		applog(7, logbuf, 0);
	}

	if (*(uint8_t *)((uint8_t *)pool + 0x298) != '\0') {
		goto activate;     /* LAB_00018d50: 已测试过, 直接走激活段 */
	}
	if (DAT_00075dd4 == 0) {
		if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
			return 0;
		}
		applog(3, "CURL Initialisation failed", 0);
		return 0;
	}
	if (*(uint8_t *)((uint8_t *)pool + POOL_OFF_ACTIVE) == '\0') {
		/* stratum 未激活 → 探测 GBT 能力; 探测段各路径均按机器码清 active 标志 */
		if (DAT_00078e09 != '\0') {
			if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)) {
				applog(7, "Probing for GBT support", 0);
				probe_debug = DAT_00078e09;    /* 机器码在打印后重读 (LAB_00018f34/00018fbc/00019004 入口) */
				if (probe_debug != '\0') {
					if (has_gbt != '\0') {
						/* LAB_00018f34 */
						memcpy(logbuf, "GBT coinbase + append support found, switching to GBT protocol", 0x3f);
						applog(7, logbuf, 0);
					} else if (gbt_solo != '\0') {
						/* LAB_00018fbc */
						memcpy(logbuf, "GBT coinbase without append found, switching to GBT solo protocol", 0x42);
						applog(7, logbuf, 0);
					} else {
						/* LAB_00019004 */
						memcpy(logbuf, "No GBT coinbase + append support found, using getwork protocol", 0x3f);
						applog(7, logbuf, 0);
					}
				}
			}
		}
		*(uint8_t *)((uint8_t *)pool + POOL_OFF_ACTIVE) = 0;
	}

	/* LAB_00018d0c: 时间戳采样 (tv_start/tv_end 此后未再使用, 按机器码保留) */
	cgtime(&tv_start);
	cgtime(&tv_end);
	url = *(char **)((uint8_t *)pool + 0x258);
	if ((url == NULL) || (DAT_00077054 != '\0')) {
retry_initiate:        /* LAB_00018d38: 握手入口 (stratum_url 缺失路径与解析失败重试共用) */
		if (initiate_stratum(pool) == 0) {
			if ((DAT_00078e09 != '\0') &&
			   (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4))) {
				snprintf(logbuf, 0x1000, "FAILED to retrieve work from pool %u %s",
				         *(uint32_t *)pool, *(char **)((uint8_t *)pool + POOL_OFF_RPC_URL));
				applog(7, logbuf, 0);
			}
			if (((pinging == 0) && (*(uint8_t *)((uint8_t *)pool + 0x81) == '\0')) &&
			    ((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0') || (3 < DAT_000766c4))) {
				snprintf(logbuf, 0x1000, "Pool %u slow/down or URL or credentials invalid",
				         *(uint32_t *)pool);
				applog(4, logbuf, 0);
			}
			return 0;
		}
	} else {
		if ((DAT_00078e09 != '\0') &&
		   (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4))) {
			snprintf(logbuf, 0x1000, "Testing pool %d stratum %s",
			         *(uint32_t *)pool, url);
			applog(7, logbuf, 0);
			url = *(char **)((uint8_t *)pool + 0x258);   /* 机器码重读 stratum_url */
		}
		parse_xnsub(pool, url);
		if ((extract_sockaddr(*(char **)((uint8_t *)pool + 0x258),
		                      (uint8_t *)pool + 0x270, (uint8_t *)pool + 0x260) == 0) ||
		    (initiate_stratum(pool) == 0)) {
			goto retry_initiate;       /* LAB_00018d38: 再试一次握手 */
		}
		if ((DAT_00078e09 != '\0') &&
		   (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4))) {
			snprintf(logbuf, 0x1000, "Switching pool %d %s to %s",
			         *(uint32_t *)pool,
			         *(char **)((uint8_t *)pool + POOL_OFF_RPC_URL),
			         *(char **)((uint8_t *)pool + 0x258));
			applog(7, logbuf, 0);
		}
		if (*(char **)((uint8_t *)pool + POOL_OFF_RPC_URL) == NULL) {
			*(char **)((uint8_t *)pool + POOL_OFF_RPC_URL) =
			    strdup(*(char **)((uint8_t *)pool + 0x258));
		}
	}
	*(uint8_t *)((uint8_t *)pool + 0x298) = 1;

activate:              /* LAB_00018d50: 激活段 */
	if (pool_tset(pool, (uint8_t *)pool + 0x29a) == 0) {
		/* testing 标志此前未置位: 鉴权并创建双线程 */
		res = initiate_stratum(pool);
		if ((res == 0) || ((res = auth_stratum(pool)) == 0)) {
			init_stratum_threads(pool);        /* 空桩, 入参被忽略 */
			pool_tclear(pool, (uint8_t *)pool + 0x29a);
			ret = 0;
		} else {
			init_stratum_threads(pool);        /* 空桩 */
			DAT_0007bc79 = 1;
			res = pthread_create((pthread_t *)((uint8_t *)pool + 0x2d0), NULL, stratum_sthread, pool);
			if (res != 0) {
				applog(3, "Failed to create stratum sthread", 1);
				quit(1);                       /* 不返回 */
			}
			res = pthread_create((pthread_t *)((uint8_t *)pool + 0x2d4), NULL, stratum_rthread, pool);
			if (res == 0) {
				ret = 1;
			} else {
				applog(3, "Failed to create stratum rthread", 1);
				quit(1);                       /* 不返回 */
				ret = 1;                       /* 机器码: quit 后仍置返回值 (不可达, 按反编译体保留) */
			}
		}
	} else {
		/* testing 已置位 (他线程正在测试): 返回 pool+0x299 字节 */
		ret = *(uint8_t *)((uint8_t *)pool + 0x299);
	}
	return ret;
}

// === switch_pools === 原 FUN_00019bf8 @ 0x00019bf8
// 来源: reversed/cgminer/21_pool_net/FUN_00019bf8.c | 分类: 21_pool_net
// 语义: 切换当前矿池: control_lock (mutex DAT_0007ba10 + 读写锁 DAT_0007ba28) 下,
//       若目标池 prio 非 0 则把它提到优先级队首 (其余 prio 更小的池序号 +1), 再按
//       挖矿策略 DAT_00076efc 选池: 0/3/4 (Failover/LoadBalance/Balance) 沿优先序
//       找非空闲 (idle@0x81==0) 且已启用 (enabled@0x84==1) 的池; 1/2 (RoundRobin/
//       Rotate) 从当前池号向后轮转查找, 全不可用回落当前池; >=5 保持当前。写入
//       DAT_0007705c 后解锁; 若确实换池且策略非 3/4, 告警日志并对旧池
//       clear_pool_work; failover-only 模式对新池 0x82 标志 pool_tset; 末尾广播
//       DAT_00077078 条件变量唤醒工作线程。
void switch_pools(void *pool)
{
	char logbuf[0x1000];
	void *old_pool;       /* 切换前的当前矿池 */
	void *selected;       /* 选中的新矿池 */
	int current_no;       /* 当前矿池的 pool_no */
	int prio;             /* 目标池 prio (循环内重读, 防别名) */
	int idx;
	int count;
	int res;

	res = pthread_mutex_lock(&DAT_0007ba10);           /* control_lock mutex 部分 */
	if (res != 0) {
		mutex_lock_fail_cgminer("switch_pools", 0x13ed);
	}
	res = pthread_rwlock_wrlock(&DAT_0007ba28);        /* control_lock rwlock 部分 */
	if (res != 0) {
		wr_lock_error("switch_pools", 0x13ed);
	}
	old_pool = DAT_0007705c;
	current_no = *(int *)DAT_0007705c;                 /* 当前池 pool_no */
	if ((pool != NULL) && (*(int *)((uint8_t *)pool + POOL_OFF_PRIO) != 0)) {
		/* 目标池提到优先级队首: 其余 prio 更小的池序号 +1 */
		if (0 < DAT_0007b198) {
			idx = 0;
			while (true) {
				prio = *(int *)((uint8_t *)*(void **)((uint8_t *)DAT_0007b438 + idx * 4) + POOL_OFF_PRIO);
				if (prio < *(int *)((uint8_t *)pool + POOL_OFF_PRIO)) {
					*(int *)((uint8_t *)*(void **)((uint8_t *)DAT_0007b438 + idx * 4) + POOL_OFF_PRIO) = prio + 1;
				}
				if (idx + 1 == DAT_0007b198) {
					break;
				}
				idx = idx + 1;
			}
		}
		*(int *)((uint8_t *)pool + POOL_OFF_PRIO) = 0;
	}
	if (DAT_00076efc < 5) {
		/* 策略掩码: 位 0/3/4 = Failover/LoadBalance/Balance, 位 1/2 = RoundRobin/Rotate */
		if (((1 << (DAT_00076efc & 0xff)) & 0x19) == 0) {
			if (((1 << (DAT_00076efc & 0xff)) & 6) != 0) {
				/* RoundRobin/Rotate: 指定池可用则直接用它, 否则从当前池号向后轮转 */
				if ((pool != NULL) && (*(uint8_t *)((uint8_t *)pool + 0x81) == '\0')) {
					selected = *(void **)((uint8_t *)DAT_0007b438 + *(int *)pool * 4);
					goto commit;
				}
				if (1 < DAT_0007b198) {
					count = 1;
					idx = current_no;
					do {
						idx = idx + 1;
						count = count + 1;
						if (DAT_0007b198 <= idx) {
							idx = 0;                   /* 池号环回 */
						}
						selected = *(void **)((uint8_t *)DAT_0007b438 + idx * 4);
						if ((*(uint8_t *)((uint8_t *)selected + 0x81) == '\0') &&
						    (*(int *)((uint8_t *)selected + 0x84) == 1)) {
							goto commit;
						}
					} while (count != DAT_0007b198);
					selected = *(void **)((uint8_t *)DAT_0007b438 + current_no * 4);   /* 全不可用 → 当前池 */
					goto commit;
				}
			}
		} else if (0 < DAT_0007b198) {
			/* Failover/LoadBalance/Balance: 沿优先序找首个非空闲且已启用的池 */
			idx = 0;
			do {
				selected = priority_pool(idx);
				if ((*(uint8_t *)((uint8_t *)selected + 0x81) == '\0') &&
				    (*(int *)((uint8_t *)selected + 0x84) == 1)) {
					selected = *(void **)((uint8_t *)DAT_0007b438 + *(int *)selected * 4);
					goto commit;
				}
				idx = idx + 1;
			} while (idx < DAT_0007b198);
		}
	}
	selected = *(void **)((uint8_t *)DAT_0007b438 + current_no * 4);   /* 默认/策略>=5: 当前池 */

commit:                /* LAB_00019d00 */
	DAT_0007705c = selected;
	res = pthread_rwlock_unlock(&DAT_0007ba28);
	if (res != 0) {
		rw_unlock_error("switch_pools", 0x142b);
	}
	res = pthread_mutex_unlock(&DAT_0007ba10);
	if (res != 0) {
		mutex_unlock_fail_cgminer("switch_pools", 0x142b);
	}
	(*DAT_00075ed8)();                                 /* 空操作钩子 (按机器码保留) */

	if (DAT_0007b625 != '\0') {                        /* opt_failover_only */
		pool_tset(selected, (uint8_t *)selected + 0x82);
	}
	if (((old_pool != selected) && (DAT_00076efc != 3)) && (DAT_00076efc != 4)) {
		if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
			snprintf(logbuf, 0x1000, "Switching to pool %d %s",
			         *(int *)selected, *(char **)((uint8_t *)selected + POOL_OFF_RPC_URL));
			applog(4, logbuf, 0);
		}
		clear_pool_work(old_pool);
	}
	res = pthread_mutex_lock(&DAT_00077060);
	if (res != 0) {
		mutex_lock_fail_cgminer("switch_pools", 0x143a);
	}
	pthread_cond_broadcast(&DAT_00077078);
	res = pthread_mutex_unlock(&DAT_00077060);
	if (res != 0) {
		mutex_unlock_fail_cgminer("switch_pools", 0x143c);
	}
	(*DAT_00075ed8)();
}

// === watchpool_thread === 原 FUN_00021850 @ 0x00021850
// 来源: reversed/cgminer/21_pool_net/FUN_00021850.c | 分类: 21_pool_net
// 语义: 矿池 watchdog 线程 (30s 周期): 对每个矿池先做内联的 reap_curl (回收空闲
//       超 300s 的 curl 句柄, 至少保留 1 个) 与 prune_stratum_shares (清除 120s 无
//       响应的 stratum 份额并计入丢失统计); 每 20 拍用 64 位份额计数差分更新矿池
//       算力 EMA (系数 0.63/1.63); 对空闲超 31s 的非禁用矿池执行 pool_active 测试
//       (恢复则 pool_resus), Failover 策略下稳定 5 分钟且优先级更高的池触发
//       switch_pools; 循环尾检查当前池是否 idle、rotate 策略是否到期, 休眠 30s。
void *watchpool_thread(void *arg)
{
	char logbuf[0x1000];
	void *pool;
	pthread_mutex_t *pool_mutex;
	struct timeval now_tv;         /* 每轮开始时刻 */
	struct timeval reap_tv;        /* reap_curl 采样时刻 */
	int *link;                     /* curl_ent 链域指针 (节点基址+4) */
	int *nxt;
	int *next_link;
	int *prev_link;
	int *fwd;
	int *sshare;                   /* stratum_share 表项 */
	int *tmpshare;
	void *head;                    /* stratum_shares 表头 (删除时会更新) */
	void *tbl;
	uint32_t hho;
	void *bucket;
	uint32_t bucket_idx;
	int prev;
	int curlfds;
	uint32_t reaped;
	uint32_t pruned;
	uint32_t new_lo;               /* pool+0x28: 64 位计数低字 */
	int32_t  new_hi;               /* pool+0x2c: 高字 (含借位) */
	uint32_t old_lo;               /* pool+0xb8: 上次采样低字 */
	double rate;                   /* pool+0xb0: 算力 EMA (double) */
	int ema_int;                   /* pool+0xbc: EMA 定点化 */
	time_t now_ts;
	int ticks = 0;
	int idx;
	int res;
	(void)arg;

	pthread_setcanceltype(1, NULL);            /* PTHREAD_CANCEL_ASYNCHRONOUS */
	rename_thread("Watchpool");
	set_lowprio();

	do {
		ticks = ticks + 1;
		cgtime(&now_tv);
		if (0x14 < ticks) {
			ticks = 0;
		}
		if (0 < DAT_0007b198) {
			idx = 0;
pool_loop_top:      /* LAB_000218e8 */
			do {
				pool = *(void **)((uint8_t *)DAT_0007b438 + idx * 4);
				if ((DAT_0007730d == '\0') && (DAT_00076ffc == 0)) {
					/* ---- reap_curl 内联: pool_lock (pool+0xdc) 下回收空闲 curl ---- */
					pool_mutex = (pthread_mutex_t *)((uint8_t *)pool + 0xdc);
					cgtime(&reap_tv);
					res = pthread_mutex_lock(pool_mutex);
					if (res != 0) {
						mutex_lock_fail_cgminer("reap_curl", 0x251f);
					}
					reaped = 0;
					link = *(int **)((uint8_t *)pool + 0x178);    /* 链头 next (首节点链域) */
					if (link == (int *)((uint8_t *)pool + 0x178)) {
						/* 空链: 仅解锁 */
					} else {
						curlfds = *(int *)((uint8_t *)pool + 0x140);   /* pool+0x140: 在用句柄数 */
						if (1 < curlfds) {
							/* curl_ent 布局 (机器码锁定): curl@+0, list.next@+4, list.prev@+8,
							 * tv_sec@+0xc; link 指向节点链域 (基址+4), free 时回退 4 字节 */
							next_link = *(int **)link;
							do {
								nxt = next_link;
								if (300 < reap_tv.tv_sec - *(int *)(link + 8)) {
									*(int *)((uint8_t *)pool + 0x140) = curlfds - 1;
									prev_link = *(int **)(link + 4);   /* node->prev */
									fwd = *(int **)link;               /* node->next */
									reaped = reaped + 1;
									*(int **)(fwd + 4) = prev_link;
									*prev_link = fwd;
									*(int **)link = NULL;
									*(int **)(link + 4) = NULL;
									free(link - 1);                    /* 回退到节点基址 */
								}
								if ((int *)((uint8_t *)pool + 0x178) == nxt) {
									break;
								}
								curlfds = *(int *)((uint8_t *)pool + 0x140);   /* 重读句柄数 */
								next_link = *(int **)nxt;
								link = nxt;
							} while (1 < curlfds);
						}
					}
					res = pthread_mutex_unlock(pool_mutex);
					if (res != 0) {
						mutex_unlock_fail_cgminer("reap_curl", 0x252d);
					}
					(*DAT_00075ed8)();                        /* 空操作钩子 (按机器码保留) */
					if (((reaped != 0) && (DAT_00078e09 != '\0')) &&
					    ((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0') || (6 < DAT_000766c4))) {
						snprintf(logbuf, 0x1000, "Reaped %d curl%s from pool %d",
						         reaped, (reaped == 1) ? "" : "s", *(int *)pool);
						applog(7, logbuf, 0);
					}

					/* ---- prune_stratum_shares 内联: 清 120s 无响应的 stratum 份额 ---- */
					now_ts = time(NULL);
					res = pthread_mutex_lock(&DAT_00077030);      /* stratum_lock */
					if (res != 0) {
						mutex_lock_fail_cgminer("prune_stratum_shares", 0x253c);
					}
					pruned = 0;
					if (DAT_00077048 != NULL) {
						tmpshare = *(int **)((uint8_t *)DAT_00077048 + SSHARE_OFF_NEXT);
						sshare = (int *)DAT_00077048;
						while (true) {
							head = DAT_00077048;                  /* 每轮重读表头 */
							/* 命中: sshare->work->pool == pool 且 提交时刻(sshare+0x2c)+120s < now */
							if ((pool == *(void **)(*(void **)((uint8_t *)sshare + SSHARE_OFF_WORK) + WORK_OFF_POOL)) &&
							    (*(int *)((uint8_t *)sshare + 0x2c) + 0x78 < now_ts)) {
								prev = *(int *)((uint8_t *)sshare + SSHARE_OFF_PREV);
								if ((prev == 0) && (*(int *)((uint8_t *)sshare + SSHARE_OFF_NEXT) == 0)) {
									/* 表中仅剩此元素: 释放桶数组与表结构, 表头清空 */
									free(*(void **)*(void **)DAT_00077048);   /* free(head->hh.tbl->buckets) */
									free(*(void **)head);                     /* free(head->hh.tbl) */
									DAT_00077048 = NULL;
								} else {
									/* uthash HASH_DELETE 内联 (同 clear_stratum_shares) */
									tbl = *(void **)head;
									hho = *(uint32_t *)((uint8_t *)tbl + HTBL_OFF_HHO);
									if (*(void **)((uint8_t *)tbl + HTBL_OFF_TAIL) == (uint8_t *)sshare + hho) {
										*(void **)((uint8_t *)tbl + HTBL_OFF_TAIL) =
										    (uint8_t *)*(void **)((uint8_t *)sshare + SSHARE_OFF_PREV) + hho;
										prev = *(int *)((uint8_t *)sshare + SSHARE_OFF_PREV);
									}
									if (prev == 0) {
										DAT_00077048 = *(void **)((uint8_t *)sshare + SSHARE_OFF_NEXT);
									} else {
										*(void **)((uint8_t *)*(void **)((uint8_t *)sshare + SSHARE_OFF_PREV) + hho + 8) =
										    *(void **)((uint8_t *)sshare + SSHARE_OFF_NEXT);
									}
									if (*(int *)((uint8_t *)sshare + SSHARE_OFF_NEXT) != 0) {
										*(void **)((uint8_t *)*(void **)((uint8_t *)sshare + SSHARE_OFF_NEXT) + hho + 4) =
										    *(void **)((uint8_t *)sshare + SSHARE_OFF_PREV);
									}
									bucket_idx = (*(uint32_t *)((uint8_t *)tbl + HTBL_OFF_NUMBUCKETS) - 1U) &
									             *(uint32_t *)((uint8_t *)sshare + SSHARE_OFF_HASHV);
									bucket = *(void **)((uint8_t *)tbl + HTBL_OFF_BUCKETS) + bucket_idx * 0xc;
									*(int *)((uint8_t *)bucket + 4) = *(int *)((uint8_t *)bucket + 4) - 1;
									if (*(void **)bucket == sshare) {
										*(void **)bucket = *(void **)((uint8_t *)sshare + SSHARE_OFF_HH_NEXT);
									}
									if (*(void **)((uint8_t *)sshare + SSHARE_OFF_HH_PREV) != NULL) {
										*(void **)((uint8_t *)*(void **)((uint8_t *)sshare + SSHARE_OFF_HH_PREV) + SSHARE_OFF_HH_NEXT) =
										    *(void **)((uint8_t *)sshare + SSHARE_OFF_HH_NEXT);
									}
									if (*(void **)((uint8_t *)sshare + SSHARE_OFF_HH_NEXT) != NULL) {
										*(void **)((uint8_t *)*(void **)((uint8_t *)sshare + SSHARE_OFF_HH_NEXT) + SSHARE_OFF_HH_PREV) =
										    *(void **)((uint8_t *)sshare + SSHARE_OFF_HH_PREV);
									}
									*(int *)((uint8_t *)tbl + HTBL_OFF_NUMITEMS) = *(int *)((uint8_t *)tbl + HTBL_OFF_NUMITEMS) - 1;
								}
								pruned = pruned + 1;
								free_work(*(void **)((uint8_t *)sshare + SSHARE_OFF_WORK));
								*(void **)((uint8_t *)sshare + SSHARE_OFF_WORK) = NULL;
								free(sshare);
							}
							if (tmpshare == NULL) {
								break;
							}
							sshare = tmpshare;
							tmpshare = *(int **)((uint8_t *)tmpshare + SSHARE_OFF_NEXT);
						}
					}
					res = pthread_mutex_unlock(&DAT_00077030);
					if (res != 0) {
						mutex_unlock_fail_cgminer("prune_stratum_shares", 0x2547);
					}
					(*DAT_00075ed8)();
					if (pruned != 0) {
						if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
							snprintf(logbuf, 0x1000,
							         "Lost %d shares due to no stratum share response from pool %d",
							         pruned, *(int *)pool);
							applog(4, logbuf, 0);
						}
						*(int *)((uint8_t *)pool + POOL_OFF_0X98) = *(int *)((uint8_t *)pool + POOL_OFF_0X98) + pruned;
						{
							uint32_t carry = (DAT_0007bc80 + pruned < DAT_0007bc80) ? 1U : 0U;   /* CARRY4 */
							DAT_0007bc80 = DAT_0007bc80 + pruned;
							DAT_0007bc84 = DAT_0007bc84 + (uint32_t)((int32_t)pruned >> 31) + carry;
						}
					}
				}

				if (ticks == 0x14) {
					/* 每 20 拍: (pool+0x28/0x2c 64 位计数 − pool+0xb8 上次采样) 差分做 EMA */
					new_lo = *(uint32_t *)((uint8_t *)pool + 0x28);
					old_lo = *(uint32_t *)((uint8_t *)pool + 0xb8);
					new_hi = *(int32_t *)((uint8_t *)pool + 0x2c) -
					         ((int32_t)old_lo >> 31) -
					         (int32_t)(uint32_t)(new_lo < old_lo);
					*(uint32_t *)((uint8_t *)pool + 0xb8) = new_lo;
					rate = (*(double *)((uint8_t *)pool + 0xb0) +
					        __floatdidf((int32_t)(new_lo - old_lo), new_hi) * 0.63) / 1.63;
					ema_int = (int)rate;                    /* VCVT.S32.F64 (RZ 截断) */
					*(double *)((uint8_t *)pool + 0xb0) = rate;
					*(int *)((uint8_t *)pool + 0xbc) = ema_int;
				}

				if ((*(int *)((uint8_t *)pool + 0x84) == 0) ||
				    (*(uint8_t *)((uint8_t *)pool + 0x13c) != '\0')) {
					goto next_pool;                         /* 禁用或维护中 → 下一池 (LAB_000218d8) */
				}
				if (*(uint8_t *)((uint8_t *)pool + 0x81) == '\0') {
					goto stable_check;                      /* LAB_000218cc: 非空闲 → 稳定性检查 */
				}
				if (now_tv.tv_sec - *(int *)((uint8_t *)pool + POOL_OFF_0XA8) < 0x1f) {
					goto next_pool;                         /* 31s 内测过 → 下一池 */
				}
				/* 空闲超 31s: 重测矿池 */
				res = pool_active(pool, 1);
				if ((res == 0) || ((res = pool_tclear(pool, (uint8_t *)pool + 0x81)) == 0)) {
					cgtime((struct timeval *)((uint8_t *)pool + POOL_OFF_0XA8));   /* 更新 tv_last_test */
				} else {
					pool_resus(pool);
				}
				if (*(uint8_t *)((uint8_t *)pool + 0x81) == '\0') {
					goto stable_check;
				}
				idx = idx + 1;                              /* 仍空闲 → 下一池 (while 条件判定) */
			} while (idx < DAT_0007b198);
			goto pool_loop_done;

next_pool:              /* LAB_000218d8 */
			idx = idx + 1;
			if (DAT_0007b198 <= idx) {
				goto pool_loop_done;
			}
			goto pool_loop_top;

stable_check:           /* LAB_000218cc */
			/* Failover 策略且该池优先级高于当前池且稳定 5 分钟 → 切换 */
			if (((DAT_00076efc == 0) &&
			     (*(int *)((uint8_t *)pool + POOL_OFF_PRIO) < cp_prio())) &&
			    (300 < now_tv.tv_sec - *(int *)((uint8_t *)pool + POOL_OFF_0XA8))) {
				if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
					snprintf(logbuf, 0x1000, "Pool %d %s stable for 5 mins",
					         *(int *)pool, *(char **)((uint8_t *)pool + POOL_OFF_RPC_URL));
					applog(4, logbuf, 0);
				}
				switch_pools(NULL);
			}
			goto next_pool;
		}
pool_loop_done:
		/* 循环尾: 当前池 idle → 切池; rotate 策略到期 → 切池 */
		pool = current_pool();
		if (*(uint8_t *)((uint8_t *)pool + 0x81) != '\0') {
			switch_pools(NULL);
		}
		if ((DAT_00076efc == 2) && (DAT_0007b558 * 0x3c < now_tv.tv_sec - DAT_00077310)) {
			cgtime((struct timeval *)&DAT_00077310);
			switch_pools(NULL);
		}
		cgsleep_ms(30000);
	} while (true);
}

// === stratum_sthread === 原 FUN_00021fe8 @ 0x00021fe8
// 来源: reversed/cgminer/21_pool_net/FUN_00021fe8.c | 分类: 21_pool_net
// 语义: stratum 发送线程体: 循环从矿池 stratum_q(+0x2f0) 阻塞取 work, 构造
//       mining.submit 请求串 (id=自增 sshare_id, params=[rpc_user, job_id,
//       ntime, nonce2hex, 参数8=work+0x16c 起 0x543 字节]) 经 stratum_send 发送;
//       发送失败按三条路径放弃 (超 119s / lowmem 标志 / work->nonce1 与当前会话
//       pool+0x27c 不匹配), 否则 sleep(5) 重试; 发送成功后在 stratum_lock 下把
//       share 挂入全局 stratum_shares uthash (含建表与桶扩展全内联代码), 清
//       pool+0x80 提交失败标志并记 submission lag time 日志。pool+0x89 停机标志
//       置位时冻结队列并返回 0 退出线程。
void *stratum_sthread(void *arg)
{
	void *pool = arg;
	char thname[16];               /* "线程名" 16 字节缓冲 (acStack_2038) */
	char submit_req[0x1000];       /* mining.submit 请求串 (acStack_2028) */
	char logbuf[0x1000];
	void *work;                    /* tq_pop 取出的 work */
	int *sshare;                   /* 新分配的 stratum_share (0x34 字节) */
	char *nonce2_hex;              /* bin2hex(work+0x14c, 0x20): nonce1hex||nonce2hex */
	char *param8_hex;              /* bin2hex(work+0x16c, 0x543) */
	size_t nonce1_len;
	time_t tv;
	time_t now;
	pthread_t self;
	int nonce1_ok;
	int stratum_q;                 /* pool+0x2f0 队列指针 (int 槽位) */
	uint8_t shutdown_flag;         /* pool+0x89 (循环尾重读, 循环顶检查) */
	uint8_t *tbl_b;                /* head->hh.tbl (UT_hash_table) */
	uint8_t *bkt;                  /* 目标桶 (12 字节) */
	uint8_t *nbkt;                 /* 桶扩展新桶数组 */
	uint32_t bkt_idx;
	uint32_t nb_old;               /* 扩展前的 num_buckets */
	uint32_t nbkt_bytes;
	uint32_t new_idx;
	uint32_t cnt;
	uint32_t bkt_i;
	uint32_t hh_off;               /* 旧桶数组字节偏移 */
	uint8_t *hh;
	uint8_t *hh_next;
	int old_head;                  /* 桶链原头 (hh 指针) */
	uint32_t jk_c;                 /* 内联 HASH_JEN 混合槽位 c (机器码 uVar12) */
	uint32_t jk_b;                 /* 槽位 b (机器码 uVar10) */
	uint32_t jk_a;                 /* 槽位 a (机器码 uVar8/uVar13) */
	uint32_t jk_a2;
	int res;

	self = pthread_self();
	pthread_detach(self);
	snprintf(thname, 0x10, "%d/SStratum", *(int *)((uint8_t *)pool + POOL_OFF_POOL_NO));
	rename_thread(thname);
	stratum_q = (int)tq_new();
	*(int *)((uint8_t *)pool + POOL_OFF_STRATUM_Q) = stratum_q;
	if (stratum_q == 0) {
		applog(3, "Failed to create stratum_q in stratum_sthread", 1);
		quit(1);                                   /* 不返回 */
	}
	memset(submit_req, 0, 0x1000);
	shutdown_flag = *(uint8_t *)((uint8_t *)pool + POOL_OFF_0X89);

	for (;;) {
		if (shutdown_flag != 0) {
			tq_freezethaw((void *)stratum_q);      /* 置冻结标志, 唤醒等待者 */
			return NULL;                           /* 机器码返回 0 */
		}
		work = tq_pop((void *)stratum_q, NULL);    /* 永久阻塞取 work */
		if (work == NULL) {
			applog(3, "Stratum q returned empty work", 1);
			quit(1);                               /* 不返回 */
		}
		sshare = calloc(0x34, 1);                  /* stratum_share 0x34 字节 */
		if (sshare == NULL) {
			snprintf(logbuf, 0x1000, "%s: calloc() failed on sshare.", "stratum_sthread");
			applog(3, logbuf, 1);
			quit(1);                               /* 不返回 */
		}
		tv = time(NULL);
		*(void **)((uint8_t *)sshare + SSHARE_OFF_WORK) = work;
		*(time_t *)((uint8_t *)sshare + SSHARE_OFF_TV_SEC) = tv;   /* 创建时刻 */
		nonce2_hex = bin2hex((uint8_t *)work + WORK_OFF_NONCE1BIN, 0x20);
		param8_hex = bin2hex((uint8_t *)work + WORK_OFF_0X16C, 0x543);

		res = pthread_mutex_lock(&DAT_00077030);       /* stratum_lock */
		if (res != 0) {
			mutex_lock_fail_cgminer("stratum_sthread", 0x1c78);
		}
		*(int *)((uint8_t *)sshare + SSHARE_OFF_ID) = DAT_0007b0f8;   /* sshare_id = 当前计数 */
		DAT_0007b0f8 = DAT_0007b0f8 + 1;
		res = pthread_mutex_unlock(&DAT_00077030);
		if (res != 0) {
			mutex_unlock_fail_cgminer("stratum_sthread", 0x1c7b);
		}
		(*DAT_00075ed8)();                             /* 空操作钩子 (按机器码保留) */

		nonce1_len = strlen(*(char **)((uint8_t *)work + WORK_OFF_NONCE1_STR));
		snprintf(submit_req, 0x1000,
		         "{\"id\": %d, \"method\": \"mining.submit\", \"params\": [\"%s\", \"%s\", \"%s\", \"%s\", \"%s\"]}",
		         *(int *)((uint8_t *)sshare + SSHARE_OFF_ID),
		         *(char **)((uint8_t *)pool + POOL_OFF_RPC_USER),
		         *(char **)((uint8_t *)work + WORK_OFF_JOB_ID),
		         *(char **)((uint8_t *)work + WORK_OFF_NTIME_STR),
		         nonce2_hex + nonce1_len,              /* 跳过 nonce1hex → 纯 nonce2hex */
		         param8_hex);
		free(nonce2_hex);
		free(param8_hex);

		/* ---- 提交重试环 ---- */
		while (true) {
			now = time(NULL);
			if (*(time_t *)((uint8_t *)sshare + SSHARE_OFF_TV_SEC) + 0x77 < now) {
				goto share_tail;                   /* LAB_0002265c: 超 119s 放弃提交 */
			}
			res = stratum_send(pool, submit_req, (int)strlen(submit_req));
			if (res == 0) {
				break;                             /* 发送成功 → 入哈希表 */
			}
			/* 发送失败: 首次失败 (标志原值 0) 且该池仍需维持连接时计数 */
			if ((pool_tset(pool, (uint8_t *)pool + POOL_OFF_0X80) == 0) &&
			    (cnx_needed(pool) != 0)) {
				if ((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0') || (3 < DAT_000766c4))) {
					snprintf(logbuf, 0x1000, "Pool %d stratum share submission failure",
					         *(int *)((uint8_t *)pool + POOL_OFF_POOL_NO));
					applog(4, logbuf, 0);
				}
				DAT_0007ae68 = DAT_0007ae68 + 1;
				*(int *)((uint8_t *)pool + POOL_OFF_0XA4) =
				    *(int *)((uint8_t *)pool + POOL_OFF_0XA4) + 1;
			}
			if (DAT_0007b644 != '\0') {            /* lowmem: 不重提交 */
				if ((DAT_00078e09 != '\0') &&
				    (((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0') || (6 < DAT_000766c4))))) {
					applog(7, "Lowmem option prevents resubmitting stratum share", 0);
				}
				goto share_tail;
			}
			/* data_lock(pool+0xf4) + 数据读锁(pool+0x10c) 下校验会话 nonce1 一致性 */
			res = pthread_mutex_lock((pthread_mutex_t *)((uint8_t *)pool + 0xf4));
			if (res != 0) {
				mutex_lock_fail_cgminer("stratum_sthread", 0x1ca2);
			}
			res = pthread_rwlock_rdlock((pthread_rwlock_t *)((uint8_t *)pool + 0x10c));
			if (res != 0) {
				rd_lock_error("stratum_sthread", 0x1ca2);
			}
			res = pthread_mutex_unlock((pthread_mutex_t *)((uint8_t *)pool + 0xf4));
			if (res != 0) {
				mutex_unlock_fail_cgminer("stratum_sthread", 0x1ca2);
			}
			if (*(char **)((uint8_t *)pool + POOL_OFF_NONCE1) == NULL) {
				nonce1_ok = 0;
			} else {
				res = strcmp(*(char **)((uint8_t *)work + WORK_OFF_NONCE1_STR),
				             *(char **)((uint8_t *)pool + POOL_OFF_NONCE1));
				nonce1_ok = (res == 0);            /* 机器码: 1-ret, ret>1 时清 0 */
			}
			res = pthread_rwlock_unlock((pthread_rwlock_t *)((uint8_t *)pool + 0x10c));
			if (res != 0) {
				rw_unlock_error("stratum_sthread", 0x1ca4);
			}
			(*DAT_00075ed8)();
			if (nonce1_ok == 0) {
				if ((DAT_00078e09 != '\0') &&
				    (((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0') || (6 < DAT_000766c4))))) {
					applog(7, "No matching session id for resubmitting stratum share", 0);
				}
				goto share_tail;
			}
			sleep(5);
		}

		/* ---- 发送成功: HASH_ADD_INT(sshare_id) 内联, stratum_lock 下 ---- */
		res = pthread_mutex_lock(&DAT_00077030);       /* stratum_lock */
		if (res != 0) {
			mutex_lock_fail_cgminer("stratum_sthread", 0x1c8a);
		}
		*(int *)((uint8_t *)sshare + SSHARE_OFF_NEXT) = 0;         /* hh.next = NULL */
		*(int *)((uint8_t *)sshare + SSHARE_OFF_KEY) = (int)((uint8_t *)sshare + SSHARE_OFF_ID);
		*(int *)((uint8_t *)sshare + SSHARE_OFF_KEYLEN) = 4;       /* key = &sshare_id, len = sizeof(int) */
		if (DAT_00077048 == NULL) {
			/* 空表: 建表 (UT_hash_table 0x2c 字节 + 32 桶 × 12 字节) */
			*(int *)((uint8_t *)sshare + SSHARE_OFF_PREV) = 0;     /* hh.prev = NULL */
			DAT_00077048 = (void *)sshare;
			tbl_b = malloc(0x2c);
			*(uint8_t **)((uint8_t *)sshare + SSHARE_OFF_TBL) = tbl_b;
			if (tbl_b == NULL) {
				goto hash_alloc_fail;                  /* LAB_000227ec */
			}
			*(uint32_t *)(tbl_b + HTBL_OFF_BUCKETS) = 0;
			*(uint32_t *)(tbl_b + HTBL_OFF_NUMITEMS) = 0;
			*(uint32_t *)(tbl_b + HTBL_OFF_TAIL) = 0;
			*(uint32_t *)(tbl_b + HTBL_OFF_HHO) = 0;
			*(uint32_t *)(tbl_b + HTBL_OFF_IDEALMAX) = 0;
			*(uint32_t *)(tbl_b + HTBL_OFF_NONIDEAL) = 0;
			*(uint32_t *)(tbl_b + HTBL_OFF_INEFF) = 0;
			*(uint32_t *)(tbl_b + HTBL_OFF_NOEXPAND) = 0;
			*(uint32_t *)(tbl_b + HTBL_OFF_SIGNATURE) = 0;
			*(uint32_t *)(tbl_b + HTBL_OFF_TAIL) = (uint32_t)sshare;   /* 表尾 = 首元素 */
			*(uint32_t *)(tbl_b + HTBL_OFF_HHO) = 0;               /* hh 在元素首, hho = 0 */
			*(uint32_t *)(tbl_b + HTBL_OFF_NUMBUCKETS) = 0x20;     /* 32 桶 */
			*(uint32_t *)(tbl_b + HTBL_OFF_LOG2NUM) = 5;
			nbkt = malloc(0x180);
			*(uint8_t **)(tbl_b + HTBL_OFF_BUCKETS) = nbkt;
			if (nbkt == NULL) {
				goto hash_alloc_fail;                  /* LAB_000227ec */
			}
			memset(nbkt, 0, 0x180);
			*(uint32_t *)(tbl_b + HTBL_OFF_SIGNATURE) = 0xa0111fe1;    /* HASH_BLOOM_SIGNATURE */
		} else {
			/* 非空表: 应用序表尾追加 (hh.prev/next 存元素指针, 经 hho 换算) */
			tbl_b = *(uint8_t **)DAT_00077048;                 /* head->hh.tbl */
			*(int **)(*(int *)(tbl_b + HTBL_OFF_TAIL) + 8) = (int *)sshare;
			*(int *)((uint8_t *)sshare + SSHARE_OFF_PREV) =
			    *(int *)(tbl_b + HTBL_OFF_TAIL) - *(int *)(tbl_b + HTBL_OFF_HHO);
			*(uint32_t *)(tbl_b + HTBL_OFF_TAIL) = (uint32_t)sshare;
		}
		/* 公共: num_items++ 与 hh.tbl 回填 */
		tbl_b = *(uint8_t **)((uint8_t *)DAT_00077048 + SSHARE_OFF_TBL);
		*(uint32_t *)(tbl_b + HTBL_OFF_NUMITEMS) = *(uint32_t *)(tbl_b + HTBL_OFF_NUMITEMS) + 1;
		*(uint8_t **)((uint8_t *)sshare + SSHARE_OFF_TBL) = tbl_b;

		/* 内联 Jenkins HASH_JEN (key = sshare_id 4 字节小端), 机器码逐字对应 */
		jk_c = (uint32_t)*(uint8_t *)((uint8_t *)sshare + SSHARE_OFF_ID) + 0x112410du
		     + (uint32_t)*(uint8_t *)((uint8_t *)sshare + 0x2b) * 0x1000000u
		     + (uint32_t)*(uint8_t *)((uint8_t *)sshare + 0x2a) * 0x10000u
		     + (uint32_t)*(uint8_t *)((uint8_t *)sshare + 0x29) * 0x100u;
		jk_c = jk_c ^ 0x7f76du;
		jk_b = (0x9f49bac6u - jk_c) ^ (jk_c << 8);
		jk_a = (0x9edcbdf3u - jk_c - jk_b) ^ (jk_b >> 13);         /* 0x9edcbdf3 = -0x112410d */
		jk_c = (jk_c - jk_b - jk_a) ^ (jk_a >> 12);
		jk_b = (jk_b - jk_a - jk_c) ^ (jk_c << 16);
		jk_a2 = (jk_a - jk_c - jk_b) ^ (jk_b >> 5);
		jk_a = (jk_c - jk_b - jk_a2) ^ (jk_a2 >> 3);
		jk_c = (jk_b - jk_a2 - jk_a) ^ (jk_a << 10);
		jk_a = (jk_a2 - jk_a - jk_c) ^ (jk_c >> 15);
		*(uint32_t *)((uint8_t *)sshare + SSHARE_OFF_HASHV) = jk_a;

		/* 桶定位与头插 */
		bkt_idx = jk_a & (*(uint32_t *)(tbl_b + HTBL_OFF_NUMBUCKETS) - 1U);
		bkt = *(uint8_t **)tbl_b + bkt_idx * 0xc;
		*(uint32_t *)(bkt + 4) = *(uint32_t *)(bkt + 4) + 1;       /* 桶计数++ */
		old_head = *(int *)(*(uint8_t **)tbl_b + bkt_idx * 0xc);   /* 原桶头 hh */
		*(uint32_t *)((uint8_t *)sshare + SSHARE_OFF_HH_PREV) = 0;
		*(uint32_t *)((uint8_t *)sshare + SSHARE_OFF_HH_NEXT) = (uint32_t)old_head;
		if (old_head != 0) {
			*(int *)(old_head + SSHARE_OFF_HH_PREV) = (int)sshare; /* 原桶头->hh_prev = 新元素 */
		}
		*(int *)(*(uint8_t **)tbl_b + bkt_idx * 0xc) = (int)sshare; /* bucket->hh_head = 新元素 */

		/* 桶过载 (expand_mult+1)*10 <= count 且未禁扩展 → 桶扩展 (uthash HASH_EXPAND_BUCKETS 内联) */
		if (((*(uint32_t *)(bkt + 8) + 1) * 10 <= *(uint32_t *)(bkt + 4)) &&
		    (*(uint32_t *)(tbl_b + HTBL_OFF_NOEXPAND) != 1)) {
			nbkt_bytes = (uint32_t)(*(int *)(tbl_b + HTBL_OFF_NUMBUCKETS) * 0x18);  /* 2×桶数×12B */
			nbkt = malloc(nbkt_bytes);
			if (nbkt == NULL) {
				goto hash_alloc_fail;                  /* LAB_000227ec */
			}
			memset(nbkt, 0, nbkt_bytes);
			hh_off = 0;                                /* 旧桶数组字节偏移 */
			/* ideal_chain_maxlen = ceil(num_items / (num_buckets*2)); 移位量按 ARM 取低 8 位 */
			*(uint32_t *)(tbl_b + HTBL_OFF_IDEALMAX) =
			    (uint32_t)(((uint32_t)(*(int *)(tbl_b + HTBL_OFF_NUMBUCKETS) * 2 - 1U) &
			                *(uint32_t *)(tbl_b + HTBL_OFF_NUMITEMS)) != 0) +
			    (*(uint32_t *)(tbl_b + HTBL_OFF_NUMITEMS) >> ((*(int *)(tbl_b + HTBL_OFF_LOG2NUM) + 1) & 0xff));
			*(uint32_t *)(tbl_b + HTBL_OFF_NONIDEAL) = 0;
			nb_old = *(uint32_t *)(tbl_b + HTBL_OFF_NUMBUCKETS);
			for (bkt_i = 0; bkt_i < nb_old; bkt_i++, hh_off += 0xc) {
				hh = *(uint8_t **)(*(uint8_t **)tbl_b + hh_off);   /* 旧桶 hh_head */
				if (hh == NULL) {
					continue;
				}
				do {
					hh_next = *(uint8_t **)(hh + SSHARE_OFF_HH_NEXT);
					new_idx = (nb_old * 2 - 1U) & *(uint32_t *)(hh + SSHARE_OFF_HASHV);
					cnt = *(uint32_t *)(nbkt + new_idx * 0xc + 4) + 1;
					*(uint32_t *)(nbkt + new_idx * 0xc + 4) = cnt;
					if (*(uint32_t *)(tbl_b + HTBL_OFF_IDEALMAX) < cnt) {
						*(uint32_t *)(tbl_b + HTBL_OFF_NONIDEAL) =
						    *(uint32_t *)(tbl_b + HTBL_OFF_NONIDEAL) + 1;
						/* __udivsi3: expand_mult = count / ideal_chain_maxlen */
						*(uint32_t *)(nbkt + new_idx * 0xc + 8) =
						    cnt / *(uint32_t *)(tbl_b + HTBL_OFF_IDEALMAX);
					}
					*(uint32_t *)(hh + SSHARE_OFF_HH_PREV) = 0;
					*(uint32_t *)(hh + SSHARE_OFF_HH_NEXT) = *(uint32_t *)(nbkt + new_idx * 0xc);
					old_head = *(int *)(nbkt + new_idx * 0xc);
					if (old_head != 0) {
						*(int *)(old_head + SSHARE_OFF_HH_PREV) = (int)hh;
					}
					*(int *)(nbkt + new_idx * 0xc) = (int)hh;      /* 新桶头 = hh */
					hh = hh_next;
				} while (hh != NULL);
			}
			free(*(void **)tbl_b);                     /* 释放旧桶数组 */
			*(int *)(tbl_b + HTBL_OFF_NUMBUCKETS) = *(int *)(tbl_b + HTBL_OFF_NUMBUCKETS) << 1;
			*(int *)(tbl_b + HTBL_OFF_LOG2NUM) = *(int *)(tbl_b + HTBL_OFF_LOG2NUM) + 1;
			*(uint8_t **)(tbl_b + HTBL_OFF_BUCKETS) = nbkt;
			/* 非理想项超过半数 → ineff_expands++, 连续 2 次后置 noexpand 禁止再扩展 */
			if (*(uint32_t *)(tbl_b + HTBL_OFF_NONIDEAL) > (*(uint32_t *)(tbl_b + HTBL_OFF_NUMITEMS) >> 1)) {
				*(int *)(tbl_b + HTBL_OFF_INEFF) = *(int *)(tbl_b + HTBL_OFF_INEFF) + 1;
			} else {
				*(int *)(tbl_b + HTBL_OFF_INEFF) = 0;
			}
			if (1 < *(uint32_t *)(tbl_b + HTBL_OFF_INEFF)) {
				*(uint32_t *)(tbl_b + HTBL_OFF_NOEXPAND) = 1;
			}
		}
		*(int *)((uint8_t *)pool + POOL_OFF_0X2F4) = *(int *)((uint8_t *)pool + POOL_OFF_0X2F4) + 1;
		res = pthread_mutex_unlock(&DAT_00077030);
		if (res != 0) {
			mutex_unlock_fail_cgminer("stratum_sthread", 0x1c8d);
		}
		(*DAT_00075ed8)();

		/* 提交失败标志此前置位且本轮发送成功 → 通报恢复 */
		if ((pool_tclear(pool, (uint8_t *)pool + POOL_OFF_0X80) != 0) &&
		    ((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0') || (3 < DAT_000766c4)))) {
			snprintf(logbuf, 0x1000, "Pool %d communication resumed, submitting work",
			         *(int *)((uint8_t *)pool + POOL_OFF_POOL_NO));
			applog(4, logbuf, 0);
		}
		if ((DAT_00078e09 != '\0') &&
		    (((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0') || (6 < DAT_000766c4))))) {
			applog(7, "Successfully submitted, adding to stratum_shares db", 0);
		}

share_tail:                /* LAB_0002265c: 提交收尾 (成功/超时/lowmem/会话不匹配共同出口) */
		now = time(NULL);
		*(time_t *)((uint8_t *)sshare + SSHARE_OFF_TAILTV) = now;
		if ((DAT_00078e09 != '\0') &&
		    (((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0') || (6 < DAT_000766c4))))) {
			snprintf(logbuf, 0x1000, "Pool %d stratum share submission lag time %d seconds",
			         *(int *)((uint8_t *)pool + POOL_OFF_POOL_NO),
			         (int)(now - *(time_t *)((uint8_t *)sshare + SSHARE_OFF_TV_SEC)));
			applog(7, logbuf, 0);
		}
		memset(submit_req, 0, 0x1000);
		shutdown_flag = *(uint8_t *)((uint8_t *)pool + POOL_OFF_0X89);   /* 循环顶复查 */
		}

hash_alloc_fail:           /* LAB_000227ec: 建表/扩桶 malloc 失败 (仅 goto 进入) */
	exit(-1);
}

// === socks4_negotiate === 原 FUN_00028fc8 @ 0x00028fc8
// 来源: reversed/cgminer/21_pool_net/FUN_00028fc8.c | 分类: 21_pool_net
// 语义: 经 SOCKS4 代理协商连接: 构造请求 (VER=4, CMD=1, DSTPORT 取 pool+0x260
//       串大端两字节, userid="CGMINER\0"); 目标地址取 pool+0x270: inet_addr 解析
//       成功 → 4 字节大端写入请求发 0x10 字节; 主机名时若第 3 参非 0 (socks4a)
//       则 IP 字段置 0.0.0.1 并附加主机名串 (限 255 字节) 发 n+0x11 字节, 否则
//       报 "Invalid IP address" 返回 0。应答首两字节为 0x00,0x5a 再丢弃 6 字节
//       返回 1, 否则报 "Bad response from %s:%s SOCKS4 server" 返回 0。
int socks4_negotiate(void *pool, int fd, int socks4a)
{
	char logbuf[0x1000];
	uint8_t req[0x210];            /* 0x10 请求头 + 500 字节主机名区 (机器栈 auStack_121c[500]) */
	uint32_t ip;                   /* 主机序目标 IP (0xffffffff = 解析失败) */
	unsigned int port;
	size_t n;
	struct addrinfo hints;         /* getaddrinfo hints (ai_family=AF_INET) */
	struct addrinfo *res;
	struct addrinfo res_stub;      /* 机器码 aStack_124c: res 指针初值占位 (失败路径不使用) */
	int discard;
	int ret;

	req[0] = 4;                    /* VER = SOCKS4 */
	req[1] = 1;                    /* CMD = CONNECT */
	port = (unsigned int)strtol(*(char **)((uint8_t *)pool + 0x260), NULL, 10);
	req[2] = (uint8_t)(port >> 8);             /* DSTPORT 大端两字节 */
	req[3] = (uint8_t)port;
	memcpy(req + 8, "CGMINER", 8);             /* userid "CGMINER\0" (机器逐字节置位) */
	ip = (uint32_t)inet_addr(*(char **)((uint8_t *)pool + 0x270));
	/* 32 位字节序反转: inet_addr 网络序 → 主机序 (按机器码) */
	ip = ip << 0x18 | (ip >> 8 & 0xff) << 0x10 | (ip >> 0x10 & 0xff) << 8 | ip >> 0x18;
	if (ip == 0xffffffffu) {
		/* 非"点分十进制"目标: AF_INET getaddrinfo 解析 */
		memset(&res_stub, 0, sizeof(res_stub));
		res = &res_stub;
		hints.ai_flags = 0;
		hints.ai_family = 2;                   /* AF_INET */
		hints.ai_socktype = 0;
		hints.ai_protocol = 0;
		hints.ai_addrlen = 0;
		hints.ai_addr = NULL;
		hints.ai_canonname = NULL;
		hints.ai_next = NULL;
		ret = getaddrinfo(*(char **)((uint8_t *)pool + 0x270), NULL, &hints, &res);
		if (ret == 0) {
			/* sin_addr 位于 sockaddr 偏移 4 (sa_data+2): 读 4 字节再反字节序 */
			ip = *(uint32_t *)(res->ai_addr->sa_data + 2);
			freeaddrinfo(res);
			ip = ip << 0x18 | (ip >> 8 & 0xff) << 0x10 | (ip >> 0x10 & 0xff) << 8 | ip >> 0x18;
			if (ip == 0xffffffffu) {
				goto invalid_ip;               /* LAB_0002920c */
			}
			goto send_ip_req;                  /* LAB_00029044 */
		}
		if (socks4a == 0) {
invalid_ip:                /* LAB_0002920c */
			if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 4)) {
				return 0;
			}
			snprintf(logbuf, 0x1000, "Invalid IP address specified for socks4 proxy: %s",
			         *(char **)((uint8_t *)pool + 0x270));
			applog(4, logbuf, 0);              /* LAB_000290d0 */
			return 0;
		}
		/* socks4a: DSTADDR = 0.0.0.1 标记, userid 后附主机名串 */
		req[4] = 0;
		req[5] = 0;
		req[6] = 0;
		req[7] = 1;
		n = strlen(*(char **)((uint8_t *)pool + 0x270));
		if (0xfe < (int)n) {
			n = 0xff;
		}
		memcpy(req + 0x10, *(char **)((uint8_t *)pool + 0x270), n);
		req[0x10 + n] = 0;
		send(fd, req, (int)n + 0x11, 0);
	} else {
send_ip_req:               /* LAB_00029044 */
		req[4] = (uint8_t)(ip >> 0x18);        /* DSTADDR 大端 4 字节 */
		req[5] = (uint8_t)(ip >> 0x10);
		req[6] = (uint8_t)(ip >> 8);
		req[7] = (uint8_t)ip;
		send(fd, req, 0x10, 0);
	}
	/* 应答: VN=0 且 CD=0x5a (granted) → 丢弃其后 6 字节 */
	if ((recv_byte(fd) == 0) && (recv_byte(fd) == 0x5a)) {
		discard = 6;
		do {
			recv_byte(fd);
			discard = discard - 1;
		} while (discard != 0);
		return 1;
	}
	if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 4)) {
		return 0;
	}
	snprintf(logbuf, 0x1000, "Bad response from %s:%s SOCKS4 server",
	         *(char **)((uint8_t *)pool + 0x274), *(char **)((uint8_t *)pool + 0x278));
	applog(4, logbuf, 0);                      /* LAB_000290d0 */
	return 0;
}

// === recv_line === 原 FUN_0002b040 @ 0x0002b040
// 来源: reversed/cgminer/21_pool_net/FUN_0002b040.c | 分类: 21_pool_net
// 语义: 从 pool sock (+0x264) 读一行以 \n 结尾的报文: sockbuf(+0x268) 已含 \n
//       则直接解析; 否则 socket_full(60s) 等数据后循环 recv (0x1ffc 字节) 追加进
//       sockbuf (不足时按 0x2000 对齐 realloc, 即内联 recalloc_sock), 超过 59s 或
//       出现 \n 停止; recv=0 视为对端关闭、recv<0 时 EAGAIN 且剩余超时未到则重试,
//       其余失败经 suspend_stratum 重连。strtok 取首行 strdup 返回并把剩余内容
//       memmove 回 sockbuf 头部, 同时累计 3 组 64 位统计 (+0x220 行数 / +0x228
//       +0x230 字节数); opt_protocol (DAT_0007b380) 开启时打印 "RECVD: %s"。
//       无行可取时 clear_sock 并返回 NULL。
char *recv_line(void *pool)
{
	char logbuf[0x1000];
	char recvbuf[0x2000];          /* recv 暂存区 (机器 local_2030 区) */
	char *buf;                     /* pool->sockbuf (+0x268) */
	char *line;                    /* strtok/strdup 出的行 */
	size_t len;                    /* 解析前 sockbuf 总长 */
	size_t tok_len;                /* 当前行长 */
	size_t chunk_len;              /* 本轮 recv 串长 */
	size_t old_len;                /* realloc 前 sockbuf 长 */
	size_t new_size;
	char *nb;
	struct timeval tv_start;       /* 进入等待时刻 */
	struct timeval tv_mid;         /* 每轮 recv 后时刻 */
	int elapsed;                   /* 累计等待秒数 */
	ssize_t nrecv;
	int discard;
	int ret;

	buf = *(char **)((uint8_t *)pool + 0x268);
	if (strchr(buf, '\n') == NULL) {
		cgtime(&tv_start);
		if (socket_full(pool, 0x3c) == false) {    /* 等数据最多 60s */
			if ((DAT_00078e09 != '\0') &&
			    (((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0') || (6 < DAT_000766c4))))) {
				applog(7, "Timed out waiting for data on socket_full", 0);
			}
			goto clear_out;                        /* LAB_0002b224 */
		}
		while (true) {
			memset(recvbuf, 0, 0x2000);
			nrecv = recv(*(int *)((uint8_t *)pool + 0x264), recvbuf, 0x1ffc, 0);
			if (nrecv == 0) {
				break;                             /* 对端关闭 → 循环外处理 */
			}
			cgtime(&tv_mid);
			elapsed = (int)((double)(int)(tv_mid.tv_sec - tv_start.tv_sec) +
			                (double)(int)(tv_mid.tv_usec - tv_start.tv_usec) / 1000000.0);
			if (nrecv < 0) {
				/* EAGAIN 且剩余超时内仍有数据 → 重试; 其余 → 报错重连 */
				if ((*__errno_location() != 0xb) || (socket_full(pool, 0x3c - elapsed) == false)) {
					if ((DAT_00078e09 != '\0') &&
					    (((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0') || (6 < DAT_000766c4))))) {
						applog(7, "Failed to recv sock in recv_line", 0);
					}
					goto reconnect;                /* LAB_0002b3fc */
				}
			} else {
				/* recalloc_sock 内联: sockbuf 不足时按 0x2000 对齐扩容 */
				chunk_len = strlen(recvbuf);
				buf = *(char **)((uint8_t *)pool + 0x268);
				old_len = strlen(buf);
				if (*(uint32_t *)((uint8_t *)pool + 0x26c) <= chunk_len + old_len + 1) {
					new_size = ((chunk_len + old_len + 1) & 0xffffe000u) + 0x2000;
					nb = realloc(buf, new_size);
					*(char **)((uint8_t *)pool + 0x268) = nb;
					if (nb == NULL) {
						snprintf(logbuf, 0x1000, "Failed to realloc pool sockbuf in %s %s():%d",
						         "util.c", "recalloc_sock", 0x7cd);
						applog(3, logbuf, 1);
						quit(1);                   /* 不返回 */
						nb = *(char **)((uint8_t *)pool + 0x268);   /* 机器码: quit 后仍重读 (不可达) */
					}
					memset(nb + old_len, 0, new_size - old_len);
					buf = *(char **)((uint8_t *)pool + 0x268);
					*(uint32_t *)((uint8_t *)pool + 0x26c) = (uint32_t)new_size;
				}
				strcat(buf, recvbuf);
			}
			buf = *(char **)((uint8_t *)pool + 0x268);
			if ((0x3b < elapsed) || (strchr(buf, '\n') != NULL)) {
				goto parse;                        /* LAB_0002b06c: 超 59s 或已有整行 */
			}
		}
		if ((DAT_00078e09 != '\0') &&
		    (((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0') || (6 < DAT_000766c4))))) {
			applog(7, "Socket closed waiting in recv_line", 0);
		}
reconnect:                 /* LAB_0002b3fc: 对端关闭/接收失败 → 重连链 */
		suspend_stratum(pool);
		buf = *(char **)((uint8_t *)pool + 0x268);
	}

parse:                     /* LAB_0002b06c */
	len = strlen(buf);
	line = strtok(buf, "\n");
	if (line == NULL) {
		if ((DAT_00078e09 != '\0') &&
		    (((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0') || (6 < DAT_000766c4))))) {
			applog(7, "Failed to parse a \\n terminated string in recv_line", 0);
		}
		goto clear_out;                        /* LAB_0002b224 */
	}
	line = strdup(line);
	tok_len = strlen(line);
	if ((int)(tok_len + 1) < (int)len) {
		/* 剩余内容 (含 \0) 前移覆盖已取走的行 */
		memmove(*(void **)((uint8_t *)pool + 0x268),
		        (char *)*(void **)((uint8_t *)pool + 0x268) + tok_len + 1,
		        (len - tok_len) + 1);
	} else {
		**(char **)((uint8_t *)pool + 0x268) = 0;
	}
	/* 3 组 64 位统计 (CARRY4 进位): 行数(+0x220)++ / 行宽(+0x228) / 字节(+0x230) 各加行长 */
	{
		uint32_t cnt_lo = *(uint32_t *)((uint8_t *)pool + 0x220);
		*(uint32_t *)((uint8_t *)pool + 0x220) = cnt_lo + 1;
		*(uint32_t *)((uint8_t *)pool + 0x224) =
		    *(uint32_t *)((uint8_t *)pool + 0x224) + (uint32_t)(0xfffffffeu < cnt_lo);
	}
	{
		uint32_t lo = *(uint32_t *)((uint8_t *)pool + 0x228);
		*(uint32_t *)((uint8_t *)pool + 0x228) = lo + (uint32_t)tok_len;
		*(uint32_t *)((uint8_t *)pool + 0x22c) = *(uint32_t *)((uint8_t *)pool + 0x22c) +
		    (uint32_t)((int)tok_len >> 31) +
		    (uint32_t)((lo + (uint32_t)tok_len) < lo);                     /* CARRY4 */
	}
	{
		uint32_t lo = *(uint32_t *)((uint8_t *)pool + 0x230);
		*(uint32_t *)((uint8_t *)pool + 0x230) = lo + (uint32_t)tok_len;
		*(uint32_t *)((uint8_t *)pool + 0x234) = *(uint32_t *)((uint8_t *)pool + 0x234) +
		    (uint32_t)((int)tok_len >> 31) +
		    (uint32_t)((lo + (uint32_t)tok_len) < lo);                     /* CARRY4 */
	}
	if (line != NULL) {
		if (DAT_0007b380 == '\0') {
			return line;
		}
		if (DAT_00078e09 == '\0') {
			return line;
		}
		if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7)) {
			return line;
		}
		snprintf(logbuf, 0x1000, "RECVD: %s", line);
		applog(7, logbuf, 0);
		return line;
	}

clear_out:                 /* LAB_0002b224 */
	clear_sock(pool);
	return NULL;
}

// === lcd === 原 FUN_00034ed4 @ 0x00034ed4
// 来源: reversed/cgminer/21_pool_net/FUN_00034ed4.c | 分类: 21_pool_net
// 语义: API "lcd" 命令 — message(7) 头后构造一条 api_data 链并 print_data 输出:
//       LCD(="0") / GHS5s(= DAT_00077158) / GHSavg(= DAT_0007acc0/1000/运行秒数
//       DAT_000755f8) / fan(= DAT_0007b8a8 十进制串) / temp(= DAT_0007bb20 十进制
//       串) / pool+user (当前池非空 → 其 rpc_url/rpc_user, 为空 → "no"/"no"。
//       "no" 串按机器码写在 temp 串缓冲区内偏移 0x20/0x40 处 — 栈复用)。
//       isjson 时先 io_add(",\"POOLS\":[") 再输出, 成功则置 io_data+0xd = 1。
//       第 2/3/5 参未用 (反编译仅示 4 参, 分发按 5 参调用)。
void lcd(void *io_data, int sock, char *params, int isjson, uint8_t group)
{
	void *cur;                     /* current_pool() */
	char str_lcd[32];              /* local_b8 区: "LCD" 值缓冲 */
	char buf_fan[32];              /* local_98: fan 计数串 */
	char buf_temp[100];            /* local_78..local_1c: temp 计数串 (含 "no" 复用区) */
	double ghs_avg;
	void *root;
	int printed;

	memset(str_lcd, 0, 32);
	memset(buf_fan, 0, 32);
	memset(buf_temp, 0, 100);
	cur = current_pool();
	message(io_data, 7, 0, 0, isjson);
	if (isjson == 0) {
		printed = 0;
	} else {
		printed = io_add(io_data, ",\"POOLS\":[");
	}
	str_lcd[0] = '0';              /* 机器码 CONCAT22 写 0x0030 → 字符串 "0" */
	ghs_avg = (DAT_0007acc0 / 1000.0) / DAT_000755f8;
	root = api_add_string(NULL, DAT_0006721c /*"LCD"*/, str_lcd, 0);
	root = api_add_mhs(root, "GHS5s", &DAT_00077158, 0);
	root = api_add_mhs(root, "GHSavg", &ghs_avg, 0);
	sprintf(buf_fan, "%d", DAT_0007b8a8);
	root = api_add_string(root, DAT_00067230 /*"fan"*/, buf_fan, 0);
	sprintf(buf_temp, "%d", DAT_0007bb20);
	root = api_add_string(root, DAT_00067234 /*"temp"*/, buf_temp, 0);
	if (cur == NULL) {
		/* 机器码把 "no" (0x6f6e) 写在 buf_temp 区内偏移 0x20/0x40 */
		*(uint32_t *)(buf_temp + 0x20) = 0x00006f6eu;
		*(uint32_t *)(buf_temp + 0x40) = 0x00006f6eu;
		root = api_add_string(root, "pool", buf_temp + 0x20, 0);
		root = api_add_string(root, DAT_0006723c /*"user"*/, buf_temp + 0x40, 0);
	} else {
		root = api_add_string(root, "pool",
		                      *(char **)((uint8_t *)cur + POOL_OFF_RPC_URL), 0);
		root = api_add_string(root, DAT_0006723c /*"user"*/,
		                      *(char **)((uint8_t *)cur + POOL_OFF_RPC_USER), 0);
	}
	print_data(io_data, root, isjson, isjson);
	if (printed != 0) {
		*(uint8_t *)((uint8_t *)io_data + 0xd) = 1;
	}
}

// === pools === 原 FUN_00035e88 @ 0x00035e88
// 来源: reversed/cgminer/21_pool_net/FUN_00035e88.c | 分类: 21_pool_net
// 语义: API "pools" 命令 — total_pools==0 → message(8)=MSG_NOPOOL; 否则
//       message(7) 后 (isjson 时 io_add(",\"POOLS\":[") 并在成功时置 io_data+0xd=1)
//       逐池输出, 跳过停机标志 (pool+0x89) 置位的池。每池一条 api_data 链:
//       POOL(序号)/URL/Status/Priority/Quota/Long Poll/Getworks/Accepted/
//       Rejected/Discarded/Stale/Get Failures/Remote Failures/User/Last Share
//       Time/Diff(pool+0x30 按字符串)/Diff1 Shares/Proxy Type+Proxy(无代理输出
//       空串)/Difficulty Accepted/Rejected/Stale/Last Share Difficulty/Has
//       Stratum/Stratum Active/Stratum URL/Has GBT/Best Share(copy=1)/
//       Pool Rejected% /Pool Stale%, 每池一个 print_data。Status 取值:
//       enabled(+0x84)==1 → Alive(但 idle@0x81 非 0 → Dead), ==0 → Disabled,
//       ==2 → Rejecting, 其余 → Unknown。第 2/3/5 参未用 (反编译仅示 4 参)。
void pools(void *io_data, int sock, char *params, int isjson, uint8_t group)
{
	char buf[260];                 /* local_128: "Last Share Time" 格式串 (0x100 字节清零) */
	void *pool;
	void *root;
	char *status;
	const char *lp_flag;           /* Long Poll: "Y"/"N" */
	time_t now;
	uint32_t secs;
	int rem;
	double pct_rej;
	double pct_sta;
	double total;
	int printed;
	int i;

	memset(buf, 0, 0x100);
	if (DAT_0007b198 == 0) {
		message(io_data, 8, 0, 0, isjson);         /* "No pools" */
	} else {
		message(io_data, 7, 0, 0, isjson);
		if (isjson == 0) {
			printed = 0;
		} else {
			printed = io_add(io_data, ",\"POOLS\":[");
		}
		if (0 < DAT_0007b198) {
			i = 0;
			do {
				pool = *(void **)((uint8_t *)DAT_0007b438 + i * 4);
				if (*(uint8_t *)((uint8_t *)pool + POOL_OFF_0X89) != '\0') {
					goto next_pool;                /* 停机标志: 跳过该池 */
				}
				switch (*(int *)((uint8_t *)pool + 0x84)) {
				case 1:
					status = "Alive";
					if (*(uint8_t *)((uint8_t *)pool + 0x81) != '\0') {
						status = "Dead";
					}
					break;
				case 0:
					status = "Disabled";
					break;
				default:
					status = "Rejecting";
					if (*(int *)((uint8_t *)pool + 0x84) != 2) {
						status = "Unknown";
					}
					break;
				}
				lp_flag = DAT_000670ac;            /* "Y" */
				if (*(int *)((uint8_t *)pool + 0x8c) == 0) {
					lp_flag = UNK_000670b0;        /* "N" */
				}
				if (*(int *)((uint8_t *)pool + 0x180) < 1) {
					buf[0] = '0';
					buf[1] = '\0';
				} else {
					now = time(NULL);
					secs = (uint32_t)(now - *(int *)((uint8_t *)pool + 0x180));
					secs = secs & ~((int32_t)secs >> 31);   /* 差值为负则清 0 (机器码) */
					rem = (int)secs % 0xe10;
					sprintf(buf, "%d:%02d:%02d", (int)secs / 0xe10, rem / 0x3c, rem % 0x3c);
				}
				root = api_add_int(NULL, DAT_00067488 /*"POOL"*/, &i, 0);
				root = api_add_escape(root, "URL",
				                      *(char **)((uint8_t *)pool + POOL_OFF_RPC_URL), 0);
				root = api_add_string(root, "Status", status, 0);
				root = api_add_int(root, "Priority", (int *)((uint8_t *)pool + POOL_OFF_PRIO), 0);
				root = api_add_int(root, "Quota", (int *)((uint8_t *)pool + POOL_OFF_QUOTA), 0);
				root = api_add_string(root, "Long Poll", (char *)lp_flag, 0);
				root = add_uint_buf(root, "Getworks", (uint32_t *)((uint8_t *)pool + 0x94), 0);
				root = add_int64_buf(root, "Accepted", (int64_t *)((uint8_t *)pool + 8), 0);
				root = add_int64_buf(root, "Rejected", (int64_t *)((uint8_t *)pool + 0x10), 0);
				root = add_uint_buf(root, "Discarded", (uint32_t *)((uint8_t *)pool + 0x9c), 0);
				root = add_uint_buf(root, "Stale", (uint32_t *)((uint8_t *)pool + 0x98), 0);
				root = add_uint_buf(root, "Get Failures", (uint32_t *)((uint8_t *)pool + 0xa0), 0);
				root = add_uint_buf(root, "Remote Failures", (uint32_t *)((uint8_t *)pool + POOL_OFF_0XA4), 0);
				root = api_add_escape(root, DAT_000674b8 /*"User"*/,
				                      *(char **)((uint8_t *)pool + POOL_OFF_RPC_USER), 0);
				root = api_add_string(root, "Last Share Time", buf, 0);
				root = api_add_string(root, DAT_000674d0 /*"Diff"*/,
				                      (char *)((uint8_t *)pool + 0x30), 0);
				root = add_int64_buf(root, "Diff1 Shares", (int64_t *)((uint8_t *)pool + 0x28), 0);
				if (*(int *)((uint8_t *)pool + POOL_OFF_0XD8) == 0) {
					root = api_add_const(root, "Proxy Type", "", 0);
					root = api_add_const(root, "Proxy", "", 0);
				} else {
					root = api_add_const(root, "Proxy Type",
					                     proxytype(*(int *)((uint8_t *)pool + 0xd4)), 0);
					root = api_add_escape(root, "Proxy",
					                      *(char **)((uint8_t *)pool + POOL_OFF_0XD8), 0);
				}
				root = add_diff_buf(root, "Difficulty Accepted",
				                    (double *)((uint8_t *)pool + 0x68), 0);
				root = add_diff_buf(root, "Difficulty Rejected",
				                    (double *)((uint8_t *)pool + 0x70), 0);
				root = add_diff_buf(root, "Difficulty Stale",
				                    (double *)((uint8_t *)pool + 0x78), 0);
				root = add_diff_buf(root, "Last Share Difficulty",
				                    (double *)((uint8_t *)pool + 0x188), 0);
				root = add_bool_buf(root, "Has Stratum", (uint8_t *)pool + 0x298, 0);
				root = add_bool_buf(root, "Stratum Active", (uint8_t *)pool + 0x299, 0);
				if (*(uint8_t *)((uint8_t *)pool + 0x299) == '\0') {
					root = api_add_const(root, "Stratum URL", "", 0);
				} else {
					root = api_add_escape(root, "Stratum URL",
					                      *(char **)((uint8_t *)pool + 0x258), 0);
				}
				root = add_bool_buf(root, DAT_0006753c /*"Has GBT"*/, (uint8_t *)pool + 0x2f8, 0);
				root = add_uint64_buf(root, "Best Share", (uint64_t *)((uint8_t *)pool + 0x190), 1);
				/* Pool Rejected% = rejected / (rejected + accepted + stale) (机器加序) */
				total = *(double *)((uint8_t *)pool + 0x70) +
				        *(double *)((uint8_t *)pool + 0x68) +
				        *(double *)((uint8_t *)pool + 0x78);
				if (total == 0.0) {
					pct_rej = 0.0;
				} else {
					pct_rej = *(double *)((uint8_t *)pool + 0x70) / total;
				}
				root = add_percent_buf(root, "Pool Rejected%", &pct_rej, 0);
				/* Pool Stale% = stale / (accepted + rejected + stale) (机器加序) */
				total = *(double *)((uint8_t *)pool + 0x68) +
				        *(double *)((uint8_t *)pool + 0x70) +
				        *(double *)((uint8_t *)pool + 0x78);
				if (total == 0.0) {
					pct_sta = 0.0;
				} else {
					pct_sta = *(double *)((uint8_t *)pool + 0x78) / total;
				}
				root = add_percent_buf(root, "Pool Stale%", &pct_sta, 0);
				print_data(io_data, root, isjson, 0);   /* 反编译仅示 3 参, 第 4 参按机器 r3=0 */
next_pool:
				i = i + 1;
			} while (i < DAT_0007b198);
		}
		if (printed != 0) {
			*(uint8_t *)((uint8_t *)io_data + 0xd) = 1;
		}
	}
}

// === http_listen_thread === 原 FUN_0005664c @ 0x0005664c
// 来源: reversed/cgminer/21_pool_net/FUN_0005664c.c | 分类: 21_pool_net
// 语义: 6060 端口 HTTP 服务监听线程体 (util.c 已按此签名引用): 初始 sleep(30s)
//       后循环 socket(AF_INET,SOCK_STREAM,TCP) → setsockopt(SO_REUSEADDR) →
//       bind(0.0.0.0:6060) → listen(100); socket/bind/listen 任一步失败都关闭
//       套接字、10s 后重试并按门控打 4/5 级日志; 监听就绪打 "start listen on
//       6060 ..." 后 10ms 轮询 accept, 接到客户端打 "one client connected
//       sock=%d" 并交 status_service_thread 处理; 停机标志 (DAT_0007acb8) 置位
//       时关闭监听套接字 (置 -1) 返回 0。
void *http_listen_thread(void *arg)
{
	char logbuf[0x1000];
	struct sockaddr sa;            /* bind 目标地址 (机器 sStack_1048) */
	struct sockaddr cli_sa;        /* accept 出参地址 (机器 sStack_1038) */
	socklen_t cli_len;
	int client;
	uint8_t stop_flag;
	int opt;
	int res;
	(void)arg;

	sleep(0x1e);                   /* 初始 30s 等待 */
	for (;;) {                     /* 机器 do-while(DAT_0007acb8 == 0) */
		DAT_00076b9c = socket(2, 1, 6);            /* AF_INET, SOCK_STREAM, IPPROTO_TCP */
		if (DAT_00076b9c < 0) {
			if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
				applog(5, "socket creating failed, try again after 10s...", 0);
			}
			sleep(10);
		} else {
			opt = 1;
			res = setsockopt(DAT_00076b9c, 1, 2, &opt, 4);     /* SOL_SOCKET, SO_REUSEADDR */
			if ((res < 0) &&
			    (((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0') || (3 < DAT_000766c4))))) {
				applog(4, "***************setsockopt(SO_REUSEADDR) failed", 0);
			}
			sa.sa_family = 2;                      /* AF_INET */
			sa.sa_data[0] = 0x17;                  /* 端口 0x17ac = 6060 (大端) */
			sa.sa_data[1] = (char)0xac;
			sa.sa_data[2] = 0;
			sa.sa_data[3] = 0;
			sa.sa_data[4] = 0;
			sa.sa_data[5] = 0;
			sa.sa_data[6] = 0;
			sa.sa_data[7] = 0;
			sa.sa_data[8] = 0;
			sa.sa_data[9] = 0;
			sa.sa_data[10] = 0;
			sa.sa_data[11] = 0;
			sa.sa_data[12] = 0;
			sa.sa_data[13] = 0;
			res = bind(DAT_00076b9c, &sa, 0x10);
			if (res < 0) {
				if (((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0') || (4 < DAT_000766c4)))) {
					applog(5, "http port bind failed! try again after 10s...", 0);
				}
			} else {
				res = listen(DAT_00076b9c, 100);
				if (res >= 0) {
					break;                         /* 监听就绪 (机器 break) */
				}
				if (((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0') || (4 < DAT_000766c4)))) {
					applog(5, "http listen failed! try again after 10s...", 0);
				}
			}
			close(DAT_00076b9c);
			DAT_00076b9c = -1;
			sleep(10);
		}
		if (DAT_0007acb8 != '\0') {
			break;                                 /* 机器 while 条件不满足 → 退出建听循环 */
		}
	}
	if (((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0') || (4 < DAT_000766c4)))) {
		applog(5, "start listen on 6060 ...", 0);
	}
	for (;;) {
		if (DAT_0007acb8 != '\0') {
			goto listen_shutdown;              /* LAB_00056860 */
		}
		stop_flag = 0;
		do {
			if (stop_flag != '\0') {
				goto listen_shutdown;          /* LAB_00056860 */
			}
			usleep(10000);
			cli_len = 0x10;
			client = accept(DAT_00076b9c, &cli_sa, &cli_len);
			stop_flag = DAT_0007acb8;
		} while (client == -1);
		if (DAT_0007acb8 != '\0') {
			close(client);
			goto listen_shutdown;              /* LAB_00056860 */
		}
		if ((DAT_00078e09 != '\0') &&
		    (((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0') || (6 < DAT_000766c4))))) {
			snprintf(logbuf, 0x1000, "one client connected sock=%d", client);
			applog(7, logbuf, 0);
		}
		status_service_thread(client);
	}

listen_shutdown:               /* LAB_00056860 */
	close(DAT_00076b9c);
	DAT_00076b9c = -1;
	return NULL;
}

/* ==================== 覆盖率表 (batch=pool_b2, 11/11) ====================
 * FUN_00017a3c @ 0x00017a3c → add_pool
 * FUN_000182f4 @ 0x000182f4 → clear_stratum_shares
 * FUN_00018ca0 @ 0x00018ca0 → pool_active
 * FUN_00019bf8 @ 0x00019bf8 → switch_pools
 * FUN_00021850 @ 0x00021850 → watchpool_thread
 * FUN_00021fe8 @ 0x00021fe8 → stratum_sthread
 * FUN_00028fc8 @ 0x00028fc8 → socks4_negotiate
 * FUN_0002b040 @ 0x0002b040 → recv_line
 * FUN_00034ed4 @ 0x00034ed4 → lcd
 * FUN_00035e88 @ 0x00035e88 → pools
 * FUN_0005664c @ 0x0005664c → http_listen_thread
 * ========================================================================= */





