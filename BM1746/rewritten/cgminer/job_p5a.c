/*
 * cgminer 20_job_mining 任务层续作 (job_p5a, 17 函数) — BM1746 固件语义转写
 * =========================================================
 * 原始二进制: bm1746/cgminer_z15_bm1746.elf (ARM32, SD 镜像 cgminer-z15, branch "bm1746")
 * 来源:      reversed/cgminer/20_job_mining/ | 命名: rewritten/name_map/cgminer.tsv
 *
 * 本文件承接停滞批次 job_b5 (job_p5.c 已完成组 A/B, 见其文件头), 转写剩余前 17 个
 * 函数 (按地址升序):
 *   [C] curses 窗口/队列清理/工作统计:
 *       check_winsizes clear_pool_work log_curses_only age_queued_work make_work
 *       share_diff update_work_stats
 *   [D] 提交/驱动工作循环/退出/统计:
 *       test_nonce_fulltest flush_queue clone_queued_work_byid hash_driver_work
 *       __quit inc_work_stats
 *   [E] 提交路径/设备初始化:
 *       submit_noffset_nonce submit_tested_work enable_curses fill_drv_defaults
 *
 * 关键机器语义 (与 job_p5.c 文件头同源):
 *   - workid 机制: work+0x724 为工作序号, 下发帧 byte[3] = work->id & 0x7f,
 *     fill_work_thread 以 128 槽映射回查。
 *   - 全局暂存队列表头 DAT_00077050: 每个排队工作在 work+0x728 处内嵌队列节点
 *     {ctrl, prev, next, hprev, hnext, .. hash}, ctrl+0x00 桶数组基址/+0x04 桶掩码/
 *     +0x0c 队列总数/+0x10 队尾节点指针(含基值)/+0x14 节点在工作结构内的偏移基值;
 *     桶槽 12 字节 {桶链头, 计数, 保留} (上游 work_ent/work_queue 等价物,
 *     与 __work_completed/job_p1 total_staged 观测一致)。
 *   - submit_noffset_nonce / submit_tested_work / hash_driver_work 中
 *     update_work_stats(thr, work)、submit_work_async(work)、record_device_fail(cgpu, 1)
 *     等被 Ghidra 丢失实参的调用, 已按 0x1c5x8/0x25dx/0x25ex 处寄存器现场恢复
 *     (与 job_p5.c 文件头同源结论)。
 *   - age_queued_work(pool, secs): secs 为 d0 传入的 double (Ghidra 显示为 in_d0)。
 *   - log_curses_only 的高优先级判定为无符号比较 (prio-3)U > 1: prio>=5 及
 *     prio<3 (负数回绕) 均视为"高优先级"。
 *   - 日志门控 (与 job_p5.c/log.c 同款): prio7 需 opt_debug && (A||B||C>6),
 *     prio5: A||B||C>4, prio3: A||B||C>2 (A=DAT_0007ad48, B=DAT_00078e08,
 *     C=DAT_000766c4)。
 *
 * 转写约定: rewritten/CONVENTIONS.md
 *   - 被调函数名一律取自 name_map/cgminer.tsv; 未知 DAT_xxx 全局保留原名 + 注释。
 *   - 日志字符串原样保留 (make_work/test_nonce_fulltest/flush_queue 的逐字节栈构造
 *     串还原为等值字面量); Ghidra 伪变量 (extraout, in_d0, uVar, local 系) 已消除,
 *     in_d0 还原为具名 double 形参。
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

/* struct work 字段偏移 (20_job_mining 全组一致观测, 同 job_p5.c) */
#define WK_TARGET        0xa0   /* 256 位目标 (LE 存放, 0xa0 最低字) */
#define WK_HASH          0xc0   /* 256 位哈希结果 (LE 存放, 0xc0 最低字) */
#define WK_DIFFACC       0x6b0  /* double 累计难度贡献 */
#define WK_SHARE_DIFF    0x6b8  /* uint64 本工作份额难度 */
#define WK_FOUND         0x6ea  /* char 满足块目标标志 (0x6eb = 满足网络难度标志) */
#define WK_POOL          0x6d4  /* struct pool* */
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
#define PL_BLOCKS        0x20   /* int 发现块计数 */
#define PL_DIFFACC       0x28   /* uint64 累计难度贡献 */
#define PL_QUEUE_LOCK    0x140  /* pthread_rwlock_t 哈希队列锁 */
#define PL_QUEUE_HEAD    0x160  /* 队列首工作指针 */
#define PL_PENDING       0x164  /* 单项挂起工作槽 (flush_queue/取单工作路径取走清零) */
#define PL_QUEUE_CNT     0x168  /* 队列工作计数 */
#define PL_BEST_DIFF     0x190  /* uint64 池内最好份额难度 (0x194 高 32 位) */

/* struct cgpu_info / struct thr_info 字段偏移 */
#define CG_DRV           0x04   /* struct device_drv* */
#define CG_DEV_ID        0x08   /* int 设备序号 */
#define CG_DEVEN         0x20   /* int 0=启用/1=禁用 */
#define CG_DIFFACC       0xc0   /* uint64 累计难度贡献 */
#define CG_LAST_WORK     0xe8   /* time_t 最近有效工作时刻 */
#define CG_SHUTDOWN      0x16c  /* char 停机标志 */
#define THR_ID           0x00   /* int 线程序号 (thr_info) */
#define THR_CGPU         0x24   /* struct cgpu_info* */
#define THR_PAUSED       0x0f   /* char 线程暂停标志 */
#define THR_WORK_RESTART 0x3f   /* char 工作重启请求标志 (0x3e 为配套清零位) */

/* struct device_drv 字段偏移 */
#define DRV_NAME         0x08   /* char* 驱动名 */
#define DRV_HASH_WORK    0x44   /* hash_work 工作函数 */
#define DRV_FLUSH        0x50   /* 重启时加锁调用的驱动钩子 */

/* 驱动钩子调用 (函数指针存于 device_drv 内, 布局未完全还原, 经偏移取用) */
typedef int64_t (*drv_hash_work_fn)(void *thr);
typedef void    (*drv_flush_fn)(void *cgpu);

/* ==================== 外部函数 (名取自 name_map/cgminer.tsv) ==================== */

