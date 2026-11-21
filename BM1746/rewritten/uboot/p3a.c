// ============================================================================
// uboot/p3a.c — U-Boot 2016.07 反编译转写 (半批 uboot_b3 前 24 函数)
// 地址范围: 0x040028a8 .. 0x04007718
// 内容: SHA256 增量更新 / 哈希表 (查找·插入·导出·导入) / 正则引擎核心 /
//       字符串与内存函数 / 计时器 / printf / simple_strtoul
// 命名事实源: rewritten/name_map/uboot.tsv
// 语义规则: rewritten/CONVENTIONS.md
// ============================================================================

#include <stdint.h>
#include <stdarg.h>

/* ----------------------------------------------------------------------------
 * 全局数据与外部占位 (Ghidra 数据段/低地址区, 语义待定; 命名事实源 name_map/uboot.tsv)
 * -------------------------------------------------------------------------- */

/* 哈希表各函数各自的错误码槽 (值与 errno 对应: 1=EPERM 3=ESRCH 12=ENOMEM 22=EINVAL)
 * 全部为指针变量: 反编译写入形如 *DAT_xxx = errno, 即先取槽内指针再解引用写错误码 */
extern uint32_t *DAT_040034a4;  /* ub_htab_find_entry 错误码槽 */
extern uint32_t *DAT_04003950;  /* ub_hsearch        错误码槽 */
extern uint32_t *DAT_04003d88;  /* ub_hexport        错误码槽 */
extern uint32_t *DAT_04004214;  /* ub_himport        错误码槽 */

extern uint32_t DAT_04003018;   /* 传给 func_0x03ffafa8 的全局参数 (语义待定) */

/* ub_hexport 用: qsort 比较函数指针 / "缓冲区过小" 格式串 */
extern uint32_t DAT_04003d80;   /* 排序比较函数指针 (语义待定) */
extern uint32_t DAT_04003d84;   /* printf 格式串 (语义待定) */

/* ub_print_u64 用: 后缀表与 printf 格式串 */
extern uint32_t DAT_040043dc;   /* 完整 64 位十进制打印格式串 (语义待定) */
extern uint32_t DAT_040043e0;   /* 容量后缀表 "EPTGMK" 类 (语义待定) */
extern uint32_t DAT_040043e4;   /* 整数部分格式串 (语义待定) */
extern uint32_t DAT_040043e8;   /* 后缀+附加串格式串 (语义待定) */
extern uint32_t DAT_040043ec;   /* 小数部分格式串 (语义待定) */

/* 正则程序缓冲溢出错误标记 (语义待定) */
extern uint32_t DAT_04004af0;
extern uint32_t DAT_04004bcc;
extern uint32_t DAT_04004bd0;

/* ub_himport 用: "删除成功"/"删除失败" 提示串, hsearch 失败提示串 */
extern uint32_t DAT_04004218;
extern uint32_t DAT_0400421c;
extern uint32_t DAT_04004220;

/* 跨文件: SHA-256 压缩函数 (p1a.c, FUN_04000090) */
void ub_sha256_transform(uint32_t *ctx, const uint8_t *block);

/* simple_strtoul 字符分类表 (U-Boot _ctype 布局: bit2=数字, bit1=小写字母, bit6=十六进制) */
extern uint8_t DAT_04007800[];

/* name_map 未收录的低地址区被调函数 (按反编译 func_0x 占位, 命名待补) */
extern int      func_0x03ffae84(void);                            /* 无参, 布尔判定 */
extern int      func_0x03ffafa8(uint32_t, uint32_t);              /* 返回 <0 才有意义 */
extern void     func_0x03fc3288(void *);                          /* 新条目插入后调用 */
extern void     func_0x03fc35fc(void *);                          /* 新条目插入后调用 */
extern void    *func_0x03fc87b0(uint32_t);                        /* malloc 型分配 */
extern void     func_0x03fc8570(void *);                          /* free 型释放 */
extern void    *func_0x03fc8dc4(uint32_t, uint32_t);              /* calloc 型分配 */
extern void     func_0x03ffd878(void *, uint32_t, uint32_t, uint32_t);  /* qsort 型排序 */
extern uint32_t func_0x03fb2008(void);                            /* 时钟频率查询 */
extern void     func_0x03fc7fbc(const char *);                    /* puts 型输出 */

/* ----------------------------------------------------------------------------
 * 哈希表布局 (U-Boot lib/hashtable.c 变体; 各偏移由 hsearch/hdelete_entry/
 * hexport/himport 反编译交叉证实)
 * -------------------------------------------------------------------------- */
typedef struct ub_entry {
    int32_t used;        /* +0x00: 0=空槽; 0xffffffff=删除标记; 活跃桶存其哈希落桶索引 */
    char   *key;         /* +0x04 */
    char   *data;        /* +0x08 */
    /* +0x0c: 条目级回调; op 观测值: 0=新增 1=删除 2=修改 */
    int32_t (*callback)(const char *key, char *value, int op, int flag);
    int32_t priv;        /* +0x10: 仅在删除时被清零 (ub_hdelete_entry), 其余路径未引用 */
} ub_entry_t;

typedef struct ub_hsearch_data {
    ub_entry_t *table;   /* +0x00: 条目数组, 步长 0x14; 槽 0 不参与探测, table[size] 为 used=0 哨兵 */
    uint32_t    size;    /* +0x04: 桶数 (质数) */
    uint32_t    filled;  /* +0x08: 活跃条目数 */
    int32_t (*change_callback)(const char *key, char *value, int op, int flag);  /* +0x0c */
} ub_hsearch_data_t;

/* ----------------------------------------------------------------------------
 * 模块内部前向声明区
 * -------------------------------------------------------------------------- */

/* 前向引用 (转写在下方): ub_hsearch, 原 FUN_040034a8, 被 ub_himport / ub_hdelete 调用 */
int ub_hsearch(const char *item_key, char *item_data, int action,
               ub_entry_t **retval, ub_hsearch_data_t *htab, int flag);

int ub_key_list_match(const char *key, int count, char **list);
void ub_memcpy(void *dst, const void *src, uint32_t len);
int ub_memcmp(const uint8_t *a, const uint8_t *b, int n);
void *ub_memmove(void *dst, const void *src, int len);
void *ub_memset(void *dst, int val, uint32_t len);
void ub_printf(const char *fmt, ...);
char *ub_strdup(const char *s);
int ub_strcmp(const char *a, const char *b);
int ub_strncmp(const char *a, const char *b, uint32_t n);
uint32_t ub_strlen(const char *s);
uint64_t ub_tick_to_ms(uint32_t tick_lo, uint32_t tick_hi);
uint64_t ub_timer_read(void);
uint32_t ub_div64_rem(uint32_t *val64, uint32_t divisor);
int ub_regex_exec(void *prog, int arg2, const char *str, int len, int *pos, void *ctx);
int ub_str_match(uint32_t flags, const char *str, const char *pattern, void *buf);
int ub_vsnprintf(char *buf, uint32_t size, const char *fmt, va_list args);  /* FUN_040069f0 */
void ub_hdelete_entry(ub_hsearch_data_t *htab, uint32_t *filled, ub_entry_t *key_ptr,
                      uint32_t slot_idx);
