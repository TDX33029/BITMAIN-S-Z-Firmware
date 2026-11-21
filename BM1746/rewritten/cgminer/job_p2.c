/*
 * cgminer 挖矿任务层 第 2 段 (批次 job_b2, 3 个大函数) — BM1746 固件语义转写
 * =========================================================
 * 原始二进制: analysis/cgminer_z15_bm1746.elf (ARM32, SD 镜像 cgminer-z15, cgminer 4.9.0 分支)
 * 来源分类:   reversed/cgminer/20_job_mining/ (清单见 rewritten/batch_plan.tsv batch=job_b2)
 *
 * 本文件 3 个函数 (~2165 反编译行):
 *   1) test_work_current   (原 FUN_0001db0c, 878 行) — 区块数据库 (自定义 lookup2 哈希表,
 *      上限 6 条, 自底向上归并排序按插入序号删除最老块) + 新块检测/网络难度更新/set_curblock。
 *   2) watchdog_thread     (原 FUN_00023344, 580 行) — 2s 周期线程: curses 状态窗刷新 +
 *      计划停/开机调度 + 设备 SICK/DEAD/WELL 看门狗。
 *   3) parse_notify_equihash (原 FUN_0002cf40, 707 行) — stratum JSON 方法分派:
 *      mining.notify(Equihash)/set_difficulty/set_extranonce/set_target/reconnect/
 *      get_version/show_message/ping; 80 字节 Zcash 工作头拼装 + 系统时钟对齐。
 *
 * 转写约定: rewritten/CONVENTIONS.md; 命名事实源 rewritten/name_map/cgminer.tsv。
 *   - 结构体布局不可证处一律按机器码锁定的字节偏移访问 (每处注明偏移含义), 不虚构完整布局;
 *   - 区块数据库三结构 (0x2C 控制块 / 12 字节桶 / 0x68 块节点) 布局由全部调用点交叉锁定,
 *     还原为具名结构体 (字段偏移注释见定义处);
 *   - 编译器辅助例程 (__floatdidf/__fixunsdfdi/__udivsi3 等) 按原 C 源语义还原为类型转换/除法;
 *   - VectorShiftLeft/VectorSignedToFloat/VectorFloatToSigned 为 Ghidra 对 NEON/VFP 转换
 *     指令的渲染, 分别还原为 64 位移位 / int→float→double / float→int (舍入按 FPSCR 当前
 *     模式, 本固件常量路径下与 C 转换等价);
 *   - 复杂 goto (区块库归并循环 / parse_notify 方法分派) 按约定保留, LAB_xxxx 改为语义标签。
 */

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>     /* strncasecmp */
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <curses.h>

#include "../include/common_types.h"

/* ==================== 外部函数 (名取自 name_map/cgminer.tsv) ==================== */

extern void  applog(uint32_t prio, char *buf, ...);        /* FUN_00038730 */
extern void  quit(int status);                                            /* FUN_00016ca8 */
extern void  rd_lock_error(const char *fn, int line);                     /* FUN_0000b69c */
extern void  rw_unlock_error(const char *fn, int line);                   /* FUN_0000b630 */
extern void  wr_lock_error(const char *fn, int line);                     /* FUN_0000b480 */
extern void  mutex_lock_fail_cgminer(const char *fn, int line);           /* FUN_0000b414 */
extern void  mutex_unlock_fail_cgminer(const char *fn, int line);         /* FUN_0000b5c4 */
extern void  mutex_lock_fail_util(const char *fn, int line);              /* FUN_0000b708 */
extern void  __bin2hex(char *dst, const uint8_t *src, int len);           /* FUN_000296d8 */
extern void  suffix_string(uint64_t value, char *buf, size_t bufsiz, int sigfigs); /* FUN_00011580 */
extern void  cgtime(struct timeval *tv);                                  /* FUN_0002a84c */
extern double tdiff(struct timeval *later, struct timeval *earlier);      /* FUN_0002aa24 */
extern void  restart_threads(void);                                       /* FUN_00017210 */
extern void *current_pool(void);                                          /* FUN_0001da58 */
extern void  set_lowprio(void);                                           /* FUN_000120f4 */
extern void  rename_thread(const char *name);                             /* FUN_0002f0b4 */
extern void  discard_stale(void);                                         /* FUN_00022f88 */
extern void  hashmeter(int thr_id, int secs, uint32_t hashes_lo, uint32_t hashes_hi); /* FUN_0001bba4 */
extern int   curses_active_locked(void);                                  /* FUN_00017fe4 */
extern int   total_staged(void);                                          /* FUN_000188a8 */
extern void  adj_width(int items, int *width);                            /* FUN_00011548 */
extern void  adj_fwidth(float value, int *width);                         /* FUN_00011514 */
extern void *get_devices(int index);                                      /* FUN_0001b83c */
extern void *get_thread(int index);                                       /* FUN_0001b124 */
extern void  _cgsem_post(sem_t *sem, const char *file, const char *func, int line); /* FUN_0002f1bc */
extern void  record_device_fail(void *cgpu, int reason);                  /* FUN_0002ed88: 3=SICK 4=DEAD */
extern void  reinit_device(void *cgpu);                                   /* FUN_00016c88 */
extern int   should_run(void);                                            /* FUN_000118d4 */

/* jansson 兼容层 (FUN_00059xxx, 见 name_map) */
typedef void json_t;
extern json_t *json_loads(char *string, size_t flags, void *error);       /* FUN_00059230 */
extern json_t *json_object_get(json_t *object, const char *key);          /* FUN_00059e20 */
extern char   *json_dumps(json_t *value, unsigned int flags);             /* FUN_00057854 */
extern char   *json_string_value(const json_t *value);                    /* FUN_0005a178 */
extern json_t *json_array_get(const json_t *array, size_t index);         /* FUN_0005a008 */
extern int64_t json_integer_value(const json_t *value);                   /* FUN_0005a248 */
extern double  json_number_value(const json_t *value);                    /* FUN_0005a378 */
extern void    json_delete(json_t *value);                                /* FUN_0005a3e4 */
extern char   *json_array_string(const json_t *array, int index);         /* FUN_000292f4: 校验取元素后 strdup */

/* stratum / 池 / 块辅助 */
extern uint32_t parse_extranonce_equihash(void *pool, const json_t *params_val); /* FUN_000299ac */
extern int      extract_sockaddr(char *url, char **socka_url, char **socka_port); /* FUN_0002ab54 */
extern void     clear_pool_work(void *pool);                              /* FUN_000185c8 */
extern void     clear_sockbuf(void *pool);                                /* FUN_00028ae8 */
extern uint32_t restart_stratum(void *pool);                              /* FUN_0002cec8 */
extern int      stratum_send(void *pool, char *buf, size_t len);          /* FUN_0002b5c8 (uart.c) */
extern int      hex2bin(uint8_t *dst, const uint8_t *src, uint32_t dst_len); /* FUN_00029830 */
extern void     _rw_unlock(pthread_rwlock_t *lock, const char *fn, int line);      /* FUN_0002930c */
extern void     mutex_unlock_util(pthread_mutex_t *lock, const char *fn, int line); /* FUN_00029388 */
extern void     pool_died_silent(void *pool);                             /* FUN_0001fb44 */

extern int *__errno_location(void);                                       /* libc */

/* ==================== 外部全局 (DAT_ 原名保留, 语义见注释) ==================== */

/* 日志门控 (与 log.c/uart.c/util.c 同一全局组) */
extern uint8_t DAT_00078e09;          /* opt_debug (日志门控外层总开关) */
extern uint8_t DAT_0007ad48;          /* applog 门控 A: 非 0 → applog 走 syslog(prio|0x80) */
extern uint8_t DAT_00078e08;          /* applog 门控 B = opt_verbose */
extern int     DAT_000766c4;          /* applog 门控 C: int 日志级别 */
extern void  (*DAT_00075ed8)(void);   /* 全局函数指针: ELF 初值 noop_hook (恒 0), 解锁后调用 */

/* 区块数据库 (test_work_current) */
extern pthread_rwlock_t DAT_00077294; /* 块库读写锁 (block_exists/新增块路径持有) */
extern void  *DAT_000772b4;           /* 块库锚节点: 最新建库/保留的最老块节点 (其 +0x44 = 控制块指针;
                                       * 同时兼作哈希表载体, job_p1.c 中 "workid 表" 注释与本文件
                                       * 机器码证据 (block_exists/Deleted block %d) 不符, 以本文件为准)
                                       * [存证互引] job_p1.c 判读证据: 段 2 建表机器码 (Jenkins lookup2
                                       * 哈希 / 0x20 桶 / 种子 0xa0111fe1 / 节点+0x44 表指针), 见其注释 */
extern int     DAT_0007bb1c;          /* 已检测新块总数 (块节点插入序号源; curses "NB") */
extern int     DAT_000770ac;          /* 区块变更序号 (写入 work+0x720 = work->block_nr) */
extern pthread_mutex_t DAT_0007b9d8;  /* 当前块信息组合锁 (set_curblock 路径, ch_lock) */
extern pthread_rwlock_t DAT_0007b9f0; /* 当前块信息读写锁 (set_curblock 路径) */
extern struct timeval DAT_0007b070;   /* 当前块检测时刻 (set_curblock 写入) */
extern char    DAT_0007bb28[];        /* 当前块哈希十六进制串 (68+ 字节, strcpy 写入) */
extern uint32_t DAT_000772c0[8];      /* 当前块哈希 8×u32 (与 DAT_0007bb28 同步) */
extern char    DAT_000772e0[];        /* 当前块起始时刻 "[%02d:%02d:%02d]" (0x20 字节) */
extern char    DAT_00077300[];        /* 当前块显示串 (去前导零后 8 字符) */
extern char    DAT_00077308;          /* DAT_00077300 的 NUL 终止符 */
extern char    DAT_000772b8[];        /* 网络难度显示串 (suffix_string 缓冲, 8 字节, 后接 DAT_000772c0) */
extern double  DAT_00075de0;          /* 上次网络难度 (变更检测比较值) */

/* 池/挖矿策略 */
extern int     DAT_00076efc;          /* 挖矿策略: 0=Failover 1=RoundRobin 2=Rotate 3=LoadBalance 4=Balance */
extern uint8_t DAT_0007bc79;          /* stratum 线程已启动标志 (pool_p2.c 置 1); 亦用作 longpoll 支持显示 */

/* watchdog_thread / curses */
extern WINDOW  *DAT_0007bc90;         /* ncurses WINDOW* (尺寸检查源窗口, 语义待定) */
extern WINDOW  *DAT_0007b7c0;         /* statuswin: curses 状态窗口 */
extern WINDOW  *DAT_0007b430;         /* logwin: curses 日志窗口 */
extern pthread_mutex_t DAT_0007b7a8;  /* curses 互斥锁 (curses_active_locked 持有) */
extern int      DAT_0007701c;         /* curses 布局行号 B (初值 9) */
extern int      DAT_00077020;         /* curses 布局行号 (statuswin 高度, 初值随终端调整) */
extern int      DAT_00077024;         /* curses 布局行号 C (初值 10) */
extern int      DAT_00077028;         /* curses 布局行号 A (初值 8, 设备列表起始行偏移) */
extern int      DAT_0007702c;         /* 历史最大设备数高水位 */
extern uint8_t  DAT_0007b940;         /* compact 模式开关 (curses 'm') */
extern uint8_t  DAT_00077318;         /* widescreen 开关 (curses 'i') */
extern uint8_t  DAT_00077154;         /* 统计行模式翻转位 (hashmeter 每轮与 DAT_00077155 异或翻转) */
extern char     DAT_00077260[];       /* 启动时刻串 (0x28 字节, main 初始化) */
extern char     DAT_00077160[];       /* hashmeter 状态行 "(%ds):..(avg):..h/s" (256 字节, job_p3.c) */
extern struct timeval DAT_00077310;   /* watchdog 启动时刻 (线程入口 cgtime 一次) */
extern double   DAT_0007b5e8;         /* 累计接受份额计数 (状态行 A:%.0f 实参, 8 字节) */
extern double   DAT_0007b670;         /* 累计拒绝份额计数 (状态行 R:%.0f 实参, 8 字节) */
extern int      DAT_0007b8a4;         /* 硬件错误计数 (HW) */
extern uint32_t DAT_0007b100;         /* 累计 diff1 份额 64 位计数低字 (WU 分子) */
extern uint32_t DAT_0007b104;         /* 累计 diff1 份额 64 位计数高字 */
extern int      DAT_000755f8;         /* total_secs: 运行总秒数 (WU/速率归一化分母) */
extern uint32_t DAT_0007bc80;         /* 累计提交份额 64 位计数低字 (SS:%lld) */
extern uint32_t DAT_0007bc84;         /* 累计提交份额 64 位计数高字 */
extern int      DAT_0007b4cc;         /* longpoll 取得工作计数 (LW) */
extern int      DAT_0007b43c;         /* 取工作失败计数 (GF) */
extern int      DAT_0007ae68;         /* 远端失败计数 (RF) */
extern int      DAT_0007b198;         /* total_pools: 已配置矿池数 */
extern int      DAT_0007b720;         /* total_devices: 设备总数 */
extern void   **DAT_0007b5e0;        /* mining_thr: thr_info* 数组基址 */
extern pthread_rwlock_t DAT_0007b650; /* mining_thr 数组锁 */
extern int      DAT_0007bb6c;         /* mining_threads: thr_info 数组元素数 */
extern int      DAT_00075df8;         /* 设备序号显示宽 (adj_width 输出) */
extern int      DAT_00075dfc;         /* WU 显示宽 (adj_width 输出) */
extern int      DAT_00075e00;         /* 接受份额显示宽 (adj_fwidth 输出) */
extern int      DAT_00075e04;         /* 拒绝份额显示宽 (adj_fwidth 输出) */
extern int      DAT_00075e08;         /* 硬件错误显示宽 (adj_width 输出) */
extern uint8_t  DAT_00075e0c;         /* 允许自动重启设备标志 (SICK/DEAD 后 reinit, 语义待定) */
extern uint8_t  DAT_0007b194;         /* 计划停机暂停标志 (0=运行 1=暂停) */
extern int      DAT_0007b5fc;         /* sched_stop.hour: 停止小时 */
extern int      DAT_0007b5f8;         /* sched_stop.min : 停止分钟 */
extern int      DAT_0007b5f0;         /* sched_stop.enable: 停机调度项启用标志 */
extern int      DAT_0007b880;         /* sched_start.hour: 启动小时 */
extern int      DAT_0007b87c;         /* sched_start.min : 启动分钟 */
extern int      DAT_0007b874;         /* sched_start.enable: 启动调度项启用标志 */
extern char     DAT_00075dc4[];       /* best share 格式化串 (8 字节, 状态行 "Best share") */

