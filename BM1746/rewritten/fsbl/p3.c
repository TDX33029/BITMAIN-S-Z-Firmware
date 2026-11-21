/**
 * p3.c — FSBL 反编译转写 批次 3/4
 *
 * 来源二进制: parts/Zynq7007_miner.elf (RAW 加载于 0x0, 反编译地址 = 文件偏移)
 * 覆盖 (reversed/fsbl_funcs/, 共 42 函数):
 *   - 异常向量: Reset, DataAbort
 *   - 调试/运行时: dbg_print (注册式打印回调 + TX 忙自旋), exit_run_handlers
 *     (newlib 风格 atexit/on_exit 处理器链遍历)
 *   - NAND (XNandPs ONFI 探测): nand_reg80_get / nand_timing_cfg_init /
 *     nand_onfi_probe ("ONFI" 签名 + 256B 参数页 CRC16(0x8005, init 0x4F4E)
 *     校验 + 几何参数提取 + 特性检查)
 *   - FatFs 文件系统核心: fat_get_fat / fat_remove_chain / fat_dir_next /
 *     fat_dir_sdi0 / fat_f_write / fat_f_closedir / fat_f_mkdir
 *   - FatFs 磁盘移植层: disk_read / disk_ioctl
 *   - SD/eMMC 主机层: sd_slot_cfg_get / sd_host_init / sd_card_select /
 *     sd_data_read / sd_send_scr / sdmmc_switch_high_speed / mmc_send_ext_csd
 *   - RSA 软件验签: rsa_bignum_sub (64 字大数减法), rsa_modexp_u32exp
 *     (平方-乘积模幂, 32 位指数 MSB 在前)
 *   - SHA-256: sha256_update / sha256_transform
 *   - eFuse (XAPP1280 流程): efuse_setup_timing / efuse_addr_is_reserved /
 *     efuse_write_bit / efuse_program_bit
 *   - XAdcPs (Zynq XADC): xadc_read_channels / xadc_selftest / xadc_cfg2_get /
 *     xadc_cfg0_field_get / xadc_set_single_chan_params /
 *     xadc_cfg2_divider_get / xadc_seq_ch1_get / xadc_avg_ch1_set
 *   - 系统时钟: cpu_freq_detect (ARM/IO PLL FDIV → 定时器分频标定)
 *   - libc: strchr (0xfefefeff/0x01010101 字扫描优化版)
 *
 * FATFS / FIL / DIR 结构布局 (全部为反编译实证偏移, 与 p4.c 一致):
 *   FATFS: +0x00 fs_type(1=FAT16,2=FAT12,3=FAT32) | +0x01 drv | +0x03 wflag(窗口脏)
 *          +0x04 标志(FSINFO 脏) | +0x08 n_rootdir(W) | +0x0A csize(W)
 *          +0x10 free_clst | +0x14 n_fatent | +0x20 fatbase | +0x24 dirbase
 *          +0x28 database | +0x2C winsect | +0x40 win[512]
 *   FIL:   +0x00 fs | +0x08 sclust | +0x0C fsize | +0x10 flag(B) | +0x11 err(B)
 *          +0x14 fptr | +0x18 clust | +0x1C dsect | +0x40 buf[512]
 *   DIR:   +0x00 fs | +0x04 id(W) | +0x06 属性(B, 本 FSBL 变体自定义) | +0x08 sclust
 *          +0x10 index(目录内字节偏移) | +0x14 clust | +0x18 sect | +0x1C dir(窗口内指针)
 *
 * 全局数据 (均为反编译实证, 语义推断处已注明):
 *   DAT_00028e84 = Xil Assert 风格错误标志 (0 = 正常, 1 = 已触发; 与 p4.c 的
 *                  g_assert_failed 宏同一地址 0x28E84)
 *   DAT_00021540 = dbg_print 输出处理函数指针 (NULL = 未注册, 不打印)
 *   DAT_00019704 = 发送忙标志 (非 0 时 dbg_print 自旋等待清零)
 *   DAT_00015b74 = 退出处理器链基址对象 (+0x148 为链头指针)
 *   DAT_0001971c / DAT_00019728 / DAT_00019740 / DAT_0001974c / DAT_00019710 =
 *                  NAND ONFI 参数页导出的时序/纠错配置字 (nand_onfi_probe 写入)
 *   DAT_00021590 = 每槽"块寻址"标志 (disk_read 按槽 0x220 步进取 +0x30)
 *   DAT_000215c4 = 每槽扇区总数 (disk_ioctl 按槽 0x220 步进取 +0x64)
 *
 * 调用方可见函数名一律取自 rewritten/name_map/fsbl.tsv (阶段 1 冻结)。
 * 注: p4.c 将 FUN_0000dfb0 声明为 str_chr, 本文件按 name_map 冻结名 strchr 定义。
 */
#include <stdint.h>
#include <stdbool.h>   /* bool (disk_write 返回类型) */

/* ==================== 共享声明 (跨批被调, 签名与反编译/兄弟批次一致) ==================== */
extern void mem_copy(void *dst, const void *src, uint32_t n);   /* FUN_00002200 (memcpy), p1 */
extern void halt_baddata(void);                                  /* 反编译伪调用 (损坏指令流) */
extern int  nand_status_get(int inst);                           /* FUN_00001554, p1 */
extern uint32_t fat_move_window(uint8_t *fs, uint32_t sect);     /* FUN_00002f3c, p4 */
extern int  fatfs_validate(uint32_t *fp, uint32_t *fs_out);      /* FUN_00002bbc, p4 */
extern int  fat_find_volume(const uint8_t **pp, uint8_t **fs_out, uint32_t mode); /* FUN_000030bc, p4 */
extern int  fat_follow_path(int *dj, const uint8_t *path);       /* FUN_00003f2c, p1 */
extern int  fat_fs_sync(uint8_t *fs);                            /* FUN_00002e20, p4 */
extern int  fat_dir_register(int *dj);                           /* FUN_00003e38, p4 */
extern int  fat_dir_sdi(int *dj, int stretch);                   /* FUN_00003b44, p4 */
extern int  fat_put_fat(uint8_t *fs, uint32_t clst, uint32_t val); /* FUN_0000351c, p1 */
extern uint32_t fat_create_chain(int *obj, uint32_t clst);       /* FUN_00003868, p1 */
extern int  fat_dir_seek(uint8_t *fs, uint32_t clst);            /* FUN_00002d5c, p1 (新簇清零) */
extern uint8_t disk_status(int pdrv);                             /* FUN_000064b8, p4 (返回状态槽值) */
extern bool disk_write(int pdrv, const void *buf, uint32_t sect, int cnt); /* FUN_0000676c, p4 */
extern uint16_t sd_send_cmd(int sd, uint32_t cmd_enc, uint32_t arg, uint16_t block_count); /* FUN_00006bcc, p2 */
extern void sd_adma_setup(int sd, int nblocks, void *buf);       /* FUN_000071f8, p1 */
extern void sd_adma_setup_cont(int sd, int nblocks);             /* FUN_00007100, p2 */
extern void mem_set(void *dst, int val, uint32_t n);             /* FUN_000027e0 (memset), p4 */
extern int  mmc_card_identify(int sd);                           /* FUN_00007574, p1 */
extern int  sdmmc_set_blocklen(int sd, uint32_t len);            /* FUN_00007d40, p2 */
extern int  sd_switch_func_check(int sd, void *buf);             /* FUN_00008168, p2 */
extern int  sd_set_clk_freq(int sd, uint32_t freq_hz);           /* FUN_000082b0, p4 */
extern int  sd_set_bus_width(int sd);                            /* FUN_00007f5c, p2 */
extern int  mmc_switch_cmd6(int sd, uint32_t arg);               /* FUN_00008994, p2 */
extern int  rsa_bignum_cmp(const uint32_t *a, const uint32_t *b); /* FUN_00008df8, p2 */
extern void rsa_mont_mul(uint32_t *dst, const uint32_t *a, const uint32_t *b,
                         const uint32_t *mod, uint32_t n0inv);        /* FUN_00008a78, p4 (b4) */
extern uint32_t rsa_inv_mod2p32(uint32_t m0);                         /* FUN_00008a00, p4 (b4) */
extern void sha256_state_reset(int ctx);                              /* FUN_00009974, p2 */
extern void sha256_transform(uint32_t *ctx, const uint32_t w_in[16]); /* FUN_000099a0, 见本文件 */
extern int  xadc_seq_mode_get(int xadc);                         /* FUN_0000ceb8, p2 */
extern uint32_t xadc_chan_data_get(int xadc, uint32_t chan);     /* FUN_0000c6dc, p1b */
extern uint32_t xadc_supply_data_get(int xadc, uint32_t chan);   /* FUN_0000c888, p2b */
extern void xadc_cfg2_pulse(int xadc);                           /* FUN_0000c654, p1b */
extern void xadc_alarm_thr_set(int xadc, uint32_t thr_sel, uint32_t value); /* FUN_0000d884, p4 */
extern uint32_t xadc_alarm_thr_get(int xadc, uint32_t sel);      /* FUN_0000d938, p2b */
extern uint32_t DAT_00028f50;                                    /* 全局, 语义待定: 定时器 tick 换算系数 (p2b 引用) */
extern int  timeout_expired(uint32_t deadline_lo, uint32_t deadline_hi); /* FUN_0000bd28, p4 */
extern void timeout_deadline_set(int64_t *deadline, uint32_t scale, uint32_t cycles, int hi); /* FUN_0000bcd8, p1 */
extern int  global_timer_counter_read(void);                     /* FUN_0000b7e0, p1 */
extern void delay_ms(uint32_t ms);                               /* func_0xffffe2ac: ROM/BSP 毫秒延时, p4 */

/* 缓存维护类外部例程 (Ghidra 地址在反编译范围外, 命名待补; 以 (缓冲, 长度) 调用,
 * 推断为 D-Cache 维护, 语义按调用位置注记) */
extern void fun_0xffffd23c(void *buf, uint32_t len);             /* 读缓冲方向 (DMA 读前/后) */
extern void fun_0xffffd370(void *buf, uint32_t len);             /* CMD6 切换读缓冲方向 */

/* ---- 本文件内定义、但被前段引用的函数前向声明 ---- */
extern int      disk_read(int pdrv, void *buf, uint32_t sect, int cnt);  /* FUN_0000667c, 见下 */
extern int      sd_data_read(uint8_t *slot, uint32_t sect, uint32_t cnt, void *buf); /* FUN_000072c8, 见下 */
extern uint32_t fat_default_date(void);                          /* FUN_00006760, 见下 */

/* NAND 命令发送例程 (Ghidra 地址 0xfffff540, 在反编译范围外, 命名待补)。
 * 实参形态有两种 (3 参 / 4 参), 以可变参数声明同时容纳两种调用形态。 */
extern void fun_0xfffff540(int inst, uint32_t cmd_desc, uint32_t mask, ...);

/* ---- Xil Assert 风格错误标志 (0x28E84, 与 p4.c g_assert_failed 同址) ---- */
#define g_assert_failed   (*(volatile uint32_t *)0x00028e84)

/* ---- dbg_print 全局 (地址常量来自反编译字面量) ---- */
#define g_dbg_print_fn    (*(volatile uint32_t *)0x00021540)     /* 原名 DAT_00021540 */
#define g_dbg_tx_busy     (*(volatile uint32_t *)0x00019704)     /* 原名 DAT_00019704 */

/* ---- NAND ONFI 配置字 (原名 DAT_000197xx, nand_onfi_probe 写入) ---- */
#define g_nand_ecc_hi_nib (*(volatile uint32_t *)0x0001974c)     /* 原名 DAT_0001974c */
#define g_nand_ecc_lo_nib (*(volatile uint32_t *)0x00019728)     /* 原名 DAT_00019728 */
#define g_nand_ecc_sum    (*(volatile uint32_t *)0x00019740)     /* 原名 DAT_00019740 */
#define g_nand_ecc_hi_cpy (*(volatile uint32_t *)0x0001971c)     /* 原名 DAT_0001971c */
#define g_nand_ecc_sum_cpy (*(volatile uint32_t *)0x00019710)    /* 原名 DAT_00019710 */

/* ---- disk 层每槽全局 (槽结构步进 0x220, 基址 0x21560) ---- */
#define g_slot_blockaddr  (*(volatile uint32_t *)0x00021590)     /* 原名 DAT_00021590 */
#define g_slot_sectcnt    (*(volatile uint32_t *)0x000215c4)     /* 原名 DAT_000215c4 */

/* ---- 退出处理器链基址 (原名 DAT_00015b74) ---- */
#define g_exit_chain_base (*(volatile uint32_t *)0x00015b74)

/* 通用寄存器读写 (地址常量来自反编译字面量, 与 p4.c 一致) */
static inline volatile uint32_t *reg32(uint32_t addr) { return (volatile uint32_t *)(uintptr_t)addr; }

/* ==================== 异常向量 ==================== */

// === Reset === 原 FUN_00000000 @ 0x00000000
// 来源: reversed/fsbl_funcs/Reset.c | 分类: 向量表 (passthrough 原名)
// 语义: 复位向量入口 (向量表第 0 项)。反编译产物受损 (bad instruction 截断):
// 可辨识内容为以寄存器携带实参 (in_stack_00000008 + unaff_r7) 调用
// memcpy(FUN_00002200), 其余参数与长度位于损坏指令流之后, 不可恢复;
// 随后进入 halt_baddata。按反编译原样保留, 不虚构恢复语义。
void Reset(void)
{
    extern void mem_copy(void *dst, const void *src, uint32_t n); /* FUN_00002200 */

    /* mem_copy((void *)(uintptr_t)(in_stack_00000008 + unaff_r7), <参数不可恢复>); */
    halt_baddata();
}

// === DataAbort === 原 FUN_00000010 @ 0x00000010
// 来源: reversed/fsbl_funcs/DataAbort.c | 分类: 向量表 (passthrough 原名)
// 语义: 数据中止异常入口 (向量表第 4 项)。反编译受损截断: 仅可辨识对
// memcpy(FUN_00002200) 的调用, 之后 halt_baddata 挂死。
void DataAbort(void)
{
    extern void mem_copy(void *dst, const void *src, uint32_t n); /* FUN_00002200 */

    /* FUN_00002200(<参数不可恢复>); — 损坏指令流之后被截断 */
    halt_baddata();
}

/* ==================== 调试/运行时 ==================== */

// === dbg_print === 原 FUN_000014e4 @ 0x000014e4
// 来源: reversed/fsbl_funcs/FUN_000014e4.c | 分类: fsbl
// 语义: 注册式调试打印。若全局输出函数指针 (DAT_00021540) 非空则经其输出
// (r0/r1 透传实参: 打印对象地址 + 行/错误码); 随后自旋等待发送忙标志
// (DAT_00019704) 清零。Ghidra 将与向量表 Reset(=地址 0) 的比较还原为
// NULL 判定, 此处保持该语义。
void dbg_print(uint32_t msg, uint32_t line)
{
    if (g_dbg_print_fn == 0u) {
        /* 处理器未注册 (=Reset 向量地址 0): 跳过输出 */
    } else {
        void (*fn)(uint32_t, uint32_t) = (void (*)(uint32_t, uint32_t))(uintptr_t)g_dbg_print_fn;
        fn(msg, line);                       /* 原反编译为 (*DAT_00021540)(), r0/r1 透传 */
    }
    do {
        if (g_dbg_tx_busy == 0u) {
            return;
        }
    } while (g_dbg_tx_busy != 0u);
    return;
}

/* ==================== NAND (XNandPs 风格) ==================== */

// === nand_reg80_get === 原 FUN_00000c1c @ 0x00000c1c
// 来源: reversed/fsbl_funcs/FUN_00000c1c.c | 分类: fsbl
// 语义: NAND 实例寄存器 +0x80 读取 (带 Xil Assert 式校验): 实例指针为空或
// 签名字段 (实例+8) 不为 0x11111111 时, dbg_print 报 0x252/0x253 并置
// g_assert_failed, 返回 0; 校验通过返回 *(实例+4 指向对象 +0x80)。
// dbg_print 首参 0x17214 为 Ghidra 误标字符串表 (实为 0x0C028Axx 指针表) 内地址。
uint32_t nand_reg80_get(int inst)
{
    if (inst == 0) {
        dbg_print(0x17214u, 0x252u);         /* s_0___P___p_0001720a + 10 */
        g_assert_failed = 1u;
        return 0u;
    }
    g_assert_failed = 0u;
    if (*(volatile int *)(uintptr_t)(inst + 8) == (int)0x11111111u) {
        return *(volatile uint32_t *)(uintptr_t)(*(volatile int *)(uintptr_t)(inst + 4) + 0x80);
    }
    dbg_print(0x17214u, 0x253u);
    g_assert_failed = 1u;
    return 0u;
}

// === nand_timing_cfg_init === 原 FUN_000019fc @ 0x000019fc
// 来源: reversed/fsbl_funcs/FUN_000019fc.c | 分类: fsbl
// 语义: NAND 控制器时序配置块初始化: 按反编译顺序写 10 个字字段
// (掩码 0x3FFF/时序步进值 0x28B/0x128 等, 各字段语义待定), 保持原写序
// (偏移 0 先写 0x28、再写 3、最后写 0x128)。
void nand_timing_cfg_init(uint32_t *cfg)
{
    cfg[3] = 0x3fffu;
    cfg[0] = 0x28u;
    cfg[0] = 3u;
    cfg[5] = 0x3fffu;
    cfg[1] = 0u;
    cfg[8] = 0x20u;
    cfg[0x11] = 0x20u;
    cfg[7] = 0u;
    cfg[6] = 0x28bu;
    cfg[0xd] = 0xfu;
    cfg[0] = 0x128u;
}

