/**
 * nostr.c — cgminer 无字符串层函数转写 (72 个)
 *
 * 来源: reversed/cgminer/70_no_strings/
 * 说明: 这些函数本身不含可打印字符串, 命名依据 name_map/cgminer.tsv
 *       (D 片 body/callgraph 语义分析结论) 与上游库对应关系
 *       (jansson / klist / sha256 / cgminer util / argp 等)。
 * 约定: 见 rewritten/CONVENTIONS.md
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include "common_types.h"

/* ---- 跨模块被调函数 (命名均出自 rewritten/name_map/cgminer.tsv) ---- */
extern void  rename_thread(const char *name);                        /* FUN_0002f0b4 */
extern void  set_lowprio(void);                                      /* FUN_000120f4 */
extern int   api(void *ctx);                                         /* FUN_00036764: API 主线程循环 (签名待定) */
extern void  kill_work(void);                                        /* FUN_000178f8 */
extern int  *add_pool(void);                                         /* FUN_00017a3c */
extern char *opt_set_charp(const char *arg, char **p);               /* FUN_0005b1d4 (cfg.c; 恒返回 NULL) */
extern pthread_mutex_t DAT_000790a0;                                 /* PIC I2C 互斥锁 (pic.c/freq_p4 同名) */
extern char *bin2hex(const void *data, size_t len);                  /* FUN_00029730 */
extern void  applog(uint32_t prio, char *buf, ...);   /* FUN_00038730 */
extern void  quit(int status);                                       /* FUN_00016ca8 */
extern void  message(void *io_data, int code, int param0, int param1, int isjson); /* FUN_0003305c */
extern void *bitmain_scanhash(void *arg);                            /* FUN_0003af90 */
extern unsigned int cgsleep_ms(unsigned int ms);                     /* FUN_0002a92c */
extern void  read_chain_asic_voltages(int chain);                    /* FUN_0003e5a4 */
extern void  send_data_to_all_asic(uint32_t reg, uint8_t *buf, int len); /* FUN_0003e6e0 */
extern void  clear_uart_rx_fifo(int chain);                          /* FUN_0003c0a0 */
extern uint32_t i2c_read_core(uint32_t cmd_val);                     /* FUN_0003c708 */
extern void  hashtable_clear(void *ht);                              /* FUN_00057994 */
extern void  json_delete(void *json);                                /* FUN_0005a3e4 */
extern int   strbuffer_append_bytes(void *buf, const void *value, size_t len); /* FUN_000596b0 */
extern char *opt_usage(const char *argv0, const char *extra);        /* FUN_0005c20c (hash.c) */
extern void  check_opt(void *entry);                                 /* FUN_0005b7c8 */
extern void  add_opt(void *entry);                                   /* FUN_0005b9a8 */

/* ============================================================
 * 主题分组:
 *   1. C 运行时弱符号桩 (__gmon_start__ / deregister_tm_clones)
 *   2. cgminer util (线程/任务队列/时间/内存/信号/调度时刻)
 *   3. cgminer util (线程取消/时间换算/strsep/信号量/审计 memcpy)
 *   4. API 统计行追加 (api_add_* 家族) 与 failover-only 命令
 *   5. klist 链表 / 矿机扫描包装
 *   6. 硬件链操作 (fan / dc-dc 电压 / pic flash / nonce fifo)
 *   7. sha256 (transform/update/final/one-shot)
 *   8. jansson 内部 (json_dump / hashtable / object / string / array)
 *   9. opt 选项 setter 家族 (argp/wrap)
 *  10. 编译器辅助 64 位除法 (__divmoddi4 / __divdi3)
 *  11. C 运行时启动/收尾 (__libc_csu_init / _fini / _start)
 * ============================================================ */

/* ------------------------------------------------------------------
 * 分组 1: C 运行时 / ELF 启动收尾
 * ------------------------------------------------------------------ */

// === __gmon_start__ === 原 FUN_0000f644 @ 0x0000f644
// 来源: reversed/cgminer/70_no_strings/FUN_0000f644.c | 分类: 70_no_strings
// 语义: gmon 弱符号跳板—调用真实 __gmon_start__ 外部弱符号后返回;
//       该弱符号未链接时 PLT 回落到下条指令, 等价于空函数。
void __gmon_start__(void)
{
	/* 原体: call __gmon_start__@plt (自名弱符号, 未绑定即空跑) → 语义为直接返回 */
	return;
}

// === deregister_tm_clones === 原 FUN_0000f668 @ 0x0000f668
// 来源: reversed/cgminer/70_no_strings/FUN_0000f668.c | 分类: 70_no_strings
// 语义: TLS 模型清理桩 (deregister_tm_clones)—无 clone 注册回调时为空。
void deregister_tm_clones(void)
{
	/* 原体仅 ret; 0xf680/0xf688/0xf68c 处的条件调用块被判定不可达 */
	return;
}

// === delayed_exit_thread === 原 FUN_00011c1c @ 0x00011c1c
// 来源: reversed/cgminer/70_no_strings/FUN_00011c1c.c | 分类: 70_no_strings
// 语义: 延迟自杀线程—脱离主线程控制, 5 秒后以状态码 1 终止整个进程。
void delayed_exit_thread(void)
{
	pthread_t th;

	th = pthread_self();
	pthread_detach(th);
	pthread_setcanceltype(1 /* PTHREAD_CANCEL_ASYNCHRONOUS */, NULL);
	sleep(5);
	exit(1);
}

/* ---- 共享全局: API 线程上下文 (本文件范围内使用) ---- */
extern void *DAT_00076ef8;  /* API 主线程 thr_info 指针全局, 语义待定 */

// === api_thread === 原 FUN_00012164 @ 0x00012164
// 来源: reversed/cgminer/70_no_strings/FUN_00012164.c | 分类: 70_no_strings
// 语义: API 监听线程入口—异步取消 + 改名 "API" + 低优先级, 进入 api() 主循环;
//       退出时把线程结构 +0xC 处的运行标志清零并返回 0。
int api_thread(void *userdata)
{
	pthread_t th;

	th = pthread_self();
	pthread_detach(th);
	pthread_setcanceltype(1 /* PTHREAD_CANCEL_ASYNCHRONOUS */, NULL);
	rename_thread("API");      /* 原 &DAT_00060570 = "API" 字符串常量 */
	set_lowprio();
	api(DAT_00076ef8);
	*(uint32_t *)((uint8_t *)userdata + 0xc) = 0;  /* 偏移 0xC: 线程在运行标志 */
	return 0;
}

/* ---- 共享全局: 调度起止时刻 (cgminer --sched-time / --sched-stop) ---- */
extern int DAT_0007b5f0;  /* sched_stop.enable: 该调度项启用标志 */
extern int DAT_0007b5f8;  /* sched_stop.min : 停止分钟 (sscanf 第 2 项) */
extern int DAT_0007b5fc;  /* sched_stop.hour: 停止小时 (sscanf 第 1 项) */
extern int DAT_0007b874;  /* sched_start.enable: 该调度项启用标志 */
extern int DAT_0007b87c;  /* sched_start.min : 启动分钟 */
extern int DAT_0007b880;  /* sched_start.hour: 启动小时 */

// === set_sched_stop === 原 FUN_00014e38 @ 0x00014e38
// 来源: reversed/cgminer/70_no_strings/FUN_00014e38.c | 分类: 70_no_strings
// 语义: 解析 "HH:MM" 写入 sched_stop 时刻, 合法则置启用标志; 返回错误串或 NULL。
//       (命名依 name_map: 0x14e38=stop, 0x14eb4=start)
char *set_sched_stop(char *arg)
{
	int n;

	n = sscanf(arg, "%d:%d", &DAT_0007b5fc, &DAT_0007b5f8);
	if (n != 2) {
		return "Invalid time set, should be HH:MM";
	}
	if (DAT_0007b5fc < 24 && DAT_0007b5f8 < 60 &&
	    DAT_0007b5fc >= 0 && DAT_0007b5f8 >= 0) {
		DAT_0007b5f0 = 1;  /* 启用该调度项 */
		return NULL;
	}
	return "Invalid time set.";
}

// === set_sched_start === 原 FUN_00014eb4 @ 0x00014eb4
// 来源: reversed/cgminer/70_no_strings/FUN_00014eb4.c | 分类: 70_no_strings
// 语义: 解析 "HH:MM" 写入 sched_start 时刻, 合法则置启用标志; 返回错误串或 NULL。
char *set_sched_start(char *arg)
{
	int n;

	n = sscanf(arg, "%d:%d", &DAT_0007b880, &DAT_0007b87c);
	if (n != 2) {
		return "Invalid time set, should be HH:MM";
	}
	if (DAT_0007b880 < 24 && DAT_0007b87c < 60 &&
	    DAT_0007b880 >= 0 && DAT_0007b87c >= 0) {
		DAT_0007b874 = 1;  /* 启用该调度项 */
		return NULL;
	}
	return "Invalid time set.";
}

/* ---- 本文件后段定义、此处先引用的选项 setter ---- */
char *opt_set_floatval(const char *arg, float *val);   /* 原 FUN_0005b1e0 @ 0x0005b1e0 */
char *opt_set_intval(const char *arg, int *val);       /* 原 FUN_0005b2e8 @ 0x0005b2e8 */

// === opt_set_floatval_range_100_250 === 原 FUN_000121b0 @ 0x000121b0
// 来源: reversed/cgminer/70_no_strings/FUN_000121b0.c | 分类: 70_no_strings
// 语义: 浮点选项 setter, 先复用 opt_set_floatval 解析, 再限定取值 [100, 250]
//       (bitmain-fan-pwm 等); 越界返回 "Value out of range"。
char *opt_set_floatval_range_100_250(const char *arg, float *val)
{
	char *err;

	err = opt_set_floatval(arg, val);
	if (err != NULL) {
		return err;
	}
	if (*val >= 100.0 && *val <= 250.0) {
		return NULL;
	}
	return "Value out of range";
}

// === opt_set_floatval_range_125_500 === 原 FUN_00012200 @ 0x00012200
// 来源: reversed/cgminer/70_no_strings/FUN_00012200.c | 分类: 70_no_strings
// 语义: 浮点选项 setter, 解析后限定取值 [125, 500] (bitmain-voltage 类选项)。
char *opt_set_floatval_range_125_500(const char *arg, float *val)
{
	char *err;

	err = opt_set_floatval(arg, val);
	if (err != NULL) {
		return err;
	}
	if (*val >= 125.0 && *val <= 500.0) {
		return NULL;
	}
	return "Value out of range";
}

/* ------------------------------------------------------------------
 * 分组 2: cgminer util (矿池参数 / 信号 / 驱动 / base58 / 工作队列)
 * ------------------------------------------------------------------ */

// === opt_set_intval_range === 原 FUN_0001545c @ 0x0001545c
// 来源: reversed/cgminer/70_no_strings/FUN_0001545c.c | 分类: 70_no_strings
// 语义: 整数选项 setter 带区间检查—先 opt_set_intval 解析, 再限定 [min, max]。
char *opt_set_intval_range(const char *arg, int *val, int min, int max)
{
	char *err;

	err = opt_set_intval(arg, val);
	if (err != NULL) {
		return err;
	}
	if (*val < min || max < *val) {
		return "Value out of range";
	}
	return NULL;
}

