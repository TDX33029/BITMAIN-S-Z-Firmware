// ============================================================================
// freq_p3.c — cgminer 频率/PLL 层第 3 段 16 个函数语义转写
// 分类: 12_freq_pll | 命名: rewritten/name_map/cgminer.tsv (阶段 1 冻结)
//
// 本文件内容 (按地址序):
//   1. scanhash_scrypt            FUN_000150f8 — scrypt 挖矿线程哈希循环
//   2. bitmain_scanhash           FUN_0003af90 — bitmain 驱动 scanhash 主循环
//   3. turbo_mode_monitor_func    FUN_0004388c — turbo 模式监控线程
//   4. check_freq_and_set_work_mode FUN_00046cbc — 频率检查并切工作模式
//   5. scan_freq_save_freq        FUN_000470a8 — 扫频结果保存
//   6. scan_freq_func             FUN_00047354 — 扫频线程 (窗口[775,815]二分逼近)
//   7. verify_sol                 FUN_0004eaf4 — Equihash 解验证
//   8. blake2b_init               FUN_00054ed0 — BLAKE2b 初始化
//   9. blake2b_init_key           FUN_00055098 — BLAKE2b 带密钥初始化
//  10. sha256_process             FUN_00055470 — SHA256 块处理
//  11. xxtea_encode               FUN_00056b3c — XXTEA 加密
//  12. hashtable_set              FUN_00057a8c — 哈希表插入 (jansson)
//  13. lex_scan                   FUN_0005852c — JSON 词法扫描 (jansson)
//  14. parse_value                FUN_00058d00 — JSON 值解析 (jansson)
//  15. __aeabi_idiv               FUN_0005c718 — 有符号 32 位除法 (libgcc)
//  16. __aeabi_idivmod            FUN_0005c710 — 有符号 32 位除法取余 (libgcc)
//
// 说明: 函数 8-14 实为 blake2b / sha2 / xxtea / jansson / 库代码, 编译时被
// 链入 12_freq_pll 区段 (静态链接产物按地址分类的固有现象), 按体原样转写。
// ============================================================================

#include <stdint.h>
#include <stdio.h>      /* snprintf/fscanf/fprintf/fopen/fclose/puts/system — 原调用保留 */
#include <stdlib.h>     /* malloc/free/strtoll/raise/exit — 原调用保留 */
#include <string.h>     /* memcpy/memset/strcmp/strcpy/strlen/memcmp — 原调用保留 */
#include <pthread.h>    /* pthread_mutex / pthread_rwlock / pthread_cond_wait — 原调用保留 */
#include <unistd.h>     /* access/sleep — 原调用保留 */
#include <errno.h>      /* errno (经 __errno_location) */

/* ================= 外部全局 (DAT_ 原名保留, 语义见注释) ================= */

extern uint8_t  DAT_0007ad48;   /* applog 门控 A (非 0 强制输出, 兼 syslog 开关; log.c) */
extern uint8_t  DAT_00078e08;   /* applog 门控 B = opt_verbose (log.c) */
extern int      DAT_000766c4;   /* applog 门控 C: 日志级别 (prio3 需 >2, prio5 需 >4, prio7 需 >6) */
extern uint8_t  DAT_00078e09;   /* opt_debug (curses 'd' 联动; LOG_GATE7 类日志外层总开关) */

extern char     DAT_0009d4bb[]; /* 统计区基址 2: +1 (0x9d4bc) 起为链存在标志数组 (4 链) */
extern uint8_t  DAT_0009d4bc[]; /* 链存在标志数组 (DAT_0009d4bb+1): 1 = 链在位 */
extern char     DAT_0009d4b7[]; /* nonce 统计计数基址: 0x9d4b7+4k 起 int (fan.c 注: 0x9d4b8+4k 与 0x9d5b8+4(k-0x40) 同址) */
extern uint8_t  DAT_0009d4b5[]; /* 统计区基址 1: +0x250+(chain*2) 起为每链当前平均频率 (uint16, api "frequency%d") */
extern uint8_t  DAT_0009d5a7[]; /* api "chain_hw%d" 基址: uint32 ×4 (硬件错误计数) */
extern uint8_t  DAT_0009d6cb;   /* 在位链计数 (check_chain 统计; chain.c/fan.c) */
extern uint8_t  DAT_0009d6f3;   /* 芯片地址步进 asic_addr_interval (calculate_address_interval 写入; chain.c) */

extern int      DAT_0007c130;   /* 0x7c130: PIC 频率配置可用标志 (check_pic_flash_support 写入, >0 = PIC 可读);
                                 * 0x7c134 起为其后每链频率监控数组区, 各数组 [链][芯片] 步进 3 (4 链 × 3 芯片) */
extern int      DAT_0007c134[]; /* 0x7c134 (基址+4): [链][芯片] 无效 nonce 计数 */
extern int      DAT_0007c138[]; /* 0x7c138 (基址+8): [链][芯片] 计数 B (turbo 复位时清零, 语义待定) */
extern int      DAT_0007c194[]; /* 0x7c194 (基址+0x64): [链][芯片] 降频请求标志 (1 = 该芯片需降频) */
extern int      DAT_0007c1c4[]; /* 0x7c1c4 (基址+0x94): [链][芯片] 频率窗口下限 (初值 0x307=775) */
extern int      DAT_0007c1f4[]; /* 0x7c1f4 (基址+0xc4): [链][芯片] 频率窗口上限 (初值 0x32f=815, 二轮 0x339=825) */
extern int      DAT_0007c224[]; /* 0x7c224 (基址+0xf4): [链][芯片] 当前目标频率 */
extern int      DAT_0007c254[]; /* 0x7c254 (基址+0x124): [链][芯片] 已到目标标志 (1 = 到达) */

extern int      DAT_0007c28c;   /* nonce fifo 写索引 (0..99, 环形) */
extern int      DAT_0007c290;   /* nonce fifo 读索引 (0..99, 环形) */
extern int      DAT_0007c294;   /* nonce fifo 计数 (上限 100, 满时生产者告警 "nonce fifo full!!!") */
extern uint8_t  DAT_0007c298[]; /* nonce fifo 数据区基址: 每项 0x540 字节 (方案/工作数据) */
extern uint8_t  DAT_0007c7d8[]; /* nonce fifo 每项 13 字节头基址 (= 0x7c298+0x540): [0]芯片地址字节 [1]work id
                                 * [2]nonce 字节 [4..7][8..11]两个 uint 字段 [0xc]链号 (生产者 FUN_0004a754) */
extern uint32_t DAT_00078e18;   /* 64 位已处理方案计数器低 4 字节 (0x78e18) */
extern uint32_t DAT_00078e1c;   /* 同上高 4 字节 (0x78e1c) */
extern uint32_t DAT_00078e60[]; /* 每链 64 位有效方案计数: 低字 [chain*2] (0x78e60), 高字 [chain*2+1] (0x78e64) */
extern char     DAT_00078e88;   /* nonce fifo 互斥锁 (pthread_mutex_t; FUN_0004a754/FUN_000497cc 共用) */
extern char     DAT_00078ea0;   /* work_id→work 映射数组互斥锁 (pthread_mutex_t) */
extern char     DAT_00078e3c;   /* 全局控制标志: 1 = 条件等待循环保持休眠 (fan.c 注为 PIC 已停发标志; 扫频/turbo 线程亦用) */
extern char     DAT_000790ec;   /* 全局, 语义待定 (hash.c 注为每链重初始化标志数组; 此处按标量读取) */
extern char     DAT_00079100;   /* ASIC 状态刷新通知互斥锁 (pthread_mutex_t; fan.c) */
extern char     DAT_0007c078;   /* ASIC 状态刷新条件变量 (pthread_cond_t; fan.c) */
extern char     DAT_00077144;   /* 全局: 硬件错误计数复位请求标志 (置位时 bitmain_scanhash 清 DAT_0009d5a7 起 16 字节) */
extern void   (*DAT_00075ed8)(void); /* 全局函数指针: ELF 初值 noop_hook (FUN_00028ae0, 恒 0) [低置信] */

extern int32_t  DAT_00069204[]; /* 频率表 0x69204: 115 条 × 16B, 条目首字段 = freq_MHz (freq_table_full.csv);
                                 * DAT_00069204[j] 即条目 j 频率; [-1]/[-2] 为表前兜底槽位 (0x691fc/0x691f8) */
extern uint8_t  DAT_00069af8[]; /* XXTEA 密钥区: 每链 16 字节 (0x69af8+chain*0x10, 4 个 uint32) */
extern const int DAT_0006c580[];/* SHA-256 轮常数表 K[64] (0x6c580, 256B) */
extern void   (*DAT_00076b94)(void *ptr, int value, uint32_t num); /* 全局函数指针, 语义待定:
                                 * 按 memset(ptr,val,n) 形参布局调用 (blake2b_init_key 保密擦除) */

/* ================= 外部函数 (名取自 name_map/cgminer.tsv) ================= */

extern int     *__errno_location(void);                        /* glibc errno 获取 (原调用保留) */
extern void     applog(uint32_t prio, char *buf, ...); /* FUN_00038730 (log.c): 第 3 参非 0 强制 curses 刷新 */
extern void     quit(int status);                               /* FUN_00016ca8 */
extern void     cgtime(void *tv);                               /* FUN_0002a84c: gettimeofday(tv, NULL) 包装 */
extern void     cgsleep_ms(int ms);                             /* FUN_0002a92c (util.c) */
extern void     scrypt_N_1_256(uint32_t *input, void *scratch, uint32_t *state_out); /* FUN_000122e8 */
extern void    *make_clone(void *work, uint32_t nonce2);        /* FUN_0001a44c (job_p1.c): make_work + copy_work_nonce2 */
extern int      clone_work_with_id(void *unused, void *work, uint32_t id, uint32_t extra); /* FUN_00025e28 (freq_p4.c) */
extern void     update_work_stats(int thr, void *work);         /* FUN_0001a730: share_diff 统计 + Found block 提交 */
extern void     log_work(void *work, uint8_t *buf, int flag);   /* FUN_0002f934 */
extern void     free_work(void *work);                          /* FUN_00015780 */
extern char    *bin2hex(const char *data, int len);             /* FUN_00029730 */
extern void     equihash_solver(void *state, int n, int k);     /* FUN_0004eee0 (50_crypto_hash): 实为按 ZcashPoW 参数
                                 * 初始化 blake2b 状态 (digest_len=(512/N*N+7)/8, fanout=depth=1, personal="ZcashPoW"+N+K) */
extern int      blake2b_update(void *state, const void *in, uint64_t inlen); /* FUN_00054f8c (freq_p4.c 定义)
                                 * 反编译源 4 参 (param_3/param_4) 实为 ARM EABI 64 位 inlen 的 r2/r3 对,
                                 * 逻辑签名 3 参, 与定义侧 freq_p4.c 统一 (原 4 参调用尾部 0 即高半字) */
extern void     sha256_oneshot(const void *data, uint32_t len, void *out32); /* FUN_00055930 (freq_p4.c) */
extern void     equihash_leaf_hash(const void *state_snapshot, uint32_t tail_word, void *out, uint8_t out_len); /* FUN_0004df30 */
extern void     expandArray(const uint8_t *in, uint32_t in_len, uint8_t *out, uint32_t out_len,
                            uint32_t bit_len, uint32_t out_width); /* FUN_0004df94 (freq_p4.c) */
extern uint32_t is_entry_less(int entry_a, int entry_b, int offset, size_t len); /* FUN_0004ea38: memcmp 索引段比较 */
extern int      check_duplicate_indices(int buf_a, int buf_b, int offset, uint32_t len); /* FUN_0004ea58 */
extern int      blake2b_init_state(void *state, const void *param_block); /* FUN_00054dcc */
extern void     set_freq_asic(int chain, int asic_idx, int freq_mhz); /* FUN_00043734 (freq_p4.c): 单 ASIC 频率帧 (reg 0x0C 带址写) */
extern void     every_chain_set_frequency(int freq_mhz);        /* FUN_00043e4c (freq_p4.c): 全部在位链广播设频 */
extern void    *create_bitmain_scan_freq_pthread(void);         /* FUN_00046880 (freq_p4.c) */
extern void    *create_bitmain_turbo_mode_monitor_pthread(void);/* FUN_00046964 (freq_p4.c) */
extern uint8_t  save_freq_to_every_chain_pic(void *buf);        /* FUN_00046b94 (freq_p4.c) */
extern void     get_every_chain_pic_version(void);              /* FUN_00040454 */
extern int      check_pic_flash_support(char *out_buf);         /* FUN_00046a48 (13_voltage_pic): 返回值写 DAT_0007c130 */
extern unsigned __udivsi3(unsigned num, unsigned den);          /* FUN_0005c648: r0=商 (r1=余数), 本文件仅用商 */
extern uint64_t __aeabi_idivmod(uint32_t num, uint32_t den);    /* FUN_0005c710 (本文件转写): r0=商, r1=余数 */

/* ---- jansson 库 (定义在 freq_p4.c 等批次文件) ---- */
extern const int DAT_0006cb74[];    /* jansson primes 桶数表 (0x6cb74) */
extern void    *jsonp_malloc(int size);                         /* FUN_0005956c */
extern void     jsonp_free(void *ptr);                          /* FUN_00059594 */
extern void     insert_to_bucket(uint32_t *hashtable, uint32_t *bucket, int *node); /* FUN_000578c0 (freq_p4.c) */
extern int      hashtable_find_pair(uint32_t *hashtable, uint32_t *bucket,
                                    const char *key, int hash); /* FUN_0005791c: 命中返回 pair 指针 */
extern void     json_delete(void *json);                        /* FUN_0005a3e4 (freq_p4.c) */
extern void     strbuffer_clear(void *strbuf);                  /* FUN_00059680 (freq_p4.c) */
extern char    *strbuffer_value(void *strbuf);                  /* FUN_00059694 (freq_p4.c) */
extern void     strbuffer_append_byte(void *strbuf, uint8_t byte); /* FUN_00059774 (freq_p4.c) */
extern int      jsonp_strtod(void *strbuf, double *out);        /* FUN_000597e8 */
extern uint32_t stream_get(uint32_t *lex, int error);           /* FUN_0005832c (freq_p4.c) */
extern void     stream_unget(int lex, uint32_t c);              /* FUN_00058274 (freq_p4.c) */
extern uint32_t lex_get_save(int lex, int error);               /* FUN_000584fc (freq_p4.c) */
extern void     lex_unget_unsave(int lex, uint32_t c);          /* FUN_000584b0 (freq_p4.c) */
extern int      decode_unicode_escape(char *str);               /* FUN_000581cc (freq_p4.c) */
extern int      utf8_encode(uint32_t codepoint, uint8_t *buf, size_t *len); /* FUN_000599dc (freq_p4.c) */
extern void     error_set(void *error, int lex, const char *msg, ...); /* FUN_00058034 (freq_p4.c, 变参) */
extern void    *json_object_new(void);                          /* FUN_00059da8 (freq_p4.c) */
extern void    *json_array(void);                               /* FUN_00059f80 (freq_p4.c) */
extern void    *json_string_nocheck(const char *value);         /* FUN_0005a0ec (freq_p4.c) */
extern void    *json_integer(long long value);                  /* FUN_0005a21c (freq_p4.c): json_int_t = long long */
extern void    *json_real(double value);                        /* FUN_0005a29c (freq_p4.c) */
extern void    *json_true(void);                                /* FUN_0005a3c0 (freq_p4.c) */
extern void    *json_false(void);                               /* FUN_0005a3cc (freq_p4.c) */
extern void    *json_null(void);                                /* FUN_0005a3d8 (freq_p4.c) */
extern int      json_array_append(void *array, void *value);    /* FUN_0005a6e8 (freq_p4.c) */
extern int      json_object_set_nocheck(void *object, const char *key, void *value); /* FUN_0005a820 (freq_p4.c) */
extern void    *json_object_get(void *object, const char *key); /* FUN_00059e20 */

