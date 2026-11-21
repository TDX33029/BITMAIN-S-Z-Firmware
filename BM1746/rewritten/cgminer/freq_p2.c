// ============================================================================
// freq_p2.c — cgminer 哈希/scrypt 层第 2 段 4 个大函数语义转写
// 覆盖: FUN_0000fc90 / FUN_000122e8 / FUN_00049cc8 / FUN_0004e198
// 来源: reversed/cgminer/12_freq_pll/{FUN_0000fc90,FUN_000122e8,
//       FUN_00049cc8,FUN_0004e198}.c | 分类: 12_freq_pll
// 命名: rewritten/name_map/cgminer.tsv
//   FUN_0000fc90 = sha256_transform_scrypt   (scrypt 侧 SHA256 压缩)
//   FUN_000122e8 = scrypt_N_1_256            (scrypt N=1 变体主循环)
//   FUN_00049cc8 = bitmain_ZCASH_init        (驱动主初始化, ARTIFACTS §4.3)
//   FUN_0004e198 = equihash_verify_solution_full (Equihash 解校验)
// 语义说明: 见各函数横幅。
// ============================================================================

#include <stdint.h>
#include <string.h>
#include <stdio.h>    /* snprintf — 原调用保留 */
#include <stdlib.h>   /* system/exit — 原调用保留 */
#include <unistd.h>   /* sleep/access — 原调用保留 */

#include "../include/common_types.h"
#include "../include/bm1746_regs.h"
#include "../include/fpga_axi.h"    /* AXI_REG_CHAIN_RESET (base+0x34) */

/* ================= 外部函数 (name_map 语义名, 定义在各批次文件) ================= */

extern void  applog(uint32_t prio, char *buf, ...);  /* FUN_00038730 */
extern void  cgsleep_ms(int ms);                                    /* FUN_0002a92c (util.c) */
extern void  start_http_listen_thread(void);                        /* FUN_00056a90 */
extern void  bitmain_axi_open(void);                                /* FUN_0003b704 (axi.c) */
extern void  init_fpga(void);                                       /* FUN_0003ba70 (axi.c) */
extern void  set_baud_pl(uint8_t baud_sel);                         /* FUN_0003bb44 (chain.c) */
extern void  set_baud(uint32_t bt8d);                               /* FUN_00043f04 (chain.c) */
extern void  fan_power_gpio_on(void);                               /* FUN_00045b30 (fan.c) */
extern void  set_fan_pwm(int pwm);                                  /* FUN_00045d88 (fan.c) */
extern void  set_PWM_according_to_temperature(void);                /* FUN_00045ec0 (fan.c) */
extern void  check_fan_number(void);                                /* FUN_00045b50 (fan.c) */
extern int   create_bitmain_check_fan_pthread(void);                /* FUN_000498f4 (fan.c) */
extern int   create_bitmain_read_temp_pthread(void);                /* FUN_00049be8 (fan.c) */
extern int   create_bitmain_check_miner_status_pthread(void);       /* FUN_000499d4 */
extern int   create_bitmain_get_hash_rate_pthread(void);            /* FUN_00049ac0 */
extern void  check_chain(void);                                     /* FUN_00044978 (chain.c) */
extern void  calculate_address_interval(void);                      /* FUN_000467b0 (chain.c) */
extern void  clear_reg_response_queue(void);                        /* FUN_000481b4 (chain.c) */
extern void  check_every_chain_asic_number(uint32_t count_mode);    /* FUN_00049578 (chain.c) */
extern void  recheck_asic_num(int thr_base, uint32_t chain);        /* FUN_00049700 (chain.c) */
extern void  check_freq_and_set_work_mode(void);                    /* FUN_00046cbc */
extern void  every_chain_set_ticket_mask(uint8_t mask);             /* FUN_00044104 */
extern void  every_chain_software_set_address(void);                /* FUN_00043060 */
extern void  every_chain_set_core_timeout(uint32_t timeout);        /* FUN_000432c0 */
extern void  every_chain_set_nonce_shift(int val);                  /* FUN_0004330c */
extern void  every_chain_enable_read_temperature_from_asic(void);   /* FUN_0004614c */
extern void  every_chain_init_temperature_sensor(void);             /* FUN_000463c8 */
extern void  every_chain_set_analog_mux_control(int val);           /* FUN_00043594 */
extern void  every_chain_reset_PIC16F1704_pic_new(void);            /* FUN_00041308 (pic.c) */
extern void  every_chain_jump_from_loader_to_app_PIC16F1704_new(void); /* FUN_000413f0 (pic.c) */
extern void  every_chain_disable_PIC16F1704_dc_dc_new(void);        /* FUN_000414d8 (pic.c) */
extern void  every_chain_enable_PIC16F1704_dc_dc_new(void);         /* FUN_00041614 (pic.c) */
extern int   send_heart_beat_to_every_chain(void);                  /* FUN_000416ec (pic.c) */
extern void  save_freq_to_every_chain_pic(void *buf);               /* FUN_00046b94 */
extern void  check_temperature_offset_value(void);                  /* FUN_000442b0 */
extern void  reset_all_hash_board_low(void);                        /* FUN_0003a354 */
extern void  tty_init(int thr_base);                                /* FUN_000448bc */

/* ================= 外部全局 (DAT_ 原名保留, 注释为已核实语义) ================= */

extern uint8_t  DAT_0007ad48;     /* applog 门控 A (log.c) */
extern uint8_t  DAT_00078e08;     /* applog 门控 B = opt_verbose (log.c) */
extern int      DAT_000766c4;     /* applog 门控 C: 日志级别 (log.c) */
extern int      DAT_0007b198;     /* total_pools: 已配置矿池数 (axi.c) */
extern void   **DAT_0007b438;     /* pools: 矿池指针数组基址 (total_pools 项) */
extern uint8_t  DAT_0009d6f3;     /* 芯片地址步进 asic_addr_interval (chain.c; 此处先置 1) */
extern uint8_t  DAT_0009d6f7;     /* fan_eft: 1=固定 PWM, 0=按温度自动 PWM (fan.c) */
extern uint32_t DAT_0009d6f8;     /* 字节布局: [0]=风扇PWM% (fan.c 以 uint8 视图引用),
                                     [1..2]=目标频率 MHz (LE16, 800<<8), [3]未用 */
extern uint8_t  DAT_0009d6f2;     /* 全局, 语义待定: 设址前步进有效标志 (置 1) */
extern char     DAT_00078b94[];   /* 版本串缓冲低段 (axi.c): 此处写 "9.0." */
extern char     DAT_00078b98[];   /* 版本串缓冲高段: 此处写 "0.5\0" (合成 "9.0.0.5") */
extern uint8_t  DAT_00076b78;     /* 全局: set_fan_pwm 上电初值 (PWM 百分比) */
extern char     DAT_00078e28;     /* 温度/矿工数联动开关 (fan.c) */
extern int      DAT_00078e38;     /* chain_rate 计算分母 (fan.c): 此处快照在位链数 */
extern uint8_t  DAT_0009d6cb;     /* 在位链计数 (chain.c check_chain 统计) */
extern char     DAT_0009d4bb[];   /* +1 (0x9d4bc) 起为链存在标志; +1+0x209 (0x9d6c5) 起为每链 ASIC 数 */
extern uint8_t *DAT_00078e10;     /* FPGA AXI 寄存器窗口 mmap 基址 (axi.c) */
extern int      DAT_0009d4c4;     /* dev.timeout (µs); 位2置位时 = 100000000 (100ms) */
extern uint8_t  DAT_00079bf4[];   /* 每链 recheck 状态标志 (0x79bf4+chain) */
extern char     DAT_000790ec[];   /* 每链"已重初始化"标志数组 (hash.c), 初值置 1 */
extern uint8_t  DAT_00076b60;     /* ticket mask 运行值 (出厂 0x10, chain.c) */
extern uint32_t DAT_00076b74;     /* 核心超时运行值 (出厂 84000, bm1746_regs.h) */
extern uint16_t DAT_00068aec[];   /* CRC16 查找表 (256×u16, CRC-CCITT 表驱动), 语义待定 */
extern uint8_t  DAT_00076b90;     /* 全局, 语义待定: 校验进度指示 (初值 3, 每轮 +3) */

