/**
 * common_types.h — BM1746 固件转写共享类型
 *
 * 来源: Ghidra 反编译类型约定 (reversed/ 全部二进制)
 * 约定: 见 rewritten/CONVENTIONS.md §3
 * 说明: 本工程为"语义重构"转写, 不保证与原始固件结构体逐字节一致;
 *       仅当布局有机器码证据(偏移/大小被调用点锁定)时才给出字段。
 */
#ifndef BM1746_REWRITTEN_COMMON_TYPES_H
#define BM1746_REWRITTEN_COMMON_TYPES_H

#include <stdint.h>
#include <stddef.h>

/* ---- 平台基础 ---- */
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

/* ---- 链/芯片拓扑 (来源: reversed/tables/topol.conf, 机器 BEZ36501) ---- */
#define CHAIN_NUM          3       /* chain_num: 物理链数 (寄存器窗口支持 0..3) */
#define CHAIN_ASIC_NUM     6       /* chain_asic_num: 每链 ASIC 数 */
#define CHAIN_DOMAIN_NUM   3       /* chain_domain_num: 供电域数 */
#define DOMAIN_ASIC_NUM    2       /* domain_asic_num: 每域 ASIC 数 */
#define ASIC_ADDR_INTERVAL 42      /* asic_addr_interval: 芯片地址步进, 设址帧用 */
#define ASIC_CHIP_TYPE     0x1746  /* chip_type "0x1746" */

/* 工作目录内每链最多芯片组数 (cgminer 按 chain*3 组寻址, 3 域×每域1组) */
#define CHAIN_GROUP_NUM    3

/* ---- 温度/风扇限值 (topol.conf) ---- */
#define MAX_CHIP_TEMP      95
#define MAX_PCB_TEMP       80
#define FAN_PWM_MIN_PCT    20
#define FAN_PWM_MAX_PCT    100
#define FAN_FULL_SPEED_TEMP 75
#define FAN_TARGET_TEMP    70

/* ---- 电压合法窗 (mV, BM1746_uart_register_ops.md §〇-1: [700,800], 越界回落 700) ---- */
#define VOLT_MIN_MV        700
#define VOLT_MAX_MV        800
#define VOLT_DEFAULT_MV    780     /* 出厂 bitmain-voltage (cgminer.conf.factory) */
#define VOLT_FALLBACK_MV   700

/* ---- 频率规则 (BM1746_uart_register_ops.md §〇-2) ---- */
#define FREQ_DEFAULT_MHZ   800     /* 配置 0/缺省/800 => 800 */
#define FREQ_SCAN_MIN_MHZ  775     /* 扫频窗口下限 */
#define FREQ_SCAN_MAX_MHZ  815     /* 扫频窗口上限 (0x32f) */
#define FREQ_SCAN_LIMIT_MHZ 825    /* 扫频停止上限 (0x339) */
#define FREQ_SAVE_MIN_MHZ  775     /* chips_freq.config 合法窗 */
#define FREQ_SAVE_MAX_MHZ  900
#define FREQ_MISS_FALLBACK_MHZ 587 /* 频率表未命中兜底槽位 0x40 */

/* ---- 未消化的 Ghidra 全局引用占位 ----
 * 未知 DAT_xxx 在转写中保留原名 (CONVENTIONS §4); 此处仅声明常见共享全局的语义名。
 * 具体地址见各模块注释。 */
/* 例: extern uint32_t g_target_freq_x8;   // 原 DAT_0009d6f8, 800<<8 */

#endif /* BM1746_REWRITTEN_COMMON_TYPES_H */
