/*
 * cgminer 20_job_mining 任务层第 5b 段 (job_p5 停滞续作, 16 函数) — BM1746 固件语义转写
 * =========================================================
 * 原始二进制: bm1746/cgminer_z15_bm1746.elf (ARM32, SD 镜像 cgminer-z15, branch "bm1746")
 * 来源:      reversed/cgminer/20_job_mining/ | 命名: rewritten/name_map/cgminer.tsv
 *
 * 本文件承接 job_p5.c (批次 job_b5) 停滞后的剩余 16 个函数, 按主题分 3 组:
 *   [E] 设备使能 / 工作队列 / stratum extranonce / 设备失败记录:
 *       enable_device parse_extranonce_equihash tq_push record_device_fail
 *   [F] sha256 终结原语 / API COIN 查询 / 日志枢纽 / 内核链表 / BM1740 帧构造:
 *       sha256_ctx_final minecoin applog k_get bm1740_makeup_work
 *   [G] jansson dump 辅助 / json_object_update 系列 / ccan-opt 注册 / libgcc 除法:
 *       dump_indent jsonp_dtostr json_object_update_missing json_object_update
 *       json_object_update_existing opt_register_table __udivsi3
 *
 * 关键机器语义 (与 job_p5.c 同款结论, ARM 反汇编核实):
 *   - applog 为全二进制日志枢纽 (~150 调用者): 门控 A (DAT_0007ad48) 非零时改走
 *     syslog(prio|0x80 = LOG_LOCAL0 设施); 否则拼 " [y-m-d h:m:s] " 时间戳 → stderr
 *     (非 tty 才打印) → 日志文件 (DAT_00078ce0 使能, 惰性 fopen) → _applog 分发。
 *   - record_device_fail: cgpu+0xf4 记 time_t, +0xf8 记原因码, +0xfc..+0x120 为
 *     10 个按原因计数器; watchdog 路径传 3=SICK/4=DEAD [上游名低置信]。
 *   - bm1740_makeup_work: 仅接受 len==0x92 的工作帧: 写 TX 前导 55 AA, byte[2]
 *     高 3 位置 0b001 (&0x1f|0x20), 对 frame[2..0x90) 按 DAT_00068aec 表逐字节
 *     CRC16, 结果字节序翻转存 frame+0x90; 返回 0x92, 长度不符返回 0。
 *   - __udivsi3 返回 (r1<<32)|r0: 低 32 位恒为商; 高 32 位在主循环路径为余数
 *     (调用点 __umodsi3 返回的 r1 即 jansson 桶索引 hash%prime 已核实),
 *     快速路径高 32 位为除数残值 [B3 低置信]。
 *   - jsonp_dtostr 返回值为串长 (int, Ghidra 误标 char*), -1 表示缓冲不足。
 *
 * 转写约定: rewritten/CONVENTIONS.md
 *   - 被调函数名一律取自 name_map; 未知 DAT_xxx 全局保留原名 + 注释。
 *   - 日志字符串原样保留; Ghidra 伪变量已消除, 偏移以 P8/LD 宏 + 注释标明。
 */

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <syslog.h>
#include <sys/time.h>

/* ==================== 基址+偏移 助记宏 (结构布局未完全还原, 偏移来自反编译) ==================== */

#define P8(p, off)   ((char *)(p) + (off))                      /* 基址 + 字节偏移 */
#define LD32(p, off) (*(uint32_t *)P8(p, off))
#define LDs32(p, off) (*(int32_t *)P8(p, off))
#define LD8(p, off)  (*(uint8_t *)P8(p, off))
#define LDptr(p, off) (*(void **)P8(p, off))

/* struct pool 字段偏移 (int* 索引 ×4 = 字节偏移) */
#define PL_MUTEX        0x0f4  /* pthread_mutex_t 数据互斥锁 (param_1+0x3d) */
#define PL_RWLOCK       0x10c  /* pthread_rwlock_t 数据读写锁 (param_1+0x43) */
#define PL_NONCE1       0x27c  /* char* extranonce1 hex 串 (param_1+0x9f) */
#define PL_NONCE1BIN    0x280  /* void* extranonce1 二进制 (param_1+0xa0) */
#define PL_N1F_288      0x288  /* int (param_1+0xa2, 清零, 语义待定) */
#define PL_N1F_28C      0x28c  /* int (param_1+0xa3, 清零, 语义待定) */
#define PL_NONCE2OFF    0x290  /* int = 0x40 - nonce1len (param_1+0xa4) */
#define PL_NONCE1LEN    0x634  /* int extranonce1 字节长 = strlen/2 (param_1+0x18d) */

/* struct cgpu_info 字段偏移 */
#define CG_DEV_ID       0x08   /* int 设备序号 (enable_device 分配) */
#define CG_DEVEN        0x20   /* int 0=启用/1=禁用 (enable_device 置 0) */
#define CG_WEIGHT       0x94   /* int 设备权重 (param_1[0x25]) */
#define CG_FAIL_TIME    0x0f4  /* time_t record_device_fail 记时 */
#define CG_FAIL_REASON  0x0f8  /* int 失败原因码 */
#define CG_FAIL_CNT0    0x0fc  /* int[10] 按原因计数器 (0xfc..0x120) */
#define CG_QLOCK        0x140  /* pthread_rwlock_t (enable_device 初始化) */
#define CG_QUEUE_HEAD   0x160  /* 队列首工作指针 (enable_device 清零) */

/* struct thread_q 字段偏移 (tq_push; 队列为侵入式双向链表) */
#define TQ_HEAD_NEXT    0x000  /* list_head head.next (首节点) */
#define TQ_HEAD_PREV    0x004  /* list_head head.prev (尾节点) */
#define TQ_DIE          0x008  /* char 停机标志 */
#define TQ_LOCK         0x00c  /* pthread_mutex_t */
#define TQ_COND         0x028  /* pthread_cond_t */

/* struct tq_ent 字段偏移 (0xc 字节; 链表节点 = &ent->list, 即 ent+4) */
#define TQE_DATA        0x000  /* void* 数据 */
#define TQE_NODE_NEXT   0x004  /* list.next (自指初始化) */
#define TQE_NODE_PREV   0x008  /* list.prev */

/* struct klist / API 项链表偏移 (k_get; cgminer api.c klist) */
#define KL_STOP         0x004  /* char 无后备标志 (置位且无当前项 → 返回 0) */
#define KL_CUR          0x00c  /* 当前项指针 */
#define KL_F_10         0x010  /* int (不足项时清零, 语义待定) */
#define KL_COUNT        0x01c  /* int 剩余项计数 */
#define KL_F_2C         0x02c  /* char (下一链为空时配合清 KL_F_10) */
#define KL_ITEM_PREV    0x004  /* 项内前驱指针 */
#define KL_ITEM_NEXT    0x008  /* 项内后继指针 */