void ub_hdestroy(ub_hsearch_data_t *htab);                       /* FUN_0400339c, 其他批次 */
int ub_is_prime(uint32_t est_size, ub_hsearch_data_t *htab);     /* FUN_04003300: 质数容量建表 */
int ub_hdelete(const char *key, ub_hsearch_data_t *htab, int flag);  /* FUN_04003954 */
int ub_match_filter(char **key_ptr, char **value_ptr, uint32_t flags,
                    int nmatch, char **strings);                 /* FUN_04003210 */

// ============================================================================
// 函数转写区 (按地址升序)
// ============================================================================

// === ub_sha256_update === 原 FUN_040028a8 @ 0x040028a8
// 来源: reversed/uboot_funcs/FUN_040028a8.c | 分类: uboot
// 语义: SHA256 增量输入。ctx 布局 (反编译证实): total[2] @+0x00 (64 位累计长度),
//       state[8] @+0x08, 缓冲 buffer[64] @+0x28。先把总长度累加 (64 位进位),
//       再把数据按 64 字节块压缩: 先补满残余块, 后整块直压, 尾部不足 64 字节存入缓冲。
void ub_sha256_update(uint32_t *ctx, const void *data, uint32_t len)
{
    const uint8_t *in = (const uint8_t *)data;
    uint32_t total_lo = ctx[0];
    uint32_t buf_used = total_lo & 0x3f;    /* 缓冲中已有字节数 */
    uint32_t buf_free = 0x40 - buf_used;

    /* 累计总长度: ctx[0]=低字, ctx[1]=高字 (原 CARRY4 进位) */
    ctx[0] = total_lo + len;
    if ((uint32_t)(total_lo + len) < total_lo)
        ctx[1]++;

    /* 缓冲非空且本次数据足以补满 → 拼满残余块并立即压缩 */
    if (buf_used != 0 && buf_free <= len) {
        len -= buf_free;
        ub_memcpy((uint8_t *)ctx + 0x28 + buf_used, in, buf_free);
        in += buf_free;
        buf_used = 0;
        ub_sha256_transform(ctx, (uint32_t *)((uint8_t *)ctx + 0x28));
    }

    /* 整块直压: 对齐到 64 字节逐块处理 */
    if (len >= 0x40) {
        const uint8_t *end = in + ((len - 0x40) & 0xffffffc0) + 0x40;
        const uint8_t *blk = in;
        do {
            blk += 0x40;
            ub_sha256_transform(ctx, (const uint32_t *)(blk - 0x40));
        } while (blk != end);
        len &= 0x3f;
    }

    /* 尾部 (<64 字节) 存入缓冲 (原反编译 param_3==0 直接返回) */
    if (len == 0)
        return;
    ub_memcpy((uint8_t *)ctx + 0x28 + buf_used, in, len);
}

// === ub_un_02f0bc === 原 FUN_04002fbc @ 0x04002fbc
// 来源: reversed/uboot_funcs/FUN_04002fbc.c | 分类: uboot
// 语义: 调用无参外部函数 func_0x03ffae84 (命名待补), 以布尔形式返回其结果是否非零。
int ub_un_02f0bc(void)
{
    return func_0x03ffae84() != 0;
}

// === ub_un_02f0dc === 原 FUN_04002fdc @ 0x04002fdc
// 来源: reversed/uboot_funcs/FUN_04002fdc.c | 分类: uboot
// 语义: 调用 func_0x03ffafa8(param_1, DAT_04003018) (命名待补); 返回值 <0 时返回
//       param_3。返回值 >=0 的分支在原始反汇编中为无法识别的指令流 (Ghidra 标注
//       "bad instruction data"/halt_baddata), 后续控制流不可还原, 以死循环表达。
uint32_t ub_un_02f0dc(uint32_t param_1, uint32_t param_2, uint32_t param_3)
{
    int ret = func_0x03ffafa8(param_1, DAT_04003018);

    if (ret >= 0) {
        /* 原始反汇编: 无法识别指令流, 不可恢复 (halt_baddata) */
        for (;;) {
        }
    }
    return param_3;
}

// === ub_key_list_match === 原 FUN_040030f8 @ 0x040030f8
// 来源: reversed/uboot_funcs/FUN_040030f8.c | 分类: uboot
// 语义: 白名单匹配 (对应上游 drop_var_from_set): 在 count 个名字里查 key;
//       命中的槽位被清为 NULL (收尾阶段不再删除该变量) 并返回 1, 否则返回 0。
//       注: 反编译中 ub_strcmp 只显示 1 个实参 (r1 已持有 *p, Ghidra 丢参),
//       按命中后清 NULL 的行为推断为 strcmp(key, *p)。命中后不提前返回。
int ub_key_list_match(const char *key, int count, char **list)
{
    int matched = 0;
    char **p;

    if (count < 1)
        return 0;
    for (p = list; p != list + count; p++) {
        if (*p != NULL && ub_strcmp(key, *p) == 0) {
            matched = 1;
            *p = NULL;
        }
    }
    return matched;
}

// === ub_htab_find_entry === 原 FUN_040033fc @ 0x040033fc
// 来源: reversed/uboot_funcs/FUN_040033fc.c | 分类: uboot
// 语义: 从 start+1 槽开始线性扫描, 找活跃 (used>0) 且 key 为前缀的条目
//       (strncmp(key, entry.key, strlen(key))==0); 命中返回槽索引且 *out=&entry.key。
//       未命中: *DAT_040034a4=3 (ESRCH), *out=NULL, 返回 0。
uint32_t ub_htab_find_entry(const char *key, uint32_t start, ub_entry_t **out,
                            ub_hsearch_data_t *htab)
{
    uint32_t idx = start + 1;
    uint32_t key_len = ub_strlen(key);      /* 反编译丢失实参, 被测串为 key */
    uint32_t size = htab->size;

    if (idx < size) {
        uint32_t off = idx * 0x14;          /* 条目步长 0x14 (20 字节) */
        do {
            if (*(int32_t *)((uint8_t *)htab->table + off) > 0) {
                if (ub_strncmp(key, *(char **)((uint8_t *)htab->table + off + 4),
                               key_len) == 0) {
                    *out = (ub_entry_t *)((uint8_t *)htab->table + off + 4);
                    return idx;
                }
                size = htab->size;          /* 匹配失败后重读 size (反编译如此) */
            }
            idx++;
            off += 0x14;
        } while (idx < size);
    }
    *DAT_040034a4 = 3;
    *out = NULL;
    return 0;
}