/* ---- Equihash 校验辅助 (定义在其他批次) ---- */
extern void  expandArray(const void *in, size_t in_len, void *out, size_t out_len,
                         size_t bit_len, size_t byte_pad);              /* FUN_0004df94 */
extern void  equihash_leaf_hash(const void *base, uint32_t val, void *out,
                         size_t out_len);                               /* FUN_0004df30 */
extern void  check_index_tree_order(uint32_t *arr, int count);          /* FUN_0004e134 */
extern char *bin2hex(const void *data, size_t len);                     /* FUN_00029730 (log.c) */
extern int   fun_0004df84(const void *a, const void *b);                /* 命名待补: qsort 比较器
                                                                         * 位于 0x0004df84, 按元素哈希前缀比较) */

/* 反编译统一日志门控: 调用点形态 (A||B||level>N) ↔ applog(prio=N+1);
 * 三条件全关时调用点直接跳过字符串构造与 applog。 */
#define LOG_GATE(prio) \
	(DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || DAT_000766c4 >= (prio))

/* SHA-256 轮常量 K[0..63]。反编译体中 64 个常量以内联立即数出现
 * (0x428a2f98, 0x71374491, -0x4a3f0431=0xb5c0fbcf, ... , -0x398e870e=0xc67178f2),
 * 逐一核对与公开 SHA-256 K 表一致, 此处恢复为表。 */
