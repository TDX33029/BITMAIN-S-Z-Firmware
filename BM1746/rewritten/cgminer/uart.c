/*
 * cgminer UART 协议层 (批次 uart_b1) — BM1746 固件语义转写
 * =========================================================
 * 原始二进制: bm1746/cgminer_z15_bm1746.elf (ARM32, SD 镜像 cgminer-z15, branch "bm1746")
 * 来源分类:   reversed/cgminer/10_uart_protocol/ (11 个函数, 清单见 rewritten/batch_plan.tsv batch=uart_b1)
 *
 * 模块功能概述:
 *   1) FPGA AXI 窗口上的 UART 收发原语 (uart 0..9):
 *      - RX 计数查询 check_how_many_uart_data_in_fpga / RX 读出 read_uart_data_in_fpga /
 *        RX 清空 clear_uart_rx_fifo / TX 清空等待 clear_uart_send_fifo / TX 发送 uart_send;
 *      - RX 计数寄存器为字节偏移 0x1F0+4*(uart>>1), 偶数号 UART 取高 10 位、奇数号取低 10 位
 *        (机器码为 ubfx #16/#0, 10 位域; Ghidra 渲染为 (v&0x3ffffff)>>16, 数学等价);
 *      - RX 读命令寄存器 = 字节偏移 0x210+8*uart (写 len&0x3ff|0x80000000 启动),
 *        RX 数据 FIFO = 字索引 0x85+2*uart (字节 0x214+8*uart), FIFO 字按大端展开为 4 字节;
 *      - TX FIFO 空闲字节数状态寄存器 = 字节偏移 0x180+(uart>>2)*4, 移位 24-8*(uart&3)
 *        (注意: include/fpga_axi.h 的 AXI_TX_IDLE_SHIFT 宏与此不一致, 本文件以反汇编为准);
 *      - TX 数据 FIFO = 字索引 0x65+2*uart (字节 0x194+8*uart), 写入前 32 位字序反序 (小端缓冲 →
 *        大端 FIFO 字); 发送长度/触发寄存器 = 字索引 0x64+2*uart (字节 0x190+8*uart),
 *        写 len|0x80000000 触发发送, bit31 同时是"发送器忙"标志 (符号位判别)。
 *   2) ASIC 寄存器读回帧处理 (reg_handle): RX 解帧线程 FUN_0004b0c0 (16_chain_mgmt 批次) 识别
 *      RX 字节流前导 AA 55 (FPGA FIFO 字序, 与 TX 前导 55 AA 不共用匹配器!) + 帧类型后, 把 9 字节
 *      帧体交给本函数; 校验 4-bit lane 校验 (FUN_0003cc6c, 51 位, 校验字节取低 5 位) 后压入
 *      100 槽 × 7 字节的应答环形队列, 由 check_asic_reg (FUN_000481ec) 消费。
 *      帧体布局: [0..1]=未消费 (推断 0x09/芯片地址回显) [2]=寄存器码回显 [3..6]=4 字节值 (帧内 BE)
 *                [7]=附加类型/I2C 通道 [8]=校验字节 (低 5 位)。
 *   3) PIC/DC-DC I2C 位带通道 (read_dc_dc / send_data_to_all_asic / read_data_from_all_asic):
 *      通过 AXI 命令通道 (pic_i2c_write_byte/pic_i2c_read_byte, 命令字 = 字节|当前链号<<16|0xa00000)
 *      逐位收发, 帧格式 [55][AA][LEN][0x32][从机地址<<1(+1 读)][reg][len][校验和 Hi][校验和 Lo],
 *      校验和 = 前导之后各字节的 16 位和, 大端存放 (先高后低)。
 *   4) stratum_send: 矿池 stratum 发送封装 (util.c, 行号锚点 0x765/0x768)。
 *   5) status_service_thread: 搜频 HTTP 应答线程 (Server: SearchFreqServer), 处理 GET /rate 与
 *      GET /productName。
 *
 * 转写约定 (rewritten/CONVENTIONS.md):
 *   - 命名事实源 rewritten/name_map/cgminer.tsv; 帧格式权威文档 bm1746/BM1746_uart_register_ops.md
 *     (2026-11-20 机器码级复核版: TX 前导 55 AA / RX 前导 AA 55 / 0x51=链广播写 / 0x41=带址读 /
 *      byte[3]=0x09 / 校验字节低 5 位 / ticket mask 出厂 0x10 / 核心超时 84000)。
 *   - AXI 访问加 volatile: 原机器码在等待循环与"尾字"补读处均为真实的重复加载, volatile 保证不被合并。
 *   - 未消化的 DAT_xxx 全局在本文件集中 extern (命名待补, 注释给出已核实的语义证据);
 *     后续批次共享时应上移到 include/ 头文件。
 *   - 原二进制多处把字符串字面量拆成栈上逐字赋值 (Ghidra 显示为单字节写入), 转写还原为等价的
 *     snprintf 字面量 — 产生的缓冲内容与 applog 调用完全一致。
 *   - 日志门控: 原固件所有日志调用点遵循统一模式 (反编译归纳):
 *       prio 7 (LOG_DEBUG): opt_debug && (A || B || C > 6)
 *       prio 5 (LOG_NOTICE): A || B || C > 4
 *       prio 4 (LOG_WARNING): A || B || C > 3
 *       prio 3 (LOG_ERR):     A || B || C > 2
 *     其中 A=DAT_0007ad48 (applog 内非 0 走 syslog 输出), B=DAT_00078e08 (curses 'v' Verbose 联动),
 *     C=DAT_000766c4 (int 日志级别)。applog 第 3 参非 0 时强制 curses 刷新 (见 FUN_000389f4 _applog)。
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/time.h>

#include "bm1746_regs.h"   /* PIC_FRAME_PREAMBLE0/1 等帧常量 */
#include "fpga_axi.h"      /* AXI_REG_UART_RX_COUNT0.. / AXI_RX_LOW_MASK / AXI_BUSY_BIT31 */

/* ==================== 外部全局 (DAT_ 保留原名, 语义见注释) ==================== */

extern uint8_t *DAT_00078e10;   /* FPGA AXI 寄存器窗口 mmap 基址 (init_fpga FUN_0003b704, /dev/axi_fpga_dev) */

extern char DAT_00078ebc[];     /* UART TX 互斥锁数组基址, 每 uart 0x18 字节 (pthread_mutex_t) */
extern char DAT_00078fac[];     /* UART RX 互斥锁数组基址, 每 uart 0x18 字节 */
extern char DAT_000790a0[];     /* PIC/I2C 位带总线互斥锁 (单把, write_dc_dc/read_dc_dc 共用) */
extern char DAT_000790d0[];     /* send/read_data_to/from_all_asic 互斥锁 (单把) */
extern char DAT_00079b78[];     /* 寄存器应答环形队列互斥锁 (reg_handle 写 / check_asic_reg 读) */
extern int  DAT_0007909c;       /* 当前链号: pic_i2c_write/read_byte 拼入 AXI 命令 bit16.. (FUN_0003c7b0) */

extern char DAT_0009d4bb[];     /* +1 起 (0x9d4bc) 为链存在标志数组: 值==1 表示链在位 (BM1746_uart_register_ops.md §四-2) */
extern uint8_t DAT_00076b48[];  /* 每 ASIC 组 I2C 从机地址表: 前 3 项 = G_I2C_PER_ASIC_ADDR0/1/2 (0x50/0x54/0x58) */

extern uint8_t DAT_0007bda0[];  /* 寄存器应答环形队列: 100 槽 × 7 字节 (@0x7bda0..0x7c013)
                                 * 槽布局: [0]=帧体[2] (reg 回显) [1..4]=帧体[3..6] 4 字节值 (原码非对齐 str)
                                 *         [5]=帧体[7] (类型/I2C 通道) [6]=链号; 由 check_asic_reg (FUN_000481ec) 消费 */
