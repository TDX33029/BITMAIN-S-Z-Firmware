/*
 * cgminer 链管理层 (批次 chain_b1) — BM1746 固件语义转写
 * =========================================================
 * 原始二进制: bm1746/cgminer_z15_bm1746.elf (ARM32, SD 镜像 cgminer-z15, branch "bm1746")
 * 来源分类:   reversed/cgminer/16_chain_mgmt/ (44 个函数, 清单见 rewritten/batch_plan.tsv batch=chain_b1)
 *
 * 模块功能概述:
 *   1) util.c 时间/信号量工具簇 (上游 cgminer util.c, 行号锚点 0x520/0xf7b/0xf89/0xf8f):
 *      绝对时间纳秒睡眠 nanosleep_abstime, 毫秒/微秒 → timespec 转换, timespec 加法 timeraddspec,
 *      相对睡眠 cgsleep_ms/_us (+ 可重入 _r 版), cgtimer_time (CLOCK_MONOTONIC),
 *      cgsem 初始化/毫秒等待, cg_completion_timeout (带超时的函数异步执行)。
 *   2) BM1746 串口命令构帧 (bm1746/BM1746_uart_register_ops.md §一/§二/§四):
 *      - 11B 命令帧 makeup_cmd_frame: [55 AA][0x41|(rw&1)<<4][0x09][chip][reg][4B BE 值][chk];
 *        0x51 = 链广播写 (rw=1), 0x41 = 带芯片地址 (rw=0);
 *      - 7B 短帧: makeup_reset_frame [55 AA 53 05 00 00 chk] (复位芯片地址计数器) /
 *        makeup_address_frame [55 AA 40 05 addr 00 chk] (逐组设址) /
 *        makeup_reg_read_frame [55 AA 0x42|rw<<4 05 chip reg chk] (7B 带址读变体);
 *      - 4bit lane 校验 calc_4bit_checksum: 5 级状态位机器, 自帧[2] 起 MSB 先行逐位
 *        (11B 帧取 64 位 = 类型..值; 7B 帧取 32 位 = 类型..参数), 校验字节取低 5 位
 *        (机器码: bfc #5,#3 清保留位 + bfi #0,#5 装校验 → 帧[尾] = crc & 0x1f, 已按
 *         FUN_0003cc6c/3cd58/3cdcc/3ce7c/3cef0 反汇编逐条核实)。
 *   3) 链初始化序 (BM1746_uart_register_ops.md §四):
 *      reset_chain (PIC 断电→base[0x34] 拉低→PIC 上电→复位→释放复位→tty_init_chain 建 rx/tx 线程→
 *        base[0x3C]=0x1A→set_baud bt8d=0→base[0x3C]=0) → software_set_address_chain (复位帧+3 组设址帧) →
 *        check_every_chain_asic_number (0x50 nonce 位移前按计数模式清点) → set_core_timeout (0x28,
 *        默认 84000) / set_ticket_mask (0x14, 出厂 0x10) / set_baud (0x1C) → set_analog_mux (0x40);
 *      check_chain 从 AXI base[8] 读链在位位图, reset_all_hash_board_low 写 base[0x34]=0xffff。
 *   4) 寄存器读回应答链路: get_asic_response (rx 线程) 从 UART 收字节流, 环形缓冲内按
 *      RX 前导 AA 55 分帧: 0xE0 前缀 93B nonce 帧 → nonce_handle; 0xCC 9B PM 帧 → pm_handle;
 *      其余 9B 寄存器应答帧 → reg_handle (uart.c, 4bit 校验后压入 100 槽应答队列);
 *      check_asic_reg 发起读帧并消费应答队列 (0x20 I2C 间接应答按 4 组芯片地址分发到
 *      温度/0x11/0xFE 通道数组); clear_reg_response_queue 清队。
 *
 * 转写约定 (rewritten/CONVENTIONS.md):
 *   - 命名事实源 rewritten/name_map/cgminer.tsv; 帧格式权威文档 bm1746/BM1746_uart_register_ops.md
 *     (2026-11-20 机器码级复核版: TX 前导 55 AA / RX 前导 AA 55 / 0x51=链广播写 / 0x41=带址读 /
 *      byte[3]=0x09 / 校验字节低 5 位 / ticket mask 出厂 0x10 / 核心超时 84000)。
 *   - 反汇编复核记录 (2026-11-21, capstone; 见各函数横幅):
 *     [1] 11B/7B 构帧的校验字节 = crc & 0x1f (Ghidra 显示的 "local_12 未初始化 & 0xe0" 经反汇编
 *         核实为 bfc 清高位 + bfi 装低 5 位, 高 3 位恒 0);
 *     [2] check_asic_reg 读队列 4 字节值处机器码有 rev 指令 → 值按大端解码 (Ghidra 未渲染 rev);
 *     [3] calculate_address_interval 取 __aeabi_idivmod 的商 (0x100/round_pow2(3) = 64);
 *     [4] set_baud_pl 形参被 Ghidra 丢弃, 机器码 r0 直通 asic_baud_to_fpga_baud (调用点传 0x1a/0/bt8d);
 *     [5] thr_info_create 返回 pthread_create 结果 (Ghidra 误标 void), tty_init_chain 判其 ==0。
 *   - AXI 访问 (DAT_00078e10 + 偏移) 加 volatile, 与 uart.c 一致。
 *   - 未消化的 DAT_xxx 全局在本文件集中 extern (命名待补, 注释给出已核实语义证据);
 *     uart.c 已声明过的全局保持相同类型。
 *   - 原固件日志门控统一模式 (反编译归纳, 同 uart.c):
 *       prio 7: opt_debug && (A || B || C > 6)   prio 5: A || B || C > 4
 *       prio 4: A || B || C > 3                  prio 3: A || B || C > 2
 *     A=DAT_0007ad48, B=DAT_00078e08, C=DAT_000766c4。
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <sys/time.h>

#include "bm1746_regs.h"   /* BM1746_FRAME_*, BM1746_REG_*, BM1746_*_FRAME_TYPE 等帧常量 */

/* ==================== 外部全局 (DAT_ 保留原名, 语义见注释) ==================== */

extern uint8_t *DAT_00078e10;   /* FPGA AXI 寄存器窗口 mmap 基址 (init_fpga FUN_0003b704) */

extern char DAT_0007ad48;       /* 全局: applog (FUN_00038730) 内非 0 走 syslog() 输出; 兼作日志门控条件 A */
extern char DAT_00078e08;       /* 全局: 冗余日志开关 (curses 'v' Verbose mode / 'd' Debug mode 联动置位); 条件 B */
extern int  DAT_000766c4;       /* 全局: 日志级别 (int); 调用点以 (优先级-1) < 该值 门控 */
extern char DAT_00078e09;       /* 全局: opt_debug (curses 'd' Debug mode) */

extern char DAT_0009d4bb[];     /* +1 起 (0x9d4bc) 为链存在标志数组: 值==1 表示链在位 (与 uart.c 共享) */
extern uint8_t DAT_0009d4bc[];  /* 链存在标志数组 (0x9d4bc 起 4 字节, ==1 在位) — DAT_0009d4bb+1 别名 */
extern uint8_t DAT_00076b48[];  /* 每 ASIC 组 I2C 从机地址表: 前 3 项 = G_I2C_PER_ASIC_ADDR0/1/2 (与 uart.c 共享) */
extern char DAT_000790d0[];     /* send/read_data_to/from_all_asic + reset_chain 互斥锁 (单把, 与 uart.c 共享) */
extern int  DAT_0007909c;       /* 当前链号: pic_i2c_write/read_byte 拼入 AXI 命令 bit16.. (与 uart.c 共享) */
extern char DAT_00078fac[];     /* UART RX 互斥锁数组基址, 每 uart 0x18 字节 (与 uart.c 共享) */
extern char DAT_00079b78[];     /* 寄存器应答环形队列互斥锁 (reg_handle 写 / check_asic_reg 读) */
extern uint8_t DAT_0007bda0[];  /* 寄存器应答环形队列: 100 槽 × 7 字节 (@0x7bda0..0x7c013, 与 uart.c 共享)
                                 * 槽布局: [0]=帧体[2] (reg 回显) [1..4]=帧体[3..6] 4 字节值 (帧内 BE)
                                 *         [5]=帧体[7] (类型/I2C 通道) [6]=链号 */
extern int  DAT_0007bd94;       /* 队列写索引 (0..99) */
extern int  DAT_0007bd98;       /* 队列读索引 (0..99, check_asic_reg 推进) */
extern int  DAT_0007bd9c;       /* 未消费应答计数 (上限 100) */

/* ---- 16_chain_mgmt 批次私有全局 (地址均经反汇编/交叉引用核实) ---- */
extern uint32_t DAT_00076b4c;   /* 最近一次 set_core_timeout_chain 写入的核心超时值 (寄存器 0x28 镜像) */
extern uint32_t DAT_00076b5c;   /* 寄存器 0x1C 值镜像: (bt8d&0x1F)<<8 | 保留位 (set_baud_chain 更新) */
extern uint8_t  DAT_00076b60;   /* ticket mask 寄存器镜像 (set_ticket_mask_chain 更新; 出厂静态初值 0x10) */
extern uint8_t  DAT_00076b4b;   /* PIC 就绪标志: reset_chain 在 PIC 复位块前后清 0/置 1 */
extern uint8_t  DAT_0009d6c5[]; /* 每链 ASIC 计数 (check_asic_reg reg=0 计数模式累加; 下标=链号) */
extern uint8_t  DAT_0009d6cb;   /* 在位链计数 (check_chain 统计) */
extern uint8_t  DAT_0009d6f3;   /* 芯片地址步进 asic_addr_interval (calculate_address_interval 写入) */
extern uint8_t  DAT_0009d6f4;   /* 各链 ASIC 计数最大值 (check_asic_reg 超时收尾时刷新) */
extern uint8_t  DAT_0009d6f5;   /* 最近 set_baud 写入的 bt8d */
extern int      DAT_0007c064[]; /* rx 线程参数槽: 每链 4 字节 int, 存链号 (get_asic_response 读 *param_1) */
extern uint8_t  DAT_00079118[]; /* 每链 RX 线程运行标志 (tty_init_chain 置 1, reset_chain 清 0;
                                 * get_asic_response 每轮检查, ==0 退出) */
extern uint8_t  DAT_000790ec[]; /* 每链标志 (reset_chain 清 0) — 全局, 语义待定 */
extern uint8_t  DAT_000790fc[]; /* 每链标志 (reset_chain 置 1 → tty_init 后清 0) — 全局, 语义待定 */
extern uint8_t  DAT_00079130[3];/* 芯片组基址表 {0, interval, interval*2} (calculate_address_interval 写;
                                 * check_asic_reg reg 0x20 按此把 I2C 通道回读分发到组) */
extern char     DAT_00079134[]; /* recheck_asic_num 互斥锁 (单把, 串行化 reset_chain) */
extern uint8_t  DAT_00079b90[]; /* 每链计数模式标志 (check_asic_num_chain 置; reg=0 应答: 1=计数, 0=打印地址) */
extern uint8_t  DAT_00079b94[]; /* I2C 间接通道 0x11 回读值 (每链 4B: chain*4 + 组号; set_temperature_offset_value_chain_sensor 写) */
extern uint8_t  DAT_00079ba4[]; /* I2C 间接通道 0xFE 回读值 (每链 4B: chain*4 + 组号) */
extern uint8_t  DAT_00079bb4[]; /* I2C 间接应答就绪标志 (每链 3 组 × 4B: [通道0/1/0x11/0xFE];
                                 * read_i2c_reg 发帧前清 0, check_asic_reg reg 0x20 收到应答置 1) */
extern uint8_t  DAT_00079be4[]; /* 温度传感器偏移累计值 (每链 4B: chain*4 + 组号, calibration_sensor_offset_chain 累计) */
extern uint8_t  DAT_0009d534[]; /* TMP451 温度存储 (每链 3 组 × 8B = 2 个 int16: local/remote; 下标 = DAT_0009d534 + chain*0x18) */

/* ==================== 外部函数 (名取自 name_map/cgminer.tsv) ==================== */

