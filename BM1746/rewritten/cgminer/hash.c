/*
 * rewritten/cgminer/hash.c — 50_crypto_hash 批次 (batch=hash_b1, 12 函数) — BM1746 固件语义转写
 * =========================================================
 * 原始二进制: analysis/cgminer_z15_bm1746.elf (ARM32, SD 镜像 cgminer-z15, branch "bm1746")
 * 来源分类:   reversed/cgminer/50_crypto_hash/ (清单见 rewritten/batch_plan.tsv batch=hash_b1)
 *
 * 模块功能概述 (该分类实为 cgminer 通用工具 + zcash 驱动接入 + Equihash BLAKE2b 初始化,
 * 并不含 SHA-256/CRC16 例行 — SHA-256 四件套在 Equihash 侧 0x4b4f0/0x4dbec/0x4dccc,
 * 属 D 片批次; CRC16 表属帧校验批次):
 *   1) 二进制↔十六进制文本转换 (util.c): __bin2hex / bin2hex / hex2bin。
 *      hex2bin 用 rodata 0x64E8C 处 256×u32 查找表 ('0'-'9'/'a'-'f'/'A'-'F' → 0..15,
 *      其余 0xFFFFFFFF), 256 项已从 ELF 逐字核对。
 *   2) ccan/opt 选项系统 (cgminer 捆绑版): opt_usage 生成用法串 (计数预算/填充串/
 *      " <arg>"/" (default: %s)" 全部与 rodata 字符串 0x6D154..0x6D184 对上),
 *      opt_verusage_and_exit 打印版本 + "Built with bitmain_zcash mining support." 后退出。
 *   3) driver-btm-zcash.c 锁初始化失败处理: mutex_init_zcash / rwlock_init_zcash
 *      (拼 errno 日志 → applog(LOG_ERR) → quit(1))。
 *   4) Equihash BLAKE2b 初始化: blake2b_init_state (状态清零 0x168 字节 + 装载标准
 *      BLAKE2b IV 并异或 64 字节参数块) 与 equihash_solver (构造 ZcashPoW 参数块)。
 *      IV 常量 rodata 0x6C540..0x6C57F 已从 ELF 逐字核对, 与标准 BLAKE2b IV 一致;
 *      digest_length = N*(512/N)/8 (N=200 → 50), 参数块偏移 56/60 额外写入 N/K
 *      (标准 BLAKE2b 参数块该区为保留位 — 按反编译原样保留)。
 *   5) 设备注册与链重初始化 (driver-btm-zcash): add_cgpu (驱动名计数哈希表 + devices
 *      数组追加), bitmain_ZCASH_detect (calloc cgpu/info 后注册), bitmain_ZCASH_reinit_chain
 *      (复位/设址/频率/超时 80000/ticket mask/温度读回 依次逐链下发)。
 *
 * 事实还原要点 (全部以反编译 + 机器码/ELF 数据为证):
 *   - add_cgpu 内的驱动名哈希为 Bob Jenkins lookup2 风格混合 (9 步移位 13,8,13,12,16,5,
 *     3,10,15), 初值 a=b=0x9e3779b9、c=0xfeedbeef, 且长度在块循环之后并入 c
 *     (与 lookup2 原版把长度并入初值不同, 本固件为变体); 哈希表控制块 11 字 (0x2C),
 *     槽位数组每槽 12 字节 (头指针带 +8 tag / 链长计数 / 深度比), 表满按 (slot 计数 ≥
 *     (slot 深度比+1)*10) 且未标记退化时翻倍重建。
 *   - opt_verusage_and_exit 的第 1 形参被原样作为 opt_usage 的 extra 实参传入
 *     (机器码 0x0000F9E0 处 mov r1,r4), exit 固定 0; 上游 cgminer 此处传 NULL,
 *     调用点传 0 时两者等价 (extra==0 走默认 usage 查找路径)。
 *   - opt_usage 中 sprintf("%s:\n") 与 sprintf("%.*s") 的可变实参被 Ghidra 丢失,
 *     已按 rodata 字符串/计数预算/上游 ccan/opt usage.c 恢复 (子表行打 entry 描述串;
 *     填充 = "%.*s" 取 OPT_SPACE_PAD(20 空格, 0x6D16C), 精度 len<20 ? 20-len : 1)。
 *   - 原固件多处把字符串字面量拆成栈上逐字赋值 (Ghidra 显示为单字节写入), 转写还原为
 *     等价 snprintf 字面量 — 产生的缓冲内容与 applog 调用完全一致 (uart.c 同约定)。
 *   - 日志门控 (反编译归纳, 见 uart.c 同款宏): prio 7 需 opt_debug && (A||B||C>6),
 *     prio 5 需 A||B||C>4, prio 3 需 A||B||C>2; applog 第 3 参非 0 强制 curses 刷新。
 *
 * 转写约定: rewritten/CONVENTIONS.md; 命名事实源 rewritten/name_map/cgminer.tsv
 * (自报名字符串按规则以小写下划线为函数名, 字符串字面量本身原样保留)。
 */

#include <stdint.h>
#include <stdio.h>      /* printf/snprintf/sprintf/fflush */
#include <stdlib.h>     /* malloc/calloc/realloc/free/exit */
#include <string.h>     /* strlen/memcmp/memcpy/strchr */
#include <errno.h>
#include <time.h>       /* time/time_t */
#include <unistd.h>     /* sleep */
#include <stdbool.h>
#include <pthread.h>

#include "../include/common_types.h"

/* ==================== 外部函数 (名取自 name_map/cgminer.tsv) ==================== */

extern void     applog(uint32_t prio, char *buf, ...);    /* 原 FUN_00038730: 第 3 参非 0 → 强制 curses 刷新 */
extern void     quit(int status);                                 /* 原 FUN_00016ca8 */
extern void     cgsleep_ms(int ms);                               /* 原 FUN_0002a92c */
extern void     reset_chain(int dev, uint32_t chain);             /* 原 FUN_00046434 (16_chain_mgmt) */
extern void     software_set_address_chain(int chain);            /* 原 FUN_00042f7c */
extern void     set_frequency_chain(int chain, int freq);         /* 原 FUN_000435e0 (12_freq_pll) */
extern void     set_core_timeout_chain(int chain, uint32_t timeout); /* 原 FUN_000430a0 */
extern void     set_ticket_mask_chain(int chain, uint32_t mask);  /* 原 FUN_00044004 */
extern void     enable_read_temperature_from_asic_chain(uint32_t chain); /* 原 FUN_0004601c */
extern void     wr_lock_error(const char *fn, int line);          /* 原 FUN_0000b480: pthread_rwlock_wrlock 失败日志+退出 */
extern void     rw_unlock_error(const char *fn, int line);        /* 原 FUN_0000b630 */
extern void     mutex_lock_fail_cgminer(const char *fn, int line);  /* 原 FUN_0000b414 */
extern void     mutex_unlock_fail_cgminer(const char *fn, int line);/* 原 FUN_0000b5c4 */
extern char    *first_sopt(int *idx);                             /* 原 FUN_0005bab8 (ccan/opt, 短选项迭代) */
extern char    *next_sopt(char *cur, int *idx);                   /* 原 FUN_0005bb14 */
extern void     opt_usage_and_exit(char *arg);                    /* 原 FUN_0005b3e0 (仅作函数指针比较用) */
extern int     *__errno_location(void);                           /* glibc errno 获取 (反编译原样调用) */
extern void     __assert_fail(const char *assertion, const char *file, unsigned int line,
                              const char *function);              /* glibc 断言失败 (bitmain_zcash_detect) */

/* ==================== 外部全局 (DAT_ 保留原名, 语义见注释) ==================== */

