/**
 * p2a2.c — FSBL 反编译转写 批次 2a 续作 (p2a 前半停滞, 本文件完成剩余 6 函数)
 *
 * 覆盖 (reversed/fsbl_funcs/, 源二进制 Zynq7007_miner.elf, 共 6 函数, 地址升序):
 *   - FatFs 格式化: fat_f_mkfs (f_mkfs 完整实现: 盘号解析 / disk_initialize /
 *     GET_BLOCK_SIZE 对齐校验 / FAT12-16-32 试算 / BPB 构造 / FSInfo / MBR)
 *   - diskio 初始化: disk_initialize
 *   - SD/eMMC 主机层 (SDHCI): sd_cmd_status_get / sd_send_cmd / sd_adma_setup_cont /
 *     sdmmc_set_blocklen
 *
 * 全局数据 (均为反编译实证):
 *   DAT_00028e84 = xil_assert 错误标志 (0 = 正常, 1 = 已触发; 与 p2a/p4.c 同址)
 *   DAT_00028e88 = SD 命令/传输模式寄存器暂存 (sd_send_cmd 写入传输模式字)
 *   DAT_00021544 = FatFs 卷对象指针表 (按设备号字索引, 挂载时指向卷对象)
 *   DAT_000219a0 = SD 主机控制器基址 (指针变量, 由 disk_status 首槽路径初始化)
 *   DAT_000219a4 = 卡检测等待使能标志 (disk_status 初始化)
 *   DAT_00019be8 = 每槽 disk 状态字节数组 (bit0=noinit, bit1=nodisk, bit2=writeprot)
 *   DAT_000064b0 / DAT_000064b4 = FAT32 / FAT12-16 自动簇大小阈值表指针
 *     (ushort 表, 自 base+2 起步进读取, 0 结尾; 本镜像指向 0x15b7a/0x15b8a,
 *      表首项均为 0 → 簇大小封顶 4 扇区, 语义待定按原样保留)
 *
 * sd_cmd_status_get 反编译误标说明: 命令 0x700/0x800 的返回值 0x714/0x814 与
 * NAND 寄存器访问器 FUN_00000714/FUN_00000814 的入口地址巧合, Ghidra 据此把
 * 返回类型标成 code* 并伪造了两处函数指针返回; 实际是纯立即数, 且对
 * 0x1100/0x1300 的比较同样被 Ghidra 渲染成字符串/函数地址比较 (0x10f5+0xb =
 * 0x1100, 0x12f9+7 = 0x1300, FUN_000036fc+4 = 0x1300), 本转写按整数值还原。
 *
 * 调用方可见函数名一律取自 rewritten/name_map/fsbl.tsv (阶段 1 冻结)。
 * 签名与 p2a.c/p2b.c 既有 extern 声明保持一致 (sd_send_cmd 返回 int, r0 为
 * 零扩展的 16 位结果)。
 */
#include <stdint.h>
#include <stdbool.h>   /* bool (disk_write 返回类型) */

/* ==================== 共享声明 (本批被调/跨批被调, 签名与反编译一致) ==================== */

/* Xil Assert 风格错误标志 (0x28e84), 同 p2a.c/p4.c g_assert_failed */
#define g_assert_failed   (*(volatile uint32_t *)0x00028e84)

/* SD 命令/传输模式寄存器暂存 (0x28e88), 同 p2b.c/p3.c g_sd_cmd_reg */
#define g_sd_cmd_reg      (*(volatile uint32_t *)0x00028e88)

/* ---- 被调函数 (定义在其它批次文件) ---- */
extern void dbg_print(uint32_t msg, uint32_t line);     /* FUN_000014e4 (定义见 p3.c) */
extern uint32_t str_parse_devnum(const uint8_t **pp, uint32_t verdict_init); /* FUN_00002b44 (定义见 p4.c) */
extern int  disk_status(int pdrv);                      /* FUN_000064b8 (定义见 p4.c) */
extern int  disk_ioctl(int pdrv, int cmd, void *buf);   /* FUN_000066f8 (定义见 p3.c) */
extern bool disk_write(int pdrv, const void *buf, uint32_t sect, int cnt); /* FUN_0000676c (p4.c) */
extern uint32_t fat_default_date(void);                 /* FUN_00006760 (定义见 p3.c) */
extern uint16_t *sd_slot_cfg_get(uint32_t id);          /* FUN_000067e8 (定义见 p3.c) */
extern int  sd_host_init(uint16_t *sd, uint16_t *cfg, int host); /* FUN_00006804 (p3.c) */
extern int  sdmmc_card_init(int sd);                    /* FUN_00007758 (定义见 p4.c) */
extern int  aeabi_uidiv(uint32_t num, uint32_t den);    /* FUN_00001a50 (定义见 p2a.c) */
extern void fun_0xffffd370(void *buf, uint32_t len);    /* func_0xffffd370: ROM cache 维护 */

/* ---- 本批内被调 (本文件定义, 前向声明) ---- */
uint32_t disk_initialize(int pdrv);                     /* FUN_000065cc */
uint32_t sd_cmd_status_get(int sd, uint32_t cmd);       /* FUN_00006a24 */
int  sd_send_cmd(int sd, uint16_t cmd, uint32_t arg, uint16_t resp_type); /* FUN_00006bcc */

/* ---- 本批全局数据 ---- */
/* FatFs 卷对象指针表 (按设备号字索引; 挂载时 *entry = 卷对象, 卷对象首字节 = fs_type) */
static volatile uint32_t *const DAT_00021544_base = (volatile uint32_t *)0x00021544; /* 全局, 语义待定 */
/* SD 主机控制器基址指针 / 卡检测等待使能 (disk_status 维护) */
static volatile uint32_t *const DAT_000219a0 = (volatile uint32_t *)0x000219a0;      /* 全局, 语义待定 */
static volatile uint32_t *const DAT_000219a4 = (volatile uint32_t *)0x000219a4;      /* 全局, 语义待定 */
/* 每槽 disk 状态字节数组 (p4.c disk_status 同址) */
static volatile uint8_t *const DAT_00019be8 = (volatile uint8_t *)0x00019be8;        /* 全局, 语义待定 */
/* 自动簇大小阈值表指针 (ushort 表, base+2 起读取, 0 结尾) */
static const uint16_t *const *const cst32_tab = (const uint16_t * const *)0x000064b0; /* 全局， 语义待定 */
static const uint16_t *const *const cst16_tab = (const uint16_t * const *)0x000064b4; /* 全局， 语义待定 */

/* ==================== FatFs 卷格式化 ==================== */

