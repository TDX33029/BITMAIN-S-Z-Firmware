/* ============================================================================
 * cgminer/log.c — 日志层转写 (batch log_b1, 分类 41_logging, 共 9 函数)
 *
 * 内容:
 *   1) set_sharelog / sharelog: share 记录 CSV 输出流的打开与写入
 *   2) switch_logsize / clear_logwin / logwin_update: curses 日志窗口操作
 *   3) curses_int / display_options: curses 交互菜单 (日志选项开关)
 *   4) my_log_curses_lock / my_log_curses_unlock: curses 互斥锁出错上报包装
 *
 * 转写约定 (rewritten/CONVENTIONS.md):
 *   - 命名事实源 rewritten/name_map/cgminer.tsv (batch_plan.tsv 分配 9 个 FUN_)。
 *   - 优先级数值沿用 syslog 标准: LOG_ERR=3 / LOG_WARNING=4 / LOG_NOTICE=5 /
 *     LOG_INFO=6 / LOG_DEBUG=7 (本文件调用点仅 LOG_ERR=3)。
 *   - 未消化的 DAT_xxx 全局在本文件集中 extern (原名保留, 注释给出语义证据);
 *     后续批次共享时应上移到 include/ 头文件。
 *   - 原二进制把字符串字面量拆成栈上逐字赋值 (Ghidra 显示为单字节写入), 转写
 *     还原为等价字面量 — 产生的缓冲内容与 applog/wlogprint 调用完全一致。
 *   - 日志门控 (反编译统一模式, 参见 uart.c 头注释):
 *       LOG_ERR(3): 跳过条件 = A==0 && B==0 && C<3
 *     其中 A=DAT_0007ad48 (applog 内非 0 走 syslog 输出), B=DAT_00078e08
 *     (curses 'v' Verbose 联动), C=DAT_000766c4 (int 日志级别)。
 * ============================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>     /* strncasecmp */
#include <pthread.h>
#include <errno.h>
#include <curses.h>

#define LOG_ERR 3        /* syslog LOG_ERR (反编译调用点恒为数值 3) */

/* 反编译统一日志门控: 三条件全关时调用点直接跳过 applog(LOG_ERR, ...) */
#define LOG_GATE_CLOSED() \
	(DAT_0007ad48 == '\0' && DAT_00078e08 == '\0' && DAT_000766c4 < 3)

/* ================= 外部函数 (name_map 语义名) ================= */

extern void  applog(uint32_t prio, char *buf, ...); /* FUN_00038730 (job_p5b): 第 3 参 force 非 0 → 强制 curses
                                                     * 刷新; 部分调用点原反编译仅 2 参 (r2 残留值), 故声明为
                                                     * 可变参使 2/3 参调用点均成立 (AAPCS 下 ABI 等价) */
extern void  quit(int status);                                     /* FUN_00016ca8: 单参包装, 内部走 __quit(FUN_0001d8fc) */
extern void  mutex_lock_fail_cgminer(const char *fn, int line);    /* FUN_0000b414 */
extern void  mutex_unlock_fail_cgminer(const char *fn, int line);  /* FUN_0000b5c4 */
extern int   curses_active_locked(void);                           /* FUN_00017fe4: 加 curses 锁并返回 curses 是否激活 */
extern void  check_winsizes(void);                                 /* FUN_00018068 */
extern void  _wlogprint(const char *str);                          /* FUN_00018bc0 */
extern void *get_thread(int id);                                   /* FUN_0001b124: 返回 thr_info* (布局未用) */
extern char *bin2hex(const void *data, size_t len);                /* FUN_00029730 */
extern char *curses_input(const char *prompt);                     /* FUN_00025fdc */
extern void  zero_stats(void);                                     /* FUN_0001b8b4 */

/* ================= 外部全局 (DAT_ 原名保留, 注释为已核实语义) ================= */