// === ub_hsearch === 原 FUN_040034a8 @ 0x040034a8
// 来源: reversed/uboot_funcs/FUN_040034a8.c | 分类: uboot
// 语义: 哈希表查找/插入 (U-Boot hsearch_r 变体)。item{key,data} 按 AAPCS 经 r0/r1
//       传参 (Ghidra 展开成 8 个参数, 系结构体+栈参伪影)。action==1 (ENTER) 时插入
//       或更新, 否则仅查找。成功: 查找/更新路径返回桶索引, 新插入返回 1, *retval =
//       &entry.key; 失败: 返回 0, *retval=NULL, 并向 DAT_04003950 写错误码
//       (1=EPERM 表级回调拒绝 / 3=ESRCH 未找到 / 12=ENOMEM 表满或串复制失败 /
//       22=EINVAL 条目回调拒绝)。哈希 = 从尾到头 hash=hash*16+字符 (初值=key 长度),
//       落桶 = hash%size, 余 0 时改用槽 1; 线性探测步长 = hash%(size-2)+1;
//       槽 used 存落桶索引, 0xffffffff 为删除标记 (探测时跳过但不算空)。
int ub_hsearch(const char *item_key, char *item_data, int action,
               ub_entry_t **retval, ub_hsearch_data_t *htab, int flag)
{
    ub_entry_t *entry;
    uint32_t hash;
    uint32_t cur;
    uint32_t step;
    uint32_t first_empty;
    uint32_t slot;
    uint32_t err;
    const char *p;

    hash = ub_strlen(item_key);             /* 哈希初值 = key 长度 */
    if (hash == 0) {
        hash = 1;                           /* 空 key 固定落桶 1 */
    } else {
        p = item_key + hash;
        do {
            p--;
            hash = hash * 0x10 + (uint8_t)*p;   /* 从尾到头: hash = hash*16 + 字符 */
        } while (p != item_key);
        hash = hash % htab->size;           /* aeabi_uidivmod, 取余数 */
        if (hash == 0)
            hash = 1;
    }

    entry = &htab->table[hash];
    first_empty = hash;                     /* 首个可插入槽候选 (空槽/删除标记时为 home) */

    if (entry->used == 0) {
        /* home 为空槽: ENTER 且未满 → 插入; 否则未找到 */
        if (action == 1) {
            if (htab->filled != htab->size) {
                slot = hash;
                goto insert;                /* 原标签 04003600 */
            }
            err = 0xc;                      /* ENOMEM: 表满 */
            goto set_err;                   /* 原标签 040035c8 */
        }
        err = 3;                            /* ESRCH */
        goto set_err;
    }

    if (entry->used != 0xffffffff)          /* 活跃条目 → 该槽不作为插入候选 */
        first_empty = 0;

    if (entry->used == hash) {              /* 桶标记与哈希一致 → 试键 */
        if (ub_strcmp(item_key, entry->key) == 0) {
            cur = hash;
            goto found;                     /* 原标签 040038ec 路径 */
        }
        goto probe;                         /* 同桶异键 → 冲突探测 (原标签 04003538) */
    }

probe:
    /* 线性探测: 步长 = hash % (size-2) + 1 */
    step = hash % (htab->size - 2) + 1;
    cur = hash;
    for (;;) {
        do {
            /* cur = (cur - step) mod size (保持落在 [1, size], 哨兵槽 used=0 会终止) */
            if (step < cur)
                cur = cur - step;
            else
                cur = cur + htab->size - step;
            if (cur == hash)
                goto probe_end;             /* 绕回起点 → 探测终止 (原标签 040035b4) */

            entry = &htab->table[cur];
            if (entry->used == hash)
                break;                      /* 落桶匹配 → 试键 */
            if (entry->used == 0)
                goto probe_end;             /* 空槽 → 探测终止 */
            /* 删除标记 (0xffffffff) 或其他落桶 → 继续探测 (原标签 04003550) */
        } while (entry->used != 0);

        if (ub_strcmp(item_key, entry->key) == 0) {
found:
            /* 查找命中 */
            if (item_data != NULL && action == 1) {
                /* ENTER 且带新值 → 更新 (先表级回调, 后条目回调, 均拒绝则报错) */
                if (htab->change_callback != NULL &&
                    htab->change_callback(&entry->key, item_data, 2, flag) != 0) {
                    err = 1;                /* EPERM (原标签 04003944) */
                    goto set_err;
                }
                if (entry->callback != NULL &&
                    entry->callback(item_key, item_data, 2, flag) != 0) {
                    err = 0x16;             /* EINVAL (原标签 0400391c) */
                    goto set_err;
                }
                func_0x03fc8570(entry->data);           /* 释放旧值 */
                entry->data = ub_strdup(item_data);
                if (entry->data == NULL) {
                    err = 0xc;              /* ENOMEM (原标签 04003840) */
                    goto set_err;
                }
            }
            *retval = &entry->key;
            /* 原反编译含 cur != 0xffffffff 判定, 达不到时继续探测;
             * 合法 size 下 cur 不会为 0xffffffff, 故恒走返回分支 */
            if (cur != 0xffffffff)
                return cur;
            continue;
        }
        /* 同落桶异键 → 继续探测 (外层 do-while 条目 used 必非 0) */
    }

probe_end:
    if (action == 1) {
        if (htab->filled != htab->size) {
            if (first_empty == 0)
                first_empty = cur;          /* 探测途中遇到的首个空槽 */
            slot = first_empty;
            entry = &htab->table[slot];
            goto insert;
        }
        err = 0xc;                          /* ENOMEM: 表满 */
        goto set_err;
    }
    err = 3;                                /* ESRCH: 未找到 */
    goto set_err;

insert:
    /* 新建条目: used 存落桶索引, key/data 各 strdup 一份 */
    entry->used = hash;
    entry->key = ub_strdup(item_key);
    entry->data = ub_strdup(item_data);
    if (entry->key != NULL && entry->data != NULL) {
        htab->filled++;
        func_0x03fc3288(&entry->key);       /* 命名待补: 插入完成钩子 1 */
        func_0x03fc35fc(&entry->key);       /* 命名待补: 插入完成钩子 2 */
        if (htab->change_callback == NULL ||
            htab->change_callback(&entry->key, item_data, 0, flag) == 0) {
            if (entry->callback != NULL &&
                entry->callback(item_key, item_data, 0, flag) != 0) {
                /* 条目回调拒绝 → 回滚删除并报 EINVAL */
                ub_hdelete_entry(htab, &htab->filled, &entry->key, slot);
                err = 0x16;
                goto set_err;
            }
            *retval = &entry->key;
            return 1;
        }
        /* 表级回调拒绝 → 回滚删除并报 EPERM */
        ub_hdelete_entry(htab, &htab->filled, &entry->key, slot);
        err = 1;
        goto set_err;
    }
    err = 0xc;                              /* ENOMEM: strdup 失败 */

set_err:
    *DAT_04003950 = err;
    *retval = NULL;
    return 0;
}

