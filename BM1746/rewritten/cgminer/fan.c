/**
 * fan.c — cgminer 温度/风扇 PWM 层 (分类 14_temp_fan)
 *
 * 转写来源: reversed/cgminer/14_temp_fan/ (Ghidra 反编译产物)
 * 约定: rewritten/CONVENTIONS.md
 * 命名: rewritten/name_map/cgminer.tsv
 *
 * 硬件背景 (bm1746/BM1746_uart_register_ops.md + topol.conf):
 *   - 风扇 PID: fan_control_mode 2, PWM 20%~100%, full_speed_temp 75,
 *     target_temp 70, startup_Kp -800, Kp -100, Ki -75, Kd 0
 *   - 双风扇: max 7100 rpm / min 750 rpm
 *   - 温度上限: 芯片 95°C / PCB 80°C / 下限 -20°C
 *   - TMP451 传感器 ×6, 经 ASIC reg 0x20 (I2C 间接寄存器) 访问
 *
 * 本文件 18 个函数 (batch=fan_b1), 按转写顺序:
 *   配置/框架:  set_temp_cutoffs, unlock_curses, restart_mine
 *   API 包装:   add_uint8_buf, add_short_buf, add_ushort_buf, api_stats
 *   监控 LED:   toggle_red_green_led
 *   风扇核心:   check_fan_number, set_fan_pwm, convert_temperature_to_pwm,
 *               set_PWM_according_to_temperature
 *   温度读取:   enable_read_temperature_from_asic_chain, write_i2c_reg,
 *               set_temperature_offset_value_chain_sensor
 *   监控线程:   check_miner_status
 *   线程创建:   create_bitmain_check_fan_pthread, create_bitmain_read_temp_pthread
 */

#include "common_types.h"
#include "bm1746_regs.h"

#include <stdio.h>
#include <stdlib.h>   /* strtol / system / calloc — 原调用保留 */
#include <string.h>   /* memset — 原调用保留 */
#include <unistd.h>   /* execv — 原调用保留 */
#include <signal.h>   /* kill — 原调用保留 */
#include <pthread.h>
#include <sys/time.h> /* struct timeval (cgtime 参数) */

/* ============================================================
 * 本文件常量
 * ============================================================ */

/* cgminer applog 日志级别 (本固件枚举整体后移, 依据 i2c.c/uart.c 已核实调用点:
 * LOG_DEBUG=7 需 日志级别>6; 3=错误 4=警告 5=通知 6=信息 7=调试) */
#define LOG_ERR     3
#define LOG_WARNING 4
#define LOG_NOTICE  5
#define LOG_INFO    6
#define LOG_DEBUG   7

/* applog 调用点门控 (同 uart.c LOG_GATE 宏): 级别 p 显示条件
 * = 门A(DAT_0007ad48) || 门B(DAT_00078e08) || (p-1) < DAT_000766c4 */
#define LOG_GATE3() (DAT_0007ad48 || DAT_00078e08 || 2 < DAT_000766c4)
#define LOG_GATE4() (DAT_0007ad48 || DAT_00078e08 || 3 < DAT_000766c4)
#define LOG_GATE5() (DAT_0007ad48 || DAT_00078e08 || 4 < DAT_000766c4)
#define LOG_GATE6() (DAT_0007ad48 || DAT_00078e08 || 5 < DAT_000766c4)
#define LOG_GATE7() (DAT_0007ad48 || DAT_00078e08 || 6 < DAT_000766c4)

/* toggle_red_green_led GPIO 编号 (0x3ad/0x3ae, 命名阶段经 monitor-recobtn 文档核实) */
#define MONITOR_LED_GPIO_RED    941   /* GPIO941 */
#define MONITOR_LED_GPIO_GREEN  942   /* GPIO942 */

/* check_miner_status 温度关机阈值 (0x50/0x6e, 日志字符串 "higher than %d/%d'C") */
#define TEMP_PCB_SHUTDOWN_C    80     /* 0x50 PCB 温度告警阈值 */
#define TEMP_CHIP_SHUTDOWN_C   110    /* 0x6e 芯片温度告警阈值 */

/* check_miner_status 风扇转速下限 (满速封顶判定) */
#define FAN1_FULL_PWM_MIN_RPM  4805   /* 0x12bf: PWM=100% 时 fan1 仍低于此值判异常 */
#define FAN2_FULL_PWM_MIN_RPM  3440   /* 0xd70:  PWM=100% 时 fan2 仍低于此值判异常 */
#define FAN_ERROR_COUNT_LIMIT  20     /* 0x14: 连续错误次数超过后停 PIC */
#define INTERNET_LOST_SEC      121    /* 0x79: 断网秒数阈值 */
#define ASIC_STATUS_INTERVAL_S 120    /* 0x78: ASIC 状态字符串刷新周期 */

/* ============================================================
 * 跨批次被调 (名称取自 name_map/cgminer.tsv)
 * ============================================================ */
extern void     applog(uint32_t prio, char *buf, ...);      /* 原 FUN_00038730, 第 3 参调用点恒传 0 */
extern void     mutex_unlock_fail_cgminer(const char *fn, int line); /* 原 FUN_0000b5c4 */
extern void     cgtime(struct timeval *tv);                          /* 原 FUN_0002a84c */
extern void     copy_timeval(void *dst, const void *src);            /* 原 FUN_0002a530: 8 字节 timeval 拷贝 (dst<-src) */
extern uint32_t cgsleep_ms(uint32_t ms);                             /* 原 FUN_0002a92c (本文件两处 pthread 创建函数读取其 r0 作返回值) */
extern void     cg_completion_timeout(void (*fn)(void *), void *arg, uint32_t timeout_ms); /* 原 FUN_0002f438 */
extern int      thr_info_create(void *thr, void *attr, void *(*fn)(void *), void *arg);    /* 原 FUN_0002f174 (+util.c): pthread_create(&thr->pth,...) */
extern void    *api_add_data_full(void *root, char *name, int type, void *data, int copied); /* 原 FUN_00031cdc, 返回链表 root */
extern void    *api_add_int(void *root, char *name, int *data, int copied);      /* 原 FUN_00032ed0 (type=6) */
extern void    *api_add_uint32(void *root, char *name, uint32_t *data, int copied); /* 原 FUN_00032f18 (type=8) */
extern void    *api_add_string(void *root, char *name, char *data, int copied);  /* 原 FUN_00032e1c (type=1) */
extern void    *add_percent_buf(void *root, char *name, double *data, int copied); /* 原 FUN_000357a4 */
extern void     makeup_cmd_frame(void *frame, uint8_t chip_addr, uint8_t chain_byte,
                                 uint8_t reg, uint32_t value);       /* 原 FUN_0003cdcc, 11 字节命令帧 */
extern uint32_t uart_send(int chain, const void *buf, uint32_t len); /* 原 FUN_0003c150 */
extern uint32_t read_i2c_reg(uint32_t chain, uint32_t group, uint8_t i2c_reg, uint8_t type); /* 原 FUN_0004886c */
extern void     disable_PIC16F1704_dc_dc_new(void);                  /* 原 FUN_0003fc24: 关闭 DAT_0007909c 指定链的 PIC DC-DC */
extern void    *check_fan(void *arg);                               /* 原 FUN_0003aa9c [低置信]: check fan 线程主体 */
extern void    *fun_00048a18(void *arg);                             /* 原 FUN_00048a18 命名待补: read temp 线程主体 */
extern void     fun_0001b4c0(void *arg);                             /* 原 0x0001b4c0 命名待补: restart_mine 超时收尾回调 */
extern int64_t  __fixdfdi(double v);                                 /* 原 FUN_0005cc10: double→int64 */
extern double   __floatdidf(int64_t v);                              /* 原 FUN_0005cb64: int64→double */
extern uint32_t __udivsi3(uint32_t a, uint32_t b);                   /* 原 FUN_0005c648: 无符号除法 */

/* ============================================================
 * 本模块引用的全局 (DAT_ 保留原名, 语义见注释)
 * ============================================================ */
/* ---- applog 门控三元组 (同 uart.c/i2c.c 已核实) ---- */
extern char DAT_0007ad48;   /* 门 A: 非 0 强制输出 (兼 syslog 开关) */
extern char DAT_00078e08;   /* 门 B: 冗余日志开关 */
extern int  DAT_000766c4;   /* 日志级别门 */
extern char DAT_00078e09;   /* opt_debug */
extern char DAT_00078e28;   /* 全局: api_stats/check_miner_status 联动开关 (语义待定) */
extern char DAT_00078e3c;   /* 全局: PIC 已停发标志 (置 1 后抑制温度/风扇告警日志) */

