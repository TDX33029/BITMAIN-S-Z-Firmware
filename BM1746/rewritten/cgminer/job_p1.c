/*
 * cgminer 20_job_mining 主初始化 (批次 job_b1) — BM1746 固件语义转写
 * =========================================================
 * 原始二进制: bm1746/cgminer_z15_bm1746.elf (ARM32, SD 镜像 cgminer-z15, branch "bm1746")
 * 来源:      reversed/cgminer/20_job_mining/FUN_0000b928.c (函数体 2491 行, ~60 局部变量, 4KB+ 栈帧)
 *
 * 本文件仅含一个巨函数 bitmain_zcash_driver_init (原 FUN_0000b928), 即 cgminer 主初始化
 * 入口 (argc/argv 形参, 由 entry.c 以 (argc, argv) 调用后永驻主循环)。
 *
 * 全函数按初始化序列分 10 段转写, 各段角色见函数体内分段横幅与文件末尾清单。
 *
 * 转写约定 (rewritten/CONVENTIONS.md):
 *   - 命名事实源 rewritten/name_map/cgminer.tsv; FUN_0000b928 语义名 bitmain_zcash_driver_init
 *     (注: bitmain_ZCASH_init 权威名属 FUN_00049cc8, 勿混用)。
 *   - 未知 DAT_xxx 全局保留原名 + 注释; 日志字符串原样保留。
 *   - Ghidra 伪变量 (extraout_ 与 code 型指针, Heritage WARNING) 已消除。
 */

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <sched.h>
#include <libgen.h>
#include <sys/time.h>

#include "bm1746_regs.h"
#include "common_types.h"
#include "fpga_axi.h"

/* ---- 本文件引用的外部全局 (DAT_ 命名待补, 按约定保留原名) ---- */
extern const char DAT_0005db00[];     /* 错误报告用源文件串 (锁初始化 __FILE__ 槽) */
extern void  *tq_new(void);           /* FUN_0002a168: calloc(1,0x58) 任务队列 */
extern void sighandler(int sig);      /* FUN_0001795c: SIGTERM/SIGINT 处理器 */
extern uint16_t DAT_00078de8;         /* applog fopen 模式串首半字: 'a','+' → "a+" */
extern uint16_t DAT_00078dea;         /* 模式串尾半字: 0 → NUL 终止 */
extern char     DAT_00078ce8[];       /* 日志文件名缓冲 (applog: fopen(名, 模式)) */
extern uint8_t  DAT_00078ce0;         /* 日志文件写开关 (applog: 非 0 才写文件) */
extern char   **DAT_00077290;         /* argv 备份数组 (restart_mine FUN_0001d7c8 execv 用) */
extern void   (*DAT_00075ed8)(void);  /* 让步钩子: ELF 初值 = noop_hook(FUN_00028ae0, 恒返 0);
                                       * 单核系统在本函数改写为 sched_yield (hash_push/
                                       * share_diff/inc_hw_errors/wake_gws 等尾部调用) */
extern void mutex_init(pthread_mutex_t *lock, const char *file, int line);   /* FUN_00017990 */
extern void cglock_init(pthread_mutex_t *lock, const char *file, int line);  /* FUN_000179e8: mutex+rwlock 组合锁 */
extern void rwlock_init(pthread_rwlock_t *lock, const char *file, int line); /* FUN_000179bc */
extern void applog(uint32_t prio, char *buf, ...);                  /* FUN_00038730 (job_p5b); 可变参声明同 log.c */
extern void quit(int status);                                                /* FUN_00016ca8: 单参包装, 内部走 __quit */
extern void __quit(int status, int silent);                                  /* FUN_0001d8fc: 延迟线程 + 清理后 exit, 不返回 */

/* ---- 锁族 (初始化序见段 1; 身份由使用方函数核实, 见各注释) ---- */
extern pthread_mutex_t  DAT_0007b384;  /* hash_lock (hashmeter/zero_stats/summary 持有) */
extern pthread_mutex_t  DAT_0007b85c;  /* 互斥锁, hash_driver_work (FUN_0001c3f4) 持有 */
extern pthread_mutex_t  DAT_0007b7a8;  /* curses 互斥锁 (curses_active_locked/check_winsizes/enable_curses) */
extern pthread_mutex_t  DAT_0007ba10;  /* control_lock 组合锁 (switch_pools/current_pool/share_diff) */
extern pthread_mutex_t  DAT_0007ad4c;  /* hash_lock (enable_device/hash.c 设备表; 与 7b384 不同, 两把) */
extern pthread_mutex_t  DAT_000770b8;  /* sharelog 写入互斥锁 (sharelog FUN_0001b600) */
extern pthread_mutex_t  DAT_0007b9d8;  /* ch_lock 组合锁 (test_work_current/minecoin) */
extern pthread_mutex_t  DAT_00077030;  /* stratum_lock (stratum_rthread/sthread/clear_stratum_shares/watchpool_thread) */
extern pthread_rwlock_t DAT_00077294;  /* 读写锁 (test_work_current 持有) */
extern pthread_rwlock_t DAT_0007b2d8;  /* 读写锁 (语义待定) */
extern pthread_rwlock_t DAT_0007b650;  /* mining_thr 数组锁 (get_thread 读 / watchdog_thread/restart_thread 写) */
extern pthread_rwlock_t DAT_0007b700;  /* devices_lock (enable_device/hash.c 设备数组) */
extern pthread_mutex_t  DAT_00077060;  /* 互斥锁 (switch_pools/stratum_rthread 持有) */
extern pthread_cond_t   DAT_00077078;  /* lp_cond (错误串 "Failed to pthread_cond_init lp_cond") */
extern pthread_mutex_t  DAT_0007b62c;  /* restart_lock (restart_wait/restart_thread 与 restart_cond 配对) */
extern pthread_cond_t   DAT_0007bae0;  /* restart_cond (上游同名) */
extern pthread_cond_t   DAT_0007b1a0;  /* gws_cond (错误串 "Failed to pthread_cond_init gws_cond") */
extern void  *DAT_0007b620;            /* getq 任务队列 (tq_new 结果, 0x58B: +0/+4 链头, +0xc mutex, +0x28 cond) */
extern pthread_mutex_t *DAT_0007704c;  /* = getq+0xc (tq->mutex) 指针别名 */
extern char     DAT_00076bf4[];       /* packagename 串缓冲 (256B, "cgminer 4.9.0") */
extern struct sigaction DAT_0007b108;  /* SIGTERM 旧处理动作 (sigaction 第 3 参保存) */
extern struct sigaction DAT_0007b94c;  /* SIGINT 旧处理动作 */
extern char    *DAT_0007bc74;          /* "/usr/bin" 字符串指针 (段 1 栈上构造) */
extern char    *DAT_0007b8b4;          /* argv[0] 目录名缓冲 (4104B 栈缓冲首址, 本函数内使用) */
extern int      DAT_00077028;         /* curses 布局行号 A (初值 8) */
extern int      DAT_0007701c;         /* curses 布局行号 B (初值 9) */
extern int      DAT_00077024;         /* curses 布局行号 C (初值 10) */
extern uint8_t *DAT_000772b4;         /* workid 哈希表载体指针: 指向哨兵 workid 容器 (段 2 建表)
                                       * [存证互引] job_p2.c 将同一指针判读为块库锚节点 (证据: block_exists
                                       * 路径 / "Deleted block %d" 字符串, 见其 DAT_000772b4 注释); 本文件按
                                       * 段 2 建表机器码判读为 workid 表载体 (证据: Jenkins lookup2 哈希 /
                                       * 0x20 桶 / 种子 0xa0111fe1 / 节点+0x44 表指针), 两说并存待裁 */
extern int      DAT_0007728c;         /* 子进程 PID (__quit 中 kill(pid, SIGTERM)) */
extern char    *DAT_00076be8;         /* 文件名前缀 (段 4: "%s%02d.txt" sprintf) */

extern void opt_parse(int *argc, char **argv, void (*errlog)(const char *fmt, ...)); /* FUN_0005bc84 */
extern void opt_parse_err(const char *fmt, ...);   /* @0x11c44 命名待补: opt_parse 错误回调
                                                    * (printf 型 stub, Ghidra 未单独反编译) */
extern void opt_register_table(void *table, const char *summary);  /* FUN_0005bbb8 */
extern char  DAT_0007bb28[];   /* 当前 job_id 缓冲 (test_work_current/minecoin 比较用) */
extern void *DAT_0007b440;     /* 空双向链表头: next = self (本函数初始化) */
extern void *DAT_0007b444;     /* 双向链表 prev 槽 = 头 */

/* ---- 段 3 引用 (选项/配置/日志全局; 类型与 log.c/json.c/cfg.c/pool_p2.c 一致) ---- */
extern uint8_t DAT_0007ad48;          /* applog 门控 A: 非 0 → applog 走 syslog(prio|0x80) */
extern uint8_t DAT_00078e08;          /* applog 门控 B = opt_verbose */
extern int     DAT_000766c4;          /* applog 门控 C: 日志级别 (LOG_ERR=3 阈值) */
extern uint8_t DAT_00076f20;          /* 配置已加载标志 (json.c 同名) */
extern char   *DAT_00076f24;          /* 配置文件路径缓冲 (0x1000, json.c/cfg.c 同名) */
extern char   *DAT_00076bdc;          /* default_config ("config" 命令记录路径; cfg.c) */
extern void   default_save_file(char *filename);   /* FUN_00016150 (cfg.c 定义) */
extern char   *load_json_file(const char *filename); /* FUN_00016d98 (json.c 定义) */
extern uint8_t DAT_0007730d;          /* opt_benchmark 标志 (pool_p2.c [低置信] → 本段核实) */
extern int     DAT_00076ffc;          /* opt_benchfile 标志 (pool_p2.c [低置信] → 本段核实) */
extern uint8_t DAT_0007b859;          /* opt_scrypt 标志 ("Cannot use benchmark mode with scrypt" 门) */
extern int    *add_pool(void);        /* FUN_00017a3c: calloc(0x750) pool, 失败报错退出 */
extern int     DAT_0007b5e4;          /* total_pools (util.c 同名) */
extern uint8_t DAT_00078ca9;          /* benchmark 运行标志 (json.c: 授权成功亦置 1) */
extern int     hex2bin(uint8_t *dst, const uint8_t *hex, uint32_t len); /* FUN_00029830 */
extern void    set_target(double diff, void *target);  /* FUN_00016244: d0=难度, r1=目标缓冲 */
extern uint8_t DAT_0007731c[0xa00];   /* benchmark 工作数据 A (16×0xa0 解码目标) */
extern uint8_t DAT_00077d1c[0xa00];   /* benchmark 工作数据 B */
extern uint32_t DAT_00078730[];       /* benchmark 目标缓冲 (set_target 第 2 参) */
extern char   *DAT_00076bf0;          /* miner version 文件路径 (--version-file; cfg.c 定义) */
extern char   DAT_00078750[];         /* Miner compile time 串缓冲 (axi.c API "CompileTime") */
extern char   DAT_00078850[];         /* Miner type 串缓冲 (axi.c API "Type") */
extern char   *DAT_00076be0;          /* opt_logfile 日志文件路径 (--log-file) */
extern char   *DAT_00076bec;          /* 日志打开模式串 (--log-file 模式; 覆盖默认 "a+") */
extern char   *DAT_00076be8;          /* log-work 路径前缀 (--log-work) */
extern char   *DAT_00076be4;          /* log-work asic 数字符串 */
extern int     DAT_00078950;          /* log-work asic 数 (strtol 结果) */
extern FILE   *DAT_00078954;          /* log-work 工作日志流 ("%s.txt") */
extern FILE   *DAT_00078bd0[];        /* FILE* 槽区锚点 @0x78bd0 (per-asic 首槽 = 0x78958) */
extern uint8_t DAT_00078a5c;          /* diff 日志槽区标志位 (非 0 才开 "%s_diff_NN.txt") */
extern uint8_t DAT_0007bb18;          /* 禁用 curses 标志 (非 0 时清 DAT_00075dd0) */
extern uint8_t DAT_00075dd0;          /* 想要 curses 界面标志 (非 0 → enable_curses) */
extern int     DAT_00076ff4;          /* 配置解析结果: -1 = 部分加载, 0 = 失败, 其他 = 成功 */
extern uint8_t DAT_0007b858;          /* Per-device stats 开关 (curses 'p'; 非 0 强制 verbose) */
extern uint8_t DAT_00078e09;          /* opt_debug (log.c 同名) */
extern void    enable_curses(void);   /* FUN_00027ba0 */

/* applog 调用点内联门控 (续):
 *   门控 7 (prio 7, opt_debug 外层): DAT_00078e09 && (A || B || C > 6)
 *   门控 4 (prio 4):                 A || B || C > 3                          */
#define APPLOG_GATE7()  (DAT_00078e09 && (DAT_0007ad48 || DAT_00078e08 || 6 < DAT_000766c4))
#define APPLOG_GATE4()  (DAT_0007ad48 || DAT_00078e08 || 3 < DAT_000766c4)

/* ---- 段 5 引用 (设备/驱动/矿池) ---- */
extern int    *DAT_0007b628;          /* devices[] 设备指针数组 (hash.c 同名) */
extern int     DAT_0007b720;          /* total_devices (api_p1a.c 同名) */
extern int     DAT_00078720;          /* 已使能设备计数 (enable_device 递增) */
extern int     DAT_00078724;          /* 备用线程累计 (DAT_0007b7d0 置位时用) */
extern uint8_t DAT_0007b7d0;          /* 计数选择标志 (0 → 累计到 DAT_0007bb6c) */
extern int     DAT_0007bb6c;          /* mining_threads: 设备线程累计 (util.c 同名) */
extern int     DAT_0007702c;          /* mining_threads 别名 = 设备总数 (hash.c/log.c 双用途) */
extern void    fill_drv_defaults(void *drv);        /* FUN_00027cd0: 补默认函数指针 */
extern void    enable_device(int *dev);             /* FUN_00027f10: 登记+使能设备 */
extern void    _rd_lock(void *lock, const char *file, int line);  /* FUN_0001ab24 */
extern void    rw_unlock(void *lock, const char *file, int line); /* FUN_00019bbc */
extern char   *DAT_00076f00;          /* --temp-cutoffs 逗号分隔串 */
extern uint8_t DAT_0007b940;          /* compact 模式开关 (log.c 同名) */
extern void    check_winsizes(void);  /* FUN_00018068 */
extern int     DAT_0007b198;          /* total_pools (api_p1a.c 同名) */
extern int     input_pool(int interactive);         /* FUN_00026d10 */
extern int    *DAT_0007b438;          /* pools[] 矿池指针数组 (api_p1a.c 同名) */
extern void   *DAT_0007705c;          /* current_pool: 首个矿池指针 (段 5 末赋值) */
extern void  (*DAT_00075ed8)(void);   /* 让步钩子 (段 1 已声明, enable_device 尾调) */
extern uint8_t DAT_0007b858;          /* Per-device stats (段 4 已声明) */
extern int     DAT_00078b64;          /* 挖矿线程槽数 = 8 (与 DAT_0007b0fc 配套) */
extern void   *DAT_0007b0fc;          /* control_thr: 8×0x40B thr_info 槽 (失败报
                                       * "Failed to calloc control_thr", 原 LAB_0000e128) */
extern int     DAT_00078b68;          /* thr 槽使用计数 (清 0) */
extern int     DAT_00078b6c;          /* --monitor 标志 (原 puVar37(&DAT_00078bd0)-100) */