// === ub_hexport === 原 FUN_04003a68 @ 0x04003a68
// 来源: reversed/uboot_funcs/FUN_04003a68.c | 分类: uboot
// 语义: 哈希表导出 (U-Boot hexport_r 变体)。扫描槽 1..size 收集匹配条目: flag 位 8
//       放行 key 以 '.' 开头的变量; flag 位 0x10/0x20 经 ub_match_filter 按 nmatch 个
//       pattern 过滤。长度估算后按比较函数 DAT_04003d80 排序, 逐项写
//       "key=value<sep>" (值中 '\\' 与 sep 前加 '\\'), 末尾 NUL。
//       resp 非空时若 size < 所需 → 打印 DAT_04003d84 并返回 0xffffffff;
//       resp 为空则按需 calloc。成功返回所需/指定缓冲字节数, 失败返回 0xffffffff,
//       错误码写 DAT_04003d88 (22=EINVAL 12=ENOMEM)。
uint32_t ub_hexport(ub_hsearch_data_t *htab, uint32_t sep, uint32_t flags,
                    char **resp, uint32_t size, int nmatch, char **strings)
{
    uint32_t needed;
    uint32_t total = 0;
    uint32_t count = 0;
    uint32_t idx;
    char *out;
    uint32_t i;
    char *sort_tab[(htab != NULL && htab->size > 0) ? htab->size : 1];   /* 反编译: -(size*4+10)&~7 栈数组 */

    if (resp == NULL || htab == NULL) {
        *DAT_04003d88 = 0x16;               /* EINVAL */
        return 0xffffffff;
    }

    if (htab->size == 0) {
        needed = 1;                         /* 空表: 只需结尾 NUL */
    } else {
        idx = 1;                            /* 槽 0 不参与; 含哨兵槽 table[size] */
        do {
            ub_entry_t *e = &htab->table[idx];
            if (e->used > 0) {
                /* 匹配过滤 (第 5 参 strings 经栈槽传入, Ghidra 以栈写形式呈现) */
                if ((ub_match_filter(&e->key, &e->data, flags, nmatch, strings) != 0 ||
                     nmatch < 1) &&
                    ((flags & 8) == 0 || e->key[0] != '.')) {
                    uint32_t l;
                    sort_tab[count] = &e->key;
                    l = ub_strlen(e->key) + total + 2;      /* name + '=' + sep */
                    if (sep == 0) {
                        l += ub_strlen(e->data);
                    } else {
                        const char *s = e->data;
                        while (*s != '\0') {                /* 逃逸字符按 2 字节计 */
                            if (*s == '\\' || *s == (char)sep)
                                l += 2;
                            else
                                l += 1;
                            s++;
                        }
                    }
                    total = l + 2;
                    count++;
                }
            }
            idx++;
        } while (idx <= htab->size);
        needed = total + 1;
    }

    /* 排序 (qsort 型: 数组, 数量, 每项 4 字节, 比较函数) */
    func_0x03ffd878(sort_tab, count, 4, DAT_04003d80);

    if (size != 0) {
        if (size < needed) {
            ub_printf((const char *)DAT_04003d84, size, needed);
            *DAT_04003d88 = 0xc;            /* ENOMEM */
            return 0xffffffff;
        }
        needed = size;                      /* 用满调用方给定尺寸 */
    }

    out = *resp;
    if (out == NULL) {
        out = (char *)func_0x03fc8dc4(1, needed);       /* calloc 型分配 */
        *resp = out;
        if (out == NULL) {
            *DAT_04003d88 = 0xc;
            return 0xffffffff;
        }
    } else {
        ub_memset(out, 0, needed);
    }

    for (i = 0; i < count; i++) {
        const char *s = sort_tab[i];        /* 指向 entry.key */
        while (*s != '\0')
            *out++ = *s++;
        *out++ = '=';
        s = *(char **)(sort_tab[i] + 4);    /* entry.data 紧随 key 字段 (+4) */
        while (*s != '\0') {
            if (*s == '\\' || *s == (char)sep)
                *out++ = '\\';              /* 逃逸转义 */
            *out++ = *s;
            s++;
        }
        *out++ = (char)sep;
    }
    *out = '\0';
    return needed;
}

