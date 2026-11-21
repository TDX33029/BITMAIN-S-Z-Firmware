/*
 * cgminer 通用工具层 (批次 util_b1) — BM1746 固件语义转写
 * =========================================================
 * 原始二进制: bm1746/cgminer_z15_bm1746.elf (ARM32, SD 镜像 cgminer-z15, branch "bm1746")
 * 来源分类:   reversed/cgminer/60_util_util/ (35 个函数, 清单见 rewritten/batch_plan.tsv batch=util_b1)
 *
 * 模块功能概述 (对应上游 cgminer util.c / cgminer.c / klist.c 的工具函数族):
 *   1) 日志/退出辅助: 互斥锁/读写锁初始化与加解锁失败的统一报错 + quit 通道
 *      (applog 第 3 参非 0 → 强制 curses 刷新, 失败一律走 quit(1) 终止进程);
 *   2) 线程生命周期: set_lowprio / miner_thread / mt_disable / restart_thread /
 *      restart_wait / completion_thread / _cgsem_wait (sem_wait EINTR 重试封装) /
 *      create_bitmain_get_hash_rate_pthread / start_http_listen_thread;
 *   3) 工作队列: get_thread (mining_thr 数组读) / add_queued + workid_hash_add
 *      (按 work+0x724 处 4 字节键的哈希表插入, 自带倍增扩容与过载冻结) /
 *      fill_queue (按设备驱动队列深度补工作, hashmeter 节流, 过期禁用) / tq_freezethaw;
 *   4) curses 池输入: _wlogprint / curses_input / input_pool / add_pool_details
 *      (新矿池 url/user/pass 拼接 userpass + 测试线程, live 模式 join 后返回 pools_active);
 *   5) klist.c 链表/仓库: _k_new_list / k_new_store / k_alloc_items (批量 calloc + 双向链
 *      + data 块) / _k_add_head / _k_add_tail / _k_unlink_tail (do_tail 守卫);
 *   6) 字符串工具: realloc_strcat (4 字节对齐 malloc) / str_text (不可打印字节 → "0xXX")
 *      / ser_string (变长长度前缀: 0xfd 2 字节 BE / 0xfe 4 字节 BE, stratum 协议序列化)。
 *
 * 结构体布局说明: 本文件涉及的 struct thr_info / cgpu_info / pool / work / K_LIST / K_ITEM
 *   均无完整头文件证据, 一律按机器码锁定的字节偏移访问 (每处注明偏移含义), 不虚构完整布局。
 *
 * 转写约定: rewritten/CONVENTIONS.md; 命名事实源 rewritten/name_map/cgminer.tsv
 *   - 日志门控 (反编译归纳, 与 log.c/uart.c 一致):
 *       prio 7 (LOG_DEBUG): DAT_00078e09 (opt_debug) && (DAT_0007ad48 || DAT_00078e08 || DAT_000766c4 > 6)
 *       prio 5:             (DAT_0007ad48 || DAT_00078e08 || DAT_000766c4 > 4)
 *       prio 4/3:           (DAT_0007ad48 || DAT_00078e08 || DAT_000766c4 > 3 / > 2)
 *   - 每次成功解锁后调用的全局函数指针 DAT_00075ed8 (ELF 初值 noop_hook, 恒 0) 按机器码保留。
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <sys/time.h>
#include <curses.h>

/* ==================== 外部全局 (DAT_ 原名保留, 语义见注释) ==================== */

extern uint8_t DAT_00078e09;          /* opt_debug (日志门控外层总开关, curses 'd') */
extern uint8_t DAT_0007ad48;          /* applog 门控 A: 非 0 → applog 走 syslog(prio|0x80) */
extern uint8_t DAT_00078e08;          /* applog 门控 B = opt_verbose (curses 'v' 联动) */
extern int     DAT_000766c4;          /* applog 门控 C: int 日志级别 (LOG_ERR=3 阈值) */
extern int     DAT_00075dcc;          /* opt_log_interval (与 log.c 同一全局, 菜单 "[L]og interval") */
extern void  (*DAT_00075ed8)(void);   /* 全局函数指针: ELF 初值 noop_hook (FUN_00028ae0, 恒 0), 运行期未见改写 [低置信] */

extern WINDOW *DAT_0007b430;          /* logwin: curses 日志窗口 (wprintw/wgetnstr/immedok) */
extern pthread_mutex_t DAT_0007b7a8;  /* curses 互斥锁 (curses_active_locked 持有) */

extern void **DAT_0007b5e0;           /* mining_thr: thr_info* 数组基址 (restart_thread 遍历) */
extern pthread_rwlock_t DAT_0007b650; /* mining_thr 数组锁 (get_thread 读 / 后台线程写) */
extern int     DAT_0007bb6c;          /* mining_threads: thr_info 数组元素数 */
extern pthread_mutex_t DAT_0007b62c;  /* restart_lock (restart_wait / restart_thread, 上游同名) */
extern pthread_cond_t  DAT_0007bae0;  /* restart_cond (上游同名) */
extern int     DAT_0007b5e4;          /* total_pools (add_pool_details 首次启用矿池时自增) */
extern uint8_t DAT_0007730c;          /* pools_active 类全局标志: main "No servers were found..." 等待循环检查 [低置信] */
extern void   *DAT_0007c05c;          /* create_bitmain_get_hash_rate_pthread 分配的 thr_info (0x40 字节) */

/* ==================== 外部函数 (name_map 语义名) ==================== */

extern void  applog(uint32_t prio, char *buf, ...);   /* FUN_00038730 */
extern void  __quit(int status, int wait_for_threads);               /* FUN_0001d8fc: 退出总控 */
extern void  wr_lock_error(const char *fn, int line);                /* FUN_0000b480 "WTF WRLOCK ERROR ON LOCK!" */
extern void  rw_unlock_error(const char *fn, int line);              /* FUN_0000b630 "WTF RWLOCK ERROR ON UNLOCK!" */
extern void  rd_lock_error(const char *fn, int line);                /* FUN_0000b69c "WTF RDLOCK ERROR ON LOCK!" */
extern void  set_thr_idle(void *thr);                                /* FUN_0000fc58: thr+0x3d 置 1 + 记时 */
extern void  rename_thread(const char *name);                        /* FUN_0002f0b4: snprintf "cg@%s" + prctl */
extern void  record_device_fail(void *cgpu, int reason);             /* FUN_0002ed88: cgpu+0xf4 时间 / +0xf8 reason / +0xfc.. 计数 */
extern void  cgtime(struct timeval *tv);                             /* FUN_0002a84c: gettimeofday(tv, NULL) */
extern void  copy_timeval(struct timeval *dst, const struct timeval *src); /* FUN_0002a530: 8 字节 dst<-src */
extern unsigned int cgsleep_ms(unsigned int ms);                     /* FUN_0002a92c (r0 直通 FUN_0002a8c4 返回值) */
extern int   thr_info_create(void *thr, const pthread_attr_t *attr, void *(*fn)(void *), void *arg); /* FUN_0002f174: _cgsem_init(thr+0x10) + 尾调 pthread_create(thr+0xc), 返回其结果 */
extern void  _cgsem_post(sem_t *sem, const char *file, const char *func, int line); /* FUN_0002f1bc */
extern void  flush_queue(void *cgpu);                                /* FUN_0001aa18 "flush_queue" */
extern void  discard_work(void *work);                               /* FUN_000159b4 */
extern void *get_work(void *thr, int thr_id);                        /* FUN_000247d4 */
extern void  hashmeter(int thr_id, int secs, unsigned int hashes_lo, unsigned int hashes_hi); /* FUN_0001bba4; 第 3/4 参 = 64 位哈希数低/高字 */
extern void  discard_stale(void);                                    /* FUN_00022f88 */
extern void *current_pool(void);                                     /* FUN_0001da58 */
extern uint8_t pool_tset(void *pool, uint8_t *flag_addr);            /* FUN_00018834: pool_lock 下 *addr=1, 返回旧值 */
extern int   curses_active_locked(void);                             /* FUN_00017fe4: 加 curses 锁并返回 curses 是否激活 */
extern void *add_pool(void);                                         /* FUN_00017a3c */
extern int   detect_stratum(void *pool, char *url);                  /* FUN_0001557c: 识别 stratum+tcp:// URL, 非 stratum 返回 0 */
extern void *get_proxy(char *url, void *pool);                       /* FUN_0002ace0 */
extern void *hashmeter_thread(void *arg);                            /* FUN_0003abf4: 从 ASIC 取算力线程体 */
extern void *http_listen_thread(void *arg);                          /* FUN_0005664c: 搜频 HTTP 服务线程体 */
extern void *fun_0001fe0c(void *arg);                                /* FUN_0001fe0c — 命名待补, 上游 test_pool_thread (池测试线程主循环) */
extern int   nice(int inc);                                          /* POSIX nice (目标平台 unistd.h) */

/* ==================== 本文件内前向声明 (定义在后, 机器码调用序不变) ==================== */

void   quit(int status);                                             /* FUN_00016ca8 */
void   _cgsem_wait(sem_t *sem, const char *file, const char *func, int line);      /* FUN_0002f280 */
void   workid_hash_add(void *thr, void *work);                       /* FUN_00016624 */
void   mt_disable(void *thr, int thr_id, void *drv);                 /* FUN_00011c8c */
void   mutex_unlock_util(pthread_mutex_t *m, const char *fn, int line); /* FUN_00029388 */
uint8_t add_pool_details(void *pool, int live, char *url, char *user, char *pass); /* FUN_00026bcc */
char  *curses_input(const char *prompt);                             /* FUN_00025fdc */
void   _wlogprint(const char *str);                                  /* FUN_00018bc0 */

/* ============================================================================
 * 结构体 (布局由机器码锁定, 仅给出有调用点证据的字段)
 * ============================================================================ */

/* ---- KL_LOCK (klist.c, 0x38 字节): 互斥锁+读写锁组合块 ----
 * rwlock 固定位于 +0x18 (机器码锁定偏移), 不依赖本机 sizeof(pthread_mutex_t)。 */