/* 驱动表: 24 个编译进固件的驱动 (struct 顺序 = 探测顺序); 槽位 +0xc = drv_detect */
extern uint8_t DAT_0007b728[]; extern void *DAT_0007b734;
extern uint8_t DAT_0007acc8[]; extern void *DAT_0007acd4;
extern uint8_t DAT_0007b7d8[]; extern void *DAT_0007b7e4;
extern uint8_t DAT_0007b448[]; extern void *DAT_0007b454;
extern uint8_t DAT_00076ac8[]; extern void **PTR_FUN_00076ad4;  /* 双重指针槽 */
extern uint8_t DAT_0007bca0[]; extern void *DAT_0007bcac;
extern uint8_t DAT_0007b4d8[]; extern void *DAT_0007b4e4;
extern uint8_t DAT_0007b678[]; extern void *DAT_0007b684;
extern uint8_t DAT_0007ba48[]; extern void *DAT_0007ba54;
extern uint8_t DAT_0007ad68[]; extern void *DAT_0007ad74;
extern uint8_t DAT_0007bb70[]; extern void *DAT_0007bb7c;
extern uint8_t DAT_0007ae70[]; extern void *DAT_0007ae7c;
extern uint8_t DAT_0007b300[]; extern void *DAT_0007b30c;
extern uint8_t DAT_0007b258[]; extern void *DAT_0007b264;
extern uint8_t DAT_0007af70[]; extern void *DAT_0007af7c;
extern uint8_t DAT_0007b8c0[]; extern void *DAT_0007b8cc;
extern uint8_t DAT_0007b078[]; extern void *DAT_0007b084;
extern uint8_t DAT_0007aff0[]; extern void *DAT_0007affc;
extern uint8_t DAT_0007b560[]; extern void *DAT_0007b56c;
extern uint8_t DAT_0007b3a0[]; extern void *DAT_0007b3ac;
extern uint8_t DAT_0007b1d0[]; extern void *DAT_0007b1dc;
extern uint8_t DAT_0007bbf0[]; extern void *DAT_0007bbfc;
extern uint8_t DAT_0007aef0[]; extern void *DAT_0007aefc;
extern uint8_t DAT_0007ade8[]; extern void *DAT_0007adf4;

/* ---- 段 6/7 引用 (线程框架与主循环) ---- */
#include <curses.h>                          /* WINDOW/wtouchln/wrefresh/halfdelay/wgetch — 原经 miner.h 引入 */
extern void fun_0001fe0c(void *pool);        /* 池线程入口 @0x1fe0c 命名待补 */
extern void *get_thread(int idx);            /* FUN_0001b124: &mining_thr[idx] */
extern int   thr_info_create(void *thr, void *attr, void *(*fn)(void *), void *arg); /* FUN_0002f174 */
extern void  _cgsem_post(void *sem, const char *file, const char *file2, int line); /* FUN_0002f1bc */
extern void *miner_thread(void *arg);        /* FUN_00011e24 */
extern void *watchpool_thread(void *arg);    /* FUN_00021850 */
extern void *watchdog_thread(void *arg);     /* FUN_00023344 */
extern int  *DAT_0007b5e0;                   /* mining_thr: thr_info* 数组 */
extern uint32_t DAT_00075600[];              /* opt_config_table: 静态双选项表 (cfg.c) */
extern uint32_t DAT_00075e24[];              /* 静态命令行专属选项表 (cfg.c) */
extern int      DAT_00075df0;                /* opt_scantime (cfg.c, ELF 初值 -1) */
extern uint8_t DAT_0007730c;                 /* 池可用/退出标志 (pool-wait 循环条件, 语义待定) */
extern WINDOW *DAT_0007b430;                 /* logwin: curses 日志窗口 (cfg.c/log.c 同名) */
extern WINDOW *stdscr;                       /* curses 标准屏 */
extern int     wtouchln(WINDOW *w, int y, int n, int changed);
extern int     wrefresh(WINDOW *w);
extern int     halfdelay(int tenths);
extern int     wgetch(WINDOW *w);
extern int     cbreak(void);
extern uint64_t DAT_0007acc0;                /* API 统计 u64 (api_p1a.c 同名) */
extern uint32_t DAT_0007acc4;                /* 相邻统计字 */
extern uint8_t DAT_000770e0[];               /* 统计清零区起点 (0x60B 至 0x787140) */
extern struct timeval DAT_000770d0;          /* 时间戳 (cgtime) */
extern struct timeval DAT_00077148;          /* 时间戳 (cgtime) */
extern struct timeval DAT_000770d8;          /* 时间戳 (cgtime) */
extern struct timeval DAT_00076f0c;          /* work update 收到时间 */
extern char    DAT_00077260[0x28];           /* 日期串缓冲 (get_datestamp) */
extern void    get_datestamp(char *buf, size_t len, struct timeval *tv); /* FUN_00015264 */
extern void    cgtime(struct timeval *tv);   /* FUN_0002a84c */
extern int     DAT_000770b0;                 /* watchpool 线程槽号 = 2 */
extern int     DAT_000770b4;                 /* watchdog 线程槽号 = 3 */
extern uint8_t DAT_0007b8ac;                 /* work update 请求标志 */
extern void   *DAT_00077050;                 /* staged_work 链表头 (job_p3.c 同名) */
extern int     DAT_00075dd8;                 /* getwork 间累计参数 A */
extern int     DAT_00075ddc;                 /* getwork 间累计参数 B (上限) */
extern uint8_t DAT_00077056;                 /* getwork 间累计标志 */
extern uint8_t DAT_00077055;                 /* 主循环等待标志 */
extern void   *current_pool(void);           /* FUN_0001da58 */
extern void    rd_lock_error(const char *fn, int line);    /* FUN_0000b69c */
extern void    rw_unlock_error(const char *fn, int line);  /* FUN_0000b630 */
extern void    mutex_lock_fail_cgminer(const char *file, int line);   /* FUN_0000b414 */
extern void    mutex_unlock_fail_cgminer(const char *file, int line); /* FUN_0000b5c4 */
extern int     hash_pop(int mode);           /* FUN_000193b4 */
extern void    discard_work(void *work);     /* FUN_000159b4 */
extern void   *make_work(void);              /* FUN_0001a3d4 */
extern void   *select_pool(void *pool);      /* FUN_00012250 (job_p5.c 定义为 void;
                                               * 机器码 r0 返回选中池, 本文件按返回值使用) */
extern int    *priority_pool(int idx);       /* FUN_00012038 */
extern void    gen_stratum_work(void *pool, void *work);  /* FUN_0001acc4 */
extern void    hash_push(void *work);        /* FUN_0001f17c (job_p3.c 定义) */
extern void    benchfile_get_work(void);     /* FUN_00017254 */
extern void    calc_diff(int mode, void *work); /* FUN_00011a5c */
extern void   *api_thread(void *arg);        /* FUN_00012164 */
extern void   *input_thread(void *arg);      /* FUN_00027a98 */
extern void    cgsleep_ms(int ms);           /* FUN_0002a92c */
extern void    copy_timeval(struct timeval *dst, const struct timeval *src); /* FUN_0002a530 */
extern int     DAT_00076efc;                 /* 挖矿策略 (axi.c: 4=balance 3=load-balance) */
extern uint8_t DAT_0007b625;                 /* opt_failover_only (freq_p4.c 同名) */
extern int     DAT_00078b74;                 /* load-balance 轮换下标 */
extern int     DAT_00076ef8;                 /* api 线程槽号 = 5 */

/* 驱动探测调用: 槽位存函数指针, 机器码以 r0=0 调用 */
#define DRV_PROBE(slot) (((void (*)(int))(uintptr_t)(slot))(0))




/* applog 调用点内联门控 (同 chain.c/uart.c 模式; 此处无 DAT_00078e09 外层):
 *   门控 3 (prio 3): A || B || C > 2                                        */
#define APPLOG_GATE3()  (DAT_0007ad48 || DAT_00078e08 || 2 < DAT_000766c4)

/* ---- workid 哈希表布局 (与 util.c workid_hash_add/FUN_00016624 完全同构) ----
 * 表头 0x2cB: +0x00 桶数组, +0x04 桶数, +0x08 shift, +0x0c 节点总数,
 *             +0x10 全局 FIFO 尾节点, +0x14 节点在容器内偏移 (哨兵=0x44, work=0x728),
 *             +0x18 load_cap, +0x1c over_cnt, +0x20 grow_cnt, +0x24 frozen, +0x28 seed。
 * 节点 (容器内偏移 0x44 起): +0x00 表指针, +0x04 FIFO 前驱, +0x08 FIFO 后继,
 *             +0x0c 桶链前驱, +0x10 桶链后继, +0x14 键指针, +0x18 键长, +0x1c 哈希值。
 * 桶 12B: +0x00 链头节点, +0x04 计数, +0x08 watermark。 */
typedef struct seg2_workid_ht {
    uint32_t *buckets;     /* +0x00 */
    uint32_t nbuckets;     /* +0x04 */
    uint32_t shift;        /* +0x08 */
    uint32_t count;        /* +0x0c */
    uint32_t tail_node;    /* +0x10 */
    uint32_t node_off;     /* +0x14 */
    uint32_t load_cap;     /* +0x18 */
    uint32_t over_cnt;     /* +0x1c */
    uint32_t grow_cnt;     /* +0x20 */
    uint32_t frozen;       /* +0x24 */
    uint32_t seed;         /* +0x28 */
} seg2_workid_ht_t;

/* Jenkins lookup2 型 mix (9 轮, 移位 13/8/13/12/16/5/3/10/15)。
 * 对应反编译 365-373 行 (尾部终混) 与 346-354 行 (主循环轮, 首轮并入 a+=w0)。 */
#define JHASH_MIX2(a, b, c) do { \
    (a) = ((a) - (b) - (c)) ^ ((c) >> 13); \
    (b) = ((b) - (c) - (a)) ^ ((a) << 8); \
    (c) = ((c) - (a) - (b)) ^ ((b) >> 13); \
    (a) = ((a) - (b) - (c)) ^ ((c) >> 12); \
    (b) = ((b) - (c) - (a)) ^ ((a) << 16); \
    (c) = ((c) - (a) - (b)) ^ ((b) >> 5); \
    (a) = ((a) - (b) - (c)) ^ ((c) >> 3); \
    (b) = ((b) - (c) - (a)) ^ ((a) << 10); \
    (c) = ((c) - (a) - (b)) ^ ((b) >> 15); \
} while (0)

/* ---- benchmark 预置工作模板 (ELF rodata 逐字提取, 每段 320 hex 字符 + 4×NUL,
 *      段距 0x144; A/B 两组各 16 个, 解码 0xa0 字节入 DAT_0007731c/DAT_00077d1c) ---- */
