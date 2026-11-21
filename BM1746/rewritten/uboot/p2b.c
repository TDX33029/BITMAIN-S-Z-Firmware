// ============================================================================
// uboot/p2b.c — U-Boot 2016.07 反编译转写 (半批 uboot_b2 后 24 函数)
// 地址范围: 0x040076b8 .. 0x0400c708
// 命名事实源: rewritten/name_map/uboot.tsv
// 语义规则: rewritten/CONVENTIONS.md
// ============================================================================

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* ----------------------------------------------------------------------------
 * 模块内部前向声明区
 * -------------------------------------------------------------------------- */

/* 本文件内定义、但定义点在后的函数 (按地址升序) */
void ub_panic_reset(void);
uint32_t parse_size_suffix(const char *str);
uint32_t compute_ip_checksum(uint16_t *buf, uint32_t len);
int arp_timeout_check(void);
void dhcp_process_options(uint8_t *popt, uint8_t *end);
void dhcp_handler(char *pkt, int dest_port, uint32_t unused_r2, int src_port, uint32_t len);
void *eth_get_dev_by_name(const char *devname);
void *eth_get_ethaddr(void);
int eth_send(void *packet, int length);
int eth_rx(void);
int eth_getenv_enetaddr(const char *name, uint8_t *enetaddr);
void eth_setenv_enetaddr_by_index(const char *base_name, int index, const uint8_t *enetaddr);
void eth_try_another(int first_route);
void net_clear_handlers(void);
void net_init_loop(void);
int net_start_again(void);
void *net_get_arp_handler(void);
int net_set_timeout_handler(uint32_t delay_ms, void *handler);
void net_set_udp_header(uint8_t *pkt, uint32_t dest_ip, uint16_t dst_port, uint16_t src_port,
                        uint32_t payload_len);
void copy_filename(char *dst, char *src, int len);
void nfs_read_req(uint32_t offset, uint32_t length);
void ping_receive(void *pkt, uint8_t *ip, int len);
int ub_un_00c65c(void);
uint32_t aeabi_idiv(uint32_t num, uint32_t denom);

/* ----------------------------------------------------------------------------
 * 跨文件被调函数 (定义于本二进制其他转写文件, 命名源: rewritten/name_map/uboot.tsv)
 * -------------------------------------------------------------------------- */
extern unsigned long simple_strtoul(const char *cp, char **endp, unsigned int base); /* FUN_04007718 */
extern void ub_memcpy(void *dest, const void *src, uint32_t n);   /* FUN_04005ea4 (p2a) */
extern int  ub_memcmp(const void *s1, const void *s2, uint32_t n);/* FUN_04005ff4 (p2a) */
extern void ub_memset(void *s, int c, uint32_t n);                /* FUN_04005e0c (p2a) */
extern uint32_t ub_strlen(const char *s);                         /* FUN_04005b98 (p2a) */
extern int  ub_strcmp(const char *s1, const char *s2);            /* FUN_04005a60 (p2a) */
extern int  ub_strncmp(const char *s1, const char *s2, uint32_t n); /* FUN_04005a90 (p2a) */
extern void ub_sprintf(char *buf, const char *fmt, ...);          /* FUN_04007514 (p2a) */
extern void ub_printf(const char *fmt, ...);                      /* FUN_04007548 (p2a) */
extern int  ub_get_timer(int base);                               /* FUN_040061e4 (p2a), 毫秒 */
extern void ub_udelay(uint32_t usec);                             /* FUN_040062f8 (p2a) */
extern void ub_div0_hang(void);                                   /* FUN_04004844 (p2a) */
extern void arp_request(void);                                    /* FUN_04007de8 */
extern void dhcp_packet_process_options(uint8_t *pkt);            /* FUN_040084d8 */
extern int  dhcp_extended(uint8_t *e, int message, uint8_t *server_ip,
                          uint8_t *requested_ip);                 /* FUN_04008110 */
extern void eth_parse_enetaddr(const char *s, uint8_t *enetaddr); /* FUN_04009550 */
extern void eth_setenv_enetaddr(const char *name, const uint8_t *enetaddr); /* FUN_04009608 */
extern void *eth_get_dev(void);                                   /* FUN_04008f68 */
extern void eth_set_current_to_next(void);                        /* FUN_04008f30 */
extern void eth_current_changed(void);                            /* FUN_04009754 */
extern void eth_halt(void);                                       /* FUN_04009260 */
extern int  eth_init(void);                                       /* FUN_04009164 */
extern void net_process_received_packet(void *packet, int len);   /* FUN_0400a0a8 */
extern int  net_update_ether(void *pkt, void *dest_mac, uint32_t proto); /* FUN_0400a4b8, 签名待校 */
extern void net_set_ip_header(uint8_t *pkt, uint32_t dest_ip, uint32_t src_ip); /* FUN_0400a538 */
extern int  net_set_ether(uint8_t *pkt, const uint8_t *dest_mac, uint16_t proto); /* FUN_0400a41c, 返回头长 */
extern void net_auto_load(void);                                  /* FUN_04009a20 */
extern int  rpc_add_credentials(uint8_t *buf);                    /* FUN_0400ac9c, 返回续写位置 */
extern void rpc_req(uint32_t prog, int proc, uint8_t *data, int len_words); /* FUN_0400ad48 */

/* ----------------------------------------------------------------------------
 * 未编目外部调用 (0x03fbxxxx / 0x03fdxxxx 区, Ghidra func_0x 直呼, name_map 未收录)
 * -------------------------------------------------------------------------- */
extern void  fun_03fc7f78(int a0);                 /* 命名待补 */
extern void  fun_03fb1d70(int a0, int a1, int a2); /* 命名待补, 疑似复位入口 */
extern void  fun_03fc7fbc(const char *s);          /* 命名待补, 用法似 puts/printf */
extern void  fun_03fbf43c(int a0);                 /* 命名待补 */
extern void  fun_03fbeb7c(const char *s);          /* 命名待补, 用法似 printf */
extern char *fun_03fbefb0(const char *name);       /* 命名待补, 用法 = env_get (getenv) */
extern void  fun_03ff78c8(void *pkt, uint32_t len);/* 命名待补 */
extern void  fun_03fd0cb8(int a0, void *out);      /* 命名待补, 取设备链描述 */
extern int   fun_03fd031c(void *dev);              /* 命名待补, 设备可用性检查 */
extern int   fun_03fd17b4(void);                   /* 命名待补 */

/* ----------------------------------------------------------------------------
 * U-Boot 全局变量 (Ghidra DAT_ 原名保留; 带"待定"注记的语义为按上下文推断)
 * 指针槽/标量按用法区分: [] 形式表示该地址处的内存实体 (可取地址/按下标),
 *                        标量形式表示直接读取该地址处变量的值
 * -------------------------------------------------------------------------- */