extern int  DAT_0007bd94;       /* 队列写索引 (0..99, 写满 99 后归 0) */
extern int  DAT_0007bd98;       /* 队列读索引 (0..99, check_asic_reg 推进, ==100 归 0) */
extern int  DAT_0007bd9c;       /* 未消费应答计数 (上限 100, 封顶时告警 "reg fifo full!!!") */

extern char DAT_0007ad48;       /* 全局: applog (FUN_00038730) 内非 0 走 syslog() 输出; 兼作日志门控条件 A */
extern char DAT_00078e08;       /* 全局: 冗余日志开关 (curses 'v' Verbose mode / 'd' Debug mode 联动置位) */
extern int  DAT_000766c4;       /* 全局: 日志级别 (int); 调用点以 (优先级-1) < 该值 门控, LOG_DEBUG=7 需 >6 */
extern char DAT_00078e09;       /* 全局: opt_debug (curses 'd' Debug mode) */
extern char DAT_0007b380;       /* 全局: opt_protocol (curses 'r' RPC protocol debugging) */
extern void (*DAT_00075ed8)(void); /* 全局函数指针: ELF 分析初值 noop_hook (FUN_00028ae0, 恒 0), 运行期未见改写 [低置信] */

extern char DAT_0007acb8;       /* 全局: 搜频 HTTP 服务退出标志 (accept/recv/send 循环检查, 非 0 收尾退出) */
extern int  DAT_00076b98;       /* 全局: 搜频状态 (仅本文件引用): 1=完成→应答固定 420000, 2=失败, 其余=进行中 */
extern char DAT_00078850[];     /* 全局: miner type 字符串 (版本文件解析, 日志 "Miner compile time: %s type: %s") */

/* ==================== 外部函数 (名取自 name_map/cgminer.tsv) ==================== */

extern void     applog(uint32_t prio, char *buf, ...);    /* FUN_00038730: 第 3 参非 0 → 强制 curses 刷新 */
extern void     mutex_lock_fail_util(const char *fn, int line);   /* FUN_0000b708 */
extern uint32_t __stratum_send(int *pool, char *s, int total_stratum_json); /* FUN_00029404: 0=成功 1=写选择失败 2=发送失败 */
extern void     quit(int status);                                 /* FUN_00016ca8 */
extern void     suspend_stratum(int *pool);                       /* FUN_0002aee0 */
extern uint32_t calc_4bit_checksum(uint8_t *bits, uint32_t nbits); /* FUN_0003cc6c: 4-bit lane 校验, nbits=位数 */
extern void     clear_reg_response_queue(void);                   /* FUN_000481b4 */
extern void     pic_i2c_write_byte(uint8_t byte);                 /* FUN_0003c7b0 */
extern uint8_t  pic_i2c_read_byte(void);                          /* FUN_0003c7c8 (Ghidra 误标 void, r0 实有返回) */
extern uint32_t write_dc_dc(uint8_t slave, uint8_t addr, uint8_t *data, int len); /* FUN_0003c7e0 */
extern void     cgsleep_ms(int ms);                               /* FUN_0002a92c */

/* ==================== 本文件导出 ==================== */

uint32_t check_how_many_uart_data_in_fpga(int uart);
int      read_uart_data_in_fpga(uint32_t uart, uint8_t *buf, uint32_t len);
uint32_t clear_uart_send_fifo(int uart);
int      clear_uart_rx_fifo(int uart);
uint32_t uart_send(int uart, void *data, uint32_t len);
void     reg_handle(uint8_t *frame, uint32_t frame_len, uint8_t chain);
int      read_dc_dc(uint8_t slave, int reg, uint8_t *out, uint32_t len);
void     send_data_to_all_asic(uint32_t addr, uint8_t *data, int len);
void     read_data_from_all_asic(uint32_t addr, uint8_t *buf, int len);
int      stratum_send(int *pool, char *s, int total_stratum_json);
int      status_service_thread(int sock);

/* ==================== 局部命名常量 ==================== */

/* ---- PIC/DC-DC 帧域 (bm1746_regs.h 未收录, 语义由 write_dc_dc/read_dc_dc 日志串证实) ---- */
#define PIC_CMD_DCDC_DATA      0x32   /* DC-DC 数据读写命令 (帧偏移 3) */
#define PIC_DCDC_READ_LEN_BYTE 7      /* read_dc_dc 帧偏移 2 的长度域 (读帧固定 7) */

/* ---- BM1746 RX 应答帧体 ---- */
#define BM1746_CHK_MASK_5BIT   0x1f   /* 校验字节低 5 位有效 (高 3 位保留, BM1746_uart_register_ops.md §一) */
#define BM1746_RESP_CRC_BITS   0x33   /* 9B 帧体校验位数 = 51: 帧体[2..7] 48 位 + 校验字节高 3 位 */

/* ---- UART FIFO 等待参数 (clear_uart_send_fifo / uart_send) ---- */
#define UART_FIFO_WAIT_TRIES   0x15   /* 重试次数 21 */
#define UART_FIFO_WAIT_US      3000   /* 每次重试间隔 (us) */
#define UART_RX_READ_MASK      0x3ff  /* RX 读命令长度域 (低 10 位) */
#define UART_TX_MIDBUF_SIZE    0x200  /* uart_send 512 字节中转缓冲 */
#define UART_RX_CLEAR_GAP_US   500000 /* clear_uart_rx_fifo 收尾延时 (us) */
#define UART_DCDC_BUF_SIZE     0x1014 /* read_dc_dc 读回区+日志区连续栈空间 (16B 清零区 + 0x1004) */

/* ---- 调用点日志门控 (统一模式, 见文件头说明) ---- */
#define LOG_GATE7() (DAT_00078e09 && (DAT_0007ad48 || DAT_00078e08 || 6 < DAT_000766c4))
#define LOG_GATE5() (DAT_0007ad48 || DAT_00078e08 || 4 < DAT_000766c4)
#define LOG_GATE4() (DAT_0007ad48 || DAT_00078e08 || 3 < DAT_000766c4)
#define LOG_GATE3() (DAT_0007ad48 || DAT_00078e08 || 2 < DAT_000766c4)

/* ==================== 函数体 ==================== */

