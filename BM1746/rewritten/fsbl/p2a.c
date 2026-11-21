/**
 * p2a.c — FSBL 反编译转写 批次 2a/4 (NAND 访问器 + libc 运行时 + FatFs 中段 1 + SD 底层)
 *
 * 覆盖 (reversed/fsbl_funcs/, 源二进制 Zynq7007_miner.elf, 共 21 函数, 地址升序):
 *   - NAND 控制器寄存器访问器 (Xilinx Nand driver 断言式): nand_ctrl_reg_or / nand_reg30_get / nand_reg0c_get
 *   - libc/运行时: delay_us / aeabi_uidiv / exit / memset / hang_forever
 *   - FatFs 中段: fatfs_validate / fat_get_fileinfo / fat_dir_entry_remove / fat_check_fs /
 *     fat_dir_sdi / fat_f_read / fat_f_unlink / fat_f_mkfs
 *   - FatFs 移植层: disk_initialize
 *   - SD/eMMC 主机层: sd_cmd_status_get / sd_send_cmd / sd_adma_setup_cont / sdmmc_set_blocklen
 *
 * 全局数据 (均为反编译实证, 语义推断处已注明):
 *   DAT_00028e84 = xil_assert 错误标志 (0 = 正常, 1 = 已触发断言; 与 p4.c g_assert_failed 同址)
 *   DAT_00015b74 = libc 退出处理函数表指针 (exit 经其 +0x3c 取最终 handler)
 *   DAT_0002000c = 退出链末尾代码指针表项 (exit 尾部循环调用, 实际不可达)
 *   DAT_0002153c = FatFs 文件对象池基址 (按 FD 索引)
 *   DAT_00021548 = FatFs 卷窗口/标志 (见各函数内注释)
 *   DAT_00021558 / DAT_0002155c = FatFs 卷对象池基址 / 槽位计数 (fatfs_validate 使用)
 *
 * 调用方可见函数名一律取自 rewritten/name_map/fsbl.tsv (阶段 1 冻结)。
 */
#include <stdint.h>
#include <stdbool.h>   /* bool (disk_write 返回类型) */

/* ==================== 共享声明 (本批被调/跨批被调, 签名与反编译一致) ==================== */

extern int  dbg_print(int line, int code);              /* FUN_000014e4: 行号+错误码日志 (与 p4.c 同约定) */
extern void exit_run_handlers(int status, int flag);    /* FUN_000029dc */
extern void fun_ffffe2d4(uint32_t out[2]);              /* func_0xffffe2d4: ROM 定时器读 (命名待补) */
extern void Reset(void);                                /* FUN_00000000: 复位入口 */
extern int  fatfs_validate(uint32_t *fp, uint32_t *vol_out);    /* FUN_00002bbc */
extern int  fat_window_read(uint8_t *fs);               /* FUN_00002ce0 */
extern int  fat_move_window(uint8_t *fs, uint32_t sect);/* FUN_00002f3c */
extern int  fat_get_fat(uint8_t *fs, uint32_t clst);    /* FUN_000036fc */
extern int  fat_create_chain(int *dj, uint32_t clst);   /* FUN_00003868 */
extern int  fat_dir_seek(uint8_t *fs, uint32_t clst);   /* FUN_00002d5c */
extern int  fat_find_volume(const uint8_t **pp, uint8_t **fs_out, uint32_t mode); /* FUN_000030bc */
extern int  fat_follow_path(int *dj, const uint8_t *path); /* FUN_00003f2c */
extern int  fat_dir_sdi0(int *dj);                      /* FUN_00003d50 */
extern int  fat_dir_next(int *dj);                      /* FUN_00003c84 */
extern int  fat_remove_chain(int *dj, uint32_t clst, uint32_t free_clst); /* FUN_00003a48 */
extern int  fat_fs_sync(uint8_t *fs);                   /* FUN_00002e20 */
extern int  disk_status(int pdrv);                      /* FUN_000064b8 */
extern int  disk_read(int pdrv, void *buf, uint32_t sect, int cnt);  /* FUN_0000667c */
extern bool disk_write(int pdrv, const void *buf, uint32_t sect, int cnt); /* FUN_0000676c */
extern uint16_t sd_send_cmd(int sd, uint16_t cmd, uint32_t arg, uint16_t resp_type); /* FUN_00006bcc */
extern void hang_forever(void);                         /* FUN_00002b40 */
extern void delay_ms(uint32_t ms);                      /* func_0xffffe2ac: ROM/BSP 毫秒延时 */