/* ---- 日志门控 (与 uart.c/axi.c 同一组全局) ---- */
extern char DAT_0007ad48;       /* applog (FUN_00038730) 内非 0 走 syslog() 输出; 兼作日志门控条件 A */
extern char DAT_00078e08;       /* 冗余日志开关 B (curses 'v' Verbose mode 联动置位) */
extern int  DAT_000766c4;       /* 日志级别 (int); 调用点以 (优先级-1) < 该值 门控 */
extern char DAT_00078e09;       /* opt_debug (curses 'd' Debug mode), LOG_GATE7 前置条件 */

/* ---- ccan/opt 选项系统 ---- */
extern char    *DAT_0009d780;   /* 程序名指针 (opt_usage(argv0,...) 的 argv0 实参; 上游同位 opt_argv0) */
extern int      DAT_0009d770;   /* opt_num_short: 已注册短选项计数 (usage 计数预算用) */
extern int      DAT_0009d774;   /* opt_count: 选项表项数 */
extern uint32_t *DAT_0009d77c;  /* opt_table: 选项表 (7×u32/项, 表体在 .data 0x75600 起, axi.c 已核实) */
extern char    *DAT_0009d784;   /* opt_hidden 单字节哨兵对象; 其地址作 opt_table 描述字段的"跳过"标记
                                 * (axi.c 同款 extern: --logfile 等选项共用的串变量地址) */

/* ---- util.c 二进制↔hex ---- */
extern const uint32_t DAT_00064e8c[256];  /* hex 字符→半字节查找表 (rodata 0x64E8C, ELF 已核对):
                                           * '0'-'9' → 0..9, 'a'-'f'/'A'-'F' → 10..15, 其余 0xFFFFFFFF(-1) */

/* ---- driver-btm-zcash / 设备注册 ---- */
extern char  DAT_00076bf4[];    /* packagename 串缓冲 (main 启动时生成, printf %s 实参) */
extern char  DAT_00076ac8[];    /* bitmain_ZCASH 驱动描述结构 (device_drv, .data 初始化) */
extern uint8_t DAT_0009d705[];  /* 每链频率字节表 (运行期填充; set_frequency_chain 第 2 实参来源) */
extern char  DAT_000790ec[];    /* 每链"已重初始化"标志数组 (reinit 完成置 1) */
extern char  DAT_00079134[];    /* 链重初始化互斥锁 (pthread_mutex_t) */
extern char  DAT_0007b700[];    /* devices_lock 读写锁 (pthread_rwlock_t) */
extern char  DAT_0007ad4c[];    /* hash_lock 互斥锁 (pthread_mutex_t) */
extern void *DAT_0007b628;      /* devices 数组 (cgpu_info* 槽, realloc 扩容) */
extern int   DAT_0007b720;      /* devices 数组常规段计数 (上游同位 total_devices) */
extern int   DAT_0007872c;      /* devices 数组追加段计数 (上游同位 dev_count/new_devices) */
extern char  DAT_0007b7d0;      /* 追加段模式标志: 0=追加至常规段并推进常规计数, 非 0=写入追加段 */
extern int   DAT_0007b8b0;      /* 最大设备数统计时从常规计数扣除的偏移 (语义待定) */
extern int   DAT_0007702c;      /* 历史最大设备数高水位 (total_devices - DAT_0007b8b0 与之比较) */
extern char *DAT_00078728;      /* 驱动名计数哈希表根: 首表项 (0x28 字节, 兼作内联名字符串),
                                 * 其 +0x08 存哈希表控制块指针; NULL = 尚未建立 */
extern void (*DAT_00075ed8)(void); /* 全局函数指针: ELF 初值 noop_hook (FUN_00028ae0, 恒 0),
                                    * 各功能函数尾部经指针调用, 运行期未见改写 [低置信] */
extern uint8_t DAT_00076b60;    /* ticket mask 运行值 (出厂静态初值 0x10, bm1746_regs.h 同源) */

/* ==================== 局部命名常量与宏 ==================== */

/* ---- 调用点日志门控 (统一模式, 见文件头说明; 与 uart.c 同款) ---- */
#define LOG_GATE7() (DAT_00078e09 && (DAT_0007ad48 || DAT_00078e08 || 6 < DAT_000766c4))
#define LOG_GATE5() (DAT_0007ad48 || DAT_00078e08 || 4 < DAT_000766c4)
#define LOG_GATE3() (DAT_0007ad48 || DAT_00078e08 || 2 < DAT_000766c4)

/* ---- ccan/opt 选项表项 (与 axi.c 同一定义; 表项 = 7×u32) ---- */
#define OPT_OFF_NAME       0   /* 选项名串 (含 '|' 别名) */
#define OPT_OFF_TYPE       1   /* 类型: 2=OPT_HASARG(带参) 8=OPT_SUBTABLE(子表) 0x10=表尾 */
#define OPT_OFF_NOARG_CB   2   /* 无参选项回调 (usage 的 extra 查找按 == opt_usage_and_exit 匹配) */
#define OPT_OFF_ARG_CB     3   /* 有参选项回调 */
#define OPT_OFF_SHOW       4   /* show 回调 (show(buf, value), 输出 "(default: %s)") */
#define OPT_OFF_VALUE      5   /* 选项变量指针 / 无参选项的 carg (usage extra 来源) */
#define OPT_OFF_DESC       6   /* 描述串; == &DAT_0009d784(opt_hidden) 的选项在 usage 中跳过 */
#define OPT_TABLE_STRIDE   7

/* ccan/opt usage 生成所用常量 (rodata 0x6D154..0x6D184 一带, ELF 已核对) */
#define OPT_SPACE_PAD   "                    "   /* 20 空格 + NUL @0x6D16C: 名字列→描述列填充串 */
#define OPT_SHOW_LEN    80                        /* show 回调缓冲长度 (showbuf = OPT_SHOW_LEN+sizeof("...")) */

/* ---- Equihash BLAKE2b 参数块 ---- */
#define BLAKE2B_STATE_SIZE   0x168   /* blake2b_state 初始化清零长度 (360 字节) */
#define BLAKE2B_BLOCK_SIZE   0x40    /* 参数块 64 字节 */

/* 标准 BLAKE2b IV (blake2b_init_state 从 rodata 0x6C540 装载; ELF 逐字核对与标准一致) */
static const uint64_t BLAKE2B_IV[8] = {
    0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
    0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
    0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
    0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
};

/* ---- Bob Jenkins lookup2 风格混合 (add_cgpu 驱动名哈希; 原码以宏形式内联 4 处) ----
 * 9 步移位序列 13,8,13,12,16,5,3,10,15; 反编译四处展开逐条一致。 */
#define LOOKUP2_MIX(a, b, c) do { \
    (a) -= (b); (a) -= (c); (a) ^= (c) >> 13; \
    (b) -= (c); (b) -= (a); (b) ^= (a) << 8;  \
    (c) -= (a); (c) -= (b); (c) ^= (b) >> 13; \
    (a) -= (b); (a) -= (c); (a) ^= (c) >> 12; \
    (b) -= (c); (b) -= (a); (b) ^= (a) << 16; \
    (c) -= (a); (c) -= (b); (c) ^= (b) >> 5;  \
    (a) -= (b); (a) -= (c); (a) ^= (c) >> 3;  \
    (b) -= (c); (b) -= (a); (b) ^= (a) << 10; \
    (c) -= (a); (c) -= (b); (c) ^= (b) >> 15; \
} while (0)

/* 小端 32 位装载 (反编译逐字节拼装: p[0] | p[1]<<8 | p[2]<<16 | p[3]<<24) */
#define LE32_LOAD(p) ((uint32_t)(uint8_t)((const uint8_t *)(p))[0]       | \
                      ((uint32_t)(uint8_t)((const uint8_t *)(p))[1] << 8)  | \
                      ((uint32_t)(uint8_t)((const uint8_t *)(p))[2] << 16) | \
                      ((uint32_t)(uint8_t)((const uint8_t *)(p))[3] << 24))

/* ==================== 本文件导出 ==================== */

