/*
 * cgminer 20_job_mining 任务层第 3 段 (批次 job_b3) — BM1746 固件语义转写
 * =========================================================
 * 原始二进制: bm1746/cgminer_z15_bm1746.elf (ARM32, SD 镜像 cgminer-z15, branch "bm1746")
 * 本文件汇集挖矿任务层的哈希队列/统计/提交/线程循环等 9 个函数。
 *
 * 函数清单 (命名事实源 rewritten/name_map/cgminer.tsv):
 *   hash_pop                原 FUN_000193b4  — 从哈希队列取出待算工作
 *   hashmeter               原 FUN_0001bba4  — 1m/5m/15m/avg 算力速率统计打印
 *   print_summary           原 FUN_0001c668  — 运行汇总统计输出
 *   hash_push               原 FUN_0001f17c  — 将工作压入哈希队列
 *   miner_thread_work_loop  原 FUN_00024db0  — 矿机线程工作循环体 (GCC 拆分冷路径)
 *   submit_work_async       原 FUN_000256a4  — 异步提交 share
 *   log_work                原 FUN_0002f934  — 工作日志记录
 *   do_dump                 原 FUN_00057178  — 调试转储
 *   __udivdi3               原 FUN_0005d160  — 64 位无符号除法运行时助手
 *
 * 转写约定 (rewritten/CONVENTIONS.md):
 *   - 被调函数名一律取自 name_map; 未知 DAT_xxx 全局保留原名 + 注释。
 *   - 日志字符串原样保留; Ghidra 伪变量 (extraout_, uVar*, local_* 等) 已消除。
 */

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#include "bm1746_regs.h"
#include "common_types.h"
#include "fpga_axi.h"

/* ====================================================================
 * 外部全局引用 (DAT_ 名按 CONVENTIONS §4 保留, 语义取自调用点证据)
 * ==================================================================== */
extern pthread_mutex_t *DAT_0007704c;  /* stgd_lock: 待算工作队列互斥锁 */
extern int           DAT_00077050;     /* staged_work: 待算工作 uthash 表头 (首个 work*) */
extern uint8_t       DAT_00077055;     /* work_update 请求标志 (hash_pop 消费后清零) */
extern uint8_t       DAT_00077056;     /* hash_pop 弹出进行标志 (置 1) */
extern int           DAT_00077058;     /* total_queued: 已排队未弹出的工作计数 */
extern int           DAT_00075dd8;     /* work_update 到达计数器 (与 DAT_00075ddc 上限配对) */
extern int           DAT_00075ddc;     /* work_update 计数上限 */
extern int64_t       DAT_0007b1a0;     /* 条件变量: 弹出/入队唤醒 (std_update_cond 类) */
extern int64_t       DAT_0007b620;     /* 线程队列基址: gws_cond = (pthread_cond_t*)(DAT_0007b620+0x28) */
extern uint8_t       DAT_0007b4c8;     /* "等待工作" 已告警标志 (恢复时清零) */
extern uint8_t       DAT_0007ad48;     /* applog 门控 A: 非 0 → applog 走 syslog(prio|0x80) */
extern uint8_t       DAT_00078e08;     /* applog 门控 B = opt_verbose */
extern int           DAT_000766c4;     /* applog 门控 C: 日志级别阈值 */
extern uint8_t       DAT_00078e09;     /* applog 门控 D (work/log 类输出开关) */
extern time_t        DAT_0007b948;     /* 最近一次 hash_pop 弹出时刻 */
extern void        (*DAT_00075ed8)(void); /* 全局函数指针: ELF 初值 noop_hook (FUN_00028ae0, 恒 0) */
extern struct timeval DAT_00077148;   /* hashmeter 全局 "now" 时间戳 */
extern struct timeval DAT_000770d8;    /* scantime 周期计时起点 (tdiff 参照) */
extern int           DAT_00077150;     /* 上次满足 scantime 全局更新时的 now.tv_sec */
extern uint8_t       DAT_00077154;     /* 相邻状态字节 (每次全局更新与 DAT_00077155 异或翻转) */
extern uint8_t       DAT_00077155;     /* 异或配对字节 */
extern int           DAT_00075dcc;     /* total_secs: scantime 统计窗口 (秒) */
extern pthread_mutex_t DAT_0007b384;   /* hashmeter/统计锁 (互斥锁对象, 取 & 使用) */
extern int           DAT_0007bb6c;     /* total_devices: 设备(cgpu)总数 */
extern double        DAT_0007acc0;     /* 全局累计哈希量 (total_hashes) */
extern double        DAT_000770e0[12]; /* hashmeter 12 窗口哈希量滚动数组 */
extern int           DAT_00077140;     /* 滚动数组当前写入槽位 (0..11, 写前自增) */
extern double        DAT_0007bb10;     /* 当前 scantime 窗速率 EMA ("(%ds)" 槽) */
extern double        DAT_0007bc98;     /* 1 分钟速率 EMA (tau=60) */
extern double        DAT_0007bc88;     /* 5 分钟速率 EMA (tau=300) */
extern double        DAT_0007b2f8;     /* 15 分钟速率 EMA (tau=700) */
extern int64_t       DAT_0007bad0;     /* llround(DAT_0007bb10)*1000000 (int64) */
extern double        DAT_000755f8;     /* 运行总秒数 (tdiff(now, DAT_000770d0)) */
extern double        DAT_00075de8;     /* last_total_secs: 防时钟跳变参照 (<=86400s 滚动) */
extern struct timeval DAT_000770d0;    /* 程序运行计时起点 (总秒数参照) */
extern double        DAT_00077158;     /* DAT_0007bb10/1000 (供外部读取的毫开/秒值) */
extern char          DAT_00077160[256];/* 全局 hashmeter 状态行 "(%ds):..(avg):..h/s" */
extern uint8_t       DAT_00077018;     /* 输出模式标志: 0 → printf("\r") 单行刷新 */