typedef struct kl_lock {
	pthread_mutex_t mutex;   /* +0x00 */
} kl_lock_t;

/* ---- K_ITEM / K_LIST (klist.c, 32 位布局) ----
 * K_ITEM 16 字节: calloc(n, 0x10); 链表方向: prev 朝 head, next 朝 tail。
 * K_LIST 0x40 字节 (16 词); store 是与主列表共享锁的"空闲项仓库", flag 区分。 */
typedef struct k_item K_ITEM;
struct k_item {
	char    *name;   /* +0x00: 所属 klist 名 (分配时写入, _k_add_* 以指针比对校验) */
	K_ITEM  *prev;   /* +0x04: 前驱 (朝 head) */
	K_ITEM  *next;   /* +0x08: 后继 (朝 tail) */
	void    *data;   /* +0x0c: 用户数据块 calloc(1, klist->siz) */
};

typedef struct k_list K_LIST;
struct k_list {
	char            *name;           /* +0x00: 列表名 (错误消息用) */
	uint8_t          is_store;       /* +0x04: 1=store (仓库不能 k_alloc_items / _k_add_tail) */
	struct kl_lock  *lock;           /* +0x08: KL_LOCK = {mutex@+0, rwlock@+0x18}, store 与主列表共享 */
	K_ITEM          *head;           /* +0x0c */
	K_ITEM          *tail;           /* +0x10 (仅 do_tail 时维护) */
	unsigned int     siz;            /* +0x14: data 块大小 */
	int              total;          /* +0x18: 已分配项总数 (仅 k_alloc_items 递增) */
	int              count;          /* +0x1c: 当前在列表中的项数 (add 增 / unlink_tail 减) */
	int              count_up;       /* +0x20: 累计加入计数 (add 增, 本批函数不减) */
	int              allocate;       /* +0x24: 每批分配项数 */
	int              limit;          /* +0x28: 项数上限 (0=不限) */
	uint8_t          do_tail;        /* +0x2c: 是否维护 tail */
	int              item_mem_count; /* +0x30: item_memory 数组元素数 */
	K_ITEM         **item_memory;    /* +0x34: 各批 K_ITEM 数组指针表 (供统一释放) */
	int              data_mem_count; /* +0x38: data_memory 数组元素数 */
	void           **data_memory;    /* +0x3c: 各 data 块指针表 */
};

/* ---- workid 哈希表节点 (嵌入 struct work 偏移 0x728..0x747) ----
 * 键 = work+0x724 处 4 字节 (workid), 哈希值存 work+0x744。 */
typedef struct workid_ht_node {
	void           *table;      /* +0x728 (node+0x00): 回指表头 */
	void           *list_prev;  /* +0x72c (node+0x04): 全局 FIFO 前驱 (work 容器指针) */
	void           *list_next;  /* +0x730 (node+0x08): 全局 FIFO 后继 (work 容器指针) */
	struct workid_ht_node *bkt_prev; /* +0x734 (node+0x0c): 桶链前驱 (节点地址) */
	struct workid_ht_node *bkt_next; /* +0x738 (node+0x10): 桶链后继 (节点地址) */
	void           *key;        /* +0x73c (node+0x14): 键指针 (= work+0x724) */
	uint32_t        key_len;    /* +0x740 (node+0x18): 键长 (恒 4) */
	uint32_t        hash;       /* +0x744 (node+0x1c): 哈希值 */
} workid_ht_node_t;

/* ---- workid 哈希表头 (workid_hash_add 内 malloc(0x2c)) ----
 * 桶数组每桶 12 字节 {head_node, count, watermark}; 表头指针存于 thr_info+0x160,
 * 首个插入的 work 充当表载体 (其 node+0x00 存表指针)。 */
typedef struct workid_ht_bucket {
	workid_ht_node_t *head;   /* +0x00: 桶链头节点 (节点地址) */
	uint32_t count;           /* +0x04: 桶内节点数 */
	uint32_t watermark;       /* +0x08: 扩容水位 (上次 resize 时的 count/load_cap) */
} workid_ht_bucket_t;

typedef struct workid_ht {
	workid_ht_bucket_t *buckets;  /* +0x00: 桶数组 (初始 0x20 桶 × 12 字节 = malloc(0x180)) */
	uint32_t     nbuckets;        /* +0x04: 桶数 (2 的幂) */
	uint32_t     shift;           /* +0x08: log2(nbuckets) (初始 5) */
	uint32_t     count;           /* +0x0c: 节点总数 */
	workid_ht_node_t *tail_node;  /* +0x10: 最后插入节点的地址 (全局 FIFO 尾; container_of 用 node_off) */
	uint32_t     node_off;        /* +0x14: 节点在容器内偏移 (0x728) */
	uint32_t     load_cap;        /* +0x18: resize 时写入的平均负载估计 (watermark 除数) */
	uint32_t     over_cnt;        /* +0x1c: 本次 resize 中超过 load_cap 的桶数 */
	uint32_t     grow_cnt;        /* +0x20: 连续"失衡扩容"计数 (>1 时冻结扩容) */
	uint32_t     frozen;          /* +0x24: 1 = 停止扩容 */
	uint32_t     seed;            /* +0x28: 哈希种子 (0xa0111fe1; 仅初始化写入, 本函数不再读取) */
} workid_ht_t;

/* ============================================================================
 * 函数转写
 * ============================================================================ */

// === restart_threads_err === 原 FUN_0000b3c8 @ 0x0000b3c8
// 来源: reversed/cgminer/60_util_util/FUN_0000b3c8.c | 分类: 60_util_util
// 语义: 报 "Failed to create restart thread" (LOG_ERR) 并 quit(1) 终止进程。
//       原机器码把字面量拆成栈上逐字赋值, 此处还原为等价字面量。
void restart_threads_err(void)
{
	char msg[4096];

	snprintf(msg, 0x1000, "Failed to create restart thread");
	applog(3, msg, 1);
	quit(1);
}

// === mutex_lock_fail_cgminer === 原 FUN_0000b414 @ 0x0000b414
// 来源: reversed/cgminer/60_util_util/FUN_0000b414.c | 分类: 60_util_util
// 语义: pthread 加锁失败统一报错 (文件名固定 "cgminer.c"), 报完 quit(1)。
void mutex_lock_fail_cgminer(const char *fn, int line)
{
	char msg[4096];

	snprintf(msg, 0x1000, "WTF MUTEX ERROR ON LOCK! errno=%d in %s %s():%d",
	         errno, "cgminer.c", fn, line);
	applog(3, msg, 1);
	quit(1);
}

// === mutex_init_fail_cgminer === 原 FUN_0000b4ec @ 0x0000b4ec
// 来源: reversed/cgminer/60_util_util/FUN_0000b4ec.c | 分类: 60_util_util
// 语义: pthread_mutex_init 失败统一报错 (文件名固定 "cgminer.c"), 报完 quit(1)。
void mutex_init_fail_cgminer(const char *fn, int line)
{
	char msg[4096];

	snprintf(msg, 0x1000, "Failed to pthread_mutex_init errno=%d in %s %s():%d",
	         errno, "cgminer.c", fn, line);
	applog(3, msg, 1);
	quit(1);
}

// === rwlock_init_fail_cgminer === 原 FUN_0000b558 @ 0x0000b558
// 来源: reversed/cgminer/60_util_util/FUN_0000b558.c | 分类: 60_util_util
// 语义: pthread_rwlock_init 失败统一报错 (文件名固定 "cgminer.c"), 报完 quit(1)。
void rwlock_init_fail_cgminer(const char *fn, int line)
{
	char msg[4096];

	snprintf(msg, 0x1000, "Failed to pthread_rwlock_init errno=%d in %s %s():%d",
	         errno, "cgminer.c", fn, line);
	applog(3, msg, 1);
	quit(1);
}

// === mutex_unlock_fail_cgminer === 原 FUN_0000b5c4 @ 0x0000b5c4
// 来源: reversed/cgminer/60_util_util/FUN_0000b5c4.c | 分类: 60_util_util
// 语义: pthread 解锁失败统一报错 (文件名固定 "cgminer.c"), 报完 quit(1)。
void mutex_unlock_fail_cgminer(const char *fn, int line)
{
	char msg[4096];

	snprintf(msg, 0x1000, "WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d",
	         errno, "cgminer.c", fn, line);
	applog(3, msg, 1);
	quit(1);
}

// === mutex_lock_fail_util === 原 FUN_0000b708 @ 0x0000b708
// 来源: reversed/cgminer/60_util_util/FUN_0000b708.c | 分类: 60_util_util
// 语义: 同 mutex_lock_fail_cgminer, 但文件名固定 "util.c" (tq_push/tq_pop 等使用)。
void mutex_lock_fail_util(const char *fn, int line)
{
	char msg[4096];

	snprintf(msg, 0x1000, "WTF MUTEX ERROR ON LOCK! errno=%d in %s %s():%d",
	         errno, "util.c", fn, line);
	applog(3, msg, 1);
	quit(1);
}

// === quit === 原 FUN_00016ca8 @ 0x00016ca8
// 来源: reversed/cgminer/60_util_util/FUN_00016ca8.c | 分类: 60_util_util
// 语义: 退出入口单参包装: __quit(status, 1) — 1 = 等待各线程收尾。
void quit(int status)
{
	__quit(status, 1);
}

// === set_lowprio === 原 FUN_000120f4 @ 0x000120f4
// 来源: reversed/cgminer/60_util_util/FUN_000120f4.c | 分类: 60_util_util
// 语义: nice(10) 降低本线程优先级; 之后按 opt_debug 门控 applog(LOG_DEBUG,
//       "Unable to set thread to low priority")。注意: 机器码为 cmp r0,#0 / popne —
//       即 nice(10)==0 (成功) 时才走到该日志 (疑为固件对上游失败分支的转写笔误), 按机器码保留。
void set_lowprio(void)
{
	char msg[4096];

	if (nice(10) != 0) {
		return;
	}
	if (DAT_00078e09 == '\0') {
		return;
	}
	if (DAT_0007ad48 == '\0' && DAT_00078e08 == '\0' && DAT_000766c4 < 7) {
		return;
	}
	strncpy(msg, "Unable to set thread to low priority", 0x25);
	applog(7, msg, 0);
}