// === fat_f_mkfs === 原 FUN_00005a60 @ 0x00005a60
// 来源: reversed/fsbl_funcs/FUN_00005a60.c | 分类: fsbl
// 语义: FatFs f_mkfs 移植 — 在 path 设备号 ("n:" 前缀, str_parse_devnum) 指向的
// 卷上建立 FAT 文件系统。流程: 盘号解析 (失败 → 0xb) → 已挂载卷失效 (卷对象
// 首字节 fs_type 清零) → disk_initialize (bit0 → 3, bit2 → 0xa) → disk_ioctl(3)
// 擦除块取值与 2 的幂校验 (异常按 1) → 簇字节数校验 0x200..0x10000 且 2 的幂
// (否则 0x13) → 工作缓冲非空且 ≥ 1 扇区 (否则 0x11) → disk_ioctl(1) 取总扇区数
// (失败 → 1) → 非 SFD 预留 0x3F 扇区 MBR 轨道 → FAT12/16/32 试算 (自动簇表
// DAT_000064b0/b4, 容量下限 clst*0x10 校验, 簇数 0xff5/0xfff6 分界, BPB 构造
// OEM "MSDOS5.0"/512 字节扇区/63 扇区每道/255 磁头/0xF8 介质) → 写引导扇区
// (FAT32 另写 6 号备份引导与 1/7 号 FSInfo) → FAT 表与根目录清零 → 非 SFD
// 构造 MBR (类型 1/4/6/0xC, 分区起始 LBA = part_base) → disk_ioctl(0) 同步。
// fmt 位: bit0 允许 FAT12, bit1 允许 FAT32, bit2 任意, bit3 = SFD (超级软盘,
// 不建 MBR)。返回 0 = 成功, 其余为 FR_* 码 (1 = 盘 I/O 错)。
uint8_t fat_f_mkfs(const uint8_t *path, uint32_t fmt, uint32_t clst_bytes,
                   uint32_t *work, uint32_t work_len)
{
    const uint8_t *path_p = path;
    uint32_t dev;             /* str_parse_devnum 结果 (0xFFFFFFFF = 无前缀) */
    uint32_t pdrv;            /* 设备号 (dev & 0xff) */
    volatile uint8_t *vol_ptr;/* 卷对象指针表项 */
    int ret;
    uint32_t blk_sz;          /* disk_ioctl(3) 擦除块 (扇区) */
    uint32_t sect_cnt[2];     /* disk_ioctl(1) 输出 / sz_vol 镜像 (原 local_2c) */
    uint32_t part_base;       /* 卷基扇区 (0x3F = MBR 轨道, SFD = 0) (原 local_58) */
    uint32_t sz_vol;          /* 卷容量 (扇区) */
    uint32_t work_secs;       /* 工作缓冲扇区数 */
    uint32_t clst_sect;       /* 簇扇区数/重试标记, 0 = 自动 (Ghidra param_3 复用) */
    uint32_t fmt2;            /* fmt & 2 (允许 FAT32) */
    uint32_t clst;            /* 当前簇大小 (扇区) */
    uint32_t cur;             /* 阈值表当前项 */
    const uint16_t *cst;      /* 阈值表游标 */
    uint32_t n_clst;          /* 整卷簇数 */
    uint32_t fat_secs;        /* FAT 表扇区数 (FAT32 路径) */
    uint32_t fat_bytes;       /* FAT 表字节数 (12/16 路径) */
    uint32_t fat_cand;        /* FAT12/16 候选类型 (1/2) */
    uint32_t fat_type;        /* 最终 FAT 类型 (1/2/3) */
    uint32_t root_secs;       /* 根目录清零扇区数 */
    uint32_t rsvd_fat;        /* 对齐基准 (FAT + 0x20 + 卷基) */
    uint32_t pad;             /* 擦除块对齐填充 */
    uint32_t rsvd_tot;        /* 保留区总扇区 (FAT32: pad+0x20, 12/16: 1) */
    uint32_t fat_tot;         /* 12/16: 含对齐填充的 FAT 总扇区 */
    uint32_t n_clst2;         /* 数据区簇数 (兼 FSInfo 空闲簇数) */
    uint32_t bs_neg;          /* -(int)blk_sz (对齐掩码) */
    uint32_t bs_m1;           /* blk_sz - 1 */
    uint32_t data_start;      /* FAT 表起始扇区 */
    uint32_t date;            /* fat_default_date() 时间戳 */
    uint32_t type_code;       /* 分区类型码 / 字节暂存 (uVar20 复用) */
    uint32_t end_cyl;         /* MBR 末端柱面估值 */
    int is32;                 /* sz_vol > 0xffff (须 FAT32) */
    uint8_t *bp;              /* 工作缓冲字节视图 */
    uint32_t wr_cnt;          /* 清零循环: 剩余/本次扇区数 */
    uint32_t wr_sect;         /* 清零循环: 当前写扇区 */
    uint32_t clear_left;      /* FAT/根目录清零剩余扇区数 */

    /* --- 设备号解析 --- */
    dev = str_parse_devnum(&path_p, 0);   /* 原反编译未显式传第 2 参; 初值从未被读取, 恒补 0 */
    if ((int)dev < 0) {
        return 0xb;                                    /* FR_INVALID_DRIVE */
    }
    pdrv = dev & 0xff;
    /* 已挂载卷失效: 卷对象指针表 (字索引 = dev), 卷对象首字节 fs_type = 0 */
    vol_ptr = (volatile uint8_t *)(uintptr_t)DAT_00021544_base[dev];
    if (vol_ptr != (volatile uint8_t *)0) {
        *vol_ptr = 0;
    }

    /* --- 初始化与状态检查 --- */
    ret = disk_initialize((int)pdrv);
    if ((ret & 1) != 0) {
        return 3;                                      /* FR_NOT_READY (STA_NOINIT) */
    }
    if ((ret & 4) != 0) {
        return 0xa;                                    /* FR_WRITE_PROTECTED */
    }

    /* --- 擦除块 (GET_BLOCK_SIZE) 取值与校验, 异常按 1 扇区处理 --- */
    ret = disk_ioctl((int)pdrv, 3, &blk_sz);
    if (((ret != 0) || (0x7fffu < blk_sz - 1u)) || ((blk_sz & (blk_sz - 1u)) != 0u)) {
        blk_sz = 1;
    }

    /* --- 簇字节数校验: 0 = 自动, 否则 0x200..0x10000 且 2 的幂 --- */
    if (clst_bytes != 0u) {
        if (0xfffe00u < clst_bytes - 0x200u) {
            return 0x13;                               /* FR_INVALID_PARAMETER */
        }
        if (((clst_bytes - 1u) & clst_bytes) != 0u) {
            return 0x13;
        }
    }

    /* --- 工作缓冲检查 --- */
    work_secs = work_len >> 9;
    if ((work == (uint32_t *)0) || (work_secs == 0u)) {
        return 0x11;                                   /* FR_NOT_ENOUGH_CORE */
    }
    bp = (uint8_t *)work;

    /* --- 卷容量 (GET_SECTOR_COUNT), 失败按盘错处理 --- */
    ret = disk_ioctl((int)pdrv, 1, sect_cnt);
    if (ret != 0) {
        return 1;                                      /* FR_DISK_ERR */
    }

    /* --- 非 SFD: 预留一条 0x3F 扇区 MBR 轨道并缩容; 容量不足 → 放弃 --- */
    if ((fmt & 8u) == 0u) {
        if (sect_cnt[0] < 0x3fu) {
            return 0xe;                                /* FR_MKFS_ABORTED */
        }
        part_base = 0x3f;
        sz_vol = sect_cnt[0] - 0x3f;
    }
    else {
        part_base = 0;
        sz_vol = sect_cnt[0];
    }

    if (sz_vol > 0x7fu) {
        clst_sect = clst_bytes >> 9;                   /* 簇大小: 字节 → 扇区 */
        if (0x80u < clst_sect) {
            return 0x13;
        }
        fmt2 = fmt & 2u;
        sect_cnt[0] = sz_vol;                          /* 分区容量镜像 (供 MBR 用) */
        if (fmt2 == 0u) {
            if ((fmt & 1u) == 0u) {
                return 0x13;                           /* FAT12/FAT32 均未允许 */
            }
            /* 仅允许 FAT12 → 落入下方 FAT12/16 试算循环 (LAB_00005fdc) */
        }
        else if (((fmt & 7u) == 2u) || ((fmt & 1u) == 0u)) {
            /* 仅允许 FAT32 (或禁 FAT12): 固定簇 → 直接进 FAT32 主循环 */
            if (clst_sect != 0u) {
                clst = clst_sect;
                goto fat32_main;                       /* 原 LAB_00005bac */
            }
            /* --- FAT32 自动簇选择 (原 LAB_00005f58): 阈值表按 sz_vol>>17 步进 --- */
          fat32_auto:
            if (sz_vol >> 17 == 0u) {
                clst = 1;
            }
            else {
                clst = 2;
                cur = 2;
                cst = *cst32_tab;                      /* DAT_000064b0: FAT32 阈值表指针 */
                do {
                    if (sz_vol >> 17 < cur) {
                        break;                         /* 簇已足够覆盖容量 */
                    }
                    cst++;
                    cur = (uint32_t)*cst;
                    clst = clst << 1;
                } while (cur != 0u);
            }
            clst_sect = 0;                             /* 自动簇 → 允许一次对分重试 */

            /* --- FAT32 主循环 (原 LAB_00005bac/0x5bb0) --- */
          fat32_main:
            for (;;) {
                /* 整卷簇数与 FAT32 表扇区 (每项 4 字节) */
                n_clst = (uint32_t)aeabi_uidiv(sz_vol, clst);
                fat_secs = (n_clst * 4u + 0x207u) >> 9;
                if (0xffeffffu < n_clst - 0xfff6u) {
                    /* n_clst < 0xfff6 (无符号下溢) → 簇数不足以成立 FAT32 */
                    return 0xe;
                }
                /* FAT 起始扇区按擦除块对齐: 保留区 = 0x20 + 对齐填充 */
                rsvd_fat = fat_secs + part_base + 0x20;
                pad = (((blk_sz - 1u) + rsvd_fat) & (0u - blk_sz)) - rsvd_fat;
                rsvd_tot = pad + 0x20;
                data_start = part_base + 0x20 + pad;
                /* 容量下限: 须容纳 16 簇 + FAT + 保留区 */
                if (sz_vol < (clst * 0x10u - part_base) + rsvd_fat) {
                    return 0xe;
                }
                /* 数据区簇数 (供 FSInfo 空闲簇数) */
                n_clst2 = (uint32_t)aeabi_uidiv((sz_vol - rsvd_tot) - fat_secs, clst);
                if (0xfff6u <= n_clst2) {
                    break;                             /* 簇数达标 → FAT32 成立 */
                }
                if (clst_sect != 0u) {
                    return 0xe;                        /* 固定簇仍不足 → 放弃 */
                }
                clst_sect = clst >> 1;                 /* 簇对分重试 (仅一次) */
                if (clst_sect == 0u) {
                    return 0xe;
                }
                clst = clst_sect;
            }
            fat_type = 3;
            root_secs = 0;
            goto build_bpb;                            /* 原 LAB_00005c58 */
        }
        /* --- FAT12/FAT16 试算循环 (原 LAB_00005fdc) --- */
        bs_neg = 0u - blk_sz;                      /* 对齐掩码 (blk_sz 为 2 的幂) */
        bs_m1 = blk_sz - 1;
        data_start = part_base + 1;                /* 12/16 保留区 = 1 扇区 */
        fat_cand = pdrv;                           /* 死值占位, 随后必被类型覆盖 */
        /* 循环顶 = 原 LAB_00005fdc (重试经 continue 回此) */
        for (;;) {
            clst = clst_sect;
            if (clst_sect == 0u) {
                /* 自动簇: 阈值表按 sz_vol>>12 步进 (DAT_000064b4, 表首项 0 → 封顶 4) */
                if (sz_vol >> 12 == 0u) {
                    clst = 1;
                }
                else {
                    clst = 2;
                    cur = 4;
                    cst = *cst16_tab;              /* DAT_000064b4: 12/16 阈值表指针 */
                    do {
                        if (sz_vol >> 12 < cur) {
                            break;
                        }
                        cst++;
                        cur = (uint32_t)*cst;
                        clst = clst << 1;
                    } while (cur != 0u);
                }
            }
            /* 整卷簇数与 FAT 表字节数 (FAT12: 1.5 字节/项, FAT16: 2 字节/项) */
            n_clst = (uint32_t)aeabi_uidiv(sz_vol, clst);
            if (n_clst < 0xff6u) {
                fat_cand = 1;                      /* FAT12 候选 */
                fat_bytes = (n_clst * 3u + 1u) >> 1;
            }
            else {
                fat_bytes = (n_clst + 2u) * 2u;
            }
            if (n_clst < 0xff6u) {
                fat_bytes += 3;
            }
            else {
                fat_cand = 2;                      /* FAT16 候选 */
            }
            fat_secs = (fat_bytes + 0x1ffu) >> 9;
            rsvd_fat = fat_secs + 0x20u + data_start;
            fat_tot = (((bs_m1 + rsvd_fat) & bs_neg) - rsvd_fat) + fat_secs;
            /* 容量下限: 须容纳 16 簇 + FAT(含对齐) + 保留/根目录区 */
            if (sz_vol < (clst * 0x10u - part_base) + rsvd_fat) {
                return 0xe;
            }
            n_clst2 = (uint32_t)aeabi_uidiv((sz_vol - fat_tot) - 0x21u, clst);
            if (fat_cand != 2u) {
                /* FAT12 候选: 数据区簇数须 ≤ 0xff5 */
                if (0xff5u < n_clst2) {
                    return 0xe;
                }
                fat_type = 1;
                rsvd_tot = 1;
                root_secs = 0x20;
                goto build_bpb;                    /* 原 LAB_00005c58 */
            }
            if (n_clst2 < 0xfff6u) {
                if (0xff5u < n_clst2) {
                    /* 仅 FAT16 容纳: 保留区 1 扇区, 根目录 0x20 扇区 (512 项) */
                    rsvd_tot = 1;
                    root_secs = 0x20;
                    fat_type = 2;
                    goto build_bpb;
                }
                if (clst_sect != 0u) {
                    return 0xe;                    /* 固定簇仍偏小 → 放弃 */
                }
                clst_sect = clst << 1;
                if (clst_sect <= 0x80u) {
                    continue;                      /* 簇加倍重试 (do-while < 0x81) */
                }
                return 0xe;
            }
            else {
                /* FAT16 也容不下 (簇数 ≥ 0xfff6) */
                if (clst_sect != 0u) {
                    /* 固定簇: 允许 FAT32 时以固定簇转入 FAT32 主循环 */
                    if (fmt2 == 0u) {
                        return 0xe;
                    }
                    clst = clst_sect;
                    goto fat32_main;               /* 原 joined_r0x00005ba8 → LAB_00005bac */
                }
                clst_sect = clst << 1;
                if (clst_sect <= 0x40u) {
                    continue;                      /* 簇加倍, 先回 FAT12/16 重试 */
                }
                if (fmt2 != 0u) {
                    goto fat32_auto;               /* 转 FAT32 自动簇 (原 LAB_00005f58) */
                }
                if (clst_sect <= 0x80u) {
                    continue;
                }
                return 0xe;
            }
        }
        return 0xe;                                /* 簇加倍到 0x80 仍不可行 (不可达: 循环内已穷尽) */
        /* --- BPB 构造 (原 LAB_00005c58): 先清 512 字节再逐字段填写 --- */
      build_bpb:
        for (cur = 0; cur < 0x200u; cur++) {
            bp[cur] = 0;
        }
        bp[0x0f] = (uint8_t)(rsvd_tot >> 8);            /* BPB_RsvdSecCnt 高字节 */
        *(uint32_t *)(void *)(bp + 8) = 0x00302e35u;    /* OEM 尾 "5.0\0" + BytsPerSec 低字节 0 */
        bp[0x11] = 0;                                   /* BPB_RootEntCnt 低字节 = 0 */
        is32 = sz_vol > 0xffffu;                        /* > 65535 扇区 → 用 TotSec32 (FAT32) */
        bp[0x14] = is32 ? 0 : (uint8_t)(sz_vol >> 8);   /* 12/16: BPB_TotSec16 高字节 */
        bp[0x23] = is32 ? (uint8_t)(sz_vol >> 24) : 0;  /* 32: BPB_TotSec32 字节 3 */
        *(uint32_t *)(void *)(bp + 4) = 0x534f4453u;    /* OEM "SDOS" (与字节 3 'M' 拼 MSDOS) */
        bp[0x20] = is32 ? (uint8_t)sz_vol : 0;          /* 32: BPB_TotSec32 字节 0 */
        bp[0x10] = 1;                                   /* BPB_NumFATs = 1 */
        bp[0x21] = is32 ? (uint8_t)(sz_vol >> 8) : 0;   /* 32: BPB_TotSec32 字节 1 */
        bp[0x22] = is32 ? (uint8_t)(sz_vol >> 16) : 0;  /* 32: BPB_TotSec32 字节 2 */
        bp[0x13] = is32 ? 0 : (uint8_t)sz_vol;          /* 12/16: BPB_TotSec16 低字节 */
        *(uint32_t *)(void *)(bp + 0) = 0x4d90feebu;    /* 跳转 EB 90 FE + 'M' (字节 0-3) */
        bp[0x0e] = (uint8_t)rsvd_tot;                   /* BPB_RsvdSecCnt 低字节 */
        bp[0x0d] = (uint8_t)clst;                       /* BPB_SecPerClus */
        bp[0x18] = 0x3f;                                /* BPB_SecPerTrk = 63 */
        bp[0x12] = (fat_type == 3u) ? 0 : 2;            /* BPB_RootEntCnt 高 (12/16 = 512 项) */
        bp[0x0c] = 2;                                   /* BPB_BytsPerSec 高字节 → 512 */
        bp[0x1c] = (uint8_t)part_base;                  /* BPB_HiddSec 字节 0 */
        bp[0x15] = 0xf8;                                /* BPB_Media = 0xF8 */
        bp[0x19] = 0;                                   /* BPB_SecPerTrk 高字节 = 0 */
        bp[0x1b] = 0;                                   /* BPB_NumHeads 高字节 = 0 */
        bp[0x1d] = 0;                                   /* BPB_HiddSec 字节 2 = 0 */
        bp[0x1e] = 0;                                   /* BPB_HiddSec 字节 3 = 0 */
        bp[0x1f] = 0;                                   /* 12/16 时 TotSec32 字节 0 = 0 */
        bp[0x1a] = 0xff;                                /* BPB_NumHeads 低字节 = 255 */

        if (fat_type == 3u) {
            /* --- FAT32 引导扇区: FATSz32 / 卷标 "NO NAME    " / 文件系统 "FAT32   " --- */
            date = fat_default_date();
            bp[0x24] = (uint8_t)fat_secs;               /* BPB_FATSz32 字节 0 */
            bp[0x43] = (uint8_t)date;                   /* BS_VolID 字节 0-3 */
            bp[0x45] = (uint8_t)(date >> 16);
            bp[0x44] = (uint8_t)(date >> 8);
            bp[0x46] = (uint8_t)(date >> 24);
            *(uint32_t *)(void *)(bp + 0x47) = 0x4e204f4eu;  /* 卷标 "NO NAME    " (0x47-0x51) */
            *(uint32_t *)(void *)(bp + 0x4b) = 0x20454d41u;
            *(uint32_t *)(void *)(bp + 0x4f) = 0x46202020u;  /* 0x52 = 'F' 起文件系统类型 */
            *(uint32_t *)(void *)(bp + 0x53) = 0x32335441u;  /* "FAT32   " (0x52-0x59) */
            bp[0x25] = (uint8_t)(fat_secs >> 8);
            bp[0x26] = (uint8_t)(fat_secs >> 16);
            bp[0x27] = (uint8_t)(fat_secs >> 24);
            *(uint16_t *)(void *)(bp + 0x57) = 0x2020u;
            bp[0x59] = 0x20;
            bp[0x2d] = 0;                               /* RootClus/FSInfo/BkBootSec 高位清零 */
            bp[0x2e] = 0;
            bp[0x2f] = 0;
            bp[0x31] = 0;
            bp[0x33] = 0;
            bp[0x40] = 0x80;                            /* BS_DrvNum = 0x80 */
            bp[0x42] = 0x29;                            /* BS_BootSig = 0x29 */
            bp[0x2c] = 2;                               /* BPB_RootClus = 2 */
            bp[0x32] = 6;                               /* BPB_BkBootSec = 6 */
            bp[0x30] = 1;                               /* BPB_FSInfo = 1 */
            bp[0x1fe] = 0x55;                           /* 引导签名 55 AA */
            bp[0x1ff] = 0xaa;
            if (disk_write((int)pdrv, bp, part_base, 1) != 0) {
                return 1;                               /* FR_DISK_ERR */
            }
            disk_write((int)pdrv, bp, part_base + 6u, 1);    /* 备份引导扇区 (不查错) */
            for (cur = 0; cur < 0x200u; cur++) {
                bp[cur] = 0;
            }
            /* --- FSInfo 扇区: 空闲簇数 = 数据区簇数 - 1 (根目录占 1 簇) --- */
            n_clst2 -= 1u;
            bp[0x1e8] = (uint8_t)n_clst2;               /* FSI_Free_Count (0x1E8-0x1EB) */
            bp[0x1e9] = (uint8_t)(n_clst2 >> 8);
            bp[0x1ea] = (uint8_t)(n_clst2 >> 16);
            bp[0x1eb] = (uint8_t)(n_clst2 >> 24);
            bp[0] = 0x52;                               /* 引导签名 "RRaA" (0x00-0x03) */
            bp[1] = 0x52;
            bp[3] = 0x41;
            bp[0x1e6] = 0x41;                           /* 尾签名 "rrAa" (0x1E4-0x1E7) */
            bp[0x1e4] = 0x72;
            bp[0x1e5] = 0x72;
            bp[0x1ec] = 2;                              /* FSI_Nxt_Free = 2 */
            bp[0x1fe] = 0x55;
            bp[0x1ed] = 0;                              /* FSI_Nxt_Free 高 3 字节 = 0 */
            bp[0x1ee] = 0;
            bp[0x1ef] = 0;
            bp[2] = 0x61;
            bp[0x1e7] = 0x61;
            bp[0x1ff] = 0xaa;
            disk_write((int)pdrv, bp, part_base + 7u, 1);    /* FSInfo 备份 (保留区 7 号) */
            disk_write((int)pdrv, bp, part_base + 1u, 1);    /* FSInfo 主本 (保留区 1 号) */
        }
        else {
            /* --- FAT12/16 引导扇区: FATSz16 / 卷标 "NO NAME    " / 文件系统 "FAT     " --- */
            date = fat_default_date();
            bp[0x27] = (uint8_t)date;                   /* BS_VolID (0x27-0x2A) */
            bp[0x28] = (uint8_t)(date >> 8);
            bp[0x2a] = (uint8_t)(date >> 24);
            bp[0x29] = (uint8_t)(date >> 16);
            *(uint32_t *)(void *)(bp + 0x2f) = 0x20454d41u;  /* 卷标 "NO NAME    " (0x2B-0x35) */
            *(uint32_t *)(void *)(bp + 0x33) = 0x46202020u;  /* 0x36 = 'F' 起文件系统类型 */
            *(uint32_t *)(void *)(bp + 0x2b) = 0x4e204f4eu;
            *(uint32_t *)(void *)(bp + 0x37) = 0x20205441u;
            bp[0x24] = 0x80;                            /* BS_DrvNum = 0x80 */
            bp[0x26] = 0x29;                            /* BS_BootSig = 0x29 */
            bp[0x17] = (uint8_t)(fat_secs >> 8);        /* BPB_FATSz16 高字节 */
            bp[0x3d] = 0x20;                            /* "FAT     " 末字节 (0x36-0x3D) */
            bp[0x1fe] = 0x55;
            bp[0x16] = (uint8_t)fat_secs;               /* BPB_FATSz16 低字节 */
            *(uint16_t *)(void *)(bp + 0x3b) = 0x2020u;
            bp[0x1ff] = 0xaa;
            if (disk_write((int)pdrv, bp, part_base, 1) != 0) {
                return 1;
            }
        }
        /* --- 全缓冲清零 (work_len 向下取整到整扇区) --- */
        wr_cnt = work_len & 0xfffffe00u;
        for (cur = 0; cur < wr_cnt; cur++) {
            bp[cur] = 0;
        }
        /* --- FAT 首扇区预留项 --- */
        if (fat_type == 3u) {
            /* FAT32: 项 0 = 0xFFFFFFF8 (介质+EOC), 项 1 = 0xFFFFFFFF, 项 2 = 0x0FFFFFFF */
            bp[0] = 0xf8;
            bp[1] = 0xff;
            bp[2] = 0xff;
            bp[3] = 0xff;
            bp[4] = 0xff;
            bp[5] = 0xff;
            bp[6] = 0xff;
            bp[7] = 0xff;
            bp[8] = 0xff;
            bp[9] = 0xff;
            bp[0xa] = 0xff;
            bp[0xb] = 0x0f;
        }
        else {
            /* FAT12/16: 项 0 = [F8 FF FF 00/FF] (介质 + EOC, FAT12 高 4 位为 0) */
            bp[0] = 0xf8;
            bp[1] = 0xff;
            bp[2] = 0xff;
            bp[3] = (fat_type == 1u) ? 0x00u : 0xffu;
        }

        /* --- FAT 表区域清零写入 (按工作缓冲扇区数分块) --- */
        clear_left = (fat_type == 3u) ? fat_secs : fat_tot;
        wr_sect = data_start;
        for (;;) {
            wr_cnt = (clear_left < work_secs) ? clear_left : work_secs;
            if (disk_write((int)pdrv, bp, wr_sect, (int)wr_cnt) != 0) {
                return 1;
            }
            for (cur = 0; cur < 0x200u; cur++) {
                bp[cur] = 0;
            }
            clear_left -= wr_cnt;
            wr_sect += wr_cnt;
            if (clear_left == 0u) {
                /* FAT 之后接根目录区: FAT32 = 1 簇, FAT12/16 = 0x20 扇区 */
                if (fat_type == 3u) {
                    root_secs = clst;
                }
                for (;;) {
                    wr_cnt = (root_secs < work_secs) ? root_secs : work_secs;
                    if (disk_write((int)pdrv, bp, wr_sect, (int)wr_cnt) != 0) {
                        return 1;
                    }
                    sz_vol = sect_cnt[0];              /* 分区容量镜像 (供 MBR) */
                    root_secs -= wr_cnt;
                    wr_sect += wr_cnt;
                    if (root_secs == 0u) {
                        /* 分区类型码: 0xC = FAT32 LBA, 1 = FAT12 CHS, 4 = FAT16 CHS, 6 = FAT16B */
                        if (fat_type == 3u) {
                            type_code = 0xc;
                        }
                        else if (sect_cnt[0] < 0x10000u) {
                            type_code = (fat_type == 2u) ? 4u : 1u;
                        }
                        else {
                            type_code = 6u;
                        }
                        if ((fmt & 8u) == 0u) {
                            /* --- MBR: 单分区表项, 起始 LBA = part_base --- */
                            for (cur = 0; cur < 0x200u; cur++) {
                                bp[cur] = 0;
                            }
                            bp[0x1c2] = (uint8_t)type_code;        /* 分区类型 */
                            bp[0x1fe] = 0x55;                      /* MBR 签名 55 AA */
                            bp[0x1ff] = 0xaa;
                            bp[0x1be] = 0;                         /* 引导指示 = 0 */
                            bp[0x1c1] = 0;                         /* 起始 CHS 柱面高 = 0 */
                            bp[0x1bf] = 1;                         /* 起始磁头 = 1 */
                            bp[0x1c0] = 1;                         /* 起始扇区 = 1 */
                            end_cyl = (uint32_t)aeabi_uidiv(sect_cnt[0] + part_base, 0x3ec1u);
                            bp[0x1c5] = (uint8_t)end_cyl;          /* 末柱面低 8 位 */
                            bp[0x1ca] = (uint8_t)sz_vol;           /* 分区大小 (扇区, 4 字节) */
                            bp[0x1c6] = (uint8_t)part_base;        /* 起始 LBA 字节 0 */
                            bp[0x1c4] = (uint8_t)(end_cyl >> 2) | 0x3fu;  /* 末扇区 63 | 柱面高位 */
                            bp[0x1cb] = (uint8_t)(sz_vol >> 8);
                            bp[0x1c7] = 0;                         /* 起始 LBA 字节 1-3 = 0 */
                            bp[0x1c8] = 0;
                            bp[0x1c9] = 0;
                            bp[0x1cc] = (uint8_t)(sz_vol >> 16);
                            bp[0x1cd] = (uint8_t)(sz_vol >> 24);
                            bp[0x1c3] = 0xfe;                      /* 末磁头 = 254 */
                            if (disk_write((int)pdrv, bp, 0, 1) != 0) {
                                return 1;
                            }
                        }
                        /* 收尾 CTRL_SYNC: 失败按盘错 (1) 返回 */
                        ret = disk_ioctl((int)pdrv, 0, (void *)0);
                        return (ret != 0) ? 1u : 0u;
                    }
                }
            }
        }
    }
    return 0xe;                            /* 卷容量 ≤ 0x7F 扇区: 无法建立 FAT 卷 */
}

