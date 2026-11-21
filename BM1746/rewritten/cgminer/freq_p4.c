// ============================================================================
// freq_p4.c — cgminer 频率/PLL 层第 4 段 78 个短函数语义转写
// 分类: 12_freq_pll | 命名: rewritten/name_map/cgminer.tsv (阶段 1 冻结)
//
// 本文件内容 (按地址序, 主题分组):
//   A. cgminer 驱动/链控制 (FUN_00025e28..FUN_00046b94):
//      clone_work_with_id / add_bool_buf / api_config / get_PIC16F1704_freq_new /
//      every_chain_software_set_address / every_chain_set_core_timeout /
//      every_chain_set_nonce_shift / every_chain_set_analog_mux_control /
//      set_frequency_chain / set_freq_asic / every_chain_set_frequency /
//      every_chain_set_ticket_mask / check_temperature_offset_value / tty_init /
//      fan_power_gpio_on / every_chain_enable_read_temperature_from_asic /
//      every_chain_init_temperature_sensor / create_bitmain_scan_freq_pthread /
//      create_bitmain_turbo_mode_monitor_pthread / save_freq_to_every_chain_pic
//   B. Equihash 辅助 (FUN_0004df30..FUN_0004ea58):
//      equihash_leaf_hash / expandArray / check_index_tree_order /
//      is_entry_less / check_duplicate_indices
//   C. Blake2b / SHA-256 预筛 (FUN_00054f8c..FUN_00055930):
//      blake2b_update / blake2b_final / blake2b / sha256_update_prescreen /
//      sha256_final_prescreen / sha256_oneshot
//   D. jansson 哈希表 (FUN_000578c0..FUN_00057d0c):
//      insert_to_bucket / hashtable_clear / hashtable_init / hashtable_free /
//      hashtable_get
//   E. jansson 词法/解析/装载 (FUN_00058034..FUN_000594bc):
//      error_set / lex_init / lex_cleanup / decode_unicode_escape / stream_unget /
//      stream_get / lex_unget_unsave / lex_get_save / parse_json / json_loadf /
//      json_load_callback
//   F. jansson 内存/strbuffer (FUN_0005956c..FUN_000597c0):
//      jsonp_malloc / jsonp_free / strbuffer_clear /
//      strbuffer_value / strbuffer_append_bytes / strbuffer_append_byte /
//      strbuffer_pop  (strbuffer_close=00059654 归属 pool_p3.c, 仅 extern 引用)
//   G. jansson UTF-8 (FUN_000599dc..FUN_00059c8c):
//      utf8_encode / utf8_check_first / utf8_check_full / utf8_decode_char /
//      utf8_check_string
//   H. jansson 值/容器构造 (FUN_00059d20..FUN_0005a95c):
//      json_array_grow / json_object_new / json_array / json_string_nocheck /
//      json_string / json_integer / json_real / json_true / json_false /
//      json_null / json_delete / json_array_insert / json_array_append /
//      json_object_set_nocheck / json_object_set
//   I. jansson 错误记录 (FUN_0005b014..FUN_0005b098):
//      jsonp_error_init / jsonp_error_vset / jsonp_error_set
//   J. libgcc (FUN_0005c7ec): __aeabi_idiv_checked
//
// 说明: 函数 B-J 实为 blake2b / sha2 / jansson / libgcc 等第三方库代码, 编译时
// 被链入 12_freq_pll 区段 (静态链接产物按地址分类的固有现象), 按体原样转写。
// ============================================================================

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>     /* va_list/va_start — error_set 变参展开 */
#include <stdio.h>      /* snprintf/printf/fscanf — 原调用保留 */
#include <stdlib.h>     /* malloc/free/realloc/abort — 原调用保留 */
#include <string.h>     /* memcpy/memset/memcmp — 原调用保留 */
#include <pthread.h>    /* pthread_mutex_lock/pthread_create — 原调用保留 */
#include <unistd.h>     /* usleep — 原调用保留 */
#include <signal.h>     /* raise — __aeabi_idiv_checked 原调用保留 */

#include "../include/common_types.h"
#include "../include/bm1746_regs.h"

/* ================= 外部函数 (name_map 语义名, 定义在各批次文件) ================= */

extern void   *make_clone(void *work, uint32_t nonce2);                  /* FUN_0001a44c (job_p1.c) */
extern void    submit_work_async(void *work);                            /* FUN_000256a4 (job_p1.c) */
extern int     io_add(void *io_data, const char *str);                   /* FUN_000307c8 */
extern int    *api_add_data_full(int *root, const char *name, int type, void *data, int copy); /* FUN_00031cdc */
extern void   *api_add_int(void *root, const char *name, int *data, int copy);      /* FUN_00032ed0 */
extern void   *api_add_const(void *root, const char *name, const char *data, int copy); /* FUN_00032e40 */
extern void    message(void *io_data, int msg, int param0, int param1, int isjson); /* FUN_0003305c */
extern void    print_data(void *io_data, void *root, int isjson, int prepend_comma);/* FUN_00032534 */
extern void    pic_i2c_write_byte(uint8_t byte);                         /* FUN_0003c7b0 (pic.c) */
extern uint8_t pic_i2c_read_byte(void);                                 /* FUN_0003c7c8 (pic.c) */
extern void    software_set_address_chain(int chain);                    /* FUN_00042f7c (chain.c) */
extern void    set_core_timeout_chain(int chain, uint32_t timeout);      /* FUN_000430a0 (chain.c) */
extern void    set_nonce_shift_chain(int chain, int val);                /* FUN_000431ac (chain.c) */
extern void    set_analog_mux_control_chain(int chain, int val);         /* FUN_000434a0 (chain.c) */
extern void    set_ticket_mask_chain(int chain, uint8_t mask);           /* FUN_00044004 (chain.c) */
extern void    tty_init_chain(int chain, int param_2);                   /* FUN_000445e8 (chain.c) */
extern void    enable_read_temperature_from_asic_chain(int chain);       /* FUN_0004601c (chain.c) */
extern void    write_i2c_reg(int chain, int reg_idx, uint8_t val, uint8_t cmd, uint8_t len); /* FUN_0004633c */
extern void    makeup_cmd_frame(uint32_t *frame, uint8_t write_flag, uint8_t addr,
                                uint8_t reg, uint32_t value);            /* FUN_0003cdcc (uart.c) */
extern uint32_t uart_send(int chain, const void *frame, uint32_t len);   /* FUN_0003c150 (uart.c) */
extern uint8_t write_data_to_PIC16F1704_flash(void *data, uint8_t mode, uint32_t len); /* FUN_00041e7c */
extern int     thr_info_create(void *thr, pthread_attr_t *attr, void *(*fn)(void *), void *arg); /* FUN_0002f174 (util.c) */
extern void   *scan_freq_func(void *arg);                                     /* FUN_00047354 (freq_p3.c, 扫频线程体) */
extern void   *turbo_mode_monitor_func(void *arg);                            /* FUN_0004388c (freq_p3.c, turbo 监控线程体) */
extern void    cgsleep_ms(int ms);                                       /* FUN_0002a92c (util.c) */
extern void    applog(uint32_t prio, char *buf, ...);     /* FUN_00038730 (log.c) */

/* ================= 外部全局 (DAT_ 原名保留, 注释为已核实语义) ================= */

extern uint8_t DAT_0007ad48;     /* applog 门控 A (log.c) */
extern uint8_t DAT_00078e08;     /* applog 门控 B = opt_verbose (log.c) */
extern int      DAT_000766c4;    /* applog 门控 C: 日志级别 (log.c) */
extern int      DAT_0007b198;    /* total_pools: 已配置矿池数 (axi.c) */
extern int      DAT_00075dcc;    /* opt_log_interval (Log Interval) */
extern int      DAT_00075ddc;    /* opt_queue (Queue) */
extern int      DAT_00075df0;    /* opt_scantime (ScanTime) */
extern int      DAT_00075df4;    /* opt_expiry (Expiry) */
extern uint8_t  DAT_0007b625;    /* opt_failover_only (Failover-Only) */
extern char     DAT_00067184[];  /* .rodata 常量串 "OS" (api_config 键名) */
extern char     DAT_000671c4[];  /* .rodata 常量串 "None" (Hotplug 缺省值) */
extern char    *DAT_00075e10[];  /* statenames: 挖矿策略名表 (.data) */
extern int      DAT_00076efc;    /* mining_strategy: 当前挖矿策略枚举 (Strategy 索引) */
extern char     DAT_0009d4bb[];  /* +1 (0x9d4bc) 起为链存在标志数组 (4 链) */
extern pthread_mutex_t DAT_000790a0; /* PIC I2C 互斥锁 (pic.c) */
extern uint8_t  DAT_00078e09;    /* opt_debug: LOG_GATE7 类日志外层总开关 (log.c) */
extern uint32_t DAT_00076b58;    /* 当前 PLL 配置字缓存 (set_frequency_chain 写入) */
extern int32_t  DAT_00069204[];  /* 频率表 freq_MHz 字段: 115 条 × 16B 条目首字段 (0x69204) */
extern uint32_t DAT_00069210[];  /* 频率表 pll_value 字段: 条目内偏移 +12 (0x69210) */
extern char     DAT_0009d4b4[];  /* 统计区基址 1: +0x163 (0x9d617) 起每链 19B 温度偏移串 */
extern char     DAT_0009d4b5[];  /* +0x250+(chain) 起为每链当前频率 (int16) */
extern char     DAT_0009d4b7[];  /* 每核温度偏移标志字节 (步进 4B) */
extern uint32_t DAT_0009d4b8[];  /* 每核温度偏移值字 (与上同为 4B 步进) */
extern uint8_t  DAT_0009d6f3;    /* 芯片地址步进 asic_addr_interval (chain.c) */
extern char     DAT_0009d617[];  /* 每链温度偏移字符串区 (19B/链, PIC ASCII 协议) */
extern void    *DAT_0009d4b0;    /* 通用 thr_info 指针 (calloc 0x40, +0xc = pthread_t) */
extern pthread_mutex_t DAT_000790d0; /* PIC flash 写互斥锁 (pic.c) */
extern int      DAT_0007909c;    /* 当前链号: PIC I2C 命令拼入 AXI 位 17..16 (pic.c) */

/* 反编译统一日志门控: 调用点形态 (A||B||level>N) ↔ applog(prio=N+1);
 * 三条件全关时调用点直接跳过字符串构造与 applog。 */
#define LOG_GATE(prio) \
	(DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || DAT_000766c4 >= (prio))

/* ================= jansson 2.x 类型 (与 json.c 保持一致) ================= */

typedef enum json_type {
	JSON_OBJECT  = 0,
	JSON_ARRAY   = 1,
	JSON_STRING  = 2,
	JSON_INTEGER = 3,
	JSON_REAL    = 4,
	JSON_TRUE    = 5,
	JSON_FALSE   = 6,
	JSON_NULL    = 7
} json_type;

/* json_t 公共头 (各节点首两字段; refcount == -1 = 永久对象) */
typedef struct json_t {
	int type;       /* +0x00: json_type 枚举值 */
	int refcount;   /* +0x04: 引用计数; -1 表示永久对象 */
} json_t;

/* json_error_t (252 字节; 偏移由 jsonp_error_init 证实) */
typedef struct json_error_t {
	int  line;        /* +0x000 */
	int  column;      /* +0x004 */
	int  position;    /* +0x008 */
	char source[80];  /* +0x00C */
	char text[160];   /* +0x05C */
} json_error_t;

/* --- jansson hashtable (hashtable.c; 偏移由 insert_to_bucket/clear/del 证实) --- */

typedef struct hashtable_list_t {
	struct hashtable_list_t *prev;   /* +0 */
	struct hashtable_list_t *next;   /* +4 */
} hashtable_list_t;

/* pair 布局: hash@0, list@4 (节点指针 = &pair->list), value@12 */
typedef struct hashtable_pair {
	size_t            hash;         /* +0 */
	hashtable_list_t  list;         /* +4 */
	json_t           *value;        /* +12 */
} hashtable_pair_t;

typedef struct hashtable_bucket {
	hashtable_list_t *first;         /* +0 */
	hashtable_list_t *last;          /* +4 */
} hashtable_bucket_t;

typedef struct hashtable_t_f1 {
	size_t            size;          /* +0 */
	hashtable_bucket_t *buckets;     /* +4 */
	size_t            num_buckets;   /* +8: prime_list 下标 (DAT_0006cb74) */
	hashtable_list_t  list;          /* +0xC: 全局链表头 {prev@0xC, next@0x10} */
} hashtable_t_f1;

/* prime_list: 桶数素数表 (.rodata 0x6cb74, 与 jansson prime_list 一致: 5,13,23,53,97...) */
extern const size_t DAT_0006cb74[];

/* --- SHA-256 预筛侧上下文 (偏移由 update/final 证实) --- */

typedef struct sha256_ctx_f1 {
	uint32_t h[8];                 /* +0x00: 链值 (初值 6a09e667..5be0cd19) */
	uint64_t bitlen;               /* +0x20: 已输入位数 */
	uint8_t  buf[64];              /* +0x28: 分组缓冲 */
} sha256_ctx_f1;

/* --- blake2b 状态 (360 字节 0x168; 偏移由 update/final 证实) --- */

typedef struct blake2b_state_f1 {
	uint32_t h[16];                /* +0x000: 链值 h[0..7] (8×u64) */
	uint32_t counter[4];           /* +0x040: 128 位输入计数器 (两个 u64) */
	uint32_t f[4];                 /* +0x050: 终结标志 (f[0]=~0 收尾, f[1]=last_node) */
	uint8_t  buf[256];             /* +0x060: 双块缓冲 */
	uint32_t buflen;               /* +0x160 */
	uint8_t  last_node;            /* +0x164 */
} blake2b_state_f1;

/* --- jansson strbuffer / lex (偏移由 strbuffer_init/close 与 lex_* 调用点证实) --- */

typedef struct strbuffer_t_f1 {
	char  *value;                  /* +0: NUL 结尾字符串 */
	size_t length;                 /* +4: 已用长度 (error_set 的 <0x15 判断) */
	size_t size;                   /* +8: 分配容量 (init = 16) */
} strbuffer_t_f1;

/* 旧版 jansson: 流状态与词法状态同体 (lex_t); 偏移由 lex_init/stream_get/
 * stream_unget/error_set 调用点共同证实。 */
typedef struct lex_t_f1 {
	int  (*get)(void *data);       /* +0x00 */
	void *data;                    /* +0x04 */
	uint8_t buffer[8];             /* +0x08: 多字节回退缓冲 (buffer_pos@+0x10 邻接;
	                                * 实际容量未证, ≥8B, NUL 标记缓冲耗尽) */
	size_t buffer_pos;             /* +0x10 */
	int state;                     /* +0x14: 0=OK, -1=EOF, -2=ERROR */
	int line;                      /* +0x18 */
	int column;                    /* +0x1C */
	int last_column;               /* +0x20 */
	size_t position;               /* +0x24 */
	strbuffer_t_f1 value;          /* +0x28: token 文本缓冲 (0x28..0x33) */
	int token;                     /* +0x34: 当前 token (init=-1; parse_json 经 &0xFFFFFFDF
	                                * 归一比较 '['0x5B/'{'0x7B; 0x100=字符串 token) */
	void *field_0x38;              /* +0x38: 字符串 token 缓冲 (token==0x100 时 cleanup 释放) */
} lex_t_f1;

