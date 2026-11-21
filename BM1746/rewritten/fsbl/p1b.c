/**
 * p1b.c — FSBL 反编译转写 批次 1 后半批 (fsbl_b1 40 函数按地址升序的后 20 个)
 *
 * 覆盖 (reversed/fsbl_funcs/, 源二进制 fsbl_ph1_raw.bin, 本文件 20 函数):
 *   - NAND ECC (GF(2) (31,26) 汉明/校验子): crc_syndrome_table_init / crc_syndrome_calc
 *   - eFuse 读取 (XAPP1280 风格): efuse_read_bit_checked / efuse_read_rows_ecc /
 *     efuse_row_blank_check
 *   - 工具: hex_char_to_nibble
 *   - XAdcPs (Zynq XADC, 实例经 DevCfg 0xF8007100 命令(+0x10)/数据(+0x14) 寄存器):
 *     xadc_init / xadc_lookup_config / xadc_cfg_initialize / xadc_cfg0_get /
 *     xadc_cfg2_pulse / xadc_chan_data_get / xadc_msts_nibble_get / xadc_avg_ch0_set /
 *     xadc_avg_ch0_get / xadc_cfg2_field_get
 *   - ARM 全局定时器: global_timer_reset
 *   - 主入口: fsbl_main_entry (PS7 初始化失败路径 / SLCR 解锁 / 异常向量注册 /
 *     横幅与版本打印 / DDR 标记 / DevCfg 初始化 / BOOT_MODE(0xF800025C&7) 分发
 *     NAND(4)|NOR(2)|SD(5)|MMC(6)|JTAG(0) / flash 基址合法性校验 / eFuse stats / 交接)
 *   - 异常向量: IRQ, UndefinedInstruction
 *
 * 全局数据 (反编译实证; 以绝对地址宏引用, 保证与 p1a/p4.c 等分批文件共享同一存储):
 *   0x00028e84 = xil_assert 错误标志 (0=正常, 1=已触发; 同 p4.c 的 g_assert_failed)
 *   0x00028eac = GF(2) 校验子矩阵 (31 行 x 5 列保留, 前 26 行有效, 行主序步进 5;
 *                crc_syndrome_table_init 生成, 与 crc_syndrome_calc / crc_syndrome_position 共用)
 *   0x00028e8c = 校验子 -> 出错位序号表 (32 字节, [0]=0xff 无错哨兵)
 *   0x00028e74 = DevCfg 实例 (fsbl_main_entry 中传给 nand_ctrl_reg_get;
 *                FUN_00000714 断言文件串为 "xdevcfg.c" @0x00017214)
 *   0x00028f4c = 全局标志 (xadc_init 清零, 语义待定)
 *   0x00019c50 = XADC 配置表: +0 DeviceID=0 (u32), +4 BaseAddress=0xF8007100 (u32)
 *   0x00021be0 = XADC 实例: +0 DeviceID(u16), +4 BaseAddress(u32), +8 IsReady=0x11111111
 *   0x0002105c = flash 基址 (fsbl_main_entry 校验/打印)
 *   0x00021060 = 线性启动设备标志 (flash 基址 = 0xE2000000 时置 1)
 *   0x00021f90 = 启动流程状态 (fsbl_main_entry 置 1, 语义待定)
 *   0x00021f94 = DevCfg 状态寄存器读回值 (bit12 = 禁止系统复位)
 *   0x00021bf8 = 按启动模式写入: NAND=0x2fa8 / NOR=0x31ac / SD=MMC=0x4124 (语义待定)
 *
 * 0xFFFF0000 区间被调函数 (不在 fsbl_funcs 导出集, name_map 未收录,
 * 依约定以 fun_<8位地址> 占位并标 命名待补; 依行为证据注明推断):
 *   fun_ffffcbec (PS7 初始化, 返回非 0 = 失败) / fun_ffffc97c (取 PS7 失败描述串) /
 *   fun_ffffde2c (printf 型, 原格式串均按字面量保留并注明原地址) /
 *   fun_ffff9580 (打印完成后终止) / fun_ffffd6a4 / fun_ffffd700 / fun_ffffdbcc /
 *   fun_ffffdbd0 (注册 1..6 号异常处理: Undef@0x1b28 SVC@0x1b0c PrefAbort@0x1af0
 *                 DataAbort@0x1ad4 IRQ@0x1ab8 FIQ@0x1a9c) /
 *   fun_ffffc174 (DevCfg 初始化, 1 = 失败) / fun_ffffa560 / fun_ffffa790 (错误终止) /
 *   fun_ffffbe84 (NAND 初始化) / fun_ffffc118 (NOR 初始化) / fun_ffffcff4 (SD/eMMC
 *                 加载 "BOOT.BIN") / fun_ffffcb4c / fun_ffffabcc (eFuse stats) /
 *   fun_ffff9dc4 (取交接地址) / fun_ffffa8e0 (交接跳转)
 *
 * 调用方可见函数名一律取自 rewritten/name_map/fsbl.tsv (阶段 1 冻结)。
 * 源码内字符串字面量按原字节恢复 (fsbl_ph1_raw.bin: vaddr = 文件偏移 + 0x7080)。
 */
#include <stdint.h>

/* ==================== 跨文件共享全局 (绝对地址宏) ==================== */

/* xil_assert 错误标志 DAT_00028e84, 与 p4.c 同一存储 */
#define g_assert_failed   (*(volatile uint32_t *)0x00028e84)

/* GF(2) 校验子矩阵 DAT_00028eac (行主序, 每行 5 字节) 与出错位表 DAT_00028e8c */
#define g_syndrome_table  ((volatile uint8_t *)0x00028eac)
#define g_errpos_table    ((volatile uint8_t *)0x00028e8c)

/* 其余全局: 保留反编译原名 (语义已在文件头注明) */
#define DAT_00028e74      ((uint8_t *)0x00028e74)
#define DAT_00028f4c      (*(volatile uint32_t *)0x00028f4c)
#define DAT_00019c50      ((uint8_t *)0x00019c50)
#define DAT_00021be0      ((uint8_t *)0x00021be0)
#define DAT_0002105c      (*(volatile uint32_t *)0x0002105c)
#define DAT_00021060      (*(volatile uint32_t *)0x00021060)
#define DAT_00021f90      (*(volatile uint32_t *)0x00021f90)
#define DAT_00021f94      (*(volatile uint32_t *)0x00021f94)
#define DAT_00021bf8      (*(volatile uint32_t *)0x00021bf8)