/* ---- 共享全局: 矿池计数器 (set_user/set_pass/set_userpass 三组互斥) ---- */
extern int DAT_00077008;  /* -u (set_user) 已设置的矿池数 */
extern int DAT_0007700c;  /* -p (set_pass)  已设置的矿池数 */
extern int DAT_00077010;  /* -O (set_userpass) 已设置的矿池数 */
extern int DAT_0007b198;  /* total_pools: 已配置矿池数 (axi.c 同名) */
extern void **DAT_0007b438;  /* pools: 矿池指针数组基址 (freq_p2.c 同名) */

// === set_work_nonce2_hex === 原 FUN_000158ac @ 0x000158ac
// 来源: reversed/cgminer/70_no_strings/FUN_000158ac.c | 分类: 70_no_strings
// 语义: 把 32 位 stratum nonce2 以大端序写入 pool+0x44, 并刷新 pool+0x704
//       处缓存的 4 字节十六进制串 (bin2hex, 先释放旧串)。
void set_work_nonce2_hex(void *pool, uint32_t nonce2)
{
	uint32_t be;

	be = nonce2 << 24 | (nonce2 >> 8 & 0xff) << 16 |
	     (nonce2 >> 16 & 0xff) << 8 | nonce2 >> 24;
	*(uint32_t *)((uint8_t *)pool + 0x44) = be;

	if (*(void **)((uint8_t *)pool + 0x704) == NULL) {
		return;
	}
	free(*(void **)((uint8_t *)pool + 0x704));
	*(void **)((uint8_t *)pool + 0x704) = bin2hex((uint8_t *)pool + 0x44, 4);
}

/* ---- 共享全局: 信号处理 struct sigaction 模板 ---- */
extern uint8_t DAT_0007b108[];  /* SIGTERM 用的 sigaction 结构, 语义待定 */
extern uint8_t DAT_0007b94c[];  /* SIGINT  用的 sigaction 结构, 语义待定 */

// === sighandler === 原 FUN_0001795c @ 0x0001795c
// 来源: reversed/cgminer/70_no_strings/FUN_0001795c.c | 分类: 70_no_strings
// 语义: 终止收尾—恢复 SIGTERM/SIGINT 默认处理, 然后走 kill_work() 全局停机。
void sighandler(void)
{
	sigaction(15 /* SIGTERM */, (struct sigaction *)DAT_0007b108, NULL);
	sigaction(2  /* SIGINT */,  (struct sigaction *)DAT_0007b94c, NULL);
	kill_work();
}

// === set_userpass === 原 FUN_00017c4c @ 0x00017c4c
// 来源: reversed/cgminer/70_no_strings/FUN_00017c4c.c | 分类: 70_no_strings
// 语义: "-O user:pass"—按 "user:pass" 拆分写入 pools[n] 的 user(+0xcc)/
//       pass(+0xd0) 字段 (userpass 整串另存 +0xc8); 无 pass 时给空串。
//       与 set_user/set_pass 互斥, 混用返回错误串。
char *set_userpass(const char *arg)
{
	char *tok_user, *tok_pass;
	char *dup;
	struct pool *pool;      /* 布局未知, 以字节偏移访问 */

	if (DAT_00077008 == 0 && DAT_0007700c == 0) {
		DAT_00077010 = DAT_00077010 + 1;
		if (DAT_0007b198 < DAT_00077010) {
			add_pool();
		}
		pool = *(struct pool **)((uint8_t *)DAT_0007b438 + (DAT_00077010 - 1) * 4);
		dup = strdup(arg);
		opt_set_charp(arg, (char **)((uint8_t *)pool + 200));  /* pool->rpc_userpass */
		tok_user = strtok(dup, ":");
		*(char **)((uint8_t *)pool + 0xcc) = tok_user;         /* pool->rpc_user */
		if (tok_user == NULL) {
			return "Failed to find : delimited user info";
		}
		tok_pass = strtok(NULL, ":");
		*(char **)((uint8_t *)pool + 0xd0) = tok_pass;         /* pool->rpc_pass */
		if (tok_pass == NULL) {
			/* 原样: pass 缺省时放一个空字符串 (calloc(1,1)) */
			*(char **)((uint8_t *)pool + 0xd0) = calloc(1, 1);
		}
		return NULL;
	}
	return "Use only user + pass or userpass, but not both";
}

// === set_pass === 原 FUN_00017f68 @ 0x00017f68
// 来源: reversed/cgminer/70_no_strings/FUN_00017f68.c | 分类: 70_no_strings
// 语义: "-p pass"—在 pools[n] 新池的 +0xd0 (rpc_pass) 处保存口令;
//       已用 userpass 时返回互斥错误串。(反编译残留的多余寄存器实参已略去)
char *set_pass(const char *arg)
{
	if (DAT_00077010 != 0) {
		return "Use only user + pass or userpass, but not both";
	}
	DAT_0007700c = DAT_0007700c + 1;
	if (DAT_0007b198 < DAT_0007700c) {
		add_pool();
	}
	opt_set_charp(arg, (char **)(*(uint8_t **)((uint8_t *)DAT_0007b438 +
	                    (DAT_0007700c - 1) * 4) + 0xd0));
	return NULL;
}

// === copy_drv === 原 FUN_00028a40 @ 0x00028a40
// 来源: reversed/cgminer/70_no_strings/FUN_00028a40.c | 分类: 70_no_strings
// 语义: 深拷贝 device_drv 结构 (0x80 字节) 并置 +0x64 处 copy 标记;
//       malloc 失败则打印源驱动名 (原/copy) 并 quit(1)。
void *copy_drv(void *drv)
{
	void *dest;
	char msg[4096];
	const char *src_kind;

	dest = malloc(0x80);
	if (dest == NULL) {
		src_kind = "original";
		if (*(char *)((uint8_t *)drv + 100) != '\0') {
			src_kind = "copy";
		}
		snprintf(msg, 0x1000, "Failed to allocate device_drv copy of %s (%s)",
		         *(char **)((uint8_t *)drv + 8), src_kind);
		applog(3, msg, 1);
		quit(1);
	}
	memcpy(dest, drv, 0x80);
	*(uint8_t *)((uint8_t *)dest + 100) = 1;  /* copy 标记 */
	return dest;
}

/* ---- 共享全局: base58 解码字符表 ---- */
extern const uint32_t DAT_000652a8[256];  /* base58 字符→数值表 (256 项), 语义待定 */

// === base58_decode === 原 FUN_00029c70 @ 0x00029c70
// 来源: reversed/cgminer/70_no_strings/FUN_00029c70.c | 分类: 70_no_strings
// 语义: base58 字符串 → 25 字节二进制: 224 位大数逐字符乘 58 加 digit
//       (查表 DAT_000652a8), 输出 out[0]=最高字顶字节, out[1..24]=W1..W6
//       大端序 (空串输出全 0)。
void base58_decode(uint8_t *out, const char *b58)
{
	uint32_t big[7];   /* 大数: big[0]=最高字 ... big[6]=最低字 */
	uint32_t carry;
	uint64_t tmp;
	uint32_t w;
	size_t len, i;
	int k;

	big[0] = 0; big[1] = 0; big[2] = 0; big[3] = 0;
	big[4] = 0; big[5] = 0; big[6] = 0;
	len = strlen(b58);
	if (len >= 1) {
		for (i = 0; i < len; i++) {
			carry = DAT_000652a8[(uint8_t)b58[i]];  /* k=6 步的加数, 之后作进位 */
			for (k = 6; k >= 0; k--) {
				tmp = (uint64_t)big[k] * 58 + (uint64_t)carry;
				big[k] = (uint32_t)tmp;
				carry = (uint32_t)(tmp >> 32) & 0x3f;
			}
		}
	}

	out[0] = (uint8_t)big[0];  /* 顶字节 (数值 ≤25 字节时其余 3 字节为 0) */
	for (k = 1; k <= 6; k++) {
		w = big[k];
		out[4 * k - 3] = (uint8_t)(w >> 24);
		out[4 * k - 2] = (uint8_t)(w >> 16);
		out[4 * k - 1] = (uint8_t)(w >> 8);
		out[4 * k]     = (uint8_t)w;
	}
}

// === tq_free === 原 FUN_0002a1a8 @ 0x0002a1a8
// 来源: reversed/cgminer/70_no_strings/FUN_0002a1a8.c | 分类: 70_no_strings
// 语义: 释放 thread_q: 逐节点从双向环摘链并 free(节点基址=list_head 前一个字),
//       销毁 cond(+0x28)/mutex(+0x0C), 整块清零 0x58 后 free。
void tq_free(void *tq)
{
	uint32_t *node;    /* 当前节点 (指向节点内 list_head) */
	uint32_t *succ;    /* 后继节点 */
	uint32_t *prev;

	if (tq == NULL) {
		return;
	}
	node = *(uint32_t **)tq;       /* tq->q.next: 首节点 */
	succ = *(uint32_t **)node;                  /* node->next */
	while ((uint32_t *)tq != node) {
		prev = (uint32_t *)node[1];/* node->prev */
		succ[1] = (uint32_t)prev;  /* 后继.prev = 前驱 */
		*prev = (uint32_t)succ;    /* 前驱.next = 后继 — 摘除 node */
		node[0] = 0;
		node[1] = 0;
		free((uint8_t *)node - 4); /* tq_ent 基址 (data 字段在 list_head 前一字) */
		node = succ;
		succ = *(uint32_t **)node;
	}
	pthread_cond_destroy((pthread_cond_t *)((uint8_t *)tq + 0x28));
	pthread_mutex_destroy((pthread_mutex_t *)((uint8_t *)tq + 0x0c));
	memset(tq, 0, 0x58);
	free(tq);
}

/* ------------------------------------------------------------------
 * 分组 3: cgminer util (线程取消 / 时间换算 / 字符串 / 信号量 / 内存)
 * ------------------------------------------------------------------ */

// === thr_info_cancel === 原 FUN_0002a414 @ 0x0002a414
// 来源: reversed/cgminer/70_no_strings/FUN_0002a414.c | 分类: 70_no_strings
// 语义: 取消线程—pthread_cancel 线程句柄 (thr+0xC) 并清零, 销毁 thr+0x10 处信号量。
void thr_info_cancel(void *thr)
{
	if (thr == NULL) {
		return;
	}
	if (*(pthread_t *)((uint8_t *)thr + 0xc) != 0) {
		pthread_cancel(*(pthread_t *)((uint8_t *)thr + 0xc));
		*(uint32_t *)((uint8_t *)thr + 0xc) = 0;
	}
	sem_destroy((sem_t *)((uint8_t *)thr + 0x10));
}

// === us_to_timeval === 原 FUN_0002a588 @ 0x0002a588
// 来源: reversed/cgminer/70_no_strings/FUN_0002a588.c | 分类: 70_no_strings
// 语义: 微秒 → struct timeval: 整除 1e6 得秒, 余数即微秒。
void us_to_timeval(void *dest, uint64_t us)
{
	lldiv_t d;

	d = lldiv(us, 1000000);
	*(uint32_t *)dest = (uint32_t)d.quot;       /* tv_sec  */
	*((uint32_t *)dest + 1) = (uint32_t)d.rem;  /* tv_usec */
}

// === ms_to_timeval === 原 FUN_0002a648 @ 0x0002a648
// 来源: reversed/cgminer/70_no_strings/FUN_0002a648.c | 分类: 70_no_strings
// 语义: 毫秒 → struct timeval: 整除 1000 得秒, 余数 ×1000 化为微秒。
void ms_to_timeval(void *dest, uint64_t ms)
{
	lldiv_t d;

	d = lldiv(ms, 1000);
	*(uint32_t *)dest = (uint32_t)d.quot;             /* tv_sec  */
	*((uint32_t *)dest + 1) = (uint32_t)d.rem * 1000; /* tv_usec */
}