/* ============================================================================
 * A. cgminer 驱动 / 链控制 / API / PIC (FUN_00025e28..FUN_00046b94)
 * ========================================================================== */

// === clone_work_with_id === 原 FUN_00025e28 @ 0x00025e28
// 来源: reversed/cgminer/12_freq_pll/FUN_00025e28.c | 分类: 12_freq_pll
// 语义: 给 work 打上任务号 (work->id, 偏移 0x4c), 克隆一份并异步提交;
//       param_1 未用, param_4 未用 (仅寄存器残迹)。恒返回 1。
int clone_work_with_id(void *unused, void *work, uint32_t id, uint32_t extra)
{
	*(uint32_t *)((char *)work + 0x4c) = id;   /* work->id = id */
	submit_work_async(make_clone(work, 0));    /* 克隆 (nonce2 拷 0) 后异步提交 */
	return 1;
}

// === add_bool_buf === 原 FUN_00032ff0 @ 0x00032ff0
// 来源: reversed/cgminer/12_freq_pll/FUN_00032ff0.c | 分类: 12_freq_pll
// 语义: API 状态输出辅助 — 以 type=0xe (API_BOOL, 1 字节) 把布尔值挂入
//       api_data 链表; 完全等价于 api_add_data_full(root,name,0xe,data,copy)。
void add_bool_buf(void *root, const char *name, void *data, int copy)
{
	api_add_data_full(root, name, 0xe /* API_BOOL */, data, copy);
}

// === api_config === 原 FUN_00034ab8 @ 0x00034ab8
// 来源: reversed/cgminer/12_freq_pll/FUN_00034ab8.c | 分类: 12_freq_pll
// 语义: API "config" 命令处理 — 先回 CONFIG 状态消息 (msg 0x21), 再按 isjson
//       选择分隔前缀; api_data 链表自 NULL 起, 依次追加 ASC/PGA/Pool Count、
//       策略、日志间隔、Device Code、OS、Failover-Only、ScanTime、Queue、
//       Expiry、Hotplug; print_data 打印; io_add 成功且 isjson 时置 io_data+0xd
//       标志 (json 应答标记, api_notify 同址)。
void api_config(int *io_data, int param_2, int param_3, int isjson)
{
	int io_ok;
	void *root = 0;           /* 原体链表根自 0 (NULL) 起, 经各 api_add_* 传递 */
	int asc_count = 0;        /* ASC Count 固定 0 */
	int pga_count = 0;        /* PGA Count 固定 0 */
	char msg[4100];
	(void)param_2; (void)param_3;   /* 反编译体未使用 */

	message(io_data, 0x21 /* MSG_CONFIG */, 0, 0, isjson);
	io_ok = io_add(io_data, (isjson != 0) ? ",\"CONFIG\":[" : "CONFIG,");

	root = api_add_int(root, "ASC Count", &asc_count, 0);
	root = api_add_int(root, "PGA Count", &pga_count, 0);
	root = api_add_int(root, "Pool Count", &DAT_0007b198, 0);
	root = api_add_const(root, "Strategy", DAT_00075e10[DAT_00076efc], 0);
	root = api_add_int(root, "Log Interval", &DAT_00075dcc, 0);
	root = api_add_const(root, "Device Code", "", 0);
	root = api_add_const(root, DAT_00067184 /* "OS" */, "Linux", 0);
	add_bool_buf(root, "Failover-Only", &DAT_0007b625, 0);
	root = api_add_int(root, "ScanTime", &DAT_00075df0, 0);
	root = api_add_int(root, "Queue", &DAT_00075ddc, 0);
	root = api_add_int(root, "Expiry", &DAT_00075df4, 0);
	root = api_add_const(root, "Hotplug", DAT_000671c4 /* "None" */, 0);
	print_data(io_data, root, isjson, 0);

	if ((io_ok & isjson) != 0)
		*(uint8_t *)((char *)io_data + 0xd) = 1;   /* json 应答标志 (api_notify 同址) */
}

// === get_PIC16F1704_freq_new === 原 FUN_00041b8c @ 0x00041b8c
// 来源: reversed/cgminer/12_freq_pll/FUN_00041b8c.c | 分类: 12_freq_pll
// 语义: 向 PIC16F1704 发 "读频率" 命令帧 55 AA 04 25 00 29 (0x2900 = LE16),
//       加锁发送、延时后读回 6 字节再解锁。注: 反编译体在此之后无条件打印
//       "failed!" 日志并恒返回 0 (成功路径位于被剔除的不可达块 0x41d60 起,
//       固件中该函数实际不可达成功分支); 打印的 read_back_data[0..3] 为字面量
//       0xff,0,0,0, 仅 [4],[5] 取自读回缓冲 — 均按反编译体原样保留。
uint8_t get_PIC16F1704_freq_new(void)
{
	uint8_t cmd_frame[6];
	uint8_t read_back_data[6];
	char msg[4100];
	int i;

	/* 发送帧: 反编译体为栈上三段散写 (0x55 / {aa,04,25} / 0x2900) 后指针步进
	 * 逐字节发出, 6 字节依次为: */
	cmd_frame[0] = 0x55;   /* TX 前导 */
	cmd_frame[1] = 0xAA;   /* TX 前导 */
	cmd_frame[2] = 0x04;   /* 帧长 */
	cmd_frame[3] = 0x25;   /* PIC 命令: 读频率 */
	cmd_frame[4] = 0x00;   /* 数据低字节 (LE16 0x2900) */
	cmd_frame[5] = 0x29;   /* 数据高字节 */

	pthread_mutex_lock(&DAT_000790a0);
	for (i = 0; i < 6; i++)
		pic_i2c_write_byte(cmd_frame[i]);
	usleep(300000);
	for (i = 0; i < 6; i++)                    /* 原体为 do-while 步进读 6 字节 */
		read_back_data[i] = pic_i2c_read_byte();
	pthread_mutex_unlock(&DAT_000790a0);
	usleep(200000);

	if (LOG_GATE(5)) {
		snprintf(msg, 0x1000,
		         "--- %s: read_back_data[0] = 0x%x, read_back_data[1] = 0x%x, "
		         "read_back_data[2] = 0x%x, read_back_data[3] = 0x%x, "
		         "read_back_data[4] = 0x%x, read_back_data[5] = 0x%x\n",
		         "get_PIC16F1704_freq_new", 0xff, 0, 0, 0,
		         (unsigned)read_back_data[4], (unsigned)read_back_data[5]);
		applog(5, msg, 0);
	}
	if (LOG_GATE(3)) {
		snprintf(msg, 0x1000, "\n--- %s failed!\n\n", "get_PIC16F1704_freq_new");
		applog(3, msg, 0);
	}
	return 0;
}

// === every_chain_software_set_address === 原 FUN_00043060 @ 0x00043060
// 来源: reversed/cgminer/12_freq_pll/FUN_00043060.c | 分类: 12_freq_pll
// 语义: 对全部 4 条链逐链发送软件设址命令 (仅链存在标志为 1 的链), 结束延时 10ms。
void every_chain_software_set_address(void)
{
	int chain;

	for (chain = 0; chain < 4; chain++) {
		if (DAT_0009d4bb[1 + chain] == 1)          /* 链存在标志 (0x9d4bc 起) */
			software_set_address_chain(chain);
	}
	cgsleep_ms(10);
}

// === every_chain_set_core_timeout === 原 FUN_000432c0 @ 0x000432c0
// 来源: reversed/cgminer/12_freq_pll/FUN_000432c0.c | 分类: 12_freq_pll
// 语义: 对全部在位链逐链下发核心超时值 (出厂 84000), 结束延时 10ms。
void every_chain_set_core_timeout(uint32_t timeout)
{
	int chain;

	for (chain = 0; chain < 4; chain++) {
		if (DAT_0009d4bb[1 + chain] == 1)
			set_core_timeout_chain(chain, timeout);
	}
	cgsleep_ms(10);
}

// === every_chain_set_nonce_shift === 原 FUN_0004330c @ 0x0004330c
// 来源: reversed/cgminer/12_freq_pll/FUN_0004330c.c | 分类: 12_freq_pll
// 语义: 对全部在位链逐链下发 nonce 偏移寄存器值, 结束延时 10ms。
void every_chain_set_nonce_shift(int val)
{
	int chain;

	for (chain = 0; chain < 4; chain++) {
		if (DAT_0009d4bb[1 + chain] == 1)
			set_nonce_shift_chain(chain, val);
	}
	cgsleep_ms(10);
}

// === every_chain_set_analog_mux_control === 原 FUN_00043594 @ 0x00043594
// 来源: reversed/cgminer/12_freq_pll/FUN_00043594.c | 分类: 12_freq_pll
// 语义: 对全部在位链逐链下发模拟多路选择控制寄存器值, 结束延时 10ms。
void every_chain_set_analog_mux_control(int val)
{
	int chain;

	for (chain = 0; chain < 4; chain++) {
		if (DAT_0009d4bb[1 + chain] == 1)
			set_analog_mux_control_chain(chain, val);
	}
	cgsleep_ms(10);
}

// === set_frequency_chain === 原 FUN_000435e0 @ 0x000435e0
// 来源: reversed/cgminer/12_freq_pll/FUN_000435e0.c | 分类: 12_freq_pll
// 语义: 频率设置唯一整链写入口 — 查频率表 0x69204 (115 条 × 16B) 命中目标
//       频率 MHz (未命中兜底槽位 0x40 = 587MHz), 取对应 PLL 配置字写入全局
//       缓存 DAT_00076b58, 构寄存器 0x0C (BM1746_REG_PLL) 链广播写帧 (write=1,
//       addr=0) 后 uart 发送 11 字节, 并把频率记入每链当前频率表。
void set_frequency_chain(int chain, int freq_mhz)
{
	uint8_t cmd_frame[11];       /* 原体为栈上散零的 11 字节命令帧 */
	int slot;                    /* 命中的频率表条目序号 (原体计数 ×16 字节偏移) */
	int i;

	memset(cmd_frame, 0, sizeof(cmd_frame));   /* 原体为 4+4+2+1 字节散零 */

	if (LOG_GATE(5)) {
		char msg[4100];
		snprintf(msg, 0x1000, "chainid%d %s freq=%d", chain, "set_frequency_chain", freq_mhz);
		applog(5, msg, 0);
	}

	slot = 0x40;                               /* 未命中兜底: 0x40 槽位 = 587MHz */
	for (i = 0; i < 0x73; i++) {               /* 115 条频率表逐条比对 */
		if (freq_mhz == DAT_00069204[i * 4])   /* 条目步进 16B, freq 字段 = u32[0] */
		{
			slot = i;
			break;
		}
	}
	DAT_00076b58 = DAT_00069210[slot * 4];     /* pll 字段 = 条目内偏移 +0xC */
	makeup_cmd_frame((uint32_t *)cmd_frame, 1 /* 链广播写 */, 0, 0xC /* REG_PLL */,
	                 DAT_00076b58);
	uart_send(chain, cmd_frame, 0xB);
	*(int16_t *)(DAT_0009d4b5 + (chain + 0x128) * 2) = (int16_t)freq_mhz;  /* 每链当前频率 */
	cgsleep_ms(0x32);
}

// === set_freq_asic === 原 FUN_00043734 @ 0x00043734
// 来源: reversed/cgminer/12_freq_pll/FUN_00043734.c | 分类: 12_freq_pll
// 语义: 单 ASIC 频率写入口 — 查频率表取 PLL 配置字, 构寄存器 0x0C 带址写帧
//       (write=0, addr = asic_addr_interval * asic_idx), 发送到指定链, 延时 100ms。
void set_freq_asic(int chain, int asic_idx, int freq_mhz)
{
	uint8_t cmd_frame[11];
	int slot;                    /* 原体命中的频率表条目序号 */
	int i;
	char msg[4100];

	memset(cmd_frame, 0, sizeof(cmd_frame));

	if (DAT_00078e09 != '\0' && LOG_GATE(7)) {
		snprintf(msg, 0x1000, "%s %d %d freq=%d\n", "set_freq_asic", chain, asic_idx, freq_mhz);
		applog(7, msg, 0);
	}

	slot = 0x40;
	for (i = 0; i < 0x73; i++) {
		if (freq_mhz == DAT_00069204[i * 4]) {
			slot = i;
			break;
		}
	}
	makeup_cmd_frame((uint32_t *)cmd_frame, 0 /* 带址写 */,
	                 (uint8_t)(DAT_0009d6f3 * asic_idx & 0xFF) /* ASIC 地址 */, 0xC,
	                 DAT_00069210[slot * 4]);
	uart_send(chain, cmd_frame, 0xB);
	cgsleep_ms(100);
}

// === every_chain_set_frequency === 原 FUN_00043e4c @ 0x00043e4c
// 来源: reversed/cgminer/12_freq_pll/FUN_00043e4c.c | 分类: 12_freq_pll
// 语义: 对全部在位链调用 set_frequency_chain 广播同一目标频率, 结束延时 10ms。
void every_chain_set_frequency(int freq_mhz)
{
	int chain;

	for (chain = 0; chain < 4; chain++) {
		if (DAT_0009d4bb[1 + chain] == 1)
			set_frequency_chain(chain, freq_mhz);
	}
	cgsleep_ms(10);
}

// === every_chain_set_ticket_mask === 原 FUN_00044104 @ 0x00044104
// 来源: reversed/cgminer/12_freq_pll/FUN_00044104.c | 分类: 12_freq_pll
// 语义: 对全部在位链逐链下发 ticket mask (无收尾延时)。
void every_chain_set_ticket_mask(uint8_t mask)
{
	int chain;

	for (chain = 0; chain < 4; chain++) {
		if (DAT_0009d4bb[1 + chain] == 1)
			set_ticket_mask_chain(chain, mask);
	}
}

