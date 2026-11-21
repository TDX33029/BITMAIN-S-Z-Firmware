/*
 * cgminer 20_job_mining 任务层第 5 段 (批次 job_b5, 49 函数) — BM1746 固件语义转写
 * =========================================================
 * 原始二进制: bm1746/cgminer_z15_bm1746.elf (ARM32, SD 镜像 cgminer-z15, branch "bm1746")
 * 来源:      reversed/cgminer/20_job_mining/ | 命名: rewritten/name_map/cgminer.tsv
 *
 * 本文件原计划按主题分 7 组汇集 49 个短函数 (batch_plan.tsv 中 batch=job_b5);
 * 2026-11-21 停滞续作分拆: 组 C/D 与组 E 前 4 函数已由 job_p5a.c 完成,
 * 组 E 后 4 函数与组 F/G 已由 job_p5b.c 完成, 本文件现含组 A/B 全部 14 个函数
 * 及 job_b5 补充批次 2 个位原语 (get_bit leading_zeros_256bit):
 *   [A] work 构造 / 难度计算 / hash 再生成:
 *       should_run calc_diff regen_hash select_pool rebuild_nonce pretest_hash copy_work
 *   [B] nonce/变长编码/哈希队列节点/统计行:
 *       diff_from_nonce2 discard_work set_target varint_encode test_nonce_diff
 *       __work_completed get_statline
 *   [C] curses 窗口/队列清理/工作统计:
 *       check_winsizes clear_pool_work log_curses_only age_queued_work make_work
 *       share_diff update_work_stats
 *   [D] 提交/驱动工作循环/退出:
 *       test_nonce_fulltest flush_queue clone_queued_work_byid hash_driver_work __quit
 *       inc_work_stats submit_noffset_nonce
 *   [E] 设备使能/驱动默认桩/stratum 解析:
 *       submit_tested_work enable_curses fill_drv_defaults enable_device
 *       parse_extranonce_equihash tq_push record_device_fail
 *   [F] sha256 原语/API COIN 查询/日志/内核链表/位原语:
 *       sha256_ctx_final minecoin applog k_get get_bit bm1740_makeup_work
 *       leading_zeros_256bit
 *   [G] jansson/ccan-opt/libgcc 库函数 (按体转写, 注明上游对照):
 *       dump_indent jsonp_dtostr json_object_update_missing json_object_update
 *       json_object_update_existing opt_register_table __udivsi3
 *
 * 关键机器语义 (Ghidra 调用点实参以 ARM 反汇编核实):
 *   - workid 机制: work+0x724 为工作序号, 下发帧 byte[3] = work->id & 0x7f,
 *     fill_work_thread 以 128 槽映射回查。
 *   - submit_noffset_nonce / submit_tested_work 中 update_work_stats(thr, work)、
 *     submit_work_async(work)、record_device_fail(cgpu, 1) 等被 Ghidra 丢失实参的
 *     调用, 已按 0x1c5x8/0x25dx/0x25ex 处寄存器现场恢复。
 *   - 难度常数: DAT_0007b859 (opt_scrypt) 选 65536/0xFFFF0000/2^240 分支,
 *     否则 2^224/0xFFFF00000000 (equihash/sha256)。
 *   - 日志门控 (与 uart.c/hash.c 同款): prio7 需 opt_debug && (A||B||C>6),
 *     prio6: C>5, prio5: A||B||C>4, prio3: A||B||C>2 (A=DAT_0007ad48, B=DAT_00078e08,
 *     C=DAT_000766c4)。
 *
 * 转写约定: rewritten/CONVENTIONS.md
 *   - 被调函数名一律取自 name_map; 未知 DAT_xxx 全局保留原名 + 注释。
 *   - 日志字符串原样保留; Ghidra 伪变量 (extraout, uVar, local 系, in_d0) 已消除,
 *     in_d0 一律还原为具名 double 形参。
 */

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <syslog.h>
#include <sys/time.h>

#include "../include/common_types.h"

struct work;
struct pool;
struct thr_info;

/* ==================== 基址+偏移 助记宏 (结构布局未完全还原, 偏移来自反编译) ==================== */

#define P8(p, off)   ((char *)(p) + (off))                      /* 基址 + 字节偏移 */
#define LD32(p, off) (*(uint32_t *)P8(p, off))
#define LD64(p, off) (*(uint64_t *)P8(p, off))
#define LDf64(p, off) (*(double *)P8(p, off))
#define LDs32(p, off) (*(int32_t *)P8(p, off))
#define LD8(p, off)  (*(uint8_t *)P8(p, off))
#define LDptr(p, off) (*(void **)P8(p, off))

/* struct work 字段偏移 (20_job_mining 全组一致观测) */
#define WK_HDR_SIZE      0x4c   /* 80 字节区块头 (0x00..0x4b), +0x4c 为第 20 字 */
#define WK_NONCE         0x4c   /* test_nonce_diff 递增 nonce 位置 */
#define WK_TARGET        0xa0   /* 256 位目标 (LE 存放, 0xa0 最低字) */
#define WK_HASH          0xc0   /* 256 位哈希结果 (LE 存放, 0xc0 最低字) */
#define WK_DIFF          0x748  /* double 本工作难度 (前导零位数约定) */
#define WK_DIFFACC       0x6b0  /* double 累计难度贡献 */
#define WK_SHARE_DIFF    0x6b8  /* uint64 本工作份额难度 */
#define WK_CLONE         0x6e0  /* char 克隆标志 (0x6e1 = longpoll/滚块标志, 见 discard_work) */
#define WK_ROLLTIME      0x6c0  /* int 滚块时间 */
#define WK_POOL          0x6d4  /* struct pool* */
#define WK_FOUND         0x6ea  /* char 满足块目标标志 (0x6eb = 满足网络难度标志) */
#define WK_WORKID        0x724  /* uint32 工作序号, 帧 byte[3] = id & 0x7f */
#define WK_QCTRL         0x728  /* 队列节点: +0 队列控制块指针 */
#define WK_QPREV         0x72c  /*          +4 前驱 */
#define WK_QNEXT         0x730  /*          +8 后继 */
#define WK_QHPREV        0x734  /*          +0xc 桶链前驱 */
#define WK_QHNEXT        0x738  /*          +0x10 桶链后继 */
#define WK_QHASH         0x744  /*          +0x1c 桶散列值 */
#define WK_TV            0x778  /* struct timeval 入队时刻 */

/* struct pool 字段偏移 */
#define PL_ID            0x00   /* int 池序号 */
#define PL_DIFFACC       0x28   /* uint64 累计难度贡献 */
#define PL_DIFFSTR       0x30   /* char[8] suffix_string 难度串缓冲 */
#define PL_STRATUM_ACTIVE 0x84  /* int ==1 表示 stratum 激活 (select_pool 筛选) */
#define PL_BEST_DIFF     0x190  /* uint64 池内最好份额难度 (0x194 高 32 位) */
#define PL_QUEUE_LOCK    0x140  /* pthread_rwlock_t 哈希队列锁 */
#define PL_QUEUE_HEAD    0x160  /* 队列首工作指针 */
#define PL_QUEUE_CNT     0x168  /* 队列工作计数 */
#define PL_IDLE          0x81   /* char 停机标志 */
#define PL_MIN_DIFF      0x1e8  /* double 池内最小工作难度 */
#define PL_MAX_DIFF      0x1f0  /* double 池内最大工作难度 */
#define PL_CUR_DIFF      0x1f8  /* double 当前工作难度 */
#define PL_MIN_DIFF_CNT  0x200  /* int */
#define PL_MAX_DIFF_CNT  0x204  /* int */
#define PL_NONCE1BIN     0x280  /* void* nonce1 二进制 (0x27c=0x9f*4, 见 parse_extranonce_equihash) */