/* parse_notify_equihash */
extern char     DAT_00061ca0[];       /* rodata 字符串常量, ELF .rodata@0x61ca0 实测内容 "id"
                                        * (name_map 旧标注 "method" 有误; get_version/ping 响应用) */
extern uint8_t  DAT_00078ca8;         /* 首条 mining.notify 已做系统时钟对齐标志 */
extern uint8_t  DAT_0007b380;         /* extranonce/notify 调试打印开关 (pool_p1.c) */
extern uint32_t DAT_0007b648;         /* 累计收到工作基元计数 64 位低字 (与 DAT_0007b64c 配对) */
extern uint32_t DAT_0007b64c;         /* 累计收到工作基元计数 64 位高字 */
extern uint8_t  DAT_0007b8ac;         /* "当前池已收到新 notify" 标志 (pool == current_pool 时置 1) */

/* 反编译统一日志门控: 调用点形态 (A||B||level>N) ↔ applog(prio=N+1);
 * 三条件全关时调用点直接跳过字符串构造与 applog。 */
#define LOG_GATE(prio) \
	(DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || DAT_000766c4 >= (prio))

/* ==================== 区块数据库结构 (布局由全部调用点交叉锁定) ==================== */

/* 哈希表控制块 (0x2C = 11 字, malloc(0x2c)) — 块节点 +0x44 字段指向它 */
struct ht_ctrl {
	char    *buckets;        /* +0x00 桶数组 (每桶 12 字节; 建表前 NULL) */
	uint32_t n_buckets;      /* +0x04 桶数 (2 的幂, 建表 0x20; 桶索引掩码 = n_buckets-1) */
	uint32_t shift;          /* +0x08 log2(n_buckets) (建表 5) */
	uint32_t count;          /* +0x0c 库内块总数 (块库超限判定 > 6 / 删除时 -1) */
	char    *head_link;      /* +0x10 年代链头: 最新块的链式指针 */
	uint32_t link_off;       /* +0x14 链式指针偏移 = 0x44 (节点形式↔链式指针换算, 运行期读) */
	uint32_t depth_div;      /* +0x18 重建时桶深比除数 (rehash 时按 count 重估) */
	uint32_t overflow;       /* +0x1c 重建过载计数 (超深桶计数) */
	uint32_t overflow_run;   /* +0x20 连续过载轮次 (≥2 → 冻结) */
	uint32_t degraded;       /* +0x24 过载冻结标志 (置 1 后不再重建) */
	uint32_t seed;           /* +0x28 哈希种子 0xa0111fe1 */
};

/* 桶项 (12 字节): 头链式指针 + 桶内节点数 + 深度比 */
struct ht_bucket {
	char    *head_link;      /* +0x00 桶链头 (链式指针, NULL = 空桶) */
	uint32_t count;          /* +0x04 桶内节点数 */
	uint32_t depth_ratio;    /* +0x08 深度比 (重建时 count/depth_div; 平时 0) */
};

/* 块数据库节点 (0x68 字节, calloc(0x68,1)) */
struct blk_db_entry {
	char             hash_str[0x44];       /* +0x00 块哈希十六进制串 (strcpy, NUL 结尾) */
	struct ht_ctrl  *ht;                  /* +0x44 哈希表控制块指针 (链式指针的基址 = 本字段地址) */
	struct blk_db_entry *prev;            /* +0x48 年代链 prev (更老块, 链首 prev = NULL) */
	struct blk_db_entry *next;            /* +0x4c 年代链 next (更新块, 链尾 next = NULL) */
	char            *bucket_prev_link;    /* +0x50 桶链 prev (链式指针形式) */
	char            *bucket_next_link;    /* +0x54 桶链 next (链式指针形式; 链尾 NULL) */
	char            *hash_self;           /* +0x58 = &hash_str[0] (查找 memcmp 用) */
	size_t           hash_len;            /* +0x5c 哈希串长度 */
	uint32_t         hash_val;            /* +0x60 lookup2 哈希值 (桶索引源) */
	uint32_t         insert_seq;          /* +0x64 插入序号 (= DAT_0007bb1c 旧值; 日志称
                                           * "Deleted block %d", 归并排序按其升序排) */
};

/* 链式指针 = 节点 +0x44 字段地址; "空链" 哨兵 = 常量 0x44 (节点 0 的 +0x44)。 */
#define BLK_LINK_OFF   0x44
#define BLK_LINK(node) ((char *)(node) + BLK_LINK_OFF)
#define BLK_NODE(link) ((struct blk_db_entry *)(void *)((char *)(link) - BLK_LINK_OFF))

/* ==================== 1. test_work_current (原 FUN_0001db0c) ==================== */