extern void        cgtime(struct timeval *tv);                    /* 原 FUN_0002a84c: gettimeofday 包装 */
extern double      __floatundidf(uint64_t v);                     /* 原 FUN_0005cb50: u64→double */
extern double      __floatdidf(int64_t v);                        /* 原 FUN_0005cb64: s64→double */
extern uint64_t    __fixunsdfdi(double v);                        /* 原 FUN_0005cc48: double→u64 截断 */
extern int64_t     __fixdfdi(double v);                           /* 原 FUN_0005cc10: double→s64 截断 */
extern void        suffix_string(uint64_t value, char *buf, size_t bufsz, int sig); /* 原 FUN_00011580 */
extern void        free_work(struct work *w);                     /* 原 FUN_00015780 */
extern void        copy_work(struct work *dst, struct work *src, int nonce_offset); /* 原 FUN_00015684 (job_p5.c) */
extern void        __work_completed(struct pool *pool, struct work *w); /* 原 FUN_00016b40 (job_p5.c) */
extern void        quit(int status);                              /* 原 FUN_00016ca8 */
extern void        mutex_lock_fail_cgminer(const char *fn, int line);   /* 原 FUN_0000b414 */
extern void        wr_lock_error(const char *fn, int line);       /* 原 FUN_0000b480 */
extern void        mutex_unlock_fail_cgminer(const char *fn, int line); /* 原 FUN_0000b5c4 */
extern void        rw_unlock_error(const char *fn, int line);     /* 原 FUN_0000b630 */
extern void        rd_lock_error(const char *fn, int line);       /* 原 FUN_0000b69c */
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
extern double      tdiff(struct timeval *a, struct timeval *b);   /* 原 FUN_0002aa24 */
extern void        copy_timeval(struct timeval *dst, struct timeval *src); /* 原 FUN_0002a530 */
extern void        print_summary(void);                           /* 原 FUN_0001c668 */
extern void       *miner_thread_work_loop(void *arg);             /* 原 FUN_00024db0 (驱动默认值引用) */
extern void        record_device_fail(void *cgpu, int reason);    /* 原 FUN_0002ed88 */
extern void        applog(uint32_t prio, char *buf, ...); /* 原 FUN_00038730 (log.c) */

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

/* ==================== 外部全局 (DAT_ 保留原名, 语义见注释) ==================== */

/* ---- 日志门控 (与 uart.c/hash.c 同一组全局) ---- */
extern char    DAT_0007ad48;   /* 门控 A; applog 内非 0 改走 syslog() */
extern char    DAT_00078e08;   /* 门控 B (curses Verbose 联动) */
extern int     DAT_000766c4;   /* 日志级别, 门控条件 C */
extern char    DAT_00078e09;   /* opt_debug (Debug mode), 门控 7/6 前置 */

/* ---- 模式开关 ---- */
extern char    DAT_0007b859;   /* opt_scrypt: 1 → diff1=65536/2^240, 0 → 2^224 分支 */

/* ---- 全局统计 ---- */
extern uint64_t DAT_0007b100;  /* 总累计难度贡献 u64 (低字 DAT_0007b100, 高字 DAT_0007b104) */
extern int     DAT_0007b8b8;   /* 找到块计数 (update_work_stats 自增) */
extern int     DAT_00075dcc;   /* get_statline 显示秒数 / hash_driver_work 上报间隔阈值 */
extern double  DAT_00075de0;   /* 网络难度 (minecoin "Network Difficulty" 同源) */
extern char    DAT_00075dc4;   /* 全局最好份额串缓冲 (suffix_string 写入 8 字节) */
extern uint64_t DAT_00076f18;  /* 全局最好份额难度 u64 (低字 DAT_00076f18, 高字 DAT_00076f1c) */

/* ---- 池/设备表 ---- */
extern void   *DAT_0007704c;   /* 全局工作队列互斥锁指针 (stgd_lock, clear_pool_work 用) */
extern void   *DAT_00077050;   /* 全局工作队列首工作指针 */

/* ---- 锁 ---- */
extern pthread_mutex_t DAT_0007ad4c;  /* hash_lock (统计互斥) */
extern pthread_mutex_t DAT_0007ba10;  /* share_diff 互斥锁 */
extern pthread_rwlock_t DAT_0007ba28; /* share_diff 读写锁 */
extern pthread_mutex_t DAT_0007b85c;  /* hash_driver_work 重启钩子锁 */

/* ---- curses ---- */
extern char    DAT_00075dd0;   /* use_curses (check_winsizes/enable_curses 前置判定) */
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

/* ---- fill_drv_defaults 默认桩 (代码段小桩函数, Ghidra 以 DAT/LAB 标注) ---- */
extern void    DAT_0000f794(void);   /* 默认桩 1 (返回 0 型) */
extern void    DAT_0000f798(void);   /* 默认桩 2 */
extern void    DAT_0000f79c(void);   /* 默认桩 3 */
extern void    DAT_0000f7a0(void);   /* 默认桩 4 */
extern void    DAT_0000f7a8(void);   /* 默认桩 5 */
extern void    DAT_0000f7b0(void);   /* 默认桩 6 */
extern void    DAT_0000f7bc(void);   /* 默认桩 7 */
extern void    DAT_0000f7c4(void);   /* 默认桩 8 */
extern void    DAT_0000f7cc(void);   /* 默认桩 9 */
extern void    DAT_0000f7d0(void);   /* 默认桩 10 */
extern void    DAT_0000f7d4(void);   /* 默认桩 11 */
extern void    DAT_0000f7d8(void);   /* 默认桩 12 */

/* ==================== 本文件函数 (供他段引用的前向声明) ==================== */

void check_winsizes(void);
void clear_pool_work(struct pool *pool);
int  log_curses_only(int prio, char *str1, char *str2);
int  age_queued_work(struct pool *pool, double secs);
struct work *make_work(void);
uint64_t share_diff(struct work *w);
void update_work_stats(void *thr, struct work *w);
int  test_nonce_fulltest(void *thr, struct work *w, uint32_t nonce, int *above_target);
void flush_queue(struct pool *pool);
struct work *clone_queued_work_byid(struct pool *pool, int id);
void hash_driver_work(struct thr_info *thr);
void __quit(int status, int wait_for_threads);
void inc_work_stats(void *thr, struct pool *pool, uint32_t addend);
int  submit_noffset_nonce(void *thr, struct work *base, uint32_t nonce, int nonce_offset);
int  submit_tested_work(void *thr, struct work *w);
void enable_curses(void);
void fill_drv_defaults(void *drv);

/* ==================== 组 C: curses 窗口/队列清理/工作统计 ==================== */