static const uint32_t sha256_K[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

static uint32_t ror32(uint32_t x, unsigned n)
{
	return (x >> n) | (x << (32 - n));
}

/* 32 位字节交换 — 反编译体中反复出现的
 * (w<<24)|((w>>8)&0xff)<<16|((w>>16)&0xff)<<8|(w>>24) 拼装 */
static uint32_t bswap32(uint32_t w)
{
	return (w << 24) | ((w >> 8) & 0xff) << 16 |
	       ((w >> 16) & 0xff) << 8 | (w >> 24);
}

// === sha256_transform_scrypt === 原 FUN_0000fc90 @ 0x0000fc90
// 来源: reversed/cgminer/12_freq_pll/FUN_0000fc90.c | 分类: 12_freq_pll
// 语义: SHA-256 压缩函数 (cgminer scrypt.c 侧, 与 Equihash 侧 0004b4f0 区分)。
//   state: 8 字工作状态, 原地累加; block: 64 字节消息块 (按 uint32 字访问);
//   swap: 0 = block 已是内部字序, 直接拷入 W; 非 0 = 每字做 32 位字节交换
//   (反编译体为完整 bswap 拼装, 即输入为大端字节流按机器字读入后的还原)。
// 反编译体为 64 轮完全展开形态 (消息调度写 W[23..70] 槽位 + 轮内寄存器旋转,
// 尾部工作变量回落栈槽 -0x13c..-0x124 后 state[0..7] 逐槽累加), 本转写恢复为
// 规范循环结构; K 表、σ0/σ1/Σ0/Σ1/Ch/Maj 及轮序均与展开体逐项核对一致。
void sha256_transform_scrypt(uint32_t *state, const uint32_t *block, int swap)
{
	uint32_t W[64];
	uint32_t a, b, c, d, e, f, g, h;
	uint32_t t1, t2;
	int i;

	/* 消息块装入 W[0..15]: swap 时逐字 32 位字节交换 (大端输入还原) */
	if (swap) {
		for (i = 0; i < 16; i++)
			W[i] = bswap32(block[i]);
	} else {
		memcpy(W, block, 64);
	}

	/* 消息扩展: W[j] = W[j-16] + σ0(W[j-15]) + W[j-7] + σ1(W[j-2])
	 *   σ0(x) = ROTR7 ^ ROTR18 ^ SHR3;  σ1(x) = ROTR17 ^ ROTR19 ^ SHR10 */
	for (i = 16; i < 64; i++) {
		uint32_t s0 = ror32(W[i - 15], 7) ^ ror32(W[i - 15], 18) ^ (W[i - 15] >> 3);
		uint32_t s1 = ror32(W[i - 2], 17) ^ ror32(W[i - 2], 19) ^ (W[i - 2] >> 10);
		W[i] = W[i - 16] + s0 + W[i - 7] + s1;
	}

	/* 64 轮压缩。初值取自 state[0..7] (a..h)。 */
	a = state[0]; b = state[1]; c = state[2]; d = state[3];
	e = state[4]; f = state[5]; g = state[6]; h = state[7];

	for (i = 0; i < 64; i++) {
		/* Σ1(e) = ROTR6 ^ ROTR11 ^ ROTR25;  Ch(e,f,g) = (e&f) ^ (~e&g) */
		uint32_t S1 = ror32(e, 6) ^ ror32(e, 11) ^ ror32(e, 25);
		uint32_t ch = (e & f) ^ (~e & g);
		/* Σ0(a) = ROTR2 ^ ROTR13 ^ ROTR22;  Maj(a,b,c) — 反编译体形如
		 * (b|c)&a | b&c, 与 (a&b)^(a&c)^(b&c) 等价 */
		uint32_t S0 = ror32(a, 2) ^ ror32(a, 13) ^ ror32(a, 22);
		uint32_t maj = (a & b) ^ (a & c) ^ (b & c);

		t1 = h + S1 + ch + sha256_K[i] + W[i];
		t2 = S0 + maj;
		h = g; g = f; f = e; e = d + t1;
		d = c; c = b; b = a; a = t1 + t2;
	}

	/* 累加回 state[0..7] (反编译尾部循环: param_1[i] += 最终工作变量) */
	state[0] += a; state[1] += b; state[2] += c; state[3] += d;
	state[4] += e; state[5] += f; state[6] += g; state[7] += h;
}

// === bitmain_ZCASH_init === 原 FUN_00049cc8 @ 0x00049cc8
// 来源: reversed/cgminer/12_freq_pll/FUN_00049cc8.c | 分类: 12_freq_pll
// 语义: 驱动主初始化 (ARTIFACTS §4.3 权威名)。流程: 打印 "z15 init..." → 起
//   API http 线程 → 死等至少 1 个 url/userpass 齐全的矿池 → AXI/FPGA/波特率
//   上电序列 → 校验 param_1+0x492 处 32 字节 config_parameter 拷贝
//   (token_type==0x51 + CRC16 表校验, 覆盖前 30 字节, CRC 存于末 2 字节) →
//   链探测/PIC 复位/心跳 → (存在 /tmp/delete_freq 则清频并 exit(1) 重扫) →
//   目标频率落 DAT_0009d6f8[1..2] (缺省/800/0 → 800) → 断电复位序列 →
//   tty_init/set_baud/地址步进 → 逐链 ASIC 数 recheck (至多 3 次) →
//   工作模式/ticket mask/软件设址/核心超时/nonce shift/温度初始化 →
//   dev.timeout (位2) 与风扇模式 (fan_eft/固定 PWM) → 创建 4 个后台线程。
//   返回 0 成功; 负值错误码: -1 token, -2 CRC, -3 心跳, -8/-7/-5/-6 线程创建。
// 参数: param_1 = 调用方 (FUN_0000b928 bitmain_zcash_driver_init) 传入的
//   config_parameter 结构基址 (字节寻址; token 等 8 个 4 字节字段位于
//   +0x492..+0x4af, 16 字节对齐步进; +6 起 512 字节为逐链清零区)。
int bitmain_ZCASH_init(int param_1)
{
	const uint8_t *cfgsrc = (const uint8_t *)param_1;
	uint32_t cfg[8];              /* config_parameter 32 字节拷贝 (字视图) */
	uint8_t *cfgb = (uint8_t *)cfg;
	char     buf[0x1000];         /* 原 -0x1028..-0x28 栈缓冲: snprintf/applog 暂存,
	                              *   前 36 字节复用为 save_freq_to_every_chain_pic 参数区 */
	uint32_t crc;                 /* CRC16 累加器 */
	int      i;
	int      chain;               /* 链号 0..3 */
	int      retry;               /* 重试计数 (3→0) */
	uint8_t  asic_num;            /* 原 bVar1: 链上探测到的 ASIC 数 */
	uint8_t  exist;               /* 链存在标志 (DAT_0009d4bc[chain]) */
	uint32_t flags;               /* cfg.w[2] 别名视图 (位1=fan_eft 位2=dev.timeout 位3=用配置频率) */
	uint32_t fan_eft, fan_pwm;    /* 原 0x4a37c 段寄存值 */
	int      ret;
	void   **pool_p;              /* 矿池指针游标 */

	/* ---- 入口: 一次性载入 config_parameter 的 8 个 4 字节字段
	 *      (字节偏移 0x492/0x496/.../0x4ae, 16 字节步进) ---- */
	for (i = 0; i < 8; i++)
		cfg[i] = *(const uint32_t *)(cfgsrc + 0x492 + i * 4);

	if (LOG_GATE(5))
		applog(5, "z15 init...\n", 0);

	start_http_listen_thread();

	/* ---- 死等有效矿池: url (+0xc4) 与 userpass (+0xcc) 均非空才算有效 ---- */
	do {
		int valid_pools = 0;
		if (0 < DAT_0007b198) {
			pool_p = DAT_0007b438;
			while (pool_p != DAT_0007b438 + DAT_0007b198) {
				char *url      = *(char **)((uint8_t *)*pool_p + 0xc4);  /* pool->rpc_url */
				char *userpass = *(char **)((uint8_t *)*pool_p + 0xcc);  /* pool->rpc_userpass */
				if (url != NULL && *url != '\0' &&
				    userpass != NULL && *userpass != '\0')
					valid_pools++;
				pool_p++;
			}
			if (valid_pools != 0)
				break;
		}
		if (LOG_GATE(4))
			applog(4, "No valid pools, please configure them first!!", 0);
		sleep(5);
	} while (1);

	/* ---- 上电序列 ---- */
	bitmain_axi_open();
	init_fpga();
	set_baud_pl(0x1a);
	DAT_0009d6f3 = 1;                    /* 地址步进先置 1 (后由 calculate_address_interval 实算) */
	*(uint32_t *)&DAT_00078b94[0] = 0x2e302e39u;  /* "9.0." */
	*(uint32_t *)&DAT_00078b98[0] = 0x00352e30u;  /* "0.5\0" (串合成为 "9.0.0.5") */

	fan_power_gpio_on();
	set_fan_pwm(DAT_00076b78);
	check_fan_number();

	/* ---- token_type 校验 (必须 0x51, 否则 -1) ---- */
	if (cfgb[0] != 0x51) {
		if (!LOG_GATE(3))
			return -1;
		snprintf(buf, 0x1000, "%s: config_parameter.token_type != 0x%x, it is 0x%x",
		         "bitmain_ZCASH_init", 0x51, cfgb[0]);
		applog(3, buf, 0);
		return -1;
	}

	/* ---- CRC16 校验: 表驱动, crc = tbl[byte ^ crc>>8] ^ crc<<8;
	 *      覆盖前 30 字节 (首字节即 token 0x51), 初值 0xff, 存值取 w[7] 高 16 位 ---- */
	crc = 0xff;
	crc = (DAT_00068aec[0x51 ^ (crc >> 8)] ^ (crc << 8)) & 0xffff;   /* 字节 0 = 0x51 */
	for (i = 1; i < 30; i++)
		crc = (DAT_00068aec[cfgb[i] ^ (crc >> 8)] ^ (crc << 8)) & 0xffff;
	if ((uint16_t)(cfg[7] >> 16) != crc) {
		if (!LOG_GATE(3))
			return -2;
		snprintf(buf, 0x1000, "%s: config_parameter.crc = 0x%x, but we calculate it as 0x%x",
		         "bitmain_ZCASH_init", (uint32_t)(uint16_t)(cfg[7] >> 16), crc);
		applog(3, buf, 0);
		return -2;
	}

	/* ---- 逐链区清零: 对 param_1+0x70 起步进 4 的 128 个槽位清 +6/+8 两字节,
	 *      合计清 param_1[6..517] 共 512 字节 ---- */
	for (i = 0; i < 128; i++) {
		uint8_t *slot = (uint8_t *)param_1 + 0x70 + i * 4;
		*(uint16_t *)(slot + 6) = 0;
		*(uint16_t *)(slot + 8) = 0;
	}

	check_chain();

	/* 温度联动模式: 快照在位链数, 强制 fan_pwm=50%, 置 fan_eft 位 */
	if (DAT_00078e28 != '\0') {
		DAT_00078e38 = (uint32_t)DAT_0009d6cb;
		cfgb[14] = 0x32;                 /* w[3] 字节2 (fan_pwm) = 50 */
		cfg[2] |= 2;                     /* w[2] 位1 (fan_eft) = 1 */
	}

	every_chain_reset_PIC16F1704_pic_new();
	every_chain_jump_from_loader_to_app_PIC16F1704_new();
	sleep(1);
	if (send_heart_beat_to_every_chain() == -3)
		return -3;

	/* ---- /tmp/delete_freq 存在 → 清零频率缓冲落 PIC、删标志文件并整体退出重扫 ---- */
	if (access("/tmp/delete_freq", 0) == 0) {
		if (LOG_GATE(5))
			applog(5, "delete freq data and scan freq again\n", 0);
		memset(buf, 0, 36);              /* 原 12+4+20 字节栈清零区 */
		save_freq_to_every_chain_pic(buf);
		system("rm /tmp/delete_freq");
		sleep(3);
		exit(1);
	}

	/* ---- 目标频率落全局: 位3 未置位或配置 800/0 → 800, 否则用配置低 16 位 ---- */
	if (((cfg[2] & 8) == 0) ||
	    ((cfg[4] & 0xffff) == 800 || (cfg[4] & 0xffff) == 0)) {
		DAT_0009d6f8 = (DAT_0009d6f8 & 0xff0000ffu) | 0x32000u;   /* [1..2] = 800 */
	} else {
		DAT_0009d6f8 = (DAT_0009d6f8 & 0xff000000u) |
		               (((cfg[4] & 0xffff) << 8) & 0x00ffff00u);  /* [0] 保留, [1..2] = 配置频率 */
	}

	reset_all_hash_board_low();
	cgsleep_ms(100);
	every_chain_disable_PIC16F1704_dc_dc_new();
	cgsleep_ms(200);
	cgsleep_ms(200);
	every_chain_enable_PIC16F1704_dc_dc_new();
	cgsleep_ms(100);
	if (LOG_GATE(5)) {
		snprintf(buf, 0x1000, "%s %d", "reset_all_hash_board_high", 0x1128);
		applog(5, buf, 0);
	}
	chain = 0;
	*(uint32_t *)(DAT_00078e10 + AXI_REG_CHAIN_RESET) = 0;   /* 链复位释放 (base+0x34) */
	cgsleep_ms(100);

	tty_init(param_1);
	sleep(3);
	set_baud(cfg[1]);                    /* w[1]: 配置波特率字段 */
	cgsleep_ms(100);
	calculate_address_interval();
	DAT_0009d6f2 = 1;
	clear_reg_response_queue();
	cgsleep_ms(100);
	sleep(2);
	check_every_chain_asic_number(1);
	cgsleep_ms(300);

	/* ---- 逐链 recheck: 在位链的 ASIC 数 != 3 时调 recheck_asic_num, 至多 3 次 ---- */
	for (chain = 0; chain != 4; chain++) {
		exist = DAT_0009d4bb[1 + chain];             /* DAT_0009d4bc[chain] */
		if (exist == 1) {
			DAT_00079bf4[chain] = 1;
			for (retry = 3; retry != 0; retry--) {
				asic_num = DAT_0009d4bb[1 + chain + 0x209];  /* = 0x9d6c5[chain] 每链 ASIC 数 */
				if (asic_num == 3) {
					DAT_00079bf4[chain] = 0;
					break;
				}
				DAT_00079bf4[chain] = 1;
				if (LOG_GATE(5)) {
					snprintf(buf, 0x1000,
					         "recheck: chainid %d exist %d asicnum %d realnum %d",
					         chain, (uint32_t)exist, (uint32_t)asic_num, 3);
					applog(5, buf, 0);
					/* 原文此处含 DAT_00079bf4[chain]==0 的退出检查, 因该标志
					 * 上一语句恒置 1, 检查恒假, 属反编译残留, 略去 */
				}
				recheck_asic_num(param_1, (uint32_t)(chain & 0xff));
			}
		}
	}

	check_freq_and_set_work_mode();
	every_chain_set_ticket_mask(DAT_00076b60);
	cgsleep_ms(100);
	every_chain_software_set_address();
	cgsleep_ms(100);
	every_chain_set_core_timeout(DAT_00076b74);
	cgsleep_ms(100);
	every_chain_set_nonce_shift(0);
	cgsleep_ms(100);
	every_chain_enable_read_temperature_from_asic();
	every_chain_init_temperature_sensor();
	every_chain_set_analog_mux_control(0);

	/* ---- dev.timeout: 位2 置位 → 100ms (100000000 µs) ---- */
	if ((cfg[2] & 4) != 0) {
		DAT_0009d4c4 = 100000000;
		if (LOG_GATE(5)) {
			snprintf(buf, 0x1000, "dev.timeout = %d us", 100000000);
			applog(5, buf, 0);
		}
	}

	/* 原 0x4a37c: fan_eft / fan_pwm 落全局
	 *   DAT_0009d6f7 = fan_eft; DAT_0009d6f8 字节0 = fan_pwm */
	fan_eft  = (cfg[2] & 3) >> 1;
	fan_pwm  = cfgb[14];
	DAT_0009d6f7 = (uint8_t)fan_eft;
	DAT_0009d6f8 = (DAT_0009d6f8 & 0xffffff00u) | (fan_pwm & 0xffu);

	/* 原 0x4a1fc 门控: 仅 B||level>=5 (不含门控 A) */
	if (DAT_00078e08 != '\0' || DAT_000766c4 >= 5) {
		snprintf(buf, 0x1000, "%s: fan_eft : %d  fan_pwm : %d",
		         "bitmain_ZCASH_init", (int)fan_eft, (int)fan_pwm);
		applog(5, buf, 0);
	}

	/* 风扇模式: 非固定模式或 PWM>100 → 按温度自动; 否则固定 PWM */
	if (((cfg[2] & 2) == 0) || (100 < fan_pwm))
		set_PWM_according_to_temperature();
	else
		set_fan_pwm((int)fan_pwm);

	/* ---- 后台线程创建 (任一失败即返回对应负码) ---- */
	if (create_bitmain_check_fan_pthread() == -8)
		return -8;
	sleep(1);
	if (create_bitmain_read_temp_pthread() == -7)
		return -7;
	if (create_bitmain_check_miner_status_pthread() == -5)
		return -5;
	if (create_bitmain_get_hash_rate_pthread() == -6)
		return -6;

	check_temperature_offset_value();
	DAT_000790ec[0] = 1;
	DAT_000790ec[1] = 1;
	DAT_000790ec[2] = 1;
	DAT_000790ec[3] = 1;

	if (LOG_GATE(5)) {
		snprintf(buf, 0x1000, "%s done", "bitmain_ZCASH_init");
		applog(5, buf, 0);
	}
	return 0;
}

// === equihash_verify_solution_full === 原 FUN_0004e198 @ 0x0004e198
// 来源: reversed/cgminer/12_freq_pll/FUN_0004e198.c | 分类: 12_freq_pll
// 语义: Equihash (N,K) 解完整校验 (本机 N=200, K=9)。流程:
//   1) expandArray 把 1344 字节位打包解 (每索引 (N/(K+1))+1 = 21 bit) 解包为
//      512 个 4 字节索引; 2) O(n²) 查重, 有重复 → 1;
//   3) 逐索引叶哈希 (一次 blake 覆盖 512/N=2 个索引, 输出 N*2=400 bit, 偶索引取
//      前 N bit / 奇索引取后 N bit), 30 字节前缀全解 XOR 折叠, 非 0 → 2;
//   4) 9 轮 Wagner 折叠 (每轮按前缀 qsort 后相邻对 XOR, 索引段拼接翻倍);
//   5) 末级元素取回 512 索引, check_index_tree_order 逐层校验子树有序;
//   6) 重建叶哈希元素再走 9 轮折叠 (第二轮无 qsort, 靠 5) 的有序保证);
//   7) 末级 30 字节前缀拷入 acc, 非 0 → 3, 全 0 → 0 (有效)。
//   期间把重建解重打包回 21 bit 格式并 bin2hex 调试 (串随即 free, 原样保留)。
// 参数: eh = {eh[0]=N, eh[1]=K, eh[2]=blake 基态/摘要指针}; sol = 位打包解。
// 返回 (uint8): 0=有效, 1=索引重复, 2=初始全解 XOR 非 0, 3=末级 XOR 非 0。
// 注: 原 FUN_0005c710/FUN_0005c7ec 为编译器运行时除法 (__aeabi_idivmod /
//     __aeabi_idiv_checked), 分别以 C 的 / 与 % 求商/余还原。
uint8_t equihash_verify_solution_full(int *eh, uint8_t *sol)
{
	int      n        = eh[0];              /* N (200) */
	int      k        = eh[1];              /* K (9) */
	const void *digest = (const void *)eh[2]; /* blake 基态 */
	int      lvl      = k + 1;              /* K+1 = 10 */
	int      len      = n / lvl;            /* 每级前缀位数 = 20 */
	int      sol_bits = (len + 1) << k;     /* 全解总位数 = 21<<9 = 10752 */
	uint32_t idx[512];                     /* 原 -0x1028 区: 解包索引 */
	uint32_t words[512];                   /* 原 -0x828 区: 叶哈希输出 / BE 索引回读 */
	uint8_t  bits[1344];                   /* 原 -0x1568 区: 位提取暂存 / 解重打包输出 */
	uint8_t  acc[30];                      /* 原 -0x15c8..-0x15ac 区: 30 字节 XOR 累加器 */
	uint8_t  tmp[31];                      /* 原 -0x15a8 区: 轮内相邻对 XOR 暂存 (30 字节有效) */
	uint8_t  ext[31];                      /* 原 -0x1588 区: expandArray 输出暂存 */
	uint8_t *bufA;                         /* 原 -0x15f4 槽: malloc(0x4400) 元素缓冲 A */
	uint8_t *bufB;                         /* 原 -0x15ec 槽: malloc(0x4400) 元素缓冲 B */
	uint8_t *cur;                          /* 元素写入游标 */
	uint32_t *rp;                          /* 索引回读游标 */
	size_t   prefix;                       /* 原 __n: (len+7)/8*(K+1) = 30 字节 */
	int      pair_div;                     /* 512/N = 2 (一次 blake 覆盖的索引数) */
	int      hash_bytes;                   /* N*pair_div>>3 = 50 字节 */
	uint32_t elem_cnt;                     /* 原 -0x1608/-0x1610 槽 */
	uint32_t idx_len;                      /* 原 -0x15d8/-0x1604 槽: 索引段字节数 */
	int      rounds;                       /* 原 -0x15dc/-0x15f8 槽 */
	uint32_t be, half, bitpos;             /* BE 索引 / idivmod 商 / 余 */
	uint32_t bitacc;                       /* 21 bit 累加器 (重打包) */
	uint32_t nbits;                        /* 累加器位计数 (重打包) */
	int      i, j, lvl2, off, idx_off;
	int      out_cnt;                      /* 原 sVar15 */
	uint8_t *swp;
	uint8_t  ret;

	DAT_00076b90 = 3;
	expandArray(sol, sol_bits >> 3, idx, 0x800, len + 1, 1);

	memset(acc, 0, sizeof(acc));
	bufA = malloc(0x4400);                 /* 512 × (30+4) 字节元素 */
	bufB = malloc(0x4400);
	memset(tmp, 0, 30);

	/* ---- 索引两两不同 (原 O(n²) 嵌套扫描, 相邻对先判) ---- */
	for (i = 0; i < 512; i++) {
		for (j = i + 1; j < 512; j++) {
			if (idx[i] == idx[j]) {
				ret = 1;
				goto cleanup;              /* 原 0x4e488 → 0x4e430 */
			}
		}
	}

	pair_div   = 512 / n;                  /* 原 FUN_0005c710(0x200, n) 商 = 2 */
	hash_bytes = (n * pair_div) >> 3;      /* 原 n*2 → 400>>3 = 50 */
	prefix     = ((len + 7) >> 3) * lvl;   /* 原 __n = 3*10 = 30 */

	/* ---- 叶哈希 + 前缀全解 XOR 折叠 ---- */
	cur = bufA;
	for (i = 0; i < 512; i++) {
		be     = bswap32(idx[i]);                 /* 索引转 BE 字节序整数 */
		half   = be / pair_div;                   /* idivmod 商: 哈希输入 = BE>>1 (对半合并) */
		bitpos = be % pair_div;                   /* idivmod 余: 选前/后半 N bit */
		equihash_leaf_hash(digest, half, words, hash_bytes);
		/* 原 FUN_0005c7ec(be, pair_div) (__aeabi_idiv_checked): 余数重复求值, 结果同 bitpos */
		expandArray((uint8_t *)words + ((n * bitpos) >> 3), n >> 3,
		            bits, prefix, len, 0);        /* 取该叶 200 bit 前缀 → 30 字节 */
		for (j = 0; j < prefix; j++)
			acc[j] ^= bits[j];                    /* 原以 bVar11 寄存传递的逐字节 XOR 折叠 */
		memcpy(cur, bits, prefix);
		*(uint32_t *)(cur + prefix) = idx[i];     /* 元素 = 30B 前缀 + 原生序索引 */
		cur += prefix + 4;
	}

	/* ---- 初始全解 XOR 预检: acc 全 0 才继续 (原逐字节寄存扫描) ---- */
	for (j = 0; j < (int)prefix; j++) {
		if (acc[j] != 0) {
			ret = 2;
			goto cleanup;                          /* 原 code_r0x0004e42c */
		}
	}

	/* ---- 第 1 轮折叠: 9 轮 qsort + 相邻对 XOR ---- */
	memset(acc, 0, 30);                            /* 原文进入轮循环前重复清零 */
	idx_len  = 4;
	rounds   = 9;
	elem_cnt = 512;
	while (1) {
		size_t elem_sz = idx_len + prefix;
		out_cnt = 0;
		qsort(bufA, elem_cnt, elem_sz, fun_0004df84);
		if (elem_cnt == 0) {
			out_cnt = elem_cnt;
		} else {
			uint32_t a = 0;                        /* 偶元素游标 */
			size_t odd_off = elem_sz;              /* 原 sVar13: 奇元素偏移 */
			while (a < elem_cnt) {
				for (j = 0; j < (int)prefix; j++)
					tmp[j] = bufA[odd_off + j] ^ bufA[a * elem_sz + j];
				{
					size_t dst = (idx_len * 2 + prefix) * (a >> 1);
					out_cnt++;
					memcpy(bufB + dst, tmp, prefix);                                  /* 新前缀 */
					memcpy(bufB + dst + prefix,
					       bufA + a * elem_sz + prefix, idx_len);                       /* 偶索引段 */
					memcpy(bufB + dst + prefix + idx_len,
					       bufA + odd_off + prefix, idx_len);                           /* 奇索引段 */
				}
				odd_off += elem_sz * 2;
				a += 2;
			}
		}
		idx_len = idx_len * 2;
		rounds--;
		DAT_00076b90 += 3;
		if (rounds == 0)
			break;
		swp = bufA; bufA = bufB; bufB = swp;
		elem_cnt = out_cnt;
	}

	/* ---- 末级元素 (bufB) 取回 512 索引并转 BE 序 (原逐 4 字节 bswap) ---- */
	for (i = 0; i < 512; i++)
		words[i] = bswap32(*(uint32_t *)(bufB + prefix + i * 4));

	/* ---- 逐层子树有序校验: 层 lvl2 内步进 2^(lvl2+1), 每块 2^lvl2 索引 ---- */
	for (lvl2 = 0; lvl2 != 9; lvl2++) {
		for (off = 0; off < 0x200; off += (2 << lvl2))
			check_index_tree_order(words + off, 1 << lvl2);
	}

	/* ---- 重建叶元素 (写入 bufB, 覆盖末级数据; 索引已拷出) ---- */
	cur = bufB;
	rp  = words;
	do {
		be     = *rp;                              /* BE 序索引 */
		half   = be / pair_div;
		bitpos = be % pair_div;
		equihash_leaf_hash(digest, half, bits, hash_bytes);
		/* 原 FUN_0005c7ec(be, pair_div): 同上, 余数重复求值 */
		expandArray(bits + ((n * bitpos) >> 3), n >> 3, ext, prefix, len, 0);
		memcpy(cur, ext, prefix);
		*(uint32_t *)(cur + prefix) = be;          /* 原 -0x15cc 槽: 当前 BE 索引 */
		free(bin2hex(cur, 0x22));                  /* 原: 34B 元素 hex 串生成后立即 free */
		cur += prefix + 4;
		rp++;
	} while (rp != words + 512);

	/* ---- 第 2 轮折叠: 9 轮无 qsort (依赖前面的有序校验) ---- */
	idx_len  = 4;
	rounds   = 9;
	elem_cnt = 512;
	while (1) {
		size_t elem_sz = idx_len + prefix;
		out_cnt = 0;
		if (elem_cnt == 0) {
			out_cnt = elem_cnt;
		} else {
			uint32_t a = 0;
			size_t odd_off = elem_sz;              /* 奇元素偏移游标 */
			while (a < elem_cnt) {
				for (j = 0; j < (int)prefix; j++)
					tmp[j] = bufB[odd_off + j] ^ bufB[a * elem_sz + j];
				{
					size_t dst = (idx_len * 2 + prefix) * (a >> 1);
					out_cnt++;
					memcpy(bufA + dst, tmp, prefix);
					memcpy(bufA + dst + prefix,
					       bufB + a * elem_sz + prefix, idx_len);
					memcpy(bufA + dst + prefix + idx_len,
					       bufB + odd_off + prefix, idx_len);
				}
				odd_off += elem_sz * 2;
				a += 2;
			}
		}
		idx_len = idx_len * 2;
		rounds--;
		DAT_00076b90 += 3;
		if (rounds == 0)
			break;
		swp = bufA; bufA = bufB; bufB = swp;
		elem_cnt = out_cnt;
	}

	/* ---- 末级元素 (bufA) 重打包为 21 bit 位格式 (与输入解同构, 1344 字节) ---- */
	bitacc  = 0;
	nbits   = 0;
	idx_off = 0;
	for (i = 0; i < 1344; i++) {
		size_t ipos = idx_off + prefix;            /* 索引段偏移 = 游标 + 前缀长 */
		if (nbits < 8) {
			idx_off += 4;
			nbits += 0x15;
			bitacc = (bufA[ipos + 1] & 0x1f) << 0x10 |
			         (uint32_t)bufA[ipos + 2] << 8 |
			         bitacc << 0x15 |
			         (uint32_t)bufA[ipos + 3];     /* 索引低 21 bit (BE 字节 1..3) */
		}
		nbits -= 8;
		bits[i] = (uint8_t)(bitacc >> (nbits & 0xff));
	}
	free(bin2hex(bits, 0x540));                    /* 原: 重打包解 hex 串生成后立即 free */
	free(bin2hex(bufA, idx_len + prefix));         /* 原: 末级元素 (2048+30B) hex 后 free */

	/* ---- 终检: 末级 30 字节前缀全 0 → 有效 ---- */
	memcpy(acc, bufA, 30);                         /* 原 7×u32 + u16 逐字拷入 */
	ret = 3;
	for (j = 0; j < (int)prefix; j++) {
		if (acc[j] != 0)
			break;
		if (j == (int)prefix - 1)
			ret = 0;                               /* 原指针走到 acc[29] 仍为 0 */
	}

cleanup:
	free(bufA);
	free(bufB);
	return ret;
}

/* ---- scrypt 常量表 (自 ELF .rodata 0x5d5d0/0x5d600/0x5d64c 实测还原) ---- */

/* 原 0x0005d5d0 表 (12 字, 字节流形式经 swap=1 装入): SHA-256 填充块
 * 字节 = 0x80 || 40×0x00 || 00 00 02 80 — 对应 80 字节消息 (640 bit) */
static const uint32_t scrypt_hdr_pad_tbl[12] = {
	0x00000080u, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x80020000u
};

/* 原 DAT_0005d600..DAT_0005d628 (11 字, 读入时逐字 bswap 为内部序):
 * PBKDF2 内层消息 (ipad 64B + 头 80B + INT(i) 4B = 148B = 0x4A0 bit) 填充尾:
 * 字节 = 0x80 || 39×0x00 || 00 00 04 A0 */
static const uint32_t pbkdf2_msg_tail[11] = {
	0x80000000u, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x000004a0u
};

/* 原 DAT_0005d64c (16 字, 内部序 swap=0 直接使用): 终轮 PBKDF2 内层
 * (ipad 64B + B 128B + INT(1) 4B = 196B = 0x620 bit) 的 INT(1)+填充块 */
static const uint32_t pbkdf2_final_pad[16] = {
	0x00000001u, 0x80000000u, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0x00000620u
};

/* SHA-256 初值 (反编译体内联立即数) */
static const uint32_t sha256_iv[8] = {
	0x6a09e667u, 0xbb67ae85u, 0x3c6ef372u, 0xa54ff53au,
	0x510e527fu, 0x9b05688cu, 0x1f83d9abu, 0x5be0cd19u
};

#define ROTL32(v, n) (((v) << (n)) | ((v) >> (32 - (n))))

/* Salsa20 四分轮 (反编译 8 个核心的公共模式, 寄存器级核对):
 * b ^= rotl(a+d,7); c ^= rotl(b+a,9); d ^= rotl(c+b,13); a ^= rotl(d+c,18) */
#define SALSA_QR(a, b, c, d) do { \
	(b) ^= ROTL32((a) + (d), 7); \
	(c) ^= ROTL32((b) + (a), 9); \
	(d) ^= ROTL32((c) + (b), 13); \
	(a) ^= ROTL32((d) + (c), 18); \
} while (0)