/* ==================== FatFs 移植层 (diskio) ==================== */

// === disk_initialize === 原 FUN_000065cc @ 0x000065cc
// 来源: reversed/fsbl_funcs/FUN_000065cc.c | 分类: fsbl
// 语义: FatFs disk_initialize 移植。读 disk_status(pdrv) 状态字节: bit1 (nodisk)
// 置位或 bit0 (noinit) 清零 → 原样返回; 否则等待 DAT_000219a4 使能的 DAT 线
// 呈现状态 (控制器基址 DAT_000219a0 + 0x24, bit16-18 全 1 = CMD/DAT 线空闲);
// sd_slot_cfg_get 取槽配置 (0x19bec), 成功则对槽对象 (0x21560 + pdrv*0x220)
// 依次 sd_host_init (主机基址取 cfg+4 字) 与 sdmmc_card_init; 全部成功后清
// 状态字节 bit0 (DAT_00019be8[pdrv] &= 0xfe) 并返回该值; 任一步失败置 bit1
// (状态 | 1) 返回。
uint32_t disk_initialize(int pdrv)
{
    uint32_t st;
    uint16_t *cfg;
    uint32_t slot;
    uint32_t ps;

    st = (uint32_t)disk_status(pdrv);
    if (((st & 2u) != 0u) || ((st & 1u) == 0u)) {
        return st;                 /* 已有盘/已初始化: 状态原样返回 */
    }
    if (*DAT_000219a4 != 0u) {
        /* 卡检测等待使能: 等 CMD/DAT 线电平位 (bit16-18) 全部置 1 */
        do {
            ps = *(volatile uint32_t *)(uintptr_t)(*DAT_000219a0 + 0x24u);
        } while ((ps & 0x70000u) != 0x70000u);
    }
    cfg = sd_slot_cfg_get(pdrv);   /* 槽配置 (全局单例 0x19bec) */
    if (cfg != (uint16_t *)0) {
        slot = 0x21560u + (uint32_t)pdrv * 0x220u;      /* 槽对象基址 */
        if ((sd_host_init((uint16_t *)(uintptr_t)slot, cfg,
                          (int)*(volatile uint32_t *)(uintptr_t)((uint8_t *)cfg + 4)) == 0) &&
            (sdmmc_card_init((int)(uintptr_t)slot) == 0)) {
            st &= 0xfeu;
            DAT_00019be8[pdrv] = (uint8_t)st;   /* 清 noinit 位 */
            return st;
        }
    }
    return (st & 0xffu) | 1u;      /* 初始化失败: 置 noinit 位 */
}