// === mutex_unlock_util === 原 FUN_00029388 @ 0x00029388
// 来源: reversed/cgminer/60_util_util/FUN_00029388.c | 分类: 60_util_util
// 语义: pthread_mutex_unlock 包装; 失败时报 "WTF MUTEX ERROR ON UNLOCK!"
//       (文件名固定 "util.c") 并 quit(1); 成功无副作用 (调用方随后自调 noop 钩子)。
void mutex_unlock_util(pthread_mutex_t *m, const char *fn, int line)
{
	if (pthread_mutex_unlock(m) != 0) {
		char msg[4096];

		snprintf(msg, 0x1000, "WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d",
		         errno, "util.c", fn, line);
		applog(3, msg, 1);
		quit(1);
	}
}


// === mt_disable === 原 FUN_00011c8c @ 0x00011c8c
// 来源: reversed/cgminer/60_util_util/FUN_00011c8c.c | 分类: 60_util_util
// 语义: 线程禁用流程 (由 fill_queue 在设备过载/失效时调用): 报 "Thread %d being disabled"
//       (LOG_WARNING), 清 cgpu+0x30/+0x34 两个统计字, 按 opt_debug 门控报
//       "Waiting on sem in miner thread" 后在 thr+0x10 的信号量上休眠, 被重新启用后
//       报 "Thread %d being re-enabled" 并回调 drv 槽位 +0x5c(thr)。
//       thr_info 偏移: +0x10=sem_t, +0x24=cgpu_info*; drv 槽位 +0x5c。
void mt_disable(void *thr, int thr_id, void *drv)
{
	char msg[4096];
	void *cgpu;

	if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 3 < DAT_000766c4) {
		snprintf(msg, 0x1000, "Thread %d being disabled", thr_id);
		applog(4, msg, 0);
	}
	cgpu = *(void **)((uint8_t *)thr + 0x24);
	*(uint32_t *)((uint8_t *)cgpu + 0x30) = 0;
	*(uint32_t *)((uint8_t *)cgpu + 0x34) = 0;
	if (DAT_00078e09 != '\0' &&
	    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 6 < DAT_000766c4)) {
		snprintf(msg, 0x1000, "Waiting on sem in miner thread");
		applog(7, msg, 0);
	}
	_cgsem_wait((sem_t *)((uint8_t *)thr + 0x10), "cgminer.c", "mt_disable", 0x2171);
	if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 3 < DAT_000766c4) {
		snprintf(msg, 0x1000, "Thread %d being re-enabled", thr_id);
		applog(4, msg, 0);
	}
	(*(void (**)(void *))((uint8_t *)drv + 0x5c))(thr);   /* drv 槽位 +0x5c (机器码, 具名待定) */
}

// === miner_thread === 原 FUN_00011e24 @ 0x00011e24
// 来源: reversed/cgminer/60_util_util/FUN_00011e24.c | 分类: 60_util_util
// 语义: 每线程主入口: 改名 "%d/Miner", set_thr_idle, 调 drv 槽位 +0x34 (thread_init)
//       失败则 record_device_fail(cgpu, 0) (机器码调用点 r1=0) 并退出; 成功则等待
//       thr+0x10 信号量, 记 cgpu+0xe8 启动时间, 依次回调 drv 槽位 +0x3c (主挖矿循环,
//       即上游 hash_work) 与 +0x58。
//       thr_info 偏移: +0x00=thr_id, +0x10=sem_t, +0x24=cgpu_info*; cgpu+4=drv。
void *miner_thread(void *userdata)
{
	uint8_t *thr = userdata;
	char threadname[16];
	char msg[4096];
	void *cgpu = *(void **)(thr + 0x24);
	void *drv = *(void **)((uint8_t *)cgpu + 4);
	int thread_init_ret;
	time_t now;

	snprintf(threadname, 0x10, "%d/Miner", *(int *)thr);
	rename_thread(threadname);
	set_thr_idle(thr);
	thread_init_ret = (*(int (**)(void *))((uint8_t *)drv + 0x34))(thr);   /* drv 槽位 +0x34 (thread_init) */
	if (thread_init_ret == 0) {
		/* 机器码: subs r1, r0, #0 之后未再装载 reason, 调用点 r1 恒为 0 → 按 0 转写 */
		record_device_fail(cgpu, 0);
		return NULL;
	}
	if (DAT_00078e09 != '\0' &&
	    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 6 < DAT_000766c4)) {
		snprintf(msg, 0x1000, "Waiting on sem in miner thread");
		applog(7, msg, 0);
	}
	_cgsem_wait((sem_t *)(thr + 0x10), "cgminer.c", "miner_thread", 0x23bb);
	now = time(NULL);
	*(time_t *)((uint8_t *)cgpu + 0xe8) = now;
	(*(void (**)(void *))((uint8_t *)drv + 0x3c))(thr);   /* drv 槽位 +0x3c (主挖矿循环 hash_work) */
	(*(void (**)(void *))((uint8_t *)drv + 0x58))(thr);   /* drv 槽位 +0x58 (机器码, 具名待定) */
	return NULL;
}

// === get_thread === 原 FUN_0001b124 @ 0x0001b124
// 来源: reversed/cgminer/60_util_util/FUN_0001b124.c | 分类: 60_util_util
// 语义: 在 mining_thr 读锁保护下返回 mining_thr[id] (thr_info*); 解锁后调 noop 钩子。
void *get_thread(int id)
{
	void *thr;

	if (pthread_rwlock_rdlock(&DAT_0007b650) != 0) {
		rd_lock_error("get_thread", 0x266);
	}
	thr = DAT_0007b5e0[id];
	if (pthread_rwlock_unlock(&DAT_0007b650) != 0) {
		rw_unlock_error("get_thread", 0x268);
	}
	(*DAT_00075ed8)();
	return thr;
}

// === add_queued === 原 FUN_00019f54 @ 0x00019f54
// 来源: reversed/cgminer/60_util_util/FUN_00019f54.c | 分类: 60_util_util
// 语义: 在 thr+0x140 读写锁写保护下调用 workid_hash_add(thr, work) 登记新工作。
void add_queued(void *thr, void *work)
{
	if (pthread_rwlock_wrlock((pthread_rwlock_t *)((uint8_t *)thr + 0x140)) != 0) {
		wr_lock_error("add_queued", 0x2279);
	}
	workid_hash_add(thr, work);
	if (pthread_rwlock_unlock((pthread_rwlock_t *)((uint8_t *)thr + 0x140)) != 0) {
		rw_unlock_error("add_queued", 0x227b);
	}
	(*DAT_00075ed8)();
}

// === restart_wait === 原 FUN_000181f4 @ 0x000181f4
// 来源: reversed/cgminer/60_util_util/FUN_000181f4.c | 分类: 60_util_util
// 语义: 在 restart_lock/restart_cond 上限时等待 msecs 毫秒, 直到本线程的重启标志
//       (thr+0x3e) 被 restart_thread 置位并广播; 返回 pthread_cond_timedwait 结果
//       (标志已置位直接返回 0)。等待终点计算: sec += msecs/1000,
//       usec += msecs*1000-(msecs/1000)*1000000 (32 位回绕), 进位修正后 ×1000 → nsec。
int restart_wait(void *thr, unsigned int msecs)
{
	struct timeval now;
	struct timespec wait_ts;
	int ret;
	int usec;

	cgtime(&now);
	wait_ts.tv_sec = now.tv_sec + msecs / 1000u;
	/* 与机器码一致: usec = now.usec + msecs*1000 - (msecs/1000)*1000000 (32 位回绕语义) */
	usec = (int)((unsigned int)now.tv_usec + msecs * 1000u - (msecs / 1000u) * 1000000u);
	if (999999 < usec) {
		wait_ts.tv_sec = wait_ts.tv_sec + 1;
		usec = usec - 1000000;
	}
	wait_ts.tv_nsec = usec * 1000;
	if (pthread_mutex_lock(&DAT_0007b62c) != 0) {
		mutex_lock_fail_cgminer("restart_wait", 0x1481);
	}
	if (*(uint8_t *)((uint8_t *)thr + 0x3e) == '\0') {
		ret = pthread_cond_timedwait(&DAT_0007bae0, &DAT_0007b62c, &wait_ts);
	} else {
		ret = 0;
	}
	if (pthread_mutex_unlock(&DAT_0007b62c) != 0) {
		mutex_unlock_fail_cgminer("restart_wait", 0x1486);
	}
	(*DAT_00075ed8)();
	return ret;
}

