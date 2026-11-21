// ============================================================================
// uboot/p1a.c — U-Boot 2016.07 反编译转写 (半批 uboot_b1 前 23 函数)
// 地址范围: 0x04000090 .. 0x04007de8
// 命名事实源: rewritten/name_map/uboot.tsv
// 语义规则: rewritten/CONVENTIONS.md
// ============================================================================

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* ----------------------------------------------------------------------------
 * 模块内部前向声明区
 * -------------------------------------------------------------------------- */

/* 全部 name_map 函数均具外部链接 (跨文件被调), 不使用 static。 */
void ub_sha256_transform(uint32_t *ctx, const uint8_t *block);

uint32_t ub_un_02c050(uint32_t arg_r3, uint32_t arg_st00, uint32_t arg_st04,
                      uint32_t *arg_st08, uint32_t arg_st14);
void ub_un_02c0e8(uint32_t arg_r3, uint32_t arg_st00, uint32_t arg_st04,
                  uint32_t *arg_st08, uint32_t arg_st14);

/* 哈希表回调形态: int (*)(a, NULL, 1, flag) — htab 级 (+0x0c) 与节点级 (节点+0x0c) 同形 */
typedef int32_t (*ub_hash_cb_t)(const void *arg1, const void *arg2,
                                int32_t arg3, uint32_t arg4);

/* ---- 跨文件被调函数 (名称来自 name_map, 原型按调用点推断) ---- */
uint32_t ub_printf(const char *fmt, ...);                    /* FUN_04007548 */
extern const char *ub_strstr(const char *haystack, const char *needle);   /* FUN_04006044 */
extern uint32_t ub_strlen(const char *s);                    /* FUN_04005b98 */
extern int32_t ub_strcmp(const char *a, const char *b);      /* FUN_04005a60 */
extern int32_t ub_regex_match(const void *prog, const char *text,
                              uint32_t text_len, void *caps);            /* FUN_0400583c */
extern int32_t ub_regex_exec(const void *prog, uint32_t pos, uint32_t arg3,
                             uint32_t arg4, uint32_t *out_end, int32_t flag); /* FUN_04005bd4, 无反编译源 */
extern uint32_t ub_vsnprintf(char *buf, uint32_t size, const char *fmt,
                             va_list args);                  /* FUN_040069f0, 无反编译源 */
extern uint32_t ub_crc32_le(uint32_t seed);                  /* FUN_0400461c */
extern void ub_udelay_raw(uint32_t usec);                    /* FUN_04006244 */
extern void ub_vprintf(const char *fmt, va_list args);       /* FUN_040075a0 */
extern uint32_t simple_strtoul(const char *nptr, char **endptr, uint32_t base); /* FUN_04007718 */
extern int32_t net_set_ether(void *pkt, const uint8_t *dest_ethaddr, uint32_t etype); /* FUN_0400a41c */
extern void ub_memcpy(void *dst, const void *src, uint32_t n);            /* FUN_04005ea4 */
extern void eth_send(const void *pkt, int32_t len);          /* FUN_0400929c */
extern uint32_t ub_hsearch(const char *key, const void *data, int32_t action,
                           void **retval, uint32_t *htab, int32_t flag); /* FUN_040034a8 */
extern void ub_hdelete_entry(uint32_t *htab, uint32_t *filled,
                             void *ep, uint32_t idx);        /* FUN_040030a8 */
extern void ub_panic_reset(void);                            /* FUN_040076b8 (noreturn) */
extern uint32_t aeabi_uidiv(uint32_t dividend, uint32_t divisor);        /* FUN_0400c66c */

/* ---- 外部调用目标 (位于 0x40000000 基址之下, 本反编译库未收录, 命名待补) ---- */
/* fun_03ffae84: 02c050 无参形态 / 03001c 4 参形态, 原型不可恢复, 按无原型声明保留 */
extern uint32_t fun_03ffae84();
/* fun_03ffafa8: 2 参形态 (03001c), 返回 <0 表示失败 */
extern int32_t fun_03ffafa8(uint32_t arg1, uint32_t arg2);
/* fun_03fc8dc4: 2 参形态 (04003300 建表分配), 返回分配基址 / 0 */
extern int32_t fun_03fc8dc4(uint32_t cnt, uint32_t size);
/* fun_03fc7fbc: 单参形态 (04007548/040043f0): u-boot puts 形态输出 (不追加换行) */
extern void fun_03fc7fbc(const char *s);
/* fun_03fc7fe4: 无参形态 (040043f0): 非零则中止转储 (行为同 ctrl-c 检测) */
extern int32_t fun_03fc7fe4(void);

/* ---- 全局数据 (语义待定, 保留 Ghidra 原名) ---- */
extern uint32_t DAT_04003060;        /* 全局, 语义待定 (03001c 引用) */
extern const char *DAT_0400320c;     /* 全局: printf 格式串指针 (str_match 兜底打印) */
extern uint32_t *DAT_04003a64;       /* 全局: errno 指针 (hdelete 写 3/1/0x16) */
extern uint32_t DAT_04004608;        /* 全局, 语义待定 (hexdump 填充输出实参) */
extern const char *DAT_0400460c;     /* 全局: printf 格式串 (hexdump 行首地址) */
extern const char *DAT_04004610;     /* 全局: printf 格式串 (hexdump 条目, "%0*x") */
extern const char *DAT_04004618;     /* 全局: printf 格式串 (hexdump 行 ASCII 段) */
extern const uint8_t DAT_04004614[]; /* 全局: 字符属性表 (可打印判定 &0x97) */
extern const uint8_t DAT_0400492c[]; /* 全局: 字符属性表 (空白类判定 &0x20) */
extern const char *DAT_0400651c;     /* 全局: "(null)" 串指针 (string_field 空指针回退), 语义待定 */
extern const uint8_t DAT_04007c50[]; /* 全局: 字符属性表 (数字类判定 &0x4) */
extern void *DAT_04007ddc;           /* 全局: 发送包缓冲指针 (arp_request), 语义待定 */
extern uint8_t DAT_04007de0[6];      /* 全局: 以太网头目的 MAC (arp_request), 语义待定 */
extern uint8_t DAT_04007de4[6];      /* 全局: 本机 MAC (arp_request ar_sha), 语义待定 */
extern uint32_t *DAT_04007e50;       /* 全局: 本机 IP 载体指针, 语义待定 */
extern uint32_t *DAT_04007e54;       /* 全局: 目标 IP 载体指针, 语义待定 */
extern uint32_t *DAT_04007e58;       /* 全局: 子网掩码载体指针, 语义待定 */
extern uint32_t *DAT_04007e5c;       /* 全局: ARP 目标 IP 载体指针, 语义待定 */
extern uint32_t *DAT_04007e60;       /* 全局: 网关 IP 载体指针, 语义待定 */
extern const char *DAT_04007e64;     /* 全局: 告警串指针 (网关未设), 语义待定 */
extern uint8_t *DAT_04007e68;        /* 全局: 目标 MAC 缓冲指针 (arp_tha 源), 语义待定 */

/* ----------------------------------------------------------------------------
 * SHA-256 轮常量 K (前 32 位为素数立方根小数部分, 与反编译字面量逐一核对)
 * -------------------------------------------------------------------------- */