/* sha256 上下文偏移 (FUN_000303a4; state 为 8 字小端存放) */
#define SHA_LEN         0x000  /* int 总消息长 (低 32 位) */
#define SHA_FILL        0x004  /* int 缓冲已有字节数 */
#define SHA_BUF         0x008  /* 填充缓冲 (128 字节, 可容两块) */
#define SHA_STATE       0x088  /* uint32[8] 压缩状态 */

/* BM1740 工作帧偏移 (bm1740_makeup_work, 帧长 0x92) */
#define BMF_PRE0        0x00   /* 0x55 TX 前导 */
#define BMF_PRE1        0x01   /* 0xaa */
#define BMF_B2          0x02   /* (原值&0x1f)|0x20: 高 3 位=0b001 [语义待定] */
#define BMF_CRC_START   0x02   /* CRC 覆盖 [0x02, 0x90) */
#define BMF_CRC         0x90   /* uint16 CRC, 字节序翻转存放 */

/* ==================== 外部函数 (名取自 name_map/cgminer.tsv) ==================== */

extern void        cgtime(struct timeval *tv);                    /* 原 FUN_0002a84c: gettimeofday 包装 */
extern void        quit(int status);                              /* 原 FUN_00016ca8 */
extern char       *json_array_string(void *json_arr, unsigned int idx); /* 原 FUN_000292f4 */
extern int         hex2bin(void *dst, const char *hex, size_t len); /* 原 FUN_00029830 */
extern void        mutex_lock_fail_util(const char *fn, int line);/* 原 FUN_0000b708 */
extern void        wr_lock_error(const char *fn, int line);       /* 原 FUN_0000b480 */
extern void        rw_unlock_error(const char *fn, int line);     /* 原 FUN_0000b630 */
extern void        rwlock_init_fail_cgminer(const char *fn, int line); /* 原 FUN_0000b558 */
extern void        mutex_unlock_util(pthread_mutex_t *m, const char *fn, int line); /* 原 FUN_00029388 */
extern void        _applog(uint32_t prio, char *timestamp, char *str, int force); /* 原 FUN_000389f4 */
extern void       *k_alloc_items(void *klist);                    /* 原 FUN_00038b58 (调用点仅置 r0) */
extern void        sha256_compress(void *ctx, void *blocks, int nblocks); /* 原 FUN_000300b0 (r3/r4 残参不传) */

/* ---- api.c json 输出链 ---- */
extern void       *message(void *io_data, int code, int param0, int param1, int isjson); /* 原 FUN_0003305c */
extern uint32_t    io_add(void *io, const char *str);             /* 原 FUN_000307c8 */
extern void       *api_add_const(void *item, const char *name, const char *value, int isjson); /* 原 FUN_00032e40 */
extern void        api_rd_lock(void *lock, const char *fn, int line); /* 原 FUN_00032190 */
extern void       *api_add_timeval(void *item, const char *name, void *tv, int isjson); /* 原 FUN_00033014 */
extern void       *api_add_string(void *item, const char *name, const char *value, int isjson); /* 原 FUN_00032e1c */
extern void       *add_bool_buf(void *item, const char *name, void *value, int isjson); /* 原 FUN_00032ff0 */
extern void       *add_diff_buf(void *item, const char *name, double *value, int isjson); /* 原 FUN_000355f8 */
extern void        print_data(void *io, void *item, int param, int isjson); /* 原 FUN_00032534 */
extern void        _mutex_lock(const char *fn, int line);         /* 原 FUN_0000b774 (api.c) */
extern void        _mutex_unlock(const char *fn, int line);       /* 原 FUN_0000b84c (api.c) */
extern void        api_rw_unlock(const char *fn, int line);       /* 原 FUN_0000b7e0 (api.c) */

/* ---- jansson (hashtable 迭代; pair+0x08=value, pair+0x10=内嵌键串) ---- */
extern void       *json_object_iter(void *obj);                   /* 原 FUN_00059e9c: 首个 pair */
extern char       *json_object_iter_key(void *pair);              /* 原 FUN_00059f14: 返回 pair+0x10 */
extern void       *json_object_iter_value(void *pair);            /* 原 FUN_00059f20: 返回 *(pair+8) */
extern void       *json_object_get(void *obj, const char *key);   /* 原 FUN_00059e20 */
extern int         json_object_set_nocheck(void *obj, const char *key, void *value); /* 原 FUN_0005a820 (取得 value 引用) */
extern void       *json_object_iter_next(void *obj, void *pair);  /* 原 FUN_00059ee8: pair->next */

/* ---- ccan/opt ---- */
extern void        add_opt(void *entry);                          /* 原 FUN_0005b9a8: 追加进 opt_table */
extern void        check_opt(void *entry);                        /* 原 FUN_0005b7c8: 条目合法性检查 */

/* ==================== 外部全局 (DAT_ 保留原名, 语义见注释) ==================== */

/* ---- 日志门控 (与 uart.c/hash.c/job_p5.c 同一组全局) ---- */
extern char    DAT_0007ad48;   /* 门控 A; applog 内非 0 改走 syslog() */
extern char    DAT_00078e08;   /* 门控 B (curses Verbose 联动) */
extern int     DAT_000766c4;   /* 日志级别, 门控条件 C */

/* ---- 日志文件 ---- */
extern char    DAT_00078ce0;   /* 日志文件使能标志 */
extern FILE   *DAT_00078ce4;   /* 日志文件句柄 (惰性 fopen) */
extern char    DAT_00078ce8[]; /* 日志文件路径 (fopen 实参) */
extern char    DAT_00078de8[]; /* fopen 模式串 */

/* ---- 设备/池表 ---- */
extern pthread_rwlock_t DAT_0007b700; /* devices_lock */
extern void   *DAT_0007b628;   /* devices 数组 (cgpu_info* 槽) */
extern int     DAT_00078720;   /* 已使能设备计数 (total_devices) */
extern int     DAT_00078724;   /* 追加段设备权重累加 */
extern int     DAT_0007bb6c;   /* 常规段设备权重累加 */
extern char    DAT_0007b7d0;   /* 追加段模式标志 */

/* ---- minecoin / api ---- */
extern struct timeval DAT_0007b070;   /* "Current Block Time" */
extern char    DAT_0007bb28[]; /* "Current Block Hash" 缓冲 */
extern char    DAT_00062288[]; /* add_bool_buf 字段名串 (rodata, 语义待定) */
extern char    DAT_0007bc79;   /* add_bool_buf 布尔值缓冲 */
extern double  DAT_00075de0;   /* 网络难度 (minecoin "Network Difficulty") */
extern pthread_mutex_t DAT_0007b9d8;  /* api.c 互斥锁 */
extern pthread_rwlock_t DAT_0007b9f0; /* api.c 读写锁 */