extern FILE *DAT_00076cf4;            /* sharelog 输出流: set_sharelog 打开, sharelog 写入 */
extern uint8_t DAT_0007ad48;          /* applog 门控 A: 非 0 → applog 走 syslog(prio|0x80) */
extern uint8_t DAT_00078e08;          /* applog 门控 B = opt_verbose (curses 'v' 联动) */
extern uint8_t DAT_00078e09;          /* opt_debug (curses 'd' 联动, 与 DAT_00078e08 地址相邻) */
extern int DAT_000766c4;              /* applog 门控 C: int 日志级别 (LOG_ERR=3 阈值) */
extern uint8_t DAT_0007b940;          /* compact 模式开关 (curses 'm'; switch_logsize 按其选布局) */
extern uint8_t DAT_0007b858;          /* Per-device stats 开关 (curses 'p') */
extern uint8_t DAT_0007b85a;          /* WorkTime details 开关 (curses 'w') */
extern uint8_t DAT_0007b380;          /* RPC debug 开关 (curses 'r') */
extern uint8_t DAT_0007bc78;          /* opt_quiet (curses 'q'; __quit/主循环亦检查) */
extern uint8_t DAT_00077155;          /* status switching 开关 (curses 't') */
extern uint8_t DAT_00077318;          /* widescreen 开关 (curses 'i') */
extern uint8_t DAT_0007bb18;          /* 静默退出标志 (curses 's' 置 1; __quit/主循环检查) [低置信] */
extern uint8_t DAT_0007bac8;          /* display_options 会话标志: 1=菜单显示中, FUN_000199e8 据此抑制 logwin 逐条刷新 */
extern int DAT_00075dcc;              /* opt_log_interval (菜单 "[L]og interval" 显示值, 秒) */
extern int DAT_00077028;              /* curses 界面布局行号 A (与 7702c/7701c/77024 连续, mvwin/mvwprintw 使用) */
extern int DAT_0007702c;              /* curses 界面布局行号 B (非 compact 模式的行偏移) */
extern int DAT_0007701c;              /* curses 界面布局行号: switch_logsize 写 = base+1 */
extern int DAT_00077024;              /* curses 界面布局行号: 日志窗口顶行 (switch_logsize 写 = base+2) */
extern pthread_mutex_t DAT_0007b7a8;  /* curses 互斥锁 (curses_active_locked / my_log_curses_lock/unlock) */
extern pthread_mutex_t DAT_000770b8;  /* sharelog 写入互斥锁 */
extern WINDOW *DAT_0007b430;          /* logwin: curses 日志窗口 (wprintw/wrefresh/mvwin/wresize/immedok) */
extern void (*DAT_00075ed8)(void);    /* 全局函数指针: ELF 初值 noop_hook (FUN_00028ae0, 恒 0), 运行期未见改写 [低置信] */

/* ============================================================================
 * 函数转写
 * ============================================================================ */

// === set_sharelog === 原 FUN_0000fa18 @ 0x0000fa18
// 来源: reversed/cgminer/41_logging/FUN_0000fa18.c | 分类: 41_logging
// 语义: 解析 sharelog 参数并打开输出流 (副作用: DAT_00076cf4)。
//       纯非负十进制数字 → fdopen(该 fd, "a"); "-" → stdout;
//       其余 → fopen(路径, "a") 追加。打开失败时流置 NULL, 且仅当日志门控
//       开启才 applog(LOG_ERR) 报错。恒返回 0。
int set_sharelog(char *arg)
{
	char buf[4096];
	char *endptr = "";
	int fd;

	fd = (int)strtol(arg, &endptr, 10);
	if (*endptr == '\0' && fd >= 0) {
		/* 参数为非负十进制整数: 视作已打开 fd 复用 */
		DAT_00076cf4 = fdopen(fd, "a");
		if (DAT_00076cf4 != NULL)
			return 0;
		if (LOG_GATE_CLOSED())
			return 0;                       /* 门控关闭: 静默失败 */
		snprintf(buf, 0x1000, "Failed to open fd %u for share log", (unsigned int)fd);
	} else if (arg[0] == '-' && arg[1] == '\0') {
		/* 参数 "-": 输出到标准输出 */
		DAT_00076cf4 = stdout;
		if (stdout != NULL)
			return 0;
		if (LOG_GATE_CLOSED())
			return 0;
		applog(LOG_ERR, "Standard output missing for share log");
		return 0;
	} else {
		/* 其余参数: 当作文件路径追加打开 */
		DAT_00076cf4 = fopen(arg, "a");
		if (DAT_00076cf4 != NULL)
			return 0;
		if (LOG_GATE_CLOSED())
			return 0;
		snprintf(buf, 0x1000, "Failed to open %s for share log", arg);
	}
	DAT_00076cf4 = NULL;
	applog(LOG_ERR, buf);
	return 0;
}