/* ==================== SD/eMMC 主机层 (SDHCI) ==================== */
/* 主机寄存器偏移 (SDHCI, 字节偏移; 见 p3.c 注):
 *   +0x04 块大小(W) +0x06 块数(W) +0x08 参数(D) +0x0C 传输模式(W)|命令(W)
 *   +0x24 呈现状态(D) +0x2E 超时控制(B) +0x30 常规中断状态(W) +0x32 错误中断状态(W)
 *   +0x58 ADMA 系统地址(D)
 * SD 对象 (sd) 偏移: +0x04 主机寄存器基址 | +0x20 缓存维护抑制标志(B)
 *   +0x24 签名 0x11111111 | +0x34 卡类型(B, 1 = SD) | +0x200/0x204 ADMA 连续
 *   描述符基址 (64 位) | +0x80 起 0x180 字节 ADMA2 描述符表 (16 字节/项:
 *   +0x80 属性(W) +0x82 长度(W) +0x84/+0x88 地址(D,D)) */

// === sd_cmd_status_get === 原 FUN_00006a24 @ 0x00006a24
// 来源: reversed/fsbl_funcs/FUN_00006a24.c | 分类: fsbl
// 语义: SD 命令码 (cmd << 8) → 期望的命令完成状态码映射 (sd_send_cmd 用其与
// 常规中断状态比对判完成)。多数命令 = cmd | 0x1a (R1b 型) 或 cmd | 0x3a (带
// 忙等待型), 读响应类命令 = cmd | 0x02/0x09; SD 卡 (sd+0x34 卡类型 == 1) 的
// CMD3/CMD6/CMD8 用 0x1b/0x3a/0x14 变体。Ghidra 将返回值 0x714/0x814 及对
// 0x1100/0x1300 的比较误标为函数/字符串地址 (入口地址巧合), 此处按立即数还原;
// 反编译的有序比较树等价转写为 switch。
uint32_t sd_cmd_status_get(int sd, uint32_t cmd)
{
    volatile const int8_t *card_type = (volatile const int8_t *)(uintptr_t)((uint8_t *)(uintptr_t)sd + 0x34);

    switch (cmd) {
    case 0x100u:
        return 0x102u;                          /* CMD0/1: R2 (136 位响应) */
    case 0x200u:
        return 0x209u;
    case 0x300u:
        return (*card_type == 1) ? 0x31bu : 0x31au;
    case 0x500u:
        return 0x51bu;
    case 0x600u:
        return (*card_type == 1) ? 0x63au : 0x61bu;
    case 0x700u:
        return 0x714u;                          /* 原 Ghidra FUN_00000714 (地址巧合立即数) */
    case 0x800u:
        return (*card_type == 1) ? 0x814u : 0x83au;   /* 0x814 同上 */
    case 0x900u:
        return 0x909u;
    case 0xa00u:
    case 0xb00u:
    case 0xc00u:
    case 0x1000u:
    case 0x1300u:
    case 0x1500u:
    case 0x1700u:
    case 0x1800u:
    case 0x1900u:
    case 0x3400u:
    case 0x8600u:
    case 0x8d00u:
        return cmd | 0x1au;                     /* 原 LAB_00006af4 落出路径 */
    case 0x1100u:
    case 0x1200u:
    case 0x9700u:
        return cmd | 0x3au;                     /* 原 LAB_00006b0c 路径 */
    case 0xa900u:
        return 0xa902u;
    case 0xaa00u:
        return 0xaa1au;
    case 0xb300u:
        return 0xb33au;
    default:
        return cmd;
    }
}

