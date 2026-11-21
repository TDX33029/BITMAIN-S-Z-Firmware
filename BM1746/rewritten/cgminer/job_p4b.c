// ============================================================
// job_p4b.c — cgminer 挖矿任务层 (p4b): SHA256 压缩 / 汇总统计 /
//   nonce 去重 / hashmeter / fill_work_thread / nonce_handle /
//   dump_string / libgcc 浮点转换
// 来源: reversed/cgminer/20_job_mining/ 共 9 个函数
// ============================================================

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

// ---- 本文件函数 (按分配序: sha256_compress / summary / isdupnonce /
//      hashmeter_thread / fill_work_thread / nonce_handle / dump_string /
//      __floatundidf / __floatdidf) ----
// === sha256_compress === 原 FUN_000300b0
// 来源: reversed/cgminer/20_job_mining/FUN_000300b0.c | 分类: 20_job_mining
// 语义: SHA-256 压缩函数: 对 data 起始的 nblocks 个 64 字节分组做迭代压缩,
//       累加进 ctx+0x88 处的 8 字 (32 字节) 中间散列状态。供 MIDstate / 工作量
//       计算等任务构造路径使用。
static void sha256_compress(int ctx, const uint8_t *data, int nblocks)
{
	uint32_t w[64];
	uint32_t hs[8];
	uint32_t a, b, c, d, e, f, g, h;
	uint32_t t1, t2;
	int block, i;

	/* SHA-256 轮常数表 K, 64 字, 位于 0x00075fdc (首项 0x428a2f98) */
	static const uint32_t k[64] = {
		0x428a2f98u, 0x71374491u, 0xb5c0fbcfu, 0xe9b5dba5u,
		0x3956c25bu, 0x59f111f1u, 0x923f82a4u, 0xab1c5ed5u,
		0xd807aa98u, 0x12835b01u, 0x243185beu, 0x550c7dc3u,
		0x72be5d74u, 0x80deb1feu, 0x9bdc06a7u, 0xc19bf174u,
		0xe49b69c1u, 0xefbe4786u, 0x0fc19dc6u, 0x240ca1ccu,
		0x2de92c6fu, 0x4a7484aau, 0x5cb0a9dcu, 0x76f988dau,
		0x983e5152u, 0xa831c66du, 0xb00327c8u, 0xbf597fc7u,
		0xc6e00bf3u, 0xd5a79147u, 0x06ca6351u, 0x14292967u,
		0x27b70a85u, 0x2e1b2138u, 0x4d2c6dfcu, 0x53380d13u,
		0x650a7354u, 0x766a0abbu, 0x81c2c92eu, 0x92722c85u,
		0xa2bfe8a1u, 0xa81a664bu, 0xc24b8b70u, 0xc76c51a3u,
		0xd192e819u, 0xd6990624u, 0xf40e3585u, 0x106aa070u,
		0x19a4c116u, 0x1e376c08u, 0x2748774cu, 0x34b0bcb5u,
		0x391c0cb3u, 0x4ed8aa4au, 0x5b9cca4fu, 0x682e6ff3u,
		0x748f82eeu, 0x78a5636fu, 0x84c87814u, 0x8cc70208u,
		0x90befffau, 0xa4506cebu, 0xbef9a3f7u, 0xc67178f2u
	};

	for (block = 0; block < nblocks; block++) {
		const uint8_t *blk = data + block * 0x40;

		/* W[0..15]: 按大端从 64 字节分组装入 */
		for (i = 0; i < 16; i++)
			w[i] = (uint32_t)blk[i * 4 + 0] << 24 |
			       (uint32_t)blk[i * 4 + 1] << 16 |
			       (uint32_t)blk[i * 4 + 2] << 8 |
			       (uint32_t)blk[i * 4 + 3];

		/* W[16..63]: 消息扩展 */
		for (i = 16; i < 64; i++) {
			uint32_t s0 = (w[i - 15] >> 7 | w[i - 15] << 25) ^
			              (w[i - 15] >> 18 | w[i - 15] << 14) ^
			              (w[i - 15] >> 3);
			uint32_t s1 = (w[i - 2] >> 17 | w[i - 2] << 15) ^
			              (w[i - 2] >> 19 | w[i - 2] << 13) ^
			              (w[i - 2] >> 10);
			w[i] = w[i - 16] + s0 + w[i - 7] + s1;
		}

		/* 8 字中间状态读自 ctx+0x88 */
		for (i = 0; i < 8; i++)
			hs[i] = *(uint32_t *)(ctx + 0x88 + i * 4);

		a = hs[0]; b = hs[1]; c = hs[2]; d = hs[3];
		e = hs[4]; f = hs[5]; g = hs[6]; h = hs[7];

		/* 64 轮压缩 (原代码以 local_140[0..7] 循环旋转寄存器, 此处为等价的标准轮函数) */
		for (i = 0; i < 64; i++) {
			uint32_t sig1 = (e >> 6 | e << 26) ^
			                (e >> 11 | e << 21) ^
			                (e >> 25 | e << 7);
			uint32_t ch = (e & f) ^ (~e & g);
			uint32_t sig0 = (a >> 2 | a << 30) ^
			                (a >> 13 | a << 19) ^
			                (a >> 22 | a << 10);
			uint32_t maj = (a & b) ^ (a & c) ^ (b & c);

			t1 = h + sig1 + ch + k[i] + w[i];
			t2 = sig0 + maj;
			h = g; g = f; f = e;
			e = d + t1;
			d = c; c = b; b = a;
			a = t1 + t2;
		}

		/* 累加回中间状态 */
		hs[0] += a; hs[1] += b; hs[2] += c; hs[3] += d;
		hs[4] += e; hs[5] += f; hs[6] += g; hs[7] += h;
		for (i = 0; i < 8; i++)
			*(uint32_t *)(ctx + 0x88 + i * 4) = hs[i];
	}
}

/* ==== 外部声明 (定义分布在其余批次文件, 名字取 name_map) ==== */
typedef struct api_data api_data_t;             /* 完整定义见 api_p2.c */
typedef struct { uint32_t siz; char *ptr; char *cur; uint8_t sock; uint8_t close; } io_data_t; /* +0x0D = close 标志 */

extern void       message(void *io_data, int code, int param0, int param1, int isjson);   /* FUN_0003305c */
extern int        io_add(io_data_t *io_data, const char *str);                             /* FUN_000307c8, 返回 io_open */
extern void       _mutex_lock(const char *file, int line);                                 /* FUN_0000b774 */
extern void       _mutex_unlock(const char *file, int line);                               /* FUN_0000b84c */
extern void       print_data(void *io_data, api_data_t *root, int isjson, int prepend_comma); /* FUN_00032534 */
extern api_data_t *api_add_elapsed(api_data_t *root, char *name, double *data, int copy);   /* FUN_00032fcc */
extern api_data_t *api_add_string(api_data_t *root, char *name, char *data, int copy);      /* FUN_00032e1c */
extern api_data_t *api_add_int(api_data_t *root, char *name, int *data, int copy);          /* FUN_00032ed0 */
extern api_data_t *api_add_mhs(api_data_t *root, char *name, double *data, int copy);       /* FUN_00034eb0 */
extern api_data_t *add_uint_buf(api_data_t *root, char *name, unsigned int *data, int copy);    /* FUN_00032ef4 */
extern api_data_t *add_int64_buf(api_data_t *root, char *name, int64_t *data, int copy);        /* FUN_00032f84 */
extern api_data_t *add_uint64_buf(api_data_t *root, char *name, uint64_t *data, int copy);      /* FUN_00032f60 */
extern api_data_t *add_utility_buf(api_data_t *root, char *name, double *data, int copy);       /* FUN_00035568 */
extern api_data_t *add_mhtotal_buf(api_data_t *root, char *name, double *data, int copy);       /* FUN_00035520 */
extern api_data_t *add_diff_buf(api_data_t *root, char *name, double *data, int copy);          /* FUN_000355f8 */
extern api_data_t *add_percent_buf(api_data_t *root, char *name, double *data, int copy);       /* FUN_000357a4 */
extern api_data_t *api_add_time(api_data_t *root, char *name, time_t *data, int copy);      /* FUN_00033038 */

