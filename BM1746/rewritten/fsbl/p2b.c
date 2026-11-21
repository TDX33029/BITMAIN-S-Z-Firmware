/**
 * p2b.c — FSBL 反编译转写 批次 p2 后半 (2/2)
 *
 * 范围: rewritten/batch_plan_other.tsv 中 batch=fsbl_b2 的 41 个函数按地址升序取后 20 个
 *       FUN_00007f5c ~ FUN_0000e948 + PrefetchAbort
 *       (SD/eMMC 总线与切换 / SHA-256 / eFuse 编程 / 超时与位包工具 / hex 密钥解析 /
 *        XAdcPs 寄存器访问 / eFuse 行写 / I-Cache 失效 / 预取中止异常处理)
 * 来源: reversed/fsbl_funcs/
 * 命名: rewritten/name_map/fsbl.tsv
 * 约定: rewritten/CONVENTIONS.md
 */
#include <stdint.h>
#include <stddef.h>
#include "../include/common_types.h"

/* ==================== 共享声明 (本批被调/跨批被调, 签名与反编译一致) ==================== */

/* Xil Assert 风格错误标志 (0x28e84), 与 p4.c 各驱动 getter 共用 */
#define g_assert_failed   (*(volatile uint32_t *)0x00028e84)

/* SD 主机命令寄存器暂存 (0x28e88): sd_send_cmd 调用前写入, 硬件层读取 (原名 DAT_00028e88) */
#define g_sd_cmd_reg      (*(volatile uint32_t *)0x00028e88)

/* ---- 被调函数 (定义在其它批次文件) ---- */
extern int  dbg_print(int line, int code);              /* FUN_000014e4: 行号+错误码日志 (p4.c) */
extern void memcpy(void *dst, const void *src, uint32_t n);  /* FUN_00002200 */
extern void memset(void *dst, int val, uint32_t n);     /* FUN_000027e0 */
extern int  strlen(const char *s);                      /* FUN_00002900 (定义见 p4.c) */
extern uint64_t global_timer_counter_read(void);        /* FUN_0000bc74 (定义见 p4.c) */
extern int  hex_char_invalid(const uint8_t *p);         /* FUN_0000bf60 (p3.c) */
extern int  hex_string_to_bytes(const char *hex, uint8_t *out, uint32_t len); /* FUN_0000ba90 (p3.c) */
extern void delay_ms(uint32_t ms);                      /* func_0xffffe2ac: ROM/BSP 毫秒延时 (p4.c) */
extern void hang_forever(void);                         /* FUN_00002b40 (p4.c) */
extern int  sd_send_cmd(int sd, uint16_t cmd, uint32_t arg, uint16_t resp_type); /* FUN_00006bcc (p4.c) */
extern void sd_adma_setup(int sd, int nblocks, void *buf); /* FUN_000071f8 (p4.c) */
extern void sha256_init(void *ctx);                     /* FUN_00009294 (p4.c) */
extern void sha256_update(void *ctx, const void *data, uint32_t len); /* FUN_000092c8 (p3.c) */
extern void sha256_final(void *ctx, void *hash);        /* FUN_000095a0 (p3.c) */
extern int  cpu_freq_detect(void);                      /* FUN_0000c024 (p3.c) */
extern int  xadc_init(void);                            /* FUN_0000bb18 (p3.c) */
extern void global_timer_reset(uint32_t freq);          /* FUN_0000bc50 (p3.c) */
extern int  efuse_setup_timing(int mode, uint32_t freq, int sub); /* FUN_0000aedc (p3.c) */
extern int  efuse_write_rows(const uint8_t *data, uint32_t freq); /* FUN_0000e88c (定义见本文件尾部) */
extern int  efuse_addr_is_reserved(uint32_t addr);      /* FUN_0000b254 (p3.c) */
extern int  efuse_read_bit_checked(uint32_t addr, uint8_t *bit_out); /* FUN_0000b3d4 (p3.c) */
extern int  efuse_program_bit(uint32_t addr);           /* FUN_0000b6a4 (p3.c) */
extern int  efuse_row_blank_check(uint32_t addr, uint32_t freq); /* FUN_0000e6c0 (p3.c) */
extern int  efuse_program_row_pair(uint32_t row, uint32_t freq); /* FUN_0000e7c4 (p4.c) */
extern void bitarray_unpack(const uint8_t *src, uint8_t *dst, uint32_t nbits); /* FUN_0000be74 (p3/p4.c) */
extern void crc_syndrome_table_init(void);              /* FUN_0000af74 (p3.c) */
extern void crc_syndrome_calc(const uint8_t *in, uint8_t *out5); /* FUN_0000b0e4 (p3.c) */

/* ---- 本批数据/常量 ---- */
extern const uint8_t s_P0_00015b96[];                   /* 数据块 @ 0x15b96; +2 起为 SHA-256 H0 初值 32 字节 */

#define SD_CMD55   0x3700       /* CMD55 编码 (APP_CMD, 与 p4.c 一致) */
#define SD_ACMD6   0x8600       /* ACMD6 编码 (SET_BUS_WIDTH) */
#define SD_CMD6    0x600        /* CMD6 编码 (SWITCH, SD 检查模式/eMMC EXT_CSD 写) */

/* ---- 通用寄存器读写 (地址常量来自反编译字面量) ---- */
static inline volatile uint32_t *reg32(uint32_t addr) { return (volatile uint32_t *)(uintptr_t)addr; }

/* 注: 下文 SD 主机寄存器偏移均为"字节偏移"(host 为 volatile uint8_t *),
 * 与反编译 *(ushort *)(base + 0x30) 等逐字节对应: +0x04 块长, +0x28 Host Control,
 * +0x30 Normal Int Status, +0x32 Error Int Status, +0x3E Host Control 2 (UHS 模式)。 */

/* ==================== SD/eMMC 总线宽度与切换 ==================== */

