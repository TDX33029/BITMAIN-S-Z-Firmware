// ============================================================================
// uboot/p3b.c — U-Boot 2016.07 反编译转写 (半批 uboot_b3 后 23 函数)
// 地址范围: 0x04007884 .. 0x0400c7d4
// 命名事实源: rewritten/name_map/uboot.tsv
// 语义规则: rewritten/CONVENTIONS.md
// 上游参照: U-Boot v2016.07 lib/strto.c, net/{net,arp,bootp,eth-uclass,eth_common}.c,
//           net/{nfs,tftp}.c, include/{net,bootstage}.h (用于核对语义, 非事实源)
// ============================================================================

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* ctype 标志表 (linux/ctype 布局: _U=0x01 _L=0x02 _D=0x04 _X=0x40; ELF .rodata 数据表) */
extern const uint8_t DAT_04007ac0[];

/* ARP/发送缓冲全局槽 (arp_init 清零; 反编译为 *DAT_xxx = ... 指针解引用写) */
extern uint32_t *DAT_04007d1c;   /* arp_wait_packet_ethaddr 指针槽 */
extern uint32_t *DAT_04007d20;   /* net_arp_wait_packet_ip 槽 */
extern uint32_t *DAT_04007d24;   /* net_arp_wait_reply_ip 槽 */
extern uint32_t *DAT_04007d28;   /* arp_wait_tx_packet_size 槽 */
extern uint32_t *DAT_04007d30;   /* arp_tx_packet 指针槽 */
extern uint32_t  DAT_04007d2c;   /* 发送缓冲区地址 +31 对齐常量 */

/* DHCP/BOOTP 全局 (dhcp_extended / dhcp_receive 用; 反编译用法为准) */
extern char     DAT_040082d8[];  /* env 变量名 (clientarch 相关, 语义待定) */
extern char     DAT_040082dc[];  /* 默认 vendor class 字符串 */
extern char     DAT_040082e0[];  /* env 变量名 (vendor class, 疑似 "bootp_vci") */
extern uint32_t DAT_04008554;    /* BOOTP vendor magic 比较值 (0x63825363) */
extern uint8_t *DAT_04008558;    /* 指向 dhcp_option_overload 字节的指针 */

/* 以太网设备轮换全局 (net_loop/eth 相关; 反编译用法为准) */
extern char     DAT_0400924c[];  /* env 名 (疑似 "ethact") */
extern char     DAT_04009250[];  /* env 名 (疑似 "ethrotate") */
extern char     DAT_04009254[];  /* 禁用轮转比较值 (疑似 "no") */
extern char     DAT_04009258[];  /* 提示串 ("No ethernet found.\n") */
extern int     *DAT_0400925c;    /* 指向 eth_errno 的指针 */

/* eth_init/eth_initialize 全局 (反编译用法为准) */
extern char     DAT_04009534[];  /* 提示串 ("No ethernet found.\n") */
extern char     DAT_04009538[];  /* env 名 (疑似 "ethprime") */
extern char     DAT_0400953c[];  /* 格式串 ("eth%d: %s") */
extern char     DAT_04009540[];  /* 提示串 (" [PRIME]") */
extern char     DAT_04009544[];  /* 格式串 ("\nWarning: %s failed to set MAC address\n") */
extern char     DAT_0400954c[];  /* 格式串 ("\nError: %s address %pM illegal value\n") */
extern char     DAT_04009548[];  /* 分隔串 (", ") */
extern char     DAT_04009724[];  /* 格式串 ("ethmacskip") */
extern char     DAT_04009728[];  /* 格式串 ("eth%dmacskip") */
extern char     DAT_040097d8[];  /* env 名 ("ethact") */
extern char     DAT_040097dc[];  /* env 名 ("ethrotate") */
extern char     DAT_040097e0[];  /* 禁用轮转比较值 ("no") */
extern int     *DAT_04009af0;    /* 指向 first_call 静态局部 (提升为全局) */
extern uint32_t DAT_04009af4;    /* net_tx_packet 地址 (对齐取 &  ~0x1f) */
extern int     *DAT_04009afc;    /* 指向 net_rx_packets[0] */
extern uint32_t *DAT_04009af8;    /* net_tx_packet 槽指针 */
extern uint32_t *DAT_0400a024;    /* udp_packet_handler 槽指针 */
extern uint32_t  DAT_0400a028;    /* dummy_handler 地址值 */
extern uint32_t *DAT_0400a3bc;    /* net_rx_packet 槽指针 */
extern uint32_t *DAT_0400a3c0;    /* net_rx_packet_len 槽指针 */
extern void     *DAT_0400a3c4;    /* push_packet (CONFIG_API) 函数指针槽 */
extern void     *DAT_0400a3c8;    /* net_our_vlan 槽 (ntohs 取半字) */
extern void     *DAT_0400a3cc;    /* net_native_vlan 槽 */
extern void     *DAT_0400a3d0;    /* net_rx_packet 目标 IP 槽 */
extern char     DAT_0400a3d4[];   /* 格式串 (" ICMP Host Redirect to %pI4 ") */
extern void     *DAT_0400a3d8;    /* packet_icmp_handler 槽 (CONFIG_CMD_TFTPPUT) */
extern void     *DAT_0400a3dc;    /* packet_handler 槽 (ICMP 处理函数指针) */
extern void     *DAT_0400a418;    /* net_our_vlan 槽 (重复使用) */
extern char      DAT_0400a530[];  /* net_ethaddr[6] 字节数组 */
extern uint32_t  DAT_0400a534;    /* htons 掩码 0xff00 常量 */
extern uint32_t  DAT_0400a5b4;    /* IP 头首字常量 (0x45|htons(20) 编译器合并) */
extern uint32_t *DAT_0400a5b8;    /* net_ip_id 槽指针 */
extern uint32_t  DAT_0400a5bc;    /* ip_id 掩码 0xff00 常量 */
extern uint32_t *DAT_0400a728;    /* &net_tx_packet 槽指针 */
extern uint8_t  *DAT_0400a72c;    /* net_bcast_ethaddr 指针槽 */
extern unsigned char DAT_0400a730[];  /* 广播 MAC 字节数组 (memcmp 基准) */
extern uint32_t *DAT_0400a734;    /* net_arp_wait_packet_ip 槽指针 */
extern uint32_t *DAT_0400a738;    /* arp_wait_packet_ethaddr 槽指针 */
extern uint32_t *DAT_0400a73c;    /* arp_wait_tx_packet_size 槽指针 */
extern uint32_t *DAT_0400a740;    /* arp_wait_try 槽指针 */
extern uint32_t *DAT_0400a744;    /* arp_wait_timer_start 槽指针 */
extern char      DAT_0400ad44[];  /* hostname 默认串 (固件定制, 空串) */
extern uint32_t *DAT_0400b1d8;    /* &rpc_id 槽指针 */
extern int      *DAT_0400b1dc;    /* &nfs_offset 槽指针 */
extern uint32_t  DAT_0400b1f0;    /* NFS 出错返回值 (-9999) */
extern int       DAT_0400b1e0;    /* 进度打印周期 (NFS_READ_SIZE/2*10 = 0x1400) */
extern char      DAT_0400b1e4[];  /* puts 串 ("
	 ") */
extern int      *DAT_0400b1e8;    /* &load_addr 槽指针 */
extern uint32_t *DAT_0400b1ec;    /* &net_boot_file_size 槽指针 */
extern int      *DAT_0400ba40;    /* &time_start 槽指针 */
extern char      DAT_0400ba44[];  /* puts 串 ("
	 ") */
extern int      *DAT_0400ba48;    /* &net_boot_file_size 槽指针 */
extern char      DAT_0400ba4c[];  /* print_size 后缀串 ("/s") */
extern char      DAT_0400ba50[];  /* puts 串 ("
done
") */
extern uint32_t *DAT_0400ba54;    /* net_state 槽指针 (NETLOOP_SUCCESS=2) */

/* ----------------------------------------------------------------------------
 * 模块内部前向声明区
 * (被调函数定义于本库其他 p*.c 文件, 名字取自 name_map/uboot.tsv)
 * -------------------------------------------------------------------------- */
