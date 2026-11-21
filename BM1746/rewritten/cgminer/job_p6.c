/*
 * cgminer 20_job_mining 任务层第 6 段 (批次 job_b6, 82 函数) — BM1746 固件语义转写
 * =========================================================
 * 原始二进制: bm1746/cgminer_z15_bm1746.elf (ARM32, SD 镜像 cgminer-z15, branch "bm1746")
 * 本文件汇集挖矿任务层剩余短函数: getter/包装/锁错误尾/小工具/libgcc 与 jansson 访问器。
 *
 * 主题分组 (82 函数, 按原地址序排布):
 *   锁错误尾与锁包装        9   wr/rw/rd_lock_error, mutex/rwlock/cglock_init, _rd_lock, _rw_unlock, rw_unlock
 *   线程重启与 curses       2   restart_threads, curses_active_locked
 *   统计与错误计数          5   new_nonce, total_staged, inc_hw_errors, wake_gws, total_work_inc
 *   work 生命周期与队列    16   set_thr_busy/idle, clean/free_work, make_clone(含 backdated),
 *                              work_completed, take/clone/find_queued_*, __find_work_bymidstate,
 *                              take_pending_work, clone_and_submit_work, submit_nonce, test_nonce
 *   设备与时间统计         10   get_devices, cgpu_runtime, tsince_restart/update, decay_time,
 *                              get_datestamp, adj_fwidth/adj_width, benchfile_dspwork, reinit_device
 *   util 小工具            11   tq_new, timersub/timeradd/timercmp_gt/lt, copy_timeval, cgtime,
 *                              tdiff, parse_xnsub, _cgsem_post, noop_hook
 *   sha256 接口             3   sha256_ctx_init, sha256_ctx_update, sha256_buffer
 *   api 数据挂接            7   add_uint/uint64/int64/mhtotal/utility/diff/percent_buf
 *   日志                    1   _applog
 *   BM1746 寄存器帧         1   set_nonce_shift_chain
 *   jansson 访问器         11   json_array_string(_nc), hashtable_iter_serial, json_object_size/iter/
 *                              iter_next/iter_key/iter_value/array_get/string_value/number_value
 *   选项表                  2   add_opt, opt_parse
 *   libgcc 运行时           4   __aeabi_uldivmod, __fixdfdi, __fixunsdfdi, __udivmoddi4
 *
 * 转写约定 (rewritten/CONVENTIONS.md):
 *   - 命名事实源 rewritten/name_map/cgminer.tsv; 被调函数名一律取自 name_map。
 *   - thr_info/work/pool/cgpu 等结构体布局不完整, 按约定保留 void* + 字节偏移注释
 *     (偏移语义与 util.c/pool_p1.c/job_p1.c 已锁定字段一致: THR_OFF_CGPU=0x24 等)。
 *   - 日志字符串原样保留; 未知 DAT_xxx 全局保留原名 + 注释; Ghidra 伪变量已消除。
 */

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <signal.h>
#include <semaphore.h>

#include "bm1746_regs.h"
#include "common_types.h"

/* ---- jansson 2.x 类型 (与 cfg.c/freq_p4.c/json.c 保持一致) ---- */
typedef enum json_type {
	JSON_OBJECT  = 0,
	JSON_ARRAY   = 1,
	JSON_STRING  = 2,
	JSON_INTEGER = 3,
	JSON_REAL    = 4,
	JSON_TRUE    = 5,
	JSON_FALSE   = 6,
	JSON_NULL    = 7
} json_type;

/* json_t 公共头 (各节点首两字段) */
typedef struct json_t {
	int type;       /* +0x00: json_type 枚举值 */
	int refcount;   /* +0x04: 引用计数 */
} json_t;

/* ---- SHA-256 初始哈希值 H0 (原 .data 0x000760dc, 8 字, 已从 ELF 数据段核实为标准值) ---- */
static const uint32_t sha256_h0[8] = {
	0x6a09e667u, 0xbb67ae85u, 0x3c6ef372u, 0xa54ff53au,
	0x510e527fu, 0x9b05688cu, 0x1f83d9abu, 0x5be0cd19u
};

/* ============================================================================
 * 外部被调 (均已在其他转写文件定义, 命名取自 name_map)
 * ============================================================================ */
extern void applog(uint32_t prio, char *buf, ...);                  /* FUN_00038730 (job_p5b); 可变参声明同 log.c */
extern void quit(int status);                                       /* FUN_00016ca8 (util.c) */
extern void restart_threads_err(void);                              /* FUN_0000b3c8 (util.c) */
extern void mutex_lock_fail_cgminer(const char *fn, int line);      /* FUN_0000b414 (util.c) */
extern void mutex_init_fail_cgminer(const char *fn, int line);      /* FUN_0000b4ec (util.c) */
extern void rwlock_init_fail_cgminer(const char *fn, int line);     /* FUN_0000b558 (util.c) */
extern void mutex_unlock_fail_cgminer(const char *fn, int line);    /* FUN_0000b5c4 (util.c) */
extern void restart_thread(void *arg);                              /* FUN_000231f0 (util.c) */
extern void discard_work(void *work);                               /* FUN_000159b4 (util.c) */
extern void workid_hash_add(void *thr, void *work);                 /* FUN_00016624 (util.c) */

extern void regen_hash(void *work);                                 /* FUN_00011f60: 全 80B 重生成 + 双 SHA-256 */
extern void rebuild_nonce(void *work);                              /* FUN_00014f30: scrypt 重建 (0x20000 草稿区) */
extern void *make_work(void);                                       /* FUN_0001a3d4: calloc 0x7b0 work */
extern void copy_work(void *dst, const void *src, int nonce2);      /* FUN_00015684 */
extern void __work_completed(void *thr, void *work);                /* FUN_00016b40: 出队 + workid 表摘除 */
extern int stale_work(void *work, int share);                       /* FUN_00022b44: 过期判定 */
extern void submit_work_async(void *work);                          /* FUN_000256a4 */
extern void submit_tested_work(void *thr, void *work);              /* FUN_00025e5c */
extern void __bin2hex(char *dst, const uint8_t *src, int len);      /* FUN_000296d8 (hash.c) */

extern int log_curses_only(int prio, const char *s1, const char *s2); /* FUN_000199e8 */
extern void my_log_curses_lock(void);                               /* FUN_000388e4 */
extern void my_log_curses_unlock(int line);                         /* FUN_0003896c */

extern uint32_t makeup_cmd_frame(uint8_t *frame, uint8_t rw, uint8_t chip_addr,
                                 uint8_t reg, uint32_t value);      /* FUN_0003cdcc (chain.c) */
extern uint32_t uart_send(int chain, const void *buf, uint32_t len);/* FUN_0003c150 (uart.c) */
extern void cgsleep_ms(int ms);                                     /* FUN_0002a92c (chain.c) */

extern void sha256_compress(uint32_t *ctx, const void *data, uint32_t nblocks); /* FUN_000300b0 */
extern void sha256_ctx_final(uint32_t *ctx, void *out);             /* FUN_000303a4 */
extern void *api_add_data_full(void *root, char *name, int type, void *data, int copied); /* FUN_00031cdc (api_p2.c) */

extern int parse_one(int *argc, char **argv, unsigned *offset,
                     void (*errlog)(const char *fmt, ...));         /* FUN_0005bdec (cfg.c) */
extern unsigned int json_array_size(json_t *array);                 /* FUN_00059fec (cfg.c) */
extern double __floatdidf(unsigned long long value);                /* FUN_0005cb64: int64→double */
extern unsigned long long __udivdi3(unsigned long long n, unsigned long long d); /* FUN_0005d160 (job_p3.c) */

extern char *strdup(const char *s);                                 /* 原码 __strdup */

/* ---- 本文件内部前向声明 (函数按原地址序排布, 先用后定义) ---- */
void cgtime(struct timeval *tv);                                    /* FUN_0002a84c */
double tdiff(const struct timeval *a, const struct timeval *b);     /* FUN_0002aa24 */
void clean_work(void *work);                                        /* FUN_00015648 */
void free_work(void *work);                                         /* FUN_00015780 */
void *make_clone(void *base, int nonce2);                           /* FUN_0001a44c */
int new_nonce(void *thr, uint32_t nonce);                           /* FUN_0000f7e8 */
void inc_hw_errors(void *thr);                                      /* FUN_000189a4 */
void wake_gws(void);                                                /* FUN_00019840 */
void benchfile_dspwork(void *work, uint32_t nonce);                 /* FUN_00011814 */
void wr_lock_error(const char *fn, int line);                       /* FUN_0000b480 */
void rw_unlock_error(const char *fn, int line);                     /* FUN_0000b630 */
void rd_lock_error(const char *fn, int line);                       /* FUN_0000b69c */
json_t *json_array_get(json_t *array, size_t index);                /* FUN_0005a008 */
char *json_string_value(json_t *value);                             /* FUN_0005a178 */
char *json_array_string_nc(json_t *json, size_t index);             /* FUN_00029294 */
void *__find_work_bymidstate(void *head, const void *midstate, size_t midstatelen,
                             const void *data, int offset, size_t datalen); /* FUN_00016a80 */
void sha256_ctx_update(uint32_t *ctx, const void *data, uint32_t len); /* FUN_00030304 */
unsigned long long __udivmoddi4(unsigned long long n, unsigned long long d,
                                unsigned long long *rp);            /* FUN_0005cccc */
unsigned long long __fixunsdfdi(double a);                          /* FUN_0005cc48 */

/* ============================================================================
 * 本文件引用的外部全局 (DAT_ 命名待补, 按约定保留原名)
 * ============================================================================ */
extern char  DAT_0007ad48;      /* applog 门控开关 A */
extern char  DAT_00078e08;      /* applog 门控开关 B */
extern char  DAT_00078e09;      /* opt_debug (门控主开关) */
extern int   DAT_000766c4;      /* 日志级别 (int) */
extern uint8_t DAT_0007bc78;    /* opt_quiet (静默标志) */
extern int   DAT_00075dd0;      /* 全局, 语义待定 (curses 输出路径开关) */
extern char  DAT_0007b859;      /* 算法模式标志: 0=sha256d, 1=scrypt (难度常量/test_nonce 路径均随其切换) */
extern char  DAT_00077018;      /* use_curses (curses 模式启用标志) */
extern void  (*DAT_00075ed8)(void); /* 让步钩子: ELF 初值 = noop_hook (恒 0), 主循环设为 sched_yield */