// === strsep === 原 FUN_0002a800 @ 0x0002a800
// 来源: reversed/cgminer/70_no_strings/FUN_0002a800.c | 分类: 70_no_strings
// 语义: BSD strsep—在 *stringp 中截断首个定界符并推进指针; 无定界符时
//       置 *stringp = NULL 并返回整串。
char *strsep(char **stringp, const char *delim)
{
	char *start;
	char *delim_pos;

	start = *stringp;
	if (start == NULL || (delim_pos = strpbrk(start, delim)) == NULL) {
		*stringp = NULL;
		return start;
	}
	*delim_pos = '\0';
	*stringp = delim_pos + 1;
	return start;
}

// === cgsem_reset === 原 FUN_0002f400 @ 0x0002f400
// 来源: reversed/cgminer/70_no_strings/FUN_0002f400.c | 分类: 70_no_strings
// 语义: 清空信号量遗留计数—循环 sem_trywait 取走全部遗留 post;
//       EINTR 打断时重试, 其余错误直接返回。
void cgsem_reset(sem_t *sem)
{
	int ret;

	for (;;) {
		ret = sem_trywait(sem);
		if (ret < 0) {
			if (errno == 4 /* EINTR */) {
				continue;  /* 被信号打断, 重试 */
			}
			return;  /* EAGAIN 等: 已清空 */
		}
		/* ret == 0: 取走一个遗留 post, 继续清空 */
	}
}

/* ---- 共享全局: applog 门控 (axi.c / log.c 同名) ---- */
extern char DAT_0007ad48;  /* applog 门控 A (opt_debug/opt_log_output 之一) */
extern char DAT_00078e08;  /* applog 门控 B = opt_verbose */
extern int  DAT_000766c4;  /* applog 日志级别 (opt_log_level) */

