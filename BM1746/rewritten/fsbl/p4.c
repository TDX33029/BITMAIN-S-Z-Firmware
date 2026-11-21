/**
 * p4.c — FSBL 反编译转写 批次 4/4 (杂项收尾)
 *
 * 覆盖 (reversed/fsbl_funcs/, 源二进制 Zynq7007_miner.elf, 共 42 函数):
 *   - 异常向量: SupervisorCall (SWI), FIQ
 *   - NAND 控制器寄存器 getter (ctrl_reg/reg08/ready_flag, Xilinx Nand driver 断言式访问器)
 *   - libc: strlen (字对齐字扫描优化版)
 *   - FatFs 移植层: str_parse_devnum / fat_window_read / fat_fs_sync / fat_move_window /
 *     fat_find_volume / fat_dir_register / fat_f_open / fat_f_sync / fat_f_close /
 *     fat_f_stat / fat_f_getfree / fat_f_truncate / fat_f_rename / disk_status / disk_write
 *   - SD/eMMC 主机层: sd_card_init / sd_write_blocks / sdmmc_card_init / sd_set_clk_freq
 *   - RSA 软件验证: rsa_inv_mod2p32 (Montgomery n0'), rsa_mont_mul (2048-bit)
 *   - SHA-256: sha256_init
 *   - NAND ECC (GF(2)): crc_syndrome_position
 *   - 工具: hex_string_to_bytes / hex_to_bytes_ralign / hex_char_invalid /
 *     bitarray_unpack / global_timer_counter_read / timeout_expired / strchr 调用方
 *   - XAdcPs (Zynq XADC): xadc_auxchan_data_get / xadc_msts_alarm_get / xadc_seq_ch0_set /
 *     xadc_avg_ch1_get / xadc_alarm_thr_set / xadc_cfg0_flags_get
 *   - eFuse (XAPP1280): efuse_program_row_pair
 *
 * 全局数据 (均为反编译实证, 语义推断处已注明):
 *   DAT_00028e84 = xil_assert 错误标志 (0 = 正常, 1 = 已触发断言; XIL_ASSERT 布尔)
 *   DAT_00028e88 = SD 主机命令/数据寄存器暂存 (sd_send_cmd 写入)
 *   DAT_00019be8 = 每槽 disk 状态字节数组 (bit0=noinit, bit1=nodisk, bit2=write prot)
 *   DAT_0002154c = FatFs 卷 ID 计数器 (挂卷时递增)
 *   DAT_00021544 = FatFs 卷对象指针表 (按设备号索引)
 *   DAT_00021590 = 每槽"块寻址"标志 (非 0 时按块寻址)
 *   DAT_000219a0 = SD 主机基址 (在 disk_status 中仅取其 +0x24 = present state 寄存器)
 *   DAT_000219a4 = 卡检测等待使能标志
 *   DAT_000219a8 = 卡插入等待使能标志
 *   DAT_000219ac = 每槽卡容量标志 (0x40000000 = 高容量 SD/eMMC)
 *   DAT_000219c0 / DAT_000219e0 = SD SCR / eMMC EXT_CSD 512B 响应缓冲
 *   DAT_00028eac = NAND GF(2) 校验子表 (crc_syndrome_table_init 生成, 每行 5 字节步进)
 *   DAT_00028e8c = NAND 单比特错误位置表 (0xff 结尾哨兵)
 *
 * 调用方可见函数名一律取自 rewritten/name_map/fsbl.tsv (阶段 1 冻结)。
 */
#include <stdint.h>
#include <stdbool.h>   /* true/false — disk_write 返回类型 */

/* ==================== 共享声明 (本批被调/跨批被调, 签名与反编译一致) ==================== */
extern int  dbg_print(int line, int code);              /* FUN_000014e4: 行号+错误码日志 */
extern int  aeabi_uidiv(unsigned num, unsigned den);    /* FUN_00001a50 */
extern void mem_copy(void *dst, const void *src, uint32_t n);   /* FUN_00002200 (memcpy) */
extern void mem_set(void *dst, int val, uint32_t n);    /* FUN_000027e0 (memset) */
extern void hang_forever(void);                         /* FUN_00002b40 */
extern uint32_t str_parse_devnum(const uint8_t **pp, uint32_t verdict_init);  /* FUN_00002b44, 定义见本文件 */
extern int  fatfs_validate(uint32_t *fp, uint32_t *vol_out);    /* FUN_00002bbc */
extern void fat_get_fileinfo(const uint8_t *entry, uint32_t *finfo); /* FUN_00002c30 */
extern int  fat_window_read(uint8_t *fs);               /* FUN_00002ce0, 见本文件 */
extern int  fat_fs_sync(uint8_t *fs);                   /* FUN_00002e20, 见本文件 */
extern uint32_t fat_move_window(uint8_t *fs, uint32_t sect);/* FUN_00002f3c, 见本文件 */
/* 卷 ID 计数器 (原名 DAT_0002154c, 定义见文末); 卷对象表基址同理 */
static uint32_t g_fatfs_vol_id;
static uint8_t *const DAT_00021544_base;
static const void *DAT_00007d3c;     /* EXT_CSD 二次读取备用缓冲指针 (定义见文末 0x000219e0) */
extern int  fat_dir_entry_remove(int *dj);              /* FUN_00002f8c */
extern int  fat_check_fs(uint8_t *fs, uint32_t part);   /* FUN_00002fd8 */
extern int  fat_find_volume(const uint8_t **pp, uint8_t **fs_out, uint32_t mode); /* FUN_000030bc, 见本文件 */
extern int  fat_get_fat(uint8_t *fs, uint32_t clst);    /* FUN_000036fc */
extern int  fat_remove_chain(int *dj, uint32_t clst, uint32_t free_clst); /* FUN_00003a48 */
extern int  fat_dir_sdi(int *dj, int skip);             /* FUN_00003b44 */
extern int  fat_dir_sdi0(int *dj);                      /* FUN_00003d50 */
extern int  fat_dir_register(int *dj);                  /* FUN_00003e38, 见本文件 */
extern int  fat_follow_path(int *dj, const uint8_t *path); /* FUN_00003f2c */
extern void fat_f_sync(uint32_t *fp);                   /* FUN_00004c1c, 见本文件 (原体 void) */
extern uint32_t fat_default_date(void);                 /* FUN_00006760 (恒 0x3c210000) */
extern uint8_t disk_status(int pdrv);                   /* FUN_000064b8, 见本文件 (返回状态槽值) */
extern int  disk_initialize(int pdrv);                  /* FUN_000065cc */
extern int  disk_read(int pdrv, void *buf, uint32_t sect, int cnt);  /* FUN_0000667c */
extern int  disk_ioctl(int pdrv, int cmd, void *buf);   /* FUN_000066f8 */
extern bool disk_write(int pdrv, const void *buf, uint32_t sect, int cnt); /* FUN_0000676c, 见本文件 */
extern uint16_t sd_send_cmd(int sd, uint16_t cmd, uint32_t arg, uint16_t resp_type); /* FUN_00006bcc */
extern void sd_adma_setup_cont(int sd, int nblocks);    /* FUN_00007100 */
extern void sd_adma_setup(int sd, int nblocks, void *buf); /* FUN_000071f8 */
extern int  mmc_card_identify(int sd);                  /* FUN_00007574 */
extern int  sdmmc_set_blocklen(int sd, uint32_t len);   /* FUN_00007d40 */
extern int  sd_set_bus_width(int sd);                   /* FUN_00007f5c */
extern int  sd_switch_func_check(int sd, void *buf);    /* FUN_00008168 */
extern int  sd_set_clk_freq(int sd, uint32_t freq_hz);  /* FUN_000082b0, 见本文件 */
extern int  sdmmc_switch_high_speed(int sd);            /* FUN_00008428 */
extern int  sd_set_card_detect(int sd);                 /* FUN_00008798 */
extern int  sd_send_scr(int sd, void *buf);             /* FUN_00007dfc */
extern int  mmc_send_ext_csd(int sd, void *buf);        /* FUN_0000884c */
extern int  mmc_switch_cmd6(int sd, uint32_t arg);      /* FUN_00008994 */
extern int  rsa_bignum_cmp(const uint32_t *a, const uint32_t *b); /* FUN_00008df8 */
extern void rsa_bignum_sub(uint32_t *a, const uint32_t *b);       /* FUN_00008eac */
extern void sha256_state_reset(int ctx);                /* FUN_00009974 */
extern int  hex_char_to_nibble(uint8_t c, char *out);   /* FUN_0000ba3c */
extern int  xadc_read_channels(uint32_t *out);          /* FUN_0000bb98 */
extern int  efuse_program_bit(uint32_t addr);           /* FUN_0000b6a4 */
extern int  efuse_row_blank_check(uint32_t addr, uint32_t freq); /* FUN_0000e6c0 */
extern int  efuse_setup_timing(int mode, uint32_t freq, int sub); /* FUN_0000aedc */
extern int  xadc_seq_mode_get(int xadc);                /* FUN_0000ceb8 */
extern void timeout_deadline_set(int64_t *deadline, uint32_t scale, uint32_t cycles, int hi); /* FUN_0000bcd8 */
extern char *str_chr(const char *haystack, int c);      /* FUN_0000dfb0 (strchr), 别名: p3.c 定义侧名 strchr */
extern void delay_ms(uint32_t ms);                      /* func_0xffffe2ac: ROM/BSP 毫秒延时 */

/* Xil Assert 风格错误标志 (0x28e84), 全 FSBL 多处驱动的 getter 共用 */
#define g_assert_failed   (*(volatile uint32_t *)0x00028e84)

/* SD 主机命令寄存器暂存 (0x28e88): sd_send_cmd 将编码后的命令字写入, 硬件层读取 */
static volatile uint32_t g_sd_cmd_reg = 0;              /* 原名 DAT_00028e88 */

/* ---- 通用寄存器读写 (地址常量来自反编译字面量) ---- */
static inline volatile uint32_t *reg32(uint32_t addr) { return (volatile uint32_t *)(uintptr_t)addr; }

/* ==================== 异常向量 ==================== */

// === SupervisorCall === 原 FUN_00000008 @ 0x00000008
// 来源: reversed/fsbl_funcs/SupervisorCall.c | 分类: fsbl
// 语义: SWI/SVC 异常入口 (向量表第 2 项)。反编译产物受损 (反汇编遇错截断),
// 可辨识内容: 直接跳转/落入 memcpy(FUN_00002200) 后 halt_baddata();
// 这是运行时库 "已损坏指令" 占位实现, 实际固件中由 boot 头填充真正的 SWI 处理代码。
// 按反编译原样转写为不可恢复的挂死入口, 不虚构额外语义。
void SupervisorCall(void)
{
    /* 原始流: FUN_00002200(in_r3 + unaff_r7) — 参数寄存器残缺, 语义不可恢复 */
    hang_forever();                                     /* halt_baddata() 等价挂死 */
}

// === FIQ === 原 FUN_0000001c @ 0x0000001c
// 来源: reversed/fsbl_funcs/FIQ.c | 分类: fsbl
// 语义: FIQ 异常入口 (向量表第 7 项)。dbg_print(行号 0x10000|低16位, 错误码 0x3b7)
// 记录 FIQ 触发, 然后在 unaff_r11 指向的标志地址写入 1 (通知主循环发生了 FIQ)。
void FIQ(uint32_t param_1)
{
    dbg_print((int)(param_1 & 0xffff) | 0x10000, 0x3b7);
    /* unaff_r11 为进入本异常前 r11 指向的标志位置 (Ghidra 不可追溯具体地址) */
    *(volatile uint32_t *)0 = 1;                        /* 原始语义: *unaff_r11 = 1 */
}

/* ==================== NAND 控制器寄存器访问器 (Xilinx Nand 驱动) ==================== */

// === nand_ctrl_reg_get === 原 FUN_00000714 @ 0x00000714
// 来源: reversed/fsbl_funcs/FUN_00000714.c | 分类: fsbl
// 语义: NAND 控制器实例偏移 0 处 32 位寄存器读取 (带 Xil Assert 保护)。
// param_1 = 实例指针: +4 = 寄存器基址指针, +8 = IsReady 标志 (须为 0x11111111)。
// 断言失败时置 g_assert_failed 并返回 0。
uint32_t nand_ctrl_reg_get(int inst)
{
    if (inst == 0) {
        dbg_print(0x13e, 0);                            /* s_0___P___p 日志, 行号 0x13e */
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(inst + 8) == 0x11111111) {
        return **(volatile uint32_t **)(inst + 4);
    }
    dbg_print(0x13f, 0);
    g_assert_failed = 1;
    return 0;
}