long simple_strtoul(char *cp, char **endp, unsigned int base);          /* FUN_04007718, p2b */
unsigned int compute_ip_checksum(unsigned short *addr, unsigned int nbytes); /* FUN_04007c5c, p2b */
void dhcp_process_options(unsigned char *popt, unsigned char *end);     /* FUN_040082e4, p2b */
void arp_init(void);                                                    /* 本文件 FUN_04007ce4 */
void arp_request(void);                                                 /* FUN_04007de8, p2b */
int net_set_ether(void *et, unsigned char *addr, unsigned int prot);    /* FUN_0400a41c, p2b */
void net_set_udp_header(void *pkt, uint32_t dest, int dport, int sport, int payload_len); /* FUN_0400a5c0, p2b */
unsigned int ub_get_timer(unsigned int base);                           /* FUN_040061e4, p1 */
int ub_memcmp(const void *a, const void *b, unsigned int n);            /* FUN_04005ff4, p2 */
int ub_strcmp(const char *a, const char *b);                            /* FUN_04005a60, p2 */
int ub_strlen(const char *s);                                           /* FUN_04005b98, p2 */
void ub_memcpy(void *dst, const void *src, unsigned int n);             /* FUN_04005ea4, p2 */
void ub_strcpy(char *dst, const char *src);                             /* FUN_04005994, p2 */
void ub_sprintf(char *buf, const char *fmt, ...);                       /* FUN_04007514, p1 */
int ub_printf(const char *fmt, ...);                                    /* FUN_04007548, p1 */
void ub_print_u64(uint32_t lo, uint32_t hi, const char *suffix);        /* FUN_040042ac, p2 */
unsigned int aeabi_uidiv(unsigned int dividend, unsigned int divisor);  /* FUN_0400c66c, p2 */
void eth_common_init(void);                                             /* FUN_040096d4, p1 */
void eth_set_dev(void *dev);                                            /* FUN_04008fa0, p1 */
void *eth_get_dev(void);                                                /* FUN_04008f68, p1 */
void *eth_get_dev_by_name(const char *name);                            /* FUN_04009008, p2 */
void eth_try_another(int fallback);                                     /* FUN_040097e4, p2 */
char *eth_get_name(void);                                               /* FUN_0400972c, p1 */
void net_clear_handlers(void);                                          /* FUN_040098f4, p1 */
void net_init_loop(void);                                               /* FUN_04009a00, p1 */
int eth_send(void *pkt, int len);                                       /* FUN_0400929c, p2 */
void arp_receive(void *et, void *ip, int len);                          /* FUN_04007efc, p1 */
int ping_receive(void *pkt, void *ip, unsigned int len);                /* FUN_0400b900, p1 */
void ub_div0_hang(void);                                                /* FUN_04004844, p2 */

/* 外部调用 (PLT/未命名, 命名待补): 按 v2016.07 调用点语义推断为 env 函数 */
extern char *func_0x03fbefb0(const char *name);                         /* getenv */
extern unsigned long func_0x03fbf064(const char *name, int base, unsigned long def); /* getenv_ulong */
extern void func_0x03fc7f78(int ch);                                    /* putc */
extern void func_0x03fc7fbc(const char *s);                             /* puts */
extern void func_0x03fbf43c(int progress);                              /* show_boot_progress */
extern void func_0x03fd1098(int uclass, void **devp);                   /* uclass_first_device */
extern void func_0x03fd10d0(void **devp);                               /* uclass_next_device */
extern int setenv(const char *name, const char *value);                 /* 仅出现于 eth_current_changed 截断尾部, 按上游恢复 */

// ============================================================================
// 函数转写区 (按地址升序)
// ============================================================================

// === simple_strtol === 原 FUN_04007884 @ 0x04007884
// 来源: reversed/uboot_funcs/FUN_04007884.c | 分类: uboot
// 语义: 带符号字符串转 long (上游 lib/strto.c simple_strtol)。
//       '-' 前缀时对 simple_strtoul(cp+1) 结果取负; 否则原样转换。
//       Ghidra 反编译仅显示首参数, 其余参数经寄存器直传 simple_strtoul。
long simple_strtol(char *cp, char **endp, unsigned int base)
{
	long ret;

	if (*cp == '-') {
		ret = -(long)simple_strtoul(cp + 1, endp, base);
		return ret;
	}

	ret = (long)simple_strtoul(cp, endp, base);	/* 实参被 Ghidra 丢弃, 按 strto.c 语义恢复 */
	return ret;
}

// === simple_strtoull === 原 FUN_0400799c @ 0x0400799c
// 来源: reversed/uboot_funcs/FUN_0400799c.c | 分类: uboot
// 语义: 无符号字符串转 64 位整数 (上游 lib/strto.c simple_strtoull)。
//       "0x"+十六进制数字 → base 16; "0" 前缀且 base 未给 → base 8;
//       其余 base 未给 → base 10。无效数字停止解析, *endp 指向停止处。
//       DAT_04007ac0 为 linux/ctype 标志表 (_U=0x01 _L=0x02 _D=0x04 _X=0x40)。
unsigned long long simple_strtoull(char *cp, char **endp, unsigned int base)
{
	unsigned long long result = 0;
	unsigned long long value;

	if (*cp == '0') {
		cp++;
		if (*cp == 'x' && (DAT_04007ac0[(unsigned char)cp[1]] & 0x44) != 0) {	/* isxdigit(cp[1]) */
			base = 16;
			cp++;
		}
		if (base == 0)
			base = 8;
	}
	if (base == 0)
		base = 10;

	/* 反编译将 64 位累加拆成低 32 位/高 32 位两路乘加 (低字寄存器/高字寄存器),
	 * 语义与 result = result * base + value 完全等价 */
	for (;;) {
		unsigned char flags = DAT_04007ac0[(unsigned char)*cp];
		if ((flags & 0x44) == 0)			/* !isxdigit(*cp) */
			break;
		if (flags & 0x04)				/* isdigit → c - '0' */
			value = (unsigned long long)(*cp - '0');
		else if (flags & 0x02)				/* islower → toupper(c) - 'A' + 10 */
			value = (unsigned long long)((*cp - 0x20) - 0x37);
		else						/* 大写 → c - 'A' + 10 */
			value = (unsigned long long)(*cp - 0x37);
		if ((int)value < 0 || (unsigned int)value >= base)
			break;
		result = result * base + value;
		cp++;
	}

	if (endp != NULL)
		*endp = cp;
	return result;
}

// === ip_checksum_ok === 原 FUN_04007cc8 @ 0x04007cc8
// 来源: reversed/uboot_funcs/FUN_04007cc8.c | 分类: uboot
// 语义: IP 头校验和检查 (上游 include/net.h static inline)。
//       对 compute_ip_checksum 的 16 位结果做 "(sum & 0xfffe) == 0" 判断,
//       即合法头补和为 0 或 0xffff。Ghidra 未显示参数 (r0/r1 直传)。
int ip_checksum_ok(void *packet, int length)
{
	unsigned int sum = compute_ip_checksum((unsigned short *)packet, (unsigned int)length);
	return (sum & 0xfffe) == 0;
}

// === arp_init === 原 FUN_04007ce4 @ 0x04007ce4
// 来源: reversed/uboot_funcs/FUN_04007ce4.c | 分类: uboot
// 语义: 复位 ARP 等待状态 (上游 net/arp.c arp_init):
//       等待应答 MAC / 等待包 IP / 应答 IP / 等待包长度清零;
//       arp_tx_packet 指向 arp_tx_packet_buf 按 PKTALIGN(32) 对齐处
//       ("ptr+(PKTALIGN-1)-ptr%PKTALIGN" 被编译器折叠为 ptr & ~0x1f)。
void arp_init(void)
{
	*DAT_04007d1c = 0;			/* arp_wait_packet_ethaddr = NULL (全局, 语义待定) */
	*DAT_04007d20 = 0;			/* net_arp_wait_packet_ip = 0 (全局, 语义待定) */
	*DAT_04007d24 = 0;			/* net_arp_wait_reply_ip = 0 (全局, 语义待定) */
	*DAT_04007d28 = 0;			/* arp_wait_tx_packet_size = 0 (全局, 语义待定) */
	*DAT_04007d30 = DAT_04007d2c & 0xffffffe0;	/* arp_tx_packet 对齐; DAT_04007d2c 为 &buf+31 常量 */
}