/* Xil Assert 风格错误标志 (0x28e84), 同 p4.c g_assert_failed */
#define g_assert_failed   (*(volatile uint32_t *)0x00028e84)

/* FS 对象布局 (字节偏移, 实证见 p4.c fat_find_volume/move_window):
 *   +0=fs_type(1=FAT12/2=FAT16/3=FAT32), +1=pdrv, +2=fat_type, +3=脏标志,
 *   +0x0C/+0x10/+0x14=free_clst/next_free/max_clst, +0x28=数据区基址,
 *   +0x2C=窗口扇区, +0x40..+0x240=512B 数据窗口。
 * FIL 对象 (uint32_t* 字序): [0]=fs, [1]低16=id, +0x10字节=标志, +0x11=err,
 *   [2]=首簇, [3]=文件长, [5]=读写指针, [6]=当前簇, [7]=数据扇区, [0x0F]=, [0x10]=512B 窗口。 */

/* ==================== NAND 控制器寄存器访问器 (Xilinx Nand driver 断言式) ==================== */

// === nand_ctrl_reg_or === 原 FUN_0000060c @ 0x0000060c
// 来源: reversed/fsbl_funcs/FUN_0000060c.c | 分类: fsbl
// 语义: NAND 控制器寄存器置位: *inst->reg |= bits。inst +4 = 寄存器基址指针
// (二级指针), +8 = IsReady 标志 (须为 0x11111111)。断言失败时置 g_assert_failed。
void nand_ctrl_reg_or(int inst, uint32_t bits)
{
    if (inst == 0) {
        dbg_print(0xff, 0);                             /* 原 FUN_000014e4(s_0___P___p_0001720a+10, 0xff) */
        g_assert_failed = 1;
        return;
    }
    g_assert_failed = 0;
    if (*(int *)(inst + 8) == 0x11111111) {
        **(volatile uint32_t **)(inst + 4) |= bits;
        return;
    }
    dbg_print(0x100, 0);                                /* 原 FUN_000014e4(s_0___P___p_0001720a+10, 0x100) */
    g_assert_failed = 1;
}

// === nand_reg30_get === 原 FUN_00000b14 @ 0x00000b14
// 来源: reversed/fsbl_funcs/FUN_00000b14.c | 分类: fsbl
// 语义: NAND 控制器寄存器 +0x30 读取 (带断言保护, 同 nand_ctrl_reg_or 访问器模式)。
uint32_t nand_reg30_get(int inst)
{
    if (inst == 0) {
        dbg_print(0x217, 0);                            /* 原 FUN_000014e4(s_0___P___p_0001720a+10, 0x217) */
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(inst + 8) == 0x11111111) {
        return *(volatile uint32_t *)(*(int *)(inst + 4) + 0x30);
    }
    dbg_print(0x218, 0);                                /* 原 FUN_000014e4(s_0___P___p_0001720a+10, 0x218) */
    g_assert_failed = 1;
    return 0;
}

// === nand_reg0c_get === 原 FUN_000011c8 @ 0x000011c8
// 来源: reversed/fsbl_funcs/FUN_000011c8.c | 分类: fsbl
// 语义: NAND 控制器寄存器 +0x0C 读取 (带断言保护, 同上访问器模式)。
uint32_t nand_reg0c_get(int inst)
{
    if (inst == 0) {
        dbg_print(0xbc, 0);                             /* 原 FUN_000014e4(s_0___P___p_0001720a+0x16, 0xbc) */
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(inst + 8) == 0x11111111) {
        return *(volatile uint32_t *)(*(int *)(inst + 4) + 0xc);
    }
    dbg_print(0xbd, 0);                                 /* 原 FUN_000014e4(s_0___P___p_0001720a+0x16, 0xbd) */
    g_assert_failed = 1;
    return 0;
}

/* ==================== libc / 运行时 ==================== */

// === delay_us === 原 FUN_000014a0 @ 0x000014a0
// 来源: reversed/fsbl_funcs/FUN_000014a0.c | 分类: fsbl
// 语义: 微秒级忙等: 目标时刻 = 当前全局定时器计数值 + us * 0x14D (每微秒 333 计数,
// 对应 CPU 主频 666MHz 的全局定时器), 轮询读取定时器 (fun_ffffe2d4, ROM 区
// func_0xffffe2d4, 写出 64 位计数值) 直到到达目标时刻。恒返回 0。
uint32_t delay_us(uint32_t us)
{
    uint32_t now[2];                                    /* 反编译 local_18(低)/uStack_14(高) */
    uint64_t target;
    fun_ffffe2d4(now);
    target = (uint64_t)now[0] | ((uint64_t)now[1] << 32);
    target += (uint64_t)us * 0x14d;
    do {
        fun_ffffe2d4(now);
    } while (((uint64_t)now[0] | ((uint64_t)now[1] << 32)) < target);
    return 0;
}