// === ub_himport === 原 FUN_04003d8c @ 0x04003d8c
// 来源: reversed/uboot_funcs/FUN_04003d8c.c | 分类: uboot
// 语义: 导入 "name=value<sep>" 文本到哈希表 (U-Boot himport_r 变体)。
//       flag&1 = 保留现有表 (H_NOCLEAR), 否则先 ub_hdestroy; 表不存在时以
//       est = size/8+64 (上限 0x200) 经 ub_is_prime 质数建表。
//       crlf_is_lf 非零时先原地收缩 "\r\n" → "\n"。逐条解析: 跳过空白; '#' 起始
//       (sep != '#') 为注释行, 跳至 sep/NUL 之后; 无值的名字 (含 "name=" 与
//       "name<sep>") → 白名单命中后 ub_hdelete; 有值 → 原地去逃逸 ('\x'→'x',
//       尾部孤立 '\' 保留) 后 ub_hsearch(ENTER) 写入, 失败打印 DAT_04004220;
//       '=' 起始的空名字 → EINVAL 并整体失败。
//       收尾: 白名单中未被导入数据清掉的剩余变量逐个 ub_hdelete, 成功打印
//       DAT_04004218, 未找到打印 DAT_0400421c。返回 1 成功 / 0 失败
//       (错误码写 DAT_04004214: 22=EINVAL 12=ENOMEM)。
int ub_himport(ub_hsearch_data_t *htab, const void *env, uint32_t size, uint8_t sep,
               uint32_t flag, int crlf_is_lf, int nvars, char **vars)
{
    uint8_t *buf;                       /* env 副本 (反编译槽 40) */
    uint8_t *end;                       /* buf + size (反编译槽 9) */
    uint8_t *cur;                       /* 当前条目起点 (pbVar16) */
    uint8_t *next;                      /* cur+1 / 扫描推进 (pbVar13) */
    uint8_t *scan;                      /* 名字终止符位置 (pbVar10) */
    uint8_t *src;
    uint8_t *dst;
    uint8_t *value;                     /* 值起点 (pbVar4) */
    uint8_t c;                          /* 当前字符 (pbVar12) */
    uint8_t ch;
    uint8_t c2;
    uint32_t crs;
    uint32_t i;
    uint32_t est;
    ub_entry_t *rv;
    char *var_list[(nvars > 0) ? nvars : 1];   /* 反编译 -(nvars*4+10)&~8 栈数组 */

    if (htab == NULL) {
        *DAT_04004214 = 0x16;           /* EINVAL */
        return 0;
    }
    buf = (uint8_t *)func_0x03fc87b0(size + 1);      /* malloc 型分配 */
    if (buf == NULL) {
        *DAT_04004214 = 0xc;            /* ENOMEM */
        return 0;
    }
    ub_memcpy(buf, env, size);
    buf[size] = 0;
    if (nvars != 0)
        ub_memcpy(var_list, vars, (uint32_t)nvars * 4);    /* 白名单拷贝到栈, 匹配时原地清 NULL */

    /* 表准备: 非 H_NOCLEAR 先销毁; 无表则质数容量新建 */
    if ((flag & 1) == 0 && htab->table != NULL)
        ub_hdestroy(htab);
    if (htab->table == NULL) {
        est = (size >> 3) + 0x40;
        if (0x1ff < est)
            est = 0x200;
        if (ub_is_prime(est, htab) == 0) {
            func_0x03fc8570(buf);
            return 0;
        }
    }
    if (size == 0) {
        func_0x03fc8570(buf);
        return 1;
    }

    /* CRLF → LF: 仅当 crlf_is_lf 非零; '\r' 后随 '\n' 且非末字节时丢弃 */
    c = buf[0];
    cur = buf;
    if (crlf_is_lf != 0 && size != 0 && c != 0) {
        uint8_t *p = buf;
        crs = 0;
        do {
            if (c == 0x0d && p < buf + (size - 1) && p[1] == 0x0a) {
                crs++;                              /* 丢弃 '\r' */
            } else {
                p[-(int)crs] = c;                   /* 左移覆盖 */
            }
            if (buf + size - 1 == p)
                break;
            p++;
            c = *p;
        } while (c != 0);
        size -= crs;
        c = buf[0];
    }
    end = buf + size;

    /* 主解析循环 */
    for (;;) {
        while (c == 0x09 || c == 0x20) {            /* 跳过前导空白 (tab/space) */
            c = cur[1];
            cur++;
        }
        next = cur + 1;

        if (c == '#') {
            /* 注释行: 扫到 sep 或 NUL, 越过一个字节继续; sep 即 '#' 时只跳 '#' */
            if (sep != '#') {
                uint8_t *q = next;
                while (*q != 0 && *q != sep)
                    q++;
                next = q + 1;
            }
            goto next_entry;
        }

        /* ---- 名字扫描 ---- */
        scan = cur;
        if (c != 0 && c != '=') {
            if (c != sep) {
                for (;;) {
                    scan = next;
                    next = scan + 1;
                    c = *scan;
                    if (c == '=' || c == 0)
                        break;                      /* 原标签 04003f1c 入口 */
                    if (c == sep)
                        goto name_at_sep;
                }
            } else {
                goto name_at_sep;                   /* 首字符即 sep → 空名字 */
            }
        }

        /* 原标签 04003f1c: 名字终止于 '=' 或 NUL (scan 指向该字节) */
        if (c == 0)
            goto delete_path;                       /* 终止于 NUL → 删除路径 */
        c2 = scan[1];
        if (c2 != 0 && c2 != sep && sep != '=') {
            *scan = 0;                              /* 覆写 '=' 终止名字 */
            value = next;                           /* 值起点 */
            goto parse_value;
        }
        /* 无值 ("name" / "name=" / '=' 为末字节) → 终止名字与空值区后删除 */
        *scan = 0;                                  /* 原标签 04003ff0 */
        *(scan + 1) = 0;                            /* 原标签 04003f24 */
        next = scan + 2;
        goto delete_path;

name_at_sep:
        *scan = 0;                                  /* 覆写 sep 终止名字 */
        /* next 已 = scan + 1 */

delete_path:
        if (nvars != 0) {
            if (ub_key_list_match(cur, nvars, var_list) == 0)
                goto next_entry;                    /* 不在白名单 → 跳过 */
        }
        ub_hdelete(cur, htab, flag);                /* 返回值弃用 (反编译存而不用) */
        goto next_entry;

parse_value:
        /* 值去逃逸 (原地): '\x' → 'x'; 值尾孤立 '\' 保留自身; 止于 NUL 或 sep */
        src = value;
        dst = value;
        c = *src;
        do {
            if (c == '\\') {
                if (src[1] != 0) {
                    ch = src[1];                    /* 跳过 '\' 取后一字符 */
                    src += 2;
                } else {
                    ch = '\\';                      /* 尾部孤立 '\' 保留 */
                    src += 1;
                }
            } else {
                ch = *src;
                src += 1;
            }
            *dst = ch;
            dst++;
            c = *src;
        } while (c != 0 && c != sep);
        *dst = 0;
        next = src + 1;                             /* 越过 NUL/sep (反编译值尾缓存槽 44) */

        if (*cur == 0) {
            /* 空名字 ('=' 起始的条目) → EINVAL, 整体失败 */
            *DAT_04004214 = 0x16;
            func_0x03fc8570(buf);
            return 0;
        }
        if (nvars != 0 && ub_key_list_match(cur, nvars, var_list) == 0)
            goto next_entry;                        /* 不在白名单 → 跳过 */
        rv = NULL;
        ub_hsearch(cur, value, 1, &rv, htab, flag); /* action = ENTER */
        if (rv == NULL)
            ub_printf((const char *)DAT_04004220, cur, value);

next_entry:
        if (end <= next)
            break;
        c = *next;
        cur = next;
        if (c == 0)
            break;
    }

    func_0x03fc8570(buf);
    /* 白名单收尾: 未被导入数据命中 (清 NULL) 的剩余变量逐个删除 */
    if (nvars > 0) {
        for (i = 0; i < (uint32_t)nvars; i++) {
            if (var_list[i] != NULL) {
                if (ub_hdelete(var_list[i], htab, flag) != 0)
                    ub_printf((const char *)DAT_04004218, var_list[i]);
                else
                    ub_printf((const char *)DAT_0400421c, var_list[i]);
            }
        }
    }
    return 1;
}