// === dhcp_extended === 原 FUN_04008110 @ 0x04008110
// 来源: reversed/uboot_funcs/FUN_04008110.c | 分类: uboot
// 语义: 构造 DHCP 扩展选项区 (上游 net/bootp.c dhcp_extended, 固件定制版):
//       RFC1048 魔数 + 选项 53(消息类型)/57(最大包长 576)/54(ServerID)/
//       50(Requested IP)/93(客户端架构, env bootp_arch 十六进制, 默认 0x15)/
//       94(Client NDI)/60(Vendor Class, env 或默认串)/55(参数请求: 3,12,17)/
//       0xff(结束), 最后补零至区内偏移 0x40 (CONFIG_DHCP_MIN_EXT_LEN)。
//       注意: 54/50 的 IP 字节序为低字节在前, 与上游 ntohl 大端写法相反
//       (按反编译原样保留, 调用方可能传入预交换值)。
//       返回写入总长度。反编译中写指针初值 0x53 的分支为死路径寄存器残留, 已消除。
int dhcp_extended(unsigned char *e, int message_type, uint32_t server_id, uint32_t requested_ip)
{
	unsigned char *start = e;
	unsigned char *p;
	int clientarch;
	char *vci;
	int vci_len;

	*p++ = 99;				/* RFC1048 Magic Cookie: 99.130.83.99 */
	*p++ = 130;
	*p++ = 83;
	*p++ = 99;

	*p++ = 53;				/* DHCP Message Type */
	*p++ = 1;
	*p++ = (unsigned char)message_type;

	*p++ = 57;				/* Maximum DHCP Message Size = 576 (0x240) */
	*p++ = 2;
	*p++ = 2;
	*p++ = 0x40;

	if (server_id != 0) {
		*p++ = 54;			/* Server Identifier */
		*p++ = 4;
		p[0] = (unsigned char)server_id;		/* 固件按小端字节序写入 (上游为 ntohl 大端) */
		p[1] = (unsigned char)(server_id >> 8);
		p[2] = (unsigned char)(server_id >> 16);
		p[3] = (unsigned char)(server_id >> 24);
		p += 4;
	}

	if (requested_ip != 0) {
		*p++ = 50;			/* Requested IP */
		*p++ = 4;
		p[0] = (unsigned char)requested_ip;		/* 同上: 小端写入 */
		p[1] = (unsigned char)(requested_ip >> 8);
		p[2] = (unsigned char)(requested_ip >> 16);
		p[3] = (unsigned char)(requested_ip >> 24);
		p += 4;
	}

	/* 选项 93: Client System Architecture。
	 * env "bootp_arch" 未设 → 默认 0x15(21); 已设 → getenv_ulong(name,16,0x15) 解析,
	 * 解析值 < 1 时跳过该选项 (等价上游 "if (clientarch > 0) 写") */
	clientarch = 0x15;			/* CONFIG_BOOTP_PXE_CLIENTARCH */
	if (func_0x03fbefb0(DAT_040082d8) != NULL) {
		clientarch = (int)func_0x03fbf064(DAT_040082d8, 0x10, 0x15);
		if (clientarch < 1)
			goto no_arch_option;	/* 原标签 04008208 */
	}
	*p++ = 93;				/* Client System Architecture */
	*p++ = 2;
	*p++ = (unsigned char)(clientarch >> 8);
	*p++ = (unsigned char)clientarch;
no_arch_option:

	*p++ = 94;				/* Client Network Interface Identifier (UNDI) */
	*p++ = 3;
	*p++ = 1;				/* type */
	*p++ = 0;				/* major revision */
	*p++ = 0;				/* minor revision */

	/* 选项 60: Vendor Class Identifier — env(DAT_040082e0) 优先, 否则默认串 DAT_040082dc。
	 * 上游 add_vci 无默认串, 此为固件定制 */
	vci = DAT_040082dc;			/* 默认 vendor class 字符串 (全局, 语义待定) */
	vci_len = 0;
	if (func_0x03fbefb0(DAT_040082e0) != NULL)
		vci = func_0x03fbefb0(DAT_040082e0);	/* 上游 getenv("bootp_vci") */
	vci_len = ub_strlen(vci);
	*p++ = 60;				/* Vendor Class Identifier */
	*p++ = (unsigned char)vci_len;
	ub_memcpy(p, vci, (unsigned int)vci_len);
	p += vci_len;

	*p++ = 55;				/* Parameter Request List */
	*p++ = 3;				/* 3 项 (编译期固定, 对应上游 BOOTP_GATEWAY/HOSTNAME/BOOTPATH) */
	*p++ = 3;				/* 3 = Router */
	*p++ = 12;				/* 12 = Hostname */
	*p++ = 17;				/* 17 = Boot path */
	*p++ = 0xff;				/* End of the list */

	while ((unsigned int)(p - start) < 0x40)	/* CONFIG_DHCP_MIN_EXT_LEN = 64 */
		*p++ = 0;

	return (int)(p - start);
}

// === dhcp_packet_process_options === 原 FUN_040084d8 @ 0x040084d8
// 来源: reversed/uboot_funcs/FUN_040084d8.c | 分类: uboot
// 语义: 解析 DHCP 报文选项 (上游 net/bootp.c dhcp_packet_process_options):
//       校验 bp_vend 魔数 (BOOTP_VENDOR_MAGIC 0x63825363, 4 字节拷贝比较) 后
//       处理 vend 区选项; 若 overload 标志置位, 再按 file/sname 区解析。
//       struct bootp_hdr 布局: sname@0x2C(64B) file@0x6C(128B) vend@0xEC;
//       end = popt + BOOTP_HDR_SIZE(0x224) — 与上游源码一致 (end 越过 vend 区, 上游原样如此)。
//       Ghidra 多出的 param_2..4 为寄存器残留, 实际单参数。
void dhcp_packet_process_options(void *bp)
{
	uint32_t magic;
	unsigned char *overload_p;

	ub_memcpy(&magic, (unsigned char *)bp + 0xec, 4);	/* &bp->bp_vend[0], net_read_u32 内联 */
	if (magic != DAT_04008554)			/* DAT_04008554 = BOOTP_VENDOR_MAGIC 0x63825363 比较值 (全局, 语义待定) */
		return;

	overload_p = DAT_04008558;			/* &dhcp_option_overload (全局字节) */
	*overload_p = 0;				/* dhcp_option_overload = 0 */

	dhcp_process_options((unsigned char *)bp + 0xf0,	/* &bp->bp_vend[4] */
			     (unsigned char *)bp + 0x314);	/* popt + BOOTP_HDR_SIZE */

	if ((*overload_p & 1) != 0) {			/* OVERLOAD_FILE: file 区含选项 (处理后重读) */
		dhcp_process_options((unsigned char *)bp + 0x6c,	/* bp->bp_file */
				     (unsigned char *)bp + 0xec);	/* + sizeof(bp_file)=128 */
	}
	if ((*overload_p & 2) != 0) {			/* OVERLOAD_SNAME: sname 区含选项 */
		dhcp_process_options((unsigned char *)bp + 0x2c,	/* bp->bp_sname */
				     (unsigned char *)bp + 0x6c);	/* + sizeof(bp_sname)=64 */
	}
}

// === eth_get_dev_index === 原 FUN_04009140 @ 0x04009140
// 来源: reversed/uboot_funcs/FUN_04009140.c | 分类: uboot
// 语义: 返回当前以太网设备序号 (上游 net/eth-uclass.c eth_get_dev_index):
//       无设备返回 -1, 否则返回 dev->seq (+0x50)。
//       原代码对 eth_get_dev() 调用两次 (编译器未做公共子表达式消除), 原样保留。
int eth_get_dev_index(void)
{
	void *dev;

	dev = eth_get_dev();
	if (dev == NULL)
		return -1;
	dev = eth_get_dev();			/* 第二次调用, 反编译如实存在 */
	return *(int *)((char *)dev + 0x50);	/* dev->seq */
}

