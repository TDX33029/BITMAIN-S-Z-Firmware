/*
 * rewritten/cgminer/axi.c — FPGA AXI 寄存器窗口层 + 归入 15_fpga_axi 分类的 API/初始化函数
 *
 * 来源: reversed/cgminer/15_fpga_axi/FUN_0000f8a4.c ~ _init.c (batch_plan: batch=axi_b1, 10 函数)
 * 范围:
 *   - AXI 窗口层: bitmain_axi_open / bitmain_axi_close / get_fpga_version / init_fpga /
 *     asic_baud_to_fpga_baud (mmap /dev/axi_fpga_dev 0x1200 字节 + /dev/fpga_mem 0x1000000 字节)
 *   - cgminer api.c 函数 (Ghidra 归入 15_fpga_axi 分类): escape_string_heap / write_config /
 *     apiversion
 *   - ELF 初始化桩: _init (ELF .init 入口) / _init_0 (.init_array 构造器, 体内不可达块已被裁剪)
 *
 * 事实还原 (全部以反编译 + 机器码反汇编为证):
 *   - 寄存器窗口偏移为"字节偏移": 机器码实测 init_fpga `str r3,[r2,#0x80]`、set_baud_pl
 *     `str r3,[r2,#0x3c]`、uart RX 计数 `ldr r3,[r3,#0x200]`、命令通道 `str r7,[r3,#0x30]`。
 *     fpga_axi.h 头注"偏移为 u32 索引"与机器码不符, 本文件按"字节偏移"经 AXI_REG32() 访问
 *     (宏数值本身正确, 仅索引口径修正)。
 *   - init_fpga: 写魔数 0x8080800F 后 do-while 轮询 (先睡 10ms 再查 bit31, 为 1 即继续忙等),
 *     退出后调 get_fpga_version() 并睡 100ms。
 *   - get_fpga_version: base[0] |= 0x60000000 触发硬件版本读出, 版本 = base[0] 低 16 位
 *     (打印 "%04X"); 打印受 applog 门控开关/级别控制 (三重条件原样保留)。
 *   - escape_string_heap: 仅转义 '\' 与 '"'; 每次分配 malloc(len*2+1) 缓冲 + 8 字节链节点,
 *     头插到全局链 DAT_00076bd8; write_config 末尾统一释放整条链。
 *   - write_config: pools 数组 (url 池 / quota 池二选一, 代理经 proxytype() 还原前缀) +
 *     opt_table 选项表遍历 (7 字/项, 0x00075600 起, 终止项 flags==0x10, 表数据已从
 *     analysis/cgminer_z15_bm1746.elf .data 段核实) + 挖矿策略四分支 + 释放转义链。
 *   - write_config 反编译中 escape_string_heap 两处调用实参被 Ghidra 丢失, 已按机器码恢复:
 *     0x00015bbc 处 r0 = proxytype() 返回值, 0x00015bd0 处 r0 = pool->rpc_proxy, 
 *     0x00015e7c 处 r0 = *(char **)选项变量指针。
 *   - write_config 选项循环内对 pool->rpc_proxy 的重复重读 (+0xd8 三次) 为编译器无法证明
 *     escape_string_heap/malloc 别名导致的重载伪影, escape_string_heap 只分配新堆块不改写
 *     pool 结构, 单次判定语义等价, 已合并为一次判定。
 *   - apiversion: message(MSG_VERSION=0x16) 状态头 + CGMiner/API/Miner/CompileTime/Type 五项;
 *     JSON 分支 io_add 返回非 0 时置 io_data->close (+0x0d)。
 */
#include <stdint.h>
#include <stdio.h>      /* printf/fprintf/fwrite/snprintf/puts */
#include <stdlib.h>     /* malloc/free */
#include <string.h>     /* strlen/strtok/__strdup */
#include <fcntl.h>      /* open */
#include <unistd.h>     /* close */
#include <sys/mman.h>   /* mmap/munmap */

#include "../include/common_types.h"
#include "../include/fpga_axi.h"

/* ================= 外部依赖 (定义于其他批次文件, name_map/cgminer.tsv 命名) ================= */