static const uint32_t sha256_k[64] = {
    0x428a2f98u, 0x71374491u, 0xb5c0fbcfu, 0xe9b5dba5u,
    0x3956c25bu, 0x59f111f1u, 0x923f82a4u, 0xab1c5ed5u,
    0xd807aa98u, 0x12835b01u, 0x243185beu, 0x550c7dc3u,
    0x72be5d74u, 0x80deb1feu, 0x9bdc06a7u, 0xc19bf174u,
    0xe49b69c1u, 0xefbe4786u, 0x0fc19dc6u, 0x240ca1ccu,
    0x2de92c6fu, 0x4a7484aau, 0x5cb0a9dcu, 0x76f988dau,
    0x983e5152u, 0xa831c66du, 0xb00327c8u, 0xbf597fc7u,
    0xc6e00bf3u, 0xd5a79147u, 0x06ca6351u, 0x14292967u,
    0x27b70a85u, 0x2e1b2138u, 0x4d2c6dfcu, 0x53380d13u,
    0x650a7354u, 0x766a0abbu, 0x81c2c92eu, 0x92722c85u,
    0xa2bfe8a1u, 0xa81a664bu, 0xc24b8b70u, 0xc76c51a3u,
    0xd192e819u, 0xd6990624u, 0xf40e3585u, 0x106aa070u,
    0x19a4c116u, 0x1e376c08u, 0x2748774cu, 0x34b0bcb5u,
    0x391c0cb3u, 0x4ed8aa4au, 0x5b9cca4fu, 0x682e6ff3u,
    0x748f82eeu, 0x78a5636fu, 0x84c87814u, 0x8cc70208u,
    0x90befffau, 0xa4506cebu, 0xbef9a3f7u, 0xc67178f2u,
};

/* 32 位循环右移 (反编译中 x>>n | x<<(32-n) / x*mul 形式均为此) */
#define SHA256_ROR(x, n)  (((x) >> (n)) | ((x) << (32 - (n))))

// ============================================================================
// 函数转写区 (按地址升序)
// ============================================================================