// === aeabi_uidiv === 原 FUN_00001a50 @ 0x00001a50
// 来源: reversed/fsbl_funcs/FUN_00001a50.c | 分类: fsbl
// 语义: ARM 运行时无符号除法 (__aeabi_uidiv), 返回 num/den 商。特例: 除数 1 直通;
// 除数 0 返回 0xFFFFFFFF (被除数非 0) 或 0; 被除数 <= 除数返回相等标志;
// 除数为 2 的幂时移位直除。主体为 32 步展开的移位-减法求商: 原反编译在
// shift != 0 时经跳表 (0x1a8c, 每项 0xC 字节) 进入展开体对应位次, 转写为
// 等价的逐位循环 (商位从 clz(den)-clz(num) 向下判定), 行为一致。
uint32_t aeabi_uidiv(uint32_t num, uint32_t den)
{
    uint32_t q;
    int shift;

    if (den - 1 == 0) {                                 /* den == 1 */
        return num;
    }
    if (den == 0) {
        return (num != 0) ? 0xFFFFFFFFu : 0;
    }
    if (num <= den) {
        return (uint32_t)(num == den);
    }
    if ((den & (den - 1)) == 0) {                       /* 2 的幂除数 → 右移 */
        return num >> (0x1f - __builtin_clz(den));
    }
    q = 0;
    for (shift = 0x1f - (__builtin_clz(den) - __builtin_clz(num));
         shift >= 0;
         shift--) {
        if (num >= (den << shift)) {
            num -= den << shift;
            q |= (uint32_t)1 << shift;
        }
    }
    return q;
}

// === exit === 原 FUN_000020ec @ 0x000020ec
// 来源: reversed/fsbl_funcs/FUN_000020ec.c | 分类: fsbl
// 语义: libc exit: 先跑退出处理链 exit_run_handlers(status, 0), 再调用全局表
// (0x15b74, 原名 DAT_00015b74 /* 全局, 语义待定 */) +0x3C 处的最终 handler
// (为 Reset 时跳过), 然后挂死。其后反编译残留的 0x20010 递减调用循环位于
// 挂死之后, 永不可达, 按原样保留。
void exit(int status)
{
    uint32_t i;
    uint32_t *pp;

    exit_run_handlers(status, 0);
    if (*(uint32_t *)0x00015bb0 != (uint32_t)(uintptr_t)Reset) {    /* DAT_00015b74+0x3c */
        ((void (*)(void))*(uint32_t *)0x00015bb0)();
    }
    hang_forever();
    /* 不可达尾部 (反编译原样): 从 0x20010 向下取函数指针调用一次 */
    i = 1;
    pp = (uint32_t *)0x00020010;
    do {
        i--;
        pp--;
        ((void (*)(void))*pp)();
    } while (i != 0);
}

// === memset === 原 FUN_000027e0 @ 0x000027e0
// 来源: reversed/fsbl_funcs/FUN_000027e0.c | 分类: fsbl
// 语义: 标准 memset (armcc 优化实现): 头部逐字节填到 4 字节对齐; 主体先按
// 16 字节组填 (n>=0x10 时按 (n-0x10)>>4+1 组写 4 字), 余量按字填, 尾部逐字节。
void memset(void *dst, uint8_t val, uint32_t n)
{
    uint32_t w;
    uint32_t *wp = (uint32_t *)dst;

    if (((uintptr_t)dst & 3u) != 0) {
        uint8_t *bp = (uint8_t *)dst;
        if (n == 0) {
            return;
        }
        for (;;) {
            *bp = val;
            bp++;
            n--;
            if (((uintptr_t)bp & 3u) == 0) {
                break;                                  /* 已对齐 → 转主体 */
            }
            if (n == 0) {
                return;
            }
        }
        wp = (uint32_t *)bp;
    }
    if (n < 4u) {
        uint8_t *bp = (uint8_t *)wp;
        if (n == 0) {
            return;
        }
        do {
            *bp++ = val;
        } while (bp != (uint8_t *)wp + n);
        return;
    }
    w = (uint32_t)val;                                  /* 字节复制到 4 个字节位 */
    w |= w << 8;
    w |= w << 16;
    if (n > 0xfu) {
        uint32_t groups = ((n - 0x10u) >> 4) + 1u;      /* 16 字节组数 (含当前组) */
        uint32_t *we = wp + groups * 4;
        while (wp != we) {
            wp[0] = w;
            wp[1] = w;
            wp[2] = w;
            wp[3] = w;
            wp += 4;
        }
        n &= 0xfu;
        if (n < 4u) {
            uint8_t *bp = (uint8_t *)wp;
            if (n != 0) {
                do {
                    *bp++ = val;
                } while (bp != (uint8_t *)wp + n);
            }
            return;
        }
    }
    {
        uint32_t words = ((n - 4u) >> 2) + 1u;
        uint32_t *we = wp + words;
        while (wp != we) {
            *wp++ = w;
        }
        n &= 3u;
    }
    if (n != 0) {
        uint8_t *bp = (uint8_t *)wp;
        do {
            *bp++ = val;
        } while (bp != (uint8_t *)wp + n);
    }
}