extern void *priority_pool(int idx);                        /* 原 FUN_00012038, 按优先级取池 */
extern void  applog(uint32_t prio, char *buf, ...);  /* 原 FUN_00038730 (第 3 实参按反编译保留) */
extern void  cgsleep_us(int64_t us);                        /* 原 FUN_0002a954 (反编译形参为 64 位实参 r0/r1 拆分) */
extern char *proxytype(int type);                           /* 原 FUN_00029690, 代理/URL 类型 → 前缀串 */

extern void  message(void *io_data, int code, char *param, int p4, int isjson);   /* 原 FUN_0003305c */
extern int   io_add(void *io_data, const char *buf);        /* 原 FUN_000307c8 */
extern void *api_add_string(void *root, const char *name, const char *value, int copy);  /* 原 FUN_00032e1c */
extern void *api_add_const(void *root, const char *name, const char *value, int copy);   /* 原 FUN_00032e40 */
extern void  print_data(void *io_data, void *root, int isjson, int p4);                  /* 原 FUN_00032534 */

extern int   __gmon_start__(void);                          /* 原 FUN_0000f644, gmon 桩 */
extern void  opt_set_floatval_range_125_500(char *arg);     /* 原 FUN_00012200 */
extern void  opt_set_floatval_range_100_250(char *arg);     /* 原 FUN_000121b0 */

/* ---- 其他批次定义的全局 (原 DAT_ 地址, 首次出现处注释语义) ---- */
extern int    DAT_0007b198;    /* total_pools: 已配置矿池数 */
extern int    DAT_00076efc;    /* 挖矿策略: 1=round-robin 2=rotate 3=load-balance 4=balance */
extern int    DAT_0007b558;    /* opt_rotate_interval: rotate 轮换间隔 (分钟) */
extern char  *DAT_0009d784;    /* --logfile/--version-file 等选项共用的串变量;
                                  其地址亦作 opt_table 描述字段的"跳过"哨兵 (见 OPT_OFF_DESC) */
extern int    DAT_00076f14;    /* 池类隐藏选项 (--pass/--url/--user/--userpass/--quota/--rotate/
                                  --pools) 的占位变量, write_config 据此排除其输出 */
extern char   DAT_0007ad48;    /* applog 门控开关 A (opt_debug/opt_log_output 之一, 语义待定) */
extern char   DAT_00078e08;    /* applog 门控开关 B */
extern int    DAT_000766c4;    /* applog 日志级别 (opt_log_level, > 4 才放行低级别日志) */
extern char   DAT_00078750[];  /* Miner compile time 串缓冲 (FUN_0000b928 填充) */
extern char   DAT_00078850[];  /* Miner type 串缓冲 */
extern char   DAT_00078b94[];  /* Miner 版本串缓冲 (FUN_00049cc8 初值 "9.0.") */
extern uint32_t DAT_0007561c;  /* opt_table 第 1 项 (0x0007561c = 表基 0x00075600 + 28 字节) */

/* ================= 本模块全局 (其他批次经 extern 引用) ================= */

int                DAT_0007bd90;   /* /dev/axi_fpga_dev 文件描述符 */
volatile uint32_t *DAT_00078e10;   /* AXI 寄存器窗口基址 (mmap 0x1200 字节) */
int                DAT_0007c0e8;   /* /dev/fpga_mem 文件描述符 */
void              *DAT_00078eb8;   /* DDR 工作下发窗口基址 (mmap 0x1000000 字节) */

/* escape_string_heap 缓冲跟踪链节点 (8 字节: node[0]=缓冲, node[1]=后继) */
typedef struct escape_node {
    char              *buf;         /* +0x00: 转义后的字符串缓冲 */
    struct escape_node *next;       /* +0x04: 链表后继 */
} escape_node_t;

escape_node_t *DAT_00076bd8 = NULL; /* 转义缓冲跟踪链表头 (write_config 末尾统一释放) */

/* ================= 寄存器访问与局部常量 ================= */

/* fpga_axi.h 的 AXI_REG_* 数值为窗口内字节偏移 (机器码核实口径见文件头), 统一经此按 u32 读写 */
#define AXI_REG32(off)   (*(volatile uint32_t *)((volatile uint8_t *)DAT_00078e10 + (off)))