// === check_temperature_offset_value === 原 FUN_000442b0 @ 0x000442b0
// 来源: reversed/cgminer/12_freq_pll/FUN_000442b0.c | 分类: 12_freq_pll
// 语义: 生成每链温度偏移字符串 (每链 19 字节, DAT_0009d617 起): 在位 ASIC 每隔
//       8 个写 ' '(0x20) 作分隔、其余写 'o'(0x6f), 尾部补 NUL; 每核同时清零
//       偏移标志字节 (DAT_0009d4b7, 4B 步进) 并把偏移值字 (DAT_0009d4b8) 保留
//       最高字节; 偏移下标超过 19 记 "offset[%d] ERR" 日志 (不中断)。
void check_temperature_offset_value(void)
{
	int chain;
	unsigned int asic;
	unsigned int group;
	unsigned int offset_idx;
	uint32_t tmp_word;
	char msg[4100];
	int word_idx;
	int zero_pos;                /* 原体中间变量 = chain*8 */

	for (chain = 0; chain < 4; chain++) {
		if (DAT_0009d4bb[1 + chain] == 0)        /* 链不存在则跳过 */
			continue;
		if (DAT_0009d4bb[1 + chain + 0x209] == 0) {   /* ASIC 数 == 0: 仅补 NUL */
			zero_pos = chain * 8;
			asic = 0;
		} else {
			zero_pos = chain * 8;
			group = 0;
			asic = 0;
			while (asic < (uint8_t)DAT_0009d4bb[1 + chain + 0x209]) {
				if ((asic & 7) == 0) {
					/* 每 8 ASIC 一组: 先记分隔符 ' ' 再递增组计数 */
					offset_idx = asic + group;
					if (offset_idx > 0x13 && LOG_GATE(3)) {
						snprintf(msg, 0x1000, "offset[%d] ERR", offset_idx);
						applog(3, msg, 0);
					}
					group = (group + 1) & 0xFF;
					DAT_0009d4b4[offset_idx + chain * 0x13 + 0x163] = 0x20;
				}
				offset_idx = asic + group;
				if (offset_idx > 0x13 && LOG_GATE(3)) {
					snprintf(msg, 0x1000, "offset[%d] ERR", offset_idx);
					applog(3, msg, 0);
				}
				DAT_0009d4b4[offset_idx + chain * 0x13 + 0x163] = 0x6f;
				word_idx = (chain * 3 + asic + 0x40) * 4;
				asic = (asic + 1) & 0xFF;   /* 原体计数自增位于字操作之前 */
				tmp_word = *(uint32_t *)((char *)DAT_0009d4b8 + word_idx);
				DAT_0009d4b7[word_idx] = 0;
				*(uint32_t *)((char *)DAT_0009d4b8 + word_idx) = tmp_word & 0xFF000000u;
			}
			asic = asic + group;   /* 偏移串总长 = ASIC 数 + 组分隔数 */
			if (asic > 0x13 && LOG_GATE(3)) {
				snprintf(msg, 0x1000, "offset[%d] ERR", asic);
				applog(3, msg, 0);
			}
		}
		DAT_0009d617[zero_pos * 2 + chain * 3 + asic] = 0;   /* 偏移串结尾 NUL */
	}
}

// === tty_init === 原 FUN_000448bc @ 0x000448bc
// 来源: reversed/cgminer/12_freq_pll/FUN_000448bc.c | 分类: 12_freq_pll
// 语义: 打印 "tty_init" (debug), 对链 0..3 逐链调用 tty_init_chain (param_1
//       为透传参数), 结束延时 10ms。
void tty_init(int param_1)
{
	int chain;

	if (LOG_GATE(5))
		applog(5, "tty_init", 0);    /* 原体为栈上逐字节构造串 "tty_init" */

	for (chain = 0; chain < 4; chain++)
		tty_init_chain(chain, param_1);
	cgsleep_ms(10);
}

// === fan_power_gpio_on === 原 FUN_00045b30 @ 0x00045b30
// 来源: reversed/cgminer/12_freq_pll/FUN_00045b30.c | 分类: 12_freq_pll
// 语义: 拉高风扇供电 GPIO (gpio954/gpio955) — 通过 sysfs 写 1。
void fan_power_gpio_on(void)
{
	system("echo 1 > /sys/class/gpio/gpio954/value");
	system("echo 1 > /sys/class/gpio/gpio955/value");
}

// === every_chain_enable_read_temperature_from_asic === 原 FUN_0004614c @ 0x0004614c
// 来源: reversed/cgminer/12_freq_pll/FUN_0004614c.c | 分类: 12_freq_pll
// 语义: 对链 0..3 逐链调用 enable_read_temperature_from_asic_chain。
void every_chain_enable_read_temperature_from_asic(void)
{
	enable_read_temperature_from_asic_chain(0);
	enable_read_temperature_from_asic_chain(1);
	enable_read_temperature_from_asic_chain(2);
	enable_read_temperature_from_asic_chain(3);
}

// === every_chain_init_temperature_sensor === 原 FUN_000463c8 @ 0x000463c8
// 来源: reversed/cgminer/12_freq_pll/FUN_000463c8.c | 分类: 12_freq_pll
// 语义: 对每条在位链 × 每个 reg_idx 0..2 调 write_i2c_reg 写温度传感器
//       初始化配置 (val=0x4c, cmd=9, len=4)。
void every_chain_init_temperature_sensor(void)
{
	int chain;
	int reg_idx;

	for (chain = 0; chain < 4; chain++) {
		if (DAT_0009d4bb[1 + chain] == 1) {
			for (reg_idx = 0; reg_idx < 3; reg_idx++)
				write_i2c_reg(chain, reg_idx, 0x4C, 9, 4);
		}
	}
}

// === create_bitmain_scan_freq_pthread === 原 FUN_00046880 @ 0x00046880
// 来源: reversed/cgminer/12_freq_pll/FUN_00046880.c | 分类: 12_freq_pll
// 语义: 创建扫频线程 (scan_freq_func, FUN_00047354) — calloc 0x40 thr_info,
//       成功则 pthread_detach 并睡 500ms 返回 0; 失败按 LOG_GATE7 打日志返回 -5。
void *create_bitmain_scan_freq_pthread(void)
{
	int ret;
	char msg[4100];

	DAT_0009d4b0 = calloc(1, 0x40);
	ret = thr_info_create(DAT_0009d4b0, 0, scan_freq_func, 0);
	if (ret == 0) {   /* thr_info_create 返回 pthread_create 结果 */
		pthread_detach(*(pthread_t *)((char *)DAT_0009d4b0 + 0xC));
		cgsleep_ms(500);
		return (void *)0;
	}
	if (DAT_00078e09 != '\0' && LOG_GATE(7)) {
		snprintf(msg, 0x1000, "%s: create thread for check miner_status",
		         "create_bitmain_scan_freq_pthread");
		applog(7, msg, 0);
	}
	return (void *)-5;
}

// === create_bitmain_turbo_mode_monitor_pthread === 原 FUN_00046964 @ 0x00046964
// 来源: reversed/cgminer/12_freq_pll/FUN_00046964.c | 分类: 12_freq_pll
// 语义: 创建 turbo 模式监控线程 (turbo_mode_monitor_func, FUN_0004388c),
//       流程与返回值同上 (成功 0 / 失败 -5)。
void *create_bitmain_turbo_mode_monitor_pthread(void)
{
	int ret;
	char msg[4100];

	DAT_0009d4b0 = calloc(1, 0x40);
	ret = thr_info_create(DAT_0009d4b0, 0, turbo_mode_monitor_func, 0);
	if (ret == 0) {
		pthread_detach(*(pthread_t *)((char *)DAT_0009d4b0 + 0xC));
		cgsleep_ms(500);
		return (void *)0;
	}
	if (DAT_00078e09 != '\0' && LOG_GATE(7)) {
		snprintf(msg, 0x1000, "%s: create thread for check miner_status",
		         "create_bitmain_turbo_mode_monitor_pthread");
		applog(7, msg, 0);
	}
	return (void *)-5;
}

// === save_freq_to_every_chain_pic === 原 FUN_00046b94 @ 0x00046b94
// 来源: reversed/cgminer/12_freq_pll/FUN_00046b94.c | 分类: 12_freq_pll
// 语义: 把 4 条链的频率数据逐链写入 PIC16F1704 flash — 每链加锁 (0x790d0)、
//       置当前链号 DAT_0007909c、调 write_data_to_PIC16F1704_flash(buf,0,9)
//       写 9 字节、睡 100ms 后解锁; 数据指针每链前移 9 字节。返回最后一次写
//       结果 (初值 1)。
uint8_t save_freq_to_every_chain_pic(void *buf)
{
	int chain;
	uint8_t ret = 1;

	for (chain = 0; chain < 4; chain++) {
		if (DAT_0009d4bb[1 + chain] == 1) {
			pthread_mutex_lock(&DAT_000790d0);
			DAT_0007909c = chain;
			ret = write_data_to_PIC16F1704_flash(buf, 0, 9);
			cgsleep_ms(100);
			pthread_mutex_unlock(&DAT_000790d0);
		}
		buf = (char *)buf + 9;
	}
	return ret;
}

/* ================= 模块内前向声明 (定义在本文件后段) ================= */

int  blake2b_update(void *state, const void *in, uint64_t inlen);        /* FUN_00054f8c */
int  blake2b_final(void *state, void *out, uint32_t outlen);             /* FUN_000551d4 */
void *jsonp_malloc(size_t size);                                         /* FUN_0005956c */
void  jsonp_free(void *ptr);                                             /* FUN_00059594 */
void  json_delete(json_t *value);                                        /* FUN_0005a3e4 */
const char *strbuffer_value(strbuffer_t_f1 *s);                          /* FUN_00059694 */
void  strbuffer_close(strbuffer_t_f1 *s);                                /* FUN_00059654 — 定义在 pool_p3.c */
void  strbuffer_append_byte(strbuffer_t_f1 *s, uint8_t c);               /* FUN_00059774 */
uint8_t strbuffer_pop(strbuffer_t_f1 *s);                                /* FUN_000597c0 */
void  stream_unget(lex_t_f1 *stream, int c);                             /* FUN_00058274 */
int   utf8_check_first(uint8_t first);                                   /* FUN_00059ac4 */
int   utf8_check_full(const uint8_t *str, int length, uint32_t *codepoint); /* FUN_00059b40 */
void  jsonp_error_set(json_error_t *error, int line, int column, size_t position,
                      const char *fmt, ...);                             /* FUN_0005b098 */
void  jsonp_error_init(json_error_t *error, const char *source);         /* FUN_0005b014 */

/* 其他批次定义 (name_map 语义名) */
extern int   strbuffer_init(strbuffer_t_f1 *s);                          /* FUN_0005961c */
extern uint32_t lex_scan(lex_t_f1 *lex, json_error_t *error);            /* FUN_0005852c (freq_p3.c) */
extern json_t *parse_value(lex_t_f1 *lex, uint32_t flags, json_error_t *error); /* FUN_00058d00 (freq_p3.c) */
extern char  *jsonp_strdup(const char *str);                             /* FUN_000595b4 (其他批次) */
extern int   hashtable_set(hashtable_t_f1 *hashtable, const char *key, size_t hash,
                           json_t *value);                               /* FUN_00057a8c (freq_p3.c) */
extern void __assert_fail(const char *assertion, const char *file, unsigned int line, const char *function); /* glibc, 原调用保留 */
extern int   fun_00057fd8(char *buf, size_t len, void *data);            /* FUN_00057fd8 命名待补
                                                                          * (json_load_callback 取字符回调) */
/* jansson 静态单例 (json_true/false/null; .data, refcount=-1 永久对象) */
extern uint32_t DAT_00076ba8[];   /* json_true  单例 */
extern uint32_t DAT_00076bb0[];   /* json_false 单例 */
extern uint32_t DAT_00076bb8[];   /* json_null  单例 */

/* jansson 自定义分配钩子 (jsonp_malloc/jsonp_free 经此调用; 语义由调用证实)
 * [存证互引] pool_p3.c jsonp_strdup (FUN_000595b4) 亦经 DAT_00076ba0 分配, 互为存证 */
extern void *(*DAT_00076ba0)(size_t size);
extern void  (*DAT_00076ba4)(void *ptr);

extern void  blake2b_compress(void *state, const void *block);           /* FUN_0004ef68 */
extern int   blake2b_init(void *state, size_t outlen);                   /* FUN_00054ed0 (freq_p3.c) */
extern int   blake2b_init_key(void *state, size_t outlen, const void *key, size_t keylen); /* FUN_00055098 (freq_p3.c) */
extern int   __aeabi_idiv(int numerator, int denominator);               /* FUN_0005c718 (freq_p3.c) */
extern void  sha256_process(void *state);                                /* FUN_00055470 (freq_p3.c) */
extern hashtable_pair_t *hashtable_find_pair(hashtable_t_f1 *hashtable, hashtable_bucket_t *bucket,
                                             const char *key, size_t hash); /* FUN_0005791c */
/* __umodsi3 (FUN_0005c6f0): ARM RTABI 商在 r0/余在 r1 成对返回, 调用点取余 —
 * 以 C 取余运算符 % 等价转写 (除 0 时两侧同为 SIGFPE 语义)。 */

/* ============================================================================
 * B. Equihash 辅助 (FUN_0004df30..FUN_0004ea58)
 *    上游对照: zcash equihash sort-verify.cpp (assert 串为证)
 * ========================================================================== */

// === equihash_leaf_hash === 原 FUN_0004df30 @ 0x0004df30
// 来源: reversed/cgminer/12_freq_pll/FUN_0004df30.c | 分类: 12_freq_pll
// 语义: Equihash 叶子哈希 — 拷贝调用方给出的 360 字节 (0x168) blake2b_state
//       中间状态, 以 blake2b_update 追加 4 字节字 (param_2) 后 blake2b_final
//       收尾, 输出 param_3 (长度 param_4 字节)。
void equihash_leaf_hash(const void *state_snapshot, uint32_t tail_word, void *out, uint8_t out_len)
{
	uint32_t word = tail_word;    /* 原体栈字: 状态尾部追加的 4 字节 */
	uint8_t state[364];           /* 原体栈帧: blake2b_state 快照 (实用 0x168) */

	memcpy(state, state_snapshot, 0x168);
	blake2b_update(state, &word, 4);
	blake2b_final(state, out, out_len);
}

// === expandArray === 原 FUN_0004df94 @ 0x0004df94
// 来源: reversed/cgminer/12_freq_pll/FUN_0004df94.c | 分类: 12_freq_pll
// 语义: 位解包 — 把 in[] 按 MSBFirst 逐位流入 32 位累加器, 每凑满 bit_len 位
//       输出一个元素: 先清零前 out_width 字节, 再从第 out_width 字节起写
//       ceil(bit_len/8) 字节数据 (每元素共 out_width + ceil(bit_len/8) 字节)。
//       移位量为 ARM 寄存器移位 (模 256); 常量 0x1FFFFFFF 用于把移位偏移
//       折算到累加器内真实位位。三条 assert 原样保留。
void expandArray(const uint8_t *in, uint32_t in_len, uint8_t *out, uint32_t out_len,
                 uint32_t bit_len, uint32_t out_width)
{
	uint32_t elem_bytes;          /* 每元素输出字节数 (原体 ×16 偏移换算) */
	uint32_t acc;                 /* 位累加器 */
	uint32_t nbits;               /* 累加器内有效位数 */
	uint32_t shift;               /* 移位量 (原体经模 256 折算) */
	uint32_t out_off;             /* 输出写偏移 */
	const uint8_t *p;
	const uint8_t *p_next;
	uint8_t *q;

	if (bit_len < 8)
		__assert_fail("bit_len >= 8", "sort-verify.c", 0x40, "expandArray");
	if (0x20 < bit_len + 7)
		__assert_fail("8 * sizeof(uint32_t) >= 7 + bit_len", "sort-verify.c", 0x41, "expandArray");

	elem_bytes = out_width + ((bit_len + 7) >> 3);
	if (elem_bytes * in_len * 8 / bit_len != out_len)
		__assert_fail("out_len == 8 * out_width * in_len / bit_len", "sort-verify.c", 0x44,
		              "expandArray");

	if (in_len != 0) {
		out_off = 0;
		nbits = 0;
		acc = 0;
		p = in;
		do {
			nbits += 8;
			p_next = p + 1;
			acc = (uint32_t)*p | (acc << 8);
			if (bit_len <= nbits) {
				nbits -= bit_len;
				if (out_width != 0)
					memset(out + out_off, 0, out_width);
				if (out_width < elem_bytes) {
					/* 从元素第 out_width 字节写到末字节, 共 ceil(bit_len/8) 字节 */
					shift = nbits + ((0x1FFFFFFF - out_width) + elem_bytes) * 8;
					q = out + out_off + out_width;
					do {
						*q = (uint8_t)((acc >> (shift & 0xFF)) &
						               ((~(0xFFFFFFFFu << bit_len)) >> ((shift - nbits) & 0xFF)));
						shift -= 8;
						q++;
					} while (q != out + elem_bytes + out_off);
				}
				out_off += elem_bytes;
			}
			p = p_next;
		} while (p_next != in + in_len);
	}
}