/* 本文件内转写的函数 (供上方与后续函数互调) */
void   *scan_freq_func(void);                                   /* FUN_00047354 */
void   *turbo_mode_monitor_func(void);                          /* FUN_0004388c */
int     verify_sol(void *blake_state, const uint8_t *sol_data); /* FUN_0004eaf4 */
void    xxtea_encode(uint32_t *v, int n, const uint32_t *key);  /* FUN_00056b3c */

/* -------------------------------------------------------------------------- */
// === scanhash_scrypt === 原 FUN_000150f8 @ 0x000150f8
// 来源: reversed/cgminer/12_freq_pll/FUN_000150f8.c | 分类: 12_freq_pll
// 语义: scrypt 挖矿线程哈希循环。从 nonce=param_9 起逐次 +1: 写 work->data[19]
//       (param_3+0x4c), 组装 20 字输入缓冲 (前 19 字 = work 数据按字大端翻转 +
//       第 19 字 = 大端 nonce), 调 scrypt_N_1_256(input, scratch, hash);
//       hash[7] 字节序翻转后 <= target[7] (param_6+0x1c) 即命中: 把大端 nonce
//       写回 data[19], *hashes_done = nonce, 返回 1。nonce 达 max_nonce
//       (param_7) 或线程 restart 标志 (*(char*)(thr+0x3e) 非 0) 时返回 0。
//       scratch = malloc(0x20200), 失败按 LOG_ERR=3 记
//       "Failed to malloc scratchbuf in scanhash_scrypt" 并返回 0。
uint8_t scanhash_scrypt(int thr, int unused2, int work_data, int unused4, int unused5,
                        int work_target, uint32_t max_nonce, uint32_t *hashes_done,
                        uint32_t nonce)
{
	uint32_t input_buf[20];  /* 19 字大端数据 + 大端 nonce */
	uint32_t hash[8];  /* scrypt 输出 8 字 (state_out) */
	uint32_t target_word;  /* = *(uint*)(work_target+0x1c) */
	void *scratch;                /* __ptr */
	uint32_t word;  /* 字翻转临时 */
	int i;

	target_word = *(uint32_t *)(work_target + 0x1c);

	/* 输入缓冲前 19 字: work_data 逐字大端翻转 (x<<24 | (x>>8&0xff)<<16 | (x>>16&0xff)<<8 | x>>24) */
	for (i = 0; i != 0x4c; i += 4) {
		word = *(uint32_t *)(work_data + i);
		input_buf[i / 4] = word << 0x18 | (word >> 8 & 0xff) << 0x10 |
		                   (word >> 0x10 & 0xff) << 8 | word >> 0x18;
	}

	scratch = malloc(0x20200);
	if (scratch == NULL) {
		if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 2 < DAT_000766c4) {
			/* Ghidra 逐字节栈上拼串, 还原为等价字面量 (uart.c 约定) */
			applog(3, "Failed to malloc scratchbuf in scanhash_scrypt", 0);
		}
		return 0;
	}

	do {
		nonce++;
		*(uint32_t *)(work_data + 0x4c) = nonce;            /* work->data[19] = nonce (本机字节序) */
		input_buf[19] = nonce << 0x18 | (nonce >> 8 & 0xff) << 0x10 |
		                (nonce >> 0x10 & 0xff) << 8 | nonce >> 0x18;  /* 大端 nonce */
		scrypt_N_1_256(input_buf, scratch, hash);
		if ((hash[7] << 0x18 | (hash[7] >> 8 & 0xff) << 0x10 |
		     (hash[7] >> 0x10 & 0xff) << 8 | hash[7] >> 0x18) <= target_word) {
			*(uint32_t *)(work_data + 0x4c) =               /* 命中: data[19] 写回大端 nonce */
			    nonce << 0x18 | (nonce >> 8 & 0xff) << 0x10 |
			    (nonce >> 0x10 & 0xff) << 8 | nonce >> 0x18;
			*hashes_done = nonce;
			free(scratch);
			return 1;
		}
	} while (nonce < max_nonce && *(char *)(thr + 0x3e) == '\0');  /* thr->work_restart 标志 */

	*hashes_done = nonce;
	free(scratch);
	return 0;
}

/* -------------------------------------------------------------------------- */
// === bitmain_scanhash === 原 FUN_0003af90 @ 0x0003af90
// 来源: reversed/cgminer/12_freq_pll/FUN_0003af90.c | 分类: 12_freq_pll
// 语义: btm-zcash 驱动 scanhash 主循环。从 nonce fifo (DAT_0007c298/7c7d8, 互斥锁
//       DAT_00078e88) 按 0x54d 字节项取出 ASIC 返回的 Equihash 方案: 头 13 字节
//       (芯片地址/work id/nonce 字节/两个 uint 字段/链号) + 0x540 数据。按 work id
//       在 (info+0xb6+id*4) 数组查 work (互斥锁 DAT_00078ea0) 并 make_clone; 用
//       ZcashPoW blake2b 状态 + SHA256(SHA256(0x5cf 数据)) 预筛后 verify_sol 校验;
//       通过且 hash<target 字节则 clone_work_with_id 提交; 更新全局 64 位方案计数
//       (DAT_00078e18/e1c) 与每链计数 (DAT_00078e60/64); 失败按返回值累计无效
//       nonce 计数 (DAT_0009d4b7 基址, 槽 0x4c) 并按需复位 chain_hw 统计;
//       收尾 log_work + free_work。函数尾把 64 位计数改为 x*0xFFFF (Ghidra 向量
//       运算渲染), 返回恒 0。
//       锁失败走 snprintf "WTF ... ERROR ... in driver-btm-zcash.c bitmain_scanhash():行号"
//       + applog(3,...,1) + quit(1) (0x189e / 0x190b)。
int bitmain_scanhash(int thr)
{
	char msg[4096];  /* snprintf 缓冲 / SHA256 输入 (0x5cf) */
	uint8_t time_buf[8];  /* cgtime 时间戳缓冲 (结果未用) */
	uint8_t sol_data[0x540];  /* 从 fifo 项拷出的方案数据 */
	uint8_t hash_and_state[392];  /* abStack_16f0: [0..0x1f]=SHA256 终值, [+0x20..]=ZcashPoW blake2b 状态 */
	uint32_t inter_hash[8];  /* SHA256 第一轮 32B 输出 */
	uint32_t item_word548;  /* fifo 项头 +0x548 的 4 字节字段 */
	uint8_t tmp_buf[4];  /* log_work 第 2 参缓冲 */
	int cgpu;  /* = *(int*)(thr+0x24) */
	void *info_mutex;             /* __mutex = *(void**)(cgpu+0x14): 驱动锁结构
	                               * (+0x00 pthread_mutex_t, +0x28 pthread_rwlock_t,
	                               *  +0xb6 起 work_id→work int 数组, 见下) */
	pthread_mutex_t *info_mutex_p; /* 同上 (pthread 调用形参) */
	int pending_count;  /* fifo 快照计数, 循环倒计时 */
	int work;  /* make_clone 结果 (0 = 未找到) */
	int slot_off;  /* fifo 项字节偏移 = 读索引*0x54d */
	int rd_idx;  /* fifo 读索引 */
	uint8_t chip_addr;  /* 项头 [0] 芯片地址字节 */
	uint8_t work_id;  /* 项头 [1] work id */
	uint8_t nonce_byte;  /* 项头 [2] nonce 字节 */
	int chain_id;                 /* param3: 项头 [0xc] 链号 */
	int *id_slot;  /* work_id→work 槽 */
	uint32_t chain_lo;  /* 每链计数低字旧值 */
	char chain_present;  /* DAT_0009d4bc[chain] */
	int chip_idx;  /* 芯片序号 = 地址/步进 */
	int verified;  /* verify_sol 返回值 */
	const uint8_t *hash_byte;  /* 终值字节游标 */
	const uint8_t *targ_byte;  /* work+0xc0 起 32B 目标游标 */
	int submit;                   /* 转写引入: 1 = 校验通过可提交 (原码由比较路径决定) */
	int drain;                    /* 循环控制 */
	int e;                        /* errno_location 返回值 */
	uint64_t cnt64;

	cgpu = *(int *)(thr + 0x24);
	info_mutex = *(void **)(cgpu + 0x14);
	info_mutex_p = (pthread_mutex_t *)info_mutex;

	memset(sol_data, 0, 0x540);
	cgtime(time_buf);  /* FUN_0002a84c: 时间戳, 结果未再使用 */
	/* 64 位方案计数器低字清零 (Ghidra 渲染为 4 字节存储) */
	DAT_00078e18 = 0;
	pthread_mutex_lock((pthread_mutex_t *)&DAT_00078e88);
	pending_count = DAT_0007c294;
	pthread_mutex_unlock((pthread_mutex_t *)&DAT_00078e88);

	/* 标准驱动锁序: mutex 包住 rdlock (行号 0x189e) */
	work = pthread_mutex_lock(info_mutex_p);
	if (work != 0) {
		e = *__errno_location();
		snprintf(msg, 0x1000, "WTF MUTEX ERROR ON LOCK! errno=%d in %s %s():%d", e,
		         "driver-btm-zcash.c", "bitmain_scanhash", 0x189e);
		applog(3, msg, 1);
		quit(1);
	}
	work = pthread_rwlock_rdlock((pthread_rwlock_t *)((char *)info_mutex + 0x28)); /* 原 __mutex+1 */
	if (work != 0) {
		e = *__errno_location();
		snprintf(msg, 0x1000, "WTF RDLOCK ERROR ON LOCK! errno=%d in %s %s():%d", e,
		         "driver-btm-zcash.c", "bitmain_scanhash", 0x189e);
		applog(3, msg, 1);
		quit(1);
	}
	work = pthread_mutex_unlock(info_mutex_p);
	if (work != 0) {
		e = *__errno_location();
		snprintf(msg, 0x1000, "WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d", e,
		         "driver-btm-zcash.c", "bitmain_scanhash", 0x189e);
		applog(3, msg, 1);
		quit(1);
	}
	/* 原码此后复用 unlock 返回值 (0) 作为 "未找到 work" 的初值 */

	if (pending_count != 0) {
		drain = pending_count;
		do {
			drain--;
			pthread_mutex_lock((pthread_mutex_t *)&DAT_00078e88);
			rd_idx = DAT_0007c290;
			slot_off = DAT_0007c290 * 0x54d;          /* 每项 0x54d = 0x540 数据 + 13 字节头 */
			DAT_0007c294--;
			nonce_byte = DAT_0007c7d8[slot_off + 2];  /* 项头 [2] nonce 字节 */
			chain_id = DAT_0007c7d8[slot_off + 0xc];  /* 项头 [0xc] 链号 */
			work_id = DAT_0007c7d8[slot_off + 1];     /* 项头 [1] work id */
			chip_addr = DAT_0007c7d8[slot_off];       /* 项头 [0] 芯片地址字节 */
			memcpy(sol_data, DAT_0007c298 + slot_off, 0x540);
			item_word548 = *(uint32_t *)(DAT_0007c7d8 + slot_off + 8); /* 项头 +8 (0x7c7e0) */
			if (rd_idx < 99) {
				DAT_0007c290 = rd_idx + 1;
			} else {
				DAT_0007c290 = 0;
			}
			pthread_mutex_unlock((pthread_mutex_t *)&DAT_00078e88);

			if (4 < chain_id) {
				if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 4 < DAT_000766c4) {
					snprintf(msg, 0x1000, "Error:scanhash chainid=%u\n", chain_id);
					applog(5, msg, 0);
				}
				break;
			}

			/* work_id→work 查找: 槽地址 = info_mutex+0xb6+work_id*4
			 * (Ghidra: &__mutex[4].__data.field5_0x14.__spins + id*4 + 2,
			 *  按 pthread_mutex_t=0x28: 4*0x28+0x14+2 = 0xb6; 与 FUN_00042b54 的
			 *  work 槽写 (基址+(id+0x1c)*4+6) 相互印证, 布局细节按字节偏移保留) */
			pthread_mutex_lock((pthread_mutex_t *)&DAT_00078ea0);
			id_slot = (int *)((char *)info_mutex + 0xb6 + (uint32_t)work_id * 4);
			if (*id_slot != 0) {
				work = (int)make_clone((void *)*id_slot, 0);
			}
			pthread_mutex_unlock((pthread_mutex_t *)&DAT_00078ea0);

			if (work == 0) {
				if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 2 < DAT_000766c4) {
					snprintf(msg, 0x1000, "%s %d: work %02x not find error",
					         *(char **)(*(int *)(cgpu + 4) + 8),   /* cgpu->drv->name */
					         *(int *)(cgpu + 8),                  /* cgpu->device_id */
					         (uint32_t)work_id);
					applog(3, msg, 0);
				}
			} else {
				*(uint32_t *)(work + 0x168) = item_word548;
				memcpy((void *)(work + 0x16f), sol_data, 0x540);

				/* 预筛 1: ZcashPoW blake2b 状态 + 0x8c=140 字节头部 */
				equihash_solver(hash_and_state + 0x20, 200, 9);
				blake2b_update(hash_and_state + 0x20, (void *)(work + 0xe0), 0x8c);

				memset(inter_hash, 0, 0x20);
				memset(hash_and_state, 0, 0x20);       /* 仅终值区清零, blake2b 状态不动 */

				/* 预筛 2: SHA256(SHA256(0x5cf 字节数据)) */
				memset(msg, 0, 0x5cf);
				memcpy(msg, (void *)(work + 0xe0), 0x5cf);
				sha256_oneshot(msg, 0x5cf, inter_hash);
				sha256_oneshot(inter_hash, 0x20, hash_and_state);

				/* 逐字节比较 hash[31..0] 与 work+0xbf..(0xc0 区): hash 字节 < 目标字节 →
				 * 可提交; 目标字节 < hash 字节 → 只校验不提交; 全等 → 可提交 */
				hash_byte = hash_and_state + 0x20;
				targ_byte = (uint8_t *)(work + 0xc0);
				submit = 1;
				do {
					hash_byte--;
					targ_byte--;
					if (*hash_byte < *targ_byte) {
						break;                          /* hash < target → break 走可提交路径 */
					}
					if (*targ_byte < *hash_byte) {
						submit = 0;
						break;                          /* hash > target → 仅校验 */
					}
				} while (hash_byte != hash_and_state);

				verified = verify_sol(hash_and_state + 0x20, sol_data);
				if (verified != 0) {
					/* 校验失败 (1/2): 累计无效 nonce 计数 */
					if (verified == 2) {
						chip_idx = (int)__udivsi3((unsigned)chip_addr, DAT_0009d6f3); /* 无符号商 */
						*(int *)(DAT_0009d4b7 +
						         (chain_id * 3 + (chip_idx & 0xff) + 0x4c) * 4) += 1;
					}
					if (DAT_00077144 != '\0') {
						DAT_00077144 = '\0';
						memset(DAT_0009d5a7, 0, 0x10);  /* 复位 chain_hw 硬件错误统计 */
					}
					log_work((void *)work, tmp_buf, 1);
					free_work((void *)work);
					work = 0;
					continue;
				}

				/* 校验通过 */
				if (submit) {
					clone_work_with_id((void *)thr, (void *)work, 1, 1); /* FUN_00025e28;
					                                     * 调用点 Ghidra 只渲染 3 参, 第 4 参
					                                     * 按其函数体 param_4==param_3 还原 */
				}
				// ---- 有效方案统计 (原 LAB_0003b35c) ----
				chain_lo = DAT_00078e60[chain_id * 2];
				chain_present = DAT_0009d4bc[chain_id];
				DAT_00078e1c += (uint32_t)(0xfffffffe < DAT_00078e18); /* 高字进位 (取旧低字) */
				DAT_00078e18 += 1;
				DAT_00078e60[chain_id * 2] = chain_lo + 1;
				DAT_00078e60[chain_id * 2 + 1] += (uint32_t)(0xfffffffe < chain_lo);
				if (chain_present == '\0') {
					if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 2 < DAT_000766c4) {
						snprintf(msg, 0x1000, "ChainID Cause Error! ChainID:[%d]", chain_id);
						applog(3, msg, 0);
					}
				} else {
					chip_idx = (int)__udivsi3((unsigned)chip_addr, DAT_0009d6f3) & 0xff;
					if (chip_idx < 3) {
						update_work_stats(thr, (void *)work);
						*(int *)(DAT_0009d4b7 +
						         (chain_id * 3 + chip_idx + 0x40) * 4) += 1; /* 每芯片有效 nonce 计数 */
						log_work((void *)work, tmp_buf, 1);
						free_work((void *)work);
						work = 0;
					} else if (DAT_00078e09 != '\0' &&
					           (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 6 < DAT_000766c4)) {
						snprintf(msg, 0x1000, "Which Nonce Cause Err![%d] %08x",
						         chip_idx, (uint32_t)nonce_byte);
						applog(7, msg, 0);
						/* 此路径不释放 work (原码如此) */
					}
				}
			}
		} while (drain != 0);
	}

	work = pthread_rwlock_unlock((pthread_rwlock_t *)((char *)info_mutex + 0x28));
	if (work != 0) {
		e = *__errno_location();
		snprintf(msg, 0x1000, "WTF RWLOCK ERROR ON UNLOCK! errno=%d in %s %s():%d", e,
		         "driver-btm-zcash.c", "bitmain_scanhash", 0x190b);
		applog(3, msg, 1);
		quit(1);
	}
	(*DAT_00075ed8)();

	/* 64 位计数器 x = (x << 16) - x, 即 x * 0xFFFF (mod 2^64)
	 * (Ghidra 以 VectorShiftLeft(x,0x10,64 位) + VectorSub(x) 渲染) */
	cnt64 = ((uint64_t)DAT_00078e1c << 32) | DAT_00078e18;
	cnt64 = (cnt64 << 16) - cnt64;
	DAT_00078e18 = (uint32_t)cnt64;
	DAT_00078e1c = (uint32_t)(cnt64 >> 32);
	return 0;
}

