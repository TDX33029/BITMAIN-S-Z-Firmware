/*
 * rewritten/monitor/cjson.c — miner-monitor 守护内嵌 cJSON 库 (语义化转写)
 *
 * 来源: reversed/monitor_funcs/FUN_0000c244.c ~ FUN_0000dfd8.c (monitor 二进制)
 * 范围: 36 个函数 (json_* 全套: parse/print/create/detach/replace/duplicate/...)
 *       parse_hex4 (FUN_0000c130) 属 monitor_core_p1.c 批次, 此处仅 extern 声明。
 *
 * 事实还原 (全部以反编译偏移为证):
 *   - 节点结构 cjson_node_t 共 0x28 字节 (json_new_item 固定分配 0x28 并 memset 清零;
 *     json_create_reference 按整节点 memcpy 0x28), 布局与经典 cJSON v1.x (32 位) 一致。
 *   - type 字段 (+0x0c) 低字节为类型 0..6; bit8 (0x100) = IsReference 引用克隆标志,
 *     被 | 0x100 的节点不拥有 child/valuestring (json_delete 不释放)。
 *   - 分配统一走全局钩子 DAT_0001b374 (malloc 语义函数指针);
 *     释放统一走 PTR_free_0001b378 (Ghidra 已解析为 free 符号)。
 *   - 解析错误位置记录于全局指针 DAT_0001b3a4 (即 cJSON 的 ep)。
 *   - print 路径为旧版两遍算法 (先统计长度再拼接), fmt 非 0 时按深度输出 '\t' 缩进。
 *   - parse 路径无嵌套深度限制 (新版 cJSON 的 nesting 计数不存在于此版本;
 *     json_parse 内被裁剪的 ParseWithOpts 残块见反编译 unreachable 注记)。
 *   - VectorSignedToFloat / VectorFloatToSigned 为 ARM VCVT 指令的 Ghidra 伪码,
 *     分别还原为 (double)(int) 与 (int)(double) 转换; in_fpscr 位噪声已略去。
 *   - 反编译中的 p == NULL 指针越界保护 (自增回绕) 均为不可达伪代码, 已略去。
 */
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>      /* sprintf */
#include <string.h>     /* strlen/strchr/strcpy/strcmp/strncmp/memcpy/memset */
#include <math.h>       /* fabs/floor/pow */
#include <float.h>      /* DBL_EPSILON */
#include <ctype.h>      /* tolower (原码内联为 __ctype_tolower_loc() 查表) */

/* ================= 类型与常量 ================= */

/* cJSON 节点 (字段偏移均有调用点证据, sizeof == 0x28) */
typedef struct cjson_node {
    struct cjson_node *next;         /* +0x00: 兄弟链后继 (d05c/d864/da90/dba4/dcfc 锁定) */
    struct cjson_node *prev;         /* +0x04: 兄弟链前驱 (d05c/dac8/dba4/dcfc 锁定) */
    struct cjson_node *child;        /* +0x08: 首子节点 (d05c/c83c/da28 锁定) */
    uint32_t           type;         /* +0x0c: 类型+标志, 低字节见 JSON_* (c83c/d05c 锁定) */
    char              *valuestring;  /* +0x10: 字符串值 (c244 写入/c83c 读取锁定) */
    int32_t            valueint;     /* +0x14: 整数值 (d05c/ddd4/c83c 锁定) */
    double             valuedouble;  /* +0x18: 浮点值 (d05c/ddd4/c83c 锁定) */
    char              *string;       /* +0x20: 键名 (d05c 搬移/da50 比较/c69c 打印锁定) */
    uint32_t           _pad;         /* +0x24: 尾部对齐填充 (0x28 尺寸由 c540/c5b4 锁定) */
} cjson_node_t;

/* type 低字节类型值 (parse_value 置 / print_value 分派) */
#define JSON_FALSE        0u
#define JSON_TRUE         1u
#define JSON_NULL         2u
#define JSON_NUMBER       3u
#define JSON_STRING       4u
#define JSON_ARRAY        5u
#define JSON_OBJECT       6u
#define JSON_IS_REFERENCE 0x100u   /* 引用克隆标志: delete 时跳过 child/valuestring */

/* UTF-8 首字节前缀表, 按编码字节数索引 (原 UNK_0001190c):
   [1]=0x00 [2]=0xC0 [3]=0xE0 [4]=0xF0; [0] 未用 (数据段未导出, 按 UTF-8 语义还原) */
static const uint8_t cjson_utf8_prefix[5] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0 };

/* ---- 外部依赖 ---- */
extern uint32_t parse_hex4(const char *hex);   /* 原 FUN_0000c130, 4 位十六进制 → 数值 */

/* ---- cJSON 全局钩子/状态 (Ghidra 原名保留) ---- */
extern void *(*DAT_0001b374)(size_t size);     /* 分配钩子 (malloc 语义), 节点与缓冲均经此 */
extern void  (*PTR_free_0001b378)(void *);     /* 释放钩子 (指向 free) */
extern const char *DAT_0001b3a4;               /* 解析失败位置指针 (cJSON 全局 ep) */

/* ---- 前向声明 (定义按原地址序排列) ---- */
char         *json_parse_string(cjson_node_t *item, const char *str);
cjson_node_t *json_new_item(void);
char         *json_strdup(const char *str);
cjson_node_t *json_create_reference(cjson_node_t *item);
int           json_strcasecmp(const char *s1, const char *s2);
char         *json_print_string(const char *str);
char         *json_print_value(cjson_node_t *item, int depth, int fmt);
const char   *json_parse_value(cjson_node_t *item, const char *value);
void          json_delete(cjson_node_t *item);
cjson_node_t *json_parse(const char *value);
char         *json_print(cjson_node_t *item);
int           json_get_array_size(const cjson_node_t *array);
cjson_node_t *json_get_array_item(cjson_node_t *array, int index);
cjson_node_t *json_get_object_item(cjson_node_t *object, const char *name);
void          json_add_item_to_array(cjson_node_t *array, cjson_node_t *item);
void          json_add_item_to_object(cjson_node_t *object, const char *name, cjson_node_t *item);
void          json_add_item_reference(cjson_node_t *array, cjson_node_t *item);
void          json_add_ref_to_object(cjson_node_t *object, const char *name, cjson_node_t *target);
cjson_node_t *json_detach_item_at(cjson_node_t *array, int index);
void          json_delete_item_at(cjson_node_t *array, int index);
cjson_node_t *json_detach_item_by_name(cjson_node_t *object, const char *name);
void          json_delete_item_by_name(cjson_node_t *object, const char *name);
void          json_replace_item_by_name(cjson_node_t *object, const char *name, cjson_node_t *newitem);
cjson_node_t *json_create_null(void);
cjson_node_t *json_create_true(void);
cjson_node_t *json_create_false(void);
cjson_node_t *json_create_bool(int b);
cjson_node_t *json_create_number(double num);
cjson_node_t *json_create_string(const char *str);
cjson_node_t *json_create_array(void);
cjson_node_t *json_create_object(void);
cjson_node_t *json_create_int_array(const int *numbers, int count);
cjson_node_t *json_create_float_array(const float *numbers, int count);
cjson_node_t *json_create_double_array(const double *numbers, int count);
cjson_node_t *json_create_string_array(const char *const *strings, int count);
cjson_node_t *json_duplicate(cjson_node_t *item, int recurse);