void     mutex_init_zcash(int line);
void     opt_verusage_and_exit(int more);
void     __bin2hex(char *dst, const uint8_t *src, int len);
char    *bin2hex(const uint8_t *data, int len);
int      hex2bin(uint8_t *dst, const uint8_t *src, uint32_t dst_len);
char    *opt_usage(const char *argv0, const char *extra);
uint32_t blake2b_init_state(uint64_t *state, const uint8_t *block);
void     equihash_solver(uint64_t *state, int N, int K);
uint32_t add_cgpu(int cgpu);
void     bitmain_zcash_detect(void);
void     bitmain_zcash_reinit_chain(uint32_t *param_1);

/* ==================== 函数体 ==================== */

// === mutex_init_zcash === 原 FUN_0000b8b8 @ 0x0000b8b8
// 来源: reversed/cgminer/50_crypto_hash/FUN_0000b8b8.c | 分类: 50_crypto_hash
// 语义: driver-btm-zcash.c 互斥锁初始化失败处理: 拼装 "Failed to pthread_mutex_init
//       errno=%d in driver-btm-zcash.c bitmain_ZCASH_prepare():<行号>" 日志
//       (applog LOG_ERR, 第 3 参 1 = 强制 curses 刷新) 后 quit(1) 退出。
//       行号由调用方传入 (bitmain_ZCASH_prepare 内各 pthread_mutex_init 调用点各自传行号)。
void mutex_init_zcash(int line)
{
    int  *errno_loc;
    char  logbuf[4096];

    errno_loc = __errno_location();
    snprintf(logbuf, 0x1000, "Failed to pthread_mutex_init errno=%d in %s %s():%d", *errno_loc,
             "driver-btm-zcash.c", "bitmain_ZCASH_prepare", line);
    applog(3, logbuf, 1);
    quit(1);
    return;
}

// === opt_verusage_and_exit === 原 FUN_0000f9c0 @ 0x0000f9c0
// 来源: reversed/cgminer/50_crypto_hash/FUN_0000f9c0.c | 分类: 50_crypto_hash
// 语义: "--version" 选项回调 (util.c): 打印 packagename + "Built with bitmain_zcash
//       mining support." 行, 再打印 opt_usage 生成的完整用法串并 fflush 后 exit(0)。
//       机器码 0x0000F9E0 处 mov r1,r4: 第 1 形参原样传入 opt_usage 的 extra 实参
//       (上游 cgminer 此处为 NULL; 调用点传 0 时 extra==0 走默认查找路径, 两者等价)。
void opt_verusage_and_exit(int more)
{
    char *usage;

    printf("%s\nBuilt with bitmain_zcash mining support.\n", DAT_00076bf4);
    usage = opt_usage(DAT_0009d780, (const char *)more);   /* 反编译实参原样 (Ghidra 未定型 → char*) */
    printf("%s", usage);
    fflush(stdout);
    exit(0);
}

// === __bin2hex === 原 FUN_000296d8 @ 0x000296d8
// 来源: reversed/cgminer/50_crypto_hash/FUN_000296d8.c | 分类: 50_crypto_hash
// 语义: bin2hex 的不分配变体 (util.c): 把 len 字节二进制按小数位在前/大数位在后写成
//       2*len 个 hex 小写字符写入 dst, 末尾补 '\0'。len < 1 时仅写 '\0'。
//       数字表 rodata 实际为合并字面量 "0123456789abcdefbin2hex" (相邻字符串),
//       索引仅取 0..15, 与 16 字符数字表逐字节等价。
void __bin2hex(char *dst, const uint8_t *src, int len)
{
    char *end;

    if (len < 1) {
        end = dst;
    } else {
        end = dst + len * 2;
        do {
            dst[0] = "0123456789abcdef"[src[0] >> 4];
            dst[1] = "0123456789abcdef"[src[0] & 0xf];
            dst += 2;
            src++;
        } while (dst != end);
    }
    *end = '\0';
    return;
}

// === bin2hex === 原 FUN_00029730 @ 0x00029730
// 来源: reversed/cgminer/50_crypto_hash/FUN_00029730.c | 分类: 50_crypto_hash
// 语义: 二进制转 hex 字符串 (util.c): 分配 (len*2+1) 向上凑整到 4 的倍数字节缓冲
//       (反编译原样: (4 - (len*2+1)%4) + (len*2+1)), 逐字节写两个小写 hex 字符,
//       失败则打 "Failed to calloc in util.c bin2hex():920" 并 quit(1); 返回缓冲指针。
char *bin2hex(const uint8_t *data, int len)
{
    const uint8_t *p;
    char           hi;
    char          *s;
    char          *cur;
    char           logbuf[4096];
    int            i;

    i = len * 2 + 1;
    s = calloc((4 - i % 4) + i, 1);
    if (s == NULL) {
        snprintf(logbuf, 0x1000, "Failed to calloc in %s %s():%d", "util.c", "bin2hex", 0x398);
        applog(3, logbuf, 1);
        quit(1);
    }
    cur = s;
    if (0 < len) {
        i = 0;
        do {
            p = data + i;
            i++;
            hi = "0123456789abcdef"[p[0] >> 4];
            cur[1] = "0123456789abcdef"[p[0] & 0xf];
            cur[0] = hi;
            cur += 2;
        } while (i != len);
        cur = s + len * 2;      /* 与循环结束时的 cur 等值, 反编译保留的显式重算 */
    }
    cur[0] = '\0';
    return s;
}

// === hex2bin === 原 FUN_00029830 @ 0x00029830
// 来源: reversed/cgminer/50_crypto_hash/FUN_00029830.c | 分类: 50_crypto_hash
// 语义: hex 字符串转二进制 (util.c): 每 2 个字符查 DAT_00064e8c 表拼 1 字节
//       (src[0] 为高半字节), 最多写 dst_len 字节。非法字符 (表项 -1) 或输入为奇数
//       长度 (含单字符尾) 或输出区填满仍有剩余输入 → 失败返回 0; 输出区恰好填满
//       且输入恰好消费完 → 返回 1。空输入仅在 dst_len==0 时成功。
//       机器码逐字节核对: 表项 '0'-'9'/'a'-'f'/'A'-'F' = 0..15, 其余 = 0xFFFFFFFF。
//       非法字符日志 "hex2bin scan failed"、奇数尾日志 "hex2bin str truncated"
//       (原串为栈上拼字, 含尾字节 100='d'), 仅日志门控 (A||B||C>2) 开启时打印。
int hex2bin(uint8_t *dst, const uint8_t *src, uint32_t dst_len)
{
    int      ret;
    uint32_t c_hi;    /* 当前对的高半字节字符 (= src[1]) */
    uint32_t c_lo;    /* 当前对的低半字节字符 (= src[0]) */
    char     logbuf[4096];

    if (*src == '\0') {
        goto exact_consume_check;                       /* 原 LAB_000298bc */
    }
    if (dst_len != 0) {
        c_hi = (uint32_t)src[1];
        if (c_hi == 0) {
            goto log_truncated;                         /* 单字符尾: 奇数长度 */
        }
        c_lo = (uint32_t)src[0];
        do {
            if ((int)(DAT_00064e8c[c_hi] | DAT_00064e8c[c_lo]) < 0) {
                /* 任一表项为 0xFFFFFFFF → 含非法字符 */
                if (LOG_GATE3()) {
                    snprintf(logbuf, 0x1000, "hex2bin scan failed");
                    applog(3, logbuf, 0);
                    return 0;
                }
                goto fail_return;                       /* 原 LAB_00029924 */
            }
            dst_len--;
            *dst = (uint8_t)DAT_00064e8c[c_hi] | (uint8_t)((DAT_00064e8c[c_lo] & 0xff) << 4);
            c_hi = (uint32_t)src[2];                    /* 下一对高半字节字符 (原码同步算其表偏移 <<2) */
            if (c_hi == 0) {
                goto exact_consume_check;               /* 输入恰好消费完 */
            }
            if (dst_len == 0) {
                goto fail_return;                       /* 还有输入但输出区已满 (无日志) */
            }
            c_hi = (uint32_t)src[3];
            c_lo = (uint32_t)src[2];                    /* 新一对低半字节字符 */
            dst++;
            src += 2;
        } while (c_hi != 0);                            /* src[3]==0: 又剩单字符尾 */
log_truncated:
        if (LOG_GATE3()) {
            snprintf(logbuf, 0x1000, "hex2bin str truncated");
            applog(3, logbuf, 0);
        }
        goto fail_return;
    }
    /* *src 非空但 dst_len==0: 直接失败 (无日志) */
    goto fail_return;

exact_consume_check:
    /* 成功条件: 剩余输出槽数 == 0 (反编译: ret = 1 - dst_len; dst_len > 1 时 ret = 0) */
    ret = 1 - (int)dst_len;
    if (1 < dst_len) {
        ret = 0;
    }
    return ret;

fail_return:
    return 0;
}