/* ==================== 共享声明 (被调函数, 签名与反编译一致) ==================== */

/* p4.c / 其它分批文件中已定义 */
extern int      dbg_print(int line, int code);            /* FUN_000014e4, p1a 断言约定 (行号,0) */
extern void     hang_forever(void);                       /* FUN_00002b40 */
extern uint32_t nand_ctrl_reg_get(int inst);              /* FUN_00000714, 见 p4.c */

/* 其它分批文件 (p1a/p2/p3) */
extern void     mem_copy(void *dst, const void *src, uint32_t n);          /* FUN_00002200 (memcpy) */
extern int      xadc_read_channels(uint32_t *out);                         /* FUN_0000bb98: 7 字输出 */
extern int      efuse_addr_is_reserved(uint32_t addr);                     /* FUN_0000b254: 1=保留 */
extern uint8_t  crc_syndrome_position(const uint8_t *data31, uint8_t *check_out5); /* FUN_0000b160 */
extern void     bitarray_pack(const uint8_t *bits, void *dst, uint32_t nbits);     /* FUN_0000bed4 */
extern int      efuse_setup_timing(int mode, uint32_t freq, int sub);      /* FUN_0000aedc */
extern int      xadc_selftest(int xadc);                                   /* FUN_0000c240 */
extern void     xadc_seq_mode_set(int xadc, uint32_t mode);                /* FUN_0000cdd4 */
extern int      xadc_seq_mode_get(int xadc);                               /* FUN_0000ceb8 */
extern void     icache_invalidate_wfi(void);                               /* FUN_0000e948 */

/* 0xFFFF0000 区间被调 (占位, 命名待补) */
extern int      fun_ffffcbec(void);
extern int      fun_ffffc97c(void);
extern int      fun_ffffde2c(const char *fmt, ...);       /* printf 型 */
extern void     fun_ffff9580(void);
extern void     fun_ffffd6a4(void);
extern void     fun_ffffd700(void);
extern void     fun_ffffdbcc(void);
extern void     fun_ffffdbd0(int id, int handler, int arg);
extern int      fun_ffffc174(void);
extern void     fun_ffffa560(void);
extern void     fun_ffffa790(void);
extern int      fun_ffffbe84(void);
extern void     fun_ffffc118(void);
extern int      fun_ffffcff4(const char *name);
extern void     fun_ffffcb4c(void);
extern int      fun_ffffabcc(void);
extern uint32_t fun_ffff9dc4(void);
extern void     fun_ffffa8e0(uint32_t addr);

/* 本文件前向声明 (定义在后、调用在前的函数) */
const uint8_t *xadc_lookup_config(uint32_t devid);
int xadc_cfg_initialize(uint8_t *inst, const uint8_t *cfg, uint32_t base);

/* ==================== 本文件函数 ==================== */

/* ==================== NAND ECC: GF(2) (31,26) 校验子 ==================== */

// === crc_syndrome_table_init === 原 FUN_0000af74 @ 0x0000af74
// 来源: reversed/fsbl_funcs/FUN_0000af74.c | 分类: fsbl
// 语义: 初始化 NAND GF(2) ECC 表:
//   1) 清零校验子矩阵 g_syndrome_table (0x28eac 起 31 行 x 5 列, 行主序步进 5);
//   2) 重建矩阵: 有效码位 bit_pos ∈ [2,0x1f] 且 (bit_pos-1)&0xfb != 3 且 bit_pos-1 != 1
//      且 bit_pos != 0x10 (原布尔判定链化简, 共 26 行), 第 row 行第 col 列 =
//      bit_pos 的第 col 位 (5 位二进制, 经典汉明校验矩阵);
//   3) 重建校验子 -> 出错位序号表 g_errpos_table (0x28e8c, 32 字节):
//      [0] = 0xff (校验子 0 = 无错哨兵); 序号 t ∈ {0,1,3,7,15} (即 t-3 清 bit2 后为 0,
//      对应汉明校验位位置) 依次取 0x1a 起计数, 其余 t 依次取 0 起计数 (数据位位置)。
void crc_syndrome_table_init(void)
{
    volatile uint8_t *p;
    uint32_t col;
    uint32_t row;
    uint32_t bit_pos;
    uint32_t m;
    uint32_t t;
    uint8_t cnt_check;                                  /* 校验位位置计数, 0x1a 起 */
    uint8_t cnt_data;                                   /* 数据位位置计数, 0 起 */

    /* 清零: 原双重循环外层步进 1 (列), 内层步进 5 (行) */
    for (col = 0; col < 5u; col++) {
        for (row = 0; row < 31u; row++) {
            g_syndrome_table[row * 5u + col] = 0;
        }
    }

    /* 校验子矩阵: 每列一遍, 有效码位依序占行 */
    for (col = 0; col < 5u; col++) {
        row = 0;
        for (bit_pos = 2; bit_pos != 0x20u; bit_pos++) {
            m = bit_pos - 1u;
            if ((m & 0xfbu) != 3u && m != 1u && bit_pos != 0x10u) {
                /* 原判定: (int)(bit_pos & 1<<col) >> col == 1, 即 bit_pos 第 col 位 */
                if ((bit_pos & (1u << col)) != 0u) {
                    g_syndrome_table[row * 5u + col] = 1;
                } else {
                    g_syndrome_table[row * 5u + col] = 0;
                }
                row++;
            }
        }
    }

    /* 出错位序号表 */
    g_errpos_table[0] = 0xff;                           /* DAT_00028e8c: 无错哨兵 */
    cnt_check = 0x1a;
    cnt_data = 0;
    for (t = 0; t < 0x1fu; t++) {
        p = g_errpos_table + t + 1;                     /* 写指针自 &DAT_00028e8d 步进 */
        if (((t - 3u) & 0xfbu) == 0u || t < 2u || t == 0xfu) {
            *p = cnt_check;
            cnt_check++;
        } else {
            *p = cnt_data;
            cnt_data++;
        }
    }
}