/* ==== 全局统计量 (名称保留 DAT_ 原名, 语义对照上游 cgminer.c) ==== */
extern double       DAT_000755f8;   /* total_secs: 运行总秒数 */
extern double       DAT_0007acc0;   /* total_mhashes_done: 累计 MH ("Total MH"/GHS av 分子) */
extern int64_t      DAT_0007b420;   /* total_accepted: 累计接受份额 (低字 +0x7b420, 高字 +0x7b424) */
extern int64_t      DAT_0007b6f8;   /* total_rejected: 累计拒绝份额 */
extern int64_t      DAT_0007b648;   /* total_getworks */
extern int64_t      DAT_0007b7c8;   /* total_discarded */
extern int64_t      DAT_0007bc80;   /* total_stale */
extern double       DAT_0007b5e8;   /* total_diff_accepted (难度计) */
extern double       DAT_0007b670;   /* total_diff_rejected */
extern double       DAT_0007b428;   /* total_diff_stale */
extern uint32_t     DAT_0007b100;   /* total_diff1 低 32 位 (+0x7b104 高 32 位) */
extern uint32_t     DAT_0007b104;
extern int          DAT_0007b8a4;   /* hw_errors: 硬件错误数 */
extern uint32_t     DAT_0007b8b8;   /* found_blocks */
extern uint32_t     DAT_0007b43c;   /* total_go: get 失败数 ("Get Failures") */
extern uint32_t     DAT_0007b4cc;   /* local_work */
extern uint32_t     DAT_0007ae68;   /* total_ro: 远端失败数 ("Remote Failures") */
extern uint32_t     DAT_0007bb1c;   /* new_blocks: 网络块数 ("Network Blocks") */
extern uint64_t     DAT_00076f18;   /* best_diff: 最佳份额难度 ("Best Share") */
extern time_t       DAT_0007b948;   /* last_getwork */
extern pthread_mutex_t DAT_0007b384;/* hash_lock (summary 统计拷贝期间持锁) */
/* 伪速率 (demo) 模式全局 */
extern int          DAT_00078e28;   /* 非 0: 启用伪统计输出 */
extern int          DAT_00078e38;   /* 伪速率基准值 */
extern char         DAT_00076b84[]; /* 机型串: "z9" / "z9-mini" */
extern double       DAT_00078e30;   /* 伪 GHS 5s 原始值 */
extern char         DAT_00078b84[]; /* GHS 5s 格式化串缓冲 (sprintf 目标) */
extern void       (*DAT_00075ed8)(void); /* noop_hook 全局函数指针 (ELF 初值 FUN_00028ae0) */

// === summary === 原 FUN_000357c8
// 来源: reversed/cgminer/20_job_mining/FUN_000357c8.c | 分类: 20_job_mining
// 语义: 组装 API "SUMMARY" 应答: message STATUS 头 → 持 hash_lock 拷贝统计 →
//   逐项 api_add_* (Elapsed/GHS 5s/GHS av/Found Blocks/.../Pool Stale%) →
//   print_data 输出。算式与上游 api.c summary() 一致; 另含固件私有的
//   伪速率分支 (DAT_00078e28 非 0 时按机型 z9/z9-mini 生成随机假速率)。
void summary(io_data_t *io_data, int c, char *param, unsigned int isjson)
{
	api_data_t *root;
	unsigned int io_open;
	int         lock_ret;
	const char *hdr;
	uint64_t    total_diff1;
	double      ghs_5s_base;   /* 原 local_70 第一阶段: 接受份额转 double */
	double      divisor;       /* 原 dVar9: WU 分母 (秒数或 1.0) */
	double      ghs_av;        /* 原 local_68 */
	double      utility;       /* 原 local_70 终值 */
	double      work_utility;  /* 原 local_60 */
	double      dtmp1;         /* 原 dVar8 */
	double      pct;           /* 原 dVar9 复用 (百分比阶段) */
	int         rnd;           /* 原 iVar6 等随机中间量 */

	/* STATUS 头 (0xb = MSG_SUMM), 再追 "SUMMARY," 或 ",\"SUMMARY\":[" */
	message(io_data, 0xb, 0, 0, isjson);
	hdr = "SUMMARY,";
	if (isjson != 0)
		hdr = ",\"SUMMARY\":[";
	io_open = (unsigned int)io_add(io_data, hdr);

	/* 拷贝统计期间持 hash_lock, 锁失败按 api.c 包装上报 */
	lock_ret = pthread_mutex_lock(&DAT_0007b384);
	if (lock_ret != 0)
		_mutex_lock("summary", 0xae9);

	/* total_diff1 = (int64)(diff_accepted + diff_rejected + diff_stale)
	 * (原 FUN_0005cc10 __fixdfdi), 存回全局 64 位计数 */
	total_diff1 = (uint64_t)(int64_t)(DAT_0007b5e8 + DAT_0007b670 + DAT_0007b428);
	DAT_0007b100 = (uint32_t)total_diff1;
	DAT_0007b104 = (uint32_t)(total_diff1 >> 32);

	/* Utility = 接受份额 / 运行秒数 × 60 (秒数为 0 时除数取 1.0);
	 * GHS av = total_mhashes_done / 秒数 (两分支均直接除, 0 秒时产生 inf/NaN,
	 * 与上游 mhs = total_mhashes_done / total_secs 的无保护写法一致) */
	ghs_5s_base = (double)DAT_0007b420;             /* 原 FUN_0005cb64 __floatdidf */
	if (DAT_000755f8 == 0.0) {
		ghs_av = DAT_0007acc0 / DAT_000755f8;
		utility = ghs_5s_base * 60.0;
		divisor = 1.0;
	} else {
		ghs_av = DAT_0007acc0 / DAT_000755f8;
		utility = (ghs_5s_base / DAT_000755f8) * 60.0;
		divisor = DAT_000755f8;
	}
	dtmp1 = (double)(int64_t)total_diff1;
	work_utility = (dtmp1 / divisor) * 60.0;

	/* 固件私有: 伪速率 demo 模式 — 按机型 z9 / z9-mini 用 rand() 生成
	 * GHS av / Utility / Work Utility / GHS 5s 串 (真实统计被覆盖) */
	if (DAT_00078e28 != 0) {
		int fake_ghs;    /* 原 iVar5: GHS av 基数 */
		int fake_util;   /* 原 iVar2: Utility 基数 */

		srand(time(NULL));
		fake_util = DAT_00078e38;
		if (DAT_00076b84[0] == 'z' && DAT_00076b84[1] == '9' && DAT_00076b84[2] == '\0') {
			/* z9: GHS av 基数 = 基准×14 + rand()%2 */
			fake_ghs = rand();
			fake_ghs = fake_util * 14 + fake_ghs % 2;
			fake_util = fake_ghs % 5;
		} else if (strcmp(DAT_00076b84, "z9-mini") == 0) {
			/* z9-mini: GHS av 基数 = 基准×3 + 1 + rand()%2 */
			fake_ghs = rand();
			fake_ghs = fake_util * 3 + 1 + fake_ghs % 2;
			fake_util = fake_ghs % 5;
		} else {
			fake_util = 4;
			fake_ghs = 14;
		}

		srand(time(NULL));                   /* 重播种后取小数部分与 WU */
		rnd = rand() % 99;                   /* 原 iVar6: 小数部分来源 */
		ghs_av = (double)fake_ghs + (double)rnd / 100.0;
		utility = (double)rnd / 100.0 + (double)fake_util;
		DAT_00078e30 = ghs_av;
		work_utility = (double)(rand() % 4 + 100) + 0.27;
		sprintf(DAT_00078b84, "%f", ghs_av + 0.79);
	}

	/* ---- 逐项追加统计条目 ---- */
	root = api_add_elapsed(NULL, "Elapsed", &DAT_000755f8, 1);
	root = api_add_string(root, "GHS 5s", DAT_00078b84, 0);
	root = api_add_mhs(root, "GHS av", &ghs_av, 0);
	root = add_uint_buf(root, "Found Blocks", &DAT_0007b8b8, 1);
	root = add_int64_buf(root, "Getworks", &DAT_0007b648, 1);
	root = add_int64_buf(root, "Accepted", &DAT_0007b420, 1);
	root = add_int64_buf(root, "Rejected", &DAT_0007b6f8, 1);
	root = api_add_int(root, "Hardware Errors", &DAT_0007b8a4, 1);
	root = add_utility_buf(root, "Utility", &utility, 0);
	root = add_int64_buf(root, "Discarded", &DAT_0007b7c8, 1);
	root = add_int64_buf(root, "Stale", &DAT_0007bc80, 1);
	root = add_uint_buf(root, "Get Failures", &DAT_0007b43c, 1);
	root = add_uint_buf(root, "Local Work", &DAT_0007b4cc, 1);
	root = add_uint_buf(root, "Remote Failures", &DAT_0007ae68, 1);
	root = add_uint_buf(root, "Network Blocks", &DAT_0007bb1c, 1);
	root = add_mhtotal_buf(root, "Total MH", &DAT_0007acc0, 1);
	root = add_utility_buf(root, "Work Utility", &work_utility, 0);
	root = add_diff_buf(root, "Difficulty Accepted", &DAT_0007b5e8, 1);
	root = add_diff_buf(root, "Difficulty Rejected", &DAT_0007b670, 1);
	root = add_diff_buf(root, "Difficulty Stale", &DAT_0007b428, 1);
	root = add_uint64_buf(root, "Best Share", &DAT_00076f18, 1);

	/* Device Hardware% = hw / (hw + total_diff1) (总和为 0 记 0) */
	{
		int64_t hw_sum = (int64_t)total_diff1 + (int64_t)DAT_0007b8a4;
		if (hw_sum == 0)
			pct = 0.0;
		else
			pct = (double)DAT_0007b8a4 / (double)hw_sum;   /* 原 __floatdidf(hw_sum) */
		root = add_percent_buf(root, "Device Hardware%", &pct, 0);
	}

	/* Device Rejected% = diff_rejected / total_diff1 */
	if (total_diff1 == 0)
		pct = 0.0;
	else
		pct = DAT_0007b670 / (double)total_diff1;          /* 原 __floatdidf(total_diff1) */
	root = add_percent_buf(root, "Device Rejected%", &pct, 0);

	/* Pool Rejected% / Pool Stale% = 各自份额 / (diffA + diffR + diffS) */
	{
		double diff_sum = DAT_0007b670 + DAT_0007b5e8 + DAT_0007b428;
		pct = (diff_sum == 0.0) ? 0.0 : DAT_0007b670 / diff_sum;
		root = add_percent_buf(root, "Pool Rejected%", &pct, 0);
		{
			double stale_sum = DAT_0007b5e8 + DAT_0007b670 + DAT_0007b428;
			pct = (stale_sum == 0.0) ? 0.0 : DAT_0007b428 / stale_sum;
			root = add_percent_buf(root, "Pool Stale%", &pct, 0);
		}
	}
	root = api_add_time(root, "Last getwork", &DAT_0007b948, 0);

	lock_ret = pthread_mutex_unlock(&DAT_0007b384);
	if (lock_ret != 0)
		_mutex_unlock("summary", 0xb2e);

	(*DAT_00075ed8)();                       /* noop_hook 全局函数指针直调 */
	print_data(io_data, root, (int)isjson, 0);
	if ((io_open & isjson) != 0)
		io_data->close = 1;                  /* 原内联 io_close: io_data->close = true */

	return;
}