// === rwlock_init_zcash === 原 FUN_0003af0c @ 0x0003af0c
// 来源: reversed/cgminer/50_crypto_hash/FUN_0003af0c.c | 分类: 50_crypto_hash
// 语义: driver-btm-zcash.c 读写锁初始化: pthread_rwlock_init(锁, 默认属性) 失败时拼
//       "Failed to pthread_rwlock_init errno=%d in driver-btm-zcash.c
//       bitmain_ZCASH_prepare():4683" 日志 (固定行号 0x1D4B) 后 quit(1)。
void rwlock_init_zcash(pthread_rwlock_t *lock)
{
    int  rc;
    int *errno_loc;
    char logbuf[4100];

    rc = pthread_rwlock_init(lock, (pthread_rwlockattr_t *)0x0);
    if (rc != 0) {
        errno_loc = __errno_location();
        snprintf(logbuf, 0x1000, "Failed to pthread_rwlock_init errno=%d in %s %s():%d", *errno_loc,
                 "driver-btm-zcash.c", "bitmain_ZCASH_prepare", 0x1d4b);
        applog(3, logbuf, 1);
        quit(1);
    }
    return;
}

// === opt_usage === 原 FUN_0005c20c @ 0x0005c20c
// 来源: reversed/cgminer/50_crypto_hash/FUN_0005c20c.c | 分类: 50_crypto_hash
// 语义: ccan/opt 用法串生成 (usage.c): "Usage: <argv0> [-<短选项>] <extra>\n" +
//       逐选项说明行 (子表打 "<描述>:\n" 标题; 普通选项 = 名字 [+ " <arg>" 当带参且
//       名字无空格/'='] + 空格填充对齐 + 描述 [+ " (default: %s)" 当有 show 回调] + \n)。
//       隐藏选项 (描述 == opt_hidden=DAT_0009d784 哨兵) 跳过。extra==NULL 时在选项表
//       中找回调 == opt_usage_and_exit 且 carg 非空的表项, 取其 carg 作 extra (找不到用 "")。
//       计数预算 (反编译常量): 头部 = opt_num_short + 20 + strlen(argv0) + strlen(extra)
//       (20 = "Usage: %s "(10)+"[-%.*s]"(7)+" "(1)+"\n"(1)); 每普通选项 = strlen(名字)
//       +6(" <arg>")+20(OPT_SPACE_PAD)+1+strlen(描述)+1, 有 show 再 +0x76=97
//       (= len("(default: %s)")13 + OPT_SHOW_LEN 80 + sizeof("...") 4);
//       每子表 = strlen(描述)+3。Ghidra 丢失 sprintf("%s:\n")/"%.*s" 可变实参, 已按
//       计数预算 + rodata 字符串 + 上游 ccan/opt usage.c 恢复 (见文件头说明)。
char *opt_usage(const char *argv0, const char *extra)
{
    size_t         len;
    size_t         name_len;
    size_t         desc_len;
    char          *ret;
    char          *p;
    const uint32_t *opt;          /* 当前表项 (u32 单位) */
    char          *desc;
    unsigned int   i;
    int            idx;           /* first_sopt/next_sopt 短选项迭代表项号 */
    char          *sopt;
    int            n_short;       /* 已写入短选项数 */
    int            n;
    int            n2;
    int            n3;
    char           showbuf[OPT_SHOW_LEN + 4];   /* +80 处写 "..." (0x2E2E2E) */

    if (extra == (const char *)0x0) {
        /* 在选项表中查找 opt_usage_and_exit 表项的 carg 作为 extra */
        extra = "";
        for (i = 0; i < (unsigned int)DAT_0009d774; i++) {
            opt = DAT_0009d77c + i * OPT_TABLE_STRIDE;
            if ((uint32_t *)opt[OPT_OFF_NOARG_CB] == (uint32_t *)opt_usage_and_exit &&
                (char *)opt[OPT_OFF_VALUE] != (char *)0x0) {
                extra = (const char *)opt[OPT_OFF_VALUE];
                break;
            }
        }
        len = (size_t)DAT_0009d770 + 0x14 + strlen(argv0) + strlen(extra);
    } else {
        len = (size_t)DAT_0009d770 + 0x14 + strlen(argv0) + strlen(extra);
        if (DAT_0009d774 == 0) {
            goto alloc_phase;                       /* 原 LAB_0005C2E8 */
        }
    }
    /* 逐表项累加预算 (反编译: 连续子表经 while 合并, 语义 = 每项二选一) */
    for (i = 0; i < (unsigned int)DAT_0009d774; i++) {
        opt = DAT_0009d77c + i * OPT_TABLE_STRIDE;
        if (opt[OPT_OFF_TYPE] == 8) {               /* OPT_SUBTABLE: 标题行 "<desc>:\n" */
            len += 3 + strlen((const char *)opt[OPT_OFF_DESC]);
        } else if ((char *)opt[OPT_OFF_DESC] != DAT_0009d784) {   /* 非隐藏选项 */
            name_len = strlen((const char *)opt[OPT_OFF_NAME]);
            desc_len = strlen((const char *)opt[OPT_OFF_DESC]);
            len += 6 + name_len + desc_len;         /* 6 = len(" <arg>") */
            len += 0x15 + 1;                        /* 0x15 = 20(OPT_SPACE_PAD)+1, 再 +1 = 换行 */
            if (opt[OPT_OFF_SHOW] != 0) {
                len += 0x76;                        /* 97 = len("(default: %s)")+OPT_SHOW_LEN+sizeof("...") */
            }
        }
    }

alloc_phase:
    ret = malloc(len);
    if (ret != (char *)0x0) {
        n = sprintf(ret, "Usage: %s", argv0);
        p = ret + n;
        strcpy(p, " [-");                           /* 原码 strncpy 内联 (4 字节): 3 字符 + '\0' */
        sopt = first_sopt(&idx);
        n_short = 0;
        if (sopt != (char *)0x0) {
            do {
                if (DAT_0009d77c[idx * OPT_TABLE_STRIDE + OPT_OFF_DESC] != (uint32_t)DAT_0009d784) {
                    p[3 + n_short] = *sopt;         /* 短选项字符紧跟 "-" (覆盖 " [-" 的 '\0') */
                    n_short++;
                }
                sopt = next_sopt(sopt, &idx);
            } while (sopt != (char *)0x0);
            if (n_short != 0) {
                p[3 + n_short] = ']';
                p[4 + n_short] = '\0';
                p = p + n_short + 4;
            }
            /* n_short == 0 (无可见短选项): p 保持指向 " [-" 处,
             * 等价上游 write_short_options 返回 0 时的 p -= 3 (p 原在 " [-" 尾) */
        }
        n = sprintf(p, " %s", extra);
        p[n] = '\n';
        p[n + 1] = '\0';
        p = p + n + 1;
        if (DAT_0009d774 != 0) {
            for (i = 0, n = 0; i < (unsigned int)DAT_0009d774; i++, n += OPT_TABLE_STRIDE * 4) {
                opt = (uint32_t *)((int)DAT_0009d77c + n);
                if ((char *)opt[OPT_OFF_DESC] == DAT_0009d784) {
                    continue;                       /* 隐藏选项不输出 */
                }
                if (opt[OPT_OFF_TYPE] == 8) {
                    /* 子表标题行 (Ghidra 丢失实参; 计数预算 3+strlen(描述) 证实打描述串) */
                    n2 = sprintf(p, "%s:\n", (const char *)opt[OPT_OFF_DESC]);
                    p = p + n2;
                } else {
                    n2 = sprintf(p, "%s", (const char *)opt[OPT_OFF_NAME]);
                    if (opt[OPT_OFF_TYPE] == 2) {   /* OPT_HASARG */
                        desc = (char *)opt[OPT_OFF_NAME];
                        if (strchr(desc, 0x20) == (char *)0x0 &&
                            (strchr(desc, 0x3d) == (char *)0x0)) {
                            /* 名字中无 ' ' 与 '=' → 追加 " <arg>" (原码 strncpy 内联, 7 字节含 '\0') */
                            strcpy(p + n2, " <arg>");
                            n2 += 6;
                        }
                    }
                    /* 名字列→描述列空格填充 (Ghidra 丢失实参; 精度 len<20 ? 20-len : 1) */
                    n3 = sprintf(p + n2, "%.*s",
                                 n2 < (int)strlen(OPT_SPACE_PAD)
                                 ? (unsigned)strlen(OPT_SPACE_PAD) - (unsigned)n2 : 1u,
                                 OPT_SPACE_PAD);
                    n2 = n3 + n2;
                    n2 = n2 + sprintf(p + n2, "%s", (const char *)opt[OPT_OFF_DESC]);
                    if (opt[OPT_OFF_SHOW] != 0) {
                        strcpy(showbuf + OPT_SHOW_LEN, "...");   /* showbuf 尾部常量 "..." */
                        ((void (*)(char *, void *))opt[OPT_OFF_SHOW])(showbuf,
                                                                      (void *)opt[OPT_OFF_VALUE]);
                        n3 = sprintf(p + n2, " (default: %s)", showbuf);
                        n2 = n2 + n3;
                    }
                    p[n2] = '\n';
                    p[n2 + 1] = '\0';
                    p = p + n2 + 1;
                }
            }
        }
        p[0] = '\0';
    }
    return ret;
}