/* -------------------------------------------------------------------------- */
// === turbo_mode_monitor_func === 原 FUN_0004388c @ 0x0004388c
// 来源: reversed/cgminer/12_freq_pll/FUN_0004388c.c | 分类: 12_freq_pll
// 语义: turbo 模式监控线程 (不死循环返回):
//   1. 初始化: 每条在位链把栈上目标拷贝 chip_state[12+链*3..] 三芯片全部置
//      0x307 (775), 并把 DAT_0007c224 区三芯片当前频率均值写入
//      DAT_0009d4b5+(chain+0x128)*2 的 uint16 (api "frequency%d")。
//   2. 等 DAT_000790ec 非 0 (每 500ms)。逐链逐芯片: 栈上已达标志
//      chip_state[链*3+芯片] 非 0 → 仅计数; 否则按 DAT_0007c224 当前频率:
//      <0x307 → 目标=当前频率下发并置标志; ==0x307 → 下发 0x307 (Ghidra 略去
//      第 3 实参) 并置标志; >0x307 → 频率表中定位当前频率与目标索引, 目标索引
//      向当前频率索引方向步进 1-2 档下发, 新目标==当前频率才置标志。
//   3. 到达芯片数 >= DAT_0009d6cb*3: 打 "All asics arrive target freq" (5 级),
//      复位各数组 [链] 的芯片 0/1 槽位 (无效 nonce 计数/DAT_0007c138 计数清零、
//      降频标志清零、下限 0x307、上限 0x32f; 芯片 2 槽位原码未复位), 然后进入
//      无限等待/降频循环: sleep(1)+pthread_cond_wait(DAT_0007c078/DAT_00079100)
//      直到 DAT_00078e3c 为 0, 再对降频标志==1 且 当前频率>下限 的芯片打
//      "low freq[链-芯片]: ..." (5 级) 并查表降一档 (cur==100 → F[-1], 未命中 →
//      F[-2], 命中 F[j] → F[j-1]), 写回 DAT_0007c224 并 set_freq_asic。
//   4. 未全部到达: sleep(3) 后回到步骤 2。
void *turbo_mode_monitor_func(void)
{
	int chain;  /* 链号 0..3 */
	int slot;  /* 芯片槽 0..2 */
	int settled_count;  /* 已达目标芯片数 */
	int chip_state[24];  /* [0..11]=已达标志, [12..23]=目标频率拷贝 */
	int freq;  /* 当前频率 */
	int newfreq;  /* 复用: 新目标 */
	int idx_cur;  /* 当前频率在频率表中的索引 */
	int idx_tgt;  /* 目标频率索引 */
	int j;                        /* 频率表搜索计数 */
	int avg_i;  /* 三芯片均值临时 */
	char msg[4100];  /* snprintf 缓冲 / "All asics..." 栈串区 */
	char chain_present;  /* 链在位标志 */
	int wait_slot;  /* 降频循环内芯片槽游标 */
	int cur;  /* 降频判断用当前频率 */
	int idx;  /* 降一档索引 */

	/* ---- 步骤 1: 初始化 ---- */
	for (chain = 0; chain != 4; chain++) {
		if (DAT_0009d4bc[chain] == '\x01') {
			avg_i = DAT_0007c224[chain * 3] + DAT_0007c224[chain * 3 + 1] +
			        DAT_0007c224[chain * 3 + 2];
			chip_state[12 + chain * 3] = 0x307;
			chip_state[13 + chain * 3] = 0x307;
			chip_state[14 + chain * 3] = 0x307;
			*(int16_t *)(void *)(DAT_0009d4b5 + (chain + 0x128) * 2) =
			    (int16_t)(avg_i / 3);      /* 每链平均频率 (uint16 统计区) */
		}
	}

	/* ---- 主循环 ---- */
	for (;;) {
		while (DAT_000790ec == '\0') {
			cgsleep_ms(500);
		}
		settled_count = 0;
		for (chain = 0; chain != 4; chain++) {
			if (DAT_0009d4bc[chain] != '\x01') {
				continue;
			}
			for (slot = 0; slot != 3; slot++) {
				if (chip_state[chain * 3 + slot] != 0) {   /* 已达目标: 只计数 */
					settled_count++;
					continue;
				}
				freq = DAT_0007c224[chain * 3 + slot];
				if (freq < 0x307) {
					chip_state[12 + chain * 3 + slot] = freq;
					set_freq_asic(chain, slot, freq);
					chip_state[chain * 3 + slot] = 1;
				} else if (freq == 0x307) {
					chip_state[12 + chain * 3 + slot] = 0x307;
					/* 第 3 实参 Ghidra 略去; 该分支恒 freq == 0x307, 按寄存器复用还原 */
					set_freq_asic(chain, slot, 0x307);
					chip_state[chain * 3 + slot] = 1;
				} else {
					/* 定位当前频率 (F[1..114], 未命中 -1) */
					idx_cur = -1;
					for (j = 1; j != 0x73; j++) {
						if (DAT_00069204[j] == freq) {
							idx_cur = j;
							break;
						}
					}
					/* 定位目标 (100 → 0, 未命中 -1) */
					idx_tgt = (chip_state[12 + chain * 3 + slot] == 100) ? 0 : -1;
					if (idx_tgt < 0) {
						for (j = 1; j != 0x73; j++) {
							if (DAT_00069204[j] == chip_state[12 + chain * 3 + slot]) {
								idx_tgt = j;
								break;
							}
						}
					}
					/* 目标索引向当前频率索引方向步进 1-2 档 */
					if (idx_cur - idx_tgt < 2) {
						idx_tgt += 1;
					} else {
						idx_tgt += 2;
					}
					newfreq = DAT_00069204[idx_tgt];
					chip_state[12 + chain * 3 + slot] = newfreq;
					set_freq_asic(chain, slot, newfreq);
					if (newfreq == freq) {
						chip_state[chain * 3 + slot] = 1;
					}
				}
			}
		}

		if ((int)((uint32_t)DAT_0009d6cb * 3) <= settled_count) {
			if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 4 < DAT_000766c4) {
				/* Ghidra 逐字节栈上拼串 ("All "+"asic"+"s ar"+"rive"+" tar"+"get "+"freq"+\n),
				 * 还原为等价字面量 */
				applog(5, "All asics arrive target freq\n", 0);
			}
			/* 复位: 每在位链仅芯片 0/1 槽位 (原码如此, 芯片 2 不复位) */
			for (chain = 0; chain != 4; chain++) {
				if (DAT_0009d4bc[chain] == '\x01') {
					DAT_0007c134[chain * 3] = 0;
					DAT_0007c194[chain * 3] = 0;
					DAT_0007c1f4[chain * 3] = 0x32f;
					DAT_0007c1c4[chain * 3] = 0x307;
					DAT_0007c138[chain * 3] = 0;  /* (基址+8 数组) */
					DAT_0007c194[chain * 3 + 1] = 0;
					DAT_0007c1f4[chain * 3 + 1] = 0x32f;
					DAT_0007c1c4[chain * 3 + 1] = 0x307;
				}
			}
			// ---- 原 LAB_00043c8c: 无限 等待/降频 循环 ----
			for (;;) {
				do {
					sleep(1);
					pthread_mutex_lock((pthread_mutex_t *)DAT_00079100);
					pthread_cond_wait((pthread_cond_t *)DAT_0007c078,
					                  (pthread_mutex_t *)DAT_00079100);
					pthread_mutex_unlock((pthread_mutex_t *)DAT_00079100);
				} while (DAT_00078e3c != '\0');

				for (chain = 0; chain != 4; chain++) {
					chain_present = DAT_0009d4bc[chain];
					if (chain_present == '\x01') {
						for (wait_slot = 0; wait_slot != 3; wait_slot++) {
							if (DAT_0007c194[chain * 3 + wait_slot] == 1) {
								cur = DAT_0007c224[chain * 3 + wait_slot];
								if ((uint32_t)DAT_0007c1c4[chain * 3 + wait_slot] < (uint32_t)cur) {
									if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 4 < DAT_000766c4) {
										snprintf(msg, 0x1000,
										         "low freq[%d-%d]: cur freq %d, [%d, %d], invalid nonce %d, ox status %d\n",
										         chain, wait_slot, cur,
										         DAT_0007c1c4[chain * 3 + wait_slot],
										         DAT_0007c1f4[chain * 3 + wait_slot],
										         DAT_0007c134[chain * 3 + wait_slot], 1);
										applog(5, msg, 0);
										cur = DAT_0007c224[chain * 3 + wait_slot];
									}
									/* 降一档: 100 → F[-1]; 未命中 → F[-2]; 命中 F[j] → F[j-1] */
									if (cur == 100) {
										idx = -1;
									} else {
										idx = -2;
										for (j = 1; j != 0x73; j++) {
											if (DAT_00069204[j] == cur) {
												idx = j - 1;
												break;
											}
										}
									}
									newfreq = DAT_00069204[idx];
									DAT_0007c224[chain * 3 + wait_slot] = newfreq;
									set_freq_asic(chain, wait_slot, newfreq);
								}
							}
						}
					}
				}
			}
		}
		sleep(3);
	}
}

