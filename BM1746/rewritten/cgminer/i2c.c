/**
 * i2c.c — cgminer I2C 传感器底层 (BM1746 reg 0x20 I2C 间接寄存器通道)
 *
 * 模块总览:
 *   BM1746 芯片内置 I2C master, 经寄存器 0x20 (BM1746_REG_I2C_INDIRECT) 间接访问
 *   外挂 ISL 电源芯片与 TMP451 温度传感器。I2C 值按位段拼装进 11 字节 UART 命令帧
 *   (帧类型 0x41 带址读 / 0x51 广播写, 见 include/bm1746_regs.h):
 *     value = DAT_00076b6c&0xFF000000 | 0x01000000 | (i2c_reg&0x7F)<<17
 *             | (type&0xFF)<<8 | data        (写侧另置 bit16=0x10000, 读侧 data=0)
 *   type 为 I2C 通道选择, 仅 0 / 1 / 0x11 / 0xFE 四种合法, 各占就绪标志一个槽位。
 *
 *   读事务为异步模型: 本文件 read_i2c_reg 只负责发帧并轮询; 读回数据由 RX 线程
 *   check_asic_reg (FUN_000481ec) 按 RX 帧链字节匹配 DAT_00079130[组] 落入
 *   per-(链,组,type) 结果区, 并将就绪标志 DAT_00079bb4 对应槽位置 1。
 *
 *   本文件 3 个函数 (batch=i2c_b1):
 *     - i2c_write (FUN_0003ade0): 写通道忙等超时回调, 仅输出告警日志
 *     - i2c_read  (FUN_0003ae24): 读通道忙等超时回调, 输出告警并返回错误值 0xff
 *     - read_i2c_reg (FUN_0004886c): 发起一次 reg 0x20 异步 I2C 读事务
 *
 *   忙等循环本体在 i2c_write_core (FUN_0003c66c) / i2c_read_core (FUN_0003c708)
 *   (AXI base[0x30] 命令通道 22 次×5ms 轮询), 属其他转写批次, 本文件仅 extern 引用;
 *   两者在超时时分别回调本文件的 i2c_write / i2c_read。
 *
 * 相关全局:
 *   DAT_00076b48  I2C 每 ASIC 地址表 (前 16 字节 0x50 0x54 0x58 0x01..., 见 bm1746_regs.h G_I2C_PER_ASIC_ADDR*)
 */

#include <stdio.h> /* snprintf — 原调用保留 */

#include "../include/common_types.h"
#include "../include/bm1746_regs.h"

/* ==================== 本文件常量 ==================== */
/* cgminer applog 日志级别 (机器码调用点级别 4/5/7 与门槛值 2/3/4/6/7 同枚举) */
#define LOG_WARNING 4
#define LOG_NOTICE  5

/* ==================== 跨批次被调 (名称取自 name_map/cgminer.tsv) ==================== */
extern void     cgsleep_ms(uint32_t ms);                       /* 原 FUN_0002a92c */
extern void     applog(uint32_t prio, char *buf, ...); /* 原 FUN_00038730, 第 3 参调用点恒传 0 */
extern uint32_t makeup_cmd_frame(uint8_t *frame, uint8_t chip_addr, uint8_t chain_byte,
                                 uint8_t reg, uint32_t value); /* 原 FUN_0003cdcc, 返回帧长 0xB */
extern uint32_t uart_send(int chain, const void *buf, uint32_t len); /* 原 FUN_0003c150 */
extern void     check_asic_reg(uint32_t chain, uint32_t chip_addr, uint32_t reg, int mode); /* 原 FUN_000481ec */
extern void     i2c_write_core(uint32_t cmd_val);              /* 原 FUN_0003c66c — 忙等写循环, 超时回调 i2c_write */
extern uint32_t i2c_read_core(uint32_t cmd_val);               /* 原 FUN_0003c708 — 忙等读循环, 超时回调 i2c_read */

/* ==================== 本模块引用的全局 ==================== */
/* 每组→链地址字节表 (0x79130..0x79132, 3 组): 构帧写入帧偏移 4, check_asic_reg
 * 以其匹配 RX 帧链字节; calculate_address_interval (FUN_000467b0) 清零初始化 */
extern uint8_t  DAT_00079130[3];
/* I2C 读回就绪标志区 (0x79BB4 = 0x78e10+0xda4, 每 chain 12B = 3 组×4 type 槽):
 * check_asic_reg 收到 reg 0x20 读回且链字节匹配时置 1; read_i2c_reg 发帧前清 0 */
extern uint8_t  DAT_00079bb4[12];
extern uint32_t DAT_00076b6c;      /* 全局, 语义待定: 0x20 值拼装高 8 位来源 (&0xFF000000) */
extern uint8_t  DAT_0007ad48;      /* 全局: applog 输出门 1 (任一开关打开则强制输出) */
extern uint8_t  DAT_00078e08;      /* 全局: applog 输出门 2 */
extern uint8_t  DAT_000766c4;      /* 全局: applog 级别门 (与门槛值比较) */

// === i2c_write === 原 FUN_0003ade0 @ 0x0003ade0
// 来源: reversed/cgminer/32_i2c_sensor/FUN_0003ade0.c | 分类: 32_i2c_sensor
// 语义: I2C 写通道忙等超时回调 — i2c_write_core (FUN_0003c66c) 对 AXI base[0x30]
//       命令通道轮询 22×5ms 仍忙 (bit31 未清零) 时调用; 仅输出告警日志, 无副作用。
void i2c_write(void)
{
	char buf[4100]; /* 原栈日志缓冲 */

	snprintf(buf, 0x1000, "%s: i2c always busy, break\n", __func__);
	applog(LOG_NOTICE, buf, 0);
}

