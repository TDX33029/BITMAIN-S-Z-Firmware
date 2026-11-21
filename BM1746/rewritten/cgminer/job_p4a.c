// ============================================================================
// job_p4a.c — cgminer 挖矿任务层 (前 9 个函数)
// 覆盖: 工作生成/过期判定/队列弹出/目标校验/extranonce 订阅/基准取工作/工具函数
// 来源: reversed/cgminer/20_job_mining/
// ============================================================================

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>

/* ==== 外部函数 (名取自 name_map/cgminer.tsv, 各批已建立同签名) ==== */

extern void  applog(uint32_t prio, char *buf, ...);            /* FUN_00038730: 第 3 参非 0 → 强制 curses 刷新 */
extern void  quit(int status);                                                /* FUN_00016ca8 */
extern void  mutex_lock_fail_cgminer(const char *fn, int line);               /* FUN_0000b414 */
extern void  wr_lock_error(const char *fn, int line);                         /* FUN_0000b480 */
extern void  rw_unlock_error(const char *fn, int line);                       /* FUN_0000b630 */
extern void  rd_lock_error(const char *fn, int line);                         /* FUN_0000b69c */
extern void  mutex_unlock_fail_cgminer(const char *fn, int line);             /* FUN_0000b5c4 */
extern int   hex2bin(uint8_t *dst, const uint8_t *src, uint32_t len);         /* FUN_00029830 */
extern char *bin2hex(const void *src, int len);                               /* FUN_00029730: malloc 新串返回 */
extern void  sha256_ctx_init(void *ctx);                                      /* FUN_000302d0 */
extern void  sha256_ctx_update(void *ctx, const void *data, int len);         /* FUN_00030304 */
extern int   varint_encode(uint8_t *p, int param_2, uint32_t value_lo, int value_hi); /* FUN_00016494 (job_p5.c 同款) */
extern void  calc_diff(void *work, double known_diff);                        /* FUN_00011a5c (job_p5.c 同款) */
extern void  cgtime(struct timeval *tv);                                      /* FUN_0002a84c: gettimeofday 包装 */
extern void  discard_work(void *work);                                        /* FUN_000159b4 */
extern void  wake_gws(void);                                                  /* FUN_00019840 */
extern void  set_thr_idle(void *thr);                                         /* FUN_0000fc58 */
extern void  set_thr_busy(void *thr);                                         /* FUN_0000fc24 */
extern int   hash_pop(int wait);                                              /* FUN_000193b4: 返回 work 指针 */
extern void *current_pool(void);                                              /* FUN_0001da58 */
extern int   stratum_send(void *pool, char *buf, size_t len);                 /* FUN_0002b5c8 */
extern int   socket_full(void *pool, unsigned int wait);                      /* FUN_00028afc: timeout 秒内 sockbuf 有数据 */
extern char *recv_line(void *pool);                                           /* FUN_0002b040: 无数据返回 NULL */
extern unsigned int parse_notify_equihash(void *pool, char *buf);             /* FUN_0002cf40: 非 0 = 方法消息已消化 */
extern void  json_delete(void *val);                                          /* FUN_0005a3e4 */
extern void *json_loads(const char *string, size_t flags, void *error);       /* FUN_00059230 (jansson) */
extern void *json_object_get(void *object, const char *key);                  /* FUN_00059e20 */
extern char *json_string_value(const void *value);                            /* FUN_0005a178 */
extern char *json_array_string_nc(void *array, int index);                    /* FUN_00029294 */
extern char *json_dumps(void *value, unsigned int flags);                     /* FUN_00057854 */

/* ==== 外部全局 (DAT_ 命名按约定保留, 语义由本批/先前批次核实) ==== */

extern char  *DAT_00076ffc;   /* benchfile 文件名 (opt_benchfile 参数值; job_p1.c 曾标 int 标志
                               * [低置信], 本文件 fopen(DAT_00076ffc,"r") 证实为 char*) */
extern FILE  *DAT_00076ff8;   /* benchfile 文件句柄 (打开后常驻, 读尽关闭重开) */
extern int    DAT_00077000;   /* benchfile 行计数 (含注释行) */
extern int    DAT_00077004;   /* benchfile 已产出工作计数 */
extern int    DAT_000770a8;   /* 全局总工作计数 (total_work_inc 在 control_lock 系锁下自增) */
extern int    DAT_000770ac;   /* 区块变更序号 block_nr (job_p2.c: 写入 work+0x720) */
extern uint32_t DAT_0007b4cc; /* local_work 计数 (job_p4b.c: API "Local Work" 字段) */
extern uint8_t  DAT_0007730d; /* opt_benchmark 标志 (job_p1.c) */
extern uint8_t  DAT_00078e09; /* opt_debug (chain.c/fan.c 同名) */
extern uint8_t  DAT_0007ad48; /* applog 门控 A: 非 0 → applog 走 syslog(prio|0x80) */
extern uint8_t  DAT_00078e08; /* applog 门控 B = opt_verbose */
extern int      DAT_000766c4; /* applog 门控 C: 日志级别 */
extern void   (*DAT_00075ed8)(void); /* 让步钩子: 单核系统置 sched_yield, 默认 noop_hook (恒空) */
extern int      DAT_00075df0; /* opt_scantime (freq_p4.c) */
extern int      DAT_00075df4; /* opt_expiry (freq_p4.c) */
extern uint8_t  DAT_0007b625; /* opt_failover_only (freq_p4.c) */
extern int      DAT_00076efc; /* mining_strategy: 3=load-balance 4=balance (axi.c/freq_p4.c) */
extern pthread_mutex_t *DAT_0007704c; /* getq 队列锁 (tq->mutex 别名, job_p1.c) */
extern void    *DAT_00077050; /* staged_work uthash 链表头 (job_p3.c 同名) */
extern pthread_cond_t DAT_0007b1a0;   /* gws_cond (job_p1.c) */
extern uint8_t  DAT_0007731c[0xa00];  /* benchmark 工作数据 A: 16×0xa0 解码模板 (job_p1.c) */
extern uint8_t  DAT_00077d1c[0xa00];  /* benchmark 工作数据 B: 16×0xa0 解码模板 */
extern int      DAT_0007b0f8; /* JSON-RPC 请求 id 计数器 (json.c 记为 pool_id 分配计数器) */
extern const signed char DAT_000655a8[]; /* 索引表 (log_work_file_index 用, 语义待定) */
extern const signed char DAT_0006569c[]; /* 索引表 (log_work_file_index 用, 语义待定) */

/* benchfile 前 4 个逗号分隔字段的描述表 @0x0005d6ac: 4 组 {int32 期望长度; const char *名称}
 * (0x5d6b0 = 名称子表); 第 5 字段 NonceTime 无逗号, 单独校验最短 10 字符 */
typedef struct { int32_t expect_len; const char *name; } benchfile_field_desc;
#define DAT_0005d6ac ((const benchfile_field_desc *)0x0005d6ac)

/* applog 调试门控 (chain.c LOG_GATE7 同款但不含 opt_debug, 各函数已另行判 opt_debug):
 * 门控 A(非 0 强制输出) || 门控 B(opt_verbose) || 日志级别 > 6 (LOG_DEBUG) */