/* ARP 超时巡检 (arp_timeout_check) */
extern int  DAT_04007ee8[];   /* 全局, ARP 等待标志 (0 = 无未决请求) */
extern int  DAT_04007eec[];   /* 全局, 上次 ARP 请求时间基准 */
extern int  DAT_04007ef0[];   /* 全局, ARP 重试计数 */
extern int  DAT_04007ef8[];   /* 全局, 网络状态字 (超时置 3) */
extern char DAT_04007ef4[];   /* 全局字符串 */
/* DHCP 选项拷贝目标 (dhcp_process_options) */
extern uint32_t DAT_040084a8[];  /* 全局, 选项 0x36 服务器标识 */
extern char     DAT_040084a4[];  /* 全局字符串, 未知选项提示格式串 */
extern char     DAT_040084b0[];  /* 全局字符串, 截断提示格式串 */
extern char     DAT_040084ac[];  /* 全局字符串, 截断提示参数 */
extern char     DAT_040084b4[];  /* 全局缓冲, 选项 0x0c 主机名 (上限 0x20) */
extern uint32_t DAT_040084b8[];  /* 全局, 选项 0x33 租约期 */
extern char     DAT_040084bc[];  /* 全局缓冲, 选项 0x43 引导文件名 */
extern uint32_t DAT_040084c0[];  /* 全局, 选项 0x01 子网掩码 */
extern uint32_t DAT_040084c4[];  /* 全局, 选项 0x03 网关 */
extern uint32_t DAT_040084c8[];  /* 全局, 选项 0x06 DNS */
extern char     DAT_040084cc[];  /* 全局字符串, 截断提示参数 */
extern char     DAT_040084d0[];  /* 全局缓冲, 选项 0x11 根路径 (上限 0x40) */
extern uint8_t  DAT_040084d4[];  /* 全局, 选项 0x34 单字节值 */
/* DHCP 处理 (dhcp_handler) */
extern int     DAT_040088d8[];   /* 全局, xid 表项数 */
extern int     DAT_040088dc[];   /* 全局, xid 表 */
extern uint8_t DAT_0400890c[];   /* 全局, 本机 MAC */
extern int     DAT_04008910[];   /* 全局, DHCP 状态机 (3=REQUESTING, 4=等 ACK, 6=BOUND) */
extern void   *DAT_04008914;     /* 全局, REQUEST 超时回调 (函数指针) */
extern uint8_t *DAT_04008918[];  /* 全局, 发送缓冲指针 (NetTxPacket) */
extern uint8_t DAT_0400891c[];   /* 全局, 目的 MAC (广播) */
extern uint32_t DAT_04008920;    /* 全局, BOOTP 头首 4 字节打包值 (op/htype/hlen/hops), 语义待定 */
extern int     DAT_04008900[];   /* 全局, 计时基准 */
extern uint32_t DAT_04008924;    /* 全局, bp_secs 换算系数, 语义待定 */
extern uint32_t DAT_04008928;    /* 全局, bp_secs 掩码, 语义待定 */
extern uint8_t *DAT_0400892c[];  /* 全局, 服务器 IP 指针 */
extern uint32_t DAT_040088e0;    /* 全局, DHCP magic cookie 校验值 (0x63825363 预期), 语义待定 */
extern uint32_t DAT_040088e4[];  /* 全局, 服务器 IP (取自 bp_siaddr) */
extern uint8_t *DAT_040088e8[];  /* 全局, 接收缓冲指针 (NetRxPacket) */
extern uint8_t DAT_040088ec[];   /* 全局, 服务器 MAC 缓存 */
extern int     DAT_040088f0[];   /* 全局, 引导文件覆盖标志 */
extern char    DAT_040088f4[];   /* 全局缓冲, 引导文件名 */
extern char    DAT_040088f8[];   /* 全局字符串, 提示格式串 */
extern uint8_t DAT_040088fc[];   /* 全局, 本机 IP (NetOurIP) */
extern char    DAT_04008904[];   /* 全局字符串, 绑定完成提示格式串 */
extern char    DAT_04008908[];   /* 全局字符串, 状态异常提示格式串 */
/* 设备管理 (eth_get_dev_by_name / eth_try_another / eth_setenv_enetaddr_by_index) */
extern char  DAT_040090f4[];     /* 全局字符串, 设备名前缀 (疑似 "eth") */
extern char  DAT_040096cc[];     /* 全局字符串, 带序号 env 名格式 ("%s%daddr" 预期) */
extern char  DAT_040096d0[];     /* 全局字符串, 无序号 env 名格式 ("%saddr" 预期) */
extern char  DAT_04009854[];     /* 全局字符串, 环境变量名 (疑似 "ethrotate") */
extern char  DAT_04009860[];     /* 全局字符串, 禁用轮转比较值 (疑似 "no") */
extern void *DAT_04009858[];     /* 全局, 轮转起点设备槽 */
extern int   DAT_0400985c[];     /* 全局, 轮转绕回标志 */
/* 回调槽 (net_clear_handlers / net_get_arp_handler / net_set_timeout_handler) */
extern void  *DAT_04009918;      /* 全局, 空回调指针常量 (函数指针变量) */
extern void  *DAT_0400991c[];    /* 全局, 回调槽 1 */
extern void  *DAT_04009920[];    /* 全局, 回调槽 2 */
extern void  *DAT_04009924[];    /* 全局, 回调槽 3 (清零) */
extern void  *DAT_0400a038[];    /* 全局, ARP 处理回调槽 */
extern void  *DAT_0400a098[];    /* 全局, 超时回调槽 */
extern uint32_t DAT_0400a09c[];  /* 全局, 超时起始时刻 */
extern uint32_t DAT_0400a0a0;    /* 全局, 毫秒→计数换算系数, 语义待定 */
extern uint32_t DAT_0400a0a4[];  /* 全局, 超时截止值 */
/* net 初始化/重试 (net_init_loop / net_start_again) */
extern uint8_t DAT_04009a1c[];   /* 全局, 本机 MAC 缓存 */
extern char    DAT_04009c2c[];   /* 全局字符串, 环境变量名 (疑似 "netretry") */
extern uint32_t DAT_04009c30[];  /* 全局, 重试计数器 */
extern int     DAT_04009c34[];   /* 全局, 网络状态槽 */
extern char    DAT_04009c38[];   /* 全局字符串 (疑似 "yes") */
extern char    DAT_04009c3c[];   /* 全局字符串 (疑似 "no") */
extern char    DAT_04009c40[];   /* 全局字符串 (疑似 "once") */
extern int     DAT_04009c44[];   /* 全局, 首轮标志 */
extern int     DAT_04009c48[];   /* 全局, 子状态判定标志 */
extern int     DAT_04009c4c[];   /* 全局, 子状态判定标志 */
extern int     DAT_04009c50[];   /* 全局, 状态槽 2 */
extern int     DAT_04009c54;     /* 全局, 待写入状态值 */
extern int     DAT_04009c58[];   /* 全局, 计时槽 */
extern int     DAT_04009c5c[];   /* 全局, 超时毫秒 (10000) */
extern int     DAT_04009c60[];   /* 全局, 状态槽 3 */
extern int     DAT_04009c64;     /* 全局, 待写入状态值 */
/* UDP 头构造 (net_set_udp_header) */
extern uint32_t DAT_0400a640[];  /* 全局, 本机 IP (源地址) */
/* NFS (nfs_read_req) */
extern uint8_t  DAT_0400b060[];  /* 全局, 32 字节 NFS 文件句柄 */
extern uint32_t DAT_0400b064;    /* 全局, NFS RPC 程序号, 语义待定 */
/* ICMP (ping_receive) */
extern uint32_t DAT_0400b9d4[];  /* 全局, 待 ping 目标 IP */
extern int      DAT_0400b9d8[];  /* 全局, ping 状态槽 */
extern uint32_t DAT_0400b9dc[];  /* 全局, 本机 IP (NetOurIP) */

// ============================================================================
// 函数转写区 (按地址升序)
// ============================================================================

// === ub_panic_reset === 原 FUN_040076b8 @ 0x040076b8
// 来源: reversed/uboot_funcs/FUN_040076b8.c | 分类: uboot
// 语义: 致命错误兜底: 先输出提示 (实参 10), 延时 100ms, 调用复位入口, 然后空转挂死
void ub_panic_reset(void)
{
    fun_03fc7f78(10);            /* 命名待补: 原 func_0x03fc7f78(10) */
    ub_udelay(100000);           /* 100 ms */
    fun_03fb1d70(0, 0, 0);       /* 命名待补: 原 func_0x03fb1d70(0,0,0), 疑似系统复位 */
    for (;;) {
        /* 原始 do{}while(true): 空转挂死 */
    }
}