// === i2c_read === 原 FUN_0003ae24 @ 0x0003ae24
// 来源: reversed/cgminer/32_i2c_sensor/FUN_0003ae24.c | 分类: 32_i2c_sensor
// 语义: I2C 读通道忙等超时回调 — i2c_read_core (FUN_0003c708) 轮询读回超时时调用;
//       输出告警日志并返回 0xff (I2C 读数据错误占位值, 调用方按字节使用)。
uint32_t i2c_read(void)
{
	char buf[4100]; /* 原栈日志缓冲 */

	snprintf(buf, 0x1000, "%s: i2c always busy, break\n", __func__);
	applog(LOG_NOTICE, buf, 0);
	return 0xff;
}

// === read_i2c_reg === 原 FUN_0004886c @ 0x0004886c
// 来源: reversed/cgminer/32_i2c_sensor/FUN_0004886c.c | 分类: 32_i2c_sensor
// 语义: 经芯片内 I2C master (reg 0x20 间接寄存器) 发起一次异步 I2C 读:
//       等 50ms → 校验 reg_type (仅 0/1/0x11/0xFE, 否则按输出门决定是否告警后返回)
//       → 构 11B 读帧 (帧类型 0x41, 帧偏移 4 = DAT_00079130[组] 链地址字节)
//       → uart_send → 等 200ms → 清该 (链,组,type) 就绪标志 → 最多 3 轮 (每轮
//       等 50ms 后发 check_asic_reg 读回查询), 标志置位即返回 (数据已由
//       check_asic_reg 落入 per-group 结果区); 3 轮无回则静默返回。无返回值。
//       调用方: set_temperature_offset_value_chain_sensor (FUN_000490fc) /
//               calibration_sensor_offset_chain (FUN_000492b0)。
void read_i2c_reg(int chain, int group, uint32_t i2c_reg, uint32_t reg_type)
{
	uint8_t frame[BM1746_FRAME_LEN] = {0}; /* 11B 命令帧 (原栈 4+4+2+1 连续局部字节) */
	char buf[4100];                        /* 原栈日志缓冲 */
	uint32_t value;
	uint32_t type_slot; /* reg_type → 就绪标志槽位: 0/1→0/1, 0x11→2, 0xFE→3 */
	int flag_idx;
	int round;

	cgsleep_ms(0x32); /* 50ms — 发帧前固定延时 */

	if ((reg_type == 0) || (reg_type == 1)) {
		type_slot = reg_type;
	}
	else if (reg_type == 0x11) {
		type_slot = 2;
	}
	else {
		if (reg_type != 0xFE) {
			/* 非法 reg_type: 未开任何调试输出则静默丢弃 */
			if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 4)) {
				return;
			}
			snprintf(buf, 0x1000, "%s unknow reg_type %d\n", __func__, reg_type);
			applog(LOG_WARNING, buf, 0);
			return;
		}
		type_slot = 3;
	}

	/* 0x20 I2C 间接值拼装 (bm1746_regs.h): 读事务 bit16=0, 数据字节=0 */
	round = 3;
	value = (DAT_00076b6c & 0xFF000000u) /* 高 8 位: 全局链级基准 */
	      | BM1746_I2C_IND_FIXED         /* 0x01000000 固定位 */
	      | ((i2c_reg & BM1746_I2C_REG_MASK) << BM1746_I2C_REG_SHIFT) /* bits 23..17: I2C 寄存器号 (7bit) */
	      | ((reg_type & 0xFFu) << BM1746_I2C_TYPE_SHIFT);            /* bits 15..8: I2C 通道类型 */
	makeup_cmd_frame(frame, 0, DAT_00079130[group], BM1746_REG_I2C_INDIRECT, value);
	uart_send(chain, frame, 0xb); /* 0xB = 11 字节整帧 */
	cgsleep_ms(200);              /* 等 I2C 读事务完成 (原码固定 200ms) */

	/* 就绪标志槽位: [链*12 + 组*4 + type槽], 与 check_asic_reg 的 &DAT_00079bb4+chain*0xc 写入一致 */
	flag_idx = (int)type_slot + (chain * 3 + group) * 4;
	DAT_00079bb4[flag_idx] = 0; /* 原码经 (int)&DAT_00078e10+偏移+0xda4 寻址, 即 DAT_00079bb4 */
	do {
		cgsleep_ms(0x32); /* 50ms */
		/* 发读回查询: mode=0 读帧, chipaddr = DAT_00079130[组] (与发帧一致) */
		check_asic_reg(chain, DAT_00079130[group], BM1746_REG_I2C_INDIRECT, 0);
		if (DAT_00079bb4[flag_idx] != 0) {
			return; /* 读回已就绪, 结果由 check_asic_reg 落入 per-group 区 */
		}
		round--;
	} while (round != 0);
	return;
}

/* ==================== 覆盖率表 (3/3) ====================
 * 原 FUN_0003ade0 @ 0x0003ade0 -> i2c_write     (reversed/cgminer/32_i2c_sensor/FUN_0003ade0.c)
 * 原 FUN_0003ae24 @ 0x0003ae24 -> i2c_read      (reversed/cgminer/32_i2c_sensor/FUN_0003ae24.c)
 * 原 FUN_0004886c @ 0x0004886c -> read_i2c_reg  (reversed/cgminer/32_i2c_sensor/FUN_0004886c.c)
 */