#define LOG_GATE() (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 6 < DAT_000766c4)

// === suffix_string === 原 FUN_00011580 @ 0x00011580
// 来源: reversed/cgminer/20_job_mining/FUN_00011580.c | 分类: 20_job_mining
// 语义: 把 64 位数值按 SI 词头 (K/M/G/T/P/E) 缩放后格式化进 buf:
//       值 <1000 且 sigdigits==0 → "%d" 直排 (空词头); 其余 sigdigits==0 → "%.3g";
//       sigdigits!=0 → "%*.*f"(宽 = sigdigits+1, 精度 = sigdigits-1-floor(log10(scaled)),
//       scaled<=0 时对数项按 0)。≥1e6 各档原码经 __aeabi_uldivmod(FUN_0005cbc4) 整除
//       取商再经 __floatundidf(FUN_0005cb50) 转 double, 缩放值因此带整数截断, 此处用
//       C 无符号整除严格等价保留 (已对照 0x11580 处汇编: vdiv.f64 作用于商的 double)。
void suffix_string(uint64_t value, char *buf, size_t bufsiz, int sigdigits)
{
	double scaled;
	char suffix[3];
	int digits;

	suffix[0] = '\0';
	suffix[1] = '\0';

	if (value < 1000ULL) {
		scaled = (double)value;                          /* 原 __floatundidf(FUN_0005cb50) */
		if (sigdigits == 0) {
			snprintf(buf, bufsiz, "%d%s", (int)scaled, suffix);  /* 原经 VectorFloatToUnsigned 转回 */
			return;
		}
		/* sigdigits != 0 → 直落下方公共格式化 (原 LAB_00011660) */
	} else if (value >= 1000000000000000000ULL) {        /* ≥ 1e18 */
		scaled = (double)(uint64_t)(value / 1000000000000000ULL) / 1000.0;   /* v/1e15 → /1000 */
		suffix[0] = 'E';
	} else if (value >= 1000000000000000ULL) {           /* ≥ 1e15 */
		scaled = (double)(uint64_t)(value / 1000000000000ULL) / 1000.0;      /* v/1e12 → /1000 */
		suffix[0] = 'P';
	} else if (value >= 1000000000000ULL) {              /* ≥ 1e12 */
		scaled = (double)(uint64_t)(value / 1000000000ULL) / 1000.0;         /* v/1e9 → /1000 */
		suffix[0] = 'T';
	} else if (value >= 1000000000ULL) {                 /* ≥ 1e9 */
		scaled = (double)(uint64_t)(value / 1000000ULL) / 1000.0;            /* v/1e6 → /1000 */
		suffix[0] = 'G';
	} else if (value >= 1000000ULL) {                    /* ≥ 1e6 */
		scaled = (double)(uint64_t)(value / 1000ULL) / 1000.0;               /* v/1e3 → /1000 */
		suffix[0] = 'M';
	} else {                                             /* [1000, 1e6): 不取整, 直接除 */
		scaled = (double)value / 1000.0;
		suffix[0] = 'K';
	}

	if (sigdigits == 0) {
		snprintf(buf, bufsiz, "%.3g%s", scaled, suffix);
		return;
	}
	/* 原 LAB_00011660: scaled<=0 (软浮点 Z/N 标志比较) 时对数项取 0,
	 * 否则精度 = (sigdigits-1) - floor(log10(scaled)) */
	if (scaled > 0.0) {
		digits = (int)((double)(sigdigits - 1) - floor(log10(scaled)));
	} else {
		digits = (int)((double)(sigdigits - 1) - 0.0);
	}
	snprintf(buf, bufsiz, "%*.*f%s", sigdigits + 1, digits, scaled, suffix);
	return;
}