/* struct cgpu_info / struct thr_info 字段偏移 */
#define CG_DRV           0x04   /* struct device_drv* */
#define CG_DEV_ID        0x08   /* int 设备序号 */
#define CG_DEVEN         0x20   /* int 0=启用/1=禁用 (enable_device 置 0, 失败置 1) */
#define CG_DIFFACC       0xc0   /* uint64 累计难度贡献 */
#define CG_LAST_WORK     0xe8   /* time_t 最近有效工作时刻 */
#define CG_HWERR         0x2c   /* int 硬件错误计数 (get_statline) */
#define CG_FAIL_TIME     0xf4   /* time_t record_device_fail 记时 */
#define CG_FAIL_REASON   0xf8   /* int 失败原因码 */
#define CG_FAIL_CNT0     0xfc   /* int[10] 按原因计数器 (0xfc..0x120) */
#define CG_SHUTDOWN      0x16c  /* char 停机标志 */
#define CG_DEV_RUNTIME   0x170  /* int 运行秒数 (cgpu_runtime 用) */
#define CG_QLOCK         0x140  /* pthread_rwlock_t */
#define THR_ID           0x00   /* int 线程序号 (thr_info) */
#define THR_CGPU         0x24   /* struct cgpu_info* */
#define THR_PAUSED       0x0f   /* char 线程暂停标志 */
#define THR_WORK_RESTART 0x3f   /* char 工作重启请求标志 (0x3e 为配套清零位) */

/* struct device_drv 字段偏移 */
#define DRV_NAME         0x08   /* char* 驱动名 */
#define DRV_STATLINE_BEF 0x14   /* get_statline 前缀钩子 */
#define DRV_STATLINE     0x18   /* get_statline 后缀钩子 */
#define DRV_HASH_WORK    0x44   /* hash_work 工作函数 */
#define DRV_FLUSH        0x50   /* 重启时加锁调用的驱动钩子 */

/* ==================== 外部函数 (名取自 name_map/cgminer.tsv) ==================== */

extern void        cgtime(struct timeval *tv);                    /* 原 FUN_0002a84c: gettimeofday 包装 */
extern double      __floatundidf(uint64_t v);                     /* 原 FUN_0005cb50: u64→double */
extern double      __floatdidf(int64_t v);                        /* 原 FUN_0005cb64: s64→double */
extern uint64_t    __fixunsdfdi(double v);                        /* 原 FUN_0005cc48: double→u64 截断 */
extern int64_t     __fixdfdi(double v);                           /* 原 FUN_0005cc10: double→s64 截断 */
extern void        suffix_string(uint64_t value, char *buf, size_t bufsz, int sig); /* 原 FUN_00011580 */
extern void        sha256_buffer(void *data, uint32_t len, void *out);  /* 原 FUN_0003049c */
extern void        sha256_compress(void *ctx, void *state, int nblocks);/* 原 FUN_000300b0 (r3/r4 残参不传) */
extern void        scrypt_N_1_256(void *data, void *scratch, void *out);/* 原 FUN_000122e8: scrypt(1024,1,1) */
extern char       *bin2hex(const void *bin, size_t len);          /* 原 FUN_00029730 */
extern int         hex2bin(void *dst, const char *hex, size_t len); /* 原 FUN_00029830 */
extern void        clean_work(struct work *w);                    /* 原 FUN_00015648 */
extern void        free_work(struct work *w);                     /* 原 FUN_00015780 */
extern void        quit(int status);                              /* 原 FUN_00016ca8 */
extern void        mutex_lock_fail_cgminer(const char *fn, int line);   /* 原 FUN_0000b414 */
extern void        wr_lock_error(const char *fn, int line);       /* 原 FUN_0000b480 */
extern void        mutex_unlock_fail_cgminer(const char *fn, int line); /* 原 FUN_0000b5c4 */
extern void        rw_unlock_error(const char *fn, int line);     /* 原 FUN_0000b630 */
extern void        rd_lock_error(const char *fn, int line);       /* 原 FUN_0000b69c */
extern void        mutex_lock_fail_util(const char *fn, int line);/* 原 FUN_0000b708 */
extern void        rwlock_init_fail_cgminer(const char *fn, int line); /* 原 FUN_0000b558 */
extern void        _mutex_lock(const char *fn, int line);         /* 原 FUN_0000b774 (api.c) */
extern void        api_rw_unlock(const char *fn, int line);       /* 原 FUN_0000b7e0 (api.c) */
extern void        _mutex_unlock(const char *fn, int line);       /* 原 FUN_0000b84c (api.c) */
extern int         curses_active_locked(void);                    /* 原 FUN_00017fe4: 加锁并返回 curses 激活态 */
extern void        unlock_curses(void);                           /* 原 FUN_00019adc */
extern void        delayed_exit_thread(void);                     /* 原 FUN_00011c1c (pthread 入口) */
extern void        mt_disable(struct thr_info *thr, int thr_id, void *drv); /* 原 FUN_00011c8c */
extern void        hashmeter(int thr_id, uint32_t sec_diff, uint64_t hashes_done); /* 原 FUN_0001bba4 */
extern void       *current_pool(void);                            /* 原 FUN_0001da58 */
extern int         total_work_inc(void);                          /* 原 FUN_0001a1e8 */
extern struct work *make_clone(struct work *w, int nonce_offset); /* 原 FUN_0001a44c */
extern void        inc_hw_errors(struct thr_info *thr);           /* 原 FUN_000189a4 */
extern bool        test_nonce(struct work *w, uint32_t nonce);    /* 原 FUN_0001654c */
extern bool        fulltest(const void *hash, const void *target);/* 原 FUN_00029f54 */
extern void        submit_work_async(struct work *w);             /* 原 FUN_000256a4 (调用点仅置 r0) */
extern void        benchfile_dspwork(struct work *w, uint32_t nonce); /* 原 FUN_00011814 */
extern char       *json_array_string(void *json_arr, unsigned int idx); /* 原 FUN_000292f4 */
extern void        mutex_unlock_util(pthread_mutex_t *m, const char *fn, int line); /* 原 FUN_00029388 */
extern double      tdiff(struct timeval *a, struct timeval *b);   /* 原 FUN_0002aa24 */
extern void        copy_timeval(struct timeval *dst, struct timeval *src); /* 原 FUN_0002a530 */
extern void        print_summary(void);                           /* 原 FUN_0001c668 */
extern void       *miner_thread_work_loop(void *arg);             /* 原 FUN_00024db0 (驱动默认值引用) */
extern void        k_alloc_items(void *klist);                    /* 原 FUN_00038b58 (调用点仅置 r0) */
extern void        _applog(uint32_t prio, char *timestamp, char *str, int force); /* 原 FUN_000389f4 */
extern uint64_t    cgpu_runtime(void *cgpu);                      /* 原 FUN_000157ec: 运行秒数 (r0/r1) */

/* ---- api.c json 输出链 ---- */
extern void       *message(void *io, int code, int params, int isjson);   /* 原 FUN_0003305c */
extern uint32_t    io_add(void *io, const char *str);             /* 原 FUN_000307c8 */
extern void       *api_add_const(void *item, const char *name, const char *value, int isjson); /* 原 FUN_00032e40 */
extern void        api_rd_lock(void *lock, const char *fn, int line); /* 原 FUN_00032190 */
extern void       *api_add_timeval(void *item, const char *name, void *tv, int isjson); /* 原 FUN_00033014 */
extern void       *api_add_string(void *item, const char *name, const char *value, int isjson); /* 原 FUN_00032e1c */
extern void       *add_bool_buf(void *item, const char *name, void *value, int isjson); /* 原 FUN_00032ff0 */
extern void       *add_diff_buf(void *item, const char *name, double *value, int isjson); /* 原 FUN_000355f8 */
extern void        print_data(void *io, void *item, int param, int isjson); /* 原 FUN_00032534 */

/* ---- jansson (hashtable 迭代, 与上游 jansson json_object_update 系列对照) ---- */
extern void       *json_object_iter(void *obj);                   /* 原 FUN_00059e9c: 首个 pair */
extern char       *json_object_iter_key(void *pair);              /* 原 FUN_00059f14: pair+0x10 (键地址) */
extern void       *json_object_iter_value(char *key);             /* 原 FUN_00059f20: 键回退 0x10 取 pair->value */
extern void       *json_object_get(void *obj, const char *key);   /* 原 FUN_00059e20 */
extern int         json_object_set_nocheck(void *obj, const char *key, void *value); /* 原 FUN_0005a820 */
extern void       *json_object_iter_next(void *obj, char *key);   /* 原 FUN_00059ee8: pair->next */