extern pthread_mutex_t DAT_0007b7a8;   /* curses_lock (lock_curses/unlock_curses) */
extern pthread_mutex_t DAT_0007ad4c;   /* stats_lock (hw_errors 计数保护) */
extern int             DAT_0007b8a4;   /* 全局硬件错误计数 (hw_errors 总计) */
extern pthread_mutex_t *DAT_0007704c;  /* stgd_lock (getq 暂存队列 mutex 指针别名) */
extern void            *DAT_00077050;  /* 全局, 语义待定 (+0x728 → 暂存统计表结构) */
extern pthread_cond_t  DAT_0007b1a0;   /* gws_cond (get_work 唤醒) */
extern pthread_mutex_t DAT_0007ba10;   /* control_lock cglock 互斥量部分 */
extern pthread_rwlock_t DAT_0007ba28;  /* control_lock cglock 读写锁部分 (mutex+0x18) */
extern int             DAT_000770a8;   /* total_work 总工作计数 */
extern pthread_rwlock_t DAT_0007b700;  /* devices_lock (设备数组读写锁) */
extern void            *DAT_0007b628;  /* devices 数组 (cgpu_info* 槽) */
extern struct timeval  DAT_00076f04;   /* 全局时刻: restart_thread 创建时刻 (restart_threads 写) */
extern struct timeval  DAT_00076f0c;   /* 全局时刻, 语义待定 (与 0x76f04 相邻成对) */
extern uint32_t        DAT_00076b50;   /* 全局 nonce2 位移值 (set_nonce_shift_chain 写) */
extern double          DAT_000755f8;   /* .data 0x755f8 常量 = 1.0 (已核实) */
extern char           *DAT_0009d780;   /* opt_argv0 (opt_parse 记录 argv[0]) */
extern void           *DAT_0009d77c;   /* opt_table 动态选项表 (7 字/项) */
extern unsigned int    DAT_0009d774;   /* opt_count 动态表项数 */
extern int             DAT_00076ffc;   /* benchfile 模式开关 A */
extern char            DAT_0007871c;   /* benchfile 模式开关 B */

/* ============================================================================
 * 函数转写 (按原地址序)
 * ============================================================================ */

// === wr_lock_error === 原 FUN_0000b480 @ 0x0000b480
// 来源: reversed/cgminer/20_job_mining/FUN_0000b480.c | 分类: 20_job_mining
// 语义: pthread_rwlock_wrlock 失败统一报错 (文件名固定 "cgminer.c"), 报完 quit(1)。
void wr_lock_error(const char *fn, int line)
{
	char msg[4100];

	snprintf(msg, 0x1000, "WTF WRLOCK ERROR ON LOCK! errno=%d in %s %s():%d",
	         errno, "cgminer.c", fn, line);
	applog(3, msg, 1);
	quit(1);
}

// === rw_unlock_error === 原 FUN_0000b630 @ 0x0000b630
// 来源: reversed/cgminer/20_job_mining/FUN_0000b630.c | 分类: 20_job_mining
// 语义: pthread_rwlock_unlock 失败统一报错 (文件名固定 "cgminer.c"), 报完 quit(1)。
void rw_unlock_error(const char *fn, int line)
{
	char msg[4100];

	snprintf(msg, 0x1000, "WTF RWLOCK ERROR ON UNLOCK! errno=%d in %s %s():%d",
	         errno, "cgminer.c", fn, line);
	applog(3, msg, 1);
	quit(1);
}

// === rd_lock_error === 原 FUN_0000b69c @ 0x0000b69c
// 来源: reversed/cgminer/20_job_mining/FUN_0000b69c.c | 分类: 20_job_mining
// 语义: pthread_rwlock_rdlock 失败统一报错 (文件名固定 "cgminer.c"), 报完 quit(1)。
void rd_lock_error(const char *fn, int line)
{
	char msg[4100];

	snprintf(msg, 0x1000, "WTF RDLOCK ERROR ON LOCK! errno=%d in %s %s():%d",
	         errno, "cgminer.c", fn, line);
	applog(3, msg, 1);
	quit(1);
}

// === new_nonce === 原 FUN_0000f7e8 @ 0x0000f7e8
// 来源: reversed/cgminer/20_job_mining/FUN_0000f7e8.c | 分类: 20_job_mining
// 语义: 重复 nonce 检测 — thr->cgpu(+0x24) 的 last_nonce(+0xec) 与本次 nonce 相同则
//       记日志并返回 0 (重复, 按 HW 错误处理); 否则登记并返回 1 (新 nonce)。
int new_nonce(void *thr, uint32_t nonce)
{
	int ret;
	char msg[4100];
	void *cgpu = *(void **)((char *)thr + 0x24);   /* thr->cgpu */

	ret = 1;
	if (*(int *)((char *)cgpu + 0xec) == (int)nonce) {
		ret = 0;
		if ((DAT_00078e09 != '\0') &&
		    ((DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 6 < DAT_000766c4))) {
			snprintf(msg, 0x1000, "%s %d duplicate share detected as HW error",
			         *(char **)(*(void **)((char *)cgpu + 0x04) + 8),  /* cgpu->drv->dname */
			         *(int *)((char *)cgpu + 0x08));                   /* cgpu->device_id */
			applog(7, msg, 0);
		}
	} else {
		*(int *)((char *)cgpu + 0xec) = (int)nonce;   /* cgpu->last_nonce = nonce */
	}
	return ret;
}

// === set_thr_busy === 原 FUN_0000fc24 @ 0x0000fc24
// 来源: reversed/cgminer/20_job_mining/FUN_0000fc24.c | 分类: 20_job_mining
// 语义: 标记线程工作繁忙: busy 字节(+0x3d)清 0, 记录时刻(+0x2c), 清 cgpu+0x60
//       并把 cgpu+0xf0 (最近工作时刻) 置为当前 time(NULL)。
void set_thr_busy(void *thr)
{
	time_t now;
	void *cgpu;

	*(uint8_t *)((char *)thr + 0x3d) = 0;
	cgtime((struct timeval *)((char *)thr + 0x2c));
	cgpu = *(void **)((char *)thr + 0x24);
	*(uint32_t *)((char *)cgpu + 0x60) = 0;
	now = time(NULL);
	*(time_t *)((char *)cgpu + 0xf0) = now;
}

// === set_thr_idle === 原 FUN_0000fc58 @ 0x0000fc58
// 来源: reversed/cgminer/20_job_mining/FUN_0000fc58.c | 分类: 20_job_mining
// 语义: 与 set_thr_busy 对称: busy 字节(+0x3d)置 1, 其余副作用相同。
void set_thr_idle(void *thr)
{
	time_t now;
	void *cgpu;

	*(uint8_t *)((char *)thr + 0x3d) = 1;
	cgtime((struct timeval *)((char *)thr + 0x2c));
	cgpu = *(void **)((char *)thr + 0x24);
	*(uint32_t *)((char *)cgpu + 0x60) = 0;
	now = time(NULL);
	*(time_t *)((char *)cgpu + 0xf0) = now;
}

// === adj_fwidth === 原 FUN_00011514 @ 0x00011514
// 来源: reversed/cgminer/20_job_mining/FUN_00011514.c | 分类: 20_job_mining
// 语义: 统计打印列宽自适应: 十进制位数 log10(f)+1 (截断取整) 超过 *width 时增 1。
void adj_fwidth(float f, int *width)
{
	int newlen;

	newlen = (int)(log10((double)f) + 1.0);
	if (*width < newlen)
		(*width)++;
}

// === adj_width === 原 FUN_00011548 @ 0x00011548
// 来源: reversed/cgminer/20_job_mining/FUN_00011548.c | 分类: 20_job_mining
// 语义: adj_fwidth 的整数版 (编译器内联展开): log10((double)val)+1 超过 *width 时增 1。
void adj_width(int val, int *width)
{
	int newlen;

	newlen = (int)(log10((double)val) + 1.0);
	if (*width < newlen)
		(*width)++;
}

// === benchfile_dspwork === 原 FUN_00011814 @ 0x00011814
// 来源: reversed/cgminer/20_job_mining/FUN_00011814.c | 分类: 20_job_mining
// 语义: BENCHFILE 模式下展示工作头: nonce 按字节反序 (大端重排) 后, 连同
//       work+0 起 128 字节头部的十六进制串一起打印 (门控: debug 或 benchfile 日志)。
void benchfile_dspwork(void *work, uint32_t nonce)
{
	uint32_t nonce_be;
	int i;
	char hex[1024];
	char msg[4096];

	/* 4 次循环字节反序: nonce_be = bswap32(nonce) */
	nonce_be = 0;
	for (i = 4; i != 0; i--) {
		nonce_be = (nonce & 0xff) + nonce_be * 0x100;
		nonce = nonce >> 8;
	}
	__bin2hex(hex, (const uint8_t *)work, 0x80);
	if ((DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 2 < DAT_000766c4)) {
		snprintf(msg, 0x1000, "BENCHFILE nonce %u=0x%08x for work=%s",
		         nonce_be, nonce_be, hex);
		applog(3, msg, 0);
	}
}

// === get_datestamp === 原 FUN_00015264 @ 0x00015264
// 来源: reversed/cgminer/20_job_mining/FUN_00015264.c | 分类: 20_job_mining
// 语义: 把 time_t 格式化为 "[YYYY-MM-DD hh:mm:ss]" (本地时间, 年 = tm_year+1900)。
void get_datestamp(char *buf, size_t siz, const time_t *tp)
{
	struct tm *tm;
	time_t t;

	t = *tp;
	tm = localtime(&t);
	snprintf(buf, siz, "[%d-%02d-%02d %02d:%02d:%02d]",
	         tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
	         tm->tm_hour, tm->tm_min, tm->tm_sec);
}

// === clean_work === 原 FUN_00015648 @ 0x00015648
// 来源: reversed/cgminer/20_job_mining/FUN_00015648.c | 分类: 20_job_mining
// 语义: 释放 work 内 4 个动态字段 (+0x6f0 job_id, +0x704 nonce1, +0x710/+0x718
//       其余字符串字段, 释放序为原机器码顺序), 然后整块清零 (size 0x7b0)。
void clean_work(void *work)
{
	free(*(void **)((char *)work + 0x6f0));
	free(*(void **)((char *)work + 0x704));
	free(*(void **)((char *)work + 0x718));
	free(*(void **)((char *)work + 0x710));
	memset(work, 0, 0x7b0);
}

// === free_work === 原 FUN_00015780 @ 0x00015780
// 来源: reversed/cgminer/20_job_mining/FUN_00015780.c | 分类: 20_job_mining
// 语义: 先 clean_work 再释放 work 本体。
void free_work(void *work)
{
	clean_work(work);
	free(work);
}

// === decay_time === 原 FUN_00015798 @ 0x00015798
// 来源: reversed/cgminer/20_job_mining/FUN_00015798.c | 分类: 20_job_mining
// 语义: 指数衰减滑动平均 (hashmeter 滚动算力用): 权重 s = 1 - 1/exp(tdiff/f_decay),
//       *f = (*f + s * (f_total/tdiff)) / (s + 1); tdiff <= 0 时不动。
//       形参顺序 (f, f_total, tdiff, f_decay) 由函数体算术与 hashmeter 调用点寄存器
//       (r0=&f, d0=f_total, d1=tdiff, d2=f_decay, 硬浮点 ABI) 推定, 与 job_p3.c 一致。
void decay_time(double *f, double f_total, double tdiff, double f_decay)
{
	double s;

	if (0.0 < tdiff) {
		s = 1.0 - 1.0 / exp(tdiff / f_decay);
		*f = (*f + s * (f_total / tdiff)) / (s + 1.0);
	}
}