/* Salsa20/8 核心: 4 个双重轮 (列轮 4 QR + 行轮 4 QR) 后 B[i] += x[i]。
 * 反编译体每核心 = 32 次 rotl(7/9/13/18), x 取块的转置视图且行/列轮次序
 * 互换 (转置下行↔列), 数学等价于本规范形态; 输入 16 字块原地混合。 */
#define SALSA20_8(blk) do { \
	uint32_t _x[16]; \
	unsigned _k; \
	memcpy(_x, (blk), 64); \
	for (_k = 0; _k < 4; _k++) { \
		SALSA_QR(_x[0], _x[4], _x[8], _x[12]); \
		SALSA_QR(_x[5], _x[9], _x[13], _x[1]); \
		SALSA_QR(_x[10], _x[14], _x[2], _x[6]); \
		SALSA_QR(_x[15], _x[3], _x[7], _x[11]); \
		SALSA_QR(_x[0], _x[1], _x[2], _x[3]); \
		SALSA_QR(_x[5], _x[6], _x[7], _x[4]); \
		SALSA_QR(_x[10], _x[11], _x[8], _x[9]); \
		SALSA_QR(_x[15], _x[12], _x[13], _x[14]); \
	} \
	for (_k = 0; _k < 16; _k++) (blk)[_k] += _x[_k]; \
} while (0)