// === restart_thread === 原 FUN_000231f0 @ 0x000231f0
// 来源: reversed/cgminer/60_util_util/FUN_000231f0.c | 分类: 60_util_util
// 语义: 重启线程体: detach 自身, pool_tset 置位当前矿池 +0x82 标志, discard_stale,
//       然后遍历 mining_thr[0..n): 跳过 cgpu 为空或 cgpu+0x20 (线程收尾标志) 非零者,
//       其余置 thr+0x3e 重启标志、flush_queue(cgpu)、回调 drv 槽位 +0x4c(cgpu);
//       最后 broadcast restart_cond 唤醒所有 restart_wait, 恒返回 0。
//       pool 偏移: +0x82 字节标志, +0xdc=pool_lock (pool_tset 内部)。
int restart_thread(void)
{
	void *cp;
	pthread_t self;
	int i;
	int n;
	void *thr;
	void *cgpu;

	cp = current_pool();
	self = pthread_self();
	pthread_detach(self);
	pool_tset(cp, (uint8_t *)cp + 0x82);
	discard_stale();
	if (pthread_rwlock_rdlock(&DAT_0007b650) != 0) {
		rd_lock_error("restart_thread", 0x1496);
	}
	n = DAT_0007bb6c;
	if (pthread_rwlock_unlock(&DAT_0007b650) != 0) {
		rw_unlock_error("restart_thread", 0x1498);
	}
	(*DAT_00075ed8)();
	if (0 < n) {
		i = 0;
		do {
			while (1) {
				thr = DAT_0007b5e0[i];
				i = i + 1;
				cgpu = *(void **)((uint8_t *)thr + 0x24);
				if (cgpu == NULL || *(uint32_t *)((uint8_t *)cgpu + 0x20) != 0) {
					break;   /* 跳过该线程, 外层条件继续下一个 */
				}
				*(uint8_t *)((uint8_t *)thr + 0x3e) = 1;   /* 置线程重启标志 (唤醒 mt_disable 等) */
				flush_queue(cgpu);
				(*(void (**)(void *))((uint8_t *)*(void **)((uint8_t *)cgpu + 4) + 0x4c))(cgpu); /* drv->+0x4c(cgpu) */
				if (i == n) {
					goto broadcast;   /* 原 LAB_000232b0 */
				}
			}
		} while (i != n);
	}
broadcast:   /* 原 LAB_000232b0 */
	if (pthread_mutex_lock(&DAT_0007b62c) != 0) {
		mutex_lock_fail_cgminer("restart_thread", 0x14a4);
	}
	pthread_cond_broadcast(&DAT_0007bae0);
	if (pthread_mutex_unlock(&DAT_0007b62c) != 0) {
		mutex_unlock_fail_cgminer("restart_thread", 0x14a6);
	}
	(*DAT_00075ed8)();
	return 0;
}

// === completion_thread === 原 FUN_0002f23c @ 0x0002f23c
// 来源: reversed/cgminer/60_util_util/FUN_0002f23c.c | 分类: 60_util_util
// 语义: cg_completion_timeout (FUN_0002f438) 创建的一次性线程体: 异步取消模式,
//       调用参数块 +0x10 的函数指针 (实参 = 块 +0x14), 结束后在块 +0x00 的信号量上
//       _cgsem_post 通知超时等待者。参数块布局: +0x00=sem_t, +0x10=fn, +0x14=arg。
void *completion_thread(void *arg)
{
	uint8_t *tq = arg;
	void (*fn)(void *);

	pthread_setcanceltype(1, NULL);   /* 1 = PTHREAD_CANCEL_ASYNCHRONOUS */
	fn = *(void (**)(void *))(tq + 0x10);
	fn(*(void **)(tq + 0x14));
	_cgsem_post((sem_t *)tq, "util.c", "completion_thread", 0xf7b);
	return NULL;
}

// === _cgsem_wait === 原 FUN_0002f280 @ 0x0002f280
// 来源: reversed/cgminer/60_util_util/FUN_0002f280.c | 分类: 60_util_util
// 语义: sem_wait 封装: 成功即返回; EINTR 重试; 其他错误报
//       "Failed to sem_wait errno=%d cgsem=0x%p in %s %s():%d" 并 quit(1)。
void _cgsem_wait(sem_t *sem, const char *file, const char *func, int line)
{
	int ret;
	int err;
	char msg[4096];

	do {
		ret = sem_wait(sem);
		if (ret == 0) {
			return;
		}
		err = errno;
	} while (err == EINTR);   /* 4=EINTR, 机器码字面量比较 */

	snprintf(msg, 0x1000, "Failed to sem_wait errno=%d cgsem=0x%p in %s %s():%d",
	         err, sem, file, func, line);
	applog(3, msg, 1);
	quit(1);
}

// === fill_queue === 原 FUN_00024aa8 @ 0x00024aa8
// 来源: reversed/cgminer/60_util_util/FUN_00024aa8.c | 分类: 60_util_util
// 语义: 队列补充线程体 (每 cgpu 一个): 循环 — cgpu+0x16c (shutdown) 置位则收尾返回;
//       清 thr+0x3f; 当 cgpu+0x164 (暂存工作槽) 为空时 get_work 并在 cgpu+0x140 读写锁
//       写保护下抢占填入 (竞态失败则 discard_work); 直到 drv 槽位 +0x48 (queue_full)
//       返回非 0。随后读 drv 槽位 +0x44(thr) 的 64 位排队数: -1 视为设备失效 →
//       报 "%s %d failure, disabling!" (drv+8=dname, cgpu+8=device_id), cgpu+0x20=1,
//       record_device_fail(cgpu, 0) (机器码调用点 r1 未装载, 按 0 转写) 并收尾返回。
//       累加排队数并按节流窗打 hashmeter(thr_id, sec_diff, total_queued):
//       静默条件 = (total==0 || (sec_diff<1 && usec_diff<200001)) && sec_diff < opt_log_interval
//       (DAT_00075dcc); 非静默时打点并把计时/计数清零。最后 thr+0x3c 或 cgpu+0x20 置位
//       时调用 mt_disable(thr, thr_id, drv); thr+0x3f 置位时回调 drv 槽位 +0x50(cgpu)。
//       thr_info 偏移: +0x3c/+0x3e/+0x3f 控制字节; cgpu 偏移: +0x04=drv, +0x08=device_id,
//       +0x20=收尾标志, +0x140=qlock(读写锁), +0x164=暂存工作槽, +0x16c=shutdown。
void fill_queue(void *thr)
{
	uint8_t *t = (uint8_t *)thr;
	int thr_id = *(int *)t;
	void *cgpu = *(void **)(t + 0x24);
	void *drv = *(void **)((uint8_t *)cgpu + 4);
	int64_t total_queued = 0;
	struct timeval tv_last = {0, 0};
	struct timeval tv_now;
	int sec_diff;
	int usec_diff;
	void *work;
	bool slot_free;
	int queue_full_ret;
	int64_t queued;
	char msg[4096];

	for (;;) {
		if (*(uint8_t *)((uint8_t *)cgpu + 0x16c) != '\0') {
		disable_exit:   /* 原 LAB_00024d3c: 置收尾标志并返回 (失效路径经 goto 无条件进入) */
			*(uint32_t *)((uint8_t *)cgpu + 0x20) = 1;
			return;
		}
		*(uint8_t *)(t + 0x3f) = 0;
		do {
			if (*(uint32_t *)((uint8_t *)cgpu + 0x164) == 0) {
				work = get_work(thr, thr_id);
				if (pthread_rwlock_wrlock((pthread_rwlock_t *)((uint8_t *)cgpu + 0x140)) != 0) {
					wr_lock_error("fill_queue", 0x223b);
				}
				slot_free = (*(uint32_t *)((uint8_t *)cgpu + 0x164) == 0);
				if (slot_free) {
					*(void **)((uint8_t *)cgpu + 0x164) = work;
				}
				if (pthread_rwlock_unlock((pthread_rwlock_t *)((uint8_t *)cgpu + 0x140)) != 0) {
					rw_unlock_error("fill_queue", 0x2242);
				}
				(*DAT_00075ed8)();
				if (!slot_free) {
					discard_work(work);   /* 竞态: 暂存槽已被其他线程占用 */
				}
			}
			queue_full_ret = (*(int (**)(void *))((uint8_t *)drv + 0x48))(cgpu);   /* drv->queue_full 槽位 +0x48 */
		} while (queue_full_ret == 0);
		queued = (*(int64_t (**)(void *))((uint8_t *)drv + 0x44))(thr);   /* drv 槽位 +0x44: 64 位排队数 (r0=低/r1=高) */
		*(uint8_t *)(t + 0x3e) = 0;
		if (queued == -1) {
			if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 2 < DAT_000766c4) {
				snprintf(msg, 0x1000, "%s %d failure, disabling!",
				         *(char **)((uint8_t *)drv + 8), *(int *)((uint8_t *)cgpu + 8));
				applog(3, msg, 0);
			}
			*(uint32_t *)((uint8_t *)cgpu + 0x20) = 1;
			/* 机器码调用点未装载 reason (r1 为 applog 调用后的残留值); 按 0 转写 */
			record_device_fail(cgpu, 0);
			goto disable_exit;
		}
		total_queued += queued;
		cgtime(&tv_now);
		sec_diff = tv_now.tv_sec - tv_last.tv_sec;
		usec_diff = tv_now.tv_usec - tv_last.tv_usec;
		if (usec_diff < 0) {
			usec_diff += 1000000;   /* 机器码拆分为 0xf4000+0x240 (ARM 立即数编码) */
			sec_diff -= 1;
		}
		if (!(((total_queued == 0) || (sec_diff < 1 && usec_diff < 200001)) &&
		      sec_diff < DAT_00075dcc)) {
			hashmeter(thr_id, sec_diff,
			          (unsigned int)(uint32_t)total_queued,
			          (unsigned int)((uint64_t)total_queued >> 32));
			copy_timeval(&tv_last, &tv_now);
			total_queued = 0;
		}
		/* 原 LAB_00024c64: 两路径汇合 — thr+0x3c 或 cgpu+0x20 置位 → mt_disable (原 LAB_00024cbc) */
		if (*(uint8_t *)(t + 0x3c) != '\0') {
			mt_disable(thr, thr_id, drv);
		} else if (*(uint32_t *)((uint8_t *)cgpu + 0x20) != 0) {
			mt_disable(thr, thr_id, drv);
		}
		if (*(uint8_t *)(t + 0x3f) != '\0') {
			(*(void (**)(void *))((uint8_t *)drv + 0x50))(cgpu);   /* drv 槽位 +0x50 (机器码, 具名待定) */
		}
	}
}