// === check_winsizes === 原 FUN_00018068 @ 0x00018068
// 来源: reversed/cgminer/20_job_mining/FUN_00018068.c | 分类: 20_job_mining
// 语义: curses 窗口尺寸自适应 (终端尺寸变化后调用): 清屏, 按 LINES 重排
//       status_win 行数与日志窗口起始行 DAT_00077024, 重设两窗大小/位置后解锁;
//       mainwin 不可用时行列取 -1 (原码不做保护直接传给 wresize)。
void check_winsizes(void)
{
	int rc;
	int status_cols;
	int main_rows;
	int main_cols;

	if (DAT_00075dd0 == '\0')                        /* use_curses 未启用 */
		return;
	if (curses_active_locked() == 0)                 /* 加锁并复查激活态 */
		return;
	werase(stdscr);
	status_cols = (DAT_0007b7c0 == NULL) ? -1 : (int)*(short *)P8(DAT_0007b7c0, 6) + 1;
	                                                /* status_win->_maxx+1 */
	DAT_00077020 = DAT_0007701c;                     /* status 行数 = 配置值 */
	if (LINES - 1 <= DAT_0007701c)
		DAT_00077020 = LINES - 2;                    /* 终端过矮时压缩到 LINES-2 */
	DAT_00077024 = DAT_00077020;                     /* 日志窗口起始行 = status 高度 */
	wresize(DAT_0007b7c0, DAT_00077020, status_cols);
	if (DAT_0007bc90 == NULL) {
		main_rows = -1;
		main_cols = -1;
	} else {
		main_rows = (int)*(short *)P8(DAT_0007bc90, 4) + 1;   /* mainwin->_maxy+1 */
		main_cols = (int)*(short *)P8(DAT_0007bc90, 6) + 1;   /* mainwin->_maxx+1 */
	}
	wresize(DAT_0007b430, main_rows - DAT_00077024, main_cols);
	mvwin(DAT_0007b430, DAT_00077024, 0);
	rc = pthread_mutex_unlock(&DAT_0007b7a8);
	if (rc != 0)
		mutex_unlock_fail_cgminer("unlock_curses", 0xb72);
	(*DAT_00075ed8)();
}

// === clear_pool_work === 原 FUN_000185c8 @ 0x000185c8
// 来源: reversed/cgminer/20_job_mining/FUN_000185c8.c | 分类: 20_job_mining
// 语义: stratum 断线时清空某池的全部暂存工作: stgd_lock (DAT_0007704c) 下遍历
//       全局队列 DAT_00077050, work->pool 命中者做与 __work_completed 同构的
//       双向链/桶链摘除 (ctrl+0x14 为节点在 work 内偏移基值, ctrl+0x10 为队尾
//       含基值指针, ctrl+0x0c 队列总数, 桶槽 0xc 字节); 队列将空时释放桶数组与
//       ctrl 并清空全局表头; 释放命中工作并计数, 调试门控下打印
//       "Cleared %d work items due to stratum disconnect on pool %d"。
void clear_pool_work(struct pool *pool)
{
	int rc;
	int cleared = 0;
	char msg[4100];
	struct work *work;
	struct work *next;
	void *ctrl;
	void *prev;
	void *qnext;
	void *bucket;
	long slot_off;

	rc = pthread_mutex_lock((pthread_mutex_t *)DAT_0007704c);       /* stgd_lock */
	if (rc != 0)
		mutex_lock_fail_cgminer("clear_pool_work", 0x1b68);

	work = (struct work *)DAT_00077050;
	if (work != NULL) {
		next = LDptr(work, WK_QNEXT);                /* 预读后继 (循环终止条件) */
		for (;;) {
			if (LDptr(work, WK_POOL) == pool) {
				prev = LDptr(work, WK_QPREV);
				qnext = LDptr(work, WK_QNEXT);
				if ((prev == NULL) && (qnext == NULL)) {
					/* 队列仅此一项: 释放桶数组 (ctrl[0]) 与 ctrl, 整队清空 */
					free(*(void **)LDptr(DAT_00077050, WK_QCTRL));
					free(LDptr(DAT_00077050, WK_QCTRL));
					DAT_00077050 = NULL;
				} else {
					ctrl = LDptr(DAT_00077050, WK_QCTRL);   /* 反编译经当前表头复读; 全队共享同一 ctrl */
					if ((uintptr_t)LDptr(ctrl, 0x10) - (uintptr_t)LDptr(ctrl, 0x14) == (uintptr_t)work) {
						/* work 为队尾: 尾指针(含基值)回退到前驱 */
						LDptr(ctrl, 0x10) = (char *)prev + *(int *)P8(ctrl, 0x14);
						prev = LDptr(work, WK_QPREV);
					}
					if (prev == NULL) {
						DAT_00077050 = qnext;            /* work 为队首: 全局表头指向后继 */
					} else {
						/* 前驱->next(+8) = 后继 (前驱节点基址 = 前驱 + 基值偏移) */
						LDptr((char *)prev + *(int *)P8(LDptr(DAT_00077050, WK_QCTRL), 0x14), 8) = qnext;
					}
					if (qnext != NULL) {
						/* 后继->prev(+4) = 前驱 */
						LDptr((char *)qnext + *(int *)P8(LDptr(DAT_00077050, WK_QCTRL), 0x14), 4) = prev;
					}
					/* 哈希桶摘链: 槽 = ((ctrl[1]-1) & work->hash) * 0xc */
					ctrl = LDptr(DAT_00077050, WK_QCTRL);
					slot_off = (long)((LD32(ctrl, 4) - 1U) & LD32(work, WK_QHASH)) * 0xc;
					bucket = LDptr(ctrl, 0);             /* ctrl[0] = 桶数组基址 */
					LDs32(bucket, slot_off + 4) -= 1;          /* 槽计数 -1 */
					if (LDptr(bucket, slot_off) == (char *)work + WK_QCTRL)
						LDptr(bucket, slot_off) = LDptr(work, WK_QHNEXT);  /* 桶首即本节点 → 前移 */
					if (LDptr(work, WK_QHPREV) != NULL)
						LDptr(LDptr(work, WK_QHPREV), 0x10) = LDptr(work, WK_QHNEXT);
					if (LDptr(work, WK_QHNEXT) != NULL)
						LDptr(LDptr(work, WK_QHNEXT), 0xc) = LDptr(work, WK_QHPREV);
					LDs32(LDptr(DAT_00077050, WK_QCTRL), 0xc) -= 1;    /* 队列总数 -1 */
				}
				cleared++;
				free_work(work);
			}
			if (next == NULL)
				break;
			work = next;
			next = LDptr(work, WK_QNEXT);
		}
	}

	rc = pthread_mutex_unlock((pthread_mutex_t *)DAT_0007704c);
	if (rc != 0)
		mutex_unlock_fail_cgminer("clear_pool_work", 0x1b72);
	(*DAT_00075ed8)();

	if ((cleared != 0) && (DAT_00078e09 != '\0') &&
	    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 6 < DAT_000766c4)) {
		snprintf(msg, 0x1000, "Cleared %d work items due to stratum disconnect on pool %d",
		         cleared, LDs32(pool, PL_ID));
		applog(7, msg, 0);
	}
}