// === ub_print_u64 === 原 FUN_040042ac @ 0x040042ac
// 来源: reversed/uboot_funcs/FUN_040042ac.c | 分类: uboot
// 语义: 容量式打印 64 位值 (U-Boot print_size 语义): 找最大 k∈{60,50,...,10} 使
//       (hi:lo) >= 2^k, 打印 整数.一位小数(四舍五入) + 后缀表 DAT_040043e0 第 k/10-1
//       项; 值 < 2^10 时按完整十进制 (DAT_040043dc) 打印。param_3 作为尾缀串透传。
void ub_print_u64(uint32_t lo, uint32_t hi, const char *tail)
{
    uint64_t value = ((uint64_t)hi << 32) | lo;
    const char *suffix_tbl = (const char *)DAT_040043e0;
    uint32_t k = 60;
    uint32_t i = 0;

    for (;;) {
        uint32_t int_part = (uint32_t)(value >> k);
        if (int_part != 0) {
            char suffix = suffix_tbl[i];
            if (suffix == '\0')
                break;                      /* 超出后缀表 → 全量打印 */
            {
                uint64_t rem = value & ((((uint64_t)1) << k) - 1);
                if (rem == 0) {
                    ub_printf((const char *)DAT_040043e4, int_part);
                } else {
                    /* 首位小数 = (余数*10 + 2^(k-1)) >> k (含四舍五入进位) */
                    uint64_t x = rem * 10 + (((uint64_t)1) << (k - 1));
                    uint32_t digit = (uint32_t)(x >> k);
                    if (9 < digit) {
                        int_part++;
                        digit -= 10;
                    }
                    ub_printf((const char *)DAT_040043e4, int_part);
                    if (digit != 0)
                        ub_printf((const char *)DAT_040043ec, digit);
                }
                ub_printf((const char *)DAT_040043e8, suffix, tail);
                return;
            }
        }
        k -= 10;
        i++;
        if (k == 0)
            break;
    }
    /* 反编译: printf(fmt, hi, lo, hi, tail) — %llu%s 的 64 位实参按 AAPCS 落在 r2:r3 */
    ub_printf((const char *)DAT_040043dc, value, tail);
}

// === ub_regex_prog_insert === 原 FUN_04004a9c @ 0x04004a9c
// 来源: reversed/uboot_funcs/FUN_04004a9c.c | 分类: uboot
// 语义: 在正则程序 prog 的 pos 处腾出 len 字节: 先在尾部追加 0 字节 (程序体以 NUL
//       结尾), 再把 [pos, 尾) 后移 len 字节, 长度 += len。程序布局: +0x000 程序体,
//       +0x200 当前长度, +0x210 错误标记; 长度已达 0x100 时写 DAT_04004af0。
void ub_regex_prog_insert(uint8_t *prog, int pos, int len)
{
    int curlen = *(int32_t *)(prog + 0x200);

    if (curlen < 0x100) {
        *(int32_t *)(prog + 0x200) = curlen + 1;
        prog[curlen] = 0;
        curlen++;
    } else {
        *(uint32_t *)(prog + 0x210) = DAT_04004af0;     /* 溢出错误标记 */
    }
    ub_memmove(prog + pos + len, prog + pos, curlen - pos);
    *(int32_t *)(prog + 0x200) += len;
}

// === ub_regex_emit_quant === 原 FUN_04004af4 @ 0x04004af4
// 来源: reversed/uboot_funcs/FUN_04004af4.c | 分类: uboot
// 语义: 生成量词节点 {ch, 距程序尾字节数}。若 pos 处已是计数≥2 的量词节点
//       (code==3), 其计数先减 1, 并在程序尾追加 {3, 旧ch+旧计数, 1} (尾插过程按
//       0x100 边界钳位/报错 DAT_04004bd0), 之后仍从原 pos 插入新节点;
//       距离字段 ≥0x100 时写错误标记 DAT_04004bcc。
void ub_regex_emit_quant(uint8_t *prog, int pos, uint8_t ch)
{
    int len;
    int n1;
    int n2;
    char old_ch;
    char old_cnt;

    if (*(char *)(prog + pos) == '\x03' && prog[pos + 2] >= 2) {
        prog[pos + 2] -= 1;
        len = *(int32_t *)(prog + 0x200);
        if (len < 0x100) {
            n1 = len + 1;
            *(int32_t *)(prog + 0x200) = n1;
            prog[len] = 3;
            if (n1 == 0x100)
                len = 0x100;
            old_ch = *(char *)(prog + pos + 1);
            old_cnt = *(char *)(prog + pos + 2);
            if (n1 == 0x100)
                goto overflow;                  /* 原标签 04004bc0 */
            n2 = len + 2;
            *(int32_t *)(prog + 0x200) = n2;
            if (n2 != 0x100)
                len = len + 3;
            prog[n1] = old_ch + old_cnt;
            if (n2 == 0x100) {
                len = 0x100;
                goto overflow;
            }
            *(int32_t *)(prog + 0x200) = len;
            prog[n2] = 1;
        } else {
overflow:
            *(uint32_t *)(prog + 0x210) = DAT_04004bd0;
        }
        pos = len - 3;
    }
    /* 原标签 04004b10 */
    ub_regex_prog_insert(prog, pos, 2);
    prog[pos] = ch;
    {
        int tail_len = *(int32_t *)(prog + 0x200) - pos;
        if (tail_len < 0x100)
            prog[pos + 1] = (char)tail_len;
        else
            *(uint32_t *)(prog + 0x210) = DAT_04004bcc;
    }
}

// === ub_regex_match === 原 FUN_0400583c @ 0x0400583c
// 来源: reversed/uboot_funcs/FUN_0400583c.c | 分类: uboot
// 语义: 在 str (长 len) 上做正则匹配。prog+0x20c 非零时只执行一次 ub_regex_exec;
//       否则从位置 0 起逐位推进尝试 (每轮位置+1), 直到匹配或尝试满 len 次。
//       返回值经 r0 传出 (调用点 ub_str_match 读取), 即最后一次 exec 的结果。
int ub_regex_match(uint8_t *prog, const char *str, int len, void *ctx)
{
    int pos = 0;
    int ret = 0;

    if (*(int32_t *)(prog + 0x20c) != 0) {
        return ub_regex_exec(prog, 0, str, len, &pos, ctx);
    }
    if (len > 0) {
        int next;
        pos = 0;
        do {
            next = pos + 1;
            ret = ub_regex_exec(prog, 0, str, len, &pos, ctx);
            pos = next;                     /* exec 返回后位置被覆写为尝试序号 (反编译如此) */
        } while (ret == 0 && next < len);
    }
    return ret;
}

// === ub_strcpy === 原 FUN_04005994 @ 0x04005994
// 来源: reversed/uboot_funcs/FUN_04005994.c | 分类: uboot
// 语义: 字符串复制, 含结尾 NUL; 无重叠保护 (原实现即逐字节前向拷贝)。
void ub_strcpy(char *dst, const char *src)
{
    do {
        *dst = *src;
        dst++;
    } while (*src++ != '\0');
}

// === ub_strchr === 原 FUN_04005b14 @ 0x04005b14
// 来源: reversed/uboot_funcs/FUN_04005b14.c | 分类: uboot
// 语义: 查找字符 c 首次出现位置; c == '\0' 时返回指向结尾 NUL 的指针 (标准 strchr)。
char *ub_strchr(char *s, char c)
{
    char cur = *s;

    if (cur == c)
        return s;
    do {
        if (cur == '\0')
            return NULL;
        s++;
        cur = *s;
    } while (cur != c);
    return s;
}