// === check_how_many_uart_data_in_fpga === 原 FUN_0003bb78 @ 0x0003bb78
// 来源: reversed/cgminer/10_uart_protocol/FUN_0003bb78.c | 分类: 10_uart_protocol
// 语义: 查 FPGA AXI 窗口指定 UART (0..9) 的 RX 可读字节计数; 每 32 位寄存器装两组 10 位计数,
//       偶数号 UART 取高位域、奇数号取低位域; 不支持的 uart 号打印告警并返回 0。
uint32_t check_how_many_uart_data_in_fpga(int uart)
{
    uint32_t count;

    switch (uart) {
    case 0:  /* AXI_REG_UART_RX_COUNT0 高域: 机器码 ubfx r0,r0,#0x10,#0xa (Ghidra 渲染 (v&0x3ffffff)>>0x10, 等价) */
        count = (*(volatile uint32_t *)(DAT_00078e10 + AXI_REG_UART_RX_COUNT0) & AXI_RX_COUNT_MASK) >> 0x10;
        break;
    case 1:  /* AXI_REG_UART_RX_COUNT0 低 10 位 */
        count = *(volatile uint32_t *)(DAT_00078e10 + AXI_REG_UART_RX_COUNT0) & AXI_RX_LOW_MASK;
        break;
    case 2:  /* AXI_REG_UART_RX_COUNT1 (0x1f4) 高域 */
        count = (*(volatile uint32_t *)(DAT_00078e10 + AXI_REG_UART_RX_COUNT1) & AXI_RX_COUNT_MASK) >> 0x10;
        break;
    case 3:
        count = *(volatile uint32_t *)(DAT_00078e10 + AXI_REG_UART_RX_COUNT1) & AXI_RX_LOW_MASK;
        break;
    case 4:  /* AXI_REG_UART_RX_COUNT2 (0x1f8) 高域 */
        count = (*(volatile uint32_t *)(DAT_00078e10 + AXI_REG_UART_RX_COUNT2) & AXI_RX_COUNT_MASK) >> 0x10;
        break;
    case 5:
        count = *(volatile uint32_t *)(DAT_00078e10 + AXI_REG_UART_RX_COUNT2) & AXI_RX_LOW_MASK;
        break;
    case 6:  /* AXI_REG_UART_RX_COUNT3 (0x1fc) 高域 */
        count = (*(volatile uint32_t *)(DAT_00078e10 + AXI_REG_UART_RX_COUNT3) & AXI_RX_COUNT_MASK) >> 0x10;
        break;
    case 7:
        count = *(volatile uint32_t *)(DAT_00078e10 + AXI_REG_UART_RX_COUNT3) & AXI_RX_LOW_MASK;
        break;
    case 8:  /* AXI_REG_UART_RX_COUNT4 (0x200) 高域 */
        count = (*(volatile uint32_t *)(DAT_00078e10 + AXI_REG_UART_RX_COUNT4) & AXI_RX_COUNT_MASK) >> 0x10;
        break;
    case 9:
        count = *(volatile uint32_t *)(DAT_00078e10 + AXI_REG_UART_RX_COUNT4) & AXI_RX_LOW_MASK;
        break;
    default:
        printf("%s: The uart%d is not supported!!!\n", __func__, uart);
        count = 0;
        break;
    }
    return count;
}

// === read_uart_data_in_fpga === 原 FUN_0003bd44 @ 0x0003bd44
// 来源: reversed/cgminer/10_uart_protocol/FUN_0003bd44.c | 分类: 10_uart_protocol
// 语义: 向 RX 读命令寄存器 (字节 0x210+8*uart) 写 len&0x3ff|0x80000000 发起读出, 再从 RX 数据 FIFO
//       (字索引 0x85+2*uart) 逐字按大端展开 4 字节到 buf (含不足 4 字节的尾字), 返回实际取出字节数。
//       注意: 循环次数用的是未掩码的 len (若 len>1023, 命令只带低 10 位但循环仍按 len 读 — 原样保留);
//       不支持 uart 号的告警 %d 传的是 len 而非 uart 号 (机器码核实: r2=len), 原样保留。
int read_uart_data_in_fpga(uint32_t uart, uint8_t *buf, uint32_t len)
{
    uint32_t cmd_off;      /* RX 读命令寄存器字节偏移 = 0x210 + 8*uart */
    uint32_t fifo_word;    /* RX 数据 FIFO 字索引 = 0x85 + 2*uart (字节偏移 = 索引*4) */
    uint32_t word;
    uint32_t n_words;
    uint32_t written;      /* 已写入 buf 的字节数 (返回值) */
    uint32_t i;
    uint8_t *p;

    switch (uart) {
    case 0: cmd_off = 0x210; fifo_word = 0x85; break;
    case 1: cmd_off = 0x218; fifo_word = 0x87; break;
    case 2: cmd_off = 0x220; fifo_word = 0x89; break;
    case 3: cmd_off = 0x228; fifo_word = 0x8b; break;
    case 4: cmd_off = 0x230; fifo_word = 0x8d; break;
    case 5: cmd_off = 0x238; fifo_word = 0x8f; break;
    case 6: cmd_off = 0x240; fifo_word = 0x91; break;
    case 7: cmd_off = 0x248; fifo_word = 0x93; break;
    case 8: cmd_off = 0x250; fifo_word = 0x95; break;
    case 9: cmd_off = 600;   fifo_word = 0x97; break;   /* 600 = 0x258 */
    default:
        printf("%s: The uart%d is not supported!!!\n", __func__, len);
        return 0;
    }

    /* RX 读命令: 低 10 位 = 请求字节数, bit31 = 启动 (FPGA 按请求数弹出 RX FIFO) */
    *(volatile uint32_t *)(DAT_00078e10 + cmd_off) = (len & UART_RX_READ_MASK) | AXI_BUSY_BIT31;

    /* 整字部分: 每个 FIFO 字大端展开为 4 字节: buf[0]=bit31..24, [1]=bit23..16, [2]=bit15..8, [3]=bit7..0 */
    n_words = len >> 2;
    written = n_words * 4;
    p = buf;
    for (i = 0; i < n_words; i++) {
        word = *(volatile uint32_t *)(DAT_00078e10 + fifo_word * 4);
        p[3] = (uint8_t)word;
        p[0] = (uint8_t)(word >> 0x18);
        p[1] = (uint8_t)(word >> 0x10);
        p[2] = (uint8_t)(word >> 8);
        p += 4;
    }

    /* 尾部不足 4 字节: 再读一个 FIFO 字, 只取前 rem 字节 (高位在前) */
    len &= 3;
    if (len != 0) {
        word = *(volatile uint32_t *)(DAT_00078e10 + fifo_word * 4);
        if (len == 2) {
            written += 2;
            buf[n_words * 4] = (uint8_t)(word >> 0x18);
            buf[n_words * 4 + 1] = (uint8_t)(word >> 0x10);
        }
        else if (len == 3) {
            buf[n_words * 4] = (uint8_t)(word >> 0x18);
            written += 3;
            buf[n_words * 4 + 1] = (uint8_t)(word >> 0x10);
            buf[n_words * 4 + 2] = (uint8_t)(word >> 8);
        }
        else {
            written += 1;
            buf[n_words * 4] = (uint8_t)(word >> 0x18);
        }
    }
    return (int)written;
}

// === clear_uart_send_fifo === 原 FUN_0003bf0c @ 0x0003bf0c
// 来源: reversed/cgminer/10_uart_protocol/FUN_0003bf0c.c | 分类: 10_uart_protocol
// 语义: 等待 UART TX FIFO 空闲字节数达到 0xFF (全空), 最多 21 次 × 3ms 轮询; 返回 0=已腾空,
//       否则返回最后一次读到的空闲字节数; 不支持的 uart 号返回 1。
uint32_t clear_uart_send_fifo(int uart)
{
    uint32_t space;        /* 最后一次读到的空闲字节数 (返回值) */
    uint32_t space_off;    /* TX FIFO 空闲字节数状态寄存器字节偏移 = 0x180 + (uart>>2)*4 */
    uint32_t space_shift;  /* 该 uart 的空闲字节域移位 = 24 - 8*(uart&3) (反汇编实测, 非头部宏) */
    int tries;
    pthread_mutex_t *mutex = (pthread_mutex_t *)(DAT_00078ebc + uart * 0x18);

    printf("--- %s\n", __func__);
    pthread_mutex_lock(mutex);
    switch (uart) {
    case 0: space_off = 0x180; space_shift = 0x18; break;
    case 1: space_off = 0x180; space_shift = 0x10; break;
    case 2: space_off = 0x180; space_shift = 8;    break;
    case 3: space_off = 0x180; space_shift = 0;    break;
    case 4: space_off = 0x184; space_shift = 0x18; break;
    case 5: space_off = 0x184; space_shift = 0x10; break;
    case 6: space_off = 0x184; space_shift = 8;    break;
    case 7: space_off = 0x184; space_shift = 0;    break;
    case 8: space_off = 0x188; space_shift = 0x18; break;
    case 9: space_off = 0x188; space_shift = 0x10; break;
    default:
        printf("%s: The uart%d is not supported!!!\n", __func__, uart);
        pthread_mutex_unlock(mutex);
        return 1;
    }

    tries = UART_FIFO_WAIT_TRIES;
    do {
        space = (*(volatile uint32_t *)(DAT_00078e10 + space_off) >> space_shift) & 0xff;
        if (space == 0xff) {           /* 空闲字节数满 0xFF = TX FIFO 全空 */
            pthread_mutex_unlock(mutex);
            return 0;
        }
        printf("%s: waiting fpga uart%d clear send fifo space ...\n", __func__, uart);
        usleep(UART_FIFO_WAIT_US);
        tries--;
    } while (tries != 0);
    printf("%s: uart%d always dose not has enough send fifo space, break\n", __func__, uart);
    pthread_mutex_unlock(mutex);
    return space;
}

