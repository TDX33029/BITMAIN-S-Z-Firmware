/**
 * pic.c — cgminer PIC16F1704 电源管理层 (UART 半双工命令 + flash 升级 + 电压/DC-DC 控制)
 *
 * 模块总览 (batch=pic_b1, 39 函数, 来源 reversed/cgminer/13_voltage_pic/):
 *   PIC16F1704 经 BM1746 同一 UART 半双工通道通信, 帧格式 (bm1746/BM1746_uart_register_ops.md §四-3):
 *     [55][AA][LEN][CMD][ARG...] + 回显校验 (PIC 回 [CMD][01] = 成功)。
 *   命令集 (include/bm1746_regs.h PIC_CMD_*):
 *     0x06 loader→app 跳转, 0x07 PIC 复位, 0x09 取频率, 0x10 设电压 (值 = param+0x15
 *     编码两字节), 0x15/0x1A 关 DC-DC, 0x15/0x1B 开 DC-DC, 心跳轮询。
 *
 *   分层:
 *     - 底层: pic_i2c_write_byte / pic_i2c_read_byte (单字节经 AXI base[0x30] 命令通道),
 *       其忙等循环 i2c_write_core / i2c_read_core (FUN_0003c66c/0003c708) 也在本文件;
 *       超时回调 i2c_write / i2c_read 在 i2c.c (batch=i2c_b1)。
 *     - 帧层: write_dc_dc / read_dc_dc (uart.c, batch=uart_b1) — PIC I2C 位带通道 9B 帧
 *       [55 AA 07 32 (slave<<1|1) reg len chk_hi chk_lo], 16 位和校验。
 *     - 命令层: set_PIC16F1704_*_new 系列 (发帧+回显校验), every_chain_* 系列 (逐链广播)。
 *     - 升级层: flash 指针/擦除/读写 + PIC1704_update_pic_app_program_new (xxtea 解密)。
 *     - 电压层: set_PIC16F1704_voltage_new (合法窗 [700,800]mV, 越界回落 700),
 *       ISL 经 reg 0x20 I2C 间接写 (set_isl_voltage 设+回读 ±4mV 容差)。
 *
 * 本文件引用的跨批次函数均按 name_map/cgminer.tsv 命名, extern 声明见下方;
 * 未知 DAT_ 全局保留原名 (CONVENTIONS §4)。
 */

#include <unistd.h>  /* usleep — 原调用保留 */
#include <stdio.h>   /* snprintf/fopen/fgets/strtoul — 原调用保留 */
#include <stdlib.h>  /* strtoul — 原调用保留 */
#include <string.h>  /* memcpy/memset — 原调用保留 */
#include <pthread.h> /* pthread_mutex_lock/unlock — PIC 位带总线互斥锁 */
#include <stdbool.h> /* true/false — 原经 miner.h 引入 */

#include "../include/common_types.h"
#include "../include/bm1746_regs.h"
#include "../include/fpga_axi.h"

/* ==================== 本文件常量 ==================== */
/* ---- PIC 位带命令通道 (AXI base[0x30], i2c_write_core 轮询) ----
 * 命令值 = 模式位域 | 当前链号 DAT_0007909c<<16 | 字节数据:
 *   写: byte | chain<<16 | 0xA00000        (pic_i2c_write_byte)
 *   读: chain<<16 | 0xA00000, 再由 i2c_read_core 置 bit25 读标志 (pic_i2c_read_byte) */
#define PIC_BB_MODE_BITS       0xA00000u /* bits 23/21 模式标志 (固定拼入, 语义待定) */
#define PIC_BB_READ_FLAG       0x2000000u /* bit25 读方向标志 (即 fpga_axi.h AXI_CMD_PREFIX) */
#define PIC_BB_CHAIN_SHIFT     16        /* bits 17..16: 当前链号 */
#define PIC_BB_POLL_TRIES      0x16      /* i2c_*_core 忙等轮询次数 22 */
#define PIC_BB_POLL_US         5000      /* 每次轮询间隔 5ms (usleep) */

/* ---- PIC UART/I2C 命令帧 (55 AA LEN CMD ARG... chk_hi chk_lo) ----
 * 16 位和校验 chk = LEN + CMD + Σ(ARG), 大端先高后低; LEN = Σ(ARG) + 4
 * (write_dc_dc/flash 系列一致; 读类命令回帧 [LEN][CMD回显][数据][chk], 回 LEN=整帧总长) */
#define PIC_CMD_FLASH_SET_POINTER  0x01  /* LEN 6: [指针Hi][指针Lo], 回 [01 01] = 成功 */
#define PIC_CMD_SEND_DATA          0x02  /* LEN 0x14: [16B 数据], 回 [02 01] = 成功 */
#define PIC_CMD_READ_FLASH_DATA    0x03  /* LEN 4: 无参, 回 [14 03 数据x16 chk chk] = 20B */
#define PIC_CMD_ERASE_FLASH        0x04  /* LEN 4: 无参, 回 [04 01] = 成功 */
#define PIC_CMD_WRITE_INTO_FLASH   0x05  /* LEN 4: 无参, 回 [05 01] = 成功 */
/* 0x06 跳转 loader→app / 0x07 PIC 复位: 见 bm1746_regs.h PIC_CMD_JUMP_LOADER_TO_APP / PIC_CMD_PIC_RESET */
#define PIC_CMD_READ_FLASH_POINTER 0x08  /* LEN 4: 无参, 回 [06 08 ptrHi ptrLo chk chk] = 6B */
#define PIC_CMD_READ_HASH_ID       0x13  /* LEN 4: 无参, 回 [10 13 id×12 chk chk] = 16B */
#define PIC_CMD_HEART_BEAT         0x16  /* LEN 4: 无参, 回 6 字节 (心跳轮询) */
#define PIC_CMD_GET_SW_VERSION     0x17  /* LEN 4: 无参, 回 [05 17 ver chk chk] = 5B */
#define PIC_CMD_WRITE_DATA_FLASH   0x33  /* LEN len+6: [reg][len][data×len], chk 不含 reg 字节 */
#define PIC_CMD_READ_DATA_FLASH    0x34  /* LEN 6: [reg][len], chk = len+0x3A (不含 reg) */
#define PIC_DCDC_DATA_CMD          0x32  /* DC-DC 位带帧命令字节 (帧偏移 3, 同 uart.c) */

#define PIC_LEN_SET_POINTER    6         /* set_PIC16F1704_flash_pointer_new 帧长度域 */
#define PIC_LEN_SEND_DATA      0x14      /* send_data_to_PIC16F1704_new 帧长度域 (16B 数据) */
#define PIC_LEN_NOARG          4         /* 无参命令 (读数据/擦除/写入/跳转/复位/读指针) 帧长度域 */

#define PIC_RESP_STATUS_OK     0x01      /* 写类命令回帧状态字节 (rx[1]) */
#define PIC_FLASH_PAGE_BYTES   0x10      /* PIC flash 每页数据字节数 (send/read data 帧载荷) */

/* ---- 调用点日志门控 (同 uart.c/i2c.c: 开关 A/B 任一打开或级别超门槛) ---- */
#define LOG_GATE7() (DAT_00078e09 && (DAT_0007ad48 || DAT_00078e08 || 6 < DAT_000766c4))
#define LOG_GATE5() (DAT_0007ad48 || DAT_00078e08 || 4 < DAT_000766c4)
#define LOG_GATE4() (DAT_0007ad48 || DAT_00078e08 || 3 < DAT_000766c4)
#define LOG_GATE3() (DAT_0007ad48 || DAT_00078e08 || 2 < DAT_000766c4)

/* ==================== 跨批次被调 (名称取自 name_map/cgminer.tsv) ==================== */
extern void     i2c_write(void);                    /* 原 FUN_0003ade0 — 忙等写超时回调 (i2c.c) */
extern uint32_t i2c_read(void);                     /* 原 FUN_0003ae24 — 忙等读超时回调 (i2c.c) */
extern void     applog(uint32_t prio, char *buf, ...); /* 原 FUN_00038730, 第 3 参调用点恒传 0 */
extern void     cgsleep_ms(int ms);                 /* 原 FUN_0002a92c */
extern int      read_dc_dc(uint8_t slave, int reg, uint8_t *out, uint32_t len); /* 原 FUN_0003ca38 (uart.c) */
extern uint32_t read_asic_voltage(uint32_t slave, uint16_t *out); /* 原 FUN_0003e564 (16_chain_mgmt, 回 0) */
extern int      thr_info_create(void *thr, void *attr, void *(*fn)(void *), void *arg); /* 原 FUN_0002f174 (+util.c) */
extern void    *fun_00040034(void *arg);  /* 原 LAB_00040034 @ 0x00040034 — PIC 心跳线程体 (命名待补) */

/* ==================== 本模块引用的全局 (axi.c / 其他批次定义) ==================== */
extern uint8_t *DAT_00078e10;   /* FPGA AXI 寄存器窗口 mmap 基址 (init_fpga FUN_0003b704, /dev/axi_fpga_dev) */
extern char DAT_0007ad48;       /* 全局: applog 输出门 1 (任一开关打开则强制输出) */
extern char DAT_00078e08;       /* 全局: applog 输出门 2 */
extern int  DAT_000766c4;       /* 全局: 日志级别 (int), 低级别日志需 > 门槛值 */
extern char DAT_000790a0[];     /* PIC/I2C 位带总线互斥锁 (单把, 与 uart.c write/read_dc_dc 共用) */
extern int  DAT_0007909c;       /* 当前链号: pic_i2c_write/read_byte 拼入 AXI 命令 bit17..16 */
extern uint16_t DAT_000790b8[12]; /* 每 ASIC ISL 目标电压表 (0x790b8, 4 链 × 3 组, mV);
                                   * every_asic_set_isl_voltage 写入源 / show_every_asic_voltage 打印 */
extern char DAT_00078e09;       /* 全局: opt_debug (curses 'd' Debug mode), LOG_GATE7 前提开关 */
extern char DAT_000790d0[];     /* send/read_data_to/from_all_asic 互斥锁 (单把, 与 uart.c 共用) */
extern char DAT_0009d4bb[];     /* +1 起 (0x9d4bc) 为链存在标志数组: 值==1 表示链在位 */
extern void *DAT_0009d4ac;      /* 心跳线程 thr_info 结构 (calloc 0x40, +0xC = pthread_t) */
extern uint8_t DAT_00069af8[];  /* PIC flash 数据 XXTEA 密钥表: 每链 16 字节 (4 字), 链 i 取 +0x10*i */
extern uint8_t DAT_00076b48[];  /* 每 ASIC 组 I2C 从机地址表: 前 3 项 0x50/0x54/0x58 (同 uart.c) */

/* 本批次内前向声明 (定义在本文件) */
void     i2c_write_core(uint32_t cmd_val);       /* 原 FUN_0003c66c — i2c.c extern 引用 */
uint32_t i2c_read_core(uint32_t cmd_val);        /* 原 FUN_0003c708 — i2c.c extern 引用 */
void     xxtea_decode(uint32_t *data, int n, uint32_t *key); /* 原 FUN_00056cd8 */

/* AXI 命令寄存器访问: DAT_00078e10 为字节指针, 0x30 为字节偏移 (反汇编 ldr [r3,#0x30] 证实) */
#define PIC_CMD_REG32() (*(volatile uint32_t *)(DAT_00078e10 + AXI_REG_CMD))

// === i2c_write_core === 原 FUN_0003c66c @ 0x0003c66c
// 来源: reversed/cgminer/13_voltage_pic/FUN_0003c66c.c | 分类: 13_voltage_pic
// 语义: PIC 位带命令通道忙等写循环 — 轮询 AXI base[0x30] 最多 22 次 × 5ms, 读值 < 0
//       (bit31=1, 通道空闲) 时写入 cmd_val 并返回; 22 次仍忙则按日志门调用超时回调
//       i2c_write (FUN_0003ade0, i2c.c) 输出 "i2c always busy"。无返回值。
void i2c_write_core(uint32_t cmd_val)
{
	int tries;

	for (tries = PIC_BB_POLL_TRIES; tries != 0; tries--) {
		if ((int)PIC_CMD_REG32() < 0) { /* bit31=1: 通道空闲, 可写入 */
			PIC_CMD_REG32() = cmd_val;
			return;
		}
		usleep(PIC_BB_POLL_US);
	}
	if (LOG_GATE5()) {
		i2c_write(); /* 忙等超时回调 (仅日志) */
	}
}

// === i2c_read_core === 原 FUN_0003c708 @ 0x0003c708
// 来源: reversed/cgminer/13_voltage_pic/FUN_0003c708.c | 分类: 13_voltage_pic
// 语义: PIC 位带命令通道忙等读循环 — 先经 i2c_write_core 写入 (cmd_val|bit25 读标志),
//       再轮询 base[0x30] 最多 22 次 × 5ms, 读值 < 0 时返回其低 8 位 (读回字节);
//       超时: 未开日志门返回 0xff, 否则调用超时回调 i2c_read (FUN_0003ae24, i2c.c)
//       输出告警并返回 0xff。
uint32_t i2c_read_core(uint32_t cmd_val)
{
	uint32_t val;
	int tries;

	i2c_write_core(cmd_val | PIC_BB_READ_FLAG);
	for (tries = PIC_BB_POLL_TRIES; tries != 0; tries--) {
		usleep(PIC_BB_POLL_US);
		val = PIC_CMD_REG32();
		if ((int)val < 0) {
			return val & 0xff; /* uxtb: 读回字节 */
		}
	}
	if (LOG_GATE5()) {
		return i2c_read(); /* 忙等超时回调: 告警并返回 0xff */
	}
	return 0xff;
}

// === pic_i2c_write_byte === 原 FUN_0003c7b0 @ 0x0003c7b0
// 来源: reversed/cgminer/13_voltage_pic/FUN_0003c7b0.c | 分类: 13_voltage_pic
// 语义: 经位带命令通道写 1 字节到 PIC — 命令值 = byte | 当前链号<<16 | 0xA00000,
//       直通 i2c_write_core 忙等写循环。当前链号取全局 DAT_0007909c (调用方先置)。
void pic_i2c_write_byte(uint8_t byte)
{
	i2c_write_core((uint32_t)byte | (DAT_0007909c << PIC_BB_CHAIN_SHIFT) | PIC_BB_MODE_BITS);
}

// === pic_i2c_read_byte === 原 FUN_0003c7c8 @ 0x0003c7c8
// 来源: reversed/cgminer/13_voltage_pic/FUN_0003c7c8.c | 分类: 13_voltage_pic
// 语义: 经位带命令通道从 PIC 读回 1 字节 — 命令值 = 当前链号<<16 | 0xA00000 (读标志由
//       i2c_read_core 置位), 返回 i2c_read_core 的低 8 位读回值 (反汇编确认 r0 实有返回)。
uint8_t pic_i2c_read_byte(void)
{
	return (uint8_t)i2c_read_core((DAT_0007909c << PIC_BB_CHAIN_SHIFT) | PIC_BB_MODE_BITS);
}