// === eth_init === 原 FUN_04009164 @ 0x04009164
// 来源: reversed/uboot_funcs/FUN_04009164.c | 分类: uboot
// 语义: 启动当前以太网设备 (上游 net/eth-uclass.c eth_init):
//       ethrotate=no 且 ethact 已设时固定用 ethact 设备 (取不到返回 -EINVAL);
//       否则用当前设备 (无设备打印并返回 -ENODEV)。循环内:
//       设备已激活 (flags&1 = DM_FLAG_ACTIVATED, dev+0x48) 时调 ops->start
//       (dev->driver(+0x00)->ops(+0x3c)->start(+0x00)), 成功则 uclass_priv
//       (dev+0x28 指向的) state 置 ETH_STATE_ACTIVE(2) 并返回 0;
//       未激活设备沿用全局 eth_errno (DAT_0400925c)。失败轮换设备直至回到起点。
//       Ghidra 对若干间接调用未标参数, 实参均为 (dev)。
int eth_init(void)
{
	char *ethact = func_0x03fbefb0(DAT_0400924c);		/* getenv("ethact") */
	char *ethrotate = func_0x03fbefb0(DAT_04009250);	/* getenv("ethrotate") */
	void *current = NULL;
	void *old_current;
	int ret = -0x13;					/* -ENODEV */

	/* 上游: ethrotate=="no" 时坚持用 ethact 设备 */
	if (ethrotate != NULL && ub_strcmp(ethrotate, DAT_04009254) == 0) {	/* strcmp(ethrotate,"no") */
		if (ethact != NULL) {
			current = eth_get_dev_by_name(ethact);
			if (current == NULL)
				return -0x16;		/* -EINVAL */
		}
	}

	if (current == NULL) {
		current = eth_get_dev();
		if (current == NULL) {
			ub_printf(DAT_04009258);	/* "No ethernet found.\n" (全局, 语义待定) */
			return -0x13;			/* -ENODEV */
		}
	}

	old_current = current;
	do {
		if (current != NULL) {
			if (((*(unsigned int *)((char *)current + 0x48)) & 1U) == 0) {
				/* 设备未激活: ret = eth_errno */
				ret = *DAT_0400925c;	/* &eth_errno (全局 int, 语义待定) */
			} else {
				/* device_active: 调 ops->start(current)
				 * dev->driver = *(dev+0x00); ops = *(driver+0x3c); start = *(ops+0x00) */
				ret = (*(int (**)(void *))(*(void **)(*(void **)current + 0x3c)))(current);
				if (ret >= 0) {
					/* priv = current->uclass_priv (+0x28); priv->state = ETH_STATE_ACTIVE */
					*(unsigned int *)(*(void **)((char *)current + 0x28)) = 2;
					return 0;
				}
			}
		}
		eth_try_another(0);			/* 轮换当前设备 */
		current = eth_get_dev();
	} while (old_current != current);

	return ret;
}

// === eth_halt === 原 FUN_04009260 @ 0x04009260
// 来源: reversed/uboot_funcs/FUN_04009260.c | 分类: uboot
// 语义: 停止当前以太网设备 (上游 net/eth-uclass.c eth_halt):
//       设备存在且已激活 (flags&1, dev+0x48) 时调 ops->stop
//       (dev->driver->ops(+0x3c)->stop(+0x10)), 并将 uclass_priv
//       (dev+0x28 指向的) state 置 ETH_STATE_PASSIVE(1)。
void eth_halt(void)
{
	void *current = eth_get_dev();

	if (current == NULL)
		return;
	if (((*(unsigned int *)((char *)current + 0x48)) & 1U) == 0)	/* !device_active(current) */
		return;

	(*(void (**)(void *))(*(void **)(*(void **)current + 0x3c) + 0x10))(current);	/* ops->stop(current) */
	*(unsigned int *)(*(void **)((char *)current + 0x28)) = 1;	/* priv->state = ETH_STATE_PASSIVE */
}

// === eth_initialize === 原 FUN_04009398 @ 0x04009398
// 来源: reversed/uboot_funcs/FUN_04009398.c | 分类: uboot
// 语义: 枚举并初始化全部以太网设备 (上游 net/eth-uclass.c eth_initialize):
//       eth_common_init() 后 uclass_first_device(UCLASS_ETH=0x13, &dev) 探测;
//       无设备打印 "No ethernet found." 并 show_boot_progress(-64)
//       (bootstage_error(BOOTSTAGE_ID_NET_ETH_START=64)); 否则按 env ethprime
//       选定 prime 设备 (bootstage_mark(0x41=BOOTSTAGE_ID_NET_ETH_INIT)),
//       逐设备打印 "eth%d: %s" (seq@+0x50, name@+0x04)、prime 设备加 " [PRIME]"、
//       内联 eth_write_hwaddr (见下), uclass_next_device 推进, 设备间补 ", "。
//       返回设备总数。
//       eth_write_hwaddr 内联逻辑: 设备激活且 ops(+0x3c)->write_hwaddr(+0x14)
//       存在时, 未设 eth<N>macskip (eth_mac_skip(dev->seq)) 且 platdata(+8)
//       的 enetaddr(+4) 合法 (!多播 && !全零) 才调用; 非法地址/写失败均打印。
int eth_initialize(void)
{
	void *dev = NULL;
	char *ethprime;
	void *prime_dev = NULL;
	int num_devices = 0;
	int ret;

	eth_common_init();

	func_0x03fd1098(0x13, &dev);			/* uclass_first_device(UCLASS_ETH, &dev) */
	if (dev == NULL) {
		ub_printf(DAT_04009534);		/* "No ethernet found.\n" (全局, 语义待定) */
		func_0x03fbf43c(-64);			/* bootstage_error(NET_ETH_START=64) → show_boot_progress(-id) */
		return 0;
	}

	ethprime = func_0x03fbefb0(DAT_04009538);	/* getenv("ethprime") */
	if (ethprime != NULL)
		prime_dev = eth_get_dev_by_name(ethprime);	/* 实参经寄存器传递, 反编译未显示, 此处为 ethprime */
	if (prime_dev != NULL) {
		eth_set_dev(prime_dev);
		eth_current_changed();
	} else {
		eth_set_dev(NULL);			/* Ghidra 显示带 0 实参 */
	}

	func_0x03fbf43c(0x41);				/* bootstage_mark(NET_ETH_INIT=65) */

	do {
		ub_printf(DAT_0400953c,			/* "eth%d: %s" (全局, 语义待定) */
			  *(int *)((char *)dev + 0x50),	/* dev->seq */
			  *(char **)((char *)dev + 0x04));	/* dev->name */
		if (ethprime != NULL && dev == prime_dev)
			ub_printf(DAT_04009540);	/* " [PRIME]" (全局, 语义待定) */

		/* --- eth_write_hwaddr(dev) 内联 --- */
		if ((*(unsigned int *)((char *)dev + 0x48) & 1U) != 0) {	/* device_active(dev) */
			void **write_hwaddr = *(void **)(*(void **)dev + 0x3c) + 0x14;	/* &ops->write_hwaddr */
			if (*write_hwaddr != NULL) {
				unsigned char *pdata = *(unsigned char **)((char *)dev + 0x08);	/* platdata */
				if (eth_mac_skip(*(int *)((char *)dev + 0x50)) == 0) {
					/* is_valid_ethaddr(enetaddr): 非多播 (addr[0]&1==0) 且非全零 */
					if (((pdata[4] & 1) == 0) &&
					    (pdata[4] != 0 || pdata[5] != 0 || pdata[6] != 0 ||
					     pdata[7] != 0 || pdata[8] != 0 || pdata[9] != 0)) {
						ret = (*(int (**)(void *))write_hwaddr)(dev);
						if (ret != 0 && ret != -0x26)	/* != -ENOSYS */
							ub_printf(DAT_04009544,	/* "\nWarning: %s failed to set MAC address\n" */
								  *(char **)((char *)dev + 0x04));
					} else {
						ub_printf(DAT_0400954c,	/* "\nError: %s address %pM illegal value\n" */
							  *(char **)((char *)dev + 0x04),
							  pdata + 4);	/* pdata->enetaddr */
					}
				}
			}
		}
		/* --- 内联结束 --- */

		num_devices++;
		func_0x03fd10d0(&dev);			/* uclass_next_device(&dev) */
		if (dev == NULL)
			break;
		ub_printf(DAT_04009548);		/* ", " (全局, 语义待定) */
	} while (1);

	func_0x03fc7f78(10);				/* putc('\n') */
	return num_devices;
}

// === eth_mac_skip === 原 FUN_040096ec @ 0x040096ec
// 来源: reversed/uboot_funcs/FUN_040096ec.c | 分类: uboot
// 语义: 查询设备 MAC 是否被环境变量跳过 (上游 net/eth_common.c eth_mac_skip):
//       sprintf(enetvar, index ? "eth%dmacskip" : "ethmacskip", index);
//       返回 getenv(enetvar) != NULL。栈上变量名缓冲 16 字节。
//       Ghidra 多出的 param_2..4 为寄存器残留。
int eth_mac_skip(int index)
{
	char enetvar[16];			/* 栈上 16 字节变量名缓冲 */
	const char *fmt;

	fmt = DAT_04009728;			/* "eth%dmacskip" (全局, 语义待定) */
	if (index == 0)
		fmt = DAT_04009724;		/* "ethmacskip" (全局, 语义待定) */
	ub_sprintf(enetvar, fmt, index);
	return func_0x03fbefb0(enetvar) != NULL;
}