// === benchfile_get_work === 原 FUN_00017254 @ 0x00017254
// 来源: reversed/cgminer/20_job_mining/FUN_00017254.c | 分类: 20_job_mining
// 语义: 从 benchfile (DAT_00076ffc) 逐行取基准工作。跳过 '#'/'\0'/'/' 开头的注释行;
//       数据行按逗号切 5 字段 (前 4 字段按 0x0005d6ac 描述表校验期望长度, 第 5 字段
//       NonceTime 含行尾换行最短 10 字符), 组装 152 hex 字符: "0000000"+Version 首字符
//       (头字节 0-3) + 字段3 64 字符按 4 字节组倒序 (头字节 4-35, prevhash 位置) +
//       字段2 64 字符同法倒序 (头字节 36-67, merkle 位置) + %08lx(strtol(NonceTime,10))
//       (头字节 68-71) + 字段4 直接拼接 (头字节 72-75, 经 0x90 偏移拷入); hex2bin 76
//       字节入 work+0 (先 memset 清 work 0x7b0), 前 64 字节逐字 bswap 后做 SHA-256
//       一轮, 中间态 8 字 (上下文偏移 0x88) 写入 work+0x80。文件读尽: 无产出则 quit,
//       否则关文件重开并递归继续。返回 1 = 取到工作。
int benchfile_get_work(void *work)
{
	char buf[4096];            /* 原 local_1028..acStack_fa0 槽: 日志串与 hex 拼装共用 */
	char line[1024];           /* fgets 行缓冲 (原 local_1428) */
	uint32_t swap_buf[16];     /* 前 64 字节 bswap 暂存 (原 local_1510) */
	uint8_t sha_ctx[0xa8];     /* SHA-256 上下文 (原 auStack_14d0[136] + 其后 8 状态字槽) */
	char *fields[5];           /* 5 字段起点 (原 local_1524/local_1520[2]/local_1518/local_1514 槽链) */
	char *comma, *cursor, *rev;
	int field_no, i, off;
	size_t hex_len, field_len;
	long nonce_time;
	uint32_t word;

	if (DAT_00076ff8 == NULL) {
		if (DAT_00076ffc == NULL) {
			applog(3, "BENCHFILE Invalid benchfile NULL", 1);
			quit(1);
		}
		DAT_00076ff8 = fopen(DAT_00076ffc, "r");
		if (DAT_00076ff8 == NULL) {
			snprintf(buf, 0x1000, "BENCHFILE Failed to open benchfile '%s'", DAT_00076ffc);
			applog(3, buf, 1);
			quit(1);
		}
		DAT_00077000 = 0;
		if (fgets(line, 0x400, DAT_00076ff8) == NULL) {
			snprintf(buf, 0x1000, "BENCHFILE Failed to read benchfile '%s'", DAT_00076ffc);
			applog(3, buf, 1);
			quit(1);
		}
		DAT_00077004 = 0;
	} else {
		if (fgets(line, 0x400, DAT_00076ff8) == NULL) {
			goto lab_eof_restart;          /* 原 joined_r0x000172e4 */
		}
	}

	do {
		DAT_00077000++;                    /* benchfile_line++ (注释行也计数) */
		if (line[0] != '#' && line[0] != '\0' && line[0] != '/') {
			/* ---- 数据行: 逐逗号切出前 4 字段 (原 do-while, field 计数 1..4) ---- */
			field_no = 1;
			cursor = line;
			for (i = 0; i < 4; i++) {
				comma = strchr(cursor, ',');
				if (comma == NULL) {
					snprintf(buf, 0x1000, "BENCHFILE Invalid input file line %d - field count is %d but should be %d",
					         DAT_00077000, field_no, 5);
					applog(3, buf, 1);
					quit(1);
				}
				if (DAT_0005d6ac[i].expect_len != 0 && (int)(comma - cursor) != DAT_0005d6ac[i].expect_len) {
					snprintf(buf, 0x1000, "BENCHFILE Invalid input file line %d field %d (%s) length is %d but should be %d",
					         DAT_00077000, field_no, DAT_0005d6ac[i].name, (int)(comma - cursor),
					         DAT_0005d6ac[i].expect_len);
					applog(3, buf, 1);
					quit(1);
				}
				fields[i] = cursor;
				*comma = '\0';
				cursor = comma + 1;
				field_no++;
			}
			fields[4] = cursor;            /* 第 5 字段 NonceTime (含行尾换行) */

			field_len = strlen(fields[4]);
			if ((int)field_len < 10) {
				snprintf(buf, 0x1000, "BENCHFILE Invalid input file line %d field %d (%s) length is %d but should be least %d",
				         DAT_00077000, 5, "NonceTime", (int)field_len, 10);
				applog(3, buf, 1);
				quit(1);
			}

			/* ---- 组装 152 hex 字符的区块头前 76 字节 ---- */
			sprintf(buf, "0000000%c", *fields[0]);    /* Version 字段首字符 → 头字节 0-3 */
			hex_len = strlen(buf);                     /* = 8 */
			rev = fields[2] + 0x38;                    /* 字段 3 (prevhash 位置): 从尾部 8 字符组向前 */
			for (off = (int)hex_len; off != (int)hex_len + 0x40; off += 8) {
				sprintf(buf + off, "%.8s", rev);
				rev -= 8;
			}
			rev = fields[1] + 0x38;                    /* 字段 2 (merkle 位置): 同法倒序 */
			for (; off != (int)hex_len + 0x80; off += 8) {
				sprintf(buf + off, "%.8s", rev);
				rev -= 8;
			}
			nonce_time = strtol(fields[4], (char **)0, 10);
			sprintf(buf + off, "%08lx", (unsigned long)nonce_time);   /* 头字节 68-71 */
			strcpy(buf + hex_len + 0x88, fields[3]);   /* 字段 4 → 偏移 0x90 (头字节 72-75) */

			/* ---- 解码入 work 并计算 SHA-256 中间态 ---- */
			memset(work, 0, 0x7b0);
			hex2bin(work, (const uint8_t *)buf, (uint32_t)(hex_len + 0x90) >> 1);      /* 0x98>>1 = 76 字节 */
			for (i = 0; i < 0x40; i += 4) {
				word = *(uint32_t *)((uint8_t *)work + i);
				*(uint32_t *)((uint8_t *)swap_buf + i) =
				    word << 0x18 | (word >> 8 & 0xff) << 0x10 |
				    (word >> 0x10 & 0xff) << 8 | word >> 0x18;
			}
			sha256_ctx_init(sha_ctx);
			sha256_ctx_update(sha_ctx, swap_buf, 0x40);
			DAT_00077004++;                            /* benchfile_work++ */
			memcpy((uint8_t *)work + 0x80, sha_ctx + 0x88, 0x20);     /* 中间态 8 字 → work+0x80 */
			return 1;
		}
	} while (fgets(line, 0x400, DAT_00076ff8) != NULL);

lab_eof_restart:
	if (DAT_00077004 == 0) {
		snprintf(buf, 0x1000, "BENCHFILE No work in benchfile '%s'", DAT_00076ffc);
		applog(3, buf, 1);
		quit(1);
	}
	fclose(DAT_00076ff8);
	DAT_00076ff8 = NULL;
	return benchfile_get_work(work);   /* 原码尾递归: 关闭后重开从头再取 */
}