/* -------------------------------------------------------------------------- */
// === check_freq_and_set_work_mode === 原 FUN_00046cbc @ 0x00046cbc
// 来源: reversed/cgminer/12_freq_pll/FUN_00046cbc.c | 分类: 12_freq_pll
// 语义: 启动时决定工作模式。先 get_every_chain_pic_version, 再由
//       check_pic_flash_support 取 PIC 频率配置 (写入 DAT_0007c130 与 36 字节
//       pic_buf, 每链 9 字节: [0]='N'/表条目号 + [1..3] 三芯片表条目号):
//   - DAT_0007c130 < 1 (PIC 不可读): /config/chips_freq.config 不存在 → 打
//     "2 Goto scan mode..." 并启动扫频线程后返回; 存在 → fscanf 每链 3 个频率,
//     超出 [775,825] 按 800 处理写入 DAT_0007c224, 打 "2 Goto turbo mode"。
//   - DAT_0007c130 >= 1 (PIC 可读): 任一在位链 pic_buf[链*9] != 'N' → 打
//     "1 Goto scan mode..." 并启动扫频线程后返回; 否则按 pic_buf 表条目号查
//     频率表 (打 "pic read chain%d asic%d freq %d"), 超出 [775,825] 按 800 写入,
//     打 "1 Goto turbo mode"。
//   - 尾部: 19 字节消息拷贝 + applog(5) (门控不通过则跳过拷贝与调用), 最后启动
//     turbo 监控线程 (create_bitmain_turbo_mode_monitor_pthread)。
void check_freq_and_set_work_mode(void)
{
	char pic_buf[36];  /* PIC 配置 4 链 × 9 字节 */
	char msg[4100];  /* 日志缓冲 */
	char *msg_ptr;  /* 消息字面量指针 (turbo 尾部共用) */
	FILE *cfg;                    /* param0 */
	int chain;
	int slot;
	int val;  /* fscanf 频率值 */
	int freq;  /* 查表频率 */
	int tbl_idx;  /* 频率表记录字节 (uint8) */
	int i;

	for (i = 0; i < 12; i++) {    /* 原码仅清前 12 字节 */
		pic_buf[i] = '\0';
	}
	get_every_chain_pic_version();
	DAT_0007c130 = check_pic_flash_support(pic_buf);

	if (DAT_0007c130 < 1) {
		if (access("/config/chips_freq.config", 0) != 0) {
			if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 4 < DAT_000766c4) {
				memcpy(msg, "2 Goto scan mode and find the best frequency\n", 46);
				applog(5, msg, 0);
			}
			create_bitmain_scan_freq_pthread();
			return;
		}
		cfg = fopen("/config/chips_freq.config", "r");
		if (cfg == NULL) {
			if (!(DAT_0007ad48 == '\0' && DAT_00078e08 == '\0' && DAT_000766c4 < 5)) {
				snprintf(msg, 0x1000, "open %s failed\n", "/config/chips_freq.config");
				applog(5, msg, 0);
			} else {
				create_bitmain_turbo_mode_monitor_pthread();
				return;
			}
		} else {
			for (chain = 0; chain != 4; chain++) {
				if (DAT_0009d4bc[chain] == '\x01') {
					for (slot = 0; slot != 3; slot++) {
						val = 0;
						fscanf(cfg, "%d", &val);
						/* 超出 [775,825] (无符号差 > 0x32) → 800 */
						if (0x32 < (uint32_t)val - 0x307U) {
							val = 800;
						}
						DAT_0007c224[chain * 3 + slot] = val;
					}
				}
			}
			fclose(cfg);
		}
		if (DAT_0007ad48 == '\0' && DAT_00078e08 == '\0' && DAT_000766c4 < 5) {
			create_bitmain_turbo_mode_monitor_pthread();
			return;
		}
		msg_ptr = "2 Goto turbo mode\n";
	} else {
		for (chain = 0; chain != 0x24; chain += 9) {   /* 链 × 9 字节记录 */
			if (DAT_0009d4bc[chain / 9] == '\x01' && pic_buf[chain] != 'N') {
				if (DAT_0007ad48 == '\0' && DAT_00078e08 == '\0' && DAT_000766c4 < 5) {
					create_bitmain_scan_freq_pthread();
					return;
				}
				memcpy(msg, "1 Goto scan mode and find the best frequency\n", 46);
				applog(5, msg, 0);
				create_bitmain_scan_freq_pthread();
				return;
			}
		}
		for (chain = 0; chain != 4; chain++) {
			if (DAT_0009d4bc[chain] == '\x01') {
				for (slot = 0; slot != 3; slot++) {
					tbl_idx = (uint8_t)pic_buf[chain * 9 + slot + 1];
					freq = DAT_00069204[tbl_idx];
					if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 4 < DAT_000766c4) {
						snprintf(msg, 0x1000, "pic read chain%d asic%d freq %d\n",
						         chain, slot, freq);
						applog(5, msg, 0);
					}
					/* 落在 [775,825] (无符号差 < 0x33) 用表值, 否则 800 */
					if ((uint32_t)freq - 0x307U < 0x33U) {
						DAT_0007c224[chain * 3 + slot] = freq;
					} else {
						DAT_0007c224[chain * 3 + slot] = 800;
					}
				}
			}
		}
		if (DAT_0007ad48 == '\0' && DAT_00078e08 == '\0' && DAT_000766c4 < 5) {
			create_bitmain_turbo_mode_monitor_pthread();
			return;
		}
		msg_ptr = "1 Goto turbo mode\n";
	}

	// 尾部: 19 字节消息拷贝 (16+3, 原 local_1018._0_3_) + applog(5)
	memcpy(msg, msg_ptr, 19);
	applog(5, msg, 0);
	create_bitmain_turbo_mode_monitor_pthread();
	return;
}

/* -------------------------------------------------------------------------- */
// === scan_freq_save_freq === 原 FUN_000470a8 @ 0x000470a8
// 来源: reversed/cgminer/12_freq_pll/FUN_000470a8.c | 分类: 12_freq_pll
// 语义: 扫频结果保存 (scan_freq_func 收尾调用)。返回 int: 0 成功 / -1 打开文件
//       失败。
//   - DAT_0007c130 < 1 (PIC 不可读): 写 /config/chips_freq.config ("w+"), 对每条
//     在位链 fprintf 三个 "%d " (DAT_0007c224 区当前频率); 失败打
//     "scan_freq_save_freq open %s failed" (5 级) 并返回 -1。
//   - DAT_0007c130 >= 1 (PIC 可读): 构造 36 字节 pic_buf (每链 9 字节, 首字节
//     'N'), 对每条在位链把三芯片当前频率在频率表中查条目号 (100 → 0, 未命中 →
//     -1, 命中 F[j] → j, 打 "pic write chain%d asic%d freq %d"), 写入记录
//     [1..3]; 对每链记录 +1 起 8 字节 (2 字) 做 XXTEA 加密 (密钥
//     DAT_00069af8+链*16), 最后 save_freq_to_every_chain_pic(pic_buf) 写 PIC。
int scan_freq_save_freq(void)
{
	char pic_buf[4136];  /* [0..0x23] 链记录区 / [+0x24] 日志缓冲 */
	char *rec;  /* 当前链记录游标 */
	FILE *cfg;                    /* __stream */
	int chain;
	int slot;                     /* param4 */
	int freq;
	int tbl_idx;  /* 频率表条目号 */
	int j;

	if (DAT_0007c130 < 1) {
		cfg = fopen("/config/chips_freq.config", "w+");
		if (cfg == NULL) {
			if (!(DAT_0007ad48 == '\0' && DAT_00078e08 == '\0' && DAT_000766c4 < 5)) {
				snprintf(pic_buf + 0x24, 0x1000, "%s open %s failed\n",
				         "scan_freq_save_freq", "/config/chips_freq.config");
				applog(5, pic_buf + 0x24, 0);
			}
			return -1;
		}
		for (chain = 0; chain != 4; chain++) {   /* 0x9d4bc..0x9d4be */
			if (DAT_0009d4bc[chain] == '\x01') {
				for (slot = 0; slot != 3; slot++) {
					fprintf(cfg, "%d ", DAT_0007c224[chain * 3 + slot]);
				}
			}
		}
		fclose(cfg);
		return 0;
	}

	/* PIC 可读: 表条目号 + XXTEA 加密 → 写 PIC flash */
	memset(pic_buf, 0, 0x24);
	for (chain = 0; chain != 4; chain++) {
		if (DAT_0009d4bc[chain] == '\x01') {
			rec = pic_buf + chain * 9;
			pic_buf[chain * 9] = 'N';
			for (slot = 0; slot != 3; slot++) {
				freq = DAT_0007c224[chain * 3 + slot];
				if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 4 < DAT_000766c4) {
					snprintf(pic_buf + 0x24, 0x1000, "pic write chain%d asic%d freq %d\n",
					         chain, slot, freq);
					applog(5, pic_buf + 0x24, 0);
				}
				if (freq == 100) {
					tbl_idx = 0;
				} else {
					tbl_idx = -1;
					for (j = 1; j != 0x73; j++) {
						if (DAT_00069204[j] == freq) {
							tbl_idx = j;
							break;
						}
					}
				}
				rec[1 + slot] = (char)tbl_idx;
			}
			xxtea_encode((uint32_t *)(void *)(pic_buf + chain * 9 + 1), 2,
			             (const uint32_t *)(void *)(DAT_00069af8 + chain * 0x10));
		}
	}
	save_freq_to_every_chain_pic(pic_buf);
	return 0;
}

/* -------------------------------------------------------------------------- */
// === scan_freq_func === 原 FUN_00047354 @ 0x00047354
// 来源: reversed/cgminer/12_freq_pll/FUN_00047354.c | 分类: 12_freq_pll
// 语义: 扫频线程 (不死循环返回)。启动先 every_chain_set_frequency(800) 对全部
//       在位链广播 800MHz; 每轮: 在位链三芯片初始化 DAT_0007c134/7c194/7c224
//       (800)/7c1f4(上限)/7c1c4(下限 0x307)/7c254 (到达标志), 然后循环
//       {sleep(1) + pthread_cond_wait(DAT_0007c078/DAT_00079100) 直到
//       DAT_00078e3c 为 0} 后逐芯片推进:
//       降频标志非 0 或无效 nonce 计数 >15 → 降频路径 (上限=当前频率查表降一档,
//       新目标=下限/新上限索引二分; 下限==当前频率则置到达标志); 否则升频路径
//       (下限抬到当前频率, 新目标=当前频率/上限索引二分, 差 1 档则 +1, 打
//       "up/down freq[链-芯片]: ..." 调试日志) — 都经 set_freq_asic 下发。
//       每轮统计在位链频率和与到达数, 全部到达 (== DAT_0009d6cb*3) 后:
//       avg = 频率和/到达数 (__aeabi_idivmod 取商); avg > 800 或上限已到
//       0x339 (825) → scan_freq_save_freq 保存 + "scan freq done..." 日志 +
//       system 重启 cgminer, 回到等待; 否则打 "scan freq again..." 并把上限
//       改 0x339 进入第二轮 (窗口 [775,825])。
void *scan_freq_func(void)
{
	char msg[4100];
	int max_bound;  /* 频率窗口上限 0x32f → 0x339 */
	int chain;                    /* param4 */
	int slot;                     /* param5 */
	int j;                        /* 频率表搜索计数 */
	int idx_cur;  /* 新目标索引 (升频路径/降频路径通用) */
	int idx_hi;  /* 复用: 上限索引 / 降频路径搜索临时 */
	int idx_dn;  /* 当前频率降一档索引 */
	int cur;  /* 当前频率 */
	int hi;  /* 上限 */
	int lo;  /* 复用: 下限 */
	int dir;  /* 1 = "up", -1 = "down" (日志方向) */
	int freq_sum;  /* 在位链频率和 */
	int settled;  /* 到达芯片数 */
	int avg;  /* 平均频率 */
	int k;

	every_chain_set_frequency(800);
	max_bound = 0x32f;
	for (;;) {                    /* 外层: 每轮重置后进入扫描 */
		for (chain = 0; chain != 4; chain++) {
			if (DAT_0009d4bc[chain] == '\x01') {
				for (k = 0; k != 3; k++) {
					DAT_0007c134[chain * 3 + k] = 0;
					DAT_0007c194[chain * 3 + k] = 0;
					DAT_0007c224[chain * 3 + k] = 800;
					DAT_0007c1f4[chain * 3 + k] = max_bound;
					DAT_0007c1c4[chain * 3 + k] = 0x307;
					DAT_0007c254[chain * 3 + k] = 0;
				}
			}
		}

		for (;;) {  /* 等待 + 推进 + 统计, 直到全部到达 */
			do {
				sleep(1);
				pthread_mutex_lock((pthread_mutex_t *)DAT_00079100);
				pthread_cond_wait((pthread_cond_t *)DAT_0007c078,
				                  (pthread_mutex_t *)DAT_00079100);
				pthread_mutex_unlock((pthread_mutex_t *)DAT_00079100);
			} while (DAT_00078e3c != '\0');

			for (chain = 0; chain != 4; chain++) {
				if (DAT_0009d4bc[chain] != '\x01') {
					continue;
				}
				for (slot = 0; slot != 3; ) {
					if (DAT_0007c194[chain * 3 + slot] != 0 ||          /* 降频标志 (flag) */
					    0xf < (uint32_t)DAT_0007c134[chain * 3 + slot]) {/* 无效 nonce > 15 */
						// ---- 降频路径 (原 LAB_00047680) ----
						lo = DAT_0007c1c4[chain * 3 + slot];
						cur = DAT_0007c224[chain * 3 + slot];
						if (lo == cur) {                                 /* 下限==当前 → 已到底 */
							DAT_0007c254[chain * 3 + slot] = 1;          /* 到达标志 */
							slot++;
							continue;
						}
						/* idx_dn: 当前频率降一档 (100 → -1, 命中 F[j] → j-1, 未命中 → -2) */
						if (cur == 100) {
							idx_dn = -1;
							idx_cur = (lo == 100) ? 0 : -1;
							if (lo != 100) {
								idx_cur = -1;
								for (j = 1; j != 0x73; j++) {
									if (DAT_00069204[j] == lo) {
										idx_cur = j;
										break;
									}
								}
							}
						} else {
							idx_dn = -2;
							for (j = 1; j != 0x73; j++) {
								if (DAT_00069204[j] == cur) {
									idx_dn = j - 1;
									break;
								}
							}
							if (lo == 100) {
								idx_cur = 0;
							} else {
								idx_cur = -1;
								for (j = 1; j != 0x73; j++) {
									if (DAT_00069204[j] == lo) {
										idx_cur = j;
										break;
									}
								}
							}
						}
						dir = -1;
						/* 新目标 = 下限索引向降档索引二分 (差 1 档则 +1) */
						if (idx_dn - idx_cur == 1) {
							idx_cur += 1;
						} else {
							idx_cur += (idx_dn - idx_cur) / 2;
						}
						DAT_0007c1f4[chain * 3 + slot] = DAT_00069204[idx_dn]; /* 上限 = 当前降一档 */
					} else {
						// ---- 升频路径 (原 LAB_0004751c) ----
						cur = DAT_0007c224[chain * 3 + slot];
						hi = DAT_0007c1f4[chain * 3 + slot];
						if (hi == cur) {                                  /* 已到上限 → 到达 */
							DAT_0007c254[chain * 3 + slot] = 1;
							slot++;
							continue;
						}
						DAT_0007c1c4[chain * 3 + slot] = cur;             /* 下限 = 当前频率 */
						/* idx_cur: 当前频率索引 (100 → 0 [该路径 flag 恒 0], 命中 j, 未命中 -1) */
						idx_cur = (cur == 100) ? 0 : -1;
						if (cur != 100) {
							for (j = 1; j != 0x73; j++) {
								if (DAT_00069204[j] == cur) {
									idx_cur = j;
									break;
								}
							}
						}
						/* idx_hi: 上限索引 (100 → 0, 命中 j, 未命中 -1) */
						idx_hi = (hi == 100) ? 0 : -1;
						if (hi != 100) {
							for (j = 1; j != 0x73; j++) {
								if (DAT_00069204[j] == hi) {
									idx_hi = j;
									break;
								}
							}
						}
						dir = 1;
						// 二分逼近 (原 LAB_0004759c)
						if (idx_hi - idx_cur == 1) {
							idx_cur += 1;
						} else {
							idx_cur += (idx_hi - idx_cur) / 2;
						}
					}

					/* ---- 编程下发 (原 do{}while(true) 体) ---- */
					cur = DAT_00069204[idx_cur];
					DAT_0007c224[chain * 3 + slot] = cur;
					if (DAT_00078e09 != '\0' &&
					    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 6 < DAT_000766c4)) {
						snprintf(msg, 0x1000,
						         "%s freq[%d-%d]: cur freq %d, [%d, %d], invalid nonce %d, ox status %d\n",
						         (dir == 1) ? "up" : "down", chain, slot, cur,
						         DAT_0007c1c4[chain * 3 + slot],
						         DAT_0007c1f4[chain * 3 + slot],
						         DAT_0007c134[chain * 3 + slot],
						         DAT_0007c194[chain * 3 + slot]);
						applog(7, msg, 0);
						cur = DAT_0007c224[chain * 3 + slot];
					}
					set_freq_asic(chain, slot, cur);
					slot++;
				}
			}

			/* 统计: 在位链频率和 / 到达芯片数 */
			freq_sum = 0;
			settled = 0;
			for (chain = 0; chain != 4; chain++) {
				if (DAT_0009d4bc[chain] == '\x01') {
					freq_sum += DAT_0007c224[chain * 3] + DAT_0007c224[chain * 3 + 1] +
					            DAT_0007c224[chain * 3 + 2];
					settled += DAT_0007c254[chain * 3] + DAT_0007c254[chain * 3 + 1] +
					           DAT_0007c254[chain * 3 + 2];
					if (DAT_00078e09 != '\0' &&
					    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 6 < DAT_000766c4)) {
						snprintf(msg, 0x1000, "chain%d %d %d %d\n", chain,
						         DAT_0007c224[chain * 3], DAT_0007c224[chain * 3 + 1],
						         DAT_0007c224[chain * 3 + 2]);
						applog(7, msg, 0);
					}
				}
			}
			if (settled == (uint32_t)DAT_0009d6cb * 3) {
				break;
			}
		}

		avg = (int)__aeabi_idivmod(freq_sum, settled);   /* 取 r0 商 (r1 余数) */
		if (800 < avg || max_bound == 0x339) {
			scan_freq_save_freq();
			if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 4 < DAT_000766c4) {
				snprintf(msg, 0x1000, "scan freq done, avg freq %d max %d, reboot cgminer\n",
				         avg, max_bound);
				applog(5, msg, 0);
			}
			system("/etc/init.d/cgminer.sh restart > /dev/null 2>&1 &");
			continue;  /* 原 goto (重置后重新等待) */
		}
		if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 4 < DAT_000766c4) {
			snprintf(msg, 0x1000, "scan freq again, avg freq %d max %d\n", avg, 0x339);
			applog(5, msg, 0);
		}
		max_bound = 0x339;                          /* 第二轮窗口上限 825 */
	}
}