/* ================= 实现 (按原地址序) ================= */

// === json_parse_string === 原 FUN_0000c244 @ 0x0000c244
// 来源: reversed/monitor_funcs/FUN_0000c244.c | 分类: monitor
// 语义: 解析 JSON 字符串字面量 (str 指向开引号 '"'): 解码 \b \f \n \r \t 及 \" \\ \/
//       等转义与 \uXXXX (含 UTF-16 代理对合成 → UTF-8 编码), 结果写入 item->valuestring
//       并置 type=JSON_STRING。返回越过闭引号后的解析位置; 非 '"' 开头返回 NULL 并把
//       str 记入全局错误指针。
char *json_parse_string(cjson_node_t *item, const char *str)
{
    char *out;              /* 解码输出缓冲 (malloc 钩子分配) */
    char *out2;             /* 输出写指针 */
    const char *p;          /* 源串读指针 */
    unsigned char c;
    char esc;               /* '\' 后的转义符 */
    size_t len;             /* 第一遍统计的缓冲上界 */
    uint32_t u;
    uint32_t u2;
    int nb;                 /* UTF-8 编码字节数 */

    if (*str != '\"') {
        DAT_0001b3a4 = str;              /* 记录错误位置 (cJSON ep) */
        return NULL;
    }

    /* ---- 第一遍: 统计缓冲上界 len ----
       原码为 for(;;){while(len++, c=='\\'){p+=2;...}} 结构: 每次内层 while 条件求值
       len+1。不变量: len >= 解码输出字节数 (\uXXXX 6 源字符计 5 >= 最大 4 字节 UTF-8)。 */
    len = 0;
    p = str + 1;
    for (;;) {
        c = (unsigned char)*p;
        if (c == '\0' || c == '\"') break;
        len++;                                        /* 内层条件首次求值 */
        while (c == '\\') {
            p += 2;                                   /* 跳过 \x 两个源字符 */
            c = (unsigned char)*p;
            if (c == '\0' || c == '\"') goto count_done;   /* 原标签 0000c2a0 */
            len++;                                    /* 内层条件再次求值 */
        }
        p++;                                          /* 原 for 增量 */
    }
count_done:

    out = (char *)(*DAT_0001b374)(len + 1);
    if (out == NULL) {
        return NULL;
    }

    /* ---- 第二遍: 解码转义序列 ---- */
    out2 = out;
    p = str + 1;
    c = (unsigned char)*p;
    while ((c != '\0') && (c != '\"')) {
        if (c != '\\') {
            *out2++ = (char)c;
            c = (unsigned char)*++p;
            continue;
        }
        esc = p[1];
        p += 2;                          /* 消费 '\' 与转义符 */
        if (esc == 'u') {
            u = parse_hex4(p);           /* 4 位十六进制 → 码点 */
            if ((u == 0) || (u >= 0xdc00 && u <= 0xdfff)) {
                /* NUL 或孤立低代理: 非法码点, 不输出, 仅跳过 \uXXXX */
                p += 4;
            } else if (u >= 0xd800 && u <= 0xdbff) {
                /* 高代理: 合法序列必须后随 "\uXXXX" 低代理 */
                if ((p[4] == '\\') && (p[5] == 'u')) {
                    u2 = parse_hex4(p + 6);
                    p += 10;             /* 消费第二个 \uXXXX */
                    if (u2 >= 0xdc00 && u2 <= 0xdfff) {
                        /* 合成增补平面码点: 0x10000 + (高10位 << 10 | 低10位) */
                        u = 0x10000 + ((u & 0x3ff) << 10) + (u2 & 0x3ff);
                        goto utf8_encode;             /* 原标签 0000c478 */
                    }
                } else {
                    p += 4;          /* 无续体: 跳过本 \uXXXX, 不输出 */
                }
            } else {
                goto utf8_encode;                 /* 原标签 0000c478 */
            }
            c = (unsigned char)*p;
            continue;
        }
        /* 简单转义: 还原为对应控制字符; 其余转义符原样保留 */
        switch (esc) {
        case 'b': *out2++ = '\b'; break;   /* 0x08 */
        case 'f': *out2++ = '\f'; break;   /* 0x0c */
        case 'n': *out2++ = '\n'; break;   /* 0x0a */
        case 'r': *out2++ = '\r'; break;   /* 0x0d */
        case 't': *out2++ = '\t'; break;   /* 0x09 */
        default:  *out2++ = esc;  break;   /* \" \\ \/ 等 */
        }
        c = (unsigned char)*p;
        continue;

utf8_encode:
        /* 按 UTF-8 编码 u (1-4 字节), 从尾字节向前写, 首字节带前缀位 */
        if (u < 0x80)         nb = 1;
        else if (u < 0x800)   nb = 2;
        else if (u < 0x10000) nb = 3;
        else                  nb = 4;
        if (nb >= 4) { out2[3] = (char)((u & 0x3f) | 0x80); u >>= 6; }
        if (nb >= 3) { out2[2] = (char)((u & 0x3f) | 0x80); u >>= 6; }
        if (nb >= 2) { out2[1] = (char)((u & 0x3f) | 0x80); u >>= 6; }
        *out2 = (char)((u & 0xff) | cjson_utf8_prefix[nb]);
        out2 += nb;
        c = (unsigned char)*p;
    }

    *out2 = '\0';
    item->valuestring = out;               /* +0x10 */
    if (*p == '\"') {
        p++;                               /* 越过闭引号 */
    }
    item->type = JSON_STRING;              /* +0x0c = 4 */
    return (char *)p;
}

// === json_new_item === 原 FUN_0000c540 @ 0x0000c540
// 来源: reversed/monitor_funcs/FUN_0000c540.c | 分类: monitor
// 语义: 分配并清零一个 cjson_node_t (0x28 字节); 失败返回 NULL。
cjson_node_t *json_new_item(void)
{
    cjson_node_t *node = (cjson_node_t *)(*DAT_0001b374)(0x28);   /* sizeof(cjson_node_t) */
    if (node != NULL) {
        memset(node, 0, 0x28);
    }
    return node;
}

// === json_strdup === 原 FUN_0000c574 @ 0x0000c574
// 来源: reversed/monitor_funcs/FUN_0000c574.c | 分类: monitor
// 语义: 经分配钩子复制字符串 (含 '\0'); 失败返回 NULL。
char *json_strdup(const char *str)
{
    size_t len = strlen(str);
    char *copy = (char *)(*DAT_0001b374)(len + 1);
    if (copy != NULL) {
        memcpy(copy, str, len + 1);
    }
    return copy;
}