// === parse_size_suffix === 原 FUN_040078a8 @ 0x040078a8
// 来源: reversed/uboot_funcs/FUN_040078a8.c | 分类: uboot
// 语义: 解析带容量后缀的十进制串 ("128M"/"64K"/"1G"), 返回换算后的字节数
// 注: Ghidra 在此遇到坏指令流 (WARNING: Control flow encountered bad instruction
//     data), 跳表被截断, 全部 case 均指向截断死代码。依据保留的 case 分组
//     (0x47 'G' 独立 / 0x4b,0x6b 'K','k' 共组 / 0x4d 'M' 独立 / 其余字母+default
//     共组) 与 U-Boot 容量解析惯例还原乘数: G=2^30, M=2^20, K/k=2^10, 其余不放大。
//     返回值经 r0 传递 (Ghidra 报 void 为参数丢失所致)。
uint32_t parse_size_suffix(const char *str)
{
    char *end;
    uint32_t value;

    value = (uint32_t)simple_strtoul(str, &end, 10);
    switch (*end) {              /* 原: switch(*(char *)endptr) — 数字后第一个字符 */
    case 'G':
        value <<= 30;
        break;
    case 'M':
        value <<= 20;
        break;
    case 'K':
    case 'k':
        value <<= 10;
        break;
    default:                     /* 其余字母 ('H'..'J','L','N'.. 等) 与 default 同为不放大 */
        break;
    }
    return value;
}

// === compute_ip_checksum === 原 FUN_04007c5c @ 0x04007c5c
// 来源: reversed/uboot_funcs/FUN_04007c5c.c | 分类: uboot
// 语义: RFC1071 因特网校验和: 按 16 位字累加, 折叠进位后取反; len 为奇数时
//       末尾半字只累加其低字节 (小端布局的最后一个有效字节)
uint32_t compute_ip_checksum(uint16_t *buf, uint32_t len)
{
    uint32_t sum;
    uint32_t folded;
    uint16_t *p;

    sum = 0;
    /* 原循环条件: 1 < (int)buf + (len - (int)p), 即剩余字节数 > 1 时继续 */
    for (p = buf; 1 < (int)((uint32_t)buf + len - (uint32_t)p); p++) {
        sum += *p;
    }
    if ((len >> 1) * -2 + len == 1) {            /* len 为奇数 */
        sum += (uint8_t)buf[len >> 1];
    }
    folded = (int)(sum & 0xffff) + ((int)sum >> 0x10);       /* 原式保留有符号高位折叠 */
    return ~(uint32_t)(folded + (folded >> 0x10)) & 0xffff;
}

// === arp_timeout_check === 原 FUN_04007e6c @ 0x04007e6c
// 来源: reversed/uboot_funcs/FUN_04007e6c.c | 分类: uboot
// 语义: ARP 请求超时巡检: 等待标志非 0 时, 距上次请求超过 5000ms 则重发;
//       累计第 5 次仍失败则打印提示、清零重试计数并置网络状态为 3
// 返回: 0 = 当前无未决 ARP 请求; 1 = 处于等待且已巡检
int arp_timeout_check(void)
{
    int now;
    int retries;

    if (*DAT_04007ee8 == 0) {                    /* 等待标志: 0 = 无未决 ARP 请求 */
        return 0;
    }
    now = ub_get_timer(0);                       /* 当前时刻 (ms) */
    if (5000 < (uint32_t)(now - *DAT_04007eec)) {
        retries = *DAT_04007ef0;
        *DAT_04007ef0 = retries + 1;
        if (retries + 1 < 5) {
            *DAT_04007eec = now;                 /* 重置计时基准 */
            arp_request();                       /* 重发 ARP 请求 (FUN_04007de8) */
        }
        else {
            fun_03fc7fbc(DAT_04007ef4);          /* 命名待补: 用法似 puts/printf, 提示 ARP 超时 */
            *DAT_04007ef0 = 0;                   /* 重试计数清零 */
            *DAT_04007ef8 = 3;                   /* 网络状态 = 3 (失败) */
        }
    }
    return 1;
}

// === dhcp_process_options === 原 FUN_040082e4 @ 0x040082e4
// 来源: reversed/uboot_funcs/FUN_040082e4.c | 分类: uboot
// 语义: 遍历 DHCP 选项 TLV 区 (popt..end): 0x00=填充 (前进 1 字节, 原式以
//       oplen=0xffffffff 实现), 0xFF=结束; 已知选项按码拷贝到对应全局缓冲;
//       0x0f/0x1c/0x35/0x37..0x3b/0x42 为本版本"仅前进不处理"的选项;
//       其余未知选项打印 DAT_040084a4 提示 (原 FUN_04007548 单参调用)
void dhcp_process_options(uint8_t *popt, uint8_t *end)
{
    uint32_t opt;
    uint32_t oplen;

    do {
        if (popt >= end) {
            return;
        }
        opt = (uint32_t)*popt;
        if (opt == 0xff) {                       /* End 选项 */
            return;
        }
        oplen = (uint32_t)popt[1];               /* TLV 长度域 */
        switch (opt) {
        case 0:                                  /* Pad: 原式 oplen=0xffffffff → 前进 0xffffffff+2 = 1 字节 */
            oplen = 0xffffffff;
            break;
        case 1:                                  /* 子网掩码 (4 字节) */
            ub_memcpy(DAT_040084c0, popt + 2, 4);
            break;
        case 3:                                  /* 网关 (4 字节) */
            ub_memcpy(DAT_040084c4, popt + 2, 4);
            break;
        case 6:                                  /* DNS 服务器 (4 字节) */
            ub_memcpy(DAT_040084c8, popt + 2, 4);
            break;
        case 0x0c:                               /* 主机名: 超长提示并截断到 0x20 */
            if (0x20 < oplen) {
                ub_printf(DAT_040084b0, DAT_040084ac, oplen, 0x20);
                oplen = 0x20;
            }
            ub_memcpy(DAT_040084b4, popt + 2, oplen);
            DAT_040084b4[oplen] = 0;
            break;
        case 0x11:                               /* 根路径: 超长提示并截断到 0x40 */
            if (0x40 < oplen) {
                ub_printf(DAT_040084b0, DAT_040084cc, oplen, 0x40);
                oplen = 0x40;
            }
            ub_memcpy(DAT_040084d0, popt + 2, oplen);
            DAT_040084d0[oplen] = 0;
            break;
        case 0x33:                               /* 租约期 (4 字节) */
            ub_memcpy(DAT_040084b8, popt + 2, 4);
            break;
        case 0x34:                               /* 选项 0x34: 单字节写入 *DAT_040084d4 */
            *DAT_040084d4 = popt[2];
            break;
        case 0x36:                               /* 服务器标识 (4 字节) */
            ub_memcpy(DAT_040084a8, popt + 2, 4);
            break;
        case 0x43:                               /* 引导文件名 (变长, 补 NUL) */
            ub_memcpy(DAT_040084bc, popt + 2, oplen);
            DAT_040084bc[oplen] = 0;
            break;
        default:
            /* 原分支链中"仅前进不处理"的选项码 (原 0x040083f0 处直达):
             * 0x0f, 0x1c, 0x35, 0x37..0x3b, 0x42 */
            if ((opt == 0x0f) || (opt == 0x1c) || (opt == 0x35) || (opt == 0x42) ||
                ((0x37 <= opt) && (opt <= 0x3b))) {
                break;
            }
            /* 其余未知选项 (0x02..0x05, 0x07..0x0b, 0x0d, 0x0e, 0x10, 0x12..0x1b,
             * 0x1d..0x32, 0x3c..0x41, 0x44..) → 提示 (原 0x04008340 处) */
            ub_printf(DAT_040084a4);
            break;
        }
        popt = popt + oplen + 2;                 /* 原式: 选项指针 += 长度域 + 2 */
    } while (1);
}