// === nand_onfi_probe === 原 FUN_0000158c @ 0x0000158c
// 来源: reversed/fsbl_funcs/FUN_0000158c.c | 分类: fsbl
// 语义: ONFI 参数页探测。发命令序列 (经外部例程 fun_0xfffff540, 描述符指针
// 依次取自 0x19768/0x19750/0x1975C/0x19780/0x19774 字面量), 每步等待状态
// 就绪 (nand_status_get bit6)。读 4 字节签名须为 "ONFI"; 随后按总线位宽
// (实例+0x10 == 0x10 为 16 位) 从数据口 (实例+100 指向的 FIFO 地址) 连读
// 256 字节参数页, 以 CRC16(多项式 0x8005, 初值 0x4F4E, 覆盖字节 0..253)
// 校验页内 CRC (字节 254..255), 失败重试至多 3 次。校验通过后提取几何参数
// 写入实例 (+0x18 每页字节/+0x1C 半页字节/+0x20 每 LUN 块数/+0x24 LUN 数/
// +0x28/+0x30/+0x38/+0x40 及 64 位总容量 +0x48/+0x4C) 和全局 NAND 配置字
// (DAT_000197xx), 再按特性/厂商魔数选择 ECC 模式 (实例+0x6ECC = 0/2/3),
// 依参数页特性 bit0 (16 位总线支持) 设定实例+0x29 (8/0x10), 将参数页可选
// 命令 bit0/bit1 存入实例+0x58/+0x5C。失败返回 1, 成功返回 0 (特性路径
// 返回参数页特性 bit0, 恒 0)。
uint16_t nand_onfi_probe(int inst)
{
    uint8_t  sig[4];        /* 原名 local_134: 签名暂存 */
    uint8_t  page[256];     /* 原名 local_128..abStack_28: ONFI 参数页 */
    uint8_t  feat[4];       /* 原名 local_12c[4]: Set Features 参数读回 */
    uint32_t feat_arg = 0x00000008u;         /* 原名 local_130 = 8, 附加零字节构成 08 00 00 00 */
    uint16_t crc_calc;                       /* 名义 local_*: 计算所得 CRC */
    uint16_t crc_stored;                     /* 原名 local_2a: 页内字节 254..255 */
    uint16_t features16;                     /* 原名 local_122: 参数页字节 6..7 (特性) */
    uint16_t opt_cmd16;                      /* 原名 local_120: 参数页字节 8..9 (可选命令) */
    uint32_t bytes_per_page;                 /* 原名 local_d8: 参数页字节 80..83 */
    uint16_t part_page;                      /* 原名 local_d4: 参数页字节 84..85 */
    uint32_t blocks_per_lun;                 /* 原名 local_cc: 参数页字节 92..95 */
    uint32_t lun_cnt;                        /* 原名 local_c8: 参数页字节 96..97 */
    uint8_t  param100;                       /* 原名 local_c4: 参数页字节 100 */
    uint8_t  param101;                       /* 原名 local_c3: 参数页字节 101 */
    uint64_t total_size;                     /* 原名 lVar1: 64 位器件容量 */
    uint32_t pages_total;                    /* 名义 uVar3: 页总数计算值 */
    uint32_t status;
    uint16_t rd0, rd1;                       /* 名义 uVar5/uVar8: 特性门控两次口读 */
    int      bus16;                          /* 原名 iVar4: 实例+0x10 == 0x10 */
    volatile uint8_t *port;                  /* 原名 *(param_1 + 100): 数据口 (读自动步进) */
    int      retry;
    int      i;
    uint16_t mask;
    const uint8_t *pb;
    uint8_t  *dst;

    feat[0] = 0; feat[1] = 0; feat[2] = 0; feat[3] = 0;   /* local_130 高位零字节语义 */

    if (inst == 0) {
        dbg_print(0x17230u, 0x1eeu);         /* s_0___P___p_0001720a + 0x26 */
        g_assert_failed = 1u;
        return 0u;
    }
    g_assert_failed = 0u;

    fun_0xfffff540(inst, 0x00019768u, 0xFFFFFFFFu);       /* DAT_000019b4 取值, 语义待定 */
    do {
        status = (uint32_t)nand_status_get(inst);
    } while ((status & 0x40u) == 0u);
    fun_0xfffff540(inst, 0x00019750u, 0xFFFFFFFFu, 0x20u);/* DAT_000019b8 取值 */
    bus16 = (*(volatile int *)(uintptr_t)(inst + 0x10) == 0x10);
    port = *(volatile uint8_t **)(uintptr_t)(inst + 100);
    for (i = 0; i < 4; i++) {
        sig[i] = bus16 ? (uint8_t)*(volatile uint16_t *)(uintptr_t)port
                       : *(volatile uint8_t *)(uintptr_t)port;
    }
    if ((sig[0] == 'O') && (sig[1] == 'N') && (sig[2] == 'F') && (sig[3] == 'I')) {
        fun_0xfffff540(inst, 0x0001975Cu, 0xFFFFFFFFu, 0u);  /* DAT_000019bc 取值 */
        do {
            status = (uint32_t)nand_status_get(inst);
        } while ((status & 0x40u) == 0u);
        retry = 3;
        *(volatile uint32_t *)(uintptr_t)(*(volatile int *)(uintptr_t)(inst + 0xC)) = 0u;
        do {
            dst = page;
            for (i = 0; i < 256; i++) {
                *dst++ = bus16 ? (uint8_t)*(volatile uint16_t *)(uintptr_t)port
                               : *(volatile uint8_t *)(uintptr_t)port;
            }
            /* ONFI CRC16: 初值 0x4F4E, 多项式 0x8005, MSB 在前, 覆盖页字节 0..253 */
            crc_calc = 0x4F4Eu;
            for (pb = &page[0]; pb != &page[254]; pb++) {
                for (mask = 0x80u; mask != 0u; mask >>= 1) {
                    uint16_t cond = ((pb[0] & mask) == 0u) ? crc_calc : (uint16_t)~crc_calc;
                    crc_calc = (uint16_t)(crc_calc << 1);
                    if ((cond & 0x8000u) != 0u) {
                        crc_calc ^= 0x8005u;
                    }
                }
            }
            crc_stored = (uint16_t)((uint16_t)page[254] | ((uint16_t)page[255] << 8));
            if (crc_calc == crc_stored) {
                /* ---- 几何参数提取 (ONFI 参数页偏移) ---- */
                bytes_per_page = (uint32_t)page[80] | ((uint32_t)page[81] << 8) |
                                 ((uint32_t)page[82] << 16) | ((uint32_t)page[83] << 24);
                part_page      = (uint16_t)((uint16_t)page[84] | ((uint16_t)page[85] << 8));
                blocks_per_lun = (uint32_t)page[92] | ((uint32_t)page[93] << 8) |
                                 ((uint32_t)page[94] << 16) | ((uint32_t)page[95] << 24);
                lun_cnt        = (uint32_t)((uint16_t)page[96] | ((uint16_t)page[97] << 8));
                param100       = page[100];   /* 语义待定: 页总数计算用 */
                param101       = page[101];   /* 语义待定: 高/低半字节 → NAND 配置字 */
                features16     = (uint16_t)((uint16_t)page[6] | ((uint16_t)page[7] << 8));
                opt_cmd16      = (uint16_t)((uint16_t)page[8] | ((uint16_t)page[9] << 8));

                *(volatile uint8_t *)(uintptr_t)(inst + 0x28) = param100;
                g_nand_ecc_hi_nib = (uint32_t)(param101 >> 4);
                g_nand_ecc_lo_nib = (uint32_t)(param101 & 0xfu);
                *(volatile uint32_t *)(uintptr_t)(inst + 0x20) = blocks_per_lun;
                g_nand_ecc_hi_cpy = g_nand_ecc_hi_nib;
                *(volatile uint16_t *)(uintptr_t)(inst + 0x1C) = part_page;
                *(volatile uint32_t *)(uintptr_t)(inst + 0x18) = bytes_per_page;
                *(volatile uint32_t *)(uintptr_t)(inst + 0x34) = 0u;
                *(volatile uint32_t *)(uintptr_t)(inst + 0x3C) = 0u;
                *(volatile int *)(uintptr_t)(inst + 0x24) = (int)lun_cnt;
                pages_total = lun_cnt * (uint32_t)param100;
                *(volatile uint32_t *)(uintptr_t)(inst + 0x44) = 0u;
                *(volatile uint8_t *)(uintptr_t)(inst + 0x50) = (uint8_t)g_nand_ecc_lo_nib;
                g_nand_ecc_sum = g_nand_ecc_lo_nib + g_nand_ecc_hi_nib;
                *(volatile uint8_t *)(uintptr_t)(inst + 0x51) = (uint8_t)g_nand_ecc_hi_nib;
                total_size = (uint64_t)pages_total * ((uint64_t)bytes_per_page * (uint64_t)blocks_per_lun);
                *(volatile uint32_t *)(uintptr_t)(inst + 0x38) = pages_total;
                g_nand_ecc_sum_cpy = g_nand_ecc_sum;
                *(volatile uint32_t *)(uintptr_t)(inst + 0x30) = pages_total * blocks_per_lun;
                *(volatile uint32_t *)(uintptr_t)(inst + 0x40) = bytes_per_page * blocks_per_lun;
                *(volatile int32_t *)(uintptr_t)(inst + 0x48) = (int32_t)(uint32_t)total_size;
                *(volatile uint32_t *)(uintptr_t)(inst + 0x4C) =
                    pages_total * (uint32_t)(((uint64_t)bytes_per_page * (uint64_t)blocks_per_lun) >> 32) +
                    (uint32_t)(total_size >> 32);
                fun_0xfffff540(inst, 0x00019750u, 0xFFFFFFFFu, 0u);  /* 同 DAT_000019b8 描述符 */

                /* 特性门控: 两次独立口读, 首字节须 0x2C, 次字节按厂商魔数判定 */
                if (bus16) {
                    rd0 = *(volatile uint16_t *)(uintptr_t)port;
                    rd1 = rd0;
                } else {
                    rd0 = (uint16_t)*(volatile uint8_t *)(uintptr_t)port;
                    rd1 = rd0;
                }
                if (((rd0 & 0xffu) == 0x2cu) &&
                    (((rd1 & 0xefu) == 0xc3u) ||
                     (((uint16_t)(rd1 + 0x36u) & 0xfdu) == 0u) ||
                     ((rd1 & 0xffu) == 0xf1u) ||
                     ((rd1 & 0xedu) == 0xa1u) ||
                     (((uint16_t)(rd1 + 0x56u) & 0xfdu) == 0u))) {
                    fun_0xfffff540(inst, 0x00019780u, 0xFFFFFFFFu, 0x90u);  /* DAT_000019c0 取值 */
                    /* 写 Set Features 参数序列: 0x08 0x00 0x00 0x00
                       (feat_arg 低字节 0x08 起头, 其后 3 个零字节来自 feat_arg 高位) */
                    {
                        const uint8_t *src = (const uint8_t *)(uintptr_t)&feat_arg + 1;
                        uint16_t out = (uint16_t)(feat_arg & 0xffu);
                        for (;;) {
                            if (bus16) {
                                *(volatile uint16_t *)(uintptr_t)port = out;
                            } else {
                                *(volatile uint8_t *)(uintptr_t)port = (uint8_t)out;
                            }
                            if (src == &feat[0]) {
                                break;
                            }
                            out = (uint16_t)*src;
                            src++;
                        }
                    }
                    do {
                        status = (uint32_t)nand_status_get(inst);
                    } while ((status & 0x40u) == 0u);
                    fun_0xfffff540(inst, 0x00019774u, 0xFFFFFFFFu, 0x90u);  /* DAT_000019c4 取值 */
                    do {
                        status = (uint32_t)nand_status_get(inst);
                    } while ((status & 0x40u) == 0u);
                    *(volatile uint32_t *)(uintptr_t)(*(volatile int *)(uintptr_t)(inst + 0xC)) = 0u;
                    for (i = 0; i < 4; i++) {
                        feat[i] = bus16 ? (uint8_t)*(volatile uint16_t *)(uintptr_t)port
                                        : *(volatile uint8_t *)(uintptr_t)port;
                    }
                    if ((feat[0] & 8u) != 0u) {
                        *(volatile uint8_t *)(uintptr_t)(inst + 0x6ECC) = 3u;
                        if ((features16 & 1u) == 0u) {
                            *(volatile uint8_t *)(uintptr_t)(inst + 0x29) = 8u;
                            return (uint16_t)(features16 & 1u);
                        }
                        *(volatile uint8_t *)(uintptr_t)(inst + 0x29) = 0x10u;
                        return 0u;
                    }
                    /* LAB_00001968 */
                    *(volatile uint8_t *)(uintptr_t)(inst + 0x6ECC) = 2u;
                } else {
                    if ((bytes_per_page - 0x200u) < 0x601u) {
                        /* 每页字节 ∈ [0x200, 0x800]: 同 LAB_00001968, ECC 模式 2 */
                        *(volatile uint8_t *)(uintptr_t)(inst + 0x6ECC) = 2u;
                    } else {
                        *(volatile uint8_t *)(uintptr_t)(inst + 0x6ECC) = 0u;
                    }
                }
                if ((features16 & 1u) == 0u) {
                    *(volatile uint8_t *)(uintptr_t)(inst + 0x29) = 8u;
                } else {
                    *(volatile uint8_t *)(uintptr_t)(inst + 0x29) = 0x10u;
                }
                *(volatile uint32_t *)(uintptr_t)(inst + 0x58) = (uint32_t)(features16 & 1u);
                *(volatile uint32_t *)(uintptr_t)(inst + 0x5C) = (uint32_t)((features16 & 3u) >> 1);
                return 0u;
            }
            retry--;
        } while (retry != 0);
    }
    return 1u;
}

/* ==================== newlib 风格退出处理器 ==================== */

// === exit_run_handlers === 原 FUN_000029dc @ 0x000029dc
// 来源: reversed/fsbl_funcs/FUN_000029dc.c | 分类: fsbl
// 语义: newlib _call_exitprocs 风格退出处理器遍历。链头位于
// DAT_00015b74+0x148; 处理器块布局 (反编译实证偏移): +0x04 登记计数,
// +0x08.. 函数表[32], +0x88.. 实参表[32], +0x108.. 注册键表[32],
// +0x188 调用方式位图 (bit i: on_exit 带参), +0x18C 任务变量位图。
// 从栈顶计数向下逐项注销 (末项直接递减计数, 其余置空函数指针), 函数指针
// 与 Reset 向量 (=NULL) 相同则跳过; 调用方式: 无带参位 → fn(code, code 可为
// 前一处理器返回值的透传, 反编译原样), 仅带参位 → fn(code), 两者皆置 →
// fn(实参表[i]); 实参键 (param_2) 非 0 时仅处理键匹配项。每次调用后若计数
// 或链头变化则重启遍历。Ghidra 已删除 0x2AD4..0x2B28 处不可达块。
void exit_run_handlers(uint32_t code, uint32_t key)
{
    uint32_t *base = (uint32_t *)(uintptr_t)g_exit_chain_base;
    uint32_t *head_slot = base + (0x148u / 4u);
    uint32_t carry;                       /* 名义 uVar4: 透传 code (可为处理器返回值) */
    uint32_t *blk;
    uint32_t *entry;
    uint32_t idx;
    uint32_t ind_before;
    uint32_t bit;
    uint32_t (*fn)(uint32_t);

    carry = code;
restart:
    blk = (uint32_t *)(uintptr_t)*head_slot;
    if (blk != NULL) {
        idx = *(volatile int32_t *)(uintptr_t)(blk + 1) - 1;
        if ((int32_t)idx >= 0) {
            entry = blk + (*(volatile int32_t *)(uintptr_t)(blk + 1) + 1);
            do {
                if ((key == 0u) || (entry[0x40] == key)) {
                    fn = (uint32_t (*)(uint32_t))(uintptr_t)entry[0];
                    if ((uint32_t)(*(volatile int32_t *)(uintptr_t)(blk + 1) - 1u) == idx) {
                        *(volatile int32_t *)(uintptr_t)(blk + 1) = (int32_t)idx;
                    } else {
                        entry[0] = 0u;
                    }
                    if (fn != (uint32_t (*)(uint32_t))0u) {  /* != Reset 向量 (=NULL) */
                        bit = 1u << (idx & 0xffu);
                        ind_before = *(volatile int32_t *)(uintptr_t)(blk + 1);
                        if ((bit & *(volatile uint32_t *)(uintptr_t)(blk + (0x188u / 4u))) == 0u) {
                            carry = fn(carry);              /* atexit 型: 透传 code */
                        } else if ((bit & *(volatile uint32_t *)(uintptr_t)(blk + (0x18Cu / 4u))) == 0u) {
                            carry = fn(code);               /* on_exit 型: 原 code */
                        } else {
                            carry = fn(entry[0x20]);        /* 任务变量型: 实参表[i] */
                        }
                        if ((*(volatile int32_t *)(uintptr_t)(blk + 1) != (int32_t)ind_before) ||
                            (*head_slot != (uint32_t)(uintptr_t)blk)) {
                            goto restart;                   /* LAB_00002a10: 链变化, 重启遍历 */
                        }
                    }
                }
                if (idx == 0u) {
                    return;
                }
                entry--;
                idx--;
            } while (1);
        }
    }
    return;
}