// === eth_current_changed === 原 FUN_04009754 @ 0x04009754
// 来源: reversed/uboot_funcs/FUN_04009754.c | 分类: uboot
// 语义: 当前以太网设备变化时同步 env ethact (上游 eth-uclass.c/eth_common.c 版本):
//       ethrotate=no 时直接返回; 有当前设备且 ethact 与 eth_get_name() 不一致
//       (或 ethact 未设) 时 setenv("ethact", name); 无设备且 ethact 已设时
//       setenv("ethact", NULL) 删除变量。
//       注意: 反编译在函数尾部遇坏指令截断 (原反编译文件以伪调用终止),
//       两个 setenv 调用按上游源码恢复, 参数流 (eth_get_name 返回值入 r0) 与
//       截断前的调用序列一致。
void eth_current_changed(void)
{
	char *act = func_0x03fbefb0(DAT_040097d8);		/* getenv("ethact") */
	char *ethrotate = func_0x03fbefb0(DAT_040097dc);	/* getenv("ethrotate") */

	if (ethrotate != NULL && ub_strcmp(ethrotate, DAT_040097e0) == 0)	/* strcmp(ethrotate,"no") */
		return;

	if (eth_get_dev() != NULL) {
		if (act == NULL || ub_strcmp(act, eth_get_name()) != 0)
			setenv(DAT_040097d8, eth_get_name());	/* setenv("ethact", name), 反编译截断处按上游恢复 */
	} else if (act != NULL) {
		setenv(DAT_040097d8, NULL);		/* setenv("ethact", NULL) 删除变量 */
	}
}

// === net_init === 原 FUN_04009a84 @ 0x04009a84
// 来源: reversed/uboot_funcs/FUN_04009a84.c | 分类: uboot
// 语义: 网络子系统一次性初始化 (上游 net/net.c net_init):
//       首次调用时对齐 net_pkt_buf 得到 net_tx_packet (PKTALIGN=32 对齐,
//       "ptr+(PKTALIGN-1)-ptr%PKTALIGN" 折叠为 & ~0x1f), 并按
//       PKTSIZE_ALIGN(0x600=1536) 依次切出 4 个 (PKTBUFSRX) 接收缓冲
//       net_rx_packets[0..3]; 随后 arp_init() + net_clear_handlers()。
//       之后当前以太网设备存在时执行 net_init_loop()。
//       (上游 net_init_loop() 无条件调用, 固件此处以 eth_get_dev()!=0 为条件)
void net_init(void)
{
	int *first_call_p = DAT_04009af0;		/* &first_call (静态局部提升为全局) */

	if (*first_call_p != 0) {
		unsigned int buf = DAT_04009af4 & 0xffffffe0;	/* net_tx_packet 对齐地址 */
		int *rx_packets = DAT_04009afc;		/* &net_rx_packets[0] (数组首地址) */

		*DAT_04009af8 = buf;			/* net_tx_packet */
		rx_packets[0] = (int)(buf + 0x600);	/* net_rx_packets[i] = tx + (i+1)*PKTSIZE_ALIGN */
		rx_packets[1] = (int)(buf + 0xc00);
		rx_packets[2] = (int)(buf + 0x1200);
		rx_packets[3] = (int)(buf + 0x1800);

		arp_init();
		net_clear_handlers();

		*first_call_p = 0;			/* first_call = 0 */
	}

	if (eth_get_dev() != NULL)
		net_init_loop();
}

// === net_set_udp_handler === 原 FUN_0400a00c @ 0x0400a00c
// 来源: reversed/uboot_funcs/FUN_0400a00c.c | 分类: uboot
// 语义: 设置 UDP 收包处理回调 (上游 net/net.c net_set_udp_handler):
//       f==NULL 时置为 dummy_handler (DAT_0400a028), 否则置为 f。
void net_set_udp_handler(void *handler)
{
	if (handler == NULL)
		*DAT_0400a024 = DAT_0400a028;		/* udp_packet_handler = dummy_handler */
	else
		*DAT_0400a024 = handler;		/* udp_packet_handler = f */
}