// === dhcp_handler === 原 FUN_0400855c @ 0x0400855c
// 来源: reversed/uboot_funcs/FUN_0400855c.c | 分类: uboot
// 语义: DHCP 应答处理 (UDP 净荷回调): 校验端口 (目的 0x44/源 0x43)、最小长度
//       0xec 与 BOOTP 头 (op=2/htype=1/hlen=6)、xid 表命中、chaddr 为本机 MAC;
//       状态 3 (REQUESTING): 按 OFFER 构造并发送 DHCP REQUEST (bootp 字段回填 +
//       dhcp_extended 选项区, 净荷长 = 选项长 + 0xec), 随后进入状态 4 并设 5s 超时;
//       状态 4: 校验 magic cookie 与消息类型 ACK(5), 落实 IP/服务器 MAC/引导文件,
//       置状态 6 (BOUND), 打印绑定完成, 触发 net_auto_load 继续加载
// 注: unused_r2 对应原反编译 param_3 (寄存器残留, 仅被无意义的转存覆盖, 函数内未使用)
void dhcp_handler(char *pkt, int dest_port, uint32_t unused_r2, int src_port, uint32_t len)
{
    char *opt;
    uint32_t xid;
    uint32_t cookie;
    uint32_t yiaddr;
    uint32_t field;
    int state;
    int i;
    uint8_t *tx;
    int eth_hdr_len;
    int opt_len;
    uint32_t ticks;
    uint64_t secs;
    char c;

    if ((dest_port != 0x44) || (src_port != 0x43) || (len < 0xec)) {
        return;                                  /* BootpCheck: 端口与最小长度 */
    }
    if (pkt[0] != '\x02') {
        return;                                  /* bp_op 必须为 BOOTREPLY */
    }
    if (pkt[1] != '\x01') {
        return;                                  /* bp_htype 必须为以太网 */
    }
    if (pkt[2] != '\x06') {
        return;                                  /* bp_hlen 必须为 6 */
    }

    /* bp_xid (pkt+4) 必须命中本端记录的 xid 表 (原式含 Ghidra 残留的多余实参,
     * 按真实 memcpy 三参转写) */
    ub_memcpy(&xid, pkt + 4, 4);
    i = 0;
    for (;;) {
        if (i == *DAT_040088d8) {
            return;                              /* 遍历完 xid 表未命中 */
        }
        if (xid == DAT_040088dc[i]) {            /* 原: *(int*)(DAT_040088dc + i*4) */
            break;
        }
        i++;
    }
    if (ub_memcmp(pkt + 0x1c, DAT_0400890c, 6) != 0) {
        return;                                  /* bp_chaddr 必须为本机 MAC */
    }
    ub_memcpy(&yiaddr, pkt + 0x10, 4);           /* bp_yiaddr */
    if (yiaddr == 0) {
        return;
    }

    state = *DAT_04008910;                       /* DHCP 状态机 */
    if (state == 4) {
        goto handle_ack;                         /* 4 = 已发 REQUEST, 等待 ACK */
    }
    if (state == 6) {
        return;                                  /* 6 = BOUND, 忽略 */
    }
    if (state != 3) {
        fun_03fc7fbc(DAT_04008908);              /* 命名待补: 用法似 puts/printf */
        return;
    }

    /* --- 状态 3 (REQUESTING): 收到 OFFER, 构造并发送 DHCP REQUEST --- */
    dhcp_packet_process_options((uint8_t *)pkt);
    fun_03ff78c8(pkt, len);                      /* 命名待补: 原 func_0x03ff78c8(pkt,len) */
    *DAT_04008910 = 4;                           /* → 状态 4 */
    net_set_timeout_handler(5000, DAT_04008914);
    tx = *DAT_04008918;                          /* 发送缓冲 (NetTxPacket) */
    ub_memset(tx, 0, 0x5f2);
    eth_hdr_len = net_set_ether(tx, DAT_0400891c, 0x800);   /* 广播 MAC / ETH_P_IP */
    tx += eth_hdr_len;
    /* 以下偏移均相对 IP 头起点: BOOTP 头起于 +0x1c (IP 20 + UDP 8) */
    *(uint32_t *)(tx + 0x1c) = DAT_04008920;     /* bp_op/htype/hlen/hops 打包字 */
    ticks = (uint32_t)ub_get_timer(*DAT_04008900);
    secs = (uint64_t)DAT_04008924 * (uint64_t)ticks >> 0x26;   /* bp_secs 计算式, 原样保留 */
    *(uint16_t *)(tx + 0x24) = (uint16_t)DAT_04008928 & (uint16_t)((uint32_t)secs << 8) |
                               (uint16_t)(secs >> 8) & 0xff;   /* bp_secs, 大端写入 */
    field = 0;                                   /* 原式该槽位 = memcmp 结果 (必为 0) */
    ub_memcpy(tx + 0x34, &field, 4);             /* bp_giaddr = 0 */
    ub_memcpy(tx + 0x38, DAT_0400890c, 6);       /* bp_chaddr = 本机 MAC */
    copy_filename((char *)tx + 0x88, DAT_040088f4, 0x80);   /* bp_file (bootp+0x6c) */
    ub_memcpy(tx + 0x20, pkt + 4, 4);            /* bp_xid = 收到的 xid */
    ub_memcpy(&field, pkt + 0x10, 4);            /* field ← bp_yiaddr (原复用 param_3 槽位) */
    opt_len = dhcp_extended(tx + 0x108, 3, *DAT_0400892c, (uint8_t *)&field);   /* type 3 = REQUEST */
    net_set_udp_header(tx, 0xffffffff, 0x43, 0x44, (uint32_t)opt_len + 0xec);   /* 广播, 净荷 = 选项 + BOOTP 头 */
    eth_send(*DAT_04008918, eth_hdr_len + 0x1c + opt_len + 0xec);
    return;

handle_ack:                                      /* 标签来源: 状态 4 分支直接落入 */
    /* --- 状态 4: 校验 ACK 并落实配置 --- */
    /* 选项区起始 4 字节 (magic cookie) 校验 (原式含 Ghidra 残留的多余实参) */
    ub_memcpy(&cookie, pkt + 0xec, 4);
    if (cookie != DAT_040088e0) {
        return;
    }
    opt = pkt + 0xf0;                            /* 跳过 cookie, 遍历选项 */
    for (;;) {
        c = *opt;
        if (c == -1) {
            return;                              /* 0xFF End: 未找到消息类型 */
        }
        if (c == '\x35') {
            break;                               /* 选项 0x35 = DHCP 消息类型 */
        }
        if (c == '\0') {
            opt++;                               /* Pad: 前进 1 字节 */
        }
        else {
            opt += (uint8_t)opt[1] + 2;          /* TLV: 前进 长度+2 */
        }
    }
    if (opt[2] != '\x05') {
        return;                                  /* 必须为 ACK (5) */
    }
    dhcp_packet_process_options((uint8_t *)pkt);
    ub_memcpy(&field, pkt + 0x14, 4);            /* bp_siaddr */
    if (field != 0) {
        ub_memcpy(DAT_040088e4, pkt + 0x14, 4);  /* 记录服务器 IP */
    }
    /* 记录应答帧以太网头中的源 MAC (帧基址+6); 长度参数被 Ghidra 丢失 (r2 未跟踪),
     * 按源 MAC 6 字节语义重建 */
    ub_memcpy(DAT_040088ec, (char *)*DAT_040088e8 + 6, 6);
    if ((*DAT_040088f0 & 1) == 0) {              /* 标志 bit0 = 0 才允许覆盖引导文件 */
        if (ub_strlen(pkt + 0x6c) != 0) {
            copy_filename(DAT_040088f4, pkt + 0x6c, 0x400);
        }
    }
    if (*DAT_040088f4 != '\0') {                 /* 已取得文件名则打印提示 */
        fun_03fbeb7c(DAT_040088f8);              /* 命名待补: 用法似 printf */
    }
    ub_memcpy(DAT_040088fc, pkt + 0x10, 4);      /* 本机 IP ← bp_yiaddr */
    *DAT_04008910 = 6;                           /* → 状态 6 (BOUND) */
    ticks = (uint32_t)ub_get_timer(*DAT_04008900);
    ub_printf(DAT_04008904, DAT_040088fc, ticks);/* 绑定完成提示 (IP + 毫秒) */
    net_set_timeout_handler(0, NULL);            /* 取消超时 (原式仅显示首参, 该分支不用第二参) */
    fun_03fbf43c(0xa8);                          /* 命名待补: 原 func_0x03fbf43c(0xa8) */
    net_auto_load();                             /* 进入下一阶段加载 (TFTP/NFS) */
    return;
}