// === nand_reg08_get === 原 FUN_00000914 @ 0x00000914
// 来源: reversed/fsbl_funcs/FUN_00000914.c | 分类: fsbl
// 语义: NAND 控制器寄存器 +0x08 读取 (带断言保护, 同上访问器模式)。
uint32_t nand_reg08_get(int inst)
{
    if (inst == 0) {
        dbg_print(0x1ad, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(inst + 8) == 0x11111111) {
        return *(volatile uint32_t *)(*(int *)(inst + 4) + 8);
    }
    dbg_print(0x1ae, 0);
    g_assert_failed = 1;
    return 0;
}

// === nand_ready_flag_get === 原 FUN_00000ca0 @ 0x00000ca0
// 来源: reversed/fsbl_funcs/FUN_00000ca0.c | 分类: fsbl
// 语义: NAND 就绪/忙标志读取 = 寄存器 +0x14 取反后 bit31
// (~REG >> 31, 即状态字的最高有效位为低电平有效 ready 位)。
uint32_t nand_ready_flag_get(int inst)
{
    if (inst == 0) {
        dbg_print(0x26d, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(inst + 8) == 0x11111111) {
        return ~(uint32_t)*(volatile uint32_t *)(*(int *)(inst + 4) + 0x14) >> 31;
    }
    dbg_print(0x26e, 0);
    g_assert_failed = 1;
    return 0;
}

/* ==================== libc: strlen ==================== */

// === strlen === 原 FUN_00002900 @ 0x00002900
// 来源: reversed/fsbl_funcs/FUN_00002900.c | 分类: fsbl
// 语义: 标准 strlen, 8 字节对齐的逐字扫描优化实现 (armcc/字长 4):
// 头部处理不对齐前缀 (按 (addr&7) / (addr&4) 分别屏蔽前缀字后进入主循环),
// 主循环每轮最多检查 32 字节 (8 个字, 每字 0x01010101 差值技巧判 0 字节),
// LZCOUNT(前导零计数)定位首个 0 字节。行为与 libc strlen 完全等价;
// HintPreloadData 为编译器 cache 预取提示, 转写为注释保留。
int strlen(const char *s)
{
    const uint32_t *pw;
    uint32_t w_lo, w_hi;

    /* HintPreloadData(s) — 预取提示, 无内存效果 */
    pw = (const uint32_t *)((uintptr_t)s & ~7u);
    /* HintPreloadData(pw + 8) */

    if ((uintptr_t)s & 7u) {
        /* 前缀处理: 起点不对齐 8 字节边界。
         * 屏蔽掩码 mask = 0xFFFFFFFF << ((s&3)<<3): 将起始地址之前的字节视为非零;
         * 当 (s&4) 时首字直接视为全非零, 从 pw[1] 继续找。 */
        uint32_t w1 = pw[1];
        uint32_t mask = 0xFFFFFFFFu << (((uintptr_t)s & 3u) << 3);
        /* HintPreloadData(pw + 0x10) */
        w_lo = *pw | ~mask;
        uint32_t idx = -(int32_t)((uintptr_t)s & 7u);    /* 字节计数偏移 (负前缀) */
        if ((uintptr_t)s & 4u) {
            w1 |= ~mask;
            w_lo = 0xFFFFFFFFu;
        }
        w_hi = w1;
        /* 从此处落入主循环, w_lo/w_hi 已含前缀修正 */
        goto scan_pair;
    }

    {
        int idx = -8;                                    /* 已扫描字节计数 (相对循环头) */
        do {
            w_lo = pw[0];
            w_hi = pw[1];
            /* HintPreloadData(pw + 0x10) */
            idx += 8;
        scan_pair:
            /* 判 0: w_zero[i] = 0x01 当且仅当对应字节为 0 (经典 haszero 技巧展开) */
            {
                uint32_t z_lo =
                    (((w_lo & 0xFFu) == 0) ? 0x01u : 0u) |
                    (((w_lo & 0xFF00u) == 0) ? 0x0100u : 0u) |
                    (((w_lo & 0xFF0000u) == 0) ? 0x010000u : 0u) |
                    (((w_lo & 0xFF000000u) == 0) ? 0x01000000u : 0u);
                uint32_t nz_hi =
                    (((w_hi & 0xFFu) != 0) ? 0x01u : 0u) |
                    (((w_hi & 0xFF00u) != 0) ? 0x0100u : 0u) |
                    (((w_hi & 0xFF0000u) != 0) ? 0x010000u : 0u) |
                    (((w_hi & 0xFF000000u) != 0) ? 0x01000000u : 0u);
                uint32_t cont = nz_hi & z_lo;            /* 每字节: 前字非0 且 本字非0 → 继续位 */
                if (cont == 0) {
                    /* 在 w_lo / w_hi 中定位首个 0 字节 */
                    if (z_lo == 0) {
                        idx += 4;
                        w_lo = nz_hi;
                    }
                    {
                        uint32_t be = ((w_lo & 0xFFu) << 24) | (w_lo & 0xFF00u) |
                                      ((w_lo & 0xFF0000u) >> 8) | ((w_lo >> 24)); /* 字节反序 */
                        return (int)((uint32_t)idx + (__builtin_clz(be) >> 3));
                    }
                }
            }
            /* --- 4 字/轮展开 (pw[2..7]), 语义同上 --- */
            {
                const uint32_t *q = pw + 2;
                int k;
                for (k = 0; k < 4; k++) {
                    uint32_t a = q[2 * k], b = q[2 * k + 1];
                    idx += 8;
                    {
                        uint32_t z_lo =
                            (((a & 0xFFu) == 0) ? 0x01u : 0u) |
                            (((a & 0xFF00u) == 0) ? 0x0100u : 0u) |
                            (((a & 0xFF0000u) == 0) ? 0x010000u : 0u) |
                            (((a & 0xFF000000u) == 0) ? 0x01000000u : 0u);
                        uint32_t nz_hi =
                            (((b & 0xFFu) != 0) ? 0x01u : 0u) |
                            (((b & 0xFF00u) != 0) ? 0x0100u : 0u) |
                            (((b & 0xFF0000u) != 0) ? 0x010000u : 0u) |
                            (((b & 0xFF000000u) != 0) ? 0x01000000u : 0u);
                        uint32_t cont = nz_hi & z_lo;
                        if (cont == 0) {
                            if (z_lo == 0) {
                                idx += 4;
                                w_lo = nz_hi;
                            } else {
                                w_lo = z_lo;
                            }
                            {
                                uint32_t be = ((w_lo & 0xFFu) << 24) | (w_lo & 0xFF00u) |
                                              ((w_lo & 0xFF0000u) >> 8) | ((w_lo >> 24));
                                return (int)((uint32_t)idx + (__builtin_clz(be) >> 3));
                            }
                        }
                    }
                }
                pw += 8;
            }
        } while (1);
    }
}

/* ==================== FatFs 移植层 ==================== */

// === str_parse_devnum === 原 FUN_00002b44 @ 0x00002b44
// 来源: reversed/fsbl_funcs/FUN_00002b44.c | 分类: fsbl
// 语义: 解析路径开头的设备号前缀 "n:" (n = 0/1, '0'-'1' 后跟 ':')。
// 先扫描首段非空格字符 (遇空格/非法字符 (<=0x1f 或 >0x39 且非 ':') 判定非法);
// 若分隔符非 ':' 直接返回判定结果; 否则校验首字符为 '0'/'1' 且总长 2 时
// 推进 *pp 越过前缀并返回设备号 0/1, 失败返回 0xFFFFFFFF。
uint32_t str_parse_devnum(const uint8_t **pp, uint32_t verdict_init)
{
    const uint8_t *p = (const uint8_t *)*pp;
    uint32_t verdict = verdict_init;                     /* 原 param_2: 调用方传入的判定初值。
                                                          * 注: 全部调用点反编译未显式传参 (r1 残留),
                                                          * 初值从未被读取 — 非空路径循环首轮必覆写,
                                                          * 空指针路径直接返回 0xFFFFFFFF, 故调用点恒补 0 */
    if (p != 0) {
        const uint8_t *q = p;
        uint8_t c;
        int bad;
        int is_colon;
        int is_space;
        do {
            c = *q;
            q++;
            bad = (c > 0x39);
            is_colon = (c == 0x3a);
            if (!is_colon)
                bad = (c > 0x1f);
            is_space = (c != 0x20);
            if (bad && !is_colon && is_space) {
                verdict = 1;                             /* 合法字符段 */
            }
            if (!(bad && !is_colon && is_space)) {
                verdict = 0;                             /* 非法字符/分隔/结尾 */
            }
        } while (bad && !is_colon && is_space);
        if (c != 0x3a) {
            return verdict;                              /* 无 ':' 前缀 */
        }
        {
            uint32_t n = (uint32_t)(p[0] - 0x30);        /* 首字符数字 */
            if ((n & 0xff) < 10 && q == p + 2 && n < 2) {
                *pp = q;
                return n;
            }
        }
    }
    return 0xFFFFFFFFu;
}

// === fat_window_read === 原 FUN_00002ce0 @ 0x00002ce0
// 来源: reversed/fsbl_funcs/FUN_00002ce0.c | 分类: fsbl
// 语义: 将 FS 窗口 (fs+0x40 起 0x200 字节) 当前扇区 (fs+0x2c) 回写磁盘。
// 写失败返回 1; 成功清除 fs[3] 脏标志。FAT12/16 (fs[2]==2) 且当前窗口
// 覆盖 FSINFO 扇区 (fs+0x18 窗口基址 <= fsinfo_sect(fs+0x20)) 时, 顺带把
// 窗口内容再写到 fsinfo 所在扇区 (0x200 窗口作为 512B FSINFO 副本)。
int fat_window_read(uint8_t *fs)
{
    if (disk_write(fs[1], fs + 0x40, *(uint32_t *)(fs + 0x2c), 1) != 0) {
        return 1;
    }
    fs[3] = 0;
    if ((uint32_t)(*(int *)(fs + 0x2c) - *(int *)(fs + 0x20)) < *(uint32_t *)(fs + 0x18) &&
        fs[2] == 2) {
        disk_write(fs[1], fs + 0x40,
                   *(uint32_t *)(fs + 0x2c) + *(uint32_t *)(fs + 0x18), 1);
    }
    return 0;
}

// === fat_fs_sync === 原 FUN_00002e20 @ 0x00002e20
// 来源: reversed/fsbl_funcs/FUN_00002e20.c | 分类: fsbl
// 语义: FatFs 卷同步。窗口脏 (fs[3]) 时先回写窗口; FAT32 (fs[0]==3) 且
// 挂载标志 (fs[4]==1) 时重建 FSINFO 扇区: 清零 0x1FF..0x240 段落, 写入
// 保留字段 (0x224='rr', 0x226="Aa", 0x227='a', 0x228..0x22f = free_clst(0x10)/
// next_free(0xc) 小端), 0x23E='U'/0x23F=0xAA (FSINFO 0x52/0x54 处 'U'aA A5.. 对应
// 'U' 0x55 0xAA 0x56), 置窗口 = fsinfo 扇区 +1 并回写, 清挂载标志。
// 最后 disk_ioctl(CTL_SYNC) 等待磁盘空闲; 失败返回 1。
// 注: 0x224..0x23F 偏移为窗口内偏移 (窗口基址 fs+0x40, 故字节偏移 = +0x40)。
int fat_fs_sync(uint8_t *fs)
{
    char type;
    if (fs[3] == 0) {
        type = fs[0];
    } else {
        uint32_t r = fat_window_read(fs);
        if (r != 0) {
            return r;
        }
        type = fs[0];
    }
    if (type == 3 && fs[4] == 1) {
        uint8_t *p;
        uint32_t free_clst = *(uint32_t *)(fs + 0x10);
        uint32_t next_free = *(uint32_t *)(fs + 0x0c);
        for (p = fs + 0x40; p != fs + 0x240; p++) {      /* 0x200..0x240 = 窗口尾 0x40 字节 */
            *p = 0;
        }
        /* FSINFO 结构 (窗口字节偏移 0x1C0..): 
         * +0x1E4(0x224) 'r' 'r'  | +0x1E6(0x226) 'A' 'a' 'A' (0x227='a')
         * +0x1E8(0x228) free_clst 小端 4B | +0x1EC(0x22C) next_free 小端 4B
         * +0x1FE(0x23E) 'U' 0x55/0xAA 尾标: 0x23E='U', 0x23F=0x56 (-0x56=0xAA 取反) */
        fs[0x228] = (uint8_t)free_clst;
        fs[0x23e] = 'U';
        fs[0x22c] = (uint8_t)next_free;
        fs[0x229] = (uint8_t)(free_clst >> 8);
        fs[0x22a] = (uint8_t)(free_clst >> 16);
        fs[0x22b] = (uint8_t)(free_clst >> 24);
        fs[0x22d] = (uint8_t)(next_free >> 8);
        fs[0x22e] = (uint8_t)(next_free >> 16);
        fs[0x23f] = (uint8_t)0xAA;
        fs[0x22f] = (uint8_t)(next_free >> 24);
        fs[0x224] = 'r';
        fs[0x225] = 'r';
        *(int *)(fs + 0x2c) = *(int *)(fs + 0x1c) + 1;   /* 窗口指向 FSINFO 所在扇区 */
        fs[0x40] = 'R';
        fs[0x41] = 'R';
        fs[0x42] = 'a';
        fs[0x227] = 'a';
        fs[0x43] = 'A';
        fs[0x226] = 'A';
        disk_write(fs[1], fs + 0x40, *(int *)(fs + 0x1c) + 1, 1);
        fs[4] = 0;
    }
    {
        int r = disk_ioctl(fs[1], 0, 0);                 /* CTRL_SYNC */
        return (uint32_t)(r != 0);
    }
}

// === fat_move_window === 原 FUN_00002f3c @ 0x00002f3c
// 来源: reversed/fsbl_funcs/FUN_00002f3c.c | 分类: fsbl
// 语义: 将 FS 窗口切换到目标扇区: 窗口脏 (fs[3]) 时先回写旧扇区,
// 失败置 *sect = 0xFFFFFFFF; 读入新扇区, 更新 fs+0x2c 窗口扇区号,
// 返回磁盘读取失败与否 (1 = 失败)。
uint32_t fat_move_window(uint8_t *fs, uint32_t sect)
{
    uint32_t r;
    if (fs[3] != 0) {
        r = fat_window_read(fs);
        if (r != 0) {
            return r;
        }
    }
    {
        int rd = disk_read(fs[1], fs + 0x40, sect, 1);
        if (rd != 0) {
            sect = 0xFFFFFFFFu;
        }
        *(uint32_t *)(fs + 0x2c) = sect;
        return (uint32_t)(rd != 0);
    }
}

// === fat_find_volume === 原 FUN_000030bc @ 0x000030bc
// 来源: reversed/fsbl_funcs/FUN_000030bc.c | 分类: fsbl
// 语义: FatFs 挂卷 (find_volume): 解析路径设备号前缀, 取/初始化卷对象,
// 校验 BPB (fat_check_fs; 首扇区非 FAT 引导记录时逐个复核 fs+0x202 起的
// 4 项候选备份引导扇区, 复核成功者的扇区号作为挂卷基扇区), 校验 BPB 几何
// (扇区 512, NumFATs∈{1,2}, SecPerClus 为 2 的幂, RootEntCnt 为 16 倍数且
// 按 FAT 型别非 0/必 0, RsvdSecCnt 非 0), 计算 FAT/根目录/数据区基扇区,
// 按簇数判型: >0xFFF5=FAT32, <=0xFF5=FAT12, 其余=FAT16; FAT32 时以
// 基扇区+1 装载 FSINFO 并校验 "RRaA"/"rrAa" 取空簇统计。
// 返回 0 = 成功 (*fs_out = 卷对象); 其余为 FR_ 错误码 (1/3/0xa/0xb/0xc/0xd)。
// 卷对象布局 (字节偏移): +0=fs_type, +1=pdrv, +2=n_fats, +3=w_flag(脏标志),
// +4=挂载标志(0x80, FSINFO 校验成功时清 0), +6=卷id, +8=n_rootdir
// (BPB_RootEntCnt 副本), +0xA=csize(BPB_SecPerClus 副本), +0xC=free_clst,
// +0x10=next_free, +0x14=n_fatent(簇数+2), +0x18=fsize(FATSz),
// +0x1C=挂卷基扇区(0 或备份引导扇区号), +0x20=fatbase, +0x24=dirbase,
// +0x28=database, +0x2C=窗口当前扇区号, +0x40..0x23F=扇区窗口 win
// (BPB 字段 = win+0xB..), +0x202..0x248=候选备份引导扇区号表
// (4 项 x 16B, 首字节非 0 时其 +4 起双字为扇区号)。
int fat_find_volume(const uint8_t **pp, uint8_t **fs_out, uint32_t mode)
{
    int      dev;                                /* 设备号 (反编译 31 行 FUN_00002b44) */
    uint32_t vol_base = 0;                       /* 挂卷基扇区 (反编译 iVar4): 80 行 =0,
                                                  * 67 行 =复核成功的备份引导扇区号 (反汇编 r7) */
    uint8_t *fs;
    uint32_t chk;                                /* 50 行首检结果 (uVar6) */
    uint32_t dstat = 0;                          /* 42 行 disk_status 结果 (uVar6 复用) */
    uint32_t tot_sec, n_fat, n_fats, csize, nr_dir, rsvd_sec;
    uint32_t fasize, bdata_off, nclst, n_fatent, fatbase_off, database_off, fat_bytes;
    uint8_t  fs_type;                            /* cVar9: 1=FAT12, 2=FAT16, 3=FAT32 */

    *fs_out = 0;
    dev = str_parse_devnum(pp, 0);   /* 原反编译未显式传第 2 参 (r1 残留 fs_out 指针,
                                      * 仅作判定初值且循环体内恒被覆写, 从未被读); 恒补 0 */
    if (dev < 0) {
        return 0xb;                                  /* 33 行 FR_INVALID_DRIVE */
    }
    fs = *(uint8_t **)(&DAT_00021544_base + dev * 4); /* 36 行 卷对象表 (按设备号) */
    if (fs == 0) {
        return 0xc;                                  /* 38 行 FR_NOT_ENABLED */
    }
    *fs_out = fs;                                    /* 41 行 */
    if (*fs == 0 || (dstat = disk_status(fs[1]), (dstat & 1) != 0)) {   /* 42 行 */
        uint32_t st;
        /* 卷未初始化或磁盘未就绪 → 重新初始化 (43-206 行) */
        *fs = 0;                                     /* 43 行 */
        fs[1] = (uint8_t)dev;                        /* 44 行 */
        st = disk_initialize(dev);                   /* 45 行 */
        if ((st & 1) != 0) {
            return 3;                                /* 210 行 FR_NOT_READY */
        }
        if (((mode & 0xfe) != 0) && ((st & 4) != 0)) {   /* 47 行 */
            return 10;                               /* 48 行 FR_WRITE_PROTECTED */
        }
        chk = fat_check_fs(fs, 0);                   /* 50 行 */
        if (chk == 2) {
            /* 52-63 行: 首扇区非 FAT 引导记录 → 读 4 项候选备份引导扇区号 */
            uint32_t backup[4];
            uint32_t last = 3;
            uint32_t i;
            uint8_t *q = fs + 0x208;
            for (i = 0; i < 4; i++, q += 0x10) {
                backup[i] = (q[-6] != 0)             /* 57 行 */
                    ? ((uint32_t)q[-2] | ((uint32_t)q[-1] << 8) |
                       ((uint32_t)*(uint16_t *)q << 16))    /* 58 行 */
                    : 0;
            }
            /* 65-73 行: 逐项复核; 首个返回 <2 者的扇区号即挂卷基扇区 */
            for (i = 0; i < 4; i++) {
                last = 3;                            /* 68 行: 每轮重置 (反汇编 0x334c) */
                if (backup[i] != 0) {
                    last = fat_check_fs(fs, backup[i]);  /* 70 行 */
                    if (last < 2) {
                        vol_base = backup[i];        /* 67 行 (反汇编 0x3348: r7 = 备份扇区号) */
                        goto geom;                   /* 71 行 → LAB_00003194 */
                    }
                }
            }
            if (last != 4) {                         /* 74 行 */
                return 0xd;
            }
            return 1;                                /* 207 行 uVar5 = 1 (反汇编 0x3398) */
        }
        if (chk < 2) {
            vol_base = 0;                            /* 80 行: 首扇区即有效, 基扇区 0 */
        } else if (chk == 4) {
            return 1;                                /* 207 行 (反汇编 0x3184 beq 0x3398) */
        } else {
            return 0xd;                              /* 205 行 (chk == 3, 反汇编 0x3190 bhi) */
        }
    geom:
        if (*(uint16_t *)(fs + 0x4b) != 0x200) {     /* 83 行: BPB_BytsPerSec (win+0x0B) 必须 512 */
            return 0xd;
        }
        n_fat = *(uint16_t *)(fs + 0x56);            /* 84 行: BPB_FATSz16 (win+0x16) */
        if (n_fat == 0) {
            n_fat = *(uint32_t *)(fs + 100);         /* 86 行: BPB_FATSz32 (win+0x24) */
        }
        n_fats = fs[0x50];                           /* 88 行: BPB_NumFATs */
        *(uint32_t *)(fs + 0x18) = n_fat;            /* 89 行: fsize */
        fs[2] = (uint8_t)n_fats;                     /* 90 行 */
        if (n_fats - 1 < 2) {                        /* 91 行: NumFATs ∈ {1,2} */
            csize = fs[0x4d];                        /* 92 行: BPB_SecPerClus */
            *(uint16_t *)(fs + 10) = (uint16_t)csize;    /* 93 行 */
            if (csize != 0 && ((csize - 1) & csize) == 0) {  /* 94 行: 2 的幂 */
                nr_dir = *(uint16_t *)(fs + 0x51);   /* 95 行: BPB_RootEntCnt (win+0x11;
                                                     * 反汇编 0x3200 ldrb [r4,#0x51..52]) */
                *(uint16_t *)(fs + 8) = (uint16_t)nr_dir;    /* 96 行 */
                if ((nr_dir & 0xf) == 0) {           /* 97 行: 须为 16 的倍数 */
                    /* 98-108 行: 总扇区数: BPB_TotSec16 (win+0x13), 为 0 时取
                     * BPB_TotSec32 (win+0x20..0x23) (反汇编 0x3218-0x3238) */
                    tot_sec = *(uint16_t *)(fs + 0x53);
                    if (tot_sec == 0) {
                        tot_sec = (uint32_t)fs[0x60] |
                                  ((uint32_t)fs[0x61] |
                                   (uint32_t)*(uint16_t *)(fs + 0x62) << 8) << 8;
                    }
                    /* 109 行: BPB_RsvdSecCnt (win+0x0E..0x0F; 反编译 uVar18) */
                    rsvd_sec = (uint32_t)fs[0x4e] | ((uint32_t)fs[0x4f] << 8);
                    if (rsvd_sec != 0) {             /* 110 行 (无 else: 为 0 → 205 行 return 0xd) */
                        fasize = n_fat * n_fats;     /* 111 行: FAT 区总扇区数 */
                        /* 112 行: 数据区相对偏移 (根目录扇区数 = RootEntCnt>>4) */
                        bdata_off = rsvd_sec + (nr_dir >> 4) + fasize;
                        if (bdata_off <= tot_sec &&
                            (nclst = aeabi_uidiv(tot_sec - bdata_off, csize),
                             /* 114 行: 簇数 = 数据区扇区数 / SecPerClus; 除数 r1 = csize
                              * (反汇编 0x31e0 ldrb r1,[r4,#0x4d] 存活至 0x3260 bl 0x1a50) */
                             (uint32_t)(nclst - 1) < 0xffffff4u || nclst == 0xffffff5u)) {
                            /* 115 行: 簇数须 ∈ [1, 0x0FFFFFF5] 且非 0 (反汇编 0x3268 cmn) */
                            fatbase_off = rsvd_sec + vol_base;   /* 116 行: FAT 区基扇区 */
                            database_off = bdata_off + vol_base; /* 117 行: 数据区基扇区 */
                            n_fatent = nclst + 2;    /* 118 行: FAT 表项数 (含 #0/#1, pcVar11) */
                            if (0xfff5u < nclst) {   /* 119 行: FAT32 (簇数 > 0xFFF5) */
                                *(uint32_t *)(fs + 0x20) = fatbase_off;  /* 120 行 */
                                *(uint32_t *)(fs + 0x14) = n_fatent;     /* 121 行 */
                                *(uint32_t *)(fs + 0x1c) = vol_base;     /* 122 行 */
                                *(uint32_t *)(fs + 0x28) = database_off; /* 123 行 */
                                if (nr_dir != 0 || *(uint16_t *)(fs + 0x6a) != 0) {
                                    /* 124 行: FAT32 要求 RootEntCnt 与 FSVer (win+0x2A) 均为 0 */
                                    return 0xd;
                                }
                                *(uint32_t *)(fs + 0x24) = *(uint32_t *)(fs + 0x6c);
                                /* 127 行: dirbase = BPB_RootClus (win+0x2C) */
                                if (n_fat < (n_fatent * 4 + 0x1ff) >> 9) {
                                    /* 128 行: FATSz 须容纳 FAT32 表 (n_fatent 项 x 4 字节;
                                     * 反汇编 0x33fc add r2,r3,r2,lsl #2 用 nclst+2) */
                                    return 0xd;
                                }
                                fs[0x10] = fs[0x11] = fs[0x12] = fs[0x13] = 0xFF;  /* 131-134 行 */
                                fs[0x0c] = fs[0x0d] = fs[0x0e] = fs[0x0f] = 0xFF;  /* 135-138 行 */
                                fs[4] = 0x80;                /* 139 行 */
                                /* 140-154 行: FSINFO 校验/装载 (FSINFO 扇区 = 基扇区+1,
                                 * 反汇编 0x344c add r1,r7,#1 / 0x345c bl 0x2f3c) */
                                if (*(uint16_t *)(fs + 0x70) == 1 &&
                                    ((vol_base + 1 == *(uint32_t *)(fs + 0x2c) ||
                                      fat_move_window(fs, vol_base + 1) == 0) &&
                                     (fs[4] = 0, *(uint16_t *)(fs + 0x23e) == 0xAA55)) &&
                                    *(uint32_t *)(fs + 0x40) == 0x41615252 &&
                                    *(uint32_t *)(fs + 0x224) == 0x61417272) {
                                    *(uint32_t *)(fs + 0x10) = *(uint32_t *)(fs + 0x228);
                                    /* 147 行: next_free = FSINFO +0x1E8 */
                                    *(uint32_t *)(fs + 0x0c) =
                                        (uint32_t)fs[0x22c] | ((uint32_t)fs[0x22d] << 8) |
                                        ((uint32_t)*(uint16_t *)(fs + 0x22e) << 16);
                                    /* 148-150 行: free_clst = FSINFO +0x1EC */
                                }
                                fs_type = 3;                 /* 146/153 行 */
                            } else {
                                if (nclst < 0xff6u) {        /* 157 行: FAT12 (簇数 <= 0xFF5) */
                                    *(uint32_t *)(fs + 0x14) = n_fatent;     /* 158 行 */
                                    *(uint32_t *)(fs + 0x1c) = vol_base;     /* 159 行 */
                                    *(uint32_t *)(fs + 0x20) = fatbase_off;  /* 160 行 */
                                    *(uint32_t *)(fs + 0x28) = database_off; /* 161 行 */
                                    if (nr_dir == 0) {       /* 162 行: RootEntCnt 须非 0 */
                                        return 0xd;
                                    }
                                    *(uint32_t *)(fs + 0x24) = fasize + fatbase_off;
                                    /* 165 行: dirbase (先写) */
                                    fat_bytes = (n_fatent & 1) + ((n_fatent * 3) >> 1);
                                    /* 166 行: FAT12 表字节数 (后算) */
                                    fs_type = 1;             /* 167 行 */
                                } else {                     /* 169-180 行: FAT16 */
                                    *(uint32_t *)(fs + 0x14) = n_fatent;     /* 170 行 */
                                    *(uint32_t *)(fs + 0x1c) = vol_base;     /* 171 行 */
                                    *(uint32_t *)(fs + 0x20) = fatbase_off;  /* 172 行 */
                                    *(uint32_t *)(fs + 0x28) = database_off; /* 173 行 */
                                    if (nr_dir == 0) {       /* 174 行 */
                                        return 0xd;
                                    }
                                    fat_bytes = n_fatent * 2;    /* 177 行: FAT16 表字节数 (先算) */
                                    *(uint32_t *)(fs + 0x24) = fasize + fatbase_off;
                                    /* 178 行: dirbase (后写) */
                                    fs_type = 2;             /* 179 行 */
                                }
                                if (n_fat < (fat_bytes + 0x1ff) >> 9) {
                                    /* 181 行: FATSz 容量校验 (FAT12/16 共用) */
                                    return 0xd;
                                }
                                fs[0x10] = fs[0x11] = fs[0x12] = fs[0x13] = 0xFF;  /* 184-187 行 */
                                fs[0x0c] = fs[0x0d] = fs[0x0e] = fs[0x0f] = 0xFF;  /* 188-191 行 */
                                fs[4] = 0x80;                /* 192 行 */
                            }
                            *fs = fs_type;                   /* 194 行 */
                            /* 195-196 行: 卷 ID 计数器递增并回写 (16 位回绕,
                             * 反汇编 0x32e8 ldrh / 0x32f4 uxth / 0x32fc strh) */
                            g_fatfs_vol_id = (uint16_t)(g_fatfs_vol_id + 1);
                            *(uint16_t *)(fs + 6) = (uint16_t)g_fatfs_vol_id;
                            return 0;
                        }
                    }
                }
            }
        }
        return 0xd;      /* 205 行: 任一几何校验失败 */
    } else if ((mode & 0xfe) == 0) {
        return 0;        /* 214 行: 卷已就绪且无写请求 */
    } else if ((dstat & 4) != 0) {   /* 218 行 (复用 42 行 disk_status 结果) */
        return 10;
    }
    return 0;            /* 217/224 行 */
}

/* FatFs 全局: 卷对象表 / 卷 ID 计数器 (反编译 DAT_00021544 / DAT_0002154c) */
static uint8_t *const DAT_00021544_base = (uint8_t *)0x00021544; /* 全局, 语义待定 */
static uint32_t g_fatfs_vol_id;                          /* 原名 DAT_0002154c (定义见文末) */

// === fat_dir_register === 原 FUN_00003e38 @ 0x00003e38
// 来源: reversed/fsbl_funcs/FUN_00003e38.c | 分类: fsbl
// 语义: 在打开的目录 (dj) 中注册新目录项: 沿目录链扫描 (fat_dir_sdi +1),
// 找到首字符为 0x00 (从未用) 或 0xE5 (已删除) 的槽位时, 将该 32 字节槽位清零,
// 写入参数字段: dj[8]/dj[9] = 首簇 (2 字, 小端), dj[10] 低字节 = 属性到槽位 +0x0B,
// dj+0x29/0x2A (NT 保留字节) 到槽位 +9/+10, 置 FS 脏标志 dj_fs[3] = 1, 返回 0。
// 目录满 (fat_dir_sdi 返回 4) 时错误码转换为 7 (FR_DENIED)。
int fat_dir_register(int *dj)
{
    uint8_t *fs = *(uint8_t **)dj;
    int r = fat_dir_sdi0(dj);
    if (r == 0) {
        do {
            if (dj[6] != *(int *)(fs + 0x2c)) {          /* 窗口未在目标扇区 */
                r = fat_move_window(fs, dj[6]);
                if (r != 0) break;
            }
            {
                uint8_t *slot = *(uint8_t **)&dj[7];
                if (*slot == 0 || *slot == 0xE5) {
                    if (dj[6] != *(int *)(fs + 0x2c)) {  /* Ghotra: 原文调用缺参, 转写按完整签名 */
                        r = fat_move_window(fs, dj[6]);
                        if (r != 0) {
                            return r;
                        }
                        slot = *(uint8_t **)&dj[7];
                    }
                    {
                        uint8_t *p;
                        for (p = slot; p != slot + 0x20; p++) {
                            *p = 0;                      /* 32 字节槽位清零 */
                        }
                        {
                            uint32_t *w = (uint32_t *)slot;
                            w[0] = dj[8];                /* 首簇低字 */
                            w[1] = dj[9];                /* 首簇高字 (FAT32) */
                            slot[8] = (uint8_t)dj[10];   /* 属性 */
                            slot[9]  = *(uint8_t *)((uintptr_t)dj + 0x29);  /* NT 保留 */
                            slot[10] = *(uint8_t *)((uintptr_t)dj + 0x2a);
                            fs[3] = 1;                   /* FS 脏标志 */
                            return 0;
                        }
                    }
                }
            }
            r = fat_dir_sdi(dj, 1);                      /* 下一个槽位 */
        } while (r == 0);
    }
    if (r == 4) {
        r = 7;                                           /* 目录满 → FR_DENIED */
    }
    return r;
}

/* ==================== FatFs 文件层 ==================== */

// === fat_f_open === 原 FUN_000042d4 @ 0x000042d4
// 来源: reversed/fsbl_funcs/FUN_000042d4.c | 分类: fsbl
// 语义: FatFs f_open 主体。流程: 解析设备号 → fat_find_volume (按 mode 决定写保护检查)
// → fat_follow_path 定位文件 → 现有文件 (非 FA_CREATE_ALWAYS/新路径) 时校验属性
// (目录/只读/存在性) → FA_CREATE_NEW/ALWAYS 时经 fat_dir_register 建目录项,
// FA_CREATE_ALWAYS 时清空簇链 (fat_remove_chain) 并更新日期 → 填充 FIL 对象
// (fp+0x10..0x240 数据窗口清零) → FA__WRITTEN (mode&0x40) 且非零长度时预卷到文件尾。
// 返回 FR_ 错误码 (0 = 成功, 9/6/4/8/7/1/2 等)。
// FIL 布局 (字节偏移): +0x00=fs 指针, +0x04=标志, +0x08..=目录项扇区, +0x0C=dir_ptr?
// +0x11=err, +0x14=fsize, +0x18=fdate/ftime…, +0x40..=窗口
int fat_f_open(uint32_t *fp, const uint8_t *path, uint8_t mode)
{
    const uint8_t *path_m = path;
    uint8_t djbuf[0x2c];                                 /* DIR 对象 (local_4c 起) */
    uint8_t *fs;
    uint8_t  dj_attr;                                    /* local_46 = djbuf+0x0A 处属性 (实际 +0x2B 域) */
    uint32_t dj_sclust;                                  /* local_30 */
    char     dj_lfnflag;                                 /* local_21 = djbuf+0x2B 域 */
    uint8_t  fa;                                         /* bVar11 = mode & 0x3F */
    int      r;

    mem_set(djbuf, 0, 0x2c);
    if (fp == 0) {
        return 9;                                        /* FR_INVALID_OBJECT */
    }
    fa = mode & 0x3f;
    r = fat_find_volume(&path_m, &fs, fa);
    if (r != 0) {
        *fp = 0;
        return r;
    }
    {
        /* DIR 对象在栈上: djbuf[0]=fs 指针, 其余 0x28 字节由 follow_path 填充 */
        uint32_t dj[11];                                 /* local_4c..local_30 语义槽位 */
        dj[0] = (uint32_t)(uintptr_t)fs;
        r = fat_follow_path((int *)dj, path_m);
        if (r == 0) {
            dj_attr    = ((uint8_t *)dj)[0x0A + 0];      /* local_46 (djbuf+0x0A) */
            dj_sclust  = dj[7];                          /* local_30 = 目录项所在首簇/扇区 */
            dj_lfnflag = ((char *)dj)[0x0A + 1];         /* local_21 */
            if (dj_lfnflag < 0) {                        /* LFN 长名条目 (属性 & 0x80) */
                *fp = 0;
                return 6;                                /* FR_INVALID_NAME */
            }
            if ((mode & 0x1c) == 0) {
                /* 打开既有文件路径 (无创建模式) */
                if ((dj_attr & 0x10) != 0) {             /* AM_DIR */
                    *fp = 0;
                    return 4;                            /* FR_NO_FILE */
                }
                if ((mode & 2) == 0 || (dj_attr & 1) == 0) { /* 非 FA_WRITE 或非只读 */
                    uint32_t win_sect = *(uint32_t *)(fs + 0x2c);
                    if ((mode & 8) == 0) {               /* 无 FA_CREATE_ALWAYS */
                        goto fill_fp;                    /* 直接填充 FIL */
                    }
                    goto truncate_always;                /* 走 FA_CREATE_ALWAYS 分支 (置 FA__WRITTEN) */
                }
                *fp = 0;
                return 7;                                /* FA_WRITE 对只读文件 → FR_DENIED */
            } else if ((dj_attr & 0x11) == 0) {
                /* 文件存在且不是目录/只读 */
                if ((mode & 4) != 0) {                   /* FA_CREATE_NEW */
                    *fp = 0;
                    return 8;                            /* FR_EXIST */
                }
                if ((mode & 8) != 0) {                   /* FA_CREATE_ALWAYS: 走建项清空 */
                    goto create_always;
                }
                {
                    uint32_t win_sect = *(uint32_t *)(fs + 0x2c);
                    goto fill_fp;
                }
            }
            *fp = 0;
            return 7;
        }
        /* 路径不存在: 仅创建类模式 (0x1c != 0) 且错误码 4 (FR_NO_FILE) 才建新文件 */
        if (((mode & 0x1c) == 0) || (r != 4)) {
            *fp = 0;
            return r;
        }
        fa |= 8;                                         /* AM_ARC 属性 */
        r = fat_dir_register((int *)dj);
        if (r != 0) {
            *fp = 0;
            return r;
        }
    create_always:
        {
            /* 新建/覆盖: 初始化目录项属性/日期, 清空旧簇链 */
            uint32_t fst = dj[0];                        /* fs 类型: 3 = FAT32 */
            uint32_t cl_hi = 0, cl_lo;
            uint32_t date;
            uint8_t *slot = (uint8_t *)dj_sclust;
            uint32_t win_sect;
            cl_lo = *(uint16_t *)(slot + 0x1a);
            if (fst == 3) {
                cl_hi = *(uint8_t *)(slot + 0x14) | 0;   /* 首簇高 16 位低字节 (原代码取 0x15/0x14 组合) */
                cl_hi = *(uint8_t *)(slot + 0x15);
                cl_lo = *(uint8_t *)(slot + 0x14);       /* 占位: 见下重组 */
            }
            /* uVar8 = 0x1A..0x1B 16 位 | (FAT32: 0x14/0x15 高 16 位) = 首簇 32 位 */
            cl_lo = *(uint16_t *)(slot + 0x1a);
            cl_hi = 0;
            if (fst == 3) {
                cl_hi = (uint32_t)*(uint8_t *)(slot + 0x14) | ((uint32_t)*(uint8_t *)(slot + 0x15) << 8);
            }
            date = fat_default_date();                   /* 0x3c210000 */
            slot[0x0e] = (uint8_t)date;                  /* CrtTime */
            slot[0x0f] = (uint8_t)(date >> 8);
            slot[0x10] = (uint8_t)(date >> 16);
            slot[0x11] = (uint8_t)(date >> 24);
            slot[0x0b] = 0x20;                           /* AM_ARC */
            slot[0x1a] = 0;                              /* 首簇低 16 位清零 */
            slot[0x1b] = 0;
            if (fst == 3) {
                slot[0x14] = 0;                          /* 首簇高 16 位清零 */
                slot[0x15] = 0;
            }
            slot[0x1c] = 0;                              /* WrtDate */
            slot[0x1d] = 0;
            slot[0x1e] = 0;
            slot[0x1f] = 0;
            fs[3] = 1;                                   /* FS 脏标志 */
            win_sect = *(uint32_t *)(fs + 0x2c);
            if (cl_lo | cl_hi) {
                /* 有旧簇链: 删除并回写目录项窗口 */
                uint32_t old_sect = win_sect;
                r = fat_remove_chain((int *)dj, cl_lo | (cl_hi << 0), 0);
                if (r != 0) {
                    *fp = 0;
                    return r;
                }
                if (old_sect != *(uint32_t *)(fs + 0x2c)) {
                    r = fat_move_window(fs, old_sect);
                    *(uint32_t *)(fs + 0x0c) = (cl_lo | (cl_hi << 16)) - 1; /* 空闲簇数减 1 (粗计数) */
                    if (r != 0) {
                        *fp = 0;
                        return r;
                    }
                    win_sect = *(uint32_t *)(fs + 0x2c);
                } else {
                    *(uint32_t *)(fs + 0x0c) = (cl_lo | (cl_hi << 16)) - 1;
                }
            }
        }
    truncate_always:
        fa |= 0x40;                                  /* FA__WRITTEN — 原 LAB_000044d8, create_always 落穿汇合 */
    fill_fp:
        {
            /* FIL 填充: dj[7]=dir slot 指针, dj[6]=目录项扇区 */
            uint8_t *slot = *(uint8_t **)&dj[7];
            uint32_t fst2 = dj[0];
            uint32_t scl = 0;
            fp[8] = *(uint32_t *)(fs + 0x2c);            /* dir_sect = 窗口扇区 */
            fp[9] = (uint32_t)(uintptr_t)slot;
            scl = *(uint16_t *)(slot + 0x1a);
            if (fst2 == 3) {
                scl = (uint32_t)*(uint8_t *)(slot + 0x14) | ((uint32_t)*(uint8_t *)(slot + 0x15) << 8) |
                      (uint32_t)*(uint16_t *)(slot + 0x1a);
            }
            fp[2] = scl;                                 /* sclust */
            fp[3] = *(uint32_t *)(slot + 0x1c);          /* fsize (0x1C..0x1F) */
            fp[0] = (uint32_t)(uintptr_t)fs;
            *(uint16_t *)(fp + 1) = *(uint16_t *)(fs + 6); /* 卷 id */
            ((uint8_t *)fp)[0x10] = fa;                  /* fp+4? — 反编译 *(byte*)(fp+4) = fa (字节域) */
            fp[3] = *(uint32_t *)(slot + 0x1c);
            ((uint8_t *)fp)[0x11] = 0;                   /* err = 0 */
            fp[7] = 0;                                   /* dir_ptr? */
            fp[5] = 0;                                   /* fptr = 0 */
            {
                uint8_t *p;
                uint8_t *end;
                for (p = (uint8_t *)(fp + 0x10), end = (uint8_t *)(fp + 0x90); p != end; p++) {
                    *p = 0;                              /* 数据窗口 0x240 字节清零 */
                }
            }
            if ((fa & 0x20) == 0) {                      /* 非 FA__WRITTEN */
                return 0;
            }
            {
                /* 预写卷动: 从首簇沿 FAT 卷到文件尾, 记录最后簇/扇区 */
                uint32_t fsize = fp[3];
                if (fsize == 0) {
                    return 0;
                }
                {
                    uint16_t csize = *(uint16_t *)(fs + 10);
                    uint32_t clst = fp[2];
                    uint32_t fptr = fsize;
                    uint32_t clst_bytes = (uint32_t)csize * 0x200;
                    fp[5] = fsize;
                    if (clst_bytes < fsize) {
                        do {
                            fptr -= clst_bytes;
                            clst = fat_get_fat(fs, clst);
                            if (clst < 2) {
                                fp[6] = clst;
                                *fp = 0;
                                return 2;                /* FR_INT_ERR */
                            }
                        } while (clst != 0xFFFFFFFFu && fptr > clst_bytes);
                        fp[6] = clst;
                        if (clst == 0xFFFFFFFFu) {
                            *fp = 0;
                            return 1;                    /* FR_DISK_ERR */
                        }
                    } else {
                        fp[6] = clst;
                    }
                    if ((fptr & 0x1ff) == 0) {           /* 整扇区对齐 */
                        return 0;
                    }
                    if (clst - 2 < *(uint32_t *)(fs + 0x14) - 2) {
                        uint32_t sect = (uint32_t)csize * (clst - 2) + *(int *)(fs + 0x28);
                        if (sect != 0) {
                            sect += (fptr >> 9);
                            fp[7] = sect;
                            r = disk_read(fs[1], (uint32_t *)(fp + 0x10), sect, 1);
                            if (r == 0) {
                                return 0;
                            }
                            r = 1;
                        } else {
                            r = 2;
                        }
                    } else {
                        r = 2;
                    }
                }
            }
        }
        *fp = 0;
        return r;
    }
}

// === fat_f_sync === 原 FUN_00004c1c @ 0x00004c1c
// 来源: reversed/fsbl_funcs/FUN_00004c1c.c | 分类: fsbl
// 语义: FatFs f_sync 主体: 校验卷对象 (fatfs_validate) 且 FA__WRITTEN (flag&0x40);
// FA__DIRTY (flag&0x80) 时回写文件数据窗口到 dir_ptr 指向扇区, 清脏位;
// 更新目录项 (槽位 +0x0B |= AM_ARC, +0x14.. = fsize 小端 (FAT32 写 4 字节),
// +0x16..0x1F = WrtTime/WrtDate/LstAccDate/首簇/大小, +0x12/0x13 = NT 保留清零),
// 置 FS 脏标志, fat_fs_sync 回写卷元数据, 最后清 FA__WRITTEN 位。
void fat_f_sync(uint32_t *fp)
{
    uint32_t fs_v;
    uint32_t date;
    uint8_t *slot;
    uint8_t  flags;
    int r = fatfs_validate(fp, &fs_v);
    if (r == 0 && ((((uint8_t *)fp)[0x10] & 0x40) != 0)) {
        flags = ((uint8_t *)fp)[0x10];
        if ((flags & 0x80) != 0) {
            if (disk_write(*(uint8_t *)(fs_v + 1), (uint32_t *)(fp + 0x10),
                           fp[7], 1) != 0) {             /* 回写 dir_ptr 所在扇区 */
                return;
            }
            ((uint8_t *)fp)[0x10] = flags & 0x7f;
        }
        date = fat_default_date();
        if (fp[8] != *(uint32_t *)(fs_v + 0x2c)) {       /* 目录项窗口扇区已换出 */
            if (fat_move_window((uint8_t *)fs_v, fp[8]) != 0) {
                return;
            }
        }
        slot = *(uint8_t **)&fp[9];
        slot[0x0b] |= 0x20;                              /* AM_ARC */
        {
            uint32_t fsize = fp[2];                      /* 反编译取 fp[2]/fp[3] 为 64 位大小低/高? 按 32 位实际 */
            uint32_t fsize_hi = fp[3];
            uint8_t *fs_t = *(uint8_t **)fp;
            slot[0x1a] = (uint8_t)fsize;
            slot[0x1b] = (uint8_t)(fsize >> 8);
            if (*fs_t == 3) {
                slot[0x14] = (uint8_t)(fsize >> 16);
                slot[0x15] = (uint8_t)(fsize >> 24);
            }
            slot[0x17] = (uint8_t)(date >> 8);
            slot[0x16] = (uint8_t)date;
            slot[0x1c] = (uint8_t)fsize_hi;
            slot[0x18] = (uint8_t)(date >> 16);
            slot[0x1d] = (uint8_t)(fsize_hi >> 8);
            slot[0x1e] = (uint8_t)(fsize_hi >> 16);
            slot[0x1f] = (uint8_t)(fsize_hi >> 24);
            slot[0x19] = (uint8_t)(date >> 24);
            slot[0x12] = 0;
            slot[0x13] = 0;
        }
        *(uint8_t *)(fs_v + 3) = 1;
        fat_fs_sync((uint8_t *)fs_v);
        ((uint8_t *)fp)[0x10] &= 0xbf;                   /* 清 FA__WRITTEN */
    }
}

// === fat_f_close === 原 FUN_00004d5c @ 0x00004d5c
// 来源: reversed/fsbl_funcs/FUN_00004d5c.c | 分类: fsbl
// 语义: FatFs f_close: 先 f_sync 回写, 若两者都成功则 FIL 对象清零 (fp[0]=0)。
void fat_f_close(uint32_t *fp)
{
    uint32_t fs_v;
    /* 原体 void (Ghidra return;), 调用点以 r0 残留值 ==0 判定 — 经 int 原型函数指针调用读取 */
    if (((int (*)(uint32_t *))fat_f_sync)(fp) == 0 && fatfs_validate(fp, &fs_v) == 0) {
        fp[0] = 0;
    }
}

// === fat_f_stat === 原 FUN_00005194 @ 0x00005194
// 来源: reversed/fsbl_funcs/FUN_00005194.c | 分类: fsbl
// 语义: FatFs f_stat: 挂卷 + fat_follow_path 定位路径, 成功后:
// 目录项属性 (FILINFO+0x2B, bit7=卷标域) 为负时返回 6 (FR_INVALID_NAME);
// 否则若 filinfo 非空: filinfo[9] (fname 首字节) 清零后由 fat_get_fileinfo
// 从目录项槽位拷贝 11 字节 8.3 名 (0x05→0xE5, 第 9 字节插 '.') 与属性/大小/日期。
// FILINFO 布局: +0x09..0x13 = fname 11 字节, +0x18 = fsize, +0x2B = fattr。
int fat_f_stat(const uint8_t *path, uint32_t *finfo)
{
    const uint8_t *p = path;
    uint8_t djbuf[0x2c];
    uint8_t *fs;
    int r;
    mem_set(djbuf, 0, 0x2c);
    r = fat_find_volume(&p, &fs, 0);
    if (r == 0) {
        r = fat_follow_path((int *)djbuf, p);
        if (r == 0) {
            if ((char)djbuf[0x2b] < 0) {                 /* LFN/卷标条目 */
                return 6;
            }
            if (finfo != 0) {
                *(uint8_t *)((uintptr_t)finfo + 9) = 0;
                if (*(int32_t *)(djbuf + 0x18) != 0) {   /* local_24 = djbuf+0x18 = 目录项指针域非 0 */
                    fat_get_fileinfo(djbuf + 0x20, finfo);
                }
            }
        }
    }
    return r;
}

// === fat_f_getfree === 原 FUN_00005224 @ 0x00005224
// 来源: reversed/fsbl_funcs/FUN_00005224.c | 分类: fsbl
// 语义: FatFs f_getfree: 挂卷 (写保护模式 0) 后统计空闲簇数。
// FSINFO free_clst (fs+0x10) 在 [2, n_fatent) 且 FAT32 时重新统计:
// 若 next_free(fs+0x0C)==0 (无缓存) → 逐 FAT 表项扫描 (fat_get_fat, 0 计空闲);
// 否则从窗口 FAT 扇区直接扫描 (FAT16: 2 字节/项 窗口偏移 0x40+ofs;
// FAT32: 4 字节/项 & 0x0FFFFFFF)。结果回写 *nclst / fs+0x10 / 置 fs[4] bit0 (缓存有效)。
// 错误: 1 = FR_DISK_ERR, 2 = FR_INT_ERR, 0 = 成功。
int fat_f_getfree(const uint8_t *path, uint32_t *nclst, uint32_t **fat)
{
    const uint8_t *p = path;
    uint8_t *fs;
    int r = fat_find_volume(&p, &fs, 0);
    if (r == 0) {
        uint32_t n = *(uint32_t *)(fs + 0x14);           /* n_fatent */
        uint32_t free_clst = *(uint32_t *)(fs + 0x10);
        *fat = (uint32_t *)(uintptr_t)fs;
        if (n - 2 < free_clst) {                         /* FSINFO 缓存可疑/缺 → 重算 */
            uint32_t clst;
            uint32_t cnt = 0;
            if (*fs == 1) {
                /* FAT12: 逐表项 (fat_get_fat 自带窗口管理) */
                clst = 2;
                do {
                    uint32_t v = fat_get_fat(fs, clst);
                    clst++;
                    if (v == 0xFFFFFFFFu) { r = 1; break; }
                    if (v == 1) { r = 2; break; }
                    if (v == 0) cnt++;
                } while (clst < n);
            } else {
                /* FAT16/32: 窗口内直接扫描 */
                uint32_t ofs = 0;
                uint32_t remain = n;
                uint32_t fat_base = *(int *)(fs + 0x20);
                for (;;) {
                    /* FAT0 窗口: 需要时切窗 (ofs==0 时比对窗口扇区) */
                    if (ofs == 0) {
                        uint32_t want = fat_base + 1;
                        if (fat_base != *(int *)(fs + 0x2c)) {
                            uint32_t mv = fat_move_window(fs, fat_base);
                            if (mv != 0) { r = mv; break; }
                        }
                        remain_check: (void)0;
                    }
                    if (*fs == 2) {                      /* FAT16 */
                        uint16_t v = *(uint16_t *)(fs + 0x40 + ofs);
                        ofs = (ofs + 2) & 0x1ff;
                        if (v == 0) cnt++;
                        remain--;
                        if (remain == 0) break;
                        continue;
                    }
                    /* FAT32 */
                    {
                        uint32_t v = *(uint32_t *)(fs + 0x40 + ofs);
                        ofs = (ofs + 4) & 0x1ff;
                        if ((v & 0x0FFFFFFF) == 0) cnt++;
                        remain--;
                    }
                }
            }
            *nclst = cnt;
            *(uint32_t *)(fs + 0x10) = cnt;
            fs[4] |= 1;                                  /* 空闲计数缓存有效 */
        } else {
            *nclst = free_clst;
        }
    }
    return r;
}

// === fat_f_truncate === 原 FUN_000053a8 @ 0x000053a8
// 来源: reversed/fsbl_funcs/FUN_000053a8.c | 分类: fsbl
// 语义: FatFs f_truncate: 校验 FIL (fatfs_validate), 已有错误 (fp+0x11) 直接返回;
// 非 FA__WRITTEN (flag&2 == 0) 返回 7; fptr (fp[5]) >= fsize (fp[3]) 已到位返回 0。
// fsize==0 → fat_remove_chain(clst=fp[2], 0) 释放整链; 否则从 fp[6] (当前簇) 起
// 释放后续簇链并更新目录项。最后置 FA__DIRTY|FA__WRITTEN (flag|=0x40), fp[3]=fptr,
// 回写目录项窗口 (fp[7] 扇区) 后清 FA__DIRTY, 错误码记入 fp+0x11。
uint32_t fat_f_truncate(uint32_t *fp)
{
    uint32_t fs_v;
    uint8_t flags;
    uint32_t err;
    uint32_t r = fatfs_validate(fp, &fs_v);
    if (r != 0) {
        return r;
    }
    if (*(uint8_t *)((uintptr_t)fp + 0x11) != 0) {
        return *(uint8_t *)((uintptr_t)fp + 0x11);
    }
    if ((((uint8_t *)fp)[0x10] & 2) == 0) {
        return 7;                                        /* FR_INVALID_OBJECT (非写打开) */
    }
    if ((uint32_t)fp[3] <= (uint32_t)fp[5]) {
        return 0;                                        /* fsize <= fptr */
    }
    if (fp[5] == 0) {
        r = fat_remove_chain((int *)fp, fp[2], 0);
        fp[2] = 0;
        flags = ((uint8_t *)fp)[0x10] | 0x40;
        ((uint8_t *)fp)[0x10] = flags;
        fp[3] = fp[5];
        if (r != 0) {
            goto set_err;
        }
        goto write_dir;
    } else {
        r = fat_get_fat((uint8_t *)fs_v, fp[6]);
        if (r == 0xFFFFFFFFu) {
            err = 1;                                     /* FR_DISK_ERR */
        } else {
            if (r != 1) {
                if (r < *(uint32_t *)(fs_v + 0x14)) {
                    r = fat_remove_chain((int *)fp, r, fp[6]);
                    flags = ((uint8_t *)fp)[0x10] | 0x40;
                    ((uint8_t *)fp)[0x10] = flags;
                    fp[3] = fp[5];
                    if (r != 0) goto set_err;
                    goto write_dir;
                }
                flags = ((uint8_t *)fp)[0x10] | 0x40;    /* 已在链尾 */
                ((uint8_t *)fp)[0x10] = flags;
                fp[3] = fp[5];
                goto write_dir;
            }
            err = 2;                                     /* FR_INT_ERR (链断裂) */
        }
        flags = ((uint8_t *)fp)[0x10] | 0x40;
        ((uint8_t *)fp)[0x10] = flags;
        fp[3] = fp[5];
    }
    goto set_err_v;
set_err:
    err = r;
set_err_v:
    *(uint8_t *)((uintptr_t)fp + 0x11) = (uint8_t)err;
    return err;
write_dir:
    /* FA__DIRTY: 回写目录项所在扇区 */
    if ((flags & 0x80) == 0) {
        return 0;
    }
    if (disk_write(*(uint8_t *)(fs_v + 1), (uint32_t *)(fp + 0x10), fp[7], 1) == 0) {
        ((uint8_t *)fp)[0x10] &= 0x7f;
        return 0;
    }
    err = 1;
    *(uint8_t *)((uintptr_t)fp + 0x11) = (uint8_t)err;
    return err;
}

// === fat_f_rename === 原 FUN_00005824 @ 0x00005824
// 来源: reversed/fsbl_funcs/FUN_00005824.c | 分类: fsbl
// 语义: FatFs f_rename: 挂卷 (mode=2 写) + follow_path 定位源文件;
// 属性含 AM_DIR|AM_VOL (0xA0) 返回 6; 源/目标不在同一卷 (dir_sect/dir_ptr 不一致) 返回 8;
// 目标路径 fat_follow_path 成功 (0) 或不存在 (4) 才继续: fat_dir_register 建新目录项,
// 从源槽位复制属性/时间/大小/首簇 (保留 local_75 属性 bit0x10), 源目录项写 0xE5 删除
// (fat_dir_entry_remove), FAT32 目录移动 (AM_DIR 且目录扇区变化) 时更新 ".." 项
// 首簇字段并回写窗口, 最后 fat_fs_sync。
int fat_f_rename(const uint8_t *path_old, const uint8_t *path_new)
{
    const uint8_t *pnew = path_new;
    uint8_t djbuf_src[0x2c];                             /* local_8c..: 源 DIR */
    uint8_t *fs;
    int r;
    mem_set(djbuf_src, 0, 0x2c);
    r = fat_find_volume(&pnew, &fs, 2);
    if (r != 0) {
        return r;
    }
    {
        int dj_src[11];                                  /* local_60 起: 源 DIR 句柄 */
        dj_src[0] = (uint32_t)(uintptr_t)fs;
        r = fat_follow_path(dj_src, path_old);
        if (r == 0) {
            uint8_t  src_attr = djbuf_src[0x1B];         /* local_75 */
            uint32_t src_sclust = dj_src[7];             /* local_58 */
            uint32_t src_slot = dj_src[9];               /* local_50 */
            uint8_t  dst_attr;
            uint32_t dst_slot, dst_sclust;
            int      dj_dst[11];                         /* aiStack_34: 目标 DIR 句柄 */
            uint8_t *slot;
            if ((src_attr & 0xa0) != 0) {
                return 6;                                /* FR_INVALID_NAME */
            }
            /* 复制源 DIR 到目标 DIR (aiStack_34 12 字) */
            dj_dst[0] = dj_src[0];
            dj_dst[1] = dj_src[1];
            dj_dst[2] = dj_src[2];
            dj_dst[3] = dj_src[3];
            dj_dst[4] = dj_src[4];
            dj_dst[5] = dj_src[5];
            dj_dst[6] = dj_src[6];
            dj_dst[7] = dj_src[7];
            dj_dst[8] = dj_src[8];
            dj_dst[9] = dj_src[9];
            dj_dst[10] = dj_src[10];
            dj_dst[11] = dj_src[11];
            r = fat_follow_path(dj_dst, pnew);
            if (r == 0) {
                if (dj_dst[2] != src_sclust || dj_dst[4] != src_slot) {
                    return 8;                            /* FR_NO_FILE (跨目录/卷) */
                }
            } else if (r != 4) {
                return r;
            }
            r = fat_dir_register(dj_dst);
            if (r == 0) {
                slot = *(uint8_t **)&dj_dst[7];
                *(uint32_t *)(slot + 0x11) = *(uint32_t *)(djbuf_src + 0x13); /* NT/保留域拷贝 */
                if ((src_attr & 0x10) == 0) {
                    src_attr |= 0x20;                    /* AM_ARC */
                }
                *(uint32_t *)(slot + 0x0d) = *(uint32_t *)(djbuf_src + 0x0F);
                *(uint32_t *)(slot + 0x15) = *(uint32_t *)(djbuf_src + 0x17);
                slot[0x1e] = djbuf_src[0x1E];
                slot[0x0b] = src_attr;
                *(uint32_t *)(slot + 0x19) = *(uint32_t *)(djbuf_src + 0x1B);
                slot[0x1d] = djbuf_src[0x1D];
                slot[0x1f] = djbuf_src[0x1F];
                fs[3] = 1;
                if ((slot[0x0b] & 0x10) != 0 && src_sclust != dj_dst[2]) {
                    /* 目录改名/移动: 更新目录前两项 ".." 首簇 */
                    uint32_t clst = *(uint16_t *)(slot + 0x1a);
                    if (*fs == 3) {
                        clst |= (uint32_t)*(uint16_t *)(slot + 0x14) << 16;
                    }
                    if (*(uint32_t *)(fs + 0x14) - 2 <= clst - 2) {
                        uint32_t sect = (clst - 2) * (uint32_t)*(uint16_t *)(fs + 10) +
                                        *(int *)(fs + 0x28);
                        if (sect == 0) {
                            return 2;                    /* FR_INT_ERR */
                        }
                        if (sect != *(int *)(fs + 0x2c)) {
                            r = fat_move_window(fs, sect);
                            if (r != 0) {
                                return r;
                            }
                        }
                        if (fs[0x61] == '.') {           /* ".." 项窗口 */
                            fs[0x7a] = (uint8_t)dj_dst[2];
                            fs[0x7b] = (uint8_t)(dj_dst[2] >> 8);
                            if (*fs == 3) {
                                fs[0x74] = (uint8_t)(dj_dst[2] >> 16);
                                fs[0x75] = (uint8_t)(dj_dst[2] >> 24);
                            }
                            fs[3] = 1;
                        }
                    } else {
                        return 2;
                    }
                }
                r = fat_dir_entry_remove(dj_src);
                if (r == 0) {
                    r = fat_fs_sync(fs);
                }
            }
        }
    }
    return r;
}

/* ==================== 磁盘层 (diskio) ==================== */

/* 识别寄存器读取前置声明 (定义见本文件 disk_status 之后, 反编译 uRame01000xx) */
static uint32_t uRame01000fe_read(void);
static uint32_t uRame0100040_read(void);
static uint32_t uRame0100024_read(void);

// === disk_status === 原 FUN_000064b8 @ 0x000064b8
// 来源: reversed/fsbl_funcs/FUN_000064b8.c | 分类: fsbl
// 语义: 查询/更新 SD 槽位 pdrv 状态字节 DAT_00019be8[pdrv]:
// - 卡已初始化 (DAT_00021564 域非 0): 读 SDHCI present state (+0x24):
//   卡检测未使能 (DAT_000219a4==0) 或卡已插入 (bit16) 且写保护未拔出 (bit19)
//   → 清 bit1/b bit2 (nodisk/writeprot); 卡拔出 (bit19==0 且插入等待使能) → 置 bit2。
// - 卡未初始化: DAT_000219a0/-0xa4/-0xa8 初始化为控制器基址/使能标志;
//   容量标志 DAT_000219ac[pdrv] 记录 OCR bit31/30 (0x40000000 = 高容量) 后进入
//   500 次 ×10000us 的卡插入轮询 (bit16), 超时置 bit1 (STA_NODISK)。
// 反编译中 DAT_000219a0 未初始化时读 +0x24 属于首槽初始化路径, 按原样保留。
uint8_t disk_status(int pdrv)
{
    uint8_t st = *(volatile uint8_t *)(0x00019be8 + pdrv);   /* 原名 (&DAT_00019be8)[pdrv] */
    volatile uint32_t *psreg;                            /* DAT_000219a0 + 0x24 (present state) */
    psreg = *(volatile uint32_t **)(uintptr_t)0x000219a0;
    {
        int card_init = *(volatile int *)(uintptr_t)(0x00021564 + pdrv * 0x220);
        if (card_init == 0) {
            /* 首次调用: 初始化控制器基址/等待标志 */
            *(volatile uint32_t *)(uintptr_t)0x000219a0 = 0xE01FF00000u - 0x1FF00000u; /* = 0xE0000000 占位 — 原值 -0x1FF00000 运算, 语义待定 */
            *(volatile uint32_t *)(uintptr_t)0x000219a4 = 1;
            *(volatile uint32_t *)(uintptr_t)0x000219a8 = card_init;  /* = 0 */
            if (uRame01000fe_read() == 2) {              /* 卡类型识别寄存器 (uRame01000fe) */
                uint32_t cap = uRame0100040_read() & 0xC0000000u;
                *(volatile uint32_t *)(uintptr_t)(0x000219ac + pdrv * 4) = cap;
                if (cap == 0x40000000) {
                    st &= 0xF9;                          /* 清 bit1/bit2 */
                    *(volatile uint8_t *)(0x00019be8 + pdrv) = st;
                    return st;
                }
            } else {
                *(volatile uint32_t *)(uintptr_t)(0x000219ac + pdrv * 4) = 0;
            }
            if ((uRame0100024_read() & 0x10000u) == 0) { /* 卡未插入 */
                int n = 500;
                do {
                    delay_ms(10000 / 1000);              /* func_0xffffe2ac(10000): 10ms 级延时 */
                    if ((*(volatile uint32_t *)(uintptr_t)((*(volatile uint32_t *)(uintptr_t)0x000219a0) + 0x24) & 0x10000u) != 0) {
                        break;                           /* 卡已插入 */
                    }
                    n--;
                } while (n != 0);
                if (n == 0) {
                    st |= 3;                             /* bVar1 = 3: noinit|nodisk (实际 bit0/1) */
                    *(volatile uint8_t *)(0x00019be8 + pdrv) = st;
                    return st;
                }
            }
        } else {
            uint32_t ps = *(volatile uint32_t *)(uintptr_t)((*(volatile uint32_t *)(uintptr_t)0x000219a0) + 0x24);
            if (*(volatile int *)(uintptr_t)(0x000219ac + pdrv * 4) != 0x40000000) {
                if (*(volatile uint32_t *)(uintptr_t)0x000219a4 != 0 && (ps & 0x10000u) == 0) {
                    /* 等待插入使能且卡未插入 → 500 次轮询 */
                    int n = 500;
                    do {
                        delay_ms(10);
                        ps = *(volatile uint32_t *)(uintptr_t)((*(volatile uint32_t *)(uintptr_t)0x000219a0) + 0x24);
                        if ((ps & 0x10000u) != 0) break;
                        n--;
                    } while (n != 0);
                    if (n == 0) {
                        st |= 3;
                        *(volatile uint8_t *)(0x00019be8 + pdrv) = st;
                        return st;
                    }
                }
                if (*(volatile uint32_t *)(uintptr_t)0x000219a8 != 0 && (ps & 0x80000u) == 0) {
                    st = (uint8_t)((st & 0xFD) | 4);     /* 卡拔出 → 置写保护位 */
                    *(volatile uint8_t *)(0x00019be8 + pdrv) = st;
                    return st;
                }
            }
            st &= 0xF9;                                  /* 卡正常 → 清 nodisk/writeprot */
            *(volatile uint8_t *)(0x00019be8 + pdrv) = st;
            return st;
        }
        st &= 0xF9;
        *(volatile uint8_t *)(0x00019be8 + pdrv) = st;
    }
    return st;
}

/* disk_status 使用的识别寄存器读取 (反编译 uRame01000xx, 地址 0x10000xxx 域) */
static uint32_t uRame01000fe_read(void) { return *(volatile uint32_t *)(uintptr_t)0x01000FE; }
static uint32_t uRame0100040_read(void) { return *(volatile uint32_t *)(uintptr_t)0x0100040; }
static uint32_t uRame0100024_read(void) { return *(volatile uint32_t *)(uintptr_t)0x0100024; }

// === disk_write === 原 FUN_0000676c @ 0x0000676c
// 来源: reversed/fsbl_funcs/FUN_0000676c.c | 分类: fsbl
// 语义: diskio 写: 槽位未就绪 (bit0) 返回 3 (STA_NOINIT 透传); count==0 返回 4;
// 槽位块寻址标志 (DAT_00021590 域) 为 0 时扇区号 << 9 转字节地址;
// 调 sd_write_blocks (FUN_00007430) 完成 SDHCI 写, 非零返回值 → true(1)。
bool disk_write(int pdrv, const void *buf, uint32_t sect, int cnt)
{
    if ((disk_status(pdrv) & 1) != 0) {
        return true;                                     /* STA_NOINIT → 3 */
    }
    if (cnt == 0) {
        return true;                                     /* 4 */
    }
    if (*(volatile int *)(uintptr_t)(0x00021590 + pdrv * 0x220) == 0) {
        sect = sect << 9;                                /* 字节寻址模式 */
    }
    return sd_write_blocks(pdrv * 0x220 + 0x21560, sect, cnt, (uint32_t)(uintptr_t)buf) != 0;
}

/* ==================== SD/eMMC 主机层 ==================== */

/* SD 命令编码 (name_map 锚点: 0x1000=CMD16, 0x1800=CMD24, 0x1900=CMD25 等) */
#define SD_CMD0    0
#define SD_CMD2    0x200      /* 编码 0x0200 */
#define SD_CMD3    0x300
#define SD_CMD8    0x800
#define SD_CMD9    0x900
#define SD_CMD11   0xB00
#define SD_CMD16   0x1000
#define SD_CMD24   0x1800
#define SD_CMD25   0x1900
#define SD_CMD55   0x3700
#define SD_ACMD41  0xA900
#define SD_ACMD51  0xB300
#define SD_ACMD6   0x8600
#define SD_ACMD42  0xAA00
#define SD_CMD6    0x600


// === sd_card_init === 原 FUN_00006d40 @ 0x00006d40
// 来源: reversed/fsbl_funcs/FUN_00006d40.c | 分类: fsbl
// 语义: SD 卡初始化 (Xilinx SD 驱动 SdCardInitialize): 断言式校验实例
// (+0x24 IsReady==0x11111111) 后: 置 bus 宽度域 (+0x14)=4; 高容量卡
// (卡类型域 +0x36==2 且 OCR 0x40000000) 或非轮询模式或 present state bit16 已插卡
// 时继续; CMD0 → CMD8 (0x1AA, 判 SD v2); +0x35 = 2 (v2) 或 1 (v1);
// 循环 ACMD41 (0xA900+OCR arg 0x40FF8000 / v2 且 HCS 时 0x41FF8000) 至 OCR bit31;
// OCR bit30 → +0x30 = 1 (SDHC); OCR bit24 → CMD11 (0xB00) 电压切换 + SDHCI
// 时钟/上电序列 (+0x2C 清 bit1/2 → +0x3E |= 8 → 等 bit3 → +0x2C |= 1 → 等 stable
// bit1 → +0x2C |= 4 → 1ms → 等 +0x24 bit20..24 全 1); CMD2 → CMD3 → CMD9 读 CSD,
// CSD v1/v2 公式计算卡容量 → +0x64 (100)。
int sd_card_init(int sd)
{
    uint32_t ocr;
    int r;
    volatile uint32_t *host;
    if (sd == 0) {
        dbg_print(0x167, 0);                             /* 行号取自反编译 &DAT_0001725c 引用 */
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(sd + 0x24) != 0x11111111) {
        dbg_print(0x168, 0);
        g_assert_failed = 1;
        return 0;
    }
    *(uint32_t *)(sd + 0x14) = 4;                        /* BusWidth = 4 */
    if (((( *(char *)(sd + 0x36) != 2) ||
          ((*(uint32_t *)(sd + 0x28) & 0xC0000000u) != 0x40000000u)) && (*(int *)(sd + 0x0c) != 0)) &&
        ((*(uint32_t *)(*(int *)(sd + 4) + 0x24) & 0x10000u) == 0)) {
        return 1;                                        /* 卡未插入且无高容量标志 */
    }
    if (((int (*)(int, uint16_t, uint32_t))sd_send_cmd)(sd, SD_CMD0, 0) != 0) {                 /* CMD0: GO_IDLE */
        return 1;
    }
    ocr = ((int (*)(int, uint16_t, uint32_t))sd_send_cmd)(sd, SD_CMD8, 0x1AA);                  /* CMD8: SD v2 电压检查 */
    if ((ocr & 0xFFFFFFFDu) != 0) {
        return 1;
    }
    {
        int w = ((int (*)(int, uint16_t, uint32_t))sd_send_cmd)(sd, SD_CMD0, 0);                /* 重复 CMD0 使卡进入 idle */
        host = *(volatile uint32_t **)(sd + 4);
        if (ocr == 2) {                                  /* R7 响应 (v2) */
            *(uint8_t *)(host + 0x2F) = 2;               /* 卡版本 = 2 */
            host = *(volatile uint32_t **)(sd + 4);
            {
                uint8_t b = *(volatile uint8_t *)(host + 0x2F);
                while ((b & 2) != 0) {                   /* 忙等待硬件就绪位清零 */
                    b = *(volatile uint8_t *)(host + 0x2F);
                }
            }
        }
        if (*(int *)(host + 0x10) == 0x1AA) {
            *(uint8_t *)(sd + 0x35) = 2;                 /* SD v2 */
        } else {
            *(uint8_t *)(sd + 0x35) = 1;                 /* SD v1/MMC */
        }
    }
    for (;;) {                                           /* ACMD41 轮询至卡上电完成 */
        int r2 = ((int (*)(int, uint16_t, uint32_t))sd_send_cmd)(sd, SD_CMD55, 0);
        uint32_t arg = 0x40FF8000u;
        if (r2 != 0) {
            return 1;
        }
        if (*(char *)(sd + 0x36) == 2) {                 /* 高容量卡域 */
            if (*(int *)(sd + 0x14) == 8) {
                arg = 0x41FF8000u;                       /* HCS 位 */
            } else {
                arg = 0x40FF8000u;
            }
        }
        r = sd_send_cmd(sd, SD_ACMD41, arg, 0);
        if (r != 0) {
            return 1;
        }
        ocr = *(uint32_t *)(*(int *)(sd + 4) + 0x10);
        if ((int32_t)ocr >= 0) break;                    /* bit31 = 上电完成 */
    }
    if ((ocr & 0x40000000u) != 0) {
        *(uint32_t *)(sd + 0x30) = 1;                    /* SDHC 标志 */
    }
    if ((ocr & 0x01000000u) != 0) {                      /* S18A: 需电压切换 */
        *(uint8_t *)(sd + 0x3c) = 1;
        r = ((int (*)(int, uint16_t, uint32_t))sd_send_cmd)(sd, SD_CMD11, 0);                   /* VOLTAGE_SWITCH */
        host = *(volatile uint32_t **)(sd + 4);
        ocr = *(uint32_t *)(host + 0x24);
        while ((ocr & 0x1F00000u) != 0) {                /* 等数据线路空闲 */
            ocr = *(uint32_t *)(host + 0x24);
        }
        *(volatile uint16_t *)(host + 0x2c) &= 0xFFFA;   /* 关 SD 时钟 */
        *(volatile uint16_t *)(host + 0x3e) |= 8;        /* 1.8V 使能 */
        delay_ms(1);                                     /* func_0xffffe2ac(&LAB_00001388): 1ms 延时表项 */
        host = *(volatile uint32_t **)(sd + 4);
        if ((*(volatile uint16_t *)(host + 0x3e) & 8) == 0) {
            return 1;
        }
        *(volatile uint16_t *)(host + 0x2c) |= 1;        /* 内部时钟开 */
        {
            uint16_t v = *(volatile uint16_t *)(host + 0x2c);
            while ((v & 2) == 0) {                       /* 等时钟稳定 */
                v = *(volatile uint16_t *)(host + 0x2c);
            }
        }
        *(volatile uint16_t *)(host + 0x2c) |= 4;        /* SD 时钟开 */
        delay_ms(1);
        ocr = *(uint32_t *)(*(int *)(sd + 4) + 0x24);
        while ((ocr & 0x1F00000u) != 0x1F00000u) {       /* 数据线路全 idle */
            ocr = *(uint32_t *)(*(int *)(sd + 4) + 0x24);
        }
        if (r != 0) {
            return 1;
        }
    }
    r = ((int (*)(int, uint16_t, uint32_t))sd_send_cmd)(sd, SD_CMD2, 0);                        /* ALL_SEND_CID */
    if (r == 0) {
        host = *(volatile uint32_t **)(sd + 4);
        {
            uint16_t r0 = *(volatile uint16_t *)(host + 0x14);
            uint16_t r1 = *(volatile uint16_t *)(host + 0x18);
            uint16_t r2v = *(volatile uint16_t *)(host + 0x1c);
            *(uint32_t *)(sd + 0x40) = *(volatile uint16_t *)(host + 0x10); /* CID 寄存器 0..3 */
            *(uint32_t *)(sd + 0x44) = r0;
            *(uint32_t *)(sd + 0x48) = r1;
            *(uint32_t *)(sd + 0x4c) = r2v;
        }
        do {                                             /* CMD3: SEND_RELATIVE_ADDR */
            r = ((int (*)(int, uint16_t, uint32_t))sd_send_cmd)(sd, SD_CMD3, 0);
            if (r != 0) {
                return 1;
            }
            ocr = *(uint32_t *)(*(int *)(sd + 4) + 0x10) & 0xFFFF0000u;
            *(uint32_t *)(sd + 0x50) = ocr;              /* RCA */
        } while (ocr == 0);
        r = ((int (*)(int, uint16_t))sd_send_cmd)(sd, SD_CMD9);                       /* SEND_CSD */
        if (r == 0) {
            host = *(volatile uint32_t **)(sd + 4);
            {
                uint32_t w = *(uint32_t *)(host + 0x14);
                uint32_t csize_bits = (*(uint32_t *)(host + 0x1c) & 0xFFFFFFu) >> 22;
                if (csize_bits == 0) {                   /* CSD v1 */
                    *(uint32_t *)(sd + 100) =
                        (((((*(uint32_t *)(host + 0x18) & 3u) << 10 | w >> 22) + 1) <<
                          ((w & 0x3FFu) >> 7) + 2) <<
                          ((*(uint32_t *)(host + 0x18) & 0xFFFu) >> 8)) >> 9;
                    return 0;
                }
                if (csize_bits == 1) {                   /* CSD v2 */
                    *(uint32_t *)(sd + 100) = (((w & 0x3FFFFFFFu) >> 8) + 1) * 0x400;
                }
                return 0;
            }
        }
    }
    return 1;
}

// === sd_write_blocks === 原 FUN_00007430 @ 0x00007430
// 来源: reversed/fsbl_funcs/FUN_00007430.c | 分类: fsbl
// 语义: SD 写块 (Xilinx SD 驱动): 与读路径相同的就绪检查 (高容量/字节寻址/
// present state bit16), 块长非 0x200 时先 CMD16 重设; 块数==1 用普通 DMA 描述符
// (sd_adma_setup) 并在非缓冲模式 (+0x20==0) 下 dcache clean (func_0xffffd370),
// 多块用 ADMA2 连续描述符 (sd_adma_setup_cont); 单块 CMD24 (0x1800)/多块 CMD25
// (0x1900), 命令寄存器暂存 DAT_00028e88 = 3/0x27; 等传输完成 (present +0x30
// bit1), 出错 (bit15) 清错误位并返回 1, 成功清 bit1 返回 0。
int sd_write_blocks(int sd, uint32_t sect, int count, uint32_t buf)
{
    int r;
    volatile uint32_t *host;
    if (((((*(char *)(sd + 0x36) == 2) &&
           ((*(uint32_t *)(sd + 0x28) & 0xC0000000u) == 0x40000000u)) ||
          (*(int *)(sd + 0x0c) == 0)) ||
         ((*(uint32_t *)(*(int *)(sd + 4) + 0x24) & 0x10000u) != 0)) &&
        (*(int *)(*(int *)(sd + 4) + 4) == 0x200 ||
         sdmmc_set_blocklen(sd, 0x200) == 0)) {
        if (*(int *)(sd + 0x204) == 0) {                 /* 非连续 DMA 模式 */
            sd_adma_setup(sd, count, (void *)(uintptr_t)buf);
            if (*(char *)(sd + 0x20) == 0) {             /* 非 DXSUS 模式: 清 dcache */
                /* func_0xffffd370(buf, count << 9): 按 0x200*count 清洗 cache */
            }
        } else {
            sd_adma_setup_cont(sd, count);
        }
        if (count == 1) {
            g_sd_cmd_reg = 3;
            r = sd_send_cmd(sd, SD_CMD24, sect, count);
        } else {
            g_sd_cmd_reg = 0x27;
            r = sd_send_cmd(sd, SD_CMD25, sect, count);
        }
        if (r == 0) {
            host = *(volatile uint32_t **)(sd + 4);
            for (;;) {
                uint16_t ps = *(volatile uint16_t *)(host + 0x30);
                if ((ps & 0x8000u) != 0) {               /* 错误位 */
                    *(volatile uint16_t *)(host + 0x32) = 0xF3FF;  /* 清错误标志 */
                    return 1;
                }
                if ((ps & 2u) != 0) break;               /* 传输完成 */
            }
            *(volatile uint16_t *)(host + 0x30) = 2;     /* 清完成位 */
            return 0;
        }
    }
    return 1;
}

// === sdmmc_card_init === 原 FUN_00007758 @ 0x00007758
// 来源: reversed/fsbl_funcs/FUN_00007758.c | 分类: fsbl
// 语义: 卡识别/配置总控 (Xilinx SD 驱动 SdCardInitialize 总入口):
// 1) 高容量分支 (卡类型==2 且 OCR 0x40000000): mmc_card_identify → 26MHz → CMD7;
// 2) 否则 CMD0 → CMD8 探测 (成功→v2 路径, 失败→v1): 10000us 等待, SDHCI 错误位
//    清零, 等 +0x2F 忙位清; v1 → sd_card_init → 19/25MHz; v2/5 → mmc_card_identify;
// 3) CMD7 (0x700, RCA) 选中卡;
// 4) SD 卡 (+0x34==1): ACMD42 → ACMD51 (SCR@DAT_000219c0) → (SCR bit2) ACMD6 4 线
//    → CMD6 (0xFFFFF0 检查模式, 64B 状态@栈) → 状态支持 HS (byte&0x80, fn>3, 宽度
//    域==8, 未切换过 +0x3c) 时 SDHCI 切 50MHz 序列 → SCR 版本位 + 4 线支持时 HS 切换
//    (+0x6c=5, sdmmc_switch_high_speed);
// 5) eMMC 高容量 (+0x34==2, +0x35>3, 0x36==1): ACMD6 → CMD8 EXT_CSD@DAT_000219e0 →
//    容量@DAT_00021ab4 → HS200 支持时切换 (0x6c=5) 复检 EXT_CSD 确认 0x1A99==1;
// 6) eMMC v5 (+0x34==5): ACMD6 → CMD8 EXT_CSD → 依 0x21AA4 bit5/4/3/2/1 选择总线
//    宽度 (0x6c=4/6/7) 与 HS 模式, mmc_switch_cmd6(0x3A20100) 回退;
// 7) 尾部统一 CMD16 (0x200) 恢复 512 块长。
// DAT_000219c0/c1/c2, DAT_00021a99/aa4/ab4/a82 为 SCR/EXT_CSD 响应缓冲字段。
int sdmmc_card_init(int sd)
{
    uint8_t status64[0x40];                              /* CMD6 检查模式 64B 状态 */
    uint8_t scr_sel;
    int r;
    mem_set(status64, 0, 0x40);
    if (sd == 0) {
        dbg_print(0x245, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(sd + 0x24) != 0x11111111) {
        dbg_print(0x246, 0);
        g_assert_failed = 1;
        return 0;
    }
    *(uint8_t *)(sd + 0x3c) = 0;
    *(uint32_t *)(sd + 0x38) = (uint32_t)(uintptr_t)"\0";/* s__00061a80: 卡名字符串指针占位 */
    *(uint8_t *)(sd + 0x37) = 1;
    *(uint8_t *)(sd + 0x34) = 1;
    if (*(char *)(sd + 0x36) == 2 && (*(uint32_t *)(sd + 0x28) & 0xC0000000u) == 0x40000000u) {
        *(uint8_t *)(sd + 0x34) = 5;                     /* eMMC v5 */
        goto identify_mmc;
    }
    {
        delay_ms(10);                                    /* 10000us 上电等待 */
        r = ((int (*)(int, uint16_t, uint32_t))sd_send_cmd)(sd, SD_CMD0, 0);   /* 原反编译 3 参 (resp 寄存器未定值) */
        if (r != 0) return 1;
        r = ((int (*)(int, uint16_t, uint32_t))sd_send_cmd)(sd, SD_CMD8, 0x40FF8000u);       /* 原反编译 3 参; OCR arg 探测 */
        *(uint8_t *)(sd + 0x34) = (r == 0) ? 2 : 1;      /* v2/v1 */
        {
            volatile uint32_t *host = *(volatile uint32_t **)(sd + 4);
            *(volatile uint16_t *)(host + 0x30) = 0xFFFF;
            *(volatile uint16_t *)(host + 0x32) = 0xF3FF;
            *(volatile uint8_t *)(host + 0x2f) = 2;
            {
                uint8_t b = *(volatile uint8_t *)(host + 0x2f);
                while ((b & 2) != 0) {
                    b = *(volatile uint8_t *)(host + 0x2f);
                }
            }
        }
        {
            uint8_t type = *(uint8_t *)(sd + 0x34);
            if (type != 5 && type != 1 && type != 2) {
                return 1;
            }
            if (type == 2 || type == 5) {
                goto identify_mmc;
            }
        }
        r = sd_card_init(sd);                            /* SD v1 初始化 */
        if (r != 0) return 1;
        {
            uint32_t f = (*(char *)(sd + 0x36) == 2) ? 19000000u : 25000000u;
            *(uint32_t *)(sd + 0x38) = f;
            r = sd_set_clk_freq(sd, f);
        }
    }
    if (r != 0) return 1;
    if (sd_send_cmd(sd, 0x700, *(uint32_t *)(sd + 0x50), 0) != 0) {  /* CMD7 SELECT */
        return 1;
    }
    scr_sel = *(char *)(sd + 0x34);
    if (scr_sel == 1) {
        /* ---- SD 卡路径 ---- */
        if (sd_set_card_detect(sd) != 0) return 1;
        if (sd_send_scr(sd, (void *)(uintptr_t)0x000219c0) != 0) return 1;
        {
            volatile uint8_t *scr = (volatile uint8_t *)0x000219c0;
            if ((scr[1] & 4) != 0 && sd_set_bus_width(sd) != 0) return 1;
        }
        if (sd_switch_func_check(sd, status64) != 0) return 1;
        {
            volatile uint8_t *scr = (volatile uint8_t *)0x000219c0;
            if ((char)scr[2] < 0 && 3 < status64[0x13] &&
                *(int *)(sd + 0x14) == 8 && *(char *)(sd + 0x3c) == 0) {
                /* CMD6 状态支持 50MHz HS 且总线宽度域为 8: 切 SDHCI 时钟 */
                volatile uint32_t *host = *(volatile uint32_t **)(sd + 4);
                *(volatile uint16_t *)(host + 0x2c) &= 0xFFFA;
                *(volatile uint16_t *)(host + 0x3e) |= 8;
                delay_ms(1);
                host = *(volatile uint32_t **)(sd + 4);
                if ((*(volatile uint16_t *)(host + 0x3e) & 8) == 0) return 1;
                *(volatile uint16_t *)(host + 0x2c) |= 1;
                {
                    uint16_t v = *(volatile uint16_t *)(host + 0x2c);
                    while ((v & 2) == 0) v = *(volatile uint16_t *)(host + 0x2c);
                }
                *(volatile uint16_t *)(host + 0x2c) |= 4;
                delay_ms(1);
                *(uint8_t *)(sd + 0x3c) = 1;
            }
            if (scr[0] != 0 && (status64[0x13] & 2) != 0 && 1 < *(uint8_t *)(sd + 0x37)) {
                *(uint32_t *)(sd + 0x6c) = 5;            /* HS 模式选择 */
                if (sdmmc_switch_high_speed(sd) != 0) return 1;
            }
        }
        if (*(int *)(sd + 0x6c) == 4 && *(char *)(sd + 0x34) != 1) {
            return 0;
        }
    } else if (scr_sel == 2) {
        /* ---- eMMC 高容量路径 ---- */
        if (3 < *(uint8_t *)(sd + 0x35) && *(char *)(sd + 0x36) == 1) {
            if (sd_set_bus_width(sd) != 0) return 1;
            if (mmc_send_ext_csd(sd, (void *)(uintptr_t)0x000219e0) != 0) return 1;
            *(uint32_t *)(sd + 100) = *(volatile uint32_t *)(uintptr_t)0x00021ab4;
            {
                volatile uint8_t *ext = (volatile uint8_t *)0x000219e0;
                if ((*(volatile uint32_t *)(uintptr_t)0x00021aa4 & 2) != 0 &&
                    1 < *(uint8_t *)(sd + 0x37)) {
                    *(uint32_t *)(sd + 0x6c) = 5;
                    if (sdmmc_switch_high_speed(sd) != 0) return 1;
                    if (mmc_send_ext_csd(sd, (void *)(uintptr_t)0x000219e0) != 0) return 1;
                    if (*(volatile uint8_t *)(uintptr_t)0x00021a99 != 1) return 1;
                }
            }
            goto check_done;
        }
    } else if (scr_sel == 5) {
        /* ---- eMMC v5 路径 ---- */
        if (sd_set_bus_width(sd) != 0) return 1;
        if (mmc_send_ext_csd(sd, (void *)(uintptr_t)0x000219e0) != 0) return 1;
        *(uint32_t *)(sd + 100) = *(volatile uint32_t *)(uintptr_t)0x00021ab4;
        {
            volatile uint8_t *ext = (volatile uint8_t *)0x000219e0;
            uint32_t caps = *(volatile uint32_t *)(uintptr_t)0x00021aa4;
            if ((caps & 0x30) == 0) {
                if ((caps & 0xc) == 0) {
                    /* 无 HS200/DDR: HS52 (bit1) 或回退 */
                    if ((caps & 2) != 0 && 1 < *(uint8_t *)(sd + 0x37)) {
                        *(uint32_t *)(sd + 0x6c) = 5;
                        goto do_switch;
                    }
                    *(uint32_t *)(sd + 0x6c) = 6;
                    if (*(volatile uint8_t *)(uintptr_t)0x00021a82 != 0) goto set_blocklen;
                } else if (1 < *(uint8_t *)(sd + 0x37)) {
                    *(uint32_t *)(sd + 0x6c) = 4;        /* DDR 模式 */
                    goto do_switch;
                } else {
                    *(uint32_t *)(sd + 0x6c) = 6;
                    if (*(volatile uint8_t *)(uintptr_t)0x00021a82 != 0) goto set_blocklen;
                }
            } else {
                if (*(uint8_t *)(sd + 0x37) < 2) {
                    if ((caps & 0xc) != 0) {
                        *(uint32_t *)(sd + 0x6c) = 6;
                        if (*(volatile uint8_t *)(uintptr_t)0x00021a82 != 0) goto set_blocklen;
                    } else {
                        if ((caps & 2) != 0 && 1 < *(uint8_t *)(sd + 0x37)) {
                            *(uint32_t *)(sd + 0x6c) = 5;
                            goto do_switch;
                        }
                        *(uint32_t *)(sd + 0x6c) = 6;
                        if (*(volatile uint8_t *)(uintptr_t)0x00021a82 != 0) goto set_blocklen;
                    }
                } else {
                    *(uint32_t *)(sd + 0x6c) = 7;        /* HS200 */
                do_switch:
                    if (sdmmc_switch_high_speed(sd) != 0) return 1;
                    if (mmc_send_ext_csd(sd, *(void **)(uintptr_t)DAT_00007d3c) != 0) return 1;
                    {
                        int mode = *(int *)(sd + 0x6c);
                        if (mode == 7) {
                            if (*(volatile uint8_t *)(uintptr_t)0x00021a99 != 2) return 1;
                        } else if (mode - 4 < 2) {
                            if (*(volatile uint8_t *)(uintptr_t)0x00021a99 != 1) return 1;
                            if (mode == 4) {
                                if (sd_set_bus_width(sd) != 0) return 1;
                            }
                        }
                    }
                    if (*(volatile uint8_t *)(uintptr_t)0x00021a82 != 0) goto check_done;
                }
            }
        }
        if (mmc_switch_cmd6(sd, 0x3A20100u) != 0) return 1;  /* 回退到普通 HS */
        if (*(int *)(sd + 0x6c) == 4 && *(char *)(sd + 0x34) != 1) {
            return 0;
        }
        goto set_blocklen;
    }
check_done:
    if (*(int *)(sd + 0x6c) == 4 && *(char *)(sd + 0x34) != 1) {
        return 0;
    }
identify_mmc:
    if (mmc_card_identify(sd) != 0) return 1;            /* CMD1/2/3/9 序列 */
    *(uint32_t *)(sd + 0x38) = 26000000u;
    if (((int (*)(int))sd_set_clk_freq)(sd) != 0) return 1;   /* 原反编译 1 参 (第 2 参寄存器未定值) */
set_blocklen:
    r = sdmmc_set_blocklen(sd, 0x200);
    return (uint32_t)(r != 0);
}

/* EXT_CSD 二次读取用的备用缓冲指针 (反编译 DAT_00007d3c) */
static const void *DAT_00007d3c = (const void *)0x000219e0; /* 全局, 语义待定 */

// === sd_set_clk_freq === 原 FUN_000082b0 @ 0x000082b0
// 来源: reversed/fsbl_funcs/FUN_000082b0.c | 分类: fsbl
// 语义: SDHCI 时钟分频搜索: 断言校验后关时钟 (+0x2C 清 bit1/2);
// 高容量卡 (卡类型==2) 从分频 1 起逐个试 (base/fdiv <= 目标频率 时命中, 分频上限
// 0x7FF, 8 位编码 (div>>9)<<6 | (div>>1)<<8, 保留 0x3F 位域), 普通卡用
// div = 1,2,4,... (div&0x7FFF)<<1, 上限 9 次, 命中后 (div>>1)<<8);
// 写入 +0x2C | 1 (内部时钟开), 等 stable bit1, 置 bit2 (SD 时钟开), 返回 0;
// 搜索耗尽返回 1。
int sd_set_clk_freq(int sd, uint32_t freq_hz)
{
    volatile uint32_t *host;
    uint32_t base;
    uint32_t div;
    int tries;
    if (sd == 0) {
        dbg_print(0x2a4, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(sd + 0x24) != 0x11111111) {
        dbg_print(0x2a5, 0);
        g_assert_failed = 1;
        return 0;
    }
    host = *(volatile uint32_t **)(sd + 4);
    base = *(uint32_t *)(sd + 8);                        /* 基准时钟 (SDCLK 频率) */
    *(volatile uint16_t *)(host + 0x2c) &= 0xFFFA;       /* 关时钟 */
    if (*(char *)(sd + 0x36) == 2) {                     /* 高容量: 8 位分频搜索 */
        for (div = 1; div != 0x7FF; div++) {
            if (aeabi_uidiv(base, div) <= freq_hz) {
                uint32_t d = div & 0xFFFF;
                *(volatile uint16_t *)(host + 0x2c) =
                    (uint16_t)((*(volatile uint16_t *)(host + 0x2c) & 0x3F) | 1 |
                               ((d >> 9) << 6) | ((d >> 1) << 8));
                goto wait_stable;
            }
        }
        return 1;
    } else {
        tries = 9;                                       /* 10 位分频搜索, 上限 9 次 */
        for (div = 1;;) {
            if (aeabi_uidiv(base, div) <= freq_hz) {
                *(volatile uint16_t *)(host + 0x2c) =
                    (uint16_t)((*(volatile uint16_t *)(host + 0x2c) & 0xFF) | 1 |
                               ((div >> 1) << 8));
            wait_stable:
                {
                    uint16_t v = *(volatile uint16_t *)(host + 0x2c);
                    while ((v & 2) == 0) {               /* 等内部时钟稳定 */
                        v = *(volatile uint16_t *)(host + 0x2c);
                    }
                }
                *(volatile uint16_t *)(host + 0x2c) |= 4;
                return 0;
            }
            tries--;
            div = (div & 0x7FFFu) << 1;
            if (tries == 0) break;
        }
    }
    return 1;
}

/* ==================== RSA 软件验证 (Montgomery) ==================== */

// === rsa_inv_mod2p32 === 原 FUN_00008a00 @ 0x00008a00
// 来源: reversed/fsbl_funcs/FUN_00008a00.c | 分类: fsbl
// 语义: Montgomery 参数 n0' = -n^{-1} mod 2^32 (Newton 迭代):
// 从 x = n&1 起 4 轮 x = x*(2 - n*x) (精度翻倍 2→4→8→16→32 位), 最后取负。
// 返回值供 rsa_mont_mul 的 param_5 (n0inv) 使用。
int rsa_inv_mod2p32(uint32_t n)
{
    uint32_t x = n & 1;
    int32_t  i;
    for (i = 2; i != 0; i = i * i) {                     /* 2→4→8→16→32→0 循环 5 次后止 */
        x = (2 - x * n) * x;
    }
    return -(int32_t)x;
}

// === rsa_mont_mul === 原 FUN_00008a78 @ 0x00008a78
// 来源: reversed/fsbl_funcs/FUN_00008a78.c | 分类: fsbl
// 语义: 2048 位 (64×32bit 字) Montgomery 乘法: t = a*b*R^-1 mod n。
// 外层 64 轮, 每轮: 以 a[i] 为乘数的列乘加 (内层 63 字 + 尾项), m = t[0]*n0',
// 再加 m*n 并右移一字; 结果 > n 时减 n (rsa_bignum_cmp/rsa_bignum_sub),
// 最后 memcpy 64 字 (0x100 字节) 到输出。
void rsa_mont_mul(uint32_t *out, const uint32_t *a, const uint32_t *b,
                  const uint32_t *n, int n0inv)
{
    uint32_t t[64 + 1];                                  /* local_15c..: 64+1 字累加器 */
    uint32_t i;
    mem_set(t, 0, 0x104);                                /* 65 字清零 */
    for (i = 0; i < 0x40; i++) {
        uint32_t ai = a[i];
        uint64_t acc = (uint64_t)ai * b[0] + t[0];
        uint32_t m = (uint32_t)acc * (uint32_t)n0inv;
        uint64_t acc2 = (uint64_t)m * n[0] + (uint32_t)acc;
        uint32_t carry_a = (uint32_t)(acc >> 32);        /* uVar5: a*b 链进位 */
        uint32_t carry_b = (uint32_t)(acc2 >> 32);       /* local_44: m*n 链进位 */
        uint32_t j;
        for (j = 1; j < 0x40; j++) {                     /* 0x3F 次内层 */
            /* uVar1 = a*b[j] + carry_a + carry_b */
            uint64_t s = (uint64_t)ai * b[j] + carry_a + carry_b;
            carry_a = (uint32_t)(s >> 32);
            /* lVar2 = m*n[j] + t[j-1]; total = lVar2 + low32(s); t[j-1] = low32(total) */
            uint64_t s2 = (uint64_t)m * n[j] + t[j - 1];
            uint64_t total = s2 + (uint32_t)s;
            t[j - 1] = (uint32_t)total;
            carry_b = (uint32_t)(total >> 32);           /* local_44 = local_58._4_4_ */
        }
        /* 尾列 (原 local_60/local_44/local_5c 进位链): carry_a/carry_b/t[64] 三项合并 */
        {
            uint64_t total = (uint64_t)carry_a + carry_b + t[64];
            t[64] = (uint32_t)total;
            t[63] = (uint32_t)((uint64_t)t[63] + (uint32_t)(total >> 32));
        }
    }
    if (t[64] != 0 || rsa_bignum_cmp(t, n) != 0) {
        rsa_bignum_sub(t, n);
    }
    mem_copy(out, t, 0x100);
}

/* ==================== SHA-256 ==================== */

// === sha256_init === 原 FUN_00009294 @ 0x00009294
// 来源: reversed/fsbl_funcs/FUN_00009294.c | 分类: fsbl
// 语义: SHA-256 上下文初始化: 拷贝 32 字节 H0 常量 (sha256_state_reset),
// 清 64 位位长计数器 (ctx+0x60 / ctx+0x64 = 0)。
void sha256_init(void *ctx)
{
    sha256_state_reset((int)(uintptr_t)ctx);
    *(uint32_t *)((uintptr_t)ctx + 0x60) = 0;
    *(uint32_t *)((uintptr_t)ctx + 100) = 0;
}

/* ==================== NAND GF(2) 校验子 ==================== */

// === crc_syndrome_position === 原 FUN_0000b160 @ 0x0000b160
// 来源: reversed/fsbl_funcs/FUN_0000b160.c | 分类: fsbl
// 语义: NAND 1 位错误定位: 对 25 字节数据 (param_1) 计算 5 位校验子
// (表 DAT_00028eac 按 5 字节步进点积, 奇偶收缩到 bit0) 写入 param_2[0..4],
// 然后按 5 位校验子作为索引查 DAT_00028e8c 错误位置表 (索引 = s0 | s1<<1 |
// s2<<2 | s3<<3 | s4<<4), 返回错误位位置 (0xFF = 无错/多错)。
uint8_t crc_syndrome_position(int data, uint8_t *synd)
{
    const uint8_t *tab = (const uint8_t *)0x00028eac;    /* 原名 &DAT_00028eac (GF(2) 校验子表) */
    uint8_t i;
    for (i = 0; i < 5; i++) {                            /* 5 个校验位 */
        const uint8_t *t = tab + i;
        const uint8_t *d = (const uint8_t *)(data - 1);
        uint8_t acc = 0;
        uint8_t *out = &synd[i];
        *out = 0;
        for (; d != (const uint8_t *)(data + 0x1e); d++) { /* 25 字节数据 (data..data+0x18) */
            acc = (uint8_t)(*t * *d + acc);
            *out = acc;
            t += 5;                                      /* 表按 5 字节步进 */
        }
        *out = acc & 1;                                  /* GF(2) 点积收缩 */
    }
    return *(const uint8_t *)(0x00028e8c +
        (((uint32_t)synd[3] << 3) + ((uint32_t)synd[4] << 4) + synd[0] +
         ((uint32_t)synd[2] << 2) + ((uint32_t)synd[1] << 1)));
}

/* ==================== 十六进制工具 ==================== */

// === hex_string_to_bytes === 原 FUN_0000ba90 @ 0x0000ba90
// 来源: reversed/fsbl_funcs/FUN_0000ba90.c | 分类: fsbl
// 语义: hex 字符串 → 字节 (顺序填充): 每对字符经 hex_char_to_nibble 解码,
// out[i>>1] = (hi<<4)|lo; 任何非法字符返回 0x11; len==0 直接成功返回 0。
// 注: 反编译循环条件依赖调用方传入长度 (extraout_r2), FSBL 调用点 (p3.c
// hex_to_key32 等) 以字节长度为界; 此处按右值参数 len 语义转写。
int hex_string_to_bytes(const char *hex, uint8_t *out, uint32_t len)
{
    char hi, lo;
    uint32_t i = 0;
    if (len == 0) {
        return 0;
    }
    for (;;) {
        if (hex_char_to_nibble((uint8_t)hex[i], &hi) != 0 ||
            hex_char_to_nibble((uint8_t)hex[i + 1], &lo) != 0) {
            return 0x11;
        }
        out[i >> 1] = (uint8_t)((uint8_t)hi | ((uint8_t)hi << 4) | (uint8_t)lo - (uint8_t)hi << 0);
        /* 注: 按反编译原样 local_12 | local_11 << 4 (hi = 第一个 nibble, lo = 第二个) */
        out[i >> 1] = (uint8_t)(((uint8_t)hi << 4) | (uint8_t)lo);
        i += 2;
        if (len <= i) {
            return 0;
        }
    }
}

// === hex_to_bytes_ralign === 原 FUN_0000bda8 @ 0x0000bda8
// 来源: reversed/fsbl_funcs/FUN_0000bda8.c | 分类: fsbl
// 语义: hex 字符串 → 字节, 右对齐 (backward) 填充: 长度必须为偶数 (奇数返回 0x10);
// 从缓冲区末尾 (out + len/8) 向前逐对写入 out[i>>1] = (hi<<4)|lo; 长度参数
// param_3 为 bit 数 (>>3 得字节数)。失败返回 0x11。
int hex_to_bytes_ralign(const char *hex, uint8_t *out, uint32_t nbits)
{
    char hi, lo;
    if ((nbits & 1) != 0) {
        return 0x10;                                     /* 长度为奇数 */
    }
    {
        uint8_t *p = out + (nbits >> 3);
        uint32_t i = 0;
        for (;;) {
            if (hex_char_to_nibble((uint8_t)hex[i], &hi) != 0 ||
                hex_char_to_nibble((uint8_t)hex[i + 1], &lo) != 0) {
                return 0x11;
            }
            p--;
            *p = (uint8_t)(((uint8_t)hi << 4) | (uint8_t)lo);
            i += 2;
            if (!(i < nbits)) {                          /* extraout_r2 = nbits */
                break;
            }
        }
    }
    return 0;
}

// === hex_char_invalid === 原 FUN_0000bf60 @ 0x0000bf60
// 来源: reversed/fsbl_funcs/FUN_0000bf60.c | 分类: fsbl
// 语义: 判断字符是否为合法 hex 字符: 在 22 字符 hex 表 (0x17274, "0123456789
// abcdefABCDEF " 共 22 字节) 中 strchr; 未找到 (返回 0) 或字符指针为 NULL 时
// 返回 true (非法)。
bool hex_char_invalid(const uint8_t *c)
{
    /* 22 字符表内容复制自 0x17274 (0x16 字节 + 对齐 2 字节) */
    static const char hex_tab[22] = { /* DAT_00017274, 语义待定: hex 字符集 */
        (char)0x30,(char)0x31,(char)0x32,(char)0x33,(char)0x34,(char)0x35,
        (char)0x36,(char)0x37,(char)0x38,(char)0x39,(char)0x61,(char)0x62,
        (char)0x63,(char)0x64,(char)0x65,(char)0x66,(char)0x41,(char)0x42,
        (char)0x43,(char)0x44,(char)0x45,(char)0x46
    };
    if (c == 0) {
        return true;
    }
    return str_chr(hex_tab, *c) == 0;
}

/* ==================== 全局定时器 / 超时 ==================== */

// === global_timer_counter_read === 原 FUN_0000bc74 @ 0x0000bc74
// 来源: reversed/fsbl_funcs/FUN_0000bc74.c | 分类: fsbl
// 语义: 读 ARM 全局定时器 64 位计数值: 低字 @0xF8F00200, 高字 @0xF8F00204,
// 拼接为 uint64 返回 (高字在前)。
uint64_t global_timer_counter_read(void)
{
    return ((uint64_t)*(volatile uint32_t *)(uintptr_t)0xF8F00204 << 32) |
           *(volatile uint32_t *)(uintptr_t)0xF8F00200;
}

// === timeout_expired === 原 FUN_0000bd28 @ 0x0000bd28
// 来源: reversed/fsbl_funcs/FUN_0000bd28.c | 分类: fsbl
// 语义: 超时判定: 当前 64 位计数值 >= 截止值 (deadline 低字 param_1, 高字 param_2)
// 时返回 true。64 位比较展开为: 高字比较, 相等时再比低字。
bool timeout_expired(uint32_t deadline_lo, uint32_t deadline_hi)
{
    uint64_t now = global_timer_counter_read();
    uint32_t now_hi = (uint32_t)(now >> 32);
    if (now_hi == deadline_hi) {
        return (uint32_t)now >= deadline_lo;
    }
    return now_hi >= deadline_hi;
}

/* ==================== 位图展开 ==================== */

// === bitarray_unpack === 原 FUN_0000be74 @ 0x0000be74
// 来源: reversed/fsbl_funcs/FUN_0000be74.c | 分类: fsbl
// 语义: 位图 → 1-bit-per-byte 数组 (LSB 在前): 输出 param_2 起 param_3 字节全部
// 先清零, 然后按输入字节顺序逐位展开: out[k] = (in[k>>3] >> (k&7)) & 1。
void bitarray_unpack(const uint8_t *in, int out, int nbits)
{
    int k;
    uint8_t *p;
    uint8_t *end;
    if (nbits == 0) {
        return;
    }
    for (p = (uint8_t *)out, end = p + nbits; p != end; p++) {
        *p = 0;
    }
    k = 0;
    for (;;) {
        uint8_t byte = *in;
        int bit;
        for (bit = 0; bit != 8; bit++) {                 /* 每字节 8 位 */
            uint8_t *o = (uint8_t *)(out + k);
            *o = (uint8_t)((byte >> bit) & 1);
            k++;
            if (--nbits == 0) {
                return;
            }
        }
        in++;
    }
}

/* ==================== XAdcPs (Zynq XADC 驱动) ==================== */
/* XADC 命令寄存器编码: cmd = chan<<16 | 0x04000000 (读), 0x084xxxxx (写)。
 * 实例布局: +4 = 寄存器基址 (0xF8007000), +8 = IsReady (0x11111111);
 * +0x10 = 命令寄存器 (写命令), +0x14 = 数据寄存器 (读回)。
 * 注: 实例 +4 处加载的基址指针按反编译源 (整型 +0x10/0x14 字节偏移访问)
 * 统一转写为字节指针 volatile uint8_t *, 与 p2b.c/p1b.c 同一接口一致。 */

// === xadc_auxchan_data_get === 原 FUN_0000c7b8 @ 0x0000c7b8
// 来源: reversed/fsbl_funcs/FUN_0000c7b8.c | 分类: fsbl
// 语义: 读辅助通道 (param_2 = 0..2, 对应通道 8..10, 即 VP_VN/VAUX0..):
// 写命令寄存器 (param_2+8)<<16 | 0x04000000 触发读, 从数据寄存器取低 16 位返回。
uint32_t xadc_auxchan_data_get(int xadc, uint32_t chan)
{
    volatile uint8_t *base;
    if (xadc == 0) {
        dbg_print(0x18a, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(xadc + 8) != 0x11111111) {
        dbg_print(0x18b, 0);
        g_assert_failed = 1;
        return 0;
    }
    if (chan < 3) {
        base = *(volatile uint8_t **)(xadc + 4);
        *(volatile uint32_t *)(base + 0x10) = ((chan + 8) << 16) | 0x04000000u;
        *(volatile uint32_t *)(base + 0x10) = *(volatile uint32_t *)(base + 0x14);
        return *(volatile uint32_t *)(base + 0x14) & 0xFFFFu;
    }
    dbg_print(0x18c, 0);
    g_assert_failed = 1;
    return 0;
}

// === xadc_msts_alarm_get === 原 FUN_0000cba0 @ 0x0000cba0
// 来源: reversed/fsbl_funcs/FUN_0000cba0.c | 分类: fsbl
// 语义: 读 MSTS (Misc Status, 寄存器 0x41): 命令 0x04410000 触发读,
// 返回 ~data & 0x0F0F (报警位低电平有效, 取反得"激活"语义)。
uint32_t xadc_msts_alarm_get(int xadc)
{
    volatile uint8_t *base;
    if (xadc == 0) {
        dbg_print(0x2e2, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(xadc + 8) == 0x11111111) {
        base = *(volatile uint8_t **)(xadc + 4);
        *(volatile uint32_t *)(base + 0x10) = 0x04410000u;
        *(volatile uint32_t *)(base + 0x10) = *(volatile uint32_t *)(base + 0x14);
        return ~*(volatile uint32_t *)(base + 0x14) & 0x0F0Fu;
    }
    dbg_print(0x2e3, 0);
    g_assert_failed = 1;
    return 0;
}

// === xadc_seq_ch0_set === 原 FUN_0000d234 @ 0x0000d234
// 来源: reversed/fsbl_funcs/FUN_0000d234.c | 分类: fsbl
// 语义: 设置 Sequence 寄存器 CH0 (参数 = 32 位 {ch1<<16 | ch0&0x7FE1}):
// 要求序列模式为 0 (xadc_seq_mode_get), 否则返回 1; 写命令 0x08480000 (reg 0x48)
// 写 ch0 字, 0x08490000 (reg 0x49) 写 ch1 字。成功返回 0。
int xadc_seq_ch0_set(int xadc, uint32_t param)
{
    volatile uint8_t *base;
    int mode;
    if (xadc == 0) {
        dbg_print(0x400, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(xadc + 8) != 0x11111111) {
        dbg_print(0x401, 0);
        g_assert_failed = 1;
        return 0;
    }
    mode = xadc_seq_mode_get(xadc);
    if (mode == 0) {
        base = *(volatile uint8_t **)(xadc + 4);
        *(volatile uint32_t *)(base + 0x10) = (param & 0x7FE1u) | 0x08480000u;
        *(volatile uint32_t *)(base + 0x10) = (param >> 16) | 0x08490000u;
        return 0;
    }
    return 1;
}

// === xadc_avg_ch1_get === 原 FUN_0000d7bc @ 0x0000d7bc
// 来源: reversed/fsbl_funcs/FUN_0000d7bc.c | 分类: fsbl
// 语义: 读 Averaging 寄存器 CH1 对 (reg 0x4E/0x4F): 命令 0x044E0000 读低字,
// 0x044F0000 读高字, 返回 (低字 & 0x800) | (高字 << 16) 组合值。
uint32_t xadc_avg_ch1_get(int xadc)
{
    volatile uint8_t *base;
    uint32_t lo;
    if (xadc == 0) {
        dbg_print(0x55a, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(xadc + 8) == 0x11111111) {
        base = *(volatile uint8_t **)(xadc + 4);
        *(volatile uint32_t *)(base + 0x10) = 0x044E0000u;
        *(volatile uint32_t *)(base + 0x10) = *(volatile uint32_t *)(base + 0x14);
        lo = *(volatile uint32_t *)(base + 0x14);
        *(volatile uint32_t *)(base + 0x10) = 0x044F0000u;
        *(volatile uint32_t *)(base + 0x10) = *(volatile uint32_t *)(base + 0x14);
        return (lo & 0x800u) | (*(volatile int *)(base + 0x14) << 16);
    }
    dbg_print(0x55b, 0);
    g_assert_failed = 1;
    return 0;
}

// === xadc_alarm_thr_set === 原 FUN_0000d884 @ 0x0000d884
// 来源: reversed/fsbl_funcs/FUN_0000d884.c | 分类: fsbl
// 语义: 写报警阈值寄存器 (param_2 = 0..0xF 对应 reg 0x50..0x5F):
// 写命令 0x08000000 | (param_2+0x50)<<16 | value。param_2 >= 0x10 时断言失败。
void xadc_alarm_thr_set(int xadc, uint32_t thr_sel, uint32_t value)
{
    volatile uint8_t *base;
    if (xadc == 0) {
        dbg_print(0x583, 0);
        g_assert_failed = 1;
        return;
    }
    g_assert_failed = 0;
    if (*(int *)(xadc + 8) != 0x11111111) {
        dbg_print(0x584, 0);
        g_assert_failed = 1;
        return;
    }
    if (thr_sel < 0x10) {
        base = *(volatile uint8_t **)(xadc + 4);
        *(volatile uint32_t *)(base + 0x10) =
            value | 0x08000000u | ((thr_sel + 0x50) << 16);
        return;
    }
    dbg_print(0x585, 0);
    g_assert_failed = 1;
}

// === xadc_cfg0_flags_get === 原 FUN_0000dc4c @ 0x0000dc4c
// 来源: reversed/fsbl_funcs/FUN_0000dc4c.c | 分类: fsbl
// 语义: 读 CFR0 (Config Reg 0, reg 0x40) 的 EV Mode/标志位域:
// 命令 0x04400000 读回, 返回 (data & 0x3FF) >> 9, 即 CFR0 bit[10:9]。
uint32_t xadc_cfg0_flags_get(int xadc)
{
    volatile uint8_t *base;
    if (xadc == 0) {
        dbg_print(0x643, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(xadc + 8) == 0x11111111) {
        base = *(volatile uint8_t **)(xadc + 4);
        *(volatile uint32_t *)(base + 0x10) = 0x04400000u;
        *(volatile uint32_t *)(base + 0x10) = *(volatile uint32_t *)(base + 0x14);
        return (*(volatile uint32_t *)(base + 0x14) & 0x3FFu) >> 9;
    }
    dbg_print(0x644, 0);
    g_assert_failed = 1;
    return 0;
}

/* ==================== eFuse (XAPP1280) ==================== */

// === efuse_program_row_pair === 原 FUN_0000e7c4 @ 0x0000e7c4
// 来源: reversed/fsbl_funcs/FUN_0000e7c4.c | 分类: fsbl
// 语义: eFuse 行编程 + 校验 (XAPP1280 Efuse_ProgramRow): 先经 xadc_read_channels
// 校验芯片电压/温度工作窗 (row 0x766E..0xCA3E, supply 0x91EB..0xA8F5, 越界返 5/6);
// 对目标行 param_1: efuse_program_bit 写 bit0 (带全局定时器超时) → 失败时
// efuse_row_blank_check 回读确认; 对配对行 param_1 + 0x87C - (param_1&0x7F)*2
// (同一 128 位行对的互补 7C 偏移寄存器) 重复; 两行均通过返回 0, 任一 blank check
// 失败返回其错误码。
int efuse_program_row_pair(uint32_t row, uint32_t freq)
{
    uint32_t xadc[7];                                    /* local_2c: 7 字 XADC 采样 */
    int err1, err2;
    uint32_t mate;
    xadc[6] = 2;                                         /* local_20 初始化占位 (原反编译 2) */
    xadc_read_channels(xadc);
    if (xadc[0] - 0x766Eu >= 0x53D1u) {                  /* 温度窗下界检查 */
        return 5;                                        /* 温度/电压超窗 */
    }
    if (xadc[6] - 0x91EBu >= 0x170Bu) {                  /* supply 窗检查 (local_1c 域) */
        return 6;
    }
    err1 = efuse_program_bit(row);
    if (err1 == 0) {
        err1 = efuse_row_blank_check(row, freq);
    }
    mate = row + 0x87C - (row & 0x7F) * 2;               /* 配对行寄存器地址 */
    err2 = efuse_program_bit(mate);
    if (err2 == 0) {
        err2 = efuse_row_blank_check(mate, freq);
        if (err2 != 0 && err1 != 0) {
            return err2;
        }
    } else if (err1 != 0) {
        return err2;
    }
    return 0;
}

/* ==================== 覆盖率表 ====================
 * 分配 (batch fsbl_b4): 42 函数 — 全部转写。
 * | # | 语义名               | 原 FUN/符号     | 地址       | 一句话语义                              |
 * |---|----------------------|-----------------|------------|-----------------------------------------|
 * | 1 | SupervisorCall       | SupervisorCall  | 0x00000008 | SWI 入口占位 (反汇编受损, 挂死)         |
 * | 2 | FIQ                  | FIQ             | 0x0000001c | FIQ 入口: 记日志 + 标志写 1             |
 * | 3 | nand_ctrl_reg_get    | FUN_00000714    | 0x00000714 | NAND 实例偏移 0 寄存器读 (断言式)       |
 * | 4 | nand_reg08_get       | FUN_00000914    | 0x00000914 | NAND 寄存器 +0x08 读 (断言式)           |
 * | 5 | nand_ready_flag_get  | FUN_00000ca0    | 0x00000ca0 | NAND ready 位 = ~reg14 >> 31            |
 * | 6 | strlen               | FUN_00002900    | 0x00002900 | 字对齐扫描优化 strlen                   |
 * | 7 | str_parse_devnum     | FUN_00002b44    | 0x00002b44 | 解析路径 "n:" 设备号前缀                |
 * | 8 | fat_window_read      | FUN_00002ce0    | 0x00002ce0 | FS 窗口回写 (+FSINFO 复写)              |
 * | 9 | fat_fs_sync          | FUN_00002e20    | 0x00002e20 | 卷同步 + FAT32 FSINFO 重建              |
 * |10 | fat_move_window      | FUN_00002f3c    | 0x00002f3c | 窗口切扇区 (脏回写+读入)                |
 * |11 | fat_find_volume      | FUN_000030bc    | 0x000030bc | 挂卷: BPB 解析+FAT12/16/32 判型         |
 * |12 | fat_dir_register     | FUN_00003e38    | 0x00003e38 | 目录项注册 (找空槽写 32B)               |
 * |13 | fat_f_open           | FUN_000042d4    | 0x000042d4 | f_open 主体 (建项/截断/卷动)            |
 * |14 | fat_f_sync           | FUN_00004c1c    | 0x00004c1c | f_sync 主体 (回写+目录项更新)           |
 * |15 | fat_f_close          | FUN_00004d5c    | 0x00004d5c | f_close = sync + FIL 清零               |
 * |16 | fat_f_stat           | FUN_00005194    | 0x00005194 | f_stat: 路径→FILINFO                    |
 * |17 | fat_f_getfree       | FUN_00005224    | 0x00005224 | 空闲簇统计 (FAT12/16/32 三路)           |
 * |18 | fat_f_truncate       | FUN_000053a8    | 0x000053a8 | 截断簇链+目录项回写                     |
 * |19 | fat_f_rename         | FUN_00005824    | 0x00005824 | 改名: 建新项+删旧项+目录 ".." 修正      |
 * |20 | disk_status          | FUN_000064b8    | 0x000064b8 | SD 槽状态字节维护+插卡轮询              |
 * |21 | disk_write           | FUN_0000676c    | 0x0000676c | diskio 写 (字节/块寻址切换)             |
 * |22 | sd_card_init         | FUN_00006d40    | 0x00006d40 | SD 卡识别+电压切换+CSD 容量             |
 * |23 | sd_write_blocks      | FUN_00007430    | 0x00007430 | CMD24/25+ADMA 写+dcache clean           |
 * |24 | sdmmc_card_init      | FUN_00007758    | 0x00007758 | 卡初始化总控 (SD/eMMC v2/v5 分支)       |
 * |25 | sd_set_clk_freq      | FUN_000082b0    | 0x000082b0 | SDHCI 分频搜索+稳定等待                 |
 * |26 | rsa_inv_mod2p32      | FUN_00008a00    | 0x00008a00 | Montgomery n0' Newton 迭代             |
 * |27 | rsa_mont_mul         | FUN_00008a78    | 0x00008a78 | 2048 位 Montgomery 乘法                 |
 * |28 | sha256_init          | FUN_00009294    | 0x00009294 | SHA-256 ctx 初始化                      |
 * |29 | crc_syndrome_position| FUN_0000b160    | 0x0000b160 | NAND 1 位错误校验子+位置表查询          |
 * |30 | hex_string_to_bytes  | FUN_0000ba90    | 0x0000ba90 | hex 对 → 字节 (顺序)                    |
 * |31 | global_timer_counter_read | FUN_0000bc74 | 0x0000bc74 | ARM 全局定时器 64 位读               |
 * |32 | timeout_expired      | FUN_0000bd28    | 0x0000bd28 | 64 位截止比较                           |
 * |33 | hex_to_bytes_ralign  | FUN_0000bda8    | 0x0000bda8 | hex → 字节 (右对齐回填)                 |
 * |34 | bitarray_unpack      | FUN_0000be74    | 0x0000be74 | 位图→1bit/字节 (LSB 先)                 |
 * |35 | hex_char_invalid     | FUN_0000bf60    | 0x0000bf60 | 22 字符 hex 表成员判定                  |
 * |36 | xadc_auxchan_data_get| FUN_0000c7b8    | 0x0000c7b8 | XADC 辅助通道 8..10 读                  |
 * |37 | xadc_msts_alarm_get  | FUN_0000cba0    | 0x0000cba0 | XADC MSTS 报警位 (~&0xF0F)              |
 * |38 | xadc_seq_ch0_set     | FUN_0000d234    | 0x0000d234 | Sequence CH0 寄存器对写 (0x48/0x49)     |
 * |39 | xadc_avg_ch1_get     | FUN_0000d7bc    | 0x0000d7bc | Averaging CH1 寄存器对读 (0x4E/0x4F)    |
 * |40 | xadc_alarm_thr_set   | FUN_0000d884    | 0x0000d884 | 报警阈值 0x50..0x5F 写                  |
 * |41 | xadc_cfg0_flags_get  | FUN_0000dc4c    | 0x0000dc4c | CFR0 bit[10:9] 标志读                   |
 * |42 | efuse_program_row_pair | FUN_0000e7c4  | 0x0000e7c4 | eFuse 行对编程+blank check              |
 */