/* ---- ccan/opt ---- */
extern void        add_opt(void *entry);                          /* 原 FUN_0005b9a8 */
extern void        check_opt(void *entry);                        /* 原 FUN_0005b7c8 */

/* ---- ncurses (真实库原型, DAT_ 为窗口变量) ---- */
typedef struct win_st WINDOW;
extern WINDOW     *stdscr;
extern int         LINES;
extern WINDOW     *initscr(void);
extern WINDOW     *newwin(int nlines, int ncols, int begin_y, int begin_x);
extern int         leaveok(WINDOW *win, bool bf);
extern int         idlok(WINDOW *win, bool bf);
extern int         scrollok(WINDOW *win, bool bf);
extern int         cbreak(void);
extern int         noecho(void);
extern int         werase(WINDOW *win);
extern int         wresize(WINDOW *win, int nlines, int ncols);
extern int         mvwin(WINDOW *win, int y, int x);
extern int         wprintw(WINDOW *win, const char *fmt, ...);
extern int         wtouchln(WINDOW *win, int y, int n, int changed);
extern int         wrefresh(WINDOW *win);
extern char       *__strdup(const char *s);                       /* glibc strdup 符号 (反编译原样) */
extern int        *__errno_location(void);                        /* glibc errno 获取 */

/* ==================== 外部全局 (DAT_ 保留原名, 语义见注释) ==================== */

/* ---- 日志门控 (与 uart.c/hash.c 同一组全局) ---- */
extern char    DAT_0007ad48;   /* 门控 A; applog 内非 0 改走 syslog() */
extern char    DAT_00078e08;   /* 门控 B (curses Verbose 联动) */
extern int     DAT_000766c4;   /* 日志级别, 门控条件 C */
extern char    DAT_00078e09;   /* opt_debug (Debug mode), 门控 7/6 前置 */

/* ---- 模式开关 ---- */
extern char    DAT_0007b859;   /* opt_scrypt: 1 → diff1=65536/0xFFFF00000000, 0 → 2^224/0xFFFF0000 */

/* ---- should_run 停机时段窗口 ---- */
extern char    DAT_0007b874;   /* 停机等待中标志 (到时自动清零) */
extern char    DAT_0007b5f0;   /* 停机时段配置标志 */
extern int     DAT_0007b5fc;   /* 停机时段起始时 (hour) */
extern int     DAT_0007b5f8;   /* 停机时段起始分 (min) */
extern int     DAT_0007b880;   /* 停机时段结束时 (hour) */
extern int     DAT_0007b87c;   /* 停机时段结束分 (min) */

/* ---- 全局统计 ---- */
extern uint32_t DAT_0007b7c8;  /* 总丢弃工作计数低 32 位 (与 DAT_0007b7cc 组成 u64) */
extern uint32_t DAT_0007b7cc;  /* 总丢弃工作计数高 32 位 */
extern uint64_t DAT_0007b100;  /* 总累计难度贡献低 32 位 (u64, 与 DAT_0007b104 组成) */
extern uint32_t DAT_0007b104;  /* 总累计难度贡献高 32 位 */
extern int     DAT_0007b8b8;   /* 找到块计数 (update_work_stats 自增) */
extern int     DAT_00075dcc;   /* get_statline 显示秒数 / hash_driver_work 上报间隔阈值 */
extern double  DAT_00075de0;   /* 网络难度 (minecoin "Network Difficulty" 同源) */
extern char    DAT_00075dc4;   /* 全局最好份额串缓冲 (suffix_string 写入 8 字节) */
extern uint32_t DAT_00076f18;  /* 全局最好份额难度低 32 位 */
extern uint32_t DAT_00076f1c;  /* 全局最好份额难度高 32 位 */

/* ---- 池/设备表 ---- */
extern int     DAT_0007b198;   /* total_pools */
extern int    *DAT_0007b438;   /* pools 指针数组 */
extern void   *DAT_0007b628;   /* devices 数组 (cgpu_info* 槽) */
extern int     DAT_00078720;   /* 已使能设备计数 (total_devices) */
extern int     DAT_00078724;   /* 追加段设备权重累加 (enable_device) */
extern int     DAT_0007bb6c;   /* 常规段设备权重累加 (enable_device) */
extern char    DAT_0007b7d0;   /* 追加段模式标志 */
extern void   *DAT_0007704c;   /* 全局工作队列互斥锁指针 (clear_pool_work 用) */
extern void   *DAT_00077050;   /* 全局工作队列首工作指针 */

/* ---- 锁 ---- */
extern pthread_mutex_t DAT_0007ad4c;  /* hash_lock (统计互斥) */
extern pthread_mutex_t DAT_0007ba10;  /* share_diff 互斥锁 */
extern pthread_rwlock_t DAT_0007ba28; /* share_diff 读写锁 */
extern pthread_mutex_t DAT_0007b85c;  /* hash_driver_work 重启钩子锁 */
extern pthread_mutex_t DAT_0007b9d8;  /* api.c 互斥锁 */
extern pthread_rwlock_t DAT_0007b9f0; /* api.c 读写锁 */
extern pthread_rwlock_t DAT_0007b700; /* devices_lock */

/* ---- curses ---- */
extern char    DAT_00075dd0;   /* curses_active (check_winsizes 前置) */
extern char    DAT_00077018;   /* curses 已初始化标志 */
extern int     DAT_0007701c;   /* status_win 行数配置 */
extern int     DAT_00077020;   /* status_win 实际行数 */
extern int     DAT_00077024;   /* 日志窗口起始行 (log cursor) */
extern WINDOW *DAT_0007b7c0;   /* status_win */
extern WINDOW *DAT_0007b430;   /* log_win */
extern WINDOW *DAT_0007bc90;   /* main/stdscr 窗口 */
extern pthread_mutex_t DAT_0007b7a8;  /* curses 互斥锁 */
extern char    DAT_0007bac8;   /* 显示挂起标志 (log_curses_only 内判定) */

/* ---- 通用尾部钩子 ---- */
extern void  (*DAT_00075ed8)(void);   /* noop 钩子 (各函数尾部经指针调用, 初值恒 0 返回) */

/* ---- 退出/基准文件 ---- */
extern int     DAT_0007728c;   /* 需 kill 的子进程 pid */
extern char    DAT_0007bb18;   /* 跳过 print_summary 标志 */
extern char    DAT_00078ca9;   /* 允许 print_summary 标志 */
extern struct timeval DAT_00077148;   /* 退出时刻记录 */
extern int     DAT_00076ffc;   /* benchfile 使能计数 */
extern char    DAT_0007871c;   /* benchfile 使能标志 */

/* ---- 日志文件 ---- */
extern char    DAT_00078ce0;   /* 日志文件使能标志 */
extern FILE   *DAT_00078ce4;   /* 日志文件句柄 */
extern char    DAT_00078ce8[]; /* 日志文件路径 (fopen 实参) */
extern char    DAT_00078de8[]; /* fopen 模式串 */

/* ---- minecoin / api ---- */
extern struct timeval DAT_0007b070;   /* "Current Block Time" */
extern char    DAT_0007bb28[]; /* "Current Block Hash" 缓冲 */
extern char    DAT_00062288[]; /* add_bool_buf 字段名串 (rodata) */
extern char    DAT_0007bc79;   /* add_bool_buf 布尔值 */
extern void   *DAT_0009d77c;   /* opt_table 选项表指针 */
extern int     DAT_0009d774;   /* opt_count 选项表项数 */

/* ---- bm1740 帧校验 ---- */
extern const uint16_t DAT_00068aec[256]; /* CRC16-CCITT 半字节表 (rodata 0x68AEC) */