// === write_dc_dc === 原 FUN_0003c7e0 @ 0x0003c7e0
// 来源: reversed/cgminer/13_voltage_pic/FUN_0003c7e0.c | 分类: 13_voltage_pic
// 语义: 经 PIC I2C 位带通道向 DC-DC 从机写数据: 持总线锁逐字节发帧
//       [55 AA (len+6) 32 (slave<<1) addr data×len chk_hi chk_lo]
//       (校验和 = 帧长+0x32+写地址+addr+Σdata 的 16 位和, 大端先高后低), 等 200ms 后
//       回读 4 字节 (仅作时序占用, 值被丢弃)。Ghidra 将回读校验成功分支标记为不可达
//       (ram,0x3ca0c/0x3ca14 移除), 可见尾部恒为: 按日志门打印 "failed!" (回读占位常量
//       0xff,0,0,0) 并返回 0xffffffff。帧区越界写 (len 过大) 原样保留。
//       调用方: send_data_to_all_asic (uart.c, 每链 3 组 I2C 从机广播写)。
uint32_t write_dc_dc(uint8_t slave, uint8_t addr, uint8_t *data, int len)
{
	/* 原栈连续布局 (回读区/帧区/日志区邻接, len 过大时帧写越界进入后续区域, 原样保留):
	 * [0..3]    local_1040 — I2C 回读 4 字节
	 * [4..15]   local_103c — 12B 间隙 (原栈邻接, 无访问)
	 * [16..31]  local_1030/local_102c/uStack_1028/local_1024 — TX 帧区 (帧基址 = +16)
	 * [32..4127] acStack_1020 — snprintf 日志缓冲 (0x1000) */
	uint8_t stack_buf[0x1040];
	uint8_t *frame = stack_buf + 16; /* 原 local_1030 */
	uint8_t *p;
	uint32_t i;
	uint32_t cnt;
	uint32_t sum;
	uint32_t sum_last;
	uint32_t chk_hi;
	uint32_t chk_lo;

	frame[0] = PIC_FRAME_PREAMBLE0;                       /* 0x55 */
	frame[1] = PIC_FRAME_PREAMBLE1;                       /* 0xAA */
	frame[2] = (uint8_t)(len + 6);                        /* 长度域 = 数据长 + 6 */
	frame[3] = PIC_DCDC_DATA_CMD;                         /* 0x32 */
	frame[4] = (uint8_t)(slave << 1);                     /* I2C 写地址: 从机地址<<1 (读帧才 |1) */
	frame[5] = addr;                                      /* DC-DC 寄存器号 */
	i = 0;
	if (0 < len) {
		do {                                             /* 数据字节拷入帧 (索引 8 位回绕, 原样保留) */
			frame[6 + i] = data[i];
			i = (i + 1) & 0xff;
		} while ((int)i < len);
	}
	if (len < -3) {
		chk_hi = 0;                                       /* 原 do-while 零圈保护分支 (Ghidra 还原) */
		chk_lo = 0;
	}
	else {
		sum = 0;
		p = frame + 1;
		do {                                             /* chk = Σ frame[2..len+5] (LEN..末数据字节) */
			p++;
			sum_last = sum + *p;
			sum = sum_last & 0xffff;
		} while (p != frame + len + 5);
		chk_hi = (sum >> 8) & 0xff;                       /* 16 位和, 大端先高后低 */
		chk_lo = sum_last & 0xff;
	}
	frame[len + 6] = (uint8_t)chk_hi;
	frame[len + 7] = (uint8_t)chk_lo;

	pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
	cnt = 0;
	do {                                                  /* 逐字节位带发送整帧 (索引 8 位回绕) */
		pic_i2c_write_byte(frame[cnt]);
		cnt = (cnt + 1) & 0xff;
	} while (cnt <= (((uint32_t)(len + 6) & 0xff) + 1));
	usleep(200000);                                       /* 200ms — 等 PIC 处理 */
	stack_buf[0] = pic_i2c_read_byte();                   /* 回读 4 字节 (local_1040[0..3]) */
	stack_buf[1] = pic_i2c_read_byte();
	stack_buf[2] = pic_i2c_read_byte();
	stack_buf[3] = pic_i2c_read_byte();
	pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
	if (LOG_GATE5()) {
		snprintf((char *)stack_buf + 32, 0x1000,
		         "--- %s failed! read_back_data[0] = 0x%02x, read_back_data[1] = 0x%02x, read_back_data[2] = 0x%02x, read_back_data[3] = 0x%02x\n\n"
		         ,"write_dc_dc",0xff,0,0,0);
		applog(5, (char *)stack_buf + 32, 0);
	}
	return 0xffffffff;
}

// === set_PIC16F1704_flash_pointer_new === 原 FUN_0003d000 @ 0x0003d000
// 来源: reversed/cgminer/13_voltage_pic/FUN_0003d000.c | 分类: 13_voltage_pic
// 语义: 设置 PIC16F1704 flash 读/写指针 (16 位地址, 参数按帧序 [addr_hi][addr_lo]):
//       发 8 字节帧 [55 AA 06 01 hi lo chk_hi chk_lo] (chk = 6+1+hi+lo), 等 100ms,
//       回读 2 字节, [01][01] = 成功返回 1; 否则按日志门打印失败并返回 0。
//       调用方: write_data_to_PIC16F1704_flash / read_data_from_PIC16F1704_flash。
uint32_t set_PIC16F1704_flash_pointer_new(int addr_hi, int addr_lo)
{
	uint8_t frame[8];
	char logbuf[0x1000]; /* 原 acStack_1010 */
	int chk;             /* 原 iVar4 (int 算术) */
	uint8_t resp0;       /* 原 param5 */
	uint8_t resp1;       /* 原 param6 */
	uint32_t ret;

	frame[0] = PIC_FRAME_PREAMBLE0;
	frame[1] = PIC_FRAME_PREAMBLE1;
	frame[2] = PIC_LEN_SET_POINTER;                       /* 0x06 */
	frame[3] = PIC_CMD_FLASH_SET_POINTER;                 /* 0x01 */
	frame[4] = (uint8_t)addr_hi;
	frame[5] = (uint8_t)addr_lo;
	chk = addr_lo + addr_hi + 7;                          /* chk = LEN(6)+CMD(1)+两参数 */
	frame[6] = (uint8_t)(chk >> 8);                       /* 大端先高后低 */
	frame[7] = (uint8_t)chk;

	pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
	for (ret = 0; ret < 8; ret++) {
		pic_i2c_write_byte(frame[ret]);
	}
	usleep(100000);                                       /* 100ms — 等 PIC 处理 */
	resp0 = pic_i2c_read_byte();                          /* 回 [CMD 回显][状态] */
	resp1 = pic_i2c_read_byte();
	pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
	if ((resp0 == 1) && (resp1 == 1)) {
		if (LOG_GATE5()) {
			snprintf(logbuf, 0x1000, "%s ok", "set_PIC16F1704_flash_pointer_new");
			applog(5, logbuf, 0);
		}
		ret = 1;
	}
	else {
		if (LOG_GATE3()) {
			snprintf(logbuf, 0x1000,
			         "%s chain %d failed! read_back_data[0] = 0x%02x, read_back_data[1] = 0x%02x\n\n",
			         "set_PIC16F1704_flash_pointer_new",DAT_0007909c,resp0,resp1);
			applog(3, logbuf, 0);
		}
		ret = 0;
	}
	return ret;
}

// === send_data_to_PIC16F1704_new === 原 FUN_0003d1ac @ 0x0003d1ac
// 来源: reversed/cgminer/13_voltage_pic/FUN_0003d1ac.c | 分类: 13_voltage_pic
// 语义: 向 PIC 发送 16 字节页数据 (配合 flash 指针与 write_into_flash): 发 22 字节帧
//       [55 AA 14 02 data×16 chk_hi chk_lo] (chk = LEN 0x14 + CMD 2 + Σdata, 大端
//       先高后低), 等 100ms 回读 [02][01] = 成功。成功返回 1, 失败按日志门
//       (级别 3) 打印并返回 0。
uint32_t send_data_to_PIC16F1704_new(uint8_t *data)
{
	uint8_t frame[22];
	char logbuf[4100];   /* 原 acStack_1010 */
	uint32_t i;
	uint32_t sum;        /* 原 uVar4: 16 位校验和 (低字节取自未截断累加值, 等价) */
	uint32_t sum_last;   /* 原 uVar5 */
	uint32_t resp0;      /* 原 uVar4 复用 */
	uint32_t resp1;      /* 原 uVar5 */
	uint32_t ret;

	sum = PIC_LEN_SEND_DATA + PIC_CMD_SEND_DATA;           /* 原 0x16 = LEN(0x14)+CMD(2) */
	for (i = 0; i < PIC_FLASH_PAGE_BYTES; i++) {
		sum_last = sum + data[i];
		sum = sum_last & 0xffff;
	}
	frame[0] = PIC_FRAME_PREAMBLE0;
	frame[1] = PIC_FRAME_PREAMBLE1;
	frame[2] = PIC_LEN_SEND_DATA;                          /* 0x14 = 16 数据 + 4 */
	frame[3] = PIC_CMD_SEND_DATA;                          /* 0x02 */
	for (i = 0; i < PIC_FLASH_PAGE_BYTES; i++) {
		frame[4 + i] = data[i];
	}
	frame[20] = (uint8_t)(sum >> 8);                       /* chk_hi (原 CONCAT11 两字节存, 大端) */
	frame[21] = (uint8_t)sum;                              /* chk_lo */

	pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
	for (i = 0; i < 22; i++) {
		pic_i2c_write_byte(frame[i]);
	}
	usleep(100000);                                        /* 100ms — 等 PIC 收页 */
	resp0 = pic_i2c_read_byte();
	resp1 = pic_i2c_read_byte();
	pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
	if ((resp0 == PIC_CMD_SEND_DATA) && (resp1 == PIC_RESP_STATUS_OK)) {
		if (LOG_GATE5()) {
			snprintf(logbuf, 0x1000, "%s ok", "send_data_to_PIC16F1704_new");
			applog(5, logbuf, 0);
		}
		ret = 1;
	}
	else {
		if (LOG_GATE3()) {
			snprintf(logbuf, 0x1000,
			         "%s chain %d failed! read_back_data[0] = 0x%02x, read_back_data[1] = 0x%02x\n\n",
			         "send_data_to_PIC16F1704_new",DAT_0007909c,resp0,resp1);
			applog(3, logbuf, 0);
		}
		ret = 0;
	}
	return ret;
}

// === read_PIC16F1704_flash_pointer_new === 原 FUN_0003d3b4 @ 0x0003d3b4
// 来源: reversed/cgminer/13_voltage_pic/FUN_0003d3b4.c | 分类: 13_voltage_pic
// 语义: 读 PIC16F1704 flash 指针: 发 6 字节帧 [55 AA 04 08 00 0C] (chk = LEN4+CMD8),
//       等 100ms 回读 6 字节 [LEN=06][CMD回显 08][ptrHi][ptrLo][chk_hi chk_lo];
//       CMD 回显与 chk (= ptrHi+ptrLo+0xE) 均对时, 把指针写入 *addr_h/*addr_l 并返回 1,
//       否则按日志门 (级别 3) 打印 6 字节读回原文并返回 0。回读区原样预置 local_1028=0xff。
uint8_t read_PIC16F1704_flash_pointer_new(uint8_t *addr_h, uint8_t *addr_l)
{
	uint8_t frame[6];
	uint8_t rb[6];       /* 读回区: 原 local_1028[4] + local_1024[2] 连续 */
	char logbuf[0x1000]; /* 原 acStack_1018 */
	uint32_t chk_expect; /* 原 uVar2: 期望校验和 */
	uint32_t resp_len;   /* 原 param5 = rb[3] (ptrLo) 兼作日志参数 */

	frame[0] = PIC_FRAME_PREAMBLE0;
	frame[1] = PIC_FRAME_PREAMBLE1;
	frame[2] = PIC_LEN_NOARG;                            /* 0x04 */
	frame[3] = PIC_CMD_READ_FLASH_POINTER;               /* 0x08 */
	frame[4] = 0x00;                                     /* chk_hi = LEN+CMD = 0x0C */
	frame[5] = 0x0c;
	rb[0] = 0xff;                                        /* 原样预置 local_1028 = 0xff (4B LE: ff 00 00 00) */
	rb[1] = 0;
	rb[2] = 0;
	rb[3] = 0;
	rb[4] = 0;                                           /* 原样预置 local_1024 = 0 (2B) */
	rb[5] = 0;

	pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
	for (chk_expect = 0; chk_expect < 6; chk_expect++) {
		pic_i2c_write_byte(frame[chk_expect]);
	}
	usleep(100000);                                      /* 100ms — 等 PIC 处理 */
	for (chk_expect = 0; chk_expect < 6; chk_expect++) {
		rb[chk_expect] = pic_i2c_read_byte();
	}
	pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
	chk_expect = (uint32_t)(rb[1] & 0xff);               /* 原 uVar2 复用: 先取 rx[1] (CMD 回显) */
	if ((chk_expect == PIC_CMD_READ_FLASH_POINTER) && (rb[0] == 6)) {
		resp_len = rb[3];                                /* ptrLo */
		chk_expect = rb[2] + resp_len + 0xe;             /* 期望 chk = ptrHi+ptrLo+LEN(6)+CMD(8) */
		if (((uint32_t)rb[4] == (chk_expect >> 8)) && ((uint32_t)rb[5] == (chk_expect & 0xff))) {
			*addr_h = rb[2];
			*addr_l = rb[3];
			if (LOG_GATE5()) {
				snprintf(logbuf, 0x1000, "%s ok! flash_addr_h = 0x%02x, flash_addr_l = 0x%02x",
				         "read_PIC16F1704_flash_pointer_new",(uint32_t)*addr_h,resp_len);
				applog(5, logbuf, 0);
			}
			return 1;
		}
		if (!LOG_GATE3()) {
			return 0;
		}
		snprintf(logbuf, 0x1000,
		         "%s failed!!! read_back_data[0] = 0x%x, read_back_data[1] = 0x%x, read_back_data[2] = 0x%x, read_back_data[3] = 0x%x, read_back_data[4] = 0x%x, read_back_data[5] = 0x%x\n\n"
		         ,"read_PIC16F1704_flash_pointer_new",6,8,(uint32_t)rb[2],resp_len,
		         (uint32_t)rb[4],(uint32_t)rb[5]);
	}
	else {
		if (!LOG_GATE3()) {
			return 0;
		}
		snprintf(logbuf, 0x1000,
		         "%s chain %d failed! read_back_data[0] = 0x%x, read_back_data[1] = 0x%x, read_back_data[2] = 0x%x, read_back_data[3] = 0x%x, read_back_data[4] = 0x%x, read_back_data[5] = 0x%x\n\n"
		         ,"read_PIC16F1704_flash_pointer_new",DAT_0007909c,rb[0],chk_expect,
		         (uint32_t)rb[2],(uint32_t)rb[3],(uint32_t)rb[4],(uint32_t)rb[5]);
	}
	applog(3, logbuf, 0);
	return 0;
}

// === read_PIC16F1704_flash_data_new === 原 FUN_0003d67c @ 0x0003d67c
// 来源: reversed/cgminer/13_voltage_pic/FUN_0003d67c.c | 分类: 13_voltage_pic
// 语义: 按 flash 指针读 PIC16F1704 一页数据: 发 6 字节帧 [55 AA 04 03 00 07]
//       (chk = LEN4+CMD3), 等 300ms 回读 20 字节 [LEN=0x14][CMD回显 03][数据×16][chk_hi chk_lo]
//       (先按日志门 5 打印 20 字节读回原文, 等 100ms 再校验); CMD 回显/LEN 对且
//       chk (= 0x17+Σ数据, 0x17=LEN+CMD) 对时把 16 字节拷入 out 并返回 1;
//       CRC 错打印 "failed! crc = 0x%04x" (计算值), 命令错打印 "chain %d failed!", 均返回 0。
uint8_t read_PIC16F1704_flash_data_new(uint8_t *out)
{
	uint8_t frame[6];
	uint8_t rb[20];      /* 读回区: 原 local_1034/local_1030/local_102c/local_1028/local_1024 连续 */
	char logbuf[0x1000]; /* 原 acStack_1020 */
	uint32_t i;
	uint32_t sum;        /* 原 uVar3: 计算校验和 */
	uint32_t sum16;      /* 原 uVar4: 16 位截断 (失败日志打印值) */
	uint8_t data0;       /* 原 uVar6: 拷贝游标当前字节 (rb[2] 起) */

	frame[0] = PIC_FRAME_PREAMBLE0;
	frame[1] = PIC_FRAME_PREAMBLE1;
	frame[2] = PIC_LEN_NOARG;                            /* 0x04 */
	frame[3] = PIC_CMD_READ_FLASH_DATA;                  /* 0x03 */
	frame[4] = 0x00;                                     /* chk_hi = LEN+CMD = 7 */
	frame[5] = 0x07;
	/* 原样预置读回区: local_1034 = 0xff (4B LE), local_1030/102c/1028/1024 = 0 (整存清零) */
	rb[0] = 0xff;
	for (i = 1; i < 20; i++) {
		rb[i] = 0;
	}

	pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
	for (i = 0; i < 6; i++) {
		pic_i2c_write_byte(frame[i]);
	}
	usleep(300000);                                      /* 300ms — 等 PIC 读 flash */
	for (i = 0; i < 20; i++) {
		rb[i] = pic_i2c_read_byte();
	}
	pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
	if (LOG_GATE5()) {
		snprintf(logbuf, 0x1000,
		         "%s: read_back_data[0] = 0x%x, read_back_data[1] = 0x%x, read_back_data[2] = 0x%x, read_back_data[3] = 0x%x, \t\tread_back_data[4] = 0x%x, read_back_data[5] = 0x%x, read_back_data[6] = 0x%x, read_back_data[7] = 0x%x, \t\tread_back_data[8] = 0x%x, read_back_data[9] = 0x%x, read_back_data[10] = 0x%x, read_back_data[11] = 0x%x, \t\tread_back_data[12] = 0x%x, read_back_data[13] = 0x%x, read_back_data[14] = 0x%x, read_back_data[15] = 0x%x, \t\tread_back_data[16] = 0x%x, read_back_data[17] = 0x%x, read_back_data[18] = 0x%x, read_back_data[19] = 0x%x\n"
		         ,"read_PIC16F1704_flash_data_new",rb[0],rb[1],rb[2],rb[3],
		         rb[4],rb[5],rb[6],rb[7],rb[8],rb[9],rb[10],rb[11],
		         rb[12],rb[13],rb[14],rb[15],rb[16],rb[17],rb[18],rb[19]);
		applog(5, logbuf, 0);
	}
	usleep(100000);                                      /* 100ms — 读回后固定延时 */
	if ((rb[1] == PIC_CMD_READ_FLASH_DATA) && (rb[0] == 0x14)) {
		data0 = rb[2];
		sum = (rb[17] & 0xff) + (rb[16] & 0xff) + rb[15] + (rb[14] & 0xff) +
		      (rb[13] & 0xff) + (rb[12] & 0xff) + rb[11] + (rb[10] & 0xff) +
		      (rb[9] & 0xff) + (rb[8] & 0xff) + rb[7] + (rb[6] & 0xff) +
		      (rb[5] & 0xff) + (rb[4] & 0xff) + data0 + rb[3] + 0x17;
		sum16 = sum & 0xffff;
		if (((rb[18] & 0xff) == (sum16 >> 8)) && (rb[19] == (sum & 0xff))) {
			for (i = 0; i < PIC_FLASH_PAGE_BYTES; i++) {  /* 数据 16 字节 → out */
				out[i] = data0;
				if (i + 1 < PIC_FLASH_PAGE_BYTES) {
					data0 = rb[3 + i];
				}
			}
			if (LOG_GATE5()) {
				snprintf(logbuf, 0x1000, "%s ok", "read_PIC16F1704_flash_data_new");
				applog(5, logbuf, 0);
			}
			return 1;
		}
		if (!LOG_GATE3()) {
			return 0;
		}
		snprintf(logbuf, 0x1000, "%s failed! crc = 0x%04x\n\n",
		         "read_PIC16F1704_flash_data_new",sum16);
	}
	else {
		if (!LOG_GATE3()) {
			return 0;
		}
		snprintf(logbuf, 0x1000, "%s chain %d failed!",
		         "read_PIC16F1704_flash_data_new",DAT_0007909c);
	}
	applog(3, logbuf, 0);
	return 0;
}