// === eth_get_dev_by_name === 原 FUN_04009008 @ 0x04009008
// 来源: reversed/uboot_funcs/FUN_04009008.c | 分类: uboot
// 语义: 按名称查找以太网设备: 名称须以 DAT_040090f4 (疑似 "eth") 开头且尾随
//       数字可解析; 遍历设备链表, 优先按名称全匹配, 其次按尾随编号与
//       设备结构 +0x50 处的序号匹配; 均未命中返回 NULL
// 注: 原反编译 param_2/param_3 为寄存器残留 (仅被无意义转存覆盖, 随后由
//     fun_03fd0cb8 填写的描述区覆盖), 实际签名按上游为单参数
void *eth_get_dev_by_name(const char *devname)
{
    uint32_t prefix_len;
    uint32_t devnum;
    int parsed;
    int cmp;
    char *num_ptr;
    char *endp;
    int desc[3];                    /* 原栈槽 -0x1c/-0x18/-0x14 三槽组成的描述区 */
    int *node;
    int *head;
    int *dev;
    int usable;

    prefix_len = ub_strlen(DAT_040090f4);
    cmp = ub_strncmp(devname, DAT_040090f4, prefix_len);
    if ((cmp == 0) && (prefix_len < ub_strlen(devname))) {
        num_ptr = (char *)devname + prefix_len;
        devnum = (uint32_t)simple_strtoul(num_ptr, &endp, 10);
        parsed = (int)devnum;
    }
    else {
        num_ptr = NULL;             /* 原式置 0 */
        parsed = -1;
        /* endp 原式未初始化 (与反编译一致) */
    }

    fun_03fd0cb8(0x13, desc);       /* 命名待补: 取以太网设备链描述 (0x13 = 类别 ID) */
    head = &desc[2];                /* 链表头锚点 = 描述区 +8 */
    node = *(int **)(desc + 2);     /* 首个链表节点 */
    for (;;) {
        if (node == head) {
            return NULL;            /* 空链或遍历一圈未命中 */
        }
        dev = node - 0xc;           /* container_of: 链表节点位于设备结构 +0x30 */
        usable = fun_03fd031c(dev); /* 命名待补: 设备可用性检查 */
        if (usable == 0) {
            if (ub_strcmp((char *)node[-0xb], devname) == 0) {
                return dev;         /* 名称命中: dev->name @ 节点-0x2c (设备+0x04) */
            }
            if (((uint32_t)num_ptr < (uint32_t)endp) && (node[8] == parsed)) {
                return dev;         /* 编号命中: dev->index @ 节点+0x20, 且确有尾随数字 */
            }
        }
        node = (int *)*node;        /* next */
    }
}

// === eth_get_ethaddr === 原 FUN_040090f8 @ 0x040090f8
// 来源: reversed/uboot_funcs/FUN_040090f8.c | 分类: uboot
// 语义: 取当前以太网设备的 MAC 存储地址: 无当前设备返回 NULL,
//       否则返回 *(dev+0x08)+0x04 (设备私有结构内 MAC 字段地址, 布局未知)
void *eth_get_ethaddr(void)
{
    void *dev;
    int mac;

    dev = eth_get_dev();
    if (dev == NULL) {
        return NULL;
    }
    dev = eth_get_dev();            /* 原式连续两次调用 eth_get_dev, 原样保留 */
    mac = *(int *)((int)dev + 8) + 4;
    return (void *)mac;
}

// === eth_send === 原 FUN_0400929c @ 0x0400929c
// 来源: reversed/uboot_funcs/FUN_0400929c.c | 分类: uboot
// 语义: 经当前以太网设备发送报文: 设备不存在返回 -19 (-ENODEV),
//       设备未激活返回 -22 (-EINVAL), 否则经驱动回调表发送
// 注: Ghidra 将驱动函数指针误报为跳表 (WARNING: Treating indirect jump as call),
//     实为一次普通间接调用, 已按三级指针链转写
int eth_send(void *packet, int length)
{
    int *dev;
    int tbl_base;
    int (*send_fn)(int *dev, void *packet, int length);

    dev = (int *)eth_get_dev();
    if (dev == NULL) {
        return -0x13;               /* -ENODEV: 无当前设备 */
    }
    if ((dev[0x12] & 1U) == 0) {    /* 设备状态标志 bit0 = 未激活 */
        return -0x16;               /* -EINVAL */
    }
    /* 指针链: dev+0x00 → 表基址; *(表基址+0x3c) → 二级表; *(二级表+4) → send 回调 */
    tbl_base = *(int *)(*dev + 0x3c);
    send_fn = *(int (**)(int *, void *, int))(tbl_base + 4);
    return send_fn(dev, packet, length);
}

// === eth_rx === 原 FUN_040092ec @ 0x040092ec
// 来源: reversed/uboot_funcs/FUN_040092ec.c | 分类: uboot
// 语义: 轮询当前设备接收队列 (最多 0x20 次): recv 回调 (+8) 取包, 取到即回调
//       net_process_received_packet, 再经 free_pkt 回调 (+0xc) 归还缓冲;
//       recv 返回 -11 (-EAGAIN) 视为队列空返回 0
// 注: 原反编译形参 (param_1/param_2) 为寄存器残留, 实际为无参的 eth_rx(void);
//     recv 调用中 Ghidra 混入的多余实参 (回调指针自身等) 已剔除
int eth_rx(void)
{
    int *dev;
    int (*recv_fn)(int *dev, int flags, void **packet);
    int (*free_fn)(int *dev, void *packet, int len);
    void *packet;
    int ret;
    int tries;
    int flags;

    dev = (int *)eth_get_dev();
    if (dev == NULL) {
        return -0x13;               /* -ENODEV */
    }
    if ((dev[0x12] & 1U) == 0) {    /* 未激活 */
        return -0x16;               /* -EINVAL */
    }
    tries = 0x20;
    flags = 1;                      /* 首轮 flags=1, 之后为 0 */
    do {
        recv_fn = *(int (**)(int *, int, void **))(*(int *)(*dev + 0x3c) + 8);
        ret = recv_fn(dev, flags, &packet);
        if (ret >= 1) {
            net_process_received_packet(packet, ret);
        }
        free_fn = *(int (**)(int *, void *, int))(*(int *)(*dev + 0x3c) + 0xc);
        if (free_fn != NULL) {
            free_fn(dev, packet, ret);
        }
        if (ret < 1) {
            if (ret == -0xb) {      /* -EAGAIN: 队列空 */
                return 0;
            }
            return ret;
        }
        tries--;
        flags = 0;
    } while (tries != 0);
    return ret;
}