// === cgpu_runtime === 原 FUN_000157ec @ 0x000157ec
// 来源: reversed/cgminer/20_job_mining/FUN_000157ec.c | 分类: 20_job_mining
// 语义: 设备运行时长 (秒): dev_start_tv(+0x170) 为 0 返回常量 1.0 (DAT_000755f8);
//       否则 cgtime 后 tdiff 求差; 结果 < 1.0 时返回 0.0 (按反编译原样转写)。
//       原函数经 VFP s0/s1 返回 double (Ghidra 仅渲染了 r0 低半字)。
double cgpu_runtime(void *cgpu)
{
	double runtime;
	struct timeval now;

	if (*(int *)((char *)cgpu + 0x170) == 0) {
		runtime = DAT_000755f8;   /* 1.0 */
	} else {
		cgtime(&now);
		runtime = tdiff(&now, (struct timeval *)((char *)cgpu + 0x170));
	}
	if (runtime < 1.0)
		runtime = 0.0;
	return runtime;
}

// === tsince_restart === 原 FUN_00015840 @ 0x00015840
// 来源: reversed/cgminer/20_job_mining/FUN_00015840.c | 分类: 20_job_mining
// 语义: 距全局重启时刻 DAT_00076f04 的秒差 (restart_threads 每次创建重启线程时刷新)。
//       原函数返回 double (s0/s1), Ghidra 误渲染为 void。
double tsince_restart(void)
{
	struct timeval now;

	cgtime(&now);
	return tdiff(&now, &DAT_00076f04);
}

// === tsince_update === 原 FUN_00015868 @ 0x00015868
// 来源: reversed/cgminer/20_job_mining/FUN_00015868.c | 分类: 20_job_mining
// 语义: 与 tsince_restart 对称, 基准为相邻全局时刻 DAT_00076f0c (语义待定)。
double tsince_update(void)
{
	struct timeval now;

	cgtime(&now);
	return tdiff(&now, &DAT_00076f0c);
}

// === test_nonce === 原 FUN_0001654c @ 0x0001654c
// 来源: reversed/cgminer/20_job_mining/FUN_0001654c.c | 分类: 20_job_mining
// 语义: nonce 快速检验: 写 work->nonce(+0x4c), 按 DAT_0007b859 (0=sha256d, 1=scrypt)
//       选 regen_hash / rebuild_nonce 重算 work->hash(+0xc0..0xe0), 再比较首字:
//       sha256d 要求 hash[7](+0xdc) <= 0 (Bitcoin diff-1 目标顶字), scrypt 要求
//       <= 0xffff (Litecoin diff-1)。返回 bool。
int test_nonce(void *work, uint32_t nonce)
{
	uint32_t limit;

	*(uint32_t *)((char *)work + 0x4c) = nonce;
	if (DAT_0007b859 == '\0')
		regen_hash(work);
	else
		rebuild_nonce(work);
	if (DAT_0007b859 == '\0')
		limit = 0;
	else
		limit = 0xffff;
	return *(uint32_t *)((char *)work + 0xdc) <= limit;
}

// === __find_work_bymidstate === 原 FUN_00016a80 @ 0x00016a80
// 来源: reversed/cgminer/20_job_mining/FUN_00016a80.c | 分类: 20_job_mining
// 语义: 在 thr+0x160 锚定的排队 work 链 (next @ +0x730) 上线性查找: 同时满足
//       midstate(+0x80, midstatelen 字节) 与 data(+offset, datalen 字节) 匹配的 work;
//       无则 NULL。
void *__find_work_bymidstate(void *head, const void *midstate, size_t midstatelen,
                             const void *data, int offset, size_t datalen)
{
	void *w;

	if (head == NULL)
		return NULL;
	for (w = head; w != NULL; w = *(void **)((char *)w + 0x730)) {
		if (memcmp((char *)w + 0x80, midstate, midstatelen) == 0 &&
		    memcmp((char *)w + offset, data, datalen) == 0)
			return w;
	}
	return NULL;
}

// === reinit_device === 原 FUN_00016c88 @ 0x00016c88
// 来源: reversed/cgminer/20_job_mining/FUN_00016c88.c | 分类: 20_job_mining
// 语义: 设备重初始化: cgpu+0x20 == 1 时直接返回, 否则经 drv(+4)->fn(+0x10) 回调。
void reinit_device(void *cgpu)
{
	if (*(int *)((char *)cgpu + 0x20) == 1)
		return;
	(*(void (**)(void *))(*(void **)((char *)cgpu + 0x04) + 0x10))(cgpu);
}

// === restart_threads === 原 FUN_00017210 @ 0x00017210
// 来源: reversed/cgminer/20_job_mining/FUN_00017210.c | 分类: 20_job_mining
// 语义: 记录全局重启时刻 DAT_00076f04 并创建重启线程 restart_thread, 失败报
//       "Failed to create restart thread" 后退出。
void restart_threads(void)
{
	pthread_t tid;
	int rc;

	cgtime(&DAT_00076f04);
	rc = pthread_create(&tid, NULL, (void *(*)(void *))restart_thread, NULL);
	if (rc != 0)
		restart_threads_err();
}

// === mutex_init === 原 FUN_00017990 @ 0x00017990
// 来源: reversed/cgminer/20_job_mining/FUN_00017990.c | 分类: 20_job_mining
// 语义: pthread_mutex_init 包装, 失败走 mutex_init_fail_cgminer (报错 + quit)。
void mutex_init(pthread_mutex_t *mutex, const char *fn, int line)
{
	int rc;

	rc = pthread_mutex_init(mutex, NULL);
	if (rc == 0)
		return;
	mutex_init_fail_cgminer(fn, line);
}

// === rwlock_init === 原 FUN_000179bc @ 0x000179bc
// 来源: reversed/cgminer/20_job_mining/FUN_000179bc.c | 分类: 20_job_mining
// 语义: pthread_rwlock_init 包装, 失败走 rwlock_init_fail_cgminer (报错 + quit)。
void rwlock_init(pthread_rwlock_t *rwlock, const char *fn, int line)
{
	int rc;

	rc = pthread_rwlock_init(rwlock, NULL);
	if (rc == 0)
		return;
	rwlock_init_fail_cgminer(fn, line);
}

// === cglock_init === 原 FUN_000179e8 @ 0x000179e8
// 来源: reversed/cgminer/20_job_mining/FUN_000179e8.c | 分类: 20_job_mining
// 语义: cglock 组合锁初始化: 先 mutex (偏移 +0), 再 rwlock (紧随其后, +0x18,
//       即 pthread_mutex_t 紧邻排布; 与 DAT_0007ba10/0x7ba28 布局一致)。
void cglock_init(pthread_mutex_t *mutex, const char *fn, int line)
{
	int rc;

	rc = pthread_mutex_init(mutex, NULL);
	if (rc != 0)
		mutex_init_fail_cgminer(fn, line);
	rc = pthread_rwlock_init((pthread_rwlock_t *)(mutex + 1), NULL);
	if (rc == 0)
		return;
	rwlock_init_fail_cgminer(fn, line);
}

// === curses_active_locked === 原 FUN_00017fe4 @ 0x00017fe4
// 来源: reversed/cgminer/20_job_mining/FUN_00017fe4.c | 分类: 20_job_mining
// 语义: lock_curses 后检查 use_curses (DAT_00077018): 非 0 直接返回该值
//       (原机器码此路径不解锁, 按反编译原样保留); 为 0 则 unlock 并返回 0。
char curses_active_locked(void)
{
	int rc;

	rc = pthread_mutex_lock(&DAT_0007b7a8);
	if (rc != 0)
		mutex_lock_fail_cgminer("lock_curses", 0xb77);
	if (DAT_00077018 != '\0')
		return DAT_00077018;
	rc = pthread_mutex_unlock(&DAT_0007b7a8);
	if (rc != 0)
		mutex_unlock_fail_cgminer("unlock_curses", 0xb72);
	(*DAT_00075ed8)();
	return '\0';
}

// === total_staged === 原 FUN_000188a8 @ 0x000188a8
// 来源: reversed/cgminer/20_job_mining/FUN_000188a8.c | 分类: 20_job_mining
// 语义: 暂存队列计数: stgd_lock (DAT_0007704c) 保护下读取 DAT_00077050+0x728
//       所指统计表结构的 +0xc 计数字段 (上游 HASH_COUNT(staged_work) 等价物)。
int total_staged(void)
{
	int ret;
	int rc;

	rc = pthread_mutex_lock(DAT_0007704c);
	if (rc != 0)
		mutex_lock_fail_cgminer("total_staged", 0xb5b);
	if (DAT_00077050 == NULL)
		ret = 0;
	else
		ret = *(int *)(*(void **)((char *)DAT_00077050 + 0x728) + 0xc);
	rc = pthread_mutex_unlock(DAT_0007704c);
	if (rc != 0)
		mutex_unlock_fail_cgminer("total_staged", 0xb5d);
	(*DAT_00075ed8)();
	return ret;
}

// === inc_hw_errors === 原 FUN_000189a4 @ 0x000189a4
// 来源: reversed/cgminer/20_job_mining/FUN_000189a4.c | 分类: 20_job_mining
// 语义: 硬件错误计数: 门控日志 "%s %d: invalid nonce - HW error" (drv->dname,
//       cgpu->device_id), stats_lock (DAT_0007ad4c) 保护下 cgpu->hw_errors(+0x2c)
//       与全局 DAT_0007b8a4 同步加 1, 尾调让步钩子并回调 drv(+4)->fn(+0x54)(thr)。
void inc_hw_errors(void *thr)
{
	int rc;
	char msg[4096];
	void *cgpu;

	if ((DAT_00078e09 != '\0') &&
	    ((DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 6 < DAT_000766c4))) {
		cgpu = *(void **)((char *)thr + 0x24);
		snprintf(msg, 0x1000, "%s %d: invalid nonce - HW error",
		         *(char **)(*(void **)((char *)cgpu + 0x04) + 8),  /* cgpu->drv->dname */
		         *(int *)((char *)cgpu + 0x08));                   /* cgpu->device_id */
		applog(7, msg, 1);
	}
	rc = pthread_mutex_lock(&DAT_0007ad4c);
	if (rc != 0)
		mutex_lock_fail_cgminer("inc_hw_errors", 0x2068);
	cgpu = *(void **)((char *)thr + 0x24);
	*(int *)((char *)cgpu + 0x2c) = *(int *)((char *)cgpu + 0x2c) + 1;
	DAT_0007b8a4 = DAT_0007b8a4 + 1;
	rc = pthread_mutex_unlock(&DAT_0007ad4c);
	if (rc != 0)
		mutex_unlock_fail_cgminer("inc_hw_errors", 0x206b);
	(*DAT_00075ed8)();
	(*(void (**)(void *))(*(void **)((char *)cgpu + 0x04) + 0x54))(thr);
}