// === log_curses_only === 原 FUN_000199e8 @ 0x000199e8
// 来源: reversed/cgminer/20_job_mining/FUN_000199e8.c | 分类: 20_job_mining
// 语义: curses 模式下写日志窗口 (不带时间戳): 高优先级 ((prio-3)U>1, 即 prio>=5
//       或 prio<3 无符号回绕) 消息在显示挂起标志 DAT_0007bac8 置位时整条跳过;
//       写入 "%s%s\n" 后仅低优先级触摸+刷新窗口; 返回 1, curses 未激活返回 0。
int log_curses_only(int prio, char *str1, char *str2)
{
	int rc;
	int rows;
	bool high_prio;

	if (curses_active_locked() == 0)
		return 0;
	high_prio = (unsigned int)(prio - 3) > 1;        /* 无符号比较 (Ghidra bVar1) */
	if (DAT_0007bac8 != '\0' && high_prio) {         /* 显示挂起 + 高优先级 → 跳过打印与刷新 */
		rc = pthread_mutex_unlock(&DAT_0007b7a8);
		if (rc != 0)
			mutex_unlock_fail_cgminer("unlock_curses", 0xb72);
		(*DAT_00075ed8)();
		return 1;
	}
	wprintw(DAT_0007b430, "%s%s\n", str1, str2);
	if (!high_prio) {                                /* 仅低优先级刷新窗口 */
		rows = (DAT_0007b430 == NULL) ? -1 : (int)*(short *)P8(DAT_0007b430, 4) + 1;
		                                               /* log_win->_maxy+1 */
		wtouchln(DAT_0007b430, 0, rows, 1);
		wrefresh(DAT_0007b430);
	}
	rc = pthread_mutex_unlock(&DAT_0007b7a8);
	if (rc != 0)
		mutex_unlock_fail_cgminer("unlock_curses", 0xb72);
	(*DAT_00075ed8)();
	return 1;
}

// === age_queued_work === 原 FUN_00019fc8 @ 0x00019fc8
// 来源: reversed/cgminer/20_job_mining/FUN_00019fc8.c | 分类: 20_job_mining
// 语义: 清理池哈希队列中超时工作: 写锁 pool+0x140 后遍历, work 入队时刻
//       (work+0x778) 距当前时间超过 secs (d0 传参, Ghidra 显示为 in_d0) 的工作
//       由 __work_completed 摘链并释放; 返回清理个数。
int age_queued_work(struct pool *pool, double secs)
{
	int rc;
	int aged = 0;
	struct timeval now;
	struct work *work;
	struct work *next;

	cgtime(&now);
	rc = pthread_rwlock_wrlock((pthread_rwlock_t *)P8(pool, PL_QUEUE_LOCK));
	if (rc != 0)
		wr_lock_error("age_queued_work", 0x22f6);

	work = LDptr(pool, PL_QUEUE_HEAD);
	if (work != NULL) {
		next = LDptr(work, WK_QNEXT);                /* 预读后继 (循环终止条件) */
		for (;;) {
			if (secs < tdiff(&now, (struct timeval *)P8(work, WK_TV))) {
				__work_completed(pool, work);
				free_work(work);
				aged++;
			}
			if (next == NULL)
				break;
			work = next;
			next = LDptr(work, WK_QNEXT);
		}
	}

	rc = pthread_rwlock_unlock((pthread_rwlock_t *)P8(pool, PL_QUEUE_LOCK));
	if (rc != 0)
		rw_unlock_error("age_queued_work", 0x2300);
	(*DAT_00075ed8)();
	return aged;
}

// === make_work === 原 FUN_0001a3d4 @ 0x0001a3d4
// 来源: reversed/cgminer/20_job_mining/FUN_0001a3d4.c | 分类: 20_job_mining
// 语义: 新建工作结构 (0x7b0 字节零初始化); 失败时打印 "Failed to calloc work in
//       make_work" (原码逐字节栈构造字符串, 反 strings 处理) 并 quit(1); 成功则
//       递增全局计数并写入 work->workid (下发帧 byte[3] = id & 0x7f 映射依赖)。
struct work *make_work(void)
{
	struct work *work;

	work = (struct work *)calloc(1, 0x7b0);
	if (work == NULL) {
		applog(3, "Failed to calloc work in make_work", 1);
		quit(1);
	}
	LD32(work, WK_WORKID) = (uint32_t)total_work_inc();
	return work;
}

// === share_diff === 原 FUN_0001a4b4 @ 0x0001a4b4
// 来源: reversed/cgminer/20_job_mining/FUN_0001a4b4.c | 分类: 20_job_mining
// 语义: 由工作哈希 (work+0xc0..0xdf, LE 4×u64) 折算份额难度 = diff1/哈希值
//       (round 后截断为 u64); share_diff 互斥锁+读写锁保护下刷新全局最好份额
//       (DAT_00076f18 u64) 与池最好份额 (pool+0x190), 全局新纪录时 suffix_string
//       写 DAT_00075dc4 串并在调试门控下打印 "New best share: %s"; 返回份额难度。
uint64_t share_diff(struct work *work)
{
	int rc;
	char msg[4100];
	double diff1;
	double hashval;
	uint64_t share_diff_val;
	bool new_best;
	struct pool *pool;

	diff1 = 1.7668201048317172e+72;                  /* 2^240 (opt_scrypt 分支) */
	if (DAT_0007b859 == '\0')
		diff1 = 2.695953529101131e+67;               /* 2^224 */
	hashval = (double)__floatundidf(LD64(work, 0xd8)) * 6.277101735386681e+57 +   /* 2^192 */
		  (double)__floatundidf(LD64(work, 0xd0)) * 3.402823669209385e+38 +       /* 2^128 */
		  (double)__floatundidf(LD64(work, 0xc8)) * 1.8446744073709552e+19 +      /* 2^64 */
		  (double)__floatundidf(LD64(work, 0xc0));
	/* 反编译中此处存在恒等赋值 if(hashval==0) hashval=0, 无行为影响, 已略去 */
	share_diff_val = (uint64_t)__fixunsdfdi(round(diff1 / hashval));

	rc = pthread_mutex_lock(&DAT_0007ba10);
	if (rc != 0)
		mutex_lock_fail_cgminer("share_diff", 0x137a);
	rc = pthread_rwlock_wrlock(&DAT_0007ba28);
	if (rc != 0)
		wr_lock_error("share_diff", 0x137a);

	new_best = share_diff_val > DAT_00076f18;        /* 全局最好份额 u64 比较 (高字 DAT_00076f1c) */
	if (new_best) {
		DAT_00076f18 = share_diff_val;
		suffix_string(share_diff_val, (char *)&DAT_00075dc4, 8, 0);
	}

	pool = LDptr(work, WK_POOL);
	if (share_diff_val > LD64(pool, PL_BEST_DIFF))   /* 池最好份额 (pool+0x190 u64) */
		LD64(pool, PL_BEST_DIFF) = share_diff_val;

	rc = pthread_rwlock_unlock(&DAT_0007ba28);
	if (rc != 0)
		rw_unlock_error("share_diff", 0x1383);
	rc = pthread_mutex_unlock(&DAT_0007ba10);
	if (rc != 0)
		mutex_unlock_fail_cgminer("share_diff", 0x1383);
	(*DAT_00075ed8)();

	if (new_best && DAT_00078e09 != '\0' &&
	    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 6 < DAT_000766c4)) {
		snprintf(msg, 0x1000, "New best share: %s", (char *)&DAT_00075dc4);
		applog(7, msg, 0);
	}
	return share_diff_val;
}