// === switch_logsize === 原 FUN_00018178 @ 0x00018178
// 来源: reversed/cgminer/41_logging/FUN_00018178.c | 分类: 41_logging
// 语义: 按 compact 模式重排 curses 界面行号布局全局 (调用方须已持 curses 锁,
//       本函数内部解锁; 解锁失败走 mutex_unlock_fail_cgminer 上报), 随后经
//       全局函数指针尾部挂钩并调 check_winsizes() 应用新窗口尺寸。
void switch_logsize(void)
{
	int base;

	if (DAT_0007b940 == '\0')                    /* 非 compact 模式 */
		base = DAT_00077028 + DAT_0007702c;
	else
		base = DAT_00077028;
	DAT_0007701c = base + 1;
	DAT_00077024 = base + 2;
	if (pthread_mutex_unlock(&DAT_0007b7a8) != 0)
		mutex_unlock_fail_cgminer("unlock_curses", 0xb72);
	(*DAT_00075ed8)();
	check_winsizes();
}

// === clear_logwin === 原 FUN_00018ad8 @ 0x00018ad8
// 来源: reversed/cgminer/41_logging/FUN_00018ad8.c | 分类: 41_logging
// 语义: curses 激活时 (curses_active_locked 内部加锁) 清空整屏与日志窗口,
//       之后解锁并经尾部挂钩返回; curses 未激活则直接返回。
void clear_logwin(void)
{
	if (curses_active_locked() == 0)
		return;
	werase(stdscr);
	wclear(DAT_0007b430);
	if (pthread_mutex_unlock(&DAT_0007b7a8) != 0)
		mutex_unlock_fail_cgminer("unlock_curses", 0xb72);
	(*DAT_00075ed8)();
}

// === logwin_update === 原 FUN_00018b44 @ 0x00018b44
// 来源: reversed/cgminer/41_logging/FUN_00018b44.c | 分类: 41_logging
// 语义: curses 激活时对日志窗口整窗标记触摸并刷新: 行数取 WINDOW->_maxy
//       (ncurses WINDOW +0x04 处 short) + 1, 窗口指针为 NULL 时以 -1 行调用
//       wtouchln; 刷新后解锁并经尾部挂钩返回。
void logwin_update(void)
{
	int lines;

	if (curses_active_locked() == 0)
		return;
	if (DAT_0007b430 == NULL)
		lines = -1;
	else
		lines = *(short *)((uint8_t *)DAT_0007b430 + 4) + 1;  /* WINDOW->_maxy + 1 */
	wtouchln(DAT_0007b430, 0, lines, 1);
	wrefresh(DAT_0007b430);
	if (pthread_mutex_unlock(&DAT_0007b7a8) != 0)
		mutex_unlock_fail_cgminer("unlock_curses", 0xb72);
	(*DAT_00075ed8)();
}

// === sharelog === 原 FUN_0001b600 @ 0x0001b600
// 来源: reversed/cgminer/41_logging/FUN_0001b600.c | 分类: 41_logging
// 语义: sharelog 流已打开时 (DAT_00076cf4) 拼一条 CSV share 记录并持锁写入:
//       "%lu,%s,%s,%s,%s%u,%u,%s,%s\n" = work+0x780 的 u64 (按 %lu)、矿工名
//       (第 1 实参)、work+0xa0 处 32B hex、pool(+0x6d4)+0xc4 字符串、cgpu 子
//       结构(+4)+8 字符串、cgpu+8 的 uint、work+0x6d0 的 uint、work+0xc0 处
//       32B hex、work+0x00 起 128B hex。snprintf 出错/写入失败按日志门控
//       applog(LOG_ERR); 写入期间持 DAT_000770b8 锁并 fflush。
//       注: work 结构布局未核实, 偏移按反编译原样保留。
void sharelog(const char *workername, void *work)
{
	char buf[1024];
	uint8_t *w = (uint8_t *)work;
	char *hex_a0, *hex_c0, *hex_head;
	void *thr, *cgpu, *pool;
	uint32_t dev_id;
	unsigned long w_u64;
	size_t len;

	if (DAT_00076cf4 == NULL)
		return;
	dev_id = *(uint32_t *)(w + 0x6d0);
	thr = get_thread((int)dev_id);
	pool = *(void **)(w + 0x6d4);
	w_u64 = *(unsigned long *)(w + 0x780);
	cgpu = *(void **)((uint8_t *)thr + 0x24);
	hex_a0 = bin2hex(w + 0xa0, 0x20);
	hex_c0 = bin2hex(w + 0xc0, 0x20);
	hex_head = bin2hex(w, 0x80);
	len = (size_t)snprintf(buf, 0x400, "%lu,%s,%s,%s,%s%u,%u,%s,%s\n",
			       w_u64, workername, hex_a0,
			       *(char **)((uint8_t *)pool + 0xc4),
			       *(char **)((uint8_t *)(*(void **)((uint8_t *)cgpu + 4)) + 8),
			       *(uint32_t *)((uint8_t *)cgpu + 8),
			       dev_id, hex_c0, hex_head);
	free(hex_a0);
	free(hex_c0);
	free(hex_head);
	if ((int)len >= 0x400)
		buf[0x3ff] = 0;                      /* 截断兜底 (snprintf 已保证, 反编译保留该写) */
	else if ((int)len < 0) {
		if (LOG_GATE_CLOSED())
			return;
		applog(LOG_ERR, "sharelog printf error");
		return;
	}
	if (pthread_mutex_lock(&DAT_000770b8) != 0)
		mutex_lock_fail_cgminer("sharelog", 0x2a1);
	len = fwrite(buf, len, 1, DAT_00076cf4);
	fflush(DAT_00076cf4);
	if (pthread_mutex_unlock(&DAT_000770b8) != 0)
		mutex_unlock_fail_cgminer("sharelog", 0x2a4);
	(*DAT_00075ed8)();
	if (len == 1)
		return;
	if (LOG_GATE_CLOSED())
		return;
	applog(LOG_ERR, "sharelog fwrite error");
}