// === erase_PIC16F1704_flash_new === 原 FUN_0003da84 @ 0x0003da84
// 来源: reversed/cgminer/13_voltage_pic/FUN_0003da84.c | 分类: 13_voltage_pic
// 语义: 擦除 PIC16F1704 flash (数据区): 发 6 字节帧 [55 AA 04 04 00 08] (chk = LEN4+CMD4),
//       等 100ms 回读 [CMD 回显 04][状态 01] = 成功; 解锁后再等 200ms。
//       成功返回 1, 失败按日志门 (级别 3) 打印并返回 0。
uint32_t erase_PIC16F1704_flash_new(void)
{
	uint8_t frame[6];
	char logbuf[0x1000]; /* 原 acStack_1010 */
	uint32_t resp0;      /* 原 param5 */
	uint32_t resp1;      /* 原 param6 */
	uint32_t ret;

	frame[0] = PIC_FRAME_PREAMBLE0;
	frame[1] = PIC_FRAME_PREAMBLE1;
	frame[2] = PIC_LEN_NOARG;                            /* 0x04 */
	frame[3] = PIC_CMD_ERASE_FLASH;                      /* 0x04 */
	frame[4] = 0x00;                                     /* chk_hi = LEN+CMD = 8 */
	frame[5] = 0x08;

	pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
	for (ret = 0; ret < 6; ret++) {
		pic_i2c_write_byte(frame[ret]);
	}
	usleep(100000);                                      /* 100ms — 等 PIC 擦除 */
	resp0 = pic_i2c_read_byte();
	resp1 = pic_i2c_read_byte();
	pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
	usleep(200000);                                      /* 200ms — 擦除后固定延时 */
	if ((resp0 == PIC_CMD_ERASE_FLASH) && (resp1 == PIC_RESP_STATUS_OK)) {
		if (LOG_GATE5()) {
			snprintf(logbuf, 0x1000, "%s ok", "erase_PIC16F1704_flash_new");
			applog(5, logbuf, 0);
		}
		ret = 1;
	}
	else {
		if (LOG_GATE3()) {
			snprintf(logbuf, 0x1000,
			         "%s chain %d failed! read_back_data[0] = 0x%02x, read_back_data[1] = 0x%02x\n\n",
			         "erase_PIC16F1704_flash_new",DAT_0007909c,resp0,resp1);
			applog(3, logbuf, 0);
		}
		ret = 0;
	}
	return ret;
}

// === write_data_into_PIC16F1704_flash_new === 原 FUN_0003dc28 @ 0x0003dc28
// 来源: reversed/cgminer/13_voltage_pic/FUN_0003dc28.c | 分类: 13_voltage_pic
// 语义: 触发 PIC 把已收数据写入 flash (配合 set 指针 + send_data): 发 6 字节帧
//       [55 AA 04 05 00 09] (chk = LEN4+CMD5), 等 200ms 回读 [05][01] = 成功;
//       解锁后再等 200ms。成功返回 1, 失败按日志门 (级别 3) 打印并返回 0。
uint32_t write_data_into_PIC16F1704_flash_new(void)
{
	uint8_t frame[6];
	char logbuf[0x1000]; /* 原 acStack_1010 */
	uint32_t resp0;      /* 原 param5 */
	uint32_t resp1;      /* 原 param6 */
	uint32_t ret;

	frame[0] = PIC_FRAME_PREAMBLE0;
	frame[1] = PIC_FRAME_PREAMBLE1;
	frame[2] = PIC_LEN_NOARG;                            /* 0x04 */
	frame[3] = PIC_CMD_WRITE_INTO_FLASH;                 /* 0x05 */
	frame[4] = 0x00;                                     /* chk_hi = LEN+CMD = 9 */
	frame[5] = 0x09;

	pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
	for (ret = 0; ret < 6; ret++) {
		pic_i2c_write_byte(frame[ret]);
	}
	usleep(200000);                                      /* 200ms — 等 PIC 写 flash */
	resp0 = pic_i2c_read_byte();
	resp1 = pic_i2c_read_byte();
	pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
	usleep(200000);                                      /* 200ms — 写入后固定延时 */
	if ((resp0 == PIC_CMD_WRITE_INTO_FLASH) && (resp1 == PIC_RESP_STATUS_OK)) {
		if (LOG_GATE5()) {
			snprintf(logbuf, 0x1000, "%s ok\n\n", "write_data_into_PIC16F1704_flash_new");
			applog(5, logbuf, 0);
		}
		ret = 1;
	}
	else {
		if (LOG_GATE3()) {
			snprintf(logbuf, 0x1000,
			         "%s chain %d failed! read_back_data[0] = 0x%02x, read_back_data[1] = 0x%02x\n\n",
			         "write_data_into_PIC16F1704_flash_new",DAT_0007909c,resp0,resp1);
			applog(3, logbuf, 0);
		}
		ret = 0;
	}
	return ret;
}

// === jump_from_loader_to_app_PIC16F1704_new === 原 FUN_0003ddd0 @ 0x0003ddd0
// 来源: reversed/cgminer/13_voltage_pic/FUN_0003ddd0.c | 分类: 13_voltage_pic
// 语义: 令 PIC 从 bootloader 跳转到 app: 发 6 字节帧 [55 AA 04 06 00 0A]
//       (chk = LEN4+CMD6), 等 100ms 回读 [06][01] = 成功; 解锁后再等 300ms。
//       成功返回 1, 失败按日志门 (级别 3) 打印 (格式串无结尾换行, 原样) 并返回 0。
uint32_t jump_from_loader_to_app_PIC16F1704_new(void)
{
	uint8_t frame[6];
	char logbuf[0x1000]; /* 原 acStack_1010 */
	uint32_t resp0;      /* 原 param5 */
	uint32_t resp1;      /* 原 param6 */
	uint32_t ret;

	frame[0] = PIC_FRAME_PREAMBLE0;
	frame[1] = PIC_FRAME_PREAMBLE1;
	frame[2] = PIC_LEN_NOARG;                            /* 0x04 */
	frame[3] = PIC_CMD_JUMP_LOADER_TO_APP;               /* 0x06 (bm1746_regs.h) */
	frame[4] = 0x00;                                     /* chk_hi = LEN+CMD = 0x0A */
	frame[5] = 0x0a;

	pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
	for (ret = 0; ret < 6; ret++) {
		pic_i2c_write_byte(frame[ret]);
	}
	usleep(100000);                                      /* 100ms — 等 PIC 跳转 */
	resp0 = pic_i2c_read_byte();
	resp1 = pic_i2c_read_byte();
	pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
	usleep(300000);                                      /* 300ms — 跳转后固定延时 */
	if ((resp0 == PIC_CMD_JUMP_LOADER_TO_APP) && (resp1 == PIC_RESP_STATUS_OK)) {
		if (LOG_GATE5()) {
			snprintf(logbuf, 0x1000, "%s ok", "jump_from_loader_to_app_PIC16F1704_new");
			applog(5, logbuf, 0);
		}
		ret = 1;
	}
	else {
		if (LOG_GATE3()) {
			snprintf(logbuf, 0x1000,
			         "%s chain %d failed! read_back_data[0] = 0x%02x, read_back_data[1] = 0x%02x",
			         "jump_from_loader_to_app_PIC16F1704_new",DAT_0007909c,resp0,resp1);
			applog(3, logbuf, 0);
		}
		ret = 0;
	}
	return ret;
}

// === reset_PIC16F1704_pic_new === 原 FUN_0003df78 @ 0x0003df78
// 来源: reversed/cgminer/13_voltage_pic/FUN_0003df78.c | 分类: 13_voltage_pic
// 语义: 复位 PIC16F1704 自身: 发 6 字节帧 [55 AA 04 07 00 0B] (chk = LEN4+CMD7),
//       等 100ms 回读 [07][01] = 成功; 解锁后再等 1s (PIC 重启)。
//       成功返回 1, 失败按日志门 (级别 3) 打印并返回 0。
uint32_t reset_PIC16F1704_pic_new(void)
{
	uint8_t frame[6];
	char logbuf[0x1000]; /* 原 acStack_1010 */
	uint32_t resp0;      /* 原 param5 */
	uint32_t resp1;      /* 原 param6 */
	uint32_t ret;

	frame[0] = PIC_FRAME_PREAMBLE0;
	frame[1] = PIC_FRAME_PREAMBLE1;
	frame[2] = PIC_LEN_NOARG;                            /* 0x04 */
	frame[3] = PIC_CMD_PIC_RESET;                        /* 0x07 (bm1746_regs.h) */
	frame[4] = 0x00;                                     /* chk_hi = LEN+CMD = 0x0B */
	frame[5] = 0x0b;

	pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
	for (ret = 0; ret < 6; ret++) {
		pic_i2c_write_byte(frame[ret]);
	}
	usleep(100000);                                      /* 100ms — 等 PIC 复位 */
	resp0 = pic_i2c_read_byte();
	resp1 = pic_i2c_read_byte();
	pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
	usleep(1000000);                                     /* 1s — 复位后固定延时 */
	if ((resp0 == PIC_CMD_PIC_RESET) && (resp1 == PIC_RESP_STATUS_OK)) {
		if (LOG_GATE5()) {
			snprintf(logbuf, 0x1000, "%s ok", "reset_PIC16F1704_pic_new");
			applog(5, logbuf, 0);
		}
		ret = 1;
	}
	else {
		if (LOG_GATE3()) {
			snprintf(logbuf, 0x1000,
			         "%s chain %d failed! read_back_data[0] = 0x%02x, read_back_data[1] = 0x%02x\n\n",
			         "reset_PIC16F1704_pic_new",DAT_0007909c,resp0,resp1);
			applog(3, logbuf, 0);
		}
		ret = 0;
	}
	return ret;
}

// === pic_transfer_data_crc16 === 原 FUN_0003e120 @ 0x0003e120
// 来源: reversed/cgminer/13_voltage_pic/FUN_0003e120.c | 分类: 13_voltage_pic
// 语义: 经 PIC I2C 位带通道带和校验地传输一段数据 (寄存器块读写):
//   写 (rw=0): 发 [55 AA (len+6) 26 (slave<<1) reg data×len chk_hi chk_lo],
//     chk = LEN+0x26+(slave<<1)+reg+Σdata, 等 200ms, 解锁返回 0 (不回读)。
//   读 (rw=1): 发 [55 AA 07 26 (slave<<1|1) reg len chk_hi chk_lo], 等 200ms,
//     回读 len+5 字节 [LEN 回显][0x26][01 状态][data×len][chk_hi chk_lo], 校验
//     LEN/命令回显/状态与 chk (chk 寄存器自 TX 阶段累加不清零, 反汇编核实: 值 =
//     TX_chk + LEN + 0x26 + 01 + Σdata) 后把 data 拷回 data 并返回 0; 失败打印
//     "pic read failed ..." / "crc failed ..." 并返回 0xffffffff。
//   逐字节收发均按日志门打印 "write/read pic i2c[i] %02x"。len 过大时读回区/帧区/
//   日志区在原栈上连续, 越界写原样保留。
uint32_t pic_transfer_data_crc16(uint8_t slave, int rw, uint8_t reg, size_t len, uint8_t *data)
{
	/* 原栈连续布局 (偏移自 -0x105C):
	 * [0..3]    local_105c — LEN 域 (写分支 len+6 / 读分支 7)
	 * [7]       bStack_1055 — RX 校验游标起点前 1 字节
	 * [8..43]   local_1054..local_102c — 读回区 rb (与 TX 帧区重叠复用)
	 * [20..51]  local_1048..local_102c — TX 帧区 frame (帧基址 = +20)
	 * [52..4151] acStack_1028 — snprintf 日志缓冲 (0x1004) */
	uint8_t buf[0x1038];
	uint8_t *rb = buf + 8;          /* 读回区 (原 local_1054) */
	uint8_t *frame = buf + 20;      /* TX 帧 (原 local_1048) */
	char *logbuf = (char *)buf + 52;
	uint8_t *p;
	uint32_t i;
	int k;                          /* 原 iVar5/iVar3: 计数 */
	uint32_t cur;                   /* 原 uVar2: 校验游标当前字节 */
	uint32_t chk;                   /* 原 param3: 累加校验和 (TX→RX 不清零) */
	int chk_terms;                  /* 原 iVar5: 校验累加项数 */
	uint32_t total;                 /* 原 local_105c: LEN 域值 */
	int last;                       /* 原 iVar6: 帧尾下标 (发送/校验存放) */

	for (i = 0; i < 8; i++) {
		rb[i] = 0;                  /* 原样预置读回区 (原 local_1054/uStack_1050 整存清零) */
	}
	frame[0] = PIC_FRAME_PREAMBLE0; /* 原样预置帧头 (读分支会重存) */
	frame[1] = PIC_FRAME_PREAMBLE1;
	frame[2] = 0;
	frame[3] = 0x26;                /* PIC 寄存器块传输命令 */
	frame[4] = (uint8_t)(rw | ((uint32_t)slave << 1)); /* I2C 地址: slave<<1 | 读标志(rw=1) */
	frame[5] = reg;                                    /* 寄存器号 */
	for (i = 8; i < 32; i++) {
		frame[i] = 0;               /* 原样预置帧区高位 (原 uStack_1040/103c/1038/1034/1030/102c 清零) */
	}

	if (rw == 0) {                  /* ---- 写分支: 数据跟在 reg 后 ---- */
		chk_terms = (int)len + 4;
		memcpy(frame + 6, data, len);                  /* 数据从帧偏移 6 起 */
		total = len + 6;
		frame[2] = (uint8_t)total;                     /* LEN = 数据长 + 6 */
		cur = total & 0xff;
		last = (int)len + 7;
		if (0 < chk_terms) {
			goto compute_chk;       /* 原 LAB_0003e1e4 */
		}
		frame[len + 6] = 0;         /* 退化分支: (int)(len+4) <= 0 (len 大到 int 溢出), 原样保留 */
		frame[len + 7] = 0;
		pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
		if (last < 0) {
			usleep(200000);
			pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
			return 0;
		}
		chk = 0;
		goto send_phase;
	}
	/* ---- 读分支: [55 AA 07 26 (slave<<1|1) reg len chk chk] ---- */
	cur = 7;
	last = 8;
	chk_terms = 5;
	total = 7;
	frame[2] = 7;
	frame[4] = (uint8_t)(rw | ((uint32_t)slave << 1));
	frame[5] = reg;
	frame[6] = (uint8_t)len;       /* CONCAT12 第 3 字节: 读长度参数 */
compute_chk:
	chk = 0;
	p = frame + 2;
	k = 0;
	while (true) {                 /* chk = Σ frame[2..chk_terms+1] (LEN+CMD+Σ参数) */
		k++;
		chk = (chk + cur) & 0xffff;
		if (chk_terms <= k) {
			break;
		}
		p++;
		cur = *p;
	}
	frame[total] = (uint8_t)(chk >> 8);                /* 16 位和, 大端先高后低 */
	frame[last] = (uint8_t)chk;
	pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
send_phase:
	for (k = 0; k <= last; k++) {  /* 逐字节位带发送整帧 */
		cur = frame[k];
		if (LOG_GATE5()) {
			snprintf(logbuf, 0x1000, "write pic i2c[%d] %02x\n", k, cur);
			applog(5, logbuf, 0);
		}
		pic_i2c_write_byte((uint8_t)cur);
	}
	usleep(200000);                /* 200ms — 等 PIC 处理 */
	if (rw != 1) {
		pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
		return 0;                  /* 写完成 */
	}
	/* ---- 读回 len+5 字节: [LEN 回显][0x26][01][data×len][chk_hi chk_lo] ---- */
	if (-1 < (int)(len + 4)) {
		k = 0;
		do {
			cur = pic_i2c_read_byte();
			rb[k] = (uint8_t)cur;
			if (LOG_GATE5()) {
				snprintf(logbuf, 0x1000, "read pic i2c[%d] %02x\n", k, cur);
				applog(5, logbuf, 0);
			}
			k++;
		} while (k <= (int)(len + 4));
	}
	if (-3 < (int)len) {           /* RX 校验: 游标自读回区前 1 字节 (bStack_1055) 起步 */
		p = buf + 7;               /* 原 &bStack_1055 */
		do {
			p++;
			chk = (chk + *p) & 0xffff;  /* chk 自 TX 阶段继续累加 Σ rb[0..len+2] */
		} while (p != rb + len + 2);
	}
	if (*(uint16_t *)(rb + len + 3) == chk) {          /* chk_hi/chk_lo (LE u16) 对 */
		if (((uint8_t)rb[0] == (uint8_t)total) && (rb[1] == 0x26) && (rb[2] == 1)) {
			memcpy(data, rb + 3, len);                 /* 数据拷回调用方 */
			pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
			return 0;
		}
		if (LOG_GATE5()) {
			snprintf(logbuf, 0x1000, "pic read failed %d %d %d\n",
			         (uint32_t)data[0],(uint32_t)data[1],(uint32_t)data[2]);
			applog(5, logbuf, 0);
		}
	}
	else if (LOG_GATE5()) {
		snprintf(logbuf, 0x1000, "crc failed 0x%x\n", chk);
		applog(5, logbuf, 0);
	}
	pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
	return 0xffffffff;
}