// === clear_uart_rx_fifo === 原 FUN_0003c0a0 @ 0x0003c0a0
// 来源: reversed/cgminer/10_uart_protocol/FUN_0003c0a0.c | 分类: 10_uart_protocol
// 语义: 清空 UART RX FIFO — 查 RX 计数, 有数据则 malloc 暂存并经 read_uart_data_in_fpga 全部读出
//       丢弃, 之后延时 500ms; 返回 1=有数据已清, 0=原本就空。
int clear_uart_rx_fifo(int uart)
{
    size_t count;
    size_t got;
    void *buf;
    int ret = 0;
    pthread_mutex_t *mutex = (pthread_mutex_t *)(DAT_00078fac + uart * 0x18);

    pthread_mutex_lock(mutex);
    count = (size_t)check_how_many_uart_data_in_fpga(uart);
    if (count == 0) {
        pthread_mutex_unlock(mutex);
        return 0;
    }
    buf = malloc(count);
    if (buf != NULL) {
        got = (size_t)read_uart_data_in_fpga(uart, (uint8_t *)buf, (uint32_t)count);
        if (got != count) {
            printf("%s: uart%d clear rx fifo error. nbytes = %d, len = %d\n",
                   __func__, uart, (int)count, (int)got);
        }
        free(buf);
    }
    usleep(UART_RX_CLEAR_GAP_US);
    pthread_mutex_unlock(mutex);
    ret = 1;
    return ret;
}

// === uart_send === 原 FUN_0003c150 @ 0x0003c150
// 来源: reversed/cgminer/10_uart_protocol/FUN_0003c150.c | 分类: 10_uart_protocol
// 语义: 经 FPGA AXI 窗口向指定 UART 发送 len 字节: 等 TX FIFO 空闲字节数 ≥ len (21 次 × 3ms),
//       再等发送长度寄存器 bit31 (符号位) 清零 (发送器空闲), 数据先 memcpy 到对齐中转缓冲后按
//       4 字节组反序写入 TX FIFO 字寄存器, 最后写 len|0x80000000 触发发送; 返回 len, 失败返回 0。
//       注意: uart 号不支持的分支按原样返回 0 且不解锁 TX 互斥锁 (原版行为, 锁泄漏), 其 %d 取
//       栈上残留值 (Ghidra: in_stack_ffffedb0) 而非 uart 号。
uint32_t uart_send(int uart, void *data, uint32_t len)
{
    uint8_t midbuf[UART_TX_MIDBUF_SIZE];   /* 原 local_1228: 512 字节对齐中转缓冲 */
    char logbuf[0x1000];                   /* 原 acStack_1028 */
    uint32_t space_off;    /* TX FIFO 空闲字节数状态寄存器字节偏移 */
    uint32_t space_shift;  /* 空闲字节域移位 */
    uint32_t tx_word;      /* TX 数据 FIFO 字索引 = 0x65 + 2*uart (字节偏移 = 索引*4) */
    uint32_t len_word;     /* 发送长度/触发寄存器字索引 = 0x64 + 2*uart */
    uint32_t word;
    uint32_t n_words;
    uint32_t rem;
    uint32_t b0, b1, b2, b3;
    uint32_t i;
    int tries;
    bool ready;
    int uninit_stack_arg;  /* 原版不支持的 uart 号分支中 %d 实取栈上残留值 (非 param_1), 忠实保留为未初始化 */
    pthread_mutex_t *mutex = (pthread_mutex_t *)(DAT_00078ebc + uart * 0x18);

    memset(midbuf, 0, UART_TX_MIDBUF_SIZE);
    pthread_mutex_lock(mutex);
    switch (uart) {
    case 0: space_off = 0x180; space_shift = 0x18; tx_word = 0x65; len_word = 100; break; /* 100=0x64: 长度字(字节 0x190), 0x65=数据字(字节 0x194) */
    case 1: space_off = 0x180; space_shift = 0x10; tx_word = 0x67; len_word = 0x66; break;
    case 2: space_off = 0x180; space_shift = 8;    tx_word = 0x69; len_word = 0x68; break;
    case 3: space_off = 0x180; space_shift = 0;    tx_word = 0x6b; len_word = 0x6a; break;
    case 4: space_off = 0x184; space_shift = 0x18; tx_word = 0x6d; len_word = 0x6c; break;
    case 5: space_off = 0x184; space_shift = 0x10; tx_word = 0x6f; len_word = 0x6e; break;
    case 6: space_off = 0x184; space_shift = 8;    tx_word = 0x71; len_word = 0x70; break;
    case 7: space_off = 0x184; space_shift = 0;    tx_word = 0x73; len_word = 0x72; break;
    case 8: space_off = 0x188; space_shift = 0x18; tx_word = 0x75; len_word = 0x74; break;
    case 9: space_off = 0x188; space_shift = 0x10; tx_word = 0x77; len_word = 0x76; break;
    default:
        /* 原版: 打印后直接 return 0, 未解锁 mutex (锁泄漏, 忠实保留) */
        if (LOG_GATE5()) {
            snprintf(logbuf, 0x1000, "%s: The uart%d is not supported!!!\n", "uart_send", uninit_stack_arg);
            applog(5, logbuf, 0);
        }
        return 0;
    }

    /* 等 TX FIFO 空闲字节数 >= len, 最多 21 次 × 3ms */
    tries = UART_FIFO_WAIT_TRIES;
    while (((*(volatile uint32_t *)(DAT_00078e10 + space_off) >> space_shift) & 0xff) < len) {
        usleep(UART_FIFO_WAIT_US);
        tries--;
        if (tries == 0) {
            pthread_mutex_unlock(mutex);
            if (LOG_GATE5()) {
                snprintf(logbuf, 0x1000, "%s: uart%d always dose not has enough send fifo space, break\n",
                         "uart_send", uart);
                applog(5, logbuf, 0);
            }
            return 0;
        }
    }

    /* 预检: 发送长度寄存器 bit31 (符号位) 已清零 → 发送器空闲, 直接发送 */
    ready = ((int32_t)(*(volatile uint32_t *)(DAT_00078e10 + len_word * 4)) >= 0);
    if (!ready) {
        tries = UART_FIFO_WAIT_TRIES;
        do {
            usleep(UART_FIFO_WAIT_US);
            ready = ((int32_t)(*(volatile uint32_t *)(DAT_00078e10 + len_word * 4)) >= 0);
            if (ready) {
                break;
            }
            tries--;
        } while (tries != 0);
        if (!ready) {
            pthread_mutex_unlock(mutex);
            if (LOG_GATE5()) {
                snprintf(logbuf, 0x1000, "%s: uart%d always busy, break\n", "uart_send", uart);
                applog(5, logbuf, 0);
            }
            return 0;
        }
    }

    /* 中转拷贝后逐字写入 TX FIFO: 32 位字序反序 (小端缓冲 → 大端 FIFO 字):
     * w = b0<<24 | b1<<16 | b2<<8 | b3 (b0..b3 = 缓冲内 4 字节) */
    memcpy(midbuf, data, len);
    n_words = len >> 2;
    for (i = 0; i < n_words; i++) {
        b0 = midbuf[i * 4];
        b1 = midbuf[i * 4 + 1];
        b2 = midbuf[i * 4 + 2];
        b3 = midbuf[i * 4 + 3];
        *(volatile uint32_t *)(DAT_00078e10 + tx_word * 4) =
            (b0 << 0x18) | (b1 << 0x10) | (b2 << 8) | b3;
    }
    /* 尾部不足 4 字节: 低位补 0 后同样反序写入一次 */
    rem = len & 3;
    if (rem != 0) {
        i = n_words * 4;
        if (rem == 2) {
            word = ((uint32_t)midbuf[i] << 0x18) | ((uint32_t)midbuf[i + 1] << 0x10);
        }
        else if (rem == 3) {
            word = ((uint32_t)midbuf[i + 2] << 8) | ((uint32_t)midbuf[i + 1] << 0x10) |
                   ((uint32_t)midbuf[i] << 0x18);
        }
        else {
            word = (uint32_t)midbuf[i] << 0x18;
        }
        *(volatile uint32_t *)(DAT_00078e10 + tx_word * 4) = word;
    }

    /* 写 长度|0x80000000 触发发送 (bit31 同时作忙标志) */
    *(volatile uint32_t *)(DAT_00078e10 + len_word * 4) = len | AXI_BUSY_BIT31;
    pthread_mutex_unlock(mutex);
    return len;
}