// === update_work_stats === 原 FUN_0001a730 @ 0x0001a730
// 来源: reversed/cgminer/20_job_mining/FUN_0001a730.c | 分类: 20_job_mining
// 语义: 工作统计总控: share_diff() 折算并写 work->share_diff (+0x6b8); 份额难度
//       ≥ 网络难度 (DAT_00075de0, opt_scrypt 时 ×65536) 即视为挖到块: 置
//       work+0x6eb 网络块标志、全局/池发现块计数自增、work+0x6ea 块标志并打印
//       "Found block for pool %d!"; 随后 hash_lock (DAT_0007ad4c) 下把
//       work->diffacc (+0x6b0) 经 double 往返累加进全局/设备 (+0xc0)/池 (+0x28)
//       三级 u64 统计并刷新设备最近有效工作时刻 (+0xe8)。
void update_work_stats(void *thr, struct work *work)
{
	int rc;
	char msg[4100];
	double diff1;
	double sum;
	uint64_t sd;
	void *cgpu;
	struct pool *pool;

	diff1 = DAT_00075de0;                            /* 网络难度 (minecoin 同源全局) */
	sd = share_diff(work);
	LD64(work, WK_SHARE_DIFF) = sd;
	if (DAT_0007b859 != '\0')                        /* opt_scrypt */
		diff1 *= 65536.0;
	if (diff1 <= (double)__floatundidf(sd)) {        /* 达到网络难度 → 挖到块 */
		LD8(work, 0x6eb) = 1;                        /* 满足网络难度标志 */
		DAT_0007b8b8 += 1;                           /* 全局发现块计数 */
		pool = LDptr(work, WK_POOL);
		LDs32(pool, PL_BLOCKS) += 1;                 /* pool+0x20 发现块计数 */
		LD8(work, WK_FOUND) = 1;                     /* +0x6ea 满足块目标标志 */
		if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 4 < DAT_000766c4) {
			snprintf(msg, 0x1000, "Found block for pool %d!", LDs32(pool, PL_ID));
			applog(5, msg, 0);
		}
	}

	rc = pthread_mutex_lock(&DAT_0007ad4c);          /* hash_lock */
	if (rc != 0)
		mutex_lock_fail_cgminer("update_work_stats", 0x20bf);

	sum = (double)__floatdidf((int64_t)DAT_0007b100) + LDf64(work, WK_DIFFACC);
	DAT_0007b100 = (uint64_t)__fixdfdi(sum);         /* 全局累计难度贡献 += work->diffacc */
	cgpu = LDptr(thr, THR_CGPU);
	sum = (double)__floatdidf((int64_t)LD64(cgpu, CG_DIFFACC)) + LDf64(work, WK_DIFFACC);
	LD64(cgpu, CG_DIFFACC) = (uint64_t)__fixdfdi(sum);
	pool = LDptr(work, WK_POOL);
	sum = (double)__floatdidf((int64_t)LD64(pool, PL_DIFFACC)) + LDf64(work, WK_DIFFACC);
	LD64(pool, PL_DIFFACC) = (uint64_t)__fixdfdi(sum);
	cgpu = LDptr(thr, THR_CGPU);
	LD64(cgpu, CG_LAST_WORK) = (uint64_t)time(NULL); /* 最近有效工作时刻 */

	rc = pthread_mutex_unlock(&DAT_0007ad4c);
	if (rc != 0)
		mutex_unlock_fail_cgminer("update_work_stats", 0x20c4);
	(*DAT_00075ed8)();
}

// === test_nonce_fulltest === 原 FUN_0001a914 @ 0x0001a914
// 来源: reversed/cgminer/20_job_mining/FUN_0001a914.c | 分类: 20_job_mining
// 语义: 完整校验 nonce (无直接调用者, 经函数指针引用 [低置信]): test_nonce 通过后
//       update_work_stats + fulltest (work+0xc0 哈希 vs work+0xa0 目标); 未达目标
//       时置出参 *above_target=1 并在调试门控下打印 "Share above target" (原码
//       逐字节栈构造串); test_nonce 失败走 inc_hw_errors; 返回 1=可提交。
int test_nonce_fulltest(void *thr, struct work *work, uint32_t nonce, int *above_target)
{
	if (above_target != NULL)
		*above_target = 0;
	if (!test_nonce(work, nonce)) {
		inc_hw_errors(thr);
		return 0;
	}
	update_work_stats(thr, work);
	if (!fulltest(P8(work, WK_HASH), P8(work, WK_TARGET))) {
		if (above_target != NULL)
			*above_target = 1;
		if (DAT_00078e09 != '\0' &&
		    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 6 < DAT_000766c4))
			applog(7, "Share above target", 0);
		return 0;
	}
	return 1;
}

// === flush_queue === 原 FUN_0001aa18 @ 0x0001aa18
// 来源: reversed/cgminer/20_job_mining/FUN_0001aa18.c | 分类: 20_job_mining
// 语义: 丢弃池 +0x164 单项挂起工作: trywrlock 池队列锁成功后取走指针并清零,
//       解锁后释放 (非空时), 调试门控下打印 "Discarded queued work item" (原码
//       逐字节栈构造串); trywrlock 失败 (锁被占用) 时静默返回。
void flush_queue(struct pool *pool)
{
	int rc;
	struct work *work;

	if (pool == NULL)
		return;
	if (pthread_rwlock_trywrlock((pthread_rwlock_t *)P8(pool, PL_QUEUE_LOCK)) != 0)
		return;                                      /* 锁占用: 原码不处理 */
	work = LDptr(pool, PL_PENDING);
	LDptr(pool, PL_PENDING) = NULL;
	rc = pthread_rwlock_unlock((pthread_rwlock_t *)P8(pool, PL_QUEUE_LOCK));
	if (rc != 0)
		rw_unlock_error("flush_queue", 0x232c);
	(*DAT_00075ed8)();
	if (work != NULL) {
		free_work(work);
		if (DAT_00078e09 != '\0' &&
		    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 6 < DAT_000766c4))
			applog(7, "Discarded queued work item", 0);
	}
}