/* ---- BM1740 帧校验 ---- */
extern const uint16_t DAT_00068aec[256]; /* CRC16 逐字节查表 (rodata 0x68AEC) */

/* ---- ccan/opt ---- */
extern void   *DAT_0009d77c;   /* opt_table 动态选项表数组基址 (项大小 0x1c) */
extern int     DAT_0009d774;   /* opt_count 选项表项数 */

/* ---- 通用尾部钩子 ---- */
extern void  (*DAT_00075ed8)(void);   /* noop 钩子 (各函数尾部经指针调用, 初值恒 0 返回) */

/* ==================== 组 E: 设备使能 / 工作队列 / extranonce / 失败记录 ==================== */

/* applog 前置声明 (组 F 定义, 组 E 错误路径引用)
 * 注: 定义侧按反编译 FUN_00038730 保持 3 参固定 (prio, buf, force — buf 为已格式化
 * 消息, force 透传 _applog); 其它 TU 一律以可变参形式 extern 引用
 * (extern void applog(uint32_t prio, char *buf, ...); 同 log.c), 兼容 2/3 参调用点,
 * ARM EABI 下固定/可变参调用寄存器约定一致。 */
void applog(uint32_t prio, char *buf, int force);

// === enable_device === 原 FUN_00027f10 @ 0x00027f10
// 来源: reversed/cgminer/20_job_mining/FUN_00027f10.c | 分类: 20_job_mining
// 语义: 使能设备: cgpu+0x20 置 0 (启用), 在 devices_lock 写锁内分配设备序号
//       (total_devices 旧值) 并挂入 devices 数组; 按 DAT_0007b7d0 (追加段模式)
//       把 cgpu+0x94 权重累加到对应段; 初始化 cgpu+0x140 队列读写锁并清队首。
void enable_device(void *cgpu)
{
    int old_total;
    int ret;

    LDs32(cgpu, CG_DEVEN) = 0;              /* 0 = 启用 */
    ret = pthread_rwlock_wrlock(&DAT_0007b700);
    if (ret != 0) {
        wr_lock_error("enable_device", 0x2894);
    }
    old_total = DAT_00078720;
    LDs32(cgpu, CG_DEV_ID) = old_total;     /* 设备序号 = 旧 total_devices */
    DAT_00078720 = old_total + 1;
    *(void **)P8(DAT_0007b628, old_total * 4) = cgpu;   /* devices[old_total] = cgpu */
    ret = pthread_rwlock_unlock(&DAT_0007b700);
    if (ret != 0) {
        rw_unlock_error("enable_device", 0x2896);
    }
    (*DAT_00075ed8)();
    if (DAT_0007b7d0 == '\0') {
        DAT_0007bb6c += LDs32(cgpu, CG_WEIGHT);         /* 常规段权重累加 */
    }
    else {
        DAT_00078724 += LDs32(cgpu, CG_WEIGHT);         /* 追加段权重累加 */
    }
    ret = pthread_rwlock_init((pthread_rwlock_t *)P8(cgpu, CG_QLOCK), NULL);
    if (ret != 0) {
        rwlock_init_fail_cgminer("enable_device", 0x289d);
    }
    LDptr(cgpu, CG_QUEUE_HEAD) = NULL;      /* 哈希队列首工作指针清零 */
}

// === parse_extranonce_equihash === 原 FUN_000299ac @ 0x000299ac
// 来源: reversed/cgminer/20_job_mining/FUN_000299ac.c | 分类: 20_job_mining
// 语义: stratum equihash extranonce1 设置: 从 json 数组取串 (param_3 非零取第 1
//       个元素否则第 0 个), 在 pool 数据互斥锁+写锁保护下释放旧 nonce1/nonce1bin,
//       存新 hex 串 (pool+0x27c), 字节长 = strlen/2 (pool+0x634), calloc 二进制
//       (pool+0x280) 并 hex2bin; +0x288/+0x28c 清零, +0x290 = 0x40 - 字节长。
//       门控 (A||B||C>5) 打印 "Pool %d extranonce set to %s"。
// 返回: 1 = 已设置 (json 串缺失返回 0)。
int parse_extranonce_equihash(void *pool, void *json_arr, int flag)
{
    char logbuf[4096];
    char *nonce1_hex;
    size_t len;
    void *newbin;
    int ret;
    int *errloc;

    nonce1_hex = json_array_string(json_arr, flag != 0);
    if (nonce1_hex == NULL) {
        return 0;
    }
    ret = pthread_mutex_lock((pthread_mutex_t *)P8(pool, PL_MUTEX));
    if (ret != 0) {
        mutex_lock_fail_util("parse_extranonce_equihash", 0x9da);
    }
    ret = pthread_rwlock_wrlock((pthread_rwlock_t *)P8(pool, PL_RWLOCK));
    if (ret != 0) {
        errloc = __errno_location();
        snprintf(logbuf, 0x1000, "WTF WRLOCK ERROR ON LOCK! errno=%d in %s %s():%d",
                 *errloc, "util.c", "parse_extranonce_equihash", 0x9da);
        applog(3, logbuf, 1);
        quit(1);
    }
    free(*(void **)P8(pool, PL_NONCE1));
    *(void **)P8(pool, PL_NONCE1) = nonce1_hex;
    len = strlen(nonce1_hex);
    LDs32(pool, PL_NONCE1LEN) = (int)(len >> 1);        /* 字节长 = hex 长度/2 */
    free(*(void **)P8(pool, PL_NONCE1BIN));
    newbin = calloc(LDs32(pool, PL_NONCE1LEN), 1);
    *(void **)P8(pool, PL_NONCE1BIN) = newbin;
    if (newbin == NULL) {
        snprintf(logbuf, 0x1000, "%s: Failed to calloc pool->nonce1bin in %s %s():%d",
                 "parse_extranonce_equihash", "util.c", "parse_extranonce_equihash", 0x9e2);
        applog(3, logbuf, 1);
        quit(1);
    }
    hex2bin(*(void **)P8(pool, PL_NONCE1BIN), nonce1_hex, LDs32(pool, PL_NONCE1LEN));
    LDs32(pool, PL_N1F_288) = 0;
    LDs32(pool, PL_N1F_28C) = 0;
    LDs32(pool, PL_NONCE2OFF) = 0x40 - LDs32(pool, PL_NONCE1LEN);
    ret = pthread_rwlock_unlock((pthread_rwlock_t *)P8(pool, PL_RWLOCK));
    if (ret != 0) {
        errloc = __errno_location();
        snprintf(logbuf, 0x1000, "WTF RWLOCK ERROR ON UNLOCK! errno=%d in %s %s():%d",
                 *errloc, "util.c", "parse_extranonce_equihash", 0x9e8);
        applog(3, logbuf, 1);
        quit(1);
    }
    ret = pthread_mutex_unlock((pthread_mutex_t *)P8(pool, PL_MUTEX));
    if (ret != 0) {
        errloc = __errno_location();
        snprintf(logbuf, 0x1000, "WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d",
                 *errloc, "util.c", "parse_extranonce_equihash", 0x9e8);
        applog(3, logbuf, 1);
        quit(1);
    }
    (*DAT_00075ed8)();
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 6)) {
        return 1;                           /* 低日志级别: 静默返回 */
    }
    snprintf(logbuf, 0x1000, "Pool %d extranonce set to %s", LDs32(pool, 0), nonce1_hex);
    applog(6, logbuf, 0);
    return 1;
}