/* ==================== FatFs 文件系统核心 ==================== */

// === fat_get_fat === 原 FUN_000036fc @ 0x000036fc
// 来源: reversed/fsbl_funcs/FUN_000036fc.c | 分类: fsbl
// 语义: FatFs get_fat — 读取 FAT 表项 (下一簇号)。簇号 <2 或 >= n_fatent
// (fs+0x14) 返回 1; 按卷类型 (fs+0x00: 1=FAT16, 2=FAT12, 3=FAT32) 定位
// fatbase(fs+0x20)+簇偏移所在扇区, 窗口扇区不符时经 fat_move_window 读入
// (Ghidra 丢失实参, 按语义补 (fs, 目标扇区)); 从窗口 fs+0x40 取值:
// FAT12 半字节拼接, FAT16 双字节小端, FAT32 取低 28 位; 窗口操作失败返回
// 0xFFFFFFFF (FR_DISK_ERR 语义)。
int fat_get_fat(uint8_t *fs, uint32_t clst)
{
    uint32_t sect;
    uint32_t ofs;
    uint8_t  b0;
    uint32_t val;

    if (clst < 2u) {
        return 1;
    }
    if (*(volatile uint32_t *)(uintptr_t)(fs + 0x14) <= clst) {
        return 1;
    }
    switch (*(volatile uint8_t *)(uintptr_t)fs) {
    case 2:                                    /* FAT12 */
        /* wt = clst * 3 / 2; 目标扇区 = fatbase + wt/512 */
        ofs = clst + (clst >> 1);
        sect = *(volatile int32_t *)(uintptr_t)(fs + 0x20) + (ofs >> 9);
        if ((*(volatile int32_t *)(uintptr_t)(fs + 0x20) + (ofs >> 9)) !=
            *(volatile int32_t *)(uintptr_t)(fs + 0x2C)) {
            if (fat_move_window(fs, sect) != 0u) {
                return (int)0xFFFFFFFFu;
            }
        }
        b0 = fs[(ofs & 0x1ffu) + 0x40u];
        sect = *(volatile int32_t *)(uintptr_t)(fs + 0x20) + ((ofs + 1u) >> 9);
        if (((*(volatile int32_t *)(uintptr_t)(fs + 0x20) + ((ofs + 1u) >> 9)) !=
             *(volatile int32_t *)(uintptr_t)(fs + 0x2C)) ||
            (fat_move_window(fs, sect) != 0u)) {
            return (int)0xFFFFFFFFu;
        }
        if ((clst & 1u) == 0u) {
            val = ((uint32_t)fs[((ofs + 1u) & 0x1ffu) + 0x40u] << 8) | b0;
            return (int)(val & 0xfffu);
        }
        val = ((uint32_t)fs[((ofs + 1u) & 0x1ffu) + 0x40u] << 8) | b0;
        return (int)((val >> 4) & 0xffffu);    /* 原反编译为 (ushort)(拼值 >> 4) */

    case 3:                                    /* FAT32 */
        ofs = (clst & 0x7fu) * 4u;
        sect = *(volatile int32_t *)(uintptr_t)(fs + 0x20) + (clst >> 7);
        if (((*(volatile int32_t *)(uintptr_t)(fs + 0x20) + (clst >> 7)) !=
             *(volatile int32_t *)(uintptr_t)(fs + 0x2C)) ||
            (fat_move_window(fs, sect) != 0u)) {
            return (int)0xFFFFFFFFu;
        }
        val = (uint32_t)fs[ofs + 0x40u] |
              ((uint32_t)fs[ofs + 0x41u] << 8) |
              ((uint32_t)fs[ofs + 0x42u] << 16);
        return (int)((val | ((uint32_t)fs[ofs + 0x43u] << 24)) & 0x0FFFFFFFu);

    case 1:                                    /* FAT16 */
        ofs = (clst & 0xffu) * 2u;
        sect = *(volatile int32_t *)(uintptr_t)(fs + 0x20) + (clst >> 8);
        if (((*(volatile int32_t *)(uintptr_t)(fs + 0x20) + (clst >> 8)) !=
             *(volatile int32_t *)(uintptr_t)(fs + 0x2C)) ||
            (fat_move_window(fs, sect) != 0u)) {
            return (int)0xFFFFFFFFu;
        }
        val = ((uint32_t)fs[ofs + 0x41u] << 8) | (uint32_t)fs[ofs + 0x40u];
        return (int)val;

    default:
        return 1;
    }
}

// === fat_remove_chain === 原 FUN_00003a48 @ 0x00003a48
// 来源: reversed/fsbl_funcs/FUN_00003a48.c | 分类: fsbl
// 语义: FatFs remove_chain — 释放簇链。clst<2 或 >= n_fatent(fs+0x14) 返回 2
// (FR_INT_ERR)。存在合法前驱 (0<prev<clst, prev!=1) 时先把前驱簇写为 EOC
// (0xFFFFFFFF, 终止旧链); 随后沿链向下: get_fat 取下一簇 (0=已空返回 0 成功,
// 1=内部错, 0xFFFFFFFF=磁盘错), 当前簇写 0 释放, free_clst(fs+0x10) 在
// 范围内时递增并置 fs+0x04 标志 bit0 (FSINFO 脏)。下一簇 >= n_fatent 时
// 结束返回 0。返回值 1 = FR_DISK_ERR, 2 = FR_INT_ERR。
int fat_remove_chain(int *dj, uint32_t clst, uint32_t free_clst)
{
    uint8_t *fs = (uint8_t *)(uintptr_t)*(volatile uint32_t **)(uintptr_t)dj;
    uint32_t nxt;
    int res;

    if (clst < 2u) {
        return 2;
    }
    if (clst >= *(volatile uint32_t *)(uintptr_t)(fs + 0x14)) {
        return 2;
    }
    if (free_clst != 0u && free_clst != 1u && free_clst < clst) {
        res = fat_put_fat(fs, free_clst, 0xFFFFFFFFu);   /* 前驱簇写 EOC */
        if (res != 0) {
            return res;
        }
        do {
            nxt = (uint32_t)fat_get_fat(fs, clst);
            if (nxt == 0u) {
                return 0;                    /* 链已空: 释放完成 */
            }
            if (nxt == 1u) {
                return 2;
            }
            if (nxt == 0xFFFFFFFFu) {
                return 1;
            }
            if (clst < 2u) {
                return 2;
            }
            if (*(volatile uint32_t *)(uintptr_t)(fs + 0x14) <= clst) {
                return 2;
            }
            res = fat_put_fat(fs, clst, 0u);  /* 当前簇置空 */
            if (res != 0) {
                return res;
            }
            if (*(volatile uint32_t *)(uintptr_t)(fs + 0x10) <
                *(volatile uint32_t *)(uintptr_t)(fs + 0x14) - 2u) {
                (*(volatile uint32_t *)(uintptr_t)(fs + 0x10))++;
                *(volatile uint8_t *)(uintptr_t)(fs + 4) |= 1u;
            }
            clst = nxt;
        } while (clst < *(volatile uint32_t *)(uintptr_t)(fs + 0x14));
        return 0;
    }
    return 2;
}

// === fat_dir_next === 原 FUN_00003c84 @ 0x00003c84
// 来源: reversed/fsbl_funcs/FUN_00003c84.c | 分类: fsbl
// 语义: 目录扫描步进 (dir_next/dir_read 变体): 从当前位置起跳过 LFN
// (attr&0x3F==0x0F)、删除项 (首字节 0xE5)、点项 ('.') 与卷标
// (attr&0x1F==AM_DIR 8), 返回第一个有效项; 同时把当前项属性写入 dj+0x06
// (本 FSBL 变体自定义字段)。窗口扇区 (fs+0x2C) 与 dj->sect (dj+0x18) 不符时
// 经 fat_move_window 换窗 (Ghidra 丢失实参, 按语义补 (fs, dj->sect))。
// 目录尾 (项首字节 0) 或步进越界返回 4 (FR_NO_FILE), 并清 dj->sect;
// 磁盘错误透传 (1), 成功返回 0。advance 经 fat_dir_sdi(dj, 0) — Ghidra
// 丢失第二实参, 按扫描语义补 0 (不延伸簇链)。
int fat_dir_next(int *dj)
{
    uint8_t *fs = (uint8_t *)(uintptr_t)*(volatile uint32_t **)(uintptr_t)dj;
    int res = 4;                              /* FR_NO_FILE 初值 */
    uint8_t c0;
    uint8_t attr;
    uint8_t attr_mask;

    for (;;) {
        if (dj[6] == 0) {                     /* dj->sect == 0: 已到尾/无效 */
            if (res == 0) {
                return 0;
            }
            dj[6] = 0;
            return res;
        }
        if (*(volatile int32_t *)(uintptr_t)(fs + 0x2C) != dj[6]) {
            res = (int)fat_move_window(fs, (uint32_t)dj[6]);
            if (res != 0) {
                dj[6] = 0;
                return res;
            }
        }
        {
            volatile uint8_t *dir = (volatile uint8_t *)(uintptr_t)dj[7];
            c0 = dir[0];
            if (c0 == 0u) {
                res = 4;                       /* 目录结束 */
                dj[6] = 0;
                return res;
            }
            attr = dir[0x0B];
            attr_mask = (uint8_t)(attr & 0x3fu);
            *(volatile uint8_t *)(uintptr_t)((uint8_t *)dj + 6) = attr_mask;
            if ((attr_mask != 0xfu) && (c0 != (uint8_t)0xE5) && (c0 != (uint8_t)'.') &&
                ((attr & 0x1fu) != 8u)) {
                return 0;                      /* 找到有效项 */
            }
        }
        res = fat_dir_sdi(dj, 0);
        if (res != 0) {
            dj[6] = 0;
            return res;
        }
    }
}

// === fat_dir_sdi0 === 原 FUN_00003d50 @ 0x00003d50
// 来源: reversed/fsbl_funcs/FUN_00003d50.c | 分类: fsbl
// 语义: dir_sdi(dj, 0) — 目录对象回卷到第一项。index (dj+0x10) 清零;
// FAT12/16 根目录 (sclust==0 且 fs_type<3 或 dirbase==0): 校验 n_rootdir
// (fs+0x08) 非 0, sect (dj+0x18) = dirbase (fs+0x24, 根目录起始扇区);
// FAT32 根 (sclust==0 且 dirbase!=0) 与子目录: 起始簇 = dirbase(作为簇号)
// 或 sclust, 簇号越界 (>= n_fatent-2) 返回 2, sect = database(fs+0x28) +
// (簇-2)*csize (fs+0x0A)。csize==0 的防御分支沿 FAT 链行走, 所有出口均
// 报错 (1/2)。clust (dj+0x14) = 起始簇, dir (dj+0x1C) = 窗口基址 fs+0x40;
// sect==0 返回 2, 否则返回 0。
int fat_dir_sdi0(int *dj)
{
    uint8_t *fs = (uint8_t *)(uintptr_t)*(volatile uint32_t **)(uintptr_t)dj;
    uint32_t clst = (uint32_t)dj[2];          /* dj->sclust */
    uint32_t walk;
    int32_t sect = 0;
    int dynamic = 0;

    dj[4] = 0;                                /* dj->index = 0 */
    walk = clst;
    if (clst == 0u) {
        if (*(volatile uint8_t *)(uintptr_t)fs >= 3u) {
            walk = *(volatile uint32_t *)(uintptr_t)(fs + 0x24);   /* FAT32: dirbase = 根簇号 */
            if (walk != 0u) {
                dynamic = 1;
            }
        }
        if (!dynamic) {
            /* FAT12/16 静态根目录 (或 FAT32 dirbase==0) */
            if (*(volatile int16_t *)(uintptr_t)(fs + 8) == 0) {
                return 2;                     /* n_rootdir == 0 */
            }
            sect = *(volatile int32_t *)(uintptr_t)(fs + 0x24);
            dj[6] = sect;
            walk = clst;
        }
    } else {
        dynamic = 1;
    }
    if (dynamic) {
        if (*(volatile uint16_t *)(uintptr_t)(fs + 0x0A) == 0u) {
            /* csize==0 防御分支: 沿链行走, 所有出口均为错误 */
            for (;;) {
                walk = (uint32_t)fat_get_fat(fs, walk);
                if (walk == 0xFFFFFFFFu) {
                    return 1;
                }
                if (walk < 2u) {
                    break;
                }
                if (*(volatile uint32_t *)(uintptr_t)(fs + 0x14) <= walk) {
                    return 2;
                }
            }
            return 2;
        }
        if ((*(volatile uint32_t *)(uintptr_t)(fs + 0x14) - 2u) <= (clst - 2u)) {
            dj[5] = (int32_t)clst;
            dj[6] = 0;
            return 2;                         /* 簇号越界 */
        }
        sect = (int32_t)((clst - 2u) * (uint32_t)*(volatile uint16_t *)(uintptr_t)(fs + 0x0A) +
                         *(volatile int32_t *)(uintptr_t)(fs + 0x28));
        dj[6] = sect;
    }
    dj[5] = (int32_t)walk;
    if (sect == 0) {
        return 2;
    }
    dj[7] = (int32_t)(uintptr_t)(fs + 0x40);  /* dj->dir = fs->win */
    return 0;
}