// === clone_queued_work_byid === 原 FUN_0001ac00 @ 0x0001ac00
// 来源: reversed/cgminer/20_job_mining/FUN_0001ac00.c | 分类: 20_job_mining
// 语义: 读锁池队列后按 work->workid (+0x724) 线性查找排队工作, 命中则在持锁
//       状态下 make_clone(work, 0) (make_work + copy_work(clone, work, 0));
//       未命中返回 NULL。
struct work *clone_queued_work_byid(struct pool *pool, int id)
{
	int rc;
	struct work *work;
	struct work *clone;

	rc = pthread_rwlock_rdlock((pthread_rwlock_t *)P8(pool, PL_QUEUE_LOCK));
	if (rc != 0)
		rd_lock_error("clone_queued_work_byid", 0x22dc);

	clone = NULL;
	work = LDptr(pool, PL_QUEUE_HEAD);
	if (work != NULL) {
		if (id == LDs32(work, WK_WORKID)) {
			clone = make_clone(work, 0);
		} else {
			work = LDptr(work, WK_QNEXT);
			while (work != NULL) {
				if (id == LDs32(work, WK_WORKID)) {
					clone = make_clone(work, 0);
					break;
				}
				work = LDptr(work, WK_QNEXT);
			}
		}
	}

	rc = pthread_rwlock_unlock((pthread_rwlock_t *)P8(pool, PL_QUEUE_LOCK));
	if (rc != 0)
		rw_unlock_error("clone_queued_work_byid", 0x22e0);
	(*DAT_00075ed8)();
	return clone;
}

// === hash_driver_work === 原 FUN_0001c3f4 @ 0x0001c3f4
// 来源: reversed/cgminer/20_job_mining/FUN_0001c3f4.c | 分类: 20_job_mining
// 语义: 驱动工作主循环: 反复调用 drv->hash_work (drv+0x44, 返回本轮哈希数, -1
//       视为失败) 累计; 每 ~200000us (0x30d41) / 每秒差 ≥ DAT_00075dcc 上报
//       hashmeter 并复位计时; 线程暂停 (thr+0xf) 或设备停用 (cgpu+0x20) 时走
//       mt_disable; thr+0x3f work_restart 置位时在 DAT_0007b85c 锁下调
//       drv->flush (drv+0x50); cgpu+0x16c 停机标志置位时停用设备返回。hash_work
//       失败时打印 "%s %d failure, disabling!"、停用设备并 record_device_fail
//       (cgpu, 1) (Ghidra 丢失第 2 参, 按 0x1c5x8 处寄存器现场恢复)。
void hash_driver_work(struct thr_info *thr)
{
	int rc;
	char msg[4100];
	int thr_id;
	void *cgpu;
	void *drv;
	struct timeval tv_start;
	struct timeval tv_end;
	uint64_t total_hashes;
	int64_t hashes;
	int sec_diff;
	int usec_diff;
	bool borrow;

	thr_id = LDs32(thr, THR_ID);
	cgpu = LDptr(thr, THR_CGPU);
	drv = LDptr(cgpu, CG_DRV);
	total_hashes = 0;
	tv_start.tv_sec = 0;                             /* 起始计时零值 (非 cgtime) */
	tv_start.tv_usec = 0;

	for (;;) {
		if (LD8(cgpu, CG_SHUTDOWN) != '\0') {        /* cgpu+0x16c 停机标志 */
			LDs32(cgpu, CG_DEVEN) = 1;               /* 停用设备后返回 */
			return;
		}
		LD8(thr, THR_WORK_RESTART) = 0;              /* 清 thr+0x3f 工作重启请求 */
		hashes = (int64_t)((drv_hash_work_fn)LDptr(drv, DRV_HASH_WORK))(thr);
		LD8(thr, 0x3e) = 0;                          /* thr+0x3e 配套清零位 */
		if (hashes == -1) {
			if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 2 < DAT_000766c4) {
				snprintf(msg, 0x1000, "%s %d failure, disabling!",
				         *(char **)P8(drv, DRV_NAME), LDs32(cgpu, CG_DEV_ID));
				applog(3, msg, 0);
			}
			LDs32(cgpu, CG_DEVEN) = 1;
			record_device_fail(cgpu, 1);
			LDs32(cgpu, CG_DEVEN) = 1;               /* 原码汇合标签 (LAB_0001c568) 二次置位 */
			return;
		}
		total_hashes += (uint64_t)hashes;
		cgtime(&tv_end);
		usec_diff = tv_end.tv_usec - tv_start.tv_usec;
		borrow = usec_diff < 0;
		if (borrow)
			usec_diff += 0xf4000;                    /* 0xf4000+0x240 = 0xf4240 = 1000000 (借位拆分) */
		sec_diff = tv_end.tv_sec - tv_start.tv_sec;
		if (borrow) {
			usec_diff += 0x240;
			sec_diff -= 1;
		}

		if (((total_hashes == 0) || (sec_diff < 1 && usec_diff < 0x30d41)) &&
		    sec_diff < DAT_00075dcc) {
			/* 快速路径: 无需上报, 直接继续 */
		} else {
			hashmeter(thr_id, (uint32_t)sec_diff, total_hashes);
			total_hashes = 0;
			copy_timeval(&tv_start, &tv_end);        /* 计时基准 = 本轮结束时刻 */
		}

		if (LD8(thr, THR_PAUSED) != '\0' || LDs32(cgpu, CG_DEVEN) != 0)
			mt_disable(thr, thr_id, drv);            /* 暂停/停用: 清哈希率并等待 */

		if (LD8(thr, THR_WORK_RESTART) == 0)
			continue;                                /* 未请求重启 → 下一轮 */

		rc = pthread_mutex_lock(&DAT_0007b85c);      /* hash_driver_work 重启钩子锁 */
		if (rc != 0)
			mutex_lock_fail_cgminer("hash_driver_work", 0x23a0);
		((drv_flush_fn)LDptr(drv, DRV_FLUSH))(cgpu);
		rc = pthread_mutex_unlock(&DAT_0007b85c);
		if (rc != 0)
			mutex_unlock_fail_cgminer("hash_driver_work", 0x23a2);
		(*DAT_00075ed8)();
	}
}

// === __quit === 原 FUN_0001d8fc @ 0x0001d8fc
// 来源: reversed/cgminer/20_job_mining/FUN_0001d8fc.c | 分类: 20_job_mining
// 语义: 双参退出总控 (单参 quit/00016ca8 包装它): 先创建 delayed_exit_thread
//       (失败直接 exit(1)); wait_for_threads=0 仅 unlock_curses, 否则记录退出
//       时刻 (cgtime → DAT_00077148) + unlock_curses + (未被抑制时) print_summary;
//       随后 SIGTERM 掉记录的子进程 pid (DAT_0007728c), pthread_cancel 延时线程,
//       最终 exit(status) 不返回。
void __quit(int status, int wait_for_threads)
{
	int rc;
	pthread_t thread;

	rc = pthread_create(&thread, NULL, (void *(*)(void *))delayed_exit_thread, NULL);
	if (rc != 0)
		exit(1);
	if (wait_for_threads == 0) {
		unlock_curses();
	} else {
		cgtime(&DAT_00077148);                       /* 记录退出时刻 */
		unlock_curses();
		if (DAT_0007bb18 == '\0' && DAT_00078ca9 != '\0')
			print_summary();
	}
	if (0 < DAT_0007728c) {
		kill(DAT_0007728c, 0xf);                     /* SIGTERM */
		DAT_0007728c = 0;
	}
	pthread_cancel(thread);
	exit(status);
}