// === tq_push === 原 FUN_0002a240 @ 0x0002a240
// 来源: reversed/cgminer/20_job_mining/FUN_0002a240.c | 分类: 20_job_mining
// 语义: 工作队列入队 (list_add_tail 语义): calloc 0xc 字节节点 {data, list.next,
//       list.prev}, 自环初始化后, 队列未停机 (tq+8 die==0) 时尾插: tq->head.prev
//       = 新节点, 节点 next = &tq->head, 节点 prev = 原尾, 原尾->next = 新节点;
//       停机则 free 并返回 false; 随后 cond_signal + 解锁。
// 返回: true = 已入队, false = calloc 失败或队列停机。
bool tq_push(void *tq, void *data)
{
    uint32_t *ent;                          /* struct tq_ent (0xc 字节) */
    uint32_t last;
    bool die;
    bool rc;
    int ret;

    ent = (uint32_t *)calloc(1, 0xc);
    if (ent == NULL) {
        return false;
    }
    ent[0] = (uint32_t)data;                /* ent->data */
    ent[1] = (uint32_t)&ent[1];             /* INIT_LIST_HEAD: next 自指 (节点头 = ent+4) */
    ent[2] = (uint32_t)&ent[1];             /*                prev 自指 */
    ret = pthread_mutex_lock((pthread_mutex_t *)P8(tq, TQ_LOCK));
    if (ret != 0) {
        mutex_lock_fail_util("tq_push", 0x4f0);
    }
    die = (LD8(tq, TQ_DIE) != 0);
    if (die) {
        free(ent);                          /* 队列停机: 丢弃 */
    }
    else {
        last = LD32(tq, TQ_HEAD_PREV);      /* 原尾节点 (list_head [next][prev]) */
        LD32(tq, TQ_HEAD_PREV) = (uint32_t)&ent[1];     /* head.prev = 新节点 */
        ent[1] = (uint32_t)tq;              /* node->next = &tq->head */
        ent[2] = last;                      /* node->prev = 原尾 */
        *(uint32_t *)last = (uint32_t)&ent[1];          /* 原尾->next = 新节点 */
    }
    rc = !die;
    pthread_cond_signal((pthread_cond_t *)P8(tq, TQ_COND));
    mutex_unlock_util((pthread_mutex_t *)P8(tq, TQ_LOCK), "tq_push", 0x4fb);
    (*DAT_00075ed8)();
    return rc;
}

// === record_device_fail === 原 FUN_0002ed88 @ 0x0002ed88
// 来源: reversed/cgminer/20_job_mining/FUN_0002ed88.c | 分类: 20_job_mining
// 语义: 记录设备失败: cgpu+0xf8 存原因码, cgpu+0xf4 存当前 time_t, 并按原因码
//       (0..9) 自增 cgpu+0xfc..+0x120 的 10 个按因计数器之一 (越界仅记时不计数)。
//       watchdog 传入 3=SICK/4=DEAD [上游名不确定, 低置信]。
void record_device_fail(void *cgpu, int reason)
{
    time_t now;

    now = time(NULL);
    LDs32(cgpu, CG_FAIL_REASON) = reason;
    *(time_t *)P8(cgpu, CG_FAIL_TIME) = now;
    switch (reason) {
    case 0:
        LDs32(cgpu, CG_FAIL_CNT0 + 0x00) += 1;
        break;
    case 1:
        LDs32(cgpu, CG_FAIL_CNT0 + 0x04) += 1;
        break;
    case 2:
        LDs32(cgpu, CG_FAIL_CNT0 + 0x08) += 1;
        break;
    case 3:                                 /* SICK (watchdog) */
        LDs32(cgpu, CG_FAIL_CNT0 + 0x0c) += 1;
        break;
    case 4:                                 /* DEAD (watchdog) */
        LDs32(cgpu, CG_FAIL_CNT0 + 0x10) += 1;
        break;
    case 5:
        LDs32(cgpu, CG_FAIL_CNT0 + 0x14) += 1;
        break;
    case 6:
        LDs32(cgpu, CG_FAIL_CNT0 + 0x18) += 1;
        break;
    case 7:
        LDs32(cgpu, CG_FAIL_CNT0 + 0x1c) += 1;
        break;
    case 8:
        LDs32(cgpu, CG_FAIL_CNT0 + 0x20) += 1;
        break;
    case 9:
        LDs32(cgpu, CG_FAIL_CNT0 + 0x24) += 1;
        break;
    }
}

/* ==================== 组 F: sha256 终结 / API COIN / 日志枢纽 / klist / BM1740 帧 ==================== */

// === sha256_ctx_final === 原 FUN_000303a4 @ 0x000303a4
// 来源: reversed/cgminer/20_job_mining/FUN_000303a4.c | 分类: 20_job_mining
// 语义: sha256 收尾: 按 fill=(ctx+4) 补 0x80 + 零填充到 64 或 128 字节 (fill&0x3f
//       >=0x38 时双块), 末尾写 32 位大端比特长 ((fill+len)*8 的低 32 位, 高 4 字节
//       为 memset 清零), 压缩 1/2 块进 ctx+0x88 状态, 再把 8 个状态字大端写出到
//       out (小端存储 → 逐字节翻转)。
// 形参: Ghidra 观测 4 参, 调用点 (sha256_buffer 0x3049c) 仅传 r0=ctx/r1=out,
//       param_3/param_4 为 r2/r3 残参 (compress 的 r3/r4 残参同款处理), 不保留。
void sha256_ctx_final(void *ctx, void *out)
{
    uint8_t *buf = (uint8_t *)P8(ctx, SHA_BUF);     /* ctx+8: 填充缓冲基址 */
    uint8_t *dst = (uint8_t *)out;
    uint32_t fill = LD32(ctx, SHA_FILL);
    uint32_t total;                                 /* fill + ctx[0] (比特数 = *8, 低 32 位) */
    uint32_t span;                                  /* 填充后总长 0x40/0x80 */
    uint32_t len_off;                               /* 长度字段起始 (相对 buf) */
    int nblocks;
    int i;
    uint32_t st;

    total = fill + LD32(ctx, SHA_LEN);
    if ((fill & 0x3f) < 0x38) {
        span = 0x40;                                /* 单块收尾 */
        len_off = 0x3c;
        nblocks = 1;
    }
    else {
        span = 0x80;                                /* 双块收尾 (填充跨越 0x38) */
        len_off = 0x7c;
        nblocks = 2;
    }
    memset(buf + fill, 0, span - fill);
    buf[fill] = 0x80;                               /* 0x80 填充头 */
    /* 比特长 (低 32 位) 大端写入块尾 4 字节; 高 4 字节已由 memset 清零 */
    buf[len_off + 3] = (uint8_t)(total * 8);                        /* bits[7:0] */
    buf[len_off + 2] = (uint8_t)((total & 0x1fffffff) >> 5);        /* bits[15:8] */
    buf[len_off + 1] = (uint8_t)((total & 0x1fffffff) >> 13);       /* bits[23:16] */
    buf[len_off + 0] = (uint8_t)((total & 0x1fffffff) >> 21);       /* bits[31:24] */
    sha256_compress(ctx, buf, nblocks);
    /* 状态 8 字 (ctx+0x88, 小端) → out 大端逐字节 */
    for (i = 0; i < 8; i++) {
        st = LD32(ctx, SHA_STATE + i * 4);
        dst[i * 4 + 0] = (uint8_t)(st >> 24);       /* 原码读 ctx+0x8b 字节 */
        dst[i * 4 + 1] = (uint8_t)(st >> 16);       /* 原码读 ctx+0x8a 半字低字节 */
        dst[i * 4 + 2] = (uint8_t)(st >> 8);
        dst[i * 4 + 3] = (uint8_t)st;
    }
}