/* ---- 本文件调用的外部函数 (命名源 rewritten/name_map/cgminer.tsv) ---- */
extern void  mutex_lock_fail_cgminer(const char *fn, int line);     /* FUN_0000b414 */
extern void  mutex_unlock_fail_cgminer(const char *fn, int line);   /* FUN_0000b5c4 */
extern void  cgtime(struct timeval *tv);                            /* FUN_0002a84c */
extern double tdiff(const struct timeval *a, const struct timeval *b); /* FUN_0002aa24 */
extern void  copy_timeval(void *dst, const void *src);              /* FUN_0002a530: dst <- src 8 字节 */
extern void  decay_time(double *f, double fadd, double fsecs, double tau); /* FUN_00015798
              * ARM 硬浮点 ABI: r0=f, d0=fadd, d1=fsecs, d2=tau;
              * fsecs>0 时: alpha = 1-exp(-fsecs/tau), *f = (*f + alpha*(fadd/fsecs))/(alpha+1) */
extern void  suffix_string(uint64_t val, char *buf, size_t bufsiz, int sigdigits); /* FUN_00011580 */
extern void  applog(uint32_t prio, char *buf, ...);  /* FUN_00038730 */
extern void  get_statline(char *buf, size_t bufsiz, void *cgpu);    /* FUN_00017744 */
extern void *get_thread(int thr_id);                                /* FUN_0001b124: 返回 thr_info*, +0x24= cgpu */
extern void  zero_stats(void);                                      /* FUN_0001b8b4 */

/* ==================================================================== */

/* === hash_pop === 原 FUN_000193b4 @ 0x000193b4
 * 来源: reversed/cgminer/20_job_mining/FUN_000193b4.c | 分类: 20_job_mining
 * 语义: 从待算工作 uthash 表 (staged_work) 弹出一个 work 并返回其指针。
 *       wait=0: 不等待、不弹出, 直接返回 0; wait!=0: 表空时以 gws_cond 10 秒
 *       定时等待, 期间用 LOG_WARNING 提示 "Waiting for work..." (仅一次)。
 *       全程持 stgd_lock (DAT_0007704c); 弹出后唤醒 DAT_0007b1a0 与 gws_cond,
 *       非 clone 且有 rolltime 的工作使 total_queued-- (DAT_00077058)。
 * work 结构偏移 (uthash 句柄 hh 位于 work+0x728):
 *   +0x6e1 char = work->clone;   +0x6e4 int = work->rolltime;
 *   hh.tbl=+0x728, hh.prev=+0x72c, hh.next=+0x730,
 *   hh.hh_prev=+0x734, hh.hh_next=+0x738, hh.hashv=+0x744。
 * 表结构 (UT_hash_table): buckets+0, num_buckets+4, num_items+0xc,
 *   tail+0x10, hho+0x14; 桶 (UT_hash_bucket) 12 字节: head+0, count+4。
 */