// === eth_getenv_enetaddr === 原 FUN_040095ac @ 0x040095ac
// 来源: reversed/uboot_funcs/FUN_040095ac.c | 分类: uboot
// 语义: 读取环境变量中的 MAC 地址: env_get(name) 后经 eth_parse_enetaddr
//       (FUN_04009550) 解析 (其返回值原式未使用); 首字节 bit0 置位视为
//       无效返回 0; 全零地址返回 0, 其余返回 1
int eth_getenv_enetaddr(const char *name, uint8_t *enetaddr)
{
    char *env;
    int valid;

    /* 命名待补 (用法 = env_get); 原式此处 Ghidra 未显示实参, 按同函数
     * 其他调用点 (eth_try_another/net_start_again 均传变量名) 还原为 name */
    env = fun_03fbefb0(name);
    eth_parse_enetaddr(env, enetaddr);
    if ((enetaddr[0] & 1) != 0) {
        return 0;
    }
    if ((enetaddr[0] != 0) || (enetaddr[1] != 0) || (enetaddr[2] != 0) ||
        (enetaddr[3] != 0) || (enetaddr[4] != 0) || (enetaddr[5] != 0)) {
        valid = 1;
    }
    if ((enetaddr[0] == 0) && (enetaddr[1] == 0) && (enetaddr[2] == 0) &&
        (enetaddr[3] == 0) && (enetaddr[4] == 0) && (enetaddr[5] == 0)) {
        valid = 0;                  /* 两条件互补, valid 必被赋值 (原式结构如此) */
    }
    return valid;
}

// === eth_setenv_enetaddr_by_index === 原 FUN_04009688 @ 0x04009688
// 来源: reversed/uboot_funcs/FUN_04009688.c | 分类: uboot
// 语义: 按设备序号拼出环境变量名 (index==0 用 DAT_040096d0 格式, 否则
//       DAT_040096cc 格式, 对应 "%saddr"/"%s%daddr") 并写入 MAC
void eth_setenv_enetaddr_by_index(const char *base_name, int index, const uint8_t *enetaddr)
{
    char enetvar[32];               /* 原 32 字节栈缓冲 */
    const char *fmt;

    if (index == 0) {
        fmt = DAT_040096d0;
    }
    else {
        fmt = DAT_040096cc;
    }
    ub_sprintf(enetvar, fmt, base_name, index);
    eth_setenv_enetaddr(enetvar, enetaddr);   /* FUN_04009608 */
}

// === eth_try_another === 原 FUN_040097e4 @ 0x040097e4
// 来源: reversed/uboot_funcs/FUN_040097e4.c | 分类: uboot
// 语义: 切换到下一个以太网设备重试: 环境变量 DAT_04009854 (疑似 "ethrotate")
//       存在且等于 DAT_04009860 (疑似 "no") 时不切换; first_route 非 0 时把
//       当前设备记为轮转起点 (DAT_04009858 槽), 切换后若绕回起点设备则置
//       绕回标志 *DAT_0400985c = 1
void eth_try_another(int first_route)
{
    char *env;
    int *first_dev_slot;
    void *dev;

    env = fun_03fbefb0(DAT_04009854);            /* env_get("ethrotate"), 命名待补 */
    if ((env != NULL) && (ub_strcmp(env, DAT_04009860) == 0)) {
        return;                                  /* 轮转被禁用 */
    }
    dev = eth_get_dev();
    first_dev_slot = DAT_04009858;               /* 全局: 轮转起点设备槽 */
    if (dev == NULL) {
        return;
    }
    if (first_route != 0) {
        dev = eth_get_dev();
        *first_dev_slot = (int)dev;              /* 记录当前设备为起点 */
    }
    eth_set_current_to_next();
    eth_current_changed();
    dev = eth_get_dev();
    if (*first_dev_slot == (int)dev) {
        *DAT_0400985c = 1;                       /* 已绕回起点设备 */
    }
    return;
}

// === net_clear_handlers === 原 FUN_040098f4 @ 0x040098f4
// 来源: reversed/uboot_funcs/FUN_040098f4.c | 分类: uboot
// 语义: 清空网络回调: 把空回调指针 DAT_04009918 (函数指针变量, 值读取)
//       写入 ARP/UDP/ICMP 三个处理槽, 并清零第四个槽
void net_clear_handlers(void)
{
    void *null_handler;

    null_handler = DAT_04009918;
    *DAT_0400991c = null_handler;
    *DAT_04009920 = null_handler;
    *DAT_04009924 = 0;
    return;
}

// === net_init_loop === 原 FUN_04009a00 @ 0x04009a00
// 来源: reversed/uboot_funcs/FUN_04009a00.c | 分类: uboot
// 语义: 网络模块初始化: 把当前设备的 MAC 存储地址 (eth_get_ethaddr) 起
//       6 字节拷入本机 MAC 缓存 DAT_04009a1c
void net_init_loop(void)
{
    void *mac;

    mac = eth_get_ethaddr();
    ub_memcpy(DAT_04009a1c, mac, 6);
    return;
}

// === net_start_again === 原 FUN_04009b00 @ 0x04009b00
// 来源: reversed/uboot_funcs/FUN_04009b00.c | 分类: uboot
// 语义: 网络重试入口: 按环境变量 DAT_04009c2c (疑似 "netretry", 取值
//       "yes"/"no"/"once"/十进制数字) 确定允许重试次数, 超过已用次数才重试;
//       重试时计数 +1、eth_halt、切换设备、重新 eth_init, 并按子状态标志
//       选定下一状态写入对应状态槽; 次数用尽则 eth_halt、状态槽置 3 并返回 -110
// 返回: eth_init 的返回值
int net_start_again(void)
{
    char *env;
    uint32_t *retry_cnt;
    uint32_t max_retry;
    int state;
    int *state_slot;
    int ret;

    env = fun_03fbefb0(DAT_04009c2c);            /* env_get("netretry"), 命名待补 */
    retry_cnt = DAT_04009c30;                    /* 全局: 重试计数器 */
    if (env == NULL) {
        max_retry = 0;                           /* 无该环境变量: 不重试 (原 0x04009b18 处) */
    }
    else {
        if (ub_strcmp(env, DAT_04009c38) == 0) {
            goto retry_always;                   /* "yes": 无限重试, 直达重试块 */
        }
        if (ub_strcmp(env, DAT_04009c3c) == 0) {
            max_retry = 0;                       /* "no" */
        }
        else if (ub_strcmp(env, DAT_04009c40) == 0) {
            max_retry = 1;                       /* "once" */
        }
        else {
            /* 原式 FUN_04007718(env,0) 仅两个可见实参, 进制参数被 Ghidra
             * 丢失, 按 U-Boot simple_strtoul 惯例还原为 10 */
            max_retry = (uint32_t)simple_strtoul(env, NULL, 10);
        }
    }
    if (max_retry <= *retry_cnt) {               /* 次数用尽 */
        eth_halt();
        *DAT_04009c34 = 3;                       /* 状态槽 = 3 (失败) */
        return -0x6e;                            /* -110 = ETIMEDOUT */
    }
retry_always:                                    /* 标签来源: 原标签位于 0x04009b94 */
    *retry_cnt = *retry_cnt + 1;
    eth_halt();
    eth_try_another(*DAT_04009c44 == 0);         /* 首轮标志为 0 时从当前设备起轮转 */
    ret = eth_init();
    if (*DAT_04009c48 == 0) {
        state = 1;
        state_slot = DAT_04009c34;
    }
    else {
        *DAT_04009c48 = 0;
        if (*DAT_04009c4c == 0) {
            state = 3;
            state_slot = DAT_04009c34;
        }
        else {
            *DAT_04009c50 = DAT_04009c54;
            state = ub_get_timer(0);             /* 原式 FUN_040061e4() 无可见实参, Ghidra 丢失, 按同类调用还原为 0 */
            *DAT_04009c58 = state;
            *DAT_04009c5c = 10000;
            state = DAT_04009c64;
            state_slot = DAT_04009c60;
        }
    }
    *state_slot = state;
    return ret;
}