static const char bench_hex_a[16][324] = {
    "000000029c6bf469abe4ad37605c097a860cff3cf5c1ef4377618f74000000000000000082b1514e7b6565941e5824f084292164ec5f97e7ea20c494bd96e524d478977b536dd2261900896c8b10020000000080000000000000000000000000000000000000000000000000000000000000000000000000000000008002000064e4e3becc01064d808269b330f40f4de82dc92e894d635025daa3e2e2c410b4",
    "00000002e790c23987181950eeb144591c3ac4d06c0705f2801d097600000000000000009ebbce2f5f0d6cc0aca284ecb1059c856ef2f7f42e7edd403d246754ee4c905a536dd2a91900896ccfda03100000008000000000000000000000000000000000000000000000000000000000000000000000000000000000800200004a78daf1b5eb3397af1c00dbd9b06659cdc04183c8baaf5be1dbf32f79e00459",
    "000000023e91fce7300a792bfbaa0c76e1aa5f9b546c1db582aee4ff0000000000000000f04650a8e748d2e6fde86a8a920b285f3e22398f583700236958323ef9ea8321536dcf431900896cb4570e200000008000000000000000000000000000000000000000000000000000000000000000000000000000000000800200000a1d654ae2b06f219ccf4601933fab408de1c3b7c8c9c85e03231d4aaf5a26cd",
    "000000023e91fce7300a792bfbaa0c76e1aa5f9b546c1db582aee4ff000000000000000074b39134c2930d2f2e7339f9d502c776c44d6ee599f7efebec6c9bbd04787aae536dce561900896ce2710f3000000080000000000000000000000000000000000000000000000000000000000000000000000000000000008002000094e60c1180022f337232ab3d298f838304b6008ab237cf7e1717f1933407e592",
    "000000023e91fce7300a792bfbaa0c76e1aa5f9b546c1db582aee4ff0000000000000000c5b821fb0b26d63b00cc26e7ac4d6cfd1d3fc109b0db188e7e792e3d18342919536dce501900896ced480540000000800000000000000000000000000000000000000000000000000000000000000000000000000000000080020000a290eac61642949c00d17f7cd5980abedb8647fc5df9955dcfe4d56a50a0c564",
    "00000002e790c23987181950eeb144591c3ac4d06c0705f2801d097600000000000000006e9d94bf5a0ab7b202d39e1200af96074e4f641f4e55e3e9e3aee72aa00a70e9536dd2ae1900896c60f701500000008000000000000000000000000000000000000000000000000000000000000000000000000000000000800200001477ca8536702eacbd65a6a162cfe90d62016a14ffe58d52b7dd4c3628a27e5b",
    "00000002194bb5b4f8ac3392fbd66f3dd3e9dcdb22370e380837fe44000000000000000003bbb250f2dc23717e8192c0b8bec6a175cd059e4089d325006eaee3446254c9536dd39e1900896c16980c6000000080000000000000000000000000000000000000000000000000000000000000000000000000000000008002000068db599d6b7a55fd61d4244a3dfa465055ead6b5c0a37c7a3d4555b58e99065e",
    "000000023e91fce7300a792bfbaa0c76e1aa5f9b546c1db582aee4ff000000000000000072e17babd4089b204797cebda7dc6e277950eab1b2908991ae1d72335f82d204536dcf441900896c5f2d0970000000800000000000000000000000000000000000000000000000000000000000000000000000000000000080020000c7d601ce3b01e569a49508d541bbcba9b3c8394b1834523ef1e5cb2c60bd34a3",
    "000000029c6bf469abe4ad37605c097a860cff3cf5c1ef4377618f74000000000000000022388b6f022144db134af1bc8e61b385ca37cae038c1d165ae98c496b3b41e8b536dd2101900896ca8ea0e80000000800000000000000000000000000000000000000000000000000000000000000000000000000000000080020000410761e97e67b494fd547cfe9ffbb36893da7aec75c6b51b8d5f38f87b5d63cf",
    "000000029c6bf469abe4ad37605c097a860cff3cf5c1ef4377618f7400000000000000000e1f0cfdf5ad8248fc4520f3bb0b2040226430348cddeff5ca9181beeb78870d536dd2161900896c0d600f90000000800000000000000000000000000000000000000000000000000000000000000000000000000000000080020000ad1a8d354a7e8b13ec47f4c3d907d00945a61e86059f4943e42c1e52398eba5d",
    "00000002194bb5b4f8ac3392fbd66f3dd3e9dcdb22370e380837fe4400000000000000002232a16d38cc0e13e4b16d917bff4c34727deb3b5c50e424fb8453ff9b2adcb4536dd4231900896cbf1002a0000000800000000000000000000000000000000000000000000000000000000000000000000000000000000080020000adc67d38f31f589b18b9d8e531b994ce5733c021a03d88d38611ee6b4c2710a5",
    "00000002194bb5b4f8ac3392fbd66f3dd3e9dcdb22370e380837fe440000000000000000a2860471277b4a93fea2a8b6d8c281fab7bde3b78f2acd1bfdc89d464ed3bb3c536dd35e1900896c9d3004b0000000800000000000000000000000000000000000000000000000000000000000000000000000000000000080020000537686c611aae4397c7c04b2c190708453d00e8c9563525610c31ba46e80dbc2",
    "00000002194bb5b4f8ac3392fbd66f3dd3e9dcdb22370e380837fe440000000000000000f370230607998fbbd10275c5890885fcd81b68018ba2373abf0f93a06d02ab28536dd33e1900896c37750bc00000008000000000000000000000000000000000000000000000000000000000000000000000000000000000800200001fdda952da6abd70022a6e5f2b9dc5e1b66011128c3fa249f0b7439f00d5943e",
    "00000002c0a2c91fc41254539a5b2a27be28de2a6187e2af3f129d6300000000000000005e45ffc512d5ca3bc4d2063dd3af1669c296ae126a5a2ef896d1e190cedf67b9536dd46b1900896cd55b00d000000080000000000000000000000000000000000000000000000000000000000000000000000000000000008002000007094d6cbe76538a88612624fc5e655cc405cb8198dcad516b88dbac5bf8b906",
    "00000002194bb5b4f8ac3392fbd66f3dd3e9dcdb22370e380837fe44000000000000000027c548815127c125147af91c356c293f0defbd2771f8dc3b1142b367528656db536dd37c1900896c96770ae00000008000000000000000000000000000000000000000000000000000000000000000000000000000000000800200001bdbb3b1be7216872ea787627b03c389a527451f6dd832d8540874306f9c07c6",
    "00000002194bb5b4f8ac3392fbd66f3dd3e9dcdb22370e380837fe440000000000000000adef758770bb90c5b13769c5b61affb322b24c747573b38ebe2ee81748d0b557536dd4071900896c29f001f00000008000000000000000000000000000000000000000000000000000000000000000000000000000000000800200001ac8eea63285353944e40eec54d2dd6cd0994b447429bb0ed0598d38f42da0e2",
};
static const char bench_hex_b[16][324] = {
    "00000002c01f502cb3e9fdb053230ec12a4954c1021a6b35862b5e29000000000000000084d1b83ae44057025e8c5b5756b44f04df5fffe4a7a30e5c12d12a97a7a4c2ea536dce431900896cb6d60e00000000800000000000000000000000000000000000000000000000000000000000000000000000000000000080020000d08f7e14c50dad77dc238b4db2901a0578e657b1954779ab9cd82a73829edf7f",
    "000000023bf53ef343a50f7599601f849c93ecce63530b0b449a44630000000000000000c1a174254a6593ffba987f68fe26e716e3c129a7f33a9c43ae7ecf90c8cd0d2c536dc4e61900896c8f8100100000008000000000000000000000000000000000000000000000000000000000000000000000000000000000800200006700aeedada2b3877900b58a183c42c40949956bb8b4a8d21481f8936b572922",
    "00000002138cf4b61dff74e3c26b2d80045064e8ab4802521bab2cda000000000000000071eef64a7ef4e47cda16e96673197d36c7235a4aadd23c21a38ce53827d1f8bc536dc4d71900896ce96b00200000008000000000000000000000000000000000000000000000000000000000000000000000000000000000800200000730cf7a6b8a85eb1cc017b109d23c392464f99aa8c020ea107c525b671adde0",
    "000000023bf53ef343a50f7599601f849c93ecce63530b0b449a44630000000000000000fe2c6b926468565e524ab7c2f111035dcde7c60955842111930589eccb410f83536dc66b1900896cf429003000000080000000000000000000000000000000000000000000000000000000000000000000000000000000008002000027dbb374a97f15c59587256662f36904d075d0e61f749618182711288ac617c7",
    "000000023bf53ef343a50f7599601f849c93ecce63530b0b449a4463000000000000000003073385e05c29f0435a6001c8eca9c8d5602890aeff9d4d103d3383cf80dae5536dc57c1900896c821d00400000008000000000000000000000000000000000000000000000000000000000000000000000000000000000800200007da3b97e82c0c3125a58dad8a0d1d0369244731f3b096e972484298d15b843d9",
    "000000029ca55e5f1bc0328c84f358fddadc13cb232599bc2ca9dbe10000000000000000b5b4d19c20a7fc2b174ff673c006edd2247c4b2336571864df93eb7ec0c8c276536dfe041900896ce63c00500000008000000000000000000000000000000000000000000000000000000000000000000000000000000000800200001514bd586511e531e2b6277a6d112b171f9e008d56ef4a971e619acf22e75072",
    "000000023bf53ef343a50f7599601f849c93ecce63530b0b449a446300000000000000003e3030629ff4258056dc9efaf922bd173a65f65ee799b0c765097d3deeddef10536dc4d81900896c14430060000000800000000000000000000000000000000000000000000000000000000000000000000000000000000080020000dcb77a9c36d894d2dbc31437e5c2a1564e927937848ea2eb20b38638afc64b96",
    "000000023bf53ef343a50f7599601f849c93ecce63530b0b449a446300000000000000005513c22bb99e9daa9936b0df5dce64d7737e3706be99e5098d112002492cf81b536dc5691900896cd44100700000008000000000000000000000000000000000000000000000000000000000000000000000000000000000800200000d896267fda5dda0f85308e77f754c8b94b7b88e3cb315475cd9efd16401e3ce",
    "00000002e155f07e652e4d671ca4db51bbde14d2b5ae34ee67ecc74400000000000000004af5cffd7e5a7087f1b484b526c7350c86d8389283509ca878502f792115e8dc536dc6ad1900896c999d0080000000800000000000000000000000000000000000000000000000000000000000000000000000000000000080020000339354568f506ac3cd69bb427b1af83a0473b87c16bf3b562a93d0a2ffc53e54",
    "000000029ca55e5f1bc0328c84f358fddadc13cb232599bc2ca9dbe100000000000000005925a624e5c84f96d2c34dce3b6a736addb891724b48a36320c7494435f9c915536dfe621900896c14fb00900000008000000000000000000000000000000000000000000000000000000000000000000000000000000000800200008362009c07cf48249f481be6b79e67247cab1d20050cf11c276085b90732110c",
    "00000002e155f07e652e4d671ca4db51bbde14d2b5ae34ee67ecc74400000000000000001e69f1d6507f4b7b50980930f7d8089834fbe65f0980b8592d53cdda08e50d24536dc7da1900896c5eeb00a000000080000000000000000000000000000000000000000000000000000000000000000000000000000000008002000021e4f654d26ab8c9164ff311657a9f9c4cdc0e8a09334925f7c02138819d7e61",
    "000000023bf53ef343a50f7599601f849c93ecce63530b0b449a4463000000000000000064923b63f53c72c04ebe6c1c9140b6377132b6e50865814fe562291bd023d348536dc65a1900896cc52e00b000000080000000000000000000000000000000000000000000000000000000000000000000000000000000008002000055db91a25401a89daf9ff7d7954bab722b894ba480fefaf1f0a95aaf5f600567",
    "000000023bf53ef343a50f7599601f849c93ecce63530b0b449a44630000000000000000ee9817160e35d4410601c8dc741c1a810c485f3b40a0859be5f58f0bf6ef1694536dc6321900896c6e1f00c0000000800000000000000000000000000000000000000000000000000000000000000000000000000000000080020000ae215785178ff6350064060ebbb219a71716a10e88528fc4bb1cb5c8fdd0cf60",
    "000000029ca55e5f1bc0328c84f358fddadc13cb232599bc2ca9dbe100000000000000001e514cf738455a54f004ec86edafcfd9fd2022017bb31c245340353911744fb7536dfe1f1900896c265f00d0000000800000000000000000000000000000000000000000000000000000000000000000000000000000000080020000dcafaa86defe850b057ae74f7218a79b0ede086a196f18f0e7c585eb88d1139a",
    "000000023bf53ef343a50f7599601f849c93ecce63530b0b449a446300000000000000005edbd53fcc64850b5334678199d769514818fbcc79861fc77e572bb4753b7fe2536dc5d91900896c938900e00000008000000000000000000000000000000000000000000000000000000000000000000000000000000000800200005e653df5956ece518a78a5d11297431af94ce8ba91d80cfb2aa8c5b3095fa256",
    "000000023bf53ef343a50f7599601f849c93ecce63530b0b449a44630000000000000000596fc4aa5da839ba267c36aa1a5b29d813747b2273dc03aa9e404c4da0238e2b536dc4cc1900896c9e7000f00000008000000000000000000000000000000000000000000000000000000000000000000000000000000000800200000e23806a533bd956787eef52dd8edee456c60d6cecbb6175458ee53fc8c6c813",
};


/* === bitmain_zcash_driver_init === 原 FUN_0000b928 @ 0x0000b928
 * 来源: reversed/cgminer/20_job_mining/FUN_0000b928.c | 分类: 20_job_mining
 * 语义: cgminer 主初始化巨函数 (等价上游 main()): 选项表构建→命令行解析→配置文件
 *       读取→默认值兜底→日志/信号/线程框架搭建→探测(probe)入口→主循环。
 * 形参: argc/argv (机器码按指针直通 strdup/execv)。
 * 返回: 原型 void, 路径上从不返回 (错误路径 __quit/quit, 正常路径驻留主循环)。
 *
 * 分段角色 (源行号; 详见函数体内各段横幅与文件末尾清单):
 *   段 1  94-270    日志默认名/mode, sysconf→让步钩子, argv 备份, 全局锁族+cond,
 *                   getq 创建, packagename, 信号处理, 程序路径, 哨兵容器 calloc
 *   段 2  271-510   workid 哨兵 ("0"×36) lookup2 哈希+插表+扩容, 选项表注册, opt_parse
 *   段 3  511-760   默认配置加载, benchmark 预置, version 文件, --log-file, log-work 路径
 *   段 4  761-1014  工作日志文件族 ("%s.txt"/per-asic/diff), curses 决策, 配置结果三态
 *   段 5  1015-1607 scan_time 兜底, control_thr, 24 驱动探测, enable_device, temp_cutoffs,
 *                   矿池 diff/userpass 初始化, --monitor 子进程
 *   段 6  1608-2028 mining_thr 分配+设备线程启动, 池启动重试环 (60s 睡眠/提示/按键),
 *                   统计清零, watchpool 线程
 *   段 7  2191-2256 watchdog/api/input 线程 + nice(-10) (一次性)
 *   段 8  2030-2489 主循环: work update → staged_work 排水 → make_work → 按策略选池
 *                   → stratum/benchmark/benchfile work 生成 + hash_push → 回环
 */