// === set_isl_voltage === 原 FUN_0003ea50 @ 0x0003ea50
// 来源: reversed/cgminer/13_voltage_pic/FUN_0003ea50.c | 分类: 13_voltage_pic
// 语义: 设置单个 ISL 电源芯片输出电压: 电压合法窗 [700,800]mV, 越界回落 700 并按
//       日志门告警; 最多 4 轮: 写 reg 0 = 0xff → 等 100ms → 写 reg 0x21 = 16 位电压
//       (小端 2 字节) → 等 200ms → read_asic_voltage 回读, 回读值落在
//       [电压-4, 电压+4] 即成功返回回读值; 未收敛则打印 "set vol ... set again!"
//       再等 200ms 重试; 4 轮后返回最后一次回读值。
uint32_t set_isl_voltage(uint32_t slave, int voltage)
{
	char logbuf[4100];     /* 原 acStack_1028 */
	uint8_t d1[1];         /* 原 uStack_102d: reg 0 写入数据 */
	uint8_t d2[2];         /* 原 uStack_102c/local_102b: reg 0x21 写入数据 (LE) */
	uint16_t readback = 0; /* 原 uStack_102a */
	uint32_t val;          /* 原 param4 */
	int verify_max;        /* 原 local_1034 */
	uint8_t lo;            /* 原 local_103c */
	uint8_t hi;            /* 原 local_1038 */
	int retry;             /* 原 cVar1 (char) */

	if ((uint32_t)voltage - 700U < 0x65) {             /* [700,800]mV 合法窗 */
		lo = (uint8_t)voltage;
		hi = (uint8_t)((uint32_t)voltage >> 8);
		verify_max = voltage + 4;
	}
	else {
		voltage = VOLT_FALLBACK_MV;                    /* 700 */
		if (LOG_GATE5()) {
			snprintf(logbuf, 0x1000, "illegal voltage [%d, %d], set default voltage %d\n",
			         700,800,700);
			applog(5, logbuf, 0);
		}
		lo = 0xbc;                                     /* 700 = 0x02BC */
		hi = 2;
		verify_max = 0x2c0;                            /* 704 = 700+4 */
	}
	for (retry = 4; retry != 0; retry--) {
		d1[0] = 0xff;
		write_dc_dc((uint8_t)slave, 0, d1, 1);         /* 写 reg 0 = 0xff */
		cgsleep_ms(100);
		d2[0] = lo;
		d2[1] = hi;
		write_dc_dc((uint8_t)slave, 0x21, d2, 2);      /* 写 reg 0x21 = 电压 (16 位 LE) */
		cgsleep_ms(200);
		read_asic_voltage(slave, &readback);           /* 回读 (read_dc_dc reg 0x21, 2 字节) */
		val = (uint32_t)readback;
		if (((int)val <= verify_max) && (voltage - 4 <= (int)val)) {
			return val;                                /* ±4mV 容差内收敛 */
		}
		if (LOG_GATE5()) {
			snprintf(logbuf, 0x1000, "set vol %d, get vol %d, set again!\n", voltage, val);
			applog(5, logbuf, 0);
		}
		cgsleep_ms(200);
	}
	return (uint32_t)readback;
}

// === every_asic_set_isl_voltage === 原 FUN_0003eec8 @ 0x0003eec8
// 来源: reversed/cgminer/13_voltage_pic/FUN_0003eec8.c | 分类: 13_voltage_pic
// 语义: 对一条链的 3 组 I2C 从机逐个设 ISL 电压: 目标电压取自每 ASIC 电压表
//       DAT_000790b8[chain*3+asic] (mV), 从机地址取 DAT_00076b48[asic], 调
//       set_isl_voltage (设+回读 ±4mV); 每组后按日志门打印设定/回读值并等 200ms。
void every_asic_set_isl_voltage(int chain)
{
	char logbuf[4100]; /* 原 acStack_1028 */
	uint16_t vol;
	uint32_t ret;
	int asic;

	for (asic = 0; asic != 3; asic++) {
		vol = DAT_000790b8[chain * 3 + asic];
		ret = set_isl_voltage((uint32_t)(uint8_t)DAT_00076b48[asic], (int)vol);
		if (LOG_GATE5()) {
			snprintf(logbuf, 0x1000, "chain%d asic%d addr %02x, %s set vol %d, get vol %d",
			         chain,asic,(uint32_t)(uint8_t)DAT_00076b48[asic],
			         "every_asic_set_isl_voltage",(uint32_t)vol,ret);
			applog(5, logbuf, 0);
		}
		cgsleep_ms(200);
	}
}

// === show_every_asic_voltage === 原 FUN_0003f254 @ 0x0003f254
// 来源: reversed/cgminer/13_voltage_pic/FUN_0003f254.c | 分类: 13_voltage_pic
// 语义: 遍历打印每 ASIC ISL 电压表 (4 链 × 3 组): "chain %d asic %d vol %d",
//       按日志门 (级别 5) 输出, 无副作用。
void show_every_asic_voltage(void)
{
	char logbuf[4096]; /* 原 acStack_1020 */
	int chain;
	int asic;

	for (chain = 0; chain != 4; chain++) {
		for (asic = 0; asic != 3; asic++) {
			if (LOG_GATE5()) {
				snprintf(logbuf, 0x1000, "chain %d asic %d vol %d\n",
				         chain,asic,(uint32_t)DAT_000790b8[chain * 3 + asic]);
				applog(5, logbuf, 0);
			}
		}
	}
}

// === set_PIC16F1704_voltage_new === 原 FUN_0003f308 @ 0x0003f308
// 来源: reversed/cgminer/13_voltage_pic/FUN_0003f308.c | 分类: 13_voltage_pic
// 语义: 设置 PIC 输出电压命令 (0x10): 发 7 字节帧 [55 AA 05 10 v_lo enc_hi enc_lo]
//       (v_lo = 参数低字节; enc = 参数+0x15 编码值, 大端 2 字节; 本命令无校验字节),
//       等 200ms 回读 [10][01] = 成功; 解锁后再等 200ms。
//       成功返回 1, 失败按日志门 (级别 3) 打印并返回 0。
uint32_t set_PIC16F1704_voltage_new(uint32_t voltage)
{
	uint8_t frame[7];
	char logbuf[4100]; /* 原 acStack_1018 */
	uint32_t resp0;    /* 原 param5 */
	uint32_t resp1;    /* 原 param6 */
	uint32_t ret;

	frame[0] = PIC_FRAME_PREAMBLE0;
	frame[1] = PIC_FRAME_PREAMBLE1;
	frame[2] = 5;                                          /* LEN (无校验字节, 总长 7) */
	frame[3] = PIC_CMD_SET_VOLTAGE;                        /* 0x10 (bm1746_regs.h) */
	frame[4] = (uint8_t)voltage;                           /* 参数低字节 */
	frame[5] = (uint8_t)((voltage + PIC_VOLTAGE_ENCODE_BIAS) >> 8);  /* 编码值 = 参数+0x15, 大端 */
	frame[6] = (uint8_t)(voltage + PIC_VOLTAGE_ENCODE_BIAS);

	pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
	for (ret = 0; ret < 7; ret++) {
		pic_i2c_write_byte(frame[ret]);
	}
	usleep(200000);                                        /* 200ms — 等 PIC 设电压 */
	resp0 = pic_i2c_read_byte();
	resp1 = pic_i2c_read_byte();
	pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
	usleep(200000);                                        /* 200ms — 电压调整后固定延时 */
	if ((resp0 == PIC_CMD_SET_VOLTAGE) && (resp1 == PIC_RESP_STATUS_OK)) {
		if (LOG_GATE5()) {
			snprintf(logbuf, 0x1000, "%s ok, voltage = 0x%02x",
			         "set_PIC16F1704_voltage_new",voltage);
			applog(5, logbuf, 0);
		}
		ret = 1;
	}
	else {
		if (LOG_GATE3()) {
			snprintf(logbuf, 0x1000,
			         "%s chain %d failed! read_back_data[0] = 0x%02x, read_back_data[1] = 0x%02x\n\n",
			         "set_PIC16F1704_voltage_new",DAT_0007909c,resp0,resp1);
			applog(3, logbuf, 0);
		}
		ret = 0;
	}
	return ret;
}

// === write_hash_ID_PIC16F1704_new === 原 FUN_0003f4bc @ 0x0003f4bc
// 来源: reversed/cgminer/13_voltage_pic/FUN_0003f4bc.c | 分类: 13_voltage_pic
// 语义: 写 12 字节 hash ID 到 PIC: 发 18 字节帧 [55 AA 10 12 id×12 chk_hi chk_lo]
//       (chk = LEN 0x10 + CMD 0x12 + Σid, 大端先高后低), 等 200ms 回读 [12][01] = 成功;
//       解锁后再等 200ms。成功返回 1, 失败按日志门 (级别 3) 打印并返回 0。
uint32_t write_hash_ID_PIC16F1704_new(uint8_t *hash_id)
{
	uint8_t frame[18];
	char logbuf[4100]; /* 原 acStack_1010 */
	uint32_t i;
	uint32_t sum;      /* 原 uVar4: 16 位校验和 (低字节取自未截断累加值, 等价) */
	uint32_t resp0;    /* 原 uVar4 复用 */
	uint32_t resp1;    /* 原 uVar5 */
	uint32_t ret;

	sum = PIC_LEN_SEND_DATA + 2 + 0;                       /* 原 0x22 = LEN(0x10)+CMD(0x12) */
	for (i = 0; i < 12; i++) {
		sum = (sum + hash_id[i]) & 0xffff;
	}
	frame[0] = PIC_FRAME_PREAMBLE0;
	frame[1] = PIC_FRAME_PREAMBLE1;
	frame[2] = 0x10;                                       /* LEN = 12 数据 + 4 */
	frame[3] = 0x12;                                       /* CMD: 写 hash ID */
	for (i = 0; i < 12; i++) {
		frame[4 + i] = hash_id[i];
	}
	frame[16] = (uint8_t)(sum >> 8);                       /* chk_hi */
	frame[17] = (uint8_t)sum;                              /* chk_lo */

	pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
	for (i = 0; i < 18; i++) {
		pic_i2c_write_byte(frame[i]);
	}
	usleep(200000);                                        /* 200ms — 等 PIC 写入 */
	resp0 = pic_i2c_read_byte();
	resp1 = pic_i2c_read_byte();
	pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
	usleep(200000);                                        /* 200ms — 写入后固定延时 */
	if ((resp0 == 0x12) && (resp1 == PIC_RESP_STATUS_OK)) {
		if (LOG_GATE5()) {
			snprintf(logbuf, 0x1000, "%s ok", "write_hash_ID_PIC16F1704_new");
			applog(5, logbuf, 0);
		}
		ret = 1;
	}
	else {
		if (LOG_GATE3()) {
			snprintf(logbuf, 0x1000,
			         "%s chain %d failed! read_back_data[0] = 0x%02x, read_back_data[1] = 0x%02x\n\n",
			         "write_hash_ID_PIC16F1704_new",DAT_0007909c,resp0,resp1);
			applog(3, logbuf, 0);
		}
		ret = 0;
	}
	return ret;
}

// === read_hash_id_PIC16F1704_new === 原 FUN_0003f6cc @ 0x0003f6cc
// 来源: reversed/cgminer/13_voltage_pic/FUN_0003f6cc.c | 分类: 13_voltage_pic
// 语义: 读 PIC 内 12 字节 hash ID: 发 6 字节帧 [55 AA 04 13 00 17] (chk = LEN4+CMD13),
//       等 200ms 回读 16 字节 [LEN=0x10][CMD 回显 13][id×12][chk_hi chk_lo]; 日志门开时
//       先打印 16 字节读回原文。LEN/命令回显对且 chk (= 0x23+Σid) 对时把 12 字节
//       拷入 hash_id 并返回 1; CRC 错打印 "failed! crc = 0x%04x" (计算值), 命令错打印
//       "chain %d failed!", 均返回 0。读回区原样预置 local_1030=0xff。
uint8_t read_hash_id_PIC16F1704_new(uint8_t *hash_id)
{
	uint8_t frame[6];
	uint8_t rb[16];      /* 读回区: 原 local_1030/local_102c/local_1028/local_1024 连续 */
	char logbuf[4096];   /* 原 acStack_1020 */
	uint32_t i;
	uint32_t sum;        /* 原 uVar2: 计算校验和 */
	uint32_t sum16;      /* 原 uVar6: 16 位截断 (失败日志打印值 / 链号) */
	uint8_t id0;         /* 原 uVar4: 拷贝游标当前字节 (rb[2] 起) */
	const char *fmt;     /* 原 __format: 失败格式串二选一 */

	frame[0] = PIC_FRAME_PREAMBLE0;
	frame[1] = PIC_FRAME_PREAMBLE1;
	frame[2] = PIC_LEN_NOARG;                            /* 0x04 */
	frame[3] = PIC_CMD_READ_HASH_ID;                     /* 0x13 */
	frame[4] = 0x00;                                     /* chk_hi = LEN+CMD = 0x17 */
	frame[5] = 0x17;
	rb[0] = 0xff;                                        /* 原样预置 local_1030 = 0xff (4B LE) */
	rb[1] = 0;
	rb[2] = 0;
	rb[3] = 0;
	for (i = 4; i < 16; i++) {
		rb[i] = 0;                                       /* 原样预置 local_102c/1028/1024 清零 */
	}

	pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
	for (i = 0; i < 6; i++) {
		pic_i2c_write_byte(frame[i]);
	}
	usleep(200000);                                      /* 200ms — 等 PIC 读 hash ID */
	for (i = 0; i < 16; i++) {
		rb[i] = pic_i2c_read_byte();
	}
	pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
	if (!LOG_GATE5()) {
		if (rb[1] == PIC_CMD_READ_HASH_ID) {
			goto chk_verify;         /* 原 LAB_0003f8f4 */
		}
	}
	else {
		snprintf(logbuf, 0x1000,
		         "%s: read_back_data[0] = 0x%x, read_back_data[1] = 0x%x, read_back_data[2] = 0x%x, read_back_data[3] = 0x%x,\t\tread_back_data[4] = 0x%x, read_back_data[5] = 0x%x, read_back_data[6] = 0x%x, read_back_data[7] = 0x%x,\t\tread_back_data[8] = 0x%x, read_back_data[9] = 0x%x, read_back_data[10] = 0x%x, read_back_data[11] = 0x%x,\t\tread_back_data[12] = 0x%x, read_back_data[13] = 0x%x, read_back_data[14] = 0x%x, read_back_data[15] = 0x%x\n"
		         ,"read_hash_id_PIC16F1704_new",rb[0],(uint32_t)rb[1],(uint32_t)rb[2],(uint32_t)rb[3],
		         rb[4],rb[5],rb[6],rb[7],rb[8],rb[9],rb[10],rb[11],
		         rb[12],rb[13],rb[14],rb[15]);
		applog(5, logbuf, 0);
		if (rb[1] == PIC_CMD_READ_HASH_ID) {
			goto chk_verify;         /* 原 LAB_0003f8f4 */
		}
	}
	if (!LOG_GATE3()) {
		return 0;
	}
	fmt = "%s chain %d failed!\n\n";
	sum16 = (uint32_t)DAT_0007909c;
	goto fail_log;               /* 原 LAB_0003f8b4 */
chk_verify:
	if (rb[0] == 0x10) {
		id0 = rb[2];
		sum = (rb[13] & 0xff) + (rb[12] & 0xff) + rb[11] + (rb[10] & 0xff) +
		      (rb[9] & 0xff) + (rb[8] & 0xff) + rb[7] + (rb[6] & 0xff) +
		      (rb[5] & 0xff) + (rb[4] & 0xff) + id0 + rb[3] + 0x23;
		sum16 = sum & 0xffff;
		if (((rb[14] & 0xff) == (sum16 >> 8)) && (rb[15] == (sum & 0xff))) {
			for (i = 0; i < 12; i++) {               /* hash ID 12 字节 → 调用方 */
				hash_id[i] = id0;
				if (i + 1 < 12) {
					id0 = rb[3 + i];
				}
			}
			if (LOG_GATE5()) {
				snprintf(logbuf, 0x1000, "%s ok", "read_hash_id_PIC16F1704_new");
				applog(5, logbuf, 0);
			}
			return 1;
		}
		if (!LOG_GATE3()) {
			return 0;
		}
		fmt = "%s failed! crc = 0x%04x";
		goto fail_log;           /* 原 LAB_0003f8b4, sum16 = 计算校验和 */
	}
	/* rx[0] != 0x10: 落入公共失败尾 */
	if (!LOG_GATE3()) {
		return 0;
	}
	fmt = "%s chain %d failed!\n\n";
	sum16 = (uint32_t)DAT_0007909c;
fail_log:                        /* 原 LAB_0003f8b4 */
	snprintf(logbuf, 0x1000, fmt, "read_hash_id_PIC16F1704_new", sum16);
	applog(3, logbuf, 0);
	return 0;
}