// === fat_f_write === 原 FUN_00004910 @ 0x00004910
// 来源: reversed/fsbl_funcs/FUN_00004910.c | 分类: fsbl
// 语义: FatFs f_write — 向文件写数据。*bw 清零; fatfs_validate 校验对象且
// err (fp+0x11) 为 0、flag (fp+0x10) 含 FA_WRITE(0x02), 否则透传校验码/
// err 码或返回 7 (FR_DENIED)。btw 以 0xFFFFFFFF-fptr 截断。主循环按扇区推进:
// 簇边界时经 fat_create_chain 分配/推进簇 (fp+0x18 clust, 首次写回 fp+0x08
// sclust); 脏扇区 (flag bit7) 先经 disk_write 回写; 整扇区直接多扇区
// disk_write (簇内余量截断), 写入范围覆盖缓存扇区 (fp+0x1C dsect) 时把
// 对应 512B 从写缓冲复制回 fp+0x40 窗口并清脏; 部分扇区则从扇区内偏移
// 起在窗口内改写并置脏 (先按需 disk_read 补读未缓存且位于文件内的扇区)。
// 尾部: fptr += 本次字节数, fsize (fp+0x0C) 取 max, *bw 累加, 循环直至
// btw 耗尽; 结束置 flag |= FA__WRITTEN(0x40)。磁盘错误置 err=1 返回 1,
// 内部错置 err=2 返回 2, 分配失败 (create_chain==0) 中断返回已写字节数。
uint32_t fat_f_write(uint8_t *fp, const uint32_t *buff, uint32_t btw, uint32_t *bw)
{
    uint32_t fs_ptr;                          /* 原名 local_2c[0]: fatfs_validate 输出 */
    uint8_t *fs;
    uint32_t res;
    uint32_t flag;
    uint32_t fptr;
    uint32_t csect;
    uint32_t cc;
    uint32_t csize;
    uint32_t sect;
    uint32_t clst;
    uint32_t fsize;
    int r;

    *bw = 0u;
    res = (uint32_t)fatfs_validate((uint32_t *)(uintptr_t)fp, &fs_ptr);
    fs = (uint8_t *)(uintptr_t)fs_ptr;
    if (res == 0u) {
        res = *(volatile uint8_t *)(uintptr_t)(fp + 0x11);
        if (res == 0u) {
            flag = *(volatile uint8_t *)(uintptr_t)(fp + 0x10);
            if ((flag & 2u) == 0u) {
                res = 7u;                     /* FR_DENIED */
            } else {
                fptr = *(volatile uint32_t *)(uintptr_t)(fp + 0x14);
                if (fptr + btw < fptr) {      /* CARRY4(fptr, btw): 32 位溢出截断 */
                    btw = ~fptr;
                }
                if (btw != 0u) {
                    do {
                        csect = fptr & 0x1ffu;
                        if (csect == 0u) {
                            csect = ((uint32_t)*(volatile uint16_t *)(uintptr_t)(fs + 0x0A) - 1u) &
                                    (fptr >> 9);
                            if (csect == 0u) {
                                if (fptr == 0u) {
                                    if (*(volatile int32_t *)(uintptr_t)(fp + 8) == 0) {
                                        clst = fat_create_chain((int *)(uintptr_t)fp, 0u);
                                        goto chk_clst;
                                    }
                                } else {
                                    clst = fat_create_chain((int *)(uintptr_t)fp,
                                        *(volatile uint32_t *)(uintptr_t)(fp + 0x18));
                                chk_clst:                                /* 原 LAB_00004ba0 */
                                    if (clst == 0u) {
                                        break;    /* 无空闲簇: 中断, 返回已写字节数 */
                                    }
                                }
                                if (clst == 1u) {
                                    *(volatile uint8_t *)(uintptr_t)(fp + 0x11) = 2u;
                                    return 2u;
                                }
                                if (clst == 0xFFFFFFFFu) {
                                    *(volatile uint8_t *)(uintptr_t)(fp + 0x11) = 1u;
                                    return 1u;
                                }
                                *(volatile uint32_t *)(uintptr_t)(fp + 0x18) = clst;
                                if (*(volatile int32_t *)(uintptr_t)(fp + 8) == 0) {
                                    *(volatile int32_t *)(uintptr_t)(fp + 8) = (int32_t)clst;
                                }
                            }
                            if (*(volatile int8_t *)(uintptr_t)(fp + 0x10) < 0) {
                                /* flag bit7: 缓存扇区脏, 回写后清脏 */
                                r = disk_write((int)*(volatile uint8_t *)(uintptr_t)(fs + 1),
                                               fp + 0x40,
                                               *(volatile uint32_t *)(uintptr_t)(fp + 0x1C), 1);
                                if (r != 0) {
                                    *(volatile uint8_t *)(uintptr_t)(fp + 0x11) = 1u;
                                    return 1u;
                                }
                                *(volatile uint8_t *)(uintptr_t)(fp + 0x10) =
                                    (uint8_t)(*(volatile uint8_t *)(uintptr_t)(fp + 0x10) & 0x7fu);
                            }
                            clst = (uint32_t)(*(volatile int32_t *)(uintptr_t)(fp + 0x18) - 2);
                            if ((uint32_t)(*(volatile int32_t *)(uintptr_t)(fs + 0x14) - 2) <= clst) {
                                *(volatile uint8_t *)(uintptr_t)(fp + 0x11) = 2u;
                                return 2u;
                            }
                            csize = (uint32_t)*(volatile uint16_t *)(uintptr_t)(fs + 0x0A);
                            sect = csize * clst + (uint32_t)*(volatile int32_t *)(uintptr_t)(fs + 0x28);
                            if (sect == 0u) {
                                *(volatile uint8_t *)(uintptr_t)(fp + 0x11) = 2u;
                                return 2u;
                            }
                            cc = btw >> 9;
                            sect += csect;
                            if (cc == 0u) {
                                /* 全部剩余数据在本扇区内: 确保窗口为当前扇区后改写 */
                                fptr = *(volatile uint32_t *)(uintptr_t)(fp + 0x14);
                                if ((*(volatile int32_t *)(uintptr_t)(fp + 0x1C) != (int32_t)sect) &&
                                    (fptr < *(volatile uint32_t *)(uintptr_t)(fp + 0x0C))) {
                                    r = disk_read((int)*(volatile uint8_t *)(uintptr_t)(fs + 1),
                                                  fp + 0x40, sect, 1);
                                    if (r != 0) {
                                        *(volatile uint8_t *)(uintptr_t)(fp + 0x11) = 1u;
                                        return 1u;
                                    }
                                    fptr = *(volatile uint32_t *)(uintptr_t)(fp + 0x14);
                                }
                                csect = fptr & 0x1ffu;
                                *(volatile int32_t *)(uintptr_t)(fp + 0x1C) = (int32_t)sect;
                                goto partial_copy;
                            }
                            if (csize < csect + cc) {
                                cc = csize - csect;
                            }
                            r = disk_write((int)*(volatile uint8_t *)(uintptr_t)(fs + 1),
                                           buff, sect, (int)cc);
                            if (r != 0) {
                                *(volatile uint8_t *)(uintptr_t)(fp + 0x11) = 1u;
                                return 1u;
                            }
                            if ((uint32_t)(*(volatile int32_t *)(uintptr_t)(fp + 0x1C) - (int32_t)sect) < cc) {
                                /* 覆盖缓存扇区: 从写缓冲复制对应 512B 回窗口并清脏 */
                                const uint32_t *src = buff +
                                    ((uint32_t)(*(volatile int32_t *)(uintptr_t)(fp + 0x1C) - (int32_t)sect)) * 0x80u;
                                volatile uint32_t *dstw = (volatile uint32_t *)(uintptr_t)(fp + 0x40);
                                const volatile uint32_t *endw = (volatile uint32_t *)(uintptr_t)(fp + 0x240);
                                const uint32_t *s = (const uint32_t *)(uintptr_t)src - 1;
                                while (dstw != endw) {
                                    s++;
                                    *dstw = *s;
                                    dstw++;
                                }
                                *(volatile uint8_t *)(uintptr_t)(fp + 0x10) =
                                    (uint8_t)(*(volatile uint8_t *)(uintptr_t)(fp + 0x10) & 0x7fu);
                            }
                            cc <<= 9;
                            fptr = *(volatile uint32_t *)(uintptr_t)(fp + 0x14);
                            goto advance;
                        }
                        /* 部分扇区改写: 窗口内自 fptr 低 9 位起 */
                        {
                            volatile uint8_t *dstb;
                            uint32_t cnt;
                            const uint8_t *srcb;
                            volatile uint8_t *d;
                            const uint8_t *s;
                            uint32_t n;
                            partial_copy:                                /* 原 LAB_00004a74 */
                            dstb = (volatile uint8_t *)(uintptr_t)(fp + 0x40 + csect);
                            cnt = 0x200u - csect;
                            if (btw <= 0x200u - csect) {
                                cnt = btw;
                            }
                            /* Ghidra 展开为字拷贝 + 尾部字节循环, 语义 = memcpy(dstb, buff, cnt) */
                            srcb = (const uint8_t *)(uintptr_t)buff;
                            d = dstb;
                            s = srcb;
                            for (n = 0; n < cnt; n++) {
                                *d++ = *s++;
                            }
                            /* flag = (flag & 0x7F) | 0x80 — 置 FA__DIRTY (Ghidra 位运算原样) */
                            *(volatile uint8_t *)(uintptr_t)(fp + 0x10) =
                                (uint8_t)(~(uint8_t)~(uint8_t)(((uint32_t)*(volatile uint8_t *)(uintptr_t)(fp + 0x10)
                                    << 25) >> 24) >> 1);
                            fptr = *(volatile uint32_t *)(uintptr_t)(fp + 0x14);
                        }
                    advance:                                 /* 原反编译公共尾 (fptr/buff 前进) */
                        fptr += cc;
                        buff = (const uint32_t *)((const uint8_t *)(uintptr_t)buff + cc);
                        fsize = *(volatile uint32_t *)(uintptr_t)(fp + 0x0C);
                        if (*(volatile uint32_t *)(uintptr_t)(fp + 0x0C) < fptr) {
                            fsize = fptr;
                        }
                        btw -= cc;
                        *bw += cc;
                        *(volatile uint32_t *)(uintptr_t)(fp + 0x0C) = fsize;
                        *(volatile uint32_t *)(uintptr_t)(fp + 0x14) = fptr;
                    } while (btw != 0u);
                    flag = *(volatile uint8_t *)(uintptr_t)(fp + 0x10);
                }
                *(volatile uint8_t *)(uintptr_t)(fp + 0x10) = (uint8_t)(flag | 0x40u);
            }
        }
    }
    return res;
}

// === fat_f_closedir === 原 FUN_000050f0 @ 0x000050f0
// 来源: reversed/fsbl_funcs/FUN_000050f0.c | 分类: fsbl
// 语义: FatFs f_closedir — fatfs_validate 校验目录对象, 通过后把 DIR 首
// 字段 (fs 指针) 清 0 断开关联; 校验失败静默返回。
void fat_f_closedir(uint32_t *dj)
{
    uint32_t fs_out;

    if (fatfs_validate(dj, &fs_out) == 0) {
        dj[0] = 0u;
    }
    return;
}

// === fat_f_mkdir === 原 FUN_00005618 @ 0x00005618
// 来源: reversed/fsbl_funcs/FUN_00005618.c | 分类: fsbl
// 语义: FatFs f_mkdir — 在 path 处创建目录。fat_find_volume(&path,&fs,2)
// 挂卷后 follow_path: 已存在返回 8 (FR_EXIST); 不存在 (4, FR_NO_FILE) 时
// 分配新簇 fat_create_chain(&fs,0) (0=无簇→7 DENIED, 1=内部错→2,
// 0xFFFFFFFF=磁盘错→1, 均先取 fat_default_date), fat_dir_seek 清零新簇,
// 在窗口构造 "."/".." 两目录项 (名+属性 AM_DIR+时间戳+dclst/父簇,
// FAT32 另写高 16 位), 置窗口脏 (fs[3]=1), fat_dir_register 登记入父目录;
// 成功后把登记项 (dj->dir) 的时间/首簇/属性字段补齐, 再次置脏并
// fat_fs_sync 落盘; 任一步失败经 fat_remove_chain 释放新簇。
int fat_f_mkdir(const uint8_t *path)
{
    const uint8_t *path_in = path;            /* 原名 local_6c (经 fat_find_volume 推进) */
    uint8_t *fs;                              /* 原名 local_68 */
    int32_t dj[8];                            /* 原名 local_54..local_38: DIR 对象 */
    uint32_t dclst;                           /* 原名 iVar8 */
    uint32_t now;                             /* 原名 uVar9: fat_default_date() 返回值 */
    volatile uint8_t *win;
    volatile uint8_t *p;
    volatile uint8_t *srcw;
    volatile uint8_t *dstw;
    int res;

    res = fat_find_volume(&path_in, &fs, 2u);
    if (res != 0) {
        return res;
    }
    dj[0] = (int32_t)(uintptr_t)fs;           /* dj->fs */
    res = fat_follow_path(dj, path_in);
    if (res == 0) {
        res = 8;                              /* FR_EXIST: 同名对象已存在 */
    } else if (res == 4) {                    /* FR_NO_FILE: 可创建 */
        dclst = fat_create_chain((int *)(uintptr_t)&fs, 0u);
        now = fat_default_date();             /* 原反编译在四个分支中各自调用 */
        if (dclst == 0u) {
            res = 7;                          /* FR_DENIED */
        } else if (dclst == 1u) {
            res = 2;                          /* FR_INT_ERR */
        } else if (dclst == 0xFFFFFFFFu) {
            res = 1;                          /* FR_DISK_ERR */
        } else {
            res = fat_dir_seek(fs, dclst);    /* 新簇清零 */
            if (res == 0) {
                win = (volatile uint8_t *)(uintptr_t)(fs + 0x40);
                /* 目录项 1: "." */
                for (p = win; p != win + 0x0B; p++) {
                    *p = (uint8_t)' ';
                }
                fs[0x56] = (uint8_t)now;      /* DIR_WrtTime 低 */
                fs[0x40] = (uint8_t)'.';
                fs[0x59] = (uint8_t)(now >> 24);   /* DIR_WrtDate 高 */
                fs[0x5A] = (uint8_t)dclst;         /* DIR_FstClusLO 低 */
                fs[0x4B] = 0x10u;                  /* DIR_Attr = AM_DIR */
                fs[0x57] = (uint8_t)(now >> 8);    /* DIR_WrtTime 高 */
                fs[0x58] = (uint8_t)(now >> 16);   /* DIR_WrtDate 低 */
                fs[0x5B] = (uint8_t)(dclst >> 8);  /* DIR_FstClusLO 高 */
                if (*(volatile uint8_t *)(uintptr_t)fs == 3u) {   /* FAT32 */
                    fs[0x54] = (uint8_t)(dclst >> 16);             /* DIR_FstClusHI 低 */
                    fs[0x55] = (uint8_t)(dclst >> 24);             /* DIR_FstClusHI 高 */
                }
                /* 目录项 2: ".." — 复制项 1 的字段区 (win+0x40..0x5B → win+0x64..0x7C) */
                srcw = (volatile uint8_t *)(uintptr_t)(fs + 0x3C);
                dstw = (volatile uint8_t *)(uintptr_t)(fs + 0x60);
                do {
                    srcw += 4;
                    dstw += 4;
                    *(volatile uint32_t *)(uintptr_t)dstw = *(volatile uint32_t *)(uintptr_t)srcw;
                } while (dstw != (volatile uint8_t *)(uintptr_t)(fs + 0x80));
                fs[0x61] = (uint8_t)'.';      /* 项 2 name[1] = '.' */
                fs[0x7A] = (uint8_t)dj[2];    /* 项 2 FstClusLO = 父目录簇 (dj->sclust) */
                fs[0x7B] = (uint8_t)((uint32_t)dj[2] >> 8);
                if (*(volatile uint8_t *)(uintptr_t)fs == 3u) {
                    fs[0x74] = (uint8_t)((uint32_t)dj[2] >> 16);
                    fs[0x75] = (uint8_t)((uint32_t)dj[2] >> 24);
                }
                fs[3] = 1u;                   /* 窗口脏标志 */
                res = fat_dir_register(dj);
                if (res == 0) {
                    volatile uint8_t *slot = (volatile uint8_t *)(uintptr_t)dj[7];  /* dj->dir */
                    slot[0x16] = (uint8_t)now;
                    slot[0x17] = (uint8_t)(now >> 8);
                    slot[0x18] = (uint8_t)(now >> 16);
                    slot[0x19] = (uint8_t)(now >> 24);
                    slot[0x1A] = (uint8_t)dclst;
                    slot[0x1B] = (uint8_t)(dclst >> 8);
                    if (*(volatile uint8_t *)(uintptr_t)fs == 3u) {
                        slot[0x14] = (uint8_t)(dclst >> 16);
                        slot[0x15] = (uint8_t)(dclst >> 24);
                    }
                    slot[0x0B] = 0x10u;       /* AM_DIR */
                    fs[3] = 1u;
                    res = fat_fs_sync(fs);
                    return res;
                }
            }
        }
        fat_remove_chain(dj, dclst, 0u);      /* 失败回收新簇 */
    }
    return res;
}

// === fat_default_date === 原 FUN_00006760 @ 0x00006760
// 来源: reversed/fsbl_funcs/FUN_00006760.c | 分类: fsbl
// 语义: FatFs get_fattime 移植 — 返回固定时间戳 0x3C210000 (无 RTC 读取:
// 日期 2014-01-01, 时间 00:00:00)。
uint32_t fat_default_date(void)
{
    return 0x3c210000u;
}

// === disk_read === 原 FUN_0000667c @ 0x0000667c
// 来源: reversed/fsbl_funcs/FUN_0000667c.c | 分类: fsbl
// 语义: FatFs disk_read 移植。disk_status bit0 (NO_INIT) 返回 3; cnt==0
// 返回 4; 槽结构 (基址 0x21560, 步进 0x220) +0x30 块寻址标志 (DAT_00021590)
// 为 0 时把扇区号左移 9 位转为字节地址; 经 sd_data_read (FUN_000072c8, 见本
// 文件) 读取, 返回 1 (失败) / 0 (成功)。
int disk_read(int pdrv, void *buf, uint32_t sect, int cnt)
{
    int r;

    if ((disk_status(pdrv) & 1u) != 0u) {
        return 3;                             /* RES_NOTRDY */
    }
    if (cnt == 0) {
        return 4;                             /* RES_PARERR */
    }
    if (*(volatile int32_t *)(uintptr_t)(0x21590u + (uint32_t)pdrv * 0x220u) == 0) {
        sect <<= 9;                           /* 非 LBA 模式: 扇区号 → 字节地址 */
    }
    r = sd_data_read((uint8_t *)(uintptr_t)(0x21560u + (uint32_t)pdrv * 0x220u),
                     sect, (uint32_t)cnt, buf);
    return (r != 0) ? 1 : 0;                  /* RES_ERROR / RES_OK */
}

// === disk_ioctl === 原 FUN_000066f8 @ 0x000066f8
// 来源: reversed/fsbl_funcs/FUN_000066f8.c | 分类: fsbl
// 语义: FatFs disk_ioctl 移植。disk_status bit0 置位返回 3; cmd==1
// (GET_SECTOR_COUNT) 输出槽结构 +0x64 扇区总数 (DAT_000215c4, 步进 0x220);
// cmd==0 (CTRL_SYNC) 直接成功; cmd==3 (GET_BLOCK_SIZE) 输出固定 0x80
// (语义待定); 其余命令返回 4 (RES_PARERR)。
int disk_ioctl(int pdrv, int cmd, void *buf)
{
    if ((disk_status(pdrv) & 1u) != 0u) {
        return 3;
    }
    if (cmd == 1) {                           /* GET_SECTOR_COUNT */
        *(volatile uint32_t *)(uintptr_t)buf =
            *(volatile uint32_t *)(uintptr_t)(0x215C4u + (uint32_t)pdrv * 0x220u);
        return 0;
    }
    if (cmd == 0) {                           /* CTRL_SYNC */
        return 0;
    }
    if (cmd == 3) {                           /* GET_BLOCK_SIZE */
        *(volatile uint32_t *)(uintptr_t)buf = 0x80u;
        return 0;
    }
    return 4;
}

/* ==================== SD/eMMC 主机层 (SDHCI) ==================== */
/* 主机寄存器偏移 (SDHCI/XSdPs 布局, 反编译实证):
 *   +0x04 块大小(W) +0x06 块数(W) +0x08 参数(D) +0x0C 传输模式(W)|命令(W)
 *   +0x24 呈现状态(D) +0x28 主机控制(B) +0x29 电源控制(B) +0x2C 时钟控制(W)
 *   +0x2E 超时控制(B) +0x30 常规中断状态(W) +0x32 错误中断状态(W)
 *   +0x34 常规中断状态使能(W) +0x36 错误中断状态使能(W)
 *   +0x38/+0x3A 中断信号使能(W) +0x3E 主机控制2(W, bit6 执行调谐/bit7 采样时钟)
 *   +0x40 能力(D) +0xFE 主机控制器版本(W)
 * SD 对象 (sd) 偏移: +0x00.. 配置副本 | +0x04 主机寄存器基址 | +0x20 缓存维护
 *   抑制标志(B) | +0x24 签名 0x11111111 | +0x28 能力(D) | +0x34 卡类型(B)
 *   +0x36 主机版本(B) | +0x38 目标频率 | +0x50 RCA | +0x6C 速度模式(D)
 *   +0x204 连续 ADMA 标志 */