// === ub_strlen === 原 FUN_04005b98 @ 0x04005b98
// 来源: reversed/uboot_funcs/FUN_04005b98.c | 分类: uboot
// 语义: 求字符串长度 (不含 NUL)。
uint32_t ub_strlen(const char *s)
{
    const char *p = s;

    if (*s == '\0')
        return 0;
    do {
        p++;
    } while (*p != '\0');
    return (uint32_t)(p - s);
}

// === ub_strdup === 原 FUN_04005c20 @ 0x04005c20
// 来源: reversed/uboot_funcs/FUN_04005c20.c | 分类: uboot
// 语义: 复制字符串到新分配内存 (func_0x03fc87b0 = malloc 型, 长度 strlen+1 含 NUL)。
//       s 为 NULL 或分配失败返回 NULL。
char *ub_strdup(const char *s)
{
    char *dup;

    if (s != NULL) {
        dup = (char *)func_0x03fc87b0(ub_strlen(s) + 1);
        if (dup != NULL) {
            char *p = dup;
            do {
                *p = *s;
                p++;
            } while (*s++ != '\0');
            return dup;
        }
    }
    return NULL;
}

// === ub_memcpy === 原 FUN_04005ea4 @ 0x04005ea4
// 来源: reversed/uboot_funcs/FUN_04005ea4.c | 分类: uboot
// 语义: 内存拷贝。dst==src 直接返回; 双侧 4 字节对齐且 len>=4 时按字拷贝
//       ((len-4)/4+1 个字) 再补尾字节, 否则逐字节前向拷贝。无重叠检测 (memcpy 语义)。
void ub_memcpy(void *dst, const void *src, uint32_t len)
{
    if (dst == src)
        return;
    if ((((uint32_t)(uintptr_t)dst | (uint32_t)(uintptr_t)src) & 3) != 0 || len < 4) {
        /* 逐字节前向拷贝 */
        uint8_t *d = (uint8_t *)dst;
        const uint8_t *s = (const uint8_t *)src;
        const uint8_t *send = s + len;
        if (len == 0)
            return;
        do {
            *d = *s;
            d++;
            s++;
        } while (s != send);
        return;
    }
    {
        /* 按字拷贝 + 尾字节 */
        uint32_t words = ((len - 4) >> 2) + 1;
        uint32_t *d = (uint32_t *)dst;
        const uint32_t *s = (const uint32_t *)src;
        uint32_t i;
        for (i = 0; i < words; i++)
            d[i] = s[i];
        if ((len & 3) == 0)
            return;
        {
            uint8_t *db = (uint8_t *)(d + words);
            const uint8_t *sb = (const uint8_t *)(s + words);
            const uint8_t *send = sb + (len & 3);
            do {
                *db = *sb;
                db++;
                sb++;
            } while (sb != send);
        }
    }
}

// === ub_memcmp === 原 FUN_04005ff4 @ 0x04005ff4
// 来源: reversed/uboot_funcs/FUN_04005ff4.c | 分类: uboot
// 语义: 内存比较 (标准 memcmp): n==0 返回 0; 逐字节无符号比较, 首个差异处返回
//       (uint)*a - (uint)*b 的 32 位差值, 全同返回 0。反编译尾部的"不可恢复跳表"
//       实为返回差值的普通出口, 已按此语义结构化。
int ub_memcmp(const uint8_t *a, const uint8_t *b, int n)
{
    const uint8_t *last;

    if (n == 0)
        return 0;
    if (*a != *b)
        return (int)((uint32_t)*a - (uint32_t)*b);
    last = b + n - 1;
    do {
        if (b == last)
            return 0;
        a++;
        b++;
    } while (*a == *b);
    return (int)((uint32_t)*a - (uint32_t)*b);
}

// === ub_tick_to_ms === 原 FUN_04006118 @ 0x04006118
// 来源: reversed/uboot_funcs/FUN_04006118.c | 分类: uboot
// 语义: 计时换算: 64 位 ticks (hi:lo) 乘 1000 后除以 func_0x03fb2008() 返回的时钟
//       频率 (U-Boot tick→ms)。反编译把 ×1000 展开为 hi/lo 分量运算, 此处用 uint64_t
//       表达 (数学等价)。积的高字为 0 时走 32 位除法 (FUN_0400c66c = aeabi_uidiv),
//       否则走 ub_div64_rem 的 64/32 就地除法; 两者均丢弃余数, 返回商。
uint64_t ub_tick_to_ms(uint32_t tick_lo, uint32_t tick_hi)
{
    uint64_t scaled = ((uint64_t)tick_hi << 32 | tick_lo) * 1000;
    uint32_t freq = func_0x03fb2008();

    if ((uint32_t)(scaled >> 32) == 0)
        return (uint64_t)((uint32_t)scaled / freq);
    ub_div64_rem((uint32_t *)&scaled, freq);        /* 就地更新为商, 余数弃 */
    return scaled;
}

// === ub_get_timer === 原 FUN_040061e4 @ 0x040061e4
// 来源: reversed/uboot_funcs/FUN_040061e4.c | 分类: uboot
// 语义: U-Boot get_timer: 读当前计时并换算毫秒, 返回相对 base 的毫秒差。
//       反编译中 ub_timer_read 的 (r0,r1) 直接流入 ub_tick_to_ms 的参数寄存器。
int ub_get_timer(int base)
{
    uint64_t ticks = ub_timer_read();
    uint32_t ms = (uint32_t)ub_tick_to_ms((uint32_t)ticks, (uint32_t)(ticks >> 32));

    return (int)ms - base;
}

// === ub_get_usecs === 原 FUN_040061fc @ 0x040061fc
// 来源: reversed/uboot_funcs/FUN_040061fc.c | 分类: uboot
// 语义: U-Boot get_usecs: 读 ticks 先乘 1000 (与 ub_tick_to_ms 内部相同的 ×1000
//       展开, 复用其除频), 返回微秒数。Ghidra 未建模返回值, 但结果按 ABI 经
//       r0/r1 传出 (调用方读取), 故转写为返回 uint64_t。
uint64_t ub_get_usecs(void)
{
    uint64_t ticks = ub_timer_read();
    uint64_t scaled = ticks * 1000;

    return ub_tick_to_ms((uint32_t)scaled, (uint32_t)(scaled >> 32));
}

// === ub_sprintf === 原 FUN_04007514 @ 0x04007514
// 来源: reversed/uboot_funcs/FUN_04007514.c | 分类: uboot
// 语义: sprintf 封装: ub_vsnprintf(buf, 0x7fffffff, fmt, args)。
//       反编译为 void (vsnprintf 返回值弃用); 可变实参经 r2/r3 压栈传递, 此处用
//       va_list 表达同一调用约定。
void ub_sprintf(char *buf, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    ub_vsnprintf(buf, 0x7fffffff, fmt, args);
    va_end(args);
}