// === enable_PIC16F1704_dc_dc_new === 原 FUN_0003fa7c @ 0x0003fa7c
// 来源: reversed/cgminer/13_voltage_pic/FUN_0003fa7c.c | 分类: 13_voltage_pic
// 语义: 打开 PIC DC-DC 输出: 发 7 字节帧 [55 AA 05 15 01 00 1B] (CMD 0x15 + 子命令
//       0x1B = 开, 无校验字节), 等 100ms 回读 [15][01] = 成功。
//       成功返回 1, 失败按日志门 (级别 3) 打印 (无结尾换行, 原样) 并返回 0。
uint32_t enable_PIC16F1704_dc_dc_new(void)
{
	uint8_t frame[7];
	char logbuf[4096]; /* 原 acStack_1010 */
	uint32_t resp0;    /* 原 param5 */
	uint32_t resp1;    /* 原 param6 */
	uint32_t ret;

	frame[0] = PIC_FRAME_PREAMBLE0;
	frame[1] = PIC_FRAME_PREAMBLE1;
	frame[2] = 5;                                          /* LEN (无校验字节, 总长 7) */
	frame[3] = PIC_CMD_DC_DC;                              /* 0x15 (bm1746_regs.h) */
	frame[4] = 1;                                          /* 开关标志: 1 = 开 */
	frame[5] = 0;
	frame[6] = PIC_SUB_ENABLE_DC_DC;                       /* 0x1B 子命令 */

	pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
	for (ret = 0; ret < 7; ret++) {
		pic_i2c_write_byte(frame[ret]);
	}
	usleep(100000);                                        /* 100ms — 等 PIC 开 DC-DC */
	resp0 = pic_i2c_read_byte();
	resp1 = pic_i2c_read_byte();
	pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
	if ((resp0 == PIC_CMD_DC_DC) && (resp1 == PIC_RESP_STATUS_OK)) {
		if (LOG_GATE5()) {
			snprintf(logbuf, 0x1000, "%s ok", "enable_PIC16F1704_dc_dc_new");
			applog(5, logbuf, 0);
		}
		ret = 1;
	}
	else {
		if (LOG_GATE3()) {
			snprintf(logbuf, 0x1000,
			         "%s chain %d failed! read_back_data[0] = 0x%02x, read_back_data[1] = 0x%02x",
			         "enable_PIC16F1704_dc_dc_new",DAT_0007909c,resp0,resp1);
			applog(3, logbuf, 0);
		}
		ret = 0;
	}
	return ret;
}

// === disable_PIC16F1704_dc_dc_new === 原 FUN_0003fc24 @ 0x0003fc24
// 来源: reversed/cgminer/13_voltage_pic/FUN_0003fc24.c | 分类: 13_voltage_pic
// 语义: 关闭 PIC DC-DC 输出: 发 7 字节帧 [55 AA 05 15 00 00 1A] (CMD 0x15 + 子命令
//       0x1A = 关, 无校验字节), 等 100ms 回读 [15][01] = 成功。
//       成功返回 1, 失败按日志门 (级别 3) 打印 (无结尾换行, 原样) 并返回 0。
uint32_t disable_PIC16F1704_dc_dc_new(void)
{
	uint8_t frame[7];
	char logbuf[4096]; /* 原 acStack_1010 */
	uint32_t resp0;    /* 原 param5 */
	uint32_t resp1;    /* 原 param6 */
	uint32_t ret;

	frame[0] = PIC_FRAME_PREAMBLE0;
	frame[1] = PIC_FRAME_PREAMBLE1;
	frame[2] = 5;                                          /* LEN (无校验字节, 总长 7) */
	frame[3] = PIC_CMD_DC_DC;                              /* 0x15 (bm1746_regs.h) */
	frame[4] = 0;                                          /* 开关标志: 0 = 关 */
	frame[5] = 0;
	frame[6] = PIC_SUB_DISABLE_DC_DC;                      /* 0x1A 子命令 */

	pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
	for (ret = 0; ret < 7; ret++) {
		pic_i2c_write_byte(frame[ret]);
	}
	usleep(100000);                                        /* 100ms — 等 PIC 关 DC-DC */
	resp0 = pic_i2c_read_byte();
	resp1 = pic_i2c_read_byte();
	pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
	if ((resp0 == PIC_CMD_DC_DC) && (resp1 == PIC_RESP_STATUS_OK)) {
		if (LOG_GATE5()) {
			snprintf(logbuf, 0x1000, "%s ok", "disable_PIC16F1704_dc_dc_new");
			applog(5, logbuf, 0);
		}
		ret = 1;
	}
	else {
		if (LOG_GATE3()) {
			snprintf(logbuf, 0x1000,
			         "%s chain %d failed! read_back_data[0] = 0x%02x, read_back_data[1] = 0x%02x",
			         "disable_PIC16F1704_dc_dc_new",DAT_0007909c,resp0,resp1);
			applog(3, logbuf, 0);
		}
		ret = 0;
	}
	return ret;
}

// === heart_beat_PIC16F1704_new === 原 FUN_0003fdc4 @ 0x0003fdc4
// 来源: reversed/cgminer/13_voltage_pic/FUN_0003fdc4.c | 分类: 13_voltage_pic
// 语义: PIC 心跳轮询: 发 6 字节帧 [55 AA 04 16 00 1A] (chk = LEN4+CMD16), 等 100ms
//       回读 6 字节 (仅时序占用)。Ghidra 将成功分支标记为不可达 (0x3ff94..0x3ffe0
//       移除), 可见尾部恒为: 按日志门 (级别 3) 打印 "chain %d failed!" 及读回原文
//       (前 4 字节为占位常量 0xff,0,0,0, 后 2 字节为实读值) 并返回 0。
uint8_t heart_beat_PIC16F1704_new(void)
{
	uint8_t frame[6];
	uint8_t rb[8];       /* 读回区: 原 local_1020[8] (实读 6 字节) */
	char logbuf[4096];   /* 原 acStack_1010 */
	uint32_t i;          /* 原 uVar5/iVar5 */

	frame[0] = PIC_FRAME_PREAMBLE0;
	frame[1] = PIC_FRAME_PREAMBLE1;
	frame[2] = PIC_LEN_NOARG;                            /* 0x04 */
	frame[3] = PIC_CMD_HEART_BEAT;                       /* 0x16 */
	frame[4] = 0x00;                                     /* chk_hi = LEN+CMD = 0x1A */
	frame[5] = 0x1a;
	rb[4] = 0;                                           /* 原样预置 local_1020[4]/[5] = 0 */
	rb[5] = 0;

	pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
	for (i = 0; i < 6; i++) {
		pic_i2c_write_byte(frame[i]);
	}
	usleep(100000);                                      /* 100ms — 等 PIC 心跳应答 */
	for (i = 0; i < 6; i++) {
		rb[i] = pic_i2c_read_byte();
	}
	pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
	if (LOG_GATE3()) {
		snprintf(logbuf, 0x1000, "%s chain %d failed!",
		         "heart_beat_PIC16F1704_new",DAT_0007909c);
		applog(3, logbuf, 0);
		if (LOG_GATE3()) {
			snprintf(logbuf, 0x1000,
			         "%s: read_back_data[0] = 0x%x, read_back_data[1] = 0x%x, read_back_data[2] = 0x%x, read_back_data[3] = 0x%x, read_back_data[4] = 0x%x, read_back_data[5] = 0x%x\n"
			         ,"heart_beat_PIC16F1704_new",0xff,0,0,0,(uint32_t)rb[4],(uint32_t)rb[5]);
			applog(3, logbuf, 0);
		}
	}
	return 0;
}

// === get_PIC16F1704_software_version_new === 原 FUN_00040128 @ 0x00040128
// 来源: reversed/cgminer/13_voltage_pic/FUN_00040128.c | 分类: 13_voltage_pic
// 语义: 读 PIC 软件版本: 发 6 字节帧 [55 AA 04 17 00 1B] (chk = LEN4+CMD17), 等
//       400ms 回读 5 字节 [LEN=05][CMD 回显 17][ver][chk_hi chk_lo]; LOG_GATE7 开时
//       先按级别 7 打印 5 字节读回原文。LEN/命令回显对且 chk (= ver+0x1C) 对时把
//       版本字节写入 *ver 并返回 1 (成功无日志); CRC 错打印 "failed! crc = 0x%04x",
//       命令错打印 "chain %d failed!" + 读回原文 (级别 3), 均返回 0。
//       读回区原样预置 local_1028=0xff。
uint32_t get_PIC16F1704_software_version_new(uint8_t *ver)
{
	uint8_t frame[6];
	uint8_t rb[5];       /* 读回区: 原 local_1028[4] + local_1024[0] 连续 */
	char logbuf[4100];   /* 原 acStack_1018 */
	uint32_t expect;     /* 原 uVar5: 期望校验和 / 暂存 rx[1] */
	uint8_t cmd_echo;    /* 原 bVar2 = rb[1] */

	frame[0] = PIC_FRAME_PREAMBLE0;
	frame[1] = PIC_FRAME_PREAMBLE1;
	frame[2] = PIC_LEN_NOARG;                            /* 0x04 */
	frame[3] = PIC_CMD_GET_SW_VERSION;                   /* 0x17 */
	frame[4] = 0x00;                                     /* chk_hi = LEN+CMD = 0x1B */
	frame[5] = 0x1b;
	rb[0] = 0xff;                                        /* 原样预置 local_1028 = 0xff (4B LE) */
	rb[1] = 0;
	rb[2] = 0;
	rb[3] = 0;
	rb[4] = 0;                                           /* 原样预置 local_1024[0] = 0 */

	pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
	for (expect = 0; expect < 6; expect++) {
		pic_i2c_write_byte(frame[expect]);
	}
	usleep(400000);                                      /* 400ms — 等 PIC 应答 */
	for (expect = 0; expect < 5; expect++) {
		rb[expect] = pic_i2c_read_byte();
	}
	pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
	cmd_echo = rb[1];
	if ((DAT_00078e09 == '\0') ||
	    ((DAT_0007ad48 == '\0' && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7))) {
		if (cmd_echo == PIC_CMD_GET_SW_VERSION) {
			goto chk_verify;         /* 原 LAB_0004027c */
		}
	}
	else {
		snprintf(logbuf, 0x1000,
		         "%s: read_back_data[0] = 0x%x, read_back_data[1] = 0x%x, read_back_data[2] = 0x%x, read_back_data[3] = 0x%x, read_back_data[4] = 0x%x"
		         ,"get_PIC16F1704_software_version_new",rb[0],(uint32_t)cmd_echo,
		         (uint32_t)rb[2],(uint32_t)rb[3],(uint32_t)rb[4]);
		applog(7, logbuf, 0);
		if (cmd_echo == PIC_CMD_GET_SW_VERSION) {
			goto chk_verify;         /* 原 LAB_0004027c */
		}
	}
	/* ---- 公共失败尾 ---- */
	if (!LOG_GATE3()) {
		return 0;
	}
	snprintf(logbuf, 0x1000, "%s chain %d failed!",
	         "get_PIC16F1704_software_version_new",DAT_0007909c);
	applog(3, logbuf, 0);
	if (!LOG_GATE3()) {
		return 0;
	}
	snprintf(logbuf, 0x1000,
	         "%s: read_back_data[0] = 0x%x, read_back_data[1] = 0x%x, read_back_data[2] = 0x%x, read_back_data[3] = 0x%x, read_back_data[4] = 0x%x"
	         ,"get_PIC16F1704_software_version_new",rb[0],(uint32_t)cmd_echo,
	         (uint32_t)rb[2],(uint32_t)rb[3],(uint32_t)rb[4]);
	/* 原 LAB_000403c8 */
	applog(3, logbuf, 0);
	return 0;
chk_verify:
	if (rb[0] == 5) {
		expect = rb[2] + 0x1c;                           /* 期望 chk = ver + LEN(5)+CMD(0x17) */
		if ((rb[3] == (expect >> 8)) && ((uint32_t)rb[4] == (expect & 0xff))) {
			*ver = rb[2];
			return 1;                                    /* 成功 (原样无日志) */
		}
		if (!LOG_GATE3()) {
			return 0;
		}
		snprintf(logbuf, 0x1000, "%s failed! crc = 0x%04x",
		         "get_PIC16F1704_software_version_new",expect);
		applog(3, logbuf, 0);                            /* 原 LAB_000403c8 */
		return 0;
	}
	/* rx[0] != 5: 落入公共失败尾 */
	if (!LOG_GATE3()) {
		return 0;
	}
	snprintf(logbuf, 0x1000, "%s chain %d failed!",
	         "get_PIC16F1704_software_version_new",DAT_0007909c);
	applog(3, logbuf, 0);
	if (!LOG_GATE3()) {
		return 0;
	}
	snprintf(logbuf, 0x1000,
	         "%s: read_back_data[0] = 0x%x, read_back_data[1] = 0x%x, read_back_data[2] = 0x%x, read_back_data[3] = 0x%x, read_back_data[4] = 0x%x"
	         ,"get_PIC16F1704_software_version_new",rb[0],(uint32_t)cmd_echo,
	         (uint32_t)rb[2],(uint32_t)rb[3],(uint32_t)rb[4]);
	applog(3, logbuf, 0);
	return 0;
}

// === get_every_chain_pic_version === 原 FUN_00040454 @ 0x00040454
// 来源: reversed/cgminer/13_voltage_pic/FUN_00040454.c | 分类: 13_voltage_pic
// 语义: 对所有在位链 (链存在标志 DAT_0009d4bc[chain]==1) 逐个取 PIC 软件版本:
//       持链互斥锁 DAT_000790d0 置当前链号 DAT_0007909c, 调
//       get_PIC16F1704_software_version_new 存入版本数组 versions[chain], 等 100ms;
//       成功/失败分别按日志门 (级别 5) 打印 "chain%d pic version %02x" /
//       "chain%d pic version read failed"。
void get_every_chain_pic_version(void)
{
	char *p_exist;       /* 原 pcVar2: 从 &DAT_0009d4bb 起步先 ++ 后判 — 即 DAT_0009d4bc[chain] */
	uint8_t versions[4]; /* 原 abStack_1024[0..3] (与日志缓冲在原栈上连续) */
	char logbuf[0x1000]; /* 原 abStack_1024 + 4 */
	int chain;
	uint32_t ret;

	p_exist = &DAT_0009d4bb[0];
	versions[0] = 0;
	versions[1] = 0;
	versions[2] = 0;
	versions[3] = 0;
	for (chain = 0; chain != 4; chain++) {
		p_exist++;
		if (*p_exist == 1) {         /* 链在位 */
			pthread_mutex_lock((pthread_mutex_t *)DAT_000790d0);
			DAT_0007909c = chain;
			ret = get_PIC16F1704_software_version_new(&versions[chain]);
			cgsleep_ms(100);
			pthread_mutex_unlock((pthread_mutex_t *)DAT_000790d0);
			if (ret == 1) {
				if (LOG_GATE5()) {
					snprintf(logbuf, 0x1000, "chain%d pic version %02x\n", chain,
					         (uint32_t)versions[chain]);
					applog(5, logbuf, 0);
				}
			}
			else if (LOG_GATE5()) {
				snprintf(logbuf, 0x1000, "chain%d pic version read failed\n", chain);
				applog(5, logbuf, 0);
			}
		}
	}
}