// === wake_gws === 原 FUN_00019840 @ 0x00019840
// 来源: reversed/cgminer/20_job_mining/FUN_00019840.c | 分类: 20_job_mining
// 语义: stgd_lock 保护下 signal gws_cond, 唤醒等待暂存工作的 get_work 线程。
void wake_gws(void)
{
	int rc;

	rc = pthread_mutex_lock(DAT_0007704c);
	if (rc != 0)
		mutex_lock_fail_cgminer("wake_gws", 0x1454);
	pthread_cond_signal(&DAT_0007b1a0);
	rc = pthread_mutex_unlock(DAT_0007704c);
	if (rc != 0)
		mutex_unlock_fail_cgminer("wake_gws", 0x1456);
	(*DAT_00075ed8)();
}

// === rw_unlock === 原 FUN_00019bbc @ 0x00019bbc
// 来源: reversed/cgminer/20_job_mining/FUN_00019bbc.c | 分类: 20_job_mining
// 语义: pthread_rwlock_unlock 包装: 失败走 rw_unlock_error, 成功路径尾调让步钩子。
void rw_unlock(pthread_rwlock_t *rwlock, const char *fn, int line)
{
	int rc;

	rc = pthread_rwlock_unlock(rwlock);
	if (rc != 0)
		rw_unlock_error(fn, line);
	(*DAT_00075ed8)();
}

// === work_completed === 原 FUN_0001a0b8 @ 0x0001a0b8
// 来源: reversed/cgminer/20_job_mining/FUN_0001a0b8.c | 分类: 20_job_mining
// 语义: 工作完成登记: thr+0x140 排队锁写锁定 → __work_completed (出队 + workid 表
//       摘除) → 解锁 → 让步钩子 → free_work 释放工作。
void work_completed(void *thr, void *work)
{
	int rc;

	rc = pthread_rwlock_wrlock((pthread_rwlock_t *)((char *)thr + 0x140));
	if (rc != 0)
		wr_lock_error("work_completed", 0x2309);
	__work_completed(thr, work);
	rc = pthread_rwlock_unlock((pthread_rwlock_t *)((char *)thr + 0x140));
	if (rc != 0)
		rw_unlock_error("work_completed", 0x230b);
	(*DAT_00075ed8)();
	free_work(work);
}

// === take_queued_work_bymidstate === 原 FUN_0001a134 @ 0x0001a134
// 来源: reversed/cgminer/20_job_mining/FUN_0001a134.c | 分类: 20_job_mining
// 语义: 写锁定下按 midstate+data 在排队链中查找并即时出队 (__work_completed),
//       解锁 → 让步钩子 → 返回取到的 work (无则 NULL)。
void *take_queued_work_bymidstate(void *thr, const void *midstate, size_t midstatelen,
                                  const void *data, int offset, size_t datalen)
{
	void *work;
	int rc;

	rc = pthread_rwlock_wrlock((pthread_rwlock_t *)((char *)thr + 0x140));
	if (rc != 0)
		wr_lock_error("take_queued_work_bymidstate", 0x2316);
	work = __find_work_bymidstate(*(void **)((char *)thr + 0x160),
	                              midstate, midstatelen, data, offset, datalen);
	if (work != NULL)
		__work_completed(thr, work);
	rc = pthread_rwlock_unlock((pthread_rwlock_t *)((char *)thr + 0x140));
	if (rc != 0)
		rw_unlock_error("take_queued_work_bymidstate", 0x231a);
	(*DAT_00075ed8)();
	return work;
}

// === total_work_inc === 原 FUN_0001a1e8 @ 0x0001a1e8
// 来源: reversed/cgminer/20_job_mining/FUN_0001a1e8.c | 分类: 20_job_mining
// 语义: total_work (DAT_000770a8) 自增并返回旧值; 双重锁保护: control_lock 的
//       mutex (DAT_0007ba10) + rwlock (DAT_0007ba28, 即 mutex+0x18)。
int total_work_inc(void)
{
	int old;
	int rc;

	rc = pthread_mutex_lock(&DAT_0007ba10);
	if (rc != 0)
		mutex_lock_fail_cgminer("total_work_inc", 0x8e1);
	rc = pthread_rwlock_wrlock(&DAT_0007ba28);
	if (rc != 0)
		wr_lock_error("total_work_inc", 0x8e1);
	old = DAT_000770a8;
	DAT_000770a8 = DAT_000770a8 + 1;
	rc = pthread_rwlock_unlock(&DAT_0007ba28);
	if (rc != 0)
		rw_unlock_error("total_work_inc", 0x8e3);
	rc = pthread_mutex_unlock(&DAT_0007ba10);
	if (rc != 0)
		mutex_unlock_fail_cgminer("total_work_inc", 0x8e3);
	(*DAT_00075ed8)();
	return old;
}

// === make_clone === 原 FUN_0001a44c @ 0x0001a44c
// 来源: reversed/cgminer/20_job_mining/FUN_0001a44c.c | 分类: 20_job_mining
// 语义: 克隆 work: make_work 分配新块后 copy_work(dst, src, nonce2) 复制
//       (nonce2 != 0 时改写 +0x44 的 BE nonce2), 返回新 work。
void *make_clone(void *base, int nonce2)
{
	void *clone;

	clone = make_work();
	copy_work(clone, base, nonce2);
	return clone;
}

// === clone_work_backdated === 原 FUN_0001a474 @ 0x0001a474
// 来源: reversed/cgminer/20_job_mining/FUN_0001a474.c | 分类: 20_job_mining
// 语义: 回溯克隆: make_clone(base, 0) 后标 clone 标志(+0x6e1)=1, tv_stamp(+0x770)
//       取当前时刻, 清 +0x6e8/+0x6ea 字节, 并把 tv_gen(+0x6d8) 减 1 (使克隆版
//       看起来早于原件)。仅经函数指针引用, 上游名不确定 [低置信]。
void *clone_work_backdated(void *base)
{
	void *clone;

	clone = make_clone(base, 0);
	*(uint8_t *)((char *)clone + 0x6e1) = 1;
	cgtime((struct timeval *)((char *)clone + 0x770));
	*(uint8_t *)((char *)clone + 0x6e8) = 0;
	*(int *)((char *)clone + 0x6d8) = *(int *)((char *)clone + 0x6d8) + -1;
	*(uint8_t *)((char *)clone + 0x6ea) = 0;
	return clone;
}

// === _rd_lock === 原 FUN_0001ab24 @ 0x0001ab24
// 来源: reversed/cgminer/20_job_mining/FUN_0001ab24.c | 分类: 20_job_mining
// 语义: pthread_rwlock_rdlock 包装: 失败走 rd_lock_error (cgminer.c 变体), 无让步钩子。
void _rd_lock(pthread_rwlock_t *rwlock, const char *fn, int line)
{
	int rc;

	rc = pthread_rwlock_rdlock(rwlock);
	if (rc == 0)
		return;
	rd_lock_error(fn, line);
}

// === clone_queued_work_bymidstate === 原 FUN_0001ab4c @ 0x0001ab4c
// 来源: reversed/cgminer/20_job_mining/FUN_0001ab4c.c | 分类: 20_job_mining
// 语义: 读锁定下按 midstate+data 查排队 work, 命中则 make_clone(work, 0) 克隆,
//       解锁 → 让步钩子 → 返回克隆 (未命中返回 NULL)。
void *clone_queued_work_bymidstate(void *thr, const void *midstate, size_t midstatelen,
                                   const void *data, int offset, size_t datalen)
{
	void *found;
	void *clone;
	int rc;

	rc = pthread_rwlock_rdlock((pthread_rwlock_t *)((char *)thr + 0x140));
	if (rc != 0)
		rd_lock_error("clone_queued_work_bymidstate", 0x22b0);
	found = __find_work_bymidstate(*(void **)((char *)thr + 0x160),
	                               midstate, midstatelen, data, offset, datalen);
	clone = NULL;
	if (found != NULL)
		clone = make_clone(found, 0);
	rc = pthread_rwlock_unlock((pthread_rwlock_t *)((char *)thr + 0x140));
	if (rc != 0)
		rw_unlock_error("clone_queued_work_bymidstate", 0x22b4);
	(*DAT_00075ed8)();
	return clone;
}

// === find_queued_work_byid === 原 FUN_0001b074 @ 0x0001b074
// 来源: reversed/cgminer/20_job_mining/FUN_0001b074.c | 分类: 20_job_mining
// 语义: 读锁定下在排队链 (next @ +0x730) 上按 work id (+0x724) 查找,
//       解锁 → 让步钩子 → 返回命中 work (无则 NULL)。
void *find_queued_work_byid(void *thr, int id)
{
	void *w;
	int rc;

	rc = pthread_rwlock_rdlock((pthread_rwlock_t *)((char *)thr + 0x140));
	if (rc != 0)
		rd_lock_error("find_queued_work_byid", 0x22d1);
	w = *(void **)((char *)thr + 0x160);
	if (w == NULL || *(int *)((char *)w + 0x724) == id) {
		/* 空表或首个即命中 */
	} else {
		while (w != NULL && *(int *)((char *)w + 0x724) != id)
			w = *(void **)((char *)w + 0x730);
	}
	rc = pthread_rwlock_unlock((pthread_rwlock_t *)((char *)thr + 0x140));
	if (rc != 0)
		rw_unlock_error("find_queued_work_byid", 0x22d3);
	(*DAT_00075ed8)();
	return w;
}

// === get_devices === 原 FUN_0001b83c @ 0x0001b83c
// 来源: reversed/cgminer/20_job_mining/FUN_0001b83c.c | 分类: 20_job_mining
// 语义: 按索引返回设备: devices_lock (DAT_0007b700) 读锁定下取 devices[index]
//       (DAT_0007b628 槽位), 解锁 → 让步钩子 → 返回 cgpu。
void *get_devices(int index)
{
	void *dev;
	int rc;

	rc = pthread_rwlock_rdlock(&DAT_0007b700);
	if (rc != 0)
		rd_lock_error("get_devices", 0x278);
	dev = *(void **)((char *)DAT_0007b628 + index * 4);
	rc = pthread_rwlock_unlock(&DAT_0007b700);
	if (rc != 0)
		rw_unlock_error("get_devices", 0x27a);
	(*DAT_00075ed8)();
	return dev;
}