/* ---- restart_mine / unlock_curses ---- */
extern char *DAT_00076f00;      /* 推断: set_temp_cutoffs 的配置串存储槽 [低置信] */
extern char DAT_00076bf4[];     /* progname 程序名字符串 */
extern char DAT_00077148[];     /* timeval 全局 (cgtime 写入), 语义待定 */
extern int  DAT_0007728c;       /* 子进程 pid (restart_mine 终止) */
extern char **DAT_00077290;     /* execv argv 指针 */
extern char DAT_0007b7a8[];     /* curses 互斥锁 (pthread_mutex_t) */
extern char DAT_00077018;       /* curses_active 标志 */
extern int  DAT_00075dd0;       /* 全局, 语义待定 (unlock_curses 清 0) */
extern void *DAT_0007b430;      /* ncurses WINDOW* (主窗口) */
extern void *DAT_0007b7c0;      /* ncurses WINDOW* */
extern void *DAT_0007bc90;      /* ncurses WINDOW* */
extern void (*DAT_00075ed8)(void); /* 全局函数指针: ELF 初值 noop_hook (FUN_00028ae0, 恒 0) [低置信] */
extern int  DAT_00079120;       /* 风扇连续错误计数 (>20 触发停 PIC 并置 DAT_00079125) */

/* ---- ncurses 终端接口 (unlock_curses 原调用保留; 以 void* 代 WINDOW*) ---- */
extern int leaveok(void *win, int bf);
extern int nocbreak(void);
extern int echo(void);
extern int delwin(void *win);
extern int endwin(void);
extern int kill(int pid, int sig); /* POSIX (Linux 头文件已有同型原型, 兼容) */

/* ---- LED / 风扇 / 温度共享统计区 (0x9d4b4 起, 与 api_stats / RX 线程共用) ---- */
extern char DAT_00076b68;       /* 红绿灯交替状态 (toggle_red_green_led 翻转) */
extern uint8_t *DAT_00078e10;   /* FPGA AXI 寄存器窗口 mmap 基址 (uint8_t* 全局指针变量) */
extern uint8_t DAT_0009d4b5[];  /* 统计区基址 1: 频率 ushort 在 +0x250 起 (api "frequency%d") */
extern char DAT_0009d4bb[];     /* 统计区基址 2: +1 (0x9d4bc) 起为链存在标志数组 */
extern uint8_t DAT_0009d69d[];  /* 每 fan RPM 数组基址: uint32 ×6, 步进 4 (api "fan%d") */
extern uint8_t DAT_0009d67f[];  /* 每 fan 在位标志数组基址 (字节, 步进 1) */
extern uint8_t DAT_0009d5b7[];  /* 每 ASIC nonce 计数数组基址: int32 ×12, 步进 4 (chain×3) */
extern uint8_t DAT_0009d5e7[];  /* 每 ASIC nonce 增量数组基址: int32 ×12 (与 0x9d5b7 平行) */
extern uint8_t DAT_0009d5a7[];  /* api "chain_hw%d" 基址: uint32 ×4, 步进 4 */
extern char DAT_0009d617[];     /* api "chain_acs%d" 状态串基址: 每 chain 19 字节 ('o'=正常 'x'=停转 ' '=分隔) */
extern uint8_t DAT_0009d6c5[];  /* 每 chain ASIC 数 (api "chain_acn%d"; 0x9d4bb+0x20a 别名) */
extern uint32_t DAT_0009d4c8;   /* 风扇在位位图 (bit n = 第 n 号风扇存在) */
extern uint8_t DAT_0009d6cb;    /* api "miner_count" / "temp_num" (同址复用, 原始即如此) */
extern uint8_t DAT_0009d6cc;    /* 检测到的风扇数量 (api "fan_num") */
extern uint16_t DAT_0009d6ce;   /* 风扇最大转速 (rpm) */
extern uint16_t DAT_0009d6d2;   /* 风扇最小转速 (rpm) */
extern int32_t DAT_0009d6d6;    /* 温度 max (api "temp_max"; check_miner_status 日志按 pcb:%d 输出) [低置信] */
extern int32_t DAT_0009d6da;    /* 第二路温度 max (check_miner_status 日志按 chip:%d 输出) [低置信] */
extern uint8_t DAT_0009d6f7;    /* 全局, 语义待定: 0 时允许自动 PWM 调整 */
extern uint8_t DAT_0009d6f8;    /* 当前风扇 PWM 百分比 (低字节; 100=满速封顶) */
extern uint8_t DAT_0009d6ca;    /* 最近一次 set_fan_pwm 写入的 PWM 百分比 */
extern uint32_t DAT_00079126;   /* 全局, 语义待定 (0x78e10+0x316): 非 0 时 set_fan_pwm 强制 100% 满速字 */
extern uint32_t DAT_00079128;   /* (0x78e10+0x318): set_PWM_according_to_temperature 记录的当前温度 */
extern uint32_t DAT_0007912c;   /* (0x78e10+0x31c): 上次重算 PWM 时的温度 (变化 >1°C 才重算) */
extern uint32_t DAT_00076b5c;   /* 温度使能命令累积值: 写 reg 0x1C 的 0x4060 位累积 (BM1746_TEMP_ENABLE_1C) */
extern uint32_t DAT_00076b6c;   /* 全局: reg 0x20 I2C 间接值拼装高 8 位来源 (i2c.c 同源) */
extern uint8_t DAT_00079130[];  /* 组→链地址字节表 (0x79130..0x79132; 本文件用 [0]/[1]) */

/* ---- api_stats 统计全局 ---- */
extern int DAT_0007b100;        /* 统计求和低 32 位 (与 DAT_0007b104 组成 int64) */
extern int DAT_0007b104;        /* 统计求和高 32 位 */
extern int DAT_0007b8a4;        /* no_matching_work 计数 (api "no_matching_work") */
extern double DAT_0007b5e8;     /* 全局, 语义待定: 统计求和分量 1 (double) */
extern double DAT_0007b670;     /* 全局, 语义待定: 统计求和分量 2 (double) */
extern double DAT_0007b428;     /* 全局, 语义待定: 统计求和分量 3 (double) */
extern double DAT_00078e20;     /* 设备硬件错误率 (api "Device Hardware%", double) */
extern char DAT_0009d741[];     /* api "temp%d" 数据基址: 2 字节值 ×4, 步进 4 */
extern char DAT_0009d751[];     /* api "temp2_%d" 数据基址: 2 字节值 ×4, 步进 4 */
extern char DAT_0007c0a8[];     /* api "chain_rate%d" 字符串基址: 4 ×16 字节 */
extern double DAT_00078e30;     /* 全局, 语义待定: chain_rate 计算分子 (原始实现对 4 条 chain 输出同一值) */
extern int DAT_00078e38;        /* 全局, 语义待定: chain_rate 计算分母 (int) */

/* ---- check_miner_status ---- */
extern int DAT_000790f4;        /* 最近有网时间 timeval.tv_sec (与 DAT_000790f8 成对) */
extern int DAT_000790f8;        /* 最近有网时间 timeval.tv_usec */
extern char DAT_00079100[];     /* ASIC 状态刷新通知互斥锁 (pthread_mutex_t) */
extern char DAT_0007c078[];     /* ASIC 状态刷新条件变量 (pthread_cond_t) */
extern uint32_t DAT_0007c134[12];  /* nonce 增量快照 (原 0x9d5e7 区, 12 字) */
extern uint32_t DAT_0007c164[12];  /* nonce 累计快照 (原 0x9d5b7 区, 12 字) */
extern uint32_t DAT_0007c194[12];  /* 每 ASIC 停转标志 (1 = 'x' 停转) */
extern uint8_t DAT_0009d4b7[];  /* nonce 计数清零别名基址 (0x9d4b7+4k+1 = 0x9d5b8+4k) */
extern int DAT_0007911c;        /* 高温连续计数 */
extern char DAT_0007911d;       /* 温度过高停机标志 */
extern char DAT_0007911e;       /* 断网标志 */
extern char DAT_00079124;       /* 全局, 语义待定 (参与恢复判定) */
extern char DAT_00079125;       /* 风扇持续异常标志 (>20 次) */
extern char DAT_00079127;       /* PIC 已关标志 (一次性触发) */
extern char DAT_000790f0;       /* LED 状态输出 (传给 toggle_red_green_led) */
extern char DAT_000790d0[];     /* send/read_data 互斥锁 (pthread_mutex_t, uart.c 同源) */
extern int DAT_0007909c;        /* 当前链号 (disable_PIC16F1704_dc_dc_new 参数) */