int hash_pop(int wait)
{
	int rc;
	int *hh_tbl;          /* work->hh.tbl (UT_hash_table*) */
	int work;             /* 弹出目标 work 指针 (int 形式, 原码按 int 算术) */
	int prev_item;        /* work->hh.prev */
	int head;             /* DAT_00077050 的影子 (删除路径内使用) */
	int bucket_off;       /* 桶字节偏移 = (num_buckets-1 & hashv) * 12 */
	int bucket;           /* 桶地址 */
	int was_bucket_head;  /* 弹出项是该桶链头 */
	struct timespec wait_ts;
	struct timeval now;
	char msg[40];

	rc = pthread_mutex_lock(DAT_0007704c);
	if (rc != 0) {
		mutex_lock_fail_cgminer("hash_pop", 0x1e23);
	}
	/* 表空 → (可选) 消费 work_update 计数, 然后等待 */
	if ((DAT_00077050 == 0) || (*(int *)(*(int *)(DAT_00077050 + 0x728) + 0xc) == 0)) {
		if ((DAT_00077055 != 0) && (DAT_00075dd8 < DAT_00075ddc)) {
			DAT_00075dd8 = DAT_00075dd8 + 1;
			DAT_00077055 = 0;
		}
		DAT_00077056 = 1;
		rc = 0;
		if (wait == 0) {
			goto out_unlock;    /* 原 LAB_000196a0: wait=0 不弹出, 返回 0 */
		}
		do {
			cgtime(&now);
			wait_ts.tv_nsec = now.tv_usec * 1000;
			wait_ts.tv_sec = now.tv_sec + 10;
			pthread_cond_signal((pthread_cond_t *)&DAT_0007b1a0);
			rc = pthread_cond_timedwait((pthread_cond_t *)(DAT_0007b620 + 0x28),
			                            DAT_0007704c, &wait_ts);
			/* 定时等待失败 (超时) 且尚未告警过 → 置标志并按日志门控提示 */
			if ((rc != 0) && (DAT_0007b4c8 == 0)) {
				DAT_0007b4c8 = 1;
				if ((DAT_0007ad48 != 0) || (DAT_00078e08 != 0) || (3 < DAT_000766c4)) {
					applog(4 /*LOG_WARNING*/, "Waiting for work to be available from pools.", 0);
				}
			}
		} while ((DAT_00077050 == 0) || (*(int *)(*(int *)(DAT_00077050 + 0x728) + 0xc) == 0));
	}
	/* 恢复提示 (仅当等待期间告警过) */
	if (DAT_0007b4c8 != 0) {
		if ((DAT_0007ad48 != 0) || (DAT_00078e08 != 0) || (3 < DAT_000766c4)) {
			applog(4 /*LOG_WARNING*/, "Work available from pools, resuming.", 0);
		}
		DAT_0007b4c8 = 0;
	}

	/* ---- 选出弹出目标 work: 默认为表头; 特定条件下向前扫描 ---- */
	head = DAT_00077050;
	if (DAT_00077050 == 0) {
		/* 原路径: 表头为 NULL 时仍读取 head+0x728 (= 地址 0x728 处内存);
		 * 实际不可达 (wait!=0 时等待循环已保证表头非空), 按机器码保留 */
		hh_tbl = *(int **)(NULL + 0x728);
		work = DAT_00077050;
	} else {
		hh_tbl = *(int **)(DAT_00077050 + 0x728);          /* work->hh.tbl */
		if ((DAT_00077058 < hh_tbl[3] /*tbl->num_items*/) &&
		    (*(char *)(DAT_00077050 + 0x6e1) == 0 /*head->clone*/) &&
		    (work = *(int *)(DAT_00077050 + 0x730), *(int *)(DAT_00077050 + 0x6e4) != 0)) {
			/* 排队数少于表项数且表头为非 clone 有 rolltime 的工作:
			 * 从表头之后向前扫描, 直到遇到 clone 项或无 rolltime 项 */
			while (work != 0) {
				if (*(char *)(work + 0x6e1) != 0) {
					prev_item = *(int *)(work + 0x72c);    /* hh.prev */
					goto picked;                            /* 原 joined_r0x0001977c */
				}
				if (*(int *)(work + 0x6e4) == 0) {
					break;
				}
				work = *(int *)(work + 0x730);             /* hh.next */
			}
		} else {
			work = DAT_00077050;
		}
	}
	prev_item = *(int *)(work + 0x72c);    /* work==0 时读地址 0x72c (原码如此, 理论不可达) */
picked:
	/* ---- uthash HASH_DELETE(staged_work, work) 等价展开 ---- */
	if ((prev_item == 0) && (*(int *)(work + 0x730) == 0)) {
		/* 表中仅剩该元素: 释放桶数组与表结构, 表头清空 */
		free((void *)*hh_tbl);                       /* tbl->buckets */
		free((void *)(*(int *)(DAT_00077050 + 0x728)));
		DAT_00077050 = 0;
	} else {
		if ((int)hh_tbl[4] - (int)hh_tbl[5] == work) {
			/* tbl->tail - hho == work → work 是表尾: tail 回退到前驱元素 */
			hh_tbl[4] = prev_item + hh_tbl[5];
			prev_item = *(int *)(work + 0x72c);
		}
		if (prev_item == 0) {
			/* 无前驱 → work 是表头: 表头后移 */
			prev_item = *(int *)(work + 0x730);
			head = prev_item;
			DAT_00077050 = prev_item;
		} else {
			/* prev->hh.next(+8) = work->hh.next */
			*(int *)(prev_item + *(int *)hh_tbl[5] + 8) = *(int *)(work + 0x730);
			prev_item = *(int *)(work + 0x730);
		}
		if (prev_item != 0) {
			/* next->hh.prev(+4) = work->hh.prev */
			*(int *)(prev_item + *(int *)hh_tbl[5] + 4) = *(int *)(work + 0x72c);
		}
		/* 桶计数维护: bucket = tbl->buckets[(num_buckets-1) & hashv] */
		bucket_off = (((unsigned)hh_tbl[1] - 1U) & *(unsigned *)(work + 0x744)) * 0xc;
		bucket = *(int *)hh_tbl + bucket_off;
		*(int *)(bucket + 4) = *(int *)(bucket + 4) - 1;      /* bucket->count-- */
		was_bucket_head = (*(int *)(*(int *)hh_tbl + bucket_off) == work + 0x728);
		if (was_bucket_head) {
			head = *(int *)(work + 0x738);                    /* work->hh.hh_next */
			*(int *)(*(int *)hh_tbl + bucket_off) = head;     /* bucket->head = hh_next */
		}
		if (was_bucket_head) {
			head = DAT_00077050;
		}
		/* 句柄链维护: hh_prev/hh_next (+0x734/+0x738) 交叉回链 */
		if (*(int *)(work + 0x734) != 0) {
			*(int *)(*(int *)(work + 0x734) + 0x10) = *(int *)(work + 0x738);
		}
		if (*(int *)(work + 0x738) != 0) {
			*(int *)(*(int *)(work + 0x738) + 0xc) = *(int *)(work + 0x734);
		}
		/* tbl->num_items-- (经 head->hh.tbl, 同一张表) */
		*(int *)(*(int *)(head + 0x728) + 0xc) = *(int *)(*(int *)(head + 0x728) + 0xc) - 1;
	}
	/* 非 clone 且有 rolltime 的工作计入 total_queued, 弹出后扣减 */
	if ((*(char *)(work + 0x6e1) == 0) && (*(int *)(work + 0x6e4) != 0)) {
		DAT_00077058 = DAT_00077058 - 1;
	}
	pthread_cond_signal((pthread_cond_t *)&DAT_0007b1a0);
	pthread_cond_signal((pthread_cond_t *)(DAT_0007b620 + 0x28));
	DAT_0007b948 = time((time_t *)0x0);

out_unlock:    /* 原 LAB_000196a0 */
	if (pthread_mutex_unlock(DAT_0007704c) != 0) {
		mutex_unlock_fail_cgminer("hash_pop", 0x1e66);
	}
	(*DAT_00075ed8)();                     /* 全局函数指针 (ELF 初值 noop_hook, 恒 0) */
	if (wait == 0) {
		return rc;                         /* wait=0 路径固定返回 0 */
	}
	return work;                           /* 弹出的 work 指针 */
}