/* ==== klist.c / noncedup.c 数据布局 (32 位 ARM, 对照上游 klist.h) ==== */
typedef struct cglock { pthread_mutex_t mutex; pthread_rwlock_t rwlock; } cglock_t;

typedef struct k_item {
	const char    *name;   /* +0x00: 所属链名 (一致性检查用) */
	struct k_item *prev;   /* +0x04 */
	struct k_item *next;   /* +0x08 */
	void          *data;   /* +0x0C */
} k_item_t;

typedef struct k_list {
	const char *name;      /* +0x00 */
	int         is_store;  /* +0x04 */
	cglock_t   *lock;      /* +0x08 */
	k_item_t   *head;      /* +0x0C */
	k_item_t   *tail;      /* +0x10 */
	/* +0x14 siz / +0x18 total / +0x1C count / +0x20 count_up / +0x2B do_tail … */
} k_list_t;

typedef struct nitem {         /* noncedup.c NITEM */
	uint32_t       work_id;  /* +0x00 */
	uint32_t       nonce;    /* +0x04 */
	struct timeval when;     /* +0x08 (32 位 time_t: 4+4) */
} nitem_t;
#define DATAN(item) ((nitem_t *)((k_item_t *)(item))->data)

typedef struct dupdata {       /* noncedup.c dupdata (cgpu->dup_data) */
	int       timelimit;   /* +0x00 */
	k_list_t *nfree_list;  /* +0x04: 空闲 NITEM 池 */
	k_list_t *nonce_list;  /* +0x08: 在用 nonce 缓存 (K_STORE) */
	uint64_t  checked;     /* +0x10 */
	uint64_t  dups;        /* +0x18 */
} dupdata_t;

/* noncedup.c / klist.c 辅助 (定义在其他批次文件) */
extern void      cgtime(struct timeval *tv);                                        /* FUN_0002a84c */
extern double    tdiff(struct timeval *earlier, struct timeval *later);             /* FUN_0002aa24 */
extern k_item_t *k_get(k_list_t *list, const char *file, const char *func, int line); /* FUN_000391fc: 链头取空闲项 (= _k_unlink_head) */
extern void      _k_add_head(k_list_t *list, k_item_t *item, const char *file, const char *func, int line); /* FUN_00039370 */
extern k_item_t *_k_unlink_tail(k_list_t *list, const char *file, const char *func, int line);         /* FUN_000392a8 */
extern void      applog(uint32_t prio, char *buf, ...);                        /* FUN_00038730: 第 3 参 flag 两见 (0/1), 语义待定 */
extern void      quit(int status);                                                  /* FUN_00016ca8 */
extern int      *__errno_location(void);

extern char   DAT_0007ad48;   /* 全局, 语义待定 (重复 nonce 告警门控) */
extern char   DAT_00078e08;   /* 全局, 语义待定 (重复 nonce 告警门控) */
extern int    DAT_000766c4;   /* 全局, 语义待定 (与 4 比较, 疑似日志级别) */