// === blake2b_init_state === 原 FUN_00054dcc @ 0x00054dcc
// 来源: reversed/cgminer/50_crypto_hash/FUN_00054dcc.c | 分类: 50_crypto_hash
// 语义: BLAKE2b 带参状态初始化 (Equihash 用, driver-btm-zcash): 状态区清零 0x168 字节,
//       h[0..7] 装载 BLAKE2b IV (rodata 0x6C540..0x6C57F, 反编译复制循环终止于 0x6C578,
//       共 8×8 字节, ELF 核对与标准 IV 一致) 后逐字异或 64 字节参数块的小端装载字,
//       即标准 blake2b_init_param 的 h[i] = IV[i] ^ P[i]。恒返回 0。
//       注: 参数块 64 字节按 8 次 VectorShiftLeft 拼装 (Ghidra 向量移位伪码), 等价于
//       b0 | b1<<8 | ... | b7<<56 的小端装载, 此处保留逐字节移位形式。
uint32_t blake2b_init_state(uint64_t *state, const uint8_t *block)
{
    const uint8_t *end;
    uint64_t       w1;
    uint64_t       w2;
    uint64_t       w3;
    uint64_t       w4;
    uint64_t       w5;
    uint64_t       w6;
    uint64_t       w7;
    uint8_t        b0;
    int            i;

    memset(state, 0, BLAKE2B_STATE_SIZE);
    /* IV 装载: 反编译循环源指针自 "hPoW"("ZcashPoW" 串内, 0x6C538) 起、每轮取 +8,
     * 即依次复制 BLAKE2B_IV[0..7]; 两段 4 字节半字写入合成 64 位字。 */
    for (i = 0; i < 8; i++) {
        state[i] = BLAKE2B_IV[i];
    }
    /* 参数块异相: 逐字小端装载后 ^ h[i], 共 64 字节 (8 轮) */
    end = block + BLAKE2B_BLOCK_SIZE;
    do {
        b0 = block[0];
        w1 = (uint64_t)block[1] << 8;
        w2 = (uint64_t)block[2] << 0x10;
        w3 = (uint64_t)block[3] << 0x18;
        w4 = (uint64_t)block[4] << 0x20;
        w5 = (uint64_t)block[5] << 0x28;
        w6 = (uint64_t)block[6] << 0x30;
        w7 = (uint64_t)block[7] << 0x38;
        block += 8;
        state[0] ^= (uint64_t)b0 | w1 | w2 | w3 | w4 | w5 | w6 | w7;
        state++;
    } while (block != end);
    return 0;
}

// === equihash_solver === 原 FUN_0004eee0 @ 0x0004eee0
// 来源: reversed/cgminer/50_crypto_hash/FUN_0004eee0.c | 分类: 50_crypto_hash
// 语义: Equihash BLAKE2b 初始化入口 (driver-btm-zcash/zcash 风格, 命名阶段锚点):
//       构造 64 字节 BLAKE2b 参数块 — digest_length = (N*(512/N))/8 (字节 0, N=200 → 50,
//       与 zcash Equihash 的 blake2b_hashlen=(512/n)*n/8 口径一致), key_length=0 (清零),
//       fanout=1 (字节 2), depth=1 (字节 3), personalization="ZcashPoW" (字节 48..55),
//       保留区字节 56..63 写 N/K (标准 BLAKE2b 该区保留, 按反编译原样保留) —
//       随后调 blake2b_init_state(state, block)。
//       除法: 原码调 __aeabi_idivmod(0x200, N) (FUN_0005c710), 取 r0 低 32 位商 = 512/N;
//       其后有符号 >>3 前的 +7 补偿 (N<0 时向零取整, 正常 N>0 不触发)。
void equihash_solver(uint64_t *state, int N, int K)
{
    uint8_t  block[BLAKE2B_BLOCK_SIZE];
    int      digest_len;
    int      q;

    memset(block, 0, BLAKE2B_BLOCK_SIZE);
    block[2] = 1;                                /* fanout */
    block[3] = 1;                                /* depth (block[1] key_length = 0) */
    q = 512 / N;                                 /* 原 __aeabi_idivmod(0x200, N) 的商 (r0) */
    digest_len = N * q;
    memcpy(block + 48, "ZcashPoW", 8);           /* personalization (原码 strncpy 内联 n=8) */
    if (digest_len < 0) {
        digest_len = digest_len + 7;             /* 有符号除 8 向零取整补偿 */
    }
    block[0] = (uint8_t)(digest_len >> 3);       /* digest_length = N*(512/N)/8 */
    memcpy(block + 56, &N, 4);                   /* 参数块保留区写 N (原码 4 字节存) */
    memcpy(block + 60, &K, 4);                   /* 参数块保留区写 K */
    blake2b_init_state(state, block);
    return;
}