// === json_create_reference === 原 FUN_0000c5b4 @ 0x0000c5b4
// 来源: reversed/monitor_funcs/FUN_0000c5b4.c | 分类: monitor
// 语义: 创建 item 的引用克隆: 整节点 0x28 字节 memcpy 后清 next/prev/string,
//       type |= 0x100 (IsReference)。child 指针随之被复制但不被清零 (原版行为,
//       引用节点因标志位不会释放它)。失败返回 NULL。
cjson_node_t *json_create_reference(cjson_node_t *item)
{
    cjson_node_t *ref = json_new_item();
    if (ref == NULL) {
        return NULL;
    }
    memcpy(ref, item, sizeof(cjson_node_t));   /* 逐字复制整个 0x28 节点 */
    ref->string = NULL;                        /* +0x20: 引用不拥有键名 */
    ref->prev   = NULL;                        /* +0x04 */
    ref->type  |= JSON_IS_REFERENCE;           /* +0x0c |= 0x100 */
    ref->next   = NULL;                        /* +0x00 */
    return ref;
}

// === json_strcasecmp === 原 FUN_0000c634 @ 0x0000c634
// 来源: reversed/monitor_funcs/FUN_0000c634.c | 分类: monitor
// 语义: 忽略大小写的 C 串比较 (键名匹配用)。NULL 处理: s1==NULL 时 s2!=NULL 返回 1,
//       两者皆 NULL 返回 0; s2==NULL 返回 1。比较经小写映射表 (原 __ctype_tolower_loc)。
int json_strcasecmp(const char *s1, const char *s2)
{
    if (s1 == NULL) {
        return (s2 != NULL) ? 1 : 0;
    }
    if (s2 == NULL) {
        return 1;
    }
    for (;;) {
        int c1 = tolower((unsigned char)*s1);
        int c2 = tolower((unsigned char)*s2);
        if (c1 != c2) {
            return c1 - c2;
        }
        if (*s1 == '\0') {
            return 0;
        }
        s1++;
        s2++;
    }
}

// === json_print_string === 原 FUN_0000c69c @ 0x0000c69c
// 来源: reversed/monitor_funcs/FUN_0000c69c.c | 分类: monitor
// 语义: 打印字符串为带引号 JSON 字面量: 转义 " \ \b \f \n \r \t, 其余 <0x20 控制字符
//       以 \u00xx 输出。str==NULL 时返回 strdup("") (原版行为, 不带引号)。
//       两遍算法: 先统计长度再拼接。失败返回 NULL。
char *json_print_string(const char *str)
{
    char *out;
    char *out2;             /* 输出写指针 */
    const char *p;
    unsigned char c;
    size_t len;

    if (str == NULL) {
        return json_strdup("");        /* NULL 串 → 空串 (无引号) */
    }

    /* 第一遍: 统计长度。特殊字符 +2 (\x), 控制字符 +6 (\u00xx), 其余 +1; 末尾 +3
       (两侧引号 + NUL)。特殊集合与写阶段一致: " \ \b \f \n \r \t */
    len = 0;
    p = str;
    for (c = (unsigned char)*p; c != '\0'; c = (unsigned char)*p) {
        if (strchr("\"\\\b\f\n\r\t", c) != NULL) {
            len += 2;
        } else {
            len += (c < 0x20) ? 6 : 1;
        }
        p++;
    }
    len += 3;

    out = (char *)(*DAT_0001b374)(len);
    if (out == NULL) {
        return NULL;
    }

    /* 第二遍: 拼输出 */
    out2 = out;
    p = str;
    *out2++ = '\"';
    for (;;) {
        c = (unsigned char)*p;
        if (c == '\0') {
            *out2 = '\"';
            out2[1] = '\0';
            return out;
        }
        p++;
        if (c >= 0x20 && c != '\"' && c != '\\') {
            /* 可直写字符: >=0x20 且非引号/反斜杠 (0x1f 被 >=0x20 自然排除) */
            *out2++ = (char)c;
            continue;
        }
        *out2++ = '\\';
        switch (c) {
        case '\f': *out2++ = 'f'; break;    /* 0x0c */
        case '\t': *out2++ = 't'; break;    /* 0x09 */
        case '\n': *out2++ = 'n'; break;    /* 0x0a */
        case '\b': *out2++ = 'b'; break;    /* 0x08 */
        case '\"': *out2++ = '\"'; break;   /* 0x22 */
        case '\\': *out2++ = '\\'; break;   /* 0x5c */
        case '\r': *out2++ = 'r'; break;    /* 0x0d */
        default:   /* \u00xx: 共 6 字符 (含已写反斜杠), sprintf 再写 5 字符 */
            sprintf(out2, "u%04x", c);
            out2 += 5;
            break;
        }
    }
}