extern int     *__errno_location(void);   /* glibc errno.h 自带声明, 此处显式给出以保证自洽 */
extern void     applog(uint32_t prio, char *buf, ...);    /* FUN_00038730: 第 3 参非 0 → 强制 curses 刷新 */
extern void     quit(int status);                                 /* FUN_00016ca8 */
extern void     cgsleep_us(int64_t us);                           /* FUN_0002a954 (本文件定义) — 64 位微秒 */
extern int      read_dc_dc(uint8_t slave, int reg, uint8_t *out, uint32_t len); /* FUN_0003ca38 (uart.c) */
extern uint32_t check_how_many_uart_data_in_fpga(int uart);       /* FUN_0003bb78 (uart.c) */
extern int      read_uart_data_in_fpga(uint32_t uart, uint8_t *buf, uint32_t len); /* FUN_0003bd44 (uart.c) */
extern uint32_t uart_send(int uart, void *data, uint32_t len);    /* FUN_0003c150 (uart.c) */
extern void     reg_handle(uint8_t *frame, uint32_t frame_len, uint8_t chain);  /* FUN_0004ae48 (uart.c) */
extern uint32_t asic_baud_to_fpga_baud(int baud_sel);             /* FUN_0003bb00 (15_fpga_axi) */
extern void     disable_PIC16F1704_dc_dc_new(void);               /* FUN_0003fc24 */
extern void     reset_PIC16F1704_pic_new(void);                   /* FUN_0003df78 */
extern void     jump_from_loader_to_app_PIC16F1704_new(void);     /* FUN_0003ddd0 */
extern void     enable_PIC16F1704_dc_dc_new(void);                /* FUN_0003fa7c */
extern void     cgtime(struct timeval *tv);                       /* FUN_0002a84c (20_job_mining): gettimeofday 包装 */
extern int      thr_info_create(void *thr_info, pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
                /* FUN_0002f174 (30_api_status): cgsem_init(thr+0x40) + pthread_create(thr+0x30);
                 * 返回 pthread_create 结果 (Ghidra 误标 void, tty_init_chain 判 ==0) */
extern void    *completion_thread(void *arg);                     /* FUN_0002f23c (60_util_util) */
extern void     read_i2c_reg(int chain, int chip_group, uint32_t i2c_reg, uint32_t type); /* FUN_0004886c (32_i2c_sensor) */
extern void     set_temperature_offset_value_chain_sensor(int chain, int chip_group);     /* FUN_000490fc (14_temp_fan) */
extern int      nonce_handle(void *dst1, void *dst2, uint8_t *frame, uint32_t frame_len, uint8_t chain);
                /* FUN_0004a754 (20_job_mining): 93B nonce 帧处理 (dst1/dst2 = 内部工作缓冲) */
extern void    *fill_work_thread(void *arg);      /* FUN_00042b54 (20_job_mining): 任务下发线程
                 * (arg = 栈上 {int thr_base; uint8_t chain}, tty_init_chain 构建; Ghidra 标 undefined4
                 *  返回, 作 pthread 线程体返回值未被使用, 转写为 void*) */

/* ==================== 本文件导出 ==================== */

void     nanosleep_abstime(struct timespec *abstime);                 /* FUN_0002926c */
void     thr_info_destroy(int *thr);                                  /* FUN_0002a3dc */
void     us_to_timespec(struct timespec *ts, int64_t us);             /* FUN_0002a5c0 */
void     ms_to_timespec(struct timespec *ts, int64_t ms);             /* FUN_0002a600 */
void     timeraddspec(struct timespec *dst, const struct timespec *src); /* FUN_0002a688 */
void     cgtimer_time(struct timespec *ts);                           /* FUN_0002a8b8 */
void     cgsleep_ms_r(struct timespec *base_p, int ms);               /* FUN_0002a8c4 */
void     cgsleep_us_r(struct timespec *base_p, int64_t us);           /* FUN_0002a8fc */
void     cgsleep_ms(int ms);                                          /* FUN_0002a92c */
void     _cgsem_init(sem_t *cgsem, char *file, char *func, int line); /* FUN_0002f0f4 */
int      _cgsem_mswait(sem_t *cgsem, int ms, char *file, char *func, const int line); /* FUN_0002f304 */
bool     cg_completion_timeout(void *fn, void *arg, int timeout_ms);  /* FUN_0002f438 */
void     reset_all_hash_board_low(void);                              /* FUN_0003a354 */
void     set_baud_pl(uint8_t baud_sel);                               /* FUN_0003bb44 */
int      read_uart_data_lock(int uart, void *buf, uint32_t len);      /* FUN_0003c5e8 */
uint32_t calc_4bit_checksum(uint8_t *bits, uint32_t nbits);           /* FUN_0003cc6c */
uint32_t makeup_reset_frame(uint8_t *frame);                          /* FUN_0003cd58 */
uint32_t makeup_cmd_frame(uint8_t *frame, uint8_t rw, uint8_t chip_addr, uint8_t reg, uint32_t value); /* FUN_0003cdcc */
uint32_t makeup_address_frame(uint8_t *frame, uint8_t addr);          /* FUN_0003ce7c */
uint32_t makeup_reg_read_frame(uint8_t *frame, uint8_t rw, uint8_t chip_addr, uint8_t reg); /* FUN_0003cef0 */
int      read_asic_voltage(uint8_t slave, uint16_t *volt_out);        /* FUN_0003e564 */
void     read_chain_asic_voltages(int chain);                         /* FUN_0003e5a4 */
void     send_reset_frame(uint8_t uart);                              /* FUN_00042f04 */
void     send_address_frame(uint8_t uart, uint8_t addr);              /* FUN_00042f40 */
void     software_set_address_chain(int chain);                       /* FUN_00042f7c */
void     set_core_timeout_chain(int chain, uint32_t timeout);         /* FUN_000430a0 */
void     set_analog_mux_control_chain(int chain, uint32_t sel);       /* FUN_000434a0 */
void     set_baud_chain(uint32_t chain, uint32_t bt8d);               /* FUN_00043e98 */
void     set_baud(uint32_t bt8d);                                     /* FUN_00043f04 */
void     set_ticket_mask_chain(int chain, uint32_t mask);             /* FUN_00044004 */
void     tty_init_chain(int chain, int thr_base);                     /* FUN_000445e8 */
void     check_chain(void);                                           /* FUN_00044978 */
void     reset_chain(int thr_base, uint32_t chain);                   /* FUN_00046434 */
int      round_asic_num_pow2(int asic_num);                           /* FUN_000466a4 */
void     calculate_address_interval(void);                            /* FUN_000467b0 */
void     clear_reg_response_queue(void);                              /* FUN_000481b4 */
void     check_asic_reg(uint32_t chain, uint32_t chip_addr, uint32_t reg_addr, int mode); /* FUN_000481ec */
void     calibration_sensor_offset_chain(int chain);                  /* FUN_000492b0 */
void     check_asic_num_chain(int chain, uint8_t count_mode);         /* FUN_0004952c */
void     check_every_chain_asic_number(uint32_t count_mode);          /* FUN_00049578 */
void     recheck_asic_num(int thr_base, uint32_t chain);              /* FUN_00049700 */
void     pm_handle(uint8_t *frame, int frame_len, int chain);         /* FUN_0004acec */
void    *get_asic_response(void *arg);                                /* FUN_0004b0c0 */

/* ==================== 局部命名常量 ==================== */

/* ---- BM1746 帧校验/类型 (bm1746_regs.h 未收录, 语义由 FUN_0003cef0 反汇编证实) ---- */
#define BM1746_CHK_MASK_5BIT        0x1f  /* 校验字节低 5 位有效, 高 3 位保留 (§一) */
#define BM1746_RD_FRAME_TYPE_BASE   0x42  /* 7B 带址读变体类型基值: 帧类型 = 0x42|(rw&1)<<4 — 语义待定 */
#define BM1746_SHORT_FIX_BYTE3      0x05  /* 7B 短帧偏移 3 固定值 (复位/设址/读帧) */

/* ---- 7B 短帧校验位数 / 11B 命令帧校验位数 ---- */
#define BM1746_SHORT_CHK_BITS       0x20  /* 7B 帧: 帧[2..5] 32 位 */
#define BM1746_CMD_CHK_BITS         0x40  /* 11B 帧: 帧[2..9] 64 位 (类型..值) */

/* ---- RX 应答帧 (9B) 校验位数: 帧体[2..7] 48 位 + 校验字节高 3 位 = 51 ---- */
#define BM1746_RESP_CRC_BITS        0x33

/* ---- reg 0x20 I2C 间接应答的通道码 (值 byte1, BM1746_uart_register_ops.md §二) ---- */
#define I2C_CHAN_TMP451_LOCAL       0x00  /* TMP451 local 温度 (值 byte0 = 温度+0x40) */
#define I2C_CHAN_TMP451_REMOTE      0x01  /* TMP451 remote 温度 */
#define I2C_CHAN_0X11               0x11  /* 通道 0x11 (DAT_00079b94) — 语义待定 */
#define I2C_CHAN_0XFE               0xfe  /* 通道 0xFE (DAT_00079ba4) — 语义待定 */
#define TMP451_TEMP_OFFSET          0x40  /* 温度回读偏置: 温度 = 数据 - 0x40 */

/* ---- read_asic_voltage 的 ISL 回读寄存器 ---- */
#define ISL_REG_OUT_VOLTAGE         0x21  /* ISL 电源芯片输出电压回读寄存器 — 语义待定 */

/* ---- get_asic_response 环形缓冲/帧参数 ---- */
#define ASIC_RX_RING_SIZE           0x122a0  /* RX 环形缓冲 74400 字节 (acStack_122c8) */
#define ASIC_RX_RING_WRAP           0x1229f  /* 索引超过此值归 0 */
#define ASIC_RX_READ_LEN            0x5d0    /* 每次 read_uart_data_lock 最大读出 1492 字节 */
#define ASIC_NONCE_FRAME_LEN        0x5d     /* 0xE0 前缀 nonce 帧 93 字节 */
#define ASIC_REG_FRAME_LEN          9        /* 0xCC PM 帧 / 寄存器应答帧 9 字节 */
#define ASIC_RX_POLL_US             500      /* 每轮读串口间隔 (cgsleep_us) */

/* ---- calibration_sensor_offset_chain ---- */
#define SENSOR_GROUP_NUM            3     /* 每链 3 个芯片组 */
#define SENSOR_CAL_MAX_TRIES        11    /* 单组校准最多 11 轮 */
#define SENSOR_CAL_DIFF_OK          3     /* |local-remote| < 3 视为收敛 */

/* ---- check_asic_reg 空轮询参数 ---- */
#define REG_POLL_TIMEOUT_US         300000  /* 每轮等待应答间隔 */
#define REG_POLL_RECHECK_US         100000  /* 队列空后的短间隔 */
#define REG_POLL_MAX_EMPTY          3       /* 连续 3 空轮判定超时 */

/* ---- 调用点日志门控 (统一模式, 见文件头说明) ---- */
#define LOG_GATE7() (DAT_00078e09 && (DAT_0007ad48 || DAT_00078e08 || 6 < DAT_000766c4))
#define LOG_GATE5() (DAT_0007ad48 || DAT_00078e08 || 4 < DAT_000766c4)
#define LOG_GATE4() (DAT_0007ad48 || DAT_00078e08 || 3 < DAT_000766c4)
#define LOG_GATE3() (DAT_0007ad48 || DAT_00078e08 || 2 < DAT_000766c4)

/* ==================== 函数体 ==================== */

// === nanosleep_abstime === 原 FUN_0002926c @ 0x0002926c
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0002926c.c | 分类: 16_chain_mgmt
// 语义: 绝对时间睡眠 — clock_nanosleep(CLOCK_MONOTONIC=1, TIMER_ABSTIME=1) 直到 abstime,
//       被信号打断 (EINTR=4) 则重入; 返回值仅用于判定重试。
void nanosleep_abstime(struct timespec *abstime)
{
    int ret;

    do {
        ret = clock_nanosleep(1, 1, abstime, NULL);   /* 1=CLOCK_MONOTONIC, 1=TIMER_ABSTIME */
    } while (ret == 4);                               /* 4=EINTR: 重入 */
    return;
}

// === thr_info_destroy === 原 FUN_0002a3dc @ 0x0002a3dc
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0002a3dc.c | 分类: 16_chain_mgmt
// 语义: 销毁 thr_info — join 线程句柄 (结构体 +0x30, int 指针算术 +0xc) 并清零, 随后
//       sem_destroy 信号量 (+0x40, +0x10); 空指针直接返回。布局由 thr_info_create (FUN_0002f174) 锁定。
void thr_info_destroy(int *thr)
{
    if (thr == 0) {
        return;
    }
    if (*(pthread_t *)(thr + 0xc) != 0) {          /* +0x30: pthread_t */
        pthread_join(*(pthread_t *)(thr + 0xc), NULL);
        *(uint32_t *)(thr + 0xc) = 0;
    }
    sem_destroy((sem_t *)(thr + 0x10));            /* +0x40: cgsem_t */
    return;
}

// === us_to_timespec === 原 FUN_0002a5c0 @ 0x0002a5c0
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0002a5c0.c | 分类: 16_chain_mgmt
// 语义: 微秒 → timespec: lldiv(us, 1000000), tv_sec = 商, tv_nsec = 余数*1000
//       (ARM32 timespec 字段 32 位, 原码 32 位存储)。
void us_to_timespec(struct timespec *ts, int64_t us)
{
    lldiv_t d;

    d = lldiv(us, 1000000);
    ts->tv_sec = (time_t)d.quot;          /* 原 *param_1 = (undefined4)quot */
    ts->tv_nsec = (long)(int)d.rem * 1000;
    return;
}

// === ms_to_timespec === 原 FUN_0002a600 @ 0x0002a600
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0002a600.c | 分类: 16_chain_mgmt
// 语义: 毫秒 → timespec: lldiv(ms, 1000), tv_sec = 商, tv_nsec = 余数*1000000。
void ms_to_timespec(struct timespec *ts, int64_t ms)
{
    lldiv_t d;

    d = lldiv(ms, 1000);
    ts->tv_sec = (time_t)d.quot;
    ts->tv_nsec = (long)(int)d.rem * 1000000;
    return;
}

// === timeraddspec === 原 FUN_0002a688 @ 0x0002a688
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0002a688.c | 分类: 16_chain_mgmt
// 语义: timespec 加法 dst += src, 按 int 算术: tv_nsec > 999999999 时向秒进位 (循环归一),
//       tv_nsec < 0 时向秒借位 (循环归一; 借位分支从"已更新"的 dst->tv_sec 重取, 原样保留)。
void timeraddspec(struct timespec *dst, const struct timespec *src)
{
    int sec;
    int nsec;

    sec = (int)dst->tv_sec + (int)src->tv_sec;
    dst->tv_sec = (time_t)sec;
    nsec = (int)dst->tv_nsec + (int)src->tv_nsec;
    dst->tv_nsec = nsec;
    if (999999999 < nsec) {
        do {
            sec = sec + 1;
            nsec = nsec - 1000000000;
        } while (999999999 < nsec);
        dst->tv_sec = (time_t)sec;
        dst->tv_nsec = nsec;
    }
    if (nsec < 0) {
        sec = (int)dst->tv_sec;            /* 原样: 重取已更新的 tv_sec */
        do {
            sec = sec - 1;
            nsec = nsec + 1000000000;
        } while (nsec < 0);
        dst->tv_sec = (time_t)sec;
        dst->tv_nsec = nsec;
    }
    return;
}

// === cgtimer_time === 原 FUN_0002a8b8 @ 0x0002a8b8
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0002a8b8.c | 分类: 16_chain_mgmt
// 语义: 取 CLOCK_MONOTONIC (=1) 当前时刻到 ts (cgminer cgtimer_time 的单调时钟实现)。
void cgtimer_time(struct timespec *ts)
{
    clock_gettime(1, ts);                  /* 1 = CLOCK_MONOTONIC */
    return;
}

// === cgsleep_ms_r === 原 FUN_0002a8c4 @ 0x0002a8c4
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0002a8c4.c | 分类: 16_chain_mgmt
// 语义: 可重入毫秒睡眠 — 在 base_p 时刻上追加 ms 得到绝对截止时刻并睡到该时刻
//       (ms 符号扩展为 64 位传入 ms_to_timespec, 原 FUN_0002a8c4 调用点 r2=ms, r3=ms>>31)。
void cgsleep_ms_r(struct timespec *base_p, int ms)
{
    struct timespec ts;

    ms_to_timespec(&ts, (int64_t)ms);
    timeraddspec(base_p, &ts);
    nanosleep_abstime(base_p);
    return;
}

// === cgsleep_us_r === 原 FUN_0002a8fc @ 0x0002a8fc
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0002a8fc.c | 分类: 16_chain_mgmt
// 语义: 可重入微秒睡眠 — 在 base_p 时刻上追加 us 得到绝对截止时刻并睡到该时刻
//       (Ghidra 仅显示指针参数, 64 位 us 经 r2:r3 传入, 见 FUN_0002a954 调用点)。
void cgsleep_us_r(struct timespec *base_p, int64_t us)
{
    struct timespec ts;

    us_to_timespec(&ts, us);
    timeraddspec(base_p, &ts);
    nanosleep_abstime(base_p);
    return;
}

// === cgsleep_ms === 原 FUN_0002a92c @ 0x0002a92c
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0002a92c.c | 分类: 16_chain_mgmt
// 语义: 相对毫秒睡眠 — 取单调当前时刻为基准, 调 cgsleep_ms_r 睡 ms 毫秒。
void cgsleep_ms(int ms)
{
    struct timespec ts;

    cgtimer_time(&ts);
    cgsleep_ms_r(&ts, ms);
    return;
}

// === cgsleep_us === 原 FUN_0002a954 @ 0x0002a954
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0002a954.c | 分类: 16_chain_mgmt
// 语义: 相对微秒睡眠 — 取单调当前时刻为基准, 调 cgsleep_us_r 睡 us 微秒
//       (64 位 us 占 r0:r1, get_asic_response 以 cgsleep_us(500) 轮询)。
void cgsleep_us(int64_t us)
{
    struct timespec ts;

    cgtimer_time(&ts);
    cgsleep_us_r(&ts, us);
    return;
}

// === _cgsem_init === 原 FUN_0002f0f4 @ 0x0002f0f4
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0002f0f4.c | 分类: 16_chain_mgmt
// 语义: cgsem 初始化 (util.c 锚点) — sem_init(cgsem, 0, 0); 失败打印 (file/func/line 锚点由调用方
//       传入, thr_info_create 传 0x520) 并 applog(3,..,1) + quit(1)。
void _cgsem_init(sem_t *cgsem, char *file, char *func, int line)
{
    int ret;
    int *errno_p;
    char logbuf[0x1000];

    ret = sem_init(cgsem, 0, 0);
    if (ret != 0) {
        errno_p = __errno_location();
        snprintf(logbuf, 0x1000, "Failed to sem_init ret=%d errno=%d in %s %s():%d",
                 ret, *errno_p, file, func, line);
        applog(3, logbuf, 1);
        quit(1);
    }
    return;
}

// === _cgsem_mswait === 原 FUN_0002f304 @ 0x0002f304
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0002f304.c | 分类: 16_chain_mgmt
// 语义: cgsem 带超时等待 (util.c 锚点 0xf8f) — 基准 = cgtime (gettimeofday, 转 timespec),
//       abs_timeout = ms + 基准; sem_timedwait 返回 0 成功; ETIMEDOUT(0x6e) 返回 0x6e;
//       EINTR(4) 重入 (机器码核实: 重试路径回跳到 timeraddspec, 即每次重试都把基准再累加一次
//       — 原样保留); 其他 errno 打印 "Failed to sem_timedwait..." 并 quit(1)。
int _cgsem_mswait(sem_t *cgsem, int ms, char *file, char *func, const int line)
{
    int ret;
    int err;
    int *errno_p;
    struct timespec abs_timeout;   /* 原 tStack_1030 */
    struct timespec base;          /* 原 uStack_1028/-0x1024: {sec, usec*1000} */
    struct timeval tv;             /* 原 uStack_1020/-0x101c: cgtime 目标 */
    char logbuf[0x1000];

    cgtime(&tv);
    base.tv_sec = tv.tv_sec;
    base.tv_nsec = tv.tv_usec * 1000;              /* timeval → timespec (机器码: mul r1, #0x3e8) */
    ms_to_timespec(&abs_timeout, (int64_t)ms);     /* 机器码: r2=ms, r3=ms>>31 (64 位符号扩展) */
    while (true) {
        timeraddspec(&abs_timeout, &base);         /* 原样: 在循环内 (含 EINTR 重试) 累加 */
        ret = sem_timedwait(cgsem, &abs_timeout);
        if (ret == 0) {
            return 0;
        }
        errno_p = __errno_location();
        err = *errno_p;
        if (err == 0x6e) {                         /* ETIMEDOUT → 返回 0x6e */
            break;
        }
        if (err != 4) {                            /* 非 EINTR: 致命错误 */
            snprintf(logbuf, 0x1000, "Failed to sem_timedwait errno=%d cgsem=0x%p in %s %s():%d",
                     err, cgsem, file, func, line);
            applog(3, logbuf, 1);
            quit(1);
            return 0;
        }
    }
    return 0x6e;
}

// === cg_completion_timeout === 原 FUN_0002f438 @ 0x0002f438
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0002f438.c | 分类: 16_chain_mgmt
// 语义: 带超时的函数异步执行 (util.c 锚点 0xf89/0xf8f) — malloc 0x18 工作块
//       {cgsem 16B | fn(+0x10) | arg(+0x14)}, _cgsem_init 后起 completion_thread 线程,
//       _cgsem_mswait(timeout): 到点 → pthread_cancel, 完成 → pthread_join + free;
//       返回 true = fn 在超时内完成。malloc 失败返回 false。
bool cg_completion_timeout(void *fn, void *arg, int timeout_ms)
{
    bool ret;
    struct cg_completion {          /* 原布局: malloc 0x18, sem 占前 16 字节 (ARM32 sem_t) */
        sem_t sem;
        void (*fn)(void *);
        void *arg;
    } *work;
    pthread_t th;
    int wret;

    work = malloc(0x18);
    if (work == NULL) {
        ret = false;
    }
    else {
        _cgsem_init(&work->sem, "util.c", "cg_completion_timeout", 0xf89);
        work->fn = fn;
        work->arg = arg;
        pthread_create(&th, NULL, completion_thread, work);
        wret = _cgsem_mswait(&work->sem, timeout_ms, "util.c", "cg_completion_timeout", 0xf8f);
        if (wret != 0) {
            pthread_cancel(th);
        }
        else {
            pthread_join(th, NULL);
            free(work);
        }
        ret = (wret == 0);
    }
    return ret;
}

// === calc_4bit_checksum === 原 FUN_0003cc6c @ 0x0003cc6c
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0003cc6c.c | 分类: 16_chain_mgmt
// 语义: BM1746 4-bit lane 校验 — 对 bits 起 nbits 位 (MSB 先行逐字节展开) 做 5 级状态位
//       线性状态机: 反馈 fb = s_c ^ 输入位, fb2 = s_d ^ fb; 每位后状态轮转
//       {s_c←s_b, s_d←s_a, s_b←s_e, s_a←fb, s_e←fb2}; 末位不做轮转 (机器码核实: 循环回边
//       在状态轮转之前, 末位直接出循环打包), 结果 = s_b<<4 | s_e<<3 | fb2<<2 | s_a<<1 | fb。
//       nbits==0 直接返回 0x1f (原 LAB_0003cd40 分支)。位计数器按 8 位截断 (uxtb, 原样保留;
//       调用点 nbits ≤ 64 不会触发回绕)。已用寄存器级模型与转写模型 3000 组随机位流比对一致。
//       调用点: 11B 命令帧取帧[2..9] 64 位, 7B 短帧取帧[2..5] 32 位, 9B 应答帧取帧体[2..7]+
//       校验高 3 位 51 位 — 见 make_*_frame / pm_handle / reg_handle。
uint32_t calc_4bit_checksum(uint8_t *bits, uint32_t nbits)
{
    uint32_t done;         /* 已处理位数 (原 r12, 8 位截断) */
    uint8_t bit_pos;       /* 字节内位计数 (原 r3) */
    uint32_t mask;         /* 当前位掩码, MSB 先行 (原 r2) */
    bool s_a, s_b, s_c, s_d, s_e;   /* 5 级状态位 (原 r5/r6/r7/r8/r9), 初值全 1 */
    bool fb;               /* 本次反馈位 = s_c ^ 输入位 (原 r4) */
    bool fb2;              /* s_d ^ fb (原 r10, 将装入 s_e) */
    uint32_t result;

    if (nbits == 0) {
        result = 0x1f;     /* 0 位: 全 1 (原 0x3cd40 分支) */
        return result;
    }
    s_a = true;
    s_b = true;
    s_c = true;
    s_d = true;
    s_e = true;
    done = 0;
    bit_pos = 0;
    mask = 0x80;
    do {
        fb = s_c ^ ((mask & *bits) != 0);      /* 反馈 = 状态C ^ 输入位 */
        fb2 = s_d ^ fb;
        done = (done + 1) & 0xff;              /* 原 uxtb: 8 位截断 */
        bit_pos = (uint8_t)(bit_pos + 1);
        mask >>= 1;
        if (bit_pos == 8) {                    /* 字节边界: 前移指针并复位掩码 */
            bits = bits + 1;
            bit_pos = 0;
            mask = 0x80;
        }
        if (done == nbits) {
            break;                             /* 末位: 跳过状态轮转 (原 bne 不跳) */
        }
        s_c = s_b;                             /* 状态轮转 (原 0x3cc9c 尾块) */
        s_d = s_a;
        s_b = s_e;
        s_a = fb;
        s_e = fb2;
    } while (true);
    result = 0x10;                             /* bit4 = s_b (非 0), 否则 0 */
    if (s_b == false) {
        result = 0;
    }
    if (s_e) {                                 /* bit3 */
        result = result | 8;
    }
    if (fb2) {                                 /* bit2 */
        result = result | 4;
    }
    if (s_a) {                                 /* bit1 */
        result = result | 2;
    }
    if (fb) {                                  /* bit0 */
        result = result | 1;
    }
    return result;
}

// === makeup_reset_frame === 原 FUN_0003cd58 @ 0x0003cd58
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0003cd58.c | 分类: 16_chain_mgmt
// 语义: 构造 7 字节链复位帧 [55 AA 53 05 00 00 chk] (软件设址前复位芯片地址计数器,
//       BM1746_uart_register_ops.md §四-4b)。校验对帧[2..5] 32 位计算, 校验字节 = crc&0x1f
//       (机器码: 校验槽清零后 bfi 装入低 5 位)。返回帧长 7。
uint32_t makeup_reset_frame(uint8_t *frame)
{
    uint8_t body[4];       /* 原 local_10: 帧[2..5] */
    uint32_t crc;

    body[0] = BM1747_RESET_FRAME_TYPE;      /* 0x53: 复位帧类型 */
    body[1] = BM1746_SHORT_FRAME_CMD_OFF;   /* 0x05 */
    body[2] = 0;
    body[3] = 0;
    crc = calc_4bit_checksum(body, BM1746_SHORT_CHK_BITS);
    frame[0] = BM1746_PREAMBLE_TX0;         /* 0x55 */
    frame[1] = BM1746_PREAMBLE_TX1;         /* 0xAA */
    frame[BM1746_SHORT_FRAME_ARG_OFF + 2] = (uint8_t)(crc & BM1746_CHK_MASK_5BIT);  /* 帧[6] */
    memcpy(&frame[BM1746_FRAME_OFF_TYPE], body, 4);   /* 帧[2..5] = body */
    return BM1746_SHORT_FRAME_LEN;
}

// === makeup_cmd_frame === 原 FUN_0003cdcc @ 0x0003cdcc
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0003cdcc.c | 分类: 16_chain_mgmt
// 语义: 构造 11 字节 BM1746 命令帧 (§一): [55 AA][0x41|(rw&1)<<4][0x09][chip_addr][reg]
//       [值 4B 大端][chk]。帧类型 0x51 = 链广播写 (rw=1, chip_addr=0), 0x41 = 带芯片地址 (rw=0);
//       byte[3] 固定 0x09 (反汇编 strb #9)。校验对帧[2..9] 64 位 (类型..值) 计算,
//       帧[10] = crc&0x1f (机器码: bfc #5,#3 清保留位 + bfi #0,#5 — Ghidra 显示的未初始化
//       local_12 高位实被清零)。返回帧长 11。
uint32_t makeup_cmd_frame(uint8_t *frame, uint8_t rw, uint8_t chip_addr, uint8_t reg, uint32_t value)
{
    uint32_t crc;

    frame[BM1746_FRAME_OFF_TYPE] = (uint8_t)(0x41u | (uint32_t)(rw & 1u) << 4);
        /* 0x41 | (rw&1)<<4: 0x51=链广播写, 0x41=带芯片地址 */
    frame[BM1746_FRAME_OFF_FIX] = BM1746_FRAME_FIX_BYTE3;      /* 0x09 */
    frame[BM1746_FRAME_OFF_CHIP] = chip_addr;
    frame[BM1746_FRAME_OFF_REG] = reg;
    frame[BM1746_FRAME_OFF_VALUE + 0] = (uint8_t)(value >> 24); /* 4B 值大端发送 (原 rev 指令) */
    frame[BM1746_FRAME_OFF_VALUE + 1] = (uint8_t)(value >> 16);
    frame[BM1746_FRAME_OFF_VALUE + 2] = (uint8_t)(value >> 8);
    frame[BM1746_FRAME_OFF_VALUE + 3] = (uint8_t)value;
    crc = calc_4bit_checksum(&frame[BM1746_FRAME_OFF_TYPE], BM1746_CMD_CHK_BITS);
    frame[BM1746_FRAME_OFF_CHK] = (uint8_t)(crc & BM1746_CHK_MASK_5BIT);
    return BM1746_FRAME_LEN;
}

// === makeup_address_frame === 原 FUN_0003ce7c @ 0x0003ce7c
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0003ce7c.c | 分类: 16_chain_mgmt
// 语义: 构造 7 字节设址帧 [55 AA 40 05 addr 00 chk] — 对第 addr 组芯片发地址锁存
//       (software_set_address_chain 逐组调用, addr = 组号*asic_addr_interval)。
//       校验对帧[2..5] 32 位计算, 帧[6] = crc&0x1f。返回帧长 7。
uint32_t makeup_address_frame(uint8_t *frame, uint8_t addr)
{
    uint8_t body[4];
    uint32_t crc;

    body[0] = BM1746_SETADDR_FRAME_TYPE;    /* 0x40 */
    body[1] = BM1746_SHORT_FRAME_CMD_OFF;   /* 0x05 */
    body[2] = addr;
    body[3] = 0;
    crc = calc_4bit_checksum(body, BM1746_SHORT_CHK_BITS);
    frame[0] = BM1746_PREAMBLE_TX0;
    frame[1] = BM1746_PREAMBLE_TX1;
    frame[BM1746_SHORT_FRAME_ARG_OFF + 2] = (uint8_t)(crc & BM1746_CHK_MASK_5BIT);
    memcpy(&frame[BM1746_FRAME_OFF_TYPE], body, 4);
    return BM1746_SHORT_FRAME_LEN;
}

// === makeup_reg_read_frame === 原 FUN_0003cef0 @ 0x0003cef0
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0003cef0.c | 分类: 16_chain_mgmt
// 语义: 构造 7 字节带址读帧 [55 AA (0x42|(rw&1)<<4) 05 chip_addr reg chk] — check_asic_reg
//       发起寄存器读用 (类型基值 0x42 与 11B 帧的 0x41 不同, 7B 帧固定域为 0x05;
//       bm1746_regs.h 未收录, 见 BM1746_RD_FRAME_TYPE_BASE 注释)。
//       校验对帧[2..5] 32 位计算, 帧[6] = crc&0x1f。返回帧长 7。
uint32_t makeup_reg_read_frame(uint8_t *frame, uint8_t rw, uint8_t chip_addr, uint8_t reg)
{
    uint8_t body[4];
    uint32_t crc;

    body[0] = (uint8_t)(BM1746_RD_FRAME_TYPE_BASE | (uint32_t)(rw & 1u) << 4);
    body[1] = BM1746_SHORT_FIX_BYTE3;       /* 0x05 */
    body[2] = chip_addr;
    body[3] = reg;
    crc = calc_4bit_checksum(body, BM1746_SHORT_CHK_BITS);
    frame[0] = BM1746_PREAMBLE_TX0;
    frame[1] = BM1746_PREAMBLE_TX1;
    frame[BM1746_SHORT_FRAME_ARG_OFF + 2] = (uint8_t)(crc & BM1746_CHK_MASK_5BIT);
    memcpy(&frame[BM1746_FRAME_OFF_TYPE], body, 4);
    return BM1746_SHORT_FRAME_LEN;
}

// === reset_all_hash_board_low === 原 FUN_0003a354 @ 0x0003a354
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0003a354.c | 分类: 16_chain_mgmt
// 语义: 全部算力板复位信号拉低 — AXI base[0x34] (链复位寄存器, bit n = 链 n) 写 0xffff
//       (4 条链全部拉低); 先打日志 "%s %d" ("reset_all_hash_board_low", 0x1122 — 0x1122 为
//       driver 源行号锚点, 非寄存器值)。
void reset_all_hash_board_low(void)
{
    char logbuf[0x1000];

    if (LOG_GATE5()) {
        snprintf(logbuf, 0x1000, "%s %d", "reset_all_hash_board_low", 0x1122);
        applog(5, logbuf, 0);
    }
    *(volatile uint32_t *)(DAT_00078e10 + 0x34) = 0xffff;
    return;
}

// === set_baud_pl === 原 FUN_0003bb44 @ 0x0003bb44
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0003bb44.c | 分类: 16_chain_mgmt
// 语义: 设置 PL 侧 UART 分频 — asic_baud_to_fpga_baud(baud_sel) 结果 &0x3f 后按 6 位域复制到
//       base[0x3C] 的 4 个字节低 6 位 (4 条链各一域, bit6..7 为 0; 机器码: bfi #0/#8/#0x10/#0x18,
//       各宽 6)。形参被 Ghidra 丢弃, 机器码核实 r0 直通 asic_baud_to_fpga_baud
//       (调用点传 0x1a / 0 / bt8d&0xff; asic_baud_to_fpga_baud: 0/1/0x1a 原样返回, 其余告警回 0x35)。
void set_baud_pl(uint8_t baud_sel)
{
    uint32_t baud6;        /* 原 uVar1 = 返回值 & 0x3f */
    uint32_t val;

    baud6 = asic_baud_to_fpga_baud(baud_sel) & 0x3fu;
    val = baud6 | (baud6 << 8) | (baud6 << 0x10) | (baud6 << 0x18);
    *(volatile uint32_t *)(DAT_00078e10 + 0x3c) = val;
    return;
}

// === read_uart_data_lock === 原 FUN_0003c5e8 @ 0x0003c5e8
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0003c5e8.c | 分类: 16_chain_mgmt
// 语义: 持 RX 互斥锁 (DAT_00078fac + uart*0x18) 读 UART 数据: 先查 RX 计数, 不足 len 则按实际
//       计数读 (计数为 0 返回 0), 足够则按 len 读; 返回读出字节数 (get_asic_response 轮询用)。
int read_uart_data_lock(int uart, void *buf, uint32_t len)
{
    uint32_t avail;
    uint32_t ret;
    pthread_mutex_t *mutex = (pthread_mutex_t *)(DAT_00078fac + uart * 0x18);

    pthread_mutex_lock(mutex);
    avail = check_how_many_uart_data_in_fpga(uart);
    if (avail < len) {
        if (avail == 0) {
            ret = 0;
        }
        else {
            ret = read_uart_data_in_fpga(uart, buf, avail);
        }
    }
    else {
        ret = read_uart_data_in_fpga(uart, buf, len);
    }
    pthread_mutex_unlock(mutex);
    return (int)ret;
}

// === read_asic_voltage === 原 FUN_0003e564 @ 0x0003e564
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0003e564.c | 分类: 16_chain_mgmt
// 语义: 经 PIC I2C 位带通道读单组电源芯片输出电压 — read_dc_dc(slave, 0x21, buf, 2)
//       (0x21 = ISL 输出电压回读寄存器), 2 字节小端拼成 16 位写入 *volt_out; 恒返回 0。
int read_asic_voltage(uint8_t slave, uint16_t *volt_out)
{
    uint8_t buf[2];        /* 原 local_14[4] ushort 区, 首槽预清零 */

    buf[0] = 0;
    buf[1] = 0;
    read_dc_dc(slave, ISL_REG_OUT_VOLTAGE, buf, 2);
    *volt_out = (uint16_t)buf[0] | (uint16_t)((uint16_t)buf[1] << 8);
    return 0;
}

// === read_chain_asic_voltages === 原 FUN_0003e5a4 @ 0x0003e5a4
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0003e5a4.c | 分类: 16_chain_mgmt
// 语义: 读指定链 3 组电源芯片电压并存储 — 对每 asic 组 (I2C 从机地址取 DAT_00076b48[0..2])
//       调 read_asic_voltage, 结果依次存入电压区 (0x790b6 + chain*6 起的 ushort 数组,
//       首槽未用, 实存偏移 1..3 — 原码 puVar1 先 ++ 后写), 并打 "chain %d asic = %d vol=%u"。
void read_chain_asic_voltages(int chain)
{
    uint16_t volt;
    uint16_t *p_store;     /* 原 puVar1: (ushort*)(chain*6 + 0x790b6), 全局, 语义待定 */
    int asic;
    char logbuf[0x1000];

    p_store = (uint16_t *)(uintptr_t)(chain * 6 + 0x790b6);
    for (asic = 0; asic != 3; asic++) {
        volt = 0;
        read_asic_voltage(DAT_00076b48[asic], &volt);
        p_store = p_store + 1;
        *p_store = volt;
        if (LOG_GATE5()) {
            snprintf(logbuf, 0x1000, "chain %d asic = %d vol=%u\n", chain, asic, (uint32_t)volt);
            applog(5, logbuf, 0);
        }
    }
    return;
}

// === send_reset_frame === 原 FUN_00042f04 @ 0x00042f04
// 来源: reversed/cgminer/16_chain_mgmt/FUN_00042f04.c | 分类: 16_chain_mgmt
// 语义: 向指定链 (uart 号) 发送 7 字节复位帧 — 局部 7 字节帧缓冲清零后 makeup_reset_frame
//       构帧, uart_send 发出。
void send_reset_frame(uint8_t uart)
{
    uint8_t frame[BM1746_SHORT_FRAME_LEN];     /* 原 local_10/local_c/local_a 连续 7 字节 */
    uint32_t len;

    memset(frame, 0, BM1746_SHORT_FRAME_LEN);
    len = makeup_reset_frame(frame);
    uart_send(uart, frame, len);
    return;
}

// === send_address_frame === 原 FUN_00042f40 @ 0x00042f40
// 来源: reversed/cgminer/16_chain_mgmt/FUN_00042f40.c | 分类: 16_chain_mgmt
// 语义: 向指定链发送 7 字节设址帧 [55 AA 40 05 addr 00 chk] — addr 形参被 Ghidra 丢弃,
//       机器码核实 r1 直通 makeup_address_frame (调用方 software_set_address_chain 传组号*步进)。
void send_address_frame(uint8_t uart, uint8_t addr)
{
    uint8_t frame[BM1746_SHORT_FRAME_LEN];
    uint32_t len;

    memset(frame, 0, BM1746_SHORT_FRAME_LEN);
    len = makeup_address_frame(frame, addr);
    uart_send(uart, frame, len);
    return;
}

// === software_set_address_chain === 原 FUN_00042f7c @ 0x00042f7c
// 来源: reversed/cgminer/16_chain_mgmt/FUN_00042f7c.c | 分类: 16_chain_mgmt
// 语义: 链软件设址 (§四-4b) — 打印 "%s chainID%d asics %d addrInterval %d" (asic 数取
//       DAT_0009d6c5[chain], 步进取 DAT_0009d6f3), 发复位帧 [55 AA 53 05 00 00 chk] 复位芯片
//       地址计数器, 等 50ms 后对 3 个芯片组逐组发设址帧 (组地址 = 组号*asic_addr_interval),
//       每组间隔 50ms。
void software_set_address_chain(int chain)
{
    int group;             /* 原 iVar2 */
    uint32_t group_addr;   /* 原 uVar1 = group * interval */
    char logbuf[0x1000];

    if (LOG_GATE5()) {
        snprintf(logbuf, 0x1000, "%s chainID%d asics %d addrInterval %d",
                 "software_set_address_chain", chain, (uint32_t)DAT_0009d6c5[chain],
                 (uint32_t)DAT_0009d6f3);
        applog(5, logbuf, 0);
    }
    group = 0;
    send_reset_frame(chain);
    cgsleep_ms(0x32);
    do {
        group_addr = (uint32_t)group * (uint32_t)DAT_0009d6f3;
        group = group + 1;
        send_address_frame(chain, (uint8_t)(group_addr & 0xff));
        cgsleep_ms(0x32);
    } while (group != 3);
    return;
}

// === set_core_timeout_chain === 原 FUN_000430a0 @ 0x000430a0
// 来源: reversed/cgminer/16_chain_mgmt/FUN_000430a0.c | 分类: 16_chain_mgmt
// 语义: 整链广播写核心超时 (reg 0x28, 默认 84000=0x14820) — 先记入镜像 DAT_00076b4c,
//       opt_debug 门控下打 "chainid%d %s: core timeout = %08x" (prio 7), 构 0x51 广播写帧发送,
//       等 1ms。
void set_core_timeout_chain(int chain, uint32_t timeout)
{
    uint8_t frame[BM1746_FRAME_LEN];
    char logbuf[0x1000];

    memset(frame, 0, BM1746_FRAME_LEN);    /* 原 uStack_101c..local_1012 连续 11 字节清零 */
    if ((DAT_00078e09 != '\0') && LOG_GATE7()) {
        snprintf(logbuf, 0x1000, "chainid%d %s: core timeout = %08x", chain,
                 "set_core_timeout_chain", timeout);
        applog(7, logbuf, 0);
    }
    DAT_00076b4c = timeout;
    makeup_cmd_frame(frame, 1, 0, BM1746_REG_CORE_TIMEOUT, timeout);
    uart_send(chain, frame, 0xb);
    cgsleep_ms(1);
    return;
}

// === set_analog_mux_control_chain === 原 FUN_000434a0 @ 0x000434a0
// 来源: reversed/cgminer/16_chain_mgmt/FUN_000434a0.c | 分类: 16_chain_mgmt
// 语义: 整链广播写模拟多路开关 (reg 0x40, 3bit 温度通道选择) — 打印
//       "chainid%d %s: analog mux = %08x" (prio 5, 值取 sel&7), 构 0x51 广播写帧发送, 等 1ms。
void set_analog_mux_control_chain(int chain, uint32_t sel)
{
    uint8_t frame[BM1746_FRAME_LEN];
    char logbuf[0x1000];

    memset(frame, 0, BM1746_FRAME_LEN);
    if (LOG_GATE5()) {
        snprintf(logbuf, 0x1000, "chainid%d %s: analog mux = %08x", chain,
                 "set_analog_mux_control_chain", sel & 7);
        applog(5, logbuf, 0);
    }
    makeup_cmd_frame(frame, 1, 0, BM1746_REG_ANA_MUX, sel & 7);
    uart_send(chain, frame, 0xb);
    cgsleep_ms(1);
    return;
}

// === set_baud_chain === 原 FUN_00043e98 @ 0x00043e98
// 来源: reversed/cgminer/16_chain_mgmt/FUN_00043e98.c | 分类: 16_chain_mgmt
// 语义: 单链写 UART 波特率分频 (reg 0x1C) — 镜像 DAT_00076b5c = (旧值 & ~0x1f00) |
//       (bt8d&0x1F)<<8 (bfi 5 位域, 保留其余位; §二: (bt8d&0x1F)<<8 | 保留位), 构 0x51 广播写帧
//       (值 = 更新后的镜像) 发送, 等 1ms。无日志。
void set_baud_chain(uint32_t chain, uint32_t bt8d)
{
    uint8_t frame[BM1746_FRAME_LEN];

    DAT_00076b5c = (DAT_00076b5c & 0xffffe0ffu) | ((bt8d & 0x1fu) << 8);
    memset(frame, 0, BM1746_FRAME_LEN);    /* 原 local_1c..local_12 连续 11 字节清零 */
    makeup_cmd_frame(frame, 1, 0, BM1746_REG_BAUD_TEMP, DAT_00076b5c);
    uart_send(chain, frame, 0xb);
    cgsleep_ms(1);
    return;
}

// === set_baud === 原 FUN_00043f04 @ 0x00043f04
// 来源: reversed/cgminer/16_chain_mgmt/FUN_00043f04.c | 分类: 16_chain_mgmt
// 语义: 全链设置波特率 — bt8d 记入 DAT_0009d6f5, opt_debug 门控下打 "%s: bt8d = %d" (prio 7),
//       对全部在位链 (DAT_0009d4bc[0..3]==1) 调 set_baud_chain; sleep(1) 后 set_baud_pl(bt8d&0xff)
//       (机器码核实: r0 = uxtb(bt8d) 直通 asic_baud_to_fpga_baud), 等 10ms。
void set_baud(uint32_t bt8d)
{
    int chain;             /* 原 cVar1 (8 位循环计数) */
    char logbuf[0x1000];

    DAT_0009d6f5 = (uint8_t)bt8d;
    if ((DAT_00078e09 != '\0') && LOG_GATE7()) {
        snprintf(logbuf, 0x1000, "%s: bt8d = %d", "set_baud", bt8d);
        applog(7, logbuf, 0);
    }
    chain = 0;
    do {
        if (DAT_0009d4bc[chain] == 1) {            /* 链在位 (原 pcVar2 先 ++ 后判 → 0x9d4bc 起) */
            set_baud_chain(chain, bt8d);
        }
        chain = chain + 1;
    } while (chain != 4);
    sleep(1);
    set_baud_pl((uint8_t)(bt8d & 0xff));
    cgsleep_ms(10);
    return;
}

// === set_ticket_mask_chain === 原 FUN_00044004 @ 0x00044004
// 来源: reversed/cgminer/16_chain_mgmt/FUN_00044004.c | 分类: 16_chain_mgmt
// 语义: 整链广播写 ticket mask (reg 0x14, 8bit, 出厂静态初值 0x10) — 镜像 DAT_00076b60 =
//       mask&0xff (机器码: bfc #8,#0x18 清高位 + bfi #0,#8 装低 8 位), 打印
//       "%s chainID%d ticket_mask = 0x%08x" (prio 5), 构 0x51 广播写帧发送, 等 1ms。
void set_ticket_mask_chain(int chain, uint32_t mask)
{
    uint8_t frame[BM1746_FRAME_LEN];
    char logbuf[0x1000];

    memset(frame, 0, BM1746_FRAME_LEN);
    if (LOG_GATE5()) {
        snprintf(logbuf, 0x1000, "%s chainID%d ticket_mask = 0x%08x", "set_ticket_mask_chain",
                 chain, mask);
        applog(5, logbuf, 0);
    }
    DAT_00076b60 = mask & 0xff;
    makeup_cmd_frame(frame, 1, 0, BM1746_REG_TICKET_MASK, DAT_00076b60);
    uart_send(chain, frame, 0xb);
    cgsleep_ms(1);
    return;
}

// === tty_init_chain === 原 FUN_000445e8 @ 0x000445e8
// 来源: reversed/cgminer/16_chain_mgmt/FUN_000445e8.c | 分类: 16_chain_mgmt
// 语义: 链串口线程初始化 — 链不在位直接返回; 把链号写入 rx 线程参数槽 DAT_0007c064[chain],
//       置 RX 运行标志 DAT_00079118[chain]=1 (get_asic_response 的退出条件), 打印
//       "%s chainid = %d"; thr_info_create 启 rx 线程 (get_asic_response, thr_info 槽
//       thr_base+chain*0x40+0x37a, arg = &DAT_0007c064[chain]), 等 50ms; 启 tx 线程
//       (fill_work_thread, 槽 +0x27a, arg = 栈上 {thr_base(+0), chain(+4)}), 等 200ms;
//       两个 create 分别按 prio 3 打 "create rx/tx read thread for chain %d ok/failed";
//       尾部按原 goto 结构打印 "open device over" (prio 5, 栈上逐字构建的串):
//       rx ok 且未走级别检查时无条件打印 (原 LAB_000447fc 仅存在于 ok-已打日志/failed 路径),
//       走检查时 DAT_000766c4 < 5 跳过 (原 LAB_000447e4)。末尾等 10ms。
//       注: thr_info_create 返回 pthread_create 结果 (Ghidra 误标 void), ==0 视为成功。
void tty_init_chain(int chain, int thr_base)
{
    int rx_ret;
    int tx_ret;
    struct {               /* 原 iStack_1028(+0)/local_1024(+4): tx 线程参数 */
        int thr_base;
        uint8_t chain;
    } tx_arg;
    char logbuf[0x1000];

    if (DAT_0009d4bc[chain] == 0) {
        return;
    }
    DAT_0007c064[chain] = chain;               /* rx 线程参数: 每 4 字节槽存链号 */
    if (LOG_GATE5()) {
        snprintf(logbuf, 0x1000, "%s chainid = %d", "tty_init_chain", chain);
        applog(5, logbuf, 0);
    }
    DAT_00079118[chain] = 1;                   /* RX 线程运行标志 */
    rx_ret = thr_info_create((void *)(uintptr_t)(thr_base + chain * 0x40 + 0x37a), NULL,
                             get_asic_response, &DAT_0007c064[chain]);
    if (rx_ret == 0) {
        if (LOG_GATE3()) {                     /* 原串为栈上拼字 */
            snprintf(logbuf, 0x1000, "create rx read thread for chain %d ok", chain);
            applog(3, logbuf, 0);
        }
    }
    else if (LOG_GATE3()) {
        snprintf(logbuf, 0x1000, "create rx read thread for chain %d failed", chain);
        applog(3, logbuf, 0);
    }
    cgsleep_ms(0x32);
    tx_arg.thr_base = thr_base;
    tx_arg.chain = (uint8_t)chain;
    tx_ret = thr_info_create((void *)(uintptr_t)(thr_base + chain * 0x40 + 0x27a), NULL,
                             fill_work_thread, &tx_arg);
    cgsleep_ms(200);
    if (tx_ret == 0) {
        if (LOG_GATE3()) {
            snprintf(logbuf, 0x1000, "create tx read thread for chain %d ok", chain);
            applog(3, logbuf, 0);
            if ((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) {
                goto lvl_check;                /* 原 LAB_000447fc */
            }
        }
        /* ok 且未走级别检查 → 无条件落到 "open device over" (原控制流) */
    }
    else if (LOG_GATE3()) {
        snprintf(logbuf, 0x1000, "create tx read thread for chain %d failed", chain);
        applog(3, logbuf, 0);
        if ((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) {
            goto lvl_check;
        }
        /* failed 且已打日志且 A/B 置位 → 落到 "open device over" (原 LAB_0004478c 复用) */
    }
    else {
lvl_check:                                     /* 原 LAB_000447fc */
        if (DAT_000766c4 < 5) {
            goto done;                         /* 原 LAB_000447e4 */
        }
    }
    /* 原栈上逐字构建的串 "open device over" */
    snprintf(logbuf, 0x1000, "open device over");
    applog(5, logbuf, 0);
done:
    cgsleep_ms(10);
    return;
}

// === check_chain === 原 FUN_00044978 @ 0x00044978
// 来源: reversed/cgminer/16_chain_mgmt/FUN_00044978.c | 分类: 16_chain_mgmt
// 语义: 链在位探测 — 打印 "check_chain" (prio 5, 原栈上逐字构建), 读 AXI base[8] 链在位
//       状态字 (bit n = 链 n), 清在位链计数 DAT_0009d6cb 后逐链写存在标志 DAT_0009d4bc[n]
//       (bit 置位 → 1 并计数, 打 "Chain %d existed!"; 未置位 → 0)。
void check_chain(void)
{
    uint32_t chain;        /* 原 param3 */
    uint32_t plug_status;  /* 原 iVar2: base[8] 位图 */
    uint8_t *p_exist;      /* 原 puVar3: &DAT_0009d4b4 起步, 每轮 +1 (标志槽 = +8 字节) */
    char logbuf[0x1000];

    if (LOG_GATE5()) {                         /* 原栈上逐字构建 "check_chain" */
        snprintf(logbuf, 0x1000, "check_chain");
        applog(5, logbuf, 0);
    }
    chain = 0;
    p_exist = (uint8_t *)DAT_0009d4bb;         /* +8 字节 = &DAT_0009d4bc[chain] */
    DAT_0009d6cb = 0;
    plug_status = *(volatile uint32_t *)(DAT_00078e10 + 8);   /* 链在位状态寄存器 (字偏移 2) */
    do {
        if (((plug_status >> (chain & 0xff)) & 1U) == 0) {
            p_exist[8] = 0;
        }
        else {
            p_exist[8] = 1;
            DAT_0009d6cb = DAT_0009d6cb + 1;
            if (LOG_GATE5()) {
                snprintf(logbuf, 0x1000, "Chain %d existed!", chain);
                applog(5, logbuf, 0);
            }
        }
        chain = chain + 1;
        p_exist = p_exist + 1;
    } while (chain != 4);
    return;
}

// === reset_chain === 原 FUN_00046434 @ 0x00046434
// 来源: reversed/cgminer/16_chain_mgmt/FUN_00046434.c | 分类: 16_chain_mgmt
// 语义: 单链硬件复位全流程 (§四-2a) —
//   1) 打 "%s chainid %d" (prio 4); 持 all_asic 锁置当前链号后 disable_PIC16F1704_dc_dc_new (PIC 断电);
//   2) 清每链标志 (DAT_000790ec=0, DAT_00079118 RX 运行标志=0, DAT_000790fc=1), 销毁 tx/rx 线程
//      (thr_info_destroy, 槽 +0x27a / +0x37a);
//   3) DAT_00076b4b=0; 持锁置链号 → reset_PIC16F1704_pic_new (PIC 复位) → 等 1s →
//      jump_from_loader_to_app_PIC16F1704_new (loader→app) → 等 1s → 解锁; DAT_00076b4b=1;
//   4) 打 "%s %d" ("reset_hash_board_low"); base[0x34] = 1<<chain (链复位拉低), 等 100ms;
//   5) enable_PIC16F1704_dc_dc_new (PIC 上电), sleep(1);
//   6) 打 "%s %d" ("reset_hash_board_high"); base[0x34] = 0 (释放复位); DAT_000790fc=0;
//   7) tty_init_chain(chain, thr_base) 重建 rx/tx 线程;
//   8) 切高速 (§四-2a 尾注, 先于设址): set_baud_pl(0x1a) → base[0x3C]=0x1A 复制 → 等 100ms →
//      set_baud_chain(chain, 0) (reg 0x1C bt8d=0) → 等 1s → set_baud_pl(0) → base[0x3C]=0 → 等 100ms。
void reset_chain(int thr_base, uint32_t chain)
{
    char logbuf[0x1000];

    if (LOG_GATE3()) {
        snprintf(logbuf, 0x1000, "%s chainid %d", "reset_chain", chain);
        applog(4, logbuf, 0);
    }
    pthread_mutex_lock((pthread_mutex_t *)DAT_000790d0);
    DAT_0007909c = chain;
    disable_PIC16F1704_dc_dc_new();            /* PIC 命令 5/0x15/0x1A: 关 DC-DC */
    pthread_mutex_unlock((pthread_mutex_t *)DAT_000790d0);
    DAT_000790ec[chain] = 0;
    DAT_00079118[chain] = 0;                   /* RX 线程退出标志 */
    DAT_000790fc[chain] = 1;
    thr_info_destroy((int *)(uintptr_t)(thr_base + chain * 0x40 + 0x27a));   /* tx 线程 (字节偏移 0x9E8) */
    thr_info_destroy((int *)(uintptr_t)(thr_base + chain * 0x40 + 0x37a));   /* rx 线程 (字节偏移 0xDE8) */
    DAT_00076b4b = 0;
    pthread_mutex_lock((pthread_mutex_t *)DAT_000790d0);
    DAT_0007909c = chain;
    reset_PIC16F1704_pic_new();                /* PIC 命令 4/07: PIC 自身复位 */
    cgsleep_ms(1000);
    jump_from_loader_to_app_PIC16F1704_new();  /* PIC 命令 4/06: loader→app */
    cgsleep_ms(1000);
    pthread_mutex_unlock((pthread_mutex_t *)DAT_000790d0);
    DAT_00076b4b = 1;
    if (LOG_GATE4()) {
        snprintf(logbuf, 0x1000, "%s %d", "reset_hash_board_low", chain);
        applog(5, logbuf, 0);
    }
    *(volatile uint32_t *)(DAT_00078e10 + 0x34) = 1 << (chain & 0xff);   /* 复位拉低 */
    cgsleep_ms(100);
    pthread_mutex_lock((pthread_mutex_t *)DAT_000790d0);
    DAT_0007909c = chain;
    enable_PIC16F1704_dc_dc_new();             /* PIC 命令 5/0x15/0x1B: 开 DC-DC */
    pthread_mutex_unlock((pthread_mutex_t *)DAT_000790d0);
    sleep(1);
    if (LOG_GATE4()) {
        snprintf(logbuf, 0x1000, "%s %d", "reset_hash_board_high", chain);
        applog(5, logbuf, 0);
    }
    *(volatile uint32_t *)(DAT_00078e10 + 0x34) = 0;                     /* 释放复位 */
    DAT_000790fc[chain] = 0;
    tty_init_chain((int)chain, thr_base);
    set_baud_pl(0x1a);                         /* base[0x3C] = 0x1A 复制 (PL 初始分频) */
    cgsleep_ms(100);
    set_baud_chain(chain, 0);                  /* reg 0x1C, bt8d = 0 */
    cgsleep_ms(1000);
    set_baud_pl(0);                            /* base[0x3C] = 0 (divider 0) */
    cgsleep_ms(100);
    return;
}

// === round_asic_num_pow2 === 原 FUN_000466a4 @ 0x000466a4
// 来源: reversed/cgminer/16_chain_mgmt/FUN_000466a4.c | 分类: 16_chain_mgmt
// 语义: ASIC 数向上取 2 的幂 (地址空间划分用) — 1/2 原样返回; 3..4→4, 5..8→8, 9..16→16,
//       17..32→32, 33..64→64, 65..128→128 (原码按 (n-k) 无符号区间判定); 其余 (0/负数/>128)
//       返回 -1, opt_debug 且日志门控满足时先打 "actual_asic_number = %d, but it is error" (prio 7)。
int round_asic_num_pow2(int asic_num)
{
    char logbuf[0x1000];

    if ((asic_num != 1) && (asic_num != 2)) {
        if ((uint32_t)(asic_num - 3) < 2u) {
            asic_num = 4;
        }
        else if ((uint32_t)(asic_num - 5) < 4u) {
            asic_num = 8;
        }
        else if ((uint32_t)(asic_num - 9) < 8u) {
            asic_num = 0x10;
        }
        else if ((uint32_t)(asic_num - 0x11) < 0x10u) {
            asic_num = 0x20;
        }
        else if ((uint32_t)(asic_num - 0x21) < 0x20u) {
            asic_num = 0x40;
        }
        else if ((uint32_t)(asic_num - 0x41) < 0x40u) {
            asic_num = 0x80;
        }
        else if ((DAT_00078e09 == '\0') || (!LOG_GATE7())) {
            asic_num = -1;
        }
        else {
            snprintf(logbuf, 0x1000, "actual_asic_number = %d, but it is error\n", asic_num);
            applog(7, logbuf, 0);
            asic_num = -1;
        }
    }
    return asic_num;
}

// === calculate_address_interval === 原 FUN_000467b0 @ 0x000467b0
// 来源: reversed/cgminer/16_chain_mgmt/FUN_000467b0.c | 分类: 16_chain_mgmt
// 语义: 计算芯片地址步进 — asic_addr_interval DAT_0009d6f3 = 0x100 / round_asic_num_pow2(3)
//       (round_pow2(3)=4 → 64; 机器码核实取 __aeabi_idivmod 的商, uxtb 截断后字节存储),
//       打印 "%s:addrInterval = %d" (prio 5), 并写芯片组基址表 DAT_00079130[] =
//       {0, interval, interval*2} (check_asic_reg reg 0x20 按 3 组地址分发 I2C 应答)。
void calculate_address_interval(void)
{
    uint32_t interval;     /* 原 param4 */
    char logbuf[0x1000];

    interval = 0x100 / round_asic_num_pow2(3);
    DAT_0009d6f3 = (uint8_t)interval;
    if (LOG_GATE5()) {
        snprintf(logbuf, 0x1000, "%s:addrInterval = %d", "calculate_address_interval", interval);
        applog(5, logbuf, 0);
        interval = (uint32_t)DAT_0009d6f3;
    }
    DAT_00079130[1] = (uint8_t)interval;
    DAT_00079130[2] = (uint8_t)(interval << 1);
    DAT_00079130[0] = 0;
    return;
}

// === clear_reg_response_queue === 原 FUN_000481b4 @ 0x000481b4
// 来源: reversed/cgminer/16_chain_mgmt/FUN_000481b4.c | 分类: 16_chain_mgmt
// 语义: 清空寄存器应答环形队列 — 持队列互斥锁把写索引 DAT_0007bd94 / 读索引 DAT_0007bd98 /
//       未消费计数 DAT_0007bd9c 归零 (uart.c reg_handle 生产, check_asic_reg 消费)。
void clear_reg_response_queue(void)
{
    pthread_mutex_lock((pthread_mutex_t *)DAT_00079b78);
    DAT_0007bd94 = 0;
    DAT_0007bd98 = 0;
    DAT_0007bd9c = 0;
    pthread_mutex_unlock((pthread_mutex_t *)DAT_00079b78);
    return;
}

// === check_asic_reg === 原 FUN_000481ec @ 0x000481ec
// 来源: reversed/cgminer/16_chain_mgmt/FUN_000481ec.c | 分类: 16_chain_mgmt
// 语义: 发起寄存器读并消费应答队列 — 清队后 opt_debug 门控下打 "check_asic_reg: check chain
//       J%d mode %d chipaddr %02x regaddr %02x" (prio 7); makeup_reg_read_frame 构 7B 帧发送
//       (type = 0x42|(mode&1)<<4, chip_addr/reg 回显); reg==0 且 DAT_00079b90[chain] 非零时先清
//       ASIC 计数 DAT_0009d6c5[chain]。随后循环: 等 300ms 查队列计数, 空 3 轮 (每轮 +100ms) 判
//       超时 — reg==0 时把最大 ASIC 数记入 DAT_0009d6f4 并打 "chain J%d has %d ASIC" (prio 5),
//       清队返回; 有应答则持锁逐条消费 pending 条: 槽[6]==链号时按 reg 分派 —
//         0x00: 计数模式 DAT_00079b90[chain]==1 → DAT_0009d6c5[chain]++ (每应答 +1),
//               否则打 "the asic address is 0x%08x";
//         0x0C/0x14/0x1C/0x54/0x30: 打 freq/tm/misc/start nonce offset/IO strength 日志 (prio 5/门控);
//         0x20: I2C 间接应答分发 — 值按大端解码 (机器码 rev, Ghidra 未渲染), 值高 2 位为 0 才处理:
//               通道 = 值 byte1 (0/1/0x11/0xFE), 数据 = 值 byte0; 槽[5] (类型) 等于哪个组基址
//               (DAT_00079130[group], 共 3 组) 就写入该组: 通道 0/1 → 温度 short
//               DAT_0009d534[chain*24 + group*8 (+2)] = 数据-0x40 (TMP451 偏置), 就绪标志
//               DAT_00079bb4[chain*12 + group*4 (+0/+1)] = 1; 通道 0x11 → DAT_00079b94[chain*4+group],
//               通道 0xFE → DAT_00079ba4[chain*4+group] (标志 +2/+3);
//       槽[6] 链号不匹配则打 "the return data is from chain%d, but it should be from chain%d..."。
//       每条消费后读索引 +1 (到 100 归 0)、计数 -1; 消费完一轮把空轮计数清零继续等待。
void check_asic_reg(uint32_t chain, uint32_t chip_addr, uint32_t reg_addr, int mode)
{
    uint8_t frame[BM1746_SHORT_FRAME_LEN];   /* 原 uStack_1030 区 7 字节 */
    char logbuf[0x1000];
    uint32_t wait_tries;   /* 原 uVar9: 空轮计数 (消费段复用为值 byte1/通道) */
    int pending;           /* 原 iVar5: 进入消费段时的队列计数 */
    uint32_t consumed;     /* 原 iVar10 */
    int slot_off;          /* 原 iVar3: 读索引*7 (槽内字节偏移) */
    uint32_t value_raw;    /* 槽[1..4] 4 字节小端装载 (原 *(uint*)(&DAT_0007bda1+off)) */
    uint32_t value;        /* 大端解码后的寄存器值 (机器码 rev 后的 r3) */
    uint8_t type_byte;     /* 槽[5] (I2C 通道/类型回显) */
    uint8_t resp_chain;    /* 槽[6] (链号) */
    uint32_t channel;      /* I2C 通道码 = 值 byte1 */
    uint8_t data;          /* I2C 数据 = 值 byte0 */
    uint8_t *flags;        /* DAT_00079bb4 + chain*12: 每组 4 字节就绪标志 (原 puVar7) */
    int16_t *temps;        /* DAT_0009d534 + chain*24: 每组 2 个温度 short (原 psVar11) */
    int group;             /* I2C 应答组号 */

    memset(frame, 0, BM1746_SHORT_FRAME_LEN);
    clear_reg_response_queue();
    if ((DAT_00078e09 != '\0') && LOG_GATE7()) {
        snprintf(logbuf, 0x1000, "%s: check chain J%d mode %d chipaddr %02x regaddr %02x",
                 "check_asic_reg", chain, mode, chip_addr, reg_addr);
        applog(7, logbuf, 0);
    }
    uart_send(chain, frame, makeup_reg_read_frame(frame, (uint8_t)mode, (uint8_t)chip_addr,
                                                  (uint8_t)reg_addr));
    if ((reg_addr == 0) && (DAT_00079b90[chain] != 0)) {
        DAT_0009d6c5[chain] = 0;           /* 重新清点前清零 ASIC 计数 */
    }
    wait_tries = 0;
    for (;;) {
        for (;;) {                          /* 等待应答: 连续 3 空轮判超时 */
            usleep(REG_POLL_TIMEOUT_US);
            pthread_mutex_lock((pthread_mutex_t *)DAT_00079b78);
            pending = DAT_0007bd9c;
            pthread_mutex_unlock((pthread_mutex_t *)DAT_00079b78);
            if (pending != 0) {
                break;
            }
            wait_tries = wait_tries + 1;
            usleep(REG_POLL_RECHECK_US);
            if (2 < wait_tries) {
                if (reg_addr == 0) {
                    if (DAT_0009d6f4 < DAT_0009d6c5[chain]) {   /* 原码无符号比较 (strblo) */
                        DAT_0009d6f4 = DAT_0009d6c5[chain];
                    }
                    if (LOG_GATE4()) {
                        snprintf(logbuf, 0x1000, "%s: chain J%d has %d ASIC", "check_asic_reg",
                                 chain, (uint32_t)DAT_0009d6c5[chain]);
                        applog(5, logbuf, 0);
                    }
                }
                clear_reg_response_queue();
                return;
            }
        }
        consumed = 0;
        do {                                /* 消费当前 pending 条应答 (持锁) */
            pthread_mutex_lock((pthread_mutex_t *)DAT_00079b78);
            slot_off = DAT_0007bd98 * 7;
            resp_chain = DAT_0007bda0[slot_off + 6];
            if (resp_chain == (uint8_t)chain) {
                type_byte = DAT_0007bda0[slot_off + 5];
                memcpy(&value_raw, &DAT_0007bda0[slot_off + 1], 4);   /* 原码非对齐 4B 装载 */
                value = ((value_raw & 0xffu) << 0x18) | ((value_raw >> 8 & 0xffu) << 0x10) |
                        ((value_raw >> 0x10 & 0xffu) << 8) | (value_raw >> 0x18);
                    /* 机器码 rev 指令: 槽[1..4] = 帧体[3..6] = 值大端字节 → 大端解码 */
                if (reg_addr == 0) {
                    if (DAT_00079b90[chain] == 0) {
                        if (LOG_GATE4()) {
                            snprintf(logbuf, 0x1000, "%s: the asic address is 0x%08x",
                                     "check_asic_reg", value);
                            applog(5, logbuf, 0);
                        }
                    }
                    else {
                        DAT_0009d6c5[chain] = DAT_0009d6c5[chain] + 1;
                    }
                }
                else if (reg_addr == 0xc) {
                    if (LOG_GATE4()) {
                        snprintf(logbuf, 0x1000, "%s: the asic freq is 0x%08x",
                                 "check_asic_reg", value);
                        applog(5, logbuf, 0);
                    }
                }
                else if (reg_addr == 0x14) {
                    if (LOG_GATE4()) {
                        snprintf(logbuf, 0x1000, "%s: the asic tm is 0x%08x",
                                 "check_asic_reg", value);
                        applog(5, logbuf, 0);
                    }
                }
                else if (reg_addr == 0x1c) {
                    if (LOG_GATE4()) {
                        snprintf(logbuf, 0x1000, "%s: the asic misc is 0x%08x",
                                 "check_asic_reg", value);
                        applog(5, logbuf, 0);
                    }
                }
                else if (reg_addr == 0x20) {
                    if ((value & 0xc0000000u) == 0) {
                        /* I2C 间接应答: 值 = [固定域][i2c_reg 7b][通道][数据] (§二) */
                        channel = (value >> 8) & 0xff;       /* 机器码 and r12, r3, #0xff00 */
                        data = (uint8_t)value;               /* 机器码 uxtb r12, r3 */
                        flags = &DAT_00079bb4[chain * 12];
                        temps = (int16_t *)(DAT_0009d534 + chain * 24);
                        group = 0;
                        for (;;) {
                            if (DAT_00079130[group] == type_byte) {
                                if ((uint8_t)channel == I2C_CHAN_TMP451_LOCAL) {
                                    temps[0] = (int16_t)(data - TMP451_TEMP_OFFSET);
                                    flags[0] = 1;
                                }
                                else if ((uint8_t)channel == I2C_CHAN_TMP451_REMOTE) {
                                    temps[1] = (int16_t)(data - TMP451_TEMP_OFFSET);
                                    flags[1] = 1;
                                }
                                else if ((uint8_t)channel == I2C_CHAN_0X11) {
                                    DAT_00079b94[chain * 4 + group] = data;
                                    flags[2] = 1;
                                }
                                else if ((uint8_t)channel == I2C_CHAN_0XFE) {
                                    DAT_00079ba4[chain * 4 + group] = data;
                                    flags[3] = 1;
                                }
                            }
                            flags = flags + 4;
                            temps = temps + 4;
                            if (group + 1 == 3) {
                                break;
                            }
                            group = group + 1;
                        }
                    }
                }
                else if (reg_addr == 0x54) {
                    if (LOG_GATE4()) {
                        snprintf(logbuf, 0x1000, "%s: the start nonce offset is 0x%08x",
                                 "check_asic_reg", value);
                        applog(5, logbuf, 0);
                    }
                }
                else if ((reg_addr == 0x30) && LOG_GATE4()) {
                    snprintf(logbuf, 0x1000, "%s: the asic IO strength is 0x%08x",
                             "check_asic_reg", value);
                    applog(5, logbuf, 0);
                }
                DAT_0007bd98 = DAT_0007bd98 + 1;
                DAT_0007bd9c = DAT_0007bd9c - 1;
                if (DAT_0007bd98 == 100) {
                    DAT_0007bd98 = 0;
                }
            }
            else {
                DAT_0007bd98 = DAT_0007bd98 + 1;
                DAT_0007bd9c = DAT_0007bd9c - 1;
                if (DAT_0007bd98 == 100) {
                    DAT_0007bd98 = 0;
                }
                if (LOG_GATE4()) {
                    snprintf(logbuf, 0x1000,
                             "%s: the return data is from chain%d, but it should be from chain%d chipaddr=0x%02x reg=0x%02x\n",
                             "check_asic_reg", (uint32_t)resp_chain, chain, chip_addr, reg_addr);
                    applog(5, logbuf, 0);
                }
            }
            consumed = consumed + 1;
            pthread_mutex_unlock((pthread_mutex_t *)DAT_00079b78);
        } while (consumed != (uint32_t)pending);
        wait_tries = 0;
    }
}

// === calibration_sensor_offset_chain === 原 FUN_000492b0 @ 0x000492b0
// 来源: reversed/cgminer/16_chain_mgmt/FUN_000492b0.c | 分类: 16_chain_mgmt
// 语义: 校准每链温度传感器偏移 — 打印 "%d %s" (prio 5); 对 3 个芯片组 (组号即芯片地址):
//       最多 11 轮, 每轮 read_i2c_reg(chain, 组, 0x4C, 1) + (..., 0) 触发 local/remote 温度采集,
//       打 "chain:%d local:%d remote:%d" (读 DAT_0009d534 温度 short), 偏移累计
//       DAT_00079be4[组 + chain*4] += (int8)local - (int8)remote, |local-remote| < 3 收敛跳出,
//       否则 set_temperature_offset_value_chain_sensor 把累计值写入芯片再采一轮。
void calibration_sensor_offset_chain(int chain)
{
    int16_t *temps;        /* 原 psVar1: DAT_0009d534 + chain*0x18, 每组前移 4 个 short */
    uint32_t group;        /* 原 uVar3 */
    uint8_t chip_addr;     /* 原 uVar2 = group & 0xff */
    int tries;             /* 原 cVar4: 剩余轮数 (初值 11) */
    int diff;              /* 原 iVar5: |local - remote| */
    char logbuf[0x1000];

    if (LOG_GATE5()) {
        snprintf(logbuf, 0x1000, "%d %s", chain, "calibration_sensor_offset_chain");
        applog(5, logbuf, 0);
    }
    group = 0;
    temps = (int16_t *)(DAT_0009d534 + chain * 24);
    for (;;) {
        chip_addr = (uint8_t)(group & 0xff);
        tries = SENSOR_CAL_MAX_TRIES;
        do {
            read_i2c_reg(chain, chip_addr, 0x4c, 1);
            read_i2c_reg(chain, chip_addr, 0x4c, 0);
            if (LOG_GATE5()) {
                snprintf(logbuf, 0x1000, "chain:%d local:%d remote:%d", chain,
                         (int)temps[0], (int)temps[1]);
                applog(5, logbuf, 0);
            }
            diff = (int)temps[0] - (int)temps[1];
            if (diff < 0) {
                diff = -diff;
            }
            DAT_00079be4[group + chain * 4] = (uint8_t)((int8_t)temps[0] +
                (int8_t)DAT_00079be4[group + chain * 4] - (int8_t)temps[1]);
            if (diff < SENSOR_CAL_DIFF_OK) {
                break;
            }
            tries = tries - 1;
            set_temperature_offset_value_chain_sensor(chain, chip_addr);
        } while (tries != 0);
        group = group + 1;
        temps = temps + 4;
        if (group == SENSOR_GROUP_NUM) {
            return;
        }
    }
}

// === check_asic_num_chain === 原 FUN_0004952c @ 0x0004952c
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0004952c.c | 分类: 16_chain_mgmt
// 语义: 单链 ASIC 计数探测 — 链不在位直接返回; 置计数模式标志 DAT_00079b90[chain] = count_mode
//       后调 check_asic_reg(chain, 0, 0, 1) 发 reg=0 地址探测读 (计数模式下每个应答令
//       DAT_0009d6c5[chain]++), 超时 (3 空轮) 自动返回。
void check_asic_num_chain(int chain, uint8_t count_mode)
{
    if (DAT_0009d4bc[chain] == 0) {
        return;
    }
    DAT_00079b90[chain] = count_mode;
    check_asic_reg(chain, 0, 0, 1);
    return;
}

// === check_every_chain_asic_number === 原 FUN_00049578 @ 0x00049578
// 来源: reversed/cgminer/16_chain_mgmt/FUN_00049578.c | 分类: 16_chain_mgmt
// 语义: 全链 ASIC 计数探测 — 打印 "check_every_asic_number" (prio 5, 原栈上逐字构建;
//       日志串无 "chain" 字样, 原样保留), 对链 0..3 逐个 check_asic_num_chain(chain, count_mode),
//       每链间隔 100ms (count_mode: 1=计数模式, 0=仅打印地址)。
void check_every_chain_asic_number(uint32_t count_mode)
{
    int chain;             /* 原 cVar1/cVar2 (8 位循环计数) */
    char logbuf[0x1000];

    if (LOG_GATE5()) {
        snprintf(logbuf, 0x1000, "check_every_asic_number");
        applog(5, logbuf, 0);
    }
    chain = 0;
    do {
        check_asic_num_chain(chain, (uint8_t)count_mode);
        cgsleep_ms(100);
        chain = chain + 1;
    } while (chain != 4);
    return;
}

// === recheck_asic_num === 原 FUN_00049700 @ 0x00049700
// 来源: reversed/cgminer/16_chain_mgmt/FUN_00049700.c | 分类: 16_chain_mgmt
// 语义: 重检单链 ASIC 数 — 持专用互斥锁 DAT_00079134 串行化 reset_chain(thr_base, chain)
//       (整链复位+重建线程), 解锁后清应答队列, 等 100ms, check_asic_num_chain(chain, 1)
//       (计数模式探测), 等 200ms, 打 "%s DONE!" ("recheck_asic_num", prio 5)。
void recheck_asic_num(int thr_base, uint32_t chain)
{
    char logbuf[0x1000];

    pthread_mutex_lock((pthread_mutex_t *)DAT_00079134);
    reset_chain(thr_base, chain);
    pthread_mutex_unlock((pthread_mutex_t *)DAT_00079134);
    clear_reg_response_queue();
    cgsleep_ms(100);
    check_asic_num_chain((int)chain, 1);
    cgsleep_ms(200);
    if (LOG_GATE5()) {
        snprintf(logbuf, 0x1000, "%s DONE!", "recheck_asic_num");
        applog(5, logbuf, 0);
    }
    return;
}

// === pm_handle === 原 FUN_0004acec @ 0x0004acec
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0004acec.c | 分类: 16_chain_mgmt
// 语义: 0xCC PM 帧处理 (9 字节, RX 线程分帧后调来) — calc_4bit_checksum(帧+2, len*8-0x15)
//       (9B 帧 51 位: 帧体[2..7] 48 位 + 校验字节高 3 位), 与帧[8]&0x1f 比较:
//       校验通过且 opt_debug 门控满足时打 "chain[%d] asic[%d] %02x %02x %02x %02x" (帧体[3..6],
//       asic 序号 = 帧[7] / asic_addr_interval, __udivsi3); 校验失败打
//       "%s chain%d CRC error crc = %02x" (prio 3)。
void pm_handle(uint8_t *frame, int frame_len, int chain)
{
    uint32_t crc;
    uint32_t asic;
    char logbuf[0x1000];

    crc = calc_4bit_checksum(frame + 2, (uint32_t)(frame_len * 8 - 0x15));
    if ((frame[8] & BM1746_CHK_MASK_5BIT) == crc) {
        if ((DAT_00078e09 != '\0') && LOG_GATE7()) {
            asic = frame[7] / (uint32_t)DAT_0009d6f3;   /* 原 __udivsi3: 芯片地址→组号 */
            snprintf(logbuf, 0x1000, "chain[%d] asic[%d] %02x %02x %02x %02x\n", chain,
                     (int)(asic & 0xff), (uint32_t)frame[3], (uint32_t)frame[4],
                     (uint32_t)frame[5], (uint32_t)frame[6]);
            applog(7, logbuf, 0);
        }
    }
    else if (LOG_GATE3()) {
        snprintf(logbuf, 0x1000, "%s chain%d CRC error crc = %02x\n", "pm_handle", chain, crc);
        applog(3, logbuf, 0);
    }
    return;
}

// === get_asic_response === 原 FUN_0004b0c0 @ 0x0004b0c0
// 来源: reversed/cgminer/16_chain_mgmt/FUN_0004b0c0.c | 分类: 16_chain_mgmt
// 语义: 每链 RX 解帧线程 (tty_init_chain 经 thr_info_create 启动, arg = 存链号的 int 槽) —
//       清零读缓冲后: 读 DAT_00079118[chain] 运行标志, 为 0 退出; cgsleep_us(500) 后
//       read_uart_data_lock 读至多 0x5d0 字节, 追加写入 74400 字节环形缓冲 (写索引超
//       0x1229f 归 0), 无新数据则继续读; 有 ≥3 字节后进入分帧循环:
//       3 字节窗口 (不推进读索引) 匹配 RX 前导 AA 55 (机器码: 0xAA/-0x56 与 'U'/0x55),
//       按第 3 字节分帧 — (type&0xf0)==0xE0 → 93B nonce 帧 (不足则回读数据) → nonce_handle;
//       0xCC → 9B PM 帧 → pm_handle(帧, 9, chain&0xff); 其余 → 9B 寄存器应答帧 →
//       reg_handle(帧, 9, chain); 帧体自读索引起整段拷入 93 字节帧组装区 (含 AA 55 前导),
//       读索引按帧长推进; 前导不匹配则丢弃 1 字节继续; 剩余 ≤2 字节回读数据。
//       93B nonce 帧交给 nonce_handle 时附带两个栈上工作缓冲 (0xfe8/0x1110 字节)。
//       注: 原 FUN_0004b0c0 Ghidra 标 void, 作为 pthread 线程体返回值未被使用, 转写返回 NULL。
void *get_asic_response(void *arg)
{
    uint8_t hdr[3];                        /* 原 acStack_149f4[0..2]: 3 字节探测窗口 (cStack_149f5 仅作指针锚) */
    uint8_t frame[ASIC_NONCE_FRAME_LEN];   /* 原 acStack_149f0: 帧组装区 (93B, 预清零) */
    uint8_t readbuf[ASIC_RX_READ_LEN];     /* 原 acStack_14991+1: UART 读出中转 */
    uint8_t ring[ASIC_RX_RING_SIZE];       /* 原 acStack_122c8: 74400B 环形缓冲 */
    uint8_t nonce_aux1[0xfe8];             /* 原 auStack_143c0: nonce_handle 工作缓冲 1 */
    uint8_t nonce_aux2[0x1110];            /* 原 auStack_133d8: nonce_handle 工作缓冲 2 */
    int32_t rd;                            /* 原 iVar5: 环形缓冲读索引 */
    int32_t wr;                            /* 原 iVar6: 写索引 */
    int32_t avail;                         /* 原 iVar1: 可处理字节数 */
    int32_t n_read;                        /* 原 iVar1 (读出字节数, 复用) */
    int32_t i;
    int32_t chain;                         /* 原 param3 */
    pthread_t self;
    char logbuf[0x1000];

    memset(readbuf, 0, ASIC_RX_READ_LEN);
    chain = *(int *)arg;
    self = pthread_self();
    pthread_detach(self);
    if (LOG_GATE5()) {                     /* 原串为栈上拼字 */
        snprintf(logbuf, 0x1000, "Start A New Asic Response.Chain Id:[%d]", chain);
        applog(5, logbuf, 0);
    }
    if ((DAT_00078e09 != '\0') && LOG_GATE7()) {
        snprintf(logbuf, 0x1000, "%s %d", "get_asic_response", chain);
        applog(7, logbuf, 0);
    }
    memset(ring, 0, ASIC_RX_RING_SIZE);
    rd = 0;
    wr = 0;
    memset(frame, 0, ASIC_NONCE_FRAME_LEN);
    for (;;) {                             /* 原最外层 do-while(true) */
        do {                               /* 读 UART 并搬入环形缓冲 */
            do {                           /* 单次读尝试 (无新数据则重读) */
                if (DAT_00079118[chain] == 0) {
                    return NULL;
                }
                cgsleep_us(ASIC_RX_POLL_US);
                n_read = read_uart_data_lock(chain & 0xff, readbuf, ASIC_RX_READ_LEN);
                if (0 < n_read) {
                    for (i = 0; i < n_read; i++) {
                        ring[wr] = readbuf[i];
                        wr = wr + 1;
                        if (ASIC_RX_RING_WRAP < wr) {   /* > 0x1229f 归 0 */
                            wr = 0;
                        }
                    }
                }
            } while (wr == rd);
            /* 可处理字节数: 读 < 写 → 写-读; 否则环形回绕 (原 if (rd < wr) avail = -rd;
             * else avail = 环大小 - rd; avail += wr) */
            if (rd < wr) {
                avail = -rd;
            }
            else {
                avail = ASIC_RX_RING_SIZE - rd;
            }
            avail = avail + wr;
        } while (avail < 3);               /* 不足 3 字节 (前导窗口) → 回读 */

        do {                               /* 分帧扫描 (原 do-while(2 < avail)) */
            hdr[0] = 0;
            hdr[1] = 0;
            hdr[2] = 0;
            i = rd;                        /* 3 字节探测窗口 (不推进 rd) */
            hdr[0] = ring[i];
            i = (i + 1 > ASIC_RX_RING_WRAP) ? 0 : i + 1;
            hdr[1] = ring[i];
            i = (i + 1 > ASIC_RX_RING_WRAP) ? 0 : i + 1;
            hdr[2] = ring[i];
            if ((hdr[0] == 0xAA) && (hdr[1] == 0x55)) {   /* RX 前导 AA 55 (FIFO 字序) */
                if ((hdr[2] & 0xf0U) == 0xe0) {           /* nonce 帧 (0xE0 前缀, 93B) */
                    if (avail < ASIC_NONCE_FRAME_LEN) {
                        break;                            /* 数据不足 → 回读 */
                    }
                    for (i = 0; i < ASIC_NONCE_FRAME_LEN; i++) {
                        frame[i] = ring[rd];
                        rd = rd + 1;
                        if (ASIC_RX_RING_WRAP < rd) {
                            rd = 0;
                        }
                    }
                    avail = avail - ASIC_NONCE_FRAME_LEN;
                    nonce_handle(nonce_aux1, nonce_aux2, frame, ASIC_NONCE_FRAME_LEN,
                                 (uint8_t)chain);
                }
                else if (hdr[2] == 0xcc) {                /* PM 帧 (9B) */
                    if (avail < ASIC_REG_FRAME_LEN) {
                        break;
                    }
                    for (i = 0; i < ASIC_REG_FRAME_LEN; i++) {
                        frame[i] = ring[rd];
                        rd = rd + 1;
                        if (ASIC_RX_RING_WRAP < rd) {
                            rd = 0;
                        }
                    }
                    avail = avail - ASIC_REG_FRAME_LEN;
                    pm_handle(frame, ASIC_REG_FRAME_LEN, chain & 0xff);
                }
                else {                                    /* 寄存器应答帧 (9B) */
                    if (avail < ASIC_REG_FRAME_LEN) {
                        break;
                    }
                    for (i = 0; i < ASIC_REG_FRAME_LEN; i++) {
                        frame[i] = ring[rd];
                        rd = rd + 1;
                        if (ASIC_RX_RING_WRAP < rd) {
                            rd = 0;
                        }
                    }
                    avail = avail - ASIC_REG_FRAME_LEN;
                    reg_handle(frame, ASIC_REG_FRAME_LEN, (uint8_t)chain);
                }
            }
            else {                                        /* 前导不匹配: 丢弃 1 字节 */
                rd = rd + 1;
                avail = avail - 1;
                if (ASIC_RX_RING_WRAP < rd) {
                    rd = 0;
                }
            }
        } while (2 < avail);
    }
}

/* ==================== 覆盖率表 (batch=chain_b1, 44/44) ====================
 * FUN_0002926c @ 0x0002926c → nanosleep_abstime
 * FUN_0002a3dc @ 0x0002a3dc → thr_info_destroy
 * FUN_0002a5c0 @ 0x0002a5c0 → us_to_timespec
 * FUN_0002a600 @ 0x0002a600 → ms_to_timespec
 * FUN_0002a688 @ 0x0002a688 → timeraddspec
 * FUN_0002a8b8 @ 0x0002a8b8 → cgtimer_time
 * FUN_0002a8c4 @ 0x0002a8c4 → cgsleep_ms_r
 * FUN_0002a8fc @ 0x0002a8fc → cgsleep_us_r
 * FUN_0002a92c @ 0x0002a92c → cgsleep_ms
 * FUN_0002a954 @ 0x0002a954 → cgsleep_us
 * FUN_0002f0f4 @ 0x0002f0f4 → _cgsem_init
 * FUN_0002f304 @ 0x0002f304 → _cgsem_mswait
 * FUN_0002f438 @ 0x0002f438 → cg_completion_timeout
 * FUN_0003a354 @ 0x0003a354 → reset_all_hash_board_low
 * FUN_0003bb44 @ 0x0003bb44 → set_baud_pl
 * FUN_0003c5e8 @ 0x0003c5e8 → read_uart_data_lock
 * FUN_0003cc6c @ 0x0003cc6c → calc_4bit_checksum
 * FUN_0003cd58 @ 0x0003cd58 → makeup_reset_frame
 * FUN_0003cdcc @ 0x0003cdcc → makeup_cmd_frame
 * FUN_0003ce7c @ 0x0003ce7c → makeup_address_frame
 * FUN_0003cef0 @ 0x0003cef0 → makeup_reg_read_frame
 * FUN_0003e564 @ 0x0003e564 → read_asic_voltage
 * FUN_0003e5a4 @ 0x0003e5a4 → read_chain_asic_voltages
 * FUN_00042f04 @ 0x00042f04 → send_reset_frame
 * FUN_00042f40 @ 0x00042f40 → send_address_frame
 * FUN_00042f7c @ 0x00042f7c → software_set_address_chain
 * FUN_000430a0 @ 0x000430a0 → set_core_timeout_chain
 * FUN_000434a0 @ 0x000434a0 → set_analog_mux_control_chain
 * FUN_00043e98 @ 0x00043e98 → set_baud_chain
 * FUN_00043f04 @ 0x00043f04 → set_baud
 * FUN_00044004 @ 0x00044004 → set_ticket_mask_chain
 * FUN_000445e8 @ 0x000445e8 → tty_init_chain
 * FUN_00044978 @ 0x00044978 → check_chain
 * FUN_00046434 @ 0x00046434 → reset_chain
 * FUN_000466a4 @ 0x000466a4 → round_asic_num_pow2
 * FUN_000467b0 @ 0x000467b0 → calculate_address_interval
 * FUN_000481b4 @ 0x000481b4 → clear_reg_response_queue
 * FUN_000481ec @ 0x000481ec → check_asic_reg
 * FUN_000492b0 @ 0x000492b0 → calibration_sensor_offset_chain
 * FUN_0004952c @ 0x0004952c → check_asic_num_chain
 * FUN_00049578 @ 0x00049578 → check_every_chain_asic_number
 * FUN_00049700 @ 0x00049700 → recheck_asic_num
 * FUN_0004acec @ 0x0004acec → pm_handle
 * FUN_0004b0c0 @ 0x0004b0c0 → get_asic_response
 */