#define AXI_VERSION_MASK 0xffffu   /* 版本号 = base[0] 低 16 位 (uxth) */

/* struct pool 相关字段偏移 (字节; 由 write_config 访问点锁定, 布局与 cgminer pool 一致) */
#define POOL_OFF_QUOTA     0x38   /* quota (int; ==1 时按 url 池写, 否则按 quota 池写) */
#define POOL_OFF_RPC_URL   0xc4   /* rpc_url */
#define POOL_OFF_RPC_USER  0xcc   /* rpc_user */
#define POOL_OFF_RPC_PASS  0xd0   /* rpc_pass */
#define POOL_OFF_PROXYTYPE 0xd4   /* proxytype (代理类型枚举, 传 proxytype() 取前缀) */
#define POOL_OFF_RPC_PROXY 0xd8   /* rpc_proxy (代理地址串, NULL = 无代理) */

/* api io_data 结构 (偏移证据: apiversion 写 +0x0d; io_add 使用 +0x0/+0x4/+0x8) */
#define API_IO_DATA_CLOSE_OFF 0x0d  /* close 标志字节 */

/* ---- opt_table 选项表 (原 DAT_00075600 起, 7 个 u32/项; 数据已从 ELF .data 核实) ---- */
#define OPT_OFF_NAME       0   /* 选项名, 含 '|' 分隔别名 (如 "--debug|-D") */
#define OPT_OFF_FLAGS      1   /* 标志: bit0=无参, bit1=有参; 0x10 = 表终止哨兵 */
#define OPT_OFF_NOARG_CB   2   /* 无参选项 setter */
#define OPT_OFF_ARG_CB     3   /* 有参选项 setter */
#define OPT_OFF_SHOW       4   /* show 函数 (int 选项为 0x0005b4c8, 写 config 不使用) */
#define OPT_OFF_VALUE      5   /* 选项变量指针 (池类隐藏选项用 &DAT_00076f14 占位) */
#define OPT_OFF_DESC       6   /* 描述串; == &DAT_0009d784 哨兵的选项跳过不写
                                *   (--hotplug/--pools/--retries/--retry-pause/--no-pool-disable) */
#define OPT_TABLE_STRIDE      7      /* 每项 7 个 u32 (28 字节) */
#define OPT_TABLE_END_FLAGS   0x10u  /* 终止项 flags 值 (终止项其余字段全 0, 位于 0x00075da8) */

/* 无参布尔选项 setter 哨兵 (表项内保存的函数地址; 两个小型 setter 未单独编目, 命名待补):
 * 0x0005b0fc = opt_set_bool 语义 (置 true), 0x0005b10c = opt_set_invbool 语义 (置 false,
 * 用于 --no-* 选项); 0x0005f740(--balance)/0x0005f770(--debug) 为特殊布尔 setter,
 * 不属于两哨兵, 因此这两个选项在选项循环中不输出 (balance 由策略分支单独处理)。 */
#define OPT_SET_BOOL_SENTINEL    0x0005b0fcu
#define OPT_SET_INVBOOL_SENTINEL 0x0005b10cu

/* 整型选项 setter 地址表: 0x0005b2e8 = 通用 opt_set_intval (name_map);
 * 0x000154c8..0x00015540 为 api.c 区各整型选项的专用 set 包装函数
 * (如 --api-port=0x000154e0, --bitmain-fan-pwm=0x000154d4, --bitmain-freq/--bitmain-voltage/
 *  --bitmain-core-timeout=0x000154c8), Ghidra 未单独命名。命中任一 → 以 "%d" 输出。 */
static const uint32_t intval_setter_addrs[12] = {
    0x0005b2e8u, 0x000154c8u, 0x000154e0u, 0x00015540u, 0x00015534u, 0x000154d4u,
    0x00015528u, 0x0001551cu, 0x00015510u, 0x00015504u, 0x000154f8u, 0x000154ecu
};

/* 取表项第 5 字保存的选项变量指针 */
#define OPT_VALUE_PTR(e)  ((void *)(uintptr_t)(e)[OPT_OFF_VALUE])