// === inc_work_stats === 原 FUN_0001f938 @ 0x0001f938
// 来源: reversed/cgminer/20_job_mining/FUN_0001f938.c | 分类: 20_job_mining
// 语义: hash_lock (DAT_0007ad4c) 下按符号扩展 64 位把 addend 累加进全局统计
//       (DAT_0007b100 u64) 与设备累计 (cgpu+0xc0); 池累计 (+0x28) 的目标池为
//       NULL 时取 current_pool(); 随后刷新设备最近工作时刻 (+0xe8)。
//       (机器码 (int)addend>>31 为符号位扩展进高字, 低字加法进位一并累加。)
void inc_work_stats(void *thr, struct pool *pool, uint32_t addend)
{
	int rc;
	int64_t add64;
	void *cgpu;
	struct pool *target;

	rc = pthread_mutex_lock(&DAT_0007ad4c);
	if (rc != 0)
		mutex_lock_fail_cgminer("inc_work_stats", 0x20c9);

	add64 = (int32_t)addend;                         /* 符号扩展 (机器码 ASR #31) */
	DAT_0007b100 = (uint64_t)((int64_t)DAT_0007b100 + add64);
	cgpu = LDptr(thr, THR_CGPU);
	LD64(cgpu, CG_DIFFACC) = (uint64_t)((int64_t)LD64(cgpu, CG_DIFFACC) + add64);
	if (pool == NULL) {
		target = (struct pool *)current_pool();
		LD64(target, PL_DIFFACC) = (uint64_t)((int64_t)LD64(target, PL_DIFFACC) + add64);
	} else {
		LD64(pool, PL_DIFFACC) = (uint64_t)((int64_t)LD64(pool, PL_DIFFACC) + add64);
	}
	cgpu = LDptr(thr, THR_CGPU);
	LD64(cgpu, CG_LAST_WORK) = (uint64_t)time(NULL);

	rc = pthread_mutex_unlock(&DAT_0007ad4c);
	if (rc != 0)
		mutex_unlock_fail_cgminer("inc_work_stats", 0x20d6);
	(*DAT_00075ed8)();
}

// === submit_noffset_nonce === 原 FUN_00025cc0 @ 0x00025cc0
// 来源: reversed/cgminer/20_job_mining/FUN_00025cc0.c | 分类: 20_job_mining
// 语义: 变长 nonce2 偏移提交路径 (与上游 submit_noffset_nonce 结构吻合 [中置信]):
//       make_work + copy_work(new, base, nonce_offset) 后 test_nonce(work, nonce),
//       失败释放工作并 inc_hw_errors 返回 0; 通过则 update_work_stats(thr, work)、
//       基准文件使能 (DAT_00076ffc/DAT_0007871c) 时 benchfile_dspwork(work, nonce),
//       fulltest 未过目标时释放工作并打印 "%s %d: Share above target" (返回 1),
//       过目标则 submit_work_async(work) 异步提交 (返回 1)。
int submit_noffset_nonce(void *thr, struct work *base, uint32_t nonce, int nonce_offset)
{
	char msg[4100];
	struct work *work;
	void *cgpu;

	work = make_work();
	copy_work(work, base, nonce_offset);
	if (!test_nonce(work, nonce)) {
		free_work(work);
		inc_hw_errors(thr);
		return 0;
	}
	update_work_stats(thr, work);
	if (DAT_00076ffc != 0 && DAT_0007871c != '\0')
		benchfile_dspwork(work, nonce);
	if (!fulltest(P8(work, WK_HASH), P8(work, WK_TARGET))) {
		free_work(work);
		if (DAT_00078e09 != '\0' &&
		    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 6 < DAT_000766c4)) {
			cgpu = LDptr(thr, THR_CGPU);
			snprintf(msg, 0x1000, "%s %d: Share above target",
			         *(char **)P8(LDptr(cgpu, CG_DRV), DRV_NAME),
			         LDs32(cgpu, CG_DEV_ID));
			applog(7, msg, 0);
		}
		return 1;
	}
	submit_work_async(work);
	return 1;
}

// === submit_tested_work === 原 FUN_00025e5c @ 0x00025e5c
// 来源: reversed/cgminer/20_job_mining/FUN_00025e5c.c | 分类: 20_job_mining
// 语义: 提交已测工作: update_work_stats(thr, work) (Ghidra 丢失实参, 按 0x25ex
//       处寄存器现场恢复) 后 fulltest 校验目标; 未过目标时打印
//       "%s %s %d: Share above target" (函数名+驱动名+设备号) 并返回 0;
//       通过则 make_clone(work, 0) 克隆并 submit_work_async(克隆) 异步提交,
//       返回 1。
int submit_tested_work(void *thr, struct work *work)
{
	char msg[4100];
	void *cgpu;

	update_work_stats(thr, work);
	if (!fulltest(P8(work, WK_HASH), P8(work, WK_TARGET))) {
		if (DAT_00078e09 != '\0' &&
		    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 6 < DAT_000766c4)) {
			cgpu = LDptr(thr, THR_CGPU);
			snprintf(msg, 0x1000, "%s %s %d: Share above target", "submit_tested_work",
			         *(char **)P8(LDptr(cgpu, CG_DRV), DRV_NAME),
			         LDs32(cgpu, CG_DEV_ID));
			applog(7, msg, 0);
		}
		return 0;
	}
	submit_work_async(make_clone(work, 0));          /* 克隆后异步提交 (原码 r0 直通) */
	return 1;
}