void bitmain_zcash_driver_init(int argc, char **argv)
{
    /* ---- 共享局部 (语义命名; 跨段使用者在首次出现处注明原 Ghidra 变量) ---- */
    long nproc;                /* 原 lVar11: sysconf(_SC_NPROCESSORS_ONLN) */
    char **argv_copy;          /* 原 pvVar12/DAT_00077290: argv 备份 */
    int i;                     /* 循环计数 */
    int rc;                    /* 原 iVar34 等: pthread_* 返回值复用 */
    char *prog_dup;            /* 原 pcVar14: strdup(argv[0]) */
    char *prog_dir;            /* 原 pcVar15: dirname 结果 */
    size_t len;                /* 原 sVar16: strlen 复用 */
    struct sigaction sig_act;  /* 原 _Stack_11bc+sStack_11b8: handler+mask 连续布局 */
    uint8_t *dummy_work;       /* 原 pbVar35: workid 哨兵容器 (calloc(0x68)) */
    uint8_t *carrier;          /* 原 pbVar42: workid 表载体指针 */
    char msgbuf[0x1000];       /* 原 local_1030..abStack_30: snprintf/applog 消息区 (4KB) */
    char verbuf[256];          /* 原 acStack_1130: version 文件内容缓冲 (memset 0x100) */
    char *msg;                 /* 原 local_11d0: = &msgbuf[0] 别名 (原 &local_1030) */
    uint32_t cutoff;           /* 原 uVar45: --temp-cutoffs 当前值 (段 5) */
    int idx;                   /* 原 iVar34/uVar40: --temp-cutoffs 设备下标 (段 5) */

    /* ================================================================
     * 段 1 (原 94-270 行): 基础环境 — 日志默认值 / argv 备份 / 全局锁族
     *   角色说明: 本段建立整个进程的同步基础设施 (10 把全局锁 + 3 个条件变量)
     *   与日志默认输出 ("cgminer.log", 追加模式)。初始化顺序与上游 cgminer
     *   main() 一致; 任何 cond 初始化失败按错误串记录后 __quit(1) 终止。
     *   ================================================================ */

    /* 日志文件参数: applog 中 fopen(&DAT_00078ce8, &DAT_00078de8) */
    *(uint16_t *)&DAT_00078de8 = 0x2b61;    /* 小端字节 'a','+' → 模式 "a+" (追加) */
    *(uint16_t *)&DAT_00078dea = 0;         /* 补 NUL */
    strcpy(DAT_00078ce8, "cgminer.log");    /* 原逐字节写 'c'..'\0' (DAT_00078ce8..78cf3) */
    DAT_00078ce0 = 0;                       /* 日志文件写开关: applog 中非 0 才写 */

    /* 单核系统 → 让步钩子改写为 sched_yield (多核保持 ELF 初值 noop_hook) */
    nproc = sysconf(0x54);                  /* 0x54 = _SC_NPROCESSORS_ONLN */
    if (nproc == 1) {
        DAT_00075ed8 = (void (*)(void))sched_yield;
    }

    /* argv 备份: DAT_00077290[i]=strdup(argv[i]), NULL 终止 (restart_mine execv 用) */
    argv_copy = malloc((argc + 1) * sizeof(char *));
    DAT_00077290 = argv_copy;
    if (0 < argc) {
        for (i = 0; i < argc; i++) {
            argv_copy[i] = strdup(argv[i]);
        }
    }
    argv_copy[argc] = NULL;

    /* 全局锁族初始化 (顺序即机器码顺序; 文件/行号实参 = 错误报告用常量):
     *   7b384  mutex_init  → hash_lock A (10693)
     *   7b85c  mutex_init  → hash_driver_work 锁 (10694)
     *   7b7a8  mutex_init  → curses 锁 (10695)
     *   7ba10  cglock_init → control_lock 组合锁 (10696)
     *   7ad4c  mutex_init  → hash_lock B/设备表锁 (10697)
     *   770b8  mutex_init  → sharelog 锁 (10698)
     *   7b9d8  cglock_init → ch_lock 组合锁 (10699)
     *   77030  mutex_init  → stratum_lock (10700)
     *   77294/7b2d8/7b650/7b700 rwlock_init (10701..10704)
     *   77060  mutex_init  → stratum 附属锁 (10706)                       */
    mutex_init(&DAT_0007b384, (const char *)&DAT_0005db00, 0x29c5);
    mutex_init(&DAT_0007b85c, (const char *)&DAT_0005db00, 0x29c6);
    mutex_init(&DAT_0007b7a8, (const char *)&DAT_0005db00, 0x29c7);
    cglock_init(&DAT_0007ba10, (const char *)&DAT_0005db00, 0x29c8);
    mutex_init(&DAT_0007ad4c, (const char *)&DAT_0005db00, 0x29c9);
    mutex_init(&DAT_000770b8, (const char *)&DAT_0005db00, 0x29ca);
    cglock_init(&DAT_0007b9d8, (const char *)&DAT_0005db00, 0x29cb);
    mutex_init(&DAT_00077030, (const char *)&DAT_0005db00, 0x29cc);
    rwlock_init(&DAT_00077294, (const char *)&DAT_0005db00, 0x29cd);
    rwlock_init(&DAT_0007b2d8, (const char *)&DAT_0005db00, 0x29ce);
    rwlock_init(&DAT_0007b650, (const char *)&DAT_0005db00, 0x29cf);
    rwlock_init(&DAT_0007b700, (const char *)&DAT_0005db00, 0x29d0);
    mutex_init(&DAT_00077060, (const char *)&DAT_0005db00, 0x29d2);

    /* 条件变量链: lp_cond → restart_lock → restart_cond → gws_cond (失败即退) */
    rc = pthread_cond_init(&DAT_00077078, NULL);          /* lp_cond */
    if (rc != 0) {
        applog(3, "Failed to pthread_cond_init lp_cond", 1);      /* 原 LAB_0000e168 */
        __quit(1, 0);
    }
    mutex_init(&DAT_0007b62c, (const char *)&DAT_0005db00, 0x29d6);
    rc = pthread_cond_init(&DAT_0007bae0, NULL);          /* restart_cond */
    if (rc != 0) {
        applog(3, "Failed to pthread_cond_init restart_cond", 1); /* 原 214-256 else 支 */
        __quit(1, 0);
    } else {
        rc = pthread_cond_init(&DAT_0007b1a0, NULL);      /* gws_cond */
        if (rc != 0) {
            applog(3, "Failed to pthread_cond_init gws_cond", 1); /* 原 LAB_0000e1ac→da08 */
            __quit(1, 0);
        }
    }

    /* getq 任务队列创建 (0x58B; DAT_0007704c 别名到其 +0xc mutex) */
    DAT_0007b620 = tq_new();
    if (DAT_0007b620 == NULL) {
        applog(3, "Failed to create getq", 1);                    /* 原 LAB_0000da08 */
        __quit(1, 0);
    }
    DAT_0007704c = (pthread_mutex_t *)((uint8_t *)DAT_0007b620 + 0xc);

    /* packagename = "cgminer 4.9.0" (api/usage 显示用) */
    snprintf(DAT_00076bf4, 0x100, "%s %s", "cgminer", "4.9.0");

    /* 信号处理: SIGTERM/SIGINT → sighandler (旧动作存 DAT_0007b108/7b94c), SIGPIPE → SIG_IGN
     * 注: sa_flags 未显式写 (机器码仅写 sa_handler/sa_mask, 与反编译一致)。 */
    sig_act.sa_handler = sighandler;                      /* FUN_0001795c */
    sigemptyset(&sig_act.sa_mask);
    sigaction(0xf, &sig_act, &DAT_0007b108);              /* 0xf = SIGTERM */
    sigaction(2, &sig_act, &DAT_0007b94c);                /* 2 = SIGINT */
    signal(0xd, SIG_IGN);                                 /* 0xd = SIGPIPE */

    /* 程序路径: "/usr/bin" 常量串入全局; argv[0] 目录名 + '/' 入 4104B 栈缓冲
     * (DAT_0007b8b4 指向栈缓冲 — 本函数永不返回, 生命周期等价全局)。 */
    {
        char usrbin[9];         /* 原 local_21f8 "/usr" + acStackY_21f4 "/bin" + local_21f0 NUL
                                 * 三个连续栈槽拼成 "/usr/bin\0" */
        uint8_t dirbuf[4104];   /* 原 acStackY_3200 (DAT_0007b8b4 目标) */

        usrbin[0] = '/';
        usrbin[1] = 'u';
        usrbin[2] = 's';
        usrbin[3] = 'r';
        usrbin[4] = '/';
        usrbin[5] = 'b';
        usrbin[6] = 'i';
        usrbin[7] = 'n';
        usrbin[8] = '\0';
        DAT_0007bc74 = usrbin;                /* "/usr/bin" 串首 */
        DAT_0007b8b4 = dirbuf;
        prog_dup = strdup(argv[0]);
        prog_dir = dirname(prog_dup);
        strcpy((char *)dirbuf, prog_dir);
        free(prog_dup);
        len = strlen((char *)dirbuf);
        dirbuf[len] = '/';
        dirbuf[len + 1] = '\0';
    }

    /* curses 布局行号初值 (switch_logsize 运行期改写) */
    DAT_00077028 = 8;
    DAT_0007701c = 9;
    DAT_00077024 = 10;

    /* workid 哨兵容器: calloc(0x68), 失败报 "main OOM" 退出 (原 LAB_0000e228) */
    dummy_work = calloc(0x68, 1);
    if (dummy_work == NULL) {
        applog(3, "main OOM", 1);
        quit(1);
    }

    /* ================================================================
     * 段 2 (原 271-510 行; 尾 switch 分支在原 1334-1365 行): workid 哨兵注册
     *   角色说明: 为哨兵 workid ("0"×36) 计算 Jenkins lookup2 型哈希并插入
     *   workid 哈希表 (首个插入充当表载体, 建表流程与 workid_hash_add
     *   FUN_00016624 同构, 表布局见文件头注释); 扩容判定/重链逻辑同源。
     *   随后注册两组选项表并执行命令行解析; 哨兵 id 存入 DAT_0007bb28。
     *   ================================================================ */

    /* id 字符串追加 0x24 个 '0' → 哨兵 workid = "000…0" (36 字符) */
    for (i = 0x24; i != 0; i--) {
        len = strlen((char *)dummy_work);
        dummy_work[len] = '0';
        dummy_work[len + 1] = '\0';
    }
    dummy_work[0x4c] = 0;                          /* node+0x08 (FIFO 后继) 清零 */
    dummy_work[0x4d] = 0;
    dummy_work[0x4e] = 0;
    dummy_work[0x4f] = 0;
    *(uint8_t **)(dummy_work + 0x58) = dummy_work; /* node+0x14: 键指针 = 容器首 (id 串) */

    {
        uint32_t ha, hb, hc;          /* lookup2 a/b/c (原 uVar40/pvVar12/uVar45) */
        size_t remaining;             /* 原 uVar17: 12 字节块外剩余长度 */
        uint8_t *k;                   /* 原 pbVar38: 哈希游标 */
        seg2_workid_ht_t *table;      /* 原 puVar18/piVar27 解引用 */
        uint32_t *buckets_new;        /* 扩容新桶数组 (原 pvVar12 复用) */
        uint32_t nbk;                 /* 原 uVar45 复用: 桶数缓存 */
        uint32_t idx;                 /* 原 uVar17 复用: 重链桶下标 */
        uint32_t bcnt;                /* 原 uVar45 复用: 桶计数 */
        uint8_t *node;                /* 原 pbVar38/local_11c8: 节点 (容器+0x44) */

        len = strlen((char *)dummy_work);
        *(uint32_t *)(dummy_work + 0x5c) = len;    /* node+0x18: 键长 = 36 */
        carrier = DAT_000772b4;
        if (carrier == NULL) {
            /* 首次插入: 本容器为表载体, 创建表 */
            dummy_work[0x48] = 0;                  /* node+0x04 (FIFO 前驱) = NULL */
            dummy_work[0x49] = 0;
            dummy_work[0x4a] = 0;
            dummy_work[0x4b] = 0;
            DAT_000772b4 = dummy_work;
            table = (seg2_workid_ht_t *)malloc(0x2c);
            *(uint8_t **)(dummy_work + 0x44) = (uint8_t *)table;   /* node+0x00: 表指针 */
            if (table == NULL) {
                exit(-1);                          /* 原 LAB_0000da38 (无日志) */
            }
            memset(table, 0, 0x2c);
            table->tail_node = (uint32_t)(dummy_work + 0x44);
            table->nbuckets = 0x20;
            table->shift = 5;
            table->node_off = 0x44;
            buckets_new = (uint32_t *)malloc(0x180);   /* 0x20 桶 × 12B */
            table->buckets = buckets_new;
            if (buckets_new == NULL) {
                exit(-1);                          /* 原 LAB_0000da38 */
            }
            memset(buckets_new, 0, 0x180);
            table->seed = 0xa0111fe1;
        } else {
            /* 已有表: 全局 FIFO 尾插 (前驱容器指针按 node_off 还原) */
            table = (seg2_workid_ht_t *)(void *)*(uint32_t *)(carrier + 0x44);
            *(uint8_t **)((uint8_t *)(uintptr_t)table->tail_node + 8) = dummy_work;
            *(uint32_t *)(dummy_work + 0x48) =
                table->tail_node - table->node_off;
            table->tail_node = (uint32_t)(dummy_work + 0x44);
        }
        carrier = DAT_000772b4;
        table = (seg2_workid_ht_t *)(void *)*(uint32_t *)(carrier + 0x44);  /* 与机器码一致的重装载 */
        table->count += 1;
        *(uint32_t *)(dummy_work + 0x44) = (uint32_t)(uintptr_t)table;      /* node->table */

        /* ---- Jenkins lookup2 型哈希: 键 = id 串 36 字节 ----
         * 初值: a=b=0x9e3779b9, c=0xfeedbeef; 每 12 字节块 a+=w0/b+=w1/c+=w2 后 mix;
         * 块外剩余 r 进入尾 switch (11→1 逐字节并入, 见原 1334-1365); c += 总长;
         * 终混后哈希存 node+0x1c (容器+0x60)。 */
        ha = 0x9e3779b9u;
        hb = 0x9e3779b9u;
        hc = 0xfeedbeefu;
        remaining = strlen((char *)dummy_work);
        k = dummy_work;
        while (0xb < remaining) {
            ha += (uint32_t)k[0] | ((uint32_t)k[1] << 8) |
                  ((uint32_t)k[2] << 16) | ((uint32_t)k[3] << 24);
            hb += (uint32_t)k[4] | ((uint32_t)k[5] << 8) |
                  ((uint32_t)k[6] << 16) | ((uint32_t)k[7] << 24);
            hc += (uint32_t)k[8] | ((uint32_t)k[9] << 8) |
                  ((uint32_t)k[10] << 16) | ((uint32_t)k[11] << 24);
            JHASH_MIX2(ha, hb, hc);
            remaining -= 0xc;
            k += 0xc;
        }
        len = strlen((char *)dummy_work);
        hc += (uint32_t)len;                       /* lookup2: c 的高字节预留长度 */
        switch (remaining) {                       /* 原 switchD_0000be78 */
        case 11:
            hc += (uint32_t)k[10] << 24;           /* fall through */
        case 10:
            hc += (uint32_t)k[9] << 16;            /* fall through */
        case 9:
            hc += (uint32_t)k[8] << 8;             /* fall through (首字节留长度) */
        case 8:
            hb += (uint32_t)k[7] << 24;            /* fall through */
        case 7:
            hb += (uint32_t)k[6] << 16;            /* fall through */
        case 6:
            hb += (uint32_t)k[5] << 8;             /* fall through */
        case 5:
            hb += (uint32_t)k[4];                  /* fall through */
        case 4:
            ha += (uint32_t)k[3] << 24;            /* fall through */
        case 3:
            ha += (uint32_t)k[2] << 16;            /* fall through */
        case 2:
            ha += (uint32_t)k[1] << 8;             /* fall through */
        case 1:
            ha += (uint32_t)k[0];                  /* fall through */
        case 0:
            break;
        }
        JHASH_MIX2(ha, hb, hc);                    /* 原 LAB_0000bf14 起终混 */
        *(uint32_t *)(dummy_work + 0x60) = hc;     /* node+0x1c: 哈希值 */

        /* ---- 桶头插 (node->bkt_prev/bkt_next; 与 workid_hash_add 相同) ---- */
        node = dummy_work + 0x44;
        {
            uint32_t bucket_base = (hc & (table->nbuckets - 1u)) * 0xc;
            uint8_t *bk = (uint8_t *)(uintptr_t)table->buckets + bucket_base;
            uint32_t head;

            *(uint32_t *)(bk + 4) += 1;            /* 桶计数 +1 */
            head = *(uint32_t *)bk;                /* 旧链头 */
            *(uint32_t *)(node + 0x0c) = 0;        /* bkt_prev = NULL (原 379-382) */
            *(uint32_t *)(node + 0x10) = head;     /* bkt_next = 旧链头 */
            if (head != 0) {
                *(uint32_t *)(head + 0xc) = (uint32_t)(uintptr_t)node;
            }
            *(uint32_t *)bk = (uint32_t)(uintptr_t)node;

            /* ---- 扩容判定: (watermark+1)*10 <= 桶计数 且未冻结 ---- */
            if ((uint32_t)((*(uint32_t *)(bk + 8) + 1) * 10) <= *(uint32_t *)(bk + 4) &&
                *(uint32_t *)((uint8_t *)(uintptr_t)table + 0x24) != 1) {
                size_t mem = (size_t)*(uint32_t *)((uint8_t *)(uintptr_t)table + 4) * 0x18;
                uint8_t *load_tbl = (uint8_t *)(uintptr_t)table;
                uint32_t new_shift;

                buckets_new = (uint32_t *)malloc(mem);
                if (buckets_new == NULL) {
                    exit(-1);                      /* 原 LAB_0000da38 */
                }
                memset(buckets_new, 0, mem);
                /* 新平均负载估计 → 扩容除数 (原 397-399) */
                *(uint32_t *)(load_tbl + 0x18) =
                    (uint32_t)(((*(int32_t *)(load_tbl + 4) * 2 - 1u) &
                                *(uint32_t *)(load_tbl + 0xc)) != 0) +
                    (*(uint32_t *)(load_tbl + 0xc) >>
                     ((*(int32_t *)(load_tbl + 8) + 1) & 0xff));
                *(uint32_t *)(load_tbl + 0x1c) = 0;    /* over_cnt 清零 */
                nbk = *(uint32_t *)(load_tbl + 4);
                if (nbk != 0) {
                    uint32_t bi = 0;               /* 原 local_11cc */
                    uint8_t *oldb = (uint8_t *)(uintptr_t)table->buckets;  /* 原 local_11c8 */

                    do {
                        uint8_t *n2 = *(uint8_t **)(oldb + bi);   /* 桶链头 (原 pbVar38) */

                        if (n2 != NULL) {
                            while (1) {
                                uint8_t *next_n = *(uint8_t **)(n2 + 0x10);  /* 旧后继 (原 pbVar42) */

                                idx = (nbk * 2u - 1u) & *(uint32_t *)(n2 + 0x1c);
                                bcnt = *(uint32_t *)((uint8_t *)buckets_new + idx * 0xc + 4) + 1;
                                *(uint32_t *)((uint8_t *)buckets_new + idx * 0xc + 4) = bcnt;
                                if (*(uint32_t *)(load_tbl + 0x18) < bcnt) {
                                    *(uint32_t *)(load_tbl + 0x1c) += 1;     /* over_cnt++ */
                                    /* watermark = count / load_cap (原 __udivsi3) */
                                    *(uint32_t *)((uint8_t *)buckets_new + idx * 0xc + 8) =
                                        bcnt / *(uint32_t *)(load_tbl + 0x18);
                                }
                                *(uint32_t *)(n2 + 0x0c) = 0;
                                *(uint32_t *)(n2 + 0x10) =
                                    *(uint32_t *)((uint8_t *)buckets_new + idx * 0xc);
                                head = *(uint32_t *)((uint8_t *)buckets_new + idx * 0xc);
                                if (head != 0) {
                                    *(uint32_t *)(head + 0xc) = (uint32_t)(uintptr_t)n2;
                                }
                                *(uint32_t *)((uint8_t *)buckets_new + idx * 0xc) =
                                    (uint32_t)(uintptr_t)n2;
                                if (next_n == NULL) {
                                    break;
                                }
                                n2 = next_n;
                            }
                        }
                        bi += 0xc;
                    } while (bi < nbk * 0xc);      /* 原: local_11cc < nbuckets */
                }
                free((void *)(uintptr_t)table->buckets);      /* 旧桶数组 */
                *(uint32_t *)(load_tbl + 4) <<= 1;            /* 桶数倍增 */
                *(uint32_t *)(load_tbl + 8) += 1;             /* shift+1 */
                *(uint32_t *)(load_tbl) = (uint32_t)(uintptr_t)buckets_new;
                new_shift = 0;
                /* 连续失衡计数: over_cnt > count/2 → grow_cnt++, 否则清零; >1 冻结扩容 */
                if (*(uint32_t *)(load_tbl + 0xc) >> 1 < *(uint32_t *)(load_tbl + 0x1c)) {
                    new_shift = *(uint32_t *)(load_tbl + 0x20) + 1;
                }
                *(uint32_t *)(load_tbl + 0x20) = new_shift;
                if (1 < *(uint32_t *)(load_tbl + 0x20)) {
                    *(uint32_t *)(load_tbl + 0x24) = 1;       /* frozen = 1 */
                }
            }
        }

        strcpy(DAT_0007bb28, (char *)dummy_work);  /* 当前 job_id = 哨兵 id */
        DAT_0007b440 = (void *)&DAT_0007b440;      /* 空双向链表: next = 头 */
        DAT_0007b444 = (void *)&DAT_0007b440;      /* prev = 头 */

        /* 选项表注册 + 命令行解析 (ccan/opt; 详见 cfg.c 批次) */
        opt_register_table((void *)&DAT_00075600, "Options for both config file and command line");
        opt_register_table((void *)&DAT_00075e24, "Options for command line only");
        opt_parse(&argc, argv, opt_parse_err);     /* 原 FUN_0005bc84(&argc, argv, &LAB_00011c44) */
        if (argc == 1) {
            /* 解析后应仅剩程序名 → 继续 */
        } else {
            applog(3, "Unexpected extra command line arguments", 1);
            __quit(1, 0);
        }
    }

    /* ================================================================
     * 段 3 (原 511-760 行): 默认配置加载 / benchmark 预置 / 编译信息 /
     *                        日志文件与 log-work 路径
     *   角色说明: 1) 未指定配置文件时生成默认路径 (~/.cgminer/cgminer.conf)
     *   并可读则 JSON 解析; 2) benchmark/benchfile 模式下补 0 号池 ("Benchmark"/
     *   "Benchfile") 并预载 16×2 组 scrypt/Zcash 测试工作模板与目标;
     *   3) 读取 miner version 文件拆出编译时间/类型 (axi.c API 暴露);
     *   4) --log-file 命中则启用文件日志; 5) log-work 路径 + asic 数 (1/32/64)
     *   校验, 失败即退出。
     *   ================================================================ */

    /* 默认配置文件: 未加载且未显式指定时尝试默认路径 */
    if (DAT_00076f20 == 0) {
        DAT_00076f24 = malloc(0x1000);
        default_save_file(DAT_00076f24);          /* FUN_00016150: $HOME/.cgminer/cgminer.conf */
        rc = access(DAT_00076f24, 4);             /* 4 = R_OK */
        if (rc == 0) {
            load_json_file(DAT_00076f24);         /* 可读 → JSON 解析 (返回值弃用) */
        } else {
            free(DAT_00076f24);
            DAT_00076f24 = NULL;
        }
    }

    /* benchmark / benchfile 模式: 补 0 号池 + 预置测试工作 */
    if ((DAT_0007730d != 0) || (DAT_00076ffc != 0)) {
        int *pool;                 /* 原 iVar34 复用 */
        char *bench_url;           /* 原 puVar18 */
        const char *bench_name;    /* 原 pcVar28 */
        uint32_t name_mid, name_tail; /* 原 uVar13/uVar29: 10 字节串拷贝中转 */
        uint32_t url_ptr;          /* 原 uVar13 重载: rpc_url 指针 */
        int bi;

        if (DAT_0007b859 != 0) {                      /* scrypt 模式禁用 benchmark */
            applog(3, "Cannot use benchmark mode with scrypt", 1);
            quit(1);
        }
        pool = add_pool();                            /* FUN_00017a3c (Ghidra 未显示实参) */
        bench_url = malloc(0xff);
        if (DAT_00076ffc == 0) {
            bench_name = "Benchmark";
        } else {
            bench_name = "Benchfile";
        }
        *(char **)(pool + 0xc4) = bench_url;          /* pool+0xc4: rpc_url */
        name_mid = *(const uint32_t *)(bench_name + 4);
        name_tail = *(const uint32_t *)(bench_name + 8);
        *(uint32_t *)bench_url = *(const uint32_t *)bench_name;
        *(uint32_t *)(bench_url + 4) = name_mid;
        *(uint16_t *)(bench_url + 8) = (uint16_t)name_tail;   /* 共 10 字节 = 串+NUL */
        url_ptr = *(uint32_t *)(pool + 0xc4);               /* 原 uVar13 重载 (行 579) */
        *(uint32_t *)(pool + 0xcc) = url_ptr;               /* 别名写入 ×4 (原 580-583) */
        *(uint32_t *)(pool + 0xd0) = url_ptr;
        *(uint32_t *)(pool + 200) = url_ptr;
        *(uint32_t *)(pool + 0x270) = url_ptr;
        strncpy((char *)(pool + 0x30), "?", 7);             /* pool+0x30: 7 字节域 = "?" */
        *(uint8_t *)(pool + 0x37) = 0;
        if (*(int *)(pool + 0x84) != 1) {                   /* pool+0x84: enabled */
            *(int *)(pool + 0x84) = 1;
            DAT_0007b5e4 = DAT_0007b5e4 + 1;                /* total_pools++ */
        }
        *(uint8_t *)(pool + 0x81) = 0;                      /* pool+0x81: 初始化 */
        DAT_00078ca9 = 1;                                   /* benchmark 运行标志 */
        for (bi = 0; bi != 0x10; bi++) {                    /* 16 组测试工作解码 */
            hex2bin(DAT_0007731c + bi * 0xa0, (const uint8_t *)bench_hex_a[bi], 0xa0);
            hex2bin(DAT_00077d1c + bi * 0xa0, (const uint8_t *)bench_hex_b[bi], 0xa0);
        }
        set_target(0, DAT_00078730);                        /* 目标 = 难度 0 (原 d0=0.0) */
    }

    /* ---- 原 LAB_0000c524: miner version 文件 (编译时间/类型) ---- */
    msg = msgbuf;
    if (DAT_00076bf0 != NULL) {
        FILE *ver_fp;              /* 原 pFVar19 */
        char *nl;                  /* 原 pcVar28: '\n' 位置 */
        char *line;                /* 原 pcVar15: 当前行起点 */
        char *dst;                 /* 原 pcVar14: 当前写目标缓冲 */

        ver_fp = fopen(DAT_00076bf0, "rb");
        memset(verbuf, 0, 0x100);
        if (ver_fp == NULL) {
            if (APPLOG_GATE3()) {
                snprintf(msg, 0x1000, "Open miner version file %s error", DAT_00076bf0);
lab_d908:                                                /* 原 LAB_0000d908 */
                applog(3, msg, 0);
                goto lab_c658;                           /* 原 LAB_0000c658 */
            }
lab_c67c:                                                /* 原 LAB_0000c67c */
            if ((DAT_00078e08 == 0) && (DAT_000766c4 < 3)) {
                goto lab_c6d8;                           /* 原 LAB_0000c6d8 */
            }
        } else {
            len = fread(verbuf, 1, 0x100, ver_fp);
            if ((int)len < 1) {
                if (APPLOG_GATE3()) {
                    snprintf(msg, 0x1000, "Read miner version file %s error %d",
                             DAT_00076bf0, (int)len);
                    goto lab_d908;
                }
                goto lab_c67c;
            }
            /* 按 '\n' 分段: 首行 → DAT_00078750 (compile time), 余下 → DAT_00078850 (type) */
            nl = strchr(verbuf, 10);
            line = verbuf;
            dst = DAT_00078750;
            if (nl != NULL) {
                memcpy(DAT_00078750, line, (int)(nl - line));
                dst = DAT_00078850;
                line = nl + 1;
            }
            strcpy(dst, line);
            /* 剥离行尾 '\n' / '\r' (原 642-656: 检查 [strlen-1] 处) */
            len = strlen(DAT_00078750);
            if (DAT_00078750[len - 1] == '\n') {
                DAT_00078750[len - 1] = 0;
            }
            len = strlen(DAT_00078750);
            if (DAT_00078750[len - 1] == '\r') {
                DAT_00078750[len - 1] = 0;
            }
            len = strlen(DAT_00078850);
            if (DAT_00078850[len - 1] == '\n') {
                DAT_00078850[len - 1] = 0;
            }
            len = strlen(DAT_00078850);
            if (DAT_00078850[len - 1] == '\r') {
                DAT_00078850[len - 1] = 0;
            }
lab_c658:                                            /* 原 LAB_0000c658 */
            if (DAT_0007ad48 == 0) {
                goto lab_c67c;
            }
        }
        snprintf(msg, 0x1000, "Miner compile time: %s type: %s",
                 DAT_00078750, DAT_00078850);
        applog(3, msg, 0);
    }
lab_c6d8:                                            /* 原 LAB_0000c6d8 */

    /* opt_logfile: 命中则启用文件日志 (applog 内 fopen 名/模式, 见段 1) */
    if (DAT_00076be0 != NULL) {
        DAT_00078ce0 = 1;
        strcpy(DAT_00078ce8, DAT_00076be0);
        if (DAT_00076bec != NULL) {
            strcpy((char *)&DAT_00078de8, DAT_00076bec);   /* 覆盖默认模式 "a+" */
        }
        if (APPLOG_GATE3()) {
            snprintf(msg, 0x1000, "Log file path: %s Open flag: %s",
                     DAT_00078ce8, (char *)&DAT_00078de8);
            applog(3, msg, 0);
        }
    }

    /* log-work: 工作日志路径 + asic 数校验 (空/非法值即退出) */
    if (DAT_00076be8 == NULL) {
        goto lab_cc0c;                                   /* 原 LAB_0000cc0c (段 4) */
    }
    memset(verbuf, 0, 0x100);                            /* 复用 256B 缓冲 (原 pcVar14) */
    if (DAT_00076be4 == NULL) {
        if (APPLOG_GATE3()) {
            snprintf(msg, 0x1000, "Log work path: %s", DAT_00076be8);
            applog(3, msg, 0);
        }
    } else {
        if (*DAT_00076be4 == '\0') {
            applog(3, "Log work asic num empty", 1);
            quit(1);
        }
        DAT_00078950 = strtol(DAT_00076be4, NULL, 10);
        if ((DAT_00078950 != 0x20 && DAT_00078950 != 1) && (DAT_00078950 != 0x40)) {
            applog(3, "Log work asic num must be 1, 32, 64", 1);
            quit(1);
        }
        if (APPLOG_GATE3()) {
            snprintf(msg, 0x1000, "Log work path: %s Asic num: %s",
                     DAT_00076be8, DAT_00076be4);
            applog(3, msg, 0);
        }
    }

    /* ================================================================
     * 段 4 (原 761-1014 行): 工作日志文件族打开 / curses 决策 / 配置结果处理
     *   角色说明: 打开聚合日志 "%s.txt" 与 per-asic 日志 ("%s%02d_%02d.txt",
     *   asic 数 32/64; asic 数 1 走 "%s%02d.txt" 单文件, 标签在段 6), 以及
     *   diff 日志 "%s_diff_%02d.txt" (65 个); DAT_00078a60.. 为连续 FILE* 槽。
     *   随后: curses 使能决策 → "Started %s" → 配置结果三态 (部分加载/致命
     *   JSON 错误/成功) 处理并释放配置路径 → "/usr/bin" 追加 '/'。
     *   ================================================================ */

    /* 聚合工作日志: "%s.txt" */
    sprintf(verbuf, "%s.txt", DAT_00076be8);
    DAT_00078954 = fopen(verbuf, "a+");
    if (APPLOG_GATE3()) {
        snprintf(msg, 0x1000, "Log work open file %s", verbuf);
        applog(3, msg, 0);
    }

    {
        FILE **slot;               /* 原 ppFVar39: FILE* 槽游标 */

        slot = (FILE **)&DAT_00078bd0;      /* 0x78bd0: 槽区锚点 */
        i = 1;
        if (DAT_00078950 == 1) {
            /* 原 LAB_0000df54: 单 asic 文件 "%s%02d.txt" (iVar34=1);
             * 槽位 (FILE **)&DAT_00078bd0[-0x9e] = 0x78958 */
            FILE *fp;                   /* 原 pFVar19 */

            sprintf(verbuf, "%s%02d.txt", DAT_00076be8, i);
            fp = fopen(verbuf, "a+");
            slot[-0x9e] = fp;
            if (APPLOG_GATE3()) {
                snprintf(msg, 0x1000, "Log work open asic %d file %s", DAT_00078950, verbuf);
                applog(3, msg, 0);
            }
        } else if ((DAT_00078950 == 0x20 || DAT_00078950 == 0x40) && (-1 < DAT_00078950)) {
            slot = (FILE **)&DAT_00078954;  /* per-asic 首槽 = 0x78958 (DAT_00078954+4) */
            for (i = 0; i <= DAT_00078950; i++) {
                FILE *fp;                   /* 原 pFVar19 */

                sprintf(verbuf, "%s%02d_%02d.txt", DAT_00076be8, DAT_00078950, i);
                fp = fopen(verbuf, "a+");
                slot += 1;
                *slot = fp;
                if (APPLOG_GATE3()) {
                    snprintf(msg, 0x1000, "Log work open asic %d file %s", DAT_00078950, verbuf);
                    applog(3, msg, 0);
                }
            }
        }

        /* 原 LAB_0000cb58: diff 日志 ("%s_diff_%02d.txt", 0..64 共 65 个) */
        if (DAT_00078a5c != 0) {
            char *diff_slot;        /* 原 pcVar28: 槽游标 (字节型) */

            diff_slot = (char *)&DAT_00078a5c;
            for (i = 0; i != 0x41; i++) {
                FILE *fp;

                sprintf(verbuf, "%s_diff_%02d.txt", DAT_00076be8, i);
                fp = fopen(verbuf, "a+");
                slot = (FILE **)(diff_slot + 4);
                *slot = fp;
                if (APPLOG_GATE3()) {
                    snprintf(msg, 0x1000, "Log work open diff file %s", verbuf);
                    applog(3, msg, 0);
                }
                diff_slot = (char *)slot;   /* 原 805-806: i++ 由 for 步进承担 */
            }
        }
    }

    /* ---- 原 LAB_0000cc0c: curses 使能决策 (log-work 路径为空时直达此处) ---- */
lab_cc0c:
    if (DAT_0007bb18 == 0) {
        if (DAT_00075dd0 != 0) {
            enable_curses();                            /* FUN_00027ba0 */
        }
    } else {
        DAT_00075dd0 = 0;
    }

    /* "Started cgminer 4.9.0" (prio 4) */
    if (APPLOG_GATE4()) {
        snprintf(msg, 0x1000, "Started %s", DAT_00076bf4);
        applog(4, msg, 0);
    }

    /* 配置文件结果处理 (DAT_00076ff4: -1 部分加载 / 0 失败 / 其他成功) */
    if (DAT_00076f24 != NULL) {
        if (DAT_00078e09 != 0 && APPLOG_GATE7()) {
            snprintf(msg, 0x1000, "Loaded configuration file %s", DAT_00076f24);
            applog(7, msg, 0);
        }
        if (DAT_00076ff4 == -1) {
            if (APPLOG_GATE4()) {
                applog(4, "Error in configuration file, partially load.", 0);
            }
            if (DAT_00075dd0 != 0 && APPLOG_GATE4()) {
                applog(4, "Start cgminer with -T to see what failed to load.", 0);
            }
        } else if (DAT_00076ff4 == 0) {
            if (APPLOG_GATE4()) {
                applog(4, "Fatal JSON error in configuration file.", 0);
                if ((DAT_0007ad48 == 0) && (DAT_00078e08 == 0)) {
                    if (DAT_000766c4 < 4) {
                        goto lab_cdec;                  /* 原 LAB_0000cd9c: 静默释放 */
                    }
                }
            }
            applog(4, "Configuration could not be used.", 0);
        }
lab_cdec:                                            /* 原 LAB_0000cdec */
        free(DAT_00076f24);
        DAT_00076f24 = NULL;
    }

    /* "/usr/bin" 串追加 '/' (原 1008-1011) */
    msg = DAT_0007bc74;
    len = strlen(DAT_0007bc74);
    msg[len] = '/';
    msg[len + 1] = '\0';

    /* Per-device stats 开 → 强制 verbose (原 1012-1014) */
    if (DAT_0007b858 != 0) {
        DAT_00078e08 = 1;
    }

    /* ================================================================
     * 段 5 (原 1015-1548 行): 驱动探测/设备使能/温度截止/矿池凭据
     *   角色说明: 1) scan_time 兜底 (非 scrypt 60s / scrypt 30s); 2) control_thr
     *   分配 (8×0x40); 3) 24 个驱动 fill_drv_defaults + 探测调用; 4) 逐设备
     *   enable_device (登记 devices[]、累计线程数); 无设备走 "All devices
     *   disabled" 分支 (段 6); 5) temp_cutoffs 解析 (缺省 95, 空列表全 0,
     *   单值全设备, >200 报错); 6) 矿池初始化: diff=99999999, userpass
     *   拼接 "user:pass", 无凭据报错退出。
     *   ================================================================ */

    /* scan_time 兜底 (cfg.c: ELF 初值 -1) */
    if (DAT_00075df0 < 0) {
        if (DAT_0007b859 == 0) {
            DAT_00075df0 = 0x3c;               /* 非 scrypt: 60s */
        } else {
            DAT_00075df0 = 0x1e;               /* scrypt: 30s */
        }
    }

    /* control_thr 线程槽分配 */
    DAT_00078b64 = 8;
    DAT_0007b0fc = calloc(8, 0x40);
    if (DAT_0007b0fc == NULL) {
        applog(3, "Failed to calloc control_thr", 1);   /* 原 LAB_0000e128 */
        __quit(1, 0);
    }
    DAT_00078b68 = 0;
    {
        int dev_i;             /* 原 pcVar14: 整型计数被 Ghidra 按指针渲染 */

        dev_i = 0;

        /* 驱动默认函数指针补全 (24 个驱动, 顺序即机器码顺序) */
        fill_drv_defaults(&DAT_0007b728);
        fill_drv_defaults(&DAT_0007acc8);
        fill_drv_defaults(&DAT_0007b7d8);
        fill_drv_defaults(&DAT_0007b448);
        fill_drv_defaults(&DAT_00076ac8);
        fill_drv_defaults(&DAT_0007bca0);
        fill_drv_defaults(&DAT_0007b4d8);
        fill_drv_defaults(&DAT_0007b678);
        fill_drv_defaults(&DAT_0007ba48);
        fill_drv_defaults(&DAT_0007ad68);
        fill_drv_defaults(&DAT_0007bb70);
        fill_drv_defaults(&DAT_0007ae70);
        fill_drv_defaults(&DAT_0007b300);
        fill_drv_defaults(&DAT_0007b258);
        fill_drv_defaults(&DAT_0007af70);
        fill_drv_defaults(&DAT_0007b8c0);
        fill_drv_defaults(&DAT_0007b078);
        fill_drv_defaults(&DAT_0007aff0);
        fill_drv_defaults(&DAT_0007b560);
        fill_drv_defaults(&DAT_0007b3a0);
        fill_drv_defaults(&DAT_0007b1d0);
        fill_drv_defaults(&DAT_0007bbf0);
        fill_drv_defaults(&DAT_0007aef0);
        fill_drv_defaults(&DAT_0007ade8);

        /* 驱动探测 (每个驱动的 drv_detect 槽, 实参恒 0) */
        DRV_PROBE(DAT_0007b734);
        DRV_PROBE(DAT_0007acd4);
        DRV_PROBE(DAT_0007b7e4);
        DRV_PROBE(DAT_0007b454);
        DRV_PROBE(*PTR_FUN_00076ad4);              /* 原 (*(code *)PTR_FUN_00076ad4)(0) */
        DRV_PROBE(DAT_0007bcac);
        DRV_PROBE(DAT_0007b4e4);
        DRV_PROBE(DAT_0007b684);
        DRV_PROBE(DAT_0007ba54);
        DRV_PROBE(DAT_0007ad74);
        DRV_PROBE(DAT_0007bb7c);
        DRV_PROBE(DAT_0007ae7c);
        DRV_PROBE(DAT_0007b30c);
        DRV_PROBE(DAT_0007b264);
        DRV_PROBE(DAT_0007af7c);
        DRV_PROBE(DAT_0007b8cc);
        DRV_PROBE(DAT_0007b084);
        DRV_PROBE(DAT_0007affc);
        DRV_PROBE(DAT_0007b56c);
        DRV_PROBE(DAT_0007b3ac);
        DRV_PROBE(DAT_0007b1dc);
        DRV_PROBE(DAT_0007bbfc);
        DRV_PROBE(DAT_0007aefc);
        DRV_PROBE(DAT_0007adf4);

        /* 逐设备使能: 登记 devices[], 累计线程数 (DAT_0007bb6c 或 DAT_00078724) */
        DAT_0007bb6c = 0;
        if (0 < DAT_0007b720) {
            do {
                int *dev = DAT_0007b628 + dev_i;

                dev_i += 1;
                enable_device(dev);
            } while (dev_i < DAT_0007b720);
        }
    }
    if (DAT_0007b720 == 0) {
        applog(3, "All devices disabled, cannot mine!", 1);   /* 原 LAB_0000df08 */
        __quit(1, 0);
    }
    DAT_0007702c = DAT_0007b720;               /* 设备总数 → curses 布局行偏移/高水位 */

    /* temp_cutoffs: 未指定 → 每设备缺省 0x5f (95) */
    if (DAT_00076f00 == NULL) {
        _rd_lock(&DAT_0007b700, "load_temp_cutoffs", 0x480);
        for (i = 0; i < DAT_0007b720; i++) {   /* 原 iVar41/iVar34/piVar27/piVar20 */
            int **dev = DAT_0007b628 + i;

            if (*(int *)(*dev + 0xb8) == 0) {
                *(uint32_t *)(*dev + 0xb8) = 0x5f;      /* TEMP_CUTOFF 缺省 95 */
            }
        }
        rw_unlock(&DAT_0007b700, "load_temp_cutoffs", 0x486);
    } else {
        /* --temp-cutoffs: 逗号分隔逐设备值 */
        char *tok;                 /* 原 pcVar28 */

        tok = strtok(DAT_00076f00, ",");
        if (tok == NULL) {
            goto lab_de30;                 /* 空列表: 全部设备 cutoff = 0 */
        }
        do {
            if (DAT_0007b720 <= idx) {
                applog(3, "Too many values passed to set temp cutoff", 1);
                quit(1);
            }
            cutoff = (uint32_t)strtol(tok, NULL, 10);
            if (200 < cutoff) {
                applog(3, "Invalid value passed to set temp cutoff", 1);
                quit(1);
            }
            _rd_lock(&DAT_0007b700, "load_temp_cutoffs", 0x479);
            {
                int **dev = DAT_0007b628 + idx;

                *(uint32_t *)(*dev + 0xb8) = cutoff;
            }
            idx += 1;
            rw_unlock(&DAT_0007b700, "load_temp_cutoffs", 0x47b);
            tok = strtok(NULL, ",");
        } while (tok != NULL);
        idx = 1;                           /* 原 1197: uVar40 = 1 (单值情形) */
        if (idx == 1) {
            goto lab_de48;                 /* 仅一个值 → 填充其余设备 */
        }
    }
    /* 顺序落回原 LAB_0000d430 */
    goto lab_d430_after;
lab_de30:                                  /* 原 LAB_0000de30 (空 --temp-cutoffs) */
    cutoff = 0;                            /* 原 uVar45 = uVar40 = 0 */
lab_de48:                                  /* 原 LAB_0000de48: cutoff 填满全部设备 */
    _rd_lock(&DAT_0007b700, "load_temp_cutoffs", 0x48c);
    for (; idx < DAT_0007b720; idx++) {
        int **dev = DAT_0007b628 + idx;

        *(uint32_t *)(*dev + 0xb8) = cutoff;
    }
    rw_unlock(&DAT_0007b700, "load_temp_cutoffs", 0x48f);
lab_d430_after: ;

    /* 原 LAB_0000d430: 每设备 +0x138 初始化为 99999999 */
    if (0 < DAT_0007b720) {
        int **devp = DAT_0007b628;             /* 原 piVar27/piVar32/piVar20 */
        int **dev_end = DAT_0007b628 + DAT_0007b720;

        do {
            int **next = devp + 1;

            *(uint32_t *)(*devp + 0x138) = 99999999u;
            devp = next;
        } while (devp != dev_end);
    }

    /* 非 compact 模式: curses 布局行号 += 设备总数 */
    if (DAT_0007b940 == 0) {
        DAT_0007701c = DAT_0007701c + DAT_0007702c;
        DAT_00077024 = DAT_0007701c + 1;
        check_winsizes();
    }

    /* 无矿池: 提示并尝试交互式输入 */
    if (DAT_0007b198 == 0) {
        if (APPLOG_GATE4()) {
            applog(4, "Need to specify at least one pool server.", 0);
        }
        if ((DAT_00075dd0 == 0) || (rc = input_pool(0), rc == 0)) {
            applog(3, "Pool setup failed", 1);   /* 原 1258-1272 逐字节串 */
            __quit(1, 0);
        }
    }

    /* 矿池初始化循环 (原 LAB_0000d5d0): diff=99999999 + userpass 拼接 */
    {
        uint32_t pool_idx = 0;     /* 原 uVar40 */
        uint8_t *pool;             /* 原 puVar43 */
        char *user;                /* 原 pcVar28 */
        char *pass;                /* 原 pcVar15 */
        char *userpass;            /* 原 pcVar14 */
        size_t uplen;              /* 原 sVar21 */

        if (DAT_0007b198 < 1) {
            goto lab_d76c;                 /* 原 LAB_0000d76c (段 6) */
        }
lab_d5d0:                                  /* 原 LAB_0000d5d0 */
        pool = (uint8_t *)DAT_0007b438[pool_idx];
        *(uint32_t *)(pool + 0x1ac) = 99999999u;
        *(uint32_t *)(pool + 0x1d0) = 99999999u;
        if (*(int *)(pool + 200) == 0) {   /* pool+0xc8: rpc_userpass 为空 */
            if (*(int *)(pool + 0xd0) == 0) {   /* pass 为空 → calloc 空串 */
                pass = calloc(1, 1);            /* 原 LAB_0000f5f4 */
                *(void **)(pool + 0xd0) = pass;
            }
            user = *(char **)(pool + 0xcc);     /* rpc_user */
            if (user != NULL) {
                pass = *(char **)(pool + 0xd0);
                uplen = strlen(user) + strlen(pass) + 2;
                userpass = malloc(uplen);
                *(char **)(pool + 200) = userpass;
                if (userpass == NULL) {
                    applog(3, "Failed to malloc userpass", 1);   /* 原 1295-1318 */
                    __quit(1, 0);
                }
                snprintf(userpass, uplen, "%s:%s", user, pass);  /* 原 code_r0x0000d648 */
            } else {
                /* 无凭据: 报 pool 号与 URL 后退出 */
                snprintf(msg, 0x1000, "No login credentials supplied for pool %u %s",
                         pool_idx, *(char **)(pool + 0xc4));
                applog(3, msg, 1);
                __quit(1, 0);
            }
        }
        /* 原 LAB_0000d5b8: 下一池 */
        pool_idx += 1;
        if (DAT_0007b198 <= (int)pool_idx) {
            goto lab_d76c;
        }
        goto lab_d5d0;
    }
lab_d76c:                                  /* 原 LAB_0000d76c (段 6 续) */
    DAT_0007705c = (void *)DAT_0007b438[0];    /* current_pool = 首个矿池 */
    if (DAT_0007ad48 != 0) {
        openlog("cgminer", 1, 8);              /* LOG_PID, LOG_USER */
    }
    if (DAT_00078b6c != 0) {
        /* --monitor: stderr 重定向到管道, 子进程回读 (原 1555-...) */
        int pipe_fd[2];                        /* 原 local_11d0/local_102c/local_1030 复用 */
        void (*old_sighandler)(int);           /* 原 p_Var22 */
        void (*prev_sighandler)(int);          /* 原 p_Var23 */

        rc = pipe(pipe_fd);
        if (rc < 0) {
            perror("pipe - failed to create pipe for --monitor");
            exit(1);
        }
        fflush(stderr);
        rc = dup2(pipe_fd[1], 2);
        if (rc < 0) {
            perror("dup2 - failed to alias stderr to write end of pipe for --monitor");
            exit(1);
        }
        rc = close(pipe_fd[1]);
        if (rc < 0) {
            perror("close - failed to close write end of pipe for --monitor");
            exit(1);
        }
        old_sighandler = (void (*)(int))signal(0xd, SIG_IGN);
        prev_sighandler = (void (*)(int))signal(0xd, SIG_IGN);
        if (prev_sighandler == (void (*)(int))-1 || old_sighandler == (void (*)(int))-1) {
            perror("signal - failed to edit signal mask for --monitor");
            exit(1);
        }
        DAT_0007728c = fork();
        if (DAT_0007728c < 0) {
            perror("fork - failed to fork child process for --monitor");
            exit(1);
        }
        if (DAT_0007728c == 0) {
            /* 子进程: 管道读端 → stdin, 执行用户命令 (原 1589-1599) */
            rc = dup2(pipe_fd[0], 0);
            if (rc < 0) {
                perror("dup2 - in child, failed to alias read end of pipe to stdin for --monitor");
                exit(1);
            }
            close(pipe_fd[0]);
            execl("/bin/bash", "/bin/bash", "-c", DAT_00078b6c, (char *)NULL);
            perror("execl - in child failed to exec user specified command for --monitor");
            exit(1);
        }
        /* 父进程: 关闭读端 (原 1601-1606) */
        rc = close(pipe_fd[0]);
        if (rc < 0) {
            perror("close - failed to close read end of pipe for --monitor");
            exit(1);
        }
    }

    /* ================================================================
     * 段 6 (原 1608-2028 行): mining_thr 线程框架 / 池线程启动 / watchdog
     *   角色说明: 1) calloc mining_threads 个 thr_info 槽 (0x40B), 逐槽
     *   calloc 并登记 mining_thr[i]; 2) 逐设备: 分配 dev->thr[] 指针数组,
     *   get_thread 取槽, 驱动 thread_prepare 成功后 thr_info_create
     *   (miner_thread), 非单线程设备 cgsem_post 唤醒; 3) 全池置 startable;
     *   4) do-while(true): 无可用池时 60s 睡眠 + 提示/列表/按键等待,
     *   有池后清统计区, cgtime×3, get_datestamp, 建 watchpool 线程
     *   ("watchpool thread create fail"), 成功 break; 5) 建 watchdog 线程
     *   ("watchdog thread create fail"); 6) nice(-10) 提升优先级。
     *   ================================================================ */

    /* mining_thr 数组分配 (失败 "Failed to calloc mining_thr" / [%d]) */
    DAT_0007b5e0 = calloc(DAT_0007bb6c, 4);
    if (DAT_0007b5e0 == NULL) {
        applog(3, "Failed to calloc mining_thr", 1);
        __quit(1, 0);
    } else {
        int mi = 0;                    /* 原 iVar34 */

        while (1) {
            void *thr;                 /* 原 pvVar12 */

            if (DAT_0007bb6c <= mi) {
                break;                 /* 全部槽就绪 → 启动设备线程 */
            }
            thr = calloc(1, 0x40);
            DAT_0007b5e0[mi] = (int *)thr;
            if (DAT_0007b5e0[mi] == NULL) {
                snprintf(msg, 0x1000, "Failed to calloc mining_thr[%d]", mi);
                applog(3, msg, 1);
                __quit(1, 0);
            }
            mi += 1;
        }

        /* 逐设备/逐线程初始化 (原 1642-1700 的 do-while(true) 已结构化) */
        {
            int dev_i = 0;             /* 原 iVar41 */
            int thr_g = 0;             /* 原 iVar24: 全局线程号 */
            int dev;

            for (dev = 0; dev < DAT_0007b720; dev++) {
                uint8_t *cgpu;         /* 原 puVar36 */
                int nthreads;          /* 原 iVar44 */
                int j;                 /* 原 iVar34 */

                cgpu = (uint8_t *)DAT_0007b628[dev];
                nthreads = *(int *)(cgpu + 0x94);              /* cgpu->threads */
                *(void **)(cgpu + 0x98) = malloc((nthreads + 1) * 4);  /* cgpu->thr[] */
                *(uint32_t *)((int *)(*(void **)(cgpu + 0x98)) + 0 + nthreads) = 0;
                *(uint32_t *)(cgpu + 0x60) = 4;
                for (j = 0; j < nthreads; j++) {
                    int *thr;          /* 原 piVar27 */
                    int prep;

                    thr = (int *)get_thread(thr_g);
                    *thr = thr_g;                      /* thr->id */
                    thr[9] = (int)(intptr_t)cgpu;      /* thr+0x24: cgpu */
                    thr[1] = j;                        /* thr->thread_no */
                    /* 驱动 thread_prepare (dev->drv(+4)->fn(+0x2c)); Ghidra 未渲染实参 */
                    prep = (*(int (**)(void))(uintptr_t)(*(uint32_t *)(cgpu + 4) + 0x2c))();
                    if (prep != 0) {
                        rc = thr_info_create(thr, NULL, miner_thread, thr);
                        if (rc != 0) {
                            snprintf(msg, 0x1000, "thread %d create failed", *thr);
                            applog(3, msg, 1);
                            __quit(1, 0);
                        }
                        ((int **)(*(void **)(cgpu + 0x98)))[j] = thr;
                        if (*(int *)(cgpu + 0x20) != 1) {
                            if (APPLOG_GATE7()) {
                                snprintf(msg, 0x1000, "Pushing sem post to thread %d", *thr);
                                applog(7, msg, 0);
                            }
                            _cgsem_post(thr + 4, "cgminer.c", (const char *)&DAT_0005db00, 0x2b5a);
                        }
                    }
                    thr_g += 1;
                }
            }

            /* 设备线程全部就绪后: 测试模式 → 统计重置 (dc70);
             * 正常模式 → 全池置 startable 后跳 f09c 启动池线程 (原 1647-1660) */
            if (DAT_0007730d != 0) {
                goto lab_dc70;                 /* benchmark */
            }
            if (DAT_00076ffc != 0) {
                goto lab_dc70;                 /* benchfile */
            }
            for (dev = 0; dev < DAT_0007b198; dev++) {
                uint8_t *pl = (uint8_t *)DAT_0007b438[dev];

                *(uint8_t *)(pl + 0x81) = 1;
                if (*(int *)(pl + 0x84) != 1) {
                    *(int *)(pl + 0x84) = 1;
                    DAT_0007b5e4 += 1;
                }
            }
            goto lab_f09c;                     /* 正常模式 → 池线程 (跳入下方 do-while) */
        }
    }

    /* ================================================================
     * 段 6b (原 1713-1955): 池启动重试环 (do-while(true))
     *   正常模式经 goto lab_f09c 直达池线程创建后回到环首; 测试模式经
     *   goto lab_dc70 直接统计重置+watchpool; 环内 60s 睡眠 + 无池提示。
     *   ================================================================ */
    do {
        int wi = 0;                /* 原 iVar34: 首轮提示标志 (提示后置 1) */

        /* 60s 睡眠 (可被 DAT_0007730c 打断) */
        {
            int s;

            for (s = 0x3c; s != 0; s--) {
                sleep(1);
                if (DAT_0007730c != 0) {
                    break;
                }
            }
        }

        /* 无可用池等待环: 提示 + 池列表 + (curses) 按键退出检查 */
        while (DAT_0007730c == 0) {
            if (wi == 0) {
                if (APPLOG_GATE3()) {
                    applog(3, "No servers were found that could be used to get work from.", 0);
                    applog(3, "Please check the details from the list below of the servers you have input", 0);
                    applog(3, "Most likely you have input the wrong URL, forgotten to add a port, or have not set up workers", 0);
                }
                /* 池列表 (GATE4); 注意: 复用 wi 的原变量 iVar34 亦被本循环改写 */
                for (i = 0; i < DAT_0007b198; i++) {
                    uint8_t *pl = (uint8_t *)DAT_0007b438[i];

                    if (APPLOG_GATE4()) {
                        snprintf(msg, 0x1000, "Pool: %d  URL: %s  User: %s  Password: %s",
                                 i, *(char **)(pl + 0xc4), *(char **)(pl + 0xcc),
                                 *(char **)(pl + 0xd0));
                        applog(4, msg, 0);
                    }
                }
                if (DAT_00075dd0 != 0) {
                    if (APPLOG_GATE3()) {
                        applog(3, "Press any key to exit, or cgminer will wait indefinitely for an alive pool.", 0);
                        goto lab_f460;
                    }
                    wi = 1;
                    goto lab_f474;
                }
lab_f4cc:                              /* 原 LAB_0000f4cc: 无 curses → 报错退出 */
                applog(3, "No servers could be used! Exiting.", 1);
                __quit(0, 0);
lab_f460:                              /* 原 LAB_0000f460 */
                if (DAT_00075dd0 == 0) {
                    goto lab_f4cc;
                }
lab_f474:                              /* 原 LAB_0000f474: curses 按键检查 */
                {
                    int lines;         /* 原 iVar41 */

                    if (DAT_0007b430 == NULL) {
                        lines = -1;
                    } else {
                        lines = *(short *)((uint8_t *)DAT_0007b430 + 4) + 1;  /* _maxy+1 */
                    }
                    wtouchln(DAT_0007b430, 0, lines, 1);
                    wrefresh(DAT_0007b430);
                    halfdelay(10);
                    if (wgetch(stdscr) != -1) {
                        goto lab_f4cc;
                    }
                }
                wi = 1;                /* 原 1803/1810: iVar34 = 1 (后续轮不再打印) */
            }
            cbreak();
        }

lab_dc70:                              /* 原 LAB_0000dc70: 测试模式与等待环共同汇入 */
        /* 统计清零 (u64 统计 + 0x7870e0..0x787140 区 + 设备计数) */
        *(uint64_t *)&DAT_0007acc0 = 0;    /* 原 _DAT_0007acc0 */
        DAT_0007acc4 = 0;
        {
            uint8_t *z = DAT_000770e0;     /* 原 puVar18 */

            do {
                z[0] = 0;
                z[1] = 0;
                z += 2;
            } while (z != DAT_000770e0 + 0x60);
        }
        for (i = 0; i < DAT_0007b720; i++) {
            uint8_t *cgpu = (uint8_t *)DAT_0007b628[i];

            *(uint32_t *)(cgpu + 0x50) = 0;
            *(uint32_t *)(cgpu + 0x54) = 0;
            *(uint32_t *)(cgpu + 0x30) = 0;
            *(uint32_t *)(cgpu + 0x34) = 0;
        }
        cgtime(&DAT_000770d0);
        cgtime(&DAT_00077148);
        cgtime(&DAT_000770d8);
        get_datestamp(DAT_00077260, 0x28, &DAT_000770d0);

        /* watchpool 线程 (control_thr 槽 2; 失败 "watchpool thread create fail",
         * 成功 break 跳出重试环) */
        DAT_000770b0 = 2;
        rc = thr_info_create((uint8_t *)DAT_0007b0fc + 0x80, NULL, watchpool_thread, NULL);
        if (rc == 0) {
            break;
        }
        applog(3, "watchpool thread create fail", 1);
        __quit(1, 0);

lab_f09c:                              /* 原 LAB_0000f09c */
        if (APPLOG_GATE7()) {
            applog(7, "Probing for an alive pool", 0);
        }
        /* 每池启动线程 (pool->pth = +0x138, 池指针为实参; 入口 @0x1fe0c 命名待补) */
        for (i = 0; i < DAT_0007b198; i++) {
            uint8_t *pl = (uint8_t *)DAT_0007b438[i];

            *(uint8_t *)(pl + 0x13c) = 1;
            pthread_create((pthread_t *)(pl + 0x138), NULL, fun_0001fe0c, pl);
        }
    } while (1);

    /* watchdog 线程 (control_thr 槽 3; 失败 "watchdog thread create fail")
     * 注: 原 Ghidra 将 nice/主循环误嵌在本 if 内 (机器码 if 体仅为失败退出)。 */
    pthread_detach(*(pthread_t *)((uint8_t *)DAT_0007b0fc + 0x8c));   /* detach watchpool pth */
    DAT_000770b4 = 3;
    rc = thr_info_create((uint8_t *)DAT_0007b0fc + 0xc0, NULL, watchdog_thread, NULL);
    if (rc != 0) {
        applog(3, "watchdog thread create fail", 1);
        __quit(1, 0);
    }

    /* ================================================================
     * 段 7 (原 2191-2256 行): API/输入线程 (一次性)
     *   角色说明: detach watchdog → api 线程 (槽 5, api_thread, 失败
     *   "API thread create failed") → 输入线程 (槽 7, input_thread, 失败
     *   "input thread create failed") → control_thr 槽数须为 8 ("incorrect
     *   total_control_threads (%d) should be 8"), 全部通过 → nice(-10) → 主循环。
     *   ================================================================ */
    pthread_detach(*(pthread_t *)((uint8_t *)DAT_0007b0fc + 0xcc));   /* detach watchdog pth */
    DAT_00076ef8 = 5;                          /* api 线程槽号 */
    rc = thr_info_create((uint8_t *)DAT_0007b0fc + 0x140, NULL, api_thread,
                         (uint8_t *)DAT_0007b0fc + 0x140);
    if (rc == 0) {
        DAT_000770b4 = 7;                      /* input 线程槽号 (原 spill 2197) */
        rc = thr_info_create((uint8_t *)DAT_0007b0fc + 0x1c0, NULL, input_thread,
                             (uint8_t *)DAT_0007b0fc + 0x1c0);
        if (rc == 0) {
            pthread_detach(*(pthread_t *)((uint8_t *)DAT_0007b0fc + 0x1cc));
            if (DAT_00078b64 == 8) {
                goto lab_e2a0;                 /* 槽数正确 → nice + 主循环 */
            }
            snprintf(msg, 0x1000, "incorrect total_control_threads (%d) should be 8",
                     DAT_00078b64);
            goto lab_de1c;
        }
        applog(3, "input thread create failed", 1);
        __quit(1, 0);
    }
    applog(3, "API thread create failed", 1);  /* 原 2232-2253 */
lab_de1c:                                      /* 原 LAB_0000de1c */
    __quit(1, rc);                             /* applog 已在上面完成 */

    /* 原 LAB_0000e2a0: nice(-10) 提升优先级 (返回 0 且 debug 时提示, 原样保留) */
lab_e2a0:
    rc = nice(-10);
    if ((rc == 0) && (DAT_00078e09 != 0) &&
        (DAT_0007ad48 != 0 || DAT_00078e08 != 0 || 6 < DAT_000766c4)) {
        applog(7, "Unable to set thread to high priority", 0);
    }

    /* ================================================================
     * 段 8 (原 2030-2489 行): 主循环 (原 LAB_0000e368, 每轮末 goto 回 2030)
     *   角色说明: 1) work update 标志处理 (b8ac → 置各线程 +0x3f=1);
     *   2) staged_work 超阈值时逐个 pop 并 discard (gws_cond 等待);
     *   3) make_work 产生新 work; 4) 按策略选池 (4=balance → select_pool;
     *   3=load-balance → 配额轮换); 5) "Selecting pool %d for work";
     *   6) stratum 池 → gen_stratum_work + hash_push; benchmark → 预置
     *   目标/模板 work + hash_push; benchfile → benchfile_get_work 流程;
     *   7) 池列表/轮换尾部标签 (e654/e668/e69c/e7e0/e7e8) 回到环首。
     *   ================================================================ */
lab_e368:                                      /* 原 LAB_0000e368: 主循环环首 */
    while (1) {
        int last_chk;              /* 原 iVar34: DAT_00075dd8 读取值 */
        int staged_cnt;            /* 原 iVar41: staged_work 计数 */
        void *dwork;               /* 原 local_11cc: 被丢弃的 work */
        void *work;                /* 原 iVar41: make_work 结果 */
        uint8_t *pool;             /* 原 pbVar35: 当前/选中池 */
        int total;                 /* 原 iVar34: total_pools 快照 */
        int rot_i;                 /* 原 iVar24/iVar44: 轮换循环下标 */
        int scan_i;                /* 原 iVar24: 配额扫描下标 */
        int uses;                  /* 原 iVar33: 池 uses 计数 */
        int **pp;                  /* 原 piVar27: 池表游标 */
        int *pool_cand;            /* 原 piVar27: 候选池 */
        uint8_t *selected;         /* 原 local_11c8: 轮换选中池 */
        char has_stratum;          /* 原 cVar9: 池 stratum 标志 */

        last_chk = DAT_00075dd8;
        /* work update 标志: 每个 mining_thr 项 +0x3f 置 1 (原 2033-2083) */
        if (DAT_0007b8ac != 0) {
            if (APPLOG_GATE7()) {
                applog(7, "Work update message received", 0);
            }
            cgtime(&DAT_00076f0c);
            rc = pthread_rwlock_rdlock((pthread_rwlock_t *)&DAT_0007b650);
            if (rc != 0) {
                rd_lock_error("signal_work_update", 0x14c2);       /* FUN_0000b69c */
            }
            if (0 < DAT_0007bb6c) {
                for (i = 0; i < DAT_0007bb6c; i++) {
                    *(uint8_t *)((uint8_t *)(uintptr_t)DAT_0007b5e0[i] + 0x3f) = 1;
                }
            }
            rc = pthread_rwlock_unlock((pthread_rwlock_t *)&DAT_0007b650);
            if (rc != 0) {
                rw_unlock_error("signal_work_update", 0x14c5);     /* FUN_0000b630 */
            }
            DAT_00075ed8();                        /* 让步钩子 */
        }
        DAT_0007b8ac = 0;                          /* 清 work update 标志 */
        current_pool();
        rc = pthread_mutex_lock(DAT_0007704c);     /* stgd_lock (getq->mutex 别名) */
        if (rc != 0) {
            mutex_lock_fail_cgminer((const char *)&DAT_0005db00, 0x2be3);
        }
        /* 排水环 (原 while(true) 2031-2122): staged_work 超阈值则逐个 pop 丢弃 */
        while (1) {
            staged_cnt = (int)(intptr_t)DAT_00077050;
            if (DAT_00077050 != 0) {
                staged_cnt = *(int *)(*(int *)((uint8_t *)DAT_00077050 + 0x728) + 0xc);
            }
            if (last_chk < staged_cnt) {
                if ((DAT_00077056 != 0) && (DAT_00075dd8 < DAT_00075ddc)) {
                    DAT_00075dd8 += 1;
                    DAT_00077056 = 0;
                }
                DAT_00077055 = 1;
                pthread_cond_wait((pthread_cond_t *)&DAT_0007b1a0, DAT_0007704c);
                staged_cnt = (int)(intptr_t)DAT_00077050;
                if (DAT_00077050 != 0) {
                    staged_cnt = *(int *)(*(int *)((uint8_t *)DAT_00077050 + 0x728) + 0xc);
                }
            }
            rc = pthread_mutex_unlock(DAT_0007704c);
            if (rc != 0) {
                mutex_unlock_fail_cgminer((const char *)&DAT_0005db00, 0x2bf5);
            }
            DAT_00075ed8();
            if (staged_cnt <= last_chk) {
                break;                             /* 计数回落 → 生成新 work */
            }
            if ((DAT_00077056 != 0) && (DAT_00075dd8 < DAT_00075ddc)) {
                DAT_00075dd8 += 1;
                DAT_00077056 = 0;
            }
            DAT_00077055 = 1;
            dwork = (void *)(intptr_t)hash_pop(0); /* 原 FUN_000193b4(0) */
            if (dwork != NULL) {
                discard_work(dwork);               /* 原 FUN_000159b4 (实参 Ghidra 未渲染) */
                dwork = NULL;
            }
        }
        if (dwork != NULL) {
            discard_work(dwork);                   /* 原 2123-2125 */
        }
        work = make_work();                        /* FUN_0001a3d4 */
        pool = (uint8_t *)current_pool();          /* FUN_0001da58 */
        total = DAT_0007b198;
        if (DAT_00076efc == 4) {
            pool = (uint8_t *)select_pool(pool);   /* balance (返回值 = 选中池) */
        } else if (DAT_00076efc == 3) {
            /* load-balance 配额轮换 (原 2132-2188, 标签 ec10/ebec/ebd8/ec2c/ece0) */
            if (total < 1) {
lab_ec10:                                          /* 原 LAB_0000ec10: 轮换下标推进 */
                rot_i = DAT_00078b74;
                DAT_00078b74 = rot_i + 1;
                if (total <= rot_i + 1) {
                    DAT_00078b74 = 0;
                }
            } else if (*(int *)(*(uint8_t **)DAT_0007b438 + 0x3c) <=
                       *(int *)(*(uint8_t **)DAT_0007b438 + 0x40)) {
                /* 池 0 配额已用尽: 扫描 pools[1..total-1] (原 ebec/ebd8) */
                pp = DAT_0007b438;
                scan_i = 0;
                while (1) {
                    scan_i += 1;                   /* 原 LAB_0000ebec */
                    if (scan_i == total) {
                        break;                     /* 全部用尽 → 重置计数 */
                    }
                    pp += 1;                       /* 原 LAB_0000ebd8 */
                    pool_cand = (int *)*pp;
                    if (*(int *)((uint8_t *)pool_cand + 0x40) <
                        *(int *)((uint8_t *)pool_cand + 0x3c)) {
                        goto lab_ec2c;             /* 找到有余量的池 */
                    }
                }
                {
                    int **pp2 = DAT_0007b438;
                    int **end2 = DAT_0007b438 + total;

                    do {
                        int **next = pp2 + 1;

                        *(uint32_t *)(*(uint8_t **)*pp2 + 0x40) = 0;
                        pp2 = next;
                    } while (pp2 != end2);
                }
                goto lab_ec10;
            }
lab_ec2c:                                          /* 原 LAB_0000ec2c: 轮换主循环 */
            for (rot_i = 0; rot_i < total; rot_i++) {
                uses = DAT_00078b74;
                pool_cand = (int *)DAT_0007b438[uses];
                i = *(int *)((uint8_t *)pool_cand + 0x40);
                *(int *)((uint8_t *)pool_cand + 0x40) = i + 1;
                if (i < *(int *)((uint8_t *)pool_cand + 0x3c)) {
                    if (((*(uint8_t *)((uint8_t *)pool_cand + 0x81) == 0) &&
                         (*(int *)((uint8_t *)pool_cand + 0x84) == 1)) &&
                        ((*(uint8_t *)((uint8_t *)pool_cand + 0x298) == 0) ||
                         (*(uint8_t *)((uint8_t *)pool_cand + 0x299) != 0))) {
                        goto lab_ece0;             /* 选中该池 */
                    }
                    if (DAT_0007b625 != 0) {
                        int *p0 = priority_pool(0);

                        total = DAT_0007b198;
                        uses = DAT_00078b74;
                        *(int *)(p0 + 0x40) -= 1;
                    }
                }
                DAT_00078b74 = uses + 1;
                if (total <= uses + 1) {
                    DAT_00078b74 = 0;
                }
            }
            /* 兜底: 按优先级逐池找可用 (原 2180-2187) */
            if (0 < total) {
                for (i = 0; i < DAT_0007b198; i++) {
                    pool_cand = priority_pool(i);
                    if ((*(uint8_t *)((uint8_t *)pool_cand + 0x81) == 0) &&
                        (*(int *)((uint8_t *)pool_cand + 0x84) == 1)) {
                        goto lab_ece0;
                    }
                }
            }
        }
        goto lab_e53c;
lab_ece0:                                          /* 原 LAB_0000ece0: 轮换选中池 */
        pool = (uint8_t *)pool_cand;
lab_e53c:                                          /* 原 LAB_0000e53c */
        if (APPLOG_GATE7()) {
            snprintf(msg, 0x1000, "Selecting pool %d for work", *(int *)pool);
            applog(7, msg, 0);
        }
        has_stratum = (char)*(int *)(pool + 0x298);
lab_joined_e5c8:                                   /* 原 joined_r0x0000e5c8 */
        if (has_stratum != 0) {
            /* stratum 池: 未就绪则重选 (原 LAB_0000e5cc, 2284-2348) */
lab_e5cc:                                          /* 原 LAB_0000e5cc */
            if ((*(uint8_t *)(pool + 0x299) == 0) || (*(uint8_t *)(pool + 0x29b) == 0)) {
                pool = (uint8_t *)current_pool();
                total = DAT_0007b198;
                if (DAT_00076efc == 4) {
                    pool = (uint8_t *)select_pool(pool);
                } else if ((DAT_00076efc == 3) || (DAT_0007b625 == 0)) {
                    if (total < 1) {
lab_e688:                                          /* 原 LAB_0000e688 */
                        rot_i = DAT_00078b74;
                        DAT_00078b74 = rot_i + 1;
                        if (total <= rot_i + 1) {
                            DAT_00078b74 = 0;
                        }
                    } else if (*(int *)(*(uint8_t **)DAT_0007b438 + 0x3c) <=
                               *(int *)(*(uint8_t **)DAT_0007b438 + 0x40)) {
                        pp = DAT_0007b438;
                        scan_i = 0;
                        while (1) {
lab_e668_scan:                                 /* 原 LAB_0000e668 (扫描环顶) */
                            scan_i += 1;
                            if (scan_i == total) {
                                break;
                            }
                            pp += 1;           /* 原 LAB_0000e654 检查体已内联 */
                            pool_cand = (int *)*pp;
                            if (*(int *)((uint8_t *)pool_cand + 0x40) <
                                *(int *)((uint8_t *)pool_cand + 0x3c)) {
                                goto lab_e69c;     /* 原 2473: 回轮换环 */
                            }
                            goto lab_e668_scan;
                        }
                        {
                            int **pp2 = DAT_0007b438;
                            int **end2 = DAT_0007b438 + total;

                            do {
                                int **next = pp2 + 1;

                                *(uint32_t *)(*(uint8_t **)*pp2 + 0x40) = 0;
                                pp2 = next;
                            } while (pp2 != end2);
                        }
                        goto lab_e688;
                    }
lab_e69c:                                          /* 原 LAB_0000e69c: 轮换环 */
                    for (rot_i = 0; rot_i < total; rot_i++) {
                        uses = DAT_00078b74;
                        pool_cand = (int *)DAT_0007b438[uses];
                        i = *(int *)((uint8_t *)pool_cand + 0x40);
                        *(int *)((uint8_t *)pool_cand + 0x40) = i + 1;
                        if (i < *(int *)((uint8_t *)pool_cand + 0x3c)) {
                            if (((*(uint8_t *)((uint8_t *)pool_cand + 0x81) == 0) &&
                                 (*(int *)((uint8_t *)pool_cand + 0x84) == 1)) &&
                                ((*(uint8_t *)((uint8_t *)pool_cand + 0x298) == 0) ||
                                 (*(uint8_t *)((uint8_t *)pool_cand + 0x299) != 0))) {
                                goto lab_e7e0;
                            }
                            if (DAT_0007b625 != 0) {
                                int *p0 = priority_pool(0);

                                total = DAT_0007b198;
                                uses = DAT_00078b74;
                                *(int *)(p0 + 0x40) -= 1;
                            }
                        }
                        DAT_00078b74 = uses + 1;
                        if (total <= uses + 1) {
                            DAT_00078b74 = 0;
                        }
                    }
                    if (0 < total) {
                        for (i = 0; i < DAT_0007b198; i++) {
                            pool_cand = priority_pool(i);
                            if ((*(uint8_t *)((uint8_t *)pool_cand + 0x81) == 0) &&
                                (*(int *)((uint8_t *)pool_cand + 0x84) == 1)) {
                                goto lab_e7e0;
                            }
                        }
                    }
                }
                goto lab_e7e8;
            }
            gen_stratum_work(pool, work);          /* FUN_0001acc4 (pool, work) */
            if (APPLOG_GATE7()) {
                applog(7, "Generated stratum work", 0);
            }
            dwork = NULL;
            hash_push(work);                       /* FUN_0001f17c */
            goto lab_e368;                         /* 主循环回环 (原 2376) */
        }
        /* 非 stratum 池: benchmark / benchfile 路径 (原 2378-2470) */
        dwork = work;
        if (DAT_00076ffc == 0) {
            if (DAT_0007730d != 0) {
                *(uint64_t *)((uint8_t *)work + 0x748) = 0x4040000000000000ull;  /* = 32.0 */
                /* 预置目标 (DAT_00078730..7874f, 2×16B → work+0xa0/0xb0) */
                memcpy((uint8_t *)work + 0xa0, (const uint8_t *)DAT_00078730, 16);
                memcpy((uint8_t *)work + 0xb0, (const uint8_t *)DAT_00078730 + 16, 16);
                *(uint32_t *)((uint8_t *)work + 0x6c4) = 0;
                *(uint8_t *)((uint8_t *)work + 0x6ea) = 1;
                *(int *)((uint8_t *)work + 0x6d4) = DAT_0007b438[0];
                cgtime((struct timeval *)((uint8_t *)work + 0x760));
                copy_timeval((struct timeval *)((uint8_t *)work + 0x768),
                             (struct timeval *)((uint8_t *)work + 0x760));
                *(uint8_t *)((uint8_t *)work + 0x788) = 0x42;   /* 'B' */
                if (APPLOG_GATE7()) {
                    applog(7, "Generated benchmark work", 0);
                }
                dwork = NULL;
                hash_push(work);
            }
        } else {
            benchfile_get_work();                  /* FUN_00017254 */
            {
                int *pl0 = DAT_0007b438;           /* 原 piVar27 */

                *(uint8_t *)((uint8_t *)work + 0x6ea) = 1;
                *(int *)((uint8_t *)work + 0x6d4) = *pl0;
                cgtime((struct timeval *)((uint8_t *)work + 0x760));
                copy_timeval((struct timeval *)((uint8_t *)work + 0x768),
                             (struct timeval *)((uint8_t *)work + 0x760));
                *(uint8_t *)((uint8_t *)work + 0x788) = 0x42;
                calc_diff(0, work);                /* FUN_00011a5c(0, work) */
                if (APPLOG_GATE7()) {
                    applog(7, "Generated benchfile work", 0);
                }
                dwork = 0;
                hash_push(work);
            }
        }
        goto lab_e368;                             /* 原 2470: 主循环回环 */

        /* ---- 轮换尾部标签块 (原 2471-2489; 仅经 goto 进入) ---- */
lab_e7e0:                                          /* 原 LAB_0000e7e0 */
        selected = (uint8_t *)pool_cand;
lab_e7e8:                                          /* 原 LAB_0000e7e8 */
        if (APPLOG_GATE7()) {
            snprintf(msg, 0x1000, "Selecting pool %d for work", *(int *)selected);
            applog(7, msg, 0);
        }
        cgsleep_ms(5000);                          /* FUN_0002a92c */
        if (pool != selected) {
            goto lab_e880;
        }
        goto lab_e5cc;
lab_e880:                                          /* 原 code_r0x0000e880 */
        has_stratum = (char)*(int *)(selected + 0x298);
        pool = selected;
        goto lab_joined_e5c8;
    }
}