/* -------------------------------------------------------------------------- */
// === verify_sol === 原 FUN_0004eaf4 @ 0x0004eaf4
// 来源: reversed/cgminer/12_freq_pll/FUN_0004eaf4.c | 分类: 12_freq_pll
// 语义: Equihash(200,9) 方案校验。返回 0 = 有效; 1 = 索引重复;
//       2 = 碰撞长度/索引树顺序错误或末轮 XOR 不为零。
//   1. expandArray(方案 0x540B, 输出 0x800B, 21 bit/元素) 展开出 512 个索引字;
//   2. 每个索引: 字节序翻转后 equihash_leaf_hash(blake2b 状态, (int)idx/2, 50B),
//      取其中 25B 半块再 expandArray(20 bit/元素) 成 30B, 与原 21bit 索引字拼成
//      34B 行 (bin2hex 调试调用结果立即 free, 原样保留), 共 512 行 0x4400B;
//   3. 9 轮归并 (每轮行数减半, 行内 30B→27B→...): 相邻行首 3 字节必须相等
//      ("invalid collision length"), 索引段必须有序 ("Index tree incorrectly
//      ordered") 且不得重复 ("duplicate indices"); 新行 = 两行 XOR (去前 3 字节)
//      + 两行索引段, 行尺寸按 2*索引宽+数据宽 重排;
//   4. 末轮剩 3 字节, 全零即有效。
int verify_sol(void *blake_state, const uint8_t *sol_data)
{
	uint8_t *rows;                /* __ptr: 行缓冲 0x4400 */
	uint8_t *rows_tmp;            /* __ptr_00: 归并输出缓冲 0x4400 */
	uint32_t expanded[512];  /* expandArray 展开的 512 个 21bit 索引字 */
	uint32_t *exp_cursor;  /* 展开数组读游标 (原指向 -4, 先自增后读) */
	uint8_t *row;  /* 行写游标 (行首 = 游标-0x1e) */
	uint8_t leaf50[50];  /* equihash_leaf_hash 的 50B 输出 */
	uint8_t half_exp[30];  /* 25B 半块 expandArray 30B 输出 */
	uint8_t xor_buf[32];  /* 归并 XOR 缓冲 (叶哈希区复用) */
	uint32_t idx_raw;  /* 展开索引字 (翻转后) */
	uint32_t idx_swap_hi;  /* 翻转中间值 (低字节未并, 21bit 下与 idx_raw 相等) */
	int sgn;  /* 翻转值符号位 (字节 7 位扩展) */
	int half_len;  /* 行内哈希长度 0x1e → 3 */
	int index_len;  /* 行内索引宽度 4 → 32 */
	int full_len;  /* 新行数据长度 0x1b → 3 */
	int row_count;  /* 当前行数 0x200 → 1 */
	int pair;  /* 已归并对数 */
	int pair2;  /* 已处理行数 (=pair*2) */
	int stride;  /* 源行距 = half_len+index_len */
	int dest_off;  /* 归并输出偏移 */
	int cmp;  /* 各比较/异或循环变量 */
	size_t z;  /* 末轮全零检查 */

	rows = malloc(0x4400);
	exp_cursor = expanded - 1;  /* 指向首元素前一位置 (原字节偏移 -4), 先自增后读 */
	rows_tmp = malloc(0x4400);
	memset(expanded, 0, 0x800);
	expandArray(sol_data, 0x540, (uint8_t *)expanded, 0x800, 0x15, 1);

	row = rows + 0x1e;
	for (cmp = 0; cmp != 0x200; cmp++) {
		exp_cursor++;
		idx_raw = *exp_cursor;
		/* 21bit 索引按字大端翻转 (低字节来源 A>>24 恒 0, 两步结果一致) */
		idx_swap_hi = idx_raw << 0x18 | (idx_raw >> 8 & 0xff) << 0x10 |
		              (idx_raw >> 0x10 & 0xff) << 8;
		idx_raw = idx_swap_hi | idx_raw >> 0x18;
		equihash_leaf_hash(blake_state, (int)idx_raw / 2, leaf50, 0x32);
		sgn = (int)idx_swap_hi >> 0x1f;
		expandArray(leaf50 + ((((int)idx_raw - sgn) & 1) + sgn) * 0x19, 0x19,
		            half_exp, 0x1e, 0x14, 0);
		/* 行 = 30B 展开数据 + 4B 原索引字 (大端序) */
		*(uint32_t *)(row - 0x1e) = *(uint32_t *)half_exp;
		*(uint32_t *)(row - 0x1a) = *(uint32_t *)(half_exp + 4);
		*(uint32_t *)(row - 0x16) = *(uint32_t *)(half_exp + 8);
		*(uint32_t *)(row - 0x12) = *(uint32_t *)(half_exp + 12);
		*(uint32_t *)(row - 0xe) = *(uint32_t *)(half_exp + 16);
		*(uint32_t *)(row - 10) = *(uint32_t *)(half_exp + 20);
		*(uint32_t *)(row - 6) = *(uint32_t *)(half_exp + 24);
		*(uint16_t *)(row - 2) = *(uint16_t *)(half_exp + 28);
		*(uint32_t *)row = exp_cursor[0];  /* 存翻转前原值 */
		free(bin2hex((char *)(row - 0x1e), 0x22));   /* 调试遗留: 转十六进制串后立即释放 */
		row += 0x22;
	}

	half_len = 0x1e;
	index_len = 4;
	full_len = 0x1b;
	row_count = 0x200;
	for (;;) {
		if (row_count == 0) {
			index_len = index_len << 1;       /* 无可归并行: 仅放宽索引宽度 */
		} else {
			stride = half_len + index_len;    /* 源行尺寸 */
			pair = 0;
			pair2 = 0;
			row = rows + stride;  /* 右行游标 */
			uint8_t *left = rows;
			do {
				/* 首部 3 字节必须相等 (碰撞长度检查) */
				for (cmp = 0; cmp != 3; cmp++) {
					if (left[cmp] != row[cmp]) {
						puts("Invalid solution: invalid collision length between StepRows");
						free(rows);
						free(rows_tmp);
						return 2;
					}
				}
				if (is_entry_less((int)row, (int)left, half_len, index_len) != 0) {
					puts("Invalid solution: Index tree incorrectly ordered");
					free(rows);
					free(rows_tmp);
					return 2;
				}
				if (check_duplicate_indices((int)left, (int)row, half_len, index_len) == 0) {
					puts("Invalid solution: duplicate indices");
					free(rows);
					free(rows_tmp);
					return 1;
				}
				memset(xor_buf, 0, sizeof(xor_buf));
				for (cmp = 0; cmp < half_len; cmp++) {
					xor_buf[cmp] = row[cmp] ^ left[cmp];
				}
				pair++;
				dest_off = (index_len * 2 + full_len) * (pair2 >> 1);
				pair2 = pair * 2;
				row += stride * 2;            /* 右行推进 (跳过一对) */
				{
					int left_idx_off = full_len + dest_off;
					int right_idx_off = left_idx_off + index_len;
					memcpy(rows_tmp + dest_off, xor_buf + 3, full_len);            /* XOR 数据 (去前 3 字节) */
					memcpy(rows_tmp + left_idx_off, left + half_len, index_len);   /* 左行索引段 */
					memcpy(rows_tmp + right_idx_off, left + half_len + stride, index_len); /* 右行索引段 */
				}
				left += stride * 2;
			} while (pair2 < row_count);
			row_count = pair;
			index_len = index_len * 2;
		}
		half_len += -3;
		memcpy(rows, rows_tmp, 0x4400);
		memset(rows_tmp, 0, 0x4400);
		if (half_len == 3) {
			break;
		}
		full_len += -3;
	}

	if (full_len == 0) {
		free(rows);
		free(rows_tmp);
		return 0;
	}
	/* 末轮 3 字节须全零 */
	if (rows[0] == '\0') {
		z = 0;
		do {
			z++;
			if (z == (size_t)full_len) {
				free(rows);
				free(rows_tmp);
				return 0;
			}
		} while (rows[z] == '\0');
	}
	free(rows);
	free(rows_tmp);
	return 2;
}

/* -------------------------------------------------------------------------- */
// === blake2b_init === 原 FUN_00054ed0 @ 0x00054ed0
// 来源: reversed/cgminer/12_freq_pll/FUN_00054ed0.c | 分类: 12_freq_pll
// 语义: BLAKE2b 初始化。outlen 不在 [1,64] 返回 -1; 否则构造 64B 参数块
//       ([0]=摘要长度, [1]=密钥长度 0, [2]=fanout 1, [3]=depth 1, 其余 0),
//       交给 blake2b_init_state 初始化状态并返回其结果。
int blake2b_init(void *state, int outlen)
{
	uint8_t param_block[64];  /* 64B 参数块 */
	int ret;

	if ((uint32_t)(outlen - 1) < 0x40) {
		param_block[0] = (uint8_t)outlen;
		param_block[1] = 0;       /* key_len */
		param_block[2] = 1;       /* fanout */
		param_block[3] = 1;       /* depth */
		memset(param_block + 4, 0, 60);
		ret = blake2b_init_state(state, param_block);
	} else {
		ret = -1;
	}
	return ret;
}

/* -------------------------------------------------------------------------- */
// === blake2b_init_key === 原 FUN_00055098 @ 0x00055098
// 来源: reversed/cgminer/12_freq_pll/FUN_00055098.c | 分类: 12_freq_pll
// 语义: BLAKE2b 带密钥初始化。条件: 1<=outlen<=64 且 key 非空且 0<keylen<=64,
//       否则返回 -1。参数块 [0]=outlen [1]=keylen [2]=fanout 1 [3]=depth 1;
//       初始化成功后把 128B 全零密钥块拷入密钥 (不足补零), blake2b_update 灌入
//       128 字节, 再经全局函数指针 DAT_00076b94 (memset 形参布局, 保密擦除) 清零
//       密钥块, 返回 0。
int blake2b_init_key(void *state, int outlen, void *key, uint32_t keylen)
{
	uint8_t param_block[64];  /* 64B 参数块 */
	uint8_t key_block[132];  /* 使用前 0x80 字节 */
	int ret;

	if (((uint32_t)(outlen - 1) < 0x40 && (key != NULL && keylen != 0)) && keylen < 0x41) {
		param_block[0] = (uint8_t)outlen;
		param_block[1] = (uint8_t)keylen;
		param_block[2] = 1;       /* fanout */
		param_block[3] = 1;       /* depth */
		memset(param_block + 4, 0, 60);
		ret = blake2b_init_state(state, param_block);
		if (ret > -1) {
			memset(key_block, 0, 0x80);
			memcpy(key_block, key, keylen);
			blake2b_update(state, key_block, 0x80);
			(*DAT_00076b94)(key_block, 0, 0x80);
			return 0;
		}
	}
	return -1;
}