// === add_cgpu === 原 FUN_00028008 @ 0x00028008
// 来源: reversed/cgminer/50_crypto_hash/FUN_00028008.c | 分类: 50_crypto_hash
// 语义: 设备注册 (cgminer.c add_cgpu): 维护全局"驱动名计数哈希表" (根表项
//       DAT_00078728), 以驱动名 (cgpu->drv(+4) 的 +8 名字串) 为键统计同名设备实例数,
//       实例序号写入 cgpu+0x08 (用于同名设备编号命名); 随后在 devices_lock
//       (DAT_0007b700) 保护下 realloc devices 数组 (DAT_0007b628, 容量
//       (常规计数+追加计数+2)*4), 在 hash_lock (DAT_0007ad4c) 保护下写
//       cgpu+0xE8 = time(NULL) (上游同位 last_device_valid_work), 按追加模式标志
//       把设备指针写入 devices 数组并推进计数, 维护最大设备数高水位 DAT_0007702c。
//       每次锁操作失败走对应错误处理; 锁区间后调全局函数指针 DAT_00075ed8 (初值
//       noop_hook)。恒返回 1。
//       哈希 = Bob Jenkins lookup2 风格 (a=b=0x9e3779b9, c=0xfeedbeef, 长度在块循环后
//       并入 c, 见文件头说明); 表项 0x28 字节兼作内联名字符串 (仅内联名字前 4 字节 +
//       尾零, 故驱动名须 ≤4 字符 — 与 cgminer 3 字符驱动名吻合); 查找用全名哈希,
//       落表用内联名哈希 (名 ≤4 字符时两者输入一致)。表满 (槽链计数 ≥ (槽深度比+1)*10
//       且未标记退化) 时槽位数翻倍并以缓存哈希重散。
//
/* 表项布局 (0x28 字节, 字号 = 字节偏移/4):
 *  +0x00 名字前 4 字节 (内联字符串)   +0x04 同名计数        +0x08 哈希控制块指针 (tag 基准字段)
 *  +0x0C 全序链 prev (项基址)         +0x10 全序链 next (项基址)
 *  +0x14 槽链 prev (tag: 基址+8)      +0x18 槽链 next (tag) +0x1C 关键字自指 (内联名)
 *  +0x20 名字长度 (size_t)            +0x24 缓存哈希
 * 控制块布局 (0x2C = 11 字):
 *  +0x00 槽位数组指针 (每槽 12 字节: [0]头(tag: 项基址+8) [1]链长计数 [2]深度比)
 *  +0x04 槽位数(0x20)  +0x08 bits(5)  +0x0C 元素总数  +0x10 全序链尾链接地址(&尾项+8)
 *  +0x14 tag 偏移(8)   +0x18 期望槽深  +0x1C 超深槽计数 +0x20 重建计数 +0x24 退化标志
 *  +0x28 常量种子 0xA0111FE1 (语义待定) */