// === workid_hash_add === 原 FUN_00016624 @ 0x00016624
// 来源: reversed/cgminer/60_util_util/FUN_00016624.c | 分类: 60_util_util
// 语义: 把 work 挂入 thr_info+0x160 锚定的 workid 哈希表 (键 = work+0x724 的 4 字节)。
//       表载体 = 首个插入的 work (其 node+0x00 存表指针); thr+0x168 为登记计数。
//       哈希: TEA 型混淆 (键小端拼装 ^ 0x7f76d, 常量 0x112410d/0x9f49bac6,
//       移位序列 8/13/12/16/5/3/10/15), 结果存 work+0x744。插入: 桶头插 +
//       全局 FIFO 尾插; 当 (watermark+1)*10 <= 桶计数且未冻结时倍增扩容:
//       旧桶全部重链到新桶, 新平均负载 load_cap = ((2n-1)&count!=0) + (count>>(shift+1)),
//       超过 load_cap 的桶计数 over_cnt, 连续两次 over_cnt > count/2 则冻结扩容 (frozen=1)。
//       任何 malloc 失败直接 exit(-1) (原 LAB_00016a78, 无日志)。
void workid_hash_add(void *thr, void *work)
{
	uint8_t *t = (uint8_t *)thr;
	uint8_t *w = (uint8_t *)work;
	workid_ht_node_t *node = (workid_ht_node_t *)(w + 0x728);
	workid_ht_t *table;
	workid_ht_bucket_t *bucket;
	uint8_t *carrier;                 /* 表载体 work (thr+0x160 指向) */
	uint32_t a, b, c;
	uint32_t i;
	uint32_t idx;
	size_t mem;
	workid_ht_node_t *next_node;
	workid_ht_node_t *old_head;
	workid_ht_bucket_t *new_buckets;
	uint32_t key_b0, key_b1, key_b2, key_b3;

	*(uint32_t *)(t + 0x168) += 1;                /* 登记计数 (thr+0x168) */
	node->list_next = NULL;                       /* work+0x730 */
	node->key = w + 0x724;                        /* work+0x73c: 键指针 */
	node->key_len = 4;                            /* work+0x740 */

	if (*(void **)(t + 0x160) == NULL) {
		/* 首个工作: 自身为表载体, 创建表 */
		*(void **)(t + 0x160) = work;
		node->list_prev = NULL;                   /* work+0x72c: FIFO 前驱 = 无 */
		table = (workid_ht_t *)malloc(0x2c);
		*(workid_ht_t **)(w + 0x728) = table;
		if (table == NULL) {
			exit(-1);   /* 原 LAB_00016a78 */
		}
		memset(table, 0, 0x2c);
		table->tail_node = (workid_ht_node_t *)(w + 0x728);
		table->nbuckets = 0x20;
		table->shift = 5;
		table->node_off = 0x728;
		table->buckets = (workid_ht_bucket_t *)malloc(0x180);   /* 0x20 桶 × 12 字节 */
		if (table->buckets == NULL) {
			exit(-1);   /* 原 LAB_00016a78 */
		}
		memset(table->buckets, 0, 0x180);
		table->seed = 0xa0111fe1;
	} else {
		carrier = *(uint8_t **)(t + 0x160);
		table = *(workid_ht_t **)(carrier + 0x728);
		/* 全局 FIFO 尾插: 前驱容器指针由 tail_node 按 node_off 还原 (container_of) */
		*(void **)((uint8_t *)table->tail_node + 8) = work;      /* 前驱->list_next = 本 work */
		node->list_prev = (void *)((uint8_t *)table->tail_node - table->node_off);
		table->tail_node = node;
	}
	carrier = *(uint8_t **)(t + 0x160);
	table = *(workid_ht_t **)(carrier + 0x728);   /* 与机器码一致的重装载 */
	table->count += 1;
	/* TEA 型混淆哈希: 键 4 字节按小端拼装 (b0 为最低字节) */
	key_b0 = *(uint8_t *)(w + 0x724);
	key_b1 = *(uint8_t *)(w + 0x725);
	key_b2 = *(uint8_t *)(w + 0x726);
	key_b3 = *(uint8_t *)(w + 0x727);
	a = ((key_b0 + 0x112410du + ((uint32_t)key_b3 << 24) + ((uint32_t)key_b2 << 16) +
	      ((uint32_t)key_b1 << 8)) ^ 0x7f76du);
	b = (0x9f49bac6u - a) ^ (a << 8);
	c = ((uint32_t)(-0x112410d - a) - b) ^ (b >> 13);
	a = ((a - b) - c) ^ (c >> 12);
	b = ((b - c) - a) ^ (a << 16);
	c = ((c - a) - b) ^ (b >> 5);
	a = ((a - b) - c) ^ (c >> 3);
	b = ((b - c) - a) ^ (a << 10);
	node->hash = ((c - a) - b) ^ (b >> 15);       /* work+0x744 */
	node->table = table;                          /* work+0x728: 回指表 */
	/* 桶头插 */
	bucket = &table->buckets[node->hash & (table->nbuckets - 1)];
	bucket->count += 1;
	node->bkt_prev = NULL;                        /* work+0x734 */
	node->bkt_next = bucket->head;                /* work+0x738 */
	old_head = bucket->head;
	if (old_head != NULL) {
		old_head->bkt_prev = node;
	}
	bucket->head = node;
	/* 扩容判定: (watermark+1)*10 <= 桶计数 且未冻结 */
	if ((uint32_t)((bucket->watermark + 1) * 10) <= bucket->count && table->frozen != 1) {
		mem = (size_t)table->nbuckets * 0x18;     /* 新桶数 = 2n, 每桶 12 字节 */
		new_buckets = (workid_ht_bucket_t *)malloc(mem);
		if (new_buckets == NULL) {
			exit(-1);   /* 原 LAB_00016a78 */
		}
		memset(new_buckets, 0, mem);
		/* 新平均负载估计 → 扩容除数; 超载桶计数清零 */
		table->load_cap = (uint32_t)(((table->nbuckets * 2u - 1u) & table->count) != 0) +
		                  (table->count >> ((table->shift + 1) & 0xff));
		table->over_cnt = 0;
		if (table->nbuckets != 0) {
			for (i = 0; i < table->nbuckets; i++) {
				workid_ht_node_t *n = table->buckets[i].head;
				if (n != NULL) {
					do {
						next_node = n->bkt_next;                 /* 改链前保存旧后继 */
						idx = (table->nbuckets * 2u - 1u) & n->hash;
						bucket = &new_buckets[idx];
						bucket->count += 1;
						if (table->load_cap < bucket->count) {
							table->over_cnt += 1;
							bucket->watermark = bucket->count / table->load_cap;   /* __udivsi3 */
						}
						n->bkt_prev = NULL;
						n->bkt_next = bucket->head;
						if (bucket->head != NULL) {
							bucket->head->bkt_prev = n;
						}
						bucket->head = n;
						n = next_node;
					} while (next_node != NULL);
				}
			}
		}
		free(table->buckets);
		table->nbuckets <<= 1;
		table->shift += 1;
		table->buckets = new_buckets;
		if (table->count >> 1 < table->over_cnt) {
			table->grow_cnt += 1;
		} else {
			table->grow_cnt = 0;
		}
		if (1 < table->grow_cnt) {
			table->frozen = 1;
		}
	}
}

// === tq_freezethaw === 原 FUN_0002a230 @ 0x0002a230
// 来源: reversed/cgminer/60_util_util/FUN_0002a230.c | 分类: 60_util_util
// 语义: 冻结 thread_q (布局: +0x00/+0x04 列表哨兵, +0x08 冻结标志字节, +0x0c 互斥锁,
//       +0x28 条件变量): 在锁内置冻结标志并 signal, tq_push (FUN_0002a240) 见标志即丢弃新项。
void tq_freezethaw(void *tq)
{
	uint8_t *q = (uint8_t *)tq;

	if (pthread_mutex_lock((pthread_mutex_t *)(q + 0xc)) != 0) {
		mutex_lock_fail_util("tq_freezethaw", 0x4d4);
	}
	*(uint8_t *)(q + 8) = 1;
	pthread_cond_signal((pthread_cond_t *)(q + 0x28));
	mutex_unlock_util((pthread_mutex_t *)(q + 0xc), "tq_freezethaw", 0x4d7);
	(*DAT_00075ed8)();
}

// === _wlogprint === 原 FUN_00018bc0 @ 0x00018bc0
// 来源: reversed/cgminer/60_util_util/FUN_00018bc0.c | 分类: 60_util_util
// 语义: curses 激活时向 logwin 打印字符串并解 curses 锁 (锁由 curses_active_locked 持有),
//       解锁失败报 "WTF MUTEX ERROR ON UNLOCK!"(cgminer.c 串) 并 quit(1)。
void _wlogprint(const char *str)
{
	if (curses_active_locked() == 0) {
		return;
	}
	wprintw(DAT_0007b430, "%s", str);
	if (pthread_mutex_unlock(&DAT_0007b7a8) != 0) {
		mutex_unlock_fail_cgminer("unlock_curses", 0xb72);
	}
	(*DAT_00075ed8)();
}

// === curses_input === 原 FUN_00025fdc @ 0x00025fdc
// 来源: reversed/cgminer/60_util_util/FUN_00025fdc.c | 分类: 60_util_util
// 语义: curses 交互输入: echo → malloc(0xff) (失败报 "Failed to malloc input" 并 quit(1))
//       → leaveok 关闭 → 打印 "%s:\n" → wgetnstr 读入 (上限 0xff) → 空输入替换为 "-1"
//       (上游约定的取消值) → leaveok 恢复 + noecho, 返回堆缓冲 (调用方负责释放)。
char *curses_input(const char *prompt)
{
	char msg[4096];
	char *input;

	echo();
	input = (char *)malloc(0xff);
	if (input == NULL) {
		snprintf(msg, 0x1000, "Failed to malloc input");
		applog(3, msg, 1);
		quit(1);
	}
	leaveok(DAT_0007b430, 0);
	snprintf(msg, 0x1000, "%s:\n", prompt);
	_wlogprint(msg);
	wgetnstr(DAT_0007b430, input, 0xff);
	if (*input == '\0') {
		input[0] = '-';
		input[1] = '1';
		input[2] = '\0';
	}
	leaveok(DAT_0007b430, 1);
	noecho();
	return input;
}