// === isdupnonce === 原 FUN_00039f2c
// 来源: reversed/cgminer/20_job_mining/FUN_00039f2c.c | 分类: 20_job_mining
// 语义: nonce 去重 (noncedup.c isdupnonce): 在 cgpu->dup_data 的 nonce 缓存
//   (K_STORE, 新项插链头) 中查 (work->id, nonce); 命中 → 告警并返回 1 (重复),
//   未命中 → 从空闲池取项写入并挂入缓存, 返回 0。插入后按 timelimit 从链尾
//   清理过期项回收进空闲池。全程持 nfree_list 的 cglock (mutex+wrlock)。
int isdupnonce(int cgpu, int work, uint32_t nonce)
{
	dupdata_t *dup;                 /* 原 puVar6 */
	cglock_t *cglock;               /* 原 ppVar7: K_LIST->lock (mutex+rwlock 对) */
	k_item_t *item;
	nitem_t  *ndata;
	int       ret;
	int       err;
	int       unique;               /* 原 uVar4 (1=唯一) */
	struct timeval now;

	dup = *(dupdata_t **)(cgpu + 0x18);              /* cgpu->dup_data */
	if (dup == NULL)
		return 0;                                    /* 无缓存 → 视为不重复 */

	cgtime(&now);
	dup->checked++;                                  /* 64 位计数 +1 */

	/* K_WLOCK(dup->nfree_list): 先 mutex 后 wrlock, 失败上报并退出 */
	cglock = *(cglock_t **)((char *)dup->nfree_list + 8);
	if (pthread_mutex_lock(&cglock->mutex) != 0) {
		err = *__errno_location();
		char errmsg[4096];
		snprintf(errmsg, 0x1000, "WTF MUTEX ERROR ON LOCK! errno=%d in %s %s():%d",
			 err, "noncedup.c", "isdupnonce", 0x46);
		applog(3, errmsg, 1);
		quit(1);
	}
	if (pthread_rwlock_wrlock(&cglock->rwlock) != 0) {
		err = *__errno_location();
		char errmsg[4096];
		snprintf(errmsg, 0x1000, "WTF WRLOCK ERROR ON LOCK! errno=%d in %s %s():%d",
			 err, "noncedup.c", "isdupnonce", 0x46);
		applog(3, errmsg, 1);
		quit(1);
	}

	/* 从链尾 (最旧) 向链头扫描, 查 (work->id, nonce) 是否已缓存 */
	unique = 1;
	item = dup->nonce_list->tail;
	while (unique && item != NULL) {
		ndata = DATAN(item);
		if (ndata->work_id == *(uint32_t *)(work + 0x724) && ndata->nonce == nonce) {
			unique = 0;
			/* 告警门控: 两全局标志置位或级别 >= 4 才输出 */
			if (DAT_0007ad48 == '\0' && DAT_00078e08 == '\0' && DAT_000766c4 < 4) {
				/* 静默 */
			} else {
				char msg[4096];
				snprintf(msg, 0x1000, "%s%d: Duplicate nonce %08x",
					 *(char **)(*(int *)(cgpu + 4) + 8),   /* cgpu->drv->name */
					 *(int *)(cgpu + 8),                   /* cgpu->device_id */
					 nonce);
				applog(4, msg, 0);
			}
		} else {
			item = item->prev;
		}
	}

	/* 未重复: 取空闲项, 记录 (work_id, nonce, now) 挂入缓存链头 */
	if (unique) {
		item = k_get(dup->nfree_list, "noncedup.c", "isdupnonce", 0x51);
		ndata = DATAN(item);
		ndata->work_id = *(uint32_t *)(work + 0x724);  /* work->id */
		ndata->nonce = nonce;
		memcpy(&ndata->when, &now, sizeof(now));       /* 原码拆两个 4 字节写 */
		_k_add_head(dup->nonce_list, item, "noncedup.c", "isdupnonce", 0x55);
	}

	/* 过期清理: 链尾停留超过 timelimit 的项逐个回收进空闲池 */
	item = dup->nonce_list->tail;
	while (item != NULL && tdiff(&(DATAN(item)->when), &now) > (double)dup->timelimit) {
		item = _k_unlink_tail(dup->nonce_list, "noncedup.c", "isdupnonce", 0x59);
		_k_add_head(dup->nfree_list, item, "noncedup.c", "isdupnonce", 0x5a);
		item = dup->nonce_list->tail;
	}

	/* K_WUNLOCK: 先 rwlock 后 mutex */
	if (pthread_rwlock_unlock(&cglock->rwlock) != 0) {
		err = *__errno_location();
		char errmsg[4096];
		snprintf(errmsg, 0x1000, "WTF RWLOCK ERROR ON UNLOCK! errno=%d in %s %s():%d",
			 err, "noncedup.c", "isdupnonce", 0x5d);
		applog(3, errmsg, 1);
		quit(1);
	}
	if (pthread_mutex_unlock(&cglock->mutex) != 0) {
		err = *__errno_location();
		char errmsg[4096];
		snprintf(errmsg, 0x1000, "WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d",
			 err, "noncedup.c", "isdupnonce", 0x5d);
		applog(3, errmsg, 1);
		quit(1);
	}

	(*DAT_00075ed8)();                               /* noop_hook 全局函数指针直调 */

	if (unique == 0)                                 /* 重复 → dups++ */
		dup->dups++;

	ret = unique ^ 1;                                /* 返回 1 = 重复 */
	return ret;
}
/* ==== hashmeter 全局 ==== */
extern uint32_t DAT_00078e60[];  /* 4 路 64 位份额计数器 (每链 8 字节: 低字+高字, bitmain_scanhash 累加) */
extern char     DAT_0009d4bc[];  /* 每链使能字节 (0x0009d4bb 为前一数据末字节) */
extern double   DAT_00078e40[4]; /* 每链 10 槽滑动窗口之和 */
extern double   DAT_00078e80;    /* 总速率 (GHS) */
extern char     DAT_0007c0a8[64];/* 每链速率串, 16 字节/条 ("%.2f") */
extern void     copy_timeval(struct timeval *dst, struct timeval *src); /* FUN_0002a530 */