// === check_index_tree_order === 原 FUN_0004e134 @ 0x0004e134
// 来源: reversed/cgminer/12_freq_pll/FUN_0004e134.c | 分类: 12_freq_pll
// 语义: 校验/调整索引树序 — 把数组前半 a[0..half) 与后半 a[half..2*half)
//       逐对比较: 一旦做过交换 (swapped 置位) 则后续无条件成对互换; 若
//       a[i] < a[i+half] (仍有序且未交换过) 提前返回; 相等则继续。
void check_index_tree_order(uint32_t *a, int half)
{
	uint32_t *second = a + half;
	uint32_t lo;
	uint32_t hi;
	int i;
	int swapped;

	if (half != 0) {
		i = 0;
		swapped = 0;
		do {
			lo = *a;
			hi = *second;
			if (swapped || hi < lo) {
				*a = hi;
				swapped = 1;
				*second = lo;
			} else if (lo < hi) {
				return;               /* 前半 < 后半: 树序合法, 提前结束 */
			}
			i++;
			a++;
			second++;
		} while (i != half);
	}
}

// === is_entry_less === 原 FUN_0004ea38 @ 0x0004ea38
// 来源: reversed/cgminer/12_freq_pll/FUN_0004ea38.c | 分类: 12_freq_pll
// 语义: 比较两个条目在偏移 offset 起的 len 字节: memcmp 结果为负 (小于) 返回 1,
//       否则 0 (取符号位实现)。
uint32_t is_entry_less(int entry_a, int entry_b, int offset, size_t len)
{
	uint32_t cmp = (uint32_t)memcmp((const void *)(entry_a + offset),
	                                (const void *)(entry_b + offset), len);
	return cmp >> 31;
}

// === check_duplicate_indices === 原 FUN_0004ea58 @ 0x0004ea58
// 来源: reversed/cgminer/12_freq_pll/FUN_0004ea58.c | 分类: 12_freq_pll
// 语义: 查重 — 对 buf_a/buf_b 在 offset 起、步进 4 字节、共 len/4 个 u32 索引
//       两两比对, 任一相等返回 0 (有重复), 全部相异返回 1。
int check_duplicate_indices(int buf_a, int buf_b, int offset, uint32_t len)
{
	int cmp;
	uint32_t i;
	uint32_t j;

	if (len != 0) {
		i = 0;
		do {
			j = 0;
			do {
				cmp = memcmp((const void *)(buf_a + offset + i),
				             (const void *)(buf_b + offset + j), 4);
				if (cmp == 0)
					return 0;
				j += 4;
			} while (j < len);
			i += 4;
		} while (i < len);
	}
	return 1;
}

/* ============================================================================
 * C. Blake2b (FUN_00054f8c/FUN_000551d4/FUN_00055388) 与 SHA-256 预筛侧
 *    (FUN_0005576c/FUN_000557d4/FUN_00055930) — 第三方库码按体转写
 * ========================================================================== */

// === blake2b_update === 原 FUN_00054f8c @ 0x00054f8c
// 来源: reversed/cgminer/12_freq_pll/FUN_00054f8c.c | 分类: 12_freq_pll
// 语义: blake2b_update — 输入流入双块缓冲 (256B): 剩余能全部放入则只追加即返回;
//       否则填满 256B 后按 128B 分组压缩 (counter 128 位累加 0x80), 缓冲后半块
//       前移, 循环至输入耗尽 (Ghidra 将 64 位 inlen 拆为 r2:r3, 此处还原为 u64)。
int blake2b_update(void *state_p, const void *in_p, uint64_t inlen)
{
	blake2b_state_f1 *state = state_p;
	const uint8_t *in = in_p;
	uint32_t space;               /* __n: 本次可填字节数 */
	uint32_t t_lo, wrap;
	uint32_t in_lo, in_hi;
	int32_t buflen;
	int borrow;

	if (inlen == 0)
		return 0;

	in_lo = (uint32_t)inlen;
	in_hi = (uint32_t)(inlen >> 32);
	buflen = (int32_t)state->buflen;
	do {
		space = 0x100 - buflen;
		if (in_hi == 0 && in_lo <= space) {
			/* 剩余输入全部放入缓冲即返回 */
			memcpy(state->buf + buflen, in, in_lo);
			state->buflen = (uint32_t)(buflen + (int32_t)in_lo);
			return 0;
		}
		memcpy(state->buf + buflen, in, space);
		/* 128 位计数器: counter[0..1] (低 64 位) += 0x80 */
		t_lo = state->counter[0] + 0x80;
		state->counter[1] += (uint32_t)(0xFFFFFF7Fu < state->counter[0]);  /* +0x80 进位 (用旧值判定) */
		state->counter[0] = t_lo;
		/* counter[2..3] (高 64 位) += 低 64 位加法回绕进位 */
		wrap = (state->counter[1] == 0 && t_lo < 0x80) ? 1u : 0u;
		{
			uint32_t hi_lo = state->counter[2];
			in = in + space;
			state->counter[2] = hi_lo + wrap;
			state->counter[3] += (uint32_t)((hi_lo + wrap) < hi_lo);
		}
		state->buflen = (uint32_t)(buflen + (int32_t)space);
		borrow = (in_lo < space) ? 1 : 0;
		in_lo -= space;
		in_hi -= (uint32_t)borrow;
		blake2b_compress(state, state->buf);
		memcpy(state->buf, state->buf + 0x80, 0x80);   /* 第二块前移 */
		buflen = (int32_t)state->buflen - 0x80;
		state->buflen = (uint32_t)buflen;
	} while (in_lo != 0 || in_hi != 0);
	return 0;
}

// === blake2b_final === 原 FUN_000551d4 @ 0x000551d4
// 来源: reversed/cgminer/12_freq_pll/FUN_000551d4.c | 分类: 12_freq_pll
// 语义: blake2b_final — out/outlen 合法且未终结时: 缓冲超 128B 先压缩一块并把
//       剩余字节前移; 计数器累加缓冲字节数; 置 f[0] (及 last_node 时 f[1]) 为
//       ~0, 补零后压缩, h 以小端序串出并拷贝 outlen 字节到 out; 返回 0, 参数
//       非法/已终结返回 -1。
int blake2b_final(void *state_p, void *out_p, uint32_t outlen)
{
	blake2b_state_f1 *state = state_p;
	uint8_t *out = out_p;
	uint8_t hash64[64];           /* 原体 64B 栈缓冲: h 的小端串出 */
	uint32_t t_lo, wrap;
	uint32_t buflen;
	uint32_t hi_lo;
	size_t remain;
	int i;
	uint32_t lo32, hi32;
	uint8_t *p;

	memset(hash64, 0, 0x40);
	if (out_p == NULL || outlen == 0 || outlen > 0x40 ||
	    state->f[0] != 0 || state->f[1] != 0)
		return -1;

	buflen = state->buflen;
	if (buflen > 0x80) {
		/* 先压缩满的第一块 */
		t_lo = state->counter[0] + 0x80;
		state->counter[1] += (uint32_t)(0xFFFFFF7Fu < state->counter[0]);
		state->counter[0] = t_lo;
		wrap = (state->counter[1] == 0 && t_lo < 0x80) ? 1u : 0u;
		hi_lo = state->counter[2];
		state->counter[2] = hi_lo + wrap;
		state->counter[3] += (uint32_t)((hi_lo + wrap) < hi_lo);
		blake2b_compress(state, state->buf);
		remain = (size_t)(state->buflen - 0x80);
		state->buflen = (uint32_t)remain;
		memcpy(state->buf, state->buf + 0x80, remain);   /* 剩余字节前移 */
		buflen = state->buflen;
	}
	/* 计数器 += 缓冲剩余字节数 (128 位) */
	t_lo = state->counter[0] + buflen;
	state->counter[1] += (uint32_t)((t_lo < buflen) ? 1 : 0);   /* CARRY4(counter[0], buflen) */
	state->counter[0] = t_lo;
	wrap = (state->counter[1] == 0 && t_lo < buflen) ? 1u : 0u;
	hi_lo = state->counter[2];
	state->counter[2] = hi_lo + wrap;
	state->counter[3] += (uint32_t)((hi_lo + wrap) < hi_lo);

	if (state->last_node != 0)                 /* last_node → f[1] (u64 @+0x58) = ~0 */
		state->f[2] = state->f[3] = 0xFFFFFFFFu;
	state->f[0] = state->f[1] = 0xFFFFFFFFu;   /* f[0] (u64 @+0x50) = ~0 */
	memset(state->buf + buflen, 0, 0x100 - buflen);
	blake2b_compress(state, state->buf);

	for (i = 0; i < 8; i++) {                  /* h 16×u32 逐个 LE 串出 (原体 8×8B 循环) */
		lo32 = state->h[2 * i];
		hi32 = state->h[2 * i + 1];
		p = hash64 + 8 * i;
		p[0] = (uint8_t)lo32;
		p[1] = (uint8_t)(lo32 >> 8);
		p[2] = (uint8_t)(lo32 >> 16);
		p[3] = (uint8_t)(lo32 >> 24);
		p[4] = (uint8_t)hi32;
		p[5] = (uint8_t)(hi32 >> 8);
		p[6] = (uint8_t)(hi32 >> 16);
		p[7] = (uint8_t)(hi32 >> 24);
	}
	memcpy(out, hash64, outlen);
	return 0;
}

// === blake2b === 原 FUN_00055388 @ 0x00055388
// 来源: reversed/cgminer/12_freq_pll/FUN_00055388.c | 分类: 12_freq_pll
// 语义: blake2b 一次性哈希 — 校验 (out 非空; in 空则 inlen 须为 0; key 空则
//       keylen 须为 0; 1<=outlen<=64; keylen<=64), 按有无 key 调 blake2b_init /
//       blake2b_init_key, 成功则 update + final 后返回 0, 否则 -1。
//       inlen 为 64 位 (Ghidra 拆为两个栈上参数)。
int blake2b(void *out, const void *in, const void *key,
            uint32_t outlen, uint64_t inlen, uint8_t keylen)
{
	blake2b_state_f1 state;       /* 原体 364B 栈上状态 */
	int ret;

	if (((in != NULL || inlen == 0) && out != NULL) &&
	    (key != NULL || keylen == 0) &&
	    ((outlen - 1u < 0x40) && (keylen < 0x41))) {
		if (keylen != 0)
			ret = blake2b_init_key(&state, outlen, key, keylen);
		else
			ret = blake2b_init(&state, outlen);
		if (ret >= 0) {
			blake2b_update(&state, in, inlen);
			blake2b_final(&state, out, outlen);
			return 0;
		}
	}
	return -1;
}

// === sha256_update_prescreen === 原 FUN_0005576c @ 0x0005576c
// 来源: reversed/cgminer/12_freq_pll/FUN_0005576c.c | 分类: 12_freq_pll
// 语义: 预筛侧 SHA-256 update — 逐字节入缓冲 (bitlen 64 位递增), 攒满 64 字节
//       调 sha256_process 压缩 (与挖矿主哈希 0004dbec 侧实现相互独立)。
void sha256_update_prescreen(void *state_p, const uint8_t *in, int len)
{
	sha256_ctx_f1 *state = state_p;
	uint32_t idx;
	uint32_t lo;
	int i;

	if (len == 0)
		return;
	idx = state->bitlen & 0x3F;
	for (i = 0; i < len; i++) {
		state->buf[idx] = in[i];
		lo = (uint32_t)state->bitlen;          /* 64 位位长 +1, 进位到高字 */
		state->bitlen = lo + 1;
		*(uint32_t *)((char *)state + 0x24) += (uint32_t)(0xFFFFFFFEu < lo);
		idx++;
		if (idx == 0x40) {
			sha256_process(state);
			idx = 0;
		}
	}
}

// === sha256_final_prescreen === 原 FUN_000557d4 @ 0x000557d4
// 来源: reversed/cgminer/12_freq_pll/FUN_000557d4.c | 分类: 12_freq_pll
// 语义: 预筛侧 SHA-256 final — 补 0x80 与零至 56 字节 (跨块时先压缩), 写 64 位
//       大端位长后压缩; 32 字节摘要以大端写入 out; 最后把上下文复位为 IV + 位长
//       0 (可复用上下文)。
void sha256_final_prescreen(void *state_p, uint8_t *out)
{
	sha256_ctx_f1 *state = state_p;
	uint32_t idx;
	uint64_t bits;
	int i;
	uint32_t word;

	idx = (uint32_t)state->bitlen & 0x3F;
	bits = state->bitlen << 3;
	state->buf[idx] = 0x80;
	for (;;) {
		idx++;
		if (idx == 0x38)
			break;
		while (idx = idx & 0x3F, idx == 0) {
			sha256_process(state);
			state->buf[0] = 0;
			idx = 1;
		}
		state->buf[idx] = 0;
	}
	/* 64 位大端位长写入 buf[0x38..0x3F] (原体为双字旋转逐字节写) */
	state->buf[0x38] = (uint8_t)(bits >> 56);
	state->buf[0x39] = (uint8_t)(bits >> 48);
	state->buf[0x3A] = (uint8_t)(bits >> 40);
	state->buf[0x3B] = (uint8_t)(bits >> 32);
	state->buf[0x3C] = (uint8_t)(bits >> 24);
	state->buf[0x3D] = (uint8_t)(bits >> 16);
	state->buf[0x3E] = (uint8_t)(bits >> 8);
	state->buf[0x3F] = (uint8_t)bits;
	sha256_process(state);

	for (i = 0; i < 32; i += 4) {              /* h 逐字大端写入 out */
		word = state->h[i / 4];
		out[i + 0] = (uint8_t)(word >> 24);
		out[i + 1] = (uint8_t)(word >> 16);
		out[i + 2] = (uint8_t)(word >> 8);
		out[i + 3] = (uint8_t)word;
	}
	/* 复位上下文: 位长清零 + SHA-256 IV (原体逐字重写) */
	state->bitlen = 0;
	state->h[0] = 0x6A09E667u;
	state->h[1] = 0xBB67AE85u;
	state->h[2] = 0x3C6EF372u;
	state->h[3] = 0xA54FF53Au;
	state->h[4] = 0x510E527Fu;
	state->h[5] = 0x9B05688Cu;
	state->h[6] = 0x1F83D9ABu;
	state->h[7] = 0x5BE0CD19u;
}