// === reg_handle === 原 FUN_0004ae48 @ 0x0004ae48
// 来源: reversed/cgminer/10_uart_protocol/FUN_0004ae48.c | 分类: 10_uart_protocol
// 语义: ASIC 寄存器读回帧处理 — RX 解帧线程 (FUN_0004b0c0, 16_chain_mgmt 批次) 识别 RX 前导 AA 55
//       + 帧类型后把 9 字节帧体交来 (注: 原帧指针在 Ghidra 中误标 int*, 机器码核实为字节偏移):
//       帧体 [0..1]=未消费 (推断 0x09/芯片地址回显) [2]=寄存器码回显 [3..6]=4 字节值 (帧内 BE)
//             [7]=附加类型/I2C 通道 [8]=校验字节 (低 5 位);
//       calc_4bit_checksum 自帧体[2] 起共 51 位 (0x33: 帧体[2..7] 48 位 + 校验字节高 3 位),
//       校验通过则把 [帧体[2]|4B 值|帧体[7]|链号] 压入 100 槽 × 7 字节应答环形队列
//       (DAT_0007bda0, 写索引 DAT_0007bd94, 计数 DAT_0007bd9c); 计数/索引越限则打印 "Will Clean!"
//       并调 clear_reg_response_queue 清队; 校验失败打印 CRC error。
void reg_handle(uint8_t *frame, uint32_t frame_len, uint8_t chain)
{
    uint32_t crc;
    int idx;               /* 当前写索引 (DAT_0007bd94) */
    int wrapped;           /* 环绕后的索引 (idx>98 归 0), 供日志与索引更新 */
    uint8_t *entry;        /* &DAT_0007bda0[idx*7]: 7 字节槽 */
    uint32_t value;        /* 帧体[3..6] 的 4 字节 (原码为非对齐 32 位装载/存储) */
    char logbuf[0x1000];

    (void)frame_len;       /* 原函数未使用该参数 (调用点传 9), 原样保留 */

    crc = calc_4bit_checksum(frame + 2, BM1746_RESP_CRC_BITS);
    if ((frame[8] & BM1746_CHK_MASK_5BIT) == crc) {
        if ((DAT_0007bd9c < 100) && (DAT_0007bd94 < 100)) {
            pthread_mutex_lock((pthread_mutex_t *)DAT_00079b78);
            idx = DAT_0007bd94;
            entry = &DAT_0007bda0[idx * 7];
            wrapped = idx;
            if (0x62 < idx) {          /* idx > 98 (无符号比较 movhi): 环绕归 0 */
                wrapped = 0;
            }
            /* 槽[1..4] = 帧体[3..6] 4 字节值 (原码 str 非对齐存储, memcpy 逐字节等价) */
            memcpy(&value, frame + 3, 4);
            memcpy(entry + 1, &value, 4);
            entry[5] = frame[7];       /* 槽[5] = 附加类型/I2C 通道 */
            entry[6] = chain;          /* 槽[6] = 链号 (RX 线程参数) */
            if (0x62 < idx) {
                DAT_0007bd94 = wrapped;    /* 99 → 0 */
            }
            entry[0] = frame[2];       /* 槽[0] = 寄存器码回显 */
            if (idx < 99) {
                if (LOG_GATE7()) {
                    snprintf(logbuf, 0x1000, "%s: p_wr = %d reg_value_num = %d",
                             __func__, wrapped, DAT_0007bd9c);
                    applog(7, logbuf, 0);
                }
                DAT_0007bd94 = wrapped + 1;
            }
            if (DAT_0007bd9c < 100) {
                DAT_0007bd9c = DAT_0007bd9c + 1;
            }
            else {
                DAT_0007bd9c = 100;
                if (LOG_GATE4()) {
                    snprintf(logbuf, 0x1000, "%s: reg fifo full!!!", __func__);
                    applog(4, logbuf, 0);
                }
            }
            pthread_mutex_unlock((pthread_mutex_t *)DAT_00079b78);
        }
        else {
            /* 队列计数或索引越限: 告警并整体清队 (原串为栈上拼字 "Will Clean!") */
            if (LOG_GATE5()) {
                snprintf(logbuf, 0x1000, "Will Clean!");
                applog(5, logbuf, 0);
            }
            clear_reg_response_queue();
        }
    }
    else if (LOG_GATE3()) {
        snprintf(logbuf, 0x1000, "%s CRC error crc = %02x\n", __func__, crc);
        applog(3, logbuf, 0);
    }
    return;
}