/* ---- fill_drv_defaults 默认桩 (代码段小桩, Ghidra 以 DAT/LAB 标注) ---- */
extern void    DAT_0000f794;   /* 默认桩 1 (返回 0 型) */
extern void    DAT_0000f798;   /* 默认桩 2 */
extern void    DAT_0000f79c;   /* 默认桩 3 */
extern void    DAT_0000f7a0;   /* 默认桩 4 */
extern void    DAT_0000f7a8;   /* 默认桩 5 */
extern void    DAT_0000f7b0;   /* 默认桩 6 */
extern void    DAT_0000f7bc;   /* 默认桩 7 */
extern void    DAT_0000f7c4;   /* 默认桩 8 */
extern void    DAT_0000f7cc;   /* 默认桩 9 */
extern void    DAT_0000f7d0;   /* 默认桩 10 */
extern void    DAT_0000f7d4;   /* 默认桩 11 */
extern void    DAT_0000f7d8;   /* 默认桩 12 */

/* ==================== 本文件内的互调前置声明 ==================== */

/* 互调原语: get_bit (原 FUN_00042958) 与 leading_zeros_256bit (原 FUN_00049824),
 * 实现见文末"补充 (job_b5 续作)"区段 */
uint32_t get_bit(const uint8_t *data, int bit_index);
int leading_zeros_256bit(const void *v256);
void applog(uint32_t prio, char *buf, int force);
void record_device_fail(void *cgpu, int reason);

/* 本文件函数供他段引用的前向声明 */
void discard_work(struct work *w);
void update_work_stats(void *thr, struct work *w);
uint64_t share_diff(struct work *w);
void hash_driver_work(struct thr_info *thr);
void __quit(int status, int wait_for_threads);
struct work *make_work(void);
bool tq_push(void *tq, void *data);

/* ==================== 组 A: work 构造 / 难度计算 / hash 再生成 ==================== */

static inline uint32_t bswap32(uint32_t v)   /* 字节序翻转 (机器码 e6bf3f33 REV) */
{
    return v << 24 | (v >> 8 & 0xff) << 16 | (v >> 16 & 0xff) << 8 | v >> 24;
}

// === should_run === 原 FUN_000118d4 @ 0x000118d4
// 来源: reversed/cgminer/20_job_mining/FUN_000118d4.c | 分类: 20_job_mining
// 语义: 判断当前时刻是否允许挖矿运行; 停机时段 [DAT_0007b5fc:b5f8, DAT_0007b880:b87c)
//       内返回 0, 时段外返回 1; 仅停机等待标志置位时到点自动清零并恢复运行。
// 返回: 1=继续运行, 0=处于停机时段。
int should_run(void)
{
    struct timeval tv;
    time_t now;
    struct tm *tmnow;
    int hour;

    if ((DAT_0007b874 == '\0') && (DAT_0007b5f0 == '\0')) {
        return 1;                               /* 无停机配置/等待 → 直接允许 */
    }
    cgtime(&tv);
    now = tv.tv_sec;
    tmnow = localtime(&now);
    hour = tmnow->tm_hour;

    if (DAT_0007b874 == '\0') {
        /* 仅配置了时段 (DAT_0007b5f0): 起始时刻之前允许运行 (LAB_00011980), 否则停机 */
        if ((hour < DAT_0007b5fc) ||
            ((hour == DAT_0007b5fc) && (tmnow->tm_min < DAT_0007b5f8))) {
            return 1;
        }
    }
    else if (DAT_0007b5f0 == '\0') {
        /* 停机等待中 (DAT_0007b874 置位): 到达结束时刻自动清除等待标志并恢复 */
        if ((DAT_0007b880 <= hour) &&
            ((hour != DAT_0007b880) || (DAT_0007b87c <= tmnow->tm_min))) {
            DAT_0007b874 = 0;
            return 1;
        }
    }
    else {
        if ((DAT_0007b5fc <= DAT_0007b880) &&
            ((DAT_0007b880 != DAT_0007b5fc) || (DAT_0007b5f8 <= DAT_0007b87c))) {
            /* 时段不跨午夜 [start, end): 窗口内停机, 窗口外运行 */
            hour = tmnow->tm_hour;
            if (hour < DAT_0007b880) {
                if (DAT_0007b5fc <= hour) {
                    /* LAB_00011a24: 起始整点需分钟已过起始分 */
                    if ((DAT_0007b5fc != hour) || (DAT_0007b5f8 <= tmnow->tm_min)) {
                        return 0;
                    }
                }
                return 1;
            }
            else if ((DAT_0007b880 == hour) && (tmnow->tm_min < DAT_0007b87c)) {
                if ((DAT_0007b5fc != hour) || (DAT_0007b5f8 <= tmnow->tm_min)) {
                    return 0;
                }
            }
            return 1;
        }
        /* 时段跨午夜: [end, start) 之外为窗口内 → 停机 */
        hour = tmnow->tm_hour;
        if ((((hour < DAT_0007b5fc) ||
              ((DAT_0007b5fc == hour) && (tmnow->tm_min < DAT_0007b5f8))) &&
             (DAT_0007b880 <= hour)) &&
            ((DAT_0007b880 != hour) || (DAT_0007b87c <= tmnow->tm_min))) {
            return 1;
        }
    }
    return 0;                                   /* LAB_000119a4 */
}

// === calc_diff === 原 FUN_00011a5c @ 0x00011a5c
// 来源: reversed/cgminer/20_job_mining/FUN_00011a5c.c | 分类: 20_job_mining
// 语义: 计算工作难度: 已知难度为 0 时按目标 256 位折算 diff1/target (并先写入),
//       随后统一以目标前导零位数 (double) 覆盖 work->diff 与 pool->当前难度,
//       维护 pool 最小/最大难度及计数, 并用 suffix_string 刷新池难度串。
void calc_diff(struct work *work, double known_diff)
{
    void *pool = LDptr(work, WK_POOL);
    double diff1;
    double target_val;
    double diff_val;
    double cur;

    if (known_diff == 0.0) {
        diff1 = 65536.0;                        /* opt_scrypt 分支的 diff1 */
        if (DAT_0007b859 == '\0') {
            diff1 = 2.695953529101131e+67;      /* 2^224 (equihash diff1) */
        }
        /* 目标 256 位 (LE, work+0xa0..0xbf) 折算 double: 4×u64 加权累加 */
        target_val = (double)__floatundidf(LD64(work, 0xb8)) * 6.277101735386681e+57 +  /* 2^192 */
                     (double)__floatundidf(LD64(work, 0xb0)) * 3.402823669209385e+38 +  /* 2^128 */
                     (double)__floatundidf(LD64(work, 0xa8)) * 1.8446744073709552e+19 +  /* 2^64 */
                     (double)__floatundidf(LD64(work, 0xa0));
        if (target_val == 0.0) {
            target_val = 1.0;
        }
        LDf64(work, WK_DIFF) = diff1 / target_val;      /* 首次写入 (后续被覆盖, -O0 保留双写) */
    }
    else {
        LDf64(work, WK_DIFF) = known_diff;
    }

    /* 本固件难度约定: 以目标前导零位数为准 (覆盖前一步结果) */
    int lz = leading_zeros_256bit(P8(work, WK_TARGET));
    diff_val = (double)(int32_t)lz;             /* VCVT 整数→双精度 (FPSCR 舍入位对 0..255 无影响) */
    LDf64(work, WK_DIFF) = diff_val;
    LDf64(pool, PL_CUR_DIFF) = diff_val;
    /* 池难度串: 先 round 再转 u64 (机器码 round→__fixunsdfdi→suffix_string) */
    suffix_string((uint64_t)round(diff_val), P8(pool, PL_DIFFSTR), 8, 0);

    /* 池最小难度及计数: 与最小值相等则计数++; 小于最小值或最小值仍为 0 则刷新 */
    cur = LDf64(pool, PL_MIN_DIFF);
    if (diff_val == cur) {
        LDs32(pool, PL_MIN_DIFF_CNT) += 1;
    }
    else if ((diff_val < cur) || (cur == 0.0)) {
        LDf64(pool, PL_MIN_DIFF) = diff_val;
        LDs32(pool, PL_MIN_DIFF_CNT) = 1;
    }
    /* 池最大难度及计数 */
    cur = LDf64(pool, PL_MAX_DIFF);
    if (diff_val == cur) {
        LDs32(pool, PL_MAX_DIFF_CNT) += 1;
    }
    else if (cur < diff_val) {
        LDf64(pool, PL_MAX_DIFF) = diff_val;
        LDs32(pool, PL_MAX_DIFF_CNT) = 1;
    }
}