/* 整型 setter 判定 */
static int is_intval_setter(uint32_t cb)
{
    int i;
    for (i = 0; i < 12; i++) {
        if (intval_setter_addrs[i] == cb)
            return 1;
    }
    return 0;
}

/* ================= _init / _init_0 (ELF 初始化桩) ================= */

// === _init === 原 FUN_0000aa1c @ 0x0000aa1c
// 来源: reversed/cgminer/15_fpga_axi/_init.c | 分类: 15_fpga_axi
// 语义: ELF .init 节入口, 调用 gmon 桩并透传其返回值 (r0); Ghidra 所标 EVP_PKEY_CTX
//       形参为类型噪音, 实际无参。
int _init(void)
{
    return __gmon_start__();   /* 原 FUN_0000f644: 仅转发外部 __gmon_start__ 的桩 */
}

// === _init_0 === 原 FUN_0000f708 @ 0x0000f708 (Ghidra 标签 _INIT_0)
// 来源: reversed/cgminer/15_fpga_axi/_INIT_0.c | 分类: 15_fpga_axi
// 语义: .init_array 构造器, 函数体为空; 原体内 0x0000f6c0..0x0000f728 的不可达块
//       已被 Ghidra 裁剪 (反编译文件头部有对应 WARNING 注记)。
void _init_0(void)
{
    return;
}

/* ================= cgminer api.c 函数 (escape_string_heap / write_config / apiversion) ================= */

// === escape_string_heap === 原 FUN_0000f8a4 @ 0x0000f8a4
// 来源: reversed/cgminer/15_fpga_axi/FUN_0000f8a4.c | 分类: 15_fpga_axi
// 语义: 堆上转义字符串副本 — 仅给 '\' 与 '"' 前加反斜杠; 缓冲 malloc(2*len+1), 并把
//       缓冲指针头插进全局跟踪链 DAT_00076bd8 (write_config 末尾统一 free), 返回缓冲。
char *escape_string_heap(char *str)
{
    size_t len = strlen(str);
    char *out = malloc(len * 2 + 1);
    escape_node_t *node = malloc(8);
    char c = *str;
    char *dst = out;
    const char *src = str;

    node->buf = out;                 /* node[0] = 缓冲指针 */
    node->next = DAT_00076bd8;       /* node[1] = 原链头 */
    DAT_00076bd8 = node;             /* 头插 */

    if (c != '\0') {
        do {
            if (c == '\\' || c == '\"')
                *dst++ = '\\';       /* 转义: 前插反斜杠 */
            *dst++ = *src++;
            c = *src;
        } while (c != '\0');
    }
    *dst = '\0';
    return out;
}