// === hashmeter_thread === 原 FUN_0003abf4
// 来源: reversed/cgminer/20_job_mining/FUN_0003abf4.c | 分类: 20_job_mining
// 语义: 周期 (sleep 5s) 汇率线程: 读各链自上次采样以来的 64 位份额计数
//   (用后清零), 折算速率 (计数 ×0xffff / 距上次采样秒数) 写入每链 10 槽
//   环形缓冲; 输出每链均值串 (DAT_0007c0a8[i]) 与总速率串 DAT_00078b84
//   (即 summary "GHS 5s") 及总速率值 DAT_00078e80。
void hashmeter_thread(void)
{
	struct timeval prev_time;                 /* 原 local_1c0/local_1bc */
	struct timeval cur_time;                  /* 原 local_1b8/local_1b4 */
	int    ring_idx[4];                       /* 原 local_1b0: 每链下一写入槽 */
	double ring[4][10];                       /* 原 local_1a0: 每链 10 槽速率窗 */
	int    sec_diff;                          /* 原 iVar1 */
	int    usec_diff;                         /* 原 iVar4 */
	double total_ghs;                         /* 原 dVar13 */
	double ring_sum;                          /* 原 dVar14 (窗口和) */
	double rate;                              /* 原 dVar16 */
	double elapsed_frac;                      /* 原 dVar15: 微秒部分 */
	double sec_d;                             /* 原 dVar17: 秒部分 */
	double *dst;                              /* 原 pdVar6 → DAT_00078e40[i] */
	uint32_t *cntp;                           /* 原 puVar8 → DAT_00078e60+8*i */
	const char *flagp;                        /* 原 pcVar10 → DAT_0009d4bc[i] */
	int i, slot;
	uint64_t counter;

	memset(ring, 0, 0x140);                   /* 40 × double 清零 */
	ring_idx[0] = 0; ring_idx[1] = 0; ring_idx[2] = 0; ring_idx[3] = 0;
	cgtime(&prev_time);
	for (;;) {
		cgtime(&cur_time);
		/* 秒/微秒差; 微秒借位时秒数 -1、微秒 +1000000
		 * (反编译拆成 +0xf4000 与 +0x240 两次条件加, 合计 0xF4240) */
		usec_diff = cur_time.tv_usec - prev_time.tv_usec;
		sec_diff = cur_time.tv_sec - prev_time.tv_sec;
		if (usec_diff < 0) {
			usec_diff += 0xf4000;
			sec_diff = sec_diff - 1;
			usec_diff += 0x240;
		}

		total_ghs = 0.0;
		dst = DAT_00078e40;
		cntp = DAT_00078e60;
		flagp = DAT_0009d4bc;
		for (i = 0; i < 4; i++) {
			if (*flagp != '\0') {
				/* 64 位计数 ×0xffff 转 double (原 FUN_0005cb50 __floatundidf,
				 * 乘积经 umull 分解传低/高两字, 此处等价写为 64 位乘) */
				counter = (uint64_t)cntp[1] << 32 | cntp[0];
				rate = (double)(counter * 0xffffULL);
				elapsed_frac = (double)(usec_diff + 1);   /* +1: 避免同微秒采样除零 */
				cntp[0] = 0;
				cntp[1] = 0;
				sec_d = (double)sec_diff;
				ring[i][ring_idx[i]] = rate / (sec_d + elapsed_frac / 1000000.0);

				*dst = 0.0;
				ring_sum = 0.0;
				for (slot = 0; slot < 10; slot++)
					ring_sum += ring[i][slot];
				*dst = ring_sum;

				ring_idx[i]++;
				ring_sum = (ring_sum / 1000.0) / 10.0;
				sprintf(&DAT_0007c0a8[i * 0x10], "%.2f", ring_sum);
				ring_sum = *dst;                  /* 总速率用窗口和 (未经 /1000/10) */
				if (ring_idx[i] > 9)
					ring_idx[i] = 0;
				total_ghs += ring_sum / 10.0;
			}
			dst++;
			cntp += 2;
			flagp++;
		}
		sprintf(DAT_00078b84, "%.2f", total_ghs); /* 即 summary "GHS 5s" 串 */
		DAT_00078e80 = total_ghs;
		copy_timeval(&prev_time, &cur_time);
		sleep(5);
	}
}
/* ==== fill_work_thread / workid 映射表全局 ==== */
extern char      DAT_00078e09;    /* opt_debug (api_p1b.c 同名) */
extern uint8_t   DAT_000790ec[];  /* 每链"已重初始化/可用"标志 (hash.c) */
extern uint8_t   DAT_000790fc[];  /* 每链退出标志 (reset_chain 置 1 → 线程返回) */
extern char      DAT_000790f0;    /* 全局暂停标志 (LED/状态输出路径使用) */
extern uint8_t   DAT_00078e14[];  /* 每链"需下发新工作"标志 (本函数发送后清 0) */
extern uint32_t  DAT_0009d4c4;    /* 下发间隔阈值 dev.timeout, µs (freq_p2.c) */
extern int       DAT_000790f1;    /* 已下发工作标志 (发送后置 1) */
extern struct timeval DAT_000790f4; /* 最近下发工作时刻 (fan.c 同名 timeval) */
extern pthread_mutex_t DAT_00078ea0; /* workid→work 映射表互斥锁 (bitmain_scanhash 同锁) */

extern void     *get_work(void *queue, int arg);                                  /* FUN_000247d4 */
extern uint8_t   bm1740_makeup_work(uint8_t *frame, int len);                      /* FUN_00042a60: 填 55AA/类型/CRC16 */
extern void      uart_send(int chain, uint8_t *frame, int len);                    /* FUN_0003c150 */
extern void      free_work(void *work);                                            /* FUN_00015780 */
extern void     *make_clone(void *work, int arg);                                  /* FUN_0001a44c */
extern void      cgsleep_ms(int ms);                                               /* FUN_0002a92c */
extern void      cgsleep_us(int us, int arg);                                      /* FUN_0002a954 */

// === fill_work_thread === 原 FUN_00042b54
// 来源: reversed/cgminer/20_job_mining/FUN_00042b54.c | 分类: 20_job_mining
// 语义: 每链工作下发线程: 链可用且距上次下发超过 dev.timeout (或要求补发) 时,
//   从 get 队列取 work, 取 work->id 低 7 位为 workid (0..127), 组 146 字节
//   下发帧 (55AA + 0x20 + workid + 140B job 数据 + CRC16, 由
//   bm1740_makeup_work 补齐) 经 uart_send 发出; 同时把克隆的 work 记入
//   128 槽映射表 (槽 k 地址 = base + k*4 + 0x76, 即 (k+0x1c)*4+6, 供
//   bitmain_scanhash 按 RX 帧内 workid 回查), 槽上旧 work 先释放。
int fill_work_thread(int *param_1)
{
	pthread_t   self;
	int         base;                     /* 原 iVar5: *param_1, 链上下文结构基址 */
	uint32_t    chain;                    /* 原 param3: 链号 (param_1+4 首字节) */
	void       *queue;                    /* 原 puVar6: *(base+0x276) get 队列 */
	uint32_t    workid;                   /* 原 param3_00: work->id & 0x7f (兼作 Work Error 计数) */
	char        exit_flag;                /* 原 cVar1 */
	char        errmsg[4100];             /* 原 acStack_1028 */
	uint8_t     frame[0x94];              /* 原 auStack_10bc/10ba/10b9/10b8: 146 字节下发帧 */
	struct timeval t_now;                 /* 原 iStack_10cc/local_10c8 */
	struct timeval t_base;                /* 原 iStack_10c4/local_10c0 */
	int         sec_diff, usec_diff;      /* 原 iVar3/iVar4 */
	int         slot;                     /* 原 iVar3: 映射表槽基址偏移 */
	void       *work;                     /* 原 iVar4 */
	void       *clone;

	self = pthread_self();
	pthread_detach(self);
	base = *param_1;
	chain = *(uint8_t *)(param_1 + 1);
	queue = *(void **)(base + 0x276);

	/* LOG_DEBUG 门控: opt_debug 且 (调试标志或日志级别 > 6) */
	if (DAT_00078e09 != '\0' &&
	    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 6 < DAT_000766c4)) {
		snprintf(errmsg, 0x1000, "Start To Fill Work!ChainIndex:[%d]", chain);
		applog(7, errmsg, 0);
	}

	cgtime(&t_now);
	workid = 0;
	cgtime(&t_base);

	do {                                   /* 原 LAB_00042bf8 */
		exit_flag = DAT_000790fc[chain];
		while (1) {
			if (exit_flag != 0)
				return 0;                  /* 退出标志 → 线程结束 */

			/* 链不可用 / 全局暂停 / 伪速率模式 → 10ms 空转 */
			if (DAT_000790ec[chain] == '\0' || DAT_000790f0 != '\0' || DAT_00078e28 != '\0') {
				cgsleep_ms(10);
				goto recheck;              /* 原 goto LAB_00042bf8: 回外层, 不走 500µs 短眠 */
			}

			/* 距上次下发不足 dev.timeout 且无补发请求 → 未到下发时机 */
			cgtime(&t_now);
			usec_diff = t_now.tv_usec - t_base.tv_usec;
			if (usec_diff < 0)
				usec_diff += 1000000;
			sec_diff = t_now.tv_sec - t_base.tv_sec;
			if (usec_diff < 0)
				sec_diff = sec_diff - 1;
			if (DAT_00078e14[chain] == '\0' &&
			    (uint32_t)(sec_diff * 1000000 + usec_diff) < DAT_0009d4c4)
				break;                     /* → 走 500µs 短眠 */

			cgtime(&t_base);               /* 重置计时 (覆盖取 work 阻塞耗时) */

			/* 从 get 队列取工作, 失败重试 */
			while ((work = get_work(queue, *(int *)queue)) == 0) {
				if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 2 < DAT_000766c4) {
					snprintf(errmsg, 0x1000, "Work Error![%d]", workid);
					applog(3, errmsg, 0);
				}
			}

			/* 组下发帧: [0..1]前导(由 makeup 填 55 AA) [2]类型0x20 [3]workid
			 * [4..143] job 数据 140 字节 [0x90..0x91] CRC16 */
			workid = *(uint32_t *)(work + 0x724) & 0x7f;   /* work->id 低 7 位 */
			DAT_00078e14[chain] = 0;
			memset(frame, 0, 0x92);
			frame[2] = 0x20;
			frame[3] = (uint8_t)workid;
			memcpy(&frame[4], (void *)(work + 0xe0), 0x8c);

			slot = base + (workid + 0x1c) * 4;
			bm1740_makeup_work(frame, 0x92);

			/* 映射表槽更新 (持 DAT_00078ea0): 旧 work 释放, 记入克隆 */
			pthread_mutex_lock(&DAT_00078ea0);
			if (*(int *)(slot + 6) != 0) {
				free_work((void *)*(int *)(slot + 6));
				*(uint16_t *)(slot + 6) = 0;
				*(uint16_t *)(slot + 8) = 0;
			}
			clone = make_clone(work, 0);
			*(int *)(base + (workid + 0x1c) * 4 + 6) = (int)clone;
			pthread_mutex_unlock(&DAT_00078ea0);

			if (DAT_00078e09 != '\0' &&
			    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 6 < DAT_000766c4)) {
				snprintf(errmsg, 0x1000, "ChainID[%d] Wirte Work. workid=%d, jobid=%s",
					 chain, workid, *(char **)(work + 0x6f0));
				applog(7, errmsg, 0);
			}

			uart_send(chain, frame, 0x92);
			DAT_000790f1 = 1;
			cgtime(&DAT_000790f4);
			cgsleep_us(500, 0);
			free_work(work);
			exit_flag = DAT_000790fc[chain];
		}
		cgsleep_us(500, 0);                /* 未到下发时机的 500µs 短眠 */
	recheck:
		;
	} while (1);
}
/* ==== nonce fifo 全局 (bitmain_scanhash 为消费者) ==== */
extern uint8_t  DAT_0009d6f3;     /* 芯片地址步进 asic_addr_interval (chain.c) */
extern int      DAT_0007c28c;     /* nonce fifo 写索引 (0..99 环形) */
extern int      DAT_0007c294;     /* nonce fifo 计数 (上限 100) */
extern int      DAT_00079bfc;     /* 错误 nonce 累计 ("receive a error nonce. total") */
extern pthread_mutex_t DAT_00078e88; /* nonce fifo 互斥锁 (生产 nonce_handle / 消费 scanhash) */
extern uint8_t  DAT_0007c7d8[];   /* fifo 条目元数据 (0x7c298 条目区 +0x540 起):
                                   *   +0x540 chip_addr / +0x541 workid / +0x542 nonceID /
                                   *   +0x543, +0x544(u32), +0x548(u32), +0x54C tag */