// === find_queued_work_bymidstate === 原 FUN_0001d9b4 @ 0x0001d9b4
// 来源: reversed/cgminer/20_job_mining/FUN_0001d9b4.c | 分类: 20_job_mining
// 语义: 与 take_queued_work_bymidstate 同查找但不出队 (只读锁定), 返回命中 work。
void *find_queued_work_bymidstate(void *thr, const void *midstate, size_t midstatelen,
                                  const void *data, int offset, size_t datalen)
{
	void *work;
	int rc;

	rc = pthread_rwlock_rdlock((pthread_rwlock_t *)((char *)thr + 0x140));
	if (rc != 0)
		rd_lock_error("find_queued_work_bymidstate", 0x22a5);
	work = __find_work_bymidstate(*(void **)((char *)thr + 0x160),
	                              midstate, midstatelen, data, offset, datalen);
	rc = pthread_rwlock_unlock((pthread_rwlock_t *)((char *)thr + 0x140));
	if (rc != 0)
		rw_unlock_error("find_queued_work_bymidstate", 0x22a7);
	(*DAT_00075ed8)();
	return work;
}

// === take_pending_work === 原 FUN_00024708 @ 0x00024708
// 来源: reversed/cgminer/20_job_mining/FUN_00024708.c | 分类: 20_job_mining
// 语义: 取走暂存 work (thr+0x164): stale_work 判定通过则登记 workid 表并返回,
//       过期则 discard_work + wake_gws 后返回 NULL; 无论何种结果 +0x164 清 0。
//       反编译调用点给 stale_work 渲染 5 实参 (第 3/4 实参为寄存器残留),
//       实际签名 2 参 (work, share) [其余调用点均为 2 参]; flag_unused 原体未使用。
void *take_pending_work(void *thr, int flag_unused, int stale_a, int stale_b)
{
	void *work;
	void *ret;
	int rc;

	(void)flag_unused;
	(void)stale_a;
	(void)stale_b;
	work = *(void **)((char *)thr + 0x164);
	ret = work;
	if (work != NULL) {
		rc = stale_work(work, 0);
		if (rc == 0) {
			workid_hash_add(thr, work);
		} else {
			ret = NULL;
			discard_work(work);
			wake_gws();
		}
		*(void **)((char *)thr + 0x164) = 0;
	}
	return ret;
}

// === clone_and_submit_work === 原 FUN_00025e48 @ 0x00025e48
// 来源: reversed/cgminer/20_job_mining/FUN_00025e48.c | 分类: 20_job_mining
// 语义: make_clone(work, 0) 后 submit_work_async 异步提交克隆 (仅函数指针引用,
//       上游名不确定 [低置信])。
void clone_and_submit_work(void *work)
{
	void *clone;

	clone = make_clone(work, 0);
	submit_work_async(clone);
}

// === submit_nonce === 原 FUN_00025f48 @ 0x00025f48
// 来源: reversed/cgminer/20_job_mining/FUN_00025f48.c | 分类: 20_job_mining
// 语义: nonce 上报主路径: new_nonce 重复检测 (重复) 或 test_nonce 校验失败 →
//       inc_hw_errors 并返回 0; 通过则 submit_tested_work, BENCHFILE 开关
//       (DAT_00076ffc && DAT_0007871c) 打开时补 benchfile_dspwork, 返回 1。
int submit_nonce(void *thr, void *work, uint32_t nonce)
{
	int rc;

	rc = new_nonce(thr, nonce);
	if (rc == 0 || (rc = test_nonce(work, nonce)) == 0) {
		inc_hw_errors(thr);
		return 0;
	}
	submit_tested_work(thr, work);
	if (DAT_00076ffc == 0 || DAT_0007871c == '\0')
		return 1;
	benchfile_dspwork(work, nonce);
	return 1;
}

// === noop_hook === 原 FUN_00028ae0 @ 0x00028ae0
// 来源: reversed/cgminer/20_job_mining/FUN_00028ae0.c | 分类: 20_job_mining
// 语义: 全局函数指针 DAT_00075ed8 的 ELF 初值, 恒返回 0 (运行时未见改写;
//       主循环将其改指 sched_yield)。多个函数尾部经该指针调用作让步钩子。
int noop_hook(void)
{
	return 0;
}

// === json_array_string_nc === 原 FUN_00029294 @ 0x00029294
// 来源: reversed/cgminer/20_job_mining/FUN_00029294.c | 分类: 20_job_mining
// 语义: jansson 数组元素字符串访问 (不拷贝): 校验 json 非空、类型为 JSON_ARRAY
//       (首条件 ==7/JSON_NULL 为编译器对非数组返回路径的合并, 与 !=1 冗余)、
//       下标未越界、元素为 JSON_STRING, 返回 json_string_value(elem); 否则 NULL。
char *json_array_string_nc(json_t *json, size_t index)
{
	json_t *elem;

	if (json == NULL || json->type == 7 || json->type != 1 ||
	    json_array_size(json) < index ||
	    (elem = json_array_get(json, index)) == NULL || elem->type != 2)
		return NULL;
	return json_string_value(elem);
}

// === json_array_string === 原 FUN_000292f4 @ 0x000292f4
// 来源: reversed/cgminer/20_job_mining/FUN_000292f4.c | 分类: 20_job_mining
// 语义: 经 json_array_string_nc 校验取数组元素字符串后 strdup 拷贝返回
//       (parse_notify_equihash/parse_extranonce_equihash 取 job_id 等字段用)。
char *json_array_string(json_t *json, size_t index)
{
	char *s;

	s = json_array_string_nc(json, index);
	if (s == NULL)
		return NULL;
	return strdup(s);
}

// === _rw_unlock === 原 FUN_0002930c @ 0x0002930c
// 来源: reversed/cgminer/20_job_mining/FUN_0002930c.c | 分类: 20_job_mining
// 语义: pthread_rwlock_unlock 包装 (util.c 变体): 失败打印
//       "WTF RWLOCK ERROR ON UNLOCK! errno=%d in %s %s():%d" 后 quit(1)。
void _rw_unlock(pthread_rwlock_t *rwlock, const char *fn, int line)
{
	int rc;

	rc = pthread_rwlock_unlock(rwlock);
	if (rc != 0) {
		char msg[4100];

		snprintf(msg, 0x1000, "WTF RWLOCK ERROR ON UNLOCK! errno=%d in %s %s():%d",
		         errno, "util.c", fn, line);
		applog(3, msg, 1);
		quit(1);
	}
}

// === tq_new === 原 FUN_0002a168 @ 0x0002a168
// 来源: reversed/cgminer/20_job_mining/FUN_0002a168.c | 分类: 20_job_mining
// 语义: 创建任务队列 (struct tq, 0x58 字节): calloc 后 +0/+4 链表头自环
//       (INIT_LIST_HEAD), +0xc 初始化 mutex, +0x28 初始化 cond; 失败返回 NULL。
void *tq_new(void)
{
	void *tq;

	tq = calloc(1, 0x58);
	if (tq != NULL) {
		*(void **)tq = tq;                          /* list.next = self */
		*(void **)((char *)tq + 4) = tq;            /* list.prev = self */
		pthread_mutex_init((pthread_mutex_t *)((char *)tq + 0xc), NULL);
		pthread_cond_init((pthread_cond_t *)((char *)tq + 0x28), NULL);
	}
	return tq;
}

// === timersub === 原 FUN_0002a448 @ 0x0002a448
// 来源: reversed/cgminer/20_job_mining/FUN_0002a448.c | 分类: 20_job_mining
// 语义: timeval 差 b = a - b (原地写入第二参数), usec 借位时 sec-1/usec+1000000。
void timersub(struct timeval *a, struct timeval *b)
{
	int sec;
	int usec;

	sec = a->tv_sec - b->tv_sec;
	usec = a->tv_usec - b->tv_usec;
	b->tv_sec = sec;
	b->tv_usec = usec;
	if (usec < 0) {
		b->tv_sec = sec - 1;
		b->tv_usec = usec + 1000000;
	}
}

// === timeradd === 原 FUN_0002a47c @ 0x0002a47c
// 来源: reversed/cgminer/20_job_mining/FUN_0002a47c.c | 分类: 20_job_mining
// 语义: timeval 和 b = a + b (原地写入第二参数), usec 超过 999999 时进位归一。
void timeradd(struct timeval *a, struct timeval *b)
{
	int sec;
	int usec;

	sec = a->tv_sec + b->tv_sec;
	usec = a->tv_usec + b->tv_usec;
	b->tv_sec = sec;
	b->tv_usec = usec;
	if (999999 < usec) {
		b->tv_sec = sec + 1;
		b->tv_usec = usec - 1000000;
	}
}

// === timercmp_gt === 原 FUN_0002a4c8 @ 0x0002a4c8
// 来源: reversed/cgminer/20_job_mining/FUN_0002a4c8.c | 分类: 20_job_mining
// 语义: timeval 比较 a > b (布尔): 先比 tv_sec, 相等再比 tv_usec。
int timercmp_gt(struct timeval *a, struct timeval *b)
{
	if (a->tv_sec == b->tv_sec)
		return b->tv_usec < a->tv_usec;
	if (a->tv_sec < b->tv_sec)
		return 0;
	return 1;
}

// === timercmp_lt === 原 FUN_0002a4fc @ 0x0002a4fc
// 来源: reversed/cgminer/20_job_mining/FUN_0002a4fc.c | 分类: 20_job_mining
// 语义: timeval 比较 a < b (布尔): 先比 tv_sec, 相等再比 tv_usec。
int timercmp_lt(struct timeval *a, struct timeval *b)
{
	if (a->tv_sec == b->tv_sec)
		return a->tv_usec < b->tv_usec;
	return a->tv_sec < b->tv_sec;
}

// === copy_timeval === 原 FUN_0002a530 @ 0x0002a530
// 来源: reversed/cgminer/20_job_mining/FUN_0002a530.c | 分类: 20_job_mining
// 语义: timeval 双字拷贝 dst <- src (8 字节; hashmeter 等处全局时刻复制)。
void copy_timeval(struct timeval *dst, const struct timeval *src)
{
	dst->tv_sec = src->tv_sec;
	dst->tv_usec = src->tv_usec;
}

// === cgtime === 原 FUN_0002a84c @ 0x0002a84c
// 来源: reversed/cgminer/20_job_mining/FUN_0002a84c.c | 分类: 20_job_mining
// 语义: gettimeofday(tv, NULL) 包装 (util.h cgtime)。
void cgtime(struct timeval *tv)
{
	gettimeofday(tv, NULL);
}

// === tdiff === 原 FUN_0002aa24 @ 0x0002aa24
// 来源: reversed/cgminer/20_job_mining/FUN_0002aa24.c | 分类: 20_job_mining
// 语义: 秒差 (浮点): (a.sec - b.sec) + (a.usec - b.usec)/1e6。
//       原函数经 VFP s0/s1 返回 double (Ghidra 仅渲染 r0)。
double tdiff(const struct timeval *a, const struct timeval *b)
{
	return (double)(a->tv_sec - b->tv_sec) +
	       (double)(a->tv_usec - b->tv_usec) / 1000000.0;
}