// === scrypt_N_1_256 === 原 FUN_000122e8 @ 0x000122e8
// 来源: reversed/cgminer/12_freq_pll/FUN_000122e8.c | 分类: 12_freq_pll
// 语义: 标准 scrypt (N=1024, r=1, p=1, dkLen=32 字节), 80 字节工作头:
//   1) K = SHA256(工作头) — 反编译体在 PBKDF2 前与终轮 PBKDF2 前各算一次 (原样);
//   2) Y = PBKDF2-HMAC-SHA256(K, 工作头, 128B): U_i = HMAC(K, 头||INT_32_BE(i)),
//      i = 1..4, 输出按 BE 字节字存入 B[32];
//   3) ROMix(N=1024, V = scratchpad): 第 1 遍 V[i] = X_i, X_{i+1} = BlockMix(X_i);
//      第 2 遍 j = X[16]&1023, X = BlockMix(X ^ V[j]) (循环 512 次 × 每次两步);
//   4) out = PBKDF2-HMAC-SHA256(K, B||INT_32_BE(1), 32B)。
//   BlockMix(r=1): T = B0^B1 → Salsa20/8 → Z0; T = Z0^B1 → Salsa20/8 → Z1;
//   输出 (Z0, Z1)。反编译体以 64 字节对齐指针 (param_2+63 & ~63) 为 V 区基址,
//   占用 1024×128B = 0x20000 字节。
//   核对注记: 反编译 8 个 Salsa20/8 核心 (每核心 4 迭代 × 32 次 rotl) 的四分轮
//   元组经数据流配对后恰为规范列/行元组集 (寄存器为块转置视图, 行/列轮次序在
//   转置下互换), 与规范实现数学等价; 外层/内层 SHA256 长度字段 0x280/0x4A0/
//   0x620/0x300 与 ELF 常量表逐一核对。
void scrypt_N_1_256(const uint32_t *in, const void *scratch_in, uint32_t *out)
{
	uint32_t *V = (uint32_t *)(((uintptr_t)scratch_in + 0x3f) &
	                           ~(uintptr_t)0x3f);          /* V 区 (1024×128B) */
	uint32_t state[8];       /* SHA-256 变换状态 (原 -0x228 槽) */
	uint32_t X[8];           /* HMAC 密钥 = SHA256(工作头 80B) (原 -0x208 槽) */
	uint32_t ipad_mid[8];    /* ipad midstate (原 -0x1a8+0x10 槽) */
	uint32_t opad_mid[8];    /* opad midstate (原 -0x108 槽) */
	uint32_t outer[8];       /* 外层变换状态 (原 -0x1c8 槽) */
	uint32_t blk[16];        /* 64 字节内层消息块 (原 -0x1a8 槽) */
	uint32_t oblk[16];       /* 外层块: 内层摘要||填充 (原 -0x108+8 槽) */
	uint32_t B[32];          /* 128B 工作块: PBKDF2 输出 / ROMix X (原 -0xa8 区) */
	uint32_t i, k;
	uint32_t j;              /* ROMix 第 2 遍随机索引 (原 -0x68 槽 & 0x3ff) */

	/* ---- 1: K = SHA256(工作头 80B) ---- */
	for (i = 0; i < 8; i++)
		state[i] = sha256_iv[i];
	sha256_transform_scrypt(state, in, 1);                /* 头[0..63] */
	for (i = 0; i < 4; i++)
		blk[i] = in[16 + i];                              /* 头[64..79] 本机字 */
	for (i = 4; i < 16; i++)
		blk[i] = scrypt_hdr_pad_tbl[i - 4];               /* 填充 + 长度 0x280 */
	sha256_transform_scrypt(state, blk, 1);
	memcpy(X, state, 32);

	/* ---- 2: HMAC-SHA256 midstate (密钥 = K, 64 字节块填充) ---- */
	/* ipad: (K^0x36) || 0x36×8 → IV 态变换, 再续头[0..63] */
	for (i = 0; i < 8; i++)
		blk[i] = X[i] ^ 0x36363636u;
	for (i = 8; i < 16; i++)
		blk[i] = 0x36363636u;
	for (i = 0; i < 8; i++)
		ipad_mid[i] = sha256_iv[i];
	sha256_transform_scrypt(ipad_mid, blk, 0);
	sha256_transform_scrypt(ipad_mid, in, 1);
	/* opad: (K^0x5c) || 0x5c×8 → IV 态变换 */
	for (i = 0; i < 8; i++)
		blk[i] = X[i] ^ 0x5c5c5c5cu;
	for (i = 8; i < 16; i++)
		blk[i] = 0x5c5c5c5cu;
	for (i = 0; i < 8; i++)
		opad_mid[i] = sha256_iv[i];
	sha256_transform_scrypt(opad_mid, blk, 0);

	/* 外层消息块尾 (inner 摘要 32B + 填充, 96B = 0x300 bit) — 循环外一次装配
	 * (原 -0x108+0x10 槽独立缓冲, 与内层块分开装配) */
	for (i = 8; i < 15; i++)
		oblk[i] = 0;
	oblk[8] = 0x80000000u;
	oblk[15] = 0x300;

	/* ---- PBKDF2 128B: U_i = HMAC(K, 头 80B || INT_32_BE(i)), i = 1..4 ---- */
	for (i = 1; i <= 4; i++) {
		for (k = 0; k < 8; k++)
			state[k] = ipad_mid[k];
		for (k = 0; k < 4; k++)
			blk[k] = bswap32(in[16 + k]);                 /* 头[64..79] BE 字 */
		blk[4] = i;                                       /* INT_32_BE(i) */
		for (k = 5; k < 16; k++)
			blk[k] = pbkdf2_msg_tail[k - 5];              /* 0x80…0x4A0 填充尾 */
		sha256_transform_scrypt(state, blk, 0);           /* 内层摘要 (148B) */
		for (k = 0; k < 8; k++)
			outer[k] = opad_mid[k];
		for (k = 0; k < 8; k++)
			oblk[k] = state[k];                           /* 块 = 内层摘要 + 尾 */
		sha256_transform_scrypt(outer, oblk, 0);          /* 外层摘要 */
		for (k = 0; k < 8; k++)
			B[(i - 1) * 8 + k] = bswap32(outer[k]);       /* BE 字节字 → B */
	}

	/* ---- 3: ROMix 第 1 遍 — 顺序填充 V[0..1023] (512 循环 × 2 步) ----
	 *   V[i] = X_i; X_{i+1} = BlockMix(X_i); V[i+1] = X_{i+1}; X_{i+2} = BlockMix(X_{i+1})
	 *   BlockMix: T = B0^B1 → SALSA20_8 → Z0; T = Z0^B1 → SALSA20_8 → Z1;
	 *   块 = (Z0, Z1)。原体每循环 4 个内联 Salsa 核心 + 2 次 128B memcpy
	 *   (memcpy 分别位于两次 BlockMix 之后)。 */
	for (i = 0; i < 1024; i += 2) {
		memcpy((uint8_t *)V + i * 128, B, 128);           /* V[i] = X_i */
		for (k = 0; k < 16; k++)
			blk[k] = B[k] ^ B[k + 16];                    /* T = B0 ^ B1 */
		SALSA20_8(blk);
		memcpy(B, blk, 64);                               /* Z0 → B[0..15] */
		for (k = 0; k < 16; k++)
			blk[k] = B[k] ^ B[k + 16];                    /* T = Z0 ^ B1 (B1 未变) */
		SALSA20_8(blk);
		memcpy(B + 16, blk, 64);                          /* Z1 → B[16..31] */
		memcpy((uint8_t *)V + (i + 1) * 128, B, 128);     /* V[i+1] = X_{i+1} */
		for (k = 0; k < 16; k++)
			blk[k] = B[k] ^ B[k + 16];                    /* T = B0 ^ B1 (新) */
		SALSA20_8(blk);
		memcpy(B, blk, 64);                               /* Z0' → B[0..15] */
		for (k = 0; k < 16; k++)
			blk[k] = B[k] ^ B[k + 16];
		SALSA20_8(blk);
		memcpy(B + 16, blk, 64);                          /* Z1' → B[16..31] (X_{i+2}) */
	}

	/* ---- ROMix 第 2 遍 — 随机回读 (512 循环 × 2 步) ----
	 *   j = X[16] & 1023 (原 -0x68 槽, 即 B[16]); X = BlockMix(X ^ V[j]) */
	for (i = 0; i < 1024; i += 2) {
		j = B[16] & 0x3ff;
		for (k = 0; k < 32; k++)
			B[k] ^= V[j * 32 + k];                        /* X ^= V[j] */
		for (k = 0; k < 16; k++)
			blk[k] = B[k] ^ B[k + 16];
		SALSA20_8(blk);
		memcpy(B, blk, 64);
		for (k = 0; k < 16; k++)
			blk[k] = B[k] ^ B[k + 16];
		SALSA20_8(blk);
		memcpy(B + 16, blk, 64);

		j = B[16] & 0x3ff;
		for (k = 0; k < 32; k++)
			B[k] ^= V[j * 32 + k];
		for (k = 0; k < 16; k++)
			blk[k] = B[k] ^ B[k + 16];
		SALSA20_8(blk);
		memcpy(B, blk, 64);
		for (k = 0; k < 16; k++)
			blk[k] = B[k] ^ B[k + 16];
		SALSA20_8(blk);
		memcpy(B + 16, blk, 64);
	}

	/* ---- 4: 终轮 PBKDF2 32B: out = HMAC(K, B||INT_32_BE(1)) ----
	 *   K 重算 (反编译体此处重新执行两次变换, 原样保留) */
	for (i = 0; i < 8; i++)
		state[i] = sha256_iv[i];
	sha256_transform_scrypt(state, in, 1);
	for (i = 0; i < 4; i++)
		blk[i] = in[16 + i];
	for (i = 4; i < 16; i++)
		blk[i] = scrypt_hdr_pad_tbl[i - 4];
	sha256_transform_scrypt(state, blk, 1);               /* state = K */

	memcpy(ipad_mid, state, 32);                          /* K 暂存 (原 0x10..0x17 槽) */
	for (i = 0; i < 8; i++)
		out[i] = sha256_iv[i];                            /* out 先置 IV (原 param_3) */
	/* opad midstate → out */
	for (i = 0; i < 8; i++)
		blk[i] = state[i] ^ 0x5c5c5c5cu;
	for (i = 8; i < 16; i++)
		blk[i] = 0x5c5c5c5cu;
	sha256_transform_scrypt(out, blk, 0);
	/* 内层: ipad midstate → 续 B[0..63B], B[64..127B], INT(1)+填充 (196B) */
	for (i = 0; i < 8; i++)
		state[i] = sha256_iv[i];
	for (i = 0; i < 8; i++)
		blk[i] = ipad_mid[i] ^ 0x36363636u;
	for (i = 8; i < 16; i++)
		blk[i] = 0x36363636u;
	sha256_transform_scrypt(state, blk, 0);
	sha256_transform_scrypt(state, B, 1);                 /* B[0..15] BE 装入 */
	sha256_transform_scrypt(state, B + 16, 1);            /* B[16..31] */
	sha256_transform_scrypt(state, pbkdf2_final_pad, 0);  /* INT(1)+填充+0x620 */
	/* 外层: out (opad midstate) 续 内层摘要(32B)+填充 (96B = 0x300 bit) */
	for (i = 0; i < 8; i++)
		blk[i] = state[i];
	blk[8] = 0x80000000u;
	for (i = 9; i < 15; i++)
		blk[i] = 0;
	blk[15] = 0x300;
	sha256_transform_scrypt(out, blk, 0);                 /* out = 派生密钥 */
}