// === ub_sha256_transform === 原 FUN_04000090 @ 0x04000090
// 来源: reversed/uboot_funcs/FUN_04000090.c | 分类: uboot
// 语义: SHA-256 压缩函数。从 64 字节块大端装载 W[0..15], 按
//       W[i] = σ1(W[i-2]) + W[i-7] + σ0(W[i-15]) + W[i-16] 扩展至 64 字,
//       执行 64 轮混淆 (K 常量表), 最终累加回上下文 state[0..7]。
// 参数: ctx   = sha256 上下文, 布局 (由反编译偏移证实, 与上游 sha256_context 一致):
//               total[2] @ +0x00 (本函数不访问) / state[8] @ +0x08 / buffer[64] @ +0x28
//       block = 64 字节消息块
// 注: 二进制内本函数为完全展开形态 (713 行), 末尾 62/63 两轮被编译器 CSE 合并进
//     state 回写; 此处按上游 v2016.07 lib/sha256.c 规范循环结构还原, 逐位等价。
//     Ghidra 的 uint3* 参数与 *(byte*)(base+k) 混合装载是对 ARM 非对齐大端取数的
//     建模产物 (uint3 指针步长为 3 字节), 还原为显式字节组装:
//       (b0<<24)|(b1<<16)|(b2<<8)|b3 形式的 "x<<0x18 | (x&0xff00)>>8<<0x10 | ..." 链。
//     位域还原说明: 对 3 字节组合值, 形如 "值<<0x18" 的项即首字节 b0<<24,
//     "((值&0xff00)>>8)<<0x10" 即 b1<<16, "(值>>0x10)<<8" 即 b2<<8。
void ub_sha256_transform(uint32_t *ctx, const uint8_t *block)
{
    uint32_t w[64];
    uint32_t a, b, c, d, e, f, g, h;
    uint32_t s0, s1, ch, maj, t1, t2;
    int i;

    /* W[0..15]: 16 个大端 32 位装载 (对应反编译中的分段字节拼装链) */
    for (i = 0; i < 16; i++) {
        w[i] = ((uint32_t)block[4 * i + 0] << 24) |
               ((uint32_t)block[4 * i + 1] << 16) |
               ((uint32_t)block[4 * i + 2] << 8) |
               ((uint32_t)block[4 * i + 3]);
    }

    /* W[16..63]: 消息扩展。
     * σ0(x) = ROR7(x) ^ ROR18(x) ^ (x >> 3)
     * σ1(x) = ROR17(x) ^ ROR19(x) ^ (x >> 10)
     * 对应反编译中 (x>>0x11|x*0x8000)^(x>>0x12|x*0x4000)^x>>3 等 CSE 形式 */
    for (i = 16; i < 64; i++) {
        s0 = SHA256_ROR(w[i - 15], 7) ^ SHA256_ROR(w[i - 15], 18) ^ (w[i - 15] >> 3);
        s1 = SHA256_ROR(w[i - 2], 17) ^ SHA256_ROR(w[i - 2], 19) ^ (w[i - 2] >> 10);
        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
    }

    /* 工作变量取自 ctx->state (反编译: *(uint*)(param_1+8/0xc/.../0x24)) */
    a = ctx[2];  /* state[0] @ +0x08 */
    b = ctx[3];  /* state[1] @ +0x0c */
    c = ctx[4];  /* state[2] @ +0x10 */
    d = ctx[5];  /* state[3] @ +0x14 */
    e = ctx[6];  /* state[4] @ +0x18 */
    f = ctx[7];  /* state[5] @ +0x1c */
    g = ctx[8];  /* state[6] @ +0x20 */
    h = ctx[9];  /* state[7] @ +0x24 */

    /* 64 轮主循环。
     * Σ1(e) = ROR6(e) ^ ROR11(e) ^ ROR25(e)   (反编译 (e>>0xb|e<<0x15)^... 形式)
     * Ch(e,f,g) = ((f^g)&e)^g ≡ (e&f) ^ (~e&g)
     * Σ0(a) = ROR2(a) ^ ROR13(a) ^ ROR22(a)
     * Maj(a,b,c) = ((a|b)&c)|(a&b) ≡ (a&b) ^ (a&c) ^ (b&c) */
    for (i = 0; i < 64; i++) {
        s1 = SHA256_ROR(e, 6) ^ SHA256_ROR(e, 11) ^ SHA256_ROR(e, 25);
        ch = (e & f) ^ (~e & g);
        t1 = h + s1 + ch + sha256_k[i] + w[i];

        s0 = SHA256_ROR(a, 2) ^ SHA256_ROR(a, 13) ^ SHA256_ROR(a, 22);
        maj = (a & b) ^ (a & c) ^ (b & c);
        t2 = s0 + maj;

        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    /* 累加回 state (反编译尾部合并表达式 (param_1+8/0x18 处) 即本回加的 CSE 形态) */
    ctx[2] += a;
    ctx[3] += b;
    ctx[4] += c;
    ctx[5] += d;
    ctx[6] += e;
    ctx[7] += f;
    ctx[8] += g;
    ctx[9] += h;
}

// === ub_un_02c050 === 原 FUN_04002c50 @ 0x04002c50
// 来源: reversed/uboot_funcs/FUN_04002c50.c | 分类: uboot (fallback 命名)
// 语义: 语义待定 (无字符串/无上游锚点)。2D 索引越界检查后, 从 fun_03ffae84()
//       返回的基址按大端取 1~2 个字, 并把第二个大端字写入 *arg_st08。
// 参数定位: Ghidra 仅识别出 r3 (arg_r3) 与调用方栈帧 +0x00/+0x04/+0x08 三个实参;
//       帧内 -0x14 槽位在读取前从未被赋值, 实为未识别的栈实参 (+0x14), 按入参 arg_st14 保留。
// 返回: 0xffffffff = 参数/边界检查失败; 否则为取出的大端字 (arg_st00==0 时为 0)。
uint32_t ub_un_02c050(uint32_t arg_r3, uint32_t arg_st00, uint32_t arg_st04,
                      uint32_t *arg_st08, uint32_t arg_st14)
{
    int32_t base;        /* fun_03ffae84() 返回的内存基址 */
    uint32_t ret_word;   /* 第一个大端字 (返回值) */
    uint32_t out_word;   /* 第二个大端字 (写入 *arg_st08) */
    uint32_t row_base;   /* base + arg_r3 * (arg_st04 + arg_st00) 的字偏移基址 */
    uint32_t limit;      /* (arg_st14 & ~0x3) 字节上限 */

    if (arg_st00 < 2 && arg_st04 < 2 && (base = fun_03ffae84()) != 0) {
        /* 行基址: r3 * (st04 + st00) 个 32 位字 */
        arg_r3 = arg_r3 * (arg_st04 + arg_st00);
        row_base = base + arg_r3 * 4;
        limit = (uint32_t)(base + (arg_st14 & 0xfffffffcu));
        if (row_base + arg_st00 * 4 + arg_st04 * 4 <= limit) {
            if (arg_st00 == 0) {
                ret_word = 0;
            } else {
                ret_word = *(uint32_t *)(base + arg_r3 * 4);
                /* 大端装载: b0<<24 | b1<<16 | b2<<8 | b3 */
                ret_word = ret_word << 24 | (ret_word >> 8 & 0xff) << 16 |
                           (ret_word >> 16 & 0xff) << 8 | ret_word >> 24;
            }
            if (arg_st08 == NULL) {
                return ret_word;
            }
            out_word = 0;
            if (arg_st04 != 0) {
                out_word = *(uint32_t *)(row_base + arg_st00 * 4);
                out_word = out_word << 24 | (out_word >> 8 & 0xff) << 16 |
                           (out_word >> 16 & 0xff) << 8 | out_word >> 24;
            }
            *arg_st08 = out_word;
            return ret_word;
        }
    }
    return 0xffffffffu;
}

// === ub_un_02c0e8 === 原 FUN_04002ce8 @ 0x04002ce8
// 来源: reversed/uboot_funcs/FUN_04002ce8.c | 分类: uboot (fallback 命名)
// 语义: 对 ub_un_02c050 的尾跳转 (branch) 壳。Ghidra 显示为无参调用
//       (FUN_04002c50();), r3 与栈实参在尾跳转中原样透传, 此处按同形参
//       转发建模; 返回值按 Ghidra 归类为 void。
void ub_un_02c0e8(uint32_t arg_r3, uint32_t arg_st00, uint32_t arg_st04,
                  uint32_t *arg_st08, uint32_t arg_st14)
{
    ub_un_02c050(arg_r3, arg_st00, arg_st04, arg_st08, arg_st14);
}

// === ub_un_03001c === 原 FUN_0400301c @ 0x0400301c
// 来源: reversed/uboot_funcs/FUN_0400301c.c | 分类: uboot (fallback 命名)
// 语义: 语义待定。先以 (arg1, DAT_04003060) 调用外部 fun_03ffafa8 取得索引
//       (返回 <0 则返回 0), 再以 (arg1, 索引, arg2, 8 字节栈缓冲) 调用外部
//       fun_03ffae84 并透传其返回值。
uint32_t ub_un_03001c(uint32_t arg1, uint32_t arg2)
{
    int32_t idx;
    uint32_t result;
    uint8_t stack_buf[8];       /* 帧内 -0x14 槽位: 8 字节栈缓冲, 作指针实参传出 */

    idx = fun_03ffafa8(arg1, DAT_04003060);
    if (idx < 0) {
        return 0;
    }
    result = fun_03ffae84(arg1, idx, arg2, stack_buf);
    return result;
}

// === ub_str_match === 原 FUN_04003148 @ 0x04003148
// 来源: reversed/uboot_funcs/FUN_04003148.c | 分类: uboot
// 语义: 按 flag 与 0x1c0 掩码的匹配方式字段选择字符串比较方法
//       (hashtable 过滤匹配的判定核心), 返回 0/1:
//       0x040 精确比较 (ub_strcmp == 0) / 0x080 子串匹配 (ub_strstr 非空) /
//       0x100 正则匹配 (ub_regex_match, 捕获组槽位于 128 字节栈缓冲)。
//       其余组合经 ub_printf 打印 (格式串存于 DAT_0400320c) 并返回假。
int ub_str_match(uint32_t flag, const char *s1, const char *s2, const void *regex_ctx)
{
    uint32_t mode;
    int32_t num_caps;
    uint32_t text_len;
    int32_t ret;
    uint8_t caps_buf[128];      /* 帧内 -0x98 槽位: 捕获组槽区 (每槽 8 字节) */

    mode = flag & 0x1c0;
    if (mode == 0x80) {
        /* 子串匹配 */
        ret = (int32_t)(uintptr_t)ub_strstr(s1, s2);
        return ret != 0;
    }
    if (mode == 0x100) {
        /* 正则匹配: 捕获组数存于 regex_ctx +0x208 (int 指针偏移, 按反编译保留) */
        num_caps = *(const int32_t *)((const uint8_t *)regex_ctx + 0x208);
        text_len = ub_strlen(s1);
        /* 槽位起址表达式原样保留: 槽区末端固定于 buf+0x68, 起址随捕获组数前移 */
        ret = ub_regex_match(regex_ctx, s1, text_len,
                             caps_buf + (0x80 - (num_caps * 8 + 0x18)));
        return ret != 0;
    }
    if (mode == 0x40) {
        /* 精确匹配 */
        return ub_strcmp(s1, s2) == 0;
    }
    ub_printf(DAT_0400320c, mode);
    return 0;
}

// === ub_is_prime === 原 FUN_04003300 @ 0x04003300
// 来源: reversed/uboot_funcs/FUN_04003300.c | 分类: uboot
// 语义: 从 (number | 1) 起按奇数步进寻找下一个素数 (先排除 3 的倍数, 再用奇数
//       试除至 sqrt), 命中后把 {外部分配结果, 素数, 0} 写入 out[0..2]: 分配经
//       fun_03fc8dc4(素数+1, 0x14) 完成; 返回分配结果是否非零。
//       该形态对应 hcreate_r 的建表核心 (素数判定在二进制中已内联进建表流程),
//       函数名沿用 name_map 冻结的 ub_is_prime。
int ub_is_prime(uint32_t number, int32_t *out)
{
    uint32_t rem;               /* 余数 (3 倍数判定与试除共用同一寄存器变量) */
    uint32_t divisor;           /* 试除因子 */
    uint32_t sq;                /* divisor * divisor */
    int32_t alloc_base;

    number = number | 1;
    for (;;) {
        rem = number % 3;       /* 3 的倍数直接跳过 (反编译用魔数乘法 0xAAAAAAAA 实现) */
        if (number < 10) {
            if (rem != 0)
                goto found;     /* 原标签 @0x04003364 */
        } else if (rem != 0) {
            divisor = 3;
            do {
                divisor = divisor + 2;
                rem = number % divisor;     /* __aeabi_uidivmod (FUN_0400c7d4), 取 r1 余数 */
                sq = divisor * divisor;
                if (number <= sq)
                    goto found;             /* 已达 sqrt(number), 余数非零即素数 */
            } while (rem != 0);
        }
        /* 未通过判定 → 下一奇数候选 */
next_candidate:
        number = number + 2;
        continue;

found:  /* 来源标签 @0x04003364: 余数非零 → 素数成立; 为零 (恰在平方界整除,
         * 如 25/5) → 同样转下一候选 */
        if (rem == 0)
            goto next_candidate;
        out[1] = (int32_t)number;
        out[2] = 0;
        alloc_base = fun_03fc8dc4(number + 1, 0x14);
        *out = alloc_base;
        return alloc_base != 0;
    }
}

// === ub_hdelete === 原 FUN_04003954 @ 0x04003954
// 来源: reversed/uboot_funcs/FUN_04003954.c | 分类: uboot
// 语义: 从哈希表删除条目 (上游 hashtable.c hdelete_r 形态):
//       hsearch_r(FIND) 未命中 → errno=3 (ENOENT); htab 级回调 (+0x0c) 拒绝 →
//       errno=1 (EPERM); 节点级回调 (节点+0x0c, 节点 0x14 字节) 存在且拒绝 →
//       errno=0x16 (22, EINVAL); 否则经 ub_hdelete_entry 摘链并返回 1。
// 注: htab 布局 (按访问偏移): [0] 表基址 / [1] size / [2] filled / [3] 回调。
//     Ghidra 对 hsearch 调用渲染出 8 参 (含 ENTRY 打包残留栈槽), 按
//     hsearch_r(ENTRY{key,NULL}, FIND, &ep, htab, 0) 还原为 6 参。
int ub_hdelete(const char *key, uint32_t *htab, uint32_t flag)
{
    uint32_t idx;
    void *ep;                   /* hsearch_r 回填的 ENTRY 指针 (原栈槽 -0x34) */
    ub_hash_cb_t htab_cb;
    ub_hash_cb_t node_cb;
    int32_t denied;

    idx = ub_hsearch(key, NULL, 0 /* ACTION_FIND */, &ep, htab, 0);
    if (idx == 0) {
        *DAT_04003a64 = 3;      /* ENOENT: 未找到 */
        return 0;
    }
    htab_cb = (ub_hash_cb_t)(uintptr_t)htab[3];     /* htab + 0x0c */
    if (htab_cb != NULL) {
        denied = htab_cb(ep, NULL, 1, flag);
        if (denied != 0) {
            *DAT_04003a64 = 1;  /* EPERM: 回调拒绝删除 */
            return 0;
        }
    }
    /* 节点回调: 表基址 + idx * 0x14 (节点大小) + 0x0c */
    node_cb = *(ub_hash_cb_t *)((uint8_t *)(uintptr_t)htab[0] + idx * 0x14 + 0xc);
    if (node_cb == NULL || node_cb(key, NULL, 1, flag) == 0) {
        ub_hdelete_entry(htab, &htab[2], ep, idx);
        return 1;
    }
    *DAT_04003a64 = 0x16;       /* EINVAL (22) */
    return 0;
}

// === ub_hexdump === 原 FUN_040043f0 @ 0x040043f0
// 来源: reversed/uboot_funcs/FUN_040043f0.c | 分类: uboot
// 语义: 按 (地址, 数据, 宽度 1/2/4, 条目数, 每行条目数) 打印 hex+ASCII 转储
//       (上游 print_buffer 形态)。每行: 地址行首 → 逐条目打印 hex → 不足处按
//       width*2+1 个填充字符补齐 → 行缓冲原样输出为字符串 (不可打印字符以
//       '.' 替换, 判定: 属性表值 &0x97 非零且字节高位为 0)。
// 注 1: 条目打印 ("%0*x") 需要值实参 — 反编译仅显式追踪到 (fmt, width*2),
//       第三实参为紧邻装载条目值的同一寄存器 (r2), 按调用约定还原。
// 注 2: 补齐循环原位递减 linelen (param_5), 反编译原样保留。
// 注 3: fun_03fc7fbc 为 u-boot puts 形态输出 (不追加换行); fun_03fc7fe4 返回
//       非零则中止转储返回 -1 (行为同 ctrl-c 检测, 命名待补)。
uint32_t ub_hexdump(uint32_t addr, const uint8_t *data, uint32_t width,
                    uint32_t count, uint32_t linelen)
{
    uint32_t pad_arg;           /* DAT_04004608: 填充输出实参 */
    const uint8_t *p;           /* 行内数据遍历指针 */
    uint8_t *q;                 /* 行缓冲 ASCII 段扫描指针 */
    const uint8_t *line_end;
    uint32_t this_linelen;      /* 本行实际条目数 */
    uint32_t copied;            /* 本行拷入行缓冲的字节数 */
    uint32_t value;             /* 当前条目原始值 (hex 打印实参) */
    uint32_t i;
    uint32_t remaining;

    uint8_t linebuf[68];        /* 帧内 -0x64 槽位: 行缓冲 */

    if (width * linelen > 0x40)
        linelen = aeabi_uidiv(0x40, width);
    if (linelen == 0)
        linelen = aeabi_uidiv(0x10, width);

    pad_arg = DAT_04004608;
    if (count == 0)
        return 0;

    remaining = count;
    do {
        ub_printf(DAT_0400460c, addr);          /* 行首地址 */
        this_linelen = (remaining < linelen) ? remaining : linelen;
        if (this_linelen == 0) {
            copied = 0;
        } else {
            /* 逐条目: 按宽度装载 → 存入行缓冲 (供 ASCII 段) → 打印 hex */
            i = 0;
            p = data;
            do {
                if (width == 4) {
                    value = *(const uint32_t *)p;
                    *(uint32_t *)(linebuf + i * 4) = value;
                } else if (width == 2) {
                    value = *(const uint16_t *)p;
                    *(uint16_t *)(linebuf + i * 2) = (uint16_t)value;
                } else {
                    value = *p;
                    linebuf[i] = (uint8_t)value;
                }
                i = i + 1;
                p = p + width;
                ub_printf(DAT_04004610, width * 2, value);   /* "%0*x": 宽 = width*2 */
            } while (this_linelen != i);

            copied = this_linelen * width;
            data = data + copied;
        }

        /* 补齐缺失条目: 每个缺位打印 width*2+1 个填充字符。
         * 反编译经 goto 进入本循环 (源标签 @0x04004524; this_linelen==0 且 linelen>0
         * 时补齐整行); linelen 原位递减 (原样保留)。 */
        for (; this_linelen < linelen; linelen--) {
            i = 0;
            do {
                i = i + 1;
                fun_03fc7fbc((const char *)(uintptr_t)pad_arg);
            } while (i < width * 2 + 1);
        }

        if (copied != 0) {
            /* ASCII 段: 不可打印字节替换为 '.' (原 goto 循环等价改写) */
            line_end = linebuf + copied;
            for (q = linebuf; q != line_end; q++) {
                if (!(((DAT_04004614[*q] & 0x97) != 0) && ((*q & 0x80) == 0)))
                    *q = '.';
            }
            addr = addr + copied;
        }

        linebuf[copied] = 0;
        remaining = remaining - this_linelen;
        ub_printf(DAT_04004618, linebuf);       /* 行 ASCII 段 */
        if (fun_03fc7fe4() != 0)
            return 0xffffffffu;
    } while (remaining != 0);
    return 0;
}

// === ub_crc32 === 原 FUN_04004700 @ 0x04004700
// 来源: reversed/uboot_funcs/FUN_04004700.c | 分类: uboot
// 语义: CRC-32 收尾包装: 对 ~seed 调用 ub_crc32_le 后再取反
//       (标准 CRC32 的初值/终值异或约定)。
uint32_t ub_crc32(uint32_t seed)
{
    uint32_t result;

    result = ub_crc32_le(~seed);    /* Ghidra 单参调用形态 (FUN_0400461c) */
    return ~result;
}

// === ub_div64_rem === 原 FUN_04004754 @ 0x04004754
// 来源: reversed/uboot_funcs/FUN_04004754.c | 分类: uboot
// 语义: 64 位被除数 (n[0]=低字, n[1]=高字) 除以 32 位除数 base:
//       商写回 *n (64 位), 返回 32 位余数。上游 __div64_32 形态: 先对高字做
//       32/32 归约 (商入结果高 32 位), 再以倍增-回减法完成剩余部分。
// 注: 32/32 除法以 C 运算符表达 (编译器生成 __aeabi_uidiv, FUN_0400c66c;
//     反编译中该调用呈单参形态系 r1 复用 base 所致)。
uint32_t ub_div64_rem(uint32_t *n, uint32_t base)
{
    uint64_t rem;       /* 余数/被除数 (= (n[1]<<32) | n[0]) */
    uint64_t b;         /* 倍增试除数 */
    uint64_t d;         /* 与 b 对应的位权 */
    uint64_t res;       /* 商累加 */
    uint32_t high;
    uint32_t q;

    rem = ((uint64_t)n[1] << 32) | n[0];
    b = base;
    d = 1;
    res = 0;
    high = (uint32_t)(rem >> 32);

    /* 归约: 高字先行除法, 商置于结果高 32 位, 低位累加器清零 */
    if (high >= base) {
        q = high / base;
        res = (uint64_t)q << 32;
        rem -= (uint64_t)q * base << 32;
    }

    /* 倍增 b/d 直至 b >= rem (b 保持 int64 正值) */
    while ((int64_t)b > 0 && b < rem) {
        b = b + b;
        d = d + d;
    }

    /* 回减法: 命中则减去并累加位权 */
    do {
        if (rem >= b) {
            rem = rem - b;
            res = res + d;
        }
        b = b >> 1;
        d = d >> 1;
    } while (d != 0);

    n[0] = (uint32_t)res;
    n[1] = (uint32_t)(res >> 32);
    return (uint32_t)rem;       /* 循环结束后余数 < base, 高字必为 0 */
}

// === ub_strtrim === 原 FUN_04004898 @ 0x04004898
// 来源: reversed/uboot_funcs/FUN_04004898.c | 分类: uboot
// 语义: 去除字符串首尾空白后返回去前导空白后的指针:
//       先跳过前导空白 (ctype 表位 0x20 判定, 表基址 DAT_0400492c),
//       再把尾部空白截断 (写入 '\0'); 空串原样返回。
char *ub_strtrim(char *s)
{
    char *end;
    uint32_t len;

    /* 跳过前导空白 */
    while ((DAT_0400492c[(uint8_t)*s] & 0x20) != 0)
        s = s + 1;

    len = ub_strlen(s);
    if (len != 0) {
        end = s + (len - 1);
        /* 无指针回绕 (反编译 CARRY4 检查) 且末字符为空白 → 逐个回退 */
        if (((uintptr_t)s + (len - 1) >= (uintptr_t)s) &&
            ((DAT_0400492c[(uint8_t)s[len - 1]] & 0x20) != 0)) {
            do {
                end = end - 1;
                if (end == s - 1)       /* 全为空白 → 截为空串 */
                    break;
            } while ((DAT_0400492c[(uint8_t)*end] & 0x20) != 0);
        }
        end[1] = '\0';
    }
    return s;
}

// === ub_regex_rep_greedy === 原 FUN_04005038 @ 0x04005038
// 来源: reversed/uboot_funcs/FUN_04005038.c | 分类: uboot
// 语义: 正则重复节点贪婪匹配 (TPOP 风格): 反复匹配节点并尝试后续模式,
//       记录最远的成功位置; 后续匹配失败即回退 out_end 并以其为最终结果。
//       节点布局: prog[node] 为操作码, prog[node+1] 为节点长度,
//       节点体自 node+2 起, 下一节点位于 node + prog[node+1]。
void ub_regex_rep_greedy(const uint8_t *prog, uint32_t node_pos, uint32_t arg3,
                         uint32_t arg4, uint32_t *out_end)
{
    uint32_t best_end;
    uint32_t cur_end;
    int32_t ok;

    best_end = *out_end;
    for (;;) {
        ok = ub_regex_exec(prog, node_pos + 2, arg3, arg4, out_end, 0);
        if (ok == 0)
            break;
        cur_end = *out_end;
        /* 尝试匹配本节点之后的剩余模式 */
        ok = ub_regex_exec(prog, (uint32_t)prog[node_pos + 1] + node_pos,
                           arg3, arg4, out_end, 0);
        *out_end = cur_end;
        if (ok != 0)
            best_end = cur_end;
    }
    *out_end = best_end;
}

// === ub_regex_rep_lazy === 原 FUN_040050dc @ 0x040050dc
// 来源: reversed/uboot_funcs/FUN_040050dc.c | 分类: uboot
// 语义: 正则重复节点懒惰匹配 (TPOP 风格): 仅在后续模式失败时继续重复节点;
//       一旦后续模式成功即停止, 结果取停止前最后一次节点匹配的结束位置。
void ub_regex_rep_lazy(const uint8_t *prog, uint32_t node_pos, uint32_t arg3,
                       uint32_t arg4, uint32_t *out_end)
{
    uint32_t best_end;
    int32_t ok;

    best_end = *out_end;
    do {
        ok = ub_regex_exec(prog, node_pos + 2, arg3, arg4, out_end, 0);
        if (ok == 0)
            break;
        best_end = *out_end;
        ok = ub_regex_exec(prog, (uint32_t)prog[node_pos + 1] + node_pos,
                           arg3, arg4, out_end, 0);
    } while (ok == 0);
    *out_end = best_end;
}

// === ub_strnlen === 原 FUN_04005bc4 @ 0x04005bc4
// 来源: reversed/uboot_funcs/FUN_04005bc4.c | 分类: uboot
// 语义: 受限串长: 返回 min(strlen(s), maxlen)。
int32_t ub_strnlen(const char *s, int32_t maxlen)
{
    const char *p;

    if (maxlen == 0)
        return 0;
    if (*s == '\0')
        return 0;
    p = s;
    do {
        p = p + 1;
        if (s + maxlen == p)
            return (int32_t)(s + maxlen) - (int32_t)s;      /* 即 maxlen */
    } while (*p != '\0');
    return (int32_t)((uintptr_t)p - (uintptr_t)s);
}

// === ub_memset === 原 FUN_04005e0c @ 0x04005e0c
// 来源: reversed/uboot_funcs/FUN_04005e0c.c | 分类: uboot
// 语义: memset: 指针 4 字节对齐时先按 32 位字填充 (填充字节展开为
//       fill*4 的字), 余数 (len & 3) 按字节; 非对齐时整段按字节。
void ub_memset(void *dst, uint8_t fill, uint32_t len)
{
    uint32_t word_fill;         /* CONCAT31(CONCAT21(CONCAT11(fill,fill),fill),fill) */
    uint32_t words;
    uint32_t *word_ptr;
    uint8_t *byte_ptr;

    if (((uint32_t)(uintptr_t)dst & 3) != 0) {
        /* 非对齐: 整段按字节填充 */
        if (len == 0)
            return;
        byte_ptr = (uint8_t *)dst;
        do {
            *byte_ptr = fill;
            byte_ptr = byte_ptr + 1;
        } while (byte_ptr != (uint8_t *)dst + len);
        return;
    }
    if (3 < len) {
        words = (len - 4) >> 2;             /* 共写 words+1 = len/4 个字 */
        word_fill = (uint32_t)fill | (uint32_t)fill << 8 |
                    (uint32_t)fill << 16 | (uint32_t)fill << 24;
        word_ptr = (uint32_t *)dst;
        do {
            *word_ptr = word_fill;
            word_ptr = word_ptr + 1;
        } while (word_ptr != (uint32_t *)dst + words + 1);
        len = len & 3;
        dst = (uint32_t *)dst + words + 1;
    }
    if (len == 0)
        return;
    /* 余数按字节填充 */
    byte_ptr = (uint8_t *)dst;
    do {
        *byte_ptr = fill;
        byte_ptr = byte_ptr + 1;
    } while (byte_ptr != (uint8_t *)dst + len);
    return;
}

// === ub_memmove === 原 FUN_04005fa0 @ 0x04005fa0
// 来源: reversed/uboot_funcs/FUN_04005fa0.c | 分类: uboot
// 语义: memmove: src == dst 直接返回; src < dst (目标在源之后) 时自尾向头
//       倒序拷贝, 否则顺序拷贝。反编译为前自增/前自减形态, 语义相同。
void ub_memmove(void *dst, const void *src, int32_t len)
{
    const uint8_t *sp;
    uint8_t *dp;

    if (src == dst)
        return;
    if ((uintptr_t)src < (uintptr_t)dst) {
        /* 倒序拷贝: 从 src+len-1 向 src[0] 逐字节回退 */
        if (len == 0)
            return;
        dp = (uint8_t *)dst + len;
        sp = (const uint8_t *)src + len;
        do {
            sp = sp - 1;
            dp = dp - 1;
            *dp = *sp;
        } while (sp != (const uint8_t *)src);
        return;
    }
    if (len == 0)
        return;
    /* 顺序拷贝: 复制 src[0..len-1] (反编译为前自增形态: 先写入旧 sp 指向的
     * src[i], 再推进 sp, 二者保持一个单位的滞后) */
    dp = (uint8_t *)dst - 1;
    sp = (const uint8_t *)src;
    do {
        dp = dp + 1;
        *dp = *sp;
        sp = sp + 1;
    } while (sp != (const uint8_t *)src + len);
    return;
}

// === ub_udelay === 原 FUN_040062f8 @ 0x040062f8
// 来源: reversed/uboot_funcs/FUN_040062f8.c | 分类: uboot
// 语义: 延时 (微秒): 分段调用 ub_udelay_raw (FUN_04006244), 每段上限
//       10000000 微秒 (0x98967f 以下按原值, 否则取 10000000)。
void ub_udelay(uint32_t usec)
{
    uint32_t chunk;

    do {
        chunk = usec;
        if (9999999 < usec)
            chunk = 10000000;
        ub_udelay_raw(chunk);
        usec = usec - chunk;
    } while (usec != 0);
    return;
}

// === ub_put_dec_helper === 原 FUN_0400636c @ 0x0400636c
// 来源: reversed/uboot_funcs/FUN_0400636c.c | 分类: uboot
// 语义: 十进制数字发射器 (vsprintf put_dec 家族的低位段): 把 16 位打包余数
//       展开为 1~5 个十进制字符写入 buf, 返回最后一个数字之后的地址
//       (前导零被抑制)。打包域: [3:0] 个位 / [7:4] 十位 / [11:8] 百位 /
//       [15+:] 千位, 各域含 Douglas Jones 式借位修正 (高域 ×6/×9/×5/×4/×2 项)。
// 注: *0xcd>>0xb 与 *0xd>>7 为编译器对除以 10 的强度削减 (商经回减验证),
//     按反编译原样保留; Ghidra 把"剩余发射量"与返回指针复用同一变量,
//     此处拆分为独立量后按等价早退结构还原。
char *ub_put_dec_helper(char *buf, uint32_t packed)
{
    uint32_t d1;        /* 十位域 */
    uint32_t d2;        /* 百位域 */
    uint32_t d3;        /* 千位域 */
    uint32_t sum0;      /* 个位 + 6*(高位域和) */
    uint32_t q0;
    uint32_t rem1;      /* 第一轮剩余发射量 (为 0 则仅 1 位) */
    uint32_t q1;
    uint32_t sum1;
    uint32_t rem2;
    uint32_t next2;
    uint32_t q3;

    d1 = (packed & 0xff) >> 4;
    d2 = (packed & 0xfff) >> 8;
    d3 = packed >> 0xc;
    sum0 = (packed & 0xf) + (d1 + d2 + d3) * 6;
    q0 = (sum0 * 0xcd) >> 0xb;                  /* ≈ sum0 / 10 */
    rem1 = q0 + d3 * 9 + d1 + d2 * 5;

    buf[0] = (char)(sum0 - q0 * 10 + '0');      /* 个位 */
    if (rem1 == 0)
        return buf + 1;

    q1 = (rem1 * 0xcd) >> 0xb;                  /* ≈ rem1 / 10 */
    sum1 = q1 + d2 * 2;
    buf[1] = (char)(rem1 - q1 * 10 + '0');      /* 十位 */
    if (d3 == 0 && sum1 == 0)
        return buf + 2;

    rem2 = (sum1 * 0xd) >> 7;                   /* ≈ sum1 / 10 (借位修正形态) */
    next2 = rem2 + d3 * 4;
    buf[2] = (char)(sum1 - rem2 * 10 + '0');    /* 百位 */
    if (next2 == 0)
        return buf + 3;

    q3 = (next2 * 0xcd) >> 0xb;                 /* ≈ next2 / 10 */
    buf[3] = (char)(next2 - q3 * 10 + '0');     /* 千位 */
    if (q3 != 0) {
        buf[4] = (char)(q3 + 0x30);             /* 进位位 (q3 < 10, 单个数字) */
        return buf + 5;
    }
    return buf + 4;
}

// === ub_string_field === 原 FUN_0400644c @ 0x0400644c
// 来源: reversed/uboot_funcs/FUN_0400644c.c | 分类: uboot
// 语义: vsprintf 的字符串字段输出 (上游 string()): s 为空时改用
//       DAT_0400651c ("(null)"); 长度 = ub_strnlen(s, precision);
//       flags 位 0x10 (LEFT) 决定左对齐: 非左对齐时先左侧补空格至
//       field_width; 再拷贝 len 字节; 最后右侧补空格至 field_width。
//       buf 越过 end 后仅计数不再写入。
uint8_t *ub_string_field(uint8_t *buf, uint8_t *end, const char *s,
                         int32_t field_width, int32_t precision, uint32_t flags)
{
    const char *str;
    int32_t len;
    int32_t i;

    str = DAT_0400651c;
    if (s != NULL)
        str = s;

    len = (int32_t)ub_strnlen(str, (uint32_t)precision);

    if ((flags & 0x10) == 0) {
        /* 右对齐: 左侧补空格 (上游 while (len < field_width--)) */
        while (len < field_width--) {
            if (buf < end)
                *buf = ' ';
            buf = buf + 1;
        }
    }

    /* 拷贝字符串本体 */
    for (i = 0; i < len; i++) {
        if (buf < end)
            *buf = (uint8_t)*str;
        buf = buf + 1;
        str = str + 1;
    }

    /* 右侧补空格 (补齐后 field_width 已被减至 len-1, 循环不进入) */
    while (len < field_width--) {
        if (buf < end)
            *buf = ' ';
        buf = buf + 1;
    }
    return buf;
}

// === ub_printf === 原 FUN_04007548 @ 0x04007548
// 来源: reversed/uboot_funcs/FUN_04007548.c | 分类: uboot
// 语义: printf 包装 (上游 printf 形态): 格式化进 2072 字节栈缓冲
//       (ub_vsnprintf, 容量实参 0x817 = 2071), 返回值超过 0x815 时钳位到
//       0x816, 最后经 fun_03fc7fbc (puts 形态, 不追加换行) 输出。
// 注: Ghidra 将 va_start 的寄存器保存区渲染为显式的 param_2..4 栈拷贝
//     (帧内 -0xc/-8/-4 三个栈槽) 并把保存区指针传给 vsnprintf — 与 AAPCS va_list 传递
//     一致, 此处按 va_start/va_list 规范形态还原。
uint32_t ub_printf(const char *fmt, ...)
{
    va_list args;
    char print_buffer[2072];    /* 帧内 -0x830 槽位缓冲 */
    uint32_t len;

    va_start(args, fmt);
    len = ub_vsnprintf(print_buffer, 0x817, fmt, args);
    va_end(args);
    if (0x815 < len)
        len = 0x816;
    fun_03fc7fbc(print_buffer);
    return len;
}

// === ub_panic === 原 FUN_040076e8 @ 0x040076e8
// 来源: reversed/uboot_funcs/FUN_040076e8.c | 分类: uboot
// 语义: panic: 输出消息后触发 ub_panic_reset (FUN_040076b8: 外部复位调用
//       func_03fc7f78(10) → ub_udelay(100000) → func_03fb1d70(0,0,0) 板级复位
//       → 死循环), 本函数不可返回。消息/格式串经寄存器传入 (Ghidra 签名为
//       void(void), 形参未还原; 上游 panic(fmt, ...) 形态)。
// 注 (反编译污染说明): ub_panic_reset 实为 noreturn (体尾死循环), Ghidra 未
//     识别, 其后的 fallthrough 使 FUN_04007718 (simple_strtoul @0x04007718) 的
//     代码域被混叠进本反编译函数: 表现为一次 ub_vprintf 调用、第二次
//     ub_panic_reset 调用及与 FUN_04007718 逐行相同的数字解析段
//     (@0x04007744/@0x040077b4/@0x04007794 均位于 0x04007718 函数域内)。
//     该污染段不重复转写 (由 FUN_04007718 所在文件承载), 本函数按上游
//     v2016.07 panic 语义还原: vprintf → 换行输出 → ub_panic_reset。
void ub_panic(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    ub_vprintf(fmt, args);
    va_end(args);
    /* 换行输出: 反编译中该 puts 形态外部调用的实参未被 Ghidra 还原,
     * 按上游 panic 行为 (putc('\n')) 补记为换行串 */
    fun_03fc7fbc("\n");
    ub_panic_reset();           /* FUN_040076b8: noreturn */
}

// === parse_trailing_index === 原 FUN_04007c54 @ 0x04007c54
// 来源: reversed/uboot_funcs/FUN_04007c54.c | 分类: uboot
// 语义: 解析字符串末尾的十进制索引 (ctype 表 DAT_04007c50, &0x4 = 数字类):
//       末字符为数字时从尾向首扫描连续数字段 (整串皆为数字则返回 -1),
//       对数字段起点调用 simple_strtoul(..., 10) 并返回其值;
//       串长 ≤ 1 提前返回 -1; 末字符非数字时按反编译路径解析空串返回 0。
// 注: ub_strlen (FUN_04005b98) 调用在反编译中未显式列出实参 (r0 复用首参)。
int32_t parse_trailing_index(const char *s)
{
    const char *p;          /* 扫描位置 (pbVar2) */
    const char *q;          /* 前一位置 (pbVar1) */
    int32_t len;
    uint32_t value;

    len = (int32_t)ub_strlen(s);
    p = s + len - 1;
    if (p <= s)
        return -1;
    if ((DAT_04007c50[(uint8_t)s[len - 1]] & 4) != 0) {
        q = s + len - 2;
        do {
            p = q;
            if (p == s)
                return -1;              /* 全串皆为数字 */
            q = p - 1;
        } while ((DAT_04007c50[(uint8_t)*p] & 4) != 0);
    }
    value = simple_strtoul(p + 1, NULL, 10);
    return (int32_t)value;
}

// === arp_request === 原 FUN_04007de8 @ 0x04007de8
// 来源: reversed/uboot_funcs/FUN_04007de8.c | 分类: uboot
// 语义: 构造并发送 ARP 请求帧 (上游 net/arp.c ArpRequest 形态):
//       1) 目标 IP 选取 (*DAT_04007e5c 为待写目标): ((*e50 ^ *e54) & *e58)==0
//          (与 *e50 同子网, *e58 为掩码) 时取 *e54; 否则取 *e60 (网关),
//          为 0 时经 puts 形态外部调用打印 *DAT_04007e64 告警串并回退 *e54。
//       2) net_set_ether(*DAT_04007ddc, DAT_04007de0, 0x806) 写以太网头
//          (目的 MAC = DAT_04007de0, 帧类型 ARP), 返回头长。
//       3) 填 ARP 报文 (报文基址 = 缓冲 + 头长): +0 ar_hrd=1, +2 ar_pro=0x0800,
//          +4 (ar_hln,ar_pln)=(6,4), +6 ar_op=1 (REQUEST), +8 ar_sha=DAT_04007de4,
//          +0xe ar_spa=*e50, +0x12 ar_tha=*e68 (MAC 缓冲), +0x18 ar_tpa=*e5c。
//       4) eth_send(缓冲, 头长 + 0x1c)。
// 注: 多字节 ARP 字段按大端存储, 小端主机上以字节交换值写入 (反编译原样);
//     SPA 槽位 (-0x24) 首次写入 (DAT_04007e68) 在复用前被覆盖, 为死赋值, 已略去。
void arp_request(void)
{
    uint32_t *wait_ip;      /* DAT_04007e5c: 目标 IP 载体 */
    uint32_t *ref_ip;       /* DAT_04007e54: 参照目标 IP 载体 */
    uint32_t *our_ip;       /* DAT_04007e50: 本机 IP 载体 */
    void *tx_pkt;           /* *DAT_04007ddc: 发送缓冲基址 */
    uint32_t gw_ip;         /* 网关 IP 临时 */
    uint32_t tpa;           /* 目标 IP (= *wait_ip) */
    uint32_t spa;           /* 本机 IP (= *our_ip) */
    uint8_t spa_slot[4];    /* 帧内 -0x24 槽位: SPA 传参临时 */
    uint8_t tpa_slot[4];    /* 帧内 -0x28 槽位: TPA 传参临时 */
    int32_t hdr_len;
    uint8_t *arp_base;

    wait_ip = DAT_04007e5c;
    ref_ip = DAT_04007e54;
    our_ip = DAT_04007e50;
    if (((*our_ip ^ *ref_ip) & *DAT_04007e58) == 0) {
        *wait_ip = *ref_ip;
    } else {
        gw_ip = *DAT_04007e60;
        if (gw_ip == 0) {
            fun_03fc7fbc(DAT_04007e64);     /* 网关未设告警 */
            gw_ip = *ref_ip;
        }
        *wait_ip = gw_ip;
    }

    tx_pkt = DAT_04007ddc;
    tpa = *wait_ip;
    spa = *our_ip;
    hdr_len = net_set_ether(tx_pkt, DAT_04007de0, 0x806);
    arp_base = (uint8_t *)tx_pkt + hdr_len;

    *(uint16_t *)(arp_base + 0) = 0x100;    /* ar_hrd: 大端 0x0001 (Ethernet) */
    *(uint16_t *)(arp_base + 2) = 8;        /* ar_pro: 大端 0x0800 (IP) */
    *(uint16_t *)(arp_base + 6) = 0x100;    /* ar_op: 大端 0x0001 (REQUEST) */
    *(uint16_t *)(arp_base + 4) = 0x406;    /* (ar_hln, ar_pln) = (6, 4) */

    ub_memcpy(arp_base + 8, DAT_04007de4, 6);               /* ar_sha: 本机 MAC */
    *(uint32_t *)spa_slot = spa;
    ub_memcpy(arp_base + 0xe, spa_slot, 4);                 /* ar_spa: 本机 IP */
    ub_memcpy(arp_base + 0x12, DAT_04007e68, 6);            /* ar_tha: 目标 MAC 缓冲 */
    *(uint32_t *)tpa_slot = tpa;
    ub_memcpy(arp_base + 0x18, tpa_slot, 4);                /* ar_tpa: 目标 IP */

    eth_send(tx_pkt, hdr_len + 0x1c);
    return;
}

/* ==================== 覆盖率表 ====================
 * 分配 (batch uboot_b1, 按地址升序前 23/45): 23 函数 — 全部转写。
 * | # | 语义名               | 原 FUN/符号     | 地址       | 一句话语义                                        |
 * |---|----------------------|-----------------|------------|---------------------------------------------------|
 * | 1 | ub_sha256_transform  | FUN_04000090    | 0x04000090 | SHA-256 压缩函数 (64 字块→W 扩展→64 轮→state 累加)|
 * | 2 | ub_un_02c050         | FUN_04002c50    | 0x04002c50 | 语义待定: 越界检查后大端取字并回写 (r3/栈实参)    |
 * | 3 | ub_un_02c0e8         | FUN_04002ce8    | 0x04002ce8 | ub_un_02c050 的尾跳转透传壳                       |
 * | 4 | ub_un_03001c         | FUN_0400301c    | 0x0400301c | 语义待定: 外部索引查询→外部调用透传               |
 * | 5 | ub_str_match         | FUN_04003148    | 0x04003148 | flag&0x1c0 分派: 0x40 精确/0x80 子串/0x100 正则   |
 * | 6 | ub_is_prime          | FUN_04003300    | 0x04003300 | 寻下一个素数并按 (素数+1,0x14) 外部分配建表       |
 * | 7 | ub_hdelete           | FUN_04003954    | 0x04003954 | 哈希表删项 (ENOENT/EPERM/EINVAL 三态)             |
 * | 8 | ub_hexdump           | FUN_040043f0    | 0x040043f0 | hex+ASCII 转储 (print_buffer 形态, ctrl-c 可中止) |
 * | 9 | ub_crc32             | FUN_04004700    | 0x04004700 | CRC-32 收尾包装 (~crc32_le(~seed))                |
 * |10 | ub_div64_rem         | FUN_04004754    | 0x04004754 | 64/32 位除法, 商写回 *n, 返回余数 (__div64_32)    |
 * |11 | ub_strtrim           | FUN_04004898    | 0x04004898 | 去首尾空白 (ctype 表位 0x20)                      |
 * |12 | ub_regex_rep_greedy  | FUN_04005038    | 0x04005038 | 正则重复节点贪婪匹配 (记录最远成功位置)           |
 * |13 | ub_regex_rep_lazy    | FUN_040050dc    | 0x040050dc | 正则重复节点懒惰匹配 (后续成功即止)               |
 * |14 | ub_strnlen           | FUN_04005bc4    | 0x04005bc4 | 受限串长 min(strlen, maxlen)                      |
 * |15 | ub_memset            | FUN_04005e0c    | 0x04005e0c | memset (对齐字填充+余数字节)                      |
 * |16 | ub_memmove           | FUN_04005fa0    | 0x04005fa0 | memmove (src<dst 倒序拷贝, 否则顺序)              |
 * |17 | ub_udelay            | FUN_040062f8    | 0x040062f8 | 微秒延时 (分段调用 ub_udelay_raw, 上限 1e7)       |
 * |18 | ub_put_dec_helper    | FUN_0400636c    | 0x0400636c | 打包余数→1~5 个十进制字符 (前导零抑制)            |
 * |19 | ub_string_field      | FUN_0400644c    | 0x0400644c | vsprintf 字符串字段 (宽度/精度/左右对齐)          |
 * |20 | ub_printf            | FUN_04007548    | 0x04007548 | printf 包装 (2072 字节栈缓冲→vsnprintf→puts)      |
 * |21 | ub_panic             | FUN_040076e8    | 0x040076e8 | panic: vprintf→换行→ub_panic_reset (noreturn)     |
 * |22 | parse_trailing_index | FUN_04007c54    | 0x04007c54 | 解析串尾十进制索引 (simple_strtoul base 10)       |
 * |23 | arp_request          | FUN_04007de8    | 0x04007de8 | 构造并发送 ARP 请求帧 (网关回退逻辑)              |
 */