/* -------------------------------------------------------------------------- */
// === sha256_process === 原 FUN_00055470 @ 0x00055470
// 来源: reversed/cgminer/12_freq_pll/FUN_00055470.c | 分类: 12_freq_pll
// 语义: SHA-256 压缩一个 64B 块 (滚动寄存器实现)。param_1 为上下文:
//       +0x00..+0x1f = 8 字状态 (本轮累加回写), +0x28..+0x67 = 64B 数据块
//       (按字大端装载)。K 常数表 = DAT_0006c580[64]。轮内结构与标准一致:
//       W = h+Σ1(e)+Ch(e,f,g)+K+W', e/f/g/h 取自滚动窗口 v[(r-3)&7]/[(r-1)&7]/
//       [(r-2)&7]/[r&7]; T2 = Σ0(a)+Maj(a,b,c), a/b/c 取 v[(r-7)&7]/[(r-6)&7]/
//       [(r-5)&7]; v[r&7] = T1+T2 且 v[(r-4)&7] += T1; 调度表 sched[16] 首轮从
//       消息块复制, 之后按 (5-r)&0xf / (-8-r)&0xf / (-r)&0xf 滚动更新。
void sha256_process(int state)
{
	uint32_t w[16];  /* 消息块大端装载 */
	uint32_t v[8];  /* 滚动工作寄存器 */
	uint32_t sched[16];  /* 消息调度表 */
	uint32_t *w_cursor;  /* 装载游标 (先自增后写, 见下方原指针说明) */
	const uint8_t *b0;  /* +0x29 字节 */
	const uint8_t *b1;  /* +0x28 字节 */
	const uint8_t *b2;  /* +0x2b 字节 */
	const uint8_t *b3;  /* +0x2a 字节 */
	int off;  /* 调度/装载字偏移 (0..0x3c) */
	int round_base;  /* 0/0x10/0x20/0x30 */
	int r;  /* 滚动轮游标 7..-8 */
	int prev_r;  /* r-1 */
	uint32_t w15;  /* v[(r-3)&7] ("e") */
	uint32_t w13;  /* v[(r-1)&7] ("f") */
	uint32_t w14;  /* v[(r-2)&7] ("g") */
	uint32_t w_ri;  /* v[r&7] ("h") */
	int kval;  /* K 常数 */
	uint32_t wv;  /* 本轮 W (首轮=消息字, 之后=调度) */
	uint32_t s5;  /* sched[(5-r)&0xf] (σ1 源) */
	uint32_t s8;  /* sched[(0xfff8-r)&0xf] (σ0 源) */
	uint32_t t;  /* 复用: T1/T2 中间值 */
	uint32_t a;  /* 复用: v[(r-7)&7] ("a") */
	int ri;  /* r&7 */

	// 原 puVar6 = local_c0+7, 自增后落入紧随 v 的消息块 (Ghidra 栈布局);
	// 转写为独立数组后直接指向 w 前一位置, 首次 ++ 落在 w[0]
	w_cursor = w - 1;
	for (off = 0; off != 0x40; off += 4) {
		b0 = (uint8_t *)(state + 0x29 + off);
		b1 = (uint8_t *)(state + 0x28 + off);
		b2 = (uint8_t *)(state + 0x2b + off);
		b3 = (uint8_t *)(state + 0x2a + off);
		w_cursor++;
		*w_cursor = (uint32_t)*b0 * 0x10000 + (uint32_t)*b1 * 0x1000000 +
		            (uint32_t)*b2 + (uint32_t)*b3 * 0x100;   /* 字大端装载 */
	}
	for (off = 0; off != 0x20; off += 4) {
		*(uint32_t *)((char *)v + off) = *(uint32_t *)(state + off);   /* v = 状态 */
	}
	round_base = 0;
	do {
		off = 0;
		r = 7;
		do {
			prev_r = r - 1;
			w15 = v[(r - 3) & 7];
			w13 = v[prev_r & 7];
			ri = r & 7;
			w14 = v[(r - 2) & 7];
			w_ri = v[ri];
			kval = DAT_0006c580[off / 4 + round_base];   /* 原 (&DAT_0006c580 + off + round_base*4) */
			if (round_base == 0) {
				wv = *(uint32_t *)((char *)w + off);
				*(uint32_t *)((char *)sched + off) = wv;             /* 首轮: 调度表 = 消息块 */
			} else {
				s5 = sched[(5 - r) & 0xf];
				s8 = sched[(0xfff8 - r) & 0xf];
				wv = sched[(-r) & 0xf] + *(uint32_t *)((char *)sched + off) +
				     ((s5 >> 0x11 | s5 << 0xf) ^ (s5 >> 0x13 | s5 << 0xd) ^ s5 >> 10) +
				     ((s8 >> 7 | s8 << 0x19) ^ (s8 >> 0x12 | s8 << 0xe) ^ s8 >> 3);
				*(uint32_t *)((char *)sched + off) = wv;
			}
			/* T1 = h + K + Σ1(e) + Ch(e,f,g) + W */
			t = w_ri + kval +
			    ((w15 >> 6 | w15 << 0x1a) ^ (w15 >> 0xb | w15 << 0x15) ^
			     (w15 >> 0x19 | w15 << 7)) +
			    (((w13 ^ w14) & w15) ^ w13) + wv;
			v[ri] = t;
			v[(r - 4) & 7] = t + v[(r - 4) & 7];
			/* T2 = Σ0(a) + Maj(a,b,c), v[r&7] = T1 + T2 */
			a = v[(r - 7) & 7];
			v[ri] = ((a >> 2 | a << 0x1e) ^ (a >> 0xd | a << 0x13) ^
			         (a >> 0x16 | a << 10)) +
			        (((a | v[(r - 6) & 7]) & v[(r - 5) & 7]) | (a & v[(r - 6) & 7])) +
			        v[ri];
			off += 4;
			r = prev_r;
		} while (prev_r != -9);  /* != 0xfffffff7: r 取遍 7..-8 共 16 字 */
		round_base += 0x10;
	} while (round_base != 0x40);
	for (off = 0; off != 0x20; off += 4) {
		*(int *)(state + off) = *(int *)(state + off) + *(int *)((char *)v + off);
	}
	return;
}

/* -------------------------------------------------------------------------- */
// === xxtea_encode === 原 FUN_00056b3c @ 0x00056b3c
// 来源: reversed/cgminer/12_freq_pll/FUN_00056b3c.c | 分类: 12_freq_pll
// 语义: XXTEA (btea) 加密 n 个 uint32 (n<2 打 "XXTEA encode fails, N <= 1
//       found." (LOG_ERR=3) 后返回)。轮数 = 6 + 52/n (经 __aeabi_idivmod(0x34,n)
//       取商; 调用点 Ghidra 漏渲染 r1 实参 n); 每轮 sum += 0x9e3779b9,
//       e = (sum&0xf)>>2, 对 p=0..n-2: v[p] += MX(sum,y,z,key[(p&3)^e]) (y=v[p+1],
//       z=v[p-1] 链式), 末步 v[n-1] += MX(sum,v[0],z,key[e^((n-1)&3)])。循环以
//       sum == q*(-0x61c88647)+0xb54cda56 (即 (6+52/n)*0x9e3779b9 mod 2^32) 退出。
void xxtea_encode(uint32_t *v, int n, const uint32_t *key)
{
	uint32_t z;  /* MX 的 z (链式) */
	uint32_t y;  /* MX 的 y */
	uint32_t sum;  /* 黄金比例累加 */
	uint32_t e;  /* (sum&0xf)>>2 */
	uint32_t p;  /* 字索引 */
	uint32_t p3;  /* p&3 */
	int last;  /* n-1 */
	int q;  /* 52/n */
	uint32_t *pv;

	if (n < 2) {
		if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 2 < DAT_000766c4) {
			/* Ghidra 逐字节栈上拼串, 还原为等价字面量 */
			applog(3, "XXTEA encode fails, N <= 1 found.\n", 0);
		}
		return;
	}
	last = n - 1;
	q = (int)__aeabi_idivmod(0x34, n);     /* 52/n (r0 商); Ghidra 漏渲染 r1=n */
	z = v[last];
	sum = 0;
	do {
		p = 0;
		sum += 0x9e3779b9;
		e = (sum & 0xf) >> 2;
		pv = v;
		do {
			p3 = p & 3;
			y = pv[1];
			p++;
			z = (((z ^ *(const uint32_t *)((const uint8_t *)key + (p3 ^ e) * 4)) +
			     (y ^ sum)) ^ ((z << 4 ^ y >> 3) + (z >> 5 ^ y << 2))) + *pv;
			*pv = z;
			pv++;
		} while (p != (uint32_t)last);
		y = *v;
		z = (((z ^ *(const uint32_t *)((const uint8_t *)key + (e ^ (last & 3)) * 4)) +
		     (sum ^ y)) ^ ((y >> 3 ^ z * 0x10) + (y << 2 ^ z >> 5))) + v[last];
		v[last] = z;
	} while (sum != (uint32_t)q * -0x61c88647U + 0xb54cda56U);
	return;
}

/* -------------------------------------------------------------------------- */
// === hashtable_set === 原 FUN_00057a8c @ 0x00057a8c
// 来源: reversed/cgminer/12_freq_pll/FUN_00057a8c.c | 分类: 12_freq_pll
// 语义: jansson hashtable 插入/更新 (hashtable_t 的 uint32 视图: [0]=size,
//       [1]=buckets, [2]=order (primes 索引), [3..4]=链表头 (prev@+12/next@+16))。
//   - size >= primes[order] 时扩容: 释放旧桶, order++, 按 primes[order]*8 分配新
//     桶 (每桶 first/last 指向链表头), 把旧链表上每个 pair (节点 = pair+4 的
//     prev/next) 按 pair->hash % 新 prime 重新 insert_to_bucket。
//   - hash = djb2 (初值 0x1505, hash = c + hash*33); slot = hash % primes[order]
//     (原 FUN_0005c6f0 __umodsi3, 取 r1 余数); hashtable_find_pair 查找:
//     未命中 → jsonp_malloc(strlen+0x15) 建 pair {hash@0, list@+4 (自环),
//     value@+12, serial@+16, key@+20 strcpy}, insert_to_bucket, size++;
//     命中 → 旧 value (pair+0xc) 引用计数 != -1 时递减, 减到 0 调 json_delete,
//     然后写入新 value。返回 0 成功 / -1 分配失败。
int hashtable_set(uint32_t *hashtable, const uint8_t *key, int serial, int value)
{
	uint32_t num_buckets;  /* primes[order] */
	uint32_t buckets;  /* 桶数组地址 */
	int prime_new;  /* 扩容后新桶数 (primes[order+1]) */
	int slot;                     /* 余数 (原 __umodsi3 的 r1) */
	int i;  /* 循环变量 */
	uint32_t ch;  /* djb2 当前字符 */
	int djb2;  /* djb2 哈希值 */
	int bucket_addr;  /* 桶地址 (buckets + slot*8) */
	int pair;  /* find 结果 pair 指针 */
	int *new_pair;  /* 新建 pair */
	int *node;  /* 新 pair 链表节点 (pair+4) */
	uint32_t *head;  /* 链表头 (hashtable+3) */
	uint32_t *walk;  /* 重排游标 */
	uint32_t *next;  /* 旧链表下一节点 */
	size_t key_len;
	int old_val;  /* 复用: 旧 value */
	int refcnt;                   /* 引用计数临时 */

	num_buckets = (uint32_t)DAT_0006cb74[hashtable[2]];
	if (hashtable[0] < num_buckets) {
		buckets = hashtable[1];
	} else {
		jsonp_free((void *)hashtable[1]);              /* 释放旧桶 */
		num_buckets = hashtable[2];
		hashtable[2] = num_buckets + 1;                /* order++ */
		prime_new = DAT_0006cb74[num_buckets + 1];
		buckets = (uint32_t)jsonp_malloc(prime_new << 3);
		hashtable[1] = buckets;
		if (buckets == 0) {
			return -1;
		}
		num_buckets = (uint32_t)DAT_0006cb74[hashtable[2]];
		if (num_buckets != 0) {                        /* 每桶 first/last = 链表头 */
			for (i = 0; i != (int)(num_buckets << 3); i += 8) {
				*(uint32_t **)(buckets + i + 4) = hashtable + 3;
				*(uint32_t **)(buckets + i) = hashtable + 3;
			}
		}
		head = hashtable + 3;
		walk = (uint32_t *)hashtable[4];               /* 旧链表首节点 (头重置前保存) */
		*head = (uint32_t)(size_t)head;                /* list.prev = 自身 */
		hashtable[4] = (uint32_t)(size_t)head;         /* list.next = 自身 */
		if (walk != head) {
			for (;;) {
				slot = (int)((uint32_t)walk[-1] % (uint32_t)prime_new); /* pair->hash % 新 prime (原 FUN_0005c6f0 取 r1) */
				next = (uint32_t *)walk[1];            /* 节点 next (pair+8) */
				insert_to_bucket(hashtable, (uint32_t *)(buckets + slot * 8), (int *)walk);
				if (head == next) {
					break;
				}
				buckets = hashtable[1];
				walk = next;
			}
			buckets = hashtable[1];
			num_buckets = (uint32_t)DAT_0006cb74[hashtable[2]];
		}
	}

	/* djb2: hash = 5381; hash = c + hash*33 */
	ch = (uint32_t)*key;
	djb2 = 0x1505;
	if (ch != 0) {
		const uint8_t *p = key;
		do {
			djb2 = (int)ch + djb2 * 0x21;
			p++;
			ch = (uint32_t)*p;
		} while (ch != 0);
	}
	slot = (int)((uint32_t)djb2 % num_buckets);    /* 原 FUN_0005c6f0 (__umodsi3) 取 r1 余数 */
	bucket_addr = (int)buckets + slot * 8;         /* 桶地址 */
	pair = hashtable_find_pair(hashtable, (uint32_t *)bucket_addr, (const char *)key, djb2);
	if (pair == 0) {
		key_len = strlen((const char *)key);
		new_pair = (int *)jsonp_malloc((int)key_len + 0x15);   /* strlen + 21 = pair 头 + key + NUL */
		if (new_pair == NULL) {
			return -1;
		}
		new_pair[0] = djb2;                            /* pair->hash */
		new_pair[4] = serial;                          /* pair->serial (+16) */
		strcpy((char *)(new_pair + 5), (const char *)key);  /* pair->key (+20) */
		node = new_pair + 1;                           /* &pair->list (+4) */
		new_pair[2] = (int)node;                       /* list.next = 自环 */
		new_pair[3] = value;                           /* pair->value (+12) */
		new_pair[1] = (int)node;                       /* list.prev = 自环 */
		insert_to_bucket(hashtable, (uint32_t *)bucket_addr, node);
		hashtable[0] = hashtable[0] + 1;               /* size++ */
		return 0;
	}
	/* 键已存在: 递减旧 value 引用计数并写入新 value */
	old_val = *(int *)(pair + 0xc);                    /* pair->value (+12) */
	if (old_val != 0 && *(int *)(old_val + 4) != -1) { /* json_t.refcount @+4 (-1 = 静态) */
		refcnt = *(int *)(old_val + 4) - 1;
		*(int *)(old_val + 4) = refcnt;
		if (refcnt == 0) {
			json_delete((void *)old_val);
		}
	}
	*(int *)(pair + 0xc) = value;
	return 0;
}