// === get_PIC16F1704_voltage_new === 原 FUN_000405ac @ 0x000405ac
// 来源: reversed/cgminer/13_voltage_pic/FUN_000405ac.c | 分类: 13_voltage_pic
// 语义: 读 PIC 当前输出电压: 发 6 字节帧 [55 AA 04 18 00 1C] (chk = LEN4+CMD18), 等
//       200ms 回读 5 字节 [LEN=05][CMD 回显 18][vol][chk_hi chk_lo]; 日志门 5 开时先
//       打印 5 字节读回原文 (带 \n)。LEN/命令回显对且 chk (= vol+0x1D) 对时把电压
//       字节写入 *voltage 并返回 1 (成功按日志门打印 "ok, voltage = 0x%02x");
//       CRC 错打印 "failed! crc = 0x%04x", 命令错打印 "chain %d failed!", 均返回 0。
//       读回区原样预置 local_1028=0xff。
uint8_t get_PIC16F1704_voltage_new(uint8_t *voltage)
{
	uint8_t frame[6];
	uint8_t rb[5];       /* 读回区: 原 local_1028[4] + local_1024[0] 连续 */
	char logbuf[4100];   /* 原 acStack_1018 */
	uint32_t i;
	uint32_t expect;     /* 原 uVar5: 期望校验和 / 暂存 rx[1] / 链号 */
	const char *fmt;     /* 原 __format */

	frame[0] = PIC_FRAME_PREAMBLE0;
	frame[1] = PIC_FRAME_PREAMBLE1;
	frame[2] = PIC_LEN_NOARG;                            /* 0x04 */
	frame[3] = 0x18;                                     /* CMD: 读 PIC 电压 */
	frame[4] = 0x00;                                     /* chk_hi = LEN+CMD = 0x1C */
	frame[5] = 0x1c;
	rb[0] = 0xff;                                        /* 原样预置 local_1028 = 0xff (4B LE) */
	rb[1] = 0;
	rb[2] = 0;
	rb[3] = 0;
	rb[4] = 0;                                           /* 原样预置 local_1024[0] = 0 */

	pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
	for (i = 0; i < 6; i++) {
		pic_i2c_write_byte(frame[i]);
	}
	usleep(200000);                                      /* 200ms — 等 PIC 应答 */
	for (i = 0; i < 5; i++) {
		rb[i] = pic_i2c_read_byte();
	}
	pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
	if (!LOG_GATE5()) {
		if (rb[1] == 0x18) {
			goto chk_verify;         /* 原 LAB_00040778 */
		}
	}
	else {
		snprintf(logbuf, 0x1000,
		         "%s: read_back_data[0] = 0x%x, read_back_data[1] = 0x%x, read_back_data[2] = 0x%x, read_back_data[3] = 0x%x, read_back_data[4] = 0x%x\n"
		         ,"get_PIC16F1704_voltage_new",rb[0],(uint32_t)rb[1],
		         (uint32_t)rb[2],(uint32_t)rb[3],(uint32_t)rb[4]);
		applog(5, logbuf, 0);
		if (rb[1] == 0x18) {
			goto chk_verify;         /* 原 LAB_00040778 */
		}
	}
	if (!LOG_GATE3()) {
		return 0;
	}
	fmt = "%s chain %d failed!";
	expect = (uint32_t)DAT_0007909c;
	goto fail_log;               /* 原 LAB_00040738 */
chk_verify:
	if (rb[0] == 5) {
		expect = rb[2] + 0x1d;                           /* 期望 chk = vol + LEN(5)+CMD(0x18) */
		if ((rb[3] == (expect >> 8)) && ((uint32_t)rb[4] == (expect & 0xff))) {
			*voltage = rb[2];
			if (!LOG_GATE5()) {
				return 1;
			}
			snprintf(logbuf, 0x1000, "%s ok, voltage = 0x%02x",
			         "get_PIC16F1704_voltage_new",(uint32_t)rb[2]);
			applog(5, logbuf, 0);
			return 1;
		}
		if (!LOG_GATE3()) {
			return 0;
		}
		fmt = "%s failed! crc = 0x%04x";
		goto fail_log;           /* 原 LAB_00040738, expect = 计算校验和 */
	}
	/* rx[0] != 5: 落入公共失败尾 */
	if (!LOG_GATE3()) {
		return 0;
	}
	fmt = "%s chain %d failed!";
	expect = (uint32_t)DAT_0007909c;
fail_log:                        /* 原 LAB_00040738 */
	snprintf(logbuf, 0x1000, fmt, "get_PIC16F1704_voltage_new", expect);
	applog(3, logbuf, 0);
	return 0;
}

// === write_temperature_offset_PIC16F1704_new === 原 FUN_00040888 @ 0x00040888
// 来源: reversed/cgminer/13_voltage_pic/FUN_00040888.c | 分类: 13_voltage_pic
// 语义: 写 8 字节温度偏移表到 PIC: 发 14 字节帧 [55 AA 0C 22 data×8 chk_hi chk_lo]
//       (chk = LEN 0xC + CMD 0x22 + Σdata, 大端先高后低; 帧头字节存于原码内联
//       strb 序列, 反汇编 0x408e4..0x40924 证实), 等 200ms 回读 [22][01] = 成功;
//       解锁后再等 200ms。成功返回 1, 失败按日志门 (级别 3) 打印 (无链号/换行,
//       原样) 并返回 0。
uint32_t write_temperature_offset_PIC16F1704_new(uint8_t *offset_data)
{
	uint8_t frame[14];
	char logbuf[4096]; /* 原 acStack_1010 */
	uint32_t i;
	uint32_t sum;      /* 原 uVar4: 16 位校验和 */
	uint32_t resp0;    /* 原 uVar4 复用 */
	uint32_t resp1;    /* 原 uVar5 */
	uint32_t ret;

	sum = 0x2e;                                            /* 原 0x2e = LEN(0xC)+CMD(0x22) */
	for (i = 0; i < 8; i++) {
		sum = (sum + offset_data[i]) & 0xffff;
	}
	frame[0] = PIC_FRAME_PREAMBLE0;
	frame[1] = PIC_FRAME_PREAMBLE1;
	frame[2] = 0x0c;                                       /* LEN = 8 数据 + 4 */
	frame[3] = 0x22;                                       /* CMD: 写温度偏移 */
	for (i = 0; i < 8; i++) {
		frame[4 + i] = offset_data[i];
	}
	frame[12] = (uint8_t)(sum >> 8);                       /* chk_hi */
	frame[13] = (uint8_t)sum;                              /* chk_lo */

	pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
	for (i = 0; i < 14; i++) {
		pic_i2c_write_byte(frame[i]);
	}
	usleep(200000);                                        /* 200ms — 等 PIC 写入 */
	resp0 = pic_i2c_read_byte();
	resp1 = pic_i2c_read_byte();
	pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
	usleep(200000);                                        /* 200ms — 写入后固定延时 */
	if ((resp0 == 0x22) && (resp1 == PIC_RESP_STATUS_OK)) {
		if (LOG_GATE5()) {
			snprintf(logbuf, 0x1000, "%s ok", "write_temperature_offset_PIC16F1704_new");
			applog(5, logbuf, 0);
		}
		ret = 1;
	}
	else {
		if (LOG_GATE3()) {
			snprintf(logbuf, 0x1000,
			         "%s failed! read_back_data[0] = 0x%02x, read_back_data[1] = 0x%02x"
			         ,"write_temperature_offset_PIC16F1704_new",resp0,resp1);
			applog(3, logbuf, 0);
		}
		ret = 0;
	}
	return ret;
}

// === read_temperature_offset_PIC16F1704_new === 原 FUN_00040a84 @ 0x00040a84
// 来源: reversed/cgminer/13_voltage_pic/FUN_00040a84.c | 分类: 13_voltage_pic
// 语义: 读 PIC 内 8 字节温度偏移表: 发 6 字节帧 [55 AA 04 23 00 27] (chk = LEN4+CMD23),
//       等 200ms 回读 12 字节 [LEN=0x0C][CMD 回显 23][data×8][chk_hi chk_lo]; 日志门 5
//       开时先打印 12 字节读回原文。LEN/命令回显对且 chk (= 0x2F+Σdata) 对时把
//       8 字节拷入 offset 并返回 1 (成功按日志门打印 ok); CRC 错打印
//       "failed! crc = 0x%04x", 命令错打印 "%s failed!", 均返回 0。
//       读回区原样预置 local_102c=0xff, 其余清零。
uint8_t read_temperature_offset_PIC16F1704_new(uint8_t *offset)
{
	uint8_t frame[6];
	uint8_t rb[12];      /* 读回区: 原 local_102c/local_1028/local_1024 连续 */
	char logbuf[4096];   /* 原 acStack_1020 */
	uint32_t i;
	uint32_t sum;        /* 原 uVar5: 校验和 */
	uint8_t d0;          /* 原 uVar3: 拷贝游标当前字节 (rb[2] 起) */

	frame[0] = PIC_FRAME_PREAMBLE0;
	frame[1] = PIC_FRAME_PREAMBLE1;
	frame[2] = PIC_LEN_NOARG;                            /* 0x04 */
	frame[3] = 0x23;                                     /* CMD: 读温度偏移 */
	frame[4] = 0x00;                                     /* chk_hi = LEN+CMD = 0x27 */
	frame[5] = 0x27;
	rb[0] = 0xff;                                        /* 原样预置 local_102c = 0xff (4B LE) */
	rb[1] = 0;
	rb[2] = 0;
	rb[3] = 0;
	rb[4] = 0;                                           /* 原样预置 local_1028/local_1024 = 0 */
	for (i = 5; i < 12; i++) {
		rb[i] = 0;
	}

	pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
	for (i = 0; i < 6; i++) {
		pic_i2c_write_byte(frame[i]);
	}
	usleep(200000);                                      /* 200ms — 等 PIC 读偏移 */
	for (i = 0; i < 12; i++) {
		rb[i] = pic_i2c_read_byte();
	}
	pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
	if (!LOG_GATE5()) {
		if (rb[1] == 0x23) {
			goto chk_verify;         /* 原 LAB_00040c88 */
		}
	}
	else {
		snprintf(logbuf, 0x1000,
		         "%s: read_back_data[0] = 0x%x, read_back_data[1] = 0x%x, read_back_data[2] = 0x%x, read_back_data[3] = 0x%x, \t\tread_back_data[4] = 0x%x, read_back_data[5] = 0x%x, read_back_data[6] = 0x%x, read_back_data[7] = 0x%x, \t\tread_back_data[8] = 0x%x, read_back_data[9] = 0x%x, read_back_data[10] = 0x%x, read_back_data[11] = 0x%x\n"
		         ,"read_temperature_offset_PIC16F1704_new",rb[0],(uint32_t)rb[1],
		         (uint32_t)rb[2],(uint32_t)rb[3],rb[4],rb[5],rb[6],rb[7],
		         rb[8],rb[9],rb[10],rb[11]);
		applog(5, logbuf, 0);
		if (rb[1] == 0x23) {
			goto chk_verify;         /* 原 LAB_00040c88 */
		}
	}
	if (!LOG_GATE3()) {
		return 0;
	}
	snprintf(logbuf, 0x1000, "%s failed!", "read_temperature_offset_PIC16F1704_new");
	goto fail_log;               /* 原 LAB_00040c4c */
chk_verify:
	if (rb[0] == 0x0c) {
		d0 = rb[2];
		sum = (rb[9] & 0xff) + (rb[8] & 0xff) + rb[7] + (rb[6] & 0xff) +
		      (rb[5] & 0xff) + (rb[4] & 0xff) + d0 + rb[3] + 0x2f;
		if (((rb[10] & 0xff) == (sum >> 8)) && (rb[11] == (sum & 0xff))) {
			for (i = 0; i < 8; i++) {                /* 偏移 8 字节 → 调用方 */
				offset[i] = d0;
				if (i + 1 < 8) {
					d0 = rb[3 + i];
				}
			}
			if (LOG_GATE5()) {
				snprintf(logbuf, 0x1000, "%s ok", "read_temperature_offset_PIC16F1704_new");
				applog(5, logbuf, 0);
			}
			return 1;
		}
		if (!LOG_GATE3()) {
			return 0;
		}
		snprintf(logbuf, 0x1000, "%s failed! crc = 0x%04x",
		         "read_temperature_offset_PIC16F1704_new",sum);
		goto fail_log;           /* 原 LAB_00040c4c */
	}
	/* rx[0] != 0x0C: 落入公共失败尾 */
	if (!LOG_GATE3()) {
		return 0;
	}
	snprintf(logbuf, 0x1000, "%s failed!", "read_temperature_offset_PIC16F1704_new");
fail_log:                        /* 原 LAB_00040c4c */
	applog(3, logbuf, 0);
	return 0;
}

// === erase_PIC16F1704_app_flash_new === 原 FUN_00040dec @ 0x00040dec
// 来源: reversed/cgminer/13_voltage_pic/FUN_00040dec.c | 分类: 13_voltage_pic
// 语义: 擦除 PIC app 区: 先 set_PIC16F1704_flash_pointer_new(6, 0) (app 起始指针),
//       按日志门打印 "erase_loop = 76", 然后连调 76 次 erase_PIC16F1704_flash_new
//       (PIC 每次擦一页)。Ghidra 误标 void, r0 实有返回 = 末次 erase 调用结果
//       (PIC1704_update_pic_app_program_new 以 ==0 判失败)。
uint32_t erase_PIC16F1704_app_flash_new(void)
{
	char logbuf[4096];   /* 原 acStack_1008 */
	int i;               /* 原 iVar1 */
	uint32_t ret;        /* 原 r0 返回值 (末次 erase 结果) */

	set_PIC16F1704_flash_pointer_new(6, 0);
	if (LOG_GATE5()) {
		snprintf(logbuf, 0x1000, "%s: erase_loop = %d\n",
		         "erase_PIC16F1704_app_flash_new",0x4c);
		applog(5, logbuf, 0);
	}
	for (i = 0x4c; i != 0; i--) {
		ret = erase_PIC16F1704_flash_new();              /* 76 页逐页擦除 */
	}
	return ret;
}