/* ==================== 覆盖率表 (1/1) ====================
 * 原 FUN 地址    新函数名                  反编译行数   转写段
 * ------------------------------------------------------------
 * FUN_0000b928  bitmain_zcash_driver_init  2491        1-8 (全函数单文件)
 * ------------------------------------------------------------
 * 注: 命名事实源 rewritten/name_map/cgminer.tsv (0000b928 = bitmain_zcash_driver_init;
 *     bitmain_ZCASH_init 权威名属 FUN_00049cc8, 见 name_map 行 16/478)。
 *
 * ==================== 分段角色清单 (反编译源行号) ====================
 * 段 1  94-270    基础环境: 日志默认名 "cgminer.log"/mode "a+", sysconf(单核→让步
 *                 钩子 sched_yield), argv 备份 (restart_mine execv 用), 10 把全局
 *                 锁 + lp_lock/restart_lock + lp/restart/gws cond, getq (tq_new),
 *                 packagename "cgminer 4.9.0", SIGTERM/SIGINT→sighandler
 *                 (旧动作存 DAT_0007b108/7b94c), SIGPIPE→SIG_IGN, "/usr/bin" 与
 *                 argv[0] 目录名, curses 布局行号 8/9/10, 哨兵容器 calloc(0x68)。
 * 段 2  271-510   workid 哨兵注册: id 追加 36 个 '0' → Jenkins lookup2 型哈希
 *                 (初值 a=b=0x9e3779b9/c=0xfeedbeef, 12B/块 mix, 尾 switch, c+=长度)
 *                 → 建表 (0x20 桶/shift 5/node_off 0x44/seed 0xa0111fe1, 与
 *                 workid_hash_add 同构) → 桶头插 + 扩容重链; 随后注册两组选项表
 *                 ("Options for both config file and command line"/"...command line
 *                 only") 并 opt_parse (错误回调 @0x11c44 命名待补); 多余参数报
 *                 "Unexpected extra command line arguments" 退出。
 * 段 3  511-760   默认配置 (~/.cgminer/cgminer.conf → load_json_file), benchmark/
 *                 benchfile 预置 (0 号池 "Benchmark"/"Benchfile" + 16×2 组测试工作
 *                 hex2bin + set_target(0)), miner version 文件 (编译时间/类型, axi.c
 *                 API "CompileTime"/"Type"), --log-file 启用文件日志, log-work 路径
 *                 + asic 数校验 (1/32/64)。
 * 段 4  761-1014  工作日志文件族: 聚合 "%s.txt" (DAT_00078954), per-asic
 *                 "%s%02d_%02d.txt" (32/64, 首槽 0x78958) 或单 asic "%s%02d.txt"
 *                 (原 LAB_0000df54), diff 日志 "%s_diff_%02d.txt"×65 (DAT_00078a5c
 *                 门控); curses 使能决策 (DAT_0007bb18/75dd0); "Started %s";
 *                 配置结果三态 (部分加载/致命 JSON 错误/成功) + 释放; "/usr/bin"
 *                 追加 '/'; per-device-stats → 强制 verbose。
 * 段 5  1015-1607 scan_time 兜底 (60s/30s), control_thr calloc(8×0x40) ("Failed to
 *                 calloc control_thr"), 24 驱动 fill_drv_defaults + 探测调用
 *                 (DRV_PROBE, r0=0), 逐设备 enable_device (登记 devices[] +
 *                 线程累计 DAT_0007bb6c), 无设备 "All devices disabled, cannot
 *                 mine!", temp_cutoffs (缺省 95/空列表全 0/单值全设备/>200 报错),
 *                 设备 +0x138=99999999, 矿池 diff(0x1ac/0x1d0)=99999999 +
 *                 userpass "user:pass" 拼接 ("No login credentials..." 报错),
 *                 --monitor 管道 + fork 子进程 execl(/bin/bash -c cmd)。
 * 段 6  1608-2028 mining_thr calloc(mining_threads×4) + 逐槽 calloc(1,0x40);
 *                 逐设备 thr[] 分配, get_thread 取槽, 驱动 thread_prepare(+0x2c)
 *                 成功 → thr_info_create(miner_thread) + 非单线程设备 _cgsem_post
 *                 ("Pushing sem post to thread %d"); 全池 startable; 池启动重试环
 *                 (60s 睡眠, 无池提示 ×3 + 池列表 + curses 按键检查
 *                 "Press any key to exit..."/"No servers could be used! Exiting."),
 *                 统计清零 (DAT_0007acc0/7acc4/0x770e0 区/设备计数), cgtime×3 +
 *                 get_datestamp, watchpool 线程 (槽 2, 失败 "watchpool thread
 *                 create fail", 成功 break), "Probing for an alive pool", 每池
 *                 pthread_create(fun_0001fe0c 命名待补)。
 * 段 7  2191-2256 watchdog 线程 (槽 3, 失败 "watchdog thread create fail"), detach;
 *                 api 线程 (槽 5, 失败 "API thread create failed"), input 线程
 *                 (槽 7, 失败 "input thread create failed"), control_thr 槽数须 8
 *                 ("incorrect total_control_threads (%d) should be 8"), nice(-10)
 *                 (返回 0 且 debug 提示 "Unable to set thread to high priority",
 *                 原样保留反编译条件)。
 * 段 8  2030-2489 主循环 (e368): work update 标志 (各 mining_thr+0x3f=1) →
 *                 current_pool → stgd_lock 下排水环 (staged_work 计数超阈值则
 *                 hash_pop+discard_work, gws_cond 等待) → make_work → 按策略选池
 *                 (4=balance → select_pool; 3=load-balance → 配额轮换 ec10/ebec/
 *                 ebd8/ec2c/ece0 + priority_pool 兜底) → "Selecting pool %d for
 *                 work" → stratum 池: 未就绪重选 (e5cc/e688/e668/e69c) 或
 *                 gen_stratum_work + "Generated stratum work" + hash_push;
 *                 benchmark: 预置目标 (DAT_00078730 区) + 'B' 标记 + hash_push;
 *                 benchfile: benchfile_get_work + calc_diff + hash_push → 回环
 *                 (尾部标签 e7e0/e7e8/e880 轮换重选 + cgsleep_ms(5000))。
 * ==================================================== */