// === parse_xnsub === 原 FUN_0002aa68 @ 0x0002aa68
// 来源: reversed/cgminer/20_job_mining/FUN_0002aa68.c | 分类: 20_job_mining
// 语义: URL 中查 "#xnsub" 后缀: 命中且 pool->extranonce_subscribe(+0x25c) 未置位时,
//       置位并截断 URL 的 '#' 起后缀, 门控日志 "Pool %d extranonce subscribing enabled."。
//       原码把后缀复制到 16 字节栈缓冲再 strcmp (保持原样, 含越界风险)。
void parse_xnsub(void *pool, char *url)
{
	char *hash;
	char suffix[16];
	char msg[4096];
	int rc;

	hash = strchr(url, '#');
	if (hash != NULL && *(char *)((char *)pool + 0x25c) == '\0') {
		strcpy(suffix, hash);
		*hash = '\0';
		rc = strcmp(suffix, "#xnsub");
		if (rc == 0) {
			*(uint8_t *)((char *)pool + 0x25c) = 1;
			if ((DAT_00078e09 != '\0') &&
			    ((DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 6 < DAT_000766c4))) {
				snprintf(msg, 0x1000, "Pool %d extranonce subscribing enabled.",
				         *(int *)pool);
				applog(7, msg, 0);
			}
		}
	}
}

// === _cgsem_post === 原 FUN_0002f1bc @ 0x0002f1bc
// 来源: reversed/cgminer/20_job_mining/FUN_0002f1bc.c | 分类: 20_job_mining
// 语义: sem_post 包装: 失败打印 "Failed to sem_post errno=%d cgsem=0x%p in %s %s():%d"
//       (util.c) 后 quit(1)。
void _cgsem_post(sem_t *sem, const char *file, const char *fn, int line)
{
	int rc;

	rc = sem_post(sem);
	if (rc != 0) {
		char msg[4100];

		snprintf(msg, 0x1000, "Failed to sem_post errno=%d cgsem=0x%p in %s %s():%d",
		         errno, sem, file, fn, line);
		applog(3, msg, 1);
		quit(1);
	}
}

// === sha256_ctx_init === 原 FUN_000302d0 @ 0x000302d0
// 来源: reversed/cgminer/20_job_mining/FUN_000302d0.c | 分类: 20_job_mining
// 语义: SHA-256 上下文初始化: 标准初始哈希 H0 (8 字) 写入 ctx+0x88 起,
//       总长计数 (+0) 与缓冲字节数 (+4) 清 0。
//       ctx 布局: [0]=总长, [1]=缓冲字节数, +8 起 0x40 缓冲, +0x88 起 H[8]。
void sha256_ctx_init(uint32_t *ctx)
{
	int i;

	for (i = 0; i < 8; i++)
		ctx[0x22 + i] = sha256_h0[i];   /* +0x88 = H[0..7] */
	ctx[1] = 0;
	ctx[0] = 0;
}

// === sha256_ctx_update === 原 FUN_00030304 @ 0x00030304
// 来源: reversed/cgminer/20_job_mining/FUN_00030304.c | 分类: 20_job_mining
// 语义: SHA-256 增量喂入: 先填满缓冲 (+8 起 0x40 字节), 满一块即压缩; 剩余数据
//       整块 (0x40 对齐) 直接压缩, 尾部不足一块的回填缓冲; 总长计数 += 处理块数×64。
void sha256_ctx_update(uint32_t *ctx, const void *data, uint32_t len)
{
	uint32_t fill;
	uint32_t total;
	uint32_t remain;
	uint32_t blocks;

	fill = 0x40 - ctx[1];
	if (len < fill)
		fill = len;
	memcpy((char *)ctx + ctx[1] + 8, data, fill);
	total = len + ctx[1];
	if (total < 0x40) {
		ctx[1] = total;
		return;
	}
	remain = (len - fill) & 0x3f;
	blocks = (len - fill) >> 6;
	sha256_compress(ctx, ctx + 2, 1);                       /* 压缩缓冲中凑满的一块 */
	sha256_compress(ctx, (const char *)data + fill, blocks); /* 新数据整块 */
	memcpy(ctx + 2, (const char *)data + fill + blocks * 0x40, remain);
	ctx[1] = remain;
	ctx[0] = ctx[0] + (blocks + 1) * 0x40;
}

// === sha256_buffer === 原 FUN_0003049c @ 0x0003049c
// 来源: reversed/cgminer/20_job_mining/FUN_0003049c.c | 分类: 20_job_mining
// 语义: 一次性 SHA-256: 栈上构造 42 字上下文 (H0 写入 +0x88, 计数清 0 — 与
//       sha256_ctx_init 等价的内联展开), update 后 final 输出到 out。
void sha256_buffer(const void *data, uint32_t len, void *out)
{
	uint32_t ctx[42];   /* 0xa8 字节: [0]=总长 [1]=缓冲字节 +8..0x47 缓冲 +0x88..0xa7 H */

	memcpy(&ctx[0x22], sha256_h0, sizeof(sha256_h0));
	ctx[0] = 0;
	ctx[1] = 0;
	sha256_ctx_update(ctx, data, len);
	sha256_ctx_final(ctx, out);
}

// === add_uint_buf === 原 FUN_00032ef4 @ 0x00032ef4
// 来源: reversed/cgminer/20_job_mining/FUN_00032ef4.c | 分类: 20_job_mining
// 语义: api 数据挂接包装 — 类型 7 (4 字节无符号, 上游 API_UINT) 挂接节点;
//       Ghidra 把尾调渲染为 void, 实际 r0 直通 api_add_data_full 的返回 root。
void *add_uint_buf(void *root, char *name, void *data, int copied)
{
	return api_add_data_full(root, name, 7 /* 4B 无符号 */, data, copied);
}

// === add_uint64_buf === 原 FUN_00032f60 @ 0x00032f60
// 来源: reversed/cgminer/20_job_mining/FUN_00032f60.c | 分类: 20_job_mining
// 语义: api 数据挂接包装 — 类型 10 (8 字节无符号, API_UINT64)。
void *add_uint64_buf(void *root, char *name, void *data, int copied)
{
	return api_add_data_full(root, name, 10 /* 8B 无符号 */, data, copied);
}

// === add_int64_buf === 原 FUN_00032f84 @ 0x00032f84
// 来源: reversed/cgminer/20_job_mining/FUN_00032f84.c | 分类: 20_job_mining
// 语义: api 数据挂接包装 — 类型 11 (8 字节有符号, API_INT64)。
void *add_int64_buf(void *root, char *name, void *data, int copied)
{
	return api_add_data_full(root, name, 11 /* 8B 有符号 */, data, copied);
}

// === add_mhtotal_buf === 原 FUN_00035520 @ 0x00035520
// 来源: reversed/cgminer/20_job_mining/FUN_00035520.c | 分类: 20_job_mining
// 语义: api 数据挂接包装 — 类型 0x12 (API_MHTOTAL, 上游为 double)。
void *add_mhtotal_buf(void *root, char *name, void *data, int copied)
{
	return api_add_data_full(root, name, 0x12 /* API_MHTOTAL */, data, copied);
}

// === add_utility_buf === 原 FUN_00035568 @ 0x00035568
// 来源: reversed/cgminer/20_job_mining/FUN_00035568.c | 分类: 20_job_mining
// 语义: api 数据挂接包装 — 类型 0x14 (API_UTILITY, 上游为 double)。
void *add_utility_buf(void *root, char *name, void *data, int copied)
{
	return api_add_data_full(root, name, 0x14 /* API_UTILITY */, data, copied);
}

// === add_diff_buf === 原 FUN_000355f8 @ 0x000355f8
// 来源: reversed/cgminer/20_job_mining/FUN_000355f8.c | 分类: 20_job_mining
// 语义: api 数据挂接包装 — 类型 0x18 (API_DIFF, 上游为 double)。
void *add_diff_buf(void *root, char *name, void *data, int copied)
{
	return api_add_data_full(root, name, 0x18 /* API_DIFF */, data, copied);
}

// === add_percent_buf === 原 FUN_000357a4 @ 0x000357a4
// 来源: reversed/cgminer/20_job_mining/FUN_000357a4.c | 分类: 20_job_mining
// 语义: api 数据挂接包装 — 类型 0x19 (API_PERCENT, 上游为 float)。
void *add_percent_buf(void *root, char *name, void *data, int copied)
{
	return api_add_data_full(root, name, 0x19 /* API_PERCENT */, data, copied);
}

// === _applog === 原 FUN_000389f4 @ 0x000389f4
// 来源: reversed/cgminer/20_job_mining/FUN_000389f4.c | 分类: 20_job_mining
// 语义: 日志入口包装 (curses 联动): opt_quiet (DAT_0007bc78) 且优先级非 3 直接丢弃;
//       第 4 参非 0 时 trylock curses 锁 (结果忽略) 后立即解锁 (+0x26 行号报错点);
//       DAT_00075dd0 关闭或 log_curses_only 失败时走 my_log_curses_lock + printf
//       (两段串直接拼接 + 20 空格回车清行尾) + 解锁 (+0x31 行号)。
void _applog(int prio, char *s1, char *s2, int flag)
{
	int rc;

	if ((DAT_0007bc78 != '\0') && prio != 3)
		return;
	if (flag != 0) {
		pthread_mutex_trylock(&DAT_0007b7a8);   /* 原码忽略 trylock 结果 */
		my_log_curses_unlock(0x26);
		(*DAT_00075ed8)();
	}
	if (DAT_00075dd0 == '\0' ||
	    (rc = log_curses_only(prio, s1, s2)) == 0) {
		my_log_curses_lock();
		printf("%s%s%s", s1, s2, "                    \n");
		my_log_curses_unlock(0x31);
		(*DAT_00075ed8)();
	}
}

// === set_nonce_shift_chain === 原 FUN_000431ac @ 0x000431ac
// 来源: reversed/cgminer/20_job_mining/FUN_000431ac.c | 分类: 20_job_mining
// 语义: 设置链级 nonce 位移寄存器: 全局 DAT_00076b50 记录 value<<27, 并构 0x50 寄存器
//       链广播写帧 (55 AA 51 09 <chip=0> <reg=0x50> <BE32 值> <crc5>) 经 uart_send
//       下发后延时 10ms (门控: debug 日志 "chainid%d %s: nonce shift = %08x" 打印原始值)。
void set_nonce_shift_chain(int chainid, uint32_t value)
{
	uint8_t frame[11] = {0};   /* 原 4+4+2+1 四段零初始化 */
	char msg[4096];

	if ((DAT_00078e09 != '\0') &&
	    ((DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 6 < DAT_000766c4))) {
		snprintf(msg, 0x1000, "chainid%d %s: nonce shift = %08x",
		         chainid, "set_nonce_shift_chain", value);
		applog(7, msg, 0);
	}
	DAT_00076b50 = value << 27;
	makeup_cmd_frame(frame, 1, 0, BM1746_REG_NONCE_SHIFT /* 0x50 */, value << 27);
	uart_send(chainid, frame, 0xb);
	cgsleep_ms(10);
}