// === net_process_received_packet === 原 FUN_0400a0a8 @ 0x0400a0a8
// 来源: reversed/uboot_funcs/FUN_0400a0a8.c | 分类: uboot
// 语义: 收包主分发 (上游 net/net.c net_process_received_packet):
//       记录 net_rx_packet/len; <14 字节丢弃; push_packet 挂接时直接转发;
//       解析 VLAN/802.2 头得到 IP 载荷与协议号, 做 VLAN 过滤;
//       ARP → arp_receive; IP → 校验头长/版本/首长/校验和/目的地址(广播或本机),
//       未分片检查后 ICMP → 重定向告警或 ping_receive + packet_icmp_handler,
//       UDP → udp_packet_handler(载荷, udp_dst, src_ip, udp_src, udp_len-8)。
//       CDP/NETCONSOLE/UDP_CHECKSUM/IP_DEFRAG 均未编译进本固件。
void net_process_received_packet(void *in_packet, unsigned int len)
{
	unsigned char *pkt = (unsigned char *)in_packet;
	unsigned short vlan_none = 0xfff;		/* VLAN_NONE */
	unsigned short myvlanid, mynvlanid, vlanid;
	unsigned short et_type;
	unsigned char *ip;
	unsigned int src_ip;

	*DAT_0400a3bc = in_packet;			/* net_rx_packet */
	*DAT_0400a3c0 = len;				/* net_rx_packet_len */
	if (len < 0xe)					/* ETHER_HDR_SIZE */
		return;

	if (*(void **)DAT_0400a3c4 != NULL) {		/* push_packet (CONFIG_API) */
		(*(void (**)(void *, unsigned int))(*(void **)DAT_0400a3c4))(in_packet, len);	/* 实参经寄存器传递, 反编译未显示, 按上游恢复 */
		return;
	}

	myvlanid = (unsigned short)((*(unsigned short *)DAT_0400a3c8 >> 8) | (*(unsigned short *)DAT_0400a3c8 << 8));	/* ntohs(net_our_vlan) */
	if (myvlanid == 0xffff)
		myvlanid = vlan_none;
	mynvlanid = (unsigned short)((*(unsigned short *)DAT_0400a3cc >> 8) | (*(unsigned short *)DAT_0400a3cc << 8));	/* ntohs(net_native_vlan) */
	if (mynvlanid == 0xffff)
		mynvlanid = vlan_none;

	et_type = (unsigned short)((*(unsigned short *)(pkt + 0xc) >> 8) | (*(unsigned short *)(pkt + 0xc) << 8));	/* ntohs(et->et_protlen) */
	if (et_type < 0x5ea) {				/* < 1514: 802.2 包 */
		et_type = (unsigned short)((*(unsigned short *)(pkt + 0x14) >> 8) | (*(unsigned short *)(pkt + 0x14) << 8));	/* ntohs(et802->et_prot) @+20 */
		ip = pkt + 0x16;			/* E802_HDR_SIZE = 22 */
		len -= 0x16;
	} else if (et_type == 0x8100) {			/* PROT_VLAN */
		if (len < 0x12)				/* VLAN_ETHER_HDR_SIZE = 18 */
			return;
		if ((myvlanid & 0xfff) == vlan_none)	/* 无 VLAN 激活 */
			return;
		vlanid = (unsigned short)((*(unsigned short *)(pkt + 0xe) >> 8) | ((*(unsigned short *)(pkt + 0xe) & 0xf) << 8));	/* cti & VLAN_IDMASK */
		et_type = (unsigned short)((*(unsigned short *)(pkt + 0x10) >> 8) | (*(unsigned short *)(pkt + 0x10) << 8));	/* ntohs(vet->vet_type) @+16 */
		ip = pkt + 0x12;			/* VLAN_ETHER_HDR_SIZE = 18 */
		len -= 0x12;
	} else {					/* 普通以太网包 */
		ip = pkt + 0xe;				/* ETHER_HDR_SIZE = 14 */
		len -= 0xe;
	}

	if ((myvlanid & 0xfff) != vlan_none) {		/* VLAN 过滤 */
		if (vlanid == vlan_none)
			vlanid = (unsigned short)(mynvlanid & 0xfff);
		if (vlanid != (unsigned short)(myvlanid & 0xfff))
			return;
	}

	if (et_type == 0x800) {				/* PROT_IP */
		unsigned int ip_len;
		unsigned int dst_ip;

		if (len <= 0x1b)			/* len < IP_UDP_HDR_SIZE(28) */
			return;
		ip_len = (unsigned int)((*(unsigned short *)(ip + 2) >> 8) | (*(unsigned short *)(ip + 2) & 0xff) << 8);	/* ntohs(ip->ip_len) @+2 */
		if ((int)ip_len > (int)len)		/* len < ip_len → 丢弃 */
			return;
		if ((ip[0] & 0xf0) != 0x40)		/* 非 IPv4 */
			return;
		if ((ip[0] & 0xf) >= 6)			/* IP 选项 (首长 > 20) 不支持 */
			return;
		if (ip_checksum_ok(ip, 0x14) == 0)	/* IP_HDR_SIZE = 20 */
			return;

		ub_memcpy(&dst_ip, ip + 0x10, 4);	/* net_read_ip(&ip->ip_dst) @+16 */
		/* 目的地址必须为广播或本机 IP (net_ip 为 0 时不过滤) */
		if (dst_ip != 0xffffffff &&
		    *(uint32_t *)DAT_0400a3d0 != dst_ip && *(uint32_t *)DAT_0400a3d0 != 0)
			return;

		ub_memcpy(&src_ip, ip + 0xc, 4);	/* net_read_ip(&ip->ip_src) @+12 */

		/* net_defragment 内联 (CONFIG_IP_DEFRAG 未启用): 非分片检查, ip_off @+6 */
		if (*(unsigned short *)(ip + 6) >> 8 != 0 || (*(unsigned short *)(ip + 6) & 0x3f) != 0)
			return;

		if (ip[9] == 1) {			/* IPPROTO_ICMP */
			if (ip[0x14] == 5) {		/* ICMP_REDIRECT */
				if (ip[0x15] == 1)	/* ICMP_REDIR_HOST */
					ub_printf(DAT_0400a3d4, ip + 0x18);	/* " ICMP Host Redirect to %pI4 ", &icmph->un.gateway (全局, 语义待定) */
			} else {
				ping_receive(in_packet, ip, ip_len);	/* CONFIG_CMD_PING */
				if (*(void **)DAT_0400a3d8 != NULL) {	/* packet_icmp_handler (CONFIG_CMD_TFTPPUT) */
					(*(void (**)(unsigned char, unsigned char, unsigned short, unsigned int,
					     unsigned short, unsigned char *, unsigned short))*(void **)DAT_0400a3d8)
						(ip[0x14],		/* icmph->type */
						 ip[0x15],		/* icmph->code */
						 (unsigned short)((*(unsigned short *)(ip + 0x16) >> 8) | (*(unsigned short *)(ip + 0x16) << 8)),	/* ntohs(ip->udp_dst) @+22 */
						 src_ip,
						 (unsigned short)((*(unsigned short *)(ip + 0x14) >> 8) | (*(unsigned short *)(ip + 0x14) << 8)),	/* ntohs(ip->udp_src) @+20 */
						 ip + 0x18,		/* icmph->un.data */
						 (unsigned short)((*(unsigned short *)(ip + 0x18) >> 8) | (*(unsigned short *)(ip + 0x18) << 8)));	/* ntohs(ip->udp_len) @+24 */
				}
			}
		} else if (ip[9] == 0x11) {			/* IPPROTO_UDP */
			/* udp_packet_handler(载荷, udp_dst, src_ip, udp_src, udp_len - UDP_HDR_SIZE) */
			(*(void (**)(unsigned char *, unsigned short, unsigned int, unsigned short, unsigned short))*(void **)DAT_0400a3dc)
				(ip + 0x1c,		/* ip + IP_UDP_HDR_SIZE(28) */
				 (unsigned short)((*(unsigned short *)(ip + 0x16) >> 8) | (*(unsigned short *)(ip + 0x16) << 8)),	/* ntohs(ip->udp_dst) */
				 src_ip,
				 (unsigned short)((*(unsigned short *)(ip + 0x14) >> 8) | (*(unsigned short *)(ip + 0x14) << 8)),	/* ntohs(ip->udp_src) */
				 (unsigned short)(((unsigned short)((*(unsigned short *)(ip + 0x18) >> 8) | (*(unsigned short *)(ip + 0x18) & 0xff) << 8)) - 8));	/* ntohs(ip->udp_len) - 8 */
		}
	} else if (et_type == 0x806) {			/* PROT_ARP */
		arp_receive(in_packet, ip, (int)len);
	}
	/* 其余协议号忽略 */
}

// === net_eth_hdr_size === 原 FUN_0400a3e0 @ 0x0400a3e0
// 来源: reversed/uboot_funcs/FUN_0400a3e0.c | 分类: uboot
// 语义: 返回应使用的以太网头长度 (上游 net/net.c net_eth_hdr_size):
//       net_our_vlan (DAT_0400a418) 有效时返回 VLAN_ETHER_HDR_SIZE(18=0x12),
//       否则 ETHER_HDR_SIZE(14=0xe)。编译器将 ntohs 与 VLAN_NONE 判定内联。
int net_eth_hdr_size(void)
{
	unsigned short myvlanid;

	myvlanid = (unsigned short)((*(unsigned short *)DAT_0400a418 >> 8) | (*(unsigned short *)DAT_0400a418 << 8));	/* ntohs(net_our_vlan) */
	if (myvlanid == 0xffff)
		myvlanid = 0xfff;			/* VLAN_NONE */
	if ((myvlanid & 0xfff) == 0xfff)
		return 0xe;				/* ETHER_HDR_SIZE */
	return 0x12;					/* VLAN_ETHER_HDR_SIZE */
}

// === net_update_ether === 原 FUN_0400a4b8 @ 0x0400a4b8
// 来源: reversed/uboot_funcs/FUN_0400a4b8.c | 分类: uboot
// 语义: 填写/更新以太网头目的 MAC、源 MAC(本机 net_ethaddr, DAT_0400a530)与
//       协议字段 htons(prot), 并按原协议字段返回实际头长:
//       原 VLAN(0x8100) → vet_type@+0x10, 返回 18; 原 < 1514(802.2) →
//       et_prot@+0x14, 返回 22; 否则 et_protlen@+0x0c, 返回 14。
//       (上游 net/net.c net_update_ether)
int net_update_ether(void *et, unsigned char *addr, unsigned short prot)
{
	unsigned short protlen;
	unsigned short prot_be;

	ub_memcpy(et, addr, 6);					/* memcpy(et->et_dest, addr, 6) */
	ub_memcpy((char *)et + 6, DAT_0400a530, 6);		/* memcpy(et->et_src, net_ethaddr, 6), Ghidra 丢弃了实参 */

	protlen = (unsigned short)((*(unsigned short *)((char *)et + 0xc) >> 8) | (*(unsigned short *)((char *)et + 0xc) << 8));	/* ntohs(et->et_protlen) */
	prot_be = (unsigned short)(((unsigned int)DAT_0400a534 & ((unsigned int)prot << 8)) | ((prot >> 8) & 0xff));	/* htons(prot), 掩码 DAT_0400a534 = 0xff00 */

	if (protlen == 0x8100) {			/* PROT_VLAN */
		*(unsigned short *)((char *)et + 0x10) = prot_be;	/* vet->vet_type */
		return 0x12;				/* VLAN_ETHER_HDR_SIZE */
	}
	if (protlen < 0x5eb) {				/* ≤ 1514: 802.2 + SNAP */
		*(unsigned short *)((char *)et + 0x14) = prot_be;	/* et802->et_prot */
		return 0x16;				/* E802_HDR_SIZE */
	}
	*(unsigned short *)((char *)et + 0xc) = prot_be;	/* et->et_protlen */
	return 0xe;					/* ETHER_HDR_SIZE */
}