/* ---- 线程创建 ---- */
extern void *DAT_0007c284;      /* check fan 线程 thr_info 指针 (calloc 0x40) */
extern void *DAT_0007c12c;      /* read temp 线程 thr_info 指针 (calloc 0x40) */

/* ============================================================
 * 函数转写区
 * ============================================================ */

// === set_temp_cutoffs === 原 FUN_000155f0 @ 0x000155f0
// 来源: reversed/cgminer/14_temp_fan/FUN_000155f0.c | 分类: 14_temp_fan
// 语义: 解析 "--temp-cutoff" 类配置串, 仅接受 0~200 (0xc9) 的十进制值并保存字符串指针;
//       参数空/过小返回错误串, 合法时存 DAT_00076f00 并返回 NULL (cgminer 配置回调约定)。
char *set_temp_cutoffs(char *param_1)
{
	if (param_1 == NULL || *param_1 == '\0') {
		return "Invalid parameters for set temp cutoff";
	}
	uint32_t val = (uint32_t)strtol(param_1, (char **)NULL, 10); /* 原始按 uint32 比较 */
	if (val < 0xc9) { /* 201: 温度截止上限 */
		DAT_00076f00 = param_1;
		return NULL;
	}
	return "Invalid value passed to set temp cutoff";
}

// === unlock_curses === 原 FUN_00019adc @ 0x00019adc
// 来源: reversed/cgminer/14_temp_fan/FUN_00019adc.c | 分类: 14_temp_fan
// 语义: 释放 ncurses 终端 — 若 curses 激活则清激活标志、恢复终端模式并销毁 3 个窗口;
//       pthread_mutex_unlock 失败时上报 mutex_unlock_fail_cgminer("unlock_curses", 行 2930);
//       尾部调用全局函数指针 DAT_00075ed8 (ELF 初值 noop_hook, 恒空操作)。
//       注: 原始反编译为 pthread_mutex_trylock (已用 PLT 重定位核实), 非常规的 lock。
void unlock_curses(void)
{
	int unlock_ret;

	pthread_mutex_trylock((pthread_mutex_t *)DAT_0007b7a8);
	if (DAT_00077018 == '\0') {
		unlock_ret = pthread_mutex_unlock((pthread_mutex_t *)DAT_0007b7a8);
	}
	else {
		DAT_00075dd0 = 0;
		DAT_00077018 = '\0';
		leaveok(DAT_0007b430, 0);
		leaveok(DAT_0007b7c0, 0);
		leaveok(DAT_0007bc90, 0);
		nocbreak();
		echo();
		delwin(DAT_0007b430);
		delwin(DAT_0007b7c0);
		delwin(DAT_0007bc90);
		endwin();
		unlock_ret = pthread_mutex_unlock((pthread_mutex_t *)DAT_0007b7a8);
	}
	if (unlock_ret != 0) {
		mutex_unlock_fail_cgminer("unlock_curses", 0xb72);
	}
	(*DAT_00075ed8)();
	return;
}

// === restart_mine === 原 FUN_0001d7c8 @ 0x0001d7c8
// 来源: reversed/cgminer/14_temp_fan/FUN_0001d7c8.c | 分类: 14_temp_fan
// 语义: 自我重启 — 打日志、给 5s 超时收尾、记录重启时刻、退出 curses、杀子进程后
//       execv 重启自身; execv 失败则打印 "Failed to restart application" 并返回。
void restart_mine(void)
{
	char buf[4100]; /* 原栈日志缓冲 */

	if (LOG_GATE4()) {
		snprintf(buf, 0x1000, "Attempting to restart %s", DAT_00076bf4);
		applog(LOG_WARNING, buf, 0);
	}
	cg_completion_timeout(fun_0001b4c0, NULL, 5000);
	cgtime((struct timeval *)&DAT_00077148);
	unlock_curses();
	if (0 < DAT_0007728c) {
		kill(DAT_0007728c, 15 /* SIGTERM */);
		DAT_0007728c = 0;
	}
	execv(*DAT_00077290, DAT_00077290);
	/* execv 失败才会到达此处 (原始代码逐字节构造该字符串) */
	if (LOG_GATE4()) {
		applog(LOG_WARNING, "Failed to restart application", 0);
	}
	return;
}

// === add_uint8_buf === 原 FUN_00032e64 @ 0x00032e64
// 来源: reversed/cgminer/14_temp_fan/FUN_00032e64.c | 分类: 14_temp_fan
// 语义: api_add_data_full 包装 — 以类型 3 (API_UINT8, 拷贝 1 字节) 挂接 api 数据节点,
//       尾调透传返回链表 root (api_stats 串联使用)。
void *add_uint8_buf(void *root, char *name, uint8_t *data, int copied)
{
	return api_add_data_full(root, name, 3 /* API_UINT8: 1 字节 */, data, copied);
}

// === add_short_buf === 原 FUN_00032e88 @ 0x00032e88
// 来源: reversed/cgminer/14_temp_fan/FUN_00032e88.c | 分类: 14_temp_fan
// 语义: api_add_data_full 包装 — 以类型 4 (2 字节有符号 short) 挂接 api 数据节点。
void *add_short_buf(void *root, char *name, int16_t *data, int copied)
{
	return api_add_data_full(root, name, 4 /* short: 2 字节 */, data, copied);
}

// === add_ushort_buf === 原 FUN_00032eac @ 0x00032eac
// 来源: reversed/cgminer/14_temp_fan/FUN_00032eac.c | 分类: 14_temp_fan
// 语义: api_add_data_full 包装 — 以类型 5 (2 字节无符号 ushort) 挂接 api 数据节点。
void *add_ushort_buf(void *root, char *name, uint16_t *data, int copied)
{
	return api_add_data_full(root, name, 5 /* ushort: 2 字节 */, data, copied);
}

// === toggle_red_green_led === 原 FUN_00044ac0 @ 0x00044ac0
// 来源: reversed/cgminer/14_temp_fan/FUN_00044ac0.c | 分类: 14_temp_fan
// 语义: 监控红绿灯切换 (GPIO941/942, 经 sysfs) — 先翻转 DAT_00076b68 交替位;
//       param_1==0 (正常) 时红灯灭、绿灯输出交替位; 否则 (异常) 绿灯灭、红灯输出交替位。
//       param_1 由 check_miner_status 传入 DAT_000790f0。
void toggle_red_green_led(int param_1)
{
	char cmd[100]; /* 原栈缓冲 100 字节 */

	DAT_00076b68 = DAT_00076b68 ^ 1;
	if (param_1 == 0) {
		sprintf(cmd, "echo %d > /sys/class/gpio/gpio%d/value", 0, MONITOR_LED_GPIO_RED);
		system(cmd);
		sprintf(cmd, "echo %d > /sys/class/gpio/gpio%d/value", (uint32_t)DAT_00076b68, MONITOR_LED_GPIO_GREEN);
		system(cmd);
	}
	else {
		sprintf(cmd, "echo %d > /sys/class/gpio/gpio%d/value", 0, MONITOR_LED_GPIO_GREEN);
		system(cmd);
		sprintf(cmd, "echo %d > /sys/class/gpio/gpio%d/value", (uint32_t)DAT_00076b68, MONITOR_LED_GPIO_RED);
		system(cmd);
	}
	return;
}

/* ---- AXI 窗口内风扇相关偏移 (DAT_00078e10 内容 + 偏移) ---- */
#define AXI_OFF_FAN_STATUS   0x04   /* 风扇转速采样寄存器: bit[7:0]=转速低字节×120=rpm, bit[10:8]=风扇号 */
#define AXI_OFF_FAN_PWM0     0x84   /* PWM 寄存器 0: 高半字=fan0 占空比, 低半字=100-占空比 */
#define AXI_OFF_FAN_PWM1     0xa0   /* PWM 寄存器 1 (与 0x84 写同一值) */

#define FAN_PWM_REG_MIN      0x28   /* set_fan_pwm 钳位下限 (40) */
#define FAN_PWM_REG_MAX      100    /* set_fan_pwm 钳位上限 */
#define FAN_PWM_FULL_WORD    0x640000u  /* 满速字: 高半字=100, 低半字=0 */
#define FAN_SAMPLE_RETRY_MAX 5      /* check_fan_number: 无效采样重试计数上限 */
#define FAN_RETRY_SLEEP_MS   50     /* 0x32: 采样间隔 */
#define FAN_RETRY_ROUND_MS   1000   /* 整轮重查间隔 */