// === hashtable_iter_serial === 原 FUN_00057f3c @ 0x00057f3c
// 来源: reversed/cgminer/20_job_mining/FUN_00057f3c.c | 分类: 20_job_mining
// 语义: 返回哈希表迭代节点 +0xc 处的序号字段 (jansson 内部节点布局, 语义待定)。
int hashtable_iter_serial(void *iter)
{
	return *(int *)((char *)iter + 0xc);
}

// === json_object_size === 原 FUN_00059e04 @ 0x00059e04
// 来源: reversed/cgminer/20_job_mining/FUN_00059e04.c | 分类: 20_job_mining
// 语义: jansson 对象元素数: NULL 或非 JSON_OBJECT 返回 0, 否则取 obj+8 计数。
int json_object_size(json_t *obj)
{
	int size;

	if (obj == NULL)
		return 0;
	if (obj->type == 0)
		size = *(int *)((char *)obj + 8);
	else
		size = 0;
	return size;
}

// === json_object_iter === 原 FUN_00059e9c @ 0x00059e9c
// 来源: reversed/cgminer/20_job_mining/FUN_00059e9c.c | 分类: 20_job_mining
// 语义: 对象迭代起始: 非 JSON_OBJECT 返回 NULL; 返回内嵌循环链表首节点 (obj+0x18),
//       为哨兵 (obj+0x14) 时表示空对象返回 NULL。
void *json_object_iter(json_t *obj)
{
	void *iter;

	if (obj == NULL || obj->type != 0)
		return NULL;
	iter = *(void **)((char *)obj + 0x18);
	if (iter == (char *)obj + 0x14)
		iter = NULL;
	return iter;
}

// === json_object_iter_next === 原 FUN_00059ee8 @ 0x00059ee8
// 来源: reversed/cgminer/20_job_mining/FUN_00059ee8.c | 分类: 20_job_mining
// 语义: 对象迭代步进: 节点 +4 的链表后继, 等于哨兵 (obj+0x14) 时返回 NULL。
void *json_object_iter_next(json_t *obj, void *iter)
{
	void *next;

	if (obj == NULL || obj->type != 0 || iter == NULL)
		return NULL;
	next = *(void **)((char *)iter + 4);
	if (next == (char *)obj + 0x14)
		next = NULL;
	return next;
}

// === json_object_iter_key === 原 FUN_00059f14 @ 0x00059f14
// 来源: reversed/cgminer/20_job_mining/FUN_00059f14.c | 分类: 20_job_mining
// 语义: 迭代节点键: 节点内联存储于 +0x10 起 (NULL 迭代返回 NULL)。
const char *json_object_iter_key(void *iter)
{
	if (iter == 0)
		return NULL;
	return (const char *)iter + 0x10;
}

// === json_object_iter_value === 原 FUN_00059f20 @ 0x00059f20
// 来源: reversed/cgminer/20_job_mining/FUN_00059f20.c | 分类: 20_job_mining
// 语义: 迭代节点值: 节点 +8 处的 json_t* (NULL 迭代返回 NULL)。
json_t *json_object_iter_value(void *iter)
{
	if (iter == 0)
		return NULL;
	return *(json_t **)((char *)iter + 8);
}

// === json_array_get === 原 FUN_0005a008 @ 0x0005a008
// 来源: reversed/cgminer/20_job_mining/FUN_0005a008.c | 分类: 20_job_mining
// 语义: 数组元素访问: 非空且为 JSON_ARRAY 且下标 < +0xc 长度时返回 +0x10 起的
//       元素指针数组第 index 项, 否则 NULL。
json_t *json_array_get(json_t *array, size_t index)
{
	if (array == NULL)
		return NULL;
	if (array->type == 1 && index < *(uint32_t *)((char *)array + 0xc))
		return *(json_t **)(*(void **)((char *)array + 0x10) + index * 4);
	return NULL;
}

// === json_string_value === 原 FUN_0005a178 @ 0x0005a178
// 来源: reversed/cgminer/20_job_mining/FUN_0005a178.c | 分类: 20_job_mining
// 语义: 字符串值: 类型为 JSON_STRING 时返回 obj+8 的字符串指针, 否则 NULL。
char *json_string_value(json_t *value)
{
	if (value == NULL)
		return NULL;
	if (value->type == 2)
		return *(char **)((char *)value + 8);
	return NULL;
}

// === json_number_value === 原 FUN_0005a378 @ 0x0005a378
// 来源: reversed/cgminer/20_job_mining/FUN_0005a378.c | 分类: 20_job_mining
// 语义: 数值取值: JSON_INTEGER(3) 经 __floatdidf 把 obj+8 的 int64 转 double;
//       JSON_REAL(4) 直接返回 obj+8 的 double; 其余/NULL 返回 0.0。
//       原函数经 VFP d0 返回 double, Ghidra 仅渲染 r0。
double json_number_value(json_t *value)
{
	if (value != NULL) {
		if (value->type == 3) {
			uint64_t i64 = (uint32_t)((int *)value)[2] |
			               ((uint64_t)(uint32_t)((int *)value)[3] << 32);
			return __floatdidf(i64);   /* int64 → double */
		}
		if (value->type == 4)
			return *(double *)((char *)value + 8);
	}
	return 0.0;
}

// === add_opt === 原 FUN_0005b9a8 @ 0x0005b9a8
// 来源: reversed/cgminer/20_job_mining/FUN_0005b9a8.c | 分类: 20_job_mining
// 语义: 动态选项表追加: realloc 扩容 (每项 7 字/0x1c 字节) 后整项拷贝, opt_count++。
void add_opt(void *entry)
{
	uint32_t *dst;
	uint32_t *src = entry;
	int i;

	DAT_0009d77c = realloc(DAT_0009d77c, (DAT_0009d774 + 1) * 0x1c);
	dst = (uint32_t *)DAT_0009d77c + DAT_0009d774 * 7;
	for (i = 0; i < 7; i++)
		dst[i] = src[i];
	DAT_0009d774++;
}

// === opt_parse === 原 FUN_0005bc84 @ 0x0005bc84
// 来源: reversed/cgminer/20_job_mining/FUN_0005bc84.c | 分类: 20_job_mining
// 语义: ccan/opt 命令行解析入口: 记录 opt_argv0 = argv[0] (DAT_0009d780) 后循环
//       parse_one 直到非 1; parse_one 返回 0 视为成功返回 1, 其余返回 0。
int opt_parse(int *argc, char **argv, void (*errlog)(const char *fmt, ...))
{
	unsigned int ret;
	unsigned int offset;

	DAT_0009d780 = argv[0];
	offset = 0;
	do {
		ret = parse_one(argc, argv, &offset, errlog);
	} while (ret == 1);
	if (ret == 0)
		return 1;
	return 0;
}

// === __aeabi_uldivmod === 原 FUN_0005cbc4 @ 0x0005cbc4
// 来源: reversed/cgminer/20_job_mining/FUN_0005cbc4.c | 分类: 20_job_mining
// 语义: ARM EABI 64 位无符号除法取余: 商在 {r0,r1}、余数在 {r2,r3} 返回,
//       故以双 uint64 结构体按值转写 (AAPCS 布局一致)。除数为 0 时 raise(SIGFPE=8)
//       并直接返回 (原码未清返回值)。实际求商/余委托 __udivmoddi4。
typedef struct {
	unsigned long long quot;   /* r0,r1 */
	unsigned long long rem;    /* r2,r3 */
} __aeabi_uldivmod_ret_t;

__aeabi_uldivmod_ret_t __aeabi_uldivmod(unsigned long long n, unsigned long long d)
{
	__aeabi_uldivmod_ret_t res;

	if (d == 0) {
		raise(8);   /* SIGFPE */
		return res; /* 内容未定 (原码 raise 后直接返回) */
	}
	res.quot = __udivmoddi4(n, d, &res.rem);
	return res;
}

// === __fixdfdi === 原 FUN_0005cc10 @ 0x0005cc10
// 来源: reversed/cgminer/20_job_mining/FUN_0005cc10.c | 分类: 20_job_mining
// 语义: double → 有符号 int64 截断: 负数先按位翻转符号位 (即取负) 走无符号
//       路径再整体取负; 非负直接走无符号路径。原码为软浮点 ABI (double 经 r0/r1)。
long long __fixdfdi(double a)
{
	long long u;

	if (a < 0.0) {
		u = __fixunsdfdi(-a);
		return -u;
	}
	return (long long)__fixunsdfdi(a);
}

// === __fixunsdfdi === 原 FUN_0005cc48 @ 0x0005cc48
// 来源: reversed/cgminer/20_job_mining/FUN_0005cc48.c | 分类: 20_job_mining
// 语义: double → 无符号 int64 截断 (VFP 路径): 高 32 位 = (uint32)(a * 2^-32),
//       低 32 位 = (uint32)(a - (double)高32 * 2^32); VectorFloatToUnsigned 即
//       FCVTZU 截断, 等价 C 强制转换。原码为软浮点 ABI。
unsigned long long __fixunsdfdi(double a)
{
	uint32_t hi;
	double hi_d;
	uint32_t lo;

	hi = (uint32_t)(a * 2.3283064365386963e-10);   /* a / 2^32 截断 */
	hi_d = (double)hi;
	lo = (uint32_t)(a - hi_d * 4294967296.0);
	return ((unsigned long long)hi << 32) | lo;
}

// === __udivmoddi4 === 原 FUN_0005cccc @ 0x0005cccc
// 来源: reversed/cgminer/20_job_mining/FUN_0005cccc.c | 分类: 20_job_mining
// 语义: 64 位无符号除法取余核心: q = __udivdi3(n, d), 余数 = n - q*d 写回 *rp,
//       返回商。原码的 64 位减法借位展开 (hi - hi - borrow) 与 C 的 64 位减法等价。
unsigned long long __udivmoddi4(unsigned long long n, unsigned long long d, unsigned long long *rp)
{
	unsigned long long q;
	unsigned long long prod;

	q = __udivdi3(n, d);
	prod = q * d;
	*rp = n - prod;
	return q;
}







