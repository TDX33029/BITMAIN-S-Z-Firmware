// ============================================================================
// BM1746 FSBL 反编译转写 — 半批 p1a (1/2)
// ============================================================================
// 范围: rewritten/batch_plan_other.tsv 中 batch=fsbl_b1 按地址升序的前 20 个函数
//       FUN_0000022c ~ FUN_000095a0 (ONFI NAND 驱动 / 定时器延时 / 基础例程)
// 来源: reversed/fsbl_funcs/
// 命名: rewritten/name_map/fsbl.tsv
// 约定: rewritten/CONVENTIONS.md
// ============================================================================

#include <stdbool.h>   /* bool (disk_write 返回类型) */
#include "../include/common_types.h"

/* ==================== 共享声明 (跨批被调, 签名与反编译/邻批 p4.c 引用侧一致) ==================== */
/* 注: p4.c 将 FUN_00002200/FUN_000027e0 以 mem_copy/mem_set 声明引用;
 * 本文件按 name_map 冻结名 memcpy 定义 FUN_00002200 (见下文), 引用侧命名差异在此说明。 */
extern int  dbg_print(int line, int code);                 /* FUN_000014e4: 行号+错误码日志;
   原第一参为断言格式串指针 (s_0___P___p @0x1720a 起), p4 批已统一折叠为 dbg_print(行号, 0) */
extern void hang_forever(void);                            /* FUN_00002b40 */

/* ROM/高地址段助手 (Ghidra 以 func_0xffffXXXX 引用, 名称原样保留, 不臆造语义名) */
extern void func_0xffffe2d4(uint32_t *now);                /* 读 64 位定时器计数: 写 2 字,
   低字 *now / 高字 *(now+1); 计数频率 ≈333.33MHz (delay_us 系数 0x14d=333 实证) */
extern void func_0xfffff540();                             /* NAND 命令/地址周期助手 (无原型,
   观测调用形: (inst, 描述表指针, 0xffffffff) 读状态类; (inst, 描述表指针, col, row) 页操作类) */
extern int  func_0xffffee18(int inst);                     /* NAND 错误恢复助手, 返回非 0 = 成功 */

/* 本文件内定义、被先文函数调用的前向声明 */
uint16_t nand_status_get(int inst);                        /* FUN_00001554, 见下文 */

/* FatFs 核心 (定义于邻批文件) */
extern int  fat_window_read(uint8_t *fs);                  /* FUN_00002ce0 */
extern int  fat_fs_sync(uint8_t *fs);                      /* FUN_00002e20 */
extern int  fat_move_window(uint8_t *fs, uint32_t sect);   /* FUN_00002f3c */
extern int  fat_check_fs(uint8_t *fs, uint32_t part);      /* FUN_00002fd8 */
extern int  fat_find_volume(const uint8_t **pp, uint8_t **fs_out, uint32_t mode); /* FUN_000030bc */
extern int  fat_get_fat(uint8_t *fs, uint32_t clst);       /* FUN_000036fc */
extern int  fat_remove_chain(int *dj, uint32_t clst, uint32_t free_clst); /* FUN_00003a48 */
extern int  fat_dir_sdi(int *dj, int skip);                /* FUN_00003b44 */
extern int  fat_dir_sdi0(int *dj);                         /* FUN_00003d50 */
extern int  fat_dir_register(int *dj);                     /* FUN_00003e38 */
extern int  fat_dir_next(int *dj);                         /* FUN_00003c84 (邻批定义) */
extern int  fat_dir_entry_remove(int *dj);                 /* FUN_00002f8c */
extern int  fatfs_validate(uint32_t *fp, uint32_t *vol_out); /* FUN_00002bbc */
/* p4.c 以 (const uint8_t *entry, ...) 声明本函数; 实际首参为"指向 dj[7] 槽位指针
 * 字段的指针"(函数内解一次引用得 32 字节目录项地址, 见 FUN_00002c30 反编译)。 */
extern void fat_get_fileinfo(int *slot_ptr_ref, uint32_t *finfo); /* FUN_00002c30 */

/* SD 主机层 (定义于邻批文件) */
/* sd_send_cmd (FUN_00006bcc, 邻批定义): 完整调用形为 (sd, 命令编码, arg, 响应类型),
 * Ghidra 在多处调用点未捕获第 4 参, 故此处按无原型 extern 声明以兼容两种调用形 */
extern uint16_t sd_send_cmd();
extern void sd_adma_setup_cont(int sd, int nblocks);       /* FUN_00007100 */

/* FatFs 磁盘写 (定义于邻批文件, 签名与 p4.c 引用侧一致) */
extern bool disk_write(int pdrv, const void *buf, uint32_t sect, int cnt); /* FUN_0000676c */
extern int disk_read(int pdrv, void *buf, uint32_t sect, int cnt);        /* FUN_0000667c */
extern int aeabi_uidiv(unsigned num, unsigned den);                       /* FUN_00001a50 */

/* libc memset (FUN_000027e0, 邻批 p1b 定义; p4.c 以 mem_set 引用同址函数) */
extern void memset(void *dst, int val, uint32_t n);

/* RSA 软件验证助手 (FUN_00008a00/FUN_00008a78 定义于 p4.c, 签名取其定义) */
extern int  rsa_inv_mod2p32(uint32_t n);   /* Newton 迭代求 n0' = -n^-1 mod 2^32 */
extern void rsa_mont_mul(uint32_t *out, const uint32_t *a, const uint32_t *b,
                         const uint32_t *n, int n0inv);  /* 64 字 Montgomery 乘法 */

/* SHA-256 核心 (FUN_000092c8 定义于邻批) */
extern void sha256_update(int ctx, const void *data, uint32_t len); /* FUN_000092c8 */

/* ---- 全局常量/标志 (值自 analysis/fsbl_ph1_raw.bin 实测, 引用名保留原名可追溯) ---- */
#define g_assert_failed   (*(volatile uint32_t *)0x00028e84)  /* 原 DAT_00028e84:
                                     Xil Assert 错误标志 (0=正常, 1=断言触发), 全 FSBL 共用 */
#define DAT_0000034c      0x00019738  /* 全局, 语义待定: 字面量池常量 → 0x19738 处
                                     NAND 命令描述序列 (0x20000000×6 / 0x3000xxxx 编码), 页编程用 */
#define DAT_00001588      0x0001972c  /* 全局, 语义待定: 字面量池常量 → 同表相邻项, 读状态用 */
#define DAT_00019c10      ((const uint8_t *)0x00019c10)  /* 全局: SHA-256 填充字节块
                                     (首字节 0x80 其余 0x00, 供 sha256_update 追加) */
#define DAT_00004250      ((const uint8_t *)0x0001724d)  /* 全局, 语义待定: 字面量池
                                     常量 (字 @0x00004250 实测 = 0x0001724d) —
                                     follow_path 非法文件名字符表指针 */

/* ==================== NAND 低层驱动 (实例字段实证: +0x10 总线宽度码(0x10=16位),
 *   +0x18 行地址, +0x1C 页大小, +100 数据口指针, +0x6ecc 宽度模式字符;
 *   注: Xilinx 断言式访问器系列把 +8 当 IsReady==0x11111111 用, 而页编程层把
 *   +8 当 SMC 控制寄存区指针用 — 两层实例布局不同, 各自按反编译原样转写) ==================== */

// === nand_page_program === 原 FUN_0000022c @ 0x0000022c
// 来源: reversed/fsbl_funcs/FUN_0000022c.c | 分类: fsbl
// 语义: NAND 页编程 (低层): 宽度模式==2 时先等待 SMC +0x400 寄存器 bit0x40 清零并清
// +0x404 中断使能 bit2/bit3; 发命令/地址周期 (ROM 助手, 参数: 描述表 DAT_0000034c,
// 列地址 col, 行地址 *(inst+0x18)); 将 (页大小-4)>>2 个 32 位字经数据口 *(inst+100)
// 连续写入 FIFO; 数据口指针切到 |0x200000 (主/备用区选择) 后写最后一个字; 等待
// SMC +0x0 寄存器 bit0x40 置位 (命令完成), 写 +0xC=0x10 收尾; nand_status_get 校验,
// 失败且宽度模式==2 时调 ROM 恢复助手。buf 为 NULL 时断言并返回 0。
int nand_page_program(int inst, uint32_t col, uint32_t *buf)
{
    uint16_t page_size;
    uint32_t tail_off;
    uint32_t words;
    uint32_t i;
    uint32_t status;
    volatile uint32_t *fifo;
    volatile uint32_t *fifo_tail;

    if (buf == (uint32_t *)0) {
        dbg_print(0x506, 0);                            /* 原引用 &DAT_00017208 断言串 */
        g_assert_failed = 1;
        return 0;                                       /* false */
    }
    g_assert_failed = 0;

    if (*(char *)(inst + 0x6ecc) == 2) {
        int ctrl = *(int *)(inst + 8);                  /* SMC 控制寄存区指针 (本层用法) */
        while (*(volatile uint32_t *)(ctrl + 0x400) & 0x40) { }   /* 等待 bit0x40 自动清零 */
        *(volatile uint32_t *)(ctrl + 0x404) &= 0xfffffff3u;      /* 清中断使能 bit2/bit3 */
    }

    page_size = *(uint16_t *)(inst + 0x1c);
    func_0xfffff540(inst, DAT_0000034c, col, *(uint32_t *)(inst + 0x18));

    tail_off = (uint32_t)page_size - 4;
    words = tail_off >> 2;
    for (i = 0; i < words; i++) {
        **(uint32_t **)(inst + 100) = buf[i];           /* 经数据口 FIFO 逐字写入 */
    }
    fifo_tail = (volatile uint32_t *)(*(volatile uint32_t *)(inst + 100) | 0x200000);
    *(volatile uint32_t *)(inst + 100) = (uint32_t)(uintptr_t)fifo_tail;  /* 切主/备用区口 */
    *fifo_tail = *(uint32_t *)((uintptr_t)buf + tail_off);                /* 末字单独写入 */

    while ((*(volatile uint32_t *)(*(int *)(inst + 8)) & 0x40) == 0) { }  /* 等命令完成 */
    *(volatile uint32_t *)(*(int *)(inst + 8) + 0xC) = 0x10;

    status = nand_status_get(inst);
    if ((status & 1) != 0) {
        return 1;                                       /* true */
    }
    if (*(char *)(inst + 0x6ecc) == 2) {
        return func_0xffffee18(inst) != 0;
    }
    return 0;
}