// === sd_slot_cfg_get === 原 FUN_000067e8 @ 0x000067e8
// 来源: reversed/fsbl_funcs/FUN_000067e8.c | 分类: fsbl
// 语义: 单例槽配置查询: 全局字 DAT_00019bec 等于给定 id 时返回其地址,
// 否则返回 NULL。
uint16_t *sd_slot_cfg_get(uint32_t id)
{
    if (*(volatile uint32_t *)0x00019bec == id) {
        return (uint16_t *)0x00019bec;
    }
    return (uint16_t *)0;
}

// === sd_host_init === 原 FUN_00006804 @ 0x00006804
// 来源: reversed/fsbl_funcs/FUN_00006804.c | 分类: fsbl
// 语义: SD 主机对象初始化。校验 sd/cfg 非空 (dbg_print 0x1725C + 0xB9/0xBA,
// 失败置 g_assert_failed 返回 0); 把 cfg 的配置字 (0x00,0x08..0x1C 双字,
// 0x20 字节) 复制进 sd, 记录主机寄存器基址 (sd+0x04) 与 0x11111111 签名
// (sd+0x24), 清 +0x64/+0x70/+0x200/+0x204, 置 +0x6C = 6。依主机版本
// (host+0xFE 低字节, 暂存 sd+0x36) 设电源控制 (host+0x29 = 0x10/0);
// delay_ms(1000) 后置 host+0x2F = 1 并自旋等待 bit0 清零 (初始化完成);
// 保存能力寄存器 (host+0x40 → sd+0x28); 版本为 2 且能力 bit30 置位时
// delay_ms(200)。sd_set_clk_freq 校准失败返回 1; 按能力 bit24/25/26 选择
// 电源控制值 0xF/0xD/0xB; 主机版本 2 → 主机控制 (host+0x28) = 0x18,
// 否则 0x10 (ADMA2); 清中断状态使能 (0xFEFF/0xF3FF/0/0), 传输模式暂存
// DAT_00028e88 = 0x13, 块大小 (host+0x04) = 0x200。返回 0 = 成功。
int sd_host_init(uint16_t *sd, uint16_t *cfg, int host)
{
    uint32_t caps;
    volatile uint8_t *hp;
    uint8_t done;
    uint32_t sel;

    if (sd == (uint16_t *)0) {
        dbg_print(0x1725Cu, 0xb9u);           /* &DAT_0001725c, 行 0xB9 */
        g_assert_failed = 1u;
        return 0;
    }
    g_assert_failed = 0u;
    if (cfg == (uint16_t *)0) {
        dbg_print(0x1725Cu, 0xbau);
        g_assert_failed = 1u;
        return 0;
    }
    caps = *(volatile uint32_t *)(uintptr_t)(cfg + 4);        /* cfg+0x08 */
    {
        uint32_t c0c = *(volatile uint32_t *)(uintptr_t)(cfg + 6);  /* cfg+0x0C */
        uint32_t c10 = *(volatile uint32_t *)(uintptr_t)(cfg + 8);  /* cfg+0x10 */
        uint32_t c14 = *(volatile uint32_t *)(uintptr_t)(cfg + 0xA);/* cfg+0x14 */
        uint32_t c18 = *(volatile uint32_t *)(uintptr_t)(cfg + 0xC);/* cfg+0x18 */
        uint32_t c1c = *(volatile uint32_t *)(uintptr_t)(cfg + 0xE);/* cfg+0x1C */
        uint8_t  c20 = *(volatile uint8_t *)(uintptr_t)(cfg + 0x10);/* cfg+0x20 */
        *(volatile uint16_t *)(uintptr_t)sd = *(volatile uint16_t *)(uintptr_t)cfg;
        *(volatile uint32_t *)(uintptr_t)((uint8_t *)sd + 0x24) = 0x11111111u;
        *(volatile int32_t *)(uintptr_t)((uint8_t *)sd + 0x04) = host;
        *(volatile uint32_t *)(uintptr_t)((uint8_t *)sd + 0x08) = caps;
        *(volatile uint32_t *)(uintptr_t)((uint8_t *)sd + 0x0C) = c0c;
        *(volatile uint32_t *)(uintptr_t)((uint8_t *)sd + 0x14) = c14;
        *(volatile uint32_t *)(uintptr_t)((uint8_t *)sd + 0x18) = c18;
        *(volatile uint32_t *)(uintptr_t)((uint8_t *)sd + 0x10) = c10;
        *(volatile uint32_t *)(uintptr_t)((uint8_t *)sd + 0x1C) = c1c;
        *(volatile uint32_t *)(uintptr_t)((uint8_t *)sd + 0x64) = 0u;
        *(volatile uint32_t *)(uintptr_t)((uint8_t *)sd + 0x70) = 0u;
        *(volatile uint8_t *)(uintptr_t)((uint8_t *)sd + 0x20) = c20;
        *(volatile uint32_t *)(uintptr_t)((uint8_t *)sd + 0x6C) = 6u;
        *(volatile uint32_t *)(uintptr_t)((uint8_t *)sd + 0x200) = 0u;
        *(volatile uint32_t *)(uintptr_t)((uint8_t *)sd + 0x204) = 0u;
    }
    if ((*(volatile uint16_t *)(uintptr_t)(host + 0xFE) & 0xffu) == 2u) {
        *(volatile uint8_t *)(uintptr_t)(host + 0x29) = 0x10u;
    } else {
        *(volatile uint8_t *)(uintptr_t)(host + 0x29) = 0u;
    }
    delay_ms(1000u);
    *(volatile uint8_t *)(uintptr_t)(host + 0x2F) = 1u;
    hp = (volatile uint8_t *)(uintptr_t)host;
    done = hp[0x2F];
    while ((done & 1u) != 0u) {
        done = hp[0x2F];
    }
    *(volatile int8_t *)(uintptr_t)((uint8_t *)sd + 0x36) =
        (int8_t)(uint8_t)*(volatile uint16_t *)(uintptr_t)(host + 0xFE);
    *(volatile uint32_t *)(uintptr_t)((uint8_t *)sd + 0x28) =
        *(volatile uint32_t *)(uintptr_t)(host + 0x40);
    *(volatile uint8_t *)(uintptr_t)(host + 0x29) = 0xfu;
    if ((*(volatile int8_t *)(uintptr_t)((uint8_t *)sd + 0x36) == 2) &&
        ((*(volatile uint32_t *)(uintptr_t)((uint8_t *)sd + 0x28) & 0xc0000000u) == 0x40000000u)) {
        delay_ms(200u);
    }
    if (sd_set_clk_freq((int)(uintptr_t)sd, 0x61A80u) != 0) {  /* 400 kHz 初频 (Ghidra 误标字符串 s__00061a80) */
        return 1;
    }
    caps = *(volatile uint32_t *)(uintptr_t)((uint8_t *)sd + 0x28);
    if ((caps & 0x1000000u) == 0u) {
        if ((caps & 0x2000000u) == 0u) {
            if ((caps & 0x4000000u) == 0u) {
                sel = 1u;
            } else {
                sel = 0xbu;
            }
        } else {
            sel = 0xdu;
        }
    } else {
        sel = 0xfu;
    }
    *(volatile uint8_t *)(uintptr_t)(host + 0x29) = (uint8_t)sel;
    if (*(volatile int8_t *)(uintptr_t)((uint8_t *)sd + 0x36) == 2) {
        sel = 0x18u;
    } else {
        sel = 0x10u;
    }
    *(volatile uint8_t *)(uintptr_t)(host + 0x28) = (uint8_t)sel;
    hp = (volatile uint8_t *)(uintptr_t)host;
    *(volatile uint16_t *)(uintptr_t)(hp + 0x34) = 0xfeffu;
    *(volatile uint16_t *)(uintptr_t)(hp + 0x36) = 0xf3ffu;
    *(volatile uint16_t *)(uintptr_t)(hp + 0x38) = 0u;
    *(volatile uint16_t *)(uintptr_t)(hp + 0x3A) = 0u;
    *(volatile uint32_t *)0x00028e88 = 0x13u;         /* DAT_00028e88: 传输模式暂存 */
    *(volatile uint16_t *)(uintptr_t)(hp + 4) = 0x200u;
    return 0;
}

// === sd_card_select === 原 FUN_000070e0 @ 0x000070e0
// 来源: reversed/fsbl_funcs/FUN_000070e0.c | 分类: fsbl
// 语义: CMD7 (enc 0x700, SELECT_DESELECT_CARD) 以 RCA (sd+0x50) 选择/取消
// 选择卡, 返回发送是否成功 (0 = 成功, 1 = 失败)。
int sd_card_select(int sd)
{
    int r = sd_send_cmd(sd, 0x700u, *(volatile uint32_t *)(uintptr_t)(sd + 0x50), 0u);
    return (r != 0) ? 1 : 0;
}

// === sd_data_read === 原 FUN_000072c8 @ 0x000072c8
// 来源: reversed/fsbl_funcs/FUN_000072c8.c | 分类: fsbl
// 语义: 多扇区数据读取。前置校验: (主机版本==2 且能力 bit30 模式) 或
// sd+0x0C 配置为 0 或呈现状态 (host+0x24) bit16 (DAT0 电平 = 卡插入) 置位,
// 且块大小为 0x200 或 sdmmc_set_blocklen(sd, 0x200) 成功, 否则返回 1。
// sd+0x204 == 0 时经 sd_adma_setup 建单次 ADMA2 描述符 (sd[0x20]==0 时先对
// 缓冲做缓存维护 fun_0xffffd23c), 否则 sd_adma_setup_cont 连续模式;
// 暂存传输模式 DAT_00028e88 = 0x13 (单块) / 0x37 (多块, ACMD12+块计数),
// 发 CMD17 (enc 0x1100) / CMD18 (enc 0x1200); 等待常规中断状态 (host+0x30)
// bit2 (传输完成): 出错 bit15 时清错误状态 (host+0x32 = 0xF3FF) 返回 1,
// 成功写 2 确认, sd[0x20]==0 时再做一次缓存维护, 返回 0。
int sd_data_read(uint8_t *sd, uint32_t sect, uint32_t cnt, void *buf)
{
    int r;
    volatile uint16_t *hp;

    if ((!((*(volatile int8_t *)(uintptr_t)(sd + 0x36) == 2) &&
           ((*(volatile uint32_t *)(uintptr_t)(sd + 0x28) & 0xc0000000u) == 0x40000000u)) &&
         (*(volatile int32_t *)(uintptr_t)(sd + 0x0C) != 0)) &&
        ((*(volatile uint32_t *)(uintptr_t)(*(volatile int32_t *)(uintptr_t)(sd + 4) + 0x24) &
          0x10000u) == 0u)) {
        return 1;
    }
    if ((*(volatile int32_t *)(uintptr_t)(*(volatile int32_t *)(uintptr_t)(sd + 4) + 4) != 0x200) &&
        (sdmmc_set_blocklen((int)(uintptr_t)sd, 0x200u) != 0)) {
        return 1;
    }
    if (*(volatile int32_t *)(uintptr_t)(sd + 0x204) == 0) {
        sd_adma_setup((int)(uintptr_t)sd, (int)cnt, buf);
        if (*(volatile int8_t *)(uintptr_t)(sd + 0x20) == 0) {
            fun_0xffffd23c(buf, cnt << 9);    /* 缓冲区缓存维护 (命名待补) */
        }
    } else {
        sd_adma_setup_cont((int)(uintptr_t)sd, (int)cnt);
    }
    if (cnt == 1u) {
        *(volatile uint32_t *)0x00028e88 = 0x13u;
        r = sd_send_cmd((int)(uintptr_t)sd, 0x1100u, sect, (uint16_t)cnt);   /* CMD17 */
    } else {
        *(volatile uint32_t *)0x00028e88 = 0x37u;
        r = sd_send_cmd((int)(uintptr_t)sd, 0x1200u, sect, (uint16_t)cnt);   /* CMD18 */
    }
    if (r == 0) {
        hp = (volatile uint16_t *)(uintptr_t)*(volatile int32_t *)(uintptr_t)(sd + 4);
        do {
            if ((hp[0x30 / 2] & 0x8000u) != 0u) {
                hp[0x32 / 2] = 0xf3ffu;       /* 清错误中断状态 */
                return 1;
            }
        } while ((hp[0x30 / 2] & 2u) == 0u);
        hp[0x30 / 2] = 2u;                    /* 确认传输完成 */
        if (*(volatile int8_t *)(uintptr_t)(sd + 0x20) == 0) {
            fun_0xffffd23c(buf, cnt << 9);
        }
        return 0;
    }
    return 1;
}

// === sd_send_scr === 原 FUN_00007dfc @ 0x00007dfc
// 来源: reversed/fsbl_funcs/FUN_00007dfc.c | 分类: fsbl
// 语义: 读 SD SCR 寄存器 (8 字节)。校验 sd 非空与签名 0x11111111
// (dbg_print 0x17264 + 0xB3/0xB4); buf 清零 8 字节; CMD55 (enc 0x3700, RCA,
// 无数据) 后设块大小 8, sd_adma_setup 单块, 传输模式 0x11, sd[0x20]==0 时
// 缓存维护, 发 ACMD51 (enc 0xB300) 读 1 块; 等待传输完成 (出错返回 1,
// 确认后返回 0)。
int sd_send_scr(int sd, void *buf)
{
    int r;
    volatile uint16_t *hp;
    volatile uint8_t *z;

    if (sd == 0) {
        dbg_print(0x17264u, 0xb3u);
        g_assert_failed = 1u;
        return 0;
    }
    g_assert_failed = 0u;
    if (*(volatile int32_t *)(uintptr_t)(sd + 0x24) != (int)0x11111111u) {
        dbg_print(0x17264u, 0xb4u);
        g_assert_failed = 1u;
        return 0;
    }
    z = (volatile uint8_t *)(uintptr_t)buf - 1;
    do {
        z++;
        *z = 0u;
    } while (z != (volatile uint8_t *)(uintptr_t)buf + 7);
    r = sd_send_cmd(sd, 0x3700u, *(volatile uint32_t *)(uintptr_t)(sd + 0x50), 0u);   /* CMD55 */
    if (r == 0) {
        *(volatile uint16_t *)(uintptr_t)(*(volatile int32_t *)(uintptr_t)(sd + 4) + 4) = 8u;
        sd_adma_setup(sd, 1, buf);
        *(volatile uint32_t *)0x00028e88 = 0x11u;
        if (*(volatile int8_t *)(uintptr_t)(sd + 0x20) == 0) {
            fun_0xffffd23c(buf, 8u);
        }
        r = sd_send_cmd(sd, 0xb300u, 0u, 1u); /* ACMD51 (SEND_SCR) */
        if (r == 0) {
            hp = (volatile uint16_t *)(uintptr_t)*(volatile int32_t *)(uintptr_t)(sd + 4);
            do {
                if ((hp[0x30 / 2] & 0x8000u) != 0u) {
                    hp[0x32 / 2] = 0xf3ffu;
                    return 1;
                }
            } while ((hp[0x30 / 2] & 2u) == 0u);
            hp[0x30 / 2] = 2u;
            return 0;
        }
    }
    return 1;
}