// === check_fan_number === 原 FUN_00045b50 @ 0x00045b50
// 来源: reversed/cgminer/14_temp_fan/FUN_00045b50.c | 分类: 14_temp_fan
// 语义: 探测风扇数量与转速 — 轮询 AXI base+4 转速寄存器 (0xffffffff=无效):
//       bit[7:0]×120=rpm, bit[10:8]=风扇号; 维护每 fan RPM 表 (DAT_0009d69d)、
//       在位标志 (DAT_0009d67f)、位图 (DAT_0009d4c8) 与计数 (DAT_0009d6cc),
//       并统计最大/最小转速 (DAT_0009d6ce/9d6d2)。≥2 台风扇时打印 fan-num 日志返回;
//       采样 6 次仍 <2 台则等待 1s 整轮重查。副作用: 更新上述全部统计全局。
void check_fan_number(void)
{
	char buf[4100]; /* 原栈日志缓冲 */
	uint32_t fan_raw;  /* 转速寄存器原始值 */
	uint32_t rpm;      /* 换算后转速 */
	uint32_t idx;      /* 风扇号 (bit[10:8], 0..7) */
	int retry = 0;

	DAT_0009d6ce = 0;
	DAT_0009d6d2 = 0;
	for (;;) {
		for (;;) { /* 采样循环 */
			if (FAN_SAMPLE_RETRY_MAX < retry) {
				if (1 < DAT_0009d6cc) {
					if (LOG_GATE5()) {
						snprintf(buf, 0x1000, "fan-num %d fan-map %d\n",
						         (uint32_t)DAT_0009d6cc, DAT_0009d4c8);
						applog(LOG_NOTICE, buf, 0);
					}
					return;
				}
				if (LOG_GATE5()) {
					snprintf(buf, 0x1000, "No %d Fan find, check again", 2);
					applog(LOG_NOTICE, buf, 0);
				}
				retry = 0;
				cgsleep_ms(FAN_RETRY_ROUND_MS);
			}
			fan_raw = *(uint32_t *)(DAT_00078e10 + AXI_OFF_FAN_STATUS);
			if (fan_raw != 0xffffffffu)
				break;
			retry++; /* 原 0x45cfc 重试尾 */
			cgsleep_ms(FAN_RETRY_SLEEP_MS);
		}
		idx = (fan_raw & 0x7ffu) >> 8;             /* 风扇号 */
		rpm = (uint32_t)(fan_raw & 0xffu) * 0x78u; /* rpm = 低字节 × 120 */
		*(uint32_t *)&DAT_0009d69d[idx * 4] = rpm;
		if ((fan_raw & 0xffu) == 0) {
			/* 转速为 0: 若该风扇原先在位则摘除 */
			if (DAT_0009d67f[idx] == 1) {
				DAT_0009d67f[idx] = (uint8_t)fan_raw; /* 即 0 (转速低字节) */
				if ((1 << idx) == 0) { /* idx∈0..7 恒假; 原始代码即如此 (0x45d20 lsls 标志位) */
					DAT_0009d4c8 = DAT_0009d4c8 & 1;
				}
				else {
					DAT_0009d4c8 = 0;
				}
				DAT_0009d6cc = DAT_0009d6cc - 1;
			}
		}
		else if (DAT_0009d67f[idx] == 0) {
			/* 新风扇出现: 置在位标志并入位图 */
			DAT_0009d67f[idx] = 1;
			DAT_0009d6cc = DAT_0009d6cc + 1;
			DAT_0009d4c8 = DAT_0009d4c8 | 1u << idx;
		}
		if ((uint32_t)DAT_0009d6ce < rpm) {
			DAT_0009d6ce = (uint16_t)rpm;
		}
		if (((rpm < (uint32_t)DAT_0009d6d2) && (rpm != 0)) || (DAT_0009d6d2 == 0)) {
			DAT_0009d6d2 = (uint16_t)rpm;
			goto next_sample; /* 原 0x45cfc 重试尾 */
		}
next_sample:
		retry++;
		cgsleep_ms(FAN_RETRY_SLEEP_MS);
	}
}

// === set_fan_pwm === 原 FUN_00045d88 @ 0x00045d88
// 来源: reversed/cgminer/14_temp_fan/FUN_00045d88.c | 分类: 14_temp_fan
// 语义: 写风扇 PWM 占空比 — 入参钳位到 [0x28,100]; DAT_00079126 非 0 时强制满速字;
//       正常字 = 占空比<<16 | (100-占空比) (双风扇字: 高半字=fan0, 低半字=fan1 反相);
//       同值写 AXI base+0x84 与 base+0xa0, 并把百分比记入 DAT_0009d6ca。
void set_fan_pwm(int param_1)
{
	uint32_t reg_val;
	uint8_t *axi = DAT_00078e10; /* mmap 基址内容 (原 r1 暂存) */

	if (param_1 < FAN_PWM_REG_MIN) {
		param_1 = FAN_PWM_REG_MIN;
	}
	else if (FAN_PWM_REG_MAX < param_1) {
		reg_val = FAN_PWM_FULL_WORD;
		DAT_0009d6ca = FAN_PWM_REG_MAX;
		goto write_reg;
	}
	if (DAT_00079126 == 0) {
		DAT_0009d6ca = (uint8_t)param_1;
		reg_val = (uint32_t)(FAN_PWM_REG_MAX - param_1) | (uint32_t)param_1 << 16;
	}
	else {
		reg_val = FAN_PWM_FULL_WORD;
		DAT_0009d6ca = FAN_PWM_REG_MAX;
	}
write_reg:
	*(uint32_t *)(axi + AXI_OFF_FAN_PWM0) = reg_val;
	*(uint32_t *)(axi + AXI_OFF_FAN_PWM1) = reg_val;
	return;
}

// === convert_temperature_to_pwm === 原 FUN_00045dec @ 0x00045dec
// 来源: reversed/cgminer/14_temp_fan/FUN_00045dec.c | 分类: 14_temp_fan
// 语义: 温度→PWM 百分比分段线性映射:
//       <35→40; 35..41→40+10/7×(t-35); 42..48→50+10/7×(t-42);
//       49..54→60+10/3×(t-49); 55..62→80+2.5×(t-55); >62→100。
float convert_temperature_to_pwm(int param_1)
{
	if ((uint32_t)(param_1 - 0x23) < 7u) { /* 35..41 */
		return (float)(param_1 - 0x23) * 1.4285715f + 40.0f;
	}
	if ((uint32_t)(param_1 - 0x2a) < 7u) { /* 42..48 */
		return (float)(param_1 - 0x2a) * 1.4285715f + 50.0f;
	}
	if ((uint32_t)(param_1 - 0x31) < 6u) { /* 49..54 */
		return (float)(param_1 - 0x31) * 3.3333333f + 60.0f;
	}
	if ((uint32_t)(param_1 - 0x37) < 8u) { /* 55..62 */
		return (float)(param_1 - 0x37) * 2.5f + 80.0f;
	}
	if (param_1 < 0x23) { /* <35: 下限 40% */
		return 40.0f;
	}
	/* >62: 满速 100% */
	if (0x3e < param_1) {
		return 100.0f;
	}
	return 0.0f; /* 不可达 (35..41..62 已被前述分支覆盖); 原始代码即如此 */
}

// === set_PWM_according_to_temperature === 原 FUN_00045ec0 @ 0x00045ec0
// 来源: reversed/cgminer/14_temp_fan/FUN_00045ec0.c | 分类: 14_temp_fan
// 语义: 按温度自动调风扇 PWM — 记录当前温度 (DAT_00079128); DAT_0009d6f7==0 或当前
//       PWM>100 时自动调节: 温度无效(0)/超 62→100% 满速, <36→40%, 否则温度较上次
//       重算点 (DAT_0007912c) 变化 >1°C 时按分段映射设 PWM (负值钳 0); 否则按既有
//       DAT_0009d6f8 重写。DAT_00079126 为强制满速开关 (AXI 0x316)。
void set_PWM_according_to_temperature(void)
{
	char buf[4096]; /* 原栈日志缓冲 */
	float f;
	uint32_t pwm;

	DAT_00079128 = DAT_0009d6d6;
	if ((DAT_0009d6f7 == 0) || (100 < (uint32_t)DAT_0009d6f8)) {
		if ((DAT_0009d6d6 == 0) || (0x3e < DAT_0009d6d6)) {
			set_fan_pwm(FAN_PWM_REG_MAX);
			DAT_0009d6f8 = FAN_PWM_REG_MAX;
		}
		else if (DAT_0009d6d6 < 0x24) {
			set_fan_pwm(FAN_PWM_REG_MIN);
			DAT_0009d6f8 = FAN_PWM_REG_MIN;
		}
		else if (2u < (uint32_t)DAT_0009d6d6 - DAT_0007912c + 1u) {
			f = convert_temperature_to_pwm(DAT_0009d6d6); /* 反汇编核实: r0=DAT_0009d6d6 */
			pwm = (uint32_t)((int32_t)f & ~((int32_t)f >> 31)); /* 向零取整并钳非负 */
			DAT_0009d6f8 = (uint8_t)pwm;
			if ((DAT_00078e09 != '\0') && LOG_GATE7()) {
				snprintf(buf, 0x1000, "%s: Set PWM percent : %d", __func__, pwm);
				applog(LOG_DEBUG, buf, 0);
			}
			set_fan_pwm((int)(pwm & 0xffu));
			DAT_0007912c = DAT_00079128;
		}
	}
	else {
		set_fan_pwm((int)DAT_0009d6f8);
	}
	return;
}