// === sha256_oneshot === 原 FUN_00055930 @ 0x00055930
// 来源: reversed/cgminer/12_freq_pll/FUN_00055930.c | 分类: 12_freq_pll
// 语义: 预筛侧 SHA-256 一次性摘要 — 栈上上下文置 IV + 位长 0, update(data,len)
//       后 final(out)。
void sha256_oneshot(const uint8_t *data, int len, uint8_t *out)
{
	sha256_ctx_f1 ctx;            /* 原体栈上上下文 */

	ctx.h[0] = 0x6A09E667u;
	ctx.h[1] = 0xBB67AE85u;
	ctx.h[2] = 0x3C6EF372u;
	ctx.h[3] = 0xA54FF53Au;
	ctx.h[4] = 0x510E527Fu;
	ctx.h[5] = 0x9B05688Cu;
	ctx.h[6] = 0x1F83D9ABu;
	ctx.h[7] = 0x5BE0CD19u;
	ctx.bitlen = 0;
	sha256_update_prescreen(&ctx, data, len);
	sha256_final_prescreen(&ctx, out);
}

/* ============================================================================
 * D. jansson hashtable (FUN_000578c0..FUN_00057d0c) — 第三方库码按体转写
 * ========================================================================== */

// === insert_to_bucket === 原 FUN_000578c0 @ 0x000578c0
// 来源: reversed/cgminer/12_freq_pll/FUN_000578c0.c | 分类: 12_freq_pll
// 语义: 把 pair 的 list 节点挂入桶 — 空桶 (first/last 均指向全局链表头) 时
//       追加到全局链表尾并独占该桶; 非空桶时插到 bucket->first 之前。
void insert_to_bucket(hashtable_t_f1 *hashtable, hashtable_bucket_t *bucket, hashtable_list_t *node)
{
	hashtable_list_t *first = bucket->first;
	hashtable_list_t *old_prev;

	if (first == &hashtable->list && first == bucket->last) {
		/* 空桶: 追加到全局链表尾 */
		node->next = first;
		node->prev = hashtable->list.prev;
		old_prev = hashtable->list.prev;
		hashtable->list.prev = node;
		old_prev->next = node;
		bucket->last = node;
		bucket->first = node;
		return;
	}
	/* 非空桶: 插到 first 之前 */
	node->prev = first->prev;
	node->next = first;
	old_prev = first->prev;
	first->prev = node;
	old_prev->next = node;
	bucket->first = node;
}

// === hashtable_clear === 原 FUN_00057994 @ 0x00057994
// 来源: reversed/cgminer/12_freq_pll/FUN_00057994.c | 分类: 12_freq_pll
// 语义: 遍历全局链表逐 pair 释放 — value 引用计数减一 (永久对象 -1 跳过),
//       减到 0 调 json_delete, 再 jsonp_free(pair 基址 = 节点-4); 不清 size。
void hashtable_clear(hashtable_t_f1 *hashtable)
{
	hashtable_list_t *node;
	hashtable_pair_t *pair;
	json_t *value;
	int refcount;

	node = hashtable->list.next;
	if (node == &hashtable->list)
		return;
	do {
		value = *(json_t **)((char *)node + 8);         /* pair->value (pair+12) */
		pair = (hashtable_pair_t *)((char *)node - 4);    /* pair 基址 (node = &pair->list) */
		node = node->next;
		if (value != 0) {
			refcount = value->refcount - 1;
			if (value->refcount != -1) {                /* -1 = 永久对象 */
				value->refcount = refcount;
				if (refcount == 0)
					json_delete(value);
			}
		}
		jsonp_free(pair);
	} while (node != &hashtable->list);
}

// === hashtable_init === 原 FUN_000579f4 @ 0x000579f4
// 来源: reversed/cgminer/12_freq_pll/FUN_000579f4.c | 分类: 12_freq_pll
// 语义: 初始化哈希表 — size=0, num_buckets=0, 按 prime_list[0]=5 分配桶数组
//       (5×8B), 全局链表头自指, 每桶 first/last 均指向链表头; 失败返回 -1。
int hashtable_init(hashtable_t_f1 *hashtable)
{
	hashtable_list_t *head = &hashtable->list;
	size_t prime;
	size_t i;

	hashtable->size = 0;
	hashtable->num_buckets = 0;
	hashtable->buckets = jsonp_malloc(0x28);
	if (hashtable->buckets == NULL)
		return -1;
	hashtable->list.next = head;
	hashtable->list.prev = head;
	prime = DAT_0006cb74[hashtable->num_buckets];
	if (prime != 0) {
		for (i = 0; i != prime * 8; i += 8) {       /* 原体按字节偏移步进 8 */
			hashtable->buckets[i / 8].first = head;
			hashtable->buckets[i / 8].last = head;
		}
	}
	return 0;
}

// === hashtable_free === 原 FUN_00057a74 @ 0x00057a74
// 来源: reversed/cgminer/12_freq_pll/FUN_00057a74.c | 分类: 12_freq_pll
// 语义: 释放哈希表 — 先 hashtable_clear 清空链表, 再释放桶数组。
void hashtable_free(hashtable_t_f1 *hashtable)
{
	hashtable_clear(hashtable);
	jsonp_free(hashtable->buckets);
}

// === hashtable_get === 原 FUN_00057d0c @ 0x00057d0c
// 来源: reversed/cgminer/12_freq_pll/FUN_00057d0c.c | 分类: 12_freq_pll
// 语义: 按 key 定位并删除 pair (即 jansson hashtable_del, name_map 冻结名为
//       hashtable_get) — djb2 (seed 0x1505, hash=hash*33+c) 散列, 对
//       prime_list[num_buckets] 取余定位桶 (原体调 __umodsi3 取 r1 余数),
//       hashtable_find_pair 查找; 命中则从桶链摘除、全局链表摘除、value 减引
//       (减到 0 调 json_delete)、jsonp_free(pair)、size-- ; 未命中返回 -1。
int hashtable_get(hashtable_t_f1 *hashtable, const char *key)
{
	uint32_t hash;
	uint32_t bucket_idx;
	hashtable_bucket_t *bucket;
	hashtable_pair_t *pair;
	hashtable_list_t *list;
	json_t *value;
	int refcount;
	const uint8_t *p;

	hash = 0x1505;
	for (p = (const uint8_t *)key; *p != 0; p++)
		hash = (uint32_t)*p + hash * 0x21;

	bucket_idx = hash % DAT_0006cb74[hashtable->num_buckets];   /* __umodsi3 (r1=余) */
	bucket = &hashtable->buckets[bucket_idx];
	pair = hashtable_find_pair(hashtable, bucket, key, hash);
	if (pair == 0)
		return -1;

	list = &pair->list;
	if (list == bucket->first) {
		hashtable_list_t *new_first;
		if (list == bucket->last)
			new_first = &hashtable->list;
		else
			new_first = list->next;               /* 新桶首 = 原 first 的 next */
		bucket->first = new_first;
		if (list == bucket->last)
			bucket->last = new_first;
	} else if (list == bucket->last) {
		bucket->last = list->prev;
	}
	/* 从全局双向链表摘除 */
	list->prev->next = list->next;
	list->next->prev = list->prev;
	/* value 引用计数减一, 减到 0 释放 */
	value = pair->value;
	if (value != 0 && value->refcount != -1) {
		refcount = value->refcount - 1;
		value->refcount = refcount;
		if (refcount == 0)
			json_delete(value);
	}
	jsonp_free(pair);
	hashtable->size--;
	return 0;
}

/* ============================================================================
 * E. jansson 词法/装载辅助 (FUN_00058034..FUN_000584fc) — 第三方库码按体转写
 *    上游对照: jansson load.c (assert 串 "load.c" 为证)
 * ========================================================================== */

// === error_set === 原 FUN_00058034 @ 0x00058034
// 来源: reversed/cgminer/12_freq_pll/FUN_00058034.c | 分类: 12_freq_pll
// 语义: 构造 JSON 解析错误消息 — vsnprintf 展开用户消息; lex 为空时以
//       line=-1/column=-1/position=0 记录; 否则按 token 文本取舍: 无文本且
//       state != -2 (ERROR) 时追加 "near end of file", 文本长度 < 0x15 时显示
//       "near '<token>'"; 最后经 jsonp_error_set 写入 error (注: 反编译体仅有
//       error!=0 判断, 无 error->text 已占用检查, 按体保留)。
void error_set(json_error_t *error, lex_t_f1 *lex, const char *fmt, ...)
{
	char msg_text[160];           /* 原体 160B 栈缓冲 (消息文本) */
	char msg2[160];               /* 原体 160B 栈缓冲 (拼合消息) */
	va_list ap;
	const char *message;
	uint32_t line;
	uint32_t column;
	uint32_t position;
	const char *token;

	if (error == 0)
		return;

	va_start(ap, fmt);
	vsnprintf(msg_text, 0xA0, fmt, ap);   /* 原体以栈上实参区作 va_list */
	va_end(ap);
	msg_text[159] = 0;

	if (lex == 0) {
		message = msg_text;
		line = (uint32_t)-1;
		column = (uint32_t)-1;
		position = 0;
	} else {
		token = strbuffer_value(&lex->value);
		line = *(uint32_t *)((char *)lex + 0x18);
		column = *(uint32_t *)((char *)lex + 0x1C);
		position = *(uint32_t *)((char *)lex + 0x24);
		if (token == 0 || *token == 0) {
			message = msg_text;
			if (*(int *)((char *)lex + 0x14) != -2) {   /* state != STREAM_ERROR */
				snprintf(msg2, 0xA0, "%s near end of file", message);
				message = msg2;
			}
		} else if (*(uint32_t *)((char *)lex + 0x2C) < 0x15) {   /* token 文本较短 */
			snprintf(msg2, 0xA0, "%s near '%s'", msg_text, token);
			message = msg2;
		} else {
			message = msg_text;
		}
	}
	jsonp_error_set(error, (int)line, (int)column, position, "%s", message);
}

// === lex_init === 原 FUN_00058154 @ 0x00058154
// 来源: reversed/cgminer/12_freq_pll/FUN_00058154.c | 分类: 12_freq_pll
// 语义: 初始化词法器 — 记录取字符函数/数据, state=OK(0), buffer_pos=0,
//       line=1, column/position=0, strbuffer_init(value); 成功后置 +0x34=-1 并
//       返回 0, strbuffer 分配失败返回 -1。
int lex_init(lex_t_f1 *lex, int (*get)(void *data), void *data)
{
	lex->get = get;
	lex->data = data;
	lex->buffer[0] = 0;          /* 原体写 +0x08 单字节 */
	lex->buffer_pos = 0;
	lex->state = 0;
	lex->column = 0;
	lex->position = 0;
	lex->line = 1;
	if (strbuffer_init(&lex->value) == 0) {
		lex->token = -1;           /* TOKEN_INVALID */
		return 0;
	}
	return -1;
}

// === lex_cleanup === 原 FUN_000581a0 @ 0x000581a0
// 来源: reversed/cgminer/12_freq_pll/FUN_000581a0.c | 分类: 12_freq_pll
// 语义: 释放词法器 — token == 0x100 (字符串 token) 时先释放 +0x38 字符串缓冲,
//       再 strbuffer_close(token 文本缓冲)。
void lex_cleanup(lex_t_f1 *lex)
{
	if (lex->token == 0x100)
		jsonp_free(lex->field_0x38);
	strbuffer_close(&lex->value);
}

// === decode_unicode_escape === 原 FUN_000581cc @ 0x000581cc
// 来源: reversed/cgminer/12_freq_pll/FUN_000581cc.c | 分类: 12_freq_pll
// 语义: 解析 "\uXXXX" 的 4 位十六进制 — 首字符须为 'u' (assert), 依次取
//       str[1..4]: 数字 0-9 / 小写 a-z / 大写 A-Z 各按标准映射, 非法字符触发
//       assert (load.c:300); 按十六进制累加返回。
int decode_unicode_escape(const char *str)
{
	int value = 0;
	int i;
	uint32_t ch;
	uint32_t digit;

	if (*str != 'u')
		__assert_fail("str[0] == 'u'", "load.c", 0x120, "decode_unicode_escape");

	for (i = 1; i != 5; i++) {
		ch = (uint8_t)str[i];
		digit = ch - 0x30;
		if (9 < (digit & 0xFF)) {
			if (ch - 0x61 < 0x1A)                    /* 小写 a-z */
				digit = ch - 0x57;
			else {
				if (0x19 < ch - 0x41)                /* 非大写 A-Z: assert */
					__assert_fail("0", "load.c", 300, "decode_unicode_escape");
				digit = ch - 0x37;
			}
		}
		value = (int)digit + value * 0x10;
	}
	return value;
}

// === stream_unget === 原 FUN_00058274 @ 0x00058274
// 来源: reversed/cgminer/12_freq_pll/FUN_00058274.c | 分类: 12_freq_pll
// 语义: 回退一个已读字符 — EOF/ERROR 值直接返回; 否则回退行/列计数
//       ('\n' 时 column=last_column、line--; 首字节时 column--), 并断言
//       缓冲非空且缓冲顶字节与 c 一致 (load.c:0xe0/0xe2)。
void stream_unget(lex_t_f1 *stream, int c)
{
	size_t pos;

	if ((uint32_t)(c + 2) < 2)                   /* c = -1(EOF) / -2(ERROR) */
		return;
	stream->position--;
	if (c == 10) {                               /* '\n' */
		stream->column = stream->last_column;
		stream->line--;
	} else if (utf8_check_first((uint8_t)c) != 0) {
		stream->column--;
	}
	if (stream->buffer_pos == 0)
		__assert_fail("stream->buffer_pos > 0", "load.c", 0xE0, "stream_unget");
	pos = stream->buffer_pos - 1;
	stream->buffer_pos = pos;
	if (stream->buffer[pos] != (uint8_t)c)
		__assert_fail("stream->buffer[stream->buffer_pos] == c", "load.c", 0xE2,
		              "stream_unget");
}