// === minecoin === 原 FUN_0003561c @ 0x0003561c
// 来源: reversed/cgminer/20_job_mining/FUN_0003561c.c | 分类: 20_job_mining
// 语义: API "COIN" 命令 (code 0x4e): 输出 "COIN," / ",\"COIN\":[" 头, 组装
//       Hash Method=sha256 / Current Block Time / Current Block Hash (api 读锁内
//       取快照) / DAT_00062288 布尔 / Network Difficulty 字段链后 print_data 输出;
//       io_add 返回值与 isjson 位与为真时 io+0x34 标志置 1。param_2/param_3 未用。
void minecoin(void *io, int param_2, int param_3, uint32_t isjson)
{
    const char *head;
    void *item;
    uint32_t headflag;
    int ret;

    (void)param_2;
    (void)param_3;
    message(io, 0x4e, 0, 0, (int)isjson);
    head = "COIN,";
    if (isjson != 0) {
        head = ",\"COIN\":[";
    }
    headflag = io_add(io, head);
    item = api_add_const(NULL, "Hash Method", "sha256", 0);
    ret = pthread_mutex_lock(&DAT_0007b9d8);
    if (ret != 0) {
        _mutex_lock("minecoin", 0xe72);
    }
    api_rd_lock(&DAT_0007b9f0, "minecoin", 0xe72);
    ret = pthread_mutex_unlock(&DAT_0007b9d8);
    if (ret != 0) {
        _mutex_unlock("minecoin", 0xe72);
    }
    item = api_add_timeval(item, "Current Block Time", &DAT_0007b070, 1);
    item = api_add_string(item, "Current Block Hash", &DAT_0007bb28, 1);
    ret = pthread_rwlock_unlock(&DAT_0007b9f0);
    if (ret != 0) {
        api_rw_unlock("minecoin", 0xe75);
    }
    (*DAT_00075ed8)();
    item = add_bool_buf(item, &DAT_00062288, &DAT_0007bc79, 0);
    item = add_diff_buf(item, "Network Difficulty", &DAT_00075de0, 1);
    print_data(io, item, (int)isjson, 0);
    if ((headflag & isjson) != 0) {
        LD8(io, 0x34) = 1;                  /* io_data 标志位 (COIN 已输出) */
    }
}

// === applog === 原 FUN_00038730 @ 0x00038730
// 来源: reversed/cgminer/20_job_mining/FUN_00038730.c | 分类: 20_job_mining
// 语义: 全二进制日志枢纽 (~150 调用者): buf 为已格式化消息。门控 A (DAT_0007ad48)
//       非零 → syslog(prio | 0x80 /* LOG_LOCAL0 设施 */, "%s", buf); 否则拼时间戳
//       " [y-m-d h:m:s] " → stderr 非 tty 才打印并 flush → 日志文件使能时惰性
//       fopen 并追加 (时间戳/消息/换行) → _applog 分发 (curses/force 逻辑)。
// 返回: 无 (param_3 = force 标志透传 _applog)。
void applog(uint32_t prio, char *buf, int force)
{
    char tsbuf[64];                         /* 原栈时间戳槽 (sp+0x58) */
    struct timeval tv;
    time_t now;
    struct tm *tmnow;
    size_t n;
    int ret;

    if (DAT_0007ad48 == '\0') {
        cgtime(&tv);
        now = tv.tv_sec;
        tmnow = localtime(&now);
        snprintf(tsbuf, 0x40, " [%d-%02d-%02d %02d:%02d:%02d] ",
                 tmnow->tm_year + 1900, tmnow->tm_mon + 1, tmnow->tm_mday,
                 tmnow->tm_hour, tmnow->tm_min, tmnow->tm_sec);
        ret = isatty(fileno(stderr));
        if (ret == 0) {
            fprintf(stderr, "%s%s\n", tsbuf, buf);
            fflush(stderr);
        }
        if ((DAT_00078ce0 != '\0') &&
            ((DAT_00078ce4 != NULL ||
              (DAT_00078ce4 = fopen(&DAT_00078ce8[0], &DAT_00078de8[0]),
               DAT_00078ce4 != NULL)))) {
            n = strlen(tsbuf);
            fwrite(tsbuf, n, 1, DAT_00078ce4);
            n = strlen(buf);
            fwrite(buf, n, 1, DAT_00078ce4);
            fwrite("\n", 1, 1, DAT_00078ce4);
            fflush(DAT_00078ce4);
        }
        _applog(prio, tsbuf, buf, force);
    }
    else {
        syslog(prio | 0x80, "%s", buf);     /* 0x80 = LOG_LOCAL0 设施位 */
    }
}