// === gen_stratum_work === 原 FUN_0001acc4 @ 0x0001acc4
// 来源: reversed/cgminer/20_job_mining/FUN_0001acc4.c | 分类: 20_job_mining
// 语义: 由 stratum 池模板生成一份挖矿工作。data_lock (pool+0xf4) + 写锁 (pool+0x10c)
//       下取 pool->nonce2 (pool+0x288, 64 位) 写入 work+0x6f8 并自增池侧计数,
//       work->nonce2_len (+0x700) = 2; 降级为读锁后拼装 work+0xe0 起的挖矿输入:
//       清 0x5cf 字节、拷贝 pool+0x644 模板 0x80 字节、nonce1 串十六进制解码到 +0x14c、
//       nonce2 二进制回填到 0x160-nonce2_len、在 +0x16c 编码 compactsize(0x540=1344,
//       Equihash 求解长度; 汇编证实 r2=0x540/r3=0); 随后复制池难度 double
//       (pool+0x2c8 → work+0x708)、strdup job_id/nonce1/ntime 三串; 解锁 + 让步钩子 +
//       opt_debug 两级日志后, 拷贝 32 字节目标 (pool+0x48 → work+0xa0)、登记总工作
//       计数与 block_nr、打标 'S' (+0x788) 与 60 (+0x6c4), calc_diff(work, work+0x708
//       处难度) 并 cgtime(work+0x6d8) 打时间戳。
void gen_stratum_work(void *pool, void *work)
{
	uint8_t *pl = (uint8_t *)pool;
	uint8_t *wk = (uint8_t *)work;
	pthread_rwlock_t *data_rwlock;
	char *nonce1;
	char *header_hex;
	char msg[4096];
	uint32_t nonce2_lo, nonce2_hi;
	size_t nonce1_hexlen, nonce2_len;
	int rc;

	rc = pthread_mutex_lock((pthread_mutex_t *)(pl + 0xf4));       /* pool->data_lock */
	if (rc != 0) {
		mutex_lock_fail_cgminer("gen_stratum_work", 0x1f0d);
	}
	data_rwlock = (pthread_rwlock_t *)(pl + 0x10c);
	rc = pthread_rwlock_wrlock(data_rwlock);
	if (rc != 0) {
		wr_lock_error("gen_stratum_work", 0x1f0d);
	}

	nonce2_lo = *(uint32_t *)(pl + 0x288);
	nonce2_hi = *(uint32_t *)(pl + 0x28c);
	*(uint32_t *)(wk + 0x6f8) = nonce2_lo;                         /* work->nonce2 (64 位) */
	*(uint32_t *)(wk + 0x6fc) = nonce2_hi;
	*(uint32_t *)(pl + 0x288) = nonce2_lo + 1;                     /* pool->nonce2++ */
	/* 进位: 原码 (uint)(0xfffffffe < lo) ⇔ lo 自增溢出 */
	*(uint32_t *)(pl + 0x28c) = nonce2_hi + (uint32_t)(0xfffffffe < nonce2_lo);
	*(uint32_t *)(wk + 0x700) = 2;                                 /* work->nonce2_len */

	rc = pthread_rwlock_unlock(data_rwlock);
	if (rc != 0) {
		rw_unlock_error("gen_stratum_work", 0x1f12);
	}
	rc = pthread_rwlock_rdlock(data_rwlock);                       /* 写锁降级为读锁 */
	if (rc != 0) {
		rd_lock_error("gen_stratum_work", 0x1f12);
	}
	rc = pthread_mutex_unlock((pthread_mutex_t *)(pl + 0xf4));
	if (rc != 0) {
		mutex_unlock_fail_cgminer("gen_stratum_work", 0x1f12);
	}

	/* ---- 拼装挖矿输入区 work+0xe0 (共 0x5cf 字节) ---- */
	memset(wk + 0xe0, 0, 0x5cf);
	memcpy(wk + 0xe0, pl + 0x644, 0x80);                           /* 池侧模板 128 字节 */
	nonce1 = *(char **)(pl + 0x27c);                               /* pool->nonce1 (hex 串) */
	nonce1_hexlen = strlen(nonce1);
	hex2bin(wk + 0x14c, (const uint8_t *)nonce1, nonce1_hexlen >> 1);
	nonce2_len = *(uint32_t *)(wk + 0x700);
	memcpy(wk + (0x160 - nonce2_len), wk + 0x6f8, nonce2_len);     /* nonce2 回填 coinbase */
	/* compactsize(0x540 = 1344 = Equihash 求解长度); 汇编: r1=&work->nonce2 (参数 2 未被
	 * 函数体使用), r2=0x540, r3=0 → 输出 fd 40 05 */
	varint_encode(wk + 0x16c, (int)(intptr_t)(wk + 0x6f8), 0x540, 0);

	/* 难度 double: pool+0x2c8 (8 字节) → work+0x708, 供 calc_diff 实参 [pool 字段语义待定] */
	*(uint32_t *)(wk + 0x708) = *(uint32_t *)(pl + 0x2c8);
	*(uint32_t *)(wk + 0x70c) = *(uint32_t *)(pl + 0x2cc);
	*(char **)(wk + 0x6f0) = strdup(*(char **)(pl + 0x2a0));       /* work->job_id */
	*(char **)(wk + 0x710) = strdup(*(char **)(pl + 0x27c));       /* work->nonce1 (串) */
	*(char **)(wk + 0x704) = strdup(*(char **)(pl + 0x2b4));       /* work->ntime (串) */

	rc = pthread_rwlock_unlock(data_rwlock);
	if (rc != 0) {
		rw_unlock_error("gen_stratum_work", 0x1f27);
	}
	(*DAT_00075ed8)();                                             /* 让步钩子 */

	if (DAT_00078e09 != '\0') {                                    /* opt_debug */
		header_hex = bin2hex(wk + 0xe0, 0x8f);                     /* 143 字节挖矿输入 → hex */
		if (LOG_GATE()) {
			snprintf(msg, 0x1000, "[THR%d] Generated stratum header %s",
			         *(int *)(wk + 0x6d0), header_hex);
			applog(7, msg, 0);
		}
		if (6 < DAT_000766c4) {
			snprintf(msg, 0x1000, "[THR%d] job_id %s, nonce1 %s, nonce2 %llu, ntime %s",
			         *(int *)(wk + 0x6d0), *(char **)(wk + 0x6f0), *(char **)(wk + 0x710),
			         *(unsigned long long *)(wk + 0x6f8), *(char **)(wk + 0x704));
			applog(7, msg, 0);
		}
		free(header_hex);
	}

	/* ---- 登记: 目标 / 计数 / 池指针 / 标志 ---- */
	memcpy(wk + 0xa0, pl + 0x48, 0x20);                            /* 32 字节目标 ← pool+0x48 */
	*(int32_t *)(wk + 0x724) = DAT_000770a8;                       /* work->id = 总工作计数 */
	DAT_000770a8 = DAT_000770a8 + 1;
	*(uint32_t *)(wk + 0x720) = (uint32_t)DAT_000770ac;            /* block_nr */
	DAT_0007b4cc++;                                                /* local_work 计数 */
	*(void **)(wk + 0x6d4) = pool;                                 /* work->pool */
	*(uint8_t *)(wk + 0x6ec) = 1;                                  /* stratum 标志 */
	*(uint8_t *)(wk + 0x6e8) = 0;                                  /* longpoll = false */
	*(uint8_t *)(wk + 0x788) = 0x53;                               /* 'S' = stratum (基准路径写 'B') */
	*(uint32_t *)(wk + 0x6c4) = 0x3c;                              /* 60 (基准路径写 0) [语义待定] */
	calc_diff(work, *(double *)(wk + 0x708));                      /* 汇编: vldr d0,[work+0x708] */
	cgtime((struct timeval *)(wk + 0x6d8));                        /* work->tv_staged 时间戳 */
	return;
}