// === sdmmc_switch_high_speed === 原 FUN_00008428 @ 0x00008428
// 来源: reversed/fsbl_funcs/FUN_00008428.c | 分类: fsbl
// 语义: 切换卡到高速模式并调整主机时钟。校验 sd/签名 (0x1E4/0x1E5);
// 64 字节暂存缓冲清零。按卡类型 (sd+0x34 字节):
//   1 (SD): 块大小 0x40, 单块 ADMA + 缓存维护 fun_0xffffd370, 传输模式 0x11,
//     CMD6 (enc 0x600) arg 0x80FFFFF1 (切换功能组 1), 成功后目标 50 MHz;
//   2 (eMMC): CMD6 arg 0x3B90100 (EXT_CSD HS_TIMING=1), 目标 52 MHz;
//   其他 (按速度模式 sd+0x6C): ==7 (HS200/SDR104) → arg 0x3B90200 + 200 MHz
//     + 块大小 0x80/0x40 + 传输模式 0x10 + 主机控制2 bit6 执行调谐, 循环发
//     调谐命令 (SD: enc 0x1300 / 其他: enc 0x1500) 至多 40 次, 要求主机控制2
//     bit7 (采样时钟有效) 置位; 非 7 → arg 0x3B90100 + 52 MHz。
// 各路径等待 CMD6 完成后 sd_set_clk_freq 设频; 失败返回 1。末尾 delay_ms(1000)
// 并置主机控制 bit2 (高速使能), 返回 0。
int sdmmc_switch_high_speed(int sd)
{
    uint8_t  tun_buf[68];                     /* 原名 auStack_58 (64 字节 + 对齐) */
    uint32_t target;
    uint32_t arg;
    int r;
    volatile uint16_t *hp;
    const void *enc;                          /* 名义 pcVar2: 调谐命令编码 */
    int retries;
    volatile uint8_t *tp;

    mem_set(tun_buf, 0, 0x40u);
    if (sd == 0) {
        dbg_print(0x17264u, 0x1e4u);
        g_assert_failed = 1u;
        return 0;
    }
    g_assert_failed = 0u;
    if (*(volatile int32_t *)(uintptr_t)(sd + 0x24) != (int)0x11111111u) {
        dbg_print(0x17264u, 0x1e5u);
        g_assert_failed = 1u;
        return 0;
    }
    if (*(volatile int8_t *)(uintptr_t)(sd + 0x34) == 1) {
        /* SD 卡: CMD6 切换功能组 1, 50 MHz */
        *(volatile uint16_t *)(uintptr_t)(*(volatile int32_t *)(uintptr_t)(sd + 4) + 4) = 0x40u;
        sd_adma_setup(sd, 1, tun_buf);
        if (*(volatile int8_t *)(uintptr_t)(sd + 0x20) == 0) {
            fun_0xffffd370(tun_buf, 0x40u);
        }
        *(volatile uint32_t *)0x00028e88 = 0x11u;
        r = sd_send_cmd(sd, 0x600u, 0x80fffff1u, 1u);
        if (r != 0) {
            return 1;
        }
        hp = (volatile uint16_t *)(uintptr_t)*(volatile int32_t *)(uintptr_t)(sd + 4);
        do {
            if ((hp[0x30 / 2] & 0x8000u) != 0u) {
                goto cmd6_err;                /* LAB_000085f8 */
            }
        } while ((hp[0x30 / 2] & 2u) == 0u);
        target = 50000000u;
    } else if (*(volatile int8_t *)(uintptr_t)(sd + 0x34) != 2) {
        /* 其他类型: 按速度模式选择 HS200/SDR104 (200 MHz, 带调谐) 或 52 MHz */
        if (*(volatile int32_t *)(uintptr_t)(sd + 0x6C) == 7) {
            target = 200000000u;
            arg = 0x3b90200u;
        } else {
            target = 52000000u;
            arg = 0x3b90100u;
        }
        *(volatile uint32_t *)(uintptr_t)(sd + 0x38) = target;
        r = sd_send_cmd(sd, 0x600u, arg, 0u);
        if (r != 0) {
            return 1;
        }
        hp = (volatile uint16_t *)(uintptr_t)*(volatile int32_t *)(uintptr_t)(sd + 4);
        do {
            if ((hp[0x30 / 2] & 0x8000u) != 0u) {
                goto cmd6_err;
            }
        } while ((hp[0x30 / 2] & 2u) == 0u);
        target = *(volatile uint32_t *)(uintptr_t)(sd + 0x38);
        hp[0x30 / 2] = 2u;
        r = sd_set_clk_freq(sd, target);
        if (r != 0) {
            return 1;
        }
        if (*(volatile int32_t *)(uintptr_t)(sd + 0x6C) == 7) {
            g_assert_failed = (uint32_t)r;    /* 反编译原样: 以 set_clk 结果 (0) 清标志 */
            if (*(volatile int32_t *)(uintptr_t)(sd + 0x24) == (int)0x11111111u) {
                hp = (volatile uint16_t *)(uintptr_t)*(volatile int32_t *)(uintptr_t)(sd + 4);
                if (*(volatile int8_t *)(uintptr_t)(sd + 0x37) == 3) {
                    hp[0x04 / 2] = 0x80u;
                } else {
                    hp[0x04 / 2] = 0x40u;
                }
                *(volatile uint32_t *)0x00028e88 = 0x10u;
                hp[0x3E / 2] |= 0x40u;  /* 执行调谐 */
                delay_ms(1u);
                retries = 0x28;               /* '(' = 40 次重试 */
                do {
                    enc = (const void *)0x1500;             /* CMD21 (eMMC 调谐) 编码 */
                    if (*(volatile int8_t *)(uintptr_t)(sd + 0x34) == 1) {
                        enc = (const void *)0x1300;         /* CMD19 (SD 调谐) 编码 */
                    }
                    r = sd_send_cmd(sd, (uint32_t)(uintptr_t)enc, 0u, 1u);
                    retries--;
                    if (r != 0) {
                        return 1;
                    }
                } while (((*(volatile uint16_t *)(uintptr_t)(*(volatile int32_t *)(uintptr_t)(sd + 4) + 0x3E) &
                           0x40u) != 0u) && (retries != 0));
                if ((*(volatile uint16_t *)(uintptr_t)(*(volatile int32_t *)(uintptr_t)(sd + 4) + 0x3E) &
                     0x80u) == 0u) {
                    return 1;                 /* 采样时钟未锁定 */
                }
                delay_ms(1u);
            } else {
                dbg_print(0x17264u, 0x475u);
                g_assert_failed = 1u;
            }
        }
        goto hs_enable;                       /* 原 LAB_00008630: 切速后公共尾 */
    } else {
        /* eMMC: CMD6 EXT_CSD HS_TIMING=1, 52 MHz */
        r = sd_send_cmd(sd, 0x600u, 0x3b90100u, 0u);
        if (r != 0) {
            return 1;
        }
        hp = (volatile uint16_t *)(uintptr_t)*(volatile int32_t *)(uintptr_t)(sd + 4);
        do {
            if ((hp[0x30 / 2] & 0x8000u) != 0u) {
                goto cmd6_err;
            }
        } while ((hp[0x30 / 2] & 2u) == 0u);
        target = 52000000u;
    }
    hp[0x30 / 2] = 2u;                        /* 确认 CMD6 完成 */
    *(volatile uint32_t *)(uintptr_t)(sd + 0x38) = target;
    r = sd_set_clk_freq(sd, target);
    if (r != 0) {
        return 1;
    }
    goto hs_enable;                           /* 原 LAB_00008630: 切速后公共尾 */
cmd6_err:                                     /* 原 LAB_000085f8 */
    hp[0x32 / 2] = 0xf3ffu;
    return 1;
hs_enable:                                    /* 原 LAB_00008630: 切速后公共尾 */
    tp = (volatile uint8_t *)(uintptr_t)*(volatile int32_t *)(uintptr_t)(sd + 4);
    delay_ms(1000u);
    *(volatile uint8_t *)(uintptr_t)(tp + 0x28) |= 4u;   /* 主机控制 bit2: 高速使能 */
    return 0;
}

// === mmc_send_ext_csd === 原 FUN_0000884c @ 0x0000884c
// 来源: reversed/fsbl_funcs/FUN_0000884c.c | 分类: fsbl
// 语义: eMMC CMD8 (enc 0x800, SEND_EXT_CSD): 校验 sd/签名 (0x349/0x34A),
// buf 清零 512 字节, 块大小 0x200, 单块 ADMA, sd[0x20]==0 时缓存维护,
// 传输模式 0x11, 发 CMD8 读 1 块; 等待完成 (出错清错误状态返回 1, 确认后
// 返回 0)。buf 收到 512 字节 EXT_CSD。
int mmc_send_ext_csd(int sd, void *buf)
{
    int r;
    volatile uint16_t *hp;
    volatile uint8_t *z;

    if (sd == 0) {
        dbg_print(0x17264u, 0x349u);
        g_assert_failed = 1u;
        return 0;
    }
    g_assert_failed = 0u;
    if (*(volatile int32_t *)(uintptr_t)(sd + 0x24) != (int)0x11111111u) {
        dbg_print(0x17264u, 0x34au);
        g_assert_failed = 1u;
        return 0;
    }
    z = (volatile uint8_t *)(uintptr_t)buf - 1;
    do {
        z++;
        *z = 0u;
    } while (z != (volatile uint8_t *)(uintptr_t)buf + 0x1ff);
    *(volatile uint16_t *)(uintptr_t)(*(volatile int32_t *)(uintptr_t)(sd + 4) + 4) = 0x200u;
    sd_adma_setup(sd, 1, buf);
    if (*(volatile int8_t *)(uintptr_t)(sd + 0x20) == 0) {
        fun_0xffffd23c(buf, 0x200u);
    }
    *(volatile uint32_t *)0x00028e88 = 0x11u;
    r = sd_send_cmd(sd, 0x800u, 0u, 1u);      /* CMD8 (SEND_EXT_CSD) */
    if (r != 0) {
        return 1;
    }
    hp = (volatile uint16_t *)(uintptr_t)*(volatile int32_t *)(uintptr_t)(sd + 4);
    do {
        if ((hp[0x30 / 2] & 0x8000u) != 0u) {
            hp[0x32 / 2] = 0xf3ffu;
            return 1;
        }
    } while ((hp[0x30 / 2] & 2u) == 0u);
    hp[0x30 / 2] = 2u;
    return 0;
}

/* ==================== RSA 大数运算 / SHA-256 ==================== */

// === rsa_bignum_sub === 原 FUN_00008eac @ 0x00008eac
// 来源: reversed/fsbl_funcs/FUN_00008eac.c | 分类: fsbl_funcs
// 语义: 64 字 (2048 位) 大数原地减法: dst[i] -= sub[i], 逐字借位链传播
//       (ARM SUBS/SBC 进位链; 借位 = (a < b) || (a-b < 前级借位))。
void rsa_bignum_sub(uint32_t *dst, const uint32_t *sub)
{
    uint32_t borrow = 0u;

    for (int i = 0; i < 64; i++) {
        uint32_t a = dst[i];
        uint32_t b = sub[i];
        uint32_t diff = a - b;
        dst[i] = diff - borrow;
        /* 原 ARM 进位链表达式; 等价于 borrow = (a < b) || (diff < borrow) */
        borrow = (uint32_t)(-(uint32_t)(diff < borrow) != (uint32_t)(a < b));
    }
}

// === rsa_modexp_u32exp === 原 FUN_00008f80 @ 0x00008f80
// 来源: reversed/fsbl_funcs/FUN_00008f80.c | 分类: fsbl_funcs
// 语义: Montgomery 模幂 (从高位到低位的平方-乘法, 32 位指数)。
//       result = msg^exp mod mod (64 字/2048 位)。
//       rr = R^2 mod m 预计算值; n0inv = rsa_inv_mod2p32(mod[0])。
//       步骤: t = toMont(msg) = mont(msg, rr); acc = toMont(1) = mont(rr, 1);
//       逐位: 平方 acc, 指数位为 1 时乘 t; 最终 fromMont: acc = mont(acc, 1)。
//       当指数为 0 时循环不生效, 结果仍经 mont(rr,1) 与 mont(acc,1)。
void rsa_modexp_u32exp(uint32_t *result, const uint32_t *msg, uint32_t exponent,
                       const uint32_t *mod, const uint32_t *rr)
{
    uint32_t msg_mont[64];                 /* auStack_218: toMont(msg) */
    uint32_t one_mont[64];                 /* local_118: toMont(1) */
    uint32_t started = 0u;                 /* local_c: 首个有效指数位标志 */
    uint32_t bit;

    mem_set(one_mont, 0, 0x100u);
    one_mont[0] = 1u;
    uint32_t n0inv = rsa_inv_mod2p32(mod[0]);
    rsa_mont_mul(msg_mont, msg, rr, mod, n0inv);        /* toMont(msg) */
    rsa_mont_mul(result, rr, one_mont, mod, n0inv);     /* toMont(1) */
    for (bit = 0x80000000u; bit != 0u; bit >>= 1) {
        uint32_t cur = exponent & bit;
        if (started != 0u) {
            rsa_mont_mul(result, result, result, mod, n0inv);   /* 平方 */
        }
        if (started == 0u) {
            started = cur;                     /* 首个置位前不做平方 */
        }
        if (cur != 0u) {
            rsa_mont_mul(result, result, msg_mont, mod, n0inv); /* 乘基 */
        }
    }
    rsa_mont_mul(result, result, one_mont, mod, n0inv); /* fromMont(result) */
}

// === sha256_update === 原 FUN_000092c8 @ 0x000092c8
// 来源: reversed/fsbl_funcs/FUN_000092c8.c | 分类: fsbl_funcs
// 语义: SHA-256 增量更新。ctx 布局: +0x00..0x1F 状态(8 字), +0x20..0x5F 64 字节
//       缓冲, +0x60 起总长度计数 (64 位: +0x60 低位/+0x64 高位)。数据按大端
//       逐字装载, 64 字节块调用 sha256_transform, 余数留在缓冲。
void sha256_update(uint32_t *ctx, const uint8_t *data, uint32_t len)
{
    uint32_t w[16];
    uint32_t buffered;
    uint32_t fill;
    const uint8_t *p = data;
    uint32_t remain = len;
    uint32_t i;

    if (len == 0u) {
        return;
    }
    /* 64 位总长度计数: ctx[0x60] += len, 进位入 ctx[0x64] */
    buffered = ctx[0x60 / 4];
    ctx[0x60 / 4] = len + buffered;
    ctx[0x64 / 4] = ctx[0x64 / 4] + (uint32_t)(len + buffered < len);

    buffered = ctx[0x60 / 4] & 0x3fu;
    fill = 0x40u - buffered;
    if ((buffered != 0u) && (fill <= len)) {
        /* 先填满残块 (残块起点 = 缓冲 + 已缓冲字节数) */
        mem_copy((uint8_t *)ctx + buffered + 0x20, p, fill);
        for (i = 0; i < 16u; i++) {
            w[i] = (uint32_t)((uint8_t *)ctx)[i * 4 + 0x20] << 0x18 |
                   (uint32_t)((uint8_t *)ctx)[i * 4 + 0x21] << 0x10 |
                   (uint32_t)((uint8_t *)ctx)[i * 4 + 0x22] << 8 |
                   (uint32_t)((uint8_t *)ctx)[i * 4 + 0x23];
        }
        sha256_transform(ctx, w);
        remain = len - fill;
        p = data + fill;
    }
    for (; remain > 0x3fu; remain -= 0x40u) {
        for (i = 0; i < 16u; i++) {
            w[i] = (uint32_t)p[i * 4] << 0x18 |
                   (uint32_t)p[i * 4 + 1] << 0x10 |
                   (uint32_t)p[i * 4 + 2] << 8 |
                   (uint32_t)p[i * 4 + 3];
        }
        sha256_transform(ctx, w);
        p += 0x40u;
    }
    if (remain != 0u) {
        mem_copy((uint8_t *)ctx + 0x20, p, remain);
    }
}

// === sha256_transform === 原 FUN_000099a0 @ 0x000099a0
// 来源: reversed/fsbl_funcs/FUN_000099a0.c | 分类: fsbl_funcs
// 语义: SHA-256 块压缩函数。ctx: +0x00..0x1F 8 字状态 (读入 8 个工作寄存器,
//       结束时累加回写); w_in[16] 为已按大端解码的消息字。轮函数 Σ0/Σ1/Ch/Maj
//       与标准 SHA-256 相同; 原 Ghidra 产物为 4×16 组展开且工作寄存器以数组
//       旋转槽位实现, 此处还原为标准 a..h 轮循环 (语义等价)。
//       注意: 轮常量表取自固件实际数据 0x15BB8 (值 = 0x30E00000 + j*0x100000,
//       非标准 SHA-256 K 常量; 标准 K 表另存于 0xEB38 但未被本函数引用,
//       语义待定)。
static const uint32_t sha256_k[64] = {
    0x30E00000u, 0x30F00000u, 0x31000000u, 0x31100000u,
    0x31200000u, 0x31300000u, 0x31400000u, 0x31500000u,
    0x31600000u, 0x31700000u, 0x31800000u, 0x31900000u,
    0x31A00000u, 0x31B00000u, 0x31C00000u, 0x31D00000u,
    0x31E00000u, 0x31F00000u, 0x32000000u, 0x32100000u,
    0x32200000u, 0x32300000u, 0x32400000u, 0x32500000u,
    0x32600000u, 0x32700000u, 0x32800000u, 0x32900000u,
    0x32A00000u, 0x32B00000u, 0x32C00000u, 0x32D00000u,
    0x32E00000u, 0x32F00000u, 0x33000000u, 0x33100000u,
    0x33200000u, 0x33300000u, 0x33400000u, 0x33500000u,
    0x33600000u, 0x33700000u, 0x33800000u, 0x33900000u,
    0x33A00000u, 0x33B00000u, 0x33C00000u, 0x33D00000u,
    0x33E00000u, 0x33F00000u, 0x34000000u, 0x34100000u,
    0x34200000u, 0x34300000u, 0x34400000u, 0x34500000u,
    0x34600000u, 0x34700000u, 0x34800000u, 0x34900000u,
    0x34A00000u, 0x34B00000u, 0x34C00000u, 0x34D00000u,
};

#define SHA256_ROTR(x, n) (((x) >> (n)) | ((x) << (32 - (n))))