// === PIC1704_update_pic_app_program_new === 原 FUN_00040e94 @ 0x00040e94
// 来源: reversed/cgminer/13_voltage_pic/FUN_00040e94.c | 分类: 13_voltage_pic
// 语义: PIC app 固件升级: 从 /sbin/pic.txt 逐行读 16 进制字 (strtoul base 16, 大端
//       半字) 填 0x980 半字 (0x1300 字节) 固件映像 (原栈连续区: fgets 行缓冲 8B +
//       16B 页缓冲 + 0x1000 日志缓冲 + 5000B 映像, fgets 0x3ff 长行越界原样保留);
//       复位 PIC → 擦 app (76 页) → 设指针 (6,0) → 逐 16 字节页 send_data (0x02) +
//       write_into_flash (0x05) 共 0x130 页 → 再次复位, 全部成功返回 1。
//       任一步失败: 按日志门打印相应错误后返回 0 (原码 fopen 失败路径把 void 的
//       applog r0 残留值作返回值, 不可依赖, 按 0 转写)。
uint32_t PIC1704_update_pic_app_program_new(void)
{
	/* 原栈连续布局 (偏移自 -0x23C8):
	 * [0..7]      acStack_23c8 — fgets 行缓冲 (8B; fgets 0x3ff 越界写原样保留)
	 * [8..23]     local_23c0 — 16 字节发送页缓冲
	 * [24..4119]  local_23b0..uStack_2390/local_238e — snprintf 日志缓冲 (0x1000)
	 * [0x1018..0x1FDB] local_13b0/local_13ac — 固件映像 (memset 5000B, 实用 0x1300B) */
	uint8_t buf[0x23a0];
	char *line = (char *)buf;              /* 原 acStack_23c8 */
	uint8_t *page = buf + 8;               /* 原 local_23c0 */
	char *logbuf = (char *)buf + 24;       /* 原 local_23b0 */
	uint8_t *image = buf + 0x1018;         /* 原 local_13b0 */
	uint8_t *image_ptr;                    /* 原 puVar4: 半字写入游标 */
	FILE *fp;
	uint32_t i;
	int k;                                 /* 原 iVar2: 页号 / 复用 */
	uint32_t val;                          /* 原 uVar1: strtoul 结果 */
	uint32_t ret;
	const char *fmt;                       /* 原 __format */

	memset(image, 0, 5000);
	page[0] = 0;                           /* 原样清零 local_23c0[0..3] (16B 页缓冲) */
	page[1] = 0;
	page[2] = 0;
	page[3] = 0;
	page[4] = 0;
	page[5] = 0;
	page[6] = 0;
	page[7] = 0;
	page[8] = 0;
	page[9] = 0;
	page[10] = 0;
	page[11] = 0;
	page[12] = 0;
	page[13] = 0;
	page[14] = 0;
	page[15] = 0;
	line[0] = 0;                           /* 原样清零 acStack_23c8[0..4] */
	line[1] = 0;
	line[2] = 0;
	line[3] = 0;
	line[4] = 0;
	if (LOG_GATE5()) {
		/* 原码逐字节把函数名存入日志缓冲后直接作为消息打印, 此处等价于字面量 */
		applog(5, "PIC1704_update_pic_app_program_new", 0);
	}
	fp = fopen("/sbin/pic.txt","r");
	if (fp == NULL) {
		if (!LOG_GATE3()) {
			return 0;
		}
		snprintf(logbuf, 0x1000, "%s: open pic16f1704_app_new.txt failed\n",
		         "PIC1704_update_pic_app_program_new");
		applog(3, logbuf, 0);
		return 0;    /* 原码返回 applog (void) 的 r0 残留值 (Ghidra uVar3), 按 0 转写 */
	}
	fseek(fp,0,0);
	memset(image, 0, 5000);
	if (LOG_GATE5()) {
		snprintf(logbuf, 0x1000, "%s: pic_flash_length = %d\n",
		         "PIC1704_update_pic_app_program_new",0x980);
		applog(5, logbuf, 0);
	}
	image_ptr = image;
	do {                                   /* 逐行读 16 进制字, 大端半字存入映像 (共 0x980 个) */
		fgets(line,0x3ff,fp);
		val = strtoul(line,(char **)0x0,0x10);
		image_ptr[1] = (uint8_t)val;                       /* 低字节在后 */
		image_ptr[0] = (uint8_t)(val >> 8);                /* 高字节在前 (大端) */
		image_ptr += 2;
	} while (image_ptr != image + 0x1300);
	fclose(fp);
	ret = reset_PIC16F1704_pic_new();
	if (ret != 0) {
		ret = erase_PIC16F1704_app_flash_new();
		if (ret == 0) {
			if (!LOG_GATE3()) {
				return 0;
			}
			fmt = "%s: erase flash error!\n\n";
			goto fail_log;       /* 原 LAB_00041074 */
		}
		ret = set_PIC16F1704_flash_pointer_new(6, 0);
		if (ret == 0) {
			if (!LOG_GATE3()) {
				return 0;
			}
			fmt = "%s: set flash pointer error!\n\n";
			goto fail_log;       /* 原 LAB_00041074 */
		}
		for (k = 0; k != 0x130; k++) {
			page[0] = image[k * 16 + 0];                   /* 原 local_13ac[4k-1..4k+2] 四字拷贝 */
			page[1] = image[k * 16 + 1];
			page[2] = image[k * 16 + 2];
			page[3] = image[k * 16 + 3];
			page[4] = image[k * 16 + 4];
			page[5] = image[k * 16 + 5];
			page[6] = image[k * 16 + 6];
			page[7] = image[k * 16 + 7];
			page[8] = image[k * 16 + 8];
			page[9] = image[k * 16 + 9];
			page[10] = image[k * 16 + 10];
			page[11] = image[k * 16 + 11];
			page[12] = image[k * 16 + 12];
			page[13] = image[k * 16 + 13];
			page[14] = image[k * 16 + 14];
			page[15] = image[k * 16 + 15];
			if (LOG_GATE5()) {
				snprintf(logbuf, 0x1000, "send pic program time: %d", k);
				applog(5, logbuf, 0);
			}
			for (i = 0; i != 0x10; i++) {
				if (LOG_GATE7()) {
					snprintf(logbuf, 0x1000, "buf[%d] = 0x%02x", i, (uint32_t)page[i]);
					applog(7, logbuf, 0);
				}
			}
			send_data_to_PIC16F1704_new(page);
			write_data_into_PIC16F1704_flash_new();
		}
		ret = reset_PIC16F1704_pic_new();
		if (ret != 0) {
			return 1;
		}
	}
	if (!LOG_GATE3()) {
		return 0;
	}
	fmt = "%s: reset pic error!\n\n";
fail_log:                        /* 原 LAB_00041074 */
	snprintf(logbuf, 0x1000, fmt, "PIC1704_update_pic_app_program_new");
	applog(3, logbuf, 0);
	return 0;
}

// === every_chain_reset_PIC16F1704_pic_new === 原 FUN_00041308 @ 0x00041308
// 来源: reversed/cgminer/13_voltage_pic/FUN_00041308.c | 分类: 13_voltage_pic
// 语义: 对所有在位链逐个复位 PIC: 按日志门 (级别 5) 打印 "chain%d every_chain_...",
//       持链互斥锁 DAT_000790d0 置当前链号, 调 reset_PIC16F1704_pic_new, 等 100ms;
//       全链完成后等 500ms。无返回值。
void every_chain_reset_PIC16F1704_pic_new(void)
{
	char *p_exist;       /* 原 pcVar2: 从 &DAT_0009d4bb 起步先 ++ 后判 — 即 DAT_0009d4bc[chain] */
	char logbuf[4100];   /* 原 acStack_1020 */
	int chain;

	p_exist = &DAT_0009d4bb[0];
	for (chain = 0; chain != 4; chain++) {
		p_exist++;
		if (*p_exist == 1) {         /* 链在位 */
			if (LOG_GATE5()) {
				snprintf(logbuf, 0x1000, "chain%d %s", chain,
				         "every_chain_reset_PIC16F1704_pic_new");
				applog(5, logbuf, 0);
			}
			pthread_mutex_lock((pthread_mutex_t *)DAT_000790d0);
			DAT_0007909c = chain;
			reset_PIC16F1704_pic_new();
			cgsleep_ms(100);
			pthread_mutex_unlock((pthread_mutex_t *)DAT_000790d0);
		}
	}
	cgsleep_ms(500);
}

// === every_chain_jump_from_loader_to_app_PIC16F1704_new === 原 FUN_000413f0 @ 0x000413f0
// 来源: reversed/cgminer/13_voltage_pic/FUN_000413f0.c | 分类: 13_voltage_pic
// 语义: 对所有在位链逐个令 PIC 跳转 app: 按日志门打印 "chain%d every_chain_...",
//       持链互斥锁置链号, 调 jump_from_loader_to_app_PIC16F1704_new, 等 100ms;
//       全链完成后等 500ms。无返回值。
void every_chain_jump_from_loader_to_app_PIC16F1704_new(void)
{
	char *p_exist;
	char logbuf[4100];   /* 原 acStack_1020 */
	int chain;

	p_exist = &DAT_0009d4bb[0];
	for (chain = 0; chain != 4; chain++) {
		p_exist++;
		if (*p_exist == 1) {         /* 链在位 */
			if (LOG_GATE5()) {
				snprintf(logbuf, 0x1000, "chain%d %s", chain,
				         "every_chain_jump_from_loader_to_app_PIC16F1704_new");
				applog(5, logbuf, 0);
			}
			pthread_mutex_lock((pthread_mutex_t *)DAT_000790d0);
			DAT_0007909c = chain;
			jump_from_loader_to_app_PIC16F1704_new();
			cgsleep_ms(100);
			pthread_mutex_unlock((pthread_mutex_t *)DAT_000790d0);
		}
	}
	cgsleep_ms(500);
}

// === every_chain_disable_PIC16F1704_dc_dc_new === 原 FUN_000414d8 @ 0x000414d8
// 来源: reversed/cgminer/13_voltage_pic/FUN_000414d8.c | 分类: 13_voltage_pic
// 语义: 对所有在位链逐个关闭 DC-DC: 先按日志门打印函数名 (原码逐字节拼入栈缓冲),
//       然后持链互斥锁置链号, 调 disable_PIC16F1704_dc_dc_new, 等 100ms;
//       全链完成后等 500ms。无返回值。
void every_chain_disable_PIC16F1704_dc_dc_new(void)
{
	char *p_exist;
	int chain;

	if (LOG_GATE5()) {
		/* 原码逐字节把函数名存入栈缓冲后直接作为消息打印, 此处等价于字面量 */
		applog(5, "every_chain_disable_PIC16F1704_dc_dc_new", 0);
	}
	p_exist = &DAT_0009d4bb[0];
	for (chain = 0; chain != 4; chain++) {
		p_exist++;
		if (*p_exist == 1) {         /* 链在位 */
			pthread_mutex_lock((pthread_mutex_t *)DAT_000790d0);
			DAT_0007909c = chain;
			disable_PIC16F1704_dc_dc_new();
			cgsleep_ms(100);
			pthread_mutex_unlock((pthread_mutex_t *)DAT_000790d0);
		}
	}
	cgsleep_ms(500);
}

// === every_chain_enable_PIC16F1704_dc_dc_new === 原 FUN_00041614 @ 0x00041614
// 来源: reversed/cgminer/13_voltage_pic/FUN_00041614.c | 分类: 13_voltage_pic
// 语义: 对所有在位链逐个打开 DC-DC: 先按日志门打印函数名 (原码逐字节拼入栈缓冲),
//       然后持链互斥锁置链号, 调 enable_PIC16F1704_dc_dc_new, 等 200ms;
//       无收尾延时 (与 disable 版差异, 原样)。
void every_chain_enable_PIC16F1704_dc_dc_new(void)
{
	char *p_exist;
	int chain;

	if (LOG_GATE5()) {
		/* 原码逐字节把函数名存入栈缓冲后直接作为消息打印, 此处等价于字面量 */
		applog(5, "every_chain_enable_PIC16F1704_dc_dc_new", 0);
	}
	p_exist = &DAT_0009d4bb[0];
	for (chain = 0; chain != 4; chain++) {
		p_exist++;
		if (*p_exist == 1) {         /* 链在位 */
			pthread_mutex_lock((pthread_mutex_t *)DAT_000790d0);
			DAT_0007909c = chain;
			enable_PIC16F1704_dc_dc_new();
			cgsleep_ms(200);
			pthread_mutex_unlock((pthread_mutex_t *)DAT_000790d0);
		}
	}
}

// === send_heart_beat_to_every_chain === 原 FUN_000416ec @ 0x000416ec
// 来源: reversed/cgminer/13_voltage_pic/FUN_000416ec.c | 分类: 13_voltage_pic
// 语义: 启动 PIC 心跳后台线程: calloc 一个 0x40 字节 thr_info 结构存入 DAT_0009d4ac,
//       thr_info_create 创建线程 (线程体 fun_00040034, 原日志串
//       "pic_heart_beat_func"), 成功则 pthread_detach 分离并返回其返回值;
//       失败按日志门 (级别 3) 打印 "create thread error for pic_heart_beat_func"
//       并返回 -3。
int send_heart_beat_to_every_chain(void)
{
	char logbuf[4096];   /* 原 acStack_1008 */
	int ret;

	DAT_0009d4ac = calloc(1, 0x40);
	ret = thr_info_create(DAT_0009d4ac, 0, fun_00040034, DAT_0009d4ac);
	if (ret == 0) {
		ret = pthread_detach(*(pthread_t *)((uint8_t *)DAT_0009d4ac + 0xc));
	}
	else {
		if (LOG_GATE3()) {
			snprintf(logbuf, 0x1000, "%s: create thread error for pic_heart_beat_func",
			         "send_heart_beat_to_every_chain");
			applog(3, logbuf, 0);
		}
		ret = -3;
	}
	return ret;
}

// === write_data_to_PIC16F1704_flash === 原 FUN_00041e7c @ 0x00041e7c
// 来源: reversed/cgminer/13_voltage_pic/FUN_00041e7c.c | 分类: 13_voltage_pic
// 语义: 向 PIC flash 写变长数据: 发 [55 AA (len+6) 33 reg len data×len chk_hi chk_lo]
//       (chk = 2*len + 0x39 + Σdata, 大端先高后低; 0x39 = LEN(len+6)+CMD(0x33)+len,
//       帧偏移 4 的 reg 参数不计入 chk, 原样), 等 200ms 回读 4 字节
//       [LEN=05][CMD 回显 33][状态 01][?] 三对即成功; 帧区/日志缓冲在原栈上连续,
//       len 过大越界写原样保留。成功返回 1 (LOG_GATE7 开则打 ok), 失败按日志门
//       (级别 3) 打印读回前 3 字节并返回 0。读回区原样预置 0xff。
uint8_t write_data_to_PIC16F1704_flash(uint8_t *data, uint8_t reg, uint32_t len)
{
	uint8_t buf[0x1018]; /* 原栈连续区: [0..16]=TX 帧 (frame), [20..4119]=日志缓冲 */
	uint8_t *frame = buf;
	char *logbuf = (char *)buf + 20;
	uint8_t rb[4];       /* 读回区: 原 local_1034 (前 1 字节 uStack_1035 为游标起点) */
	uint32_t i;
	uint32_t chk;        /* 原 uVar4/uVar5: 16 位校验和 */
	uint32_t ret;

	chk = ((len & 0xffff) * 2 + 0x39) & 0xffff;            /* 原 (param_3&0xffff)*2+0x39 */
	if (0 < (int)len) {
		i = 0;
		do {
			chk = (chk + data[i]) & 0xffff;                /* 累加数据 (索引 8 位回绕) */
			i = (i + 1) & 0xff;
		} while ((int)i < (int)len);
	}
	frame[0] = 'U';                                        /* 0x55 */
	frame[1] = PIC_FRAME_PREAMBLE1;                        /* 0xAA */
	frame[2] = (uint8_t)(len + 6);                         /* LEN = 数据长 + 6 */
	frame[3] = PIC_CMD_WRITE_DATA_FLASH;                   /* 0x33 */
	frame[4] = reg;                                        /* 帧偏移 4 参数 (不计 chk, 语义待定) */
	frame[5] = (uint8_t)len;                               /* 长度参数 (计入 chk) */
	memcpy(frame + 6, data, len);                          /* 数据从帧偏移 6 起 */
	frame[len + 6] = (uint8_t)(chk >> 8);                  /* chk_hi */
	frame[len + 7] = (uint8_t)chk;                         /* chk_lo */
	rb[0] = 0xff;                                          /* 原样预置 local_1034 = 0xff (4B LE) */
	rb[1] = 0;
	rb[2] = 0;
	rb[3] = 0;

	pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
	if (0 < (int)(len + 8)) {
		i = 0;
		do {                                               /* 逐字节位带发送整帧 (索引 8 位回绕) */
			pic_i2c_write_byte(frame[i]);
			i = (i + 1) & 0xff;
		} while ((int)i < (int)(len + 8));
	}
	usleep(200000);                                        /* 200ms — 等 PIC 写 flash */
	rb[0] = pic_i2c_read_byte();                           /* 回读 [LEN][CMD][状态][?] */
	rb[1] = pic_i2c_read_byte();
	rb[2] = pic_i2c_read_byte();
	rb[3] = pic_i2c_read_byte();
	pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
	if (((rb[0] & 0xff) == 5) && (rb[1] == PIC_CMD_WRITE_DATA_FLASH) && (rb[2] == 1)) {
		ret = 1;
		if (LOG_GATE7()) {
			snprintf(logbuf, 0x1000, "\n--- %s ok\n\n", "write_data_to_PIC16F1704_flash");
			applog(7, logbuf, 0);
			ret = 1;
		}
	}
	else {
		if (LOG_GATE3()) {
			snprintf(logbuf, 0x1000,
			         "\n--- %s failed! read_back_data[0] = 0x%02x, read_back_data[1] = 0x%02x, read_back_data[2] = 0x%02x\n\n"
			         ,"write_data_to_PIC16F1704_flash",rb[0] & 0xff,(uint32_t)rb[1],
			         (uint32_t)rb[2]);
			applog(3, logbuf, 0);
		}
		ret = 0;
	}
	return (uint8_t)ret;
}