// === read_dc_dc === 原 FUN_0003ca38 @ 0x0003ca38
// 来源: reversed/cgminer/10_uart_protocol/FUN_0003ca38.c | 分类: 10_uart_protocol
// 语义: 经 PIC I2C 位带通道读 DC-DC/传感器: 持总线锁发 9 字节帧 [55 AA 07 32 (slave<<1|1) reg len
//       chk_hi chk_lo] (校验和 = 帧长+0x32+读地址+reg+len 的 16 位和, 大端先高后低), 等 200ms 后
//       回读 len+5 字节 [len 回显][0x32][0x01 状态][data×len][chk_hi chk_lo], 校验命令回显与状态后
//       把 data 拷到 out; 成功返回 0, 失败 (含 len 溢出 int 的畸形输入) 打印读回前 4 字节并返回 -1。
//       读回区与日志区在原栈上连续共用 (共 0x1014 字节): len>4111 时原版即栈溢出, 忠实保留访问范围。
int read_dc_dc(uint8_t slave, int reg, uint8_t *out, uint32_t len)
{
    uint8_t frame[9];                  /* TX 帧: [0..4] 与 [5..8] 在原栈上相邻 (abStack_1044 + local_103f..103c) */
    uint8_t dcdc_buf[UART_DCDC_BUF_SIZE]; /* [0..0xF]=读回区 (原 local_1038..102c), [0x10..]=日志区 (原 acStack_1028) */
    uint32_t i;
    uint32_t ff = 0xff;
    uint8_t addr_rd;       /* I2C 读地址 = slave<<1 | 1 */
    int32_t chk;           /* 16 位校验和 (原 iVar4, int 算术) */

    frame[0] = PIC_FRAME_PREAMBLE0;    /* 0x55 */
    frame[1] = PIC_FRAME_PREAMBLE1;    /* 0xAA */
    frame[2] = PIC_DCDC_READ_LEN_BYTE; /* 长度域 = 7 */
    frame[3] = PIC_CMD_DCDC_DATA;      /* 0x32 */
    addr_rd = (uint8_t)(slave << 1) | 1;   /* I2C 读地址: 从机地址<<1 | 1 */
    frame[4] = addr_rd;
    /* (原样) 读回区预置: local_1034/1030/102c = 0, local_1038 = 0xff — 发送后会被整体清零覆盖 */
    memset(&dcdc_buf[4], 0, 12);
    /* 校验和 = len + reg + 读地址 + (帧长 7 + 命令 0x32 = 0x39): 覆盖帧内前导之后到 len 的全部字节 */
    chk = (int32_t)(len & 0xff) + reg + (int32_t)addr_rd + 0x39;
    frame[5] = (uint8_t)reg;
    frame[6] = (uint8_t)len;
    frame[7] = (uint8_t)(chk >> 8);    /* 校验和大端: 先高后低 (int 算术右移, 原 iVar4) */
    frame[8] = (uint8_t)chk;
    memcpy(&dcdc_buf[0], &ff, 4);      /* local_1038 = 0xff (LE: ff 00 00 00) */

    pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
    /* 逐字节写 I2C 位带命令: 首字节 0x55, 随后 frame[1..8] 共 9 字节 */
    pic_i2c_write_byte(0x55);
    for (i = 1; i < 9; i++) {
        pic_i2c_write_byte(frame[i]);
    }
    cgsleep_ms(200);
    memset(dcdc_buf, 0, 16);           /* 清读回区前 16 字节 (local_1038/34/30/2c = 0) */
    if ((int32_t)(len + 4) < 0) {      /* len 大到 uint+4 溢出 int — 原样保留的畸形防护, 直接走失败路径 */
        pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
    }
    else {
        /* 回读 len+5 字节: [len 回显][0x32][0x01][data×len][chk_hi chk_lo] */
        for (i = 0; i <= len + 4; i++) {
            dcdc_buf[i] = pic_i2c_read_byte();
        }
        pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
        /* 命令回显 0x32 + 状态 0x01 = 成功 (原判定: byte1==0x32 && byte2==0x01);
         * 数据位于读回区偏移 3 起 (原拷贝 acStack_1028[u-0xd] = 读回区[3+u]) */
        if (dcdc_buf[1] == PIC_CMD_DCDC_DATA && dcdc_buf[2] == 0x01) {
            if (0 < (int32_t)len) {
                for (i = 0; i < len; i++) {
                    out[i] = dcdc_buf[3 + i];
                }
            }
            return 0;
        }
    }
    if (LOG_GATE5()) {
        snprintf((char *)dcdc_buf + 0x10, 0x1000,
                 "--- %s failed! read_back_data[0] = 0x%02x, read_back_data[1] = 0x%02x, read_back_data[2] = 0x%02x, read_back_data[3] = 0x%02x\n",
                 __func__, dcdc_buf[0], dcdc_buf[1], dcdc_buf[2], dcdc_buf[3]);
        applog(5, (char *)dcdc_buf + 0x10, 0);
    }
    return -1;                          /* 0xffffffff */
}

// === send_data_to_all_asic === 原 FUN_0003e6e0 @ 0x0003e6e0
// 来源: reversed/cgminer/10_uart_protocol/FUN_0003e6e0.c | 分类: 10_uart_protocol
// 语义: 对所有在位链 (链存在标志 DAT_0009d4bc[chain]==1) 广播写 DC-DC 数据: 每链持互斥锁置当前链号
//       (DAT_0007909c, 供 pic_i2c_* 拼链选择), 逐个 I2C 从机组 (DAT_00076b48[0..2]) 调 write_dc_dc
//       写 addr+len 字节, 每次间隔 200ms。
void send_data_to_all_asic(uint32_t addr, uint8_t *data, int len)
{
    char *p_exist;         /* 原 pcVar2: 从 &DAT_0009d4bb 起步先 ++ 后判 — 即 DAT_0009d4bc[chain] */
    uint8_t slave;
    int chain;
    int asic;
    char logbuf[0x1000];

    p_exist = &DAT_0009d4bb[0];
    for (chain = 0; chain < 4; chain++) {
        p_exist++;
        if (*p_exist == 1) {           /* 链在位 */
            pthread_mutex_lock((pthread_mutex_t *)DAT_000790d0);
            DAT_0007909c = chain;
            for (asic = 0; asic < 3; asic++) {   /* 每链 3 组 I2C 从机 (语义待定: 每组覆盖关系未核实) */
                slave = DAT_00076b48[asic];
                if (LOG_GATE5()) {
                    snprintf(logbuf, 0x1000,
                             "chain%d asic%d, set data slave:%02x addr:%02x %02x %02x, len=%d\n",
                             chain, asic, slave, addr, data[0], data[1], len);
                    applog(5, logbuf, 0);
                }
                write_dc_dc(slave, (uint8_t)addr, data, len);
                cgsleep_ms(200);
            }
            pthread_mutex_unlock((pthread_mutex_t *)DAT_000790d0);
        }
    }
    return;
}

// === read_data_from_all_asic === 原 FUN_0003e814 @ 0x0003e814
// 来源: reversed/cgminer/10_uart_protocol/FUN_0003e814.c | 分类: 10_uart_protocol
// 语义: 对所有在位链的 3 组 I2C 从机逐个 read_dc_dc(slave, addr, buf, len) 读回数据并按 len 打印
//       (len==1 打 2 字节摘要, 否则打 3 字节), 每次间隔 200ms; 读前同样置当前链号。
void read_data_from_all_asic(uint32_t addr, uint8_t *buf, int len)
{
    char *p_exist;
    uint8_t slave;
    int chain;
    int asic;
    char logbuf[0x1000];

    p_exist = &DAT_0009d4bb[0];
    for (chain = 0; chain < 4; chain++) {
        p_exist++;
        if (*p_exist == 1) {
            pthread_mutex_lock((pthread_mutex_t *)DAT_000790d0);
            DAT_0007909c = chain;
            for (asic = 0; asic < 3; asic++) {
                slave = DAT_00076b48[asic];
                read_dc_dc(slave, (int)addr, buf, (uint32_t)len);
                if (len == 1) {
                    if (LOG_GATE5()) {
                        snprintf(logbuf, 0x1000, "chain%d asic%d, slave:%02x, addr:%02x %02x\n",
                                 chain, asic, slave, addr, buf[0]);
                        applog(5, logbuf, 0);
                    }
                }
                else if (LOG_GATE5()) {
                    snprintf(logbuf, 0x1000, "chain%d asic%d, slave:%02x, addr:%02x %02x %02x\n",
                             chain, asic, slave, addr, buf[0], buf[1]);
                    applog(5, logbuf, 0);
                }
                cgsleep_ms(200);
            }
            pthread_mutex_unlock((pthread_mutex_t *)DAT_000790d0);
        }
    }
    return;
}