// === _cg_memcpy === 原 FUN_0002f4f0 @ 0x0002f4f0
// 来源: reversed/cgminer/70_no_strings/FUN_0002f4f0.c | 分类: 70_no_strings
// 语义: 带审计的 memcpy: n>=1 时正常拷贝; n==0 时按日志门控
//       打印 "ERR: Asked to memcpy ..." 报警 (不拷贝)。
void _cg_memcpy(void *dest, const void *src, size_t n,
                const char *file, const char *func, int line)
{
	char msg[4096];

	if ((int)(n - 1) >= 0) {  /* n >= 1 */
		memcpy(dest, src, n);
		return;
	}
	if ((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0') || (2 < DAT_000766c4)) {
		snprintf(msg, 0x1000, "ERR: Asked to memcpy %u bytes from %s %s():%d",
		         n, file, func, line);
		applog(3, msg, 0);
	}
	return;
}

/* ------------------------------------------------------------------
 * 分组 4: API 统计行追加 (api_add_* 家族)
 * ------------------------------------------------------------------ */

char *api_add_data_full(char *root, const char *name, int type,
                        void *data, bool iscopied);  /* 原 FUN_00031cdc @ 0x00031cdc */

// === api_add_hex32 === 原 FUN_00032f3c @ 0x00032f3c
// 来源: reversed/cgminer/70_no_strings/FUN_00032f3c.c | 分类: 70_no_strings
// 语义: 向 API 状态环链表追加 32 位十六进制项 (type=9, API-B 会话按
//       api_data 类型表以 8 字符十六进制格式化)。
void *api_add_hex32(void *root, const char *name, uint32_t *data, bool iscopied)
{
	return api_add_data_full(root, name, 9, data, iscopied);
}

// === api_add_double === 原 FUN_00032fa8 @ 0x00032fa8
// 来源: reversed/cgminer/70_no_strings/FUN_00032fa8.c | 分类: 70_no_strings
// 语义: 向 API 状态环链表追加双精度项 (type=0xC, 8 字节)。
void *api_add_double(void *root, const char *name, double *data, bool iscopied)
{
	return api_add_data_full(root, name, 0xc, data, iscopied);
}

/* ---- 共享全局: API 配置 ---- */
extern uint8_t DAT_0007b625;  /* opt_failover_only (freq_p4.c 同名) */

// === failoveronly === 原 FUN_00033b88 @ 0x00033b88
// 来源: reversed/cgminer/70_no_strings/FUN_00033b88.c | 分类: 70_no_strings
// 语义: API "failover-only" 配置命令—无参数回 STATUS 码 0x4B; 参数首字母
//       小写化 (写回原缓冲), 'f'/'t' 写 opt_failover_only ('t'=开) 回 0x4D;
//       其他回 0x4C。第 4 参 (isjson) 原码经尾调用栈槽转发给 message
//       第 5 参 (反汇编 0x33bfc/0x33bd8 证实)。
void failoveronly(void *io, void *unused2, uint8_t *arg, int isjson)
{
	uint8_t c;

	if (arg == NULL || *arg == '\0') {
		message(io, 0x4b, 0, 0, isjson);
		return;
	}
	c = tolower(*arg);
	*arg = c;
	if (c == 'f' || c == 't') {
		DAT_0007b625 = (c == 't');  /* 't' → 启用 failover only */
		message(io, 0x4d, DAT_0007b625, 0, isjson);
		return;
	}
	message(io, 0x4c, 0, 0, isjson);
}

// === api_add_data_type19 === 原 FUN_00035544 @ 0x00035544
// 来源: reversed/cgminer/70_no_strings/FUN_00035544.c | 分类: 70_no_strings
// 语义: api_add_data_full 包装 (type=0x13=19); 上游枚举语义待考 [低置信]。
void *api_add_data_type19(void *root, const char *name, void *data, bool iscopied)
{
	return api_add_data_full(root, name, 0x13, data, iscopied);
}

// === api_add_freq === 原 FUN_0003558c @ 0x0003558c
// 来源: reversed/cgminer/70_no_strings/FUN_0003558c.c | 分类: 70_no_strings
// 语义: 向 API 状态环链表追加频率项 (type=0x15)。
void *api_add_freq(void *root, const char *name, void *data, bool iscopied)
{
	return api_add_data_full(root, name, 0x15, data, iscopied);
}

// === api_add_volts === 原 FUN_000355b0 @ 0x000355b0
// 来源: reversed/cgminer/70_no_strings/FUN_000355b0.c | 分类: 70_no_strings
// 语义: 向 API 状态环链表追加电压项 (type=0x16)。
void *api_add_volts(void *root, const char *name, void *data, bool iscopied)
{
	return api_add_data_full(root, name, 0x16, data, iscopied);
}

// === api_add_hs === 原 FUN_000355d4 @ 0x000355d4
// 来源: reversed/cgminer/70_no_strings/FUN_000355d4.c | 分类: 70_no_strings
// 语义: 向 API 状态环链表追加算力项 (type=0x17)。
void *api_add_hs(void *root, const char *name, void *data, bool iscopied)
{
	return api_add_data_full(root, name, 0x17, data, iscopied);
}

// === api_add_avg === 原 FUN_000363d8 @ 0x000363d8
// 来源: reversed/cgminer/70_no_strings/FUN_000363d8.c | 分类: 70_no_strings
// 语义: 向 API 状态环链表追加平均值项 (type=0x1A)。
void *api_add_avg(void *root, const char *name, void *data, bool iscopied)
{
	return api_add_data_full(root, name, 0x1a, data, iscopied);
}

/* ------------------------------------------------------------------
 * 分组 5: klist 链表 / 矿机扫描包装
 * ------------------------------------------------------------------ */

void *k_get(void *k_list);  /* 原 FUN_000391fc @ 0x000391fc — 从 free 链取空闲项 */

// === _k_unlink_head_zero === 原 FUN_0003927c @ 0x0003927c
// 来源: reversed/cgminer/70_no_strings/FUN_0003927c.c | 分类: 70_no_strings
// 语义: klist 取空闲项并清零数据块—k_get 后把 item->data (+0xC) 按链上
//       记录的分配大小 (k_list+0x14) memset 清零; 无空闲项时返回 NULL。
void *_k_unlink_head_zero(void *k_list)
{
	void *item;

	item = k_get(k_list);
	if (item != NULL) {
		memset(*(void **)((uint8_t *)item + 0xc), 0,
		       *(size_t *)((uint8_t *)k_list + 0x14));
	}
	return item;
}

/* ---- 共享全局: scanwork 结果 ---- */
extern uint64_t DAT_00078e18;  /* bitmain_scanhash 单轮结果累加 (哈希数) */

// === bitmain_zcash_scanwork === 原 FUN_0003a4a4 @ 0x0003a4a4
// 来源: reversed/cgminer/70_no_strings/FUN_0003a4a4.c | 分类: 70_no_strings
// 语义: 同步执行一轮扫描—清零结果全局, 起 bitmain_scanhash 线程并 join,
//       返回 DAT_00078e18 (本轮哈希数)。
uint64_t bitmain_zcash_scanwork(void *arg)
{
	pthread_t th;

	DAT_00078e18 = 0;
	pthread_create(&th, NULL, bitmain_scanhash, arg);
	pthread_join(th, NULL);
	return DAT_00078e18;
}

/* ------------------------------------------------------------------
 * 分组 6: 硬件链操作 (风扇监测 / dc-dc 电压 / PIC flash / nonce fifo)
 * ------------------------------------------------------------------ */

/* ---- 共享全局: FPGA AXI 窗口 / 链状态 (axi.c, chain.c 同名) ---- */
extern uint8_t *DAT_00078e10;  /* FPGA AXI 寄存器窗口 mmap 基址 (init_fpga) */
extern char DAT_0009d4bb[];    /* +1 (0x9d4bc) 起为链存在标志数组 (==1 在位) */
extern char DAT_000790d0[];    /* send/read_data_to/from_all_asic 互斥锁 */
extern int  DAT_0007909c;      /* 当前链号 (拼入 AXI/PIC 命令 bit16..) */

/* ---- 共享全局: 风扇监测统计 (本文件专属) ---- */
extern char     DAT_0009d67f[];               /* 每风扇有效标志 (0x9d67f 起, uint8 ×8) */
extern uint8_t  DAT_0009d69d[];               /* 每风扇转速数组 (uint32 ×8, 非对齐基址) */
extern uint32_t DAT_0009d4c8;                 /* 在位风扇位图 (bit idx) */
extern int      DAT_0009d6cc;                 /* 在位风扇计数 */
extern uint8_t  DAT_0009d6ce[];               /* 本轮最大转速 (0x9d6ce, 非对齐 4 字节视图) */
extern uint8_t  DAT_0009d6d2[];               /* 本轮最小转速 (0x9d6d2, 非对齐 4 字节视图) */

// === check_fan === 原 FUN_0003aa9c @ 0x0003aa9c
// 来源: reversed/cgminer/70_no_strings/FUN_0003aa9c.c | 分类: 70_no_strings
// 语义: 风扇监测常驻循环—每轮 (2×6 次, 每次间隔 50ms) 读 AXI 字偏移 4
//       (字节 0x10 /* 语义待定 */) 转速寄存器: 低 8 位×120 = RPM,
//       bit8..10 = 风扇号; 维护转速数组/在位标志/位图/计数与轮内最大最小值;
//       读到 0xffffffff 重试。每轮结束 sleep(1)。
void check_fan(void)
{
	uint32_t fan_reg;
	uint32_t fan_idx;
	uint32_t rpm;
	int round;
	int try_n;

	for (;;) {
		for (round = 2; round != 0; round--) {
			*(uint32_t *)(void *)DAT_0009d6ce = 0;  /* 轮内 max 归零 */
			*(uint32_t *)(void *)DAT_0009d6d2 = 0;  /* 轮内 min 归零 */
			for (try_n = 6; try_n != 0; try_n--) {
				fan_reg = *(volatile uint32_t *)((uint8_t *)DAT_00078e10 + 0x10);
				if (fan_reg == 0xffffffff) {
					cgsleep_ms(50);  /* 读失败, 重试 */
					continue;
				}
				fan_idx = (fan_reg & 0x7ff) >> 8;
				rpm = (fan_reg & 0xff) * 0x78;
				*(uint32_t *)(DAT_0009d69d + fan_idx * 4) = rpm;
				if ((fan_reg & 0xff) == 0) {
					if (DAT_0009d67f[fan_idx] == '\x01') {
						DAT_0009d67f[fan_idx] = (char)fan_reg;  /* 低字节为 0 → 摘除 */
						/* 原 if (1 << fan_idx == 0) 恒假 (fan_idx ≤ 7), 恒走 else */
						DAT_0009d4c8 = 0;
						DAT_0009d6cc = DAT_0009d6cc - 1;
					}
				} else if (DAT_0009d67f[fan_idx] == '\0') {
					DAT_0009d67f[fan_idx] = 1;
					DAT_0009d6cc = DAT_0009d6cc + 1;
					DAT_0009d4c8 = DAT_0009d4c8 | 1 << fan_idx;
				}
				if (*(uint32_t *)(void *)DAT_0009d6ce < rpm) {
					*(uint32_t *)(void *)DAT_0009d6ce = rpm;
				}
				if ((rpm < *(uint32_t *)(void *)DAT_0009d6d2 && rpm != 0) ||
				    *(uint32_t *)(void *)DAT_0009d6d2 == 0) {
					*(uint32_t *)(void *)DAT_0009d6d2 = rpm;
				}
				cgsleep_ms(50);
			}
		}
		sleep(1);
	}
}

// === read_dc_dc_voltage_all === 原 FUN_0003e688 @ 0x0003e688
// 来源: reversed/cgminer/70_no_strings/FUN_0003e688.c | 分类: 70_no_strings
// 语义: 遍历 4 条链, 在位链 (标志==1) 加锁后逐链读 ASIC 电压
//       (置当前链号 → read_chain_asic_voltages)。
void read_dc_dc_voltage_all(void)
{
	int chain;

	for (chain = 0; chain != 4; chain++) {
		if (DAT_0009d4bb[chain + 1] == '\x01') {
			pthread_mutex_lock((pthread_mutex_t *)DAT_000790d0);
			DAT_0007909c = chain;
			read_chain_asic_voltages(chain);
			pthread_mutex_unlock((pthread_mutex_t *)DAT_000790d0);
		}
	}
}

// === set_dc_dc_voltage_all === 原 FUN_0003e9ac @ 0x0003e9ac
// 来源: reversed/cgminer/70_no_strings/FUN_0003e9ac.c | 分类: 70_no_strings
// 语义: 全链 dc-dc 电压设定 (mV): 先广播 1 字节 0x20 命令, sleep(1),
//       再把 mV 折算为编码 ((mV/1000 − 0.25)/0.005 + 1) 后经 0x13 寄存器
//       发 2 字节 [编码低字节, 0x40]。
void set_dc_dc_voltage_all(uint32_t voltage_mv)
{
	uint8_t buf[4];
	uint32_t code;
	double v;

	buf[0] = 0x20;
	send_data_to_all_asic(0, buf, 1);
	sleep(1);
	v = (double)(float)voltage_mv;              /* 原 VCVT u32→f32→f64 */
	code = (uint32_t)((v / 1000.0 - 0.25) / 0.005 + 1.0);  /* 原 VCVT f64→u32 */
	/* 原栈缓冲首 4 字节: [code 低字节, 0x40, 0x00, 0x00], 仅发前 2 字节 */
	buf[0] = (uint8_t)code;
	buf[1] = 0x40;
	buf[2] = 0x00;
	buf[3] = 0x00;
	send_data_to_all_asic(0x13, buf, 2);
}

// === write_pic16f1704_flash_all_chain === 原 FUN_00046c10 @ 0x00046c10
// 来源: reversed/cgminer/70_no_strings/FUN_00046c10.c | 分类: 70_no_strings
// 语义: 遍历 4 条链, 在位链加锁后把 9 字节全零记录写入 PIC16F1704 flash
//       (每链数据偏移 +9 字节, 写后延时 100ms); 无链操作时返回 1。
uint32_t write_pic16f1704_flash_all_chain(void)
{
	uint8_t flash_buf[40];   /* 4 × 9 字节链记录 (补齐 40 字节, 全 0) */
	int chain;
	uint32_t ret;

	memset(flash_buf, 0, sizeof(flash_buf));
	ret = 1;
	for (chain = 0; chain != 4; chain++) {
		if (DAT_0009d4bb[chain + 1] == '\x01') {
			pthread_mutex_lock((pthread_mutex_t *)DAT_000790d0);
			DAT_0007909c = chain;
			ret = write_data_to_PIC16F1704_flash(flash_buf + chain * 9, 0, 9);
			cgsleep_ms(100);
			pthread_mutex_unlock((pthread_mutex_t *)DAT_000790d0);
		}
	}
	return ret;
}

/* ---- 共享全局: nonce fifo 计数与串口锁 (uart.c 同名) ---- */
extern char DAT_00078e88[];  /* nonce fifo 互斥锁 */
extern int  DAT_0007c28c;    /* fifo 计数 A */
extern int  DAT_0007c290;    /* fifo 计数 B */
extern int  DAT_0007c294;    /* fifo 计数 C */

// === clear_nonce_fifo === 原 FUN_000497cc @ 0x000497cc
// 来源: reversed/cgminer/70_no_strings/FUN_000497cc.c | 分类: 70_no_strings
// 语义: 清 nonce fifo—锁下清零三个计数, 再对 4 条链逐链清串口接收 fifo。
void clear_nonce_fifo(void)
{
	pthread_mutex_lock((pthread_mutex_t *)DAT_00078e88);
	DAT_0007c28c = 0;
	DAT_0007c290 = 0;
	DAT_0007c294 = 0;
	clear_uart_rx_fifo(0);
	clear_uart_rx_fifo(1);
	clear_uart_rx_fifo(2);
	clear_uart_rx_fifo(3);
	pthread_mutex_unlock((pthread_mutex_t *)DAT_00078e88);
}

// === get_dc_dc_voltage === 原 FUN_00049ba0 @ 0x00049ba0
// 来源: reversed/cgminer/70_no_strings/FUN_00049ba0.c | 分类: 70_no_strings
// 语义: 读单链 dc-dc 电压—命令字 = chain<<16 | 0x1900000 (bit24..: 0x01 读类,
//       bit16.. = 0x90|chain), PIC/I2C 总线锁下连读两次 (仅第一次结果返回),
//       间隔 100ms。
uint32_t get_dc_dc_voltage(int chain)
{
	uint32_t cmd;
	uint32_t val;

	cmd = (uint32_t)chain << 16 | 0x1900000;
	pthread_mutex_lock((pthread_mutex_t *)DAT_000790a0);
	val = i2c_read_core(cmd);
	i2c_read_core(cmd);   /* 原码二次读取, 结果弃用 */
	cgsleep_ms(100);
	pthread_mutex_unlock((pthread_mutex_t *)DAT_000790a0);
	return val;
}

/* ------------------------------------------------------------------
 * 分组 7: sha256 (transform / update / final / one-shot)
 * ------------------------------------------------------------------
 * 上下文布局 (机器码锁定, 见 FUN_0004b4f0/0004dbec 访问偏移):
 *   +0x00 uint64 bit_count (按字节累计, 输出前 ×8)
 *   +0x08 uint32 h[8]      中间散列状态
 *   +0x28 uint8  buf[64]  数据缓冲
 * 原 FUN_0004b4f0 为 64 轮全展开直线代码, K 常量以内联立即数出现;
 * 此处恢复为标准循环 + K 表 (FIPS 180-2), 语义等价。
 */

static const uint32_t sha256_k[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

#define SHA256_ROTR(x, n) (((x) >> (n)) | ((x) << (32 - (n))))

// === sha256_transform === 原 FUN_0004b4f0 @ 0x0004b4f0
// 来源: reversed/cgminer/70_no_strings/FUN_0004b4f0.c | 分类: 70_no_strings
// 语义: SHA-256 压缩函数—吸收一个 64 字节块 (大端入 W), 64 轮混合后
//       累加回 ctx+0x08 处 8 字状态; 不改缓冲。
void sha256_transform(void *ctx, const uint8_t *data)
{
	uint32_t *h = (uint32_t *)((uint8_t *)ctx + 8);
	uint32_t W[64];
	uint32_t a, b, c, d, e, f, g, hh, t1, t2;
	uint32_t s0, s1;
	int i;

	a = h[0]; b = h[1]; c = h[2]; d = h[3];
	e = h[4]; f = h[5]; g = h[6]; hh = h[7];

	for (i = 0; i < 16; i++) {
		W[i] = (uint32_t)data[i * 4] << 24 | (uint32_t)data[i * 4 + 1] << 16 |
		       (uint32_t)data[i * 4 + 2] << 8 | (uint32_t)data[i * 4 + 3];
	}
	for (i = 16; i < 64; i++) {
		s0 = SHA256_ROTR(W[i - 15], 7) ^ SHA256_ROTR(W[i - 15], 18) ^ (W[i - 15] >> 3);
		s1 = SHA256_ROTR(W[i - 2], 17) ^ SHA256_ROTR(W[i - 2], 19) ^ (W[i - 2] >> 10);
		W[i] = W[i - 16] + s0 + W[i - 7] + s1;
	}
	for (i = 0; i < 64; i++) {
		s1 = SHA256_ROTR(e, 6) ^ SHA256_ROTR(e, 11) ^ SHA256_ROTR(e, 25);
		t1 = hh + s1 + ((e & f) ^ (~e & g)) + sha256_k[i] + W[i];
		s0 = SHA256_ROTR(a, 2) ^ SHA256_ROTR(a, 13) ^ SHA256_ROTR(a, 22);
		t2 = s0 + ((a & b) ^ (a & c) ^ (b & c));
		hh = g; g = f; f = e; e = d + t1;
		d = c; c = b; b = a; a = t1 + t2;
	}
	h[0] += a; h[1] += b; h[2] += c; h[3] += d;
	h[4] += e; h[5] += f; h[6] += g; h[7] += hh;
}

// === sha256_update === 原 FUN_0004dbec @ 0x0004dbec
// 来源: reversed/cgminer/70_no_strings/FUN_0004dbec.c | 分类: 70_no_strings
// 语义: 喂数据—累计字节计数 (64 位, 低 32 位进位到高位); 先补满 ctx 残块
//       触发 transform, 再整块直推, 余数存入 buf (+0x28)。
void sha256_update(void *ctx, const void *data, uint32_t len)
{
	uint32_t cnt_lo, cnt_hi;
	uint32_t buf_idx;
	size_t fill;

	if ((int)len < 1) {
		return;
	}
	cnt_lo = *(uint32_t *)ctx;
	cnt_hi = *((uint32_t *)ctx + 1);
	*(uint32_t *)ctx = len + cnt_lo;            /* bit_count 低 32 位 */
	fill = 0x40 - (cnt_lo & 0x3f);
	if (len + cnt_lo < len) {                   /* 无符号回卷 → 高位进位 */
		*((uint32_t *)ctx + 1) = cnt_hi + 1;
	}
	buf_idx = cnt_lo & 0x3f;
	if (buf_idx != 0 && (int)fill <= (int)len) {
		len -= fill;
		memcpy((uint8_t *)ctx + buf_idx + 0x28, data, fill);
		data = (const uint8_t *)data + fill;
		sha256_transform(ctx, (uint8_t *)ctx + 0x28);
		buf_idx = 0;
	}
	while (0x3f < (int)len) {                   /* 整块直推 */
		sha256_transform(ctx, data);
		data = (const uint8_t *)data + 0x40;
		len -= 0x40;
	}
	if ((int)len < 1) {
		return;
	}
	memcpy((uint8_t *)ctx + buf_idx + 0x28, data, len);
}

/* ---- 共享全局: SHA-256 填充序列 ---- */
extern const uint8_t DAT_0006c3e4[];  /* 0x80 起始的全零填充串 (语义待定) */

// === sha256_final === 原 FUN_0004dccc @ 0x0004dccc
// 来源: reversed/cgminer/70_no_strings/FUN_0004dccc.c | 分类: 70_no_strings
// 语义: 收尾—补 0x80 与零至 56/120 字节边界, 追加 8 字节大端位长
//       (字节计数 ×8), 最后把 ctx+0x08 处 8 字状态以大端写入 32 字节摘要。
void sha256_final(void *ctx, uint8_t *digest)
{
	uint32_t cnt_lo, cnt_hi;
	uint32_t buf_idx;
	uint8_t len_be[8];
	uint64_t bits;
	int pad_len;
	int i;

	cnt_lo = *(uint32_t *)ctx;
	cnt_hi = *((uint32_t *)ctx + 1);
	buf_idx = cnt_lo & 0x3f;
	pad_len = (buf_idx < 0x38) ? (0x38 - buf_idx) : (0x78 - buf_idx);

	bits = ((uint64_t)cnt_hi << 32 | cnt_lo) * 8;  /* 字节计数 ×8 = 位长 */
	for (i = 0; i < 8; i++) {
		len_be[i] = (uint8_t)(bits >> (56 - 8 * i));
	}
	sha256_update(ctx, DAT_0006c3e4, pad_len);
	sha256_update(ctx, len_be, 8);

	/* 大端输出 h[0..7] → digest[0..31] (原为逐字节展开) */
	for (i = 0; i < 8; i++) {
		uint32_t w = *(uint32_t *)((uint8_t *)ctx + 8 + i * 4);
		digest[i * 4]     = (uint8_t)(w >> 24);
		digest[i * 4 + 1] = (uint8_t)(w >> 16);
		digest[i * 4 + 2] = (uint8_t)(w >> 8);
		digest[i * 4 + 3] = (uint8_t)w;
	}
}

// === sha256 === 原 FUN_0004de80 @ 0x0004de80
// 来源: reversed/cgminer/70_no_strings/FUN_0004de80.c | 分类: 70_no_strings
// 语义: 一次性 SHA-256—栈上构建上下文 (计数清零 + IV 初值), update 后
//       final 输出 32 字节摘要, 并按原帧大小 0xE8 清零栈上下文。
void sha256(const void *data, uint32_t len, uint8_t *digest)
{
	uint8_t ctx[0xe8];   /* 原 memset 范围: ctx 0x68 字节 + 栈垫 */

	memset(ctx, 0, 0x10);                       /* 位计数 8 字节 */
	*(uint32_t *)(ctx + 0x08) = 0x6a09e667;     /* h[0..7] = SHA-256 IV */
	*(uint32_t *)(ctx + 0x0c) = 0xbb67ae85;
	*(uint32_t *)(ctx + 0x10) = 0x3c6ef372;
	*(uint32_t *)(ctx + 0x14) = 0xa54ff53a;
	*(uint32_t *)(ctx + 0x18) = 0x510e527f;
	*(uint32_t *)(ctx + 0x1c) = 0x9b05688c;
	*(uint32_t *)(ctx + 0x20) = 0x1f83d9ab;
	*(uint32_t *)(ctx + 0x24) = 0x5be0cd19;
	sha256_update(ctx, data, len);
	sha256_final(ctx, digest);
	memset(ctx, 0, 0xe8);
}

/* ------------------------------------------------------------------
 * 分组 8: jansson 内部 (json_dump / hashtable / object / string)
 * ------------------------------------------------------------------ */

/* 原标签 0x0005709c: json_dump_callback 的文件输出回调 (jansson dump_to_file)
 * —签名待补: (const char *buffer, size_t size, void *data) → int */
extern int fun_0005709c(void);

// === json_dumpf === 原 FUN_000577f0 @ 0x000577f0
// 来源: reversed/cgminer/70_no_strings/FUN_000577f0.c | 分类: 70_no_strings
// 语义: 把 JSON 树经文件回调 (fun_0005709c) 写入 FILE* 流, flags 透传;
//       返回 json_dump_callback 的结果码。
int json_dumpf(void *json, FILE *output, uint32_t flags)
{
	return json_dump_callback(json, fun_0005709c, output, flags);
}

int json_dump_callback(void *json, int (*callback)(void), void *data, uint32_t flags);  /* 原 FUN_000577a4 @ 0x000577a4 */

// === json_dump_file === 原 FUN_00057804 @ 0x00057804
// 来源: reversed/cgminer/70_no_strings/FUN_00057804.c | 分类: 70_no_strings
// 语义: 把 JSON 树写入文件 ("w" 方式打开), 失败返回 -1, 成功返回 dump 结果。
int json_dump_file(void *json, char *path, uint32_t flags)
{
	FILE *fp;
	int ret;

	fp = fopen(path, "w");
	if (fp == NULL) {
		return -1;
	}
	ret = json_dumpf(json, fp, flags);
	fclose(fp);
	return ret;
}

/* ---- 共享全局: jansson 桶数质数表 ---- */
extern const int DAT_0006cb74[];  /* prime_list: hashtable 桶数质数序列 */

// === hashtable_reset === 原 FUN_00057e28 @ 0x00057e28
// 来源: reversed/cgminer/70_no_strings/FUN_00057e28.c | 分类: 70_no_strings
// 语义: 清空哈希表但保留桶数组—hashtable_clear 后按 prime_list[num_buckets]
//       把每个桶 (first/last) 与链表头全部指回 &ht->list, size 归零。
//       ht 布局: +0 size, +4 buckets, +8 num_buckets, +0xC/+0x10 list 链头。
void hashtable_reset(void *ht)
{
	uint8_t *base = (uint8_t *)ht;
	uint32_t *buckets;
	int n_buckets;
	int32_t size;
	int i;

	hashtable_clear(ht);
	n_buckets = *(int32_t *)(base + 8);
	size = DAT_0006cb74[n_buckets];
	if (size != 0) {
		buckets = *(uint32_t **)(base + 4);
		for (i = 0; i < size; i++) {
			buckets[i * 2 + 1] = (uint32_t)(base + 0xc);  /* bucket.last = &ht->list */
			buckets[i * 2]     = (uint32_t)(base + 0xc);  /* bucket.first = &ht->list */
		}
	}
	*(uint32_t **)(base + 0x10) = base + 0xc;  /* list.prev = &ht->list */
	*(uint32_t **)(base + 0x0c) = base + 0xc;  /* list.next = &ht->list */
	*(uint32_t *)base = 0;                     /* ht->size = 0 */
}

// === json_object_pair_set_value === 原 FUN_00057f4c @ 0x00057f4c
// 来源: reversed/cgminer/70_no_strings/FUN_00057f4c.c | 分类: 70_no_strings
// 语义: 写 pair 的 value 槽 (+0x08): 旧值引用计数 (+4, JSON 常量值为 -1)
//       减到 0 时 json_delete 释放, 再挂上新值。
void json_object_pair_set_value(void *pair, void *value)
{
	uint8_t *old;

	old = *(uint8_t **)((uint8_t *)pair + 8);
	if (old != NULL && *(int32_t *)(old + 4) != -1) {
		if (--*(int32_t *)(old + 4) == 0) {
			json_delete(old);   /* 原码 Ghidra 未显式传参 (r0 保持旧值指针) */
			*(void **)((uint8_t *)pair + 8) = value;
			return;
		}
	}
	*(void **)((uint8_t *)pair + 8) = value;
}

// === json_string_length === 原 FUN_00059798 @ 0x00059798
// 来源: reversed/cgminer/70_no_strings/FUN_00059798.c | 分类: 70_no_strings
// 语义: 取字符串长度后转调 strbuffer_append_bytes 把整串追加进缓冲
//       (jansson strbuffer_append 语义)。
void json_string_length(void *strbuffer, const char *str)
{
	size_t len;

	len = strlen(str);
	strbuffer_append_bytes(strbuffer, str, len);
}

// === json_object_clear === 原 FUN_00059e68 @ 0x00059e68
// 来源: reversed/cgminer/70_no_strings/FUN_00059e68.c | 分类: 70_no_strings
// 语义: 清空 JSON 对象—类型非 JSON_OBJECT(0) 返回 -1; 否则复位内部
//       hashtable (json+8) 并把 +0x1C 处序列化缓存指针清零, 返回 0。
int json_object_clear(int32_t *object)
{
	if (object == NULL || object[0] != 0) {
		return -1;
	}
	hashtable_reset((uint8_t *)object + 8);
	object[7] = 0;   /* +0x1C: 序列化缓存 */
	return 0;
}

/* ---- 本文件后段定义、此处先引用的 jansson 基础件 ---- */
int json_array_grow(void *array, size_t need, int linear, int extra);  /* 原 FUN_00059d20 @ 0x00059d20 */
char *jsonp_strdup(const char *s);    /* 原 FUN_000595b4 @ 0x000595b4 */
void jsonp_free(void *p);             /* 原 FUN_00059594 @ 0x00059594 */
int utf8_check_string(const char *s, int max);  /* 原 FUN_00059c8c @ 0x00059c8c */

// === json_array_extend === 原 FUN_0005a03c @ 0x0005a03c
// 来源: reversed/cgminer/70_no_strings/FUN_0005a03c.c | 分类: 70_no_strings
// 语义: 数组拼接—校验双方均为 JSON_ARRAY(1), 先 json_array_grow 扩容
//       (线性 1,1), 对 other 各元素引用计数 +1 (常量值 -1 除外), 再把
//       other 的 items 整体 memcpy 到数组尾部并累加 size。
int json_array_extend(int32_t *array, int32_t *other)
{
	int32_t *src;
	size_t bytes;
	int32_t n;
	int32_t i;
	int32_t item;

	if (array == NULL || array[0] != 1 || other == NULL ||
	    other[0] != 1 || json_array_grow(array, other[3], 1, 1) == 0) {
		return -1;
	}
	n = other[3];       /* other->size  */
	src = (int32_t *)other[4];  /* other->items */
	if (n != 0) {
		for (i = 0; i < n; i++) {
			item = src[i];
			if (item != 0 && *(int32_t *)(item + 4) != -1) {
				*(int32_t *)(item + 4) += 1;   /* 引用计数 +1 */
			}
		}
	}
	memcpy((void *)(array[4] + array[3] * 4), src, n * 4);
	array[3] = array[3] + other[3];
	return 0;
}

// === json_string_set_nocheck === 原 FUN_0005a194 @ 0x0005a194
// 来源: reversed/cgminer/70_no_strings/FUN_0005a194.c | 分类: 70_no_strings
// 语义: 直接换字符串内容—校验为 JSON_STRING(2) 后, jsonp_strdup 新值,
//       jsonp_free 旧值 (+0x08 槽), 挂新值; 任一校验失败返回 -1。
int json_string_set_nocheck(int32_t *str, const char *value)
{
	void *dup;

	if (str == NULL || str[0] != 2 || value == 0 ||
	    (dup = jsonp_strdup(value)) == NULL) {
		return -1;
	}
	jsonp_free((void *)str[2]);
	str[2] = (int32_t)dup;
	return 0;
}

// === json_string_set === 原 FUN_0005a1e0 @ 0x0005a1e0
// 来源: reversed/cgminer/70_no_strings/FUN_0005a1e0.c | 分类: 70_no_strings
// 语义: 带编码检查的字符串赋值—先 utf8_check_string(value, -1) 校验,
//       通过后转调 json_string_set_nocheck; 返回同上。
int json_string_set(void *str, const char *value)
{
	if (value == 0 || utf8_check_string(value, -1) == 0) {
		return -1;
	}
	return json_string_set_nocheck(str, value);
}

// === json_real_set === 原 FUN_0005a328 @ 0x0005a328
// 来源: reversed/cgminer/70_no_strings/FUN_0005a328.c | 分类: 70_no_strings
// 语义: 实数赋值—校验为 JSON_REAL(4) 且值非 NaN/Inf 后写入 +0x08 处
//       的 double 槽; 否则返回 -1。
int json_real_set(int32_t *real, double value)
{
	if (real == NULL || real[0] != 4 || __isnan(value) || __isinf(value)) {
		return -1;
	}
	*(double *)(real + 2) = value;
	return 0;
}

// === json_array_clear === 原 FUN_0005a4b8 @ 0x0005a4b8
// 来源: reversed/cgminer/70_no_strings/FUN_0005a4b8.c | 分类: 70_no_strings
// 语义: 清空数组元素—逐元素引用计数 -1, 减到 0 的 json_delete 释放
//       (常量值 -1 与 NULL 跳过), 最后 size 归零。
int json_array_clear(int32_t *array)
{
	int32_t item;
	uint32_t n;
	uint32_t i;
	int32_t refcnt;

	if (array == NULL || array[0] != 1) {
		return -1;
	}
	n = array[3];
	if (n != 0) {
		for (i = 0; i < n; i++) {
			item = *(int32_t *)(array[4] + i * 4);
			if (item != 0 && *(int32_t *)(item + 4) != -1) {
				refcnt = *(int32_t *)(item + 4) - 1;
				*(int32_t *)(item + 4) = refcnt;
				if (refcnt == 0) {
					json_delete(item);
					n = array[3];   /* 原码释放后重读 size 作循环上界 */
				}
			}
		}
	}
	array[3] = 0;
	return 0;
}

// === json_array_remove === 原 FUN_0005a534 @ 0x0005a534
// 来源: reversed/cgminer/70_no_strings/FUN_0005a534.c | 分类: 70_no_strings
// 语义: 按下标删元素—释放被删项 (引用计数), 其后元素 memmove 前移,
//       size 减 1; 下标越界返回 -1。
int json_array_remove(int32_t *array, uint32_t index)
{
	int32_t item;
	uint32_t n;
	int32_t refcnt;

	if (array == NULL || array[0] != 1 || array[3] <= index) {
		return -1;
	}
	item = *(int32_t *)(array[4] + index * 4);
	if (item != 0 && *(int32_t *)(item + 4) != -1) {
		refcnt = *(int32_t *)(item + 4) - 1;
		*(int32_t *)(item + 4) = refcnt;
		if (refcnt == 0) {
			json_delete(item);
			n = array[3];
		}
	}
	n = array[3] - 1;
	if (index < n) {
		memmove((void *)(array[4] + index * 4),
		        (void *)(array[4] + (index + 1) * 4),
		        (n - index) * 4);
		n = array[3] - 1;
	}
	array[3] = n;
	return 0;
}

// === json_array_set === 原 FUN_0005a76c @ 0x0005a76c
// 来源: reversed/cgminer/70_no_strings/FUN_0005a76c.c | 分类: 70_no_strings
// 语义: 按下标替换元素—校验通过 (数组型/非自嵌套/下标合法) 时释放旧项
//       后挂新值返回 0; 任何失败路径都把传入 value 引用计数 -1 (为 0 则
//       json_delete) 并返回 -1。
int json_array_set(int32_t *array, uint32_t index, int32_t *value)
{
	int32_t *slot;
	int32_t item;
	int32_t refcnt;

	if (value != NULL) {
		if (array != NULL && array[0] == 1 && array != value &&
		    index < (uint32_t)array[3]) {
			slot = (int32_t *)(array[4] + index * 4);
			item = *(int32_t *)(array[4] + index * 4);
			if (item != 0 && *(int32_t *)(item + 4) != -1) {
				refcnt = *(int32_t *)(item + 4) - 1;
				*(int32_t *)(item + 4) = refcnt;
				if (refcnt == 0) {
					json_delete(item);   /* 原码 r0 保持旧项指针 */
					slot = (int32_t *)(array[4] + index * 4);
				}
			}
			*slot = (int32_t)value;
			return 0;
		}
		if (value[1] != -1) {   /* value->refcount */
			refcnt = value[1] - 1;
			value[1] = refcnt;
			if (refcnt == 0) {
				json_delete(value);
			}
		}
	}
	return -1;
}

/* ------------------------------------------------------------------
 * 分组 9: opt 选项 setter 家族 (argp/wrap)
 * ------------------------------------------------------------------ */

// === arg_bad === 原 FUN_0005b0c0 @ 0x0005b0c0
// 来源: reversed/cgminer/70_no_strings/FUN_0005b0c0.c | 分类: 70_no_strings
// 语义: 构造错误串—malloc(strlen(fmt)+strlen(arg)) 后 sprintf(fmt, arg)
//       返回堆上消息 (调用方负责 free)。
char *arg_bad(const char *fmt, const char *arg)
{
	size_t len_fmt;
	size_t len_arg;
	char *msg;

	len_fmt = strlen(fmt);
	len_arg = strlen(arg);
	msg = malloc(len_arg + len_fmt);
	sprintf(msg, fmt, arg);
	return msg;
}

/* 注: 本组 setter 原体 Ghidra 标为 void, 但上游调用方 (如
 * opt_set_floatval_range_100_250) 均消费 r0 返回的错误串指针,
 * 成功时 r0 = NULL — 故按 char * 语义转写。 */

// === opt_invalid_argument === 原 FUN_0005b120 @ 0x0005b120
// 来源: reversed/cgminer/70_no_strings/FUN_0005b120.c | 分类: 70_no_strings
// 语义: 布尔参数解析—大小写不识别 "yes"/"true" 置 1, "no"/"false" 置 0
//       并返回 NULL; 其余串 malloc(strlen+0x16) 后构
//       "Invalid argument '<arg>'" 返回。
char *opt_invalid_argument(char *arg, uint8_t *val)
{
	size_t len;
	char *msg;
	int cmp;

	cmp = strcasecmp(arg, "yes");
	if (cmp == 0 || strcasecmp(arg, "true") == 0) {
		*val = 1;
		return NULL;
	}
	cmp = strcasecmp(arg, "no");
	if (cmp == 0 || strcasecmp(arg, "false") == 0) {
		*val = 0;
		return NULL;
	}
	len = strlen(arg);
	msg = malloc(len + 0x16);
	sprintf(msg, "Invalid argument '%s'", arg);
	return msg;
}

// === opt_set_floatval === 原 FUN_0005b1e0 @ 0x0005b1e0
// 来源: reversed/cgminer/70_no_strings/FUN_0005b1e0.c | 分类: 70_no_strings
// 语义: 浮点解析 (strtof)—errno 清零后解析; 尾部残留字符或空串 →
//       "'%s' is not a number"; errno 非零 (溢出) → "'%s' is out of range"。
char *opt_set_floatval(const char *arg, float *val)
{
	int *errno_loc;
	float tmp;
	char *end;

	errno_loc = __errno_location();
	*errno_loc = 0;
	tmp = strtof(arg, &end);
	*val = tmp;
	if (*end == '\0' && *arg != '\0') {
		if (*errno_loc != 0) {
			return arg_bad("'%s' is out of range", arg);
		}
		return NULL;
	}
	return arg_bad("'%s' is not a number", arg);
}

// === opt_set_longval === 原 FUN_0005b264 @ 0x0005b264
// 来源: reversed/cgminer/70_no_strings/FUN_0005b264.c | 分类: 70_no_strings
// 语义: 长整解析 (strtol, 进制自动)—错误语义同 opt_set_floatval。
char *opt_set_longval(const char *arg, long *val)
{
	int *errno_loc;
	long tmp;
	char *end;

	errno_loc = __errno_location();
	*errno_loc = 0;
	tmp = strtol(arg, &end, 0);
	*val = tmp;
	if (*end == '\0' && *arg != '\0') {
		if (*errno_loc != 0) {
			return arg_bad("'%s' is out of range", arg);
		}
		return NULL;
	}
	return arg_bad("'%s' is not a number", arg);
}

// === opt_set_intval === 原 FUN_0005b2e8 @ 0x0005b2e8
// 来源: reversed/cgminer/70_no_strings/FUN_0005b2e8.c | 分类: 70_no_strings
// 语义: 整数解析—经 opt_set_longval 解析成功后截断写入 int。
char *opt_set_intval(const char *arg, int *val)
{
	long tmp;
	char *err;

	err = opt_set_longval(arg, &tmp);
	if (err == NULL) {
		*val = (int)tmp;
		return NULL;
	}
	return err;
}

// === opt_set_uintval === 原 FUN_0005b310 @ 0x0005b310
// 来源: reversed/cgminer/70_no_strings/FUN_0005b310.c | 分类: 70_no_strings
// 语义: 无符号整数解析—经 opt_set_intval, 解析成功但为负 →
//       "'%s' is negative" (不写入); 成功写入并返回 NULL。
char *opt_set_uintval(const char *arg, unsigned int *val)
{
	int tmp[2];
	char *err;

	err = opt_set_intval(arg, tmp);
	if (err == NULL) {
		if (tmp[0] < 0) {
			return arg_bad("'%s' is negative", arg);
		}
		*val = (unsigned int)tmp[0];
	}
	return err;
}

// === opt_set_ulongval === 原 FUN_0005b35c @ 0x0005b35c
// 来源: reversed/cgminer/70_no_strings/FUN_0005b35c.c | 分类: 70_no_strings
// 语义: 无符号长整解析—经 opt_set_longval; 注意原码先写入 *val 再检查
//       负值 (与 opt_set_uintval 相反), 为负时返回 "'%s' is negative"。
char *opt_set_ulongval(const char *arg, unsigned long *val)
{
	long tmp[2];
	char *err;

	err = opt_set_longval(arg, tmp);
	if (err == NULL) {
		*val = (unsigned long)tmp[0];
		if (tmp[0] < 0) {
			return arg_bad("'%s' is negative", arg);
		}
	}
	return err;
}

/* ---- 共享全局: 程序名 (hash.c 同名) ---- */
extern char *DAT_0009d780;  /* opt_usage(argv0, ...) 的 argv0 实参 (程序名指针) */

// === opt_version_and_exit === 原 FUN_0005b3c0 @ 0x0005b3c0
// 来源: reversed/cgminer/70_no_strings/FUN_0005b3c0.c | 分类: 70_no_strings
// 语义: 打印版本串后退出 (exit 0)。
void opt_version_and_exit(char *version)
{
	puts(version);
	fflush(stdout);
	exit(0);
}

// === opt_usage_and_exit === 原 FUN_0005b3e0 @ 0x0005b3e0
// 来源: reversed/cgminer/70_no_strings/FUN_0005b3e0.c | 分类: 70_no_strings
// 语义: 打印用法串后退出—opt_usage(程序名, extra) 生成全文, printf 后
//       exit(0)。(反编译残留的多余寄存器实参已略去, 与 hash.c 调用一致)
void opt_usage_and_exit(const char *extra)
{
	char *usage;

	usage = opt_usage(DAT_0009d780, extra);
	printf("%s", usage);
	fflush(stdout);
	exit(0);
}

// === opt_show_charp === 原 FUN_0005b468 @ 0x0005b468
// 来源: reversed/cgminer/70_no_strings/FUN_0005b468.c | 分类: 70_no_strings
// 语义: ccan/opt show 回调 (charp)—向 OPT_SHOW_LEN(0x50) 缓冲写
//       "引号+字符串+引号"; 超过 0x4E 字节截断 (buf[0x4F] 收引号),
//       恰 0x4E 时不写结尾 NUL (缓冲正好填满)。
void opt_show_charp(char *buf, char **val)
{
	size_t len;

	len = strlen(*val);
	*buf = '"';
	if (0x4e < len) {
		strncpy(buf + 1, *val, 0x4e);
		buf[0x4f] = '"';
		return;
	}
	strncpy(buf + 1, *val, len);
	buf[len + 1] = '"';
	if (len != 0x4e) {
		buf[len + 2] = '\0';
	}
}

// === _opt_register === 原 FUN_0005bb70 @ 0x0005bb70
// 来源: reversed/cgminer/70_no_strings/FUN_0005bb70.c | 分类: 70_no_strings
// 语义: ccan/opt 注册—把 7 个字打包成 28 字节选项表项 (栈上 0x24..0x0C
//       七字槽, 字段序同 ccan opt_table: name/type/cb/arg/show/desc/…, 语义待定),
//       依次调 check_opt 与 add_opt 挂入选项表。
void _opt_register(uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4,
                   uint32_t p5, uint32_t p6, uint32_t p7)
{
	uint32_t entry[7];

	entry[0] = p1;
	entry[1] = p2;
	entry[2] = p3;
	entry[3] = p4;
	entry[4] = p5;
	entry[5] = p6;
	entry[6] = p7;
	check_opt(entry);   /* 原 FUN_0005b7c8 */
	add_opt(entry);     /* 原 FUN_0005b9a8 */
}

/* ------------------------------------------------------------------
 * 分组 10: 编译器辅助 64 位除法 (libgcc)
 * ------------------------------------------------------------------ */

int64_t __divdi3(uint32_t num_lo, uint32_t num_hi, uint32_t den_lo, uint32_t den_hi);  /* 原 FUN_0005cd08 @ 0x0005cd08 */

// === __divmoddi4 === 原 FUN_0005cc90 @ 0x0005cc90
// 来源: reversed/cgminer/70_no_strings/FUN_0005cc90.c | 分类: 70_no_strings
// 语义: 带余数除法—商 = __divdi3(num, den), 余数 = num − 商×den 写入
//       *rem_out (借位入高位); 返回商 (libgcc __divmoddi4 语义, 原码
//       r0/r1 返回值 Ghidra 未捕获)。
int64_t __divmoddi4(uint32_t num_lo, uint32_t num_hi, uint32_t den_lo, uint32_t den_hi,
                    int64_t *rem_out)
{
	int64_t quot;
	int64_t prod;
	uint32_t prod_lo;

	quot = __divdi3(num_lo, num_hi, den_lo, den_hi);
	prod = quot * (int64_t)((uint64_t)den_hi << 32 | den_lo);
	prod_lo = (uint32_t)prod;
	*(uint32_t *)rem_out = num_lo - prod_lo;
	*((uint32_t *)rem_out + 1) = num_hi - (uint32_t)((uint64_t)prod >> 32)
	                             - (uint32_t)(num_lo < prod_lo);
	return quot;
}

/* ---- __divdi3 所用的 32 位除法辅助 (另批次转写) ---- */
extern uint64_t __udivsi3(uint32_t num, uint32_t den);  /* 原 FUN_0005c648: 返回 (余数<<32)|商; 除数 0 → raise(SIGFPE) */
extern uint32_t __umodsi3(uint32_t num, uint32_t den);  /* 原 FUN_0005c6f0: 除数 0 → raise(SIGFPE) */

/* 半字除法步 — 原码逐点成对展开 __umodsi3(除零守卫)+__udivsi3(商/余配对)
 * 的等价封装, 不改变任何点位行为 */
static uint32_t divmod_half32(uint32_t part, uint32_t dh, uint32_t *rem_out)
{
	uint64_t pair;

	__umodsi3(part, dh);
	pair = __udivsi3(part, dh);
	*rem_out = (uint32_t)(pair >> 32);
	return (uint32_t)pair;
}

/* 原码估计修正: part < est 时至多两轮 part += d 并降商
 * (q 以原始值 −1/−2 落定, CARRY4 进位守护; 第二轮加法在
 * 路径 A-else 尾站为死存储, 一并保留不影响行为) */
static uint32_t q_fixdown(uint32_t *part, uint32_t est, uint32_t d, uint32_t q)
{
	if (*part < est) {
		uint32_t sum = *part + d;
		uint32_t carry = (sum < *part);   /* 原 CARRY4(part, d) */

		*part = sum;
		q = q - 1;
		if (carry == 0 && *part < est) {
			*part = *part + d;
			q = q - 2;
		}
	}
	return q;
}

// === __divdi3 === 原 FUN_0005cd08 @ 0x0005cd08
// 来源: reversed/cgminer/70_no_strings/FUN_0005cd08.c | 分类: 70_no_strings
// 语义: libgcc 带符号 64 位除法 (ARM udivmoddi4 型算法)—符号归一后:
//       除数高字为 0 走 64/32 路径 (商可 达 64 位, 两程半字除法), 否则走
//       64/64 路径 (商必 < 2^32, 乘积校验回退一步); 除零经
//       __udivsi3(1,0) 触发 SIGFPE; 商按符号标志取负返回。
//       注: 反编译 LZCOUNT(param_4) 在 64/32 低程 (0x5cd78) 处经反汇编
//       核实实为 clz den_lo (r12), 已按机器码修正。
int64_t __divdi3(uint32_t num_lo, uint32_t num_hi, uint32_t den_lo, uint32_t den_hi)
{
	uint32_t n_lo, n_hi;      /* |num| */
	uint32_t d_lo, d_hi;      /* |den| */
	uint32_t neg;             /* 0xffffffff: 商为负 */
	uint32_t q_hi = 0, q_lo = 0;
	uint32_t shift, d_word, dh, dlow16;
	uint32_t top, midd, part, rem, est, q_a, q_b;
	uint32_t d2hi, rem2;      /* 64/64 路径 */
	uint64_t prod;

	/* 符号归一 (原 64 位取负: hi = -(hi + (lo != 0))) */
	if ((int32_t)num_hi < 0) {
		n_lo = -num_lo;
		n_hi = -(num_hi + (num_lo != 0));
		neg = 0xffffffff;
	} else {
		neg = 0;
		n_lo = num_lo;
		n_hi = num_hi;
	}
	d_lo = den_lo;
	if ((int32_t)den_hi < 0) {
		d_lo = -den_lo;
		d_hi = -(den_hi + (den_lo != 0));
		neg = ~neg;
	} else {
		d_hi = den_hi;
	}

	if (d_hi == 0) {
		/* ---------------- 64/32 路径 ---------------- */
		if (d_lo <= n_hi) {
			/* 商可达 64 位: 归一化后两程半字除法 */
			if (d_lo == 0) {
				d_lo = (uint32_t)__udivsi3(1, 0);   /* 除零: raise(SIGFPE) */
			}
			shift = __builtin_clz(d_lo);            /* 原 LZCOUNT (arm clz) */
			d_word = d_lo << shift;                 /* 归一化除数 D' */
			if (shift == 0) {
				q_hi = 1;                    /* 商高 32 位 = 1 */
				rem = n_hi - d_lo;           /* 高程余数 */
			} else {
				top = n_hi >> (32 - shift);
				midd = (n_lo >> (32 - shift)) | (n_hi << shift);
				dh = d_word >> 16;
				dlow16 = d_word & 0xffff;
				q_a = divmod_half32(top, dh, &rem);
				part = (midd >> 16) | (rem << 16);
				est = dlow16 * q_a;
				q_a = q_fixdown(&part, est, d_word, q_a);
				q_b = divmod_half32(part - est, dh, &rem);
				part = (midd & 0xffff) | (rem << 16);
				est = dlow16 * q_b;
				q_b = q_fixdown(&part, est, d_word, q_b);
				rem = part - est;            /* 高程余数 */
				q_hi = q_b | (q_a << 16);
			}
			/* 公共低半程: 商低 32 位 */
			n_lo = n_lo << shift;
			dh = d_word >> 16;
			dlow16 = d_word & 0xffff;
			q_a = divmod_half32(rem, dh, &rem);
			part = (n_lo >> 16) | (rem << 16);
			est = dlow16 * q_a;
			q_a = q_fixdown(&part, est, d_word, q_a);
			q_b = divmod_half32(part - est, dh, &rem);
			part = (n_lo & 0xffff) | (rem << 16);
			est = dlow16 * q_b;
			q_b = q_fixdown(&part, est, d_word, q_b);
			q_lo = q_b | (q_a << 16);
		} else {
			/* n_hi < d_lo: 商 < 2^32, 单程半字除法 (原 0x5cd78 起) */
			shift = __builtin_clz(d_lo);
			d_word = d_lo << shift;
			midd = (shift == 0) ? n_hi
			      : ((n_lo >> (32 - shift)) | (n_hi << shift));
			n_lo = n_lo << shift;
			dh = d_word >> 16;
			dlow16 = d_word & 0xffff;
			q_a = divmod_half32(midd, dh, &rem);
			part = (n_lo >> 16) | (rem << 16);
			est = dlow16 * q_a;
			q_a = q_fixdown(&part, est, d_word, q_a);
			q_b = divmod_half32(part - est, dh, &rem);
			part = (n_lo & 0xffff) | (rem << 16);
			est = dlow16 * q_b;
			q_b = q_fixdown(&part, est, d_word, q_b);
			q_hi = 0;                     /* 原标签 0x5d130: 商高位清零 */
			q_lo = q_b | (q_a << 16);
		}
	} else {
		/* ---------------- 64/64 路径 (商必 < 2^32) ---------------- */
		if (d_hi <= n_hi) {
			shift = __builtin_clz(d_hi);
			if (shift == 0) {
				/* D' 已归一: 商 ∈ {0,1}, 64 位比较定夺 */
				if (n_hi > d_hi || (n_hi == d_hi && n_lo >= d_lo)) {
					q_hi = 0;   /* 原 movhs r5, r10 (=0) */
					q_lo = 1;   /* 原 movhs r12, #1 */
				} else {
					q_hi = 0;
					q_lo = 0;
				}
			} else {
				d2hi = (d_lo >> (32 - shift)) | (d_hi << shift);
				top = n_hi >> (32 - shift);
				midd = (n_lo >> (32 - shift)) | (n_hi << shift);
				dh = d2hi >> 16;
				dlow16 = d2hi & 0xffff;
				q_a = divmod_half32(top, dh, &rem);
				part = (midd >> 16) | (rem << 16);
				est = dlow16 * q_a;
				q_a = q_fixdown(&part, est, d2hi, q_a);
				q_b = divmod_half32(part - est, dh, &rem);
				part = (midd & 0xffff) | (rem << 16);
				est = dlow16 * q_b;
				q_b = q_fixdown(&part, est, d2hi, q_b);
				q_lo = q_b | (q_a << 16);           /* 商 32 位 */
				rem2 = part - est;
				/* 乘积校验: q×(d_lo<<shift) 与 (rem2 : n_lo<<shift) 比较,
				 * 估高一步则回退 (原 0x5d104 umull 校验) */
				prod = (uint64_t)(d_lo << shift) * (uint64_t)q_lo;
				if (((uint32_t)(prod >> 32) < rem2 ||
				     (uint32_t)(prod >> 32) == rem2) &&
				    ((uint32_t)prod <= (n_lo << shift) ||
				     (uint32_t)(prod >> 32) != rem2)) {
					/* 接受 (原商高位清零后直达 0x5d140) */
				} else {
					q_lo = q_lo - 1;
				}
				q_hi = 0;
			}
		} else {
			q_hi = 0;   /* d_hi > n_hi: 商 = 0 (原 0x5d138) */
			q_lo = 0;
		}
	}

	/* 原标签 0x5d140: 商按符号取负 (64 位取负) */
	if (neg != 0) {
		uint32_t borrow = (q_lo != 0);

		q_lo = -q_lo;
		q_hi = -(q_hi + borrow);
	}
	return (int64_t)((uint64_t)q_hi << 32 | q_lo);
}

/* ------------------------------------------------------------------
 * 分组 11: C 运行时启动/收尾 (csu 与 ELF 桩)
 * ------------------------------------------------------------------ */

extern void _init(void);  /* 原 .init 节入口 */

/* ---- 共享全局: init_array / csu_fini / dtors 完成标志 ---- */
extern void (*DAT_000751b0)(int argc, char **argv, char **envp);  /* init_array 首项函数指针, 语义待定 */
extern char DAT_0005d5a4;      /* __libc_csu_fini 桩 (_fini 前一字), 语义待定 */
extern char DAT_00076bd4;      /* dtors 已执行标志 */

// === __libc_csu_init === 原 FUN_0005d540 @ 0x0005d540
// 来源: reversed/cgminer/70_no_strings/FUN_0005d540.c | 分类: 70_no_strings
// 语义: glibc csu 初始化—先 _init(), 再遍历 init_array (基址 0x000751b0,
//       原指针自 0x000751ac 步进; 本固件恰 1 项, 循环仅执行一次) 并以
//       (argc, argv, envp) 调用。
void __libc_csu_init(int argc, char **argv, char **envp)
{
	int i;

	_init();   /* 原码实参为寄存器残留, 略去 */
	i = 0;
	do {
		i = i + 1;
		DAT_000751b0(argc, argv, envp);   /* 原 *(entry++) 间接调用 */
	} while (i != 1);
}

// === __do_global_dtors_aux === 原 FUN_0000f6e0 @ 0x0000f6e0
// 来源: reversed/cgminer/70_no_strings/_FINI_0.c | 分类: 70_no_strings
// 语义: 析构收尾桩—DAT_00076bd4 标志未置时执行 deregister_tm_clones
//       并置位 (保证只跑一次)。
void __do_global_dtors_aux(void)
{
	if (DAT_00076bd4 == '\0') {
		deregister_tm_clones();
		DAT_00076bd4 = 1;
	}
}

// === _fini === 原 FUN_0005d5a8 @ 0x0005d5a8
// 来源: reversed/cgminer/70_no_strings/_fini.c | 分类: 70_no_strings
// 语义: .fini 节入口—空体, 立即返回。
void _fini(void)
{
	return;
}

/* ---- 共享全局: 进程入口相关 ---- */
extern int bitmain_zcash_driver_init(void);  /* 原 FUN_0000b928: main 函数本体 (签名待定) */

// === _start === 原 FUN_0000f608 @ 0x0000f608
// 来源: reversed/cgminer/70_no_strings/entry.c | 分类: 70_no_strings
// 语义: ELF 入口—__libc_start_main(main, argc, &argv 栈区, __libc_csu_init,
//       csu_fini, 参数回传) 后不应返回, 返回即 abort。
void _start(uint32_t param_1, uint32_t param_2, uint8_t param_3)
{
	__libc_start_main(bitmain_zcash_driver_init, param_2, &param_3,
	                  __libc_csu_init, &DAT_0005d5a4, param_1);
	abort();
}

/* ============================================================
 * 覆盖率表 (72/72) — nostr_b1 / reversed/cgminer/70_no_strings/
 * 原地址         | name_map 冻结名               | 本文函数名                   | 分组
 * ------------------------------------------------------------
// 01 | FUN_0000f644 | __gmon_start__             | __gmon_start__             | 组1
// 02 | FUN_0000f668 | deregister_tm_clones       | deregister_tm_clones       | 组1
// 03 | FUN_00011c1c | delayed_exit_thread        | delayed_exit_thread        | 组2
// 04 | FUN_00012164 | api_thread                 | api_thread                 | 组2
// 05 | FUN_000121b0 | opt_set_floatval_range_100_250 | opt_set_floatval_range_100_250 | 组9
// 06 | FUN_00012200 | opt_set_floatval_range_125_500 | opt_set_floatval_range_125_500 | 组9
// 07 | FUN_00014e38 | set_sched_stop             | set_sched_stop             | 组2
// 08 | FUN_00014eb4 | set_sched_start            | set_sched_start            | 组2
// 09 | FUN_0001545c | opt_set_intval_range       | opt_set_intval_range       | 组9
// 10 | FUN_000158ac | set_work_nonce2_hex        | set_work_nonce2_hex        | 组2
// 11 | FUN_0001795c | sighandler                 | sighandler                 | 组2
// 12 | FUN_00017c4c | set_userpass               | set_userpass               | 组2
// 13 | FUN_00017f68 | set_pass                   | set_pass                   | 组2
// 14 | FUN_00028a40 | copy_drv                   | copy_drv                   | 组2
// 15 | FUN_00029c70 | base58_decode              | base58_decode              | 组2
// 16 | FUN_0002a1a8 | tq_free                    | tq_free                    | 组2
// 17 | FUN_0002a414 | thr_info_cancel            | thr_info_cancel            | 组3
// 18 | FUN_0002a588 | us_to_timeval              | us_to_timeval              | 组3
// 19 | FUN_0002a648 | ms_to_timeval              | ms_to_timeval              | 组3
// 20 | FUN_0002a800 | strsep                     | strsep                     | 组3
// 21 | FUN_0002f400 | cgsem_reset                | cgsem_reset                | 组3
// 22 | FUN_0002f4f0 | _cg_memcpy                 | _cg_memcpy                 | 组3
// 23 | FUN_00032f3c | api_add_hex32              | api_add_hex32              | 组4
// 24 | FUN_00032fa8 | api_add_double             | api_add_double             | 组4
// 25 | FUN_00033b88 | failoveronly               | failoveronly               | 组4
// 26 | FUN_00035544 | api_add_data_type19        | api_add_data_type19        | 组4
// 27 | FUN_0003558c | api_add_freq               | api_add_freq               | 组4
// 28 | FUN_000355b0 | api_add_volts              | api_add_volts              | 组4
// 29 | FUN_000355d4 | api_add_hs                 | api_add_hs                 | 组4
// 30 | FUN_000363d8 | api_add_avg                | api_add_avg                | 组4
// 31 | FUN_0003927c | _k_unlink_head_zero        | _k_unlink_head_zero        | 组5
// 32 | FUN_0003a4a4 | bitmain_zcash_scanwork     | bitmain_zcash_scanwork     | 组5
// 33 | FUN_0003aa9c | check_fan                  | check_fan                  | 组6
// 34 | FUN_0003e688 | read_dc_dc_voltage_all     | read_dc_dc_voltage_all     | 组6
// 35 | FUN_0003e9ac | set_dc_dc_voltage_all      | set_dc_dc_voltage_all      | 组6
// 36 | FUN_00046c10 | write_pic16f1704_flash_all_chain | write_pic16f1704_flash_all_chain | 组6
// 37 | FUN_000497cc | clear_nonce_fifo           | clear_nonce_fifo           | 组6
// 38 | FUN_00049ba0 | get_dc_dc_voltage          | get_dc_dc_voltage          | 组6
// 39 | FUN_0004b4f0 | sha256_transform           | sha256_transform           | 组7
// 40 | FUN_0004dbec | sha256_update              | sha256_update              | 组7
// 41 | FUN_0004dccc | sha256_final               | sha256_final               | 组7
// 42 | FUN_0004de80 | sha256                     | sha256                     | 组7
// 43 | FUN_000577f0 | json_dumpf                 | json_dumpf                 | 组8
// 44 | FUN_00057804 | json_dump_file             | json_dump_file             | 组8
// 45 | FUN_00057e28 | hashtable_reset            | hashtable_reset            | 组8
// 46 | FUN_00057f4c | json_object_pair_set_value | json_object_pair_set_value | 组8
// 47 | FUN_00059798 | json_string_length         | json_string_length         | 组8
// 48 | FUN_00059e68 | json_object_clear          | json_object_clear          | 组8
// 49 | FUN_0005a03c | json_array_extend          | json_array_extend          | 组8
// 50 | FUN_0005a194 | json_string_set_nocheck    | json_string_set_nocheck    | 组8
// 51 | FUN_0005a1e0 | json_string_set            | json_string_set            | 组8
// 52 | FUN_0005a328 | json_real_set              | json_real_set              | 组8
// 53 | FUN_0005a4b8 | json_array_clear           | json_array_clear           | 组8
// 54 | FUN_0005a534 | json_array_remove          | json_array_remove          | 组8
// 55 | FUN_0005a76c | json_array_set             | json_array_set             | 组8
// 56 | FUN_0005b0c0 | arg_bad                    | arg_bad                    | 组9
// 57 | FUN_0005b120 | opt_invalid_argument       | opt_invalid_argument       | 组9
// 58 | FUN_0005b1e0 | opt_set_floatval           | opt_set_floatval           | 组9
// 59 | FUN_0005b264 | opt_set_longval            | opt_set_longval            | 组9
// 60 | FUN_0005b2e8 | opt_set_intval             | opt_set_intval             | 组9
// 61 | FUN_0005b310 | opt_set_uintval            | opt_set_uintval            | 组9
// 62 | FUN_0005b35c | opt_set_ulongval           | opt_set_ulongval           | 组9
// 63 | FUN_0005b3c0 | opt_version_and_exit       | opt_version_and_exit       | 组9
// 64 | FUN_0005b3e0 | opt_usage_and_exit         | opt_usage_and_exit         | 组9
// 65 | FUN_0005b468 | opt_show_charp             | opt_show_charp             | 组9
// 66 | FUN_0005bb70 | _opt_register              | _opt_register              | 组9
// 67 | FUN_0005cc90 | __divmoddi4                | __divmoddi4                | 组10
// 68 | FUN_0005cd08 | __divdi3                   | __divdi3                   | 组10
// 69 | FUN_0005d540 | __libc_csu_init            | __libc_csu_init            | 组11
// 70 | _FINI_0      | __do_global_dtors_aux (原 _FINI_0) | __do_global_dtors_aux      | 组11
// 71 | _fini        | _fini (原 FUN_0005d5a8)    | _fini                      | 组11
// 72 | entry        | _start (原 FUN_0000f608)   | _start                     | 组11
 * ------------------------------------------------------------
 * 语义等价说明: 除 __divdi3 中 Ghidra 对 LZCOUNT 操作数的误归属
 * (已按反汇编 0x5cd78 修正为 clz den_lo) 与各处 Ghidra 丢失的
 * 寄存器实参 (均按反汇编核实补回) 外, 与反编译产物逐点一致。
 * ============================================================ */