// === hang_forever === 原 FUN_00002b40 @ 0x00002b40
// 来源: reversed/fsbl_funcs/FUN_00002b40.c | 分类: fsbl
// 语义: 空死循环 (错误退出终点)。
void hang_forever(void)
{
    for (;;) {
        /* 原始流: 无操作死循环 */
    }
}

/* ==================== FatFs 中段: 对象校验 / 目录项 / 引导记录 ==================== */

// === fatfs_validate === 原 FUN_00002bbc @ 0x00002bbc
// 来源: reversed/fsbl_funcs/FUN_00002bbc.c | 分类: fsbl
// 语义: FatFs 对象有效性校验 (validate): fp[0] = 卷对象指针, +4 半字 = 对象 id,
// 卷对象 +0 半字 = 卷 id (须相等), 卷对象 +0 字节 = fs_type (非 0 = 已挂卷),
// 卷对象 +1 字节 = pdrv。磁盘未初始化 (disk_status bit0) 视为无效。
// 通过时 *vol_out = 卷指针, 返回 0 (FR_OK); 否则 *vol_out = 0, 返回 9 (FR_INVALID_OBJECT)。
int fatfs_validate(uint32_t *fp, uint32_t *vol_out)
{
    uint8_t *fs;

    if (fp != 0 &&
        (fs = *(uint8_t **)fp) != 0 &&
        fs[0] != 0 &&
        *(uint16_t *)((uintptr_t)fp + 4) == *(uint16_t *)(fs + 6)) {
        if ((disk_status(fs[1]) & 1) == 0) {
            *vol_out = *fp;                              /* 输出卷对象指针 */
            return 0;
        }
    }
    *vol_out = 0;
    return 9;
}

// === fat_get_fileinfo === 原 FUN_00002c30 @ 0x00002c30
// 来源: reversed/fsbl_funcs/FUN_00002c30.c | 分类: fsbl
// 语义: 从目录项槽位 (*param_1 = 目录项指针, 即 DIR 对象 +0x1C 的 dj->dir 域,
// 调用方传 DIR+0x1C) 拷贝 8.3 短名到 FILINFO.fname (+9 起): 跳过空格,
// 0x05 → 0xE5 (KANJI 前缀恢复), 读到第 9 字节 (基名结束) 时插入 '.',
// 共 11 字节 (entry[0..0x0A]), 补 NUL。随后填 fattr (entry+0x0B → FILINFO+8)、
// fsize (entry+0x1C → +0)、fdate (entry+0x16 → +6)、ftime (entry+0x18 → +4)。
void fat_get_fileinfo(int *dir_slot, uint32_t *finfo)
{
    uint8_t *entry = *(uint8_t **)dir_slot;
    char c;
    int pos = 0;                                         /* fName 已写长度 */
    int i = 0;                                           /* 已读字节计数 1..11 */
    uint8_t *p = entry - 1;

    do {
        p++;
        c = *p;
        i++;
        if (c != ' ') {
            if (c == '\x05') {
                c = '\xE5';                              /* 0x05 → 0xE5 */
            }
            if (i == 9) {
                *(char *)((uintptr_t)finfo + pos + 9) = '.';
                pos++;
            }
            *(char *)((uintptr_t)finfo + pos + 9) = c;
            pos++;
        }
    } while (i != 0xb);
    *(uint8_t *)((uintptr_t)finfo + pos + 9) = 0;        /* NUL 结尾 */
    *(uint8_t *)((uintptr_t)finfo + 8) = entry[0xb];     /* fattr */
    *(uint32_t *)finfo = *(uint32_t *)(entry + 0x1c);    /* fsize */
    *(uint16_t *)((uintptr_t)finfo + 6) = *(uint16_t *)(entry + 0x16);   /* fdate */
    *(uint16_t *)((uintptr_t)finfo + 4) = *(uint16_t *)(entry + 0x18);   /* ftime */
}