// === write_config === 原 FUN_00015b64 @ 0x00015b64
// 来源: reversed/cgminer/15_fpga_axi/FUN_00015b64.c | 分类: 15_fpga_axi
// 语义: 把当前配置写回 config 文件 — "{\n\"pools\" : [" 起头的 pools 数组 (每池按
//       quota==1 走 "url" 或走 "quota" 模板, 代理经 proxytype() 还原前缀与 '|' 分隔),
//       随后 "\n]\n"、opt_table 选项遍历 (布尔→true, 整型→"%d", 浮点→"%.1f", 其余→
//       转义串)、挖矿策略四分支、"\n}\n", 最后释放本文件所有 escape_string_heap 缓冲。
void write_config(FILE *fcfg)
{
    int i;

    fwrite("{\n\"pools\" : [", 1, 13, fcfg);              /* 原 &DAT_00060700 (13 字节) */

    if (DAT_0007b198 > 0) {
        for (i = 0; i < DAT_0007b198; i++) {
            void *pool = priority_pool(i);
            int quota = *(int *)((char *)pool + POOL_OFF_QUOTA);
            const char *sep = (i == 0) ? "" : ",";       /* 首池前无逗号 */

            if (quota == 1) {
                /* URL 池: "url" : "<代理前缀><代理地址><|><URL>" (无代理时三段全空) */
                const char *prefix_esc = "";
                const char *proxy_esc = "";
                const char *pipe = "";

                if (*(char **)((char *)pool + POOL_OFF_RPC_PROXY) != NULL) {
                    prefix_esc = escape_string_heap(
                        proxytype(*(int *)((char *)pool + POOL_OFF_PROXYTYPE)));
                    proxy_esc = escape_string_heap(
                        *(char **)((char *)pool + POOL_OFF_RPC_PROXY));
                    pipe = "|";
                }
                {
                    char *url_esc = escape_string_heap(
                        *(char **)((char *)pool + POOL_OFF_RPC_URL));
                    fprintf(fcfg, "%s\n\t{\n\t\t\"url\" : \"%s%s%s%s\",",
                            sep, prefix_esc, proxy_esc, pipe, url_esc);
                }
            } else {
                /* quota 池: "quota" : "<代理前缀><代理地址><|><配额>;<URL>" */
                const char *prefix_esc = "";
                const char *proxy_esc = "";
                const char *pipe = "";

                if (*(char **)((char *)pool + POOL_OFF_RPC_PROXY) != NULL) {
                    prefix_esc = escape_string_heap(
                        proxytype(*(int *)((char *)pool + POOL_OFF_PROXYTYPE)));
                    proxy_esc = escape_string_heap(
                        *(char **)((char *)pool + POOL_OFF_RPC_PROXY));
                    pipe = "|";
                }
                {
                    char *url_esc = escape_string_heap(
                        *(char **)((char *)pool + POOL_OFF_RPC_URL));
                    fprintf(fcfg, "%s\n\t{\n\t\t\"quota\" : \"%s%s%s%d;%s\",",
                            sep, prefix_esc, proxy_esc, pipe, quota, url_esc);
                }
            }
            fprintf(fcfg, "\n\t\t\"user\" : \"%s\",",
                    escape_string_heap(*(char **)((char *)pool + POOL_OFF_RPC_USER)));
            fprintf(fcfg, "\n\t\t\"pass\" : \"%s\"\n\t}",
                    escape_string_heap(*(char **)((char *)pool + POOL_OFF_RPC_PASS)));
        }
    }

    fwrite("\n]\n", 1, 3, fcfg);                          /* 原 &DAT_00060710 (3 字节) */

    /* ---- opt_table 选项遍历 ----
     * 遍历指针指向"当前项+1 项"的名字字段 (反编译变量形态), 等价于从表基 0x00075600
     * 按 28 字节步进; 表终止哨兵 = 该项 flags == 0x10。首次外层判断即首项 flags
     * (原对 DAT_00075604 的判断)。 */
    {
        uint32_t *opt = &DAT_0007561c - OPT_TABLE_STRIDE; /* 当前项 = 0x00075600 */

        if (opt[OPT_OFF_FLAGS] != OPT_TABLE_END_FLAGS) {
            do {
                const char *name_copy = __strdup((const char *)opt[OPT_OFF_NAME]);
                char *alias = strtok((char *)name_copy, "|");

                while (alias != NULL) {
                    /* 只写长名别名 ("-x" 短名跳过), 且描述字段 == &DAT_0009d784
                     * 哨兵的选项跳过 (池类等由上方 pools 段单独处理) */
                    if (alias[1] == '-' &&
                        opt[OPT_OFF_DESC] != (uint32_t)(uintptr_t)&DAT_0009d784) {
                        uint32_t flags = opt[OPT_OFF_FLAGS];
                        uint32_t noarg_cb = opt[OPT_OFF_NOARG_CB];
                        void *val = OPT_VALUE_PTR(opt);

                        /* 布尔选项: 无参 + setter 为 opt_set_bool/opt_set_invbool 哨兵
                         * + 当前值与 setter 语义真值一致 → 输出 true */
                        if ((flags & 1) != 0 &&
                            (noarg_cb == OPT_SET_BOOL_SENTINEL ||
                             noarg_cb == OPT_SET_INVBOOL_SENTINEL) &&
                            ((*(char *)val != 0) == (noarg_cb == OPT_SET_BOOL_SENTINEL))) {
                            fprintf(fcfg, ",\n\"%s\" : true", alias + 2);
                        } else if ((flags & 2) != 0) {
                            /* 有参选项按 setter 类型分派输出格式 */
                            uint32_t arg_cb = opt[OPT_OFF_ARG_CB];

                            if (is_intval_setter(arg_cb)) {
                                fprintf(fcfg, ",\n\"%s\" : \"%d\"", alias + 2, *(int *)val);
                            } else if (arg_cb == (uint32_t)(uintptr_t)opt_set_floatval_range_125_500 ||
                                       arg_cb == (uint32_t)(uintptr_t)opt_set_floatval_range_100_250) {
                                fprintf(fcfg, ",\n\"%s\" : \"%.1f\"", alias + 2,
                                        (double)*(float *)val);
                            } else {
                                goto write_string_opt;   /* 其余 setter (charp 等) 走字符串路径
                                                          * (原地址 0x00015e5c, 两条路径共用) */
                            }
                        } else {
write_string_opt:
                            /* 字符串路径: 仅"有参"标志命中, 排除占位变量 DAT_00076f14,
                             * 且变量内指针非空才输出 (机器码核实第 3 实参 = *(char **)val) */
                            if (((flags & 6) != 0) &&
                                (val != (void *)&DAT_00076f14) &&
                                (*(int *)val != 0)) {
                                fprintf(fcfg, ",\n\"%s\" : \"%s\"", alias + 2,
                                        escape_string_heap(*(char **)val));
                            }
                        }
                    }
                    alias = strtok(NULL, "|");
                }
                free((void *)name_copy);
                opt += OPT_TABLE_STRIDE;
            } while (opt[OPT_OFF_FLAGS] != OPT_TABLE_END_FLAGS);
        }
    }

    /* ---- 挖矿策略 (balance/load-balance/round-robin 输出 true, rotate 输出间隔) ---- */
    if (DAT_00076efc == 4) {
        fwrite(",\n\"balance\" : true", 1, 0x12, fcfg);
    }
    if (DAT_00076efc == 3) {
        fwrite(",\n\"load-balance\" : true", 1, 0x17, fcfg);
    }
    if (DAT_00076efc == 1) {
        fwrite(",\n\"round-robin\" : true", 1, 0x16, fcfg);
    }
    if (DAT_00076efc == 2) {
        fprintf(fcfg, ",\n\"rotate\" : \"%d\"", DAT_0007b558);
    }

    fwrite("\n}\n", 1, 3, fcfg);                          /* 原 &DAT_00060814 (3 字节) */

    /* ---- 释放本次写配置期间 escape_string_heap 累积的全部缓冲 ---- */
    {
        escape_node_t *node = DAT_00076bd8;
        DAT_00076bd8 = NULL;
        while (node != NULL) {
            escape_node_t *next = node->next;
            free(node->buf);
            free(node);
            node = next;
        }
    }
}