extern uint8_t  DAT_0007c7d9[];
extern uint8_t  DAT_0007c7da[];
extern uint8_t  DAT_0007c7db[];
extern uint8_t  DAT_0007c7dc[];   /* +0x544 起 4 字节 */
extern uint8_t  DAT_0007c7e0[];   /* +0x548 起 4 字节 */
extern uint8_t  DAT_0007c7e4[];   /* +0x54C: 来源 tag (RX 线程传入) */
extern uint16_t DAT_00068aec[];   /* CRC16 查表 (bm1740_makeup_work 共用) */

extern unsigned __udivsi3(unsigned a, unsigned b); /* FUN_0005c648: 无符号除法 (libgcc) */

// === nonce_handle === 原 FUN_0004a754
// 来源: reversed/cgminer/20_job_mining/FUN_0004a754.c | 分类: 20_job_mining
// 语义: RX nonce 帧处理 (93 字节帧: AA 55 前导 + 91 字节芯片包, 类型字节高半
//   字节 0xE): 按 chipid(=芯片地址/asic_addr_interval, 0..2) 与包内 loop 序号
//   (低 4 位) 把芯片包暂存到 pkts 区; 收满 16 包后做 CRC16 整合性校验
//   (bm1740_verify_nonce_integrality 内联) 与包序/地址/workid/nonceID 一致性
//   校验, 再把 16 包数据拼装为 0x54D 字节条目推入 nonce fifo (0x7c298 起
//   100 项环形队列, 持 DAT_00078e88 锁), 供 bitmain_scanhash 回查。
// 返回: 1 = 已收 1 包尚未凑齐 16 包; 0 = 成功入队; -1 = 错误。
int nonce_handle(uint8_t *dst, uint8_t *pkts, uint8_t *frame, int frame_len, uint8_t tag)
{
	char     errmsg[4100];             /* 原 acStack_1028 */
	uint32_t chipid;                   /* 原 uVar1 */
	uint32_t loop_idx;                 /* 原 uVar7 (包内 loop 序号, 复用作 CRC) */
	int      k;                        /* 原 iVar8 */
	int      n;                        /* 原 iVar2 */
	uint8_t *p;                        /* 原 pbVar3 */
	uint8_t *q;                        /* 原 pbVar5/pbVar4 */
	uint8_t *entry;                    /* 原 __src: 拼装目标条目 */
	uint32_t crc;
	uint16_t crc_be;                   /* 原 uStack_102a: 字节交换后 CRC */
	uint32_t total;                    /* 原 uVar1 (错误计数读出) */
	uint32_t qbase;                    /* 原 iVar8: fifo 条目字节基址 */
	int log3 = (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || DAT_000766c4 >= 3);
	int log4 = (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || DAT_000766c4 >= 4);

	(void)frame_len;                   /* 调用方传帧长 0x5D, 函数体内未使用 */

	/* 帧内芯片地址 (帧[3]) 除以地址步进得 chipid, 合法 0..2 */
	chipid = __udivsi3(frame[3], DAT_0009d6f3) & 0xff;
	if (chipid > 2) {
		if (!log4)
			return -1;
		snprintf(errmsg, 0x1000, "%s chipid %d is not exist", "nonce_handle", chipid);
		applog(4, errmsg, 0);
		return -1;
	}

	/* 芯片包 (91 字节 = 帧[2..0x5C]) 暂存到该芯片包区第 loop_idx 槽 */
	pkts += chipid * 0x5b0;
	loop_idx = frame[2] & 0xf;
	memcpy(pkts + loop_idx * 0x5b, frame + 2, 0x5b);
	if (loop_idx != 0xf)
		return 1;                      /* 16 包未收齐 */

	/* ---- CRC16 整合性校验 (bm1740_verify_nonce_integrality, 原 goto 收敛) ----
	 * 每包: 表驱动 CRC16 (初值 0xFFFF) 覆盖包 [0..0x58] 共 89 字节,
	 * 结果字节交换后与包尾 [0x59..0x5A] 的 2 字节 CRC 比对 */
	for (k = 0; k < 0x10; k++) {
		p = pkts + k * 0x5b;
		crc = 0xffff;
		for (q = p; q != p + 0x59; q++)
			crc = ((crc << 8) ^ DAT_00068aec[(*q ^ (crc >> 8)) & 0xff]) & 0xffff;
		crc_be = (uint16_t)((crc >> 8) | ((crc & 0xff) << 8));
		if (memcmp(&crc_be, pkts + k * 0x5b + 0x59, 2) != 0) {
			if (log3) {
				snprintf(errmsg, 0x1000,
					 "%s CRC error. cal-crc=%x, chip-crc=%02x%02x\n",
					 "bm1740_verify_nonce_integrality", crc_be,
					 pkts[k * 0x5b + 0x59], pkts[k * 0x5b + 0x5a]);
				applog(3, errmsg, 0);
			}
			if (log4) {
				total = DAT_00079bfc;
				DAT_00079bfc = DAT_00079bfc + 1;
				snprintf(errmsg, 0x1000, "receive a error nonce. total = %u", total);
				applog(4, errmsg, 0);
			}
			return -1;
		}
	}

	/* 包序连续性: 每包首字节低 4 位必须为 0,1,2,...,15 */
	for (k = 0; k < 0x10; k++) {
		if (k != (pkts[k * 0x5b] & 0xf)) {
			if (log3) {
				snprintf(errmsg, 0x1000, "nonce is not continous i=%d\n", k);
				applog(3, errmsg, 0);
			}
			if (log4) {
				total = DAT_00079bfc;
				DAT_00079bfc = DAT_00079bfc + 1;
				snprintf(errmsg, 0x1000, "receive a error nonce. total = %u", total);
				applog(4, errmsg, 0);
			}
			return -1;
		}
	}

	/* 芯片地址一致性: 包 1..15 的 [1] 必须与包 0 相同 */
	for (n = 0; n < 0xf; n++) {
		if (pkts[(n + 1) * 0x5b + 1] != pkts[1]) {
			if (log3) {
				snprintf(errmsg, 0x1000, "chip_addr is different %02x != %02x\n",
					 pkts[1], pkts[(n + 1) * 0x5b + 1]);
				applog(3, errmsg, 0);
			}
			if (log4) {
				total = DAT_00079bfc;
				DAT_00079bfc = DAT_00079bfc + 1;
				snprintf(errmsg, 0x1000, "receive a error nonce. total = %u", total);
				applog(4, errmsg, 0);
			}
			return -1;
		}
	}

	/* workID 一致性: 包 1..15 的 [2] 必须与包 0 相同 */
	for (n = 0; n < 0xf; n++) {
		if (pkts[(n + 1) * 0x5b + 2] != pkts[2]) {
			if (log3) {
				snprintf(errmsg, 0x1000, "workID is different %02x != %02x\n",
					 pkts[2], pkts[(n + 1) * 0x5b + 2]);
				applog(3, errmsg, 0);
			}
			if (log4) {
				total = DAT_00079bfc;
				DAT_00079bfc = DAT_00079bfc + 1;
				snprintf(errmsg, 0x1000, "receive a error nonce. total = %u", total);
				applog(4, errmsg, 0);
			}
			return -1;
		}
	}

	/* nonceID 一致性: 包 1..15 的 [3] 必须与包 0 相同 */
	for (n = 0; n < 0xf; n++) {
		if (pkts[(n + 1) * 0x5b + 3] != pkts[3]) {
			if (log3) {
				snprintf(errmsg, 0x1000, "nonceID is different %02x != %02x chipid=%d\n",
					 pkts[3], pkts[(n + 1) * 0x5b + 3], chipid);
				applog(3, errmsg, 0);
			}
			if (log4) {
				total = DAT_00079bfc;
				DAT_00079bfc = DAT_00079bfc + 1;
				snprintf(errmsg, 0x1000, "receive a error nonce. total = %u", total);
				applog(4, errmsg, 0);
			}
			return -1;
		}
	}

	/* ---- 拼装 0x54D 字节条目 (dst 按 chipid 0x54D 分区) ----
	 * 包 0..14 各贡献 [4..0x58] 85 字节 (15×85=0x4FB); 包 15 贡献
	 * [4..0x48] 69 字节 (接 0x4FB..0x53F, 合计 0x540) 及尾部字段 */
	entry = dst + chipid * 0x54d;
	for (k = 0; k < 0xf; k++)
		memcpy(entry + k * 0x55, pkts + k * 0x5b + 4, 0x55);
	memcpy(entry + 0x4fb, pkts + 0x559, 0x45);
	*(uint32_t *)(entry + 0x548) = *(uint32_t *)(pkts + 0x5aa);
	entry[0x540] = pkts[0x556];            /* 包15[1]: chip_addr */
	entry[0x541] = pkts[0x557];            /* 包15[2]: workid */
	entry[0x542] = pkts[0x558];            /* 包15[3]: nonceID */
	entry[0x543] = pkts[0x5a1];            /* 包15[0x4C] */
	*(uint32_t *)(entry + 0x544) = *(uint32_t *)(pkts + 0x5a2);  /* 包15[0x4D..0x50] */

	/* ---- 推入 nonce fifo (0x7c298 起 100 × 0x54D 环形) ---- */
	pthread_mutex_lock(&DAT_00078e88);
	total = DAT_0007c28c;
	qbase = DAT_0007c28c * 0x54d;
	DAT_0007c7e4[qbase] = tag;
	DAT_0007c7d8[qbase] = entry[0x540];              /* chip_addr */
	DAT_0007c7d9[qbase] = entry[0x541] & 0x7f;       /* workid 低 7 位 */
	DAT_0007c7da[qbase] = entry[0x542];              /* nonceID */
	DAT_0007c7db[qbase] = entry[0x543];
	*(uint32_t *)(DAT_0007c7dc + qbase) = *(uint32_t *)(entry + 0x544);
	*(uint32_t *)(DAT_0007c7e0 + qbase) = *(uint32_t *)(entry + 0x548);
	memcpy((void *)(qbase + 0x7c298), entry, 0x540); /* 0x540 字节 nonce 数据体 */
	if (total < 99)
		DAT_0007c28c = total + 1;
	else
		DAT_0007c28c = 0;
	if (DAT_0007c294 < 100) {
		DAT_0007c294 = DAT_0007c294 + 1;
	} else {
		DAT_0007c294 = 100;
		if (log4) {
			snprintf(errmsg, 0x1000, "%s: nonce fifo full!!!", "nonce_handle");
			applog(4, errmsg, 0);
		}
	}
	pthread_mutex_unlock(&DAT_00078e88);
	return 0;
}
/* ==== dump_string 相关 (json.c, jansson dump.c 语义) ==== */
/* 输出回调: 成功返回 0, 失败非 0; 与 jansson dump 一致 */
typedef int (*dump_callback_t)(const char *buf, int len, void *data);