// === regen_hash === 原 FUN_00011f60 @ 0x00011f60
// 来源: reversed/cgminer/20_job_mining/FUN_00011f60.c | 分类: 20_job_mining
// 语义: 用 work 头 80 字节重新生成 double-sha256 存回 work->hash (+0xc0),
//       调试门控下打印 "regen:<hex>"。
void regen_hash(struct work *work)
{
    uint32_t data[20];                  /* 0x50 字节: 头部逐字字节序翻转 */
    uint8_t midstate[32];
    char logbuf[4096];
    int i;
    uint32_t w;

    for (i = 0; i < 0x50; i += 4) {
        w = LD32(work, i);
        *(uint32_t *)P8(data, i) = bswap32(w);
    }
    sha256_buffer(data, 0x50, midstate);
    sha256_buffer(midstate, 0x20, P8(work, WK_HASH));
    char *hex = bin2hex(P8(work, WK_HASH), 0x20);
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
        snprintf(logbuf, 0x1000, "regen:%s", hex);
        applog(5, logbuf, 0);
    }
    free(hex);
}

// === select_pool === 原 FUN_00012250 @ 0x00012250
// 来源: reversed/cgminer/20_job_mining/FUN_00012250.c | 分类: 20_job_mining
// 语义: 在全部池中挑选 +0xbc 计数最小者 (跳过停机/stratum 未激活/断线重连中的池),
//       选中池的计数 +1; 入参 pool 为初始候选 (通常当前池)。
void select_pool(void *pool)
{
    int best;
    int **pp;
    int **end;
    void *p;

    best = LDs32(pool, 0xbc);           /* 选择权重计数 (语义待定) */
    if (0 < DAT_0007b198) {
        pp = (int **)DAT_0007b438;
        end = (int **)(DAT_0007b438 + DAT_0007b198);
        for (; pp != end; pp++) {
            p = *pp;
            /* 跳过: 停机(+0x81) / stratum 未激活(+0x84!=1) / 断线重连中(+0x298 且非 +0x299) */
            if (((LD8(p, 0x81) != '\0') || (LDs32(p, 0x84) != 1)) ||
                ((LD8(p, 0x298) != '\0') && (LD8(p, 0x299) == '\0'))) {
                continue;
            }
            if (LDs32(p, 0xbc) < best) {
                best = LDs32(p, 0xbc);
                pool = p;
            }
        }
        best = LDs32(pool, 0xbc);       /* LAB_000122cc: 复读选中池的计数 */
    }
    LDs32(pool, 0xbc) = best + 1;       /* 选中池计数自增 */
}

// === rebuild_nonce === 原 FUN_00014f30 @ 0x00014f30
// 来源: reversed/cgminer/20_job_mining/FUN_00014f30.c | 分类: 20_job_mining
// 语义: opt_scrypt 路径的哈希再生成: 头 80 字节逐字翻转 → scrypt(1024,1,1)
//       (栈上 0x20250 字节刮擦区) → 结果 8 字回写字节序到 work->hash。
// 注意: 原函数栈帧 0x20250 字节, scratch 数组长度与机器码一致。
void rebuild_nonce(struct work *work)
{
    uint8_t data[0x50];                 /* sp+0x00: 翻转后的 80 字节头 */
    uint8_t scrypt_scratch[0x20200];    /* sp+0x50: 4 字节零字 + 0x201fc 刮擦区 */
    int i;
    uint32_t w;

    memset(scrypt_scratch, 0, sizeof(scrypt_scratch));  /* 机器码: [sp+0x50]=0 + memset(+0x54,0,0x201fc) */
    for (i = 0; i < 0x4c; i += 4) {
        w = LD32(work, i);
        *(uint32_t *)P8(data, i) = bswap32(w);
    }
    *(uint32_t *)P8(data, 0x4c) = bswap32(LD32(work, 0x4c));    /* 头部第 20 字 */
    scrypt_N_1_256(data, scrypt_scratch, P8(work, WK_HASH));
    for (i = 0; i < 8; i++) {           /* 结果 8 字回写: 哈希从 +0xc0 到 +0xe0 */
        w = LD32(work, WK_HASH + i * 4);
        LD32(work, WK_HASH + i * 4) = bswap32(w);
    }
}

// === pretest_hash === 原 FUN_00014fd0 @ 0x00014fd0
// 来源: reversed/cgminer/20_job_mining/FUN_00014fd0.c | 分类: 20_job_mining
// 语义: 快速预检: 头 80 字节翻转 (nonce 为第 20 字) → scrypt → 取哈希首字
//       (字节序翻转): 首字 < 0x10000 且 target_src+0x1c 的目标字 >= 哈希首字
//       返回 1 (通过), 目标字 < 哈希首字返回 0, 否则返回 -1 (哈希首字过大)。
// 参数: hdr_src 提供前 0x4c 字节头; target_src 的 +0x1c 为目标低字 ("htarget")。
int pretest_hash(void *hdr_src, void *target_src, uint32_t nonce)
{
    uint8_t scrypt_scratch[131592];     /* auStack_21288 */
    uint32_t data[20];                  /* local_1060..local_1014: 0x50 字节翻转头 */
    uint32_t hash[8];                   /* auStack_1080(+local_1064): 32 字节输出 */
    char logbuf[4096];
    uint32_t target_word = LD32(target_src, 0x1c);
    uint32_t hash_word0;
    int i;
    uint32_t w;

    for (i = 0; i < 0x4c; i += 4) {
        w = LD32(hdr_src, i);
        *(uint32_t *)P8(data, i) = bswap32(w);
    }
    *(uint32_t *)P8(data, 0x4c) = bswap32(nonce);   /* 第 20 字 = nonce */
    scrypt_N_1_256(data, scrypt_scratch, hash);
    hash_word0 = bswap32(hash[0]);
    if ((DAT_00078e09 != '\0') &&
        (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4))) {
        snprintf(logbuf, 0x1000, "htarget %08lx diff1 %08lx hash %08lx",
                 (unsigned long)target_word, (unsigned long)0xffff, (unsigned long)hash_word0);
        applog(7, logbuf, 0);
    }
    if (hash_word0 < 0x10000) {
        if (target_word < hash_word0) {
            return 0;                   /* 哈希首字超过目标 → 未通过 */
        }
        return 1;                       /* 通过 */
    }
    return -1;                          /* 哈希首字 ≥ 0x10000, 不再比较 */
}

// === copy_work === 原 FUN_00015684 @ 0x00015684
// 来源: reversed/cgminer/20_job_mining/FUN_00015684.c | 分类: 20_job_mining
// 语义: 复制工作 (0x7b0 字节) 但保留目标 workid (帧 byte[3]=id&0x7f 映射依赖),
//       重 strdup 各字符串字段; 带 nonce_offset 时 BE 的 nonce2 (+0x44) 与其
//       hex 串表示 (+0x704) 同步加偏移。
void copy_work(struct work *dst, struct work *src, int nonce_offset)
{
    uint32_t workid = LD32(dst, WK_WORKID);
    uint32_t n2;
    uint32_t v;

    clean_work(dst);                    /* 机器码 r0 直通: clean_work(dst) */
    memcpy(dst, src, 0x7b0);
    LD32(dst, WK_WORKID) = workid;
    if (LDs32(src, 0x6f0) != 0) {
        LDptr(dst, 0x6f0) = __strdup(LDptr(src, 0x6f0));
    }
    if (LDs32(src, 0x710) != 0) {
        LDptr(dst, 0x710) = __strdup(LDptr(src, 0x710));
    }
    if (LDs32(src, 0x704) == 0) {
        if (nonce_offset != 0) {
            v = bswap32(LD32(dst, 0x44)) + (uint32_t)nonce_offset;      /* +0x44 为 BE nonce2 */
            LD32(dst, 0x44) = bswap32(v);
        }
    }
    else if (nonce_offset == 0) {
        LDptr(dst, 0x704) = __strdup(LDptr(src, 0x704));
    }
    else {
        v = bswap32(LD32(dst, 0x44)) + (uint32_t)nonce_offset;
        LD32(dst, 0x44) = bswap32(v);
        /* nonce2 的 hex 串表示同步偏移 (hex2bin 长度实参 4, 由 bin2hex(4) 核实) */
        hex2bin(&n2, LDptr(src, 0x704), 4);
        n2 = bswap32(bswap32(n2) + (uint32_t)nonce_offset);
        LDptr(dst, 0x704) = bin2hex(&n2, 4);
    }
    if (LDs32(src, 0x718) != 0) {
        LDptr(dst, 0x718) = __strdup(LDptr(src, 0x718));
    }
}
/* ==================== 组 B: nonce/变长编码/哈希队列节点/统计行 ==================== */