/*
 * ---- 覆盖率表 (batch=job_b6, 82/82) ----
 * 函数                         原地址       反编译行数  横幅行  一句话语义
 * wr_lock_error                0x_0000b480   16
       180  rwlock 写锁失败报 "WTF WRLOCK ERROR ON LOCK!" 后 quit(1)
 * rw_unlock_error              0x_0000b630   16
       193  rwlock 解锁失败报 "WTF RWLOCK ERROR ON UNLOCK!" 后 quit(1)
 * rd_lock_error                0x_0000b69c   16
       206  rwlock 读锁失败报 "WTF RDLOCK ERROR ON LOCK!" 后 quit(1)
 * new_nonce                    0x_0000f7e8   27
       219  cgpu->last_nonce 重复检测, 重复记日志返回 0, 新 nonce 登记返回 1
 * set_thr_busy                 0x_0000fc24   17
       245  busy 字节(+0x3d)清 0 + 记 thr 时刻 + 清 cgpu+0x60/置 cgpu+0xf0
 * set_thr_idle                 0x_0000fc58   17
       262  busy 字节(+0x3d)置 1 + 其余副作用同 set_thr_busy
 * adj_fwidth                   0x_00011514   16
       278  列宽自适应: log10(f)+1 (截断) 超过 *width 时增 1
 * adj_width                    0x_00011548   18
       290  adj_fwidth 整数版 (log10((double)val)+1), 内联展开
 * benchfile_dspwork            0x_00011814   26
       302  BENCHFILE 日志: nonce 字节反序 + work 头 128B 十六进制串
 * get_datestamp                0x_00015264   15
       327  time_t → "[YYYY-MM-DD hh:mm:ss]" (本地时间)
 * clean_work                   0x_00015648   13
       342  释放 work 4 个动态字段 (+0x6f0/704/710/718) 并整块清零 (0x7b0)
 * free_work                    0x_00015780   10
       355  clean_work 后释放 work 本体
 * decay_time                   0x_00015798   18
       364  指数衰减滑动平均 (hashmeter 滚动算力), tdiff<=0 不动
 * cgpu_runtime                 0x_000157ec   24
       380  设备运行秒数: 未启动=1.0, tdiff 结果 <1.0 归 0
 * tsince_restart               0x_00015840   12
       401  距全局重启时刻 DAT_00076f04 的浮点秒差
 * tsince_update                0x_00015868   12
       413  距全局时刻 DAT_00076f0c 的浮点秒差 (语义待定)
 * test_nonce                   0x_0001654c   23
       424  写 nonce 后按 sha256d/scrypt 重算 hash, hash[7] 阈值快检 (0/0xffff)
 * __find_work_bymidstate       0x_00016a80   25
       446  排队 work 链上按 midstate(+0x80)+data(+off) 查找
 * reinit_device                0x_00016c88   12
       466  cgpu+0x20 != 1 时回调 drv(+4)->fn(+0x10)
 * restart_threads              0x_00017210   16
       476  记全局重启时刻并创建 restart_thread, 失败报错后退出
 * mutex_init                   0x_00017990   15
       491  pthread_mutex_init 包装, 失败走 mutex_init_fail_cgminer
 * rwlock_init                  0x_000179bc   15
       504  pthread_rwlock_init 包装, 失败走 rwlock_init_fail_cgminer
 * cglock_init                  0x_000179e8   19
       517  cglock 组合锁初始化 (mutex@+0 + rwlock@+0x18)
 * curses_active_locked         0x_00017fe4   22
       534  lock_curses 后查 use_curses, 非 0 直接返回该值 (不解锁)
 * total_staged                 0x_000188a8   26
       554  stgd_lock 下读 DAT_00077050+0x728 表结构 +0xc 计数
 * inc_hw_errors                0x_000189a4   30
       577  硬件错误计数 (cgpu->hw_errors+全局 DAT_0007b8a4) + drv->fn(+0x54) 回调
 * wake_gws                     0x_00019840   20
       609  stgd_lock 下 signal gws_cond 唤醒取活线程
 * rw_unlock                    0x_00019bbc   15
       626  pthread_rwlock_unlock 包装 + 让步钩子
 * work_completed               0x_0001a0b8   21
       639  写锁出队 __work_completed 后 free_work 释放
 * take_queued_work_bymidstate  0x_0001a134   25
       658  写锁按 midstate 查找并即时出队, 返回 work
 * total_work_inc               0x_0001a1e8   30
       682  control_lock 双锁下 total_work 自增并返回旧值
 * make_clone                   0x_0001a44c   12
       709  make_work + copy_work(dst, src, nonce2) 克隆 work
 * clone_work_backdated         0x_0001a474   16
       722  回溯克隆: clone 标志置 1 + tv_stamp 取现在 + tv_gen-1 [低置信]
 * _rd_lock                     0x_0001ab24   15
       740  pthread_rwlock_rdlock 包装 (失败 rd_lock_error, 无让步钩子)
 * clone_queued_work_bymidstate 0x_0001ab4c   27
       753  读锁按 midstate 查找 + make_clone(work,0) 克隆返回
 * find_queued_work_byid        0x_0001b074   29
       779  读锁按 work id (+0x724) 线性查排队链
 * get_devices                  0x_0001b83c   21
       805  devices_lock 读锁下按索引取 devices 数组槽位
 * find_queued_work_bymidstate  0x_0001d9b4   23
       825  读锁按 midstate 查找 (不出队), 返回 work
 * take_pending_work            0x_00024708   26
       846  取暂存 work (+0x164): 合法则登记 workid 表, 过期则丢弃并唤醒 [低置信]
 * clone_and_submit_work        0x_00025e48   10
       877  make_clone(work,0) 后 submit_work_async 提交克隆 [低置信]
 * submit_nonce                 0x_00025f48   20
       889  nonce 上报: 重复/校验失败计 HW 错, 通过提交 + BENCHFILE 补记
 * noop_hook                    0x_00028ae0    8
       910  让步钩子 DAT_00075ed8 的 ELF 初值, 恒返回 0
 * json_array_string_nc         0x_00029294   18
       919  数组元素字符串校验访问 (不拷贝, 非 STRING 返 NULL)
 * json_array_string            0x_000292f4   15
       935  json_array_string_nc 校验后 strdup 拷贝返回
 * _rw_unlock                   0x_0002930c   20
       949  pthread_rwlock_unlock 包装 (util.c 变体, 失败报错 quit)
 * tq_new                       0x_0002a168   17
       968  calloc 0x58 任务队列 (链表头自环 + mutex/cond 初始化)
 * timersub                     0x_0002a448   21
       986  timeval 差 b = a-b (usec 借位 sec-1/usec+1e6)
 * timeradd                     0x_0002a47c   21
      1004  timeval 和 b = a+b (usec 进位 sec+1/usec-1e6)
 * timercmp_gt                  0x_0002a4c8   22
      1022  timeval 比较 a > b (先 sec 后 usec)
 * timercmp_lt                  0x_0002a4fc   11
      1034  timeval 比较 a < b (先 sec 后 usec)
 * copy_timeval                 0x_0002a530   13
      1044  timeval 双字拷贝 dst <- src
 * cgtime                       0x_0002a84c    9
      1053  gettimeofday(tv, NULL) 包装
 * tdiff                        0x_0002aa24   14
      1061  浮点秒差 (sec 差 + usec 差/1e6), 经 VFP 返回 double
 * parse_xnsub                  0x_0002aa68   24
      1071  URL "#xnsub" 后缀: extranonce_subscribe(+0x25c) 置位并截断 URL
 * _cgsem_post                  0x_0002f1bc   20
      1100  sem_post 包装, 失败报 "Failed to sem_post ..." 后 quit(1)
 * sha256_ctx_init              0x_000302d0   20
      1119  SHA-256 上下文初始化 (H0 写 ctx+0x88, 计数清 0)
 * sha256_ctx_update            0x_00030304   30
      1134  SHA-256 增量喂入 (缓冲凑满压缩/整块直压/尾块回填)
 * sha256_buffer                0x_0003049c   26
      1163  一次性 SHA-256 (栈上 42 字上下文 + update + final)
 * add_uint_buf                 0x_00032ef4    9
      1178  api 数据挂接类型 7 (4B 无符号) — r0 直通 root
 * add_uint64_buf               0x_00032f60    9
      1187  api 数据挂接类型 10 (8B 无符号) — r0 直通 root
 * add_int64_buf                0x_00032f84    9
      1195  api 数据挂接类型 11 (8B 有符号) — r0 直通 root
 * add_mhtotal_buf              0x_00035520    9
      1203  api 数据挂接类型 0x12 (MHTOTAL) — r0 直通 root
 * add_utility_buf              0x_00035568    9
      1211  api 数据挂接类型 0x14 (UTILITY) — r0 直通 root
 * add_diff_buf                 0x_000355f8    9
      1219  api 数据挂接类型 0x18 (DIFF) — r0 直通 root
 * add_percent_buf              0x_000357a4    9
      1227  api 数据挂接类型 0x19 (PERCENT) — r0 直通 root
 * _applog                      0x_000389f4   25
      1235  日志入口: quiet 门控 + curses trylock/解锁联动 + printf 拼接清行尾
 * set_nonce_shift_chain        0x_000431ac   28
      1261  下发 0x50 nonce 位移寄存器链广播写帧 (值<<27) + 延时 10ms
 * hashtable_iter_serial        0x_00057f3c    8
      1283  哈希表节点 +0xc 序号字段读取 (语义待定)
 * json_object_size             0x_00059e04   19
      1291  对象元素数 (NULL/非 JSON_OBJECT 返回 0)
 * json_object_iter             0x_00059e9c   17
      1307  对象迭代起始 (obj+0x18 首节点, 哨兵 obj+0x14 为空)
 * json_object_iter_next        0x_00059ee8   17
      1323  迭代步进 (节点+4 后继, 等于哨兵返回 NULL)
 * json_object_iter_key         0x_00059f14   11
      1338  迭代节点键 (+0x10 内联存储)
 * json_object_iter_value       0x_00059f20   11
      1348  迭代节点值 (+8 json_t*)
 * json_array_get               0x_0005a008   14
      1358  数组元素访问 (越界/非数组返回 NULL)
 * json_string_value            0x_0005a178   19
      1371  字符串值 (obj+8 指针, 非 STRING 返 NULL)
 * json_number_value            0x_0005a378   19
      1383  数值取值 (INTEGER 经 __floatdidf, REAL 直读, 其余 0.0)
 * add_opt                      0x_0005b9a8   28
      1402  动态选项表 realloc 追加一项 (7 字/0x1c 字节) 并计数
 * opt_parse                    0x_0005bc84   21
      1418  命令行解析循环 (parse_one 驱动, 记录 opt_argv0)
 * __aeabi_uldivmod             0x_0005cbc4   13
      1437  EABI 64 位除法取余 (商 r0-r1/余 r2-r3, 除 0 SIGFPE)
 * __fixdfdi                    0x_0005cc10   15
      1459  double→int64 截断 (负数翻转符号位走无符号路径再取负)
 * __fixunsdfdi                 0x_0005cc48   16
      1474  double→uint64 截断 (高字乘 2^-32, 低字差值)
 * __udivmoddi4                 0x_0005cccc   16
      1491  64 位除法取余核心 (__udivdi3 + 余数 n-q*d 回写 *rp)
 */