// === test_work_current === 原 FUN_0001db0c @ 0x0001db0c
// 来源: reversed/cgminer/20_job_mining/FUN_0001db0c.c | 分类: 20_job_mining
// 语义: 校验 work 是否对应当前区块。先在区块数据库 (自定义 lookup2 哈希表, 上限 6 条) 中
//       查找 work 的区块哈希; 未命中 → 新块入库 (超限先按插入序号自底向上归并排序后删除
//       最老块) + 更新网络难度显示 + set_curblock 写全局块信息 + restart_threads;
//       命中 → 对比矿池/全局哈希字判定 up-to-date/stale; work 带 longpoll 标志且过期时
//       触发 restart_threads。
// 返回: 1 = work 可继续测试 (视为当前), 0 = stale。
// work 字段偏移 (机器码锁定): +0x04..+0x3b 8×u32 区块哈希字; +0x48 4B 难度压缩值;
//   +0x6d4 pool*; +0x6e8 longpoll 字节; +0x6ea mandatory(挖中区块) 字节 (update_work_stats
//   置 1); +0x6ec stratum 字节 (gen_stratum_work 置 1); +0x714 gbt 字节; +0x720 block_nr。
int test_work_current(void *work)
{
	uint8_t *wp = (uint8_t *)work;
	uint8_t *pool = *(uint8_t **)(wp + 0x6d4);
	int ret;
	char hashhex[68];
	uint32_t cleaned[8];
	int i;
	uint32_t ha, hb, hc, w1, w2, w3, tmp, rem;
	const uint8_t *hp;
	struct blk_db_entry *base, *node, *new_blk, *anchor, *removed, *next;
	struct ht_ctrl *ctrl;
	struct ht_bucket *bucket;
	char *entry_link, *cur, *ahead, *ahead_next, *pos, *new_head, *new_tail;
	struct blk_db_entry *prev_node;
	uint32_t width, skip, skip_saved, ahead_left, rounds;
	int deleted_seq;
	uint64_t diff_shift;
	double net_diff, net_diff_prev, diff_mant;
	struct tm *tm;
	time_t block_time;
	int total_blocks;

	if (*(uint8_t *)(wp + 0x6ea) != 0)      /* mandatory: 该 work 已挖中区块, 恒视为当前 */
		return 1;

	/* —— 取 work 的 8 个哈希字 (字序按机器码) 并转十六进制 —— */
	cleaned[0] = *(uint32_t *)(wp + 0x20);
	cleaned[1] = *(uint32_t *)(wp + 0x1c);
	cleaned[2] = *(uint32_t *)(wp + 0x18);
	cleaned[3] = *(uint32_t *)(wp + 0x14);
	cleaned[4] = *(uint32_t *)(wp + 0x0c);
	cleaned[5] = *(uint32_t *)(wp + 0x10);
	cleaned[6] = *(uint32_t *)(wp + 0x04);
	cleaned[7] = *(uint32_t *)(wp + 0x08);
	__bin2hex(hashhex, (uint8_t *)cleaned, 0x20);

	/* —— 持读锁在块库中查找 (原 block_exists 内联) —— */
	if (pthread_rwlock_rdlock(&DAT_00077294) != 0)
		rd_lock_error("block_exists", 0x14e3);

	node = NULL;
	base = (struct blk_db_entry *)DAT_000772b4;
	if (base != NULL) {
		rem = strlen(hashhex);
		/* Jenkins lookup2 风格混合 (与 hash.c add_cgpu 同款: 移位 13,8,13,12,16,5,3,10,15;
		 * 初值 a=b=0x9e3779b9, c=0xfeedbeef, 长度在尾部并入 c)。 */
		ha = 0x9e3779b9;
		hb = 0x9e3779b9;
		hc = 0xfeedbeef;
		hp = (const uint8_t *)hashhex;
		for (; rem > 0xb; rem -= 0xc) {
			w1 = (uint32_t)hp[0] | (uint32_t)hp[1] << 8 | (uint32_t)hp[2] << 16 | (uint32_t)hp[3] << 24;
			w2 = (uint32_t)hp[4] | (uint32_t)hp[5] << 8 | (uint32_t)hp[6] << 16 | (uint32_t)hp[7] << 24;
			w3 = (uint32_t)hp[8] | (uint32_t)hp[9] << 8 | (uint32_t)hp[10] << 16 | (uint32_t)hp[11] << 24;
			hc += w3;
			tmp = w2 + hb;
			ha = (w1 - tmp - hc) + ha ^ hc >> 13;
			w3 = (tmp - hc) - ha ^ ha << 8;
			hb = (hc - ha) - w3 ^ w3 >> 13;
			ha = (ha - w3) - hb ^ hb >> 12;
			w3 = (w3 - hb) - ha ^ ha << 16;
			hc = (hb - ha) - w3 ^ w3 >> 5;
			ha = (ha - w3) - hc ^ hc >> 3;
			hb = (w3 - hc) - ha ^ ha << 10;
			hc = (hc - ha) - hb ^ hb >> 15;
			hp += 0xc;
		}
		hc += rem;
		/* 尾部余量 (switch 贯穿, 无 break) */
		switch (rem) {
		case 11:
			hc += (uint32_t)hp[10] << 24;
		case 10:
			hc += (uint32_t)hp[9] << 16;
		case 9:
			hc += (uint32_t)hp[8] << 8;
		case 8:
			hb += (uint32_t)hp[7] << 24;
		case 7:
			hb += (uint32_t)hp[6] << 16;
		case 6:
			hb += (uint32_t)hp[5] << 8;
		case 5:
			hb += hp[4];
		case 4:
			ha += (uint32_t)hp[3] << 24;
		case 3:
			ha += (uint32_t)hp[2] << 16;
		case 2:
			ha += (uint32_t)hp[1] << 8;
		case 1:
			ha += hp[0];
		default:
			break;
		}
		/* 终混 (同款 9 步) → 桶索引 = C 态 & (n_buckets-1) */
		ha = (ha - hb) - hc ^ hc >> 13;
		w3 = (hb - hc) - ha ^ ha << 8;
		hc = (hc - ha) - w3 ^ w3 >> 13;
		hb = (ha - w3) - hc ^ hc >> 12;
		ha = (w3 - hc) - hb ^ hb << 16;
		hc = (hc - hb) - ha ^ ha >> 5;
		hb = (hb - ha) - hc ^ hc >> 3;
		ha = (ha - hc) - hb ^ hb << 10;
		hc = (hc - hb) - ha ^ ha >> 15;
		ctrl = base->ht;
		entry_link = ((struct ht_bucket *)(ctrl->buckets +
		             (hc & (ctrl->n_buckets - 1U)) * sizeof(struct ht_bucket)))->head_link;
		if (entry_link == NULL)
			goto lookup_check;               /* 空桶 (原 LAB_0001de10 直入) */
		/* 沿桶链逐节点比较哈希串 */
		do {
			node = BLK_NODE(entry_link);
lookup_check:                                /* 原 LAB_0001de10 */
			if (node == NULL ||
			    (rem == node->hash_len &&
			     memcmp(node->hash_self, hashhex, rem) == 0))
				break;                       /* 命中 (node != NULL) / 空链 (node == NULL) */
			entry_link = node->bucket_next_link;
			node = NULL;
		} while (entry_link != NULL);
	}
	if (pthread_rwlock_unlock(&DAT_00077294) != 0)
		rw_unlock_error("block_exists", 0x14e5);
	DAT_00075ed8();

	if (node == NULL)
		goto not_in_db;

	/* —— 已在库中: 对比矿池/全局哈希字, 判定 up-to-date/stale —— */
	if (memcmp((uint8_t *)pool + 0x238, cleaned, 0x20) == 0) {   /* pool+0x238: 矿池最近 8 哈希字 */
		ret = 1;
	} else if (memcmp(cleaned, DAT_000772c0, 0x20) == 0) {       /* 与全局当前块哈希字一致 */
		if (DAT_00078e09 != '\0' && LOG_GATE(7)) {
			char msg[0x1000];
			snprintf(msg, 0x1000, "Pool %d now up to date", *(int *)pool);
			applog(7, msg, 0);
		}
		ret = 1;
		*(uint32_t *)(pool + 0x238) = cleaned[0];
		*(uint32_t *)(pool + 0x23c) = cleaned[1];
		*(uint32_t *)(pool + 0x240) = cleaned[2];
		*(uint32_t *)(pool + 0x244) = cleaned[3];
		*(uint32_t *)(pool + 0x248) = cleaned[4];
		*(uint32_t *)(pool + 0x24c) = cleaned[5];
		*(uint32_t *)(pool + 0x250) = cleaned[6];
		*(uint32_t *)(pool + 0x254) = cleaned[7];
	} else {
		ret = 0;
		if (DAT_00078e09 != '\0' && LOG_GATE(7)) {
			char msg[0x1000];
			snprintf(msg, 0x1000, "Stale data from pool %d", *(int *)pool);
			applog(7, msg, 0);
		}
	}
	if (*(uint8_t *)(wp + 0x6e8) != 0) {                          /* longpoll work */
		int strategy = DAT_00076efc;
		DAT_000770ac = DAT_000770ac + 1;
		*(uint32_t *)(wp + 0x720) = DAT_000770ac;
		/* 策略为 LoadBalance/Balance (同时用多池) 或本 work 即当前池 → 请求重启动 */
		if ((uint32_t)(strategy - 3) < 2 ||
		    ((void *)(*(uint8_t **)(wp + 0x6d4)) == current_pool())) {
			if (*(uint8_t *)(wp + 0x6ec) == 0) {
				if (DAT_00078e09 != '\0' && LOG_GATE(7)) {
					char msg[0x1000];
					snprintf(msg, 0x1000, "%sLONGPOLL from pool %d requested work restart",
					         (*(uint8_t *)(wp + 0x714) != 0) ? "GBT " : "",
					         *(int *)(*(uint8_t **)(wp + 0x6d4)));
					applog(7, msg, 0);
				}
			} else if (DAT_00078e09 != '\0' && LOG_GATE(7)) {
				char msg[0x1000];
				snprintf(msg, 0x1000, "Stratum from pool %d requested work restart",
				         *(int *)pool);
				applog(7, msg, 0);
			}
			restart_threads();
		}
	}
	*(uint8_t *)(wp + 0x6e8) = 0;
	return ret;

not_in_db:
	/* ================================================================
	 * 新块入库路径
	 * ================================================================ */
	new_blk = calloc(0x68, 1);
	if (new_blk == NULL) {
		applog(3, "test_work_current OOM", 1);
		quit(1);
	}
	strcpy(new_blk->hash_str, hashhex);
	new_blk->insert_seq = DAT_0007bb1c;       /* 插入序号 = 旧计数 */
	DAT_0007bb1c = DAT_0007bb1c + 1;

	if (pthread_rwlock_wrlock(&DAT_00077294) != 0)
		wr_lock_error("test_work_current", 0x1526);

	anchor = (struct blk_db_entry *)DAT_000772b4;
	if (anchor == NULL) {
		/* 首块: 建库 (原 LAB_0001e2c8) */
		new_blk->prev = NULL;
		DAT_000772b4 = new_blk;
		ctrl = malloc(0x2c);
		new_blk->ht = ctrl;
		if (ctrl == NULL)
			goto oom_exit;                    /* 原 LAB_0001f11c: exit(-1) */
		ctrl->buckets = NULL;
		ctrl->count = 0;
		ctrl->head_link = NULL;
		ctrl->link_off = 0;
		ctrl->depth_div = 0;
		ctrl->overflow = 0;
		ctrl->overflow_run = 0;
		ctrl->degraded = 0;
		ctrl->seed = 0;
		ctrl->n_buckets = 0x20;
		ctrl->head_link = BLK_LINK(new_blk);
		ctrl->shift = 5;
		ctrl->link_off = BLK_LINK_OFF;
		ctrl->buckets = malloc(0x180);        /* 32 桶 × 12 字节 */
		if (ctrl->buckets == NULL)
			goto oom_exit;
		memset(ctrl->buckets, 0, 0x180);
		ctrl->seed = 0xa0111fe1;
		anchor = new_blk;
		deleted_seq = 0;
		goto hash_insert;                     /* 首块路径不经 LAB_0001e518 (原码直落哈希段) */
	}

	ctrl = anchor->ht;
	if (ctrl->count > 6) {
		/* —— 库超限 (>6): 自底向上归并排序按 insert_seq 升序重建年代链, 删除序号最小
		 *    (最老) 的块。原 LAB_0001e07c/0001e08c/0001e0ec/0001e0f4/0001e154/0001e160/
		 *    0001e390 归并循环, Ghidra 拼接的 do-while(true) 结构按原样保留。 —— */
		width = 1;
		cur = BLK_LINK(anchor);               /* 链首 = 锚节点 (链式指针) */
		new_head = NULL;
		new_tail = NULL;
		rounds = 0;
		prev_node = NULL;

restart_pass:                                 /* 原 LAB_0001e07c: 新宽度首轮 */
		new_head = NULL;
		new_tail = NULL;
		rounds = 0;
next_pass:                                    /* 原 LAB_0001e08c: 归并一对相邻游程 */
		rounds++;
		/* 从 cur 起走 width 个节点定位下一游程边界 ahead */
		if (width == 0) {
			skip = 0;
			ahead = cur;
		} else if (*(char **)(cur + 8) == NULL) {          /* cur->next == NULL */
			skip = 1;
			ahead = NULL;
		} else {
			skip = 1;
			ahead = *(char **)(cur + 8) + ctrl->link_off;
			while (skip != width) {
				ahead = *(char **)(ahead + 8);             /* 裸 next (节点形式) */
				skip++;
				if (ahead == NULL)
					break;
				ahead += ctrl->link_off;
				if (ahead == NULL)
					break;
			}
		}
		ahead_left = width;
merge_loop:                                   /* 原 LAB_0001e0ec (do-while(true)) */
		do {
			pos = cur;
			skip_saved = skip;
			if (skip == 0)
				goto pass_end;
step:                                         /* 原 LAB_0001e0f4 */
			cur = pos;
			if (ahead_left == 0 || ahead == NULL) {
				/* ahead 游程已耗尽: pos 一侧继续 */
				if (pos != NULL && *(char **)(pos + 8) != NULL)
					cur = *(char **)(pos + 8) + ctrl->link_off;
			} else if (0 < *(int *)(pos - ctrl->link_off + 100) -
			                *(int *)(ahead - ctrl->link_off + 100)) {
				/* pos->insert_seq > ahead->insert_seq: ahead (较小) 先入新链, pos 留待
				 * 下轮比较 */
				ahead_next = *(char **)(ahead + 8);        /* 裸 next */
				skip = skip_saved;
				if (ahead_next == NULL)
					goto take_ahead;
				ahead_next += ctrl->link_off;
				goto take_ahead;
			} else {
				if (pos != NULL && *(char **)(pos + 8) != NULL)
					cur = *(char **)(pos + 8) + ctrl->link_off;
			}
			skip = skip_saved - 1;
			ahead_next = ahead;
			if (new_tail == NULL) {
				/* 新链尚空: pos 成为首节点 */
				new_head = pos;
				new_tail = pos;
				if (pos != NULL) {
					prev_node = NULL;
					goto link_node;
				}
				continue;                     /* 原 goto LAB_0001e0ec */
			}
			while ((ahead = ahead_next, pos != NULL)) {
				BLK_NODE(new_tail)->next = BLK_NODE(pos);   /* 新尾->next = pos */
				prev_node = BLK_NODE(new_tail);
				new_tail = pos;
link_node:                                    /* 原 LAB_0001e154 (while(true)) */
				while (1) {
					BLK_NODE(new_tail)->prev = prev_node;
					pos = cur;
					skip_saved = skip;
					if (skip != 0)
						goto step;
pass_end:                                     /* 原 LAB_0001e160 */
					if (ahead == NULL || ahead_left == 0) {
						cur = ahead;
						if (ahead != NULL)
							goto next_pass;   /* 尚有未归并段: 同宽度下一对 */
						/* 整条链已并完 */
						if (new_tail != NULL)
							BLK_NODE(new_tail)->next = NULL;   /* 原 4 字节清零 */
						if (rounds > 1) {
							/* 本宽度归并了 >1 对 → 翻倍宽度重来 */
							width <<= 1;
							cur = new_head;
							if (new_head != NULL)
								goto restart_pass;
							new_tail = new_head;   /* 原 (new_head == 0) 死路径 */
						}
						goto purge_done;
					}
					/* skip==0 但 ahead 侧尚有余: 逐节点排空 ahead 游程
					 * (原 pbVar26 = pbVar11: skip==0 时作 NULL 哨兵) */
					ahead_next = (char *)(uintptr_t)skip;
					if (*(char **)(ahead + 8) != NULL)
						ahead_next = *(char **)(ahead + 8) + ctrl->link_off;
take_ahead:                                   /* 原 LAB_0001e390 */
					pos = ahead;
					ahead_left--;
					if (new_tail != NULL)
						break;            /* 回追加循环 */
					prev_node = NULL;
					new_head = pos;
					ahead = ahead_next;
					new_tail = pos;
				}
			}
			/* 追加循环退出 (pos == NULL): 终止新链 */
			BLK_NODE(new_tail)->next = NULL;
			new_tail = pos;
		} while (1);

purge_done:
		/* 归并结果: new_head = 最老 (最小 seq), new_tail = 最新; ctrl->head 指向新链尾 */
		ctrl->head_link = new_tail;
		removed = BLK_NODE(new_head);
		prev_node = removed->prev;
		deleted_seq = removed->insert_seq;
		DAT_000772b4 = removed;               /* 锚暂指向被删块 (其 +0x44 同指控制块) */
		if (removed->prev == NULL && removed->next == NULL) {
			/* 全库仅此一块 (ctrl->count > 6 时不可达, 保留原分支) */
			free(ctrl->buckets);
			free(ctrl);
			anchor = NULL;
			DAT_000772b4 = NULL;
		} else {
			struct ht_ctrl *rctrl = removed->ht;
			if (BLK_NODE(rctrl->head_link) == removed) {
				/* 被删块即链头 (不可达, 保留原分支) */
				rctrl->head_link = BLK_LINK(prev_node);
			}
			if (removed->prev == NULL) {
				next = removed->next;
				anchor = next;
				DAT_000772b4 = next;          /* 锚 = 次老块 (新链首) */
			} else {
				BLK_NODE(removed->prev)->next = removed->next;
				next = removed->next;
				anchor = removed;             /* 原码锚指向将释放块 (释放后即用后读, 按机器码保留) */
			}
			if (next != NULL)
				BLK_NODE(next)->prev = removed->prev;
			/* 从桶链摘除被删块 */
			bucket = (struct ht_bucket *)(anchor->ht->buckets +
				((anchor->ht->n_buckets - 1U) & removed->hash_val) * sizeof(struct ht_bucket));
			bucket->count--;
			if (bucket->head_link == BLK_LINK(removed))
				bucket->head_link = removed->bucket_next_link;
			if (removed->bucket_prev_link != NULL)
				BLK_NODE(removed->bucket_prev_link)->bucket_next_link = removed->bucket_next_link;
			if (removed->bucket_next_link != NULL)
				BLK_NODE(removed->bucket_next_link)->bucket_prev_link = removed->bucket_prev_link;
			anchor->ht->count--;
		}
		free(removed);
		/* 被删块腾出后, 重建新块节点信息并回链 (或整库重建) */
		new_blk->hash_self = (char *)new_blk;
		new_blk->next = NULL;
		rem = strlen(new_blk->hash_str);
		new_blk->hash_len = rem;
		if (anchor != NULL)
			goto link_new_block;
		goto init_db;
	}

	/* 未超限: 直接挂链尾 */
	new_blk->hash_self = (char *)new_blk;
	new_blk->next = NULL;
	rem = strlen(new_blk->hash_str);
	deleted_seq = 0;
	new_blk->hash_len = rem;

link_new_block:                               /* 原 LAB_0001e518 */
	BLK_NODE(anchor->ht->head_link)->next = new_blk;              /* 旧头(最新块)->next = 新块 */
	new_blk->prev = BLK_NODE(anchor->ht->head_link);
	anchor->ht->head_link = BLK_LINK(new_blk);                    /* 头 = 新块 */
	ctrl = anchor->ht;
	goto hash_insert;

init_db:                                      /* 原 LAB_0001e2c8 (整库重建, 见上方首块路径复用) */
	new_blk->prev = NULL;
	DAT_000772b4 = new_blk;
	ctrl = malloc(0x2c);
	new_blk->ht = ctrl;
	if (ctrl == NULL)
		goto oom_exit;
	ctrl->buckets = NULL;
	ctrl->count = 0;
	ctrl->head_link = NULL;
	ctrl->link_off = 0;
	ctrl->depth_div = 0;
	ctrl->overflow = 0;
	ctrl->overflow_run = 0;
	ctrl->degraded = 0;
	ctrl->seed = 0;
	ctrl->n_buckets = 0x20;
	ctrl->head_link = BLK_LINK(new_blk);
	ctrl->shift = 5;
	ctrl->link_off = BLK_LINK_OFF;
	ctrl->buckets = malloc(0x180);
	if (ctrl->buckets == NULL)
		goto oom_exit;
	memset(ctrl->buckets, 0, 0x180);
	ctrl->seed = 0xa0111fe1;
	anchor = new_blk;

hash_insert:
	/* —— 为新块计算 lookup2 哈希并插入桶链 (插入路径哈希, 与查找路径同款混合) —— */
	ha = 0x9e3779b9;
	hb = 0x9e3779b9;
	hc = 0xfeedbeef;
	rem = strlen(new_blk->hash_str);
	hp = (const uint8_t *)new_blk->hash_str;
	if (rem > 0xb) {
		do {
			w1 = (uint32_t)hp[0] | (uint32_t)hp[1] << 8 | (uint32_t)hp[2] << 16 | (uint32_t)hp[3] << 24;
			w2 = (uint32_t)hp[4] | (uint32_t)hp[5] << 8 | (uint32_t)hp[6] << 16 | (uint32_t)hp[7] << 24;
			w3 = (uint32_t)hp[8] | (uint32_t)hp[9] << 8 | (uint32_t)hp[10] << 16 | (uint32_t)hp[11] << 24;
			hc += w3;
			tmp = w2 + hb;
			ha = (w1 - tmp - hc) + ha ^ hc >> 13;
			w3 = (tmp - hc) - ha ^ ha << 8;
			hb = (hc - ha) - w3 ^ w3 >> 13;
			ha = (ha - w3) - hb ^ hb >> 12;
			w3 = (w3 - hb) - ha ^ ha << 16;
			hc = (hb - ha) - w3 ^ w3 >> 5;
			ha = (ha - w3) - hc ^ hc >> 3;
			hb = (w3 - hc) - ha ^ ha << 10;
			hc = (hc - ha) - hb ^ hb >> 15;
			new_blk->hash_val = hc;           /* 逐块写入中间 C 态 (原码), 最终被尾部覆盖 */
			hp += 0xc;
			rem -= 0xc;
		} while (rem > 0xb);
		rem = strlen(new_blk->hash_str);      /* 原码重取长度 (值不变) */
	}
	hc += rem;
	new_blk->hash_val = hc;
	switch (rem) {
	case 11:
		hc += (uint32_t)hp[10] << 24;
		new_blk->hash_val = hc;
	case 10:
		hc += (uint32_t)hp[9] << 16;
		new_blk->hash_val = hc;
	case 9:
		hc += (uint32_t)hp[8] << 8;
		new_blk->hash_val = hc;
	case 8:
		hb += (uint32_t)hp[7] << 24;
	case 7:
		hb += (uint32_t)hp[6] << 16;
	case 6:
		hb += (uint32_t)hp[5] << 8;
	case 5:
		hb += hp[4];
	case 4:
		ha += (uint32_t)hp[3] << 24;
	case 3:
		ha += (uint32_t)hp[2] << 16;
	case 2:
		ha += (uint32_t)hp[1] << 8;
	case 1:
		ha += hp[0];
	default:
		break;
	}
	/* 终混 → hash_val = C 态 */
	ha = (ha - hb) - hc ^ hc >> 13;
	w3 = (hb - hc) - ha ^ ha << 8;
	hc = (hc - ha) - w3 ^ w3 >> 13;
	hb = (ha - w3) - hc ^ hc >> 12;
	ha = (w3 - hc) - hb ^ hb << 16;
	hc = (hc - hb) - ha ^ ha >> 5;
	hb = (hb - ha) - hc ^ hc >> 3;
	ha = (ha - hc) - hb ^ hb << 10;
	hc = (hc - hb) - ha ^ ha >> 15;
	new_blk->hash_val = hc;
	/* 头插入桶链 */
	bucket = (struct ht_bucket *)(ctrl->buckets + (hc & (ctrl->n_buckets - 1U)) * sizeof(struct ht_bucket));
	bucket->count++;
	new_blk->bucket_prev_link = NULL;
	new_blk->bucket_next_link = bucket->head_link;
	if (bucket->head_link != NULL)
		BLK_NODE(bucket->head_link)->bucket_prev_link = BLK_LINK(new_blk);
	bucket->head_link = BLK_LINK(new_blk);
	/* 桶深超阈值 (深度比+1)*10 ≤ 桶计数 且未冻结 → 桶表翻倍重建 */
	if ((uint32_t)((bucket->depth_ratio + 1) * 10) <= bucket->count && ctrl->degraded != 1) {
		struct ht_bucket *new_table;
		uint32_t bi;
		new_table = malloc(ctrl->n_buckets * 24);       /* (n_buckets*2)*12 字节 */
		if (new_table == NULL)
			goto oom_exit;
		memset(new_table, 0, ctrl->n_buckets * 24);
		ctrl->depth_div = ((ctrl->n_buckets * 2 - 1U & ctrl->count) != 0) +
		                  (ctrl->count >> (ctrl->shift + 1U));
		/* 逐旧桶遍历, 全部重挂到新桶 */
		for (bi = 0; bi < ctrl->n_buckets; bi++) {
			entry_link = ((struct ht_bucket *)(ctrl->buckets + bi * sizeof(struct ht_bucket)))->head_link;
			if (entry_link == NULL)
				continue;
			while (1) {
				char *next_link = BLK_NODE(entry_link)->bucket_next_link;
				uint32_t idx = (ctrl->n_buckets * 2 - 1U) & BLK_NODE(entry_link)->hash_val;
				struct ht_bucket *slot = (struct ht_bucket *)(new_table + idx * sizeof(struct ht_bucket));
				slot->count++;
				if (ctrl->depth_div < slot->count) {
					ctrl->overflow++;
					slot->depth_ratio = slot->count / ctrl->depth_div;   /* 原 __udivsi3 */
				}
				BLK_NODE(entry_link)->bucket_prev_link = NULL;
				BLK_NODE(entry_link)->bucket_next_link = slot->head_link;
				if (slot->head_link != NULL)
					BLK_NODE(slot->head_link)->bucket_prev_link = entry_link;
				slot->head_link = entry_link;
				if (next_link == NULL)
					break;
				entry_link = next_link;
			}
		}
		free(ctrl->buckets);
		ctrl->n_buckets <<= 1;
		ctrl->shift++;
		ctrl->buckets = (char *)new_table;
		if (ctrl->count >> 1 < ctrl->overflow)
			ctrl->overflow_run++;
		else
			ctrl->overflow_run = 0;
		if (ctrl->overflow_run > 1)
			ctrl->degraded = 1;           /* 连续过载 → 冻结重建 */
	}

	/* —— 网络难度: 0xffff << ((0x1d - 难度指数字节)*8) / 尾数 (原 VectorShiftLeft/VectorSignedToFloat) —— */
	diff_shift = (uint64_t)0xffff << ((0x1d - (uint32_t)*(uint8_t *)(wp + 0x48)) * 8);
	net_diff = (double)diff_shift;                    /* 原 __floatundidf */
	diff_mant = (double)(float)(int32_t)((*(uint32_t *)(wp + 0x48) >> 8 & 0xff) << 0x10 |
	                                     (*(uint32_t *)(wp + 0x48) >> 0x10 & 0xff) << 8 |
	                                     (*(uint32_t *)(wp + 0x48) >> 0x18));
	net_diff = net_diff / diff_mant;
	net_diff_prev = DAT_00075de0;
	if (net_diff != net_diff_prev) {
		suffix_string((uint64_t)net_diff, DAT_000772b8, 8, 0);   /* 原 __fixunsdfdi 后 suffix_string */
		DAT_00075de0 = net_diff;
		if (DAT_00078e09 != '\0' && LOG_GATE(7)) {
			char msg[0x1000];
			snprintf(msg, 0x1000, "Network diff set to %s", DAT_000772b8);
			applog(7, msg, 0);
		}
	}
	if (pthread_rwlock_unlock(&DAT_00077294) != 0)
		rw_unlock_error("test_work_current", 0x1536);
	DAT_00075ed8();
	if (deleted_seq != 0 && DAT_00078e09 != '\0' && LOG_GATE(7)) {
		char msg[0x1000];
		snprintf(msg, 0x1000, "Deleted block %d from database", deleted_seq);
		applog(7, msg, 0);
	}

	/* —— set_curblock: 写全局当前块信息 —— */
	if (pthread_mutex_lock(&DAT_0007b9d8) != 0)
		mutex_lock_fail_cgminer("set_curblock", 0x14cc);
	if (pthread_rwlock_wrlock(&DAT_0007b9f0) != 0)
		wr_lock_error("set_curblock", 0x14cc);
	cgtime(&DAT_0007b070);
	strcpy(DAT_0007bb28, hashhex);
	block_time = DAT_0007b070.tv_sec;
	DAT_000772c0[0] = cleaned[0];
	DAT_000772c0[1] = cleaned[1];
	DAT_000772c0[2] = cleaned[2];
	DAT_000772c0[3] = cleaned[3];
	DAT_000772c0[4] = cleaned[4];
	DAT_000772c0[5] = cleaned[5];
	DAT_000772c0[6] = cleaned[6];
	DAT_000772c0[7] = cleaned[7];
	tm = localtime(&block_time);
	snprintf(DAT_000772e0, 0x20, "[%02d:%02d:%02d]", tm->tm_hour, tm->tm_min, tm->tm_sec);
	if (pthread_rwlock_unlock(&DAT_0007b9f0) != 0)
		rw_unlock_error("set_curblock", 0x14d1);
	if (pthread_mutex_unlock(&DAT_0007b9d8) != 0)
		mutex_unlock_fail_cgminer("set_curblock", 0x14d1);
	DAT_00075ed8();
	/* 去前导零截取 8 字符作显示串 */
	i = 0;
	do {
		if (DAT_0007bb28[i] != '0')
			break;
		i++;
	} while (i != 0x39);
	strncpy(DAT_00077300, DAT_0007bb28 + i, 8);
	DAT_00077308 = 0;
	if (DAT_00078e09 != '\0' && LOG_GATE(7)) {
		char msg[0x1000];
		snprintf(msg, 0x1000, "New block: %s... diff %s", DAT_0007bb28, DAT_000772b8);
		applog(7, msg, 0);
	}

	/* —— 矿池哈希字同步 + 新块通知 —— */
	total_blocks = DAT_0007bb1c;
	*(uint32_t *)(pool + 0x238) = cleaned[0];
	*(uint32_t *)(pool + 0x23c) = cleaned[1];
	*(uint32_t *)(pool + 0x240) = cleaned[2];
	*(uint32_t *)(pool + 0x244) = cleaned[3];
	*(uint32_t *)(pool + 0x248) = cleaned[4];
	*(uint32_t *)(pool + 0x24c) = cleaned[5];
	*(uint32_t *)(pool + 0x250) = cleaned[6];
	*(uint32_t *)(pool + 0x254) = cleaned[7];
	if (total_blocks != 1) {                  /* 非首个块 */
		DAT_000770ac = DAT_000770ac + 1;
		*(uint32_t *)(wp + 0x720) = DAT_000770ac;
		if (*(uint8_t *)(wp + 0x6e8) == 0) {  /* 非 longpoll */
			if (DAT_0007bc79 == 0) {          /* stratum 未启用 */
				if (*(uint8_t *)(pool + 0x39c) == 0 && DAT_00078e09 != '\0' && LOG_GATE(7)) {
					/* pool+0x39c 标志, 语义待定 (新块通知路径选择) */
					applog(7, "New block detected on network", 0);
				}
			} else if (*(uint8_t *)(pool + 0x39c) == 0 && DAT_00078e09 != '\0' && LOG_GATE(7)) {
				applog(7, "New block detected on network before pool notification", 0);
			}
		} else if (*(uint8_t *)(wp + 0x6ec) == 0) {
			if (DAT_00078e09 != '\0' && LOG_GATE(7)) {
				char msg[0x1000];
				snprintf(msg, 0x1000, "%sLONGPOLL from pool %d detected new block",
				         (*(uint8_t *)(wp + 0x714) != 0) ? "GBT " : "",
				         *(int *)(*(uint8_t **)(wp + 0x6d4)));
				applog(7, msg, 0);
			}
		} else if (DAT_00078e09 != '\0' && LOG_GATE(7)) {
			char msg[0x1000];
			snprintf(msg, 0x1000, "Stratum from pool %d detected new block", *(int *)pool);
			applog(7, msg, 0);
		}
		restart_threads();
		*(uint8_t *)(wp + 0x6e8) = 0;
		return 1;
	}
	ret = 0;
	goto done;

oom_exit:                                     /* 原 LAB_0001f11c */
	exit(-1);

done:
	*(uint8_t *)(wp + 0x6e8) = 0;
	return ret;
}