// === json_print_value === 原 FUN_0000c83c @ 0x0000c83c
// 来源: reversed/monitor_funcs/FUN_0000c83c.c | 分类: monitor
// 语义: 按 type 低字节分派打印: false/true/null → strdup 字面量; number → 内联
//       print_number (%d / %.0f / %f / %e 四路); string → print_string;
//       array/object → 内联 print_array/print_object (两遍拼接, fmt 缩进)。
//       返回 malloc 的 JSON 文本; item/分配失败返回 NULL。
char *json_print_value(cjson_node_t *item, int depth, int fmt)
{
    char *out;
    char *ptr;
    cjson_node_t *n;
    char **entries;         /* 子项文本数组 */
    char **names;           /* 键名文本数组 (对象用) */
    char *ret;
    int count;              /* 子节点数 */
    int len;                /* 累计输出长度 */
    int i;
    int j;
    int fail;

    if (item == NULL) {
        return NULL;
    }

    switch ((unsigned char)item->type) {       /* 低字节 = 基础类型, 屏蔽 0x100 等标志 */
    case JSON_FALSE:
        return json_strdup("false");
    case JSON_TRUE:
        return json_strdup("true");            /* 原 DAT_00011934 */
    case JSON_NULL:
        return json_strdup("null");            /* 原 DAT_00011924 */

    case JSON_NUMBER: {
        /* 内联 print_number: 与整数值近似时输出 %d (21 字节缓冲),
           否则按浮点形态选 %.0f / %f / %e (64 字节缓冲) */
        double dInt = (double)item->valueint;              /* VCVT int→double */
        double d    = item->valuedouble;
        if ((fabs(dInt - d) > DBL_EPSILON) || (d > 2147483647.0) || (d < -2147483648.0)) {
            out = (char *)(*DAT_0001b374)(0x40);
            if (out != NULL) {
                double dFloor = floor(d);
                double dAbs   = fabs(d);
                if ((fabs(dFloor - d) <= DBL_EPSILON) && (dAbs < 1e60)) {
                    sprintf(out, "%.0f", d);               /* 整值大数 */
                    return out;
                }
                if ((1e-6 <= dAbs) && (dAbs <= 1e9)) {
                    sprintf(out, "%f", d);                 /* 常规范围 */
                    return out;
                }
                sprintf(out, "%e", d);                     /* 极小/极大 → 科学计数 */
                return out;
            }
        } else {
            out = (char *)(*DAT_0001b374)(0x15);           /* 21 字节: 容纳 "-2147483648" */
            if (out != NULL) {
                sprintf(out, "%d", item->valueint);
                return out;
            }
        }
        break;                                             /* 分配失败 → NULL */
    }

    case JSON_STRING:
        return json_print_string(item->valuestring);       /* +0x10 */

    case JSON_ARRAY: {
        /* 内联 print_array: "[e1,e2,...]", fmt 时逗号后加空格 */
        n = item->child;
        if (n == NULL) {
            out = (char *)(*DAT_0001b374)(3);              /* "[]" + NUL */
            if (out == NULL) {
                break;
            }
            out[0] = '[';
            out[1] = ']';
            out[2] = '\0';
            return out;
        }
        count = 0;
        for (; n != NULL; n = n->next) {
            count++;
        }
        entries = (char **)(*DAT_0001b374)(count * 4);     /* 字指针宽 4 (32 位目标) */
        if (entries == NULL) {
            break;
        }
        memset(entries, 0, count * 4);
        len = 5;                                           /* '[' ']' NUL + 逗号余量 */
        fail = 0;
        i = 0;
        for (n = item->child; n != NULL; n = n->next, i++) {
            ret = json_print_value(n, depth + 1, fmt);
            entries[i] = ret;
            if (ret == NULL) {
                fail = 1;                                  /* 原 0000ca30 清理入口 */
                break;
            }
            len += (int)strlen(ret) + (fmt ? 3 : 2);
        }
        if (!fail) {
            out = (char *)(*DAT_0001b374)(len);
            if (out == NULL) {
                fail = 1;
            }
        }
        if (fail) {
            for (i = 0; i < count; i++) {
                if (entries[i] != NULL) {
                    PTR_free_0001b378(entries[i]);
                }
            }
            PTR_free_0001b378(entries);
            return NULL;
        }
        /* 拼接 "[...]" */
        ptr = out;
        *ptr++ = '[';
        *ptr = '\0';
        for (i = 0; i < count; i++) {
            strcpy(ptr, entries[i]);
            ptr += strlen(entries[i]);
            if (i != count - 1) {
                *ptr++ = ',';
                if (fmt) {
                    *ptr++ = ' ';
                }
                *ptr = '\0';
            }
            PTR_free_0001b378(entries[i]);
        }
        PTR_free_0001b378(entries);
        *ptr++ = ']';
        *ptr = '\0';
        return out;
    }

    case JSON_OBJECT: {
        /* 内联 print_object: {"k":v,...}, fmt 时 '\t' 缩进 + 换行 */
        n = item->child;
        if (n == NULL) {
            /* 空对象: fmt 时 "{\n<(depth-1)缩进>}" */
            out = (char *)(*DAT_0001b374)(fmt ? depth + 4 : 3);
            if (out == NULL) {
                return NULL;
            }
            ptr = out;
            *ptr++ = '{';
            if (fmt) {
                *ptr++ = '\n';
                for (j = 0; j < depth - 1; j++) {
                    *ptr++ = '\t';
                }
            }
            *ptr = '}';
            ptr[1] = '\0';
            return out;
        }
        count = 0;
        for (; n != NULL; n = n->next) {
            count++;
        }
        entries = (char **)(*DAT_0001b374)(count * 4);
        if (entries == NULL) {
            return NULL;
        }
        names = (char **)(*DAT_0001b374)(count * 4);
        if (names == NULL) {
            PTR_free_0001b378(entries);
            return NULL;
        }
        memset(entries, 0, count * 4);
        memset(names, 0, count * 4);
        /* len 初值: 原式 len=7; depth++; if(fmt) len+=depth → 7 / depth+8 */
        len = fmt ? depth + 8 : 7;
        fail = 0;
        i = 0;
        for (n = item->child; n != NULL; n = n->next, i++) {
            names[i]   = json_print_string(n->string);          /* +0x20 键名 */
            entries[i] = json_print_value(n, depth + 1, fmt);
            if (names[i] == NULL || entries[i] == NULL) {
                fail = 1;                                       /* 原 0000cbcc 清理入口 */
                break;
            }
            len += (int)strlen(entries[i]) + (int)strlen(names[i])
                   + 2 + (fmt ? depth + 3 : 0);
        }
        if (!fail) {
            out = (char *)(*DAT_0001b374)(len);
            if (out == NULL) {
                fail = 1;
            }
        }
        if (fail) {
            for (i = 0; i < count; i++) {
                if (names[i] != NULL) {
                    PTR_free_0001b378(names[i]);
                }
                if (entries[i] != NULL) {
                    PTR_free_0001b378(entries[i]);
                }
            }
            PTR_free_0001b378(names);
            PTR_free_0001b378(entries);
            return NULL;
        }
        /* 拼接 "{...}" */
        ptr = out;
        *ptr++ = '{';
        if (fmt) {
            *ptr++ = '\n';
        }
        *ptr = '\0';
        for (i = 0; i < count; i++) {
            size_t klen;
            size_t vlen;
            if (fmt) {
                for (j = 0; j < depth + 1; j++) {   /* 条目缩进 = depth+1 */
                    *ptr++ = '\t';
                }
            }
            strcpy(ptr, names[i]);
            klen = strlen(names[i]);
            ptr[klen] = ':';
            if (fmt) {
                ptr[klen + 1] = '\t';
                ptr += klen + 2;
            } else {
                ptr += klen + 1;
            }
            strcpy(ptr, entries[i]);
            vlen = strlen(entries[i]);
            ptr += vlen;
            if (i != count - 1) {
                *ptr++ = ',';
            }
            if (fmt) {
                *ptr++ = '\n';
            }
            *ptr = '\0';
            PTR_free_0001b378(names[i]);
            PTR_free_0001b378(entries[i]);
        }
        PTR_free_0001b378(names);
        PTR_free_0001b378(entries);
        if (fmt && depth > 0) {
            for (j = 0; j < depth; j++) {           /* 闭括号缩进 = depth */
                *ptr++ = '\t';
            }
        }
        *ptr = '}';
        ptr[1] = '\0';
        return out;
    }

    default:
        break;
    }
    return NULL;
}

