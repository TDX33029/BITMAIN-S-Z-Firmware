/**
 * bm1746_regs.h — BM1746 ASIC 串口命令帧与寄存器/命令码
 *
 * 全部常量来源: bm1746/BM1746_uart_register_ops.md (2026-11-20 机器码级复核版)
 *   + reversed/tables/{known_constants.md, pll_decode.md, freq_table_full.csv, topol.conf}
 * 帧构函数: FUN_0003cdcc (11 字节命令帧) / FUN_0003cd58 (7B 复位帧) / FUN_0003ce7c (7B 设址帧)
 * 校验函数: FUN_0003cc6c (4-bit lane 校验, 低 5 位校验字节)
 */
#ifndef BM1746_REWRITTEN_BM1746_REGS_H
#define BM1746_REWRITTEN_BM1746_REGS_H

#include "common_types.h"

/* ==================== 11 字节命令帧 (BM1746_uart_register_ops.md §一) ====================
 * 偏移  长度  内容
 *  0..1   2   前导 TX=0x55,0xAA (RX 字节流实测为 AA 55, FPGA FIFO 字序, 收发不共用匹配器!)
 *  2      1   帧类型 = 0x40 | (rw<<4) | 0x01
 *             0x51 = 链广播写 (rw=1, 不带芯片地址, chip_addr=0)
 *             0x41 = 带芯片地址 (读, rw=0)
 *  3      1   固定 0x09 (非"保留0")
 *  4      1   芯片地址 = chip_addr * ASIC_ADDR_INTERVAL (广播写时 0)
 *  5      1   寄存器/命令码 (见下方 REG_* 宏)
 *  6..9   4   32 位值, 字节内交换后大端发送
 * 10      1   校验: 低 5 位 = 4-bit lane 校验, 高 3 位保留
 */
#define BM1746_FRAME_LEN        11
#define BM1746_PREAMBLE_TX0     0x55
#define BM1746_PREAMBLE_TX1     0xAA
#define BM1746_PREAMBLE_RX0     0xAA   /* RX 字节流前导 (FIFO 字序) */
#define BM1746_PREAMBLE_RX1     0x55
#define BM1746_FRAME_FIX_BYTE3  0x09

#define BM1746_FT_BROADCAST_WRITE  0x51   /* 帧类型: 链广播写 rw=1 */
#define BM1746_FT_ADDRESSED_READ   0x41   /* 帧类型: 带芯片地址 读 rw=0 */

#define BM1746_FRAME_OFF_TYPE      2
#define BM1746_FRAME_OFF_FIX       3
#define BM1746_FRAME_OFF_CHIP      4
#define BM1746_FRAME_OFF_REG       5
#define BM1746_FRAME_OFF_VALUE     6   /* 4 字节 BE */
#define BM1746_FRAME_OFF_CHK       10

/* ==================== 寄存器/命令码 (§二, 帧偏移 5) ==================== */
#define BM1746_REG_PLL          0x0C  /* PLL 分频值: 频率表 pll 字段 (freq_table) */
#define BM1746_REG_TICKET_MASK  0x14  /* ticket mask, 8bit; 出厂静态初值 0x10 (DAT_00076b60) */
#define BM1746_REG_BAUD_TEMP    0x1C  /* 波特率分频 (bt8d&0x1F)<<8 | 保留位; 0x4060|.. 开温度读回 */
#define BM1746_REG_I2C_INDIRECT 0x20  /* I2C 间接寄存器: 芯片内 I2C master 访问 ISL/TMP451 */
#define BM1746_REG_CORE_TIMEOUT 0x28  /* 核心超时, 默认 84000=0x14820 (DAT_00076b74) */
#define BM1746_REG_ANA_MUX      0x40  /* 模拟多路开关 (温度通道, 3bit) */
#define BM1746_REG_NONCE_SHIFT  0x50  /* nonce 位移 (值<<27) */

/* --- 0x1C 细分 --- */
#define BM1746_TEMP_ENABLE_1C   0x4060  /* enable_read_temperature_from_asic_chain: 逐芯片写 0x4060 */

/* --- 0x20 I2C 间接值拼装 (read/write_i2c_reg, FUN_0004633c 等) ---
 * value = base[0x76B6C]&0xFF000000 | 0x01000000 | (i2c_reg&0x7F)<<17 | (type&0xFF)<<8 | data
 * type: 0 / 1 / 0x11 / 0xFE 四种 I2C 通道 */