// === apiversion === 原 FUN_00034c60 @ 0x00034c60
// 来源: reversed/cgminer/15_fpga_axi/FUN_00034c60.c | 分类: 15_fpga_axi
// 语义: API "version" 命令处理 — 先发 message(MSG_VERSION=0x16) 状态头, 再按普通/JSON
//       两种形态追加 CGMiner/API/Miner/CompileTime/Type 五项并由 print_data 输出;
//       JSON 分支 io_add(",\"VERSION\":[") 返回非 0 时置 io_data->close (+0x0d)。
//       注: 第 2/3 形参未用, 第 4 形参 (isjson) 为 JSON 分支判据 (按反编译)。
void apiversion(void *io_data, void *param, void *param_unused, int isjson)
{
    void *root;

    message(io_data, 0x16, NULL, 0, isjson);

    if (isjson == 0) {
        io_add(io_data, "VERSION,");
        root = api_add_string(NULL, "CGMiner", "4.9.0", 0);
        root = api_add_const(root, "API", "3.1", 0);            /* 原 &DAT_00060570/"API", &DAT_000671e4/"3.1" */
        root = api_add_string(root, "Miner", DAT_00078b94, 0);
        root = api_add_string(root, "CompileTime", DAT_00078750, 0);
        root = api_add_string(root, "Type", DAT_00078850, 0);   /* 原 &DAT_000671fc/"Type" */
        print_data(io_data, root, 0, 0);
        return;
    }

    {
        int first = io_add(io_data, ",\"VERSION\":[");
        root = api_add_string(NULL, "CGMiner", "4.9.0", 0);
        root = api_add_const(root, "API", "3.1", 0);
        root = api_add_string(root, "Miner", DAT_00078b94, 0);
        root = api_add_string(root, "CompileTime", DAT_00078750, 0);
        root = api_add_string(root, "Type", DAT_00078850, 0);
        print_data(io_data, root, 1, 0);
        if (first != 0) {
            *(uint8_t *)((uint8_t *)io_data + API_IO_DATA_CLOSE_OFF) = 1;
        }
    }
}