// === json_parse_value === 原 FUN_0000d05c @ 0x0000d05c
// 来源: reversed/monitor_funcs/FUN_0000d05c.c | 分类: monitor
// 语义: 递归下降解析单个 JSON 值: null/false/true/字符串/数字/[数组]/{对象}。
//       item 为预分配节点, 成功时置 type(数字还置 valueint/valuedouble)并返回越过
//       该值之后的指针; 失败把位置记入 DAT_0001b3a4 并返回 NULL。
//       注: 反编译中的 p==NULL 越界判空为 Ghidra 伪代码, 已略去。
const char *json_parse_value(cjson_node_t *item, const char *value)
{
    const char *p;
    cjson_node_t *child;
    cjson_node_t *next;
    unsigned char c;

    if (value == NULL) {
        return NULL;
    }
    c = (unsigned char)*value;

    if (strncmp(value, "null", 4) == 0) {
        item->type = JSON_NULL;                        /* 2 */
        return value + 4;
    }
    if (strncmp(value, "false", 5) == 0) {
        item->type = JSON_FALSE;                       /* 0 */
        return value + 5;
    }
    if (strncmp(value, "true", 4) == 0) {
        item->type = JSON_TRUE;                        /* 1 */
        item->valueint = 1;                            /* +0x14 */
        return value + 4;
    }
    if (c == '\"') {
        return json_parse_string(item, value);
    }

    if ((c >= '0' && c <= '9') || (c == '-')) {
        /* 内联 parse_number: 文法 <int><frac><exp>, n*pow(10, scale+exp) 合成 */
        double n = 0.0;
        double sign = 1.0;
        double scale = 0.0;                /* 小数位数 (负值) */
        int expval = 0;                    /* signsubscale * subscale */
        p = value;
        if (*p == '-') {
            sign = -1.0;
            p++;
        }
        if (*p == '0') {
            p++;                           /* 前导 0 只消费一位 */
        }
        if (*p >= '1' && *p <= '9') {
            do {
                p++;
                n = n * 10.0 + (double)(p[-1] - '0');
            } while (*p >= '0' && *p <= '9');
        }
        if (*p == '.') {
            if (p[1] < '0' || p[1] > '9') {
                /* '.' 后无数字: 指数一并跳过, p 停在 '.' (原跳转 0000d280) */
                goto finish_number;
            }
            p++;                           /* 越过 '.' */
            do {
                scale -= 1.0;
                p++;
                n = n * 10.0 + (double)(p[-1] - '0');
            } while (*p >= '0' && *p <= '9');
        }
        if ((*p == 'e') || (*p == 'E')) {
            int esign = 1;
            if (p[1] == '+') {
                p += 2;
            } else if (p[1] == '-') {
                p += 2;
                esign = -1;
            } else {
                p += 1;                    /* 无符号指数也消费 'e' */
            }
            if (*p >= '0' && *p <= '9') {
                int ev = 0;
                do {
                    p++;
                    ev = ev * 10 + (p[-1] - '0');
                } while (*p >= '0' && *p <= '9');
                expval = esign * ev;
            }
        }
finish_number:
        item->type = JSON_NUMBER;                      /* 3 */
        {
            double d = sign * n * pow(10.0, scale + (double)expval);
            item->valuedouble = d;                     /* +0x18 */
            item->valueint = (int)d;                   /* +0x14, 原 VCVT double→int */
        }
        return p;
    }

    if (c == '[') {
        /* 内联 parse_array: '[' value (',' value)* ']' */
        item->type = JSON_ARRAY;                       /* 5 */
        p = value + 1;
        while (*p != '\0' && (unsigned char)*p <= 0x20) p++;    /* 跳过空白 (0x01-0x20) */
        if (*p == ']') {
            return p + 1;                              /* 空数组 */
        }
        child = json_new_item();
        item->child = child;                           /* +0x08 */
        if (child == NULL) {
            return NULL;
        }
        while (*p != '\0' && (unsigned char)*p <= 0x20) p++;    /* 原码此处重复 skip */
        p = json_parse_value(child, p);
        if (p == NULL) {
            return NULL;
        }
        while (*p != '\0' && (unsigned char)*p <= 0x20) p++;
        while (*p == ',') {
            next = json_new_item();
            if (next == NULL) {
                return NULL;
            }
            child->next = next;                        /* +0x00 */
            next->prev = child;                        /* +0x04 */
            p++;                                       /* 越过 ',' */
            while (*p != '\0' && (unsigned char)*p <= 0x20) p++;
            p = json_parse_value(next, p);
            if (p == NULL) {
                return NULL;
            }
            while (*p != '\0' && (unsigned char)*p <= 0x20) p++;
            child = next;
        }
        if (*p != ']') {
            DAT_0001b3a4 = p;                          /* 格式错误: 记录位置 */
            return NULL;
        }
        return p + 1;
    }

    if (c != '{') {
        DAT_0001b3a4 = value;                          /* 无法识别的值 */
        return NULL;
    }

    /* 内联 parse_object: '{' string ':' value (',' string ':' value)* '}' */
    item->type = JSON_OBJECT;                          /* 6 */
    p = value + 1;
    while (*p != '\0' && (unsigned char)*p <= 0x20) p++;
    if (*p == '}') {
        return p + 1;                                  /* 空对象 */
    }
    child = json_new_item();
    item->child = child;
    if (child == NULL) {
        return NULL;
    }
    while (*p != '\0' && (unsigned char)*p <= 0x20) p++;
    p = json_parse_string(child, p);                   /* 先解析键名 */
    if (p == NULL) {
        return NULL;
    }
    while (*p != '\0' && (unsigned char)*p <= 0x20) p++;
    /* 键名暂存于 valuestring, 挪到 string 字段 (+0x20), valuestring 清空 */
    child->string = child->valuestring;
    child->valuestring = NULL;
    if (*p != ':') {
        DAT_0001b3a4 = p;
        return NULL;
    }
    p++;                                               /* 越过 ':' */
    while (*p != '\0' && (unsigned char)*p <= 0x20) p++;
    p = json_parse_value(child, p);                    /* 复用同一节点存值 */
    if (p == NULL) {
        return NULL;
    }
    while (*p != '\0' && (unsigned char)*p <= 0x20) p++;
    while (*p == ',') {
        next = json_new_item();
        if (next == NULL) {
            return NULL;
        }
        child->next = next;
        next->prev = child;
        p++;                                           /* 越过 ',' */
        while (*p != '\0' && (unsigned char)*p <= 0x20) p++;
        p = json_parse_string(next, p);
        if (p == NULL) {
            return NULL;
        }
        while (*p != '\0' && (unsigned char)*p <= 0x20) p++;
        next->string = next->valuestring;
        next->valuestring = NULL;
        if (*p != ':') {
            DAT_0001b3a4 = p;
            return NULL;
        }
        p++;
        while (*p != '\0' && (unsigned char)*p <= 0x20) p++;
        p = json_parse_value(next, p);
        if (p == NULL) {
            return NULL;
        }
        while (*p != '\0' && (unsigned char)*p <= 0x20) p++;
        child = next;
    }
    if (*p != '}') {
        DAT_0001b3a4 = p;
        return NULL;
    }
    return p + 1;
}

// === json_delete === 原 FUN_0000d864 @ 0x0000d864
// 来源: reversed/monitor_funcs/FUN_0000d864.c | 分类: monitor
// 语义: 释放整棵兄弟链 (沿 next 迭代, child 递归)。非引用节点释放 child 子树与
//       valuestring; string 键名一律释放 (原码此处不检查 IsReference, 引用节点因
//       create_reference 已置 string=NULL 而安全); 最后释放节点本身。
void json_delete(cjson_node_t *item)
{
    while (item != NULL) {
        cjson_node_t *next = item->next;
        if ((item->type & JSON_IS_REFERENCE) == 0) {
            if (item->child != NULL) {
                json_delete(item->child);
            }
            /* 反编译在递归后重读一次 type 标志; 递归不修改本节点, 故合并为同一判断 */
            if (item->valuestring != NULL) {
                PTR_free_0001b378(item->valuestring);
            }
        }
        if (item->string != NULL) {                    /* +0x20, 不检查引用标志 */
            PTR_free_0001b378(item->string);
        }
        PTR_free_0001b378(item);
        item = next;
    }
}