/* ==================== 2. watchdog_thread (原 FUN_00023344) ==================== */

// === watchdog_thread === 原 FUN_00023344 @ 0x00023344
// 来源: reversed/cgminer/20_job_mining/FUN_00023344.c | 分类: 20_job_mining
// 语义: 2 秒周期看门狗线程。每轮: discard_stale + hashmeter 全局汇总; curses 激活时加锁
//       刷新状态窗 (窗口尺寸自适应/版本与统计行/矿池行/块信息行/菜单行/每设备速率与
//       SICK/DEAD/OFF/REST 状态); 之后 cgtime 取当前时刻, 处理计划停机 (should_run()==0 →
//       置暂停/终止) 与计划开机 (恢复各线程信号量); 最后遍历设备做健康检查 (空闲 >60s
//       判 SICK, SICK 超 10 分钟判 DEAD, 恢复判 WELL, 允许时 reinit_device)。
// 形参: 反编译原型无线程实参 (r0 实参未使用); 恒不返回 (结尾 while(1), 终止走 quit(0))。
int watchdog_thread(void)
{
	char msg[0x1000];
	struct timeval now;
	char dev_repr[256];
	char name8[12];
	char statbuf[256];
	char hs_str1[16];
	char hs_str2[16];
	void *dev;
	void *thr;
	void *cp;
	int i, row, status, elapsed, rc;
	int width, hw_errs;
	double timebase, wu, rate1, rate2;
	uint64_t hashes, shares64, diff1_total;
	time_t tnow;

	pthread_setcanceltype(1 /*PTHREAD_CANCEL_ASYNCHRONOUS*/, NULL);
	rename_thread("Watchdog");
	set_lowprio();
	cgtime(&DAT_00077310);
	do {
		sleep(2);
		discard_stale();
		/* 原码 hashmeter(0xffffffff, <r1 残留>, 0, 0): thr_id<0 路径不使用第 2 参 */
		hashmeter(-1, 0, 0, 0);

		if (curses_active_locked() != 0) {
			/* —— 窗口尺寸自适应 (原 check_winsizes 内联) —— */
			if (DAT_0007bc90 != NULL) {
				int maxy = (int)*(short *)((char *)DAT_0007bc90 + 4);       /* _maxy */
				int cols = (int)*(short *)((char *)DAT_0007bc90 + 6) + 1;   /* _maxx+1 */
				int rows = maxy + 1;
				bool do_resize;
				/* 原 GCC 分支比较 (SBORROW 渲染): cols≤79 时条件 = cols<79;
				 * cols>79 时条件 = rows<24 (终端过小 → 压缩 statuswin 高度) */
				if (cols > 79)
					do_resize = rows < 24;
				else
					do_resize = cols < 79;
				if (do_resize) {
					if (DAT_00077020 + 2 < rows && DAT_00077020 < DAT_0007701c) {
						DAT_00077020 = DAT_0007701c;
						if (maxy <= DAT_0007701c)
							DAT_00077020 = maxy - 1;
						DAT_00077024 = DAT_00077020 + 1;
						mvwin(DAT_0007b430, DAT_00077024, 0);
						wresize(DAT_0007b7c0, DAT_00077020, cols);
					}
					int lrows, lcols;
					if (DAT_0007b430 == NULL) {
						lrows = -1;
						lcols = -1;
					} else {
						lrows = (int)*(short *)((char *)DAT_0007b430 + 4) + 1;
						lcols = (int)*(short *)((char *)DAT_0007b430 + 6) + 1;
					}
					if (cols != lcols || rows - DAT_00077024 != lrows)
						wresize(DAT_0007b430, rows - DAT_00077024, cols);
				}
			}

			cp = current_pool();
			width = (DAT_00077318 != '\0') ? 100 : 0x50;
			wattr_on(DAT_0007b7c0, 0x200000, 0);          /* A_BOLD */
			snprintf(msg, 0x100, " cgminer version 4.9.0 - Started: %s", DAT_00077260);
			mvwprintw(DAT_0007b7c0, 0, 0, "%s", msg);
			wattr_off(DAT_0007b7c0, 0x200000, 0);
			rc = wmove(DAT_0007b7c0, 1, 0);
			if (rc != -1)
				whline(DAT_0007b7c0, '-', width);
			snprintf(msg, 0x100, " %s", DAT_00077160);
			mvwprintw(DAT_0007b7c0, 2, 0, "%s", msg);
			wclrtoeol(DAT_0007b7c0);
			hw_errs = DAT_0007b8a4;
			diff1_total = (uint64_t)DAT_0007b104 << 32 | DAT_0007b100;
			wu = (double)(int64_t)diff1_total / DAT_000755f8;   /* WU 分子 = Σdiff1 (原 __floatdidf) */
			if (DAT_00077318 == '\0') {
				if (DAT_00077154 == '\0') {
					/* 原码 snprintf 变参被 Ghidra 丢失; 依 wide 分支与残余寄存器实参重建 */
					snprintf(msg, 0x100, " A:%.0f  R:%.0f  HW:%d  WU:%.1f/m",
					         DAT_0007b5e8, DAT_0007b670, hw_errs, wu * 60.0);
					mvwprintw(DAT_0007b7c0, 3, 0, "%s", msg);
				} else {
					i = total_staged();
					shares64 = (uint64_t)DAT_0007bc84 << 32 | DAT_0007bc80;
					snprintf(msg, 0x100, " ST: %d  SS: %lld  NB: %d  LW: %d  GF: %d  RF: %d",
					         i, (long long)shares64, DAT_0007bb1c, DAT_0007b4cc,
					         DAT_0007b43c, DAT_0007ae68);
					mvwprintw(DAT_0007b7c0, 3, 0, "%s", msg);
				}
			} else {
				i = total_staged();
				shares64 = (uint64_t)DAT_0007bc84 << 32 | DAT_0007bc80;
				snprintf(msg, 0x100,
				         " A:%.0f  R:%.0f  HW:%d  WU:%.1f/m | ST: %d  SS: %lld  NB: %d  LW: %d  GF: %d  RF: %d",
				         DAT_0007b5e8, DAT_0007b670, hw_errs, wu * 60.0, i, (long long)shares64,
				         DAT_0007bb1c, DAT_0007b4cc, DAT_0007b43c, DAT_0007ae68);
				mvwprintw(DAT_0007b7c0, 3, 0, "%s", msg);
			}
			wclrtoeol(DAT_0007b7c0);
			/* 矿池行 (原 LAB_00024440 复用同一 mvwprintw 出口) */
			if ((uint32_t)(DAT_00076efc - 3) < 2 && 1 < DAT_0007b198) {
				const char *lp = (DAT_0007bc79 != 0) ? "" : "out";
				snprintf(msg, 0x100, " Connected to multiple pools with%s block change notify", lp);
				mvwprintw(DAT_0007b7c0, 4, 0, "%s", msg);
			} else if (*(uint8_t *)((char *)cp + 0x298) != 0) {   /* pool+0x298: has_stratum */
				snprintf(msg, 0x100, " Connected to %s diff %s with stratum as user %s",
				         *(char **)((char *)cp + 0x270), (char *)((char *)cp + 0x30),
				         *(char **)((char *)cp + 0xcc));
				mvwprintw(DAT_0007b7c0, 4, 0, "%s", msg);
			} else {
				const char *lp = (DAT_0007bc79 != 0) ? "" : "out";
				const char *proto = (*(uint8_t *)((char *)cp + 0x2f8) != 0) ? "GBT" : "LP";
				snprintf(msg, 0x100, " Connected to %s diff %s with%s %s as user %s",
				         *(char **)((char *)cp + 0x270), (char *)((char *)cp + 0x30), lp, proto,
				         *(char **)((char *)cp + 0xcc));
				mvwprintw(DAT_0007b7c0, 4, 0, "%s", msg);
			}
			wclrtoeol(DAT_0007b7c0);
			snprintf(msg, 0x100, " Block: %s...  Diff:%s  Started: %s  Best share: %s   ",
			         DAT_00077300, DAT_000772b8, DAT_000772e0, DAT_00075dc4);
			mvwprintw(DAT_0007b7c0, 5, 0, "%s", msg);
			rc = wmove(DAT_0007b7c0, 6, 0);
			if (rc != -1)
				whline(DAT_0007b7c0, '-', width);
			rc = wmove(DAT_0007b7c0, DAT_00077020 - 1, 0);
			if (rc != -1)
				whline(DAT_0007b7c0, '-', width);
			mvwprintw(DAT_0007b7c0, DAT_00077028 - 1, 1, "%s",
			          "[P]ool management [S]ettings [D]isplay options [Q]uit");

			/* —— 每设备状态行 —— */
			if (0 < DAT_0007b720) {
				row = 0;
				for (i = 0; i < DAT_0007b720; i++) {
					dev = get_devices(i);
					if (dev == NULL)
						continue;                     /* 原 LAB_000241cc 续行 */
					int drow = row;
					row = row + 1;
					if (DAT_0007b940 == '\0' && drow + DAT_00077028 < LINES - 1 &&
					    drow < DAT_0007702c) {
						int disp_row = drow + DAT_00077028;
						if (*(int *)((char *)dev + 0x170) == 0)
							timebase = DAT_000755f8;
						else {
							cgtime(&now);
							timebase = tdiff(&now, (struct timeval *)((char *)dev + 0x170));
							disp_row = drow + DAT_00077028;
						}
						hashes = (uint64_t)*(uint32_t *)((char *)dev + 0xc4) << 32 |
						         *(uint32_t *)((char *)dev + 0xc0);
						wu = (double)(float)(int32_t)*(uint32_t *)((char *)dev + 0x24);
						if (timebase < 1.0)
							timebase = 1.0;
						*(double *)((char *)dev + 0x58) = (wu / timebase) * 60.0;   /* dev->WU */
						wmove(DAT_0007b7c0, disp_row, 0);
						adj_width(i, &DAT_00075df8);
						if (*(char **)((char *)dev + 0x1c) == NULL) {
							sprintf(name8, "%-8d", *(uint32_t *)((char *)dev + 8));
						} else {
							name8[8] = 0;
							memset(name8, ' ', 8);
							strncpy(name8, *(char **)((char *)dev + 0x1c), 8);
						}
						snprintf(msg, 0x100, " %*d: %s %-8s: ", DAT_00075df8, i,
						         *(char **)(*(void **)((char *)dev + 4) + 8), name8);
						wprintw(DAT_0007b7c0, "%s", msg);
						statbuf[0] = 0;
						/* drv->get_statline_before(buf, 0x100, dev) (drv+0x14) */
						((void (*)(char *, int, void *))*(uintptr_t *)((uintptr_t)*(uintptr_t *)((char *)dev + 4) + 0x14))(statbuf, 0x100, dev);
						size_t slen = strlen(statbuf);
						if (slen < 0x17)
							strncat(statbuf, "                                        ", 0x17 - slen);
						snprintf(msg, 0x100, "%s | ", statbuf);
						wprintw(DAT_0007b7c0, "%s", msg);
						status = *(int *)((char *)dev + 0x60);
						const char *st = NULL;
						if (status == 2)
							st = "DEAD  ";
						else if (status == 1)
							st = "SICK  ";
						else if (*(int *)((char *)dev + 0x20) == 1)   /* deven: OFF */
							st = "OFF   ";
						else if (*(int *)((char *)dev + 0x20) == 2)   /* deven: REST */
							st = "REST  ";
						if (st != NULL) {
							wprintw(DAT_0007b7c0, "%s", st);
						} else {
							wu = (double)(int64_t)hashes / timebase * 60.0;   /* 设备 WU (原 __floatdidf) */
							if (DAT_00077318 == '\0') {
								if (DAT_00077154 == '\0') {
									rate2 = (*(double *)((char *)dev + 0x50) / timebase) * 1000000.0;
									suffix_string((uint64_t)rate2, hs_str2, 0x10, 4);   /* 原 __fixunsdfdi */
									rate1 = *(double *)((char *)dev + 0x30) * 1000000.0;
									suffix_string((uint64_t)rate1, hs_str1, 0x10, 4);
									adj_width((int)wu, &DAT_00075dfc);
									snprintf(msg, 0x100, "%6s / %6sh/s WU:%*.1f/m",
									         hs_str1, hs_str2, DAT_00075dfc + 2, wu);
									wprintw(DAT_0007b7c0, "%s", msg);
								} else {
									adj_fwidth((float)*(double *)((char *)dev + 200), &DAT_00075e00);
									adj_fwidth((float)*(double *)((char *)dev + 0xd0), &DAT_00075e04);
									adj_width(*(uint32_t *)((char *)dev + 0x2c), &DAT_00075e08);
									snprintf(msg, 0x100, "A:%*.0f R:%*.0f HW:%*d",
									         DAT_00075e00, *(double *)((char *)dev + 200),
									         DAT_00075e04, *(double *)((char *)dev + 0xd0),
									         DAT_00075e08, *(int *)((char *)dev + 0x2c));
									wprintw(DAT_0007b7c0, "%s", msg);
								}
							} else {
								rate2 = (*(double *)((char *)dev + 0x50) / timebase) * 1000000.0;
								suffix_string((uint64_t)rate2, hs_str2, 0x10, 4);
								rate1 = *(double *)((char *)dev + 0x30) * 1000000.0;
								suffix_string((uint64_t)rate1, hs_str1, 0x10, 4);
								adj_width((int)wu, &DAT_00075dfc);
								adj_fwidth((float)*(double *)((char *)dev + 200), &DAT_00075e00);
								adj_fwidth((float)*(double *)((char *)dev + 0xd0), &DAT_00075e04);
								adj_width(*(uint32_t *)((char *)dev + 0x2c), &DAT_00075e08);
								snprintf(msg, 0x100, "%6s / %6sh/s WU:%*.1f/m A:%*.0f R:%*.0f HW:%*d",
							        	 hs_str1, hs_str2, DAT_00075dfc + 2, wu, DAT_00075e00,
							        	 *(double *)((char *)dev + 200), DAT_00075e04,
							        	 *(double *)((char *)dev + 0xd0), DAT_00075e08,
							        	 *(int *)((char *)dev + 0x2c));
								wprintw(DAT_0007b7c0, "%s", msg);
							}
						}
						statbuf[0] = 0;
						/* drv->get_statline(buf, 0x100, dev) (drv+0x18) */
						((void (*)(char *, int, void *))*(uintptr_t *)((uintptr_t)*(uintptr_t *)((char *)dev + 4) + 0x18))(statbuf, 0x100, dev);
						snprintf(msg, 0x100, "%s", statbuf);
						wprintw(DAT_0007b7c0, "%s", msg);
						wclrtoeol(DAT_0007b7c0);
					}
				}
			}
			/* 刷新两窗口 (原 LAB_00024218) */
			rc = (DAT_0007b7c0 == NULL) ? -1 : (int)*(short *)((char *)DAT_0007b7c0 + 4) + 1;
			wtouchln(DAT_0007b7c0, 0, rc, 1);
			wrefresh(DAT_0007b7c0);
			rc = (DAT_0007b430 == NULL) ? -1 : (int)*(short *)((char *)DAT_0007b430 + 4) + 1;
			wtouchln(DAT_0007b430, 0, rc, 1);
			wrefresh(DAT_0007b430);
			if (pthread_mutex_unlock(&DAT_0007b7a8) != 0)
				mutex_unlock_fail_cgminer("unlock_curses", 0xb72);
			DAT_00075ed8();
		}

		cgtime(&now);
		/* —— 计划停机/开机调度 —— */
		if (DAT_0007b194 == '\0') {
			if (should_run() == 0) {
				if (LOG_GATE(4)) {
					snprintf(msg, 0x1000, "Pausing execution as per stop time %02d:%02d scheduled",
					         DAT_0007b5fc, DAT_0007b5f8);
					applog(4, msg, 0);
				}
				if (DAT_0007b874 == '\0') {           /* 无启动调度 → 终止 */
					applog(3, "Terminating execution as planned", 1);
					quit(0);
					return 0;
				}
				if (LOG_GATE(4)) {
					snprintf(msg, 0x1000, "Will restart execution as scheduled at %02d:%02d",
					         DAT_0007b880, DAT_0007b87c);
					applog(4, msg, 0);
				}
				DAT_0007b194 = '\x01';
				if (pthread_rwlock_rdlock(&DAT_0007b650) != 0)
					rd_lock_error("watchdog_thread", 0x25f9);
				if (0 < DAT_0007bb6c) {
					void **thr_p = DAT_0007b5e0;
					void **thr_end = DAT_0007b5e0 + DAT_0007bb6c;
					do {
						*(uint8_t *)((char *)*thr_p + 0x3c) = 1;   /* thr->pause 标志 */
						thr_p++;
					} while (thr_p != thr_end);
				}
				if (pthread_rwlock_unlock(&DAT_0007b650) != 0)
					rw_unlock_error("watchdog_thread", 0x25fc);
				DAT_00075ed8();
			} else if (DAT_0007b194 != '\0') {
				goto resume_check;                    /* 原 LAB_000233e4 跳转 (此处恒假, 死路径) */
			}
		} else {
resume_check:                                         /* 原 LAB_000233e4 */
			if (should_run() != 0) {
				if (LOG_GATE(4)) {
					snprintf(msg, 0x1000, "Restarting execution as per start time %02d:%02d scheduled",
					         DAT_0007b880, DAT_0007b87c);
					applog(4, msg, 0);
				}
				if (DAT_0007b5f0 != '\0' && LOG_GATE(4)) {
					snprintf(msg, 0x1000, "Will pause execution as scheduled at %02d:%02d",
					         DAT_0007b5fc, DAT_0007b5f8);
					applog(4, msg, 0);
				}
				DAT_0007b194 = '\0';
				if (0 < DAT_0007bb6c) {
					for (i = 0; i < DAT_0007bb6c; i++) {
						void *thr_i = get_thread(i);
						/* thr+0x24 = pool; pool+0x20 标志非 1 才恢复 */
						if (*(int *)(*(uintptr_t *)((uintptr_t)thr_i + 0x24) + 0x20) != 1) {
							*(uint8_t *)((char *)thr_i + 0x3c) = 0;
							if (DAT_00078e09 != '\0' && LOG_GATE(7)) {
								snprintf(msg, 0x1000, "Pushing sem post to thread %d",
							        	 *(int *)thr_i);
								applog(7, msg, 0);
							}
							_cgsem_post((sem_t *)((char *)thr_i + 0x10), "cgminer.c",
							            "watchdog_thread", 0x2612);
						}
					}
				}
			}
		}

		/* —— 设备健康看门狗 —— */
		if (0 < DAT_0007b720) {
			for (i = 0; i < DAT_0007b720; i++) {
				dev = get_devices(i);                 /* 原码未判空, 按机器码保留 */
				thr = *(void **)(*(void **)((char *)dev + 0x98));   /* dev->thr[0] */
				if (thr != NULL) {
					/* drv+0x20 钩子 (原码无参调用) */
					((void (*)(void))*(uintptr_t *)((uintptr_t)*(uintptr_t *)((char *)dev + 4) + 0x20))();
					snprintf(dev_repr, 8, "%s %d",
					         *(char **)(*(void **)((char *)dev + 4) + 8),
					         *(int *)((char *)dev + 8));
					if (*(uint8_t *)((char *)thr + 0x3d) == 0 &&      /* thr 非空闲 */
					    *(int *)((char *)dev + 0x20) != 1) {          /* deven 非 OFF */
						status = *(int *)((char *)dev + 0x60);
						elapsed = now.tv_sec - *(int *)((char *)thr + 0x2c);
						if (status == 0) {
							if (elapsed < 0x79) {
recheck:                                                  /* 原 LAB_00023904 */
								if (0x3c < now.tv_sec - *(int *)((char *)thr + 0x34) &&
								    (uint32_t)(status - 1) < 2) {
									cgtime((struct timeval *)((char *)thr + 0x34));
									if (DAT_00075e0c != '\0')
										goto reinit;      /* 原 LAB_000237c4 */
								}
							} else {
								*(uint64_t *)((char *)dev + 0x30) = 0;
								*(int *)((char *)dev + 0x60) = 1;   /* status = SICK */
								if (LOG_GATE(3)) {
									snprintf(msg, 0x1000, "%s: Idle for more than 60 seconds, declaring SICK!",
							        		 dev_repr);
									applog(3, msg, 0);
								}
								cgtime((struct timeval *)((char *)thr + 0x34));
								record_device_fail(dev, 3);
								if (DAT_00075e0c != '\0') {
									if (LOG_GATE(3)) {
										snprintf(msg, 0x1000, "%s: Attempting to restart", dev_repr);
										applog(3, msg, 0);
									}
									goto reinit;
								}
							}
						} else {
							elapsed = now.tv_sec - *(int *)((char *)thr + 0x2c);
							if (elapsed < 0x78) {
								if (status != 4 && LOG_GATE(3)) {
									snprintf(msg, 0x1000, "%s: Recovered, declaring WELL!", dev_repr);
									applog(3, msg, 0);
								}
								*(int *)((char *)dev + 0x60) = 0;
								tnow = time(NULL);
								*(time_t *)((char *)dev + 0xf0) = tnow;
							} else {
								if (status != 1 || elapsed < 0x259)
									goto recheck;
								*(int *)((char *)dev + 0x60) = 2;   /* status = DEAD */
								if (LOG_GATE(3)) {
									snprintf(msg, 0x1000,
							        		 "%s: Not responded for more than 10 minutes, declaring DEAD!",
							        		 dev_repr);
									applog(3, msg, 0);
								}
								cgtime((struct timeval *)((char *)thr + 0x34));
								record_device_fail(dev, 4);
							}
						}
					}
				}
				continue;
reinit:                                               /* 原 LAB_000237c4 */
				reinit_device(dev);
			}
		}
	} while (1);
}