extern char     DAT_00060810[];   /* "\"" (开/闭引号串) */
extern char     DAT_0006cb30[];   /* "\\r" (回车转义串) */
extern uint8_t *utf8_decode_char(uint8_t *pos, uint32_t *codepoint); /* FUN_00059c18: 返回下一字符位置, 非法 UTF-8 返回 NULL */

// === dump_string === 原 FUN_00056e6c
// 来源: reversed/cgminer/20_job_mining/FUN_00056e6c.c | 分类: 20_job_mining
// 语义: jansson dump.c dump_string — 经回调输出带引号的 JSON 字符串字面量:
//   先输出 "\"", 再按 UTF-8 逐字符扫描, 无需转义的段整段输出; 遇 \\ " 控制
//   字符 (含 \\b \\t \\n \\f \\r) 及 flags&0x400 (JSON_ESCAPE_SLASH) 时的
//   '/'、flags&0x40 (JSON_ENSURE_ASCII) 时的 >0x7F 字符, 输出对应转义;
//   其余控制/非 ASCII 输出 \\u%04x, ≥0x10000 输出 UTF-16 代理对, 最后闭引号。
// 返回: 0 = 成功; -1 = 回调失败或非法 UTF-8。
int dump_string(const char *str, dump_callback_t emit, void *data, unsigned int flags)
{
	const char *pos;                  /* 原 param_1: 当前扫描位置 */
	const char *chunk;                /* 原 pcVar3: 未转义段起点 */
	const char *next;                 /* 原 pcVar2: 下一字符位置/闭引号判据 */
	uint32_t    codepoint;            /* 原 local_34 */
	int         ret;
	char        escbuf[20];           /* 原 acStack_30 */

	ret = emit(DAT_00060810, 1, data);           /* 开引号 "\"" */
	if (ret != 0)
		return -1;

	pos = str;
	chunk = str;
	do {
		next = NULL;
		while (*pos != '\0') {
			next = (const char *)utf8_decode_char((uint8_t *)pos, &codepoint);
			if (next == NULL)
				return -1;                /* 非法 UTF-8 */
			if (codepoint == 0x5c || codepoint == 0x22 || (int)codepoint < 0x20 ||
			    ((flags & 0x400) != 0 && codepoint == 0x2f) ||
			    ((flags & 0x40) != 0 && (int)codepoint > 0x7f))
				break;                    /* 需要转义 → 先输出未转义段 */
			pos = next;
		}
		if (next == NULL)                 /* 内循环因 *pos == 0 结束 */
			next = pos;

		if (pos != chunk) {
			ret = emit(chunk, (int)(pos - chunk), data);
			if (ret != 0)
				return -1;
		}
		if (next == pos) {                /* 无转义字符 → 输出闭引号收尾 */
			return emit(DAT_00060810, 1, data);
		}

		switch (codepoint) {
		case 0x08: ret = emit("\\b", 2, data); break;
		case 0x09: ret = emit("\\t", 2, data); break;
		case 0x0a: ret = emit("\\n", 2, data); break;
		case 0x0c: ret = emit("\\f", 2, data); break;
		case 0x0d: ret = emit(DAT_0006cb30, 2, data); break;   /* "\\r" */
		case 0x22: ret = emit("\\\"", 2, data); break;
		case 0x2f: ret = emit("\\/", 2, data); break;          /* 仅 flags&0x400 可达 */
		case 0x5c: ret = emit("\\\\", 2, data); break;
		default:
			if ((int)codepoint < 0x10000) {
				sprintf(escbuf, "\\u%04x", codepoint);
				ret = emit(escbuf, 6, data);
			} else {
				/* UTF-16 代理对: 高位 0xD800+(cp-0x10000)>>10, 低位 0xDC00+(cp&0x3FF) */
				uint32_t hi = ((codepoint - 0x10000) & 0xfffff) >> 10 | 0xd800;
				uint32_t lo = (codepoint & 0x3ff) | 0xdc00;
				sprintf(escbuf, "\\u%04x\\u%04x", hi, lo);
				ret = emit(escbuf, 0xc, data);
			}
			break;
		}
		if (ret != 0)
			return -1;

		pos = next;
		chunk = next;
	} while (1);
}
// === __floatundidf === 原 FUN_0005cb50
// 来源: reversed/cgminer/20_job_mining/FUN_0005cb50.c | 分类: 20_job_mining
// 语义: libgcc 无符号 64 位整数 → 双精度浮点转换 (ARM ABI: r0=低字/r1=高字,
//   Ghidra 显示为 (uint,uint) 两参)。原码为位级实现: 0 快速返回; 按 CLZ
//   (LZCOUNT) 归一化移位, 小值走左移路径、大值走右移路径, 舍入位进位
//   (round-to-nearest-even), 指数 ×0x100000 装配 (0x432 偏移), 溢出返回 +inf
//   (0x7ff0000000000000, 对 u64 输入域恒不可达)。此处以等价的标准算法转写,
//   对全部 uint64 输入与原码逐位一致 (非零输入恒为规格数, 无子正规输出)。
double __floatundidf(uint64_t value)
{
	union { uint64_t u; double d; } res;
	uint64_t mant, round_bits;
	uint32_t exp_field;
	int bitlen;

	if (value == 0)
		return 0.0;

	/* 位长 = 最高置位位置 + 1 (原 LZCOUNT: 高字 CLZ, 高字为 0 时低字 CLZ+32) */
	bitlen = 64;
	while (((value >> (bitlen - 1)) & 1) == 0)
		bitlen--;

	/* 归一化: 隐含位移到 bit63; 低 11 位为舍入位 (52 位尾数 + 隐含位) */
	mant = value << (64 - bitlen);
	round_bits = mant & 0x7ff;
	mant >>= 11;
	if (round_bits > 0x400 || (round_bits == 0x400 && (mant & 1) != 0))
		mant++;                                /* 原码: CARRY4(param_1, round) */

	exp_field = 1022 + bitlen;
	if ((mant >> 53) != 0) {                   /* 舍入进位 → 阶码 +1 */
		mant >>= 1;
		exp_field++;
	}

	res.u = ((uint64_t)exp_field << 52) | (mant & 0xFFFFFFFFFFFFFULL);
	return res.d;
}