// === json_parse === 原 FUN_0000d9d8 @ 0x0000d9d8
// 来源: reversed/monitor_funcs/FUN_0000d9d8.c | 分类: monitor
// 语义: cJSON_Parse 入口: 新建节点, 清全局错误指针, 跳过前导空白后解析整值。
//       解析失败 (含 value==NULL) 释放节点并返回 NULL。裁剪的 ParseWithOpts 残块
//       见反编译 unreachable 注记, 运行语义与此一致。
cjson_node_t *json_parse(const char *value)
{
    cjson_node_t *node = json_new_item();
    DAT_0001b3a4 = NULL;                   /* ep 清零 */
    if (node == NULL) {
        return NULL;
    }
    if (value != NULL && *value != '\0') {
        while (*value != '\0' && (unsigned char)*value <= 0x20) {
            value++;                       /* 跳过前导空白 */
        }
    }
    if (json_parse_value(node, value) == NULL) {
        json_delete(node);
        return NULL;
    }
    return node;
}

// === json_print === 原 FUN_0000d9e4 @ 0x0000d9e4
// 来源: reversed/monitor_funcs/FUN_0000d9e4.c | 分类: monitor
// 语义: cJSON_Print 入口: 以 fmt=1 (缩进)、depth=0 打印整树。调用点 (fa54/fc84)
//       使用其返回值, Ghidra 原型误标为 void, 实际 r0 即文本指针。
char *json_print(cjson_node_t *item)
{
    return json_print_value(item, 0, 1);
}

// === json_get_array_size === 原 FUN_0000d9fc @ 0x0000d9fc
// 来源: reversed/monitor_funcs/FUN_0000d9fc.c | 分类: monitor
// 语义: 统计数组/对象孩子数 (沿 child->next 链)。无孩子返回 0。
int json_get_array_size(const cjson_node_t *array)
{
    const cjson_node_t *c;
    int count = 0;
    if (array->child == NULL) {
        return 0;
    }
    for (c = array->child; c != NULL; c = c->next) {
        count++;
    }
    return count;
}

// === json_get_array_item === 原 FUN_0000da28 @ 0x0000da28
// 来源: reversed/monitor_funcs/FUN_0000da28.c | 分类: monitor
// 语义: 取数组第 index 个孩子 (0 基)。index<0 或越界时原码返回首个孩子/NULL:
//       while (c && index>0) 行走后直接返回 c。
cjson_node_t *json_get_array_item(cjson_node_t *array, int index)
{
    cjson_node_t *c = array->child;
    while ((c != NULL) && (index > 0)) {
        index--;
        c = c->next;
    }
    return c;
}

// === json_get_object_item === 原 FUN_0000da50 @ 0x0000da50
// 来源: reversed/monitor_funcs/FUN_0000da50.c | 分类: monitor
// 语义: 按键名 (忽略大小写) 查找对象孩子; 未命中返回 NULL。
cjson_node_t *json_get_object_item(cjson_node_t *object, const char *name)
{
    cjson_node_t *c = object->child;
    while ((c != NULL) && (json_strcasecmp(c->string, name) != 0)) {
        c = c->next;
    }
    return c;
}

// === json_add_item_to_array === 原 FUN_0000da90 @ 0x0000da90
// 来源: reversed/monitor_funcs/FUN_0000da90.c | 分类: monitor
// 语义: 把 item 追加到 array 孩子链尾。首孩子直接挂 child; 否则走到尾节点接上,
//       并置 item->prev=尾节点 (item->next 不清零, 原版如此)。
void json_add_item_to_array(cjson_node_t *array, cjson_node_t *item)
{
    cjson_node_t *c;
    if (item == NULL) {
        return;
    }
    c = array->child;
    if (c == NULL) {
        array->child = item;
        return;
    }
    while (c->next != NULL) {
        c = c->next;
    }
    c->next = item;
    item->prev = c;
}

// === json_add_item_to_object === 原 FUN_0000dac8 @ 0x0000dac8
// 来源: reversed/monitor_funcs/FUN_0000dac8.c | 分类: monitor
// 语义: 为 item 设置键名 (先释放旧键名, 再 strdup 新名) 并追加到 object 孩子链尾
//       (追加逻辑与 json_add_item_to_array 相同, 原码内联)。
void json_add_item_to_object(cjson_node_t *object, const char *name, cjson_node_t *item)
{
    cjson_node_t *c;
    if (item == NULL) {
        return;
    }
    if (item->string != NULL) {                /* +0x20 旧键名 */
        PTR_free_0001b378(item->string);
    }
    item->string = json_strdup(name);
    c = object->child;
    if (c == NULL) {
        object->child = item;
        return;
    }
    while (c->next != NULL) {
        c = c->next;
    }
    c->next = item;
    item->prev = c;
}

// === json_add_item_reference === 原 FUN_0000db34 @ 0x0000db34
// 来源: reversed/monitor_funcs/FUN_0000db34.c | 分类: monitor
// 语义: 创建 item 的引用克隆并追加到 array 链尾 (追加逻辑内联)。
void json_add_item_reference(cjson_node_t *array, cjson_node_t *item)
{
    cjson_node_t *ref = json_create_reference(item);
    cjson_node_t *c;
    if (ref == NULL) {
        return;
    }
    c = array->child;
    if (c == NULL) {
        array->child = ref;
        return;
    }
    while (c->next != NULL) {
        c = c->next;
    }
    c->next = ref;
    ref->prev = c;
}

// === json_add_ref_to_object === 原 FUN_0000db7c @ 0x0000db7c
// 来源: reversed/monitor_funcs/FUN_0000db7c.c | 分类: monitor
// 语义: 创建 target 的引用克隆并以键名 name 挂入 object (AddItemReferenceToObject)。
void json_add_ref_to_object(cjson_node_t *object, const char *name, cjson_node_t *target)
{
    json_add_item_to_object(object, name, json_create_reference(target));
}

// === json_detach_item_at === 原 FUN_0000dba4 @ 0x0000dba4
// 来源: reversed/monitor_funcs/FUN_0000dba4.c | 分类: monitor
// 语义: 从数组链上摘下第 index 个孩子 (双链摘除, 不释放): 修补前后驱指针, 若为首
//       孩子则更新 child, 清空被摘节点的 next/prev。返回被摘节点; 越界返回 NULL。
cjson_node_t *json_detach_item_at(cjson_node_t *array, int index)
{
    cjson_node_t *c = array->child;
    while ((c != NULL) && (index > 0)) {
        index--;
        c = c->next;
    }
    if (c == NULL) {
        return NULL;
    }
    if (c->prev != NULL) {
        c->prev->next = c->next;
    }
    if (c->next != NULL) {
        c->next->prev = c->prev;
    }
    if (c == array->child) {
        array->child = c->next;
    }
    c->next = NULL;
    c->prev = NULL;
    return c;
}

// === json_delete_item_at === 原 FUN_0000dc0c @ 0x0000dc0c
// 来源: reversed/monitor_funcs/FUN_0000dc0c.c | 分类: monitor
// 语义: 摘下第 index 个孩子并整树释放 (DeleteItemFromArray)。
void json_delete_item_at(cjson_node_t *array, int index)
{
    json_delete(json_detach_item_at(array, index));
}