// === stratum_send === 原 FUN_0002b5c8 @ 0x0002b5c8
// 来源: reversed/cgminer/10_uart_protocol/FUN_0002b5c8.c | 分类: 10_uart_protocol
// 语义: 矿池 stratum 发送封装 (util.c 锚点 0x765/0x768): opt_protocol+opt_debug 时打印 SEND 行,
//       持 pool+0x2D8 stratum_lock 后检查 pool+0x299 stratum_active (未激活置 rv=3), 调
//       __stratum_send 并映射 ret = (rv==0); 解锁失败则报 WTF MUTEX ERROR 并 quit(1), 解锁后调
//       全局函数指针 DAT_00075ed8 (初值 noop_hook); 按 rv 分类: 2="Failed to send"(挂起矿池),
//       3="no pool stratum_active"(不挂起), 1="Write select failed"(挂起矿池), 其余直接返回;
//       返回 1=成功, 0=失败。
int stratum_send(int *pool, char *s, int total_stratum_json)
{
    int ret;               /* 返回值: 1=成功, 0=失败 */
    uint32_t rv;           /* __stratum_send 结果: 0=OK 1=写选择失败 2=发送失败 3=链路未激活 */
    int *errno_p;
    char msg[0x1000];

    if ((DAT_0007b380 != '\0') && LOG_GATE7()) {   /* opt_protocol && opt_debug && 日志门控 */
        snprintf(msg, 0x1000, "SEND: %s", s);
        applog(7, msg, 0);
    }
    if (pthread_mutex_lock((pthread_mutex_t *)(pool + 0xb6)) != 0) {   /* +0xb6*4 = +0x2D8 stratum_lock */
        mutex_lock_fail_util("stratum_send", 0x765);
    }
    if (*(char *)((uint8_t *)pool + 0x299) == '\0') {  /* +0x299 stratum_active */
        rv = 3;
        ret = 0;
    }
    else {
        rv = __stratum_send(pool, s, total_stratum_json);
        ret = 1 - (int)rv;             /* rv==0(成功) → 1 */
        if (1 < rv) {                  /* rv>=2 → 0 (Ghidra 无符号比较) */
            ret = 0;
        }
    }
    if (pthread_mutex_unlock((pthread_mutex_t *)(pool + 0xb6)) != 0) {
        errno_p = __errno_location();
        snprintf(msg, 0x1000, "WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d",
                 *errno_p, "util.c", "stratum_send", 0x768);
        applog(3, msg, 1);             /* 第 3 参=1: 强制 curses 刷新 */
        quit(1);
    }
    (*DAT_00075ed8)();                 /* 全局函数指针 (初值 noop_hook, 恒 0, 低置信) */
    if (rv == 2) {
        if (LOG_GATE7()) {             /* 原串为栈上拼字 "Failed to send in stratum_send" */
            snprintf(msg, 0x1000, "Failed to send in stratum_send");
            applog(7, msg, 0);
        }
        suspend_stratum(pool);
        return ret;
    }
    if (rv == 3) {
        if (LOG_GATE7()) {             /* 原串为栈上拼字 "Stratum send failed due to no pool stratum_active" */
            snprintf(msg, 0x1000, "Stratum send failed due to no pool stratum_active");
            applog(7, msg, 0);
        }
        return ret;                    /* 注意: 此分支不挂起矿池 (原样) */
    }
    if (rv != 1) {
        return ret;
    }
    if (LOG_GATE7()) {
        snprintf(msg, 0x1000, "Write select failed on pool %d sock", *pool);  /* *pool = pool_no (+0x000) */
        applog(7, msg, 0);
    }
    suspend_stratum(pool);
    return ret;
}