// === enable_read_temperature_from_asic_chain === 原 FUN_0004601c @ 0x0004601c
// 来源: reversed/cgminer/14_temp_fan/FUN_0004601c.c | 分类: 14_temp_fan
// 语义: 使能某链逐芯片温度读回 — 将 reg 0x1C 命令值累积 0x4060 (BM1746_TEMP_ENABLE_1C)
//       后, 若链在位则对 DAT_00079130[0..1] 各组以广播帧逐组写 reg 0x1C 并等 50ms。
void enable_read_temperature_from_asic_chain(uint32_t param_1)
{
	uint8_t frame[11] = {0}; /* 原 11 字节命令帧, 先清零 */
	uint32_t reg_val;
	uint32_t i;
	char buf[4100]; /* 原栈日志缓冲 */

	reg_val = DAT_00076b5c | BM1746_TEMP_ENABLE_1C;
	DAT_00076b5c = reg_val;
	if (DAT_0009d4bb[1 + param_1] == 1) { /* 链存在标志 (0x9d4bc 起) */
		if (LOG_GATE5()) {
			snprintf(buf, 0x1000, "%s: reg_value=0x%08x chainid %u",
			         __func__, reg_val, param_1);
			applog(LOG_NOTICE, buf, 0);
		}
		/* 原始: 指针自 &DAT_0007912c+3 起步先自增后取值, 终点 &DAT_00079132
		 * — 即遍历组→链地址字节 DAT_00079130[0]、DAT_00079130[1] */
		for (i = 0; i < 2; i++) {
			makeup_cmd_frame(frame, 0, DAT_00079130[i], BM1746_REG_BAUD_TEMP, reg_val);
			uart_send((int)(param_1 & 0xffu), frame, BM1746_FRAME_LEN);
			cgsleep_ms(FAN_RETRY_SLEEP_MS);
		}
	}
	return;
}

// === write_i2c_reg === 原 FUN_0004633c @ 0x0004633c
// 来源: reversed/cgminer/14_temp_fan/FUN_0004633c.c | 分类: 14_temp_fan
// 语义: 经 ASIC reg 0x20 (I2C 间接寄存器) 发起一次 I2C 写 — 32 位值拼装:
//       DAT_00076b6c 高 8 位 | 0x01000000 | (i2c_reg&0x7F)<<17 | (type&0xFF)<<8
//       | data | 0x10000 (写侧标记); 以 DAT_00079130[group] 为链字节构 11 字节帧
//       发出后等 50ms。
void write_i2c_reg(uint32_t param_1, int param_2, uint32_t param_3, uint32_t param_4, uint8_t param_5)
{
	uint8_t frame[11] = {0}; /* 原 11 字节命令帧区, 先清零 */

	makeup_cmd_frame(frame, 0, DAT_00079130[param_2], BM1746_REG_I2C_INDIRECT,
	                 (DAT_00076b6c & 0xff010000u) | BM1746_I2C_IND_FIXED
	                 | (param_3 & BM1746_I2C_REG_MASK) << BM1746_I2C_REG_SHIFT
	                 | (param_4 & 0xffu) << BM1746_I2C_TYPE_SHIFT
	                 | (uint32_t)param_5 | 0x10000u); /* bit16: 写事务标记 */
	uart_send((int)param_1, frame, BM1746_FRAME_LEN);
	cgsleep_ms(FAN_RETRY_SLEEP_MS);
	return;
}

// === set_temperature_offset_value_chain_sensor === 原 FUN_000490fc @ 0x000490fc
// 来源: reversed/cgminer/14_temp_fan/FUN_000490fc.c | 分类: 14_temp_fan
// 语义: 设置并回读校验 TMP451 温度偏移 — 槽位 = sensor + chain*4, 值取自 AXI 窗口
//       +0xdd4 配置区; 经 write_i2c_reg (I2C 子寄存器 0x4c, 通道 0x11) 写入, 等 50ms,
//       read_i2c_reg 发起回读后比较 +0xd84 读回区; 相等打成功日志, 否则打错误日志。
void set_temperature_offset_value_chain_sensor(int param_1, int param_2)
{
	char buf[4100]; /* 原栈日志缓冲 */
	int idx = param_2 + param_1 * 4; /* 每 (chain, sensor) 槽位, 步进 4 */
	int readback;
	int expected;

	if (LOG_GATE4()) {
		snprintf(buf, 0x1000, "Chain %d %s", param_1, __func__);
		applog(LOG_WARNING, buf, 0);
	}
	write_i2c_reg((uint32_t)param_1, param_2, 0x4c, 0x11,
	              DAT_00078e10[idx + 0xdd4]); /* +0xdd4: 温度偏移配置区 */
	cgsleep_ms(FAN_RETRY_SLEEP_MS);
	read_i2c_reg((uint32_t)param_1, (uint32_t)param_2, 0x4c, 0x11);
	readback = (int)(int8_t)DAT_00078e10[idx + 0xd84]; /* +0xd84: 读回数据区 */
	expected = (int)(int8_t)DAT_00078e10[idx + 0xdd4];
	if (readback == expected) {
		if (LOG_GATE5()) {
			snprintf(buf, 0x1000, "%s: Chain%d Sensor%d temp offset : %02d, ",
			         __func__, param_1, param_2, readback);
			applog(LOG_NOTICE, buf, 0);
		}
	}
	else if (LOG_GATE3()) {
		snprintf(buf, 0x1000,
		         "%s: Chain%d Sensor%d temp offset value set error. It should be %02d, but read back is %02d\n",
		         __func__, param_1, param_2, expected, readback);
		applog(LOG_ERR, buf, 0);
	}
	return;
}