// === json_detach_item_by_name === 原 FUN_0000dc1c @ 0x0000dc1c
// 来源: reversed/monitor_funcs/FUN_0000dc1c.c | 分类: monitor
// 语义: 按键名查找并从对象上摘下该孩子 (记录序号后调 detach_item_at)。
//       未命中返回 NULL。
cjson_node_t *json_detach_item_by_name(cjson_node_t *object, const char *name)
{
    cjson_node_t *c = object->child;
    int index = 0;
    while (c != NULL) {
        if (json_strcasecmp(c->string, name) == 0) {
            return json_detach_item_at(object, index);
        }
        c = c->next;
        index++;
    }
    return NULL;
}

// === json_delete_item_by_name === 原 FUN_0000dc78 @ 0x0000dc78
// 来源: reversed/monitor_funcs/FUN_0000dc78.c | 分类: monitor
// 语义: 按键名摘下并整树释放 (DeleteItemFromObject)。
void json_delete_item_by_name(cjson_node_t *object, const char *name)
{
    json_delete(json_detach_item_by_name(object, name));
}

// === json_replace_item_by_name === 原 FUN_0000dcfc @ 0x0000dcfc
// 来源: reversed/monitor_funcs/FUN_0000dcfc.c | 分类: monitor
// 语义: 按键名定位旧孩子后用 newitem 原位替换: 先 strdup 新键名到 newitem->string
//       (原码不释放旧值), 再按序号重走链定位, 修补双链指针, 旧节点清链后整树删除。
void json_replace_item_by_name(cjson_node_t *object, const char *name, cjson_node_t *newitem)
{
    cjson_node_t *c = object->child;
    int index = 0;
    int w;

    if (c == NULL) {
        return;
    }
    while (json_strcasecmp(c->string, name) != 0) {
        c = c->next;
        index++;
        if (c == NULL) {
            return;                            /* 未命中 */
        }
    }
    newitem->string = json_strdup(name);       /* +0x20, 原码不释放旧键名 */
    /* 按序号重新定位被替换节点 (原 ReplaceItemInArray 内联) */
    c = object->child;
    w = index;
    while ((c != NULL) && (w > 0)) {
        w--;
        c = c->next;
    }
    if (c == NULL) {
        return;
    }
    newitem->next = c->next;
    newitem->prev = c->prev;
    if (newitem->next != NULL) {
        newitem->next->prev = newitem;
    }
    if (c == object->child) {
        object->child = newitem;
    } else {
        newitem->prev->next = newitem;         /* 原码此处不判空 */
    }
    c->prev = NULL;
    c->next = NULL;
    json_delete(c);
}

// === json_create_null === 原 FUN_0000dd68 @ 0x0000dd68
// 来源: reversed/monitor_funcs/FUN_0000dd68.c | 分类: monitor
// 语义: 新建 type=JSON_NULL(2) 节点; 调用点使用 r0 返回值。
cjson_node_t *json_create_null(void)
{
    cjson_node_t *node = json_new_item();
    if (node != NULL) {
        node->type = JSON_NULL;
    }
    return node;
}

// === json_create_true === 原 FUN_0000dd80 @ 0x0000dd80
// 来源: reversed/monitor_funcs/FUN_0000dd80.c | 分类: monitor
// 语义: 新建 type=JSON_TRUE(1) 节点 (valueint 不置位, 打印走 strdup("true"))。
cjson_node_t *json_create_true(void)
{
    cjson_node_t *node = json_new_item();
    if (node != NULL) {
        node->type = JSON_TRUE;
    }
    return node;
}

// === json_create_false === 原 FUN_0000dd98 @ 0x0000dd98
// 来源: reversed/monitor_funcs/FUN_0000dd98.c | 分类: monitor
// 语义: 新建 type=JSON_FALSE(0) 节点。
cjson_node_t *json_create_false(void)
{
    cjson_node_t *node = json_new_item();
    if (node != NULL) {
        node->type = JSON_FALSE;
    }
    return node;
}

// === json_create_bool === 原 FUN_0000ddb0 @ 0x0000ddb0
// 来源: reversed/monitor_funcs/FUN_0000ddb0.c | 分类: monitor
// 语义: 新建布尔节点: type = (b != 0) ? JSON_TRUE : JSON_FALSE。
cjson_node_t *json_create_bool(int b)
{
    cjson_node_t *node = json_new_item();
    if (node != NULL) {
        node->type = (b != 0) ? JSON_TRUE : JSON_FALSE;
    }
    return node;
}

// === json_create_number === 原 FUN_0000ddd4 @ 0x0000ddd4
// 来源: reversed/monitor_funcs/FUN_0000ddd4.c | 分类: monitor
// 语义: 新建数值节点: valuedouble=num, valueint=(int)num (VCVT 截断), type=NUMBER。
//       原 Ghidra 原型缺参/缺返回值 (双精度走 d0 寄存器), 调用点使用 r0。
cjson_node_t *json_create_number(double num)
{
    cjson_node_t *node = json_new_item();
    if (node != NULL) {
        node->valuedouble = num;               /* +0x18 */
        node->valueint = (int)num;             /* +0x14 */
        node->type = JSON_NUMBER;              /* +0x0c = 3 */
    }
    return node;
}

// === json_create_string === 原 FUN_0000de04 @ 0x0000de04
// 来源: reversed/monitor_funcs/FUN_0000de04.c | 分类: monitor
// 语义: 新建字符串节点: type=STRING(4), valuestring=strdup(str)。
cjson_node_t *json_create_string(const char *str)
{
    cjson_node_t *node = json_new_item();
    if (node != NULL) {
        node->type = JSON_STRING;
        node->valuestring = json_strdup(str);
    }
    return node;
}

// === json_create_array === 原 FUN_0000de34 @ 0x0000de34
// 来源: reversed/monitor_funcs/FUN_0000de34.c | 分类: monitor
// 语义: 新建空数组节点 (type=ARRAY(5)); 调用点 (10210/fa54) 使用 r0 返回值。
cjson_node_t *json_create_array(void)
{
    cjson_node_t *node = json_new_item();
    if (node != NULL) {
        node->type = JSON_ARRAY;
    }
    return node;
}

// === json_create_object === 原 FUN_0000de4c @ 0x0000de4c
// 来源: reversed/monitor_funcs/FUN_0000de4c.c | 分类: monitor
// 语义: 新建空对象节点 (type=OBJECT(6)); 调用点 (fa54/fc84) 使用 r0 返回值。
cjson_node_t *json_create_object(void)
{
    cjson_node_t *node = json_new_item();
    if (node != NULL) {
        node->type = JSON_OBJECT;
    }
    return node;
}

// === json_create_int_array === 原 FUN_0000de64 @ 0x0000de64
// 来源: reversed/monitor_funcs/FUN_0000de64.c | 分类: monitor
// 语义: 新建数组并逐元素 create_number((double)numbers[i]) 头插串联成孩子链。
//       count<=0 或建数组失败时直接返回数组节点 (元素循环不执行)。
cjson_node_t *json_create_int_array(const int *numbers, int count)
{
    cjson_node_t *array = json_create_array();
    cjson_node_t *n;
    cjson_node_t *prev = NULL;
    int i;

    if (array == NULL || count <= 0) {
        return array;
    }
    for (i = 0; i < count; i++) {
        n = json_create_number((double)numbers[i]);
        if (i == 0) {
            array->child = n;                  /* 首元素挂 child */
        } else {
            prev->next = n;                    /* 原码不判 n 空 */
            n->prev = prev;
        }
        prev = n;
    }
    return array;
}