// === net_get_arp_handler === 原 FUN_0400a02c @ 0x0400a02c
// 来源: reversed/uboot_funcs/FUN_0400a02c.c | 分类: uboot
// 语义: 返回当前 ARP 应答处理回调 (全局槽 DAT_0400a038)
void *net_get_arp_handler(void)
{
    return *DAT_0400a038;
}

// === net_set_timeout_handler === 原 FUN_0400a04c @ 0x0400a04c
// 来源: reversed/uboot_funcs/FUN_0400a04c.c | 分类: uboot
// 语义: 设置网络超时: 记录回调至 DAT_0400a098 槽, 记起始时刻, 并按全局系数
//       DAT_0400a0a0 把 毫秒*1000 换算为截止值 (>>0x26) 存 DAT_0400a0a4;
//       delay_ms==0 表示取消超时 (仅清回调槽)
// 返回: 换算式低 32 位 (原式 (int)lVar1)
int net_set_timeout_handler(uint32_t delay_ms, void *handler)
{
    uint64_t product;

    if (delay_ms == 0) {
        *DAT_0400a098 = 0;                       /* 清回调槽 */
        return 0;
    }
    *DAT_0400a098 = handler;
    *DAT_0400a09c = (uint32_t)ub_get_timer(0);   /* 起始时刻 */
    product = (uint64_t)DAT_0400a0a0 * (uint64_t)(uint32_t)(delay_ms * 1000);
    *DAT_0400a0a4 = (uint32_t)(product >> 0x26); /* 截止值 */
    return (int)product;
}

// === net_set_udp_header === 原 FUN_0400a5c0 @ 0x0400a5c0
// 来源: reversed/uboot_funcs/FUN_0400a5c0.c | 分类: uboot
// 语义: 在 IP 头起点 pkt 上续填 UDP 头并补齐 IP 字段: 净荷奇长度时在尾部
//       (pkt+净荷+0x1c) 填 0; net_set_ip_header 填源/目的 IP; IP 总长
//       (=净荷+0x1c, 即 IP 20 + UDP 8)、协议 0x11 (UDP)、头校验和;
//       UDP 源口(+0x14)/目的口(+0x16)/长度(=净荷+8)/校验和(+0x1a)=0;
//       各 16 位字段均按大端 (字节交换) 写入
void net_set_udp_header(uint8_t *pkt, uint32_t dest_ip, uint16_t dst_port, uint16_t src_port,
                        uint32_t payload_len)
{
    uint16_t cksum;
    uint32_t udp_len;
    uint32_t ip_total;

    if ((payload_len & 1) != 0) {
        pkt[payload_len + 0x1c] = 0;             /* 奇长度: 尾部填 0 */
    }
    net_set_ip_header(pkt, dest_ip, *DAT_0400a640);   /* 源 IP = 本机地址 */
    ip_total = (payload_len & 0xffff) + 0x1c;    /* IP 总长 = 净荷 + IP(20) + UDP(8) */
    *(uint16_t *)(pkt + 2) = (uint16_t)((ip_total & 0xff) << 8) | (uint16_t)(ip_total >> 8) & 0xff;
    pkt[9] = 0x11;                               /* IP 协议 = UDP (17) */
    udp_len = (payload_len & 0xffff) + 8;        /* UDP 长度 = 净荷 + UDP 头 */
    cksum = compute_ip_checksum((uint16_t *)pkt, 0x14);
    *(uint16_t *)(pkt + 10) = cksum;             /* IP 头校验和 */
    *(uint16_t *)(pkt + 0x14) = (uint16_t)((src_port & 0xff) << 8) | (uint16_t)(src_port >> 8) & 0xff;
    *(uint16_t *)(pkt + 0x16) = (uint16_t)((dst_port & 0xff) << 8) | (uint16_t)(dst_port >> 8) & 0xff;
    *(uint16_t *)(pkt + 0x18) = (uint16_t)((udp_len & 0xff) << 8) | (uint16_t)(udp_len >> 8) & 0xff;
    *(uint16_t *)(pkt + 0x1a) = 0;               /* UDP 校验和 = 0 (不计算) */
    return;
}

// === copy_filename === 原 FUN_0400a748 @ 0x0400a748
// 来源: reversed/uboot_funcs/FUN_0400a748.c | 分类: uboot
// 语义: 拷贝文件名: 首字符为引号则跳过 (额度 -1), 随后逐字符拷贝到 dst,
//       遇引号或 NUL 截断; 最多拷 len-1 字符, 末尾补 NUL
void copy_filename(char *dst, char *src, int len)
{
    char c;

    if (*src == '"') {
        src++;
        len--;
    }
    src--;                                       /* 预减, 循环体内先自增再取字符 */
    while (len--, 0 < len) {
        src++;
        c = *src;
        if ((c == '"') || (c == '\0')) {
            break;
        }
        *dst = c;
        dst++;
    }
    *dst = '\0';
    return;
}

// === nfs_read_req === 原 FUN_0400aff0 @ 0x0400aff0
// 来源: reversed/uboot_funcs/FUN_0400aff0.c | 分类: uboot
// 语义: 构造并发送 NFS READ 请求: RPC 凭证后接 32 字节文件句柄
//       (DAT_0400b060), 依次大端写入读取偏移 (+0x20)、0 (+0x28)、读取长度
//       (+0x24), 再以 rpc_req (程序号 DAT_0400b064, 过程号 6) 按字长发出
void nfs_read_req(uint32_t offset, uint32_t length)
{
    uint8_t buf[4100];               /* 原 4100 字节栈缓冲 */
    uint8_t *p;

    p = (uint8_t *)rpc_add_credentials(buf);     /* 返回凭证区之后的写入位置 */
    ub_memcpy(p, DAT_0400b060, 0x20);            /* 文件句柄 (NFS_FHSIZE = 32) */
    *(uint32_t *)(p + 0x20) = offset << 0x18 | (offset >> 8 & 0xff) << 0x10 |
                              (offset >> 0x10 & 0xff) << 8 | offset >> 0x18;   /* 大端偏移 */
    *(uint32_t *)(p + 0x28) = 0;
    *(uint32_t *)(p + 0x24) = length << 0x18 | (length >> 8 & 0xff) << 0x10 |
                              (length >> 0x10 & 0xff) << 8 | length >> 0x18;   /* 大端长度 */
    rpc_req(DAT_0400b064, 6, buf, ((int)(p + 0x2c) - (int)buf) >> 2);   /* 长度按 32 位字计 */
    return;
}