// === check_miner_status === 原 FUN_00044b5c @ 0x00044b5c
// 来源: reversed/cgminer/14_temp_fan/FUN_00044b5c.c | 分类: 14_temp_fan
// 语义: 矿机状态监控线程主体 (create_bitmain_check_miner_status_pthread 创建), 常驻循环:
//   1. 每 ~120s (ASIC_STATUS_INTERVAL_S) 汇总每链每 ASIC nonce 计数, 计算平均/8 为门槛,
//      重刷 api "chain_acs%d" 状态串 ('o'=正常 'x'=停转, 每 8 片插一空格), 快照两块
//      12 字计数区到 DAT_0007c134/7c164, 通知条件变量后清零计数区;
//   2. 温度守门: temp_max(DAT_0009d6d6)≥81 或第二路(DAT_0009d6da)≥111 时累计告警,
//      连续 3 次置停机标志 DAT_0007911d;
//   3. 断网守门: 距 DAT_000790f4 (最近有网时刻) ≥121s 置 DAT_0007911e;
//   4. 风扇守门: 数量<2 / 转速低于 pwm×6000(4300)/130 / 满速(PWM=100) 仍低于
//      4805(3440) rpm 判错, 连续 >20 次置 DAT_00079125 并打 "Fan Err! Disable PIC!";
//   5. 五个异常标志 (DAT_00079124/11e/11d/25/26) 全清时恢复正常; 高温或风扇异常首次
//      触发时逐链停 PIC DC-DC; 最终以 DAT_000790f0 调 toggle_red_green_led 并睡 1s。
void check_miner_status(void)
{
	char buf[4100]; /* 原栈日志缓冲 */
	struct timeval tv_now;  /* 原栈 timeval 1 (循环顶刷新) */
	struct timeval tv_net;  /* 原栈 timeval 2 (断网计时用) */
	struct timeval tv_ref;  /* 原栈 timeval 3 (ASIC 块周期基准) */
	int dead_count[4];      /* 原栈 4×int (逐链停转计数, 原始仅写不读) */
	int elapsed;            /* 秒差 (多处复用) */
	int chain;
	int asic;
	int pos;
	int sep_count;
	uint32_t asic_num;
	int total_asic_num;     /* 在位 ASIC 总数 */
	int total_nonce;        /* 全部 ASIC nonce 总和 */
	uint32_t avg_thresh;    /* 平均 nonce/8 */
	char *chain_flag;       /* DAT_0009d4bb 起链标志游标 */
	int fan_ret;            /* 1=数量<2 2/3=fan1/fan2 转速过低 4=满速仍异常 */
	int fan1_idx;           /* 位图首个置位位索引 */
	int fan2_idx;           /* 位图第二个置位位索引 */
	int fan1_present;       /* fan1 在位标志 */
	int fan2_present;       /* fan2 在位标志 */
	uint32_t fan1_speed;
	uint32_t fan2_speed;
	const char *msg;
	int32_t speed_arg;
	int i;

	tv_ref.tv_sec = 0;
	tv_ref.tv_usec = 0;
	cgtime(&tv_now);
	cgtime(&tv_net);
	copy_timeval(&tv_ref, &tv_now);
	dead_count[0] = 0;
	dead_count[1] = 0;
	dead_count[2] = 0;
	dead_count[3] = 0;
	for (;;) { /* 主循环 (原 0x00044bf0 标号) */
		for (;;) { /* 等待门: 距上次基准 ≥12000s 才整轮等待 (tv_ref 每 ~120s 被重置, 稳态不触发) */
			cgtime(&tv_now);
			cgtime(&tv_net);
			elapsed = tv_now.tv_sec - tv_ref.tv_sec;
			if (tv_now.tv_usec - tv_ref.tv_usec < 0) {
				elapsed--;
			}
			if (elapsed < 12000) {
				break;
			}
			copy_timeval(&tv_ref, &tv_now);
			cgsleep_ms(3000);
		}
		if (ASIC_STATUS_INTERVAL_S < elapsed) {
			/* ---- 快照 12 字计数区 ×2 (供其他线程读取) ---- */
			for (i = 0; i < 12; i++) {
				DAT_0007c134[i] = *(uint32_t *)&DAT_0009d5e7[i * 4];
			}
			for (i = 0; i < 12; i++) {
				DAT_0007c164[i] = *(uint32_t *)&DAT_0009d5b7[i * 4];
			}
			/* ---- 汇总每链在位 ASIC 的 nonce 计数 ---- */
			total_asic_num = 0;
			total_nonce = 0;
			chain = 0;
			chain_flag = DAT_0009d4bb;
			do {
				if (chain_flag[1] != '\0') { /* 链在位 (DAT_0009d4bc[chain]) */
					asic_num = (uint8_t)chain_flag[0x20a]; /* = DAT_0009d6c5[chain] */
					total_asic_num = total_asic_num + (int)asic_num;
					if (asic_num != 0) {
						for (asic = 0; (uint32_t)asic < (uint32_t)asic_num; asic++) {
							total_nonce = total_nonce +
							              *(int32_t *)&DAT_0009d5b7[(chain * 3 + asic) * 4];
							if ((DAT_00078e09 != '\0') && LOG_GATE7()) {
								snprintf(buf, 0x1000, "%s: chain %d asic %d asic_nonce_num %d",
								         __func__, chain, asic,
								         *(int32_t *)&DAT_0009d5b7[(chain * 3 + asic) * 4]);
								applog(LOG_DEBUG, buf, 0);
							}
						}
					}
				}
				chain++;
				chain_flag++;
			} while (chain != 4);
			/* ---- 平均 nonce /8 作为停转门槛 ---- */
			if (total_asic_num == 0) {
				avg_thresh = 1;
			}
			else {
				if ((DAT_00078e09 != '\0') && LOG_GATE7()) {
					snprintf(buf, 0x1000, "%s: avg_num %d asic_num %d",
					         __func__, total_nonce, total_asic_num);
					applog(LOG_DEBUG, buf, 0);
				}
				avg_thresh = __udivsi3((uint32_t)total_nonce, (uint32_t)total_asic_num) >> 3;
			}
			/* ---- 逐链逐 ASIC 分类: 'o'=正常 'x'=停转, 每 8 片插一空格 ---- */
			chain = 0;
			chain_flag = DAT_0009d4bb;
			do {
				if (chain_flag[1] != '\0') {
					asic_num = (uint8_t)chain_flag[0x20a];
					sep_count = 0;
					asic = 0;
					if (asic_num != 0) {
						do {
							if ((asic & 7) == 0) {
								pos = sep_count + asic;
								if (0x13 < pos) {
									if (LOG_GATE3()) {
										snprintf(buf, 0x1000, "asic num err![%d]", pos);
										applog(LOG_ERR, buf, 0);
									}
								}
								sep_count++;
								DAT_0009d617[pos + chain * 0x13] = ' '; /* 0x20 分隔 */
							}
							if (avg_thresh < *(uint32_t *)&DAT_0009d5b7[(chain * 3 + asic) * 4]) {
								DAT_0007c194[chain * 3 + asic] = 0;
								goto set_ok; /* 原 0x45848 'o' 标号 */
							}
							DAT_0007c194[chain * 3 + asic] = 1;
							if (DAT_00078e28 != '\0') {
								goto set_ok; /* DAT_00078e28 置位时不标停转 */
							}
							pos = sep_count + asic;
							if (0x13 < pos) {
								if (LOG_GATE3()) {
									snprintf(buf, 0x1000, "asic num err![%d]", pos);
									applog(LOG_ERR, buf, 0);
								}
							}
							DAT_0009d617[pos + chain * 0x13] = 'x'; /* 0x78 停转 */
							dead_count[chain]++; /* 原始按 chain*4 字节偏移递增, 等价 */
							if ((DAT_00078e09 != '\0') && LOG_GATE7()) {
								snprintf(buf, 0x1000, "x chain%d asic%d nonce_num=%d avg_num=%d",
								         chain, asic,
								         *(int32_t *)&DAT_0009d5b7[(chain * 3 + asic) * 4],
								         avg_thresh);
								applog(LOG_DEBUG, buf, 0);
							}
							goto next_asic;
set_ok:
							pos = sep_count + asic;
							if (0x13 < pos) {
								if (LOG_GATE3()) {
									snprintf(buf, 0x1000, "asic num err![%d]", pos);
									applog(LOG_ERR, buf, 0);
								}
							}
							DAT_0009d617[pos + chain * 0x13] = 'o'; /* 0x6f 正常 */
next_asic:
							if (0x13 < asic) {
								if (LOG_GATE3()) {
									snprintf(buf, 0x1000, "asic num err![%d]", asic);
									applog(LOG_ERR, buf, 0);
								}
							}
							/* 每 ASIC nonce 计数清零 (原实现以 DAT_0009d4b7 基址做字节/字混合寻址,
							 * 保留计数最高字节): 0x9d5b7+4k 清零, 0x9d5b8+4k &= 0xff000000 */
							{
								uint32_t k = (uint32_t)(chain * 3 + asic + 0x40) * 4;
								uint32_t cnt = *(uint32_t *)(DAT_0009d4b7 + k + 1);
								DAT_0009d4b7[k] = 0;
								*(uint32_t *)(DAT_0009d4b7 + k + 1) = cnt & 0xff000000u;
							}
							asic++;
						} while ((uint32_t)asic < (uint32_t)asic_num);
						pos = sep_count + asic;
						if (0x13 < pos) {
							if (LOG_GATE3()) {
								snprintf(buf, 0x1000, "asic num err![%d]", pos);
								applog(LOG_ERR, buf, 0);
							}
						}
					}
					else {
						pos = 0;
					}
					/* 字符串收尾 (索引 = chain*16 + chain*3 + pos, 原始即如此) */
					DAT_0009d617[chain * 0x13 + pos] = '\0';
				}
				chain++;
				chain_flag++;
			} while (chain != 4);
			/* ---- 通知等待方并清零计数区 ---- */
			pthread_mutex_lock((pthread_mutex_t *)DAT_00079100);
			pthread_cond_signal((pthread_cond_t *)DAT_0007c078);
			pthread_mutex_unlock((pthread_mutex_t *)DAT_00079100);
			memset(DAT_0009d5e7, 0, 0x30);
			memset(DAT_0009d5b7, 0, 0x30);
			copy_timeval(&tv_ref, &tv_now);
		}
		/* ---- 温度守门: pcb(DAT_0009d6d6)/chip(DAT_0009d6da) 高于 80/110 ---- */
		if ((DAT_0009d6d6 < 0x51) && (DAT_0009d6da < 0x6f)) {
			DAT_0007911c = 0;
			DAT_0007911d = '\0';
		}
		else {
			DAT_0007911c++;
			if (DAT_0007911c < 3) {
				if ((DAT_00078e3c == '\0') && LOG_GATE3()) {
					snprintf(buf, 0x1000,
					         "Temperature pcb:%d/chip:%d is higher than %d/%d\'C for %d time, PWM is %d",
					         DAT_0009d6d6, DAT_0009d6da, TEMP_PCB_SHUTDOWN_C, TEMP_CHIP_SHUTDOWN_C,
					         (uint32_t)DAT_0007911c, (uint32_t)DAT_0009d6ca);
					applog(LOG_ERR, buf, 0);
				}
			}
			else {
				DAT_0007911d = '\x01';
				if ((DAT_00078e3c == '\0') && LOG_GATE3()) {
					snprintf(buf, 0x1000,
					         "%s: the temperature pcb:%d/chip:%d is too high, close PIC and need reboot!!!",
					         __func__, DAT_0009d6d6, DAT_0009d6da);
					applog(LOG_ERR, buf, 0);
				}
			}
		}
		/* ---- 断网守门: 距 DAT_000790f4/790f8 ≥121s ---- */
		elapsed = tv_net.tv_sec - DAT_000790f4;
		if (tv_net.tv_usec - DAT_000790f8 < 0) {
			elapsed--;
		}
		if (elapsed < INTERNET_LOST_SEC) {
			DAT_0007911e = '\0';
		}
		else {
			DAT_0007911e = '\x01';
			if (((DAT_00078e3c == '\0') && (DAT_00078e09 != '\0')) && LOG_GATE7()) {
				snprintf(buf, 0x1000,
				         "%s: We have lost internet for %d seconds, so don\'t send work to hashboard anymore",
				         __func__, elapsed);
				applog(LOG_DEBUG, buf, 0);
			}
		}
		/* ---- 风扇位图取前两个置位位 (原 0x44d7c/0x44d84 扫描链, 等价结构化) ---- */
		fan1_idx = -1;
		fan2_idx = -1;
		for (i = 0; i < 32; i++) {
			if ((DAT_0009d4c8 & (1u << i)) != 0) {
				if (fan1_idx < 0) {
					fan1_idx = i;
				}
				else {
					fan2_idx = i;
					break;
				}
			}
		}
		fan1_present = (fan1_idx >= 0);
		fan2_present = (fan2_idx >= 0);
		fan1_speed = 0;
		fan2_speed = 0;
		if (fan1_idx >= 0) {
			fan1_speed = *(uint32_t *)&DAT_0009d69d[fan1_idx * 4];
		}
		if (fan2_idx >= 0) {
			fan2_speed = *(uint32_t *)&DAT_0009d69d[fan2_idx * 4];
		}
		/* ---- 风扇守门 ---- */
		if (DAT_0009d6cc < 2) {
			fan_ret = 1;
			goto fan_error_log; /* 原 0x44fec 告警标号 */
		}
		if (fan1_present != 0) {
			if (fan1_speed < (uint32_t)(uint8_t)DAT_0009d6f8 * 6000u / 130u) {
				/* 原 32 位魔数除法: (int64)(int)(pwm*6000) * 0x7e07e07f >> 38 = pwm*6000/130 */
				fan_ret = 2;
				goto fan_error_log;
			}
			if ((DAT_0009d6ca == 100) && (fan1_speed <= FAN1_FULL_PWM_MIN_RPM)) {
				fan_ret = 4;
				goto fan_error_log;
			}
		}
		if (fan2_present != 0) {
			if (fan2_speed < (uint32_t)(uint8_t)DAT_0009d6f8 * 4300u / 130u) {
				/* 原 32 位魔数除法: pwm*4300/130 */
				fan_ret = 3;
				goto fan_error_log;
			}
			if ((DAT_0009d6ca == 100) && (fan2_speed < FAN2_FULL_PWM_MIN_RPM)) {
				fan_ret = 4;
				goto fan_error_log;
			}
		}
		DAT_00079120 = 0;
		DAT_00079125 = '\0';
		goto led_update; /* 原 0x44e5c LED 标号 */
fan_error_log:
		DAT_00079120++;
		if ((DAT_00078e09 != '\0') && LOG_GATE7()) {
			snprintf(buf, 0x1000,
			         "ret:%d [%d:%d]fan_error_num:%d fan_num %d fan_pwm %d fan1_speed_value %d fan2_speed_value %d",
			         fan_ret, fan1_present, fan2_present, DAT_00079120,
			         (uint32_t)DAT_0009d6cc, (uint32_t)(uint8_t)DAT_0009d6f8,
			         fan1_speed, fan2_speed);
			applog(LOG_DEBUG, buf, 0);
		}
		if (FAN_ERROR_COUNT_LIMIT < DAT_00079120) {
			DAT_00079125 = '\x01';
			/* 原始在此重新扫描位图 (joined_r0x000450e8), DAT_0009d4c8 未变, 结果与外层一致 */
			if (fan_ret == 3) {
				if ((DAT_00078e3c != '\0') || !LOG_GATE3()) {
					goto led_update;
				}
				speed_arg = (int32_t)fan2_speed;
				msg = "Fan Err! Disable PIC! Fan2 speed is too low %d pwm %d ";
			}
			else if (fan_ret == 4) {
				if ((DAT_00078e3c == '\0') && LOG_GATE3()) {
					snprintf(buf, 0x1000, "Fan Err! Disable PIC! Fan1:%d Fan2:%d pwm %d",
					         fan1_speed, fan2_speed, (uint32_t)DAT_0009d6ca);
					applog(LOG_ERR, buf, 0);
				}
				goto led_update;
			}
			else if (fan_ret != 2) {
				if ((DAT_00078e3c == '\0') && LOG_GATE3()) {
					snprintf(buf, 0x1000, "Fan Err! Disable PIC! Fan num is %d",
					         (uint32_t)DAT_0009d6cc);
					applog(LOG_ERR, buf, 0);
				}
				goto led_update;
			}
			else {
				if ((DAT_00078e3c != '\0') || !LOG_GATE3()) {
					goto led_update;
				}
				speed_arg = (int32_t)fan1_speed;
				msg = "Fan Err! Disable PIC! Fan1 speed is too low %d pwm %d ";
			}
			snprintf(buf, 0x1000, msg, speed_arg, (uint32_t)DAT_0009d6ca);
			applog(LOG_ERR, buf, 0);
			goto led_update;
		}
		DAT_00079125 = '\0'; /* ≤20 次: 标志清零 (原始控制流如此) */
led_update:
		/* ---- 恢复/告警裁决与 LED 输出 ---- */
		if (DAT_00078e28 != '\0') {
			DAT_0007911e = '\0';
		}
		if (((DAT_00079124 == '\0') && (DAT_0007911e == '\0')) &&
		    ((DAT_0007911d == '\0') && ((DAT_00079125 == '\0') && (DAT_00079126 == '\0')))) {
			DAT_000790f0 = 0;
			DAT_00078e3c = '\0';
			if (DAT_00079127 != '\0') {
				DAT_000790f0 = 1;
				DAT_00078e3c = '\x01';
			}
		}
		else {
			DAT_000790f0 = 1;
			if ((DAT_00079127 == '\0') && ((DAT_0007911d != '\0') || (DAT_00079125 != '\0'))) {
				/* 首次高温/风扇异常: 逐链停 PIC DC-DC */
				DAT_00078e3c = '\x01';
				DAT_00079127 = '\x01';
				chain = 0;
				do {
					chain++; /* 原始游标先自增后判定 (DAT_0009d4bb[chain]), 链号 = chain-1 */
					if (DAT_0009d4bb[chain] == 1) {
						pthread_mutex_lock((pthread_mutex_t *)DAT_000790d0);
						DAT_0007909c = chain - 1;
						disable_PIC16F1704_dc_dc_new();
						cgsleep_ms(100);
						pthread_mutex_unlock((pthread_mutex_t *)DAT_000790d0);
					}
				} while (chain != 4);
			}
		}
		toggle_red_green_led(DAT_000790f0);
		cgsleep_ms(1000);
	}
}