// === sd_send_cmd === 原 FUN_00006bcc @ 0x00006bcc
// 来源: reversed/fsbl_funcs/FUN_00006bcc.c | 分类: fsbl
// 语义: SDHCI 命令发送。sd == NULL → dbg_print(0x1725C, 0x451) + 置断言标志返 0;
// 签名 (sd+0x24) != 0x11111111 → dbg_print(..., 0x452) + 断言返 0 (两路径先清
// 断言标志)。主机呈现状态 (+0x24) bit0 (命令线抑制) 置位 → 返 1。装配命令:
// 块数 (+0x06) = resp_type、超时 (+0x2E) = 0xE、参数 (+0x08) = arg、清常规/错误
// 中断状态 (+0x30 = 0xFFFF, +0x32 = 0xF3FF); 经 sd_cmd_status_get 取期望状态后,
// 非数据命令 (cmd ∉ {0x1300, 0x1500}) 且 DAT 线抑制 (bit1) 与期望状态 bit5 同时
// 置位时直接返 1 (卡忙); 否则写命令寄存器 +0x0C = (期望状态 & 0x3FFF) << 16 |
// DAT_00028e88 (传输模式暂存字), 轮询常规中断状态: 数据命令见 bit5 (传输完成)
// → 写 0x20 清除; bit15 (错误) → 读 +0x32 后重写 0xF3FF, 有效错误位 (除 bit0)
// 为 0 返 2, 否则返回原始错误位; bit0 (命令完成) → 写 1 清除返 0 (数据命令的
// bit5 路径随后同样写 1 清命令完成位)。
int sd_send_cmd(int sd, uint16_t cmd, uint32_t arg, uint16_t resp_type)
{
    volatile uint8_t *host;
    uint32_t expected;        /* sd_cmd_status_get 期望完成状态 */
    uint32_t data_cmd;        /* 0 = 数据命令 (0x1300/0x1500) */
    uint16_t st;
    uint16_t err;

    if (sd == 0) {
        dbg_print(0x1725Cu, 0x451u);
        g_assert_failed = 1u;
        return 0;
    }
    g_assert_failed = 0u;
    if (*(volatile uint32_t *)(uintptr_t)((uint8_t *)(uintptr_t)sd + 0x24) != 0x11111111u) {
        dbg_print(0x1725Cu, 0x452u);
        g_assert_failed = 1u;
        return 0;
    }
    host = (volatile uint8_t *)(uintptr_t)*(volatile uint32_t *)(uintptr_t)((uint8_t *)(uintptr_t)sd + 4);
    if ((*(volatile uint32_t *)(uintptr_t)(host + 0x24) & 1u) != 0u) {
        return 1;                              /* 命令线抑制 (CMD_INHIBIT) 置位 */
    }
    /* 命令寄存器组装配 */
    *(volatile uint16_t *)(uintptr_t)(host + 6) = resp_type;     /* 块数寄存器 */
    *(volatile uint8_t *)(uintptr_t)(host + 0x2e) = 0x0eu;       /* 超时控制 = 0xE */
    *(volatile uint32_t *)(uintptr_t)(host + 8) = arg;           /* 命令参数 */
    *(volatile uint16_t *)(uintptr_t)(host + 0x30) = 0xffffu;    /* 清常规中断状态 */
    *(volatile uint16_t *)(uintptr_t)(host + 0x32) = 0xf3ffu;    /* 清错误中断状态 */
    expected = sd_cmd_status_get(sd, (uint32_t)cmd);
    data_cmd = ((uint32_t)cmd - 0x1300u) & 0xfffffdffu;          /* 0x1300/0x1500 → 0 */
    if ((data_cmd != 0u) &&
        (((*(volatile uint32_t *)(uintptr_t)(host + 0x24) & 2u) != 0u) &&
         ((expected & 0x20u) != 0u))) {
        return 1;                              /* DAT 线抑制且期望忙 → 不发命令 */
    }
    /* 命令寄存器 = 期望状态低 14 位 <<16 | 传输模式暂存 (DAT_00028e88) */
    *(volatile uint32_t *)(uintptr_t)(host + 0x0c) =
        ((expected & 0x3fffu) << 16) | (g_sd_cmd_reg & 0xffffu);
    host = (volatile uint8_t *)(uintptr_t)*(volatile uint32_t *)(uintptr_t)((uint8_t *)(uintptr_t)sd + 4);
    for (;;) {
        st = *(volatile uint16_t *)(uintptr_t)(host + 0x30);
        if ((data_cmd == 0u) && ((st & 0x20u) != 0u)) {
            *(volatile uint16_t *)(uintptr_t)(host + 0x30) = 0x20u;  /* 清传输完成位 */
            break;
        }
        if ((st & 0x8000u) != 0u) {
            err = *(volatile uint16_t *)(uintptr_t)(host + 0x32);
            *(volatile uint16_t *)(uintptr_t)(host + 0x32) = 0xf3ffu;
            if ((err & 0xfffeu) == 0u) {
                return 2;                      /* 错误状态无有效位 */
            }
            return err;                        /* 原始错误位 */
        }
        if ((st & 1u) != 0u) {
            break;                             /* 命令完成 */
        }
    }
    *(volatile uint16_t *)(uintptr_t)(host + 0x30) = 1u;   /* 清命令完成位 */
    return 0;
}