// === fat_dir_entry_remove === 原 FUN_00002f8c @ 0x00002f8c
// 来源: reversed/fsbl_funcs/FUN_00002f8c.c | 分类: fsbl
// 语义: 删除目录项: 若 dj->sect (dj[6]) 已是当前窗口扇区或换窗成功, 将目录项
// 首字节写 0xE5 (删除标记) 并置 FS 脏标志 (fs+3 = 1), 返回 0; 换窗失败返回磁盘错误。
int fat_dir_entry_remove(int *dj)
{
    uint8_t *fs = *(uint8_t **)dj;
    int r;

    if (dj[6] == *(int *)(fs + 0x2c) ||
        (r = fat_move_window(fs, dj[6]), r == 0)) {      /* 原调用缺参, 按 dir_register 同款补 dj[6] */
        *(uint8_t *)dj[7] = 0xE5;                        /* dj[7] = 目录项指针 */
        fs[3] = 1;                                       /* FS 脏标志 */
        return 0;
    }
    return r;
}

// === fat_check_fs === 原 FUN_00002fd8 @ 0x00002fd8
// 来源: reversed/fsbl_funcs/FUN_00002fd8.c | 分类: fsbl
// 语义: 校验目标扇区是否为 FAT 引导记录 (BPB): 清 FS 脏标志并将窗口扇区置 -1
// (强制重载), sect != -1 时装载扇区; 检查 55AA 尾标 (窗口 +510); 再依 BS_JmpBoot
// (win[0], 机器码条件为 UXTB(jmp+0x18)<=1 || jmp==0xEB, 即接受 0xE8/0xE9/0xEB)
// 与 "FAT"(win+0x36) / "FAT32"(win+0x52, 数据串 @0x17244) 判型。
// 返回 0 = FAT12/16 引导记录, 2 = 非 BPB/FAT32, 3 = 55AA 签名错, 4 = 磁盘错误。
int fat_check_fs(uint8_t *fs, uint32_t sect)
{
    uint8_t jmp;
    uint32_t r;

    fs[3] = 0;
    *(uint32_t *)(fs + 0x2c) = 0xFFFFFFFF;               /* 窗口扇区失效 */
    if (sect != 0xFFFFFFFF) {
        r = fat_move_window(fs, sect);
        if (r != 0) {
            return 4;
        }
    }
    if (*(uint16_t *)(fs + 0x23e) != 0xAA55) {           /* 窗口 +510 尾标 */
        return 3;
    }
    jmp = fs[0x40];                                      /* win[0] = BS_JmpBoot */
    /* 原反编译布尔分解: bVar7 = (jmp > 0xEA); jmp != 0xEB 时 bVar7 = (jmp != 0xE8);
     * 进入条件 !bVar7 || jmp==0xEB || jmp==0xE9 —— 等价于 jmp ∈ {0xE8, 0xEB, 0xE9}。 */
    if (jmp == 0xE8 || jmp == 0xEB || jmp == 0xE9) {
        if (fs[0x76] == 'F' && fs[0x77] == 'A' && fs[0x78] == 'T') {   /* win+0x36 = BS_FilSysType */
            return 0;
        }
        {   /* win[0x52..0x56] 与 "FAT32" (数据串 @0x00017244) 逐字节比较 */
            const uint8_t *ref = (const uint8_t *)0x00017244;
            uint8_t *p = fs + 0x93;                      /* win[0x53] */
            int diff = (int)fs[0x92] - (int)ref[0];      /* win[0x52] - 'F' */
            while (diff == 0) {
                ref++;
                diff = (int)*p - (int)*ref;
                p++;
                if (p == fs + 0x97) {                    /* 5 字节比完 */
                    return (diff == 0) ? 0 : 2;
                }
            }
        }
    }
    return 2;
}

/* ==================== FatFs 中段: 目录游标推进 ==================== */