/* ==================== 3. parse_notify_equihash (原 FUN_0002cf40) ==================== */

/* 字节序辅助 (与反编译体中的移位拼装表达式等价的记法) */
#define BSW32(x) \
	(((uint32_t)(x) >> 24) | (((uint32_t)(x) >> 8) & 0xff00u) | \
	 (((uint32_t)(x) << 8) & 0xff0000u) | ((uint32_t)(x) << 24))
static uint32_t load_le32(const uint8_t *p, size_t off)
{
	return (uint32_t)p[off] | (uint32_t)p[off + 1] << 8 |
	       (uint32_t)p[off + 2] << 16 | (uint32_t)p[off + 3] << 24;
}

// === parse_notify_equihash === 原 FUN_0002cf40 @ 0x0002cf40
// 来源: reversed/cgminer/20_job_mining/FUN_0002cf40.c | 分类: 20_job_mining
// 语义: stratum 收到一行 JSON 后的方法分派 (Equihash 变体)。解析 method/params:
//   mining.notify → 释放池上旧 5 串, 存 job_id/prevhash/version/ntime/nbit, 拼装
//     version+prevhash+merkle+reserved+ntime+nbit+40 个'0' 的 256 hex 字符工作头并
//     hex2bin 128 字节到 pool+0x644, workbase 计数自增; notify 参数不齐 → 释放并置失败。
//   mining.set_difficulty → 写 pool+0x730 (double) 并按整数/小数打日志;
//   mining.set_extranonce → 转发 parse_extranonce_equihash;
//   mining.set_target → 32 字节目标 hex2bin 后按大端逐字写入 pool+0x48..0x67;
//   client.reconnect → 域名白名单校验后换池地址重启 stratum;
//   client.get_version / mining.ping → 回发固定 JSON;
//   client.show_message → 打印矿池消息。
// 返回: 1 = 方法已处理 (notify 成功/难度或目标已更新等), 0 = 未处理/失败。
// pool 字段偏移 (字节, 机器码锁定): +0x30 diff 显示串; +0x94 workbase 计数; +0xcc user;
//   +0xf4 mutex / +0x10c rwlock (sdata 锁); +0x258 socka_url; +0x260 port; +0x264 fd;
//   +0x270 url; +0x288/+0x28c notify 期间清零标志; +0x298 has_stratum; +0x299/+0x29b 状态字节;
//   +0x2a0 job_id / +0x2a4 prevhash / +0x2a8 merkle / +0x2ac version / +0x2b0 ntime /
//   +0x2b4 nbit; +0x2b8 clean; +0x2c0 header_len; +0x2c8 double; +0x2d8 mutex;
//   +0x644 header_bin[128]; +0x730 stratum_diff; +0x738 notify diff; +0x740 头长中间值。
uint32_t parse_notify_equihash(void *pool, char *payload)
{
	uint8_t *pp = (uint8_t *)pool;
	char msg[0x1000];
	int32_t err_head[23];                 /* json_error_t: +0 line (+4 column, +8 position, +0xc source[80]) */
	char err_text[160];                   /* json_error_t.text */
	json_t *root;
	json_t *method_val, *error_val, *params_val;
	char *method;
	char *job_id, *prevhash, *merkle, *version, *reserved, *ntime, *nbit;
	char *host, *port;
	char *header = NULL;
	bool clean, parsed_ok;
	uint32_t ret;
	size_t l1, l2, header_len;
	int hlen2, id;
	char reconnect_url[256];
	char *socka_url, *socka_port;
	char *old_str;
	struct timeval tv;
	uint32_t block_ntime;
	uint8_t target_w_bytes[32];
	double diff, diff_old, diff_round;
	const char *reply_fmt;

	if (payload == NULL)
		return 0;
	root = json_loads(payload, 0, err_head);
	if (root == NULL) {
		if (DAT_00078e09 != '\0' && LOG_GATE(7)) {
			snprintf(msg, 0x1000, "JSON decode failed(%d): %s", err_head[0], err_text);
			applog(7, msg, 0);
		}
		return 0;
	}
	method_val = json_object_get(root, "method");
	if (method_val == NULL)
		goto return_zero;                 /* 原 LAB_0002d0d8 */
	error_val = json_object_get(root, "error");
	params_val = json_object_get(root, "params");
	if (error_val != NULL && *(int *)error_val != 7 /* JSON_NULL */) {
		char *es = json_dumps(error_val, 3);
		if (DAT_00078e09 != '\0' && LOG_GATE(7)) {
			snprintf(msg, 0x1000, "JSON-RPC method decode failed: %s", es);
			applog(7, msg, 0);
		}
		ret = 0;
		free(es);
		goto cleanup;
	}
	method = json_string_value(method_val);
	if (method == NULL)
		goto return_zero;

	if (strncasecmp(method, "mining.notify", 0xd) != 0) {
		/* —— 非 notify 方法分派 (原 101-415 行) —— */
		if (strncasecmp(method, "mining.set_extranonce", 0x15) == 0) {
			ret = parse_extranonce_equihash(pool, params_val);
			goto cleanup;
		}
		if (strncasecmp(method, "mining.set_difficulty", 0x15) != 0) {
			if (strncasecmp(method, "client.reconnect", 0x10) == 0) {
				/* —— client.reconnect —— */
				memset(reconnect_url, 0, 0xff);
				host = json_string_value(json_array_get(params_val, 0));
				if (host == NULL) {
					host = *(char **)(pp + 0x270);        /* pool->rpc_url */
do_reconnect:                                         /* 原 LAB_0002df74 */
					port = json_string_value(json_array_get(params_val, 1));
					if (port == NULL)
						port = *(char **)(pp + 0x260);    /* pool->rpc_port */
					snprintf(reconnect_url, 0xfe, "%s:%s", host, port);
					if (extract_sockaddr(reconnect_url, &socka_url, &socka_port) != 0) {
						if (LOG_GATE(4)) {
							snprintf(msg, 0x1000, "Stratum reconnect requested from pool %d to %s",
							         *(int *)pp, reconnect_url);
							applog(4, msg, 0);
						}
						clear_pool_work(pool);
						if (pthread_mutex_lock((pthread_mutex_t *)(pp + 0x2d8)) != 0)
							mutex_lock_fail_util("parse_reconnect", 0xa51);
						clear_sockbuf(pool);
						*(uint8_t *)(pp + 0x29b) = 0;
						*(uint8_t *)(pp + 0x299) = 0;
						if (*(int *)(pp + 0x264) != 0)
							close(*(int *)(pp + 0x264));
						old_str = *(char **)(pp + 0x270);
						*(int *)(pp + 0x264) = 0;
						*(char **)(pp + 0x258) = socka_url;
						*(char **)(pp + 0x270) = socka_url;
						free(old_str);
						old_str = *(char **)(pp + 0x260);
						*(char **)(pp + 0x260) = socka_port;
						free(old_str);
						if (pthread_mutex_unlock((pthread_mutex_t *)(pp + 0x2d8)) != 0) {
							int *e = __errno_location();
							snprintf(msg, 0x1000, "WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d",
							         *e, "util.c", "parse_reconnect", 0xa5a);
							applog(3, msg, 1);
							quit(1);
						}
						DAT_00075ed8();
						ret = restart_stratum(pool);
						goto cleanup;
					}
				} else {
					/* —— host 由 params 携带: 域名白名单校验 (原 157-186, LAB_0002dc04 拒绝) ——
					 * 原 gate 关闭时拒绝路径落空同样落到 339 → 返回 0, 故统一 ret=0 返回 */
					char *dom1 = strchr(*(char **)(pp + 0x270), '.');
					if (dom1 == NULL) {
						if (LOG_GATE(3)) {
							snprintf(msg, 0x1000, "Denied stratum reconnect request for pool without domain '%s'",
							         *(char **)(pp + 0x270));
							applog(3, msg, 0);
						}
						ret = 0;
						goto cleanup;
					}
					char *dom2 = strchr(host, '.');
					if (dom2 == NULL) {
						if (LOG_GATE(3)) {
							snprintf(msg, 0x1000, "Denied stratum reconnect request to url without domain '%s'", host);
							applog(3, msg, 0);
						}
						ret = 0;
						goto cleanup;
					}
					if (strcmp(dom1, dom2) == 0)
						goto do_reconnect;
					if (LOG_GATE(3)) {
						snprintf(msg, 0x1000, "Denied stratum reconnect request to non-matching domain url '%s'",
						         *(char **)(pp + 0x270));
						applog(3, msg, 0);
					}
					ret = 0;
					goto cleanup;
				}
			} else {
				/* —— get_version / show_message / ping / set_target 链 (原 189-338) —— */
				if (strncasecmp(method, "client.get_version", 0x12) == 0) {
					json_t *id_val = json_object_get(root, DAT_00061ca0);
					ret = 0;
					if (id_val == NULL)
						goto cleanup;
					id_val = json_object_get(root, DAT_00061ca0);   /* 原码重复取一次 (两次调用) */
					id = (int)json_integer_value(id_val);
					reply_fmt = "{\"id\": %d, \"result\": \"cgminer/4.9.0\", \"error\": null}";
					goto send_reply;                          /* 原 LAB_0002d2b0 */
				} else if (strncasecmp(method, "client.show_message", 0x13) == 0) {
					if (params_val != NULL && *(int *)params_val == 1 /* JSON_ARRAY */) {
						char *m = json_string_value(json_array_get(params_val, 0));
						if (m != NULL) {
							if (DAT_00078e09 != '\0' && LOG_GATE(7)) {
								snprintf(msg, 0x1000, "Pool %d message: %s", *(int *)pp, m);
								applog(7, msg, 0);
							}
							ret = 1;                          /* 原 LAB_0002d3f4 */
							goto cleanup;
						}
					}
				} else if (strncasecmp(method, "mining.ping", 0xb) == 0) {
					if (DAT_00078e09 != '\0' && LOG_GATE(7)) {
						snprintf(msg, 0x1000, "Pool %d ping", *(int *)pp);
						applog(7, msg, 0);
					}
					json_t *id_val = json_object_get(root, DAT_00061ca0);
					if (id_val != NULL) {
						id_val = json_object_get(root, DAT_00061ca0);   /* 原码重复取一次 */
						id = (int)json_integer_value(id_val);
						reply_fmt = "{\"id\": %d, \"result\": \"pong\", \"error\": null}";
						goto send_reply;
					}
				} else if (strncasecmp(method, "mining.set_target", 0x11) == 0) {
					char *tstr = json_array_string(params_val, 0);
					if (tstr != NULL) {
						hex2bin(target_w_bytes, (uint8_t *)tstr, 0x20);
						if (pthread_mutex_lock((pthread_mutex_t *)(pp + 0xf4)) != 0)
							mutex_lock_fail_util("parse_target", 0xa9a);
						if (pthread_rwlock_wrlock((pthread_rwlock_t *)(pp + 0x10c)) != 0) {
							int *e = __errno_location();
							snprintf(msg, 0x1000, "WTF WRLOCK ERROR ON LOCK! errno=%d in %s %s():%d",
							         *e, "util.c", "parse_target", 0xa9a);
							applog(3, msg, 1);
							quit(1);
						}
						/* 32 字节目标按大端逐字写入 pool+0x48..0x67 (pool[0x12..0x19];
						 * 原码目标缓冲前 8 字节复用 timeval 形栈槽, 写入序 0x19,0x13..0x16,0x12,0x17,0x18) */
						*(uint32_t *)(pp + 0x64) = BSW32(load_le32(target_w_bytes, 0));    /* pool[0x19] */
						*(uint32_t *)(pp + 0x4c) = BSW32(load_le32(target_w_bytes, 24));   /* pool[0x13] */
						*(uint32_t *)(pp + 0x50) = BSW32(load_le32(target_w_bytes, 20));   /* pool[0x14] */
						*(uint32_t *)(pp + 0x54) = BSW32(load_le32(target_w_bytes, 16));   /* pool[0x15] */
						*(uint32_t *)(pp + 0x58) = BSW32(load_le32(target_w_bytes, 12));   /* pool[0x16] */
						*(uint32_t *)(pp + 0x48) = BSW32(load_le32(target_w_bytes, 28));   /* pool[0x12] */
						*(uint32_t *)(pp + 0x5c) = BSW32(load_le32(target_w_bytes, 8));    /* pool[0x17] */
						*(uint32_t *)(pp + 0x60) = BSW32(load_le32(target_w_bytes, 4));    /* pool[0x18] */
						_rw_unlock((pthread_rwlock_t *)(pp + 0x10c), "parse_target", 0xa9d);
						if (pthread_mutex_unlock((pthread_mutex_t *)(pp + 0xf4)) != 0) {
							int *e = __errno_location();
							snprintf(msg, 0x1000, "WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d",
							         *e, "util.c", "parse_target", 0xa9d);
							applog(3, msg, 1);
							quit(1);
						}
						ret = 1;
						DAT_00075ed8();
						free(tstr);
						goto cleanup;
					}
					if (DAT_00078e09 != '\0' && LOG_GATE(7))
						applog(7, "parse_target: Missing an array value", 0);
				}
			}
			goto return_zero;                     /* 原 LAB_0002d0d8 (339) */
		}
		/* —— mining.set_difficulty (set_difficulty 命中直落, 原 341-415) —— */
		diff = json_number_value(json_array_get(params_val, 0));
		if (diff == 0.0) {
			ret = 0;
			goto cleanup;
		}
		if (pthread_mutex_lock((pthread_mutex_t *)(pp + 0xf4)) != 0)
			mutex_lock_fail_util("parse_diff", 0x9b3);
		if (pthread_rwlock_wrlock((pthread_rwlock_t *)(pp + 0x10c)) != 0) {
			int *e = __errno_location();
			snprintf(msg, 0x1000, "WTF WRLOCK ERROR ON LOCK! errno=%d in %s %s():%d",
			         *e, "util.c", "parse_diff", 0x9b3);
			applog(3, msg, 1);
			quit(1);
		}
		diff_old = *(double *)(pp + 0x730);
		*(double *)(pp + 0x730) = diff;
		if (pthread_rwlock_unlock((pthread_rwlock_t *)(pp + 0x10c)) != 0) {
			int *e = __errno_location();
			snprintf(msg, 0x1000, "WTF RWLOCK ERROR ON UNLOCK! errno=%d in %s %s():%d",
			         *e, "util.c", "parse_diff", 0x9b6);
			applog(3, msg, 1);
			quit(1);
		}
		if (pthread_mutex_unlock((pthread_mutex_t *)(pp + 0xf4)) != 0) {
			int *e = __errno_location();
			snprintf(msg, 0x1000, "WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d",
			         *e, "util.c", "parse_diff", 0x9b6);
			applog(3, msg, 1);
			quit(1);
		}
		DAT_00075ed8();
		if (diff == diff_old) {
			if (DAT_00078e09 != '\0' && LOG_GATE(7)) {
				snprintf(msg, 0x1000, "Pool %d difficulty set to %f", *(int *)pp, diff);
				applog(7, msg, 0);
			}
		} else {
			int diff_int = (int)diff;                     /* 原 VectorFloatToSigned */
			diff_round = (double)(float)diff_int;         /* 原 VectorSignedToFloat 回转 */
			if (diff == diff_round) {
				if (DAT_00078e09 != '\0' && LOG_GATE(7)) {
					snprintf(msg, 0x1000, "Pool %d difficulty changed to %d", *(int *)pp, diff_int);
					applog(7, msg, 0);
				}
			} else if (DAT_00078e09 != '\0' && LOG_GATE(7)) {
				snprintf(msg, 0x1000, "Pool %d difficulty changed to %.1f", *(int *)pp, diff);
				applog(7, msg, 0);
			}
		}
		ret = 1;                                          /* 原 LAB_0002d3f4 */
		goto cleanup;
	}

	/* ==================== mining.notify (Equihash) ==================== */
	job_id = json_array_string(params_val, 0);
	version = json_array_string(params_val, 1);
	prevhash = json_array_string(params_val, 2);
	merkle = json_array_string(params_val, 3);
	reserved = json_array_string(params_val, 4);
	ntime = json_array_string(params_val, 5);
	nbit = json_array_string(params_val, 6);
	{
		json_t *clean_val = json_array_get(params_val, 7);
		clean = (clean_val != NULL && *(int *)clean_val == 5 /* JSON_TRUE */);
	}
	if (job_id == NULL || prevhash == NULL) {
		if (job_id != NULL)
			free(job_id);
		goto free_rest;                       /* 原 LAB_0002dae8 */
	}
	if (merkle == NULL || reserved == NULL || version == NULL || nbit == NULL || ntime == NULL) {
free_job_and_rest:                            /* 原 LAB_0002dcb0 */
		free(job_id);
free_rest:                                    /* 原 LAB_0002dae8 */
		if (prevhash != NULL)
			free(prevhash);
		if (reserved != NULL)
			free(reserved);
		if (merkle != NULL)
			free(merkle);
		if (version != NULL)
			free(version);
		if (nbit != NULL)
			free(nbit);
		if (ntime != NULL)
			free(ntime);
		parsed_ok = false;
	} else {
		if (DAT_00078e09 != '\0' && LOG_GATE(7))
			applog(7, "Valid Notify\n", 0);
		if (pthread_mutex_lock((pthread_mutex_t *)(pp + 0xf4)) != 0)
			mutex_lock_fail_util("parse_notify_equihash", 0x85e);
		if (pthread_rwlock_wrlock((pthread_rwlock_t *)(pp + 0x10c)) != 0) {
			int *e = __errno_location();
			snprintf(msg, 0x1000, "WTF WRLOCK ERROR ON LOCK! errno=%d in %s %s():%d",
			         *e, "util.c", "parse_notify_equihash", 0x85e);
			applog(3, msg, 1);
			quit(1);
		}
		free(*(char **)(pp + 0x2a0));
		free(*(char **)(pp + 0x2a4));
		free(*(char **)(pp + 0x2ac));
		free(*(char **)(pp + 0x2b0));
		free(*(char **)(pp + 0x2b4));
		*(char **)(pp + 0x2a0) = job_id;
		*(char **)(pp + 0x2a4) = prevhash;
		*(char **)(pp + 0x2ac) = version;
		*(char **)(pp + 0x2b0) = ntime;
		*(char **)(pp + 0x2b4) = nbit;
		*(uint8_t *)(pp + 0x2b8) = clean ? 1 : 0;
		if (0.0 < *(double *)(pp + 0x738))
			*(double *)(pp + 0x2c8) = *(double *)(pp + 0x738);
		if (clean) {
			*(int *)(pp + 0x288) = 0;
			*(int *)(pp + 0x28c) = 0;
		}
		/* 工作头长度: ((ntime_len>>1) + (nbit_len>>1) + 0x54 + (len(version)+len(prevhash))/2) * 2 + 1 */
		l1 = strlen(version);
		l2 = strlen(prevhash);
		*(int *)(pp + 0x740) = l1 + l2;
		hlen2 = (int)(l1 + l2) / 2;
		l1 = strlen(ntime);
		l2 = strlen(nbit);
		*(int *)(pp + 0x740) = hlen2;
		header_len = ((l2 >> 1) + (l1 >> 1) + 0x54 + hlen2) * 2 + 1;
		*(int *)(pp + 0x2c0) = header_len;
		if (DAT_00078e09 != '\0' && LOG_GATE(7)) {
			snprintf(msg, 0x1000, "%s: pool->swork.header_len = %d", "parse_notify_equihash", (int)header_len);
			applog(7, msg, 0);
			header_len = *(uint32_t *)(pp + 0x2c0);
		}
		if ((header_len & 3) != 0) {          /* 对齐到 4 的倍数 */
			header_len = (header_len & 0xfffffffc) + 4;
			*(int *)(pp + 0x2c0) = header_len;
		}
		header = malloc(header_len);
		if (header == NULL) {
			snprintf(msg, 0x1000, "%s: Failed to malloc header. in %s %s():%d",
			         "parse_notify_equihash", "util.c", "parse_notify_equihash", 0x88c);
			applog(3, msg, 1);
			quit(1);
			header_len = *(uint32_t *)(pp + 0x2c0);
		}
		snprintf(header, header_len, "%s%s%s%s%s%s%s",
		         *(char **)(pp + 0x2ac), *(char **)(pp + 0x2a4), merkle, reserved,
		         *(char **)(pp + 0x2b0), *(char **)(pp + 0x2b4),
		         "0000000000000000000000000000000000000000");
		if (hex2bin((uint8_t *)(pp + 0x644), (uint8_t *)header, 0x80) == 0) {
			if (LOG_GATE(4)) {
				snprintf(msg, 0x1000, "%s: Failed to convert header to header_bin, got %s",
				         "parse_notify_equihash", header);
				applog(4, msg, 0);
			}
			_rw_unlock((pthread_rwlock_t *)(pp + 0x10c), "parse_notify_equihash", 0x89d);
			mutex_unlock_util((pthread_mutex_t *)(pp + 0xf4), "parse_notify_equihash", 0x89d);
			DAT_00075ed8();
			pool_died_silent(pool);
			goto free_job_and_rest;
		}
		*(int *)(pp + 0x94) = *(int *)(pp + 0x94) + 1;
		{   /* 64 位计数 ++ (DAT_0007b648/7b64c 低/高字, 原码 0xfffffffe < 低字 → 进位) */
			uint32_t carry = (DAT_0007b648 > 0xfffffffe) ? 1 : 0;
			DAT_0007b648++;
			DAT_0007b64c += carry;
		}
		if (pthread_rwlock_unlock((pthread_rwlock_t *)(pp + 0x10c)) != 0) {
			int *e = __errno_location();
			snprintf(msg, 0x1000, "WTF RWLOCK ERROR ON UNLOCK! errno=%d in %s %s():%d",
			         *e, "util.c", "parse_notify_equihash", 0x8a6);
			applog(3, msg, 1);
			quit(1);
		}
		mutex_unlock_util((pthread_mutex_t *)(pp + 0xf4), "parse_notify_equihash", 0x8a6);
		DAT_00075ed8();
		if (pool == current_pool())
			DAT_0007b8ac = 1;
		if (DAT_0007b380 != '\0' && DAT_00078e09 != '\0' && LOG_GATE(7)) {
			/* 原码为 8 级 GCC 分支链, 每级条件相同 (A||B||C>6) — 展平后逐条打印 */
			snprintf(msg, 0x1000, "job_id: %s", job_id);
			applog(7, msg, 0);
			snprintf(msg, 0x1000, "version: %s", version);
			applog(7, msg, 0);
			snprintf(msg, 0x1000, "prev_hash: %s", prevhash);
			applog(7, msg, 0);
			snprintf(msg, 0x1000, "merkle: %s", merkle);
			applog(7, msg, 0);
			snprintf(msg, 0x1000, "reserved: %s", reserved);
			applog(7, msg, 0);
			snprintf(msg, 0x1000, "nbit: %s", nbit);
			applog(7, msg, 0);
			snprintf(msg, 0x1000, "ntime: %s", ntime);
			applog(7, msg, 0);
			snprintf(msg, 0x1000, "clean: %s", clean ? "yes" : "no");
			applog(7, msg, 0);
		}
		if (DAT_00078ca8 == '\0') {
			hex2bin((uint8_t *)&block_ntime, (uint8_t *)ntime, 4);
			gettimeofday(&tv, NULL);
			if (LOG_GATE(5)) {
				snprintf(msg, 0x1000, "stime.tv_sec %lu, block_ntime %lu\n",
			        	 (unsigned long)tv.tv_sec, (unsigned long)block_ntime);
				applog(5, msg, 0);
			}
			if ((uint32_t)tv.tv_sec + 0xe10U < block_ntime) {   /* 本地时钟落后区块 >1h → 校准 */
				tv.tv_sec = block_ntime;
				settimeofday(&tv, NULL);
			}
			DAT_00078ca8 = '\x01';
		}
		parsed_ok = true;
	}
	/* 原码 r11 为入口寄存器残留 (Ghidra unaff_r11, 参数不齐路径在函数未赋值时即释放该
	 * 残留值, 行为不可复现); C 侧以 NULL 初始化, 无效路径等价于不释放 */
	if (header != NULL)
		free(header);
	*(uint8_t *)(pp + 0x29b) = parsed_ok ? 1 : 0;
	ret = parsed_ok ? 1 : 0;
	goto cleanup;

send_reply:                                   /* 原 LAB_0002d2b0 */
	sprintf(msg, reply_fmt, id);
	ret = stratum_send(pool, msg, strlen(msg));
	goto cleanup;

cleanup:                                      /* 原 LAB_0002cfe4: json 引用计数释放 */
	if (*(int *)((char *)root + 4) == -1)
		return ret;
	*(int *)((char *)root + 4) = *(int *)((char *)root + 4) - 1;
	if (*(int *)((char *)root + 4) == 0)
		json_delete(root);
	return ret;

return_zero:                                  /* 原 LAB_0002d0d8 */
	ret = 0;
	goto cleanup;
}