// === curses_int === 原 FUN_000260d0 @ 0x000260d0
// 来源: reversed/cgminer/41_logging/FUN_000260d0.c | 分类: 41_logging
// 语义: 弹出 curses 输入框取一串输入并按十进制 strtol 转整数返回, 输入缓冲
//       用后释放。注: Ghidra 未恢复形参, 实际 r0 透传给 curses_input 作提示
//       文案 (调用点 display_options 传 "Interval in seconds")。
long curses_int(const char *prompt)
{
	char *str;
	long ret;

	str = curses_input(prompt);
	ret = strtol(str, NULL, 10);
	free(str);
	return ret;
}

// === display_options === 原 FUN_000260fc @ 0x000260fc
// 来源: reversed/cgminer/41_logging/FUN_000260fc.c | 分类: 41_logging
// 语义: curses 日志选项交互菜单主循环。置会话标志 DAT_0007bac8=1 并对日志窗
//       口 immedok(1) 后循环: 打印选项列表与各开关当前状态 ("off/on" 或
//       "disabled/enabled"), 显示 "Select an option..." 提示, logwin_update()
//       后 wgetch 取键分发: q/v/n/d/m/p/r/w 切换对应输出模式并回显 (d/v/r/p
//       打开时联动关 quiet, d/v/p 联动 verbose), n/m 后经
//       curses_active_locked→switch_logsize 重排布局, l 询问并校验 (<10000)
//       新日志间隔, t/i 静默切换状态栏/widescreen, z 调 zero_stats 清零统计,
//       c 或未知键 → 清屏退出, s → 置静默退出标志 DAT_0007bb18 退出。退出前
//       恢复 immedok(0) 并清 DAT_0007bac8。
void display_options(void)
{
	char buf[4096];
	char ch;
	unsigned int loginterval;
	const char *debug_state, *perdev_state, *quiet_state, *verbose_state;
	const char *rpc_state, *worktime_state, *compact_state;
	const char *switching_state, *widescreen_state;

	DAT_0007bac8 = 1;                        /* 菜单显示中: 抑制 logwin 逐条刷新 (FUN_000199e8) */
	immedok(DAT_0007b430, 1);
	clear_logwin();
	for (;;) {
		_wlogprint("[N]ormal [C]lear [S]ilent mode (disable all output)\n");
		/* 各开关当前状态: 输出模式类用 off/on, 状态切换/宽屏用 disabled/enabled */
		debug_state = (DAT_00078e09 != 0) ? "on" : "off";
		perdev_state = (DAT_0007b858 != 0) ? "on" : "off";
		quiet_state = (DAT_0007bc78 != 0) ? "on" : "off";
		verbose_state = (DAT_00078e08 != 0) ? "on" : "off";
		rpc_state = (DAT_0007b380 != 0) ? "on" : "off";
		worktime_state = (DAT_0007b85a != 0) ? "on" : "off";
		compact_state = (DAT_0007b940 != 0) ? "on" : "off";
		switching_state = (DAT_00077155 != 0) ? "enabled" : "disabled";
		widescreen_state = (DAT_00077318 != 0) ? "enabled" : "disabled";
		snprintf(buf, 0x1000,
			 "[D]ebug:%s\n[P]er-device:%s\n[Q]uiet:%s\n[V]erbose:%s\n[R]PC debug:%s\n[W]orkTime details:%s\nco[M]pact: %s\n[T]oggle status switching:%s\nw[I]descreen:%s\n[Z]ero statistics\n[L]og interval:%d\n",
			 debug_state, perdev_state, quiet_state, verbose_state,
			 rpc_state, worktime_state, compact_state, switching_state,
			 widescreen_state, DAT_00075dcc);
		_wlogprint(buf);
		_wlogprint("Select an option or any other key to return\n");
		logwin_update();
		ch = (char)wgetch(stdscr);

		if (strncasecmp(&ch, "q", 1) == 0) {
			DAT_0007bc78 ^= 1;
			snprintf(buf, 0x1000, "Quiet mode %s\n", (DAT_0007bc78 != 0) ? "enabled" : "disabled");
			_wlogprint(buf);
			continue;
		}
		if (strncasecmp(&ch, "v", 1) == 0) {
			DAT_00078e08 ^= 1;
			if (DAT_00078e08 != 0)
				DAT_0007bc78 = 0;        /* verbose 打开联动关 quiet */
			snprintf(buf, 0x1000, "Verbose mode %s\n", (DAT_00078e08 != 0) ? "enabled" : "disabled");
			_wlogprint(buf);
			continue;
		}
		if (strncasecmp(&ch, "n", 1) == 0) {
			/* 全部输出模式复位为普通模式 */
			DAT_00078e09 = 0;
			DAT_00078e08 = 0;
			DAT_0007bc78 = 0;
			DAT_0007b380 = 0;
			DAT_0007b940 = 0;
			DAT_0007b858 = 0;
			_wlogprint("Output mode reset to normal\n");
			if (curses_active_locked() != 0)
				switch_logsize();
			continue;
		}
		if (strncasecmp(&ch, "d", 1) == 0) {
			DAT_00078e09 ^= 1;
			if (DAT_00078e09 != 0)
				DAT_0007bc78 = 0;        /* debug 打开联动关 quiet */
			DAT_00078e08 = DAT_00078e09; /* debug 联动 verbose */
			snprintf(buf, 0x1000, "Debug mode %s\n", (DAT_00078e09 != 0) ? "enabled" : "disabled");
			_wlogprint(buf);
			continue;
		}
		if (strncasecmp(&ch, "m", 1) == 0) {
			DAT_0007b940 ^= 1;
			snprintf(buf, 0x1000, "Compact mode %s\n", (DAT_0007b940 != 0) ? "enabled" : "disabled");
			_wlogprint(buf);
			if (curses_active_locked() != 0)
				switch_logsize();
			continue;
		}
		if (strncasecmp(&ch, "p", 1) == 0) {
			DAT_0007b858 ^= 1;
			DAT_00078e08 = DAT_0007b858; /* per-device 联动 verbose */
			snprintf(buf, 0x1000, "Per-device stats %s\n", (DAT_0007b858 != 0) ? "enabled" : "disabled");
			_wlogprint(buf);
			continue;
		}
		if (strncasecmp(&ch, "r", 1) == 0) {
			DAT_0007b380 ^= 1;
			if (DAT_0007b380 != 0)
				DAT_0007bc78 = 0;        /* RPC debug 打开联动关 quiet */
			snprintf(buf, 0x1000, "RPC protocol debugging %s\n", (DAT_0007b380 != 0) ? "enabled" : "disabled");
			_wlogprint(buf);
			continue;
		}
		if (strncasecmp(&ch, "c", 1) == 0)
			goto out_clear;              /* [C]lear: 清屏后退出菜单 */
		if (strncasecmp(&ch, "l", 1) == 0) {
			loginterval = (unsigned int)curses_int("Interval in seconds");
			if (loginterval < 10000) {
				DAT_00075dcc = (int)loginterval;
				snprintf(buf, 0x1000, "Log interval set to %d seconds\n", (int)loginterval);
				_wlogprint(buf);
			} else {
				_wlogprint("Invalid selection\n");
			}
			continue;
		}
		if (strncasecmp(&ch, "s", 1) == 0) {
			DAT_0007bb18 = 1;            /* 静默模式: 置退出标志, 不清屏直接退出 */
			goto out_close;
		}
		if (strncasecmp(&ch, "w", 1) == 0) {
			DAT_0007b85a ^= 1;
			snprintf(buf, 0x1000, "WorkTime details %s\n", (DAT_0007b85a != 0) ? "enabled" : "disabled");
			_wlogprint(buf);
			continue;
		}
		if (strncasecmp(&ch, "t", 1) == 0) {
			DAT_00077155 ^= 1;           /* 状态栏切换开关 (无回显) */
			continue;
		}
		if (strncasecmp(&ch, "i", 1) == 0) {
			DAT_00077318 ^= 1;           /* 宽屏开关 (无回显) */
			continue;
		}
		if (strncasecmp(&ch, "z", 1) != 0)
			goto out_clear;              /* 未知按键: 清屏退出菜单 */
		zero_stats();                    /* [Z]ero statistics */
		continue;

out_clear:                               /* 原 LAB_000266d8 */
		clear_logwin();
out_close:                               /* 原 LAB_000266dc */
		immedok(DAT_0007b430, 0);
		DAT_0007bac8 = 0;
		return;
	}
}