// === net_set_ip_header === 原 FUN_0400a538 @ 0x0400a538
// 来源: reversed/uboot_funcs/FUN_0400a538.c | 分类: uboot
// 语义: 构造 20 字节 IP 头 (上游 net/net.c net_set_ip_header):
//       首字 = DAT_0400a5b4 (0x45,0x00,htons(IP_HDR_SIZE=20) 编译器合并的 32 位常量);
//       ip_id = htons(net_ip_id++) (DAT_0400a5b8 = &net_ip_id, DAT_0400a5bc = 0xff00 掩码);
//       ip_off = 0x4000 (IP_FLAGS_DFRAG 不分片, htons 后半字 0x0040);
//       ip_ttl = 255; ip_sum = 0; ip_src = source / ip_dst = dest (已是网络序)。
void net_set_ip_header(void *pkt, uint32_t dest, uint32_t source)
{
	unsigned char *ip = (unsigned char *)pkt;
	unsigned int id;

	*(uint32_t *)ip = DAT_0400a5b4;			/* ip_hl_v=0x45, ip_tos=0, ip_len=htons(20) */

	id = *(uint32_t *)DAT_0400a5b8;			/* net_ip_id */
	*(uint32_t *)DAT_0400a5b8 = id + 1;		/* net_ip_id++ */
	/* ip_id = htons(net_ip_id++) — 反编译形式: (0xff00 & (id<<8)) | ((id>>8)&0xff) */
	*(unsigned short *)(ip + 4) = (unsigned short)(((unsigned int)DAT_0400a5bc & ((id << 8) & 0xffffU)) | ((id >> 8) & 0xff));
	*(unsigned short *)(ip + 6) = 0x40;		/* ip_off = htons(IP_FLAGS_DFRAG=0x4000) */
	*(unsigned char *)(ip + 8) = 0xff;		/* ip_ttl = 255 */
	*(unsigned short *)(ip + 10) = 0;		/* ip_sum = 0 */

	ub_memcpy(ip + 12, &source, 4);			/* ip_src = source (Ghidra 将实参暂存到栈局部) */
	ub_memcpy(ip + 16, &dest, 4);			/* ip_dst = dest */
}

// === net_send_udp_packet === 原 FUN_0400a644 @ 0x0400a644
// 来源: reversed/uboot_funcs/FUN_0400a644.c | 分类: uboot
// 语义: 经 UDP 发包 (上游 net/net.c net_send_udp_packet):
//       net_tx_packet (DAT_0400a728) 未初始化则返回 -1;
//       目的 IP 为 0 或广播(0xffffffff) 时改写目的并使用广播 MAC (DAT_0400a72c);
//       net_set_ether + net_set_udp_header 组帧; MAC 未解析 (与 net_null_ethaddr
//       DAT_0400a730 比较 6 字节相等) 时保存等待状态 (目的 IP/ether/包长 = 头长
//       eth_hdr+0x1c+payload、arp_wait_try=1、arp_wait_timer_start=get_timer(0))
//       并 arp_request() 返回 1 (等待); 否则 net_send_packet 发送返回 0。
//       Ghidra 对 net_send_packet 未标长度实参, 按上游恢复 (pkt_hdr_size + payload_len)。
int net_send_udp_packet(unsigned char *ether, uint32_t dest, int dport, int sport, int payload_len)
{
	unsigned int *tx_packet_p = DAT_0400a728;	/* &net_tx_packet */
	unsigned int tx_packet = *tx_packet_p;
	int eth_hdr_size;
	unsigned char *effective_ether = ether;
	uint32_t effective_dest = dest;

	if (tx_packet == 0)				/* assert(net_tx_packet != NULL) */
		return -1;

	/* dest==0 → 0xffffffff; 广播 → 广播 MAC (编译器将两个 if 合并为一个条件) */
	if ((effective_dest == 0) || (effective_dest == 0xffffffff)) {
		effective_dest = 0xffffffff;
		effective_ether = DAT_0400a72c;		/* net_bcast_ethaddr */
	}

	eth_hdr_size = net_set_ether((void *)tx_packet, effective_ether, 0x800);	/* PROT_IP; 反编译多出的 param_4/param_1/param_2 为寄存器残留 */
	net_set_udp_header((void *)(tx_packet + (unsigned int)eth_hdr_size), effective_dest, dport, sport, payload_len);

	if (ub_memcmp(effective_ether, DAT_0400a730, 6) == 0) {	/* MAC 未解析 → ARP 路径 */
		*DAT_0400a734 = effective_dest;		/* net_arp_wait_packet_ip = dest */
		*DAT_0400a738 = effective_ether;	/* arp_wait_packet_ethaddr = ether */
		*DAT_0400a73c = eth_hdr_size + 0x1c + payload_len;	/* arp_wait_tx_packet_size (0x1c = IP_UDP_HDR_SIZE) */
		*DAT_0400a740 = 1;			/* arp_wait_try = 1 */
		*DAT_0400a744 = ub_get_timer(0);	/* arp_wait_timer_start = get_timer(0), 实参反编译未显示 */
		arp_request();
		return 1;				/* waiting */
	}

	eth_send(tx_packet, eth_hdr_size + 0x1c + payload_len);	/* net_send_packet 内联: eth_send(net_tx_packet, 总长) */
	return 0;					/* transmitted */
}

// === rpc_add_credentials === 原 FUN_0400ac9c @ 0x0400ac9c
// 来源: reversed/uboot_funcs/FUN_0400ac9c.c | 分类: uboot
// 语义: 构造 RPC AUTH_UNIX 凭据 (上游 net/nfs.c rpc_add_credentials):
//       flavor=htonl(1)、auth_len=htonl(对齐后主机名长+20)、stamp=0、
//       主机名长度+内容(4 字节对齐补零)、uid/gid/辅助组列表全零,
//       共 5 个零字结尾。上游主机名为空串 "", 固件改为 DAT_0400ad44 指向的字符串。
//       反编译签名为 void (r0 返回值未被调用方使用; 上游返回推进后的指针)。
void rpc_add_credentials(uint32_t *p)
{
	char hostname[256];			/* 栈上 256 字节缓冲 */
	uint32_t *body = p + 4;			/* 跳过 4 字头 (flavor/len/stamp/hostnamelen) */
	int hostnamelen;
	int hl;
	uint32_t v;

	ub_strcpy(hostname, DAT_0400ad44);	/* 上游 strcpy(hostname, ""), 固件定制 (全局, 语义待定) */
	hostnamelen = ub_strlen(hostname);
	hl = (hostnamelen + 3) & ~3;

	p[0] = 0x01000000;			/* htonl(1): AUTH_UNIX */
	v = (uint32_t)hl + 20;
	p[1] = v * 0x1000000u | (v >> 8 & 0xff) << 0x10 | (v >> 0x10 & 0xff) << 8 | v >> 0x18;	/* htonl(hl+20): auth length */
	p[2] = 0;				/* htonl(0): stamp */
	v = (uint32_t)hostnamelen;
	p[3] = v * 0x1000000u | (v >> 8 & 0xff) << 0x10 | (v >> 0x10 & 0xff) << 8 | v >> 0x18;	/* htonl(hostnamelen) */

	if ((hostnamelen & 3) != 0) {
		unsigned int idx = (hostnamelen < 0) ? (unsigned int)(hostnamelen + 3) : (unsigned int)hostnamelen;
		body[idx >> 2] = 0;		/* 主机名后的对齐补零字 (hostnamelen>=0 时等价 body[hostnamelen/4]) */
	}
	ub_memcpy(body, hostname, (unsigned int)hostnamelen);	/* 主机名内容 */

	{	/* hl/4 向前推进后写 5 个零字: uid, gid, 辅助组列表, AUTH_NONE, auth_len */
		int wi = (hostnamelen + 3) >> 2;	/* hl / 4 */
		body[wi] = 0;
		body[wi + 1] = 0;
		body[wi + 2] = 0;
		body[wi + 3] = 0;
		body[wi + 4] = 0;
	}
}