/* -------------------------------------------------------------------------- */
// === lex_scan === 原 FUN_0005852c @ 0x0005852c
// 来源: reversed/cgminer/12_freq_pll/FUN_0005852c.c | 分类: 12_freq_pll
// 语义: jansson load.c lex_scan — 扫描下一 token。lex_t (int 视图): +0x10=输入
//       缓冲位置, +0x24=位置计数, +0x28=saved_text (strbuffer_t), +0x2c=saved_text
//       长度, +0x34=token.type, +0x38/+0x3c=token.value。
//       token 类型: '{' '}' '[' ']' ':' ',' 用字符码; 0x100=STRING,
//       0x101=INTEGER (long long, strtoll, ERANGE 报 "too big (negative) integer"),
//       0x102=REAL (jsonp_strtod, 溢出报 "real number overflow"),
//       0x103=TRUE / 0x104=FALSE / 0x105=NULL ("true"/"false"/"null"),
//       0xffffffff=INVALID, stream EOF → 0 (token.type=0), STREAM_STATE_ERROR
//       (0xfffffffe) → INVALID。上一 token 为字符串时先释放 value.string.val。
//       跳过空白; 字符串经 saved_text 整体保存后从 +1 处解码 (转义 \u 代理对、
//       \b\f\n\r\t\\\"\/), malloc(saved_text 长度+1) 存放 NUL 结尾结果;
//       数字: '-'/[0-9] 起, 前导零限制、小数/指数校验; 字母: true/false/null;
//       其余: 流式输入分支 (从输入缓冲逐字符追加 saved_text 直到 NUL, Ghidra 漏
//       渲染 append 的字符实参) 后置 INVALID。
uint32_t lex_scan(int lex, int error)
{
	uint32_t c;  /* stream_get/lex_get_save 结果 */
	uint8_t ch;  /* saved_text 字节 */
	char cc;  /* 流式分支字符 */
	char *text;  /* 消息/文本指针 */
	char *msg;  /* 复用 */
	int *errno_loc;
	void *str_val;  /* 待释放的字符串值 */
	uint32_t type;  /* token 类型 */
	long long llval;  /* strtoll 结果 */
	char *endp;
	double dval;  /* jsonp_strtod 结果 */
	const uint8_t *save_pos;  /* saved_text 游标 (越过开引号) */
	uint8_t *dest;                /* __dest: 输出游标 */
	int codepoint;  /* \u 码点 */
	int low_surrogate;  /* 低位代理/循环计数复用 */
	size_t utf8_len;
	uint8_t utf8_buf[4];
	void *strbuf;  /* lex+0x28 (saved_text) */
	int hexcnt;                   /* \u 十六进制计数 */
	uint32_t d;  /* 复用: 十六进制校验临时 */
	int valid;  /* isxdigit 结果 */

	strbuf = (void *)(lex + 0x28);
	strbuffer_clear(strbuf);
	if (*(int *)(lex + 0x34) == 0x100) {          /* 上一个 token 是字符串 */
		jsonp_free(*(void **)(lex + 0x38));
		*(void **)(lex + 0x38) = NULL;
	}
	do {
		do {
			c = stream_get((uint32_t *)lex, error);
		} while (c == 0x20 || c == 9);            /* ' ' / '\t' */
	} while (c == 10 || c == 0xd);                /* '\n' / '\r' */
	if (c == 0xffffffff) {                        /* STREAM_STATE_EOF → TOKEN_EOF(0) */
		*(uint32_t *)(lex + 0x34) = 0;
		return 0;
	}
	if (c != 0xfffffffe) {                        /* 非 STREAM_STATE_ERROR */
		strbuffer_append_byte(strbuf, (uint8_t)c);        /* 保存首字符 */
		if (c == 0x7b || c == 0x7d || c == 0x5b || c == 0x5d || c == 0x3a || c == 0x2c) {
			/* { } [ ] : , */
			*(uint32_t *)(lex + 0x34) = c;
			return c;
		}
		if (c == 0x22) {
			/* ---- 字符串 ---- */
			*(void **)(lex + 0x38) = NULL;
			*(uint32_t *)(lex + 0x34) = 0xffffffff;       /* 先置 INVALID */
			for (;;) {
				c = lex_get_save(lex, error);
				for (;;) {
					if (c == 0x22) {                      /* 结束引号 (已保存) */
						dest = (uint8_t *)jsonp_malloc(*(int *)(lex + 0x2c) + 1);
						*(uint8_t **)(lex + 0x38) = dest;
						str_val = NULL;
						if (dest == NULL) {
							goto free_out;
						}
						save_pos = (const uint8_t *)strbuffer_value(strbuf) + 1;  /* 越过开引号 */
						goto decode;
					}
					if (c == 0xfffffffe) {                /* STREAM_STATE_ERROR */
						str_val = *(void **)(lex + 0x38);
						goto free_out;
					}
					if (c == 0xffffffff) {
						error_set((void *)error, lex, "premature end of input");
						str_val = *(void **)(lex + 0x38);
						goto free_out;
					}
					if (c < 0x20) {
						lex_unget_unsave(lex, c);
						if (c == 10) {
							msg = "unexpected newline";
						} else {
							msg = "control character 0x%x";
						}
						error_set((void *)error, lex, msg, c);
						str_val = *(void **)(lex + 0x38);
						goto free_out;
					}
					if (c != 0x5c) {
						break;                            /* 普通字符 → 读下一字符 */
					}
					c = lex_get_save(lex, error);         /* 转义字符 */
					if (c == 0x75) {                      /* \uXXXX: 4 个十六进制 */
						c = lex_get_save(lex, error);
						hexcnt = 4;
						do {
							/* 等价 isxdigit (0-9/A-F/a-f), 与原位运算展开逐值一致 */
							d = c - 0x30U;
							valid = (d < 10) | ((c - 0x41U) < 6) | ((c - 0x61U) < 6);
							if (!valid) {
								goto invalid_escape;
							}
							c = lex_get_save(lex, error);
							hexcnt--;
						} while (hexcnt != 0);
						/* c = \uXXXX 之后的首字符 */
					} else {
						/* 合法简单转义: " \ / b f n r t (f/n 以 (c&~8)==0x66 折叠) */
						if (!(((c == 0x22 || c == 0x5c) || c == 0x2f) || c == 0x62) &&
						    ((c & 0xfffffff7) != 0x66) && (c != 0x72 && c != 0x74)) {
							goto invalid_escape;
						}
						c = lex_get_save(lex, error);
					}
				}
			}
		}
		if (c == 0x2d || c - 0x30U < 10) {
			/* ---- 数字 ('-' 或 0-9 开头) ---- */
			*(uint32_t *)(lex + 0x34) = 0xffffffff;
			if (c == 0x2d) {
				c = lex_get_save(lex, error);             /* '-' 已保存 */
			}
			if (c == 0x30) {
				c = lex_get_save(lex, error);
				if (c - 0x30U < 10) {
					goto num_invalid;                     /* 前导零后跟数字 */
				}
			} else {
				if (9 < c - 0x30U) {                      // 首位非 1-9 → invalid
					lex_unget_unsave(lex, c);
					return *(uint32_t *)(lex + 0x34);
				}
				do {
					c = lex_get_save(lex, error);
				} while (c - 0x30U < 10);
			}
			if (c == 0x45 || c == 0x2e) {                 /* 'E' 或 '.' */
				if (c == 0x2e) {
					c = stream_get((uint32_t *)lex, error);   /* '.' 已保存; 小数首位不保存 */
					if (9 < c - 0x30U) {
						stream_unget(lex, c);
						return *(uint32_t *)(lex + 0x34);
					}
					strbuffer_append_byte(strbuf, (uint8_t)c);  /* 手工保存小数首位 */
					do {
						c = lex_get_save(lex, error);
					} while (c - 0x30U < 10);
				}
				if ((c & 0xffffffdf) != 0x45) {
					goto make_real;                       /* 非 e/E → 实数收尾 */
				}
			} else if (c != 0x65) {                       /* 'e' → 指数; 其余 → 整数 */
				lex_unget_unsave(lex, c);
				text = strbuffer_value(strbuf);
				errno_loc = __errno_location();
				*errno_loc = 0;
				llval = strtoll(text, &endp, 10);
				if (*errno_loc == 0x22) {                 /* ERANGE */
					if (llval < 0) {
						msg = "too big negative integer";
					} else {
						msg = "too big integer";
					}
					error_set((void *)error, lex, msg);
					return *(uint32_t *)(lex + 0x34);
				}
				if (endp != text + *(int *)(lex + 0x2c)) {
					__assert_fail("end == saved_text + lex->saved_text.length", "load.c", 0x206,
					              "lex_scan_number");
				}
				*(long long *)(lex + 0x38) = llval;
				*(uint32_t *)(lex + 0x34) = 0x101;        /* TOKEN_INTEGER */
				return 0x101;
			}
			/* 指数部分 */
			c = lex_get_save(lex, error);                 /* 'e'/'E' 已保存 */
			if (c == 0x2b || c == 0x2d) {
				c = lex_get_save(lex, error);
			}
			if (9 < c - 0x30U) {
				goto num_invalid;
			}
			do {
				c = lex_get_save(lex, error);
			} while (c - 0x30U < 10);
make_real:
			lex_unget_unsave(lex, c);
			if (jsonp_strtod(strbuf, &dval) == 0) {
				*(uint32_t *)(lex + 0x34) = 0x102;        /* TOKEN_REAL */
				*(double *)(lex + 0x38) = dval;  /* 原以两个 4 字节存储 */
				return 0x102;
			}
			error_set((void *)error, lex, "real number overflow");
			return *(uint32_t *)(lex + 0x34);
num_invalid:
			lex_unget_unsave(lex, c);
			return *(uint32_t *)(lex + 0x34);
		}
		if ((c & 0xffffffdf) - 0x41 < 0x1a) {             /* 字母 ((c|0x20) 在 a..z) */
			do {
				c = lex_get_save(lex, error);
			} while ((c & 0xffffffdf) - 0x41 < 0x1a);
			lex_unget_unsave(lex, c);
			text = strbuffer_value(strbuf);
			if (strcmp(text, "true") == 0) {
				*(uint32_t *)(lex + 0x34) = 0x103;        /* TOKEN_TRUE */
				return 0x103;
			}
			if (strcmp(text, "false") == 0) {
				*(uint32_t *)(lex + 0x34) = 0x104;        /* TOKEN_FALSE */
				return 0x104;
			}
			if (strcmp(text, "null") == 0) {
				*(uint32_t *)(lex + 0x34) = 0x105;        /* TOKEN_NULL */
				return 0x105;
			}
		} else {
			/* 流式输入分支: 从输入缓冲 (lex+8, 位置 lex+0x10) 逐字符追加 saved_text
			 * 直到 NUL; lex+0x24 位置计数随增。Ghidra 漏渲染 append 的字符实参。 */
			cc = *(char *)(lex + *(int *)(lex + 0x10) + 8);
			while (cc != '\0') {
				strbuffer_append_byte(strbuf, (uint8_t)cc);
				low_surrogate = *(int *)(lex + 0x10) + 1;
				*(int *)(lex + 0x10) = low_surrogate;
				*(int *)(lex + 0x24) = *(int *)(lex + 0x24) + 1;
				cc = *(char *)(lex + low_surrogate + 8);
			}
		}
	}
	type = 0xffffffff;                                    /* TOKEN_INVALID */
	*(uint32_t *)(lex + 0x34) = type;
	return type;

invalid_escape:
	error_set((void *)error, lex, "invalid escape");
	str_val = *(void **)(lex + 0x38);
	goto free_out;

decode:
	for (;;) {
		ch = *save_pos;
		if (ch == 0x22) {                                 /* saved_text 中的结束引号 */
			*dest = '\0';
			*(uint32_t *)(lex + 0x34) = 0x100;            /* TOKEN_STRING */
			return 0x100;
		}
		if (ch != 0x5c) {                                 /* 普通字符原样拷贝 */
			*dest = ch;
			save_pos++;
			dest++;
			continue;
		}
		ch = save_pos[1];
		if (ch == 0x75) {                                 /* \u */
			codepoint = decode_unicode_escape((char *)(save_pos + 1));
			if ((uint32_t)codepoint - 0xd800U < 0x400) {      /* 高代理 → 需配对 */
				if (save_pos[6] != 0x5c || save_pos[7] != 0x75) {
					error_set((void *)error, lex, "invalid Unicode '\\u%04X'", codepoint);
					str_val = *(void **)(lex + 0x38);
					goto free_out;
				}
				low_surrogate = decode_unicode_escape((char *)(save_pos + 7));
				save_pos += 0xc;                          /* 跳过 \uXXXX\uXXXX */
				if (0x3ff < (uint32_t)low_surrogate - 0xdc00U) {
					error_set((void *)error, lex, "invalid Unicode '\\u%04X\\u%04X'",
					          codepoint, low_surrogate);
					str_val = *(void **)(lex + 0x38);
					goto free_out;
				}
				codepoint = (low_surrogate - 0xdc00) + (codepoint - 0xd800) * 0x400 + 0x10000;
			} else {
				if ((uint32_t)codepoint - 0xdc00U < 0x400) {  /* 孤立低代理 */
					error_set((void *)error, lex, "invalid Unicode '\\u%04X'", codepoint);
					str_val = *(void **)(lex + 0x38);
					goto free_out;
				}
				if (codepoint == 0) {
					error_set((void *)error, lex, "\\u0000 is not allowed");
					str_val = *(void **)(lex + 0x38);
					goto free_out;
				}
				save_pos += 6;                            /* 跳过 \uXXXX */
			}
			if (utf8_encode(codepoint, utf8_buf, &utf8_len) != 0) {
				__assert_fail("0", "load.c", 0x1ae, "lex_scan_string");
			}
			memcpy(dest, utf8_buf, utf8_len);
			dest += utf8_len;
			continue;                                     /* 回到解码循环顶部 */
		}
		if (ch == 0x66) {                                 /* \f */
			*dest = 0xc;
		} else if (ch < 0x67) {
			if (ch != 0x5c) {
				if (0x5c < ch) {
					if (ch != 0x62) {                     /* 非法 (校验已保证不发生) */
						__assert_fail("0", "load.c", 0x1bc, "lex_scan_string");
					}
					*dest = 8;                            /* \b */
					dest++;
					save_pos += 2;
					continue;
				}
				if (ch != 0x22 && ch != 0x2f) {           /* \" \/ 以外非法 */
					__assert_fail("0", "load.c", 0x1bc, "lex_scan_string");
				}
			}
			*dest = ch;                                   /* \\ \" \/ → 自身 */
		} else if (ch == 0x72) {                          /* \r */
			*dest = 0xd;
		} else if (ch == 0x74) {                          /* \t */
			*dest = 9;
		} else {
			if (ch != 0x6e) {                             /* \n 以外非法 */
				__assert_fail("0", "load.c", 0x1bc, "lex_scan_string");
			}
			*dest = 10;
		}
		dest++;
		save_pos += 2;
	}

free_out:
	jsonp_free(str_val);
	return *(uint32_t *)(lex + 0x34);
}