// === k_get === 原 FUN_000391fc @ 0x000391fc
// 来源: reversed/cgminer/20_job_mining/FUN_000391fc.c | 分类: 20_job_mining
// 语义: API 项链表 (klist) 取下一项: 当前项 (klist+0xc) 为空时先 k_alloc_items
//       补充分配 (仍空或 klist+4 停备标志置位 → 返回 NULL); 摘除当前项: 前移
//       klist+0xc, 后继非空则其后继前驱 (+4) 清零, 后继为空且 klist+0x2c 标志
//       置位时清 klist+0x10; 摘除项的双链指针清零, klist+0x1c 计数 -1。
// 返回: 摘除的项指针 (调用方用毕 k_free_item 回收), 无项时 NULL。
void *k_get(void *klist)
{
    void *item;
    void *next;

    item = LDptr(klist, KL_CUR);
    if (item == NULL) {
        if (LD8(klist, KL_STOP) != 0) {
            return NULL;
        }
        k_alloc_items(klist);               /* 调用点仅置 r0=klist (Ghidra 丢参) */
        item = LDptr(klist, KL_CUR);
        if (item == NULL) {
            return NULL;
        }
    }
    next = LDptr(item, KL_ITEM_NEXT);
    LDptr(klist, KL_CUR) = next;
    if (next == NULL) {
        if (LD8(klist, KL_F_2C) != 0) {
            LDs32(klist, KL_F_10) = 0;
        }
    }
    else {
        LDptr(next, KL_ITEM_PREV) = NULL;
    }
    LDptr(item, KL_ITEM_NEXT) = NULL;       /* 摘除项双链清零 */
    LDptr(item, KL_ITEM_PREV) = NULL;
    LDs32(klist, KL_COUNT) -= 1;
    return item;
}

// === bm1740_makeup_work === 原 FUN_00042a60 @ 0x00042a60
// 来源: reversed/cgminer/20_job_mining/FUN_00042a60.c | 分类: 20_job_mining
// 语义: BM1740 工作帧补全: 仅接受 len==0x92 (146 字节 = 前导 2 + 载荷 142 + CRC 2):
//       写 TX 前导 55 AA, frame[2] = (原值&0x1f)|0x20 (高 3 位=0b001, 先于 CRC),
//       对 frame[2..0x90) 以 DAT_00068aec 表逐字节 CRC16 (crc = 表[b ^ crc>>8] ^
//       crc<<8), 结果高低字节翻转写 frame+0x90 (小端存大端 CRC); 返回 0x92。
//       长度不符: 门控 (A||B||C>3) 下打印 "%s len error\n" 并返回 0。
uint8_t bm1740_makeup_work(uint8_t *frame, int len)
{
    char logbuf[4096];
    uint8_t *p;
    uint8_t *next;
    uint32_t crc;
    uint8_t ret = 0;

    if (len == 0x92) {
        crc = 0xffff;
        frame[BMF_PRE0] = 0x55;             /* TX 前导 55 AA */
        frame[BMF_B2] = (uint8_t)((frame[BMF_B2] & 0x1f) | 0x20);
        frame[BMF_PRE1] = 0xaa;
        p = frame + BMF_CRC_START;
        do {
            next = p + 1;
            /* CRC16 逐字节: 查表项 = b ^ crc 高字节, 回代 crc<<8 */
            crc = (DAT_00068aec[(uint32_t)*p ^ (crc >> 8)] ^ (crc << 8)) & 0xffff;
            p = next;
        } while (next != frame + BMF_CRC);  /* 覆盖 frame[2..0x90) 共 142 字节 */
        ret = 0x92;
        *(uint16_t *)P8(frame, BMF_CRC) = (uint16_t)(crc >> 8) | (uint16_t)(crc << 8);
    }
    else if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
        snprintf(logbuf, 0x1000, "%s len error\n", "bm1740_makeup_work");
        applog(4, logbuf, 0);
        ret = 0;
    }
    return ret;
}

/* ==================== 组 G: jansson dump/update 系列 / ccan-opt / libgcc 除法 ==================== */

/* jansson dump.c 的输出回调原型 */
typedef int (*dump_func_t)(const char *buffer, size_t size, void *data);

// === dump_indent === 原 FUN_000570d4 @ 0x000570d4
// 来源: reversed/cgminer/20_job_mining/FUN_000570d4.c | 分类: 20_job_mining
// 语义: jansson JSON 缩进输出: flags 低 5 位 (JSON_INDENT) 为 0 时仅在 space 且
//       非 JSON_COMPACT (bit5) 下输出单个空格; 否则先输出换行, 再输出 depth 行
//       每行 (flags&0x1f) 个空格 (从 32 空格常量串按长度截取)。dump 回调非 0
//       返回值视为错误传播 -1; 缩进分支原样返回 dump 结果。
// 返回: 0 = 成功, -1/回调返回值 = 输出失败。
int dump_indent(uint32_t flags, int depth, int space, dump_func_t dump, void *data)
{
    int i;
    int ret;

    if ((flags & 0x1f) == 0) {              /* JSON_INDENT(flags) == 0 */
        if ((space != 0) && ((flags & 0x20) == 0)) {    /* space 且非 JSON_COMPACT */
            return dump(" ", 1, data);      /* 紧凑分隔空格 */
        }
    }
    else {
        ret = dump("\n", 1, data);
        if (ret != 0) {
            return -1;
        }
        if (0 < depth) {
            i = 0;
            do {
                i++;
                ret = dump("                                ", flags & 0x1f, data);
                if (ret != 0) {
                    return -1;
                }
            } while (i != depth);           /* depth 行缩进 */
        }
    }
    return 0;
}

// === jsonp_dtostr === 原 FUN_000598a8 @ 0x000598a8
// 来源: reversed/cgminer/20_job_mining/FUN_000598a8.c | 分类: 20_job_mining
// 语义: jansson 浮点转串: "%.17g" 打印 (不足/出错返回 -1), 本地化小数点归一为
//       '.', 无 '.' 且无 'e' 时补 ".0" (需再留 3 字节空间); 指数存在时去除 'e'
//       后的前导零 (含符号跳位: dst = e+1 或带负号 e+2), memmove 收紧并返回
//       新串长。注: 返回值为串长 int (Ghidra 误标 char*), 0xffffffff 即 -1。
// 返回: 串长 (调用方据此截取 buffer), 失败 -1。
int jsonp_dtostr(double value, char *buffer, size_t size)
{
    int len;
    char *dot;
    char *e;
    char *dst;
    char *digits;
    char *p;
    struct lconv *loc;

    len = snprintf(buffer, size, "%.17g", value);
    if ((len < 0) || (size <= (size_t)len)) {
        return -1;
    }
    loc = localeconv();
    if ((uint8_t)*loc->decimal_point != '.') {
        dot = strchr(buffer, (unsigned char)*loc->decimal_point);
        if (dot != NULL) {
            *dot = '.';                     /* 本地化小数点 → '.' */
        }
    }
    dot = strchr(buffer, '.');
    if (dot == NULL) {
        e = strchr(buffer, 'e');
        if (e == NULL) {
            /* 纯整数形式: 补 ".0" 保证可被解析为浮点字面量 */
            if (size <= (size_t)(len + 3)) {
                return -1;
            }
            buffer[len] = '.';
            buffer[len + 1] = '0';
            buffer[len + 2] = '\0';
            return len + 2;                 /* 补点后必无 'e', 直接返回 */
        }
    }
    else {
        e = strchr(buffer, 'e');
        if (e == NULL) {
            return len;                     /* 已有小数点, 无需处理 */
        }
    }
    /* 原 0x00059928 处标签: 指数前导零去除 (此时 len 为最终基准串长) */
    if (e[1] == '-') {
        dst = e + 2;                        /* 带符号: 保留 '-', 从首数字位写入 */
    }
    else {
        dst = e + 1;
    }
    digits = e + 2;                         /* 默认源 = 指数首数字 */
    if (e[2] == '0') {
        p = e + 3;
        do {
            digits = p;
            p = digits + 1;
        } while (*digits == '0');           /* digits = 首个非 '0' 字符 (可为 '\0') */
    }
    if (digits != dst) {
        memmove(dst, digits, (size_t)((buffer + len) - digits));    /* 含结尾 NUL 一起前移 */
        len = (int)(dst - digits) + len;    /* 新串长 = 前移量 + 原长 */
    }
    return len;
}