// === enable_curses === 原 FUN_00027ba0 @ 0x00027ba0
// 来源: reversed/cgminer/20_job_mining/FUN_00027ba0.c | 分类: 20_job_mining
// 语义: 初始化 curses UI (curses_lock/DAT_0007b7a8 保护, 经 DAT_00077018 仅一次):
//       initscr 建 mainwin, 按 mainwin 尺寸建 status_win (newwin(DAT_0007701c,
//       宽, 0, 0), leaveok) 与 log_win (newwin(主高-DAT_00077024, 0, 起始行, 0),
//       idlok/scrollok/leaveok), cbreak/noecho, 置已初始化标志与 status 行数快照。
//       内联 lock_curses/unlock_curses (锁错误助手串 "lock_curses"/"unlock_curses")。
void enable_curses(void)
{
	int rc;
	int main_rows;
	int main_cols;

	rc = pthread_mutex_lock(&DAT_0007b7a8);
	if (rc != 0)
		mutex_lock_fail_cgminer("lock_curses", 0xb77);
	if (DAT_00077018 == '\0') {                      /* 尚未初始化 */
		DAT_0007bc90 = initscr();                    /* mainwin */
		if (DAT_0007bc90 == NULL) {
			main_rows = -1;
			main_cols = -1;
		} else {
			main_rows = (int)*(short *)P8(DAT_0007bc90, 4) + 1;   /* _maxy+1 */
			main_cols = (int)*(short *)P8(DAT_0007bc90, 6) + 1;   /* _maxx+1 */
		}
		DAT_0007b7c0 = newwin(DAT_0007701c, main_cols, 0, 0);            /* status_win */
		leaveok(DAT_0007b7c0, 1);
		DAT_0007b430 = newwin(main_rows - DAT_00077024, 0, DAT_00077024, 0);  /* log_win */
		idlok(DAT_0007b430, 1);
		scrollok(DAT_0007b430, 1);
		leaveok(DAT_0007b430, 1);
		cbreak();
		noecho();
		DAT_00077018 = '\x01';                       /* curses_active = true */
		DAT_00077020 = DAT_0007701c;                 /* status 行数快照 */
		rc = pthread_mutex_unlock(&DAT_0007b7a8);
	} else {
		rc = pthread_mutex_unlock(&DAT_0007b7a8);
	}
	if (rc != 0)
		mutex_unlock_fail_cgminer("unlock_curses", 0xb72);
	(*DAT_00075ed8)();
}

// === fill_drv_defaults === 原 FUN_00027cd0 @ 0x00027cd0
// 来源: reversed/cgminer/20_job_mining/FUN_00027cd0.c | 分类: 20_job_mining
// 语义: 设备驱动结构默认值填充: 对 +0xc..+0x60 的空函数指针逐项填入代码段默认
//       小桩 DAT_0000f7xx/LAB_0000f7xx (Ghidra 以数据/标签标注, 上游名不确定
//       [低置信]); +0x3c (miner_thread 钩子) 缺省为 miner_thread_work_loop
//       (00024db0); +0x68 浮点字段为 0.0 时填 1.0。
void fill_drv_defaults(void *drv)
{
	if (LDptr(drv, 0x0c) == NULL)
		LDptr(drv, 0x0c) = (void *)DAT_0000f7d8;
	if (LDptr(drv, 0x10) == NULL)
		LDptr(drv, 0x10) = (void *)DAT_0000f794;
	if (LDptr(drv, 0x14) == NULL)
		LDptr(drv, 0x14) = (void *)DAT_0000f798;
	if (LDptr(drv, 0x18) == NULL)
		LDptr(drv, 0x18) = (void *)DAT_0000f79c;
	if (LDptr(drv, 0x20) == NULL)
		LDptr(drv, 0x20) = (void *)DAT_0000f7a0;    /* Ghidra 标注 LAB_0000f7a0 */
	if (LDptr(drv, 0x2c) == NULL)
		LDptr(drv, 0x2c) = (void *)DAT_0000f7a8;    /* LAB_0000f7a8 */
	if (LDptr(drv, 0x30) == NULL)
		LDptr(drv, 0x30) = (void *)DAT_0000f7b0;    /* LAB_0000f7b0 */
	if (LDptr(drv, 0x34) == NULL)
		LDptr(drv, 0x34) = (void *)DAT_0000f7bc;    /* LAB_0000f7bc */
	if (LDptr(drv, 0x38) == NULL)
		LDptr(drv, 0x38) = (void *)DAT_0000f7c4;    /* LAB_0000f7c4 */
	if (LDptr(drv, 0x54) == NULL)
		LDptr(drv, 0x54) = (void *)DAT_0000f7cc;
	if (LDptr(drv, 0x58) == NULL)
		LDptr(drv, 0x58) = (void *)DAT_0000f7d0;
	if (LDptr(drv, 0x5c) == NULL)
		LDptr(drv, 0x5c) = (void *)DAT_0000f7d4;
	if (LDptr(drv, 0x3c) == NULL)
		LDptr(drv, 0x3c) = (void *)miner_thread_work_loop;   /* miner_thread 钩子 */
	if (LDptr(drv, 0x4c) == NULL)
		LDptr(drv, 0x4c) = (void *)DAT_0000f794;
	if (LDptr(drv, 0x50) == NULL)
		LDptr(drv, 0x50) = (void *)DAT_0000f794;
	if (LDptr(drv, 0x48) == NULL)
		LDptr(drv, 0x48) = (void *)DAT_0000f7a0;    /* LAB_0000f7a0 */
	if (LDptr(drv, 0x60) == NULL)
		LDptr(drv, 0x60) = (void *)DAT_0000f794;
	if (LDf64(drv, 0x68) == 0.0)
		LDf64(drv, 0x68) = 1.0;                      /* 机器码写位型 0x3ff00000 (1.0) */
}

/* ============================================================
 * 覆盖率表 (17/17 — job_p5a, 全部完成)
 * ============================================================
 * | #  | 原地址     | 语义名                   | 状态 |
 * |----|------------|--------------------------|------|
 * | 1  | 0x00018068 | check_winsizes           | 完成 |
 * | 2  | 0x000185c8 | clear_pool_work          | 完成 |
 * | 3  | 0x000199e8 | log_curses_only          | 完成 |
 * | 4  | 0x00019fc8 | age_queued_work          | 完成 |
 * | 5  | 0x0001a3d4 | make_work                | 完成 |
 * | 6  | 0x0001a4b4 | share_diff               | 完成 |
 * | 7  | 0x0001a730 | update_work_stats        | 完成 |
 * | 8  | 0x0001a914 | test_nonce_fulltest      | 完成 |
 * | 9  | 0x0001aa18 | flush_queue              | 完成 |
 * | 10 | 0x0001ac00 | clone_queued_work_byid   | 完成 |
 * | 11 | 0x0001c3f4 | hash_driver_work         | 完成 |
 * | 12 | 0x0001d8fc | __quit                   | 完成 |
 * | 13 | 0x0001f938 | inc_work_stats           | 完成 |
 * | 14 | 0x00025cc0 | submit_noffset_nonce     | 完成 |
 * | 15 | 0x00025e5c | submit_tested_work       | 完成 |
 * | 16 | 0x00027ba0 | enable_curses            | 完成 |
 * | 17 | 0x00027cd0 | fill_drv_defaults        | 完成 |
 * ============================================================ */