// === json_create_float_array === 原 FUN_0000dec8 @ 0x0000dec8
// 来源: reversed/monitor_funcs/FUN_0000dec8.c | 分类: monitor
// 语义: 同 int 版, 元素经 float→double 提升后 create_number。
cjson_node_t *json_create_float_array(const float *numbers, int count)
{
    cjson_node_t *array = json_create_array();
    cjson_node_t *n;
    cjson_node_t *prev = NULL;
    int i;

    if (array == NULL || count <= 0) {
        return array;
    }
    for (i = 0; i < count; i++) {
        n = json_create_number((double)numbers[i]);
        if (i == 0) {
            array->child = n;
        } else {
            prev->next = n;
            n->prev = prev;
        }
        prev = n;
    }
    return array;
}

// === json_create_double_array === 原 FUN_0000df24 @ 0x0000df24
// 来源: reversed/monitor_funcs/FUN_0000df24.c | 分类: monitor
// 语义: 同 int 版, 元素为 double 直传 create_number。
cjson_node_t *json_create_double_array(const double *numbers, int count)
{
    cjson_node_t *array = json_create_array();
    cjson_node_t *n;
    cjson_node_t *prev = NULL;
    int i;

    if (array == NULL || count <= 0) {
        return array;
    }
    for (i = 0; i < count; i++) {
        n = json_create_number(numbers[i]);
        if (i == 0) {
            array->child = n;
        } else {
            prev->next = n;
            n->prev = prev;
        }
        prev = n;
    }
    return array;
}

// === json_create_string_array === 原 FUN_0000df7c @ 0x0000df7c
// 来源: reversed/monitor_funcs/FUN_0000df7c.c | 分类: monitor
// 语义: 新建数组并逐元素 create_string(strings[i]) 串联成孩子链。
cjson_node_t *json_create_string_array(const char *const *strings, int count)
{
    cjson_node_t *array = json_create_array();
    cjson_node_t *n;
    cjson_node_t *prev = NULL;
    int i;

    if (array == NULL || count <= 0) {
        return array;
    }
    for (i = 0; i < count; i++) {
        n = json_create_string(strings[i]);
        if (i == 0) {
            array->child = n;
        } else {
            prev->next = n;
            n->prev = prev;
        }
        prev = n;
    }
    return array;
}

// === json_duplicate === 原 FUN_0000dfd8 @ 0x0000dfd8
// 来源: reversed/monitor_funcs/FUN_0000dfd8.c | 分类: monitor
// 语义: 深拷贝节点: 复制 valuedouble/valueint, type 清 0x100 引用标志; valuestring
//       与 string 非空则各自 strdup (失败走统一清理); recurse 非 0 时递归复制整条
//       孩子链。任一步失败释放新节点并返回 NULL。
cjson_node_t *json_duplicate(cjson_node_t *item, int recurse)
{
    cjson_node_t *newitem;
    cjson_node_t *cptr;
    cjson_node_t *nptr;
    cjson_node_t *newchild;

    if (item == NULL) {
        return NULL;
    }
    newitem = json_new_item();
    if (newitem == NULL) {
        return NULL;
    }
    newitem->valuedouble = item->valuedouble;              /* +0x18 (双字复制) */
    newitem->valueint = item->valueint;                    /* +0x14 */
    newitem->type = item->type & ~JSON_IS_REFERENCE;       /* 0xfffffeff */
    if (item->valuestring != NULL) {                       /* +0x10 */
        newitem->valuestring = json_strdup(item->valuestring);
        if (newitem->valuestring == NULL) {
            goto dup_fail;                                 /* 原标签 0000e0a0 */
        }
    }
    if (item->string != NULL) {                            /* +0x20 */
        newitem->string = json_strdup(item->string);
        if (newitem->string == NULL) {
            goto dup_fail;
        }
    }
    if (recurse != 0) {
        cptr = item->child;
        nptr = NULL;
        while (cptr != NULL) {
            newchild = json_duplicate(cptr, 1);
            if (newchild == NULL) {
                goto dup_fail;
            }
            if (nptr != NULL) {
                nptr->next = newchild;
                newchild->prev = nptr;
            } else {
                newitem->child = newchild;
            }
            nptr = newchild;
            cptr = cptr->next;
        }
    }
    return newitem;

dup_fail:                                                  /* 原标签 0000e0a0 */
    json_delete(newitem);
    return NULL;
}

/*
 * ================= 覆盖率表 (36/36) =================
 * 原 FUN 地址    新函数名                  反编译行数
 * ---------------------------------------------
 * FUN_0000c244  json_parse_string         150
 * FUN_0000c540  json_new_item              14
 * FUN_0000c574  json_strdup                16
 * FUN_0000c5b4  json_create_reference      43
 * FUN_0000c634  json_strcasecmp            32
 * FUN_0000c69c  json_print_string         112
 * FUN_0000c83c  json_print_value          358
 * FUN_0000d05c  json_parse_value          358
 * FUN_0000d864  json_delete                25
 * FUN_0000d9d8  json_parse                 35
 * FUN_0000d9e4  json_print                  9
 * FUN_0000d9fc  json_get_array_size        20
 * FUN_0000da28  json_get_array_item        27
 * FUN_0000da50  json_get_object_item       15
 * FUN_0000da90  json_add_item_to_array     23
 * FUN_0000dac8  json_add_item_to_object    31
 * FUN_0000db34  json_add_item_reference    25
 * FUN_0000db7c  json_add_ref_to_object     12
 * FUN_0000dba4  json_detach_item_at        44
 * FUN_0000dc0c  json_delete_item_at        10
 * FUN_0000dc1c  json_detach_item_by_name   27
 * FUN_0000dc78  json_delete_item_by_name   10
 * FUN_0000dcfc  json_replace_item_by_name  62
 * FUN_0000dd68  json_create_null           14
 * FUN_0000dd80  json_create_true           14
 * FUN_0000dd98  json_create_false          14
 * FUN_0000ddb0  json_create_bool           15
 * FUN_0000ddd4  json_create_number         19
 * FUN_0000de04  json_create_string         17
 * FUN_0000de34  json_create_array          14
 * FUN_0000de4c  json_create_object         14
 * FUN_0000de64  json_create_int_array      42
 * FUN_0000dec8  json_create_float_array    39
 * FUN_0000df24  json_create_double_array   39
 * FUN_0000df7c  json_create_string_array   39
 * FUN_0000dfd8  json_duplicate             62
 *
 * 外部依赖: parse_hex4 = FUN_0000c130 (定义于 monitor/monitor_core_p1.c);
 *           DAT_0001b374 / PTR_free_0001b378 / DAT_0001b3a4 为 monitor 模块全局。
 */