// === input_pool === 原 FUN_00026d10 @ 0x00026d10
// 来源: reversed/cgminer/60_util_util/FUN_00026d10.c | 分类: 60_util_util
// 语义: curses 交互添加矿池: 打印 "Input server details.\n" (immedok 开启), 依次
//       curses_input("URL"/"Username"/"Password")。任一输入为 "-1" 即中止 (释放已得
//       缓冲并 immedok 关闭, 返回 0; 机器码在 URL="-1" 短路路径上 user = url[2] = NULL)。
//       密码 "-1" 替换为 calloc(1,1) 空串。非 stratum 且无 http://(7)/https://(8) 前缀的
//       URL 重拼为 "http://"+url (malloc(0x100), 失败报 "Failed to malloc httpinput"
//       — 原字面量无空格 — 并 quit(1)), 原 url 释放。add_pool_details 成功 (非 0) 时
//       字符串所有权移交矿池, 直接返回其值; 否则释放三个缓冲返回 0。
//       注意: 字面量为固件 .rodata 原文, 按 CONVENTIONS 原样保留。
int input_pool(int use_pool)
{
	char msg[4096];
	char *url;
	char *user;
	char *pass;
	char *full_url;
	void *pool;
	int ret;

	immedok(DAT_0007b430, 1);
	snprintf(msg, 0x1000, "Input server details.\n");
	_wlogprint(msg);
	url = curses_input("URL");
	full_url = url;
	/* 机器码: url=="-1" 时 user = url[2] (=0, 即 NULL), 否则 user = curses_input("Username") */
	if (((url[0] == '-') && (url[1] == '1') && ((user = NULL), url[2] == '\0')) ||
	    (((user = curses_input("Username")), user[0] == '-' && user[1] == '1') &&
	     (user[2] == '\0'))) {
		immedok(DAT_0007b430, 0);
		pass = NULL;
	} else {
		pass = curses_input("Password");
		if ((pass[0] == '-') && (pass[1] == '1') && (pass[2] == '\0')) {
			free(pass);
			pass = (char *)calloc(1, 1);   /* 空串占位 */
		}
		pool = add_pool();
		ret = detect_stratum(pool, url);
		if ((ret == 0) && (strncmp(url, "http://", 7) != 0) && (strncmp(url, "https://", 8) != 0)) {
			full_url = (char *)malloc(0x100);
			if (full_url == NULL) {
				snprintf(msg, 0x1000, "Failed to malloc httpinput");
				applog(3, msg, 1);
				quit(1);
			}
			strncpy(full_url, "http://", 8);   /* 机器码为 8 字节字面量拷贝 (含 NUL) */
			strncat(full_url, url, 0xf8);
			free(url);
		}
		ret = add_pool_details(pool, use_pool, full_url, user, pass);
		immedok(DAT_0007b430, 0);
		if (ret != 0) {
			return ret;   /* 字符串所有权已移交矿池, 不再释放 */
		}
	}
	free(full_url);
	free(user);
	free(pass);
	return 0;
}

// === add_pool_details === 原 FUN_00026bcc @ 0x00026bcc
// 来源: reversed/cgminer/60_util_util/FUN_00026bcc.c | 分类: 60_util_util
// 语义: 填充矿池详细信息并启动测试线程: pool+0xc4=rpc_proxy(get_proxy), +0xcc=rpc_user,
//       +0xd0=rpc_pass, +0xc8=rpc_userpass (malloc(strlen(user)+strlen(pass)+2), 失败报
//       "Failed to malloc userpass" 并 quit(1)), snprintf "%s:%s"。pool+0x8b = live^1
//       (0=live: 测试线程跑完一轮即退出), pool+0x84 != 1 时置 enabled=1 并 total_pools++
//       (DAT_0007b5e4), pool+0x13c/+0x81 置 1, pthread_create(pool+0x138, NULL,
//       池测试线程, pool)。live==0 时 join 测试线程并返回全局标志 DAT_0007730c
//       (main "No servers were found..." 等待循环所查的 pools_active 类标志 [低置信]);
//       live 非 0 直接返回 1。
uint8_t add_pool_details(void *pool, int live, char *url, char *user, char *pass)
{
	char msg[4096];
	char *userpass;
	size_t len;
	bool first_enable;
	void *proxy;

	proxy = get_proxy(url, pool);
	*(char **)((uint8_t *)pool + 0xcc) = user;
	*(char **)((uint8_t *)pool + 0xd0) = pass;
	*(void **)((uint8_t *)pool + 0xc4) = proxy;
	len = strlen(user) + strlen(pass) + 2;
	userpass = (char *)malloc(len);
	*(char **)((uint8_t *)pool + 0xc8) = userpass;
	if (userpass == NULL) {
		snprintf(msg, 0x1000, "Failed to malloc userpass");
		applog(3, msg, 1);
		quit(1);
		/* 机器码 quit 后的汇合块仍从矿池字段重取指针 (quit 实际不返回, 保留其形态) */
		userpass = *(char **)((uint8_t *)pool + 0xc8);
		user = *(char **)((uint8_t *)pool + 0xcc);
		pass = *(char **)((uint8_t *)pool + 0xd0);
	}
	snprintf(userpass, len, "%s:%s", user, pass);
	first_enable = (*(uint32_t *)((uint8_t *)pool + 0x84) != 1);
	*(uint8_t *)((uint8_t *)pool + 0x8b) = (uint8_t)(live ^ 1);
	if (first_enable) {
		*(uint32_t *)((uint8_t *)pool + 0x84) = 1;   /* pool->enabled = 1 */
	}
	*(uint8_t *)((uint8_t *)pool + 0x13c) = 1;
	*(uint8_t *)((uint8_t *)pool + 0x81) = 1;
	if (first_enable) {
		DAT_0007b5e4 += 1;   /* total_pools++ */
	}
	pthread_create((pthread_t *)((uint8_t *)pool + 0x138), NULL, fun_0001fe0c, pool);   /* 池测试线程 (命名待补, 上游 test_pool_thread) */
	if (live == 0) {
		pthread_join(*(pthread_t *)((uint8_t *)pool + 0x138), NULL);
		return DAT_0007730c;
	}
	return 1;
}

// === ser_string === 原 FUN_00029e48 @ 0x00029e48
// 来源: reversed/cgminer/60_util_util/FUN_00029e48.c | 分类: 60_util_util
// 语义: stratum 协议字符串序列化: malloc(strlen+9), 按长度分级编码长度前缀 (大端):
//       len < 0xfd → [len][data], 写入数 = len+1; len < 0x10000 → [0xfd][len_be16][data],
//       写入数 = len+3; 否则 → [0xfe][len_be32][data], 写入数 = len+5。
//       *out_len = 写入字节数; 失败 (malloc) 报 "Failed to malloc ret in ser_string" 并 quit(1)。
uint8_t *ser_string(char *str, int *out_len)
{
	char msg[4096];
	size_t len;
	uint8_t *buf;

	len = strlen(str);
	buf = (uint8_t *)malloc(len + 9);
	if (buf == NULL) {
		snprintf(msg, 0x1000, "Failed to malloc ret in ser_string");
		applog(3, msg, 1);
		quit(1);
	}
	if (len < 0xfd) {
		buf[0] = (uint8_t)len;
		memcpy(buf + 1, str, len);
		*out_len = (int)(len + 1);
	} else if (len < 0x10000) {
		/* 机器码: 存 bswap16(len), 小端写入等价于大端长度 */
		*(uint16_t *)(buf + 1) = (uint16_t)(((len >> 8) & 0xff) | (len << 8));
		buf[0] = 0xfd;
		memcpy(buf + 3, str, len);
		*out_len = (int)(len + 3);
	} else {
		/* 机器码: 存 bswap32(len), 小端写入等价于大端长度 */
		*(uint32_t *)(buf + 1) = (uint32_t)((len << 24) | (((len >> 8) & 0xff) << 16) |
		                                   (((len >> 16) & 0xff) << 8) | (len >> 24));
		buf[0] = 0xfe;
		memcpy(buf + 5, str, len);
		*out_len = (int)(len + 5);
	}
	return buf;
}

// === realloc_strcat === 原 FUN_0002ee78 @ 0x0002ee78
// 来源: reversed/cgminer/60_util_util/FUN_0002ee78.c | 分类: 60_util_util
// 语义: 追加字符串: s2 为空 → 原样返回 s1 (不分配); 否则按 4 字节对齐大小
//       malloc(strlen(s1)+strlen(s2)+1 向上取整) (失败报 "Failed to malloc in util.c
//       realloc_strcat():0xe79" 并 quit(1)); s1 为 NULL → strcpy(s2), 否则 sprintf
//       "%s%s" 并释放旧 s1, 返回新缓冲。
char *realloc_strcat(char *dest, const char *src)
{
	char msg[4096];
	size_t len;
	char *out;

	len = strlen(src);
	out = dest;
	if (len != 0) {
		size_t total = 0;
		if (dest != NULL) {
			total = strlen(dest);
		}
		total = len + 1 + total;
		if ((total & 3) != 0) {
			total = (total & 0xfffffffcu) + 4;
		}
		out = (char *)malloc(total);
		if (out == NULL) {
			snprintf(msg, 0x1000, "Failed to malloc in %s %s():%d", "util.c", "realloc_strcat", 0xe79);
			applog(3, msg, 1);
			quit(1);
		}
		if (dest == NULL) {
			strcpy(out, src);
		} else {
			sprintf(out, "%s%s", dest, src);
			free(dest);
		}
	}
	return out;
}