#define BM1746_I2C_IND_FIXED    0x01000000u
#define BM1746_I2C_REG_MASK     0x7F
#define BM1746_I2C_REG_SHIFT    17
#define BM1746_I2C_TYPE_SHIFT   8

/* ==================== 核心超时 / ticket mask 默认值 ==================== */
#define BM1746_CORE_TIMEOUT_DEFAULT  84000u  /* 0x14820, DAT_00076b74 出厂静态初值 */
#define BM1746_TICKET_MASK_FACTORY   0x10    /* DAT_00076b60 出厂静态初值 */
#define BM1746_NONCE_SHIFT_VAL       27      /* set_nonce_shift_chain: 值<<27 */

/* ==================== 7 字节帧 (链初始化设址, §四-4b; 2026-11-20 修正版) ==================== */
#define BM1746_SHORT_FRAME_LEN        7
#define BM1747_RESET_FRAME_TYPE       0x53  /* 复位帧 [55 AA 53 05 00 00 chk]: 复位芯片地址计数器 */
#define BM1746_SETADDR_FRAME_TYPE     0x40  /* 设址帧 [55 AA 40 05 addr 00 chk]: 逐组设址 */
#define BM1746_SHORT_FRAME_CMD_OFF    3     /* 偏移3 固定 0x05 */
#define BM1746_SHORT_FRAME_ARG_OFF    4

/* ==================== PIC16F1704 命令 (同一 UART 半双工, §四-3) ====================
 * 帧格式: [55][AA][LEN][CMD][ARG...] + 回显校验 (回 CMD,01 = 成功) */
#define PIC_FRAME_PREAMBLE0   0x55
#define PIC_FRAME_PREAMBLE1   0xAA

#define PIC_CMD_JUMP_LOADER_TO_APP  0x06  /* 4/06, 回 06,01 */
#define PIC_CMD_PIC_RESET           0x07  /* 4/07, 回 07,01 = PIC 自身复位 */
#define PIC_CMD_GET_FREQ            0x09  /* 4/09? get PIC freq */
#define PIC_CMD_SET_VOLTAGE         0x10  /* 5/10 [电压Lo][电压Hi], 电压值 = param+0x15 编码 */
#define PIC_CMD_DC_DC               0x15  /* 5/15/1A 关 DC-DC; 5/15/1B 开 DC-DC, 回 15,01 */
#define PIC_SUB_DISABLE_DC_DC       0x1A
#define PIC_SUB_ENABLE_DC_DC        0x1B
#define PIC_VOLTAGE_ENCODE_BIAS     0x15  /* set_voltage 编码偏置 */

/* ==================== PLL 配置字 (reg 0x0C, pll_decode.md 已验证) ====================
 * fout(MHz) = 25 * N / (P * Q);  N = pll[31:16], P = pll[7:4], Q = pll[11:8]; bit0 固定 1
 * 频率表 0x69204: 115 条 × 16B [freq_MHz][?][?][pll_value]; 未命中兜底槽位 0x40=587MHz */
#define PLL_REF_CLK_MHZ      25
#define PLL_N_SHIFT          16
#define PLL_Q_SHIFT          8
#define PLL_P_SHIFT          4
#define PLL_N_MASK           0xFFFFu
#define PLL_PQ_MASK          0xFu
#define PLL_BIT0_ENABLE      0x1u

#define FREQ_TABLE_ADDR      0x69204  /* ELF 内频率表位置 */
#define FREQ_TABLE_ENTRIES   115
#define FREQ_TABLE_ENTRY_SZ  16

#define PLL_800MHZ           0x00800221u  /* 本机目标点 */
#define PLL_587MHZ_FALLBACK  0x005E0221u  /* 兜底槽位 0x40 */

/* ==================== 已验证全局数据 ==================== */
/* I2C 每 ASIC 地址表 DAT_00076b48 前 16 字节 (known_constants.md) */
#define G_I2C_PER_ASIC_ADDR0  0x50
#define G_I2C_PER_ASIC_ADDR1  0x54
#define G_I2C_PER_ASIC_ADDR2  0x58
#define G_I2C_PER_ASIC_ADDR3  0x01

/* ISL 电压回读校验容差 (FUN_0003ea50: set+回读 ±4mV) */
#define VOLT_VERIFY_TOLERANCE_MV  4

#endif /* BM1746_REWRITTEN_BM1746_REGS_H */