// === sd_set_bus_width === 原 FUN_00007f5c @ 0x00007f5c
// 来源: reversed/fsbl_funcs/FUN_00007f5c.c | 分类: fsbl
// 语义: 切换卡总线宽度 (Xilinx SD 驱动): SD 卡 (+0x34==1) 走 CMD55(RCA) + ACMD6;
// eMMC 走 CMD6 SWITCH, 参数低字节域对应 EXT_CSD[183] BUS_WIDTH (0x3B70100/0x3B70200
// /0x3B70500/0x3B70600 = 4 位/8 位/4 位 DDR/8 位 DDR)。完成后按宽度状态 +0x37
// (2=4 位, 3=8 位) 置 SDHCI Host Control (+0x28) 的 bit1/bit5, 若 +0x6c==4 (DDR
// 模式) 再置 Host Control 2 (+0x3E) bit[2:0]=4 (DDR50 时序)。
// 实例布局: +4 = SDHCI 寄存器基址, +0x14 = 请求总线宽度, +0x24 = IsReady (0x11111111),
// +0x34 = 卡类型 (1=SD), +0x36 = 卡类型域 (2=高容量), +0x37 = 宽度状态, +0x50 = RCA,
// +0x6c = 模式选择 (4=DDR)。
int sd_set_bus_width(int sd)
{
    volatile uint8_t *host;
    uint32_t arg;
    uint8_t hc;

    if (sd == 0) {
        dbg_print(0x109, 0);                            /* 原 FUN_000014e4(&DAT_00017264, 0x109) */
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(sd + 0x24) != 0x11111111) {            /* IsReady */
        dbg_print(0x10a, 0);
        g_assert_failed = 1;
        return 0;
    }
    if ((*(char *)(sd + 0x36) != 2) || (3 < *(uint32_t *)(sd + 0x14))) {
        /* 非"高容量域且宽度<=3"时执行切换 */
        if (*(char *)(sd + 0x34) == 1) {                /* SD 卡: CMD55 + ACMD6 */
            if (sd_send_cmd(sd, SD_CMD55, *(uint32_t *)(sd + 0x50), 0) != 0) {
                return 1;
            }
            *(uint8_t *)(sd + 0x37) = 2;                /* 宽度状态 = 4 位 */
            /* 原反编译仅传 2 参: sd_send_cmd(sd, 0x8600), arg/resp 寄存器沿用未定值 */
            if (((int (*)(int, uint16_t))sd_send_cmd)(sd, SD_ACMD6) != 0) {
                return 1;
            }
        }
        else {
            /* eMMC: CMD6 SWITCH 选择 EXT_CSD BUS_WIDTH */
            if (((*(uint32_t *)(sd + 0x34) & 0xff00ff) == 0x20005) && (*(int *)(sd + 0x14) == 8)) {
                /* eMMC v5 高容量且 8 位: +0x6c==4 → 8 位 DDR (0x3B70600), 否则 8 位 (0x3B70200) */
                arg = (*(int *)(sd + 0x6c) == 4) ? 0x3B70600u : 0x3B70200u;
                *(uint8_t *)(sd + 0x37) = 3;
            }
            else {
                arg = (*(int *)(sd + 0x6c) == 4) ? 0x3B70500u : 0x3B70100u;
                *(uint8_t *)(sd + 0x37) = 2;
            }
            if (sd_send_cmd(sd, SD_CMD6, arg, 0) != 0) {
                return 1;
            }
            host = *(volatile uint8_t **)(sd + 4);
            for (;;) {
                uint16_t ps = *(volatile uint16_t *)(host + 0x30);
                if ((ps & 0x8000u) != 0) {              /* 错误位 */
                    *(volatile uint16_t *)(host + 0x32) = 0xF3FF;  /* 清错误标志 */
                    return 1;
                }
                if ((ps & 2u) != 0) break;              /* 传输完成 */
            }
            *(volatile uint16_t *)(host + 0x30) = 2;    /* 清完成位 */
        }
        delay_ms(1);                                    /* func_0xffffe2ac(1000): 1ms 延时 */
        host = *(volatile uint8_t **)(sd + 4);
        hc = *(volatile uint8_t *)(host + 0x28);        /* SDHCI Host Control */
        if (*(char *)(sd + 0x37) == 3) {
            hc |= 0x20;                                 /* bit5: 8 位数据宽度 */
        }
        else {
            hc |= 0x02;                                 /* bit1: 4 位数据宽度 */
        }
        *(volatile uint8_t *)(host + 0x28) = hc;
        if (*(int *)(sd + 0x6c) == 4) {
            /* Host Control 2: UHS 模式选择 bit[2:0] = 4 (DDR50) */
            *(volatile uint16_t *)(host + 0x3E) =
                 (uint16_t)(*(volatile uint16_t *)(host + 0x3E) & 0xFFF8u) | 4;
            return 0;
        }
    }
    return 0;
}