void sha256_transform(uint32_t *ctx, const uint32_t w_in[16])
{
    uint32_t w[64];
    uint32_t a, b, c, d, e, f, g, h;
    uint32_t t1, t2;
    int j;

    /* 原产物 FUN_00002200(ctx→工作区,0x20): 状态 H0..H7 载入工作寄存器 */
    a = ctx[0]; b = ctx[1]; c = ctx[2]; d = ctx[3];
    e = ctx[4]; f = ctx[5]; g = ctx[6]; h = ctx[7];
    for (j = 0; j < 16; j++) {
        w[j] = w_in[j];
    }
    for (j = 16; j < 64; j++) {
        /* 消息扩展: W[j] = W[j-16] + σ0(W[j-15]) + W[j-7] + σ1(W[j-2]) */
        uint32_t s0 = SHA256_ROTR(w[j - 15], 7) ^ SHA256_ROTR(w[j - 15], 18) ^ (w[j - 15] >> 3);
        uint32_t s1 = SHA256_ROTR(w[j - 2], 17) ^ SHA256_ROTR(w[j - 2], 19) ^ (w[j - 2] >> 10);
        w[j] = w[j - 16] + s0 + w[j - 7] + s1;
    }
    for (j = 0; j < 64; j++) {
        uint32_t sig1 = SHA256_ROTR(e, 6) ^ SHA256_ROTR(e, 11) ^ SHA256_ROTR(e, 25);
        uint32_t ch = (e & f) ^ (~e & g);
        uint32_t sig0 = SHA256_ROTR(a, 2) ^ SHA256_ROTR(a, 13) ^ SHA256_ROTR(a, 22);
        uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
        t1 = h + sig1 + ch + sha256_k[j] + w[j];
        t2 = sig0 + maj;
        h = g; g = f; f = e; e = d + t1;
        d = c; c = b; b = a; a = t1 + t2;
    }
    ctx[0] += a; ctx[1] += b; ctx[2] += c; ctx[3] += d;
    ctx[4] += e; ctx[5] += f; ctx[6] += g; ctx[7] += h;
}

/* ==================== eFuse (SLCR 0xF800D/E 寄存器空间) ==================== */

/* EFUSE_CLK_CTRL (SLCR 0xF800D00C) 与定时寄存器 */
#define EFUSE_CLK_CTRL       (*(volatile uint32_t *)0xF800D00Cu)
#define EFUSE_REG_D018       (*(volatile uint32_t *)0xF800D018u)  /* 语义待定 */
#define EFUSE_REG_D01C       (*(volatile uint32_t *)0xF800D01Cu)  /* 语义待定 */

// === efuse_setup_timing === 原 FUN_0000aedc @ 0x0000aedc
// 来源: reversed/fsbl_funcs/FUN_0000aedc.c | 分类: fsbl_funcs
// 语义: 配置 eFuse 时钟控制与定时参数。0xF800D00C: mode==1 时置 bit16 (时钟源
//       选择), 否则清 bit16; 总是清 bit4/bit5 后按 sub 重新置位 (sub==2 → bit4,
//       sub==3 → bit5, sub==1 → 两者保持清除)。定时寄存器 0xD018/0xD01C 由
//       频率 freq 换算: D018 = freq*12/1000000, D01C = freq*15/100000000
//       (32 位乘法可回绕, 除法为无符号)。返回 0。
int efuse_setup_timing(int mode, uint32_t freq, int sub)
{
    if (mode == 1) {
        EFUSE_CLK_CTRL = EFUSE_CLK_CTRL | 0x10000u;
    } else {
        EFUSE_CLK_CTRL = EFUSE_CLK_CTRL & 0xfffeffffu;
    }
    EFUSE_CLK_CTRL = EFUSE_CLK_CTRL & 0xffffffcfu;
    if (sub == 2) {
        EFUSE_CLK_CTRL = EFUSE_CLK_CTRL | 0x10u;
    } else if (sub == 3) {
        EFUSE_CLK_CTRL = EFUSE_CLK_CTRL | 0x20u;
    }
    EFUSE_REG_D018 = (uint32_t)(freq * 12u) / 1000000u;
    EFUSE_REG_D01C = (uint32_t)(freq * 15u) / 100000000u;
    return 0;
}

// === efuse_addr_is_reserved === 原 FUN_0000b254 @ 0x0000b254
// 来源: reversed/fsbl_funcs/FUN_0000b254.c | 分类: fsbl_funcs
// 语义: 判定 eFuse 寄存器地址是否为保留 (禁止写入) 地址, 返回 1 = 保留。
//       (1) 命中 20 个排除地址 → 保留: 0xF800E080 与两组等差寄存器组
//           (0xE104+0x84*k*9 / 0xE8FC+0x7C*k*10, eFuse 行数据/密钥读回组)。
//       (2) 未命中时经四个范围比较: 令 off1 = addr-0xF800E040, off2 =
//           addr-0xF800E000, off3 = (addr&~0x800)-0xF800E600, off4 =
//           addr-0xF800E860, off5 = addr-0xF800E800 (各常数即 2^32 补码加数)。
//           仅当 off1>0x3C 且 off2>0x1B 且 off2!=0x1C 且 off3>0x1FC 且
//           off4>0x1C 时返回 (off5<0x3D), 否则返回 1 (保留)。
int efuse_addr_is_reserved(uint32_t addr)
{
    static const uint32_t reserved_list[20] = {
        0xF800E080u, 0xF800E104u, 0xF800E188u, 0xF800E20Cu, 0xF800E290u,
        0xF800E314u, 0xF800E398u, 0xF800E41Cu, 0xF800E4A0u, 0xF800E524u,
        0xF800E8FCu, 0xF800E978u, 0xF800E9F4u, 0xF800EA70u, 0xF800EAECu,
        0xF800EB68u, 0xF800EBE4u, 0xF800EC60u, 0xF800ECDCu, 0xF800ED58u,
    };
    uint32_t off1;
    uint32_t off2;
    int i;

    for (i = 0; i < 20; i++) {
        if (addr == reserved_list[i]) {
            return 1;
        }
    }
    off1 = addr + 0x7ff1fc0u;                 /* addr - 0xF800E040 */
    off2 = addr + 0x7ff2000u;                 /* addr - 0xF800E000 */
    if (off1 > 0x3Cu && off2 > 0x1Bu && off2 != 0x1Cu) {
        uint32_t off3 = (addr & 0xfffff7ffu) + 0x7ff1a00u; /* (addr & ~0x800) - 0xF800E600 */
        uint32_t off4 = addr + 0x7ff17a0u;                 /* addr - 0xF800E860 */
        if (off3 > 0x1FCu && off4 > 0x1Cu) {
            return (addr + 0x7ff1800u) < 0x3Du;            /* addr - 0xF800E800 < 0x3D */
        }
    }
    return 1;
}

// === efuse_write_bit === 原 FUN_0000b680 @ 0x0000b680
// 来源: reversed/fsbl_funcs/FUN_0000b680.c | 分类: fsbl_funcs
// 语义: 对非保留 eFuse 寄存器做 bit0 掩码写: 读寄存器字, 取 bit0, 以字节写
//       写回该寄存器。地址保留时跳过写入。返回 1 = 保留未写, 0 = 已执行写入。
//       (Ghidra 产物将写目标误显示为返回值高位指针, 实为调用方以 r0/r1 双寄存
//       器传入的同一行地址; 见 FUN_0000b6a4 中 r1=r0 的调用形态。)
int efuse_write_bit(uint32_t *row)
{
    if (efuse_addr_is_reserved((uint32_t)(uintptr_t)row) != 0) {
        return 1;
    }
    *(volatile uint8_t *)row = (uint8_t)(*(volatile uint32_t *)row & 1u);
    return 0;
}

// === efuse_program_bit === 原 FUN_0000b6a4 @ 0x0000b6a4
// 来源: reversed/fsbl_funcs/FUN_0000b6a4.c | 分类: fsbl_funcs
// 语义: 编程一个 eFuse 位。地址保留 → 返回 1; 寄存器 bit0 已为 1 → 返回 0;
//       否则写 1 启动编程, 设定全局定时器超时 (0x96 = 150 个 DAT_00028f50
//       单位, 原 bcd8 调用第 2 参经 r1 传入行地址但该函数未使用), 忙等
//       timeout_expired 返回 1 (截止到达) 后返回 0。
int efuse_program_bit(uint32_t addr)
{
    int64_t deadline = 0;

    if (efuse_addr_is_reserved(addr) != 0) {
        return 1;
    }
    if ((*(volatile uint32_t *)(uintptr_t)addr & 1u) == 0u) {
        *(volatile uint32_t *)(uintptr_t)addr = 1u;
        /* 第 2 参原调用传 r1 = addr, bcd8 未使用该参数 (仅占位) */
        timeout_deadline_set(&deadline, addr, 0x96u, 0);
        do {
            ;                              /* 忙等截止到达 */
        } while (timeout_expired((uint32_t)((uint64_t)deadline & 0xffffffffu),
                                 (uint32_t)((uint64_t)deadline >> 32)) != 1);
    }
    return 0;
}

/* ==================== XADC (PS XADC 接口, 句柄 = 全局实例 DAT_00021be0) ==================== */

/* XADC 单例实例 (p1b.c 同款宏): 布局 +0x00 未知/+0x04 基地址指针/+0x08 魔数 0x11111111 */
#define DAT_00021be0      ((uint8_t *)0x00021be0)

/* XADC 内部寄存器接口偏移: 写命令寄存器 (reg<<16|0x4000000 读 / |0x8400000 写),
 * 读回数据寄存器 */
#define XADC_CMD_REG      0x10u
#define XADC_DATA_REG     0x14u

// === xadc_read_channels === 原 FUN_0000bb98 @ 0x0000bb98
// 来源: reversed/fsbl_funcs/FUN_0000bb98.c | 分类: fsbl_funcs
// 语义: 批量读取 XADC 通道到 7 字输出结构: out[0] = 通道 0 数据, out[1] =
//       supply 4 数据, out[2] = supply 0 数据; 再按 out[3] 中预设的模式值
//       (Ghidra 顺序: 先读 out[3] 后写 out[2]) 选组读 out[4..6]: 模式 2..5
//       时取 0x15CB8/0x15CBC/0x15CC0 三张字节表第 (模式-2) 项, 其余固定
//       (通道 0xd / supply 0xc / supply 8)。
void xadc_read_channels(uint32_t *out)
{
    uint32_t chan_sel;
    uint32_t supply_sel;
    uint32_t supply_sel2;
    uint32_t mode;

    if (out == (void *)0) {
        return;
    }
    out[0] = xadc_chan_data_get((int)(uintptr_t)DAT_00021be0, 0);
    out[1] = xadc_supply_data_get((int)(uintptr_t)DAT_00021be0, 4);
    mode = out[3];                          /* 原反编译在写 out[2] 前读取 */
    out[2] = xadc_supply_data_get((int)(uintptr_t)DAT_00021be0, 0);
    if (mode - 2u < 4u) {
        /* 三张字节表位于 0x15CB8/0x15CBC/0x15CC0 (Ghidra 记 DAT_00015cb6/
         * 00015cba/00015cbe, 索引 = 模式值)。模式 2/3 取值均为 0; 模式 4/5
         * 取出 0xE0/0xF0/0x34/0x35 等超出常规通道域的值, 语义待定。 */
        static const uint8_t tab_chan[4]    = { 0x00u, 0x00u, 0xE0u, 0x34u };
        static const uint8_t tab_supply[4]  = { 0x00u, 0x00u, 0xF0u, 0x34u };
        static const uint8_t tab_supply2[4] = { 0x00u, 0x00u, 0x00u, 0x35u };
        chan_sel    = tab_chan[mode - 2u];
        supply_sel  = tab_supply[mode - 2u];
        supply_sel2 = tab_supply2[mode - 2u];
    } else {
        chan_sel = 0xd;
        supply_sel = 0xc;
        supply_sel2 = 8;
    }
    out[4] = xadc_chan_data_get((int)(uintptr_t)DAT_00021be0, chan_sel);
    out[5] = xadc_supply_data_get((int)(uintptr_t)DAT_00021be0, supply_sel);
    out[6] = xadc_supply_data_get((int)(uintptr_t)DAT_00021be0, supply_sel2);
}

// === cpu_freq_detect === 原 FUN_0000c024 @ 0x0000c024
// 来源: reversed/fsbl_funcs/FUN_0000c024.c | 分类: fsbl_funcs
// 语义: 读取 PLL 分频并换算定时器 tick 系数: arm_fdiv = ARM_PLL_CTRL
//       (0xF8000100) 的 (值&0x7ffff)>>12, io_fdiv = IO_PLL_CTRL (0xF8000120)
//       的 (值&0x3fff)>>8。返回 (io_fdiv * 0x27bc86bf) / arm_fdiv (无符号除法,
//       0x27bc86bf 语义待定), 并把 DAT_00028f50 = ((返回值*arm_fdiv)/io_fdiv)
//       / 20000000 写入定时器换算系数 (进入函数时先清 0)。
int cpu_freq_detect(void)
{
    uint32_t arm_fdiv;
    uint32_t io_fdiv;
    uint32_t tick;
    int32_t ret;

    DAT_00028f50 = 0;
    arm_fdiv = (*(volatile uint32_t *)0xF8000100u & 0x7ffffu) >> 0xc;
    io_fdiv = (*(volatile uint32_t *)0xF8000120u & 0x3fffu) >> 8;
    ret = (int32_t)((uint32_t)(io_fdiv * 0x27bc86bFu) / arm_fdiv);  /* __aeabi_uidiv */
    tick = (uint32_t)((uint32_t)ret * arm_fdiv) / io_fdiv;
    DAT_00028f50 = tick / 20000000u;
    return ret;
}

// === xadc_selftest === 原 FUN_0000c240 @ 0x0000c240
// 来源: reversed/fsbl_funcs/FUN_0000c240.c | 分类: fsbl_funcs
// 语义: XADC 自检: 报警阈值 1 (reg 0x51) 写 0x55 后回读, 读回值 == 0x55 视为
//       通过返回 0, 否则返回 1。写前后用 xadc_cfg2_pulse 触发 CFR2。句柄为空
//       或实例魔数 (+0x08) != 0x11111111 时打印 (0x1728C, 0x69/0x6A) 置断言
//       标志返回 0。
int xadc_selftest(int xadc)
{
    uint32_t readback;

    if (xadc == 0) {
        dbg_print(0x0001728Cu, 0x69u);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(volatile uint32_t *)(uintptr_t)(xadc + 8) == 0x11111111u) {
        xadc_cfg2_pulse(xadc);
        xadc_alarm_thr_set(xadc, 1u, 0x55u);
        readback = xadc_alarm_thr_get(xadc, 1u);
        xadc_cfg2_pulse(xadc);
        return readback != 0x55u;
    }
    dbg_print(0x0001728Cu, 0x6Au);
    g_assert_failed = 1;
    return 0;
}

// === xadc_cfg2_get === 原 FUN_0000c5d0 @ 0x0000c5d0
// 来源: reversed/fsbl_funcs/FUN_0000c5d0.c | 分类: fsbl_funcs
// 语义: 读 CFBR2 寄存器原值 (基地址 +0x18)。句柄为空/魔数不符时打印
//       (0x172A0, 0x11D/0x11E) 置断言标志并返回 0。
uint32_t xadc_cfg2_get(int xadc)
{
    if (xadc == 0) {
        dbg_print(0x000172A0u, 0x11Du);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(volatile uint32_t *)(uintptr_t)(xadc + 8) == 0x11111111u) {
        return *(volatile uint32_t *)(uintptr_t)
               (*(volatile int32_t *)(uintptr_t)(xadc + 4) + 0x18);
    }
    dbg_print(0x000172A0u, 0x11Eu);
    g_assert_failed = 1;
    return 0;
}

// === xadc_cfg0_field_get === 原 FUN_0000ca3c @ 0x0000ca3c
// 来源: reversed/fsbl_funcs/FUN_0000ca3c.c | 分类: fsbl_funcs
// 语义: 读 CFR0 (reg 0x40) 的 [13:12] 位字段 (平均模式选择): 写命令
//       0x4400000 后经 DATA 触发转换, 返回 (数据 & 0x3fff) >> 12。句柄为空/
//       魔数不符时打印 (0x172A0, 0x20A/0x20B) 置断言标志返回 0。
uint32_t xadc_cfg0_field_get(int xadc)
{
    volatile uint32_t *base;

    if (xadc == 0) {
        dbg_print(0x000172A0u, 0x20Au);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(volatile uint32_t *)(uintptr_t)(xadc + 8) == 0x11111111u) {
        base = (volatile uint32_t *)(uintptr_t)*(volatile int32_t *)(uintptr_t)(xadc + 4);
        base[XADC_CMD_REG / 4] = 0x4400000u;             /* 读 reg 0x40 (CFR0) */
        base[XADC_CMD_REG / 4] = base[XADC_DATA_REG / 4];
        return (base[XADC_DATA_REG / 4] & 0x3fffu) >> 0xc;
    }
    dbg_print(0x000172A0u, 0x20Bu);
    g_assert_failed = 1;
    return 0;
}