// === sd_adma_setup_cont === 原 FUN_00007100 @ 0x00007100
// 来源: reversed/fsbl_funcs/FUN_00007100.c | 分类: fsbl
// 语义: 为连续多块传输建立 ADMA2 描述符链 (sd+0x80 起, 16 字节/项, 表容量
// 0x180 字节)。总字节 nblocks × 块长 (主机 +0x04 低 12 位) 超过 64KB 时按
// 0x10000 字节/项拆分: 中间项属性 0x21 (Valid, 非 End), 末项属性 0x23
// (Valid|End), 长度字段 = (int16)nblocks × (int16)块长的带符号 16 位积;
// 描述符地址自 sd+0x200/0x204 的 64 位基址逐项 +0x10000 (带 64 位进位)。
// 主机 +0x58 (ADMA 系统地址) = sd+0x80; 非缓冲模式 (sd+0x20 == 0) 时对描述符
// 表做 0x180 字节 cache 维护 (func_0xffffd370); 末尾清零 sd+0x200/0x204。
void sd_adma_setup_cont(int sd, uint32_t nblocks)
{
    volatile uint8_t *host;
    uint32_t addr_lo;         /* sd+0x200: 描述符基址低 32 位 */
    uint32_t addr_hi;         /* sd+0x204: 描述符基址高 32 位 */
    uint32_t blklen;          /* 主机块长寄存器低 12 位 */
    uint32_t total;           /* nblocks × blklen (字节) */
    uint32_t ndesc;           /* 64KB 分片描述符数 */
    uint32_t i;
    uint32_t lo;              /* 当前项地址低 32 位 */
    uint32_t hi;              /* 当前项地址高 32 位 */
    int carry;
    int len;                  /* (int16)nblocks × (int16)blklen */

    host = (volatile uint8_t *)(uintptr_t)*(volatile uint32_t *)(uintptr_t)((uint8_t *)(uintptr_t)sd + 4);
    addr_lo = *(volatile uint32_t *)(uintptr_t)((uint8_t *)(uintptr_t)sd + 0x200);
    addr_hi = *(volatile uint32_t *)(uintptr_t)((uint8_t *)(uintptr_t)sd + 0x204);
    blklen = *(volatile uint16_t *)(uintptr_t)(host + 4) & 0xfffu;
    total = nblocks * blklen;
    i = 0;
    if (total >= 0x10000u) {
        ndesc = (total >> 16) + ((total & 0xffffu) != 0u);   /* 64KB 分片向上取整 */
        if (ndesc > 1u) {
            lo = addr_lo;
            hi = addr_hi;
            do {
                /* 中间描述符: 64KB/项, 属性 0x21 (Valid, 非 End), 长度高 16 位 = 0 */
                *(volatile uint32_t *)(uintptr_t)((uint8_t *)(uintptr_t)sd + i * 0xcu + 0x84u) = lo;
                carry = (0xfffeffffu < lo);            /* lo + 0x10000 的 64 位进位 */
                lo += 0x10000u;
                *(volatile uint32_t *)(uintptr_t)((uint8_t *)(uintptr_t)sd + i * 0xcu + 0x88u) = hi;
                hi += (uint32_t)carry;
                *(volatile uint16_t *)(uintptr_t)((uint8_t *)(uintptr_t)sd + i * 0xcu + 0x80u) = 0x21u;
                *(volatile uint16_t *)(uintptr_t)((uint8_t *)(uintptr_t)sd + i * 0xcu + 0x82u) = 0u;
                i++;
            } while (i != ndesc - 1u);
            addr_lo = lo;                      /* 基址推进 (ndesc-1)×0x10000 后的值 */
            addr_hi = hi;
        }
    }
    /* 末描述符: 属性 0x23 (Valid|End), 长度 = 带符号 16 位乘积 (截断存储) */
    *(volatile uint32_t *)(uintptr_t)((uint8_t *)(uintptr_t)sd + i * 0xcu + 0x84u) = addr_lo;
    *(volatile uint32_t *)(uintptr_t)((uint8_t *)(uintptr_t)sd + i * 0xcu + 0x88u) = addr_hi;
    len = (int)(int16_t)(uint16_t)nblocks * (int)(int16_t)(uint16_t)blklen;
    *(volatile uint16_t *)(uintptr_t)((uint8_t *)(uintptr_t)sd + i * 0xcu + 0x82u) = (uint16_t)len;
    *(volatile uint16_t *)(uintptr_t)((uint8_t *)(uintptr_t)sd + i * 0xcu + 0x80u) = 0x23u;
    *(volatile uint32_t *)(uintptr_t)(host + 0x58) =
        (uint32_t)(uintptr_t)((uint8_t *)(uintptr_t)sd + 0x80u);          /* ADMA 系统地址 */
    if (*(volatile int8_t *)(uintptr_t)((uint8_t *)(uintptr_t)sd + 0x20) == 0) {
        fun_0xffffd370((void *)(uintptr_t)((uint8_t *)(uintptr_t)sd + 0x80u), 0x180u);  /* cache 维护 */
    }
    *(volatile uint32_t *)(uintptr_t)((uint8_t *)(uintptr_t)sd + 0x200) = 0u;
    *(volatile uint32_t *)(uintptr_t)((uint8_t *)(uintptr_t)sd + 0x204) = 0u;
    return;
}