// === stale_work === 原 FUN_00022b44 @ 0x00022b44
// 来源: reversed/cgminer/20_job_mining/FUN_00022b44.c | 分类: 20_job_mining
// 语义: 判定一份 work 是否过期。返回 1 = stale (应丢弃), 0 = 仍可用。
//       - 基准 (DAT_0007730d) 或 benchfile (DAT_00076ffc) 模式恒返回 0 (原 LAB_00022b74);
//       - work->block_nr (+0x720) != 当前块序号 DAT_000770ac → "block mismatch" 过期;
//       - share==0 且池带 stratum (+0x298): stratum_active(+0x299) 或 +0x29b 字节为 0
//         → "stratum inactivity" 过期; 否则 data_lock+读锁下 strcmp(work->job_id(+0x6f0),
//         pool->swork.job_id(+0x2a0)) 不一致 → "job_id mismatch" 过期 (均先解锁再判日志);
//       - 一致/非 stratum 路径 (原 LAB_00022cd8) 按有效期窗口判定: 窗口 = work->expiry
//         (+0x6e4, ≤ opt_scantime 时改取 opt_expiry) 减 (int)(pool+0x1d8 double * 5 + 1),
//         下限 5 秒; now - work->tv(+0x6d8).tv_sec 超窗 → "expiry" 过期; 未超窗且
//         (!opt_failover_only || share || 是当前池 || work+0x6ea mandatory || 策略 3/4)
//         → 返回 0, 否则 "fail only pool mismatch" 过期。
//       原码全部过期分支在日志门未开时经公共落点 uVar1=1 返回, 此处结构化为显式 return,
//       行为等价。
int stale_work(void *work, int share)
{
	uint8_t *wk = (uint8_t *)work;
	uint8_t *pool;
	int expiry, window, rc, rc2;
	struct timeval now;

	if (DAT_0007730d == '\0' && DAT_00076ffc == 0) {
		if (*(uint32_t *)(wk + 0x720) != (uint32_t)DAT_000770ac) {
			if (DAT_00078e09 != '\0' && LOG_GATE()) {
				applog(7, "Work stale due to block mismatch", 0);
			}
			return 1;
		}

		pool = *(uint8_t **)(wk + 0x6d4);              /* work->pool */
		expiry = *(int32_t *)(wk + 0x6e4);             /* work->expiry [语义待定] */
		if (*(int32_t *)(wk + 0x6e4) <= DAT_00075df0) {
			expiry = DAT_00075df4;                     /* opt_expiry 兜底 */
		}

		if (share == 0 && *(uint8_t *)(pool + 0x298) != '\0') {    /* pool->has_stratum */
			if (*(uint8_t *)(pool + 0x299) == '\0' || *(uint8_t *)(pool + 0x29b) == '\0') {
				/* stratum 会话未就绪 (+0x29b 字节语义待定) */
				if (DAT_00078e09 != '\0' && LOG_GATE()) {
					applog(7, "Work stale due to stratum inactivity", 0);
				}
				return 1;
			}
			/* data_lock + 读锁下比对 job_id (锁序: mutex → rdlock → mutex 释放) */
			rc = pthread_mutex_lock((pthread_mutex_t *)(pool + 0xf4));
			if (rc != 0) {
				mutex_lock_fail_cgminer("stale_work", 0x1347);
			}
			rc = pthread_rwlock_rdlock((pthread_rwlock_t *)(pool + 0x10c));
			if (rc != 0) {
				rd_lock_error("stale_work", 0x1347);
			}
			rc = pthread_mutex_unlock((pthread_mutex_t *)(pool + 0xf4));
			if (rc != 0) {
				mutex_unlock_fail_cgminer("stale_work", 0x1347);
			}
			rc = strcmp(*(char **)(wk + 0x6f0), *(char **)(pool + 0x2a0));
			rc2 = pthread_rwlock_unlock((pthread_rwlock_t *)(pool + 0x10c));
			if (rc2 != 0) {
				rw_unlock_error("stale_work", 0x134a);
			}
			(*DAT_00075ed8)();                         /* 让步钩子 */
			if (rc != 0) {
				if (DAT_00078e09 != '\0' && LOG_GATE()) {
					applog(7, "Work stale due to stratum job_id mismatch", 0);
				}
				return 1;
			}
		}

		/* 原 LAB_00022cd8: 有效期窗口判定 */
		window = expiry - (int)(*(double *)(pool + 0x1d8) * 5.0 + 1.0);   /* pool+0x1d8 double [语义待定] */
		cgtime(&now);
		if (window < 5) {
			window = 5;
		}
		if (now.tv_sec - *(int32_t *)(wk + 0x6d8) < window) {
			/* 未超窗: failover-only 下仅当前池/强制工作可继续 */
			if (DAT_0007b625 == '\0' || share != 0 ||
			    pool == (uint8_t *)current_pool() ||
			    *(uint8_t *)(wk + 0x6ea) != '\0' ||    /* mandatory (挖中区块) */
			    DAT_00076efc == 3 || DAT_00076efc == 4) {
				return 0;                              /* 原 LAB_00022b74: 不过期 */
			}
			if (DAT_00078e09 != '\0' && LOG_GATE()) {
				applog(7, "Work stale due to fail only pool mismatch", 0);
			}
			return 1;
		}
		if (DAT_00078e09 != '\0' && LOG_GATE()) {
			applog(7, "Work stale due to expiry", 0);
		}
		return 1;
	}
	/* 基准 / benchfile 模式: 恒不过期 (原 LAB_00022b74) */
	return 0;
}

// === discard_stale === 原 FUN_00022f88 @ 0x00022f88
// 来源: reversed/cgminer/20_job_mining/FUN_00022f88.c | 分类: 20_job_mining
// 语义: 在 getq 锁 (DAT_0007704c) 下遍历 staged_work uthash 链 (头 DAT_00077050,
//       hh 句柄位于 work+0x728): 预读 hh.next(+0x730) 作迭代续点, 对每项调
//       stale_work(w,0); 判过期者执行内联 HASH_DELETE —— 空表 (prev/next 均空) 时
//       free(buckets)+free(tbl)+清链头, 否则维护 tbl->tail(+0x10)/hho(+0x14)、
//       hh.prev(+0x72c)/hh.next(+0x730) 交叉回链、桶计数 (桶项+4) 与桶头指针、
//       hh_prev(+0x734)/hh_next(+0x738) 与 tbl->num_items(+0xc) —— 随后 discard_work。
//       结束后 gws_cond (DAT_0007b1a0) 发信号、解锁、让步钩子; 丢弃数 >0 且 opt_debug
//       与调试门开时打印 "Discarded %d stales that didn't match current hash"。
//       (uthash 布局与 job_p3.c hash_pop 内联删除一致。)
void discard_stale(void)
{
	uint8_t *w, *next, *prev_item;
	uint32_t *hh_tbl, *buckets;
	uint32_t bucket_off;
	int was_bucket_head;
	int rc, discarded;
	char msg[4096];

	rc = pthread_mutex_lock(DAT_0007704c);
	if (rc != 0) {
		mutex_lock_fail_cgminer("discard_stale", 0x145e);
	}
	discarded = 0;
	if (DAT_00077050 != 0) {
		w = (uint8_t *)DAT_00077050;
		next = *(uint8_t **)(w + 0x730);               /* 预读 hh.next, 当前节点可能被删 */
		for (;;) {
			if (stale_work(w, 0) != 0) {
				/* ---- 内联 HASH_DELETE (hh = work+0x728) ---- */
				hh_tbl = *(uint32_t **)((uint8_t *)DAT_00077050 + 0x728);   /* work->hh.tbl (同一表) */
				prev_item = *(uint8_t **)(w + 0x72c);   /* hh.prev (指向元素) */
				if (prev_item == 0 && *(uint8_t **)(w + 0x730) == 0) {
					/* 表中仅剩此元素: 释放 buckets + tbl, 链头清空 */
					free(*(void **)hh_tbl);             /* tbl->buckets */
					free(hh_tbl);
					DAT_00077050 = 0;
				} else {
					if (*(int32_t *)((uint8_t *)hh_tbl + 0x10) -        /* tbl->tail */
					    *(int32_t *)((uint8_t *)hh_tbl + 0x14) == (int32_t)(intptr_t)w) {
						/* 尾元素被删: tbl->tail = hh.prev + tbl->hho */
						*(int32_t *)((uint8_t *)hh_tbl + 0x10) =
						    (int32_t)(intptr_t)prev_item + *(int32_t *)((uint8_t *)hh_tbl + 0x14);
					}
					if (prev_item == 0) {
						/* 被删的是链头: head = hh.next (DECLTYPE_ASSIGN) */
						DAT_00077050 = *(void **)(w + 0x730);
					} else {
						*(uint32_t *)(prev_item + 0x728 + 8) = *(uint32_t *)(w + 0x730);  /* prev->hh.next */
					}
					if (*(uint32_t *)(w + 0x730) != 0) {
						*(uint32_t *)(*(uint32_t *)(w + 0x730) + 0x728 + 4) =
						    *(uint32_t *)(w + 0x72c);   /* next->hh.prev */
					}
					/* 桶维护: 桶号 = (num_buckets-1) & hashv, 桶项 12 字节 {头指针, 计数, ...} */
					bucket_off = ((*(uint32_t *)((uint8_t *)hh_tbl + 4) - 1U) &
					              *(uint32_t *)(w + 0x744)) * 0xc;
					buckets = *(uint32_t **)hh_tbl;
					*(int32_t *)((uint8_t *)buckets + bucket_off + 4) -= 1;   /* 桶计数-- */
					was_bucket_head = (*(int32_t *)((uint8_t *)buckets + bucket_off) ==
					                   (int32_t)(intptr_t)(w + 0x728));
					if (was_bucket_head) {
						/* 桶头原指向 &w->hh → 改挂 hh.hh_next (+0x738) */
						*(uint32_t *)((uint8_t *)buckets + bucket_off) =
						    *(uint32_t *)(w + 0x738);
					}
					if (*(uint32_t *)(w + 0x734) != 0) {                     /* hh_prev */
						*(uint32_t *)(*(uint32_t *)(w + 0x734) + 0x10) =
						    *(uint32_t *)(w + 0x738);
					}
					if (*(uint32_t *)(w + 0x738) != 0) {                     /* hh_next */
						*(uint32_t *)(*(uint32_t *)(w + 0x738) + 0xc) =
						    *(uint32_t *)(w + 0x734);
					}
					/* tbl->num_items-- (原码经删除后的链头取 tbl, 与 hh_tbl 同一表对象) */
					*(int32_t *)((uint8_t *)hh_tbl + 0xc) -= 1;
				}
				discarded++;
				discard_work(w);
			}
			if (next == 0) {
				break;
			}
			w = next;
			next = *(uint8_t **)(w + 0x730);
		}
	}
	pthread_cond_signal((pthread_cond_t *)&DAT_0007b1a0);          /* gws_cond 唤醒 */
	rc = pthread_mutex_unlock(DAT_0007704c);
	if (rc != 0) {
		mutex_unlock_fail_cgminer("discard_stale", 0x1469);
	}
	(*DAT_00075ed8)();                                             /* 让步钩子 */
	if (discarded != 0 && DAT_00078e09 != '\0' && LOG_GATE()) {
		snprintf(msg, 0x1000, "Discarded %d stales that didn't match current hash", discarded);
		applog(7, msg, 0);
	}
	return;
}