uint32_t add_cgpu(int cgpu)
{
    uint32_t      *entry;       /* 当前表项 (0x28 字节) */
    uint32_t      *ht;          /* 哈希控制块 (0x2C 字节) */
    const char    *drv_name;    /* 驱动名字符串 */
    const char    *p;
    size_t         name_len;
    size_t         remaining;
    uint32_t       a;
    uint32_t       b;
    uint32_t       c;
    uint32_t       hash;
    uint32_t       idx;         /* 槽号 */
    uint32_t      *slot;
    uint32_t      *old_slots;
    uint32_t      *new_slots;
    uint32_t       new_bytes;
    uint32_t       slot_i;
    uintptr_t      cur;         /* 槽链游标 (tag: 项基址+8) */
    uintptr_t      next;
    uint32_t       count;
    uint32_t       dev_idx;
    int            rc;
    time_t         now;

    drv_name = *(const char * const *)(*(int *)(cgpu + 4) + 8);   /* cgpu+4=drv, drv+8=名字串 */
    entry = (uint32_t *)DAT_00078728;
    if (entry == (uint32_t *)0x0) {
        /* ---- 首次调用: 建立根表项 + 哈希控制块 + 槽位数组 ---- */
        entry = malloc(0x28);
        entry[1] = 0;                                  /* +0x04 同名计数 */
        *(uint32_t *)entry = *(const uint32_t *)drv_name;  /* 名字前 4 字节内联 */
        *(int *)(cgpu + 8) = 0;                        /* 首个同名实例序号 */
        entry[4] = 0;                                  /* +0x10 全序链 next */
        entry[7] = (uint32_t)entry;                    /* +0x1C 关键字自指 */
        name_len = strlen((const char *)entry);
        entry[3] = 0;                                  /* +0x0C 全序链 prev */
        DAT_00078728 = (char *)entry;
        entry[8] = (uint32_t)name_len;                 /* +0x20 名字长度 */
        ht = malloc(0x2c);
        entry[2] = (uint32_t)ht;                       /* +0x08 控制块指针 */
        if (ht == (uint32_t *)0x0) {
            goto oom_exit;                             /* 原 LAB_000289D4 */
        }
        ht[0] = 0;
        ht[3] = 0;
        ht[4] = 0; ht[5] = 0; ht[6] = 0; ht[7] = 0; ht[8] = 0; ht[9] = 0; ht[10] = 0;
        ht[4] = (uint32_t)(entry + 2);                 /* 全序链尾链接地址 = &根项+8 */
        ht[1] = 0x20;                                  /* 槽位数 32 */
        ht[2] = 5;                                     /* bits */
        ht[5] = 8;                                     /* tag 偏移 */
        new_slots = malloc(0x180);                     /* 槽位数组 32×12 字节 */
        ht[0] = (uint32_t)new_slots;
        if (new_slots == (uint32_t *)0x0) {
            goto oom_exit;                             /* 原 LAB_000289D4 */
        }
        memset(new_slots, 0, 0x180);
        ht[10] = 0xa0111fe1;                           /* 常量种子 (语义待定) */
    } else {
        /* ---- 哈希查找同名驱动 ---- */
        name_len = strlen(drv_name);
        a = 0x9e3779b9;
        b = 0x9e3779b9;
        c = 0xfeedbeef;
        remaining = name_len;
        p = drv_name;
        while (0xb < remaining) {
            a += LE32_LOAD(p);
            b += LE32_LOAD(p + 4);
            c += LE32_LOAD(p + 8);
            LOOKUP2_MIX(a, b, c);
            p += 12;
            remaining -= 12;
        }
        c += name_len;                                 /* 原码: 长度在块循环后并入 c */
        switch (remaining) {
        case 11: c += (uint32_t)(uint8_t)p[10] << 24;  /* fall through */
        case 10: c += (uint32_t)(uint8_t)p[9] << 16;   /* fall through */
        case 9:  c += (uint32_t)(uint8_t)p[8] << 8;    /* fall through */
        case 8:  b += (uint32_t)(uint8_t)p[7] << 24;   /* fall through */
        case 7:  b += (uint32_t)(uint8_t)p[6] << 16;   /* fall through */
        case 6:  b += (uint32_t)(uint8_t)p[5] << 8;    /* fall through */
        case 5:  b += (uint32_t)(uint8_t)p[4];         /* fall through */
        case 4:  a += (uint32_t)(uint8_t)p[3] << 24;   /* fall through */
        case 3:  a += (uint32_t)(uint8_t)p[2] << 16;   /* fall through */
        case 2:  a += (uint32_t)(uint8_t)p[1] << 8;    /* fall through */
        case 1:  a += (uint32_t)(uint8_t)p[0];
        default: break;
        }
        LOOKUP2_MIX(a, b, c);
        hash = c;                                      /* 终值 = c */
        ht = *(uint32_t **)(DAT_00078728 + 8);
        idx = hash & (ht[1] - 1U);
        slot = (uint32_t *)((uint8_t *)(uintptr_t)ht[0] + idx * 0xc);
        cur = (uintptr_t)slot[0];
        if (cur != 0) {
            cur -= ht[5];                              /* 解 tag: 槽头存项基址+8 */
        }
        for (; cur != 0; cur = *(uint32_t *)(cur + 0x18) - (uintptr_t)ht[5]) {
            if (name_len == *(size_t *)(cur + 0x20) &&
                memcmp((const void *)(cur + 0x1c), drv_name, name_len) == 0) {
                /* 命中同名表项: 计数++ 并写回 cgpu 实例序号 */
                count = *(uint32_t *)(cur + 4) + 1;
                *(uint32_t *)(cur + 4) = count;
                *(int *)(cgpu + 8) = (int)count;
                goto list_append;                      /* 原 LAB_000284A8 */
            }
            if (*(uint32_t *)(cur + 0x18) == 0) {
                break;                                 /* 槽链结束, 未命中 */
            }
        }
        /* ---- 未命中: 新建表项并挂入全序链尾 ---- */
        entry = malloc(0x28);
        entry[1] = 0;                                  /* +0x04 同名计数 */
        *(uint32_t *)entry = *(const uint32_t *)drv_name;
        *(int *)(cgpu + 8) = 0;
        entry[4] = 0;                                  /* +0x10 全序链 next */
        entry[7] = (uint32_t)entry;                    /* +0x1C 关键字自指 */
        name_len = strlen((const char *)entry);
        entry[8] = (uint32_t)name_len;                 /* +0x20 名字长度 */
        ht = *(uint32_t **)(DAT_00078728 + 8);
        *(uint32_t *)(ht[4] + 8) = (uint32_t)entry;    /* 前尾项 +0x10 (next) = 新项基址 */
        entry[3] = ht[4] - ht[5];                      /* +0x0C prev = 前尾项基址 (tag 偏移还原) */
        ht[4] = (uint32_t)(entry + 2);                 /* 尾链接地址推进 = &新项+8 */
    }
    /* ---- 公共尾部: 元素计数 + 缓存哈希 (内联名) + 槽链插入 + 按需翻倍重建 ---- */
    ht[3]++;                                           /* 元素总数++ */
    a = 0x9e3779b9;
    b = 0x9e3779b9;
    c = 0xfeedbeef;
    entry[9] = 0xfeedbeef;                             /* +0x24 缓存哈希初值 (LE: ef be ed fe) */
    entry[2] = (uint32_t)ht;                           /* +0x08 = 控制块指针 (tag 基准字段) */
    name_len = strlen((const char *)entry);            /* 内联名长度 (≤4, 与全名一致) */
    remaining = name_len;
    p = (const char *)entry;
    while (0xb < remaining) {
        a += LE32_LOAD(p);
        b += LE32_LOAD(p + 4);
        c += LE32_LOAD(p + 8);
        LOOKUP2_MIX(a, b, c);
        entry[9] = c;                                  /* 原码每块回写缓存哈希 */
        p += 12;
        remaining -= 12;
    }
    c += name_len;
    entry[9] = c;
    switch (remaining) {
    case 11: c += (uint32_t)(uint8_t)p[10] << 24; entry[9] = c;  /* fall through */
    case 10: c += (uint32_t)(uint8_t)p[9] << 16;  entry[9] = c;  /* fall through */
    case 9:  c += (uint32_t)(uint8_t)p[8] << 8;   entry[9] = c;  /* fall through */
    case 8:  b += (uint32_t)(uint8_t)p[7] << 24;                /* fall through */
    case 7:  b += (uint32_t)(uint8_t)p[6] << 16;                /* fall through */
    case 6:  b += (uint32_t)(uint8_t)p[5] << 8;                 /* fall through */
    case 5:  b += (uint32_t)(uint8_t)p[4];                      /* fall through */
    case 4:  a += (uint32_t)(uint8_t)p[3] << 24;                /* fall through */
    case 3:  a += (uint32_t)(uint8_t)p[2] << 16;                /* fall through */
    case 2:  a += (uint32_t)(uint8_t)p[1] << 8;                 /* fall through */
    case 1:  a += (uint32_t)(uint8_t)p[0];
    default: break;
    }
    LOOKUP2_MIX(a, b, c);
    hash = c;
    entry[9] = hash;                                   /* +0x24 最终缓存哈希 */
    /* 槽链头插 (头指针带 +8 tag) */
    idx = hash & (ht[1] - 1U);
    slot = (uint32_t *)((uint8_t *)(uintptr_t)ht[0] + idx * 0xc);
    slot[1]++;                                         /* 槽链长计数++ */
    cur = (uintptr_t)slot[0];                          /* 旧头 (tag) */
    entry[5] = 0;                                      /* +0x14 槽链 prev 清零 */
    entry[6] = (uint32_t)cur;                          /* +0x18 槽链 next = 旧头 */
    if (cur != 0) {
        *(uint32_t *)(cur + 0xc) = (uint32_t)(entry + 2);   /* 旧头 (tag+0xC → 基址+0x14) prev = 新项+8 */
    }
    slot[0] = (uint32_t)(entry + 2);                   /* 新头入槽 */
    /* 槽链过深 (槽链计数 ≥ (槽深度比+1)*10) 且未标记退化 → 槽位数翻倍重建 */
    if ((uint32_t)((*(int *)((uint8_t *)(uintptr_t)ht[0] + idx * 0xc + 8) + 1) * 10) <=
        *(uint32_t *)((uint8_t *)(uintptr_t)ht[0] + idx * 0xc + 4) && ht[9] != 1) {
        new_bytes = ht[1] * 0x18;                      /* 新表字节数 = 槽位数×2 ×12 */
        new_slots = malloc(new_bytes);
        if (new_slots == (uint32_t *)0x0) {
            goto oom_exit;                             /* 原 LAB_000289D4 */
        }
        memset(new_slots, 0, new_bytes);
        /* 期望槽深 (反编译: ((槽位数*2-1)&元素数 != 0) + 元素数>>(bits+1)) */
        ht[6] = (uint32_t)(((ht[1] * 2 - 1U) & ht[3]) != 0) + (ht[3] >> (ht[2] + 1));
        ht[7] = 0;                                     /* 超深槽计数清零 */
        old_slots = (uint32_t *)(uintptr_t)ht[0];
        for (slot_i = 0; slot_i < (uint32_t)ht[1]; slot_i++) {
            cur = (uintptr_t)old_slots[slot_i * 3];    /* 旧槽头 (tag) */
            while (cur != 0) {
                next = *(uint32_t *)(cur + 0x10);      /* 槽链 next (tag+0x10 → 基址+0x18) */
                idx = (ht[1] * 2 - 1U) & *(uint32_t *)(cur + 0x1c);   /* 新槽号 = 新掩码 & 缓存哈希 (基址+0x24) */
                count = *(uint32_t *)((uint8_t *)new_slots + idx * 0xc + 4) + 1;
                *(uint32_t *)((uint8_t *)new_slots + idx * 0xc + 4) = count;   /* 新槽链长计数++ */
                if ((uint32_t)ht[6] < count) {
                    ht[7]++;                           /* 超深槽计数++ */
                    *(uint32_t *)((uint8_t *)new_slots + idx * 0xc + 8) =
                        count / ht[6];                 /* 深度比 = 计数/期望槽深 (原 __udivsi3 FUN_0005C648) */
                }
                *(uint32_t *)(cur + 0xc) = 0;          /* 槽链 prev 清零 (tag+0xC → 基址+0x14) */
                *(uint32_t *)(cur + 0x10) =
                    *(uint32_t *)((uint8_t *)new_slots + idx * 0xc);   /* next = 新槽旧头 */
                if (*(uint32_t *)((uint8_t *)new_slots + idx * 0xc) != 0) {
                    *(uint32_t *)(*(uint32_t *)((uint8_t *)new_slots + idx * 0xc) + 0xc) =
                        (uint32_t)cur;                 /* 新槽旧头 prev = 当前项 (tag) */
                }
                *(uint32_t *)((uint8_t *)new_slots + idx * 0xc) = (uint32_t)cur;   /* 新槽头 */
                if (next == 0) {
                    break;
                }
                cur = next;
            }
        }
        free(old_slots);
        ht[1] = ht[1] << 1;                            /* 槽位数 ×2 */
        ht[2] = ht[2] + 1;                             /* bits++ */
        ht[0] = (uint32_t)new_slots;
        if (ht[3] >> 1 < ht[7]) {                      /* 超深槽 > 元素数一半 → 重建计数++, 否则清零 */
            ht[8]++;
        } else {
            ht[8] = 0;
        }
        if (1 < ht[8]) {
            ht[9] = 1;                                 /* 标记退化: 不再自动重建 */
        }
    }

list_append:                                           /* 原 LAB_000284A8 */
    rc = pthread_rwlock_wrlock((pthread_rwlock_t *)&DAT_0007b700);
    if (rc != 0) {
        wr_lock_error("add_cgpu", 0x28cc);
    }
    DAT_0007b628 = realloc(DAT_0007b628, (uint32_t)(DAT_0007b720 + DAT_0007872c + 2) * 4);
    rc = pthread_rwlock_unlock((pthread_rwlock_t *)&DAT_0007b700);
    if (rc != 0) {
        rw_unlock_error("add_cgpu", 0x28ce);
    }
    (*DAT_00075ed8)();
    rc = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007ad4c);
    if (rc != 0) {
        mutex_lock_fail_cgminer("add_cgpu", 0x28d0);
    }
    now = time((time_t *)0x0);
    *(time_t *)(cgpu + 0xe8) = now;                    /* 上游同位 cgpu->last_device_valid_work */
    rc = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007ad4c);
    if (rc != 0) {
        mutex_unlock_fail_cgminer("add_cgpu", 0x28d2);
    }
    (*DAT_00075ed8)();
    if (DAT_0007b7d0 == '\0') {
        ((int *)DAT_0007b628)[DAT_0007b720] = cgpu;    /* 常规段追加 */
        DAT_0007b720++;
    } else {
        dev_idx = (uint32_t)(DAT_0007872c + DAT_0007b720);
        DAT_0007872c++;
        ((int *)DAT_0007b628)[dev_idx] = cgpu;         /* 追加段写入 (段计数先增) */
    }
    if (DAT_0007702c < DAT_0007b720 - DAT_0007b8b0) {
        DAT_0007702c = DAT_0007b720 - DAT_0007b8b0;    /* 最大设备数高水位 */
    }
    return 1;