// ============================================================================
// 覆盖率表 (batch=freq_b2, 4/4 函数, 源合计 2397 行)
//
// sha256_transform_scrypt  | 原 FUN_0000fc90 |  489 行 | 覆盖 489/489
//   语义: SHA-256 压缩函数 (scrypt 侧; swap 控制块按大端装入或直取)。
//   反编译体为 64 轮全展开形态, 转写恢复为标准轮循环 + K 常量表。
//   64 个 K 立即数逐一与反编译体比对一致; W 消息调度与 Σ/σ 选择函数按展开
//   语句核对首末轮。gcc 编译后对 64B 输入两次变换输出与 hashlib.sha256
//   一致。
//
// bitmain_ZCASH_init       | 原 FUN_00049cc8 |  414 行 | 覆盖 414/414
//   语义: 驱动主初始化 (矿池等待→AXI/串口上电→token/CRC16 校验→链探测/PIC→
//   频率落全局→recheck→设址/超时/温度→风扇与 4 线程创建), 返回 0/-1/-2/-3/
//   -8/-7/-5/-6 各错误出口保留。
//   CRC16 查表 (DAT_00068aec) 校验按原逐字节流程转写 (首字节字面量 0x51,
//   字节 0..29, 结果取 cfg[7]>>16); 链探测/频率写全局段逐行核对; 日志门控
//   统一还原为 LOG_GATE(prio) (= 原 DAT_0007ad48||DAT_00078e08||
//   DAT_000766c4 >= prio, 与 applog 的门控语义一致)。
//
// equihash_verify_solution_full | 原 FUN_0004e198 | 361 行 | 覆盖 361/361
//   语义: Equihash(200,9) 解完整校验: 查重→叶哈希折叠 (512/N=2)→两轮
//   Wagner 树验证→终 30B XOR 检查, 返回 0=有效/1=重复/2=初始 XOR 失败/
//   3=终 XOR 失败; 失败路径统一 goto 清理并 free 两块 0x4400 元素缓冲。
//   expandArray 21bit 紧凑索引解包/重打包按位级流程核对 (BE 索引读回,
//   21bit 累加器打包); 叶哈希一次 blake 覆盖 pair_div=2 个索引 (512/200
//   整除关系经 0x200/n 除法商确认); 轮内相邻对 XOR→字节对齐比较与原栈
//   布局 (-0x1568/-0x15a8/-0x15c8 区) 逐段对应。qsort 比较器 (0x4df84,
//   前缀字节序比较) 为外部函数, 语义在调用点注释。
//
// scrypt_N_1_256           | 原 FUN_000122e8 | 1133 行 | 覆盖 1133/1133
//   语义: 标准 scrypt(N=1024, r=1, p=1, dkLen=32): SHA256(80B 工作头) 为
//   HMAC 密钥 → PBKDF2-HMAC-SHA256 派生 128B → ROMix (1024 项 V 表,
//   BlockMix=Salsa20/8, r=1) → 终轮 PBKDF2 输出 32B DK。
//   数值验证 (gcc 编译比对, 四级):
//     a) SHA-256 单块 (64B) 与 hashlib.sha256 一致;
//     b) 内层 PBKDF2 (128B 消息, 4 次外层续块) 输出与 hashlib.pbkdf2_hmac
//        ('sha256', key=SHA256(hdr), salt=hdr, i=1..4, dklen=128) 逐字一致;
//     c) ROMix 第 1 遍结束 (V[1023] 与 X) 与标准 scrypt 内部状态 (LE 字
//        视图) 一致 — 原固件把 PBKDF2/ROMix 词以 bswap 形式存储, LE 整数
//        视图即标准 scrypt 词义;
//     d) 最终 DK 与 hashlib.scrypt(hdr, salt=hdr, n=1024, r=1, p=1,
//        dklen=32) 一致 (out 以内部词序直写, 词视图=标准字节流), 两组随机
//        工作头向量 0 失配。
//   常量表取证: 阶段-1 填充块 (0x5d5d0, 12 词, 尾 0x80020000=0x5d5d0 bit)、
//   PBKDF2 消息尾 (0x5d600, 0x4a0 bit)、终轮填充 (0x5d64c, INT(1)+0x620)
//   均由 ELF .rodata 直接 dump 比对; 阶段-1 头 4 词 (in[64..67]) 在反编译
//   文本中为 Ghidra 渲染伪影, 经 capstone 反汇编确认 stm 存在 (本转写按
//   disasm 恢复)。Salsa20/8 核心: 4 组列/行四元组按数据流配对提取, 与
//   标准 QR (rotl 7/9/13/18) 完全一致, 转置对合 (T 视图 ↔ x 视图) 已证
//   等价。中间态缓冲分离: 内层消息块与外层续块在原代码中为两个独立栈区
//   (-0x1a8 与 -0x108+8), 转写以 blk/oblk 分立对应 (单缓冲复用会破坏外层
//   尾部, 已由数值验证捕获并修正)。
// ========================================================================== */