// === crc_syndrome_calc === 原 FUN_0000b0e4 @ 0x0000b0e4
// 来源: reversed/fsbl_funcs/FUN_0000b0e4.c | 分类: fsbl
// 语义: 对 26 位码字 (data[0..0x19], 每字节 1 位) 计算 (31,26) 校验位:
// 先把 26 个数据位复制到 out[0..0x19], 再对 5 列分别做
// out[0x1a+j] = (Σ g_syndrome_table[i*5+j] * data[i]) & 1 (逐行点积取奇偶)。
// 副作用: 写 out[0..0x1e] 共 31 字节。
void crc_syndrome_calc(const uint8_t *data, uint8_t *out)
{
    uint32_t i;
    uint32_t j;
    uint8_t acc;

    /* 数据位复制 (26 字节) */
    for (i = 0; i < 0x1au; i++) {
        out[i] = data[i];
    }
    /* 5 个校验位 */
    for (j = 0; j < 5u; j++) {
        acc = 0;
        for (i = 0; i < 26u; i++) {
            acc = (uint8_t)(g_syndrome_table[i * 5u + j] * data[i] + acc);
            out[0x1au + j] = acc;                       /* 原循环体内逐步写回, 最终被覆盖 */
        }
        out[0x1au + j] = acc & 1u;
    }
}

// === efuse_read_bit_checked === 原 FUN_0000b3d4 @ 0x0000b3d4
// 来源: reversed/fsbl_funcs/FUN_0000b3d4.c | 分类: fsbl
// 语义: 经 XADC 温度/电压字合法性检查后, 读 eFuse 行寄存器 (*row) 的 bit0 写入 *out。
// 返回: 0 = 成功; 1 = 地址为保留 eFuse 行; 2 = 温度字越界; 3 = 电压字越界。
int efuse_read_bit_checked(volatile uint32_t *row, uint8_t *out)
{
    uint32_t chan[7];                                   /* xadc_read_channels 输出 7 字 */
    int ret;

    xadc_read_channels(chan);
    if (chan[0] - 0x766eu < 0x53d1u) {                  /* 温度字 ∈ [0x766e, 0xca3f) */
        if (chan[4] - 0x91ebu < 0x170bu) {              /* 电压字 ∈ [0x91eb, 0xa8f6) */
            if (efuse_addr_is_reserved((uint32_t)(uintptr_t)row) == 0) {
                *out = (uint8_t)(*row & 1u);            /* 行寄存器 bit0 = 编程状态 */
                ret = 0;
            } else {
                ret = 1;
            }
        } else {
            ret = 3;
        }
    } else {
        ret = 2;
    }
    return ret;
}

// === efuse_read_rows_ecc === 原 FUN_0000b450 @ 0x0000b450
// 来源: reversed/fsbl_funcs/FUN_0000b450.c | 分类: fsbl
// 语义: 从 eFuse 行寄存器 0xF800E080 起逐字扫描 (保留行跳到下一字), 共 10 轮 x 31 位,
// 每轮: 读 31 位 -> crc_syndrome_position 校验, 位序号 < 0x1f 时翻转该位纠错 ->
// 把 bits[0..0x19] (26 字节) 追加到 packed 缓冲 (目的偏移 +1 起)。10 轮共 260 位后,
// bitarray_pack 把前 256 位 (LSB 优先) 打包到 dst。返回 0 = 成功, 非 0 = 读取错误码。
int efuse_read_rows_ecc(void *dst)
{
    uint8_t check[5];                                   /* 原帧 -0x14c: 校验位 5 字节输出 */
    uint8_t raw[32];                                    /* 原帧 -0x145..-0x125: raw[0] 为 -1 基准字节, raw[1..31] = 31 位缓冲 */
    uint8_t *bits = raw + 1;                            /* 31 位缓冲 (每字节 1 位), 即原帧 -0x144 起 */
    uint8_t packed[261];                                /* 原帧 -0x125: 打包前位阵列 */
    uint32_t addr;                                      /* 当前行寄存器地址游标 */
    uint32_t a;                                         /* 本轮实际读取地址 (保留行 +4) */
    uint32_t out_off;                                   /* packed 内输出偏移 */
    uint32_t i;
    uint8_t pos;
    const uint8_t *s;
    uint8_t *d;
    int ret;

    crc_syndrome_table_init();                          /* 原 FUN_0000af74, 每次读前重建表 */
    addr = 0xF800E080u;                                 /* 原 -0x7ff1f80 */
    out_off = 0;
    do {
        for (i = 0; i < 31u; i++) {
            a = addr;
            if (efuse_addr_is_reserved(a) == 1) {
                a = addr + 4;                           /* 保留行: 跳过 4 字节再读 */
            }
            addr = a + 4;
            ret = efuse_read_bit_checked((volatile uint32_t *)(uintptr_t)a, &bits[i]);
            if (ret != 0) {
                return ret;
            }
        }
        pos = crc_syndrome_position(bits, check);       /* 0xff = 无错/双错 */
        if (pos < 0x1fu) {
            bits[pos] = (uint8_t)((bits[pos] + 1u) & 1u);   /* 单比特纠错: 翻转 (原 +1 & 1) */
        }
        /* bits[0..0x19] -> packed[out_off+1 .. out_off+0x1a] (原以帧内 -1 基准字节起拷) */
        s = raw;
        d = packed + out_off;
        do {
            s++;
            d++;
            *d = *s;
        } while (raw + 0x1au != s);
        out_off += 0x1au;
    } while (out_off != 0x104u);                        /* 10 轮 x 26 = 260 位 */
    bitarray_pack(packed + 1, dst, 0x100);              /* 打包前 256 位到 dst */
    return 0;
}

// === hex_char_to_nibble === 原 FUN_0000ba3c @ 0x0000ba3c
// 来源: reversed/fsbl_funcs/FUN_0000ba3c.c | 分类: fsbl
// 语义: 十六进制字符 -> 半字节: '0'-'9' -> 0-9, 'a'-'f'/'A'-'F' -> 10-15;
// 非法字符返回 0x11 且不写 *out。
int hex_char_to_nibble(uint8_t c, char *out)
{
    if ((c - 0x30u) < 10u) {                            /* 原 (param_1 - 0x30U & 0xff) < 10 */
        *out = (char)(c - 0x30u);
        return 0;
    }
    if (c - 0x61u < 6u) {                               /* a-f */
        *out = (char)(c - 0x57u);
        return 0;
    }
    if (c - 0x41u < 6u) {                               /* A-F */
        *out = (char)(c - 0x37u);
        return 0;
    }
    return 0x11;
}

/* ==================== XAdcPs (Zynq XADC, 命令/数据寄存器接口) ==================== */