// === stream_get === 原 FUN_0005832c @ 0x0005832c
// 来源: reversed/cgminer/12_freq_pll/FUN_0005832c.c | 分类: 12_freq_pll
// 语义: 读取下一字符 — state != OK 时直接返回 state; 缓冲有回退字节则取之;
//       否则调 get(): EOF(-1) 置 state=EOF 返回 -1; ASCII (c<0x80) 单字节入
//       缓冲 (buffer[1]=0 结束标记); 0x80..0xFF 首字节则按 utf8_check_first 的
//       长度读满续字节、utf8_check_full 校验 (失败置 state=ERROR 并
//       error_set "unable to decode byte 0x%x" 返回 -2), 成功后 buffer[count]=0
//       并返回首字节。每次读取 position++、'\n' 时换行计数, 其余首字节 column++。
uint32_t stream_get(lex_t_f1 *stream, json_error_t *error)
{
	uint32_t c;
	uint32_t count;
	size_t pos;
	uint32_t old_pos;

	if (stream->state != 0)                      /* STREAM_STATE_OK */
		return (uint32_t)stream->state;

	c = stream->buffer[stream->buffer_pos];
	if (c == 0) {
		c = (uint32_t)stream->get(stream->data);
		if (c == 0xFFFFFFFFu) {                  /* get() == EOF */
			stream->state = -1;
			return 0xFFFFFFFFu;
		}
		stream->buffer_pos = 0;
		stream->buffer[0] = (uint8_t)c;
		if (0x7F < c - 0x80)                     /* ASCII: 单字节, buffer[1]=0 结束 */
			stream->buffer[1] = 0;
		if (c - 0x80 < 0x80) {                   /* 0x80..0xFF: 多字节首字节 */
			count = (uint32_t)utf8_check_first((uint8_t)c);
			if (count != 0) {
				uint32_t k;
				if (count < 2)
					__assert_fail("count >= 2", "load.c", 0xAF, "stream_get");
				for (k = 1; k != count; k++)     /* 读 count-1 个续字节 */
					stream->buffer[k] = (uint8_t)stream->get(stream->data);
				if (utf8_check_full(stream->buffer, (int)count, 0) != 0) {
					old_pos = stream->buffer_pos;    /* = 0 */
					stream->buffer[count] = 0;
					stream->buffer_pos = old_pos + 1;
					c = stream->buffer[old_pos];     /* 返回首字节 */
					pos = old_pos + 1;
					goto advance;
				}
			}
			stream->state = -2;                      /* STREAM_STATE_ERROR */
			error_set(error, stream, "unable to decode byte 0x%x", c);
			return 0xFFFFFFFEu;
		}
		pos = 1;                                 /* ASCII: buffer_pos = 1 */
	} else {
		pos = stream->buffer_pos + 1;
	}

advance:
	stream->buffer_pos = pos;
	stream->position++;
	if (c == 10) {                               /* '\n' */
		stream->last_column = stream->column;
		stream->column = 0;
		stream->line++;
		return 10;
	}
	if (utf8_check_first((uint8_t)c) != 0)
		stream->column++;
	return c;
}

// === lex_unget_unsave === 原 FUN_000584b0 @ 0x000584b0
// 来源: reversed/cgminer/12_freq_pll/FUN_000584b0.c | 分类: 12_freq_pll
// 语义: 回退字符并核对已保存文本 — EOF/ERROR 直接返回; 先 stream_unget,
//       再 strbuffer_pop 弹出保存字节, 断言与 c 相等 (load.c:0x10c)。
void lex_unget_unsave(lex_t_f1 *lex, int c)
{
	int saved;

	if ((uint32_t)(c + 2) < 2)
		return;
	stream_unget(lex, c);
	saved = strbuffer_pop(&lex->value);
	if (saved != c)
		__assert_fail("c == d", "load.c", 0x10C, "lex_unget_unsave");
}

// === lex_get_save === 原 FUN_000584fc @ 0x000584fc
// 来源: reversed/cgminer/12_freq_pll/FUN_000584fc.c | 分类: 12_freq_pll
// 语义: 读取下一字符并保存 — stream_get 取字符; 非 EOF/ERROR (c+2 >= 2) 时
//       把该字节追加进 token 缓冲; 返回读取值。
uint32_t lex_get_save(lex_t_f1 *lex, json_error_t *error)
{
	uint32_t c;

	c = stream_get(lex, error);
	if (1 < c + 2)                               /* c != -1/-2 */
		strbuffer_append_byte(&lex->value, (uint8_t)c);
	return c;
}

/* ============================================================================
 * F. jansson 装载入口 / 内存 / strbuffer (FUN_00059154..FUN_000597c0)
 * ========================================================================== */

// === parse_json === 原 FUN_00059154 @ 0x00059154
// 来源: reversed/cgminer/12_freq_pll/FUN_00059154.c | 分类: 12_freq_pll
// 语义: JSON 解析主入口 — 先 lex_scan; 非 JSON_DISABLE_EOF_CHECK (flags&2) 且
//       首 token 不是 '[' / '{' (token&0xFFFFFFDF 归一到 0x5B) 时报
//       "'[' or '{' expected"; 否则 parse_value; 成功后 (flags&2 或下一 token
//       为 0) 把 position 写回 error->position 并返回值; 多余内容报
//       "end of file expected" 并在引用计数归零时 json_delete。
//       注: Ghidra 拆分中 lex_scan 部分实参为寄存器残迹, 实参 (lex, error)。
json_t *parse_json(lex_t_f1 *lex, uint32_t flags, json_error_t *error)
{
	json_t *value;
	int refcount;

	lex_scan(lex, error);
	if ((flags & 4) == 0 && (lex->token & 0xFFFFFFDF) != 0x5B) {
		error_set(error, lex, "'[' or '{' expected");
	} else {
		value = parse_value(lex, flags, error);
		if (value != 0) {
			if ((flags & 2) != 0 || (lex_scan(lex, error), lex->token == 0)) {
				if (error == 0)
					return value;
				error->position = lex->position;      /* error+8 */
				return value;
			}
			error_set(error, lex, "end of file expected");
			if (value->refcount != -1) {              /* 解析结果减引并释放 */
				refcount = value->refcount - 1;
				value->refcount = refcount;
				if (refcount == 0) {
					json_delete(value);
					return 0;
				}
			}
		}
	}
	return 0;
}

// === json_loadf === 原 FUN_00059360 @ 0x00059360
// 来源: reversed/cgminer/12_freq_pll/FUN_00059360.c | 分类: 12_freq_pll
// 语义: 从 FILE 流装载 JSON — error->source 记 "<stdin>"/"<stream>", 流为空报
//       "wrong arguments"; 否则 lex_init(lex, fgetc, stream) + parse_json +
//       lex_cleanup; 失败返回 NULL。
json_t *json_loadf(FILE *stream, size_t flags, json_error_t *error)
{
	lex_t_f1 lex;                 /* 原体栈上词法器 */
	const char *source;
	json_t *result;

	if (stream == stdin)
		source = "<stdin>";
	else
		source = "<stream>";
	jsonp_error_init(error, source);
	if (stream == 0) {
		error_set(error, 0, "wrong arguments");
		return 0;
	}
	if (lex_init(&lex, (int (*)(void *))fgetc, stream) == 0) {
		result = parse_json(&lex, (uint32_t)flags, error);
		lex_cleanup(&lex);
		return result;
	}
	return 0;
}

// === json_load_callback === 原 FUN_000594bc @ 0x000594bc
// 来源: reversed/cgminer/12_freq_pll/FUN_000594bc.c | 分类: 12_freq_pll
// 语义: 经回调装载 JSON — 0x410 字节回调上下文清零后存 {callback, data};
//       error->source = "<callback>"; 回调为空报 "wrong arguments"; 否则
//       lex_init(lex, fun_00057fd8, ctx) + parse_json + lex_cleanup。
//       注: fun_00057fd8 为 0x00057fd8 处的回调取字符包装 (命名待补)。
json_t *json_load_callback(int (*callback)(void *buffer, size_t buflen, void *data),
                           void *data, size_t flags, json_error_t *error)
{
	lex_t_f1 lex;                 /* 原体栈上词法器 */
	uint8_t ctx[1040];            /* 原体栈上回调流上下文 (0x410, 清零) */
	json_t *result;

	memset(ctx, 0, 0x410);
	*(int (**)(void *, size_t, void *))(ctx + 0) = callback;   /* 原体首字: 回调函数 */
	*(void **)(ctx + 4) = data;                   /* 原体次字: 回调数据 */
	jsonp_error_init(error, "<callback>");
	if (callback == 0) {
		error_set(error, 0, "wrong arguments");
		return 0;
	}
	if (lex_init(&lex, (int (*)(void *))fun_00057fd8, ctx) == 0) {
		result = parse_json(&lex, (uint32_t)flags, error);
		lex_cleanup(&lex);
		return result;
	}
	return 0;
}

// === jsonp_malloc === 原 FUN_0005956c @ 0x0005956c
// 来源: reversed/cgminer/12_freq_pll/FUN_0005956c.c | 分类: 12_freq_pll
// 语义: 分配器包装 — size==0 返回 NULL, 否则经全局分配函数指针 DAT_00076ba0
//       (jansson 设定的自定义 malloc) 调用。
void *jsonp_malloc(size_t size)
{
	if (size == 0)
		return 0;
	return (*DAT_00076ba0)(size);
}

// === jsonp_free === 原 FUN_00059594 @ 0x00059594
// 来源: reversed/cgminer/12_freq_pll/FUN_00059594.c | 分类: 12_freq_pll
// 语义: 释放包装 — ptr 非空时经全局释放函数指针 DAT_00076ba4 (自定义 free)。
void jsonp_free(void *ptr)
{
	if (ptr == 0)
		return;
	(*DAT_00076ba4)(ptr);
}

// 注: strbuffer_close (FUN_00059654) 归属 pool_b3 批次 (pool_p3.c 定义),
//     本文件经 extern 声明调用 (lex_cleanup 依赖), 不在此重复定义。

// === strbuffer_clear === 原 FUN_00059680 @ 0x00059680
// 来源: reversed/cgminer/12_freq_pll/FUN_00059680.c | 分类: 12_freq_pll
// 语义: 清空字符串缓冲 — length=0 且 value[0]=NUL, 不释放内存。
void strbuffer_clear(strbuffer_t_f1 *s)
{
	s->length = 0;
	s->value[0] = 0;
}

// === strbuffer_value === 原 FUN_00059694 @ 0x00059694
// 来源: reversed/cgminer/12_freq_pll/FUN_00059694.c | 分类: 12_freq_pll
// 语义: 返回缓冲字符串指针 (value 字段)。
const char *strbuffer_value(strbuffer_t_f1 *s)
{
	return s->value;
}

// === strbuffer_append_bytes === 原 FUN_000596b0 @ 0x000596b0
// 来源: reversed/cgminer/12_freq_pll/FUN_000596b0.c | 分类: 12_freq_pll
// 语义: 追加 len 字节 — 剩余容量足够 (size-length > len) 直接 memcpy 并补 NUL;
//       否则溢出检查 (size>=0, len!=0xFFFFFFFF, length <= -len-2) 后按
//       max(length+1+len, size*2) 经 jsonp_malloc 扩容, 拷旧串并释放旧值;
//       失败返回 -1。
int strbuffer_append_bytes(strbuffer_t_f1 *s, const void *data, size_t len)
{
	size_t length = s->length;
	size_t size = s->size;
	char *dest;

	if (len < size - length) {
		dest = s->value;
append:                              /* 原体 0x0005973c 汇合点 */
		memcpy(dest + length, data, len);
		s->length = len + length;
		s->value[len + length] = 0;
		return 0;
	}
	if ((int)size >= 0 && len != (size_t)-1 && length <= (size_t)(-(int)len - 2)) {
		size_t newsize = length + 1 + len;
		if (newsize < size * 2)
			newsize = size * 2;
		dest = jsonp_malloc(newsize);
		if (dest != 0) {
			memcpy(dest, s->value, s->length);
			jsonp_free(s->value);
			length = s->length;
			s->value = dest;
			s->size = newsize;
			goto append;
		}
	}
	return -1;
}

// === strbuffer_append_byte === 原 FUN_00059774 @ 0x00059774
// 来源: reversed/cgminer/12_freq_pll/FUN_00059774.c | 分类: 12_freq_pll
// 语义: 追加单字节 — 1 字节局部缓冲转发 strbuffer_append_bytes。
void strbuffer_append_byte(strbuffer_t_f1 *s, uint8_t byte)
{
	uint8_t buf[5];               /* 原体 5 字节栈缓冲 (实用 1 字节) */

	buf[0] = byte;
	strbuffer_append_bytes(s, buf, 1);
}

// === strbuffer_pop === 原 FUN_000597c0 @ 0x000597c0
// 来源: reversed/cgminer/12_freq_pll/FUN_000597c0.c | 分类: 12_freq_pll
// 语义: 弹出末字节 — length 为 0 返回 0; 否则 length--, 返回该字节并把
//       value[length] 置 NUL。
uint8_t strbuffer_pop(strbuffer_t_f1 *s)
{
	size_t pos;

	if (s->length == 0)
		return 0;
	pos = s->length - 1;
	s->length = pos;
	{
		uint8_t c = (uint8_t)s->value[pos];
		s->value[pos] = 0;
		return c;
	}
}

/* ============================================================================
 * G. jansson UTF-8 (FUN_000599dc..FUN_00059c8c) — 第三方库码按体转写
 * ========================================================================== */

// === utf8_encode === 原 FUN_000599dc @ 0x000599dc
// 来源: reversed/cgminer/12_freq_pll/FUN_000599dc.c | 分类: 12_freq_pll
// 语义: 把码点编成 UTF-8 写入 buffer 并回写字节数 *size; 码点 <0 或 >0x10FFFF
//       返回 -1, 成功返回 0。各首字节减 0x40/0x20/0x10 即标准前缀
//       0xC0/0xE0/0xF0 (char 截断等价)。
int utf8_encode(uint32_t codepoint, uint8_t *buffer, uint32_t *size)
{
	uint8_t low = (uint8_t)codepoint;

	if (-1 < (int)codepoint) {
		if ((int)codepoint < 0x80) {
			buffer[0] = low;
			*size = 1;
			return 0;
		}
		if ((int)codepoint < 0x800) {
			buffer[0] = (uint8_t)((codepoint & 0x7FF) >> 6) - 0x40;   /* 0xC0 | ... */
			buffer[1] = (uint8_t)(low & 0x3F) + 0x80;
			*size = 2;
			return 0;
		}
		if ((int)codepoint < 0x10000) {
			buffer[1] = (uint8_t)((codepoint & 0xFFF) >> 6) + 0x80;
			buffer[2] = (uint8_t)(low & 0x3F) + 0x80;
			buffer[0] = (uint8_t)((codepoint & 0xFFFF) >> 0xC) - 0x20;  /* 0xE0 | ... */
			*size = 3;
			return 0;
		}
		if ((int)codepoint < 0x110000) {
			buffer[3] = (uint8_t)(low & 0x3F) + 0x80;
			buffer[2] = (uint8_t)((codepoint & 0xFFF) >> 6) + 0x80;
			buffer[0] = (uint8_t)((codepoint & 0x1FFFFF) >> 0x12) - 0x10;  /* 0xF0 | ... */
			buffer[1] = (uint8_t)((codepoint & 0x3FFFF) >> 0xC) + 0x80;
			*size = 4;
			return 0;
		}
	}
	return -1;
}

// === utf8_check_first === 原 FUN_00059ac4 @ 0x00059ac4
// 来源: reversed/cgminer/12_freq_pll/FUN_00059ac4.c | 分类: 12_freq_pll
// 语义: 判断首字节并返回该 UTF-8 序列长度 — <0x80 返回 1; 连续字节 0x80..0xBF
//       与过long首字节 0xC0/0xC1 返回 0; 0xC2..0xDF 返回 2; 0xE0..0xEF 返回 3;
//       0xF0..0xF4 返回 4; 其余返回 0 (区间判定为 (byte)加法回绕比较)。
int utf8_check_first(uint8_t first)
{
	uint32_t b = (uint32_t)first;

	if ((b & 0x80) == 0)
		return 1;
	if ((b ^ 0x80) < 0x40 || (b + 0x40 & 0xFF) < 2)     /* 连续字节 / 0xC0-0xC1 */
		return 0;
	if ((b + 0x3E & 0xFF) < 0x1E)                       /* 0xC2..0xDF */
		return 2;
	if ((b + 0x20 & 0xFF) < 0x10)                       /* 0xE0..0xEF */
		return 3;
	if ((b + 0x10 & 0xFF) < 5)                          /* 0xF0..0xF4 */
		return 4;
	return 0;
}