oom_exit:                                              /* 原 LAB_000289D4 */
    exit(-1);
}

// === bitmain_zcash_detect === 原 FUN_0003a940 @ 0x0003a940
// 来源: reversed/cgminer/50_crypto_hash/FUN_0003a940.c | 分类: 50_crypto_hash
// 语义: driver-btm-zcash.c 设备检测: calloc 0x188 字节 cgpu_info (失败触发断言
//       "cgpu" @ driver-btm-zcash.c:4680), 置 +0x94 标志=1、+0x04 = bitmain_ZCASH
//       驱动描述结构 (DAT_00076ac8)、+0x20 显式清零 (calloc 已清零, 反编译保留),
//       再 calloc 0x4B4 字节设备私有数据区挂到 +0x14 (失败打 "Failed to calloc
//       cgpu_info data" 并 quit(1)), 调 add_cgpu 注册 (返回 0 触发断言
//       "add_cgpu(cgpu)" @ 行 4672); opt_debug 且日志门控 (A||B||C>6) 开启时打
//       "bitmain_ZCASH_detect detect new device" (LOG_DEBUG=7)。
void bitmain_zcash_detect(void)
{
    void *cgpu;
    void *info;
    char  logbuf[4096];

    cgpu = calloc(1, 0x188);
    if (cgpu == (void *)0x0) {
        __assert_fail("cgpu", "driver-btm-zcash.c", 0x1d38, "bitmain_ZCASH_detect");
    }
    *(uint32_t *)((int)cgpu + 0x94) = 1;               /* 标志位 (语义待定) */
    *(char **)((int)cgpu + 4) = DAT_00076ac8;          /* cgpu->drv = 驱动描述结构 */
    *(uint32_t *)((int)cgpu + 0x20) = 0;               /* 反编译保留的显式清零 */
    info = calloc(0x4b4, 1);
    *(void **)((int)cgpu + 0x14) = info;               /* cgpu 设备私有数据区 */
    if (info == (void *)0x0) {
        snprintf(logbuf, 0x1000, "Failed to calloc cgpu_info data");   /* 原串为栈上拼字 */
        applog(3, logbuf, 1);
        quit(1);
    }
    if (add_cgpu((int)cgpu) == 0) {
        __assert_fail("add_cgpu(cgpu)", "driver-btm-zcash.c", 0x1d40, "bitmain_ZCASH_detect");
    }
    if (DAT_00078e09 != '\0' && LOG_GATE7()) {
        snprintf(logbuf, 0x1000, "%s detect new device", "bitmain_ZCASH_detect");
        applog(7, logbuf, 0);
    }
    return;
}

// === bitmain_zcash_reinit_chain === 原 FUN_0004791c @ 0x0004791c
// 来源: reversed/cgminer/50_crypto_hash/FUN_0004791c.c | 分类: 50_crypto_hash
// 语义: driver-btm-zcash.c 链重初始化线程体 (自 detach): 在链互斥锁 (DAT_00079134)
//       保护下对参数块指定的链依次执行 — reset_chain(参数字 0, 链号) →
//       software_set_address_chain(链号) → 隔 100ms → set_frequency_chain(链号,
//       每链频率表 DAT_0009d705[链号]) → 隔 100ms → set_core_timeout_chain(链号,
//       80000) (注意: 此处 80000, 非出厂默认 84000/DAT_00076b74) → 隔 100ms →
//       set_ticket_mask_chain(链号, DAT_00076b60) → 隔 100ms →
//       enable_read_temperature_from_asic_chain(链号), 置链已重初始化标志
//       DAT_000790ec[链号]=1, sleep(1) 后解锁。Begin/Done 日志 (LOG_NOTICE=5,
//       门控 A||B||C>4)。链号取参数块 +4 字节。
void bitmain_zcash_reinit_chain(uint32_t *param_1)
{
    pthread_t self;
    uint32_t  chain;
    char      logbuf[4096];

    if (LOG_GATE5()) {
        snprintf(logbuf, 0x1000, "%s Begin\n", "bitmain_ZCASH_reinit_chain");
        applog(5, logbuf, 0);
    }
    self = pthread_self();
    pthread_detach(self);
    pthread_mutex_lock((pthread_mutex_t *)&DAT_00079134);
    chain = *(uint8_t *)((uint8_t *)param_1 + 4);      /* 链号 (参数块 +4 字节) */
    reset_chain((int)*param_1, chain);                 /* 参数字 0 一并传入 */
    software_set_address_chain((int)chain);
    cgsleep_ms(100);
    set_frequency_chain((int)chain, DAT_0009d705[chain]);
    cgsleep_ms(100);
    set_core_timeout_chain((int)chain, 80000);         /* 重初始化用 80000, 非出厂 84000 */
    cgsleep_ms(100);
    set_ticket_mask_chain((int)chain, DAT_00076b60);
    cgsleep_ms(100);
    enable_read_temperature_from_asic_chain(chain);
    DAT_000790ec[chain] = 1;                           /* 链已重初始化标志 */
    sleep(1);
    pthread_mutex_unlock((pthread_mutex_t *)&DAT_00079134);
    if (LOG_GATE5()) {
        snprintf(logbuf, 0x1000, "%s Done\n", "bitmain_ZCASH_reinit_chain");
        applog(5, logbuf, 0);
    }
    return;
}

/* ==================== 覆盖率表 (batch=hash_b1, 12/12) ====================
 * FUN_0000b8b8 @ 0x0000b8b8 → mutex_init_zcash
 * FUN_0000f9c0 @ 0x0000f9c0 → opt_verusage_and_exit
 * FUN_00028008 @ 0x00028008 → add_cgpu
 * FUN_000296d8 @ 0x000296d8 → __bin2hex
 * FUN_00029730 @ 0x00029730 → bin2hex
 * FUN_00029830 @ 0x00029830 → hex2bin
 * FUN_0003a940 @ 0x0003a940 → bitmain_zcash_detect
 * FUN_0003af0c @ 0x0003af0c → rwlock_init_zcash
 * FUN_0004791c @ 0x0004791c → bitmain_zcash_reinit_chain
 * FUN_0004eee0 @ 0x0004eee0 → equihash_solver
 * FUN_00054dcc @ 0x00054dcc → blake2b_init_state
 * FUN_0005c20c @ 0x0005c20c → opt_usage
 */