/* === hashmeter === 原 FUN_0001bba4 @ 0x0001bba4
 * 来源: reversed/cgminer/20_job_mining/FUN_0001bba4.c | 分类: 20_job_mining
 * 语义: 算力速率计量。thr_id>=0 时: 取该线程 cgpu, 累加本周期哈希量并按
 *       tau=scantime/60/300/700 秒衰减 cgpu 的 4 个速率 EMA (+0x30/+0x38/
 *       +0x40/+0x48), 打印单机状态行; 每 scantime 秒一次还更新全局统计
 *       (12 槽滚动均值 → DAT_0007bb10, 1m/5m/15m EMA, 打印 "(%ds)..h/s" 行)。
 *       thr_id<0 时只做"无新增哈希"的全设备衰减 + 全局统计刷新。
 *       total_hashes/运行总秒数 参与日均 86400s 时钟跳变防护 (越界 → applog
 *       "cgminer time error ..." + zero_stats)。
 * cgpu 偏移: +0x8c timeval(上次更新), +0xf0 int(上次更新 tv_sec),
 *   +0x50 double(累计哈希), +0x30/0x38/0x40/0x48 double(4 个速率 EMA)。
 * thr 偏移: +0x24 = cgpu, +0x2c = timeval(线程最近取 work 时刻)。
 * 形参: thr_id (r0); hashes (r2:r3, uint64); r1 为 64 位对齐占位。
 */