// === create_bitmain_check_fan_pthread === 原 FUN_000498f4 @ 0x000498f4
// 来源: reversed/cgminer/14_temp_fan/FUN_000498f4.c | 分类: 14_temp_fan
// 语义: 创建 check_fan (FUN_0003aa9c) 线程 — calloc 0x40 字节 thr_info, thr_info_create
//       成功后 pthread_detach 并睡 500ms (返回其 r0); 失败打调试日志 (文案仍为
//       "check miner_status", 原始即如此) 并返回 -8。
uint32_t create_bitmain_check_fan_pthread(void)
{
	char buf[4096]; /* 原栈日志缓冲 */
	int ret;

	DAT_0007c284 = calloc(1, 0x40);
	ret = thr_info_create(DAT_0007c284, NULL, check_fan, NULL);
	if (ret == 0) {
		pthread_detach(*(pthread_t *)((char *)DAT_0007c284 + 0xc)); /* thr_info.pth @0xc */
		return cgsleep_ms(500);
	}
	if ((DAT_00078e09 != '\0') && LOG_GATE7()) {
		snprintf(buf, 0x1000, "%s: create thread for check miner_status", __func__);
		applog(LOG_DEBUG, buf, 0);
	}
	return 0xfffffff8u;
}

// === create_bitmain_read_temp_pthread === 原 FUN_00049be8 @ 0x00049be8
// 来源: reversed/cgminer/14_temp_fan/FUN_00049be8.c | 分类: 14_temp_fan
// 语义: 创建读温度线程 (主体 FUN_00048a18, 命名待补) — calloc 0x40 字节 thr_info,
//       thr_info_create 以 thr 自身为参数; 成功后 pthread_detach 并睡 500ms (返回其 r0);
//       失败打调试日志并返回 -9。
uint32_t create_bitmain_read_temp_pthread(void)
{
	char buf[4096]; /* 原栈日志缓冲 */
	int ret;

	DAT_0007c12c = calloc(1, 0x40);
	ret = thr_info_create(DAT_0007c12c, NULL, fun_00048a18, DAT_0007c12c);
	if (ret == 0) {
		pthread_detach(*(pthread_t *)((char *)DAT_0007c12c + 0xc)); /* thr_info.pth @0xc */
		return cgsleep_ms(500);
	}
	if ((DAT_00078e09 != '\0') && LOG_GATE7()) {
		snprintf(buf, 0x1000, "%s: create thread for read temp", __func__);
		applog(LOG_DEBUG, buf, 0);
	}
	return 0xfffffff9u;
}