// === fat_dir_sdi === 原 FUN_00003b44 @ 0x00003b44
// 来源: reversed/fsbl_funcs/FUN_00003b44.c | 分类: fsbl
// 语义: 推进目录游标到下一 0x20 字节槽位 (dir_next 语义, 索引为字节偏移):
// dj[4] = 索引, dj[5] = 簇, dj[6] = 扇区, dj[7] = 窗口内项指针, dj[0] = 卷对象。
// 跨扇区时扇区号 +1; FAT12/16 根目录受 n_rootdir (fs+8) 限制; 跨簇时经
// fat_get_fat 取下一簇, 链尾且 stretch 非 0 时经 fat_create_chain 伸展并
// fat_dir_seek 清零新簇。错误路径统一清 dj[6] 并返回 4 (FR_NO_FILE)。
int fat_dir_sdi(int *dj, int stretch)
{
    uint32_t next;                                       /* 下一项字节偏移 */
    uint32_t clst;
    uint32_t max_clst;
    uint32_t sect;
    uint8_t *fs = *(uint8_t **)dj;

    next = *(uint32_t *)&dj[4] + 0x20;
    if (0x1fffff < next) {
        goto fail_end;                                   /* 原 LAB_00003c54 */
    }
    if (dj[6] == 0) {
        return 4;
    }
    if ((next & 0x1ff) == 0) {                           /* 跨扇区 */
        dj[6] = dj[6] + 1;
        if (dj[5] == 0) {                                /* FAT12/16 根目录 */
            if (*(uint16_t *)(fs + 8) <= next >> 5) {
                goto fail_end;
            }
        } else if (((*(uint16_t *)(fs + 0xa) - 1) & (next >> 9)) == 0) {   /* 跨簇 */
            clst = fat_get_fat(fs, dj[5]);
            if (clst < 2) {
                return 2;
            }
            if (clst == 0xFFFFFFFF) {
                return 1;
            }
            max_clst = *(uint32_t *)(fs + 0x14);
            if (max_clst <= clst) {                      /* 链尾: 视 stretch 伸展 */
                if (stretch == 0) {
                    goto fail_end;
                }
                clst = fat_create_chain(dj, dj[5]);
                if (clst == 0) {
                    return 7;
                }
                if (clst == 1) {
                    return 2;
                }
                if (clst == 0xFFFFFFFF) {
                    return 1;
                }
                if (fat_dir_seek(fs, clst) != 0) {       /* 新簇清零 */
                    return 1;
                }
                max_clst = *(uint32_t *)(fs + 0x14);
            }
            dj[5] = clst;
            if (clst - 2 < max_clst - 2) {
                sect = (clst - 2) * *(uint16_t *)(fs + 0xa) + *(uint32_t *)(fs + 0x28);
            } else {
                sect = 0;
            }
            dj[6] = sect;
        }
    }
    *(uint32_t *)&dj[4] = next;
    dj[7] = (int)(fs + 0x40 + (next & 0x1ff));           /* 窗口内项指针 */
    return 0;
fail_end:
    dj[6] = 0;
    return 4;
}

/* ==================== FatFs: 文件读取 / 删除 ==================== */