// === str_text === 原 FUN_0002ef64 @ 0x0002ef64
// 来源: reversed/cgminer/60_util_util/FUN_0002ef64.c | 分类: 60_util_util
// 语义: 把字符串转成可打印表示: 可打印字节 (0x20..0x7e) 原样拷贝, 其余字节
//       (含结尾 NUL!) 以 sprintf "0x%02x" 展开 (4 字符), 最后补 NUL; 返回
//       malloc(strlen*4+5) 缓冲。NULL 入参: 分配 7 字节写入 "(null)"
//       (机器码为 *(uint32*)=0x6c756e28 / *(uint16*)=0x296c) — 但原机器码该分支
//       不返回, 直落 strlen(NULL) (必崩), 本转写按机器码保留该死代码路径。
char *str_text(const uint8_t *ptr)
{
	char msg[4096];
	char *txt;
	char *out;
	const uint8_t *p = ptr;
	uint8_t ch;

	if (p == NULL) {
		txt = (char *)malloc(7);
		if (txt == NULL) {
			snprintf(msg, 0x1000, "Failed to malloc null in %s %s():%d", "util.c", "str_text", 0xe92);
			applog(3, msg, 1);
			quit(1);
		} else {
			txt[6] = '\0';
			*(uint32_t *)txt = 0x6c756e28;   /* "(nul" */
			*(uint16_t *)(txt + 4) = 0x296c; /* "l)"   */
		}
		/* 机器码: 未返回 txt, 继续以 ptr(=NULL) 走主路径 (死代码, 语义保留) */
	}
	txt = (char *)malloc(strlen((const char *)p) * 4 + 5);
	if (txt == NULL) {
		snprintf(msg, 0x1000, "Failed to malloc txt in %s %s():%d", "util.c", "str_text", 0xe99);
		applog(3, msg, 1);
		quit(1);
	}
	out = txt;
	do {
		ch = *p;
		if ((uint8_t)(ch - 0x20) < 0x5f) {
			/* 可打印: 原样拷贝 */
			*out = (char)ch;
			out++;
			p++;
		} else {
			/* 不可打印 (含 NUL): "0x%02x" 4 字符展开 */
			sprintf(out, "0x%02x", (unsigned int)ch);
			out += 4;
			p++;
		}
	} while (ch != 0);
	*out = '\0';
	return txt;
}

/* ============================================================================
 * klist.c — 链表/仓库 (K_LIST 主列表 + K_LIST store 空闲仓库, 共享 KL_LOCK)
 * ============================================================================ */

// === k_alloc_items === 原 FUN_00038b58 @ 0x00038b58
// 来源: reversed/cgminer/60_util_util/FUN_00038b58.c | 分类: 60_util_util
// 语义: 为列表批量分配 K_ITEM (calloc(n,0x10)) 并链成双向空闲链, 逐项配 data 块
//       (calloc(1,siz)); 数量 = limit<1 ? allocate : min(allocate, limit-total)
//       (limit>0 且 total>=limit 时直接返回); store 不可调用 (报错后 quit)。
//       item_memory/data_memory 指针表 realloc 追踪各批分配 (失败分别报
//       "...item_memory failed to realloc" / "...data_memory failed to realloc" /
//       "...failed to calloc ... items" / "...failed to calloc item data" 并 quit(1))。
//       机器码对首项无条件写 next=&items[1]、尾项 prev=&items[n-2] (n=1 时为越界
//       指针, 不解引用, 原样保留)。
void k_alloc_items(K_LIST *klist, const char *file, const char *func, const int line)
{
	char msg[4096];
	int alloc_items;
	K_ITEM *items;
	K_ITEM *last;
	void *data;
	int i;
	int n;

	if (klist->is_store != '\0') {
		snprintf(msg, 0x1000,
		         "List %s store can't %s() - from %s %s() line %d in %s %s():%d",
		         klist->name, "k_alloc_items", file, func, line, "klist.c", "k_alloc_items", 0x13);
		applog(3, msg, 1);
		quit(1);
	}
	if (klist->limit < 1) {
		alloc_items = klist->allocate;
	} else {
		if (klist->limit <= klist->total) {
			return;
		}
		alloc_items = klist->allocate;
		if (klist->limit < (int)(klist->total + alloc_items)) {
			alloc_items = klist->limit - klist->total;
		}
	}
	klist->item_mem_count += 1;
	klist->item_memory = (K_ITEM **)realloc(klist->item_memory, (size_t)klist->item_mem_count * 4);
	if (klist->item_memory == NULL) {
		snprintf(msg, 0x1000, "List %s item_memory failed to realloc count=%d in %s %s():%d",
		         klist->name, klist->item_mem_count, "klist.c", "k_alloc_items", 0x21);
		applog(3, msg, 1);
		quit(1);
	}
	items = (K_ITEM *)calloc((size_t)alloc_items, 0x10);
	if (items == NULL) {
		snprintf(msg, 0x1000,
		         "List %s failed to calloc %d new items - total was %d, limit was %d in %s %s():%d",
		         klist->name, alloc_items, klist->total, klist->limit, "klist.c", "k_alloc_items", 0x26);
		applog(3, msg, 1);
		quit(1);
	}
	klist->item_memory[klist->item_mem_count - 1] = items;
	n = alloc_items - 1;
	/* 链成双向链: items[0] 为头, items[n] 为尾 */
	items[0].name = klist->name;
	items[0].prev = NULL;
	items[0].next = &items[1];            /* 机器码无条件 +1 项地址 (n=0 时越界指针, 不解引用) */
	if (1 < n) {
		i = 1;
		do {
			items[i].name = klist->name;
			items[i].prev = &items[i - 1];
			items[i].next = &items[i + 1];
			i++;
		} while (i != n);
	}
	last = &items[n];
	last->name = klist->name;
	last->prev = &items[alloc_items - 2]; /* alloc_items==1 时越界指针 (机器码原样) */
	last->next = NULL;
	klist->head = items;
	if (klist->do_tail != '\0') {
		klist->tail = last;
	}
	klist->total += alloc_items;
	klist->count = alloc_items;
	klist->count_up = alloc_items;
	/* 逐项分配 data 块并登记 */
	do {
		klist->data_mem_count += 1;
		klist->data_memory = (void **)realloc(klist->data_memory, (size_t)klist->data_mem_count * 4);
		if (klist->data_memory == NULL) {
			snprintf(msg, 0x1000, "List %s data_memory failed to realloc count=%d in %s %s():%d",
			         klist->name, klist->data_mem_count, "klist.c", "k_alloc_items", 0x45);
			applog(3, msg, 1);
			quit(1);
		}
		data = calloc(1, klist->siz);
		items->data = data;
		if (data == NULL) {
			snprintf(msg, 0x1000, "List %s failed to calloc item data in %s %s():%d",
			         klist->name, "klist.c", "k_alloc_items", 0x49);
			applog(3, msg, 1);
			quit(1);
			data = items->data;   /* 机器码汇合块重取 (quit 实际不返回) */
		}
		klist->data_memory[klist->data_mem_count - 1] = data;
		items = items->next;
	} while (items != NULL);
}

// === k_new_store === 原 FUN_00038e94 @ 0x00038e94
// 来源: reversed/cgminer/60_util_util/FUN_00038e94.c | 分类: 60_util_util
// 语义: 建空闲仓库: calloc(1,0x40), is_store=1, 与主列表共享 lock, 复制 name/do_tail;
//       分配失败报 "Failed to calloc store for %s" 并 quit(1)。
K_LIST *k_new_store(K_LIST *klist)
{
	char msg[4096];
	K_LIST *store;

	store = (K_LIST *)calloc(1, 0x40);
	if (store == NULL) {
		snprintf(msg, 0x1000, "Failed to calloc store for %s in %s %s():%d",
		         klist->name, "klist.c", "k_new_store", 0x55);
		applog(3, msg, 1);
		quit(1);
	}
	store->is_store = 1;
	store->lock = klist->lock;
	store->name = klist->name;
	store->do_tail = klist->do_tail;
	return store;
}

// === _k_new_list === 原 FUN_00038f3c @ 0x00038f3c
// 来源: reversed/cgminer/60_util_util/FUN_00038f3c.c | 分类: 60_util_util
// 语义: 建主列表: 校验 allocate > 0 与 limit >= 0 (违规报 "Invalid new list ..." 并
//       quit(1)), calloc(1,0x40), is_store=0, 再 calloc KL_LOCK (0x38 字节: mutex@+0,
//       rwlock@+0x18) 并初始化两把锁 (失败分别报 "...pthread_mutex_init / ...pthread_rwlock_init"
//       errno=%d ...():0x73 并 quit(1)); 填 name/siz/allocate/limit/do_tail 后调
//       k_alloc_items 做首次批量分配, 返回列表指针。
K_LIST *_k_new_list(const char *name, unsigned int siz, int allocate, int limit, uint8_t do_tail,
                    const char *file, const char *func, const int line)
{
	char msg[4096];
	K_LIST *klist;
	struct kl_lock *lock;
	int ret;

	if (allocate < 1) {
		snprintf(msg, 0x1000, "Invalid new list %s with allocate %d must be > 0 in %s %s():%d",
		         name, allocate, "klist.c", "_k_new_list", 100);
		applog(3, msg, 1);
		quit(1);
	}
	if (limit < 0) {
		snprintf(msg, 0x1000, "Invalid new list %s with limit %d must be >= 0 in %s %s():%d",
		         name, limit, "klist.c", "_k_new_list", 0x67);
		applog(3, msg, 1);
		quit(1);
	}
	klist = (K_LIST *)calloc(1, 0x40);
	if (klist == NULL) {
		snprintf(msg, 0x1000, "Failed to calloc list %s in %s %s():%d",
		         name, "klist.c", "_k_new_list", 0x6b);
		applog(3, msg, 1);
		quit(1);
	}
	klist->is_store = 0;
	lock = (struct kl_lock *)calloc(1, 0x38);
	klist->lock = lock;
	if (lock == NULL) {
		snprintf(msg, 0x1000, "Failed to calloc lock for list %s in %s %s():%d",
		         name, "klist.c", "_k_new_list", 0x71);
		applog(3, msg, 1);
		quit(1);
		lock = klist->lock;   /* 机器码汇合块重取 (quit 实际不返回) */
	}
	ret = pthread_mutex_init(&lock->mutex, NULL);
	if (ret != 0) {
		snprintf(msg, 0x1000, "Failed to pthread_mutex_init errno=%d in %s %s():%d",
		         errno, "klist.c", "_k_new_list", 0x73);
		applog(3, msg, 1);
		quit(1);
	}
	/* rwlock 固定位于 KL_LOCK+0x18 (机器码锁定偏移) */
	ret = pthread_rwlock_init((pthread_rwlock_t *)((uint8_t *)lock + 0x18), NULL);
	if (ret != 0) {
		snprintf(msg, 0x1000, "Failed to pthread_rwlock_init errno=%d in %s %s():%d",
		         errno, "klist.c", "_k_new_list", 0x73);
		applog(3, msg, 1);
		quit(1);
	}
	klist->name = (char *)name;
	klist->siz = siz;
	klist->allocate = allocate;
	klist->limit = limit;
	klist->do_tail = do_tail;
	k_alloc_items(klist, file, func, line);
	return klist;
}