/* ================= FPGA AXI 寄存器窗口层 ================= */

// === bitmain_axi_open === 原 FUN_0003b704 @ 0x0003b704
// 来源: reversed/cgminer/15_fpga_axi/FUN_0003b704.c | 分类: 15_fpga_axi
// 语义: 打开并映射 FPGA 窗口 — open("/dev/axi_fpga_dev") + mmap 0x1200 字节寄存器窗口,
//       打印基址与首字 (版本); 再 open("/dev/fpga_mem") + mmap 0x1000000 字节 DDR 工作下发
//       窗口; 任一步失败打印并返回 -1, 全部成功返回 0。
int bitmain_axi_open(void)
{
    DAT_0007bd90 = open(AXI_FPGA_DEV_PATH, 2 /* O_RDWR */);
    if (DAT_0007bd90 < 0) {
        printf("/dev/axi_fpga_dev open failed. fd = %d\n", DAT_0007bd90);
        return -1;
    }

    DAT_00078e10 = mmap(NULL, AXI_WINDOW_SIZE, 3 /* PROT_READ|PROT_WRITE */,
                        1 /* MAP_SHARED */, DAT_0007bd90, 0);
    if (DAT_00078e10 == NULL) {
        printf("mmap axi_fpga_addr failed. axi_fpga_addr = 0x%x\n", 0);
        return -1;
    }
    printf("mmap axi_fpga_addr = 0x%x\n", (unsigned)(uintptr_t)DAT_00078e10);
    printf("axi_fpga_addr data = 0x%x\n", (unsigned)AXI_REG32(AXI_REG_VERSION));

    DAT_0007c0e8 = open(FPGA_MEM_DEV_PATH, 2 /* O_RDWR */);
    if (DAT_0007c0e8 < 0) {
        printf("/dev/fpga_mem open failed. fd_fpga_mem = %d\n", DAT_0007c0e8);
        return -1;
    }

    DAT_00078eb8 = mmap(NULL, DDR_WINDOW_SIZE, 3 /* PROT_READ|PROT_WRITE */,
                        1 /* MAP_SHARED */, DAT_0007c0e8, 0);
    if (DAT_00078eb8 == NULL) {
        printf("mmap fpga_mem_addr failed. fpga_mem_addr = 0x%x\n", 0);
        return -1;
    }
    printf("mmap fpga_mem_addr = 0x%x\n", (unsigned)(uintptr_t)DAT_00078eb8);
    return 0;
}

// === bitmain_axi_close === 原 FUN_0003b844 @ 0x0003b844
// 来源: reversed/cgminer/15_fpga_axi/FUN_0003b844.c | 分类: 15_fpga_axi
// 语义: 解除两处映射 (失败各打印一次 "munmap failed!") 并关闭两个设备描述符。
void bitmain_axi_close(void)
{
    int ret;

    printf("\n\n--- %s\n", "bitmain_axi_close");

    ret = munmap((void *)DAT_00078e10, AXI_WINDOW_SIZE);
    if (ret < 0) {
        puts("munmap failed!");
    }
    ret = munmap(DAT_00078eb8, DDR_WINDOW_SIZE);
    if (ret < 0) {
        puts("munmap failed!");
    }
    close(DAT_0007bd90);
    close(DAT_0007c0e8);
}