// === json_object_update_missing === 原 FUN_0005a8a0 @ 0x0005a8a0
// 来源: reversed/cgminer/20_job_mining/FUN_0005a8a0.c | 分类: 20_job_mining
// 语义: jansson: 把 other 中 object 缺失的键复制过来: 遍历 other 的 pair 迭代
//       (key = pair+0x10 内嵌键串, value = *(pair+8)); json_object_get 未命中时
//       json_incref(value) 内联 (refcount!= -1 才自增, 静态常量不计数) 后
//       json_object_set_nocheck 插入 (取得该引用); 迭代中 value 为 NULL 跳出。
// 返回: 0 = 成功, -1 = 入参非 JSON 对象 (type 字段非 0)。
int json_object_update_missing(void *object, void *other)
{
    void *pair;
    char *key;
    void *value;
    void *existing;

    if (((object == NULL) || (LDs32(object, 0) != 0)) ||
        ((other == NULL) || (LDs32(other, 0) != 0))) {
        return -1;
    }
    pair = json_object_iter(other);
    key = json_object_iter_key(pair);       /* pair+0x10; pair==NULL → key==NULL */
    while (true) {
        if (key == NULL) {
            return 0;
        }
        value = json_object_iter_value(key - 0x10);     /* 实参 = pair = key-0x10 */
        if (value == NULL) {
            break;                          /* 空值跳出, 仍返回 0 */
        }
        existing = json_object_get(object, key);
        if (existing == NULL) {
            /* json_incref(value) 内联: refcount==-1 (静态) 不自增 */
            if (LDs32(value, 4) != -1) {
                LDs32(value, 4) += 1;
            }
            json_object_set_nocheck(object, key, value);
        }
        pair = json_object_iter_next(other, key - 0x10);
        key = json_object_iter_key(pair);
    }
    return 0;
}

// === json_object_update === 原 FUN_0005a9d0 @ 0x0005a9d0
// 来源: reversed/cgminer/20_job_mining/FUN_0005a9d0.c | 分类: 20_job_mining
// 语义: jansson: 全量合并 other → object: 遍历 other 每个 pair, json_incref
//       内联后 json_object_set_nocheck 覆盖插入; set 失败立即返回 -1 (已有键被
//       替换释放, 新键插入)。Ghidra 观测 param_3/param_4 为 set_nocheck 调用点
//       r3/r5 残参, 函数自身仅 (object, other) 两参, 不保留。
// 返回: 0 = 成功, -1 = 入参非 JSON 对象或 set 失败。
int json_object_update(void *object, void *other)
{
    void *pair;
    char *key;
    void *value;
    int32_t newrc;
    int ret;

    if (((object == NULL) || (LDs32(object, 0) != 0)) ||
        ((other == NULL) || (LDs32(other, 0) != 0))) {
        return -1;
    }
    pair = json_object_iter(other);
    key = json_object_iter_key(pair);
    while (true) {
        if (key == NULL) {
            return 0;
        }
        value = json_object_iter_value(key - 0x10);
        if (value == NULL) {
            break;
        }
        /* json_incref(value) 内联: 先算新值, refcount==-1 (静态) 不回写 */
        newrc = LDs32(value, 4) + 1;
        if (LDs32(value, 4) != -1) {
            LDs32(value, 4) = newrc;
        }
        ret = json_object_set_nocheck(object, key, value);
        if (ret != 0) {
            return -1;
        }
        pair = json_object_iter_next(other, key - 0x10);
        key = json_object_iter_key(pair);
    }
    return 0;
}

// === json_object_update_existing === 原 FUN_0005aa7c @ 0x0005aa7c
// 来源: reversed/cgminer/20_job_mining/FUN_0005aa7c.c | 分类: 20_job_mining
// 语义: jansson: 仅更新 object 中已存在的键: 遍历 other, json_object_get 命中
//       时 json_incref(value) 内联 + json_object_set_nocheck 覆盖 (set 返回值
//       忽略); 未命中的键跳过。迭代中 value 为 NULL 跳出, 仍返回 0。
// 返回: 0 = 成功, -1 = 入参非 JSON 对象 (type 字段非 0)。
int json_object_update_existing(void *object, void *other)
{
    void *pair;
    char *key;
    void *value;
    void *existing;

    if (((object == NULL) || (LDs32(object, 0) != 0)) ||
        ((other == NULL) || (LDs32(other, 0) != 0))) {
        return -1;
    }
    pair = json_object_iter(other);
    key = json_object_iter_key(pair);
    while (true) {
        if (key == NULL) {
            return 0;
        }
        value = json_object_iter_value(key - 0x10);
        if (value == NULL) {
            break;
        }
        existing = json_object_get(object, key);
        if (existing != NULL) {
            if (LDs32(value, 4) != -1) {
                LDs32(value, 4) += 1;
            }
            json_object_set_nocheck(object, key, value);
        }
        pair = json_object_iter_next(other, key - 0x10);
        key = json_object_iter_key(pair);
    }
    return 0;
}