// === nand_reg04_get === 原 FUN_00000814 @ 0x00000814
// 来源: reversed/fsbl_funcs/FUN_00000814.c | 分类: fsbl
// 语义: Xilinx NandPs 断言式寄存器访问器 — 读控制器寄存器 +0x04
// (inst+4 = 控制器基址指针, inst+8 = IsReady, 0x11111111 = 已初始化标记)。
uint32_t nand_reg04_get(int inst)
{
    if (inst == 0) {
        dbg_print(0x175, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(inst + 8) == 0x11111111) {
        return *(volatile uint32_t *)(*(int *)(inst + 4) + 4);
    }
    dbg_print(0x176, 0);
    g_assert_failed = 1;
    return 0;
}

// === nand_reg14_get === 原 FUN_00000a14 @ 0x00000a14
// 来源: reversed/fsbl_funcs/FUN_00000a14.c | 分类: fsbl
// 语义: Xilinx NandPs 断言式寄存器访问器 — 读控制器寄存器 +0x14 (同上访问器模式)。
uint32_t nand_reg14_get(int inst)
{
    if (inst == 0) {
        dbg_print(0x1e1, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(inst + 8) == 0x11111111) {
        return *(volatile uint32_t *)(*(int *)(inst + 4) + 0x14);
    }
    dbg_print(0x1e2, 0);
    g_assert_failed = 1;
    return 0;
}

// === nand_reg10_get === 原 FUN_00001140 @ 0x00001140
// 来源: reversed/fsbl_funcs/FUN_00001140.c | 分类: fsbl
// 语义: Xilinx NandPs 断言式寄存器访问器 — 读寄存器 +0x10 并按位取反返回
// (p4 批 nand_ready_flag_get 注: 取反后的最高位即低电平有效的 ready 位)。
uint32_t nand_reg10_get(int inst)
{
    if (inst == 0) {
        dbg_print(0x9e, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(inst + 8) == 0x11111111) {
        return ~*(volatile uint32_t *)(*(int *)(inst + 4) + 0x10);
    }
    dbg_print(0x9f, 0);
    g_assert_failed = 1;
    return 0;
}

// === delay_seconds === 原 FUN_00001458 @ 0x00001458
// 来源: reversed/fsbl_funcs/FUN_00001458.c | 分类: fsbl
// 语义: 忙等 seconds 秒。ROM 64 位定时器 (func_0xffffe2d4) 为时基, 截止值 =
// now + seconds * 0x13de435f (0x13de435f = 333,333,343 ≈ 每秒计数, ≈333.33MHz);
// 轮询直到定时器到达/越过截止值。恒返回 0。
void delay_seconds(uint32_t seconds)
{
    uint32_t now[2];                                   /* 原局部量: 低/高字 */
    uint64_t deadline;

    func_0xffffe2d4(now);
    deadline = (uint64_t)seconds * 0x13de435f + (((uint64_t)now[1] << 32) | now[0]);
    do {
        func_0xffffe2d4(now);
    } while ((((uint64_t)now[1] << 32) | now[0]) < deadline);
}

// === nand_status_get === 原 FUN_00001554 @ 0x00001554
// 来源: reversed/fsbl_funcs/FUN_00001554.c | 分类: fsbl
// 语义: NAND 状态读 (低层): 经 ROM 助手发读状态命令周期 (描述表 DAT_00001588,
// 行地址 0xffffffff), 从数据口 *(inst+100) 读回状态字节 (16 位总线读 16 位字,
// 否则取低字节), 返回低 8 位。
uint16_t nand_status_get(int inst)
{
    uint16_t status;

    func_0xfffff540(inst, DAT_00001588, 0xffffffff);
    if (*(int *)(inst + 0x10) == 0x10) {               /* 16 位总线宽度码 */
        status = **(uint16_t **)(inst + 100);
    }
    else {
        status = (uint8_t)**(uint16_t **)(inst + 100); /* 8 位总线取低字节 */
    }
    return status & 0xff;
}

/* ==================== libc: memcpy (armcc 字对齐优化版) ==================== */

// === memcpy === 原 FUN_00002200 @ 0x00002200
// 来源: reversed/fsbl_funcs/FUN_00002200.c | 分类: fsbl
// 语义: 标准 memcpy (前向复制, dst[0..n) = src[0..n))。原函数为 armcc 多级展开
// 实现: (1) 8 字节对齐路径 — 先把 dst 抄齐到 8 字节边界 (原 (dst*-0x20000000)>>0x1d
// 位技巧 + word/half/byte 展开), 余量 ≥0x2c0 走 0x140 字节大块循环 (8 组 qword
// 暂存), ≥0x240 再走 0x180 字节展开块, 其后 0x40 字节循环, 尾部按 8/4/2/1 字节
// switch 展开 (原 case 0x38..0x0 穿透序列); (2) dst/src 对齐类别不同路径 — 字节/
// 半字前导对齐后 0x40 字节字循环 (8 字暂存双缓冲), 尾部按 4/2/1 字节 switch 展开
// (原 case 0x3c..0x0 穿透序列); (3) n≤0x3f 直接走尾部 switch。
// HintPreloadData(...) 为编译器 cache 预取提示, 按约定转写为注释保留。
// 本转写按 CONVENTIONS §5 恢复意图为等价循环 (内存效果逐字节一致), 分块阈值与
// 对齐分类在注释中保留。
void memcpy(void *dst, const void *src, uint32_t n)
{
    uint8_t *d = (uint8_t *)dst;
    const uint8_t *s = (const uint8_t *)src;
    uint32_t k;

    if ((int)n > 0x3f) {
        if (((uintptr_t)d & 7) == ((uintptr_t)s & 7)) {
            /* ---- 8 字节对齐路径 ---- */
            k = (uintptr_t)d & 7;
            n += k;                                    /* 原: n -= (dst*-0x20000000)>>0x1d */
            while (k != 0) { *d++ = *s++; k--; }       /* 对齐前导 (原 word→half→byte 展开) */
            while (n >= 0x2c0) {                       /* 原 0x140 字节大块循环 (8 组 qword 暂存) */
                uint32_t i;
                for (i = 0; i < 0x140; i++) { d[i] = s[i]; }
                d += 0x140; s += 0x140; n -= 0x140;
            }
            if (n >= 0x240) {                          /* 原 0x180 字节展开块 */
                uint32_t i;
                for (i = 0; i < 0x180; i++) { d[i] = s[i]; }
                d += 0x180; s += 0x180; n -= 0x180;
            }
            while (n >= 0x40) {                        /* 原 0x40 字节循环 */
                uint32_t i;
                for (i = 0; i < 0x40; i++) { d[i] = s[i]; }
                d += 0x40; s += 0x40; n -= 0x40;
            }
            /* 尾部: 8 字节字 (原 switch (n&0x38) case 0x38..8 穿透) + 4/2/1 */
            k = n & 0x38;
            while (k != 0) { *d++ = *s++; k--; n--; }
            if (n & 4) { *(uint32_t *)d = *(const uint32_t *)s; d += 4; s += 4; }
            if (n & 2) { *(uint16_t *)d = *(const uint16_t *)s; d += 2; s += 2; }
            if (n & 1) { *d = *s; }
            return;
        }
        /* ---- dst/src 对齐类别不同路径 ---- */
        /* HintPreloadData(src); HintPreloadData(src+0x10); HintPreloadData(src+0x20); */
        k = (uintptr_t)d & 7;
        n += k;
        while (k != 0) { *d++ = *s++; k--; }           /* 对齐前导 (原 byte→half 展开) */
        /* HintPreloadData(src+0x30); */
        n -= 0x40;                                     /* 原预减 (对应 -1 < (int)n 判断) */
        if ((int)n >= 0) {
            /* HintPreloadData(src+0x40); */
            while (1) {                                /* 原 0x40 字节循环, 8 字暂存双缓冲 */
                uint32_t i;
                for (i = 0; i < 0x40; i++) { d[i] = s[i]; }
                d += 0x40; s += 0x40;
                if (n < 0x40) { break; }               /* 原最后一次进入前判断 param_3 < 0x40 */
                n -= 0x40;
                /* HintPreloadData(src+0x49); */
            }
            n -= 0x40;
        }
        n &= 0x3f;
        if (n == 0) {
            return;
        }
    }
    /* ---- 尾部/小拷贝共用 (原 switch (n&0x3c) case 0x3c..4 穿透 + case 0) ---- */
    k = n & 0x3c;
    while (k != 0) { *d++ = *s++; k--; n--; }
    if (n & 2) { *(uint16_t *)d = *(const uint16_t *)s; d += 2; s += 2; }
    if (n & 1) { *d = *s; }
}

/* ==================== FatFs 核心 (窗口/目录层) ==================== */

// === fat_dir_seek === 原 FUN_00002d5c @ 0x00002d5c
// 来源: reversed/fsbl_funcs/FUN_00002d5c.c | 分类: fsbl
// 语义: 目录簇清零/初始化 (f_mkdir 路径): 窗口脏 (fs[3]) 时先回写窗口
// (fat_window_read); 校验簇号合法 ((clst-2) < (fs+0x14 即 n_fatent)-2) 后计算
// 簇首扇区 = (clst-2) * 每簇扇区数(fs+0x10 处 ushort) + 数据区首扇区(fs+0x28),
// 存入窗口当前扇区 fs+0x2c; 清零 0x200 字节窗口 (fs+0x40..fs+0x240); 再把该零窗口
// 连续写每簇扇区数 (fs+0x10 ushort) 个扇区, 任一盘写失败返回 1, 全部成功返回 0。
// 注: name_map 冻结名 fat_dir_seek, 实际观测语义为目录簇清零。
int fat_dir_seek(uint8_t *fs, uint32_t clst)
{
    uint32_t nsect;
    uint32_t i;
    uint32_t sect;
    int rc;

    if (fs[3] != 0) {                                  /* 窗口脏标志 */
        if (fat_window_read(fs) != 0) {                /* 回写窗口 */
            return 1;                                  /* true: 回写失败 */
        }
    }
    if (clst - 2 < (uint32_t)(*(int *)(fs + 0x14) - 2)) {
        sect = (clst - 2) * (uint32_t)*(uint16_t *)(fs + 0x0a)
             + *(int *)(fs + 0x28);                    /* 簇首扇区 (csize 在 fs+0x0a) */
    }
    else {
        sect = 0;
    }
    *(int *)(fs + 0x2c) = sect;

    for (i = 0; i < 0x200; i++) {                      /* 清零 0x200 字节窗口 */
        fs[0x40 + i] = 0;
    }

    nsect = (uint32_t)*(uint16_t *)(fs + 0x0a);        /* 每簇扇区数 (csize, fs+0x0a) */
    i = 0;
    if (*(uint16_t *)(fs + 0x0a) != 0) {
        do {
            rc = disk_write(fs[1], fs + 0x40, sect + i, 1);
            nsect = (uint32_t)*(uint16_t *)(fs + 0x0a);
            if (rc != 0) {
                break;
            }
            i++;
        } while (i < nsect);
    }
    else {
        nsect = 0;
        i = 0;
    }
    return i != nsect;                                 /* 1 = 有扇区写失败 */
}

// === fat_put_fat === 原 FUN_0000351c @ 0x0000351c
// 来源: reversed/fsbl_funcs/FUN_0000351c.c | 分类: fsbl
// 语义: 向窗口化 FAT 写一个簇链表项 (按 fs[0] 卷类型分派): FAT16 (type 2) 每 512 字节
// 扇区 256 项, 窗口内偏移 (clst&0xff)*2, 扇区 = fatbase(fs+0x20)+clst>>8; FAT32 (type 3)
// 128 项/扇区, 偏移 (clst&0x7f)*4, 写低 28 位并保留条目高 4 位 (原值高位保留:
// 先读窗口 +0x42 处 u16, 目标字节 = 原字节高半 & 0xf0 | val>>24); FAT12 (type 1)
// 条目 1.5 字节: 偶数项写低字节 + 下一字节低半; 奇数项首字节高半 |= val<<4,
// 下一字节 = (val&0xfff)>>4。跨扇区时先回写当前窗口再 fat_move_window 装入目标扇区。
// 每次写入置窗口脏标志 fs[3]=1。非 FAT 卷返回 2, 换窗失败返回其错误码, 成功返回 0。
int fat_put_fat(uint8_t *fs, uint32_t clst, uint32_t val)
{
    uint32_t fatbase = *(uint32_t *)(fs + 0x20);
    uint32_t wsect   = *(uint32_t *)(fs + 0x2c);       /* 窗口当前扇区 */
    uint32_t off;
    uint32_t nxt;                                      /* 原局部量: 条目第二字节字节偏移 */
    uint8_t  hi;                                       /* 原局部量 */
    uint16_t w;                                        /* 原局部量 */
    int rc;

    if (fs[0] == 2) {                                  /* FAT16 */
        if (fatbase + (clst >> 8) == wsect ||
            (rc = fat_move_window(fs, fatbase + (clst >> 8))) == 0) {
            off = (clst & 0xff) * 2;
            fs[off + 0x40] = (uint8_t)val;
            fs[off + 0x41] = (uint8_t)(val >> 8);
            fs[3] = 1;                                 /* 置窗口脏标志 */
            return 0;
        }
        return rc;
    }
    if (fs[0] == 3) {                                  /* FAT32 */
        if (fatbase + (clst >> 7) == wsect ||
            (rc = fat_move_window(fs, fatbase + (clst >> 7))) == 0) {
            val &= 0x0fffffff;                         /* FAT32 条目只有 28 位 */
            off = (clst & 0x7f) * 4;
            w = *(uint16_t *)(fs + off + 0x42);        /* 取原 +0x43 字节以保留其高半 */
            fs[off + 0x40] = (uint8_t)val;
            fs[off + 0x41] = (uint8_t)(val >> 8);
            fs[off + 0x42] = (uint8_t)(val >> 16);
            fs[off + 0x43] = ((uint8_t)(w >> 8) & 0xf0) | (uint8_t)(val >> 24);
            fs[3] = 1;
            return 0;
        }
        return rc;
    }
    if (fs[0] != 1) {                                  /* FAT12 */
        return 2;                                      /* 非 FAT 卷 */
    }
    nxt = clst + (clst >> 1);                          /* 字节偏移 = clst*1.5 */
    if (fatbase + (nxt >> 9) == wsect ||
        (rc = fat_move_window(fs, fatbase + (nxt >> 9))) == 0) {
        hi = (uint8_t)(val >> 8);
        if ((clst & 1) == 0) {                         /* 偶数项: 低字节全量, 高半进下一字节低半 */
            fs[(nxt & 0x1ff) + 0x40] = (uint8_t)val;
            fs[3] = 1;
            if ((uint32_t)(nxt + 1 >> 9) + fatbase != wsect &&
                (rc = fat_move_window(fs, fatbase + (uint32_t)(nxt + 1 >> 9))) != 0) {
                return rc;
            }
            hi = (fs[((nxt + 1) & 0x1ff) + 0x40] & 0xf0) | (hi & 0x0f);
        }
        else {                                         /* 奇数项: 首字节高半 |= 低半, 下一字节 = val 中高半 */
            fs[(nxt & 0x1ff) + 0x40] = (fs[(nxt & 0x1ff) + 0x40] & 0x0f)
                                     | (uint8_t)(val << 4);
            fs[3] = 1;
            if ((uint32_t)(nxt + 1 >> 9) + fatbase != wsect &&
                (rc = fat_move_window(fs, fatbase + (uint32_t)(nxt + 1 >> 9))) != 0) {
                return rc;
            }
            hi = (uint8_t)((val & 0xfff) >> 4);
        }
        fs[((nxt + 1) & 0x1ff) + 0x40] = hi;
        fs[3] = 1;
        return 0;
    }
    return rc;
}

// === fat_create_chain === 原 FUN_00003868 @ 0x00003868
// 来源: reversed/fsbl_funcs/FUN_00003868.c | 分类: fsbl
// 语义: 在 FAT 中为新簇分配/扩展簇链 (dj[0] = fs)。clst==0 (分配新链): 以
// next_free 提示 (fs+0x0c, 越界回退 1) 起循环扫描 get_fat, 首个空闲项 (0) 即分配;
// 空闲簇计数 (fs+0x10) 为 0 或整表扫完 (回到起点) 返回 0 (盘满)。clst!=0 (扩展链):
// 读 clst 表项 — <2 返回 1 (非法), 0xffffffff 透传, <n_fatent (fs+0x14) 说明链已有
// 后继直接返回它, 空闲计数 0 返回 0; 否则先试提示簇 clst+1 (越界回退 2), 再回退到
// next_free/CLST 起全表扫描。分配时对找到的簇写 EOC (0xffffffff), clst!=0 时再
// 把前簇 clst 表项指向新簇; 成功后更新 next_free 提示, 空闲计数 -1 (≤n_fatent-2 时,
// 防下溢), 置 fs+4 字节 bit0 修改标志, 返回新簇号。put_fat 返回 1 时返回 0xffffffff。
uint32_t fat_create_chain(int *dj, uint32_t clst)
{
    uint8_t *fs = *(uint8_t **)dj;                     /* dj[0] = 卷对象 */
    uint32_t n_fatent = *(uint32_t *)(fs + 0x14);
    uint32_t cand;                                     /* 原局部量: 当前候选簇 */
    uint32_t start;                                    /* 原局部量: 搜索起点/回绕检测 */
    uint32_t stat;                                     /* 原局部量: get_fat 返回值 */
    uint32_t free_cnt;                                 /* 原局部量 (作计数用) */
    int rc;

    if (clst == 0) {
        /* ---- 分配全新链 ---- */
        free_cnt = *(uint32_t *)(fs + 0x10);           /* 空闲簇计数 */
        if (free_cnt == 0) {
            return 0;                                  /* 盘满 */
        }
        start = *(uint32_t *)(fs + 0x0c);              /* next_free 提示 */
        if (start == 0 || n_fatent <= start) {
            start = 1;
        }
        cand = start;
    }
    else {
        /* ---- 扩展已有链 ---- */
        stat = fat_get_fat(fs, clst);                  /* clst 表项值 */
        if (stat < 2) {
            return 1;                                  /* 非法簇值 (0/1) */
        }
        if (stat == 0xffffffff) {
            return 0xffffffff;                         /* 表项读错误 */
        }
        if (stat < n_fatent) {
            return stat;                               /* 链已有后继, 直接返回 */
        }
        if (*(uint32_t *)(fs + 0x10) == 0) {
            return 0;                                  /* 无空闲簇 */
        }
        cand = clst + 1;                               /* 提示簇 */
        if (n_fatent <= cand) {
            cand = 2;
        }
        start = fat_get_fat(fs, cand);
        if (start == 0xffffffff || start == 1) {
            return start;                              /* 提示簇表项错误透传 */
        }
        if (start == 0) {
            if (cand != 0) {                           /* cand 恒非 0 (≥2), 原代码判断保留 */
                goto alloc;                            /* 原 标签 0x00003908 */
            }
            start = n_fatent;
            cand = clst;
        }
        else {
            start = *(uint32_t *)(fs + 0x0c);          /* 回退 next_free 提示 */
            if (start < 2 || n_fatent <= start) {
                start = clst;                          /* 提示无效 → 从 clst 全扫 */
            }
        }
        cand = start;
    }
    /* ---- 空闲簇搜索循环 (两种入口共用) ---- */
    while (1) {
        cand = cand + 1;
        if (n_fatent <= cand) {
            cand = 2;                                  /* 回绕到簇 2 */
            if (start < 2) {
                return 0;                              /* 起点已 <2 → 全表扫完, 盘满 */
            }
        }
        stat = fat_get_fat(fs, cand);
        if (stat == 0) {
            break;                                     /* 找到空闲簇 */
        }
        if (stat == 0xffffffff || stat == 1) {
            return stat;                               /* 表项错误透传 */
        }
        if (cand == start) {
            return 0;                                  /* 绕回起点, 盘满 */
        }
    }
alloc:                                             /* 原 标签 0x00003908: 分配 cand */
    if (cand > 1 && cand < n_fatent) {
        rc = fat_put_fat(fs, cand, 0xffffffff);        /* 新簇表项写 EOC */
        if (rc == 0 && clst != 0) {
            if (clst < 2 || n_fatent <= clst) {
                return 1;                              /* 前簇号非法 */
            }
            rc = fat_put_fat(fs, clst, cand);          /* 前簇 → 新簇 */
        }
        if (rc == 0) {
            *(uint32_t *)(fs + 0x0c) = cand;           /* next_free 提示 = 新簇 */
            if (*(uint32_t *)(fs + 0x10) <= n_fatent - 2) {
                *(uint32_t *)(fs + 0x10) -= 1;         /* 空闲计数 -1 (≤n_fatent-2 时) */
            }
            *(uint8_t *)(fs + 4) |= 1;                 /* fs+4 bit0 置位 (卷修改标志) */
            return cand;
        }
        if (rc == 1) {
            return 0xffffffff;
        }
    }
    return 1;
}

// === fat_follow_path === 原 FUN_00003f2c @ 0x00003f2c
// 来源: reversed/fsbl_funcs/FUN_00003f2c.c | 分类: fsbl
// 语义: 沿路径逐分量查找 (FatFs follow_path, 内联了目录项搜索)。dj[0]=fs, dj[2]=当前
// 目录簇 (0=根)。先跳过前导 '/'/'\\'; 空路径 (首字符 <0x20) 时置 dj+0x2b=0x80 (卷标)
// 并 dir_sdi0 后直接返回。每个分量: fn[11] (dj+0x20..0x2a) 清空格, 解析 8.3 名
// (非法字符经 DAT_00004250 表检查, 首比较 0x2a '*'; 双字节码 (DBL) 字符占 2 格;
// '.' 切换到扩展名 (idx=8, 上限 0xb); 超长/非法返回 6); fn[0]==0xE5 转义为 0x05;
// 末分量置 dj+0x2b bit2。dir_sdi0 后扫描目录项: 项首字节 0 = 目录尾 (末分量→4,
// 否则→5), 11 字节名逐字节比较, 命中且末分量→0; 命中非末分量必须是目录 (attr&0x10,
// 否则 5), 取其首簇 (窗口项 +0x1A, FAT32 再加 +0x14 高位) 为 dj[2], 继续下一分量。
int fat_follow_path(int *dj, const uint8_t *path)
{
    uint8_t *fs;
    uint8_t *slot;
    uint8_t attr;                                  /* 原局部量 命中后复用 */
    uint8_t cur;                                   /* 原局部量 (解析期: 当前字符) */
    uint8_t trail;                                 /* 原局部量: DBL 第二字节 */
    uint32_t idx;                                  /* 原局部量: fn 写入下标 */
    uint32_t limit;                                /* 原局部量: 当前段上限 (8 主名/0xb 扩展名) */
    uint32_t next_i;                               /* 原局部量: 下一字符下标 */
    uint32_t prev_i;                               /* 原局部量: 当前字符下标 */
    uint32_t first_clst;                           /* 原局部量 */
    const uint8_t *tab;                            /* 原局部量: 非法字符表游标 */
    const uint8_t *rest;                           /* 原局部量: 分量结束后的剩余路径 */
    uint32_t cmp;                                  /* 原局部量 (复用作比较差值) */
    int rc;

    if (*path == 0x2f || *path == 0x5c) {          /* 跳过前导分隔符 */
        do {
            path++;
        } while (*path == 0x5c || *path == 0x2f);
    }
    dj[2] = 0;                                     /* sclust = 根目录 */
    if (*path < 0x20) {                            /* 空路径: 卷标/起始目录本身 */
        *(uint8_t *)((int)dj + 0x2b) = 0x80;
        return fat_dir_sdi0(dj);
    }

parse_component:                                   /* 原 标签 0x00003f78 */
    {                                              /* fn[11] 清空格 (dj+0x20..0x2a) */
        uint8_t *p = (uint8_t *)dj + 0x20;
        while (p != (uint8_t *)((int)dj + 0x2b)) { *p++ = 0x20; }
    }
    idx = 0;
    limit = 8;
    cur = *path;
    prev_i = 0;
    next_i = 1;
    while ((uint32_t)cur > 0x20) {
        if (cur == 0x5c || cur == 0x2f) {          /* 分量结束 (分隔符) */
            rest = path + next_i;
            if (path[next_i] == 0x2f || path[next_i] == 0x5c) {
                const uint8_t *q = path + prev_i + 2;
                do {
                    rest = q;
                    q++;
                } while (*rest == 0x5c || *rest == 0x2f);   /* 吞掉连续分隔符 */
            }
            goto find;                             /* 原 标签 0x00003ffc */
        }
        if (cur != 0x2e && idx <= limit && limit != idx) {
            /* 合法字符 (idx < limit 且非 '.'): 检查非法字符表 / DBL 双字节 */
            if (cur < 0x81) {
                if (cur == 0x22) {
                    return 6;                      /* '"' 非法 */
                }
                tab = DAT_00004250;                /* 表首字符 0x2a '*' 由初值承担 */
            }
            else if (cur < 0xa0 || ((cur + 0x20) & 0xff) < 0x1d) {
                /* DBL 双字节码首字节 (0x81-0x9f/0xe0-0xff): 校验并写入 2 字节 */
                trail = path[next_i];
                next_i = prev_i + 2;
                if (trail < 0x40) {
                    return 6;
                }
                if (0x7e < trail && 0x7c < (trail ^ 0x80)) {
                    return 6;                      /* 第二字节 0x7f/0xfd-0xff 非法 */
                }
                if (limit - 1 <= idx) {
                    return 6;                      /* 剩余不足 2 格 */
                }
                *(uint8_t *)((int)dj + 0x20 + idx) = cur;
                *(uint8_t *)((int)dj + 0x20 + idx + 1) = trail;
                idx += 2;
                goto advance;                      /* 原 标签 0x00003fe4 */
            }
            else {
                tab = DAT_00004250;
            }
            /* 非法字符表扫描: 先比 0x2a, 再比表内字符 (表[0] 即 0x2a, 循环从表[1] 起) */
            cmp = 0x2a;
            do {
                if (cur == cmp) {
                    return 6;
                }
                cmp = tab[1];
                tab++;
            } while (cmp != 0);
            if ((uint32_t)cur - 0x61 < 0x1a) {     /* 小写 → 大写 */
                cur = cur - 0x20;
            }
            *(uint8_t *)((int)dj + 0x20 + idx) = cur;
            idx += 1;
        }
        else {                                     /* '.' 或超长 */
            if (limit == 0xb || cur != 0x2e) {
                return 6;                          /* 扩展名内 '.' 或超长 */
            }
            idx = 8;
            limit = 0xb;                           /* '.' → 切换扩展名段 */
        }
advance:                                       /* 原 标签 0x00003fe4 */
        cur = path[next_i];
        prev_i = next_i;
        next_i = next_i + 1;
    }
    rest = path + next_i;

find:                                          /* 原 标签 0x00003ffc */
    if (idx == 0) {
        return 6;                              /* 空名 */
    }
    fs = *(uint8_t **)dj;
    if ((char)*(uint8_t *)((int)dj + 0x20) == (char)0xe5) {   /* 0xE5 名首转义 */
        *(uint8_t *)((int)dj + 0x20) = 5;
    }
    *(uint8_t *)((int)dj + 0x2b) = ((uint32_t)*rest < 0x21) ? 4 : 0;  /* 末分量标志 */

    rc = fat_dir_sdi0(dj);
    if (rc != 0) {
        /* 原 标签 0x000040d4 / 标签 0x000040dc */
        if (rc != 4) {
            return rc;
        }
        return (*(uint8_t *)((int)dj + 0x2b) & 4) ? 4 : 5;
    }
    do {                                       /* 目录项扫描 (内联 dir_find) */
        if (dj[6] != *(int *)(fs + 0x2c)) {    /* 窗口不在目标扇区 */
            rc = fat_move_window(fs, dj[6]);
            if (rc != 0) {
                if (rc != 4) {
                    return rc;
                }
                return (*(uint8_t *)((int)dj + 0x2b) & 4) ? 4 : 5;
            }
        }
        slot = *(uint8_t **)&dj[7];
        if (*slot == 0) {                      /* 目录项尾 */
            return (*(uint8_t *)((int)dj + 0x2b) & 4) ? 4 : 5;
        }
        attr = slot[0xb];
        *(uint8_t *)((int)dj + 6) = attr & 0x3f;    /* dj+6 字节 = 属性 (AM_MASK 0x3f) */
        cmp = 0;
        if ((attr & 8) == 0) {                 /* 卷标 (bit3) 不参与名比较 */
            cmp = (uint32_t)slot[0] - *(uint8_t *)((int)dj + 0x20);
            {
                const uint8_t *sp = slot + 1;
                const uint8_t *fp = (uint8_t *)dj + 0x20 + 1;
                do {
                    if (cmp != 0) {
                        break;                 /* 原 标签 0x000040bc 不相等出口 */
                    }
                    cmp = (uint32_t)*sp - *fp;
                    sp++;
                    fp++;
                } while (sp != slot + 0xb);
            }
        }
        if (cmp == 0) {
            break;                             /* 命中 */
        }
        rc = fat_dir_sdi(dj, 0);               /* 下一目录项 */
        if (rc != 0) {
            if (rc != 4) {
                return rc;
            }
            return (*(uint8_t *)((int)dj + 0x2b) & 4) ? 4 : 5;
        }
    } while (1);

    if ((*(uint8_t *)((int)dj + 0x2b) & 4) != 0) {
        return 0;                              /* 末分量命中 */
    }
    if ((attr & 0x10) == 0) {
        return 5;                              /* 非目录, 不能下钻 */
    }
    first_clst = *(uint16_t *)(fs + (dj[4] & 0x1ff) + 0x5a);      /* 项 +0x1A 首簇低 */
    if (fs[0] == 3) {                          /* FAT32: 加 +0x14 高位 */
        first_clst = (uint32_t)*(uint16_t *)(fs + (dj[4] & 0x1ff) + 0x54) << 16
                   | *(uint16_t *)(fs + (dj[4] & 0x1ff) + 0x5a);
    }
    dj[2] = (int)first_clst;                   /* sclust = 首簇 */
    path = rest;                               /* 剩余路径 */
    goto parse_component;
}

// === fat_f_lseek === 原 FUN_00004d90 @ 0x00004d90
// 来源: reversed/fsbl_funcs/FUN_00004d90.c | 分类: fsbl
// 语义: 文件读写指针移动 (f_lseek)。先 fatfs_validate(fp,&fs) 校验; 挂起错误
// (fp+0x11) 非 0 直接返回。只读文件 (fp+0x10 字节 bit1=0) 时 ofs 截到 fsize(fp[3])。
// 记录旧 fptr(fp[5]) 后清 0; ofs==0 即完成。簇内偏移按 csize(fs+0x0a)*0x200 折算:
// 目标簇号 < 旧簇号时回到文件首簇 (fp[2]); 否则从当前簇 (fp[6]) 沿链前进 (读模式走
// fat_get_fat, 写模式 (flag bit1) 走 fat_create_chain 扩链; 盘满则停在簇边界)。
// fsize < 新 fptr 时更新 fsize 并置 flag bit6; 新 fptr 非扇区对齐时把数据窗口
// (fp+0x40) 加载到目标数据扇区 (database + (clst-2)*csize + ofs>>9; 窗口脏 bit7
// 先回写)。错误置 fp+0x11 (1=盘错, 2=非法簇) 并返回, 成功返回 0。
uint32_t fat_f_lseek(uint32_t *fp, uint32_t ofs)
{
    uint32_t fsobj[2];                             /* 原局部量: validate 输出, [0]=fs */
    uint32_t csize;                                /* 原局部量: 每簇扇区数 */
    uint32_t clsize;                               /* 原局部量: 簇字节数 */
    uint32_t clst;                                 /* 原局部量: 当前/目标簇 */
    uint32_t within;                               /* 原局部量: 簇内偏移 (终值=新 fptr) */
    uint32_t prev_fptr;                            /* 原局部量 前段 */
    int tsect;                                     /* 原局部量 后段: 目标扇区 */
    int rc;

    rc = fatfs_validate(fp, fsobj);
    if (rc != 0) {
        return (uint32_t)rc;
    }
    if (*(uint8_t *)((int)fp + 0x11) != 0) {       /* 挂起错误 */
        return *(uint8_t *)((int)fp + 0x11);
    }
    if (fp[3] < ofs && (*(uint8_t *)((int)fp + 0x10) & 2) == 0) {
        ofs = fp[3];                               /* 只读文件截到 fsize */
    }
    prev_fptr = fp[5];
    fp[5] = 0;
    if (ofs == 0) {
        return 0;
    }
    csize = *(uint16_t *)(fsobj[0] + 0x0a);
    clsize = csize * 0x200;
    if (prev_fptr == 0) {
rewind:                                            /* 原 标签 0x00004eec: 从文件首簇起 */
        clst = fp[2];
        if (clst != 0) {
            within = 0;
            fp[6] = clst;                          /* 原 标签 0x00004efc */
            goto walk_done;                        /* 原 标签 0x00004f00 */
        }
        clst = fat_create_chain(fp, 0);            /* 空文件先分配首簇 */
        if (clst != 1) {
            if (clst == 0xffffffff) {
                goto disk_err;                     /* 原 标签 0x00004fc0 */
            }
            fp[2] = clst;
            within = fp[5];
            fp[6] = clst;
            goto walk_done;
        }
        goto bad_clst;                             /* 原 标签 0x00004fd4: 错误 2 */
    }
    else {
        if (aeabi_uidiv(ofs - 1, clsize) < aeabi_uidiv(prev_fptr - 1, clsize)) {
            goto rewind;                           /* 目标在当前簇之前 → 回到首簇 */
        }
        clst = fp[6];
        within = (csize * -0x200u) & (prev_fptr - 1);   /* (fptr-1) mod 簇字节数 */
        fp[5] = within;
        ofs = ofs - within;
    }
walk_done:                                         /* 原 标签 0x00004f00 */
    tsect = 0;
    if (clst != 0) {
        while (clsize < ofs) {                     /* 逐簇前进 */
            fp[5] = clsize + within;
            ofs = ofs - clsize;                    /* 原 += csize*-0x200 */
            if ((*(uint8_t *)((int)fp + 0x10) & 2) == 0) {
                clst = fat_get_fat(*(uint8_t **)fp, clst);   /* 读模式: 沿链 */
            }
            else {
                clst = fat_create_chain(fp, clst); /* 写模式: 扩链 */
                if (clst == 0) {                   /* 盘满: 停在簇边界 */
                    within = fp[5];
                    tsect = 0;
                    goto update_fptr;              /* 原 标签 0x00004e7c */
                }
            }
            if (clst == 0xffffffff) {
                goto disk_err;
            }
            if (clst < 2 || *(uint32_t *)(fsobj[0] + 0x14) <= clst) {
                goto bad_clst;
            }
            fp[6] = clst;
            within = fp[5];
        }
        within = within + ofs;
        fp[5] = within;
        if ((ofs & 0x1ff) == 0) {
            tsect = 0;                             /* 扇区对齐, 无需读窗口 */
        }
        else {
            if ((*(uint32_t *)(fsobj[0] + 0x14) - 2 <= clst - 2) ||
                ((tsect = (uint32_t)*(uint16_t *)(fsobj[0] + 0x0a) * (clst - 2)
                        + *(int *)(fsobj[0] + 0x28)) == 0)) {
                goto bad_clst;                     /* 簇号非法/数据库=0 */
            }
            tsect = tsect + (ofs >> 9);            /* 簇内扇区 */
        }
    }
update_fptr:                                       /* 原 标签 0x00004e7c */
    if (fp[3] < within) {
        fp[3] = within;                            /* fsize 扩展 */
        *(uint8_t *)((int)fp + 0x10) |= 0x40;      /* flag bit6: 尺寸已改 */
    }
    if ((within & 0x1ff) == 0) {
        return 0;
    }
    if (fp[7] == (uint32_t)tsect) {
        return 0;                                  /* 窗口已在目标扇区 */
    }
    if (*(char *)((int)fp + 0x10) < 0) {           /* flag bit7: 数据窗口脏 */
        rc = disk_write(*(uint8_t *)(fsobj[0] + 1), (uint8_t *)((int)fp + 0x40),
                        fp[7], 1);
        if (rc != 0) {
            goto disk_err;
        }
        *(uint8_t *)((int)fp + 0x10) &= 0x7f;      /* 清脏标志 */
    }
    rc = disk_read(*(uint8_t *)(fsobj[0] + 1), (uint8_t *)((int)fp + 0x40),
                   (uint32_t)tsect, 1);
    if (rc != 0) {
        goto disk_err;
    }
    fp[7] = (uint32_t)tsect;                       /* dsect = 目标扇区 */
    return 0;

disk_err:                                          /* 原 标签 0x00004fc0 */
    *(uint8_t *)((int)fp + 0x11) = 1;
    return 1;
bad_clst:                                          /* 原 标签 0x00004fd4 */
    *(uint8_t *)((int)fp + 0x11) = 2;
    return 2;
}

/* ==================== FatFs API: 目录操作 ==================== */

// === fat_f_opendir === 原 FUN_00005008 @ 0x00005008
// 来源: reversed/fsbl_funcs/FUN_00005008.c | 分类: fsbl
// 语义: 打开目录 (f_opendir)。dj 为 NULL 返回 9 (FR_INVALID_OBJECT)。先
// fat_find_volume(&path,&fs,0) 解析卷前缀; 成功后 dj[0]=fs 并 fat_follow_path
// 解析路径。命中且非卷标对象 (dj+0x2b bit7 清) 时: 非 AM_DIR 目录项 → dj[0]=0
// 返回 5; 否则取目录项首簇 (槽位 +0x1A, FAT32 加 +0x14 高位) 为 dj[2]。
// dj+4(u16) = fs->id (fs+6)。fat_dir_sdi0 回绕: 返回 4 (空目录) → dj[0]=0 返回 5,
// 0 → 返回 0, 其它错误 → dj[0]=0 返回错误码。follow_path 返回 4 时同样转 5。
int fat_f_opendir(int *dj, const uint8_t *path)
{
    const uint8_t *pathv[1];                       /* 原局部量[0]: 路径指针槽 */
    uint8_t *fs;                                   /* 原局部量 */
    uint16_t clst_lo;                              /* 原局部量 */
    uint32_t sclust;                               /* 原局部量 */
    int rc;

    if (dj == (int *)0) {
        return 9;
    }
    pathv[0] = path;
    rc = fat_find_volume(pathv, &fs, 0);
    if (rc == 0) {
        *dj = (int)fs;
        rc = fat_follow_path(dj, pathv[0]);
        if (rc == 0) {
            if ((char)*(uint8_t *)((int)dj + 0x2b) >= 0) {    /* bit7 清: 普通目录 */
                if ((*(uint8_t *)((int)dj + 6) & 0x10) == 0) {
                    *dj = 0;                           /* 原 标签 0x000050e0 */
                    return 5;                          /* 非目录 */
                }
                clst_lo = *(uint16_t *)(*(uint32_t *)&dj[7] + 0x1a);
                sclust = (uint32_t)clst_lo;
                if (fs[0] == 3) {                      /* FAT32: 首簇含高位字 */
                    sclust = ((uint32_t)*(uint16_t *)(*(uint32_t *)&dj[7] + 0x14) << 16)
                           | (uint32_t)clst_lo;
                }
                dj[2] = (int)sclust;
            }
            *(uint16_t *)((int)dj + 4) = *(uint16_t *)(fs + 6);   /* 目录对象 id */
            rc = fat_dir_sdi0(dj);
            if (rc == 4) {
                *dj = 0;                               /* 原 标签 0x000050e0: 空目录 */
                return 5;
            }
            if (rc == 0) {
                return 0;
            }
        }
        else if (rc == 4) {                            /* 路径不存在 */
            *dj = 0;                                   /* 原 标签 0x000050e0 */
            return 5;
        }
    }
    *dj = 0;
    return rc;
}

// === fat_f_readdir === 原 FUN_00005114 @ 0x00005114
// 来源: reversed/fsbl_funcs/FUN_00005114.c | 分类: fsbl
// 语义: 读取下一个目录项 (f_readdir)。fatfs_validate 校验 dj; finfo 为 NULL 时
// 仅 dir_sdi0 回绕到目录头。否则 fat_dir_next 前进一项; 返回 0 或 4 (目录尾,
// 4&0xfb==0) 时: 清 finfo+9 (fname 首字节), 当前扇区 dj[6] 非 0 时经
// fat_get_fileinfo(&dj[7], finfo) 填充 (FILINFO: +0 fsize / +4 fdate / +6 ftime /
// +8 fattrib / +9 fname)。最后 fat_dir_sdi(dj,0) 前进, 返回 4 转为 0 (目录尾=正常)。
int fat_f_readdir(int *dj, uint32_t *finfo)
{
    uint32_t rc;                                   /* 原局部量 */
    uint32_t cur_sect;                             /* 原局部量 */
    uint32_t fsobj[2];                             /* 原局部量: validate 输出 */

    rc = fatfs_validate((uint32_t *)dj, fsobj);
    if (rc == 0) {
        if (finfo == (uint32_t *)0) {              /* NULL: 回绕 */
            return fat_dir_sdi0(dj);
        }
        rc = fat_dir_next(dj);
        if ((rc & 0xfb) == 0) {                    /* 0 或 4 */
            cur_sect = *(int *)((int)dj + 0x18);   /* dj[6]: 当前窗口扇区 */
            *(uint8_t *)((int)finfo + 9) = 0;      /* fname[0] = 0 */
            if (cur_sect != 0) {
                fat_get_fileinfo(&dj[7], finfo);   /* 首参为 dj[7] 字段地址 (内部解引用) */
            }
            rc = fat_dir_sdi(dj, 0);               /* 前进到下一项 */
            if (rc == 4) {
                rc = 0;                            /* 目录尾按成功返回 */
            }
        }
    }
    return (int)rc;
}

/* ==================== SD/eMMC 主机层 (ADMA 描述表 + eMMC 卡识别) ==================== */

// === sd_adma_setup === 原 FUN_000071f8 @ 0x000071f8
// 来源: reversed/fsbl_funcs/FUN_000071f8.c | 分类: fsbl
// 语义: 为 SDHCI ADMA2 构建 12 字节/项描述表 (位于 sd+0x80: +0x80 属性 u16,
// +0x82 长度 u16, +0x84 地址 u32, +0x88 保留 u32)。块大小 = *(u16*)(基址+4)&0xfff
// (基址 = *(int*)(sd+4)); 总字节数 = nblocks*块大小。≥64KB 时按 64KB 分段: 前段
// 属性 0x21 (Valid|Trans, 长度 0=64KB), 末段属性 0x23 (Valid|Trans|End), 长度 =
// (u16)(nblocks*块大小)。描述表地址写入 SDHCI +0x58 (ADMA 系统地址寄存器)。
// 尾部 *(char*)(sd+0x20)==0 分支在原反编译中遇坏指令截断, 按产物保留挂死。
void sd_adma_setup(int sd, int nblocks, void *buf)
{
    int base;                                      /* 原局部量: SDHCI 寄存器基址 */
    uint16_t csize;                                /* 原局部量: 块大小 */
    uint32_t total;                                /* 原局部量 */
    uint32_t segs;                                 /* 原局部量: 64KB 段数 */
    uint32_t i;                                    /* 原局部量 */
    int desc;                                      /* 原局部量: 描述项地址 */

    base = *(int *)(sd + 4);
    csize = *(uint16_t *)(base + 4) & 0xfff;
    total = (uint32_t)nblocks * csize;
    i = 0;
    if (total < 0x10000) {
        /* 单段: 直接落末段描述 */
    }
    else {
        segs = total >> 0x10;
        if ((total & 0xffff) != 0) {
            segs = segs + 1;
        }
        if (segs != 1) {
            do {
                desc = sd + i * 0xC;
                *(int *)(desc + 0x84) = (int)buf + i * 0x10000;   /* 段地址 */
                *(uint32_t *)(desc + 0x88) = 0;
                *(uint16_t *)(desc + 0x80) = 0x21; /* Valid|Trans */
                *(uint16_t *)(desc + 0x82) = 0;    /* 长度 0 = 64KB 段 */
                i = i + 1;
            } while (i != segs - 1);
            buf = (uint8_t *)buf + i * 0x10000;    /* 末段地址 (i = segs-1) */
        }
    }
    desc = sd + i * 0xC;
    *(int *)(desc + 0x84) = (int)buf;
    *(uint32_t *)(desc + 0x88) = 0;
    *(uint16_t *)(desc + 0x82) = (uint16_t)((int16_t)nblocks * (int16_t)csize);
    *(uint16_t *)(desc + 0x80) = 0x23;             /* Valid|Trans|End (末段) */
    *(uint32_t *)(base + 0x58) = sd + 0x80;        /* ADMA 系统地址寄存器 */
    if (*(char *)(sd + 0x20) == 0) {
        /* 原反编译在此分支遇坏指令截断 (halt_baddata), 按产物保留 */
        hang_forever();
    }
    return;
}

// === mmc_card_identify === 原 FUN_00007574 @ 0x00007574
// 来源: reversed/fsbl_funcs/FUN_00007574.c | 分类: fsbl
// 语义: eMMC 卡识别序列。断言保护 (sd==NULL → 行 0x6ce; sd+0x24 != 0x11111111
// 未初始化 → 行 0x6cf, 均置断言标志并返回 0)。前提 ((type==2 且 OCR 高位
// (sd+0x28 & 0xC0000000)==0x40000000) || sd+0xC==0 || 卡插入 (基址+0x24 bit16))
// 时: CMD0 → 循环 CMD1 (arg OCR 0x40ff8000, 响应寄存器基址+0x10) 直到 busy 位
// (bit31) 置位; OCR bit30 (CCS) 置 sd+0x30=1 (高容量标志); CMD2 读 CID
// (响应字基址+0x10/+0x14/+0x18/+0x1C → sd+0x40/0x44/0x48/0x4C); RCA=0x1234
// (sd+0x50=0x12340000) → CMD3 → CMD9 (arg=RCA) 读 CSD: sd+0x35 = CSD+0x1C 的
// bits[21:18] (C_SIZE_MULT 域), sd+100 = 容量公式 ((C_SIZE+1)<<...>>9)。
// 任一命令失败返回 1, 成功返回 0。注: 多处调用 sd_send_cmd 的第 4 参 (响应类型)
// 反编译未捕获, 按 p4 批约定省略并在此说明。
int mmc_card_identify(int sd)
{
    uint16_t cid_w1;                               /* 原局部量 */
    uint16_t cid_w2;                               /* 原局部量 */
    int base;                                      /* 原局部量 */
    uint32_t csd_w1;                               /* 原局部量 */
    uint32_t csd_w2;                               /* 原局部量 */
    uint32_t ocr;                                  /* 原局部量 */

    if (sd == 0) {
        dbg_print(0x6ce, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(sd + 0x24) != 0x11111111) {
        dbg_print(0x6cf, 0);
        g_assert_failed = 1;
        return 0;
    }
    if (((((*(char *)(sd + 0x36) == 2) &&
           ((*(uint32_t *)(sd + 0x28) & 0xc0000000) == 0x40000000)) ||
          (*(int *)(sd + 0xC) == 0)) ||
         ((*(uint32_t *)(*(int *)(sd + 4) + 0x24) & 0x10000) != 0)) &&
        (sd_send_cmd(sd, 0, 0) == 0)) {            /* CMD0: GO_IDLE */
        do {
            if (sd_send_cmd(sd, 0x100, 0x40ff8000, 0) != 0) {  /* CMD1: SEND_OP_COND */
                return 1;
            }
            ocr = *(uint32_t *)(*(int *)(sd + 4) + 0x10);      /* R3 响应 (OCR) */
        } while ((int)ocr < 0);                        /* bit31 busy 未置位则重试 */
        if ((ocr & 0x40000000) != 0) {
            *(uint32_t *)(sd + 0x30) = 1;              /* CCS: 高容量标志 */
        }
        if (sd_send_cmd(sd, 0x200, 0) == 0) {          /* CMD2: ALL_SEND_CID */
            base = *(int *)(sd + 4);
            cid_w1 = *(uint16_t *)(base + 0x14);       /* 响应字暂存 */
            cid_w2 = *(uint16_t *)(base + 0x1C);
            *(uint32_t *)(sd + 0x40) = (uint32_t)*(uint16_t *)(base + 0x10);
            *(uint32_t *)(sd + 0x44) = (uint32_t)cid_w1;
            *(uint32_t *)(sd + 0x4C) = (uint32_t)cid_w2;
            *(uint32_t *)(sd + 0x48) = (uint32_t)*(uint16_t *)(base + 0x18);
            *(uint32_t *)(sd + 0x50) = 0x12340000;     /* RCA = 0x1234 */
            if ((sd_send_cmd(sd, 0x300, 0x12340000) == 0) &&      /* CMD3: SET_RELATIVE_ADDR */
                (sd_send_cmd(sd, 0x900, *(uint32_t *)(sd + 0x50)) == 0)) {  /* CMD9: SEND_CSD */
                base = *(int *)(sd + 4);
                csd_w1 = *(uint32_t *)(base + 0x14);   /* CSD 响应字 (原局部量) */
                csd_w2 = *(uint32_t *)(base + 0x18);   /* (原局部量) */
                *(char *)(sd + 0x35) = (char)((*(uint32_t *)(base + 0x1C) & 0x3fffff) >> 0x12);
                *(uint32_t *)(sd + 100) =              /* 容量公式 (原样转写) */
                    ((((csd_w2 & 3) << 10) | (csd_w1 >> 0x16)) + 1)
                        << (((csd_w1 & 0x3ff) >> 7) + 2)
                    << ((csd_w2 & 0xfff) >> 8)
                    >> 9;
                return 0;
            }
        }
    }
    return 1;
}

// === sd_set_card_detect === 原 FUN_00008798 @ 0x00008798
// 来源: reversed/fsbl_funcs/FUN_00008798.c | 分类: fsbl
// 语义: 卡检测 (CD) 引脚配置命令序列 (ACMD42 SET_CLR_CARD_DETECT)。断言保护
// (sd==NULL → 行 0x319; sd+0x24 != 0x11111111 → 行 0x31a, 返回 0)。CMD55 (编码
// 0x3700, arg = RCA sd+0x50) 成功后发 ACMD42 (编码 0xaa00, arg 0 = 清除检测),
// 返回其错误状态 (0 = 命令序列完成); CMD55 失败返回 1。
// 注: ACMD42 调用的第 4 参 (响应类型) 反编译未捕获, 按约定省略。
int sd_set_card_detect(int sd)
{
    int rc;

    if (sd == 0) {
        dbg_print(0x319, 0);
        g_assert_failed = 1;
        return 0;
    }
    g_assert_failed = 0;
    if (*(int *)(sd + 0x24) != 0x11111111) {
        dbg_print(0x31a, 0);
        g_assert_failed = 1;
        return 0;
    }
    rc = sd_send_cmd(sd, 0x3700, *(uint32_t *)(sd + 0x50), 0);   /* CMD55 */
    if (rc == 0) {
        rc = sd_send_cmd(sd, 0xaa00, 0);            /* ACMD42: SET_CLR_CARD_DETECT */
        return rc != 0;
    }
    return 1;
}

// === rsa_bignum_cmp === 原 FUN_00008df8 @ 0x00008df8
// 来源: reversed/fsbl_funcs/FUN_00008df8.c | 分类: fsbl
// 语义: 64 字 (2048 位) 大数比较: 从最高字 (下标 63) 向下找第一个不同的字,
// 返回该字处 a > b 的布尔结果 (全部相同时返回最高字比较结果 = false)。
int rsa_bignum_cmp(const uint32_t *a, const uint32_t *b)
{
    int i;                                         /* 原局部量: 字下标 63..0 */

    i = 0x40;
    do {
        i = i - 1;
        if (*(uint32_t *)((int)a + i * 4) != *(uint32_t *)((int)b + i * 4)) {
            break;
        }
    } while (i > 0);
    return *(uint32_t *)((int)b + i * 4) < *(uint32_t *)((int)a + i * 4);
}

// === rsa_modexp === 原 FUN_000090d8 @ 0x000090d8
// 来源: reversed/fsbl_funcs/FUN_000090d8.c | 分类: fsbl
// 语义: 64 字 (2048 位) 蒙哥马利模幂 (MSB-first 平方-乘法)。栈上建 64 字常量 1
// (memset 后 one[0]=1) 与 64 字 RR 临时; n0' = rsa_inv_mod2p32(n[0]);
// RR_tmp = mont_mul(x, rr); 累加器 out = mont_mul(rr, 1); 对 64 字指数 exp 从
// 最高字到最低字、每字从 bit31 到 bit0: 已遇有效位则先平方 (out*out), 首个有效位
// 记录后置 started, 该位为 1 再乘 RR_tmp (out*RR_tmp); 末尾 out = mont_mul(out, 1)
// 转出蒙哥马利域。参数接线按反编译原样: (param_1=x, param_2=n, param_3=rr,
// param_4=exp, param_5=out)。
void rsa_modexp(uint32_t *x, uint32_t *n, uint32_t *rr, uint32_t *exp, uint32_t *out)
{
    uint32_t rr_tmp[64];                           /* 原局部量[256] */
    uint32_t one[64];                              /* 原局部量[64] */
    uint32_t n0;                                   /* 原局部量 */
    uint32_t word;                                 /* 原局部量 */
    uint32_t mask;                                 /* 原局部量 */
    uint32_t bit;                                  /* 原局部量 */
    uint32_t started;                              /* 原局部量: 已遇首个有效位 */
    int wi;                                        /* 原局部量: 指数字下标 */

    memset(one, 0, 0x100);
    one[0] = 1;
    n0 = rsa_inv_mod2p32(n[0]);
    rsa_mont_mul(rr_tmp, x, rr, n, n0);            /* RR 相关临时积 */
    rsa_mont_mul(out, rr, one, n, n0);             /* 累加器初值 */
    for (wi = 0x3f; wi >= 0; wi--) {
        word = exp[wi];
        for (mask = 0x80000000u; mask != 0; mask = mask >> 1) {
            bit = word & mask;
            if (started != 0) {
                rsa_mont_mul(out, out, out, n, n0);        /* 平方 */
            }
            if (started == 0) {
                started = bit;                     /* 跳过前导 0 位 */
            }
            if (bit != 0) {
                rsa_mont_mul(out, out, rr_tmp, n, n0);     /* 乘 RR_tmp */
            }
        }
    }
    rsa_mont_mul(out, out, one, n, n0);            /* 乘 1: 转出蒙哥马利域 */
}

/* ==================== SHA-256 (final) ==================== */

// === sha256_final === 原 FUN_000095a0 @ 0x000095a0
// 来源: reversed/fsbl_funcs/FUN_000095a0.c | 分类: fsbl
// 语义: SHA-256 收尾 (ctx: +0x00..0x3C 状态 H[0..7], +0x60/+0x64 字节计数低/高字,
// +0x68 64 字节块缓冲; out = 32 字节摘要)。把 64 位位长 (字节计数 << 3) 以大端
// 8 字节展开 (原函数以 (hi&0x1fffffff)>>21 等移位逐字节计算); 按 ctx 已缓冲字节
// 数 (计数 & 0x3f) 追加填充: <0x38 填 0x38-len 字节, 否则填 0x78-len 字节
// (输入来自全局填充块 DAT_00019c10, 首字节 0x80 其余 0x00); 追加 8 字节长度后,
// 8 个状态字按大端逐字节写出 32 字节摘要。原第一次 update 调用的多余参数为
// Ghidra 寄存器污染伪参数, 实际签名为 (ctx, data, len)。
void sha256_final(int ctx, uint8_t *out)
{
    uint32_t len_lo;                               /* 原 *(ctx+0x60) */
    uint32_t len_hi;                               /* 原 *(ctx+0x64) (word @+100) */
    uint64_t bitlen;                               /* 64 位位长 (原 8 个移位字节) */
    uint8_t  tail[8];                              /* 原局部量..局部量 */
    uint32_t buffered;                             /* 原局部量: 计数 & 0x3f */
    int pad_len;                                   /* 原局部量 */
    int i;                                         /* 原局部量 */

    len_lo = *(uint32_t *)(ctx + 0x60);
    len_hi = *(uint32_t *)(ctx + 0x64);
    bitlen = ((uint64_t)len_hi << 32 | len_lo) << 3;   /* 位长 = 字节计数 << 3 */
    for (i = 0; i < 8; i++) {                      /* 大端 8 字节 (原逐字节移位展开) */
        tail[i] = (uint8_t)(bitlen >> (56 - i * 8));
    }
    buffered = len_lo & 0x3f;
    if (buffered < 0x38) {
        pad_len = 0x38 - buffered;
    }
    else {
        pad_len = 0x78 - buffered;
    }
    sha256_update(ctx, DAT_00019c10, (uint32_t)pad_len);    /* 追加 0x80... 填充 */
    sha256_update(ctx, tail, 8);                   /* 追加 64 位位长 */
    for (i = 0; i < 8; i++) {                      /* 状态字 → 大端摘要 */
        uint32_t h = *(uint32_t *)(ctx + i * 4);
        *(char *)(out + i * 4)     = (char)(h >> 0x18);
        *(char *)(out + i * 4 + 1) = (char)(h >> 0x10);
        *(char *)(out + i * 4 + 2) = (char)(h >> 8);
        *(char *)(out + i * 4 + 3) = (char)h;
    }
    return;
}

/* ==================== 覆盖率表 ====================
 * 分配 (batch fsbl_b1, 按地址升序前 20/40): 20 函数 — 全部转写。
 * | # | 语义名               | 原 FUN/符号     | 地址       | 一句话语义                              |
 * |---|----------------------|-----------------|------------|-----------------------------------------|
 * | 1 | nand_page_program    | FUN_0000022c    | 0x0000022c | NAND 页编程 (ROM 助手+数据口 FIFO 写)   |
 * | 2 | nand_reg04_get       | FUN_00000814    | 0x00000814 | NAND 寄存器 +0x04 读 (断言式)           |
 * | 3 | nand_reg14_get       | FUN_00000a14    | 0x00000a14 | NAND 寄存器 +0x14 读 (断言式)           |
 * | 4 | nand_reg10_get       | FUN_00001140    | 0x00001140 | NAND 寄存器 +0x10 读并取反              |
 * | 5 | delay_seconds        | FUN_00001458    | 0x00001458 | 64 位定时器忙等 seconds 秒 (×0x13de435f)|
 * | 6 | nand_status_get      | FUN_00001554    | 0x00001554 | NAND 状态读 (数据口低 8 位)             |
 * | 7 | memcpy               | FUN_00002200    | 0x00002200 | armcc 字对齐优化 memcpy (恢复为等价循环)|
 * | 8 | fat_dir_seek         | FUN_00002d5c    | 0x00002d5c | 目录簇清零 (窗口回写+逐扇区零写)        |
 * | 9 | fat_put_fat          | FUN_0000351c    | 0x0000351c | FAT12/16/32 表项写 (窗口化)             |
 * |10 | fat_create_chain     | FUN_00003868    | 0x00003868 | FAT 空闲簇搜索+链分配 (EOC+回链)        |
 * |11 | fat_follow_path      | FUN_00003f2c    | 0x00003f2c | 路径逐分量解析查找 (内联目录项比较)     |
 * |12 | fat_f_lseek          | FUN_00004d90    | 0x00004d90 | 读写指针移动 (沿簇链前进+窗口加载)      |
 * |13 | fat_f_opendir        | FUN_00005008    | 0x00005008 | 打开目录 (find_volume+follow_path)      |
 * |14 | fat_f_readdir        | FUN_00005114    | 0x00005114 | 读目录项 (dir_next+FILINFO 填充)        |
 * |15 | sd_adma_setup        | FUN_000071f8    | 0x000071f8 | SDHCI ADMA2 描述表构建 (64KB 分段)      |
 * |16 | mmc_card_identify    | FUN_00007574    | 0x00007574 | eMMC 识别: CMD0/1/2/3/9+CSD 容量        |
 * |17 | sd_set_card_detect   | FUN_00008798    | 0x00008798 | CMD55+ACMD42 卡检测配置                 |
 * |18 | rsa_bignum_cmp       | FUN_00008df8    | 0x00008df8 | 64 字大数比较 (a>b 布尔)                |
 * |19 | rsa_modexp           | FUN_000090d8    | 0x000090d8 | 2048 位蒙哥马利模幂 (平方-乘法)         |
 * |20 | sha256_final         | FUN_000095a0    | 0x000095a0 | SHA-256 收尾 (填充+位长+大端摘要)       |
 */