// === diff_from_nonce2 === 原 FUN_000158e4 @ 0x000158e4
// 来源: reversed/cgminer/20_job_mining/FUN_000158e4.c | 分类: 20_job_mining
// 语义: 按哈希 (work+0xc0..0xdf, LE 4×u64) 折算份额难度 = diff1/哈希值,
//       返回 u64 截断; work 为空或哈希为 0 返回 0。
// 注: 上游名待考 [低置信: name_map 标注]。
uint64_t diff_from_nonce2(struct work *work)
{
    double diff1;
    double hashval;
    uint64_t ret = 0;

    if (work != NULL) {
        diff1 = 1.7668201048317172e+72;         /* 2^240 (opt_scrypt 分支) */
        if (DAT_0007b859 == '\0') {
            diff1 = 2.695953529101131e+67;      /* 2^224 */
        }
        hashval = (double)__floatundidf(LD64(work, 0xd8)) * 6.277101735386681e+57 +  /* 2^192 */
                  (double)__floatundidf(LD64(work, 0xd0)) * 3.402823669209385e+38 +  /* 2^128 */
                  (double)__floatundidf(LD64(work, 200)) * 1.8446744073709552e+19 +  /* 0xc8, 2^64 */
                  (double)__floatundidf(LD64(work, 0xc0));
        if (hashval != 0.0) {
            ret = (uint64_t)__fixunsdfdi(diff1 / hashval);
        }
    }
    return ret;
}

// === discard_work === 原 FUN_000159b4 @ 0x000159b4
// 来源: reversed/cgminer/20_job_mining/FUN_000159b4.c | 分类: 20_job_mining
// 语义: 释放工作; 非克隆/滚块工作先回填池队列计数并累加全局 total_discarded
//       (u64, DAT_0007b7c8/7cc), 否则调试门控下打印 "Discarded cloned or rolled work"。
void discard_work(struct work *work)
{
    char logbuf[4096];
    void *pool;
    uint32_t old_lo;

    if (((LD8(work, WK_CLONE + 1) == '\0') && (LDs32(work, WK_ROLLTIME) == 0)) &&
        (LD8(work, WK_CLONE) == '\0')) {
        pool = LDptr(work, WK_POOL);
        if (pool != NULL) {
            LDs32(pool, 0x9c) += 1;             /* 池队列历史计数 */
            LDs32(pool, 0x40) -= 1;             /* 池在途工作计数 -1 */
            LDs32(pool, 0x44) -= 1;             /* 池在途工作计数 2 -1 */
        }
        old_lo = DAT_0007b7c8;                  /* total_discarded u64 自增 (32 位进位) */
        DAT_0007b7c8 = old_lo + 1;
        if (old_lo > 0xfffffffeu) {             /* 机器码: 0xfffffffe < 旧值 */
            DAT_0007b7cc += 1;
        }
    }
    else if ((DAT_00078e09 != '\0') &&
             (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4))) {
        snprintf(logbuf, 0x1000, "Discarded cloned or rolled work");
        applog(7, logbuf, 0);
    }
    free_work(work);
}

// === set_target === 原 FUN_00016244 @ 0x00016244
// 来源: reversed/cgminer/20_job_mining/FUN_00016244.c | 分类: 20_job_mining
// 语义: 按难度 diff 生成 256 位目标 (LE 8×u32, 存 target[0..3] 4×u64):
//       逐级 2^192/2^128/2^64 拆分 (fixunsdfdi + floatundidf 回代消元);
//       diff 为 0 时报 "Diff zero passed to set_target" 并按 1 处理;
//       opt_debug 下打印 "Generated target %s"。
void set_target(uint64_t *target, double diff)
{
    char logbuf[4096];
    double diff1;
    double scaled;
    double rem;
    double tmp;
    uint64_t w[4];                      /* w[0]..w[3] = 目标 8 个 LE 字, bin2hex 按此序 */
    char *hex;

    if (diff == 0.0) {
        if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
            diff = 1.0;                 /* 非调试模式静默置 1 */
        }
        else {
            snprintf(logbuf, 0x1000, "Diff zero passed to set_target");
            applog(3, logbuf, 0);
            diff = 1.0;
        }
    }
    diff1 = 1.7668201048317172e+72;     /* 2^240 (opt_scrypt 分支) */
    if (DAT_0007b859 == '\0') {
        diff1 = 2.695953529101131e+67;  /* 2^224 */
    }
    scaled = diff1 / diff;
    /* 字 6/7 (最高): scaled*2^-192 取整, 再回代消元 */
    w[3] = __fixunsdfdi(scaled * 1.5930919111324523e-58);           /* 2^-192 */
    tmp = __floatundidf(w[3]);          /* (double)w3 (r0/r1 直通, 反汇编 0x16294 核实) */
    rem = scaled - tmp * 6.277101735386681e+57;                     /* 2^192 */
    /* 字 4/5 */
    w[2] = __fixunsdfdi(rem * 2.938735877055719e-39);               /* 2^-128 */
    tmp = __floatundidf(w[2]);
    rem = rem - tmp * 3.402823669209385e+38;                        /* 2^128 */
    /* 字 2/3 */
    w[1] = __fixunsdfdi(rem * 5.421010862427522e-20);               /* 2^-64 */
    tmp = __floatundidf(w[1]);
    rem = rem - tmp * 1.8446744073709552e+19;                       /* 2^64 */
    /* 字 0/1 (最低) */
    w[0] = __fixunsdfdi(rem);
    if (DAT_00078e09 != '\0') {         /* opt_debug: 打印 32 字节十六进制目标 */
        hex = bin2hex(&w[0], 0x20);
        if ((DAT_00078e09 != '\0') &&
            (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4))) {
            snprintf(logbuf, 0x1000, "Generated target %s", hex);
            applog(7, logbuf, 0);
        }
        free(hex);
    }
    target[0] = w[0];                   /* param_1[0..3] 依次存放 (LE 8×u32) */
    target[1] = w[1];
    target[2] = w[2];
    target[3] = w[3];
}

// === varint_encode === 原 FUN_00016494 @ 0x00016494
// 来源: reversed/cgminer/20_job_mining/FUN_00016494.c | 分类: 20_job_mining
// 语义: Bitcoin 变长整数编码到 p: <0xfd 直接 1 字节; <0x10000 前缀 0xfd + 2 字节 LE;
//       32 位内前缀 0xfe + 4 字节 LE; 更大前缀 0xff + 8 字节 LE (param_4 为高位字)。
// 返回: 编码总字节数。param_2 未被函数体使用 (Ghidra 观测保留)。
int varint_encode(uint8_t *p, int param_2, uint32_t value_lo, int value_hi)
{
    size_t n;
    int ret;
    uint64_t packed;

    if (value_hi == 0 && value_lo < 0xfd) {
        *p = (uint8_t)value_lo;
        ret = 1;
        n = 0;
    }
    else {
        bool small = (value_hi == 0);
        if (value_hi == 0) {
            small = value_lo < 0x10000;
        }
        if (small) {
            ret = 3;
            *p = 0xfd;
            n = 2;
        }
        else if (value_hi == 0) {
            ret = 5;
            n = 4;
            *p = 0xfe;
        }
        else {
            ret = 9;
            n = 8;
            *p = 0xff;
        }
    }
    /* 机器码: local_18=lo, iStack_14=hi 相邻, memcpy 前缀后的 n 字节 (LE) */
    packed = (uint64_t)value_lo | ((uint64_t)(uint32_t)value_hi << 32);
    memcpy(p + 1, &packed, n);
    return ret;
}