// === nfs_read_reply === 原 FUN_0400b0c0 @ 0x0400b0c0
// 来源: reversed/uboot_funcs/FUN_0400b0c0.c | 分类: uboot
// 语义: 处理 NFS READ 应答 (上游 net/nfs.c nfs_read_reply):
//       拷贝 100 字节应答头; 应答 id 与 rpc_id (DAT_0400b1d8) 比较,
//       大于 → -1 (-NFS_RPC_ERR), 小于 → -124 (-NFS_RPC_DROP);
//       rstatus/verifier/astatus 任一非零 → rstatus 或 astatus 时 -9999
//       (DAT_0400b1f0), 否则 -ntohl(data[0]);
//       全部为零 (NFS_OK) 时按 nfs_offset 打印进度 '#' 哈希
//       (每 NFS_READ_SIZE/2*10=0x1400 字节一枚, 每大周期 DAT_0400b1e0 换行),
//       并将数据块拷入 load_addr(DAT_0400b1e8)+nfs_offset, 更新
//       net_boot_file_size (DAT_0400b1ec), 返回 rlen (ntohl(data[18]))。
//       本固件 rpc_t.reply 布局: id@0 type@4 rstatus@8 verifier@12
//       verifier长度@16 astatus@20 data[0]@24 data[18]@96 (比上游多一字)。
//       Ghidra 未显示 len 参数 (函数体未使用), 按上游签名保留。
int nfs_read_reply(void *pkt, unsigned int len)
{
	uint32_t hdr[25];			/* 应答头 100 字节 (反编译栈局部缓冲) */
	unsigned int *rpc_id_p = DAT_0400b1d8;	/* &rpc_id (全局, 语义待定) */
	int *nfs_offset_p = DAT_0400b1dc;	/* &nfs_offset (全局, 语义待定) */
	uint32_t id_be;
	uint32_t rlen;
	uint32_t newsize;
	int nfs_offset;

	(void)len;				/* 反编译体未使用 */
	ub_memcpy(hdr, pkt, 100);		/* memcpy(&rpc_pkt.u.reply, pkt, sizeof(rpc_pkt.u.reply)) */

	id_be = hdr[0] << 0x18 | (hdr[0] >> 8 & 0xff) << 0x10 | (hdr[0] >> 0x10 & 0xff) << 8 | hdr[0] >> 0x18;	/* ntohl(id) */
	if (id_be > *rpc_id_p)
		return -1;			/* -NFS_RPC_ERR */
	if (id_be < *rpc_id_p)
		return -124;			/* -NFS_RPC_DROP (0xffffff84) */

	if (hdr[2] != 0 || hdr[3] != 0 || hdr[5] != 0 || hdr[6] != 0) {	/* rstatus/verifier/astatus/data[0] */
		if (hdr[2] != 0)		/* rstatus */
			return (int)DAT_0400b1f0;	/* -9999 (全局, 语义待定) */
		if (hdr[5] != 0)		/* astatus */
			return (int)DAT_0400b1f0;	/* -9999 */
		return -(int)(hdr[6] << 0x18 | (hdr[6] >> 8 & 0xff) << 0x10 | (hdr[6] >> 0x10 & 0xff) << 8 | hdr[6] >> 0x18);	/* -ntohl(data[0]) */
	}

	nfs_offset = *nfs_offset_p;
	/* 进度哈希: 反编译经 aeabi_idivmod(FUN_0400c7ec) 检查余数, 此处还原为 % 运算 */
	if (nfs_offset != 0 && (nfs_offset % (int)DAT_0400b1e0) == 0)
		func_0x03fc7fbc(DAT_0400b1e4);	/* puts("\n\t ") (全局, 语义待定) */
	if ((nfs_offset % 0x1400) == 0)		/* (NFS_READ_SIZE/2)*10 = 5120 */
		func_0x03fc7f78(0x23);		/* putc('#') */

	rlen = hdr[24] << 0x18 | (hdr[24] >> 8 & 0xff) << 0x10 | (hdr[24] >> 0x10 & 0xff) << 8 | hdr[24] >> 0x18;	/* ntohl(data[18]) */
	newsize = rlen + (uint32_t)nfs_offset;
	/* store_block 内联: memcpy(load_addr + nfs_offset, pkt + sizeof(reply), rlen) */
	ub_memcpy((void *)(nfs_offset + *(int *)DAT_0400b1e8), (unsigned char *)pkt + 100, rlen);
	if (*(uint32_t *)DAT_0400b1ec < newsize)	/* net_boot_file_size < newsize */
		*(uint32_t *)DAT_0400b1ec = newsize;
	return (int)rlen;
}

// === tftp_complete === 原 FUN_0400b9e0 @ 0x0400b9e0
// 来源: reversed/uboot_funcs/FUN_0400b9e0.c | 分类: uboot
// 语义: TFTP 传输收尾 (上游 net/tftp.c tftp_complete, 无 CONFIG_TFTP_TSIZE):
//       time_start(DAT_0400ba40) = get_timer(time_start); 耗时非零时打印
//       "\n\t " 并以 print_size(ub_print_u64) 显示速率
//       net_boot_file_size(DAT_0400ba48)/耗时*1000 "/s"
//       (除法为 FUN_0400c66c aeabi_uidiv 调用); 最后 puts("\ndone\n") 并
//       net_set_state(NETLOOP_SUCCESS=2, 内联为 *DAT_0400ba54 = 2)。
void tftp_complete(void)
{
	int *time_start_p = DAT_0400ba40;	/* &time_start (全局, 语义待定) */
	int elapsed;

	elapsed = ub_get_timer((unsigned int)*time_start_p);	/* get_timer(time_start) */
	*time_start_p = elapsed;

	if (elapsed != 0) {			/* 上游 time_start > 0 (无符号比较等价于 != 0) */
		int speed;

		func_0x03fc7fbc(DAT_0400ba44);	/* puts("\n\t ") (全局, 语义待定) */
		speed = (int)aeabi_uidiv((unsigned int)*(int *)DAT_0400ba48, (unsigned int)*time_start_p);	/* net_boot_file_size / time_start */
		ub_print_u64((unsigned int)(speed * 1000), 0, DAT_0400ba4c);	/* print_size(speed*1000, "/s") (串全局, 语义待定) */
	}

	func_0x03fc7fbc(DAT_0400ba50);		/* puts("\ndone\n") (全局, 语义待定) */
	*DAT_0400ba54 = 2;			/* net_set_state(NETLOOP_SUCCESS) 内联, NETLOOP_SUCCESS=2 */
}

// === aeabi_uidivmod === 原 FUN_0400c7d4 @ 0x0400c7d4
// 来源: reversed/uboot_funcs/FUN_0400c7d4.c | 分类: uboot
// 语义: EABI 无符号除法取模入口 (__aeabi_uidivmod): r0=被除数, r1=除数,
//       商回 r0, 余数留 r1。实现为对内部除法例程 aeabi_uidiv (FUN_0400c66c)
//       的尾跳转 — 该例程同时计算商与余数, aeabi_uidiv 入口只取商。
//       除数为 0 时除法例程转入 ub_div0_hang (0x04004844) 死循环。
//       Ghidra 反编译体仅为一个尾调用, 无参数/返回值可标。
void aeabi_uidivmod(void)
{
	((unsigned (*)(void))aeabi_uidiv)();	/* FUN_0400c66c 尾调用: (r0, r1) 寄存器透传入, r0=商, r1=余数 */
}

/* ============================================================================
 * 覆盖率表 (batch=uboot_b3, 地址升序后 23 个)
 * ========================================================================== */
/*
 * #  语义名                       原地址      反编译行数  状态
 * 1  simple_strtol                0x04007884      14     已转写
 * 2  simple_strtoull              0x0400799c      73     已转写
 * 3  ip_checksum_ok               0x04007cc8      11     已转写
 * 4  arp_init                     0x04007ce4      13     已转写
 * 5  dhcp_extended                0x04008110      86     已转写
 * 6  dhcp_packet_process_options  0x040084d8      24     已转写
 * 7  eth_get_dev_index            0x04009140      15     已转写
 * 8  eth_init                     0x04009164      48     已转写
 * 9  eth_halt                     0x04009260      19     已转写
 * 10 eth_initialize               0x04009398      72     已转写
 * 11 eth_mac_skip                 0x040096ec      25     已转写
 * 12 eth_current_changed          0x04009754      36     已转写 (尾部按上游恢复, 反编译坏指令截断)
 * 13 net_init                     0x04009a84      31     已转写
 * 14 net_set_udp_handler          0x0400a00c      14     已转写
 * 15 net_process_received_packet  0x0400a0a8     111     已转写
 * 16 net_eth_hdr_size             0x0400a3e0      21     已转写
 * 17 net_update_ether             0x0400a4b8      28     已转写
 * 18 net_set_ip_header            0x0400a538      24     已转写
 * 19 net_send_udp_packet          0x0400a644      45     已转写
 * 20 rpc_add_credentials          0x0400ac9c      42     已转写
 * 21 nfs_read_reply               0x0400b0c0      67     已转写
 * 22 tftp_complete                0x0400b9e0      21     已转写
 * 23 aeabi_uidivmod               0x0400c7d4       9     已转写
 */