// === api_stats === 原 FUN_0003a4ec @ 0x0003a4ec
// 来源: reversed/cgminer/14_temp_fan/FUN_0003a4ec.c | 分类: 14_temp_fan
// 语义: 组装 cgminer API "stats" 应答数据链 — 依次挂接 miner_count/fan_num/fan1..6/
//       temp_num/temp1..4/temp2_1..4/temp_max/Device Hardware%/no_matching_work/
//       chain_acn1..4/chain_acs1..4/chain_hw1..4/frequency1..4/chain_rate1..4。
//       Device Hardware% = ((double)DAT_0007b8a4/256) / (double)(三分量总和+DAT_0007b8a4)。
void api_stats(void)
{
	char names[36];      /* 原栈 36 字节名字区: [0..0xF] 主名缓冲, [0x10..0x23] 副名缓冲 (name2) */
	char *name2 = names + 0x10; /* 原 param0 */
	void *root = NULL;
	int i;
	int64_t total64;
	int64_t hw64;
	double hw_pct;
	double rate_denom;

	root = add_uint8_buf(NULL, "miner_count", &DAT_0009d6cb, 0);
	root = add_uint8_buf(root, "fan_num", &DAT_0009d6cc, 0);
	for (i = 0; i < 6; i++) {
		sprintf(name2, "fan%d", i + 1);
		root = api_add_uint32(root, name2, (uint32_t *)&DAT_0009d69d[i * 4], 0);
	}
	/* 第 4 参 (copied) 原始反编译未跟踪, 按调用点惯常 0 处理 */
	root = add_uint8_buf(root, "temp_num", &DAT_0009d6cb, 0);
	for (i = 0; i < 4; i++) {
		sprintf(name2, "temp%d", i + 1);
		root = add_short_buf(root, name2, (int16_t *)&DAT_0009d741[i * 4], 0);
	}
	for (i = 0; i < 4; i++) {
		sprintf(name2, "temp2_%d", i + 1);
		root = add_ushort_buf(root, name2, (uint16_t *)&DAT_0009d751[i * 4], 0);
	}
	root = api_add_uint32(root, "temp_max", (uint32_t *)&DAT_0009d6d6, 0);
	/* ---- Device Hardware% = (no_matching_work/256) / (三分量总和 + no_matching_work) ---- */
	total64 = __fixdfdi(DAT_0007b5e8 + DAT_0007b670 + DAT_0007b428);
	DAT_0007b104 = (int32_t)(total64 >> 32); /* 高字 (strd 存 DAT_0007b100/104) */
	DAT_0007b100 = (int32_t)total64;         /* 低字 */
	hw64 = (int64_t)DAT_0007b8a4;            /* 原 adc r1, r3, r12, asr #31: int32 符号扩展 */
	if (total64 + hw64 == 0) { /* 原 adds/adc/orrs 两级判零 */
		hw_pct = 0.0;
	}
	else {
		/* 原始分子为 vcvt.f64.s32 d8, d8, #8 定点转换 (折叠除以 256);
		 * 分母为 64 位和 total+hw (反汇编核实 r0:r1 为 adds/adc 结果) */
		hw_pct = ((double)DAT_0007b8a4 / 256.0) / __floatdidf(total64 + hw64);
	}
	DAT_00078e20 = hw_pct; /* 原 DAT_00078e24(高)/78e20(低) 两半字分开写 */
	root = add_percent_buf(root, "Device Hardware%", &DAT_00078e20, 1);
	root = api_add_int(root, "no_matching_work", &DAT_0007b8a4, 1);
	for (i = 0; i < 4; i++) {
		sprintf(name2, "chain_acn%d", i + 1);
		root = add_uint8_buf(root, name2, &DAT_0009d6c5[i], 0);
	}
	for (i = 0; i < 4; i++) {
		sprintf(name2, "chain_acs%d", i + 1);
		root = api_add_string(root, name2, &DAT_0009d617[i * 0x13], 0);
	}
	for (i = 0; i < 4; i++) {
		sprintf(name2, "chain_hw%d", i + 1);
		root = api_add_uint32(root, name2, (uint32_t *)&DAT_0009d5a7[i * 4], 0);
	}
	for (i = 0; i < 4; i++) {
		memset(names, 0, 0x20); /* 原始对两个名字缓冲逐字节清零 */
		sprintf(names, "frequency%d", i + 1);
		/* 副名缓冲被格式化但结果未使用 (死代码, 原始即如此) */
		sprintf(name2, "%u", (uint32_t)*(uint16_t *)&DAT_0009d4b5[(i + 0x128) * 2]);
		root = add_ushort_buf(root, names, (uint16_t *)&DAT_0009d4b5[(i + 0x128) * 2], 0);
	}
	for (i = 0; i < 4; i++) {
		sprintf(name2, "chain_rate%d", i + 1);
		if (DAT_00078e28 != '\0') {
			rate_denom = (double)(float)DAT_00078e38; /* 原始 VectorSignedToFloat (int→float) 升 double */
			sprintf(&DAT_0007c0a8[i * 0x10], "%2.2f", DAT_00078e30 / rate_denom);
		}
		root = api_add_string(root, name2, &DAT_0007c0a8[i * 0x10], 0);
	}
	return;
}

/* ============================================================
 * 覆盖率表 (18/18 — batch=fan_b1, 全部完成)
 * ============================================================
 * | #  | 原地址       | 语义名                                    | 状态 |
 * |----|--------------|-------------------------------------------|------|
 * | 1  | 0x000155f0 | set_temp_cutoffs                          | 完成 |
 * | 2  | 0x00019adc | unlock_curses                             | 完成 |
 * | 3  | 0x0001d7c8 | restart_mine                              | 完成 |
 * | 4  | 0x00032e64 | add_uint8_buf                             | 完成 |
 * | 5  | 0x00032e88 | add_short_buf                             | 完成 |
 * | 6  | 0x00032eac | add_ushort_buf                            | 完成 |
 * | 7  | 0x00044ac0 | toggle_red_green_led                      | 完成 |
 * | 8  | 0x00045b50 | check_fan_number                          | 完成 |
 * | 9  | 0x00045d88 | set_fan_pwm                               | 完成 |
 * | 10 | 0x00045dec | convert_temperature_to_pwm                | 完成 |
 * | 11 | 0x00045ec0 | set_PWM_according_to_temperature          | 完成 |
 * | 12 | 0x0004601c | enable_read_temperature_from_asic_chain   | 完成 |
 * | 13 | 0x0004633c | write_i2c_reg                             | 完成 |
 * | 14 | 0x000490fc | set_temperature_offset_value_chain_sensor | 完成 |
 * | 15 | 0x00044b5c | check_miner_status                        | 完成 |
 * | 16 | 0x000498f4 | create_bitmain_check_fan_pthread          | 完成 |
 * | 17 | 0x00049be8 | create_bitmain_read_temp_pthread          | 完成 |
 * | 18 | 0x0003a4ec | api_stats                                 | 完成 |
 */