// === xadc_init === 原 FUN_0000bb18 @ 0x0000bb18
// 来源: reversed/fsbl_funcs/FUN_0000bb18.c | 分类: fsbl
// 语义: XADC 初始化: 清全局标志 (DAT_00028f4c), 查配置表 (DeviceID 0),
// CfgInitialize 到实例 DAT_00021be0, 自检, 失败再置序列模式 0。
// 返回: 0 = 成功; 0xd = 找不到配置; 0xe = 初始化失败; 0xf = 自检失败。
int xadc_init(void)
{
    const uint8_t *cfg;
    int ret;

    DAT_00028f4c = 0;
    cfg = xadc_lookup_config(0);
    if (cfg == 0) {
        return 0xd;
    }
    ret = xadc_cfg_initialize(DAT_00021be0, cfg,
                              *(const volatile uint32_t *)(cfg + 4));   /* Config->BaseAddress */
    if (ret != 0) {
        return 0xe;
    }
    if (xadc_selftest((int)(uintptr_t)DAT_00021be0) == 0) {
        xadc_seq_mode_set((int)(uintptr_t)DAT_00021be0, 0);
        return 0;
    }
    return 0xf;
}

/* ==================== 全局定时器 ==================== */

// === global_timer_reset === 原 FUN_0000bc50 @ 0x0000bc50
// 来源: reversed/fsbl_funcs/FUN_0000bc50.c | 分类: fsbl
// 语义: ARM 全局定时器复位: 计数器 (0xF8F00200 低 32 位 / 0xF8F00204 高 32 位) 清零,
// 控制寄存器 0xF8F00208 写 1 (bit0 = 定时器使能)。
void global_timer_reset(void)
{
    *(volatile uint32_t *)0xF8F00200u = 0;
    *(volatile uint32_t *)0xF8F00204u = 0;
    *(volatile uint32_t *)0xF8F00208u = 1;
}

/* ==================== XAdcPs 实例与寄存器访问 ==================== */
/* 注: 实例 +4 处加载的寄存器基址指针按反编译源统一转写为字节指针
 * volatile uint8_t * (原 `*(int *)(param_1+4)` 加载后以整型 +0x10/0x14/0x18
 * 字节偏移访问命令/数据寄存器), 与 p2b.c/p4.c 同一 XAdcPs 接口保持一致。 */

// === xadc_lookup_config === 原 FUN_0000c2f8 @ 0x0000c2f8
// 来源: reversed/fsbl_funcs/FUN_0000c2f8.c | 分类: fsbl
// 语义: 按设备号查 XADC 配置表: 全表仅 DeviceID 0 一项 (DAT_00019c50, 其 +4 处
// BaseAddress = 0xF8007100); 匹配返回配置表指针, 否则 NULL。
const uint8_t *xadc_lookup_config(uint32_t devid)
{
    if (*(volatile uint32_t *)DAT_00019c50 == devid) {  /* Config->DeviceID */
        return DAT_00019c50;
    }
    return 0;
}