// === utf8_check_full === 原 FUN_00059b40 @ 0x00059b40
// 来源: reversed/cgminer/12_freq_pll/FUN_00059b40.c | 分类: 12_freq_pll
// 语义: 校验 length 字节完整序列 (length 须为 2/3/4) — 依 length 掩出首字节
//       有效位, 逐续字节 (须 0x80..0xBF) 累计码点; 拒绝 >0x10FFFF、代理区
//       D800-DFFF 及 overlong 编码; 合法时 codepoint 非空则写出码点并返回 1。
//       overlong 判断按反编译体字面转写 (等价于 size=2 且 cp<0x80 / size=3 且
//       cp<0x800 / size=4 且 cp<0x10000)。
int utf8_check_full(const uint8_t *str, int length, uint32_t *codepoint)
{
	uint32_t value;
	uint32_t byte;
	int i;
	int reject;

	value = (uint32_t)str[0];
	if (length == 2)
		value &= 0x1F;
	else if (length == 3)
		value &= 0xF;
	else if (length == 4)
		value &= 7;
	else
		return 0;

	byte = (uint32_t)str[1];
	if ((byte ^ 0x80) < 0x40) {                  /* str[1] 为连续字节 */
		i = 1;
		do {
			i++;
			value = (byte & 0x3F) + value * 0x40;
			if (length <= i) {
				if (0x10FFFF < (int)value)
					return 0;
				if (value - 0xD800 < 0x800)      /* 代理区 */
					return 0;
				reject = (value == 0x7F);        /* 恒假 (cp>=0x80 时不可达) */
				if ((int)value < 0x80)
					reject = (length == 2);      /* overlong-2 */
				if (reject)
					return 0;
				reject = (value == 0x7FF);       /* 恒假 */
				if ((int)value < 0x800)
					reject = (length == 3);      /* overlong-3 */
				if (reject)
					return 0;
				if (length == 4 && (int)value < 0x10000)   /* overlong-4 */
					return 0;
				if (codepoint == 0)
					return 1;
				*codepoint = value;
				return 1;
			}
			byte = (uint32_t)str[i];
		} while ((byte ^ 0x80) < 0x40);
	}
	return 0;
}

// === utf8_decode_char === 原 FUN_00059c18 @ 0x00059c18
// 来源: reversed/cgminer/12_freq_pll/FUN_00059c18.c | 分类: 12_freq_pll
// 语义: 从 str 解出一个字符 — NUL 返回原指针; utf8_check_first 求长度, <1
//       返回 NULL; 长度 1 直接取该字节, 否则 utf8_check_full 解码 (失败 NULL);
//       前进 count 字节, codepoint 非空则写出码点。
const uint8_t *utf8_decode_char(const uint8_t *str, uint32_t *codepoint)
{
	int count;
	uint32_t value;               /* 解出的码点 (原体栈字) */
	const uint8_t *ret;

	value = (uint32_t)str[0];     /* 原体读取首字节 */
	if (value == 0)
		return str;
	count = utf8_check_first((uint8_t)value);
	if (count < 1)
		return 0;
	if (count == 1)
		value = (uint32_t)(uint8_t)str[0];
	else if (utf8_check_full(str, count, &value) == 0)
		return 0;
	ret = str + count;
	if (codepoint != 0)
		*codepoint = value;
	return ret;
}

// === utf8_check_string === 原 FUN_00059c8c @ 0x00059c8c
// 来源: reversed/cgminer/12_freq_pll/FUN_00059c8c.c | 分类: 12_freq_pll
// 语义: 校验整个串为合法 UTF-8 — len==-1 时按 strlen; 逐字符: 首字节非法或
//       多字节序列越界/不完整即返回 0; 全部合法返回 1。
int utf8_check_string(const char *str, size_t len)
{
	int count;
	int check;
	size_t i;

	if (len == (size_t)-1)
		len = strlen(str);
	if (0 < (int)len) {
		i = 0;
		do {
			count = utf8_check_first((uint8_t)str[i]);
			if (count == 0)
				return 0;
			if (1 < count) {
				if ((int)len < i + count)
					return 0;
				check = utf8_check_full((const uint8_t *)str + i, count, 0);
				i = i + count - 1;
				if (check == 0)
					return 0;
			}
			i++;
		} while (i < len);
	}
	return 1;
}

/* jansson 容器/值节点布局 (偏移由构造/删除/插入函数调用点证实) */

/* json_object_t: json 头 + hashtable (0x14) + serial/visited = 0x24 */
typedef struct json_object_t_f1 {
	json_t         json;         /* +0x00: {type, refcount} */
	hashtable_t_f1 hasht;        /* +0x08 */
	uint32_t       serial;       /* +0x1C: key 序号 (作 hashtable_set 的 hash 实参) */
	uint32_t       visited;      /* +0x20 */
} json_object_t_f1;

/* json_array_t: 0x18 */
typedef struct json_array_t_f1 {
	json_t    json;             /* +0x00 */
	uint32_t  size;             /* +0x08: 分配容量 (初值 8) */
	uint32_t  length;           /* +0x0C */
	json_t  **table;            /* +0x10 */
	uint32_t  last_visited;     /* +0x14 */
} json_array_t_f1;

/* json_string_t: 0x0C */
typedef struct json_string_t_f1 {
	json_t json;               /* +0x00 */
	char  *data;               /* +0x08 */
} json_string_t_f1;

/* json_integer_t / json_real_t: 0x10 (value 为 64 位) */
typedef struct json_intreal_t_f1 {
	json_t   json;             /* +0x00 */
	union {
		int64_t integer;       /* +0x08 (JSON_INTEGER) */
		double  real;          /* +0x08 (JSON_REAL) */
	} value;
} json_intreal_t_f1;

extern int __isnan(double value);          /* 原调用保留 (libgcc/libm) */
extern int __isinf(double value);          /* 原调用保留 */

/* ============================================================================
 * H. jansson 值/容器构造与操作 (FUN_00059d20..FUN_0005a95c)
 * ========================================================================== */

// === json_array_grow === 原 FUN_00059d20 @ 0x00059d20
// 来源: reversed/cgminer/12_freq_pll/FUN_00059d20.c | 分类: 12_freq_pll
// 语义: 数组扩容 — size < length+amount 时按 max(size*2, size+amount) 表项
//       经 jsonp_malloc 重配表; copy 非 0 时拷贝 length 个旧表项并释放旧表,
//       否则返回旧表指针 (由调用方处理); 无需扩容/失败语义见返回值。
void *json_array_grow(json_array_t_f1 *array, uint32_t amount, int copy)
{
	uint32_t new_size;
	uint32_t old_size = array->size;
	void *old_table;
	void *new_table;

	if (old_size < amount + array->length) {
		new_size = old_size * 2;
		if (new_size <= amount + old_size)
			new_size = amount + old_size;
		old_table = array->table;
		new_table = jsonp_malloc((size_t)new_size * 4);
		if (new_table == 0)
			return 0;
		array->size = new_size;
		array->table = new_table;
		if (copy == 0)
			return old_table;
		memcpy(new_table, old_table, array->length * 4);
		jsonp_free(old_table);
	}
	return array->table;
}

// === json_object_new === 原 FUN_00059da8 @ 0x00059da8
// 来源: reversed/cgminer/12_freq_pll/FUN_00059da8.c | 分类: 12_freq_pll
// 语义: 创建空对象 (0x24 字节) — type=JSON_OBJECT, refcount=1, hashtable_init
//       初始化桶表, serial/visited 清零; 失败释放并返回 NULL。
json_t *json_object_new(void)
{
	json_object_t_f1 *object;

	object = jsonp_malloc(0x24);
	if (object == 0)
		return 0;
	object->json.refcount = 1;
	object->json.type = JSON_OBJECT;
	if (hashtable_init(&object->hasht) == 0) {
		object->serial = 0;
		object->visited = 0;
		return &object->json;
	}
	jsonp_free(object);
	return 0;
}

// === json_array === 原 FUN_00059f80 @ 0x00059f80
// 来源: reversed/cgminer/12_freq_pll/FUN_00059f80.c | 分类: 12_freq_pll
// 语义: 创建空数组 (0x18 字节) — type=JSON_ARRAY, refcount=1, size=8,
//       length=0, 分配 8 表项 (0x20) 的 table, last_visited=0; 失败释放并返回 NULL。
json_t *json_array(void)
{
	json_array_t_f1 *array;

	array = jsonp_malloc(0x18);
	if (array == 0)
		return 0;
	array->json.type = JSON_ARRAY;
	array->json.refcount = 1;
	array->length = 0;
	array->size = 8;
	array->table = jsonp_malloc(0x20);
	if (array->table == 0) {
		jsonp_free(array);
		return 0;
	}
	array->last_visited = 0;
	return &array->json;
}

// === json_string_nocheck === 原 FUN_0005a0ec @ 0x0005a0ec
// 来源: reversed/cgminer/12_freq_pll/FUN_0005a0ec.c | 分类: 12_freq_pll
// 语义: 不校验地建字符串节点 — type=JSON_STRING, refcount=1, data=jsonp_strdup;
//       参数空/分配失败/复制失败均返回 NULL。
json_t *json_string_nocheck(const char *value)
{
	json_string_t_f1 *str;

	if (value == 0)
		return 0;
	str = jsonp_malloc(0x0C);
	if (str == 0)
		return 0;
	str->json.type = JSON_STRING;
	str->json.refcount = 1;
	str->data = jsonp_strdup(value);
	if (str->data == 0) {
		jsonp_free(str);
		return 0;
	}
	return &str->json;
}

// === json_string === 原 FUN_0005a148 @ 0x0005a148
// 来源: reversed/cgminer/12_freq_pll/FUN_0005a148.c | 分类: 12_freq_pll
// 语义: 建字符串节点 — 先 utf8_check_string 校验合法性, 合法才走
//       json_string_nocheck; 非法返回 NULL。
json_t *json_string(const char *value)
{
	if (value == 0 || utf8_check_string(value, (size_t)-1) == 0)
		return 0;
	return json_string_nocheck(value);
}

// === json_integer === 原 FUN_0005a21c @ 0x0005a21c
// 来源: reversed/cgminer/12_freq_pll/FUN_0005a21c.c | 分类: 12_freq_pll
// 语义: 建 64 位整数节点 (0x10 字节) — type=JSON_INTEGER, refcount=1, 值存 +8;
//       分配失败返回 NULL (原体 64 位实参拆 r0:r1)。
json_t *json_integer(int64_t value)
{
	json_intreal_t_f1 *node;

	node = jsonp_malloc(0x10);
	if (node == 0)
		return 0;
	node->value.integer = value;
	node->json.type = JSON_INTEGER;
	node->json.refcount = 1;
	return &node->json;
}

// === json_real === 原 FUN_0005a29c @ 0x0005a29c
// 来源: reversed/cgminer/12_freq_pll/FUN_0005a29c.c | 分类: 12_freq_pll
// 语义: 建浮点节点 (0x10 字节) — NaN/Inf 拒绝返回 NULL; type=JSON_REAL,
//       refcount=1, 值存 +8; 分配失败返回 NULL。
json_t *json_real(double value)
{
	json_intreal_t_f1 *node;

	if (__isnan(value) != 0 || __isinf(value) != 0)
		return 0;
	node = jsonp_malloc(0x10);
	if (node == 0)
		return 0;
	node->value.real = value;
	node->json.type = JSON_REAL;
	node->json.refcount = 1;
	return &node->json;
}

// === json_true === 原 FUN_0005a3c0 @ 0x0005a3c0
// 来源: reversed/cgminer/12_freq_pll/FUN_0005a3c0.c | 分类: 12_freq_pll
// 语义: 返回静态 true 单例 (DAT_00076ba8, .data 永久对象)。
void *json_true(void)
{
	return &DAT_00076ba8;
}

// === json_false === 原 FUN_0005a3cc @ 0x0005a3cc
// 来源: reversed/cgminer/12_freq_pll/FUN_0005a3cc.c | 分类: 12_freq_pll
// 语义: 返回静态 false 单例 (DAT_00076bb0)。
void *json_false(void)
{
	return &DAT_00076bb0;
}

// === json_null === 原 FUN_0005a3d8 @ 0x0005a3d8
// 来源: reversed/cgminer/12_freq_pll/FUN_0005a3d8.c | 分类: 12_freq_pll
// 语义: 返回静态 null 单例 (DAT_00076bb8)。
void *json_null(void)
{
	return &DAT_00076bb8;
}

// === json_delete === 原 FUN_0005a3e4 @ 0x0005a3e4
// 来源: reversed/cgminer/12_freq_pll/FUN_0005a3e4.c | 分类: 12_freq_pll
// 语义: 递归释放节点 — object: hashtable_free + free; array: 逐元素减引
//       (归零递归 json_delete) 后释放表和节点; string: 释放 data 与节点;
//       integer/real: 仅释放节点; true/false/null 静态对象不动。
void json_delete(json_t *value)
{
	json_object_t_f1 *object;
	json_array_t_f1 *array;
	json_string_t_f1 *str;
	json_intreal_t_f1 *intreal;
	uint32_t i;
	uint32_t length;
	json_t **table;
	json_t *element;
	int refcount;

	if (value == 0)
		return;

	switch (value->type) {
	case JSON_OBJECT:
		object = (json_object_t_f1 *)value;
		hashtable_free(&object->hasht);
		jsonp_free(object);
		break;

	case JSON_ARRAY:
		array = (json_array_t_f1 *)value;
		table = array->table;
		length = array->length;
		if (length != 0) {
			for (i = 0; i < length; i++) {
				element = table[i];
				if (element != 0 && element->refcount != -1) {
					refcount = element->refcount - 1;
					element->refcount = refcount;
					if (refcount == 0) {
						json_delete(element);
						table = array->table;     /* 原体递归删除后重读表/长度 */
						length = array->length;
					}
				}
			}
		}
		jsonp_free(table);
		jsonp_free(array);
		break;

	case JSON_STRING:
		str = (json_string_t_f1 *)value;
		jsonp_free(str->data);
		jsonp_free(str);
		break;

	case JSON_INTEGER:
	case JSON_REAL:
		intreal = (json_intreal_t_f1 *)value;
		jsonp_free(intreal);
		break;

	default:                                  /* true/false/null 静态对象 */
		break;
	}
}

// === json_array_insert === 原 FUN_0005a5e8 @ 0x0005a5e8
// 来源: reversed/cgminer/12_freq_pll/FUN_0005a5e8.c | 分类: 12_freq_pll
// 语义: 在 index 处插入 value — 前提: value/array 非 NULL、array 为数组且
//       index <= length、array != value、扩容成功; 表未重配时整体后移
//       (memmove), 重配时前后两段拷入新表并释放旧表; 失败时对 value 减引并
//       返回 -1, 成功返回 0。
int json_array_insert(json_t *array_p, uint32_t index, json_t *value_p)
{
	json_array_t_f1 *array = (json_array_t_f1 *)array_p;
	json_t *value = value_p;
	void *new_table;
	json_t **table;
	uint32_t length;
	int refcount;

	if (value != 0) {
		if (array_p != 0 && array_p->type == JSON_ARRAY && array_p != value &&
		    index <= array->length &&
		    (new_table = json_array_grow(array, 1, 0)) != 0) {
			table = array->table;
			if ((void *)table == new_table) {
				/* 未重配: [index..length) 后移一格 */
				memmove((char *)table + (index + 1) * 4, (char *)table + index * 4,
				        (array->length - index) * 4);
			} else {
				/* 重配为 copy=0: 新旧两段分别拷入 */
				memcpy(table, new_table, index * 4);
				memcpy((char *)table + (index + 1) * 4, (char *)new_table + index * 4,
				       (array->length - index) * 4);
				jsonp_free(new_table);
			}
			length = array->length;
			table[index] = value;
			array->length = length + 1;
			return 0;
		}
		if (value->refcount != -1) {
			refcount = value->refcount - 1;
			value->refcount = refcount;
			if (refcount == 0)
				json_delete(value);
		}
	}
	return -1;
}