// === get_work === 原 FUN_000247d4 @ 0x000247d4
// 来源: reversed/cgminer/20_job_mining/FUN_000247d4.c | 分类: 20_job_mining
// 语义: 挖矿线程取工作主入口 (get_work(thr, thr_id), 返回 work 指针)。
//       set_thr_idle(thr) 标记等待 (汇编证实实参 = thr, Ghidra 未显示); opt_debug +
//       调试门开时打印 "Popping work from get queue to get work"; 计时段内循环
//       hash_pop(1) 弹出工作, stale_work 判过期则 discard_work + wake_gws 后重弹;
//       整段阻塞秒数 >0 时累加到关联设备块 +0xe8 并 (调试门开) 打印
//       "Get work blocked for %d seconds"。取到后 work->thr_id (+0x6d0) = thr_id;
//       opt_benchmark 下按设备块 +0x17c (往返计数) / +0x180 (方向 ±1) / +0x178 (A 组
//       序号) 从 DAT_00077d1c (B 组, 按计数) 或 DAT_0007731c (A 组, 折返时按序号)
//       选 0xa0 字节模板 memcpy 覆盖 work 头部。随后 set_thr_busy(thr)、work+0x6e0
//       置 1 (discard_work 检查字节), 把 work+0x748 的难度值经设备内层指针 (设备块
//       +0x04) 的 +0x68/+0x78 double 区间夹取后写入 work+0x6b0。
//       原码 "Get work blocked/Got work" 两日志的 cVar4 门控推导后均为
//       opt_debug && LOG_GATE(), 此处直接写为等价条件。
void *get_work(void *thr, int thr_id)
{
	uint8_t *dev;                    /* thr+0x24 → 关联设备块 (set_thr_idle 写其 +0x60/+0xf0) */
	void *work;
	time_t t_start, t_end;
	int blocked_secs, bench_idx;
	char msg[4096];
	double clamped, clamp_hi;

	dev = *(uint8_t **)((uint8_t *)thr + 0x24);
	set_thr_idle(thr);
	if (DAT_00078e09 != '\0' && LOG_GATE()) {
		applog(7, "Popping work from get queue to get work", 0);
	}

	t_start = time(NULL);
	do {
		for (;;) {
			work = (void *)(intptr_t)hash_pop(1);
			if (stale_work(work, 0) == 0) {
				break;
			}
			discard_work(work);
			wake_gws();
		}
	} while (work == 0);
	t_end = time(NULL);
	blocked_secs = (int)(t_end - t_start);

	if (blocked_secs > 0) {
		*(int32_t *)(dev + 0xe8) += blocked_secs;    /* 阻塞耗时累计 (设备块) */
		if (DAT_00078e09 != '\0' && LOG_GATE()) {
			snprintf(msg, 0x1000, "Get work blocked for %d seconds", blocked_secs);
			applog(7, msg, 0);
		}
	}
	if (DAT_00078e09 != '\0' && LOG_GATE()) {
		snprintf(msg, 0x1000, "Got work from get queue to get work for thread %d", thr_id);
		applog(7, msg, 0);
	}

	*(int32_t *)((uint8_t *)work + 0x6d0) = thr_id;  /* work->thr_id */

	if (DAT_0007730d != '\0') {                      /* opt_benchmark: 模板轮换 */
		bench_idx = *(int32_t *)(dev + 0x17c) + *(int32_t *)(dev + 0x180);
		*(int32_t *)(dev + 0x17c) = bench_idx;       /* 往返计数推进 */
		if (bench_idx > 0xf) {
			/* 计数超出 0..15: 方向反转, A 组按序号 (+0x178) 循环选取 */
			bench_idx = *(int32_t *)(dev + 0x178) + 1;
			*(int32_t *)(dev + 0x180) = -1;          /* 0xffffffff = -1 */
			if (bench_idx > 0xf) {
				bench_idx = 0;
			}
			*(int32_t *)(dev + 0x178) = bench_idx;
			memcpy(work, DAT_0007731c + bench_idx * 0xa0, 0xa0);
		} else {
			if (bench_idx < 1) {
				*(int32_t *)(dev + 0x180) = 1;       /* 计数触底: 方向转正 */
			}
			memcpy(work, DAT_00077d1c + bench_idx * 0xa0, 0xa0);   /* B 组按计数选取 */
		}
	}

	/* 原 LAB_0002491c */
	set_thr_busy(thr);
	*(uint8_t *)((uint8_t *)work + 0x6e0) = 1;
	/* 难度夹取: max(min(dev_inner+0x68, work+0x748), dev_inner+0x78) → work+0x6b0
	 * (dev_inner = 设备块 +0x04 处内层指针; +0x68/+0x78 double [语义待定: 疑似难度界限]) */
	clamped = *(double *)(*(int32_t *)(dev + 4) + 0x68);
	if (*(double *)((uint8_t *)work + 0x748) < clamped) {
		clamped = *(double *)((uint8_t *)work + 0x748);
	}
	*(double *)((uint8_t *)work + 0x6b0) = clamped;
	clamp_hi = *(double *)(*(int32_t *)(dev + 4) + 0x78);
	if (*(double *)((uint8_t *)work + 0x6b0) < clamp_hi) {
		*(double *)((uint8_t *)work + 0x6b0) = clamp_hi;
	}
	return work;
}