// === ub_vprintf === 原 FUN_040075a0 @ 0x040075a0
// 来源: reversed/uboot_funcs/FUN_040075a0.c | 分类: uboot
// 语义: vprintf 封装: 格式化到 2072 字节栈缓冲 (ub_vsnprintf 上限 0x817), 返回
//       长度钳到 0x816, 经 func_0x03fc7fbc (puts 型, 命名待补) 整串输出。
uint32_t ub_vprintf(const char *fmt, va_list args)
{
    char buf[2072];
    uint32_t len = (uint32_t)ub_vsnprintf(buf, 0x817, fmt, args);

    if (0x815 < len)
        len = 0x816;
    func_0x03fc7fbc(buf);
    return len;
}

// === simple_strtoul === 原 FUN_04007718 @ 0x04007718
// 来源: reversed/uboot_funcs/FUN_04007718.c | 分类: uboot
// 语义: U-Boot simple_strtoul。base==0 时按前缀推断: "0x"+十六进制位 → 16 (忽略
//       传入 base), "0" 开头 → 8 (含 "0x" 后无十六进制位时按 8 从 '0' 后继续),
//       其余 → 10。逐位累计 result = result*base + digit: 数字 c-'0'; 其余按分类表
//       DAT_04007800 (U-Boot _ctype 布局: 位 0x04=数字, 0x02=小写字母, 0x40=十六进制):
//       大写 c-'7', 小写 (c-0x20)-'7'; 位值 >= base 或非数字/十六进制字符即止。
//       endp 非空时写停止位置 (指向终止字符); 溢出不检测。
uint32_t simple_strtoul(const char *cp, char **endp, uint32_t base)
{
    const uint8_t *p = (const uint8_t *)cp;
    uint32_t result = 0;
    uint8_t cur;
    uint8_t cls;

    if (*p == '0') {
        cur = p[1];
        p++;
        if (cur == 'x') {
            uint8_t c2 = p[1];                  /* cp[2] */
            if ((DAT_04007800[c2] & 0x44) != 0) {
                p++;                            /* 跳过 "0x" */
                cur = c2;
                cls = DAT_04007800[c2];
                base = 0x10;
                goto parse;                     /* cls 已过判别, 直接累计 */
            }
            /* "0x" 后无十六进制位 → 按 base 8 从 '0' 后继续 ('x' 处即止) */
        }
        if (base == 0)
            base = 8;
        cls = DAT_04007800[cur];
        goto check_first;
    }
    if (base == 0)
        base = 10;
    cur = *p;
    cls = DAT_04007800[cur];
check_first:
    if ((cls & 0x44) == 0)
        goto done;
parse:
    for (;;) {
        uint32_t digit = (uint32_t)cur - 0x30;
        if ((cls & 4) == 0) {
            digit = (uint32_t)cur - 0x37;
            if ((cls & 2) != 0)
                digit = ((uint32_t)cur - 0x20 & 0xff) - 0x37;
        }
        if (base <= digit)
            break;
        p++;
        cur = *p;
        result = base * result + digit;
        cls = DAT_04007800[cur];
        if ((cls & 0x44) == 0)
            break;
    }
done:
    if (endp != NULL)
        *endp = (char *)p;
    return result;
}

/* ==================== 覆盖率表 (batch=uboot_b3 前半, 24/24) ====================
 * FUN_040028a8 @ 0x040028a8 → ub_sha256_update          (原 42 行)
 * FUN_04002fbc @ 0x04002fbc → ub_un_02f0bc              (原 11 行)
 * FUN_04002fdc @ 0x04002fdc → ub_un_02f0dc              (原 17 行)
 * FUN_040030f8 @ 0x040030f8 → ub_key_list_match         (原 28 行)
 * FUN_040033fc @ 0x040033fc → ub_htab_find_entry        (原 34 行)
 * FUN_040034a8 @ 0x040034a8 → ub_hsearch                (原 207 行)
 * FUN_04003a68 @ 0x04003a68 → ub_hexport                (原 163 行)
 * FUN_04003d8c @ 0x04003d8c → ub_himport                (原 248 行)
 * FUN_040042ac @ 0x040042ac → ub_print_u64              (原 57 行)
 * FUN_04004a9c @ 0x04004a9c → ub_regex_prog_insert      (原 21 行)
 * FUN_04004af4 @ 0x04004af4 → ub_regex_emit_quant       (原 57 行)
 * FUN_0400583c @ 0x0400583c → ub_regex_match            (原 25 行)
 * FUN_04005994 @ 0x04005994 → ub_strcpy                 (原 18 行)
 * FUN_04005b14 @ 0x04005b14 → ub_strchr                 (原 21 行)
 * FUN_04005b98 @ 0x04005b98 → ub_strlen                 (原 17 行)
 * FUN_04005c20 @ 0x04005c20 → ub_strdup                 (原 35 行)
 * FUN_04005ea4 @ 0x04005ea4 → ub_memcpy                 (原 53 行)
 * FUN_04005ff4 @ 0x04005ff4 → ub_memcmp                 (原 29 行)
 * FUN_04006118 @ 0x04006118 → ub_tick_to_ms             (原 33 行)
 * FUN_040061e4 @ 0x040061e4 → ub_get_timer              (原 12 行)
 * FUN_040061fc @ 0x040061fc → ub_get_usecs              (原 19 行)
 * FUN_04007514 @ 0x04007514 → ub_sprintf                (原 14 行)
 * FUN_040075a0 @ 0x040075a0 → ub_vprintf                (原 16 行)
 * FUN_04007718 @ 0x04007718 → simple_strtoul            (原 64 行)
 *
 * 外部依赖 (定义在 uboot 其他批次文件):
 *   ub_sha256_transform(FUN_04000090)  ub_hdelete_entry(FUN_040030a8)
 *   ub_str_match(FUN_04003148)  ub_match_filter(FUN_04003210)
 *   ub_is_prime(FUN_04003300)  ub_hdestroy(FUN_0400339c)  ub_hdelete(FUN_04003954)
 *   ub_regex_exec(FUN_04004bd4)  ub_div64_rem(FUN_04004754)  ub_memset(FUN_04005e0c)
 *   ub_strcmp(FUN_04005a60)  ub_strncmp(FUN_04005a90)  ub_memmove(FUN_04005fa0)
 *   ub_timer_read(FUN_040061b0)  ub_vsnprintf(FUN_040069f0)  ub_printf(FUN_04007548)
 *   aeabi_uidiv(FUN_0400c66c)
 * 低地址区占位 (Ghidra func_0x, 命名待补):
 *   func_0x03ffae84  func_0x03ffafa8  func_0x03fc3288  func_0x03fc35fc
 *   func_0x03fc8570(free 型)  func_0x03fc87b0(malloc 型)  func_0x03fc8dc4(calloc 型)
 *   func_0x03fc7fbc(puts 型)  func_0x03ffd878(qsort 型)  func_0x03fb2008(时钟频率)
 */

/* ============================================================================
 * 覆盖率表
 * ========================================================================== */