// === json_array_append === 原 FUN_0005a6e8 @ 0x0005a6e8
// 来源: reversed/cgminer/12_freq_pll/FUN_0005a6e8.c | 分类: 12_freq_pll
// 语义: 数组尾插 value — 前提同上 (扩容须 copy=1); 成功 table[length]=value 且
//       length++; 失败减引 value 返回 -1。
int json_array_append(json_t *array_p, json_t *value_p)
{
	json_array_t_f1 *array = (json_array_t_f1 *)array_p;
	json_t *value = value_p;
	uint32_t length;
	int refcount;

	if (value != 0) {
		if (array_p != 0 && array_p->type == JSON_ARRAY && array_p != value &&
		    json_array_grow(array, 1, 1) != 0) {
			length = array->length;
			array->table[length] = value;
			array->length = length + 1;
			return 0;
		}
		if (value->refcount != -1) {
			refcount = value->refcount - 1;
			value->refcount = refcount;
			if (refcount == 0)
				json_delete(value);
		}
	}
	return -1;
}

// === json_object_set_nocheck === 原 FUN_0005a820 @ 0x0005a820
// 来源: reversed/cgminer/12_freq_pll/FUN_0005a820.c | 分类: 12_freq_pll
// 语义: 不校验 key 地写对象键值 — 前提: value/object/key 非 NULL、object 为
//       对象且 object != value; 以 serial++ 作 hash 调 hashtable_set; 失败或
//       前提不满足时对 value 减引并返回 -1, 成功返回 0。
int json_object_set_nocheck(json_t *object_p, const char *key, json_t *value_p)
{
	json_object_t_f1 *object = (json_object_t_f1 *)object_p;
	json_t *value = value_p;
	uint32_t serial;
	int ret;
	int refcount;

	if (value != 0) {
		if (object_p != 0 && key != 0 && object_p->type == JSON_OBJECT &&
		    object_p != value) {
			serial = object->serial;
			object->serial = serial + 1;
			ret = hashtable_set(&object->hasht, key, serial, value);
			if (ret == 0)
				return 0;
		}
		if (value->refcount != -1) {
			refcount = value->refcount - 1;
			value->refcount = refcount;
			if (refcount == 0)
				json_delete(value);
		}
	}
	return -1;
}

// === json_object_set === 原 FUN_0005a95c @ 0x0005a95c
// 来源: reversed/cgminer/12_freq_pll/FUN_0005a95c.c | 分类: 12_freq_pll
// 语义: 带校验地写对象键值 — key 合法 UTF-8 才走 set_nocheck; key 非法时对
//       value 减引并返回 -1。
int json_object_set(json_t *object, const char *key, json_t *value)
{
	int refcount;

	if (key != 0 && utf8_check_string(key, (size_t)-1) != 0)
		return json_object_set_nocheck(object, key, value);
	if (value != 0 && value->refcount != -1) {
		refcount = value->refcount - 1;
		value->refcount = refcount;
		if (refcount == 0)
			json_delete(value);
	}
	return -1;
}

/* ============================================================================
 * I. jansson 错误记录 (FUN_0005b014..FUN_0005b098)
 *    J. libgcc (FUN_0005c7ec)
 * ========================================================================== */

// === jsonp_error_init === 原 FUN_0005b014 @ 0x0005b014
// 来源: reversed/cgminer/12_freq_pll/FUN_0005b014.c | 分类: 12_freq_pll
// 语义: 初始化错误结构 — text[0]=0 (未占用), position=0, line=column=-1;
//       source 非空时按长度填入: <80 字节直接复制, 否则前 3 字节 "..." +
//       末尾 0x4C 字节 (共 80 字节); source 为空则 source[0]=0。
void jsonp_error_init(json_error_t *error, const char *source)
{
	size_t len;

	if (error == 0)
		return;
	error->text[0] = 0;
	error->position = 0;
	error->line = -1;
	error->column = -1;
	if (source != 0) {                 /* 原体此处另有 error/source 非空死检查 */
		len = strlen(source);
		if (len < 0x50) {
			memcpy(error->source, source, len + 1);
			return;
		}
		strncpy(error->source, "...", 4);
		strcpy(error->source + 3, source + (len - 0x4C));   /* 保留末 0x4C 字节 */
		return;
	}
	error->source[0] = 0;
}

// === jsonp_error_vset === 原 FUN_0005b058 @ 0x0005b058
// 来源: reversed/cgminer/12_freq_pll/FUN_0005b058.c | 分类: 12_freq_pll
// 语义: 记录错误 — 仅当 error->text[0]==0 (首次) 时生效: 写 line/column/
//       position, vsnprintf 格式化消息到 text (160 字节) 并保证结尾 NUL。
void jsonp_error_vset(json_error_t *error, int line, int column, size_t position,
                      const char *fmt, va_list ap)
{
	if (error == 0)
		return;
	if (error->text[0] != 0)               /* 已有错误, 保留首个 */
		return;
	error->line = line;
	error->column = column;
	error->position = position;
	vsnprintf(error->text, 0xA0, fmt, ap);
	error->text[159] = 0;                  /* 原体写 +0xFB 单字节 */
}

// === jsonp_error_set === 原 FUN_0005b098 @ 0x0005b098
// 来源: reversed/cgminer/12_freq_pll/FUN_0005b098.c | 分类: 12_freq_pll
// 语义: 变参包装 — 以 (fmt 后实参) 构造 va_list 转调 jsonp_error_vset。
void jsonp_error_set(json_error_t *error, int line, int column, size_t position,
                     const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	jsonp_error_vset(error, line, column, position, fmt, ap);
	va_end(ap);
}

// === __aeabi_idiv_checked === 原 FUN_0005c7ec @ 0x0005c7ec
// 来源: reversed/cgminer/12_freq_pll/FUN_0005c7ec.c | 分类: 12_freq_pll
// 语义: 带除 0 检查的有符号除法 — 除数为 0 时 raise(SIGFPE=8) 后以 raise 的
//       返回值返回 (原体尾随 r0 未清); 否则尾转 __aeabi_idiv (FUN_0005c718)。
int __aeabi_idiv_checked(int numerator, int denominator)
{
	if (denominator == 0) {
		return raise(8);               /* SIGFPE */
	}
	return __aeabi_idiv(numerator, denominator);
}

/*
 * ================= 覆盖率表 (78/78) =================
 * 原 FUN 地址    新函数名                            行数  主题分组
 * ------------------------------------------------------------------
 * FUN_00025e28  clone_work_with_id                    11  A 驱动/链控制/API/PIC
 * FUN_00032ff0  add_bool_buf                           9  A 驱动/链控制/API/PIC
 * FUN_00034ab8  api_config                            38  A 驱动/链控制/API/PIC
 * FUN_00041b8c  get_PIC16F1704_freq_new               64  A 驱动/链控制/API/PIC
 * FUN_00043060  every_chain_software_set_address      25  A 驱动/链控制/API/PIC
 * FUN_000432c0  every_chain_set_core_timeout          21  A 驱动/链控制/API/PIC
 * FUN_0004330c  every_chain_set_nonce_shift           21  A 驱动/链控制/API/PIC
 * FUN_00043594  every_chain_set_analog_mux_control    21  A 驱动/链控制/API/PIC
 * FUN_000435e0  set_frequency_chain                   39  A 驱动/链控制/API/PIC
 * FUN_00043734  set_freq_asic                         39  A 驱动/链控制/API/PIC
 * FUN_00043e4c  every_chain_set_frequency             21  A 驱动/链控制/API/PIC
 * FUN_00044104  every_chain_set_ticket_mask           20  A 驱动/链控制/API/PIC
 * FUN_000442b0  check_temperature_offset_value        65  A 驱动/链控制/API/PIC
 * FUN_000448bc  tty_init                              33  A 驱动/链控制/API/PIC
 * FUN_00045b30  fan_power_gpio_on                     10  A 驱动/链控制/API/PIC
 * FUN_0004614c  every_chain_enable_read_temperature_from_asic    16  A 驱动/链控制/API/PIC
 * FUN_000463c8  every_chain_init_temperature_sensor    27  A 驱动/链控制/API/PIC
 * FUN_00046880  create_bitmain_scan_freq_pthread      28  A 驱动/链控制/API/PIC
 * FUN_00046964  create_bitmain_turbo_mode_monitor_pthread    28  A 驱动/链控制/API/PIC
 * FUN_00046b94  save_freq_to_every_chain_pic          27  A 驱动/链控制/API/PIC
 * FUN_0004df30  equihash_leaf_hash                    15  B Equihash 辅助
 * FUN_0004df94  expandArray                           68  B Equihash 辅助
 * FUN_0004e134  check_index_tree_order                34  B Equihash 辅助
 * FUN_0004ea38  is_entry_less                         11  B Equihash 辅助
 * FUN_0004ea58  check_duplicate_indices               28  B Equihash 辅助
 * FUN_00054f8c  blake2b_update                        49  C Blake2b/SHA256预筛
 * FUN_000551d4  blake2b_final                         74  C Blake2b/SHA256预筛
 * FUN_00055388  blake2b                               26  C Blake2b/SHA256预筛
 * FUN_0005576c  sha256_update_prescreen               36  C Blake2b/SHA256预筛
 * FUN_000557d4  sha256_final_prescreen                60  C Blake2b/SHA256预筛
 * FUN_00055930  sha256_oneshot                        29  C Blake2b/SHA256预筛
 * FUN_000578c0  insert_to_bucket                      29  D jansson hashtable
 * FUN_00057994  hashtable_clear                       29  D jansson hashtable
 * FUN_000579f4  hashtable_init                        32  D jansson hashtable
 * FUN_00057a74  hashtable_free                        10  D jansson hashtable
 * FUN_00057d0c  hashtable_get                         61  D jansson hashtable
 * FUN_00058034  error_set                             52  E jansson lex/解析/装载
 * FUN_00058154  lex_init                              27  E jansson lex/解析/装载
 * FUN_000581a0  lex_cleanup                           12  E jansson lex/解析/装载
 * FUN_000581cc  decode_unicode_escape                 37  E jansson lex/解析/装载
 * FUN_00058274  stream_unget                          34  E jansson lex/解析/装载
 * FUN_0005832c  stream_get                            77  E jansson lex/解析/装载
 * FUN_000584b0  lex_unget_unsave                      19  E jansson lex/解析/装载
 * FUN_000584fc  lex_get_save                          15  E jansson lex/解析/装载
 * FUN_00059154  parse_json                            33  E jansson lex/解析/装载
 * FUN_00059360  json_loadf                            32  E jansson lex/解析/装载
 * FUN_000594bc  json_load_callback                    33  E jansson lex/解析/装载
 * FUN_0005956c  jsonp_malloc                          14  F jansson 内存/strbuffer
 * FUN_00059594  jsonp_free                            12  F jansson 内存/strbuffer
 * FUN_00059680  strbuffer_clear                       10  F jansson 内存/strbuffer
 * FUN_00059694  strbuffer_value                        8  F jansson 内存/strbuffer
 * FUN_000596b0  strbuffer_append_bytes                39  F jansson 内存/strbuffer
 * FUN_00059774  strbuffer_append_byte                 12  F jansson 内存/strbuffer
 * FUN_000597c0  strbuffer_pop                         20  F jansson 内存/strbuffer
 * FUN_000599dc  utf8_encode                           39  G jansson UTF-8
 * FUN_00059ac4  utf8_check_first                      28  G jansson UTF-8
 * FUN_00059b40  utf8_check_full                       65  G jansson UTF-8
 * FUN_00059c18  utf8_decode_char                      35  G jansson UTF-8
 * FUN_00059c8c  utf8_check_string                     35  G jansson UTF-8
 * FUN_00059d20  json_array_grow                       32  H jansson 值/容器
 * FUN_00059da8  json_object_new                       24  H jansson 值/容器
 * FUN_00059f80  json_array                            26  H jansson 值/容器
 * FUN_0005a0ec  json_string_nocheck                   22  H jansson 值/容器
 * FUN_0005a148  json_string                           15  H jansson 值/容器
 * FUN_0005a21c  json_integer                          17  H jansson 值/容器
 * FUN_0005a29c  json_real                             22  H jansson 值/容器
 * FUN_0005a3c0  json_true                              8  H jansson 值/容器
 * FUN_0005a3cc  json_false                             8  H jansson 值/容器
 * FUN_0005a3d8  json_null                              8  H jansson 值/容器
 * FUN_0005a3e4  json_delete                           52  H jansson 值/容器
 * FUN_0005a5e8  json_array_insert                     40  H jansson 值/容器
 * FUN_0005a6e8  json_array_append                     26  H jansson 值/容器
 * FUN_0005a820  json_object_set_nocheck               23  H jansson 值/容器
 * FUN_0005a95c  json_object_set                       22  H jansson 值/容器
 * FUN_0005b014  jsonp_error_init                      30  I jansson 错误记录
 * FUN_0005b058  jsonp_error_vset                      20  I jansson 错误记录
 * FUN_0005b098  jsonp_error_set                        9  I jansson 错误记录
 * FUN_0005c7ec  __aeabi_idiv_checked                  13  J libgcc
 *
 * 主题统计: A 驱动/链控制/API/PIC = 20; B Equihash = 5; C Blake2b/SHA256预筛 = 6;
 *           D jansson hashtable = 5; E lex/解析/装载 = 11; F 内存/strbuffer = 7;
 *           G UTF-8 = 5; H 值/容器 = 15; I 错误记录 = 3; J libgcc = 1。
 *
 * 特别注明: FUN_00059654 (strbuffer_close) 名义上属 21_pool_net 分类,
 *           定义归属 pool_b3/pool_p3.c, 本文件仅 extern 引用 (lex_cleanup 依赖)。
 *           i2c_write_core/i2c_read_core (0003c66c/0003c708) 不在本批清单 (pic.c)。
 * 外部依赖: 见文件头 extern 区 (make_clone/submit_work_async, api_add 系列/io_add/print_data,
 *           uart/makeup_cmd_frame, chain.c 各 per-chain setter, pic.c I2C, thr_info_create,
 *           applog/cgsleep_ms; freq_p3.c 的 lex_scan/parse_value/blake2b_init/init_key/sha256_process/
 *           hashtable_set/__aeabi_idiv; pool_p3.c 的 strbuffer_close; jansson 分配钩子
 *           DAT_00076ba0/ba4; 静态单例 DAT_00076ba8/bb0/bb8)。
 */