// === get_fpga_version === 原 FUN_0003b994 @ 0x0003b994
// 来源: reversed/cgminer/15_fpga_axi/FUN_0003b994.c | 分类: 15_fpga_axi
// 语义: base[0] |= 0x60000000 触发硬件版本读出, 读回 base[0] 低 16 位即 FPGA 版本,
//       以 "FPGA version: %04X" 经 applog(5, ...) 打印; 打印前按日志门控三重条件
//       (开关 A / 开关 B / 级别 > 4) 判定 (applog 内部还有同等门控)。
void get_fpga_version(void)
{
    char buf[4096];                                          /* 反编译栈缓冲 4096 字节 */

    AXI_REG32(AXI_REG_VERSION) |= AXI_HW_VERSION_SET;

    if ((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0') || (4 < DAT_000766c4)) {
        snprintf(buf, 0x1000, "FPGA version: %04X",
                 (unsigned)AXI_REG32(AXI_REG_VERSION) & AXI_VERSION_MASK);
        applog(5, buf, 0);
    }
}

// === init_fpga === 原 FUN_0003ba70 @ 0x0003ba70
// 来源: reversed/cgminer/15_fpga_axi/FUN_0003ba70.c | 分类: 15_fpga_axi
// 语义: PL 初始化 — base[0x80] 写魔数 0x8080800F, do-while 每 10ms 轮询一次直到该寄存器
//       bit31 (忙位) 清零, 随后调 get_fpga_version() 读出并打印版本, 再睡 100ms 收尾。
void init_fpga(void)
{
    printf("\n--- %s\n", "init_fpga");

    AXI_REG32(AXI_REG_INIT_MAGIC_OFF) = AXI_INIT_MAGIC;
    do {
        cgsleep_us(10000);
    } while ((int)AXI_REG32(AXI_REG_INIT_MAGIC_OFF) < 0);   /* bit31 (AXI_BUSY_BIT31) 置位 = 忙 */

    get_fpga_version();
    cgsleep_us(100000);
}

// === asic_baud_to_fpga_baud === 原 FUN_0003bb00 @ 0x0003bb00
// 来源: reversed/cgminer/15_fpga_axi/FUN_0003bb00.c | 分类: 15_fpga_axi
// 语义: ASIC 波特率 → PL 分频参数校验 — 只接受 0 / 1 / 0x1A (上电默认, 与
//       AXI_BAUD_PL_INITIAL 一致) 并原样返回; 其余值打印报错并返回 0x35 (53) 表示不支持。
uint32_t asic_baud_to_fpga_baud(int baud)
{
    if (baud == 1) {
        return 1;
    }
    if (baud == 0) {
        return 0;
    }
    if (baud == 0x1a) {
        return 0x1a;
    }
    printf("%s: Don't support ASIC baud = %d, error!!!\n", "asic_baud_to_fpga_baud", baud);
    return 0x35;
}

/*
 * ---- 覆盖率表 (batch=axi_b1, 10/10) ----
 * 函数                    原地址       反编译行数  横幅行  一句话语义
 * _init                   0x0000aa1c  11          156     ELF .init 入口, 调 __gmon_start__ 桩并透传返回值
 * _init_0 (_INIT_0)       0x0000f708  13          165     .init_array 空构造器 (不可达块已裁剪)
 * escape_string_heap      0x0000f8a4  39          176     '\'/'"' 转义堆副本, 头插 DAT_00076bd8 跟踪链
 * write_config            0x00015b64  159         205     写回 cgminer 配置: pools + opt_table 选项 + 策略 + 释放转义链
 * apiversion              0x00034c60  32          362     API version 命令: 状态头 + CGMiner/API/Miner/CompileTime/Type
 * bitmain_axi_open        0x0003b704  42          401     open+mmap axi_fpga_dev(0x1200) 与 fpga_mem(0x1000000)
 * bitmain_axi_close       0x0003b844  21          439     解除两处映射并关闭两个设备描述符
 * get_fpga_version        0x0003b994  15          460     base[0]|=0x60000000, 低 16 位为版本 ("%04X" 打印)
 * init_fpga               0x0003ba70  15          478     base[0x80]=0x8080800F 后轮询 bit31 清零, 随后读版本
 * asic_baud_to_fpga_baud  0x0003bb00  18          495     波特率校验: 仅 0/1/0x1A 原样通过, 否则报错返回 0x35
 */