// === read_data_from_PIC16F1704_flash === 原 FUN_00042100 @ 0x00042100
// 来源: reversed/cgminer/13_voltage_pic/FUN_00042100.c | 分类: 13_voltage_pic
// 语义: 从 PIC flash 读变长数据: 发 8 字节帧 [55 AA 06 34 reg len chk_hi chk_lo]
//       (chk = len + 0x3A = LEN(6)+CMD(0x34)+len, 不含 reg, 原样), 等 200ms 回读
//       len+5 字节 [LEN=len+5][CMD 回显 34][状态 01][data×len][chk_hi chk_lo];
//       LEN/命令回显/状态对且 chk (= LEN+0x34+01+Σdata, 与发送侧公式不同, 原样) 对时
//       把 len 字节拷入 out 并返回 1 (LOG_GATE7 开则打 ok); chk 错按日志门 (级别 3)
//       打印 "failed! crc = 0x%04x"; 帧头不对静默返回 0。读回区/日志缓冲在原栈上
//       连续, len 过大越界写原样保留。读回区原样预置 local_1048=0xff + 0x24 字节清零。
uint8_t read_data_from_PIC16F1704_flash(uint8_t *out, uint8_t reg, uint32_t len)
{
	uint8_t buf[0x1030]; /* 原栈连续区: [0..7]=TX 帧, [8..]=读回区+日志缓冲 (4132B) */
	uint8_t *rb = buf + 8;                 /* 读回区 (原 local_1048 + abStack_1044) */
	char *logbuf = (char *)buf + 0x28;     /* 原 abStack_1044 + 0x24 */
	uint32_t i;
	uint32_t cur;        /* 原 uVar3: 累加器兼当前字节 */
	uint32_t sum;        /* 原 uVar6: 16 位校验和 */
	uint8_t ret;

	buf[0] = PIC_FRAME_PREAMBLE0;                          /* 原 uStack_1050 = 0x55 */
	buf[1] = PIC_FRAME_PREAMBLE1;                          /* 原 local_104f[0] */
	buf[2] = 6;                                            /* LEN (总长 8) */
	buf[3] = PIC_CMD_READ_DATA_FLASH;                      /* 0x34 */
	buf[4] = reg;                                          /* 帧偏移 4 参数 (不计 chk) */
	buf[5] = (uint8_t)len;                                 /* 长度参数 (计入 chk) */
	buf[6] = (uint8_t)((len + 0x3a) >> 8);                 /* chk_hi = len + 0x3A */
	buf[7] = (uint8_t)(len + 0x3a);                        /* chk_lo */
	buf[8] = 0xff;                                         /* 原样预置 local_1048 = 0xff (4B LE) */
	buf[9] = 0;
	buf[10] = 0;
	buf[11] = 0;
	for (i = 12; i < 12 + 0x24; i++) {                     /* 原样预置 abStack_1044[0..0x23] = 0 */
		buf[i] = 0;
	}

	pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
	for (i = 0; i < 8; i++) {
		pic_i2c_write_byte(buf[i]);
	}
	usleep(200000);                                        /* 200ms — 等 PIC 读 flash */
	i = 0;
	do {                                                   /* 回读 len+5 字节 (索引 8 位回绕) */
		buf[8 + i] = pic_i2c_read_byte();
		i = (i + 1) & 0xff;
	} while ((int)i <= (int)(len + 4));
	pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
	if ((rb[1] == PIC_CMD_READ_DATA_FLASH) && ((rb[0] & 0xff) == len + 5) && (rb[2] == 1)) {
		cur = rb[0];                                       /* 原 uVar3 初值 = LEN 回显 */
		i = 0;                                             /* 原 uVar4 */
		sum = 0;                                           /* 原 uVar6 */
		while (true) {
			sum = (sum + cur) & 0xffff;                    /* chk = Σ rb[0..len+2] */
			i = (i + 1) & 0xff;
			if ((int)(len + 2) < (int)i) {
				break;
			}
			cur = rb[i];
		}
		if (((uint32_t)rb[len + 3] == (sum >> 8)) && ((uint32_t)rb[len + 4] == (sum & 0xff))) {
			if (LOG_GATE7()) {
				snprintf(logbuf, 0x1000, "\n--- %s ok\n\n", "read_data_from_PIC16F1704_flash");
				applog(7, logbuf, 0);
			}
			memcpy(out, rb + 3, len);                      /* 数据拷回调用方 */
			ret = 1;
		}
		else {
			if (LOG_GATE3()) {
				snprintf(logbuf, 0x1000, "\n--- %s failed! crc = 0x%04x\n\n",
				         "read_data_from_PIC16F1704_flash",sum);
				applog(3, logbuf, 0);
			}
			ret = 0;
		}
	}
	else {
		ret = 0;                                           /* 帧头不对: 静默失败 */
	}
	return ret;
}

// === check_pic_flash_support === 原 FUN_00046a48 @ 0x00046a48
// 来源: reversed/cgminer/13_voltage_pic/FUN_00046a48.c | 分类: 13_voltage_pic
// 语义: 探测各链 PIC 是否支持 flash 数据存储: 对每条在位链读 9 字节 (reg 0, len 9),
//       读失败 (返回 0) 按日志门打印 "chain%d pic not support flash data storage,
//       adopt file mode" 并直接返回 0; 成功则对 data[1..8] (2 字, 原栈上非对齐访问,
//       原样) 做 XXTEA 解密 (密钥 = DAT_00069af8 + 链号×16), 把 9 字节 (字, 字, 字节)
//       连续拷入 out + 9*chain。返回最后一次读调用的结果 (无在位链时为初值 1)。
int check_pic_flash_support(uint8_t *out)
{
	char *p_exist;       /* 原 pcVar2: 从 &DAT_0009d4bb 起步先 ++ 后判 — 即 DAT_0009d4bc[chain] */
	uint8_t data[9];     /* 原 local_1034/local_1030/local_102c 连续 9 字节 */
	char logbuf[4100];   /* 原 acStack_1028 */
	int out_off;         /* 原 iVar1: 输出偏移 (每链 +9 字节) */
	int chain;           /* 原 param3 */
	int ret;             /* 原 iVar3 */

	p_exist = &DAT_0009d4bb[0];
	out_off = 0;
	chain = 0;
	ret = 1;
	while (true) {
		p_exist++;
		if (*p_exist == 1) {         /* 链在位 */
			data[0] = 0;                               /* 原样清零 9 字节读回区 */
			data[1] = 0;
			data[2] = 0;
			data[3] = 0;
			data[4] = 0;
			data[5] = 0;
			data[6] = 0;
			data[7] = 0;
			data[8] = 0;
			pthread_mutex_lock((pthread_mutex_t *)DAT_000790d0);
			DAT_0007909c = chain;
			ret = read_data_from_PIC16F1704_flash(data, 0, 9);
			cgsleep_ms(100);
			pthread_mutex_unlock((pthread_mutex_t *)DAT_000790d0);
			if (ret == 0) {
				if (!LOG_GATE5()) {
					return 0;
				}
				snprintf(logbuf, 0x1000,
				         "chain%d pic not support flash data storage, adopt file mode\n"
				         ,chain);
				applog(5, logbuf, 0);
				return 0;
			}
			if (0 < ret) {
				/* data+1 处 2 字 XXTEA 解密 (原码字节指针 +1, 非对齐字访问原样) */
				xxtea_decode((uint32_t *)(data + 1), 2,
				             (uint32_t *)(DAT_00069af8 + chain * 0x10));
				out[out_off + 0] = data[0];            /* 9 字节连续拷出 (字, 字, 字节) */
				out[out_off + 1] = data[1];
				out[out_off + 2] = data[2];
				out[out_off + 3] = data[3];
				out[out_off + 4] = data[4];
				out[out_off + 5] = data[5];
				out[out_off + 6] = data[6];
				out[out_off + 7] = data[7];
				out[out_off + 8] = data[8];
			}
		}
		chain++;
		out_off += 9;
		if (chain == 4) {
			return ret;
		}
	}
}

// === xxtea_decode === 原 FUN_00056cd8 @ 0x00056cd8
// 来源: reversed/cgminer/13_voltage_pic/FUN_00056cd8.c | 分类: 13_voltage_pic
// 语义: 标准 XXTEA 解密 (Corrected Block TEA, delta = 0x9E3779B9): 对 data[0..n-1]
//       共 n 个 32 位字原位解密, 轮数 = 6 + 52/n (原码经 __aeabi_idivmod(0x34, n)
//       取商, r1 残留 n, 反汇编 0x56cfc 证实)。n < 2 时按日志门 (级别 3) 打印
//       "XXTEA decode fails, N <= 1 found." 后返回。e = (sum>>2)&3 (原码写法
//       (sum&0xF)>>2, 等价), MX 与标准算法定义一致 (y = v[p+1] 新值, z = v[p-1] 旧值)。
void xxtea_decode(uint32_t *data, int n, uint32_t *key)
{
	uint32_t y;          /* 原 uVar3: MX 的 y (v[p+1] 新值, 跨迭代/轮次传递) */
	uint32_t z;          /* 原 uVar4/uVar2: MX 的 z (v[p-1] / v[n-1]) */
	uint32_t sum;        /* 原 uVar5: 轮次累加和 */
	uint32_t e;          /* 原 uVar7 */
	uint32_t kidx;       /* 原 uVar6: (p & 3) */
	uint32_t *pv;        /* 原 puVar8: 写游标 (指向待更新字的下一位置) */
	int i;               /* 原 uVar2: 内层计数 p */
	int rounds;          /* 原 iVar1 */

	if (n < 2) {
		if (LOG_GATE3()) {
			/* 原码逐字节拼入栈缓冲后直接作为消息打印, 此处等价于字面量 */
			applog(3, "XXTEA decode fails, N <= 1 found.\n", 0);
		}
		return;
	}
	rounds = 6 + 52 / n;                       /* 原 __aeabi_idivmod(0x34, n) 商 + 6 */
	sum = rounds * 0x9e3779b9;                 /* 原 rounds * (-0x61c88647) */
	y = data[0];
	do {
		e = (sum & 0xf) >> 2;                  /* = (sum >> 2) & 3 (标准 XXTEA) */
		i = n - 1;
		pv = data + n;                         /* puVar8[-1] = v[n-1] */
		do {
			kidx = (uint32_t)i & 3;
			z = pv[-2];                        /* z = v[p-1] */
			i--;
			y = pv[-1] - (((z ^ key[kidx ^ e]) + (y ^ sum)) ^
			              ((y >> 3 ^ z << 4) + (y << 2 ^ z >> 5)));
			pv--;
			*pv = y;                           /* v[p] -= MX */
		} while (i != 0);
		z = data[n - 1];
		rounds--;
		y = data[0] - (((z << 4 ^ y >> 3) + (z >> 5 ^ y << 2)) ^
		              ((y ^ sum) + (z ^ key[e])));
		sum += 0x61c88647;                     /* sum -= 0x9E3779B9 (原码写法) */
		data[0] = y;
	} while (rounds != 0);
}
/* ==================== 覆盖率表 (39/39, batch=pic_b1) ====================
 * 原 FUN_0003c66c @ 0x0003c66c -> i2c_write_core                               (reversed/cgminer/13_voltage_pic/FUN_0003c66c.c)
 * 原 FUN_0003c708 @ 0x0003c708 -> i2c_read_core                                (reversed/cgminer/13_voltage_pic/FUN_0003c708.c)
 * 原 FUN_0003c7b0 @ 0x0003c7b0 -> pic_i2c_write_byte                           (reversed/cgminer/13_voltage_pic/FUN_0003c7b0.c)
 * 原 FUN_0003c7c8 @ 0x0003c7c8 -> pic_i2c_read_byte                            (reversed/cgminer/13_voltage_pic/FUN_0003c7c8.c)
 * 原 FUN_0003c7e0 @ 0x0003c7e0 -> write_dc_dc                                  (reversed/cgminer/13_voltage_pic/FUN_0003c7e0.c)
 * 原 FUN_0003d000 @ 0x0003d000 -> set_PIC16F1704_flash_pointer_new             (reversed/cgminer/13_voltage_pic/FUN_0003d000.c)
 * 原 FUN_0003d1ac @ 0x0003d1ac -> send_data_to_PIC16F1704_new                  (reversed/cgminer/13_voltage_pic/FUN_0003d1ac.c)
 * 原 FUN_0003d3b4 @ 0x0003d3b4 -> read_PIC16F1704_flash_pointer_new            (reversed/cgminer/13_voltage_pic/FUN_0003d3b4.c)
 * 原 FUN_0003d67c @ 0x0003d67c -> read_PIC16F1704_flash_data_new               (reversed/cgminer/13_voltage_pic/FUN_0003d67c.c)
 * 原 FUN_0003da84 @ 0x0003da84 -> erase_PIC16F1704_flash_new                   (reversed/cgminer/13_voltage_pic/FUN_0003da84.c)
 * 原 FUN_0003dc28 @ 0x0003dc28 -> write_data_into_PIC16F1704_flash_new         (reversed/cgminer/13_voltage_pic/FUN_0003dc28.c)
 * 原 FUN_0003ddd0 @ 0x0003ddd0 -> jump_from_loader_to_app_PIC16F1704_new       (reversed/cgminer/13_voltage_pic/FUN_0003ddd0.c)
 * 原 FUN_0003df78 @ 0x0003df78 -> reset_PIC16F1704_pic_new                     (reversed/cgminer/13_voltage_pic/FUN_0003df78.c)
 * 原 FUN_0003e120 @ 0x0003e120 -> pic_transfer_data_crc16                      (reversed/cgminer/13_voltage_pic/FUN_0003e120.c)
 * 原 FUN_0003ea50 @ 0x0003ea50 -> set_isl_voltage                              (reversed/cgminer/13_voltage_pic/FUN_0003ea50.c)
 * 原 FUN_0003eec8 @ 0x0003eec8 -> every_asic_set_isl_voltage                   (reversed/cgminer/13_voltage_pic/FUN_0003eec8.c)
 * 原 FUN_0003f254 @ 0x0003f254 -> show_every_asic_voltage                      (reversed/cgminer/13_voltage_pic/FUN_0003f254.c)
 * 原 FUN_0003f308 @ 0x0003f308 -> set_PIC16F1704_voltage_new                   (reversed/cgminer/13_voltage_pic/FUN_0003f308.c)
 * 原 FUN_0003f4bc @ 0x0003f4bc -> write_hash_ID_PIC16F1704_new                 (reversed/cgminer/13_voltage_pic/FUN_0003f4bc.c)
 * 原 FUN_0003f6cc @ 0x0003f6cc -> read_hash_id_PIC16F1704_new                  (reversed/cgminer/13_voltage_pic/FUN_0003f6cc.c)
 * 原 FUN_0003fa7c @ 0x0003fa7c -> enable_PIC16F1704_dc_dc_new                  (reversed/cgminer/13_voltage_pic/FUN_0003fa7c.c)
 * 原 FUN_0003fc24 @ 0x0003fc24 -> disable_PIC16F1704_dc_dc_new                 (reversed/cgminer/13_voltage_pic/FUN_0003fc24.c)
 * 原 FUN_0003fdc4 @ 0x0003fdc4 -> heart_beat_PIC16F1704_new                    (reversed/cgminer/13_voltage_pic/FUN_0003fdc4.c)
 * 原 FUN_00040128 @ 0x00040128 -> get_PIC16F1704_software_version_new          (reversed/cgminer/13_voltage_pic/FUN_00040128.c)
 * 原 FUN_00040454 @ 0x00040454 -> get_every_chain_pic_version                  (reversed/cgminer/13_voltage_pic/FUN_00040454.c)
 * 原 FUN_000405ac @ 0x000405ac -> get_PIC16F1704_voltage_new                   (reversed/cgminer/13_voltage_pic/FUN_000405ac.c)
 * 原 FUN_00040888 @ 0x00040888 -> write_temperature_offset_PIC16F1704_new      (reversed/cgminer/13_voltage_pic/FUN_00040888.c)
 * 原 FUN_00040a84 @ 0x00040a84 -> read_temperature_offset_PIC16F1704_new       (reversed/cgminer/13_voltage_pic/FUN_00040a84.c)
 * 原 FUN_00040dec @ 0x00040dec -> erase_PIC16F1704_app_flash_new               (reversed/cgminer/13_voltage_pic/FUN_00040dec.c)
 * 原 FUN_00040e94 @ 0x00040e94 -> PIC1704_update_pic_app_program_new           (reversed/cgminer/13_voltage_pic/FUN_00040e94.c)
 * 原 FUN_00041308 @ 0x00041308 -> every_chain_reset_PIC16F1704_pic_new         (reversed/cgminer/13_voltage_pic/FUN_00041308.c)
 * 原 FUN_000413f0 @ 0x000413f0 -> every_chain_jump_from_loader_to_app_PIC16F1704_new (reversed/cgminer/13_voltage_pic/FUN_000413f0.c)
 * 原 FUN_000414d8 @ 0x000414d8 -> every_chain_disable_PIC16F1704_dc_dc_new     (reversed/cgminer/13_voltage_pic/FUN_000414d8.c)
 * 原 FUN_00041614 @ 0x00041614 -> every_chain_enable_PIC16F1704_dc_dc_new      (reversed/cgminer/13_voltage_pic/FUN_00041614.c)
 * 原 FUN_000416ec @ 0x000416ec -> send_heart_beat_to_every_chain               (reversed/cgminer/13_voltage_pic/FUN_000416ec.c)
 * 原 FUN_00041e7c @ 0x00041e7c -> write_data_to_PIC16F1704_flash               (reversed/cgminer/13_voltage_pic/FUN_00041e7c.c)
 * 原 FUN_00042100 @ 0x00042100 -> read_data_from_PIC16F1704_flash              (reversed/cgminer/13_voltage_pic/FUN_00042100.c)
 * 原 FUN_00046a48 @ 0x00046a48 -> check_pic_flash_support                      (reversed/cgminer/13_voltage_pic/FUN_00046a48.c)
 * 原 FUN_00056cd8 @ 0x00056cd8 -> xxtea_decode                                 (reversed/cgminer/13_voltage_pic/FUN_00056cd8.c)
 */