// === ping_receive === 原 FUN_0400b900 @ 0x0400b900
// 来源: reversed/uboot_funcs/FUN_0400b900.c | 分类: uboot
// 语义: ICMP 报文处理: 收到 echo 应答 (type 0) 且源 IP 匹配待测地址
//       DAT_0400b9d4 时置状态槽 *DAT_0400b9d8 = 2; 收到 echo 请求 (type 8)
//       则原地改写收包为应答帧回发: net_update_ether 交换 MAC (目的←帧+6),
//       IP 分片字段清零, IP 收发地址互换 (源←本机 DAT_0400b9dc), 重算 IP
//       头校验和 (0x14 字节), ICMP type 清 0 并重算其校验和 (余下报文)
// 参数: pkt = 以太帧基址, ip = IP 头基址, len = IP 总长
void ping_receive(void *pkt, uint8_t *ip, int len)
{
    int eth_hdr_len;
    uint32_t src_ip;
    uint16_t cksum;

    if (*(char *)(ip + 0x14) == 0) {             /* ICMP type 0: echo 应答 */
        ub_memcpy(&src_ip, ip + 0xc, 4);         /* IP 源地址 (+0x0c) */
        if (*DAT_0400b9d4 == src_ip) {           /* 与待 ping 目标一致 */
            *DAT_0400b9d8 = 2;                   /* 状态 = 2 (收到应答) */
        }
    }
    else if (*(char *)(ip + 0x14) == '\b') {     /* ICMP type 8: echo 请求 → 构造应答 */
        eth_hdr_len = net_update_ether(pkt, (char *)pkt + 6, 0x800);   /* 目的 MAC ← 帧内源 MAC, 协议 IP */
        *(uint16_t *)(ip + 10) = 0;              /* IP 校验和清零 */
        *(uint16_t *)(ip + 6) = 0;               /* IP 分片字段清零 */
        ub_memcpy(ip + 0x10, ip + 0xc, 4);       /* IP 目的 ← 原 IP 源 */
        ub_memcpy(ip + 0xc, DAT_0400b9dc, 4);    /* IP 源 ← 本机地址 */
        cksum = compute_ip_checksum((uint16_t *)ip, 0x14);
        *(uint16_t *)(ip + 10) = cksum;          /* 重填 IP 校验和 */
        *(char *)(ip + 0x14) = 0;                /* ICMP type → 0 (echo 应答) */
        *(uint16_t *)(ip + 0x16) = 0;            /* ICMP 校验和清零 */
        cksum = compute_ip_checksum((uint16_t *)(ip + 0x14), len - 0x14);
        *(uint16_t *)(ip + 0x16) = cksum;        /* 重填 ICMP 校验和 */
        eth_send(pkt, eth_hdr_len + len);
    }
    return;
}

// === ub_un_00c65c === 原 FUN_0400c65c @ 0x0400c65c
// 来源: reversed/uboot_funcs/FUN_0400c65c.c | 分类: uboot
// 语义: 未编目小函数 (name_map 标记 fallback, 无命名依据): 调用外部
//       fun_03fd17b4 (命名待补) 后固定返回 0
int ub_un_00c65c(void)
{
    fun_03fd17b4();
    return 0;
}

// === aeabi_idiv === 原 FUN_0400c708 @ 0x0400c708
// 来源: reversed/uboot_funcs/FUN_0400c708.c | 分类: uboot
// 语义: __aeabi_idiv 有符号整数除法 (商): 除数为 0 先进 ub_div0_hang 挂死
//       (其后的 return 0 不可达, 原样保留); 其余按符号处理 + 逐位恢复法求商,
//       LZCOUNT 对应 ARM CLZ 指令 (转写为 __builtin_clz)
uint32_t aeabi_idiv(uint32_t num, uint32_t denom)
{
    uint32_t sign_xor;
    uint32_t abs_den;
    uint32_t abs_num;
    uint32_t quotient;
    uint32_t shifted_den;
    uint32_t bit;

    sign_xor = num ^ denom;                      /* 结果符号因子 */
    if (denom == 0) {
        ub_div0_hang();                          /* 原 FUN_04004844 跳板: 除 0 挂死 */
        return 0;
    }
    abs_den = denom;
    if ((int)denom < 0) {
        abs_den = -denom;
    }
    if (abs_den - 1 == 0) {                      /* 除数为 ±1 */
        if ((int)denom < 0) {
            num = -num;
        }
        return num;
    }
    abs_num = num;
    if ((int)num < 0) {
        abs_num = -num;
    }
    if (abs_num <= abs_den) {                    /* 商为 0 或 ±1 */
        if (abs_num < abs_den) {
            num = 0;
        }
        if (abs_num == abs_den) {
            num = ((int)sign_xor >> 0x1f) | 1;   /* 按结果符号取 ±1 */
        }
        return num;
    }
    if ((abs_den & (abs_den - 1)) == 0) {        /* 除数为 2^n: 直接移位 */
        quotient = abs_num >> ((0x1f - __builtin_clz(abs_den)) & 0xff);
        if ((int)sign_xor < 0) {
            quotient = -quotient;
        }
        return quotient;
    }
    shifted_den = abs_den << ((__builtin_clz(abs_den) - __builtin_clz(abs_num)) & 0xff);
    bit = 1u << ((__builtin_clz(abs_den) - __builtin_clz(abs_num)) & 0xff);
    quotient = 0;
    for (;;) {
        if (shifted_den <= abs_num) {            /* 4 步一轮的恢复法主体 */
            abs_num = abs_num - shifted_den;
            quotient = quotient | bit;
        }
        if ((shifted_den >> 1) <= abs_num) {
            abs_num = abs_num - (shifted_den >> 1);
            quotient = quotient | bit >> 1;
        }
        if ((shifted_den >> 2) <= abs_num) {
            abs_num = abs_num - (shifted_den >> 2);
            quotient = quotient | bit >> 2;
        }
        if ((shifted_den >> 3) <= abs_num) {
            abs_num = abs_num - (shifted_den >> 3);
            quotient = quotient | bit >> 3;
        }
        if (abs_num == 0) {
            break;
        }
        bit = bit >> 4;
        if (bit == 0) {
            break;
        }
        shifted_den = shifted_den >> 4;
    }
    if ((int)sign_xor < 0) {
        quotient = -quotient;                    /* 结果取负 */
    }
    return quotient;
}

/* ============================================================================
 * 覆盖率表 — batch=uboot_b2 后 24/24 函数 (来源 reversed/uboot_funcs/,
 * 行数为 batch_plan_other.tsv 登记的反编译行数)
 * ==========================================================================
 *  语义名                        原地址       原函数名        反编译行数
 *  ub_panic_reset                0x040076b8  FUN_040076b8    13
 *  parse_size_suffix             0x040078a8  FUN_040078a8    61 (Ghidra 坏指令流, 依 case 分组重建)
 *  compute_ip_checksum           0x04007c5c  FUN_04007c5c    22
 *  arp_timeout_check             0x04007e6c  FUN_04007e6c    32
 *  dhcp_process_options          0x040082e4  FUN_040082e4    102
 *  dhcp_handler                  0x0400855c  FUN_0400855c    132
 *  eth_get_dev_by_name           0x04009008  FUN_04009008    49
 *  eth_get_ethaddr               0x040090f8  FUN_040090f8    15
 *  eth_send                      0x0400929c  FUN_0400929c    21
 *  eth_rx                        0x040092ec  FUN_040092ec    50
 *  eth_getenv_enetaddr           0x040095ac  FUN_040095ac    25
 *  eth_setenv_enetaddr_by_index  0x04009688  FUN_04009688    19
 *  eth_try_another               0x040097e4  FUN_040097e4    30
 *  net_clear_handlers            0x040098f4  FUN_040098f4    14
 *  net_init_loop                 0x04009a00  FUN_04009a00    12
 *  net_start_again               0x04009b00  FUN_04009b00    65
 *  net_get_arp_handler           0x0400a02c  FUN_0400a02c    8
 *  net_set_timeout_handler       0x0400a04c  FUN_0400a04c    20
 *  net_set_udp_header            0x0400a5c0  FUN_0400a5c0    25
 *  copy_filename                 0x0400a748  FUN_0400a748    23
 *  nfs_read_req                  0x0400aff0  FUN_0400aff0    21
 *  ping_receive                  0x0400b900  FUN_0400b900    35
 *  ub_un_00c65c                  0x0400c65c  FUN_0400c65c    9
 *  aeabi_idiv                    0x0400c708  FUN_0400c708    81
 * ========================================================================== */