// === status_service_thread === 原 FUN_000559d0 @ 0x000559d0
// 来源: reversed/cgminer/10_uart_protocol/FUN_000559d0.c | 分类: 10_uart_protocol
// 语义: 搜频 HTTP 应答线程 (Server: SearchFreqServer), 每个已 accept 的连接调一次: recvfrom 聚包
//       至 "\r\n\r\n" 终结 (或 DAT_0007acb8 退出标志置位提前收尾), 仅支持 GET /rate 与
//       GET /productName: /rate 按搜频状态返回 420000 / "searchfailed:search_failed_info" /
//       "searching"; /productName 返回全局 miner type 串; 组 HTTP/1.0 应答头 + 体分块 send
//       (EAGAIN 以 100ms 重试, 收发超时各 3s), 结束统一 close(fd); 返回应答体已发送字节数 (或 0)。
int status_service_thread(int sock)
{
    ssize_t n;
    char *p_find;          /* strstr 结果 (原 pcVar2) */
    int *errno_p;
    size_t body_len;       /* 应答体长度 (原 param2) */
    size_t hdr_len;        /* 应答头长度 (原 sVar6, 每轮重算) */
    int total;             /* 已聚合的请求字节数 (原 iVar8) */
    int sent;              /* 应答体已发送字节数 (原 iVar8, 返回值) */
    int hdr_acc;           /* 应答头累计偏移 (原 iVar8/iVar4/iVar5) */
    bool terminated;       /* 找到 "\r\n\r\n" 请求终结 (原 pcVar2 != NULL) */
    bool aborted;          /* 退出标志提前终止收包 (原 goto LAB_00055b50, 跳过 "find..." 日志) */
    bool body_failed;      /* 应答体 send 失败 (原 break 进 "send http data error") */
    socklen_t fromlen = 0;                 /* 原 sStack_704c: fromlen 初值 0, 不取对端地址 */
    struct sockaddr from;                  /* 原 sStack_7038 */
    struct timeval tv;                     /* 原 uStack_7040/local_703c: 3 秒收发超时 */
    static const char CRLF[] = "\r\n";     /* 原 acStack_7050: 头各行行尾 */
    static const char CRLFCRLF[] = "\r\n\r\n"; /* 原 acStack_7048: 请求终结符 (strstr 针) */
    char body[0x1000];                     /* 原 local_7028: 应答体 */
    char rsp[0x2804];                      /* 原 local_2828..local_27fc: 日志缓冲兼应答头构建区 (同栈区复用) */
    char msg2[0x1000];                     /* 原 local_6028: 发送阶段日志缓冲 */
    char rxbuf[0x2800];                    /* 原 acStack_5ffc+0xfd4: 请求聚合缓冲 */

    tv.tv_sec = 3;
    tv.tv_usec = 0;
    memset(body, 0, 0x1000);
    memset(rxbuf, 0, 0x2800);
    total = 0;
    terminated = false;
    aborted = false;
    while (!terminated) {
        if (DAT_0007acb8 != '\0') {        /* 服务退出标志: 跳出收包 (原 goto, 不打 "find..." 日志) */
            aborted = true;
            break;
        }
        n = recvfrom(sock, rxbuf + total, 0x27ff - total, 0, &from, &fromlen);
        total += (int)n;
        if (n < 1) {
            close(sock);
            if (LOG_GATE7()) {             /* 原串为栈上拼字 "statusServiceThread recvfrom<=0" */
                snprintf(rsp, 0x1000, "statusServiceThread recvfrom<=0");
                applog(7, rsp, 0);
            }
            return 0;
        }
        if (0x27fe < total) {
            close(sock);
            if (LOG_GATE7()) {             /* 原串为栈上拼字 "BUFSIZE is too small!" */
                snprintf(rsp, 0x1000, "BUFSIZE is too small!");
                applog(7, rsp, 0);
            }
            return 0;
        }
        p_find = strstr(rxbuf, CRLFCRLF);
        terminated = (p_find != NULL);
    }
    if (LOG_GATE7()) {
        if (!aborted) {                    /* 原串为栈上拼字 "find http request end flag!" */
            snprintf(rsp, 0x1000, "find http request end flag!");
            applog(7, rsp, 0);
        }
        snprintf(rsp, 0x1000, "get http=%s", rxbuf);
        applog(7, rsp, 0);
    }

    if (((rxbuf[0] == 'G') && (rxbuf[1] == 'E')) &&
        ((rxbuf[2] == 'T') && (rxbuf[3] == ' '))) {
        p_find = strstr(rxbuf + 4, "/rate");
        if (p_find != NULL) {
            if (LOG_GATE7()) {             /* 原串为栈上拼字 "cmd : get rate" */
                snprintf(rsp, 0x1000, "cmd : get rate");
                applog(7, rsp, 0);
            }
            if (DAT_00076b98 == 1) {
                snprintf(body, 0x1000, "%d", 420000);
            }
            else if (DAT_00076b98 == 2) {
                snprintf(body, 0x1000, "%s:%s", "searchfailed", "search_failed_info");
            }
            else {
                memcpy(body, "searching", 10);     /* 原栈上逐字构建 "searching\0" */
            }
        }
        else {
            p_find = strstr(rxbuf + 4, "/productName");
            if (p_find == NULL) {
                close(sock);
                if (LOG_GATE7()) {         /* 原串为栈上拼字 "statusServiceThread exit for Error cmd!" */
                    snprintf(rsp, 0x1000, "statusServiceThread exit for Error cmd!");
                    applog(7, rsp, 0);
                }
                return 0;
            }
            if (LOG_GATE7()) {             /* 原串为栈上拼字 "cmd : get miner type" */
                snprintf(rsp, 0x1000, "cmd : get miner type");
                applog(7, rsp, 0);
            }
            snprintf(body, 0x1000, DAT_00078850);  /* 全局 miner type 串直接作格式串 (原样) */
        }

        /* 收发各 3 秒超时 (SOL_SOCKET=1, SO_SNDTIMEO=0x15 / SO_RCVTIMEO=0x14, timeval 8 字节) */
        if (setsockopt(sock, 1, 0x15, &tv, 8) != 0) {
            close(sock);
            if (LOG_GATE7()) {             /* 原串为栈上 30 字节拷贝 "setsockopt SO_SNDTIMEO failed" */
                snprintf(rsp, 0x1000, "setsockopt SO_SNDTIMEO failed");
                applog(7, rsp, 0);
            }
            return 0;
        }
        if (setsockopt(sock, 1, 0x14, &tv, 8) != 0) {
            close(sock);
            if (LOG_GATE7()) {             /* 原串为栈上 30 字节拷贝 "setsockopt SO_RCVTIMEO failed" */
                snprintf(rsp, 0x1000, "setsockopt SO_RCVTIMEO failed");
                applog(7, rsp, 0);
            }
            return 0;
        }

        /* 应答头逐行 sprintf 到同一缓冲 (原版即 local_2828 日志缓冲复用); "HTTP/1.0  200  OK" 双空格为原样 */
        body_len = strlen(body);
        hdr_acc = sprintf(rsp, "HTTP/1.0  200  OK%s", CRLF);
        hdr_acc += sprintf(rsp + hdr_acc, "Server: SearchFreqServer%s", CRLF);
        hdr_acc += sprintf(rsp + hdr_acc, "Cache-Control: no-cache%s", CRLF);
        hdr_acc += sprintf(rsp + hdr_acc, "Pragma: no-cache%s", CRLF);
        hdr_acc += sprintf(rsp + hdr_acc, "Content-Type: text/plain%s", CRLF);
        hdr_acc += sprintf(rsp + hdr_acc, "Content-Length: %d%s", (int)body_len, CRLF);
        sprintf(rsp + hdr_acc, "Connection: Keep-Alive%s", CRLFCRLF);
        if (LOG_GATE7()) {                 /* 原串为栈上拼字 "send http response...\n" */
            snprintf(msg2, 0x1000, "send http response...\n");
            applog(7, msg2, 0);
        }
        if (DAT_0007acb8 == '\0') {        /* 退出标志置位则跳过头部分块发送 (原 if 包裹) */
            hdr_len = strlen(rsp);
            sent = 0;
            do {
                n = send(sock, rsp + sent, hdr_len - sent, 0);
                if (n == -1) {
                    errno_p = __errno_location();
                    if (*errno_p != 0xb) {         /* 非 EAGAIN → 发送失败收尾 (原 LAB_00056154) */
                        close(sock);
                        if (LOG_GATE7()) { /* 原串为栈上拼字 "statusServiceThread send http response error" */
                            snprintf(msg2, 0x1000, "statusServiceThread send http response error");
                            applog(7, msg2, 0);
                        }
                        return 0;
                    }
                    if (LOG_GATE7()) {     /* 原串为栈上拼字 "statusServiceThread send http timeout, try again..." */
                        snprintf(msg2, 0x1000, "statusServiceThread send http timeout, try again...");
                        applog(7, msg2, 0);
                    }
                    usleep(100000);
                }
                else {
                    if (n < 1) {           /* 发送 0 字节 → 同发送失败收尾 */
                        close(sock);
                        if (LOG_GATE7()) {
                            snprintf(msg2, 0x1000, "statusServiceThread send http response error");
                            applog(7, msg2, 0);
                        }
                        return 0;
                    }
                    sent += n;
                }
                hdr_len = strlen(rsp);     /* 原版每轮重算头长 */
            } while ((sent < (int)hdr_len) && (DAT_0007acb8 == '\0'));
        }
        if (LOG_GATE7()) {                 /* 原串为栈上拼字 "send http data..." (无换行) */
            snprintf(msg2, 0x1000, "send http data...");
            applog(7, msg2, 0);
        }

        /* 应答体分块发送: 每块打 "send http data ret=%d"; 失败走 error 日志, 完成或退出标志走 disconnected */
        sent = 0;
        body_failed = false;
        while (true) {
            n = send(sock, body + sent, body_len - (size_t)sent, 0);
            if (LOG_GATE7()) {
                snprintf(msg2, 0x1000, "send http data ret=%d", (int)n);
                applog(7, msg2, 0);
            }
            if (n < 1) {
                body_failed = true;
                break;
            }
            sent += n;
            if (((int)body_len <= sent) || (DAT_0007acb8 != '\0')) {
                break;
            }
        }
        if (body_failed) {
            if (LOG_GATE7()) {             /* 原串为栈上拼字 "statusServiceThread send http data error" */
                snprintf(msg2, 0x1000, "statusServiceThread send http data error");
                applog(7, msg2, 0);
            }
        }
        if (LOG_GATE7()) {                 /* 原串为栈上拼字 "one client disconnected!" */
            snprintf(msg2, 0x1000, "one client disconnected!");
            applog(7, msg2, 0);
        }
        close(sock);
        return sent;
    }

    /* 非 GET 请求: 关连接收尾 */
    close(sock);
    if (LOG_GATE7()) {                     /* 原串为栈上拼字 "statusServiceThread not support http command" */
        snprintf(rsp, 0x1000, "statusServiceThread not support http command");
        applog(7, rsp, 0);
    }
    return 0;
}

/* ==================== 覆盖率表 (batch=uart_b1, 11/11) ====================
 * FUN_0002b5c8 @ 0x0002b5c8 → stratum_send
 * FUN_0003bb78 @ 0x0003bb78 → check_how_many_uart_data_in_fpga
 * FUN_0003bd44 @ 0x0003bd44 → read_uart_data_in_fpga
 * FUN_0003bf0c @ 0x0003bf0c → clear_uart_send_fifo
 * FUN_0003c0a0 @ 0x0003c0a0 → clear_uart_rx_fifo
 * FUN_0003c150 @ 0x0003c150 → uart_send
 * FUN_0003ca38 @ 0x0003ca38 → read_dc_dc
 * FUN_0003e6e0 @ 0x0003e6e0 → send_data_to_all_asic
 * FUN_0003e814 @ 0x0003e814 → read_data_from_all_asic
 * FUN_0004ae48 @ 0x0004ae48 → reg_handle
 * FUN_000559d0 @ 0x000559d0 → status_service_thread
 */