// === my_log_curses_lock === 原 FUN_000388e4 @ 0x000388e4
// 来源: reversed/cgminer/41_logging/FUN_000388e4.c | 分类: 41_logging
// 语义: 加 curses 互斥锁 DAT_0007b7a8; 失败时拼 "WTF MUTEX ERROR ON LOCK!"
//       串 (errno / logging.c / my_log_curses / 固定行号 0x2f=47), 经
//       applog(LOG_ERR, 强制刷新) 上报后 quit(1) 退出进程。
void my_log_curses_lock(void)
{
	char buf[4096];

	if (pthread_mutex_lock(&DAT_0007b7a8) != 0) {
		snprintf(buf, 0x1000, "WTF MUTEX ERROR ON LOCK! errno=%d in %s %s():%d",
			 errno, "logging.c", "my_log_curses", 0x2f);
		applog(LOG_ERR, buf, 1);
		quit(1);                         /* 不返回 */
	}
}

// === my_log_curses_unlock === 原 FUN_0003896c @ 0x0003896c
// 来源: reversed/cgminer/41_logging/FUN_0003896c.c | 分类: 41_logging
// 语义: 解 curses 互斥锁 DAT_0007b7a8; 失败时拼 "WTF MUTEX ERROR ON UNLOCK!"
//       串 (errno / logging.c / my_log_curses / 行号取自参数 — 调用点以各自
//       __LINE__ 传入), 经 applog(LOG_ERR, 强制刷新) 上报后 quit(1) 退出进程。
void my_log_curses_unlock(int line)
{
	char buf[4096];

	if (pthread_mutex_unlock(&DAT_0007b7a8) != 0) {
		snprintf(buf, 0x1000, "WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d",
			 errno, "logging.c", "my_log_curses", line);
		applog(LOG_ERR, buf, 1);
		quit(1);                         /* 不返回 */
	}
}

/* ============================================================================
 * 覆盖率表 — batch log_b1: cgminer/log.c (9/9 已转写)
 * ---------------------------------------------------------------------------
 * 语义名                | 原地址    | 来源 (reversed/cgminer/41_logging/) | 源行数
 * ----------------------+-----------+-------------------------------------+-------
 * set_sharelog          | 0000fa18  | FUN_0000fa18.c                      | 98
 * switch_logsize        | 00018178  | FUN_00018178.c                      | 24
 * clear_logwin          | 00018ad8  | FUN_00018ad8.c                      | 21
 * logwin_update         | 00018b44  | FUN_00018b44.c                      | 27
 * sharelog              | 0001b600  | FUN_0001b600.c                      | 80
 * curses_int            | 000260d0  | FUN_000260d0.c                      | 14
 * display_options       | 000260fc  | FUN_000260fc.c                      | 390
 * my_log_curses_lock    | 000388e4  | FUN_000388e4.c                      | 20
 * my_log_curses_unlock  | 0003896c  | FUN_0003896c.c                      | 20
 * ============================================================================ */