// === fat_f_read === 原 FUN_00004670 @ 0x00004670
// 来源: reversed/fsbl_funcs/FUN_00004670.c | 分类: fsbl
// 语义: FatFs f_read 主体: 校验对象 (fatfs_validate) 且无错误标志 (fp+0x11),
// 要求 FA_READ (fp+0x10 bit0); 按 min(btr, fsize-fptr) 分段读取: 扇区中部直接
// 从 fp 窗口 (fp+0x40, 512B) 拷贝; 扇区边界换簇/换扇区 (经 fat_get_fat), 单扇区
// 走窗口路径 (脏窗口先回写), 多扇区直达磁盘 (脏窗口覆盖读入缓冲)。每段累计
// *br 并前移 fp[5] (fptr)。错误置 fp->err (1=磁盘错/2=内部错) 并立即返回。
// FIL 字序: [0]=fs, +0x10=标志, +0x11=err, [2]=首簇, [3]=fsize, [5]=fptr,
// [6]=当前簇, [7]=数据扇区, +0x40..=512B 窗口。
uint32_t fat_f_read(uint32_t *fp, void *buff, uint32_t btr, uint32_t *br)
{
    uint8_t *fs;
    uint32_t res;
    uint32_t rcnt;                                       /* 剩余字节数 */
    uint32_t fptr;
    uint32_t off;                                        /* 扇区内字节偏移 */
    uint32_t csect;                                      /* 簇内扇区序号 */
    uint32_t sect;
    uint32_t nsect;                                      /* 本段扇区数 (多扇区路径后转字节数) */
    uint32_t cbytes;                                     /* 本段已读字节数 */
    uint32_t csize;
    uint32_t clst;
    uint8_t *wbuf;                                       /* fp->buf = fp+0x40 */
    uint8_t *dst;
    uint8_t *src;
    uint32_t i;

    *br = 0;
    res = fatfs_validate(fp, (uint32_t *)&fs);
    if (res == 0) {
        res = *(uint8_t *)((uintptr_t)fp + 0x11);        /* fp->err 非 0 直接返回 */
        if (res == 0) {
            if ((*(uint8_t *)((uintptr_t)fp + 0x10) & 1) == 0) {   /* 未按读方式打开 */
                res = 7;
            } else {
                fptr = fp[5];
                rcnt = fp[3] - fptr;                     /* fsize - fptr */
                if (btr <= fp[3] - fptr) {
                    rcnt = btr;
                }
                if (rcnt != 0) {
                    wbuf = (uint8_t *)fp + 0x40;
                    do {
                        off = fptr & 0x1ff;
                        if (off == 0) {
                            /* 扇区边界: 簇内扇区序号 = (fptr>>9) & (csize-1) */
                            csect = (uint32_t)*(uint16_t *)(fs + 0xa) - 1 & fptr >> 9;
                            if (csect == 0) {            /* 跨簇 → 取下一簇 */
                                if (fptr == 0) {
                                    clst = fp[2];        /* 文件首簇 */
                                } else {
                                    clst = fat_get_fat(fs, fp[6]);
                                }
                                if (clst < 2) {
                                    *(uint8_t *)((uintptr_t)fp + 0x11) = 2;
                                    return 2;
                                }
                                if (clst == 0xFFFFFFFF) {
                                    *(uint8_t *)((uintptr_t)fp + 0x11) = 1;
                                    return 1;
                                }
                                fp[6] = clst;
                            } else {
                                clst = fp[6];
                            }
                            if (*(uint32_t *)(fs + 0x14) - 2U <= clst - 2) {   /* 越过最大簇 */
                                *(uint8_t *)((uintptr_t)fp + 0x11) = 2;
                                return 2;
                            }
                            csize = *(uint16_t *)(fs + 0xa);
                            sect = csize * (clst - 2) + *(uint32_t *)(fs + 0x28);
                            if (sect == 0) {
                                *(uint8_t *)((uintptr_t)fp + 0x11) = 2;
                                return 2;
                            }
                            sect += csect;               /* 目标扇区号 */
                            nsect = rcnt >> 9;
                            if (nsect == 0) {
                                /* 单扇区路径: 先保证窗口为目标扇区 (脏窗口先回写) */
                                if (fp[7] != sect) {
                                    if ((int8_t)*(uint8_t *)((uintptr_t)fp + 0x10) < 0) {
                                        if (disk_write(fs[1], wbuf, fp[7], 1) != 0) {
                                            *(uint8_t *)((uintptr_t)fp + 0x11) = 1;
                                            return 1;
                                        }
                                        *(uint8_t *)((uintptr_t)fp + 0x10) &= 0x7f;
                                    }
                                    if (disk_read(fs[1], wbuf, sect, 1) != 0) {
                                        *(uint8_t *)((uintptr_t)fp + 0x11) = 1;
                                        return 1;
                                    }
                                }
                                fp[7] = sect;
                                off = fptr & 0x1ff;      /* 原 uVar12 重取 (= 0) */
                                goto copy_from_window;   /* 原 LAB_000047ac */
                            }
                            /* 多扇区路径: 本簇内尽量多读, 直达用户缓冲 */
                            if (csize < csect + nsect) {
                                nsect = csize - csect;
                            }
                            if (disk_read(fs[1], buff, sect, nsect) != 0) {
                                *(uint8_t *)((uintptr_t)fp + 0x11) = 1;
                                return 1;
                            }
                            if ((int8_t)*(uint8_t *)((uintptr_t)fp + 0x10) < 0 &&
                                (uint32_t)(fp[7] - sect) < nsect) {
                                /* 脏窗口扇区落在本次读范围内 → 用窗口内容覆盖 */
                                uint32_t *sw = (uint32_t *)wbuf;
                                uint32_t *dw = (uint32_t *)buff + (fp[7] - sect) * 0x80;
                                for (i = 0; i < 0x80; i++) {
                                    dw[i] = sw[i];
                                }
                            }
                            cbytes = nsect << 9;         /* 直读字节数, 不过窗口拷贝 */
                            goto accounting;             /* 原 LAB_00004804 */
                        }
                        /* 扇区中部 (窗口已是当前扇区) 也落入窗口拷贝 */
copy_from_window:                                        /* 原 LAB_000047ac */
                        src = wbuf + off;
                        cbytes = 0x200 - off;
                        if (rcnt <= 0x200 - off) {
                            cbytes = rcnt;
                        }
                        dst = (uint8_t *)buff;
                        if (cbytes < 4) {
                            for (i = 0; i < cbytes; i++) {
                                dst[i] = src[i];
                            }
                        } else {
                            uint32_t w = (cbytes - 4) >> 2;
                            uint32_t *sw = (uint32_t *)src;
                            uint32_t *dw = (uint32_t *)dst;
                            for (i = 0; i <= w; i++) {
                                dw[i] = sw[i];           /* (w+1) 个字 */
                            }
                            src = (uint8_t *)(sw + w + 1);
                            dst = (uint8_t *)(dw + w + 1);
                            for (i = 0; i < (cbytes & 3); i++) {
                                dst[i] = src[i];
                            }
                        }
accounting:                                              /* 原 LAB_00004804 */
                        rcnt -= cbytes;
                        fptr += cbytes;
                        buff = (uint8_t *)buff + cbytes;
                        *br += cbytes;
                        fp[5] = fptr;
                    } while (rcnt != 0);
                }
            }
        }
    }
    return res;
}