// === fulltest === 原 FUN_00029f54 @ 0x00029f54
// 来源: reversed/cgminer/20_job_mining/FUN_00029f54.c | 分类: 20_job_mining
// 语义: 256 位哈希与目标逐 u32 比较 (自最高字 word[7] 向低字): hash > target → 返回 0
//       (false positive), hash < target 提前退出返回 1, 全部相等亦返回 1。
//       opt_debug + 调试门开时把两个 32 字节串整体字节反序 (字内 bswap + 字序倒排,
//       原 16 个槽的展开还原为循环) 后 bin2hex, 打印
//       " Proof: %s\nTarget: %s\nTrgVal? %s" (ret!=0 → "YES (hash <= target)")。
int fulltest(const uint32_t *hash, const uint32_t *target)
{
	int i, ret;
	uint32_t proof_sw[8], target_sw[8];     /* 原局部槽 uStack_1068..local_102c */
	char *proof_hex, *target_hex;
	const char *verdict;
	char msg[4096];

	ret = 1;
	for (i = 7; i >= 0; i--) {
		if (target[i] < hash[i]) {          /* 原 if (*puVar2 < *puVar9) → iVar3 = 0 */
			ret = 0;
			break;
		}
		if (target[i] > hash[i]) {          /* 原 while 条件 (*t <= *h) 不成立 → 退出置 1 */
			break;
		}
	}

	if (DAT_00078e09 != '\0') {
		for (i = 0; i < 8; i++) {           /* 整串字节反序 (原 16 处展开的等价循环) */
			proof_sw[7 - i] = hash[i] << 0x18 | (hash[i] >> 8 & 0xff) << 0x10 |
			                  (hash[i] >> 0x10 & 0xff) << 8 | hash[i] >> 0x18;
			target_sw[7 - i] = target[i] << 0x18 | (target[i] >> 8 & 0xff) << 0x10 |
			                   (target[i] >> 0x10 & 0xff) << 8 | target[i] >> 0x18;
		}
		proof_hex = bin2hex(proof_sw, 0x20);
		target_hex = bin2hex(target_sw, 0x20);
		if (LOG_GATE()) {
			verdict = "no (false positive; hash > target)";
			if (ret != 0) {
				verdict = "YES (hash <= target)";
			}
			snprintf(msg, 0x1000, " Proof: %s\nTarget: %s\nTrgVal? %s", proof_hex, target_hex, verdict);
			applog(7, msg, 0);
		}
		free(proof_hex);
		free(target_hex);
	}
	return ret;
}

// === extranonce_subscribe === 原 FUN_0002e69c @ 0x0002e69c
// 来源: reversed/cgminer/20_job_mining/FUN_0002e69c.c | 分类: 20_job_mining
// 语义: 向池发送 {"id": N, "method": "mining.extranonce.subscribe", "params": []}
//       (id 取全局 JSON-RPC 计数器 DAT_0007b0f8 并自增); 发送失败返回 0。
//       socket_full(pool,2) 有数据期间 recv_line 循环: parse_notify_equihash 认定为
//       方法消息 (非 0) 则跳过继续等; 否则 JSON 解析取 result/error:
//       - result 缺失或 false (jansson type=6): error 缺失 → "(unknown reason)" 打
//         "Pool %d JSON extranonce.subscribe failed: %s" 返回 0; error 存在 → 与
//         正常路径共用错误提取 (原 LAB_0002e81c);
//       - result 有效且 error 缺失或 null (type=7) → "Stratum extranonce subscribe for
//         pool %d" 返回 1 (原 LAB_0002e8fc);
//       - error 存在: 原因串 (json_array_string_nc(error,1), 退 json_string_value)
//         等于 "Method 'subscribe' not found for service 'mining.extranonce'" 或
//         "Unrecognized request provided" → "Cannot subscribe to mining.extranonce
//         for pool %d" 返回 1; 否则 json_dumps(error,3) 打 failed 返回 0。
//       socket_full 超时退出 → opt_debug+调试门开时打 "Timed out waiting for response
//       extranonce.subscribe" 返回 1。响应处理完对解析对象内联 json_decref
//       (jansson json_t: +0 type 枚举, +4 refcount; refcount==-1 为永久引用, 归零
//       json_delete; NULL 直接跳过)。
int extranonce_subscribe(void *pool)
{
	char send_buf[8196];       /* 原 acStack_2018 (8196 字节槽) */
	char msg[4096];            /* 原 local_3018..local_2fe8 日志槽 */
	void *val, *result, *error;
	char *line, *reason;
	uint8_t json_err[252];     /* jansson json_error_t (原 auStack_3114[252]) */
	int rc, id, ret;

	id = DAT_0007b0f8;
	DAT_0007b0f8 = DAT_0007b0f8 + 1;
	sprintf(send_buf, "{\"id\": %d, \"method\": \"mining.extranonce.subscribe\", \"params\": []}", id);
	if (stratum_send(pool, send_buf, strlen(send_buf)) == 0) {
		return 0;                                   /* 原 LAB_0002e7a8 */
	}

	while (socket_full(pool, 2) != 0) {
		line = recv_line(pool);
		if (line == NULL) {
			return 0;                               /* 原 LAB_0002e7a8 */
		}
		rc = parse_notify_equihash(pool, line);
		if (rc == 0) {
			/* 非 stratum 方法消息 → 视为本次订阅的响应 */
			val = json_loads(line, 0, json_err);
			free(line);
			result = json_object_get(val, "result");
			error = json_object_get(val, "error");

			if (result == NULL || *(const int *)result == 6) {          /* json_is_false */
				if (error == NULL) {
					reason = malloc(0x11);
					if (reason != NULL) {
						strncpy(reason, "(unknown reason)", 0x11);      /* 原 builtin_strncpy */
					}
					if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 5 < DAT_000766c4) {
						snprintf(msg, 0x1000, "Pool %d JSON extranonce subscribe failed: %s",
						         *(int *)pool, reason);
						applog(6, msg, 0);
					}
					free(reason);
					ret = 0;
					goto lab_decref;
				}
				/* error 存在 → 落入下方共用错误提取 (原 goto LAB_0002e81c) */
			} else {
				if (error == NULL || *(const int *)error == 7) {        /* json_is_null */
					if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 5 < DAT_000766c4) {
						snprintf(msg, 0x1000, "Stratum extranonce subscribe for pool %d",
						         *(int *)pool);
						applog(6, msg, 0);
					}
					ret = 1;
					goto lab_decref;
				}
			}

			/* ---- 共用错误提取 (原 LAB_0002e81c) ---- */
			reason = json_array_string_nc(error, 1);
			if (reason == NULL) {
				reason = (char *)json_string_value(error);
			}
			if (reason == NULL ||
			    (strcmp(reason, "Method 'subscribe' not found for service 'mining.extranonce'") != 0 &&
			     strcmp(reason, "Unrecognized request provided") != 0)) {
				reason = json_dumps(error, 3);
				if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 5 < DAT_000766c4) {
					snprintf(msg, 0x1000, "Pool %d JSON extranonce subscribe failed: %s",
					         *(int *)pool, reason);
					applog(6, msg, 0);
				}
				free(reason);
				ret = 0;                                /* 原 LAB_0002e868 */
			} else {
				if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 5 < DAT_000766c4) {
					snprintf(msg, 0x1000, "Cannot subscribe to mining.extranonce for pool %d",
					         *(int *)pool);
					applog(6, msg, 0);
				}
				ret = 1;                                /* 原 LAB_0002e8fc / 公共落点 uVar7=1 */
			}