/* -------------------------------------------------------------------------- */
// === parse_value === 原 FUN_00058d00 @ 0x00058d00
// 来源: reversed/cgminer/12_freq_pll/FUN_00058d00.c | 分类: 12_freq_pll
// 语义: jansson load.c parse_value — 按 token.type 构造 json_t (返回值, 失败 0):
//       0x100 STRING → json_string_nocheck(value.string.val); 0x101 INTEGER →
//       flags&8 (JSON_DECODE_INT_AS_REAL) 时 jsonp_strtod(saved_text) 成功则
//       json_real 否则报 "real number overflow", 否则 json_integer(long long);
//       0x102 REAL → json_real(value.real); 0x103/4/5 → json_true/false/null;
//       0x5b '[' → json_array + 循环 parse_value/json_array_append (增减引用
//       计数, refcount@+4, -1 为静态), 分隔 ','、结束 ']'、异常报 "']' expected";
//       0x7b '{' → json_object_new + 键值循环: flags&1 (JSON_REJECT_DUPLICATES)
//       时 json_object_get 查重报 "duplicate object key"; 期待 ':' ("':' expected"),
//       json_object_set_nocheck 失败时释放键并减引用, 逗号分隔, '}' 结束, 否则
//       "'}' expected"/"string or '}' expected"; -1 INVALID → "invalid token";
//       其余 → "unexpected token"。错误路径统一递减容器引用计数 (减到 0
//       json_delete) 后返回 0。
int parse_value(int lex, uint32_t flags, int error)
{
	int type;  /* token.type */
	int json;  /* 复用: 结果 json_t* */
	int value;  /* 子 parse_value 结果 */
	int key;  /* 对象键字符串 (所有权转移) */
	int refcnt;  /* 引用计数临时 */
	double dval;  /* jsonp_strtod 输出 */

	type = *(int *)(lex + 0x34);
	if (type == 0x101) {                                  /* TOKEN_INTEGER */
		if ((flags & 8) == 0) {                           /* 非 JSON_DECODE_INT_AS_REAL */
			return (int)json_integer(*(long long *)(lex + 0x38));
		}
		if (jsonp_strtod((void *)(lex + 0x28), &dval) == 0) {
			return (int)json_real(dval);
		}
		error_set((void *)error, lex, "real number overflow");
		return 0;
	}
	if (type < 0x102) {
		if (type == 0x5b) {                               /* '[' → 数组 */
			json = (int)json_array();
			if (json == 0) {
				return 0;
			}
			lex_scan(lex, error);
			type = *(int *)(lex + 0x34);                  /* 首元素 token */
			if (type == 0x5d) {                           /* 空数组 */
				return json;
			}
			while (type != 0) {
				value = parse_value(lex, flags, error);
				if (value == 0) {
					goto error_free;
				}
				if (*(int *)(value + 4) != -1) {          /* json_incref */
					*(int *)(value + 4) = *(int *)(value + 4) + 1;
				}
				if (json_array_append((int *)json, (int *)value) != 0) {
					refcnt = *(int *)(value + 4);
					goto decref_value;
				}
				refcnt = *(int *)(value + 4) - 1;         /* json_decref */
				if (*(int *)(value + 4) != -1) {
					*(int *)(value + 4) = refcnt;
					if (refcnt == 0) {
						json_delete((void *)value);
					}
				}
				lex_scan(lex, error);
				if (*(int *)(lex + 0x34) != 0x2c) {       /* ',' */
					if (*(int *)(lex + 0x34) == 0x5d) {   /* ']' */
						return json;
					}
					break;
				}
				lex_scan(lex, error);
				type = *(int *)(lex + 0x34);
			}
			error_set((void *)error, lex, "']' expected");
			goto error_free;
		}
		if (type < 0x5c) {
			if (type == -1) {                             /* TOKEN_INVALID */
				error_set((void *)error, lex, "invalid token");
				return 0;
			}
		} else if (type == 0x7b) {                        /* '{' → 对象 */
			json = (int)json_object_new();
			if (json == 0) {
				return 0;
			}
			lex_scan(lex, error);
			if (*(int *)(lex + 0x34) == 0x7d) {           /* 空对象 */
				return json;
			}
			if (*(int *)(lex + 0x34) == 0x100) {          /* TOKEN_STRING → 键 */
				key = *(int *)(lex + 0x38);
				*(void **)(lex + 0x38) = NULL;            /* 键所有权转移 */
				if (key == 0) {
					return 0;
				}
				do {
					if ((flags & 1) != 0 &&
					    (json_object_get((void *)json, (const char *)key) != NULL)) {
						jsonp_free((void *)key);
						error_set((void *)error, lex, "duplicate object key");
						goto error_free;
					}
					lex_scan(lex, error);
					if (*(int *)(lex + 0x34) != 0x3a) {   /* ':' */
						jsonp_free((void *)key);
						error_set((void *)error, lex, "':' expected");
						goto error_free;
					}
					lex_scan(lex, error);
					value = parse_value(lex, flags, error);
					if (value == 0) {
						jsonp_free((void *)key);
						goto error_free;
					}
					if (*(int *)(value + 4) != -1) {      /* json_incref */
						*(int *)(value + 4) = *(int *)(value + 4) + 1;
					}
					if (json_object_set_nocheck((void *)json, (const char *)key,
					                            (void *)value) != 0) {
						jsonp_free((void *)key);
						refcnt = *(int *)(value + 4);
decref_value:
						if (refcnt != -1) {               /* json_decref (原 joined 块) */
							refcnt = refcnt - 1;
							*(int *)(value + 4) = refcnt;
							if (refcnt == 0) {
								json_delete((void *)value);
							}
						}
						goto error_free;
					}
					if (*(int *)(value + 4) != -1) {      /* json_decref */
						refcnt = *(int *)(value + 4) - 1;
						*(int *)(value + 4) = refcnt;
						if (refcnt == 0) {
							json_delete((void *)value);
						}
					}
					jsonp_free((void *)key);
					lex_scan(lex, error);
					if (*(int *)(lex + 0x34) != 0x2c) {   /* ',' */
						if (*(int *)(lex + 0x34) == 0x7d) {
							return json;
						}
						error_set((void *)error, lex, "'}' expected");
						goto error_free;
					}
					lex_scan(lex, error);
					if (*(int *)(lex + 0x34) != 0x100) {
						break;                            /* 键不是字符串 */
					}
					key = *(int *)(lex + 0x38);
					*(void **)(lex + 0x38) = NULL;
					if (key == 0) {
						return 0;
					}
				} while (1);
			}
			error_set((void *)error, lex, "string or '}' expected");
			goto error_free;
		}
	} else {
		if (type == 0x103) {                              /* TOKEN_TRUE */
			return (int)json_true();
		}
		if (type < 0x103) {                               /* 0x102 = TOKEN_REAL */
			return (int)json_real(*(double *)(lex + 0x38));  /* Ghidra 以 8 字节整型截断渲染 double 传参 */
		}
		if (type == 0x104) {                              /* TOKEN_FALSE */
			return (int)json_false();
		}
		if (type == 0x105) {                              /* TOKEN_NULL */
			return (int)json_null();
		}
	}
	error_set((void *)error, lex, "unexpected token");
	return 0;

	// 原 LAB_00058fcc: 递减容器引用计数 (减到 0 调 json_delete), 返回 0
	error_free:  /* 递减容器引用 */
	if (*(int *)(json + 4) == -1 ||
	    (refcnt = *(int *)(json + 4) - 1, *(int *)(json + 4) = refcnt, refcnt != 0)) {
		return 0;
	}
	json_delete((void *)json);
	return 0;
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* 转写辅助: ARM CLZ 指令语义 (前导零计数, 输入 0 返回 32) — 对应反编译 LZCOUNT */
static int lzcount32(uint32_t x)
{
	int n = 0;

	if (x == 0) {
		return 32;
	}
	while ((int32_t)x >= 0) {
		x = x << 1;
		n++;
	}
	return n;
}

// === __aeabi_idiv === 原 FUN_0005c718 @ 0x0005c718
// 来源: reversed/cgminer/12_freq_pll/FUN_0005c718.c | 分类: 12_freq_pll
// 语义: libgcc 有符号 32 位除法 (r0=商)。路径: |除数|==1 → ±被除数;
//       |被除数|<=|除数| → 0 或 ±1 (符号 = (x^y)>>31); 除数为 2 的幂 → 移位
//       (31-CLZ(除数)); 其余 → 基 16 恢复式除法 (每轮 4 位, CLZ 对齐)。注意:
//       本函数无除零检查 (除 0 时按原算术路径产生未定义结果, 原码如此);
//       Ghidra 的 in_NG 标志寄存器伪条件按 "(int)除数 < 0" 还原;
//       LZCOUNT 为 ARM CLZ 指令 (0 → 32)。
uint32_t __aeabi_idiv(uint32_t param_1, uint32_t param_2)
{
	uint32_t sign;  /* 被除数^除数 (商符号) */
	uint32_t bmag;  /* |除数| / 位掩码 */
	uint32_t amag;  /* |被除数| / 余数 */
	uint32_t div;  /* 对齐除数 */
	uint32_t quot;  /* 商 */
	int done;
	uint32_t shift;               /* CLZ 差 */

	sign = param_1 ^ param_2;
	bmag = param_2;
	if ((int)param_2 < 0) {       /* 原 in_NG (N 标志来自对 param_2 的取负判定) */
		bmag = -param_2;
	}
	if (bmag - 1 == 0) {                                  /* |b| == 1 */
		if ((int)param_2 < 0) {
			param_1 = -param_1;
		}
		return param_1;
	}
	amag = param_1;
	if ((int)param_1 < 0) {
		amag = -param_1;
	}
	if (amag <= bmag) {
		if (amag < bmag) {
			param_1 = 0;
		}
		if (amag == bmag) {
			param_1 = (int)sign >> 0x1f | 1;
		}
		return param_1;
	}
	if ((bmag & (bmag - 1)) == 0) {                       /* 2 的幂 → 移位 */
		amag = amag >> ((0x1f - lzcount32(bmag)) & 0xff);
		if ((int)sign < 0) {
			amag = -amag;
		}
		return amag;
	}
	shift = (lzcount32(bmag) - lzcount32(amag)) & 0xff;
	div = bmag << shift;
	bmag = 1 << shift;                                    /* 位掩码 */
	quot = 0;
	for (;;) {
		if (div <= amag) {
			amag = amag - div;
			quot = quot | bmag;
		}
		if (div >> 1 <= amag) {
			amag = amag - (div >> 1);
			quot = quot | bmag >> 1;
		}
		if (div >> 2 <= amag) {
			amag = amag - (div >> 2);
			quot = quot | bmag >> 2;
		}
		if (div >> 3 <= amag) {
			amag = amag - (div >> 3);
			quot = quot | bmag >> 3;
		}
		done = amag == 0;
		if (!done) {
			bmag = bmag >> 4;
			done = bmag == 0;
		}
		if (done) {
			break;
		}
		div = div >> 4;
	}
	if ((int)sign < 0) {
		quot = -quot;
	}
	return quot;
}

/* -------------------------------------------------------------------------- */
// === __aeabi_idivmod === 原 FUN_0005c710 @ 0x0005c710
// 来源: reversed/cgminer/12_freq_pll/FUN_0005c710.c | 分类: 12_freq_pll
// 语义: libgcc 有符号 32 位除法取余 (r0=商, r1=余数, ARM ABI 以 64 位返回值
//       呈现)。与 __aeabi_idiv 相同的算法, 另有: 除数 0 → raise(SIGFPE=8) 并以
//       其返回值作低 32 位返回; 各早退路径的返回值按反编译 CONCAT44(高位, 低位)
//       原样保留 (高位 = 对应中间量, Ghidra 对 r1 的寄存器追踪结果; 本二进制内
//       全部调用点仅消费低 32 位商)。
uint64_t __aeabi_idivmod(uint32_t param_1, uint32_t param_2)
{
	uint32_t sign;
	uint32_t bmag;
	uint32_t amag;
	uint32_t div;
	uint32_t quot;
	uint32_t raise_ret;  /* raise(8) 返回值 */
	int done;
	uint32_t shift;
	uint64_t ret64;

	if (param_2 == 0) {
		raise_ret = (uint32_t)raise(8);
		return (uint64_t)raise_ret;
	}
	sign = param_1 ^ param_2;
	bmag = param_2;
	if ((int)param_2 < 0) {
		bmag = -param_2;
	}
	if (bmag - 1 == 0) {                                  /* |b| == 1 */
		if ((int)param_2 < 0) {
			param_1 = -param_1;
		}
		return (uint64_t)bmag << 32 | param_1;
	}
	amag = param_1;
	if ((int)param_1 < 0) {
		amag = -param_1;
	}
	if (amag <= bmag) {
		if (amag < bmag) {
			param_1 = 0;
		}
		if (amag == bmag) {
			param_1 = (int)sign >> 0x1f | 1;
		}
		return (uint64_t)bmag << 32 | param_1;
	}
	if ((bmag & (bmag - 1)) == 0) {                       /* 2 的幂 → 移位 */
		amag = amag >> ((0x1f - lzcount32(bmag)) & 0xff);
		if ((int)sign < 0) {
			amag = -amag;
		}
		return (uint64_t)bmag << 32 | amag;
	}
	shift = (lzcount32(bmag) - lzcount32(amag)) & 0xff;
	div = bmag << shift;
	bmag = 1 << shift;
	quot = 0;
	for (;;) {
		if (div <= amag) {
			amag = amag - div;
			quot = quot | bmag;
		}
		if (div >> 1 <= amag) {
			amag = amag - (div >> 1);
			quot = quot | bmag >> 1;
		}
		if (div >> 2 <= amag) {
			amag = amag - (div >> 2);
			quot = quot | bmag >> 2;
		}
		if (div >> 3 <= amag) {
			amag = amag - (div >> 3);
			quot = quot | bmag >> 3;
		}
		done = amag == 0;
		if (!done) {
			bmag = bmag >> 4;
			done = bmag == 0;
		}
		if (done) {
			break;
		}
		div = div >> 4;
	}
	if ((int)sign < 0) {
		quot = -quot;
	}
	ret64 = (uint64_t)div << 32 | quot;  /* 原以 CONCAT44(高位中间量, 商) 组合返回 */
	return ret64;
}

/* ============================================================================
 * 覆盖率表: freq_b3 批次 16/16 函数 (来源 batch_plan.tsv, 命名 name_map/cgminer.tsv)
 *
 *  #  原地址      语义名                          反编译行数  转写说明
 *  1  FUN_000150f8 scanhash_scrypt                 113   scrypt 哈希循环: BE 装载+nonce 扫描+target 命中写回
 *  2  FUN_0003af90 bitmain_scanhash                235   nonce fifo 出队→work 查找→blake2b/sha256 预筛→verify_sol→统计提交
 *  3  FUN_0004388c turbo_mode_monitor_func         287   turbo 监控线程: 初始化均值→逐芯片查表逼近→全到达后复位+等待/降频循环
 *  4  FUN_00046cbc check_freq_and_set_work_mode    156   启动模式判定: PIC 配置/配置文件缺失→扫频线程, 有配置→turbo 线程
 *  5  FUN_000470a8 scan_freq_save_freq             143   扫频结果保存: 文件 fprintf / 表条目号+XXTEA+写 PIC flash
 *  6  FUN_00047354 scan_freq_func                  220   扫频线程: 800 起步, cond 等待驱动, [775,815]→[775,825] 二分逼近, 收尾保存+重启
 *  7  FUN_0004eaf4 verify_sol                      168   Equihash(200,9) 校验: expandArray 索引→叶哈希→9 轮归并→末轮全零
 *  8  FUN_00054ed0 blake2b_init                     82   BLAKE2b 初始化 (参数块 fanout/depth=1)
 *  9  FUN_00055098 blake2b_init_key                 87   BLAKE2b 带密钥初始化 (128B 密钥块 + 保密擦除)
 * 10  FUN_00055470 sha256_process                   94   SHA-256 压缩 (滚动寄存器实现, K 表 DAT_0006c580)
 * 11  FUN_00056b3c xxtea_encode                     93   XXTEA 加密 (轮数 6+52/n, 黄金比例 0x9e3779b9)
 * 12  FUN_00057a8c hashtable_set                   103   jansson 哈希表插入/更新 (djb2+primes 扩容重排+引用计数)
 * 13  FUN_0005852c lex_scan                        315   jansson 词法扫描 (字符串转义/\u 代理对/数字/true-false-null/流式分支)
 * 14  FUN_00058d00 parse_value                     176   jansson 值解析 (数组/对象/标量, 引用计数与错误路径)
 * 15  FUN_0005c710 __aeabi_idivmod                  81   libgcc 有符号除法取余 (r0 商/r1 余数; 本二进制调用点仅用商)
 * 16  FUN_0005c718 __aeabi_idiv                     78   libgcc 有符号除法 (无除零检查, 原码如此)
 *
 * 语句行覆盖: 各函数反编译体逐行对应 (Ghidra 打散的字节序翻转/逐 4 字节字符串
 * 拼装/向量运算按约定还原为等价字面量与算式, 见各函数注释)。已消除: Ghidra extraout(余数寄存器)
 * (→ __umodsi3 语义 = unsigned %), in_NG (→ (int)x < 0), VectorShiftLeft/VectorSub
 * (→ (x<<16)-x), CONCAT44 (→ 64 位组合)。
 * 说明: 函数 7-14 为链入 12_freq_pll 区段的第三方库代码 (blake2b/sha2/xxtea/
 * jansson), 15-16 为 libgcc; 均按体原样转写。
 * ========================================================================== */