// === sd_switch_func_check === 原 FUN_00008168 @ 0x00008168
// 来源: reversed/fsbl_funcs/FUN_00008168.c | 分类: fsbl
// 语义: SD CMD6 检查模式 (Switch Function, arg 0xFFFFF0): 清零 64 字节状态缓冲,
// 块长设 64B, ADMA 指向缓冲, 命令寄存器暂存 0x11, 非 cache-coherent 实例先做
// DMA 前 dcache 维护 (ROM 例程), 发 CMD6 并等待传输完成。
int sd_switch_func_check(int sd, void *buf)
{
    volatile uint8_t *host;
    uint8_t *p;

    if (sd == 0) {
        dbg_print(0x193, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(sd + 0x24) != 0x11111111) {
        dbg_print(0x194, 0);
        g_assert_failed = 1;
        return 0;
    }
    p = (uint8_t *)buf;                                 /* 清零 buf[0..0x3F] (64 字节) */
    do {
        *p++ = 0;
    } while (p != (uint8_t *)buf + 0x40);
    *(volatile uint16_t *)(*(volatile uint8_t **)(sd + 4) + 4) = 0x40;  /* 块长 = 64B */
    sd_adma_setup(sd, 1, buf);
    g_sd_cmd_reg = 0x11;                                /* DAT_00028e88: R1 响应+读数据路径 */
    if (*(char *)(sd + 0x20) == 0) {                    /* 非 cache-coherent 实例 */
        /* 原 func_0xffffd23c(buf, 0x40): DMA 前 dcache 维护 (ROM 例程, 语义按调用形态) */
    }
    if (sd_send_cmd(sd, SD_CMD6, 0xFFFFF0u, 1) != 0) {  /* CMD6 检查模式 */
        return 1;
    }
    host = *(volatile uint8_t **)(sd + 4);
    for (;;) {
        uint16_t ps = *(volatile uint16_t *)(host + 0x30);
        if ((ps & 0x8000u) != 0) {
            *(volatile uint16_t *)(host + 0x32) = 0xF3FF;
            return 1;
        }
        if ((ps & 2u) != 0) break;
    }
    *(volatile uint16_t *)(host + 0x30) = 2;
    return 0;
}

// === mmc_switch_cmd6 === 原 FUN_00008994 @ 0x00008994
// 来源: reversed/fsbl_funcs/FUN_00008994.c | 分类: fsbl
// 语义: eMMC CMD6 (SWITCH) 裸封装: 以给定 arg 发 CMD6 (无数据相位), 等传输完成;
// 出错清错误标志并返回 1, 成功清完成位返回 0。
int mmc_switch_cmd6(int sd, uint32_t arg)
{
    volatile uint8_t *host;

    if (sd_send_cmd(sd, SD_CMD6, arg, 0) != 0) {
        return 1;
    }
    host = *(volatile uint8_t **)(sd + 4);
    for (;;) {
        uint16_t ps = *(volatile uint16_t *)(host + 0x30);
        if ((ps & 0x8000u) != 0) {
            *(volatile uint16_t *)(host + 0x32) = 0xF3FF;
            return 1;
        }
        if ((ps & 2u) != 0) break;
    }
    *(volatile uint16_t *)(host + 0x30) = 2;
    return 0;
}

/* ==================== SHA-256 ==================== */

// === sha256_hash === 原 FUN_00009924 @ 0x00009924
// 来源: reversed/fsbl_funcs/FUN_00009924.c | 分类: fsbl
// 语义: 一次性 SHA-256: 栈上 104 字节上下文, init → update(data,len) → final(hash)。
void sha256_hash(const void *data, uint32_t len, void *hash)
{
    uint32_t ctx[26];                                   /* SHA-256 上下文, 104 字节 */

    sha256_init(ctx);
    sha256_update(ctx, data, len);
    sha256_final(ctx, hash);
}

// === sha256_state_reset === 原 FUN_00009974 @ 0x00009974
// 来源: reversed/fsbl_funcs/FUN_00009974.c | 分类: fsbl
// 语义: 将 SHA-256 上下文前 32 字节 (H[0..7]) 复位为 H0 初值常量
// (数据块 s_P0_00015b96 + 2 起 0x20 字节)。
void sha256_state_reset(int ctx)
{
    memcpy((void *)(uintptr_t)ctx, s_P0_00015b96 + 2, 0x20);
}

/* ==================== eFuse 编程 (XAPP1280 流程) ==================== */

// === efuse_program === 原 FUN_0000b710 @ 0x0000b710
// 来源: reversed/fsbl_funcs/FUN_0000b710.c | 分类: fsbl
// 语义: eFuse 编程总控。参数块 param_1[0..5] 为使能标志 (各须 0/1, 否则 0x8600):
// [0]=AES 密钥行 (0xF800E020/0xE024 两行都尝试, 仅两行全失败才报错),
// [1]=行 0xF800E02C, [2]=行 0xF800E028, [3]=行 0xF800E030, [4]=行 0xF800E034,
// [5]=256 位用户数据 (param_1[6..]: 先扫描 0xF800E080 起 0x136 个非保留行确认
// 全空, 再 efuse_write_rows 烧写)。流程: 0xF800D004=0xDF0D 解锁 → 忙检查 (D008
// bit0) → 加密/锁检查 (D010 bit12-13) → CPU 频率窗检查 (20~60MHz) → 时序初始化 →
// 逐行编程 → 恢复寄存器。返回状态码: 0=成功, 0x81xx/0x86xx 参数,
// 0x82xx/0x83xx/0x84xx 初始化, 0x85xx 时序/频率, 0x91xx~0x99xx 各行/写回错误。
int efuse_program(uint32_t *param_1)
{
    int freq;
    int status;
    int st_aes_lo;
    uint32_t addr;
    uint8_t bitbuf[5];                                  /* local_21: 位值出口 (仅 [0] 使用) */

    if (param_1 == NULL) {
        return 0x8100;
    }
    freq = cpu_freq_detect();
    if ((((((1 < param_1[0]) || (1 < param_1[1])) || (1 < param_1[2])) ||
          ((1 < param_1[5]) || (1 < param_1[3]))) || (1 < param_1[4]))) {
        return 0x8600;
    }
    if (xadc_init() != 0) {
        return 0x8200;
    }
    *reg32(0xF800D004) = 0xDF0D;                        /* 解锁字 (EFUSE, 语义按 XAPP1280) */
    if ((*reg32(0xF800D008) & 1) != 0) {                /* 忙/编程进行中 */
        *reg32(0xF800D004) = 0xDF0D;                    /* 原样重写解锁字 */
        return 0x8300;
    }
    if ((*reg32(0xF800D010) & 0x3000) != 0) {           /* bit12-13: 加密/写保护状态 */
        *reg32(0xF800D000) = 0x767B;                    /* 恢复 CFG 默认值 */
        *reg32(0xF800D004) = 0xDF0D;
        return 0x8400;
    }
    if (40000000u < (uint32_t)freq + 0xFECED300u) {
        /* 无符号环绕: ⇔ freq < 20MHz 或 freq > 60MHz */
        status = 0x850b;
        goto epilogue;                                  /* 原 LAB_0000b7d4 */
    }
    status = efuse_setup_timing(0, freq, 1);
    if (status != 0) {
        status += 0x8500;
        goto epilogue;
    }
    *reg32(0xF800D014) = (*reg32(0xF800D014) & 0xFFFFFFECu) | 0x10;   /* 清 bit0/1/4, 置 bit4 */
    global_timer_reset(freq);
    if (param_1[2] != 0) {
        status = efuse_program_row_pair(0xF800E028, freq);
        if (status != 0) {
            status += 0x9100;
            goto epilogue;
        }
    }
    if (param_1[5] == 0) {
        goto program_rows;                              /* 原 LAB_0000b860 */
    }
    /* 256 位用户数据: timing mode=1 下扫描 0xF800E080 起 0x136 个非保留行寄存器,
     * 逐位确认空白 (bit0=0); 任一已烧写位 (错误码 9) 或读取出错即中止 (错误码
     * 恢复 timing 后 +0x9700)。 */
    status = ((int (*)(int, uint32_t))efuse_setup_timing)(1, freq);  /* 原反编译仅传 2 参 (sub 寄存器沿用未定值) */
    if (status != 0) {
        goto bail_9700;                                 /* 原 LAB_0000b9f4 */
    }
    addr = 0xF800E080;                                  /* 原 -0x7ff1f80 (int) 同值 */
    {
        uint32_t remain = 0x136;
        do {
            while (efuse_addr_is_reserved(addr) == 1) { /* 跳过保留/保护行 */
                addr += 4;
            }
            status = efuse_read_bit_checked(addr, bitbuf);
            addr += 4;
            if (status != 0) {
                goto restore_then_bail;                 /* 原 LAB_0000b9dc */
            }
            if (bitbuf[0] != 0) {                       /* 该位已烧写 → 错误码 9 */
                status = 9;
                goto restore_then_bail;
            }
            remain--;
        } while (remain != 0);
    }
    status = efuse_setup_timing(0, freq, 1);
    if (status != 0) {
        goto bail_9700;
    }
    status = efuse_write_rows(param_1 + 6, freq);       /* 见本文件 (FUN_0000e88c) */
    if (status != 0) {
        status += 0x9400;
        goto epilogue;
    }

program_rows:                                           /* 原 LAB_0000b860 */
    if (param_1[1] != 0) {
        status = efuse_program_row_pair(0xF800E02C, freq);
        if (status != 0) {
            status += 0x9500;
            goto epilogue;
        }
    }
    if (param_1[3] != 0) {
        status = efuse_program_row_pair(0xF800E030, freq);
        if (status != 0) {
            status += 0x9800;
            goto epilogue;
        }
    }
    if (param_1[4] != 0) {
        status = efuse_program_row_pair(0xF800E034, freq);
        if (status != 0) {
            status += 0x9900;
            goto epilogue;
        }
    }
    status = 0;
    if (param_1[0] != 0) {
        /* AES 密钥: 两行都尝试编程; 仅当两行都失败才报错 (+0x9600)。
         * 注: 原代码以 freq 变量承接第二次调用的返回值 (freq 此后不再使用)。 */
        st_aes_lo = efuse_program_row_pair(0xF800E020, freq);
        status = efuse_program_row_pair(0xF800E024, freq);
        if (st_aes_lo != 0 && status != 0) {
            status = st_aes_lo + 0x9600;
        }
    }

epilogue:                                               /* 原 LAB_0000b7d4 */
    *reg32(0xF800D014) = (*reg32(0xF800D014) & 0xFFFFFFEFu) | 3;   /* 清 bit4, 置 bit0/bit1 */
    *reg32(0xF800D000) = 0x767B;
    return status;

restore_then_bail:                                      /* 原 LAB_0000b9dc: 恢复 timing mode 0 */
    {
        int s = efuse_setup_timing(0, freq, 1);
        if (s == 0) {
            s = status;
        }
        status = s;
    }
bail_9700:                                              /* 原 LAB_0000b9f4 */
    status += 0x9700;
    goto epilogue;
}

/* ==================== 超时与位包/字符串工具 ==================== */

// === timeout_deadline_set === 原 FUN_0000bcd8 @ 0x0000bcd8
// 来源: reversed/fsbl_funcs/FUN_0000bcd8.c | 分类: fsbl
// 语义: 设置超时截止值: *deadline = global_timer_counter_read() + cycles*DAT_00028f50
// (64 位乘积) + (DAT_00028f50*hi 的 32 位环绕积)<<32。param_2 在反编译中未使用 (死参数)。
extern uint32_t DAT_00028f50;                           /* 全局, 语义待定: 定时器 tick 换算系数 (cpu_freq_detect 设置) */
void timeout_deadline_set(int64_t *deadline, uint32_t scale, uint32_t cycles, int hi)
{
    uint64_t now = global_timer_counter_read();
    uint64_t prod = (uint64_t)cycles * (uint64_t)DAT_00028f50;
    uint32_t lo = (uint32_t)prod;
    /* DAT_00028f50 * param_4: 原代码为 32 位环绕乘法后并入高字 */
    uint32_t hi32 = (uint32_t)(prod >> 32) + DAT_00028f50 * (uint32_t)hi;

    *deadline = (int64_t)(now + ((uint64_t)hi32 << 32) + lo);
    (void)scale;                                        /* 原反编译未使用该参数 */
}

// === bitarray_pack === 原 FUN_0000bed4 @ 0x0000bed4
// 来源: reversed/fsbl_funcs/FUN_0000bed4.c | 分类: fsbl
// 语义: 1-bit-per-byte 数组打包: 先清零 packed[0..ceil(nbits/8)), 再对每个位 b:
// packed[b>>3] |= (bits[b] & 1) << (b&7) (LSB-first)。nbits==0 时仅返回。
void bitarray_pack(const uint8_t *bits, uint8_t *packed, uint32_t nbits)
{
    uint32_t nbytes = nbits >> 3;
    uint32_t b;

    if ((nbits & 7) != 0) {
        nbytes = (nbits >> 3) + 1;
    }
    for (b = 0; b < nbytes; b++) {
        packed[b] = 0;
    }
    for (b = 0; b < nbits; b++) {
        packed[b >> 3] |= (uint8_t)((bits[b] & 1u) << (b & 7));
    }
}

// === hex_string_validate === 原 FUN_0000bfbc @ 0x0000bfbc
// 来源: reversed/fsbl_funcs/FUN_0000bfbc.c | 分类: fsbl
// 语义: 校验十六进制字符串: 长度必须与给定值一致且每个字符都属于 hex 字符表
// (hex_char_invalid 逐字符检查)。成功返回 NULL, 失败返回错误消息指针 (指向
// 字符串池合并的 "Calculated checksum\r\n" 数据块尾部)。
extern const char s_Calculated_checksum_0000f018[];     /* 字符串池数据块 @ 0xf018: "Calculated checksum\r\n..." */
const char *hex_string_validate(const char *s, uint32_t len)
{
    const char *p;
    const char *next;

    if (s == 0) {
        return s_Calculated_checksum_0000f018 + 0xb;    /* "checksum\r\n" */
    }
    if (len == 0) {
        return s_Calculated_checksum_0000f018 + 9;      /* "d checksum\r\n" */
    }
    if (strlen(s) != (int)len) {
        return s_Calculated_checksum_0000f018 + 8;      /* "ed checksum\r\n" */
    }
    p = s;
    do {
        next = p + 1;
        if (hex_char_invalid((const uint8_t *)p) != 0) {
            return s_Calculated_checksum_0000f018 + 10; /* " checksum\r\n" */
        }
        p = next;
    } while (s + len != next);
    return 0;
}

// === hex_to_key32 === 原 FUN_0000c0dc @ 0x0000c0dc
// 来源: reversed/fsbl_funcs/FUN_0000c0dc.c | 分类: fsbl
// 语义: <=64 个 hex 字符 → 32 字节 AES 密钥 (右对齐进 64 字节暂存缓冲)。
// 长度 >0x40 返回 0x20, 否则拷贝 hex[0..len] (含结尾 NUL) 到缓冲尾部后返回 0。
// 注意: 反汇编核实, 函数尾部的 8 次 hex_string_to_bytes(conv_in, out4, 8) 调用
// 输入取自本函数从未写入的栈区 (sp+0x08..0x0F), 8 次参数完全相同且结果不外传 —
// 按二进制原样保留此形态 (疑为编译产物缺陷); ==0x40 拷贝路径还会越过 memset
// 区域写 1 字节到已保存寄存器槽位 (pop 恢复时被覆盖)。
int hex_to_key32(const char *hex)
{
    uint8_t out4[4];                                    /* 实际 sp+0x04 (原 auStack_54) */
    uint8_t conv_in[8];                                 /* 实际 sp+0x08 (原 auStack_50[7]+uStack_49) */
    uint8_t hexbuf[0x42];                               /* 实际 sp+0x10 (原 local_48[64], 拷贝路径越过 +0x40) */
    uint32_t len;
    uint32_t i;

    memset(hexbuf, 0, 0x40);
    len = (uint32_t)strlen(hex);
    if (len > 0x40) {
        return 0x20;
    }
    if (len == 0x40) {
        /* 拷贝 hex[0..0x40] 共 0x41 字节 → hexbuf[0..0x40] */
        for (i = 0; i <= 0x40; i++) {
            hexbuf[i] = (uint8_t)hex[i];
        }
    }
    else {
        /* 右对齐拷贝 hex[0..len] (含 NUL) → hexbuf[0x41-len .. 0x41] */
        for (i = 0; i <= len; i++) {
            hexbuf[0x40 - len + 1 + i] = (uint8_t)hex[i];
        }
    }
    for (i = 8; i != 0; i--) {
        /* 原调用 hex_string_to_bytes(sp+0x08, sp+0x04, 8), 见函数头注释 */
        hex_string_to_bytes((const char *)conv_in, out4, 8);
    }
    return 0;
}

/* ==================== XAdcPs 寄存器访问 (Zynq XADC) ==================== */
/* 实例布局: +4 = 寄存器基址 (0xF8007000), +8 = IsReady (0x11111111);
 * +0x10 = 命令寄存器 (写命令触发), +0x14 = 数据寄存器 (读回)。
 * 注: 实例 +4 处加载的基址指针按反编译源 (整型 +0x10/0x14 字节偏移访问)
 * 转写为字节指针 volatile uint8_t *; p1b.c/p4.c 同一接口已统一到此宽度。 */

// === xadc_cfg3_get === 原 FUN_0000c4d0 @ 0x0000c4d0
// 来源: reversed/fsbl_funcs/FUN_0000c4d0.c | 分类: fsbl
// 语义: 直读配置寄存器 3 (实例基址 +0x0C), 带 Xil Assert 保护。
uint32_t xadc_cfg3_get(int xadc)
{
    if (xadc == 0) {
        dbg_print(0xe2, 0);                             /* 原 FUN_000014e4(s_0___P___p_0001728e+0x12, 0xe2) */
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(xadc + 8) == 0x11111111) {
        return *(volatile uint32_t *)(*(volatile uint8_t **)(xadc + 4) + 0x0C);
    }
    dbg_print(0xe3, 0);
    g_assert_failed = 1;
    return 0;
}

// === xadc_supply_data_get === 原 FUN_0000c888 @ 0x0000c888
// 来源: reversed/fsbl_funcs/FUN_0000c888.c | 分类: fsbl
// 语义: 读供电通道数据: 命令寄存器写 ((chan+0x20)<<16 | 0x04000000) 触发读
// (通道号 0x20..0x2E 区段), 数据寄存器取低 16 位。通道合法性: 0..10 恒可,
// 12/13/14 可 (chan-0xC<=2 或 ==2), 11 及 >=15 断言失败返回 0。
uint32_t xadc_supply_data_get(int xadc, uint32_t chan)
{
    volatile uint8_t *base;
    int out_of_range;
    int special;

    if (xadc == 0) {
        dbg_print(0x1b4, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(xadc + 8) != 0x11111111) {
        dbg_print(0x1b5, 0);
        g_assert_failed = 1;
        return 0;
    }
    if (chan > 10) {
        out_of_range = (uint32_t)(1 < chan - 0xC);
        special = ((chan - 0xC) == 2);
    }
    else {
        out_of_range = (uint32_t)(9 < chan);
        special = (chan == 10);
    }
    if (out_of_range && !special) {
        dbg_print(0x1b8, 0);
        g_assert_failed = 1;
        return 0;
    }
    base = *(volatile uint8_t **)(xadc + 4);
    *(volatile uint32_t *)(base + 0x10) = ((chan + 0x20) << 16) | 0x04000000u;
    *(volatile uint32_t *)(base + 0x10) = *(volatile uint32_t *)(base + 0x14);   /* 触发读回 */
    return *(volatile uint32_t *)(base + 0x14) & 0xFFFFu;
}

/* ==================== XADC Sequence 寄存器访问 ==================== */

// === xadc_seq_mode_set === 原 FUN_0000cdd4 @ 0x0000cdd4
// 来源: reversed/fsbl_funcs/FUN_0000cdd4.c | 分类: fsbl
// 语义: 设置 Sequence 寄存器 0x41 的模式位域 bit[15:12]: 先读回旧值 (命令
// 0x04410000), 再写命令 0x08410000 | (旧值 & 0xFFF) | (mode & 0xF)<<12。
// 合法模式: 0..3, 4, 8; 其它值断言失败直接返回。
void xadc_seq_mode_set(int xadc, uint32_t mode)
{
    volatile uint8_t *base;
    int invalid;

    if (xadc == 0) {
        dbg_print(0x368, 0);
        g_assert_failed = 1;
        return;
    }
    g_assert_failed = 0;
    if (*(int *)(xadc + 8) != 0x11111111) {
        dbg_print(0x369, 0);
        g_assert_failed = 1;
        return;
    }
    /* 合法性判断按反编译原样: mode==8 → (7<mode); 否则 (3<mode);
     * 失败条件再排除 8 和 4 → 有效集合 {0,1,2,3,4,8} */
    invalid = (uint32_t)(7 < mode);
    if (mode != 8) {
        invalid = (uint32_t)(3 < mode);
    }
    if (invalid && (mode != 8) && (mode != 4)) {
        dbg_print(0x36b, 0);
        g_assert_failed = 1;
        return;
    }
    base = *(volatile uint8_t **)(xadc + 4);
    *(volatile uint32_t *)(base + 0x10) = 0x04410000u;              /* 读 reg 0x41 */
    *(volatile uint32_t *)(base + 0x10) = *(volatile uint32_t *)(base + 0x14);   /* 触发读回 */
    *(volatile uint32_t *)(base + 0x10) =
        (*(volatile uint32_t *)(base + 0x14) & 0xFFFu) | ((mode & 0xFu) << 12) | 0x08410000u;
}

// === xadc_seq_mode_get === 原 FUN_0000ceb8 @ 0x0000ceb8
// 来源: reversed/fsbl_funcs/FUN_0000ceb8.c | 分类: fsbl
// 语义: 读 Sequence 模式位域: 命令 0x04410000 读 reg 0x41,
// 返回 (data & 0xFFFF) >> 12 (即 bit[15:12])。
uint32_t xadc_seq_mode_get(int xadc)
{
    volatile uint8_t *base;

    if (xadc == 0) {
        dbg_print(0x393, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(xadc + 8) == 0x11111111) {
        base = *(volatile uint8_t **)(xadc + 4);
        *(volatile uint32_t *)(base + 0x10) = 0x04410000u;
        *(volatile uint32_t *)(base + 0x10) = *(volatile uint32_t *)(base + 0x14);
        return (*(volatile uint32_t *)(base + 0x14) & 0xFFFFu) >> 12;
    }
    dbg_print(0x394, 0);
    g_assert_failed = 1;
    return 0;
}

// === xadc_seq_ch0_get === 原 FUN_0000d300 @ 0x0000d300
// 来源: reversed/fsbl_funcs/FUN_0000d300.c | 分类: fsbl
// 语义: 读 Sequence CH0/CH1 对 (reg 0x48/0x49, 命令 0x04480000/0x04490000),
// 返回 32 位组合: (ch0 & 0x7FE1) | ch1<<16。
uint32_t xadc_seq_ch0_get(int xadc)
{
    volatile uint8_t *base;
    uint32_t ch0;

    if (xadc == 0) {
        dbg_print(0x434, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(xadc + 8) == 0x11111111) {
        base = *(volatile uint8_t **)(xadc + 4);
        *(volatile uint32_t *)(base + 0x10) = 0x04480000u;          /* 读 reg 0x48 */
        *(volatile uint32_t *)(base + 0x10) = *(volatile uint32_t *)(base + 0x14);
        ch0 = *(volatile uint32_t *)(base + 0x14);
        *(volatile uint32_t *)(base + 0x10) = 0x04490000u;          /* 读 reg 0x49 */
        *(volatile uint32_t *)(base + 0x10) = *(volatile uint32_t *)(base + 0x14);
        return (ch0 & 0x7FE1u) | (*(volatile uint32_t *)(base + 0x14) << 16);
    }
    dbg_print(0x435, 0);
    g_assert_failed = 1;
    return 0;
}

// === xadc_seq_ch1_set === 原 FUN_0000d3cc @ 0x0000d3cc
// 来源: reversed/fsbl_funcs/FUN_0000d3cc.c | 分类: fsbl
// 语义: 写 Sequence CH0/CH1 对 (param = {ch1<<16 | ch0&0x7FE0}):
// 要求当前模式为 0 (xadc_seq_mode_get), 否则返回 1; 写命令 0x084A0000 (reg 0x4A)
// 写 ch0 字 (param & 0x7FE0), 0x084B0000 (reg 0x4B) 写 ch1 字 (param >> 16)。
int xadc_seq_ch1_set(int xadc, uint32_t param)
{
    volatile uint8_t *base;
    int mode;

    if (xadc == 0) {
        dbg_print(0x463, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(xadc + 8) != 0x11111111) {
        dbg_print(0x464, 0);
        g_assert_failed = 1;
        return 0;
    }
    mode = xadc_seq_mode_get(xadc);     /* 原反编译 0 参调用 (r0 沿用 param_1) */
    if (mode == 0) {
        base = *(volatile uint8_t **)(xadc + 4);
        *(volatile uint32_t *)(base + 0x10) = (param & 0x7FE0u) | 0x084A0000u;
        *(volatile uint32_t *)(base + 0x10) = (param >> 16) | 0x084B0000u;
        return 0;
    }
    return 1;
}

// === xadc_alarm_thr_get === 原 FUN_0000d938 @ 0x0000d938
// 来源: reversed/fsbl_funcs/FUN_0000d938.c | 分类: fsbl
// 语义: 读报警阈值寄存器 (sel = 0..0xF 对应 reg 0x50..0x5F): 写命令
// ((sel+0x50)<<16 | 0x04000000) 触发读, 返回数据低 16 位。sel >= 0x10 断言失败。
uint32_t xadc_alarm_thr_get(int xadc, uint32_t sel)
{
    volatile uint8_t *base;

    if (xadc == 0) {
        dbg_print(0x5a5, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(xadc + 8) != 0x11111111) {
        dbg_print(0x5a6, 0);
        g_assert_failed = 1;
        return 0;
    }
    if (sel < 0x10) {
        base = *(volatile uint8_t **)(xadc + 4);
        *(volatile uint32_t *)(base + 0x10) = ((sel + 0x50) << 16) | 0x04000000u;
        *(volatile uint32_t *)(base + 0x10) = *(volatile uint32_t *)(base + 0x14);
        return *(volatile uint32_t *)(base + 0x14) & 0xFFFFu;
    }
    dbg_print(0x5a7, 0);
    g_assert_failed = 1;
    return 0;
}

// === efuse_write_rows === 原 FUN_0000e88c @ 0x0000e88c
// 来源: reversed/fsbl_funcs/FUN_0000e88c.c | 分类: fsbl
// 语义: 将 256 位用户数据 (data = 32 字节) 逐位烧写到 eFuse 行:
// bitarray_unpack 展开为位工作区 (0x104 字节, 先清零) → crc_syndrome_table_init
// 建校验子表 → 外层 10 组 (每组以 0x1A 字节步进取工作区分片, crc_syndrome_calc
// 生成 31 位校验位组), 内层对每组 31 位: 行地址自 0xF800E080 步进 +4 (保留行
// 额外 +4 跳过, 此形态按反编译原样保留), 位==1 时 efuse_program_row_pair 烧写
// 当前行, 出错原样返回其错误码。全部完成返回 0。
int efuse_write_rows(const uint8_t *data, uint32_t freq)
{
    uint8_t check[32];                                  /* local_144: 每组校验位 (bit0 有效) */
    uint8_t work[264];                                  /* auStack_124: 位展开工作区 */
    uint32_t addr;                                      /* iVar3 */
    int grp;                                            /* iVar4 */
    int j;                                              /* iVar5 */
    int err;
    const uint8_t *bit;

    memset(work, 0, 0x104);
    addr = 0xF800E080;                                  /* 原 -0x7ff1f80 (int) 同值 */
    grp = 0;
    bitarray_unpack(data, work, 0x100);                 /* 256 字节 → 1-bit-per-byte */
    crc_syndrome_table_init();
    for (;;) {
        j = 0;
        crc_syndrome_calc(work + grp * 0x1A, check);
        do {
            err = efuse_addr_is_reserved(addr);
            bit = check + j;
            if (err == 1) {
                addr += 4;                              /* 保留行: 额外跳过一行 */
            }
            j++;
            if (((*bit & 1) != 0) &&
                ((err = efuse_program_row_pair(addr, freq), err != 0))) {
                return err;
            }
            addr += 4;
        } while (j != 0x1F);
        grp++;
        if (grp == 10) {
            return 0;
        }
    }
}

/* ==================== 缓存/异常入口 ==================== */

// === icache_invalidate_wfi === 原 FUN_0000e948 @ 0x0000e948
// 来源: reversed/fsbl_funcs/FUN_0000e948.c | 分类: fsbl
// 语义: 关机/跳转前收尾序列: CP15 ICIALLU (c7,c5,0) 整体无效 I-Cache →
// ICIMVAU (c7,c5,6) 按 MVA 无效指令缓存 → DSB SY → ISB SY → SCTLR (c1,c0,0)
// 清零 (关 MMU/Cache) → ISB SY → 死循环 WFE (WaitForEvent) 等待停机。
// 反编译内部名: coproc_moveto_Invalidate_Entire_Instruction / coproc_moveto_
// Instruction_cache / coproc_moveto_Control。
void icache_invalidate_wfi(uint32_t mva)
{
    __asm__ volatile ("mcr p15, 0, %0, c7, c5, 0" :: "r"(mva));     /* ICIALLU */
    __asm__ volatile ("mcr p15, 0, %0, c7, c5, 6" :: "r"(mva));     /* ICIMVAU */
    __asm__ volatile ("dsb sy");                                    /* DataSynchronizationBarrier(0xF) */
    __asm__ volatile ("isb sy");                                    /* InstructionSynchronizationBarrier(0xF) */
    __asm__ volatile ("mcr p15, 0, %0, c1, c0, 0" :: "r"(0));       /* SCTLR = 0 */
    __asm__ volatile ("isb sy");
    for (;;) {
        __asm__ volatile ("wfe");                                   /* WaitForEvent */
    }
}

// === PrefetchAbort === 原 FUN_0000000c @ 0x0000000c
// 来源: reversed/fsbl_funcs/PrefetchAbort.c | 分类: fsbl
// 语义: 预取中止异常入口 (向量表第 3 项 @ 0x0C)。反编译产物受损 (反汇编遇错
// 截断), 可辨识内容: memcpy(FUN_00002200)(in_r3 + unaff_r7) 后 halt_baddata()。
// 与 SupervisorCall (p4.c) 同为运行时库"已损坏指令"占位实现, 实际固件中由 boot
// 头填充真正的处理代码; 按反编译原样转写为不可恢复的挂死入口, 不虚构额外语义。
void PrefetchAbort(void)
{
    /* 原始流: FUN_00002200(in_r3 + unaff_r7) — 参数寄存器残缺, 语义不可恢复 */
    hang_forever();                                     /* halt_baddata() 等价挂死 */
}

/* ============================================================================
 * 覆盖率表 — batch=fsbl_b2 按地址升序后 20 个 (20/20)
 * | #  | 语义名                 | 原符号        | 地址        | 一句话语义 |
 * |----|-----------------------|---------------|-------------|------------|
 * | 1  | sd_set_bus_width      | FUN_00007f5c  | 0x00007f5c  | SD CMD55+ACMD6 / eMMC CMD6 切总线宽度, 置 SDHCI Host Control |
 * | 2  | sd_switch_func_check  | FUN_00008168  | 0x00008168  | SD CMD6 检查模式 (0xFFFFF0, 64B ADMA 读状态) |
 * | 3  | mmc_switch_cmd6       | FUN_00008994  | 0x00008994  | eMMC CMD6 SWITCH 裸封装 + 等传输完成 |
 * | 4  | sha256_hash           | FUN_00009924  | 0x00009924  | 一次性 SHA-256 (栈上 104B ctx, init+update+final) |
 * | 5  | sha256_state_reset    | FUN_00009974  | 0x00009974  | memcpy 32 字节 H0 初值 (s_P0+2) 入 ctx |
 * | 6  | efuse_program         | FUN_0000b710  | 0x0000b710  | eFuse 编程总控: 解锁/检查/时序/空白扫描/逐行编程, 状态码 0x81xx~0x99xx |
 * | 7  | timeout_deadline_set  | FUN_0000bcd8  | 0x0000bcd8  | 截止值 = 全局定时器计数 + cycles*scale (+hi<<32) |
 * | 8  | bitarray_pack         | FUN_0000bed4  | 0x0000bed4  | 1-bit-per-byte 数组 → packed 字节 (LSB-first) |
 * | 9  | hex_string_validate   | FUN_0000bfbc  | 0x0000bfbc  | hex 串长度+字符表校验, 失败返回错误串指针, 成功 NULL |
 * | 10 | hex_to_key32          | FUN_0000c0dc  | 0x0000c0dc  | <=64 hex 字符右对齐入 64B 缓冲 (>0x40 返回 0x20); 尾部 8 次 hex_string_to_bytes 输入为未初始化栈 (按二进制保留) |
 * | 11 | xadc_cfg3_get         | FUN_0000c4d0  | 0x0000c4d0  | 直读 XADC 实例 +0x0C (Cfg3), 带断言 |
 * | 12 | xadc_supply_data_get  | FUN_0000c888  | 0x0000c888  | 读供电通道 (0x20+chan, 0x20..0x2A/0x2C..0x2E) 数据低 16 位 |
 * | 13 | xadc_seq_mode_set     | FUN_0000cdd4  | 0x0000cdd4  | 写 reg 0x41 模式位 bit[15:12] (保留旧低 12 位), 模式 0..3/4/8 |
 * | 14 | xadc_seq_mode_get     | FUN_0000ceb8  | 0x0000ceb8  | 读 reg 0x41, 返回 (data&0xFFFF)>>12 模式位 |
 * | 15 | xadc_seq_ch0_get      | FUN_0000d300  | 0x0000d300  | 读 reg 0x48/0x49 对, 返回 (ch0&0x7FE1)|ch1<<16 |
 * | 16 | xadc_seq_ch1_set      | FUN_0000d3cc  | 0x0000d3cc  | 模式 0 时写 reg 0x4A/0x4B = param&0x7FE0 / param>>16 |
 * | 17 | xadc_alarm_thr_get    | FUN_0000d938  | 0x0000d938  | 读报警阈值 reg 0x50+sel (sel<0x10) 低 16 位 |
 * | 18 | efuse_write_rows      | FUN_0000e88c  | 0x0000e88c  | 256 位用户数据按位烧写 eFuse 行 (0xF800E080 起步进 +4, 10 组×31 校验位) |
 * | 19 | icache_invalidate_wfi | FUN_0000e948  | 0x0000e948  | CP15 ICIALLU/ICIMVAU + DSB/ISB + SCTLR=0 + WFE 死循环 |
 * | 20 | PrefetchAbort         | PrefetchAbort | 0x0000000c  | 预取中止向量入口 (反编译受损, halt_baddata 占位) |
 * ============================================================================ */