// === opt_register_table === 原 FUN_0005bbb8 @ 0x0005bbb8
// 来源: reversed/cgminer/20_job_mining/FUN_0005bbb8.c | 分类: 20_job_mining
// 语义: ccan/opt 选项表注册: extra 非空时先以栈上伪 OPT_SUBTABLE 条目 (type=8,
//       +0x18=extra, 其余清零) add_opt 占位; 随后线性遍历 entry (步长 0x1c, 7 字:
//       [0]=子表指针/[1]=type/[6]=desc): type==0x10 (OPT_END) 停, ==8 (OPT_SUBTABLE)
//       递归注册子表 (entry[0], entry[6]), 否则 check_opt + add_opt 逐项注册;
//       最后把伪条目 +0x14 回填为本轮新增项数 (opt_count 差值)。
void opt_register_table(void *entry, const char *extra)
{
    uint32_t pseudo[7];                     /* struct opt_table (0x1c 字节) 栈上副本 */
    uint32_t *e;
    uint32_t type;
    int old_count;

    old_count = DAT_0009d774;               /* opt_count (伪条目插入前取值) */
    if (extra != NULL) {
        pseudo[0] = 0;                      /* 子表指针 (伪条目无子表) */
        pseudo[2] = 0;
        pseudo[3] = 0;
        pseudo[4] = 0;
        pseudo[5] = 0;
        pseudo[1] = 8;                      /* type = OPT_SUBTABLE */
        pseudo[6] = (uint32_t)extra;        /* +0x18 = extra 描述 */
        add_opt(pseudo);
    }
    e = (uint32_t *)entry;
    type = e[1];
    while (type != 0x10) {                  /* OPT_END */
        if (type == 8) {                    /* OPT_SUBTABLE */
            opt_register_table((void *)e[0], *(const char **)P8(e, 0x18));
        }
        else {
            check_opt(e);
            add_opt(e);
        }
        e += 7;                             /* 下一项 (+0x1c) */
        type = e[1];
    }
    if (extra != NULL) {
        /* 伪条目 +0x14 = 本表新增项数 (opt_usage 分组计数的来源) */
        *(int32_t *)P8(DAT_0009d77c, old_count * 0x1c + 0x14) = DAT_0009d774 - old_count;
    }
}

// === __udivsi3 === 原 FUN_0005c648 @ 0x0005c648
// 来源: reversed/cgminer/20_job_mining/FUN_0005c648.c | 分类: 20_job_mining
// 语义: libgcc 无符号 32 位除法 (ARM 整型除): 商 = num/den。除 0 → raise(SIGFPE=8)
//       (经 div0 桩 0x5cc00, r1 保留); den==1 直接商=num; num<=den 商=(num==den);
//       den 为 2 的幂按 (31-clz(den)) 右移; 其余走对齐减 4 位/轮主循环。
// 返回: Ghidra 合并 r1:r0 为 u64 — 低 32 位恒为商 (0x5c6c0 mov r0,r2); 高 32 位
//       仅为寄存器残值 (快速路径 = den, 循环路径 = 最终轮起始的移位除数), 不含
//       余数: __umodsi3 (0x5c6f8..0x5c708, 反汇编核实) 是 push{num,den} → bl 本
//       函数取商 → mul/sub 自算 rem = num - den*quot 后放 r1 返回, 调用方读 r1。
uint64_t __udivsi3(uint32_t num, uint32_t den)
{
    uint32_t d_aligned;                     /* 对齐到 num 量级的除数 (r1 移位寄存器) */
    uint32_t bit;                           /* 当前商位标志 (r3) */
    uint32_t quot;                          /* 商累加器 (r2) */
    uint32_t shift;

    if (den - 1 == 0) {                     /* den == 1: 商 = num (r1 残值 = den) */
        return ((uint64_t)den << 32) | num;
    }
    if (den == 0) {
        return (uint64_t)(uint32_t)raise(SIGFPE);   /* 8 = SIGFPE, r1 残值 = 0 */
    }
    if (num <= den) {                       /* 商 0 或 1 (r1 残值 = den) */
        return ((uint64_t)den << 32) | (uint32_t)(num == den);
    }
    if ((den & (den - 1)) == 0) {           /* 2 的幂: 移位即商 (r1 残值 = den) */
        shift = 0x1fU - (uint32_t)__builtin_clz(den);
        return ((uint64_t)den << 32) | (num >> (shift & 0xffU));
    }
    /* 主循环: 除数对齐到 num 最高位, 每轮 4 次条件减 + 置 4 个商位 */
    shift = (uint32_t)__builtin_clz(den) - (uint32_t)__builtin_clz(num);
    d_aligned = den << (shift & 0xffU);
    bit = 1 << (shift & 0xffU);
    quot = 0;
    while (true) {
        if (d_aligned <= num) {
            num = num - d_aligned;
            quot = quot | bit;
        }
        if ((d_aligned >> 1) <= num) {
            num = num - (d_aligned >> 1);
            quot = quot | (bit >> 1);
        }
        if ((d_aligned >> 2) <= num) {
            num = num - (d_aligned >> 2);
            quot = quot | (bit >> 2);
        }
        if ((d_aligned >> 3) <= num) {
            num = num - (d_aligned >> 3);
            quot = quot | (bit >> 3);
        }
        if (num == 0) {
            break;                          /* 余数已尽 (0x5c6b0), 本轮不再移位 */
        }
        bit = bit >> 4;
        if (bit == 0) {
            break;                          /* 商位已尽 (0x5c6b4 lsrsne 置 Z) */
        }
        d_aligned = d_aligned >> 4;
    }
    return ((uint64_t)d_aligned << 32) | quot;  /* 0x5c6c0: mov r0, r2 (r1 = 残值) */
}

/* ==================== 覆盖率表 (16/16) ==================== */
/*
 * | 函数                      | 原地址       | 来源文件 (reversed/cgminer/20_job_mining/) | 状态 |
 * |---------------------------|--------------|--------------------------------------------|------|
 * | enable_device             | FUN_00027f10 | FUN_00027f10.c                             | 完成 |
 * | parse_extranonce_equihash | FUN_000299ac | FUN_000299ac.c                             | 完成 |
 * | tq_push                   | FUN_0002a240 | FUN_0002a240.c                             | 完成 |
 * | record_device_fail        | FUN_0002ed88 | FUN_0002ed88.c                             | 完成 |
 * | sha256_ctx_final          | FUN_000303a4 | FUN_000303a4.c                             | 完成 |
 * | minecoin                  | FUN_0003561c | FUN_0003561c.c                             | 完成 |
 * | applog                    | FUN_00038730 | FUN_00038730.c                             | 完成 |
 * | k_get                     | FUN_000391fc | FUN_000391fc.c                             | 完成 |
 * | bm1740_makeup_work        | FUN_00042a60 | FUN_00042a60.c                             | 完成 |
 * | dump_indent               | FUN_000570d4 | FUN_000570d4.c                             | 完成 |
 * | jsonp_dtostr              | FUN_000598a8 | FUN_000598a8.c                             | 完成 |
 * | json_object_update_missing| FUN_0005a8a0 | FUN_0005a8a0.c                             | 完成 |
 * | json_object_update        | FUN_0005a9d0 | FUN_0005a9d0.c                             | 完成 |
 * | json_object_update_existing| FUN_0005aa7c | FUN_0005aa7c.c                            | 完成 |
 * | opt_register_table        | FUN_0005bbb8 | FUN_0005bbb8.c                             | 完成 |
 * | __udivsi3                 | FUN_0005c648 | FUN_0005c648.c (经 0x5c648..0x5c6ec 反汇编核实) | 完成 |
 *
 * 覆盖率: 16/16 (job_p5 批次剩余段, 全部落盘于本文件)
 */