// ============================================================================
// 覆盖率表 (batch=job_b2, 3 函数, 共 2165 反编译行)
// ---------------------------------------------------------------------------
// 原函数           语义名                  原地址       反编译行  转写行  覆盖说明
// FUN_0001db0c     test_work_current       0x0001db0c  878       ~700    全量: mandatory 早退/8 哈希字提取/lookup2
//                                                                         查找(两份内联哈希其一)/块库(0x2C 控制块
//                                                                         +12B 桶 +0x68 节点)超限自底向上归并清除/
//                                                                         桶表翻倍重建/网络难度/set_curblock/新块
//                                                                         通知/restart_threads; LAB_* 保留语义标签
// FUN_00023344     watchdog_thread         0x00023344  580       ~404    全量: curses 窗口自适应/统计与设备状态行
//                                                                         /菜单行/计划停开机调度/设备 SICK·DEAD·
//                                                                         WELL 看门狗 (LAB_000233e4/00023904/
//                                                                         000237c4 保留语义标签)
// FUN_0002cf40     parse_notify_equihash   0x0002cf40  707       ~483    全量: notify(Equihash 80B 工作头拼装)/
//                                                                         set_difficulty/set_target/set_extranonce
//                                                                         /reconnect(域名白名单)/get_version/
//                                                                         show_message/ping; 时钟对齐; LAB_0002cfe4
//                                                                         /d0d8/d2b0/dae8/dcb0/df74/dc04 保留
//
// 转写说明:
//   - 反编译统一日志门控展开为 opt_debug && LOG_GATE(prio) 宏 (与 util.c/log.c 一致);
//   - 编译器辅助 __floatdidf/__floatundidf/__fixunsdfdi/__udivsi3 还原为 C 转换/除法;
//   - VectorShiftLeft/VectorSignedToFloat/VectorFloatToSigned (NEON/VFP 渲染) 还原为
//     64 位移位 / int→float→double / float→int (FPSCR 舍入模式按本固件常量路径等价);
//   - 原码把 "test_work_current OOM"/"New block detected on network..."/"Terminating
//     execution as planned"/"Valid Notify\n"/"parse_target: Missing an array value"
//     等字符串拆为栈上逐字节赋值, 按约定还原为等价字面量;
//   - hashmeter(-1,...) 第 2 参在原码中为 r1 残留 (thr_id<0 路径不使用), 按 0 转写并注释;
//   - parse_notify 无效参数路径的原 r11 释放 (入口寄存器残留, 行为不可复现) 以 NULL
//     归零处理并注释; watchdog 窗口尺寸检查的 SBORROW 渲染还原为两分支语义比较;
//   - 块库结构体 (ht_ctrl/ht_bucket/blk_db_entry) 布局由调用点交叉锁定后具名还原,
//     其余 work/pool/dev/thr 结构一律按字节偏移访问并注明偏移语义。