void hashmeter(int thr_id, uint64_t hashes)
{
	double hashes_d;       /* d8/d11: (double)hashes */
	double dev_secs;       /* d11: tdiff(now, cgpu->tv(+0x8c)) — 设备更新间隔 */
	double cycle_secs;     /* d10: tdiff(now, DAT_000770d8) — scantime 周期间隔 */
	double total_secs_f;   /* tdiff(now, DAT_000770d0) — 运行总秒数 */
	double slot_old;       /* 滚动槽旧值 */
	uint64_t sum_u64;      /* 12 槽有效值之和 (按 double 累加后取整) */
	uint64_t avg_u64;      /* 有效槽均值 (count==0 时退化为 hashes 原值) */
	int count;             /* 参与均值的槽数 */
	int upd_global;        /* r6: 是否到达 scantime 全局更新点 */
	int i;
	uint64_t hashes_u64 = hashes;
	char statline[256];    /* get_statline 缓冲; 尾段复用为 (5m) 后缀缓冲 */
	char logbuf[4100];     /* applog 拼接缓冲; 尾段复用为 (15m) 后缀缓冲 */
	char avg_suffix[16];   /* "(avg)" */
	char cur_suffix[16];   /* "(%ds)" */
	char m1_suffix[16];    /* "(1m)" */
	char m5_suffix[16];    /* "(5m)" */
	char m15_suffix[16];   /* "(15m)" */

	cgtime(&DAT_00077148);
	cycle_secs = tdiff(&DAT_00077148, &DAT_000770d8);
	if (DAT_00077148.tv_sec - DAT_00077150 < DAT_00075dcc) {
		/* scantime 未满: thr_id<0 直接返回; 否则仅做本线程更新 */
		if (thr_id < 0) {
			return;
		}
		copy_timeval(&DAT_000770d8, &DAT_00077148);
		upd_global = 0;
	} else {
		/* scantime 满: 记录 tv_sec, 翻转状态字节, 刷新周期起点 */
		DAT_00077150 = DAT_00077148.tv_sec;
		DAT_00077154 = (uint8_t)(DAT_00077155 ^ DAT_00077154);
		copy_timeval(&DAT_000770d8, &DAT_00077148);
		if (thr_id < 0) {
			/* 全设备"零增量"衰减 (时间推进但不新增哈希) */
			if (pthread_mutex_lock(&DAT_0007b384) != 0) {
				mutex_lock_fail_cgminer("hashmeter", 0x1a6b);
			}
			if (0 < DAT_0007bb6c) {
				for (i = 0; i < DAT_0007bb6c; i++) {
					void *cgpu = *(void **)((char *)get_thread(i) + 0x24);
					dev_secs = tdiff(&DAT_00077148, (struct timeval *)((char *)cgpu + 0x8c));
					copy_timeval((char *)cgpu + 0x8c, &DAT_00077148);
					decay_time((double *)((char *)cgpu + 0x30), 0.0, dev_secs,
					           (double)(int)DAT_00075dcc);
					decay_time((double *)((char *)cgpu + 0x38), 0.0, dev_secs, 60.0);
					decay_time((double *)((char *)cgpu + 0x40), 0.0, dev_secs, 300.0);
					decay_time((double *)((char *)cgpu + 0x48), 0.0, dev_secs, 700.0);
				}
			}
			if (pthread_mutex_unlock(&DAT_0007b384) != 0) {
				mutex_unlock_fail_cgminer("hashmeter", 0x1a78);
			}
			upd_global = 1;
			(*DAT_00075ed8)();
			goto global_stats;                 /* 原 LAB_0001be98 */
		}
		upd_global = 1;
	}

	/* ---- 本线程/设备更新 ---- */
	{
		void *thr = get_thread(thr_id);
		void *cgpu = *(void **)((char *)thr + 0x24);
		copy_timeval((char *)thr + 0x2c, &DAT_00077148);
		*(int *)((char *)cgpu + 0xf0) = DAT_00077148.tv_sec;
		dev_secs = tdiff(&DAT_00077148, (struct timeval *)((char *)cgpu + 0x8c));
		copy_timeval((char *)cgpu + 0x8c, &DAT_00077148);
		if (pthread_mutex_lock(&DAT_0007b384) != 0) {
			mutex_lock_fail_cgminer("hashmeter", 0x1a51);
		}
		hashes_d = (double)hashes_u64;
		*(double *)((char *)cgpu + 0x50) = *(double *)((char *)cgpu + 0x50) + hashes_d;
		/* 4 个速率 EMA: tau = scantime / 60s / 300s / 700s */
		decay_time((double *)((char *)cgpu + 0x30), hashes_d, dev_secs,
		           (double)(int)DAT_00075dcc);
		decay_time((double *)((char *)cgpu + 0x38), hashes_d, dev_secs, 60.0);
		decay_time((double *)((char *)cgpu + 0x40), hashes_d, dev_secs, 300.0);
		decay_time((double *)((char *)cgpu + 0x48), hashes_d, dev_secs, 700.0);
		if (pthread_mutex_unlock(&DAT_0007b384) != 0) {
			mutex_unlock_fail_cgminer("hashmeter", 0x1a57);
		}
		(*DAT_00075ed8)();
		get_statline(statline, 0x100, cgpu);
	}
	if (DAT_00077018 == 0) {
		printf("%s          \r", statline);
		fflush(stdout);
	} else if ((DAT_00078e09 != 0) &&
	           ((DAT_0007ad48 != 0 || (DAT_00078e08 != 0)) || (6 < DAT_000766c4))) {
		snprintf(logbuf, 0x1000, "%s", statline);
		applog(7 /*LOG_DEBUG*/, logbuf, 0);
	}

global_stats:    /* 原 LAB_0001be98 */
	if (pthread_mutex_lock(&DAT_0007b384) != 0) {
		mutex_lock_fail_cgminer("hashmeter", 0x1a7b);
	}
	hashes_d = (double)hashes_u64;
	DAT_0007acc0 = hashes_d + DAT_0007acc0;        /* 全局累计哈希量 */
	if (upd_global) {
		/* ---- 12 槽滚动均值 → 当前窗速率 DAT_0007bb10 ---- */
		int idx = DAT_00077140 + 1;
		const double *p = DAT_000770e0;
		const double *end = DAT_000770e0 + 12;
		sum_u64 = 0;
		count = 0;
		if (0xb < idx) {
			idx = 0;
		}
		DAT_00077140 = idx;
		do {
			double slot = *p++;
			/* 原判据: !(slot < 0.0) == isnan(slot) — 即求和非负且非 NaN 的槽 */
			if (!(slot < 0.0) && !isnan(slot)) {
				sum_u64 = (uint64_t)(slot + (double)sum_u64);
				count++;
			}
		} while (p != end);
		avg_u64 = hashes_u64;      /* 无有效槽时沿用原始 hashes 值 (原码如此) */
		if (count != 0) {
			avg_u64 = sum_u64 / (uint64_t)count;
		}
		decay_time(&DAT_0007bb10, (double)avg_u64,
		           (double)(int)DAT_00075dcc, (double)(int)DAT_00075dcc);
		/* 1m/5m/15m EMA: 增量为本周期哈希量, 间隔为 scantime 周期实际时长 */
		decay_time(&DAT_0007bc98, hashes_d, cycle_secs, 60.0);
		decay_time(&DAT_0007bc88, hashes_d, cycle_secs, 300.0);
		decay_time(&DAT_0007b2f8, hashes_d, cycle_secs, 700.0);
		DAT_0007bad0 = llround(DAT_0007bb10) * 1000000;
		slot_old = 0.0;            /* 新窗口槽清零后计入本轮哈希量 */
	} else {
		slot_old = DAT_000770e0[DAT_00077140];
	}
	DAT_000770e0[DAT_00077140] = slot_old + hashes_d;

	/* ---- 运行总秒数与 86400s 时钟跳变防护 ---- */
	total_secs_f = tdiff(&DAT_00077148, &DAT_000770d0);
	DAT_000755f8 = total_secs_f;
	if (total_secs_f - DAT_00075de8 <= 86400.0) {
		DAT_00075de8 = total_secs_f;
	} else {
		if ((DAT_0007ad48 != 0) || (DAT_00078e08 != 0) || (2 < DAT_000766c4)) {
			snprintf(logbuf, 0x1000,
			         "cgminer time error total_secs = %f last_total_secs = %f",
			         total_secs_f, DAT_00075de8);
			applog(3 /*LOG_ERR*/, logbuf, 0);
		}
		if (pthread_mutex_unlock(&DAT_0007b384) != 0) {
			mutex_unlock_fail_cgminer("hashmeter", 0x1aa3);
		}
		(*DAT_00075ed8)();
		zero_stats();
		if (pthread_mutex_lock(&DAT_0007b384) != 0) {
			mutex_lock_fail_cgminer("hashmeter", 0x1aa5);
		}
	}
	if (upd_global) {
		/* ---- 生成全局速率行 "(%ds):.. (1m):.. (5m):.. (15m):.. (avg):..h/s" ---- */
		suffix_string((uint64_t)((DAT_0007acc0 / DAT_000755f8) * 1000000.0),
		              avg_suffix, 0x10, 4);
		DAT_00077158 = DAT_0007bb10 / 1000.0;
		suffix_string((uint64_t)(DAT_0007bb10 * 1000000.0), cur_suffix, 0x10, 4);
		suffix_string((uint64_t)(DAT_0007bc98 * 1000000.0), m1_suffix, 0x10, 4);
		suffix_string((uint64_t)(DAT_0007bc88 * 1000000.0), m5_suffix, 0x10, 4);
		suffix_string((uint64_t)(DAT_0007b2f8 * 1000000.0), m15_suffix, 0x10, 4);
		snprintf(DAT_00077160, 0x100,
		         "(%ds):%s (1m):%s (5m):%s (15m):%s (avg):%sh/s", DAT_00075dcc,
		         cur_suffix, m1_suffix, m5_suffix, m15_suffix, avg_suffix);
	}
	if (pthread_mutex_unlock(&DAT_0007b384) != 0) {
		mutex_unlock_fail_cgminer("hashmeter", 0x1ac2);
	}
	(*DAT_00075ed8)();
	if (upd_global) {
		if (DAT_00077018 == 0) {
			printf("%s          \r", DAT_00077160);
			fflush(stdout);
		} else if ((DAT_00078e09 != 0) &&
		           ((DAT_0007ad48 != 0 || (DAT_00078e08 != 0)) || (6 < DAT_000766c4))) {
			snprintf(logbuf, 0x1000, "%s", DAT_00077160);
			applog(7 /*LOG_DEBUG*/, logbuf, 0);
		}
	}
	return;
}

/* ==== 转写区 (逐函数追加) ==== */