// === fat_f_unlink === 原 FUN_000054fc @ 0x000054fc
// 来源: reversed/fsbl_funcs/FUN_000054fc.c | 分类: fsbl
// 语义: FatFs f_unlink 主体: 挂卷 (mode 2) + fat_follow_path 定位 → 卷标条目
// (dj+0x2B 字节负, 属性 bit7) 返回 6; 只读属性 (dj+6 字节 bit1... bit0) 返回 7;
// 子目录 (属性 bit4) 时以首簇为根扫描 (fat_dir_sdi0 + fat_dir_next) 确认空目录,
// 非空返回 7; 删目录项 (fat_dir_entry_remove), 簇链非 0 时释放 (fat_remove_chain),
// 最后卷同步 (fat_fs_sync)。
int fat_f_unlink(const uint8_t *path)
{
    const uint8_t *path_m = path;
    uint32_t dj[11];                                     /* 原 local_60: 目录对象 */
    uint32_t sdj[11];                                    /* 原 local_34: 子目录扫描对象 */
    uint8_t *fs;                                         /* 原 local_64 */
    uint32_t clst;                                       /* 原 local_2c */
    uint8_t *dir;                                        /* 原 local_44 = dj[7] */
    int res;

    res = fat_find_volume(&path_m, &fs, 2);
    if (res != 0) {
        return res;
    }
    dj[0] = (uint32_t)(uintptr_t)fs;
    res = fat_follow_path((int *)dj, path_m);
    if (res != 0) {
        return res;
    }
    if ((int8_t)((uint8_t *)dj)[0x2b] < 0) {             /* 卷标条目 */
        return 6;
    }
    if (((uint8_t *)dj)[6] & 1) {                        /* 只读文件 */
        return 7;
    }
    dir = *(uint8_t **)&dj[7];
    clst = *(uint16_t *)(dir + 0x1a);                    /* DIR_FstClusLO */
    if (fs[0] == 3) {                                    /* FAT32: 补首簇高 16 位 */
        clst |= (uint32_t)*(uint16_t *)(dir + 0x14) << 16;
    }
    if (((uint8_t *)dj)[6] & 0x10) {                     /* 子目录: 须为空 */
        sdj[0] = (uint32_t)(uintptr_t)fs;
        sdj[2] = clst;                                   /* dir_sdi0 的目标簇域 */
        res = fat_dir_sdi0((int *)sdj);
        if (res != 0) {
            return res;
        }
        res = fat_dir_next((int *)sdj);
        if (res == 0) {
            return 7;                                    /* 目录非空 */
        }
        if (res != 4) {
            return res;
        }
    }
    res = fat_dir_entry_remove((int *)dj);
    if (res == 0 && clst != 0) {
        res = fat_remove_chain((int *)dj, clst, 0);
    }
    if (res == 0) {
        res = fat_fs_sync(fs);
    }
    return res;
}



