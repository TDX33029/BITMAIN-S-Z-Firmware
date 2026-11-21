/**
 * fpga_axi.h — PL/FPGA AXI 寄存器窗口 (/dev/axi_fpga_dev, mmap 0x40000000, 0x1200 字节)
 *
 * 来源: bm1746/BM1746_uart_register_ops.md §〇 (FUN_0003b704/3ba70/3b994/3bb44/3bb78/
 *       3c66c/3c708/3c150 实测语义) — 全部偏移为**字节偏移** (经反汇编 str/ldr 立即数确认),
 *       访问统一用 *(volatile uint32_t*)((uint8_t*)base + AXI_REG_XXX), 见 cgminer/axi.c 的 AXI_REG32() 宏
 *
 * 同时打开 /dev/fpga_mem mmap 0x1000000 (工作下发用 DDR 窗口, 见 ko/fpga_mem.c)
 */
#ifndef BM1746_REWRITTEN_FPGA_AXI_H
#define BM1746_REWRITTEN_FPGA_AXI_H

#include "common_types.h"

/* ---- 窗口参数 (FUN_0003b704) ---- */
#define AXI_FPGA_DEV_PATH     "/dev/axi_fpga_dev"
#define FPGA_MEM_DEV_PATH     "/dev/fpga_mem"
#define AXI_WINDOW_SIZE       0x1200
#define DDR_WINDOW_SIZE       0x1000000

/* ---- 硬件版本/状态 (FUN_0003ba70 init_fpga, FUN_0003b994) ---- */
#define AXI_REG_VERSION       0     /* 读: version = base[0]&0xFFFF (打印 "%04X") */
#define AXI_INIT_MAGIC        0x8080800F  /* init_fpga 写 base[0x80], 轮询等 bit31==0 */
#define AXI_HW_VERSION_SET    0x60000000u /* init_fpga: base[0] |= 0x60000000 */
#define AXI_BUSY_BIT31        0x80000000u

/* ---- 命令通道 (FUN_0003c66c/0003c708) ----
 * base[0x30] = 0x02000000|cmd 写入 (等 bit31 清零再写); 读回 = base[0x30]&0xFF */
#define AXI_REG_CMD           0x30
#define AXI_CMD_PREFIX        0x02000000u

/* ---- 链复位 (FUN_00046434 reset_chain) ---- */
#define AXI_REG_CHAIN_RESET   0x34   /* base[0x34]=1<<chain 拉低 -> sleep -> =0 释放 */

/* ---- 波特率 (FUN_0003bb44 set_baud_pl) ---- */
#define AXI_REG_BAUD_PL       0x3C   /* bt8d|bt8d<<8|bt8d<<16|bt8d<<24 (4 链各 8bit) */
#define AXI_BAUD_PL_INITIAL   0x1A   /* reset_chain 尾部: PL 初始分频 */

/* ---- UART RX 计数 (FUN_0003bb78 check_how_many_uart_data_in_fpga) ----
 * uart0: (base[0x1F0]>>16)&0x3FFFFFF; 低位 base[0x1F0]&0x3FF
 * uart1..: base[0x1F4], base[0x1F8], base[0x1FC], base[0x200] */
#define AXI_REG_UART_RX_COUNT0 0x1F0
#define AXI_REG_UART_RX_COUNT1 0x1F4
#define AXI_REG_UART_RX_COUNT2 0x1F8
#define AXI_REG_UART_RX_COUNT3 0x1FC
#define AXI_REG_UART_RX_COUNT4 0x200
#define AXI_RX_COUNT_MASK      0x3FFFFFFu
#define AXI_RX_LOW_MASK        0x3FFu

/* ---- UART TX FIFO (FUN_0003c150 uart_send) ----
 * 等 base[0x180+0x04*grp] 的 TX 空闲字节域, 数据/触发寄存器 0x65+2u / 0x64+2u (u=uart 号,
 * 字索引), 写前 32 位字序交换 — 详细位域见 cgminer/uart.c (反汇编为准):
 * 空闲域移位 uart0=24, uart1=16, uart2=8, uart3=0 (clear_uart_send_fifo 反汇编实测)
 * (注: 本文件早版 AXI_TX_IDLE_SHIFT 宏移位与此不符, 已弃用, 勿引用) */
#define AXI_REG_UART_TX_BASE   0x180   /* TX FIFO 组寄存器 0x180 + 0x04*grp (字节偏移) */
#define AXI_REG_UART_TX_STRIDE 4
#define AXI_REG_UART_TX_STAT0  0x190   /* 0x190..0x18C 区状态字 (0x65..0x77) */

/* ---- PL 内部其它 (§〇; 具体语义以各转写函数注释为准) ---- */
#define AXI_REG_INIT_MAGIC_OFF 0x80

/* ---- 设备字符 (reversed/analysis/gap_reports/devicetree_analysis.md) ---- */
/* UART0 @0xe0000000, GEM @0xe000b000, efuse @0xf800d000; SLCR 0xF800xxxx (fsbl) */

#endif /* BM1746_REWRITTEN_FPGA_AXI_H */