// === xadc_cfg_initialize === 原 FUN_0000c314 @ 0x0000c314
// 来源: reversed/fsbl_funcs/FUN_0000c314.c | 分类: fsbl
// 语义: XADC 实例初始化 (XAdcPs_CfgInitialize): 拷贝 DeviceID(u16), 存 BaseAddress,
// 解锁 DevCfg (0xF8007034 = 0x757BDF0D), XADC CFG0 (base+0x00) |= 0x80ff0000,
// base+0x18 清零, IsReady = 0x11111111。断言失败时置 g_assert_failed 并返回 0。
int xadc_cfg_initialize(uint8_t *inst, const uint8_t *cfg, uint32_t base)
{
    volatile uint8_t *ip = (volatile uint8_t *)inst;
    uint16_t devid;

    if (ip == 0) {
        dbg_print(0x6e, 0);                             /* 断言文件 "xadcps.c" @0x000172a0 */
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (cfg == 0) {
        dbg_print(0x6f, 0);
        g_assert_failed = 1;
        return 0;
    }
    devid = *(volatile uint16_t *)cfg;                  /* Config->DeviceID */
    *(volatile uint32_t *)(ip + 4) = base;              /* InstPtr->BaseAddress */
    *(volatile uint16_t *)ip = devid;                   /* InstPtr->DeviceID */
    *(volatile uint32_t *)0xF8007034u = 0x757BDF0Du;    /* DevCfg 解锁键 */
    *(volatile uint32_t *)(*(volatile uint32_t *)(ip + 4)) |= 0x80ff0000u;
    *(volatile uint32_t *)(*(volatile uint32_t *)(ip + 4) + 0x18u) = 0;
    *(volatile uint32_t *)(ip + 8) = 0x11111111u;       /* InstPtr->IsReady */
    return 0;
}

// === xadc_cfg0_get === 原 FUN_0000c44c @ 0x0000c44c
// 来源: reversed/fsbl_funcs/FUN_0000c44c.c | 分类: fsbl
// 语义: 读 XADC CFG0 (base+0x00), 带 xil_assert 双重保护 (实例非空 + IsReady 标志)。
uint32_t xadc_cfg0_get(int xadc)
{
    if (xadc == 0) {
        dbg_print(0xc4, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(xadc + 8) == 0x11111111) {
        return **(volatile uint32_t **)(xadc + 4);
    }
    dbg_print(0xc5, 0);
    g_assert_failed = 1;
    return 0;
}

// === xadc_cfg2_pulse === 原 FUN_0000c654 @ 0x0000c654
// 来源: reversed/fsbl_funcs/FUN_0000c654.c | 分类: fsbl
// 语义: 向 XADC base+0x18 先写 0x10 再写 0 (CFR2 脉冲, 自检前由 xadc_selftest 调用)。
void xadc_cfg2_pulse(int xadc)
{
    volatile uint8_t *base;

    if (xadc == 0) {
        dbg_print(0x139, 0);
        g_assert_failed = 1;
        return;
    }
    g_assert_failed = 0;
    if (*(int *)(xadc + 8) == 0x11111111) {
        base = *(volatile uint8_t **)(xadc + 4);
        *(volatile uint32_t *)(base + 0x18) = 0x10u;
        *(volatile uint32_t *)(base + 0x18) = 0;
        return;
    }
    dbg_print(0x13a, 0);
    g_assert_failed = 1;
}

// === xadc_chan_data_get === 原 FUN_0000c6dc @ 0x0000c6dc
// 来源: reversed/fsbl_funcs/FUN_0000c6dc.c | 分类: fsbl
// 语义: 读 XADC 通道数据: CMD (base+0x10) = chan<<16 | 0x04000000 触发读,
// 随后 CMD = DATA (base+0x14) 启动转换, 读回 DATA 低 16 位。
// 合法通道: 0..6 与 0xd..0x1f (原判定链化简)。
uint32_t xadc_chan_data_get(int xadc, uint32_t chan)
{
    volatile uint8_t *base;
    int invalid;

    if (xadc == 0) {
        dbg_print(0x164, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(xadc + 8) != 0x11111111) {
        dbg_print(0x165, 0);
        g_assert_failed = 1;
        return 0;
    }
    /* 原双布尔判定: chan<=6 时非法 = (chan>5 && chan!=6);
       chan>6 时非法 = (chan-0xd > 0x11 && chan-0xd != 0x12) */
    invalid = (chan > 5u) && (chan != 6u);
    if (chan > 6u) {
        invalid = ((chan - 0xdu) > 0x11u) && ((chan - 0xdu) != 0x12u);
    }
    if (invalid) {
        dbg_print(0x168, 0);
        g_assert_failed = 1;
        return 0;
    }
    base = *(volatile uint8_t **)(xadc + 4);
    *(volatile uint32_t *)(base + 0x10) = (chan << 16) | 0x04000000u;
    *(volatile uint32_t *)(base + 0x10) = *(volatile uint32_t *)(base + 0x14);
    return *(volatile uint32_t *)(base + 0x14) & 0xffffu;
}

// === xadc_msts_nibble_get === 原 FUN_0000cd30 @ 0x0000cd30
// 来源: reversed/fsbl_funcs/FUN_0000cd30.c | 分类: fsbl
// 语义: 读 MSTS (Misc Status, 寄存器 0x41): CMD = 0x04410000 触发读,
// 返回 DATA & 0xf0 (报警位高半字节)。
uint32_t xadc_msts_nibble_get(int xadc)
{
    volatile uint8_t *base;

    if (xadc == 0) {
        dbg_print(0x33f, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(xadc + 8) == 0x11111111) {
        base = *(volatile uint8_t **)(xadc + 4);
        *(volatile uint32_t *)(base + 0x10) = 0x04410000u;
        *(volatile uint32_t *)(base + 0x10) = *(volatile uint32_t *)(base + 0x14);
        return *(volatile uint32_t *)(base + 0x14) & 0xf0u;
    }
    dbg_print(0x340, 0);
    g_assert_failed = 1;
    return 0;
}

// === xadc_avg_ch0_set === 原 FUN_0000d564 @ 0x0000d564
// 来源: reversed/fsbl_funcs/FUN_0000d564.c | 分类: fsbl
// 语义: 写 Averaging 寄存器 CH0 对 (reg 0x4C/0x4D): 要求序列模式为 0
// (xadc_seq_mode_get), 否则返回 1; 写 CMD 0x084C0000 | (value & 0x800) 低字,
// CMD 0x084D0000 | (value >> 16) 高字。成功返回 0。
int xadc_avg_ch0_set(int xadc, uint32_t value)
{
    volatile uint8_t *base;
    int mode;

    if (xadc == 0) {
        dbg_print(0x4c3, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(xadc + 8) != 0x11111111) {
        dbg_print(0x4c4, 0);
        g_assert_failed = 1;
        return 0;
    }
    mode = xadc_seq_mode_get(xadc);
    if (mode == 0) {
        base = *(volatile uint8_t **)(xadc + 4);
        *(volatile uint32_t *)(base + 0x10) = (value & 0x800u) | 0x084C0000u;
        *(volatile uint32_t *)(base + 0x10) = (value >> 16) | 0x084D0000u;
        return 0;
    }
    return 1;
}

// === xadc_avg_ch0_get === 原 FUN_0000d62c @ 0x0000d62c
// 来源: reversed/fsbl_funcs/FUN_0000d62c.c | 分类: fsbl
// 语义: 读 Averaging 寄存器 CH0 对 (reg 0x4C/0x4D): 命令 0x044C0000 读低字,
// 0x044D0000 读高字, 返回 (低字 & 0x800) | (高字 << 16) 组合值。
uint32_t xadc_avg_ch0_get(int xadc)
{
    volatile uint8_t *base;
    uint32_t lo;

    if (xadc == 0) {
        dbg_print(0x4f6, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(xadc + 8) == 0x11111111) {
        base = *(volatile uint8_t **)(xadc + 4);
        *(volatile uint32_t *)(base + 0x10) = 0x044C0000u;
        *(volatile uint32_t *)(base + 0x10) = *(volatile uint32_t *)(base + 0x14);
        lo = *(volatile uint32_t *)(base + 0x14);
        *(volatile uint32_t *)(base + 0x10) = 0x044D0000u;
        *(volatile uint32_t *)(base + 0x10) = *(volatile uint32_t *)(base + 0x14);
        return (lo & 0x800u) | (*(volatile int *)(base + 0x14) << 16);
    }
    dbg_print(0x4f7, 0);
    g_assert_failed = 1;
    return 0;
}

// === xadc_cfg2_field_get === 原 FUN_0000deac @ 0x0000deac
// 来源: reversed/fsbl_funcs/FUN_0000deac.c | 分类: fsbl
// 语义: 读 CFR2 (Config Reg 2, 寄存器 0x42) 位域: 命令 0x04420000 触发读,
// 返回 (DATA >> 4) & 0x0ffffffc (原 0xffffffc 掩码)。
uint32_t xadc_cfg2_field_get(int xadc)
{
    volatile uint8_t *base;

    if (xadc == 0) {
        dbg_print(0x6d0, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(xadc + 8) == 0x11111111) {
        base = *(volatile uint8_t **)(xadc + 4);
        *(volatile uint32_t *)(base + 0x10) = 0x04420000u;
        *(volatile uint32_t *)(base + 0x10) = *(volatile uint32_t *)(base + 0x14);
        return *(volatile uint32_t *)(base + 0x14) >> 4 & 0x0ffffffcu;
    }
    dbg_print(0x6d1, 0);
    g_assert_failed = 1;
    return 0;
}

/* ==================== eFuse (XAPP1280) ==================== */

// === efuse_row_blank_check === 原 FUN_0000e6c0 @ 0x0000e6c0
// 来源: reversed/fsbl_funcs/FUN_0000e6c0.c | 分类: fsbl
// 语义: eFuse 行空白检查: XADC 温度/电压字与频率合法性检查 (freq ∈ [20e6, 60e6+1))
// 后, 以 efuse_setup_timing(0, freq, 1..3) 逐档设置时序并读行寄存器 bit0, 最多 3 次。
// 返回: 8 = 已空白 (bit0=0); 0 = 非空白; 1 = 保留地址; 2/3 = 温度/电压字越界;
// 0xb = 频率越界; 时序设置失败返回其错误码。
int efuse_row_blank_check(uint32_t row_addr, uint32_t freq)
{
    volatile uint32_t *row = (volatile uint32_t *)(uintptr_t)row_addr;
    uint32_t chan[7];                                   /* xadc_read_channels 输出 7 字 */
    int ret;

    xadc_read_channels(chan);
    if (chan[0] - 0x766eu >= 0x53d1u) {                 /* 温度字越界 */
        return 2;
    }
    if (chan[4] - 0x91ebu >= 0x170bu) {                 /* 电压字越界 */
        return 3;
    }
    if (freq + 0xfeced300u >= 0x2625a01u) {             /* freq ∉ [0x1312d00, 0x3938701) */
        return 0xb;
    }
    ret = efuse_setup_timing(0, freq, 1);
    if (ret != 0) {
        return ret;
    }
    if (efuse_addr_is_reserved(row_addr) != 0) {
        return 1;
    }
    ret = 0;
    if ((*row & 1u) == 0u) {
        ret = 8;                                        /* 第 1 次读即为空白 */
    } else {
        ret = efuse_setup_timing(0, freq, 2);
        if (ret == 0 && (*row & 1u) == 0u) {
            ret = 8;                                    /* 第 2 次读为空白 */
        } else if (ret == 0) {
            ret = efuse_setup_timing(0, freq, 3);
            if (ret == 0 && (*row & 1u) == 0u) {
                ret = 8;                                /* 第 3 次读为空白 */
            }
            /* ret 为 0 (仍非空白) 或时序设置错误码, 原样返回 */
        }
    }
    return ret;
}

/* ==================== 异常向量 ==================== */

// === IRQ === 原 FUN_00000018 @ 0x00000018
// 来源: reversed/fsbl_funcs/IRQ.c | 分类: 向量表 (passthrough 原名)
// 语义: IRQ 异常入口 (向量表第 6 项)。dbg_print 记录 (与 FIQ 相同的标记行号 0x3b7,
// 首参原为断言文件串指针 "xnandps.c" @0x00017208, 按本文件 dbg_print 约定省略),
// 然后向经 r11 寄存器传入的标志地址写入 1 (通知主循环发生了 IRQ)。
static volatile uint32_t *g_irq_flag_ptr;               /* r11 寄存器携带的标志变量地址, 反编译不可追溯 */

void IRQ(void)
{
    dbg_print(0x3b7, 0);
    *g_irq_flag_ptr = 1;                                /* 原始语义: 向 r11 指向的标志写 1 */
}

// === UndefinedInstruction === 原 FUN_00000004 @ 0x00000004
// 来源: reversed/fsbl_funcs/UndefinedInstruction.c | 分类: 向量表 (passthrough 原名)
// 语义: Undef 异常入口 (向量表第 2 项)。反编译产物受损 (Control flow encountered
// bad instruction data): 可辨识内容为调用 memcpy(FUN_00002200) 处理 r3+r7 携带的
// 上下文块, 随即落入 halt_baddata 挂死。形参残缺不可恢复, 依约定不虚构。
void UndefinedInstruction(void)
{
    extern void halt_baddata(void);

    /* 原始流: FUN_00002200(r3 + r7 寄存器携带的地址) — 首参为寄存器携带的地址,
       其余实参与长度位于损坏指令流之后, 不可恢复 — 以注释存证 */
    /* mem_copy((void *)(uintptr_t)(r3 + r7 携带地址), <参数不可恢复>); */
    halt_baddata();
}

/* ==================== FSBL 主入口 ==================== */

// === fsbl_main_entry === 原 FUN_0000e138 @ 0x0000e138
// 来源: reversed/fsbl_funcs/FUN_0000e138.c | 分类: fsbl
// 语义: FSBL 主入口 (main)。流程: PS7 初始化失败则打印并终止 -> SLCR 解锁 ->
// BSP 初始化三连 -> 注册 1..6 号异常处理 -> 打印 "Xilinx First Stage Boot Loader"
// 横幅与版本 (2018.3, 2021-05-25) -> DDR 0x100000/0x200000 写标记 -> DevCfg 初始化
// (失败打印 PCAP_INIT_FAIL) -> 读 DevCfg 状态 (bit12 = 禁止系统复位) ->
// REBOOT_STATUS 置 0x60000000 -> 按 BOOT_MODE (0xF800025C & 7) 分发:
// 4=NAND / 2=NOR / 5=SD / 6=MMC / 0=JTAG / 其它=非法 -> flash 基址合法性校验
// (豁免 0xE1000000/0xE2000000/0xFC000000/0xE0100000) -> 线性设备标志 -> eFuse stats
// -> 打印 Handoff 地址并交接。返回值为启动设备初始化状态 (正常路径为 0)。
// 注: 所有 printf 型 fun_ffffde2c 调用的格式串按原字节字面量保留 (原地址注明);
//     末尾打印/等待 UART0 TXEMPTY (0xE000002C bit3) 后终止的路径与反编译一致。
int fsbl_main_entry(void)
{
    int status;                                         /* PS7/DevCfg/启动设备状态复用 (原单变量) */
    int efuse_stats;                                    /* eFuse stats 检查结果 */
    uint32_t ps7_err;                                   /* PS7 失败描述串指针 */
    uint32_t handoff;                                   /* 交接地址 */
    uint32_t boot_mode;

    status = fun_ffffcbec();                            /* PS7 初始化, 非 0 = 失败 */
    if (status != 0) {
        ps7_err = (uint32_t)fun_ffffc97c();             /* 取 PS7 失败描述串指针 */
        fun_ffffde2c("PS7_INIT_FAIL : %s\r\n", ps7_err);            /* 原 fmt @0x00016954 */
        fun_ffffde2c("FSBL Status = 0x%.4lx\r\n", 0xa012u);         /* 原 fmt @0x00016410 */
        while ((*(volatile uint32_t *)0xE000002Cu & 8u) == 0u) { }  /* UART0 SR.bit3 TXEMPTY */
        fun_ffff9580();                                 /* 终止 */
    }

    *(volatile uint32_t *)0xF8000008u = 0xDF0DDF0Du;    /* SLCR 解锁 */
    fun_ffffd6a4();
    fun_ffffd700();
    fun_ffffdbcc();
    fun_ffffdbd0(1, 0x1b28, 0);                         /* 1 = Undef, 处理函数 @0x00001b28 */
    fun_ffffdbd0(2, 0x1b0c, 0);                         /* 2 = SVC @0x00001b0c */
    fun_ffffdbd0(3, 0x1af0, 0);                         /* 3 = PrefetchAbort @0x00001af0 */
    fun_ffffdbd0(4, 0x1ad4, 0);                         /* 4 = DataAbort @0x00001ad4 */
    fun_ffffdbd0(5, 0x1ab8, 0);                         /* 5 = IRQ @0x00001ab8 */
    fun_ffffdbd0(6, 0x1a9c, 0);                         /* 6 = FIQ @0x00001a9c */

    fun_ffffde2c("\n\rXilinx First Stage Boot Loader \n\r");        /* 原 fmt @0x0001696c */
    fun_ffffde2c("Release %d.%d\t%s-%s\r\n", 0x7e2u, 3,
                 "May 25 2021", "12:02:23");            /* 原 fmt @0x0001699c (2018.3) */

    *(volatile uint32_t *)0x00200000u = 0xAA55AA55u;    /* DDR 测试标记 */
    *(volatile uint32_t *)0x00100000u = 0xAA55AA55u;

    status = fun_ffffc174();                            /* DevCfg 初始化, 1 = 失败 */
    if (status == 1) {
        fun_ffffde2c("PCAP_INIT_FAIL \n\r");            /* 原 fmt @0x000169d4 */
        fun_ffffde2c("FSBL Status = 0x%.4lx\r\n", 0xa002u);         /* 原 fmt @0x00016410 */
        while ((*(volatile uint32_t *)0xE000002Cu & 8u) == 0u) { }
        fun_ffff9580();
    }
    fun_ffffde2c("Devcfg driver initialized \r\n");     /* 原 fmt @0x000169e8 */
    fun_ffffa560();

    /* 读 DevCfg 状态寄存器 (实例 DAT_00028e74; FUN_00000714 断言文件 "xdevcfg.c") */
    DAT_00021f94 = nand_ctrl_reg_get((int)(uintptr_t)DAT_00028e74);
    if ((DAT_00021f94 & 0x1000u) != 0u) {
        fun_ffffde2c("User not allowed to do any system resets\r\n");   /* 原 fmt @0x00016a08 */
    }

    *(volatile uint32_t *)0xF8000258u |= 0x60000000u;   /* REBOOT_STATUS 置 bit29/30 */
    boot_mode = *(volatile uint32_t *)0xF800025Cu & 7u; /* BOOT_MODE 寄存器 */

    if (boot_mode == 4u) {                              /* NAND */
        fun_ffffde2c("Boot mode is NAND\n");            /* 原 fmt @0x00016a34 */
        status = fun_ffffbe84();                        /* NAND 初始化 */
        if (status != 0) {
            fun_ffffde2c("NAND_INIT_FAIL \r\n");        /* 原 fmt @0x00016a48 */
            fun_ffffde2c("FSBL Status = 0x%.4lx\r\n", 0xa00au);
            while ((*(volatile uint32_t *)0xE000002Cu & 8u) == 0u) { }
            fun_ffffa790();
        }
        DAT_00021bf8 = 0x2fa8u;
        fun_ffffde2c("NAND Init Done \r\n");            /* 原 fmt @0x00016a5c */
    } else if (boot_mode == 2u) {                       /* NOR */
        fun_ffffde2c("Boot mode is NOR\n\r");           /* 原 fmt @0x00016a70 */
        fun_ffffc118();                                 /* NOR 初始化 (无状态返回) */
        fun_ffffde2c("NOR Init Done \r\n");             /* 原 fmt @0x00016a84 */
        DAT_00021bf8 = 0x31acu;
    } else if (boot_mode == 5u) {                       /* SD */
        fun_ffffde2c("Boot mode is SD\r\n");            /* 原 fmt @0x00016a98 */
        status = fun_ffffcff4("BOOT.BIN");              /* SD 加载 (原文件名 @0x00016aac) */
        if (status != 0) {
            fun_ffffde2c("SD_INIT_FAIL\r\n");           /* 原 fmt @0x00016ab8 */
            fun_ffffde2c("FSBL Status = 0x%.4lx\r\n", 0xa009u);
            while ((*(volatile uint32_t *)0xE000002Cu & 8u) == 0u) { }
            fun_ffffa790();
        }
        DAT_00021bf8 = 0x4124u;
        fun_ffffde2c("SD Init Done \r\n");              /* 原 fmt @0x00016ac8 */
    } else if (boot_mode == 6u) {                       /* MMC/eMMC */
        fun_ffffde2c("Booting Device is MMC\r\n");      /* 原 fmt @0x00016ad8 */
        status = fun_ffffcff4("BOOT.BIN");              /* eMMC 加载 */
        if (status != 0) {
            fun_ffffde2c("MMC_INIT_FAIL\r\n");          /* 原 fmt @0x00016af0 */
            fun_ffffde2c("FSBL Status = 0x%.4lx\r\n", 0xa009u);
            while ((*(volatile uint32_t *)0xE000002Cu & 8u) == 0u) { }
            fun_ffffa790();
        }
        DAT_00021bf8 = 0x4124u;
        fun_ffffde2c("MMC Init Done \r\n");             /* 原 fmt @0x00016b00 */
    } else if (boot_mode == 0u) {                       /* JTAG: 无启动设备 */
        fun_ffffde2c("Boot mode is JTAG\r\n");          /* 原 fmt @0x00016b14 */
        if ((*(volatile uint32_t *)0xF800700Cu & 4u) != 0u) {   /* DevCfg MCTRL bit2 */
            fun_ffffcb4c();
            *(volatile uint32_t *)0xF8000008u = 0xDF0DDF0Du;    /* SLCR 解锁 */
        }
        *(volatile uint32_t *)0xF8000258u &= 0x0FFFFFFFu;   /* REBOOT_STATUS 清高 4 位 */
        *(volatile uint32_t *)0xF8000004u = 0x767B767Bu;    /* SLCR 加锁 */
        icache_invalidate_wfi();                        /* FUN_0000e948: 清 I-Cache 后 WFI 待机 */
    } else {                                            /* 非法启动模式 */
        fun_ffffde2c("ILLEGAL_BOOT_MODE \r\n");         /* 原 fmt @0x00016b28 */
        fun_ffffde2c("FSBL Status = 0x%.4lx\r\n", 0xa000u);
        while ((*(volatile uint32_t *)0xE000002Cu & 8u) == 0u) { }
        fun_ffffa790();
    }

    fun_ffffde2c("Flash Base Address: 0x%08lx\r\n", DAT_0002105c);  /* 原 fmt @0x00016b40 */
    /* 合法性豁免集: (X+0x1F000000)&0xFEFFFFFF == 0 (即 0xE1000000/0xE2000000),
       或 X == 0xFC000000 (原 -0x4000000) / 0xE0100000 (原 -0x1ff00000) */
    if (((DAT_0002105c + 0x1f000000u) & 0xfeffffffu) != 0u &&
        (DAT_0002105c != 0xFC000000u && DAT_0002105c != 0xE0100000u)) {
        fun_ffffde2c("INVALID_FLASH_ADDRESS \r\n");     /* 原 fmt @0x00016b60 */
        fun_ffffde2c("FSBL Status = 0x%.4lx\r\n", 0xa006u);
        while ((*(volatile uint32_t *)0xE000002Cu & 8u) == 0u) { }
        fun_ffffa790();
    }
    if (DAT_0002105c == 0xE2000000u) {                  /* 原 -0x1e000000: NOR 线性区 */
        fun_ffffde2c("Linear Boot Device\r\n");         /* 原 fmt @0x00016b7c */
        DAT_00021060 = 1u;
    }

    DAT_00021f90 = 1u;
    efuse_stats = fun_ffffabcc();                       /* eFuse stats 检查 */
    if (efuse_stats != 0) {
        fun_ffffde2c("EFUSE STATS ERROR, fallback\r\n");/* 原 fmt @0x00016b94 */
        fun_ffffa790();
    }
    handoff = fun_ffff9dc4();                           /* 取交接地址 */
    fun_ffffde2c("Handoff Address: 0x%08lx\r\n", handoff);  /* 原 fmt @0x00016bb4 */
    fun_ffffa8e0(handoff);                              /* 交接跳转 */
    return status;
}

/* ==================== 覆盖率表 (fsbl_b1 后半批 20/20, 按地址升序) ==================== */
/*
 * | #  | 语义名                   | 原 FUN         | 地址        | 说明                                        |
 * |----|--------------------------|----------------|-------------|---------------------------------------------|
 * | 1  | crc_syndrome_table_init  | FUN_0000af74   | 0x0000af74  | GF(2) 校验子矩阵 + 出错位表初始化           |
 * | 2  | crc_syndrome_calc        | FUN_0000b0e4   | 0x0000b0e4  | 26 位码字 -> 5 校验位 (矩阵点积奇偶)        |
 * | 3  | efuse_read_bit_checked   | FUN_0000b3d4   | 0x0000b3d4  | XADC 合法性检查 + eFuse 行 bit0 读取        |
 * | 4  | efuse_read_rows_ecc      | FUN_0000b450   | 0x0000b450  | 0xF800E080 起 10x31 位扫描 + 单比特纠错     |
 * | 5  | hex_char_to_nibble       | FUN_0000ba3c   | 0x0000ba3c  | 十六进制字符 -> 半字节 (非法 0x11)          |
 * | 6  | xadc_init                | FUN_0000bb18   | 0x0000bb18  | XADC 查表/CfgInitialize/自检/序列模式 0     |
 * | 7  | global_timer_reset       | FUN_0000bc50   | 0x0000bc50  | 全局定时器清零 + 控制寄存器使能             |
 * | 8  | xadc_lookup_config       | FUN_0000c2f8   | 0x0000c2f8  | 按设备号查 XADC 配置表 (仅 ID 0)            |
 * | 9  | xadc_cfg_initialize      | FUN_0000c314   | 0x0000c314  | XADC CfgInitialize + DevCfg 解锁            |
 * | 10 | xadc_cfg0_get            | FUN_0000c44c   | 0x0000c44c  | 读 CFG0 (base+0x00), 双重断言               |
 * | 11 | xadc_cfg2_pulse          | FUN_0000c654   | 0x0000c654  | base+0x18 写 0x10 再写 0 (CFR2 脉冲)        |
 * | 12 | xadc_chan_data_get       | FUN_0000c6dc   | 0x0000c6dc  | 读通道数据 (合法域 0..6, 0xd..0x1f)         |
 * | 13 | xadc_msts_nibble_get     | FUN_0000cd30   | 0x0000cd30  | 读 MSTS (0x41), 返回 DATA & 0xf0            |
 * | 14 | xadc_avg_ch0_set         | FUN_0000d564   | 0x0000d564  | 写 Averaging CH0 对 (0x4C/0x4D)             |
 * | 15 | xadc_avg_ch0_get         | FUN_0000d62c   | 0x0000d62c  | 读 Averaging CH0 对 (0x4C/0x4D)             |
 * | 16 | xadc_cfg2_field_get      | FUN_0000deac   | 0x0000deac  | 读 CFR2 (0x42) 位域 (>>4 & 0x0ffffffc)      |
 * | 17 | fsbl_main_entry          | FUN_0000e138   | 0x0000e138  | FSBL 主入口 (横幅/异常注册/启动模式分发)    |
 * | 18 | efuse_row_blank_check    | FUN_0000e6c0   | 0x0000e6c0  | eFuse 行空白检查 (时序 3 档, 8=空白)        |
 * | 19 | IRQ                      | FUN_00000018   | 0x00000018  | IRQ 向量: dbg_print(0x3b7) + 标志写 1       |
 * | 20 | UndefinedInstruction     | FUN_00000004   | 0x00000004  | Undef 向量: memcpy(参数残缺) + 挂死         |
 *
 * 未收录占位: 0xFFFF0000 区间被调 fun_ffffcbec/c97c/de2c/9580/d6a4/d700/dbcc/dbd0/
 * c174/a560/a790/be84/c118/cff4/cb4c/abcc/9dc4/a8e0 (命名待补, 依行为证据注释)。
 */