// === __floatdidf === 原 FUN_0005cb64
// 来源: reversed/cgminer/20_job_mining/FUN_0005cb64.c | 分类: 20_job_mining
// 语义: libgcc 有符号 64 位整数 → 双精度浮点转换 (与 FUN_0005cb50 同一算法,
//   仅前置符号提取与 64 位取负: param_2 高字符号位 → 取负后转无符号装配,
//   结果最高位 OR 符号位; 0 直接返回 +0.0)。|value| ≤ 2^63, 溢出 +inf 路径
//   对 i64 输入域同样不可达。
double __floatdidf(int64_t value)
{
	union { uint64_t u; double d; } res;
	uint64_t uval, sign, mant, round_bits;
	uint32_t exp_field;
	int bitlen;

	if (value == 0)
		return 0.0;

	if (value < 0) {
		sign = 0x8000000000000000ULL;
		uval = -(uint64_t)value;               /* 原码: 低字取负, 高字 -(高+借位) */
	} else {
		sign = 0;
		uval = (uint64_t)value;
	}

	bitlen = 64;
	while (((uval >> (bitlen - 1)) & 1) == 0)
		bitlen--;

	mant = uval << (64 - bitlen);
	round_bits = mant & 0x7ff;
	mant >>= 11;
	if (round_bits > 0x400 || (round_bits == 0x400 && (mant & 1) != 0))
		mant++;

	exp_field = 1022 + bitlen;
	if ((mant >> 53) != 0) {
		mant >>= 1;
		exp_field++;
	}

	res.u = ((uint64_t)exp_field << 52) | (mant & 0xFFFFFFFFFFFFFULL) | sign;
	return res.d;
}

// ============================================================
// 覆盖率表 (9/9)
// ============================================================
// | 原函数         | 语义名              | 状态 |
// |----------------|---------------------|------|
// | FUN_000300b0   | sha256_compress     | 完成 |
// | FUN_000357c8   | summary             | 完成 |
// | FUN_00039f2c   | isdupnonce          | 完成 |
// | FUN_0003abf4   | hashmeter_thread    | 完成 |
// | FUN_00042b54   | fill_work_thread    | 完成 |
// | FUN_0004a754   | nonce_handle        | 完成 |
// | FUN_00056e6c   | dump_string         | 完成 |
// | FUN_0005cb50   | __floatundidf       | 完成 |
// | FUN_0005cb64   | __floatdidf         | 完成 |