// === _k_unlink_tail === 原 FUN_000392a8 @ 0x000392a8
// 来源: reversed/cgminer/60_util_util/FUN_000392a8.c | 分类: 60_util_util
// 语义: 摘除尾项 (调用方摘前自行读 klist->tail — 本函数无返回值, 机器码 r0 不带出):
//       do_tail 为假时报 "...do_tail is false..." 并 quit(1); tail 为空则无事;
//       否则 tail = tail->prev, 新尾为空时 head 亦清空, 摘除项的前后链清零, count--。
void _k_unlink_tail(K_LIST *klist, const char *file, const char *func, const int line)
{
	char msg[4096];
	K_ITEM *tail;
	K_ITEM *new_tail;

	if (klist->do_tail == '\0') {
		snprintf(msg, 0x1000,
		         "List %s can't %s() - do_tail is false - from %s %s() line %d in %s %s():%d",
		         klist->name, "_k_unlink_tail", file, func, line, "klist.c", "_k_unlink_tail", 0xb5);
		applog(3, msg, 1);
		quit(1);
	}
	tail = klist->tail;
	if (tail != NULL) {
		new_tail = tail->prev;
		klist->tail = new_tail;
		if (new_tail == NULL) {
			klist->head = NULL;
		} else {
			new_tail->next = NULL;
		}
		tail->next = NULL;
		tail->prev = NULL;
		klist->count -= 1;
	}
}

// === _k_add_head === 原 FUN_00039370 @ 0x00039370
// 来源: reversed/cgminer/60_util_util/FUN_00039370.c | 分类: 60_util_util
// 语义: 头插: item->name 与列表名指针不符时报 "...a %s item..." 并 quit(1);
//       item->prev=NULL, item->next=旧 head, 旧 head->prev=item, head=item;
//       do_tail 且 tail 为空时 tail=item; count++ / count_up++。
void _k_add_head(K_LIST *klist, K_ITEM *item, const char *file, const char *func, const int line)
{
	char msg[4096];

	if (item->name != klist->name) {
		snprintf(msg, 0x1000,
		         "List %s can't %s() a %s item - from %s %s() line %d in %s %s():%d",
		         klist->name, "_k_add_head", item->name, file, func, line, "klist.c", "_k_add_head", 0xcd);
		applog(3, msg, 1);
		quit(1);
	}
	item->prev = NULL;
	item->next = klist->head;
	if (klist->head != NULL) {
		klist->head->prev = item;
	}
	klist->head = item;
	if ((klist->do_tail != '\0') && (klist->tail == NULL)) {
		klist->tail = item;
	}
	klist->count += 1;
	klist->count_up += 1;
}

// === _k_add_tail === 原 FUN_00039460 @ 0x00039460
// 来源: reversed/cgminer/60_util_util/FUN_00039460.c | 分类: 60_util_util
// 语义: 尾插: 校验 item->name 与 do_tail (违规分别报 "...a %s item..." /
//       "...do_tail is false..." 并 quit(1)); item->next=NULL, item->prev=旧 tail,
//       旧 tail->next=item, tail=item, head 为空时 head=item; count++ / count_up++。
void _k_add_tail(K_LIST *klist, K_ITEM *item, const char *file, const char *func, const int line)
{
	char msg[4096];
	K_ITEM *old_tail;

	if (item->name != klist->name) {
		snprintf(msg, 0x1000,
		         "List %s can't %s() a %s item - from %s %s() line %d in %s %s():%d",
		         klist->name, "_k_add_tail", item->name, file, func, line, "klist.c", "_k_add_tail", 0xec);
		applog(3, msg, 1);
		quit(1);
	}
	if (klist->do_tail == '\0') {
		snprintf(msg, 0x1000,
		         "List %s can't %s() - do_tail is false - from %s %s() line %d in %s %s():%d",
		         klist->name, "_k_add_tail", file, func, line, "klist.c", "_k_add_tail", 0xf1);
		applog(3, msg, 1);
		quit(1);
	}
	old_tail = klist->tail;
	item->next = NULL;
	item->prev = old_tail;
	if (klist->tail != NULL) {
		klist->tail->next = item;
	}
	klist->tail = item;
	if (klist->head == NULL) {
		klist->head = item;
	}
	klist->count += 1;
	klist->count_up += 1;
}

// === create_bitmain_get_hash_rate_pthread === 原 FUN_00049ac0 @ 0x00049ac0
// 来源: reversed/cgminer/60_util_util/FUN_00049ac0.c | 分类: 60_util_util
// 语义: 创建 "从 ASIC 取算力" 线程: 全局 DAT_0007c05c = calloc(1,0x40) (新 thr_info),
//       thr_info_create(..., hashmeter_thread, 同指针); 成功则 pthread_detach(thr+0xc)
//       并 cgsleep_ms(500) (返回值经 r0 直通作为本函数返回值); 失败按 opt_debug 门控报
//       "%s: create thread for get hashrate from asic failed" 并返回 -6 (0xfffffffa)。
int create_bitmain_get_hash_rate_pthread(void)
{
	char msg[4096];
	int ret;

	DAT_0007c05c = calloc(1, 0x40);
	ret = thr_info_create(DAT_0007c05c, NULL, hashmeter_thread, DAT_0007c05c);
	if (ret == 0) {
		pthread_detach(*(pthread_t *)((uint8_t *)DAT_0007c05c + 0xc));
		ret = (int)cgsleep_ms(500);
	} else {
		if (DAT_00078e09 != '\0' &&
		    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 6 < DAT_000766c4)) {
			snprintf(msg, 0x1000, "%s: create thread for get hashrate from asic failed", __func__);
			applog(7, msg, 0);
		}
		ret = -6;   /* 0xfffffffa */
	}
	return ret;
}

// === start_http_listen_thread === 原 FUN_00056a90 @ 0x00056a90
// 来源: reversed/cgminer/60_util_util/FUN_00056a90.c | 分类: 60_util_util
// 语义: 以 0x200000 (2MiB) 栈大小创建搜频 HTTP 服务线程 (http_listen_thread,
//       FUN_0005664c, 端口 6060), 按 prio 5 门控报 "httpListenThread start ret=%d"。
void start_http_listen_thread(void)
{
	char msg[4096];
	pthread_attr_t attr;
	pthread_t thread;
	int ret;

	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, 0x200000);
	ret = pthread_create(&thread, &attr, http_listen_thread, NULL);
	if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 4 < DAT_000766c4) {
		snprintf(msg, 0x1000, "httpListenThread start ret=%d", ret);
		applog(5, msg, 0);
	}
}

/* ==================== 覆盖率表 (batch=util_b1, 35/35) ====================
 * FUN_0000b3c8 @ 0x0000b3c8 → restart_threads_err
 * FUN_0000b414 @ 0x0000b414 → mutex_lock_fail_cgminer
 * FUN_0000b4ec @ 0x0000b4ec → mutex_init_fail_cgminer
 * FUN_0000b558 @ 0x0000b558 → rwlock_init_fail_cgminer
 * FUN_0000b5c4 @ 0x0000b5c4 → mutex_unlock_fail_cgminer
 * FUN_0000b708 @ 0x0000b708 → mutex_lock_fail_util
 * FUN_00011c8c @ 0x00011c8c → mt_disable
 * FUN_00011e24 @ 0x00011e24 → miner_thread
 * FUN_000120f4 @ 0x000120f4 → set_lowprio
 * FUN_00016624 @ 0x00016624 → workid_hash_add
 * FUN_00016ca8 @ 0x00016ca8 → quit
 * FUN_000181f4 @ 0x000181f4 → restart_wait
 * FUN_00018bc0 @ 0x00018bc0 → _wlogprint
 * FUN_00019f54 @ 0x00019f54 → add_queued
 * FUN_0001b124 @ 0x0001b124 → get_thread
 * FUN_000231f0 @ 0x000231f0 → restart_thread
 * FUN_00024aa8 @ 0x00024aa8 → fill_queue
 * FUN_00025fdc @ 0x00025fdc → curses_input
 * FUN_00026bcc @ 0x00026bcc → add_pool_details
 * FUN_00026d10 @ 0x00026d10 → input_pool
 * FUN_00029388 @ 0x00029388 → mutex_unlock_util
 * FUN_00029e48 @ 0x00029e48 → ser_string
 * FUN_0002a230 @ 0x0002a230 → tq_freezethaw
 * FUN_0002ee78 @ 0x0002ee78 → realloc_strcat
 * FUN_0002ef64 @ 0x0002ef64 → str_text
 * FUN_0002f23c @ 0x0002f23c → completion_thread
 * FUN_0002f280 @ 0x0002f280 → _cgsem_wait
 * FUN_00038b58 @ 0x00038b58 → k_alloc_items
 * FUN_00038e94 @ 0x00038e94 → k_new_store
 * FUN_00038f3c @ 0x00038f3c → _k_new_list
 * FUN_000392a8 @ 0x000392a8 → _k_unlink_tail
 * FUN_00039370 @ 0x00039370 → _k_add_head
 * FUN_00039460 @ 0x00039460 → _k_add_tail
 * FUN_00049ac0 @ 0x00049ac0 → create_bitmain_get_hash_rate_pthread
 * FUN_00056a90 @ 0x00056a90 → start_http_listen_thread
 */