// === xadc_set_single_chan_params === 原 FUN_0000cf5c @ 0x0000cf5c
// 来源: reversed/fsbl_funcs/FUN_0000cf5c.c | 分类: fsbl_funcs
// 语义: 设置 CFR0 单通道采集参数 (reg 0x40 写): 保留原 [13:12] 平均位,
//       flag1==1 置 0x100, flag2==1 置 0x200, flag3==1 置 0x400, 通道号取
//       [4:0], 命令 0x8400000 (写 CFR0)。通道合法域: 0..5/6/8/0xd..0x1f,
//       三个标志均须 <= 1, 序列模式 (xadc_seq_mode_get) 必须为 3, flag 置位
//       时通道须为 3/0x1e/0x1f。各校验失败打印 (0x172A0, 0x24A..0x254) 置
//       断言标志返回 0, 模式不符/通道不符返回 1。
uint32_t xadc_set_single_chan_params(int xadc, uint32_t chan, uint32_t flag1,
                                    uint32_t flag2, uint32_t flag3)
{
    volatile uint32_t *base;
    uint32_t cfg;
    int in_range;
    int aux_ok;

    if (xadc == 0) {
        dbg_print(0x000172A0u, 0x24Au);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(volatile uint32_t *)(uintptr_t)(xadc + 8) != 0x11111111u) {
        dbg_print(0x000172A0u, 0x24Bu);
        g_assert_failed = 1;
        return 0;
    }
    in_range = (chan > 7u);
    if (chan != 8u) {
        in_range = (chan > 5u);
    }
    if (in_range && chan != 8u && chan != 6u && (chan - 0xdu) > 0x12u) {
        dbg_print(0x000172A0u, 0x24Fu);
        g_assert_failed = 1;
        return 0;
    }
    if (flag1 > 1u) {
        dbg_print(0x000172A0u, 0x251u);
        g_assert_failed = 1;
        return 0;
    }
    if (flag2 > 1u) {
        dbg_print(0x000172A0u, 0x252u);
        g_assert_failed = 1;
        return 0;
    }
    if (flag3 > 1u) {
        dbg_print(0x000172A0u, 0x254u);
        g_assert_failed = 1;
        return 0;
    }
    if (xadc_seq_mode_get(xadc) != 3) {
        return 1;
    }
    base = (volatile uint32_t *)(uintptr_t)*(volatile int32_t *)(uintptr_t)(xadc + 4);
    base[XADC_CMD_REG / 4] = 0x4400000u;                 /* 读 reg 0x40 (CFR0) */
    base[XADC_CMD_REG / 4] = base[XADC_DATA_REG / 4];
    cfg = base[XADC_DATA_REG / 4] & 0x3000u;             /* 保留平均位 [13:12] */
    aux_ok = (chan > 2u);
    if (chan != 3u) {
        aux_ok = ((chan - 0x10u) > 0xEu);
    }
    if (flag1 == 1u) {
        if (aux_ok && chan != 3u && (chan - 0x10u) != 0xFu) {
            return 1;
        }
        cfg |= 0x100u;
        if (flag3 != 1u) {
            goto set_flags;                              /* 原 LAB_0000d09c */
        }
    } else {
        if (flag3 != 1u) {
            goto set_flags;                              /* 原 LAB_0000d09c */
        }
        if (aux_ok && chan != 3u && (chan - 0x10u) != 0xFu) {
            return 1;
        }
    }
    cfg |= 0x400u;
set_flags:                                               /* 原 LAB_0000d09c */
    if (flag2 == 1u) {
        cfg |= 0x200u;
    }
    base[XADC_CMD_REG / 4] = (chan & 0x1fu) | 0x8400000u | cfg;
    return 0;
}

// === xadc_cfg2_divider_get === 原 FUN_0000d190 @ 0x0000d190
// 来源: reversed/fsbl_funcs/FUN_0000d190.c | 分类: fsbl_funcs
// 语义: 读 CFR2 (reg 0x42) 的平均除数位 [15:8]: 写命令 0x4420000 后经 DATA
//       触发转换, 返回 (数据 & 0xffff) >> 8。句柄为空/魔数不符时打印
//       (0x172A0, 0x3D8/0x3D9) 置断言标志返回 0。
uint32_t xadc_cfg2_divider_get(int xadc)
{
    volatile uint32_t *base;

    if (xadc == 0) {
        dbg_print(0x000172A0u, 0x3D8u);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(volatile uint32_t *)(uintptr_t)(xadc + 8) == 0x11111111u) {
        base = (volatile uint32_t *)(uintptr_t)*(volatile int32_t *)(uintptr_t)(xadc + 4);
        base[XADC_CMD_REG / 4] = 0x4420000u;             /* 读 reg 0x42 (CFR2) */
        base[XADC_CMD_REG / 4] = base[XADC_DATA_REG / 4];
        return (base[XADC_DATA_REG / 4] & 0xffffu) >> 8;
    }
    dbg_print(0x000172A0u, 0x3D9u);
    g_assert_failed = 1;
    return 0;
}

// === xadc_seq_ch1_get === 原 FUN_0000d498 @ 0x0000d498
// 来源: reversed/fsbl_funcs/FUN_0000d498.c | 分类: fsbl_funcs
// 语义: 读序列寄存器对 SEQ1 (reg 0x4A) / SEQ2 (reg 0x4B), 返回 32 位组合:
//       (SEQ1 & 0x7fe0) | (SEQ2 << 16)。句柄为空/魔数不符时打印
//       (0x172A0, 0x496/0x497) 置断言标志返回 0。
uint32_t xadc_seq_ch1_get(int xadc)
{
    volatile uint32_t *base;
    uint32_t seq1;

    if (xadc == 0) {
        dbg_print(0x000172A0u, 0x496u);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(volatile uint32_t *)(uintptr_t)(xadc + 8) == 0x11111111u) {
        base = (volatile uint32_t *)(uintptr_t)*(volatile int32_t *)(uintptr_t)(xadc + 4);
        base[XADC_CMD_REG / 4] = 0x44a0000u;             /* 读 reg 0x4A (SEQ1) */
        base[XADC_CMD_REG / 4] = base[XADC_DATA_REG / 4];
        seq1 = base[XADC_DATA_REG / 4];
        base[XADC_CMD_REG / 4] = 0x44b0000u;             /* 读 reg 0x4B (SEQ2) */
        base[XADC_CMD_REG / 4] = base[XADC_DATA_REG / 4];
        return (seq1 & 0x7fe0u) | (base[XADC_DATA_REG / 4] << 0x10);
    }
    dbg_print(0x000172A0u, 0x497u);
    g_assert_failed = 1;
    return 0;
}

// === xadc_avg_ch1_set === 原 FUN_0000d6f4 @ 0x0000d6f4
// 来源: reversed/fsbl_funcs/FUN_0000d6f4.c | 分类: fsbl_funcs
// 语义: 写平均设置寄存器对 AVG1 (reg 0x4E) / AVG2 (reg 0x4F):
//       CMD = (param & 0x800) | 0x84E0000, CMD = (param >> 16) | 0x84F0000。
//       仅当序列模式 (xadc_seq_mode_get) == 0 时执行并返回 0, 否则返回 1。
//       句柄为空/魔数不符时打印 (0x172A0, 0x526/0x527) 置断言标志返回 0。
uint32_t xadc_avg_ch1_set(int xadc, uint32_t param)
{
    volatile uint32_t *base;

    if (xadc == 0) {
        dbg_print(0x000172A0u, 0x526u);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(volatile uint32_t *)(uintptr_t)(xadc + 8) != 0x11111111u) {
        dbg_print(0x000172A0u, 0x527u);
        g_assert_failed = 1;
        return 0;
    }
    if (xadc_seq_mode_get(xadc) == 0) {
        base = (volatile uint32_t *)(uintptr_t)*(volatile int32_t *)(uintptr_t)(xadc + 4);
        base[XADC_CMD_REG / 4] = (param & 0x800u) | 0x84e0000u;
        base[XADC_CMD_REG / 4] = (param >> 0x10) | 0x84f0000u;
        return 0;
    }
    return 1;
}

/* ==================== 字符串工具 ==================== */

// === strchr === 原 FUN_0000dfb0 @ 0x0000dfb0
// 来源: reversed/fsbl_funcs/FUN_0000dfb0.c | 分类: fsbl_funcs
// 语义: libc strchr (newlib ARM 字对齐优化版)。c 截取低 8 位; c==0 时定位
//       字符串结尾 NUL: 先按字节对齐到字边界, 再用零字节检测技巧
//       ((x - 0x01010101) & ~x, 0xfefefeff 即 -0x01010101) 按字扫描, 命中后
//       逐字节确定 NUL 位置。c!=0 时构造 4 字重复掩码, 同一技巧按字检测
//       "含 NUL 或 c" 后逐字节判别。未找到返回 0。
//       注: p4.c 中以 str_chr 名字外引同一 FUN_0000dfb0, 本文件按 name_map
//       冻结名为 strchr。
char *strchr(const char *haystack, int c)
{
    const volatile uint32_t *wp;
    const volatile uint8_t *bp;
    uint32_t mask;
    uint32_t word;
    uint32_t test;

    bp = (const volatile uint8_t *)(uintptr_t)haystack;
    c &= 0xff;
    if (c == 0) {
        /* 定位 NUL 结束符 */
        if (((uintptr_t)bp & 3u) != 0u) {
            if (*bp == 0u) {
                return (char *)(uintptr_t)bp;
            }
            do {
                bp = bp + 1;
                if (*bp == 0u) {
                    return (char *)(uintptr_t)bp;
                }
            } while (((uintptr_t)bp & 3u) != 0u);
        }
        wp = (const volatile uint32_t *)(uintptr_t)bp;
        word = *wp;
        while (((word + 0xfefefeffu) & ~word & 0x80808080u) == 0u) {
            wp = wp + 1;
            word = *wp;
        }
        bp = (const volatile uint8_t *)(uintptr_t)wp;
        if (*bp == 0u) {
            return (char *)(uintptr_t)bp;
        }
        do {
            bp = bp + 1;
        } while (*bp != 0u);
        return (char *)(uintptr_t)bp;
    }
    /* 定位字符 c */
    if (((uintptr_t)bp & 3u) != 0u) {
        if (*bp == 0u) {
            return (char *)0;
        }
        if ((uint32_t)c == (uint32_t)*bp) {
            return (char *)(uintptr_t)bp;
        }
        while (((uintptr_t)bp & 3u) != 0u) {
            bp = bp + 1;
            if (*bp == 0u) {
                return (char *)0;
            }
            if ((uint32_t)c == (uint32_t)*bp) {
                return (char *)(uintptr_t)bp;
            }
        }
    }
    mask = (uint32_t)c | ((uint32_t)c << 8);
    mask = mask | (mask << 0x10);
    wp = (const volatile uint32_t *)(uintptr_t)bp;
    for (;;) {
        word = *wp;
        test = word ^ mask;                       /* c 字节处为 0 */
        /* 字内含 NUL 或 c ⇒ 置位任一最高位 */
        if ((((word + 0xfefefeffu) & ~word) | ((test + 0xfefefeffu) & ~test)) & 0x80808080u) {
            break;
        }
        wp = wp + 1;
    }
    bp = (const volatile uint8_t *)(uintptr_t)wp;
    if (*bp != 0u) {
        if ((uint32_t)c == (uint32_t)*bp) {
            return (char *)(uintptr_t)bp;
        }
        for (;;) {
            bp = bp + 1;
            if (*bp == 0u) {
                break;
            }
            if ((uint32_t)c == (uint32_t)*bp) {
                return (char *)(uintptr_t)bp;
            }
        }
    }
    return (char *)0;
}

/* ==================== 覆盖率表 (42/42) ==================== */
/*
 * | #  | 函数名                    | 原 FUN          | 地址        | 一行语义                                        |
 * |  1 | Reset                    | FUN_00000000    | 0x00000000  | 复位向量 (指令流损坏: 仅含 memcpy 调用后挂死)   |
 * |  2 | DataAbort                | FUN_00000010    | 0x00000010  | 数据中止向量 (指令流损坏: 仅含 memcpy 后挂死)   |
 * |  3 | dbg_print                | FUN_000014e4    | 0x000014e4  | 注册式调试打印 + 等待发送忙标志清零             |
 * |  4 | nand_reg80_get           | FUN_00000c1c    | 0x00000c1c  | 读 NAND 实例 +0x80 寄存器值                     |
 * |  5 | nand_timing_cfg_init     | FUN_000019fc    | 0x000019fc  | 按 ONFI 时序模式写 NAND 时序寄存器              |
 * |  6 | nand_onfi_probe          | FUN_0000158c    | 0x0000158c  | ONFI 参数页探测: CRC16 校验/几何/ECC 模式选择   |
 * |  7 | exit_run_handlers        | FUN_000029dc    | 0x000029dc  | newlib 风格 atexit 处理器链遍历执行             |
 * |  8 | fat_get_fat              | FUN_000036fc    | 0x000036fc  | FAT 表项读取 (FAT12/16/32) 含窗口换入           |
 * |  9 | fat_remove_chain         | FUN_00003a48    | 0x00003a48  | 释放 FAT 簇链 (含链变化重启遍历)                |
 * | 10 | fat_dir_next             | FUN_00003c84    | 0x00003c84  | 目录游标推进到下一目录项                        |
 * | 11 | fat_dir_sdi0             | FUN_00003d50    | 0x00003d50  | 目录项索引定位 (dir_sdi), 0 号簇处理            |
 * | 12 | fat_f_write              | FUN_00004910    | 0x00004910  | f_write: 整扇区/部分扇区写 + 簇链分配 + 缓存回写 |
 * | 13 | fat_f_closedir           | FUN_000050f0    | 0x000050f0  | f_closedir: 校验目录对象并断开关联              |
 * | 14 | fat_f_mkdir              | FUN_00005618    | 0x00005618  | f_mkdir: 建目录簇 + '.'/'..' 项 + 注册父目录    |
 * | 15 | fat_default_date         | FUN_00006760    | 0x00006760  | 缺省日期打包 (固定 0x4971: 2014-01-01)          |
 * | 16 | disk_read                | FUN_0000667c    | 0x0000667c  | 磁盘层读 (pdrv 界定 + 委托 slot 读)             |
 * | 17 | disk_ioctl               | FUN_000066f8    | 0x000066f8  | 磁盘层 IOCTL (CTRL_SYNC 等, 大多直接成功)       |
 * | 18 | sd_slot_cfg_get          | FUN_000067e8    | 0x000067e8  | 读 SD 槽位配置结构                              |
 * | 19 | sd_host_init             | FUN_00006804    | 0x00006804  | SDHCI 主机控制器初始化 (寄存器全序列)           |
 * | 20 | sd_card_select           | FUN_000070e0    | 0x000070e0  | CMD7 选中/取消卡                                |
 * | 21 | sd_data_read             | FUN_000072c8    | 0x000072c8  | 多块读数据流 (ADMA + 命令完成轮询)              |
 * | 22 | sd_send_scr              | FUN_00007dfc    | 0x00007dfc  | ACMD51 读 SCR 寄存器                            |
 * | 23 | sdmmc_switch_high_speed  | FUN_00008428    | 0x00008428  | CMD6 切高速 + 执行调谐循环 (CMD19/CMD21)        |
 * | 24 | mmc_send_ext_csd         | FUN_0000884c    | 0x0000884c  | CMD8 (SEND_EXT_CSD) 读扩展 CSD                  |
 * | 25 | rsa_bignum_sub           | FUN_00008eac    | 0x00008eac  | 64 字大数原地减法 (借位链)                      |
 * | 26 | rsa_modexp_u32exp        | FUN_00008f80    | 0x00008f80  | Montgomery 模幂 (32 位指数, 平方-乘法)          |
 * | 27 | sha256_update            | FUN_000092c8    | 0x000092c8  | SHA-256 增量更新 (缓冲 + 大端字装载)            |
 * | 28 | sha256_transform         | FUN_000099a0    | 0x000099a0  | SHA-256 块压缩 (轮常量表取 0x15BB8, 非标准 K)   |
 * | 29 | efuse_setup_timing       | FUN_0000aedc    | 0x0000aedc  | eFuse 时钟控制/定时寄存器配置                   |
 * | 30 | efuse_addr_is_reserved   | FUN_0000b254    | 0x0000b254  | eFuse 保留地址判定 (排除表 + 范围比较)          |
 * | 31 | efuse_write_bit          | FUN_0000b680    | 0x0000b680  | 非保留行 bit0 掩码写                            |
 * | 32 | efuse_program_bit        | FUN_0000b6a4    | 0x0000b6a4  | eFuse 编程 + 150 单位定时器忙等                 |
 * | 33 | xadc_read_channels       | FUN_0000bb98    | 0x0000bb98  | 批量读 XADC 通道/供电数据到 7 字结构            |
 * | 34 | cpu_freq_detect          | FUN_0000c024    | 0x0000c024  | PLL 分频换算定时器 tick 系数 DAT_00028f50       |
 * | 35 | xadc_selftest            | FUN_0000c240    | 0x0000c240  | 阈值 0x51 写 0x55 回读自检                      |
 * | 36 | xadc_cfg2_get            | FUN_0000c5d0    | 0x0000c5d0  | 读 CFBR2 原值 (+0x18)                           |
 * | 37 | xadc_cfg0_field_get      | FUN_0000ca3c    | 0x0000ca3c  | 读 CFR0 [13:12] 平均模式字段                    |
 * | 38 | xadc_set_single_chan_params | FUN_0000cf5c | 0x0000cf5c  | CFR0 单通道参数写 (通道/标志域校验)             |
 * | 39 | xadc_cfg2_divider_get    | FUN_0000d190    | 0x0000d190  | 读 CFR2 [15:8] 平均除数                         |
 * | 40 | xadc_seq_ch1_get         | FUN_0000d498    | 0x0000d498  | SEQ1/SEQ2 寄存器对 32 位组合读                  |
 * | 41 | xadc_avg_ch1_set         | FUN_0000d6f4    | 0x0000d6f4  | AVG1/AVG2 寄存器对写 (模式 0 时)                |
 * | 42 | strchr                   | FUN_0000dfb0    | 0x0000dfb0  | libc strchr (字对齐 NUL/字符扫描)               |
 */