// === test_nonce_diff === 原 FUN_000165a0 @ 0x000165a0
// 来源: reversed/cgminer/20_job_mining/FUN_000165a0.c | 分类: 20_job_mining
// 语义: 以 diff 阈值测试 nonce: 写入 work+0x4c → regen_hash/rebuild_nonce 重算哈希
//       → 阈值 u64 = diff1/diff (diff1 随 opt_scrypt 取 0xFFFF00000000/0xFFFF0000),
//       哈希首 64 位 (work+0xd8) <= 阈值即通过。
bool test_nonce_diff(struct work *work, uint32_t nonce, double diff)
{
    uint64_t threshold;
    uint32_t thr_hi;
    bool ok;

    LD32(work, WK_NONCE) = nonce;       /* +0x4c: 哈希第 20 字写入 nonce */
    if (DAT_0007b859 == '\0') {
        regen_hash(work);               /* 机器码 r0 直通 */
    }
    else {
        rebuild_nonce(work);
    }
    double diff1 = 281470681743360.0;   /* 0xFFFF00000000 (opt_scrypt 分支) */
    if (DAT_0007b859 == '\0') {
        diff1 = 4294901760.0;           /* 0xFFFF0000 */
    }
    threshold = (uint64_t)__fixunsdfdi(diff1 / diff);
    thr_hi = (uint32_t)(threshold >> 32);
    ok = LD32(work, 0xdc) <= thr_hi;    /* 哈希首 u64 高 32 位比较 */
    if (thr_hi == LD32(work, 0xdc)) {
        ok = LD32(work, 0xd8) <= (uint32_t)threshold;   /* 高位相等再比低 32 位 */
    }
    return ok;
}

// === __work_completed === 原 FUN_00016b40 @ 0x00016b40
// 来源: reversed/cgminer/20_job_mining/FUN_00016b40.c | 分类: 20_job_mining
// 语义: 从 thr 的哈希队列 (+0x160 队首/+0x168 计数) 摘除 work: 双向链摘除、
//       桶链 (节点 +0x734/+0x738, 桶槽 0xc 字节) 摘除并递减各计数;
//       队列将空时释放桶数组与队列控制块并清空队首指针。
void __work_completed(void *thr, struct work *work)
{
    void *q_head;
    void *ctrl;
    void *prev;
    long slot_off;
    void *bucket;

    LDs32(thr, PL_QUEUE_CNT) -= 1;
    prev = LDptr(work, WK_QPREV);
    if ((prev == NULL) && (LDptr(work, WK_QNEXT) == NULL)) {
        /* 队列将空: 释放桶数组与控制块 */
        free(*(void **)LDptr(LDptr(thr, PL_QUEUE_HEAD), WK_QCTRL));
        free(LDptr(LDptr(thr, PL_QUEUE_HEAD), WK_QCTRL));
        LDptr(thr, PL_QUEUE_HEAD) = NULL;
        return;
    }
    ctrl = LDptr(LDptr(thr, PL_QUEUE_HEAD), WK_QCTRL);
    if ((uintptr_t)LDptr(ctrl, 0x10) - (uintptr_t)LDptr(ctrl, 0x14) == (uintptr_t)work) {
        /* work 为队首: 头指针改为 前驱+基址 (ctrl+0x14 为节点基址常量) */
        *(void **)P8(ctrl, 0x10) = (char *)prev + *(int *)P8(ctrl, 0x14);
        prev = LDptr(work, WK_QPREV);
    }
    if (prev != NULL) {
        /* 前驱->next(+8) = 后继 (前驱节点 = prev + 基址) */
        void *pprev = (char *)prev + *(int *)P8(LDptr(LDptr(thr, PL_QUEUE_HEAD), WK_QCTRL), 0x14);
        *(void **)P8(pprev, 8) = LDptr(work, WK_QNEXT);
    }
    else {
        LDptr(thr, PL_QUEUE_HEAD) = LDptr(work, WK_QNEXT);      /* work 为队首 */
    }
    q_head = LDptr(thr, PL_QUEUE_HEAD);         /* Ghidra 寄存器复用点: prev!=0 时复读, prev==0 时为新队首 */
    if (LDptr(work, WK_QNEXT) != NULL) {
        /* 后继->prev(+4) = 前驱 */
        *(void **)P8((char *)LDptr(work, WK_QNEXT) + *(int *)P8(LDptr(q_head, WK_QCTRL), 0x14), 4) = prev;
    }
    /* 桶链摘除: 槽位 = ((ctrl[1]-1) & work->hash) * 0xc */
    ctrl = LDptr(q_head, WK_QCTRL);
    slot_off = (long)((*(uint32_t *)P8(ctrl, 4) - 1U) & LD32(work, WK_QHASH)) * 0xc;
    bucket = *(void **)P8(ctrl, 0);             /* ctrl[0] = 桶数组基址 */
    LDs32(bucket, slot_off + 4) -= 1;            /* 槽计数 -1 */
    bucket = *(void **)P8(LDptr(thr, PL_QUEUE_HEAD), WK_QCTRL);
    bucket = *(void **)P8(bucket, 0);
    if (*(void **)P8(bucket, slot_off) == (char *)work + WK_QCTRL) {
        *(void **)P8(bucket, slot_off) = LDptr(work, WK_QHNEXT);        /* 桶首即本节点 → 前移 */
    }
    if (LDptr(work, WK_QHPREV) != NULL) {
        *(void **)P8(LDptr(work, WK_QHPREV), 0x10) = LDptr(work, WK_QHNEXT);
    }
    if (LDptr(work, WK_QHNEXT) != NULL) {
        *(void **)P8(LDptr(work, WK_QHNEXT), 0xc) = LDptr(work, WK_QHPREV);
    }
    ctrl = LDptr(LDptr(thr, PL_QUEUE_HEAD), WK_QCTRL);
    LDs32(ctrl, 0xc) -= 1;                       /* 队列总数 -1 */
}