lab_decref:
			/* 内联 json_decref(val) (jansson): NULL / 永久引用 (refcount==-1) 跳过,
			 * refcount (json_t+4) 归零时 json_delete */
			if (val != NULL) {
				if (*(int32_t *)((uint8_t *)val + 4) != -1) {
					*(int32_t *)((uint8_t *)val + 4) -= 1;
					if (*(int32_t *)((uint8_t *)val + 4) == 0) {
						json_delete(val);
					}
				}
			}
			return ret;
		}
		free(line);                                     /* 方法消息: 释放后继续等 */
	}

	if (DAT_00078e09 != '\0' && LOG_GATE()) {
		applog(7, "Timed out waiting for response extranonce.subscribe", 0);
	}
	return 1;
}

// === log_work_file_index === 原 FUN_0002f64c @ 0x0002f64c
// 来源: reversed/cgminer/20_job_mining/FUN_0002f64c.c | 分类: 20_job_mining
// 语义: 按元素字节宽度 kind (1/2/4/8/16/32/64) 与值 value 计算文件索引号:
//       - kind=1 → 1;
//       - kind=2/4/8/16 → 取 value 高 log2(kind) 位组成的数值加 1
//         (即 1 + 前缀值, 覆盖 2..2^log2(kind) 全部档位);
//       - kind=32 → 查表 DAT_000655a8[(value & ~7)-8 (& 0xff)], 下标回绕 (≥0xf1) 返 1;
//       - kind=64 → 查表 DAT_0006569c[(value & ~3)-4 (& 0xff)], 下标回绕 (≥0xf9) 返 1;
//       - 其余 kind 返回 0; 各查表项为有符号字节。
//       (switch 结构按原码保留; kind≤16 各档的枚举顺序系 Ghidra 比较树展开, 已合并为
//       等价穷举, 返回值逐一核对一致。)
int log_work_file_index(uint32_t kind, uint32_t value)
{
	uint32_t idx;

	switch (kind) {
	case 1:
		break;                                   /* → 公共返回 1 */
	case 2:
		if ((value & 0x80) != 0) {
			return 2;
		}
		break;                                   /* 前缀 0 → 1 */
	case 4:
		value = value & 0xc0;
		if (value == 0x80) return 3;             /* 10 */
		if (value == 0xc0) return 4;             /* 11 */
		if (value == 0x40) return 2;             /* 01 */
		break;                                   /* 00 → 1 */
	case 8:
		value = value & 0xe0;
		if (value == 0x80) return 5;
		if (value < 0x81) {
			if (value == 0x40) return 3;
			if (value == 0x60) return 4;
			if (value == 0x20) return 2;
		} else {
			if (value == 0xc0) return 7;
			if (value == 0xe0) return 8;
			if (value == 0xa0) return 6;
		}
		break;                                   /* 000 → 1 */
	case 0x10:
		value = value & 0xf0;
		if (value == 0x80) return 9;
		if (value < 0x81) {
			if (value == 0x40) return 5;
			if (value < 0x41) {
				if (value == 0x20) return 3;
				if (value == 0x30) return 4;
				if (value == 0x10) return 2;
			} else {
				if (value == 0x60) return 7;
				if (value == 0x70) return 8;
				if (value == 0x50) return 6;
			}
		} else {
			if (value == 0xc0) return 0xd;
			if (value < 0xc1) {
				if (value == 0xa0) return 0xb;
				if (value == 0xb0) return 0xc;
				if (value == 0x90) return 10;
			} else {
				if (value == 0xe0) return 0xf;
				if (value == 0xf0) return 0x10;
				if (value == 0xd0) return 0xe;
			}
		}
		break;                                   /* 0000 → 1 */
	case 0x20:
		idx = ((value & 0xfffffff8) - 8) & 0xff;
		if (idx < 0xf1) {
			return (int)DAT_000655a8[idx];       /* 有符号字节表 */
		}
		break;                                   /* 回绕 → 1 */
	case 0x40:
		idx = ((value & 0xfffffffc) - 4) & 0xff;
		if (idx < 0xf9) {
			return (int)DAT_0006569c[idx];       /* 有符号字节表 */
		}
		break;                                   /* 回绕 → 1 */
	default:
		return 0;
	}
	return 1;
}

/*
 * ============================================================================
 * 覆盖率表 — job_p4a.c (cgminer 挖矿任务层前 9 个) 9/9 全部完成
 * ============================================================================
 * | 原地址       | 语义名                | 源行数 | 源分类      | 备注                     |
 * |--------------|-----------------------|--------|-------------|--------------------------|
 * | FUN_00011580 | suffix_string         |    119 | 20_job_mining | SI 词头格式化, 保留整除截断 |
 * | FUN_00017254 | benchfile_get_work    |    200 | 20_job_mining | benchfile 解析 + SHA256 中间态, 尾递归重开 |
 * | FUN_0001acc4 | gen_stratum_work      |    124 | 20_job_mining | stratum 工作生成 (锚点函数) |
 * | FUN_00022b44 | stale_work            |    210 | 20_job_mining | 4 类过期判定 (锚点: 4 类原因日志) |
 * | FUN_00022f88 | discard_stale         |     99 | 20_job_mining | uthash 内联删除 + 丢弃计数 |
 * | FUN_000247d4 | get_work              |    136 | 20_job_mining | hash_pop/stale 循环 + 基准模板轮换 |
 * | FUN_00029f54 | fulltest              |    115 | 20_job_mining | 256 位 hash<=target 校验 |
 * | FUN_0002e69c | extranonce_subscribe  |    174 | 20_job_mining | mining.extranonce.subscribe 应答处理 |
 * | FUN_0002f64c | log_work_file_index   |    136 | 20_job_mining | 宽度/前缀 → 索引 (2 表查表) |
 *
 * 汇编核对: FUN_0001acc4 (varint r2=0x540/r3=0, calc_diff d0=[work+0x708], +0xa0 目标
 * 32 字节, DAT_000770a8/0x7b4cc 自增), FUN_00011580 (uldivmod→floatundidf→vdiv),
 * FUN_000247d4 (set_thr_idle 实参 = thr) — 均以 analysis/cgminer_z15_bm1746.elf
 * objdump(capstone) 证实。
 */