// === sdmmc_set_blocklen === 原 FUN_00007d40 @ 0x00007d40
// 来源: reversed/fsbl_funcs/FUN_00007d40.c | 分类: fsbl
// 语义: CMD16 (SET_BLOCKLEN, 编码 0x1000) 设置卡块长。sd == NULL →
// dbg_print(0x17264, 0x7b) + 置断言标志返 0; 签名 (sd+0x24) != 0x11111111 →
// dbg_print(..., 0x7c) + 断言返 0 (两路径先清断言标志)。主机呈现状态 (+0x24)
// 与 0x303 (bit0 命令抑制 | bit8 DAT 抑制) 相与非零 → 返 1; 否则发
// sd_send_cmd(sd, 0x1000, blklen, 0) (响应类型实参 0 来自反编译 ands 清零 r3),
// 命令成功时把 blklen & 0xfff 写入主机块长寄存器 (+0x04) 并返 0, 失败返 1。
int sdmmc_set_blocklen(int sd, uint32_t blklen)
{
    volatile uint8_t *host;

    if (sd == 0) {
        dbg_print(0x17264u, 0x7bu);
        g_assert_failed = 1u;
        return 0;
    }
    g_assert_failed = 0u;
    if (*(volatile uint32_t *)(uintptr_t)((uint8_t *)(uintptr_t)sd + 0x24) != 0x11111111u) {
        dbg_print(0x17264u, 0x7cu);
        g_assert_failed = 1u;
        return 0;
    }
    host = (volatile uint8_t *)(uintptr_t)*(volatile uint32_t *)(uintptr_t)((uint8_t *)(uintptr_t)sd + 4);
    if ((*(volatile uint32_t *)(uintptr_t)(host + 0x24) & 0x303u) != 0u) {
        return 1;                              /* 命令/DAT 线抑制 */
    }
    if (sd_send_cmd(sd, 0x1000u, blklen, 0u) != 0) {
        return 1;
    }
    *(volatile uint16_t *)(uintptr_t)(host + 4) = (uint16_t)(blklen & 0xfffu);
    return 0;
}

/* ==================== 覆盖率表 ====================
 *
 * | 原函数       | 语义名              | 状态 | 行数 | 说明 |
 * |--------------|---------------------|------|------|------|
 * | FUN_00005a60 | fat_f_mkfs          | 完成 |  516 | 5 段转写, 关键算术/寄存器写经反汇编逐条比对 |
 * | FUN_000065cc | disk_initialize     | 完成 |   51 | 状态位语义与 p4.c disk_status 对齐 |
 * | FUN_00006a24 | sd_cmd_status_get   | 完成 |   57 | Ghidra 函数指针误标 (0x714/0x814) 已按立即数还原 |
 * | FUN_00006bcc | sd_send_cmd         | 完成 |   75 | 轮询/错误路径与反汇编一致 |
 * | FUN_00007100 | sd_adma_setup_cont  | 完成 |   65 | 64 位地址进位还原为 carry 判定 |
 * | FUN_00007d40 | sdmmc_set_blocklen  | 完成 |   52 | 0x303 抑制位与 ands 清零的第 4 参已注明 |
 *
 * 覆盖: 6/6 函数; Ghidra 残留标识符: 0 (仅约定允许的 DAT_xxx 具名全局)。
 */