// === get_statline === 原 FUN_00017744 @ 0x00017744
// 来源: reversed/cgminer/20_job_mining/FUN_00017744.c | 分类: 20_job_mining
// 语义: 生成设备状态行: "驱动名+序号 " → 驱动 statline_before 钩子 →
//       "(运行秒s):(已接受难度) (avg):(平均哈希)h/s | A:.. R:.. HW:.. WU:../m"
//       → 追加 → 驱动 statline 钩子; 缓冲不足则报错退出。
void get_statline(char *buf, size_t bufsz, void *cgpu)
{
    char suffix_avg[16];                /* acStack_1098: (avg) 后缀串 */
    char suffix_acc[16];                /* acStack_1088: 已接受难度后缀串 */
    char statline_buf[64];              /* acStack_1078: 状态行格式化缓冲 */
    char logbuf[4100];                  /* acStack_1038 */
    uint64_t runtime;
    double accepted_f64;
    uint64_t accepted;
    uint64_t avg_u64;
    double wu_per_min;
    size_t len1;
    size_t len2;
    void *drv;
    void (*statline_before)(char *, size_t, void *);

    runtime = cgpu_runtime(cgpu);
    accepted_f64 = (double)__floatdidf((int64_t)LD64(cgpu, 0xc0));      /* +0xc0 u64 已接受难度→double (WU 用) */
    accepted = __fixunsdfdi(LDf64(cgpu, 0x30));         /* +0x30 double 已接受难度→u64 截断 (后缀串用) */
    avg_u64 = __fixunsdfdi(LDf64(cgpu, 0x50) / (double)runtime);        /* +0x50 double 平均 MH/s ÷ 运行秒 */
    suffix_string(avg_u64, suffix_avg, sizeof(suffix_avg), 4);
    suffix_string(accepted, suffix_acc, sizeof(suffix_acc), 4);
    drv = LDptr(cgpu, CG_DRV);                  /* 原码在 cgpu_runtime 之后才读 cgpu->drv */
    statline_before = *(void (**)(char *, size_t, void *))P8(drv, DRV_STATLINE_BEF);
    snprintf(buf, bufsz, "%s%d ", *(char **)P8(drv, DRV_NAME), LDs32(cgpu, CG_DEV_ID));
    statline_before(buf, bufsz, cgpu);          /* 驱动 statline_before 钩子 (原码无 NULL 保护直调) */
    len1 = strlen(buf);
    wu_per_min = accepted_f64 / (double)runtime * 60.0;
    snprintf(statline_buf, 0x40, "(%ds):%s (avg):%sh/s | A:%.0f R:%.0f HW:%d WU:%.1f/m",
             DAT_00075dcc, suffix_acc, suffix_avg,
             LDf64(cgpu, 0xc8),                 /* +0xc8 double 已接受份额难度 (A:%.0f) */
             LDf64(cgpu, 0xd0),                 /* +0xd0 double 已拒绝份额难度 (R:%.0f) */
             LDs32(cgpu, CG_HWERR), wu_per_min);
    len2 = strlen(statline_buf);
    if (bufsz <= len2 + len1) {                 /* 缓冲不足: 报错退出 (cgminer.c:3081) */
        snprintf(logbuf, 0x1000, "tailsprintf buffer overflow in %s %s line %d",
                 "cgminer.c", "get_statline", 0xc09);
        applog(3, logbuf, 1);
        quit(1);
    }
    strcat(buf, statline_buf);
    /* 尾部驱动 statline 钩子: 原码在调用点重读 cgpu->drv (drv+0x18) */
    (*(void (**)(char *, size_t, void *))P8(LDptr(cgpu, CG_DRV), DRV_STATLINE))(buf, bufsz, cgpu);
}

/* ==================== 组 C-G 迁移说明 (停滞续作) ====================
 * 本批次 (job_b5) 停滞后由续作代理分拆完成, 本文件保留组 A/B 全部 14 个函数:
 *   组 C (curses 窗口/队列清理/工作统计) + 组 D (提交/驱动工作循环/退出/统计)
 *     + 组 E 前 4 (submit_noffset_nonce submit_tested_work enable_curses
 *     fill_drv_defaults)                → rewritten/cgminer/job_p5a.c (17 函数)
 *   组 E 后 4 (enable_device parse_extranonce_equihash tq_push
 *     record_device_fail) + 组 F (sha256_ctx_final minecoin applog k_get
 *     bm1740_makeup_work) + 组 G (jansson/ccan-opt/libgcc) → job_p5b.c (16 函数)
 * 注: 组 F 计划中的 get_bit (FUN_00042958) / leading_zeros_256bit (FUN_00049824)
 *     原仅存前置声明, 已由 job_b5 补充批次转写至下方"补充"区段 (见表 #15/#16)。
 */

/* ==================== 补充 (job_b5 续作): 位原语 ==================== */

// === get_bit === 原 FUN_00042958 @ 0x00042958
// 来源: reversed/cgminer/20_job_mining/FUN_00042958.c | 分类: 20_job_mining
// 语义: 位图取位: 返回字节数组 data 中第 bit_index 位的值 (0/1)。字节序号按 C
//       向零取整的 bit_index/8, 字节内按 LSB 编号 (位 0 = 最低位)。原码把每字节
//       位序展开成 switch 逐位屏蔽; 负索引时 bit_index%8 为负落入 default 返回 0
//       (负的 8 倍数索引仍命中 case 0, 读 data[负偏移] 最低位)。
// 返回: 该位值 (0/1); 负且非 8 倍数的索引返回 0。
uint32_t get_bit(const uint8_t *data, int bit_index)
{
    int byte_off;

    /* 负数 +7 修正后算术右移 3 位 == C 向零取整的 bit_index/8 (ARM 有符号除法惯用序列) */
    byte_off = bit_index;
    if (byte_off < 0) {
        byte_off += 7;
    }
    byte_off >>= 3;

    switch (bit_index % 8) {
    case 0:
        return data[byte_off] & 1;
    case 1:
        return (data[byte_off] & 3) >> 1;
    case 2:
        return (data[byte_off] & 7) >> 2;
    case 3:
        return (data[byte_off] & 0xf) >> 3;
    case 4:
        return (data[byte_off] & 0x1f) >> 4;
    case 5:
        return (data[byte_off] & 0x3f) >> 5;
    case 6:
        return (data[byte_off] & 0x7f) >> 6;
    case 7:
        return data[byte_off] >> 7;
    default:
        return 0;                               /* 负索引: bit_index%8 < 0 (原码 default) */
    }
}

// === leading_zeros_256bit === 原 FUN_00049824 @ 0x00049824
// 来源: reversed/cgminer/20_job_mining/FUN_00049824.c | 分类: 20_job_mining
// 语义: 统计 256 位整数的二进制前导零个数。输入按小端存放 (如 work+0xa0 目标),
//       原码先做字节反转拷贝使 reversed[0] = 最高有效字节, 再借 get_bit 从最高位
//       向最低位逐位扫描, 遇首个 1 即返回已扫过的零位数。
// 返回: 前导零个数 0..255; 输入全 0 时返回 256。本固件难度约定: diff = 该值。
int leading_zeros_256bit(const void *v256)
{
    const uint8_t *src = v256;                  /* 32 字节小端整数 (最低字节在前) */
    uint8_t reversed[32];                       /* 原码栈上 32 字节缓冲 (local_38..local_1c) */
    int byte_idx;
    int bit_idx;
    int zeros;

    /* 字节反转拷贝: reversed[i] = src[31-i], 使扫描自最高有效字节开始 */
    for (byte_idx = 0; byte_idx < 32; byte_idx++) {
        reversed[byte_idx] = src[31 - byte_idx];
    }

    /* 从最高位向低位扫描 (get_bit 位序按字节内 LSB 编号, 故每字节自位 7 起) */
    zeros = 0;
    for (byte_idx = 0; byte_idx < 32; byte_idx++) {
        for (bit_idx = 7; bit_idx >= 0; bit_idx--) {
            if (get_bit(reversed + byte_idx, bit_idx) != 0) {
                return zeros;
            }
            zeros++;
        }
    }
    return zeros;                               /* 256 位全 0 → 256 */
}

/* ============================================================
 * 覆盖率表 (16/16 — job_p5 组 A/B + 补充 get_bit/leading_zeros_256bit; 组 C-G 见 job_p5a/p5b)
 * ============================================================
 * | #  | 原地址     | 语义名             | 状态 |
 * |----|------------|--------------------|------|
 * | 1  | 0x000118d4 | should_run         | 完成 |
 * | 2  | 0x00011a5c | calc_diff          | 完成 |
 * | 3  | 0x00011f60 | regen_hash         | 完成 |
 * | 4  | 0x00012250 | select_pool        | 完成 |
 * | 5  | 0x00014f30 | rebuild_nonce      | 完成 |
 * | 6  | 0x00014fd0 | pretest_hash       | 完成 |
 * | 7  | 0x00015684 | copy_work          | 完成 |
 * | 8  | 0x000158e4 | diff_from_nonce2   | 完成 |
 * | 9  | 0x000159b4 | discard_work       | 完成 |
 * | 10 | 0x00016244 | set_target         | 完成 |
 * | 11 | 0x00016494 | varint_encode      | 完成 |
 * | 12 | 0x000165a0 | test_nonce_diff    | 完成 |
 * | 13 | 0x00016b40 | __work_completed   | 完成 |
 * | 14 | 0x00017744 | get_statline       | 完成 |
 * | 15 | 0x00042958 | get_bit            | 完成 |
 * | 16 | 0x00049824 | leading_zeros_256bit | 完成 |
 * ============================================================ */
