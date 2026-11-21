// ============================================================================
// uboot/p1b.c — U-Boot 2016.07 反编译转写 (半批 uboot_b1 后 22 函数)
// 地址范围: 0x04007efc .. 0x0400c7ec
// 覆盖: ARP 接收 / eth DM 辅助族 / net 回调注册 / NFS RPC 请求构造 / basename /
//       libgcc aeabi 整除辅助
// 命名事实源: rewritten/name_map/uboot.tsv
// 语义规则: rewritten/CONVENTIONS.md
// ============================================================================

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* ----------------------------------------------------------------------------
 * 常量 (上游 v2016.07 net/arp.h、net/nfs.h、net/net.h; 与反编译立即数一致)
 * -------------------------------------------------------------------------- */
#define ARP_HDR_SIZE          0x1c   /* 28: ARP 头长 */
#define ARP_ETHER             1      /* ar_hrd 硬件类型: 以太网 */
#define PROT_IP               0x0800 /* 以太帧协议: IP */
#define PROT_ARP              0x0806 /* 以太帧协议: ARP */
#define ARPOP_REQUEST         1
#define ARPOP_REPLY           2
#define ETHER_HDR_SIZE        0x0e   /* 14 */
#define VLAN_ETHER_HDR_SIZE   0x12   /* 18 */
#define VLAN_NONE             0xfff
#define VLAN_IDMASK           0xfff
#define UCLASS_ETH            0x13   /* DM ETH uclass id (反编译实参) */
#define SUNRPC_PORT           111    /* 0x6f: portmapper 端口 */
#define PROG_PORTMAP          100000
#define PROG_NFS              100003
#define PROG_MOUNT            100005
#define MSG_CALL              0
#define MOUNT_ADDENTRY        1
#define NFS_READLINK          5
#define NFS_FHSIZE            32     /* 0x20 */
#define IP_UDP_HDR_SIZE       0x1c   /* 28: IP+UDP 头合计 */

/* ----------------------------------------------------------------------------
 * 类型与调用桩声明
 * -------------------------------------------------------------------------- */

/* RX 包处理函数原型 (上游 net.h rxhand_f / rxhand_icmp_f; sip 为 4 字节 IP 按值传) */
typedef void ub_rxhand_f(uint8_t *pkt, unsigned dport, uint32_t sip,
                         unsigned sport, unsigned len);
typedef ub_rxhand_f *ub_rxhand_fp_t;
typedef void ub_rxhand_icmp_f(unsigned type, unsigned code, unsigned dport,
                              uint32_t sip, unsigned sport, uint8_t *pkt,
                              unsigned len);
typedef ub_rxhand_icmp_f *ub_rxhand_icmp_fp_t;

/* DM/环境框架调用桩 — 位于 0x03FDxxxx 跳转区, name_map 未收录, 命名待补;
 * 语义由调用点反编译 + 上游 v2016.07 (net/eth-uclass.c 等) 结构比对推断 */
extern int      fun_03fd0cb8(int uclass_id, uint32_t **ucp);  /* 原名 func_0x03fd0cb8; 语义推断: uclass_get */
extern int      fun_03fd0ed0(int uclass_id, int seq, int by_seq, uint32_t **devp); /* 原名 func_0x03fd0ed0; 语义推断: uclass_find_device_by_seq */
extern int      fun_03fd1098(int uclass_id, uint32_t **devp); /* 原名 func_0x03fd1098; 语义推断: uclass_first_device */
extern int      fun_03fd10d0(uint32_t **devp);                /* 原名 func_0x03fd10d0; 语义推断: uclass_next_device */
extern int      fun_03fd031c(void *dev);                      /* 原名 func_0x03fd031c; 语义推断: device_probe */
extern int      fun_03fbeb7c(const char *name, const char *value); /* 原名 func_0x03fbeb7c; 语义推断: setenv */
extern int      fun_03fbeb6c(void);                           /* 原名 func_0x03fbeb6c; 语义推断: get_env_id */
extern char    *fun_03fbefb0(const char *name);               /* 原名 func_0x03fbefb0; 语义推断: getenv */
extern int      fun_03fbf43c(int bootstage_id);               /* 原名 func_0x03fbf43c; 语义推断: bootstage_mark */
extern int      fun_03fc4768(void);                           /* 原名 func_0x03fc4768; 语义推断: miiphy_init */

/* ----------------------------------------------------------------------------
 * 本二进制其他 uboot 转写单元实现的被调函数 (名取 name_map/uboot.tsv)
 * -------------------------------------------------------------------------- */
extern void     ub_printf(const char *fmt, ...);                   /* FUN_04007548 */
extern void     ub_sprintf(char *buf, const char *fmt, ...);       /* FUN_04007514 */
extern void     ub_memcpy(void *dst, const void *src, uint32_t n); /* FUN_04005ea4 */
extern void     ub_memset(void *dst, int c, uint32_t n);           /* FUN_04005e0c */
extern uint32_t ub_strlen(const char *s);                          /* FUN_04005b98 */
extern void     ub_strcpy(char *dst, const char *src);             /* FUN_04005994 */
extern uint32_t simple_strtoul(const char *cp, char **endp, unsigned int base); /* FUN_04007718 */
extern int      net_update_ether(uint8_t *et, const uint8_t *addr, uint16_t prot); /* FUN_0400a4b8 */
extern int      eth_send(void *packet, int length);                /* FUN_0400929c */
extern void    *eth_get_dev_by_name(const char *devname);          /* FUN_04009008 */
extern void     eth_current_changed(void);                         /* FUN_04009754 */
extern int      net_eth_hdr_size(void);                            /* FUN_0400a3e0 */
extern int      net_send_udp_packet(uint8_t *ether, uint32_t dest_ip, int sport, int dport, int payload_len); /* FUN_0400a644 */
extern uint32_t *rpc_add_credentials(uint32_t *p);                 /* FUN_0400ac9c (Ghidra 标 void, 但调用方使用其返回值) */
extern int      eth_getenv_enetaddr(const char *name, uint8_t *enetaddr); /* FUN_040095ac */
extern void     ub_div0_hang(void);                                /* FUN_04004844 (除零陷阱) */
extern void     aeabi_idiv(void);                                  /* FUN_0400c708 (r0/r1 直通) */

/* ----------------------------------------------------------------------------
 * U-Boot 网络全局变量 (命名同上游 v2016.07 net/arp.c、net/net.c、net/nfs.c;
 * 与反编译 0x04008xxx..0x0400b0xx 字面量池引用逐一对应, 语义推断)
 * -------------------------------------------------------------------------- */
extern uint32_t net_ip;                    /* 本机 IP (原 struct in_addr; arp_receive 字面量 DAT_040080f4) */
extern const char DAT_040080f0[];          /* arp_receive 长度告警格式串 "bad length %d < %d\n" — 语义推断 */
extern uint32_t net_arp_wait_packet_ip;    /* 等待 ARP 解析的目标 IP (arp_receive: DAT_040080f8) */
extern uint32_t net_arp_wait_reply_ip;     /* 实际请求的 IP: 目标或网关 (arp_receive: DAT_040080fc) */
extern uint8_t *arp_wait_packet_ethaddr;   /* 挂起包目的 MAC 保存位 (arp_receive: DAT_04008100) */
extern int      arp_wait_tx_packet_size;   /* 挂起包长度 (arp_receive: DAT_04008108) */
extern uint8_t *net_tx_packet;             /* 网络发送缓冲 (arp_receive: DAT_04008104; rpc_req: DAT_0400ae30) */
extern uint8_t  net_ethaddr[6];            /* 本机 MAC (arp_receive: DAT_0400810c; net_set_ether: DAT_0400a4b0) */
extern int      eth_errno;                 /* eth 最近 DM 错误码, eth-uclass.c static (字面量 DAT_04008f9c / DAT_04008fdc) */
extern uint16_t net_our_vlan;              /* 本机 VLAN (net_set_ether: DAT_0400a4ac; net_eth_hdr_size: DAT_0400a418) */
extern ub_rxhand_fp_t arp_packet_handler;  /* ARP RX 处理函数指针 (net_get_arp_handler 读: 字面量 DAT_0400a038) */
extern ub_rxhand_icmp_fp_t packet_icmp_handler; /* ICMP RX 处理函数指针 (net_set_icmp_handler 写: 字面量 DAT_0400a048) */
extern unsigned long rpc_id;               /* RPC 事务序号, nfs.c static (rpc_req: DAT_0400ae28) */
extern const uint32_t DAT_0400ae2c;        /* rpc_req 立即数: htonl(rpc_prog) 的高位补全 (0x00860100,
                                            * rpc_prog ∈ {100000,100003,100005} 时结果正确) — 全局, 语义待定 */
extern uint8_t  net_server_ethaddr[6];     /* 服务器 MAC (rpc_req 实参: 字面量 DAT_0400ae48) */
extern uint32_t nfs_server_ip;             /* NFS 服务器 IP (rpc_req: DAT_0400ae44) */
extern uint16_t nfs_server_mount_port;     /* MOUNT 服务端口 (rpc_req: DAT_0400ae38) */
extern uint16_t nfs_server_port;           /* NFS 服务端口 (rpc_req: DAT_0400ae3c) */
extern uint16_t nfs_our_port;              /* 本机 NFS 请求端口 (rpc_req: DAT_0400ae40) */
extern uint8_t  filefh[NFS_FHSIZE];        /* NFS 文件句柄 (nfs_readlink_req: DAT_0400b0b8) */

/* 字符串字面量 (地址保持追溯; 内容按上游 v2016.07 对应源码推断) */
extern const char DAT_04009638[];          /* eth_setenv_enetaddr 格式串 "%pM" */
extern const char DAT_04009680[];          /* eth_getenv_enetaddr_by_index 格式串 "%s%daddr" */
extern const char DAT_04009684[];          /* eth_getenv_enetaddr_by_index 格式串 "%saddr" */
extern const char DAT_04009750[];          /* eth_get_name 无设备回退串 "unknown" */

/* ----------------------------------------------------------------------------
 * 模块内部前向声明 (定义在本文件后段, 按地址升序)
 * -------------------------------------------------------------------------- */
void *eth_get_uclass_priv(void);
void *eth_get_dev(void);
void  eth_set_dev(void *dev);
void  eth_parse_enetaddr(const char *addr, uint8_t *enetaddr);
ub_rxhand_fp_t net_get_arp_handler(void);
int   net_set_ether(uint8_t *xet, const uint8_t *dest_ethaddr, uint32_t prot);

// ============================================================================
// 函数转写区 (按地址升序)
// ============================================================================

// === arp_receive === 原 FUN_04007efc @ 0x04007efc
// 来源: reversed/uboot_funcs/FUN_04007efc.c | 分类: uboot
// 语义: 处理收到的 ARP 包。长度 < ARP_HDR_SIZE 打印告警; 帧头 (ar_hrd/ar_pro/
//       ar_hln/ar_pln) 校验通过且目标 IP (ar_tpa) 等于本机 net_ip 后, 按 ar_op:
//       REQUEST → 就地改写为 REPLY (tha←sha, tpa←spa, sha←本机 MAC, spa←本机 IP)
//       并经 eth_send 回发; REPLY → 若处于 ARP 等待且 spa == net_arp_wait_reply_ip,
//       保存请求方 MAC、调用 arp 处理器、把 MAC 填入挂起包并发送、清等待状态。
//       偏移注: arp 为 uint16_t 视角, +n 对应字节 2n; ARP 头布局字节 0-27
//       (hrd/pro/hln/pln/op/sha/spa/tha/tpa)。上游 net/arp.c 同名函数; 本固件
//       直接调 eth_send 而非上游的 net_send_packet。
void arp_receive(uint8_t *et, uint16_t *arp, int len)
{
	uint32_t ip_val;             /* 原栈 -0x24: 4 字节 IP 暂存 */
	uint32_t *arp_wait_ip_p;     /* 原指针暂存, 指向 net_arp_wait_packet_ip */
	uint32_t *tx_pktp;           /* 原指针暂存, 指向 net_tx_packet */
	int *tx_sizep;               /* 原指针暂存, 指向 arp_wait_tx_packet_size */
	uint16_t opcode;
	int et_hdr_size;             /* 原整型暂存: net_update_ether 返回的头长 */
	ub_rxhand_fp_t arp_hdl;

	if (len < ARP_HDR_SIZE) {
		ub_printf(DAT_040080f0, len, ARP_HDR_SIZE);   /* "bad length %d < %d\n" */
		return;
	}

	/* 帧头校验: ntohs(ar_hrd)==ARP_ETHER && ntohs(ar_pro)==PROT_IP && ar_hln==6 && ar_pln==4 */
	if (((uint16_t)(arp[0] >> 8 | arp[0] << 8) == ARP_ETHER) &&
	    ((uint16_t)(arp[1] >> 8 | arp[1] << 8) == PROT_IP) &&
	    ((uint8_t *)arp)[4] == 6 && ((uint8_t *)arp)[5] == 4) {
		/* 本机 IP 必须已设置 (原 *DAT_040080f4 间接读) */
		if (net_ip != 0) {
			ub_memcpy(&ip_val, arp + 12, 4);      /* ar_tpa, 字节 24 */
			arp_wait_ip_p = &net_arp_wait_packet_ip;
			if (net_ip == ip_val) {
				opcode = (uint16_t)(arp[3] >> 8 | arp[3] << 8);  /* ar_op, 字节 6 */

				if (opcode == ARPOP_REQUEST) {
					/* 回复目的 MAC 用请求方 et_src (帧内字节 6) */
					et_hdr_size = net_update_ether(et, et + 6, PROT_ARP);
					arp[3] = 0x200;                        /* ar_op = htons(ARPOP_REPLY) */
					ub_memcpy(arp + 9, arp + 4, 6);        /* ar_tha(18) ← ar_sha(8) */
					ub_memcpy(arp + 12, arp + 7, 4);       /* ar_tpa(24) ← ar_spa(14) */
					ub_memcpy(arp + 4, net_ethaddr, 6);    /* ar_sha ← 本机 MAC */
					ub_memcpy(arp + 7, &net_ip, 4);        /* ar_spa ← 本机 IP (原 DAT_040080f4 间接) */
					eth_send(et, et_hdr_size + ARP_HDR_SIZE);
				}
				else if (opcode == ARPOP_REPLY) {
					/* 是否处于 ARP 等待 (原 *DAT_040080f8 间接读) */
					if (*arp_wait_ip_p != 0) {
						uint32_t reply_ip;

						ub_memcpy(&reply_ip, arp + 7, 4);  /* ar_spa, 字节 14 */
						if (reply_ip == net_arp_wait_reply_ip) {   /* 原 *DAT_040080fc */
							if (arp_wait_packet_ethaddr != NULL) { /* 原 *DAT_04008100 */
								ub_memcpy(arp_wait_packet_ethaddr, arp + 4, 6);
							}
							arp_hdl = net_get_arp_handler();
							arp_hdl((uint8_t *)arp, 0, reply_ip, 0, len);
							tx_pktp = (uint32_t *)&net_tx_packet;      /* 原 DAT_04008104 */
							tx_sizep = &arp_wait_tx_packet_size;       /* 原 DAT_04008108 */
							ub_memcpy((void *)*tx_pktp, arp + 4, 6);   /* 挂起包 et_dest ← ar_sha */
							eth_send((void *)*tx_pktp, *tx_sizep);
							/* 清 ARP 等待状态 */
							*arp_wait_ip_p = 0;
							*tx_sizep = 0;
							arp_wait_packet_ethaddr = NULL;
						}
					}
				}
			}
		}
	}
	return;
}

// === eth_get_uclass_priv === 原 FUN_04008c70 @ 0x04008c70
// 来源: reversed/uboot_funcs/FUN_04008c70.c | 分类: uboot
// 语义: 取 ETH uclass 私有数据 (struct eth_uclass_priv, 首成员为 current 设备指针)。
//       经 DM 桩 fun_03fd0cb8 (语义推断: uclass_get(UCLASS_ETH=0x13, &uc)) 得
//       struct uclass 指针后返回其首成员 priv (uclass 偏移 0); 反编译对其余
//       实参不作使用 (Ghidra 寄存器噪声, 已略)。
void *eth_get_uclass_priv(void)
{
	uint32_t *uc = NULL;                 /* 原栈 -0x0c/-0x08 双字槽: uclass 指针出参 */

	fun_03fd0cb8(UCLASS_ETH, &uc);       /* 命名待补: 原 func_0x03fd0cb8 */
	return (void *)*uc;                  /* uc->priv */
}

// === on_ethaddr === 原 FUN_04008c90 @ 0x04008c90
// 来源: reversed/uboot_funcs/FUN_04008c90.c | 分类: uboot
// 语义: 环境变量回调 ("ethaddr"/"ethNaddr")。从变量名偏移 3 解析序号 N ("ethaddr"
//       无数字时解析为 0), 经 DM 桩 fun_03fd0ed0 (语义推断:
//       uclass_find_device_by_seq(UCLASS_ETH, N, false, &dev)) 找到设备后按 env_op
//       分支: op==1 → 清零 enetaddr (delete); op==0/2 → 解析 value 写入 enetaddr
//       (create/overwrite, 本固件枚举序)。恒返回 0。
//       偏移注: udevice 偏移 8 = platdata; eth_pdata 偏移 4 = enetaddr[6]。
int on_ethaddr(const char *name, const char *value, int op /* env_op */, int flags)
{
	int index;
	uint32_t *dev;

	/* "eth" 之后的序号 */
	index = simple_strtoul(name + 3, NULL, 10);

	if (fun_03fd0ed0(UCLASS_ETH, index, 0, &dev) == 0) {   /* 命名待补: 原 func_0x03fd0ed0 */
		uint8_t *enetaddr = (uint8_t *)dev[2] + 4;         /* dev->platdata + 4 */

		if (op == 1) {
			ub_memset(enetaddr, 0, 6);
		}
		else if ((op == 0) || (op == 2)) {
			eth_parse_enetaddr(value, enetaddr);
		}
	}
	return 0;
}

// === eth_set_current_to_next === 原 FUN_04008f30 @ 0x04008f30
// 来源: reversed/uboot_funcs/FUN_04008f30.c | 分类: uboot
// 语义: 当前 eth 设备切换到下一个; 若已为空 (到末尾) 则回到第一个设备。
//       注意: 原反编译在本函数末段遇到 bad instruction 数据被截断, 截断段按
//       FUN_04008f68 (eth_get_dev) 中完全相同的调用模式补齐为
//       fun_03fd1098(UCLASS_ETH, &current) (上游: uclass_first_device), 已标注。
void eth_set_current_to_next(void)
{
	uint32_t *uc_priv = (uint32_t *)eth_get_uclass_priv();   /* 首成员 = current */

	if (*uc_priv != 0) {
		fun_03fd10d0(uc_priv);           /* 命名待补: 原 func_0x03fd10d0 (语义推断: uclass_next_device(&current)) */
	}
	if (*uc_priv == 0) {
		/* 原反编译此处因 bad instruction 截断; 依 FUN_04008f68 同模式补齐 */
		fun_03fd1098(UCLASS_ETH, uc_priv);   /* 语义推断: uclass_first_device(UCLASS_ETH, &current) */
	}
	return;
}

// === eth_get_dev === 原 FUN_04008f68 @ 0x04008f68
// 来源: reversed/uboot_funcs/FUN_04008f68.c | 分类: uboot
// 语义: 返回当前 eth 设备; 当前为空时先探测第一个 ETH 设备
//       (fun_03fd1098: uclass_first_device(UCLASS_ETH, &current)), DM 错误码
//       记入 eth_errno (原经字面量 DAT_04008f9c 间接写入)。
void *eth_get_dev(void)
{
	uint32_t *uc_priv = (uint32_t *)eth_get_uclass_priv();

	if (*uc_priv == 0) {
		eth_errno = fun_03fd1098(UCLASS_ETH, uc_priv);
	}
	return (void *)*uc_priv;
}

// === eth_set_dev === 原 FUN_04008fa0 @ 0x04008fa0
// 来源: reversed/uboot_funcs/FUN_04008fa0.c | 分类: uboot
// 语义: 设置当前 eth 设备。dev 非 NULL 且未激活 (udevice 偏移 0x48 的 flags
//       bit0 = DM_FLAG_ACTIVE) 时先探测 (fun_03fd031c: device_probe), 失败则
//       当前设备置空; DM 错误码记入 eth_errno (原经字面量 DAT_04008fdc 间接写入)。
void eth_set_dev(void *dev)
{
	if (dev != NULL) {
		if ((*(uint32_t *)((uint8_t *)dev + 0x48) & 1) == 0) {   /* 未激活才探测 */
			int err = fun_03fd031c(dev);     /* 命名待补: 原 func_0x03fd031c (语义推断: device_probe) */

			eth_errno = err;
			if (err != 0) {
				dev = NULL;                  /* 探测失败 → 当前设备置空 (原直落 04008fd0 前的赋值) */
			}
		}
	}
	*(uint32_t *)eth_get_uclass_priv() = (uint32_t)(uintptr_t)dev;   /* uc_priv->current = dev */
	return;
}

// === eth_pre_unbind === 原 FUN_04008fe0 @ 0x04008fe0
// 来源: reversed/uboot_funcs/FUN_04008fe0.c | 分类: uboot
// 语义: ETH 设备解绑前回调: 若解绑的正是当前设备, 将当前设备置空 (eth_set_dev(NULL))
//       避免悬挂指针。恒返回 0。
int eth_pre_unbind(void *dev)
{
	if (*(uint32_t *)eth_get_uclass_priv() == (uint32_t)(uintptr_t)dev) {
		eth_set_dev(NULL);
	}
	return 0;
}

// === eth_halt_state_only === 原 FUN_04009118 @ 0x04009118
// 来源: reversed/uboot_funcs/FUN_04009118.c | 分类: uboot
// 语义: 仅置状态而不调用驱动 stop: 当前设备激活 (udevice 偏移 0x48 的 flags bit0)
//       时把其 uclass_priv (udevice 偏移 0x28) 首成员 state 置 1 (ETH_STATE_PASSIVE)。
void eth_halt_state_only(void)
{
	void *current = eth_get_dev();

	if (current == NULL)
		return;
	if ((*(uint32_t *)((uint8_t *)current + 0x48) & 1) != 0) {
		**(uint32_t **)((uint8_t *)current + 0x28) = 1;   /* priv->state = ETH_STATE_PASSIVE */
	}
	return;
}

// === eth_parse_enetaddr === 原 FUN_04009550 @ 0x04009550
// 来源: reversed/uboot_funcs/FUN_04009550.c | 分类: uboot
// 语义: 解析 "xx:xx:xx:xx:xx:xx" 形式 MAC 串到 6 字节 enetaddr (addr 为 NULL 时
//       写 0)。循环 6 次 simple_strtoul(base 16), 每轮从分隔符后一位继续; 与上游
//       string_to_enetaddr/eth_parse_enetaddr 一致。
void eth_parse_enetaddr(const char *addr, uint8_t *enetaddr)
{
	char *end;                       /* 原栈 -0x14: simple_strtoul 出参 endp */
	uint8_t *out = enetaddr - 1;     /* 原字符指针暂存, 写入游标 */

	do {
		uint32_t octet;

		if (addr == NULL) {
			octet = 0;
		}
		else {
			octet = simple_strtoul(addr, &end, 0x10);
		}
		out = out + 1;
		*out = (uint8_t)octet;
		if (addr != NULL) {
			addr = end;
			if (*end != '\0')
				addr = end + 1;      /* 跳过分隔符 */
		}
	} while (enetaddr + 5 != out);
	return;
}

// === eth_setenv_enetaddr === 原 FUN_04009608 @ 0x04009608
// 来源: reversed/uboot_funcs/FUN_04009608.c | 分类: uboot
// 语义: 把 enetaddr 按格式串 (原 DAT_04009638, "%pM") 格式化进 20 字节缓冲并
//       写入环境变量 name (setenv, DM 桩 fun_03fbeb7c)。
void eth_setenv_enetaddr(const char *name, const uint8_t *enetaddr)
{
	char buf[20];                    /* 原栈 -0x1c 起 20 字节 */

	ub_sprintf(buf, DAT_04009638, enetaddr);
	fun_03fbeb7c(name, buf);         /* 命名待补: 原 func_0x03fbeb7c (语义推断: setenv) */
	return;
}

// === eth_getenv_enetaddr_by_index === 原 FUN_0400963c @ 0x0400963c
// 来源: reversed/uboot_funcs/FUN_0400963c.c | 分类: uboot
// 语义: 构造变量名: index==0 用格式串 DAT_04009684 ("%saddr"), 否则 DAT_04009680
//       ("%s%daddr"); 随后 eth_getenv_enetaddr 取 env 并解析到 enetaddr。
//       (上游函数返回解析结果; 本反编译为 void 且调用点未使用返回值)
void eth_getenv_enetaddr_by_index(const char *base_name, int index, uint8_t *enetaddr)
{
	char enetvar[32];                /* 原栈 -0x28 起 32 字节 */
	const char *fmt;

	if (index == 0) {
		fmt = DAT_04009684;
	}
	else {
		fmt = DAT_04009680;
	}
	ub_sprintf(enetvar, fmt, base_name, index);
	eth_getenv_enetaddr(enetvar, enetaddr);
	return;
}

// === eth_common_init === 原 FUN_040096d4 @ 0x040096d4
// 来源: reversed/uboot_funcs/FUN_040096d4.c | 分类: uboot
// 语义: DM 与 legacy 共用的网络前段初始化: bootstage_mark(0x40 /* BOOTSTAGE_ID_NET_ETH_START */)
//       与 miiphy_init()。原反编译在第二个调用之后遇 bad instruction 截断
//       (上游对应 CONFIG_PHYLIB 下的 phy_init(), 本固件是否存在无法确认, 不臆造)。
void eth_common_init(void)
{
	fun_03fbf43c(0x40);              /* 命名待补: 原 func_0x03fbf43c (语义推断: bootstage_mark) */
	fun_03fc4768();                  /* 命名待补: 原 func_0x03fc4768 (语义推断: miiphy_init) */
	/* 原反编译此处因 bad instruction 截断, 控制流不完整 */
	return;
}

// === eth_get_name === 原 FUN_0400972c @ 0x0400972c
// 来源: reversed/uboot_funcs/FUN_0400972c.c | 分类: uboot
// 语义: 返回当前 eth 设备名 (udevice 偏移 4 = name); 无设备时返回字符串
//       DAT_04009750 ("unknown")。与上游一致: eth_get_dev() 调用两次。
const char *eth_get_name(void)
{
	uint32_t dev;

	dev = (uint32_t)(uintptr_t)eth_get_dev();
	if (dev == 0) {
		return DAT_04009750;
	}
	dev = (uint32_t)(uintptr_t)eth_get_dev();
	return *(const char **)(uintptr_t)(dev + 4);
}

// === eth_set_current === 原 FUN_04009864 @ 0x04009864
// 来源: reversed/uboot_funcs/FUN_04009864.c | 分类: uboot
// 语义: 按 "ethact" 环境变量恢复当前 eth 设备 (static 局部 act/env_changed_id,
//       原经字面量 DAT_040098e4/DAT_040098e8 间接访问)。环境序号 (get_env_id)
//       变化或 act 为空时重读 env。act==NULL: 改用 "ethprime", 找不到该设备则置空;
//       否则按设备名 act 查找。最后 eth_set_dev + eth_current_changed()。
//       注: 原反编译中 eth_get_dev_by_name(FUN_04009008) 的实参未显式示出
//       (Ghidra 未跟踪寄存器), 按分支语义与上游补为 ethprime/act。
void eth_set_current(void)
{
	static char *act;                /* 原 DAT_040098e4 间接访问的 static 局部 */
	static int env_changed_id;       /* 原 DAT_040098e8 间接访问的 static 局部 */
	int env_id;
	void *dev;

	env_id = fun_03fbeb6c();         /* 命名待补: 原 func_0x03fbeb6c (语义推断: get_env_id) */
	if ((act == NULL) || (env_changed_id != env_id)) {
		act = fun_03fbefb0("ethact");            /* 原 func_0x03fbefb0 (getenv), "ethact" 原字面量 DAT_040098ec */
		env_changed_id = env_id;
	}
	if (act == NULL) {
		char *ethprime = fun_03fbefb0("ethprime");   /* "ethprime" 原字面量 DAT_040098f0 */

		if (ethprime == NULL) {
			dev = NULL;
		}
		else {
			dev = eth_get_dev_by_name(ethprime); /* 查不到时返回 NULL, 结果同置空 */
		}
	}
	else {
		dev = eth_get_dev_by_name(act);
	}
	eth_set_dev(dev);
	eth_current_changed();
	return;
}

// === net_set_icmp_handler === 原 FUN_0400a03c @ 0x0400a03c
// 来源: reversed/uboot_funcs/FUN_0400a03c.c | 分类: uboot
// 语义: 注册 ICMP RX 处理函数到全局指针 packet_icmp_handler (原经字面量
//       DAT_0400a048 间接写入; 上游 net.c static rxhand_icmp_f *packet_icmp_handler)。
void net_set_icmp_handler(ub_rxhand_icmp_fp_t handler)
{
	packet_icmp_handler = handler;
	return;
}

// === net_set_ether === 原 FUN_0400a41c @ 0x0400a41c
// 来源: reversed/uboot_funcs/FUN_0400a41c.c | 分类: uboot
// 语义: 构造以太帧头: et_dest ← dest_ethaddr, et_src ← 本机 net_ethaddr; 按
//       net_our_vlan (原 DAT_0400a4ac, 先 ntohs; 0xffff 视为 VLAN_NONE=0xfff) 决定:
//       无 VLAN → et_protlen(偏移 0xc) = htons(prot), 返回 ETHER_HDR_SIZE(14);
//       有 VLAN → vet_vlan_type(0xc) = 0x8100(网络序, 反编译写 0x0081)、
//       vet_tag(0xe) = htons(vid)、vet_type(0x10) = htons(prot), 返回 18。
int net_set_ether(uint8_t *xet, const uint8_t *dest_ethaddr, uint32_t prot)
{
	uint32_t myvlanid;
	uint16_t prot_swapped;           /* htons(prot) */

	/* ntohs(net_our_vlan) (原经字面量 DAT_0400a4ac 间接读) */
	myvlanid = (uint32_t)(uint16_t)((net_our_vlan >> 8) | (net_our_vlan << 8));
	if (myvlanid == 0xffff) {
		myvlanid = VLAN_NONE;        /* (ushort)-1 → VLAN_NONE */
	}
	ub_memcpy(xet, dest_ethaddr, 6);             /* et->et_dest */
	ub_memcpy(xet + 6, net_ethaddr, 6);          /* et->et_src ← 本机 MAC (原 DAT_0400a4b0) */
	prot_swapped = (uint16_t)(0xff00 & (uint16_t)(prot << 8) |
	                          (uint16_t)((uint32_t)prot >> 8) & 0xff);   /* 原 DAT_0400a4b4 = 0xff00 掩码 */

	if ((myvlanid & VLAN_IDMASK) == VLAN_NONE) {
		*(uint16_t *)(xet + 0xc) = prot_swapped;     /* et->et_protlen = htons(prot) */
		return ETHER_HDR_SIZE;
	}
	*(uint16_t *)(xet + 0xc) = 0x81;                 /* vet_vlan_type = 0x8100 (网络序) */
	*(uint16_t *)(xet + 0xe) = (uint16_t)((myvlanid & 0xff) << 8) |
	                           (uint16_t)((myvlanid & VLAN_IDMASK) >> 8);   /* vet_tag = htons(vid) */
	*(uint16_t *)(xet + 0x10) = prot_swapped;        /* vet_type = htons(prot) */
	return VLAN_ETHER_HDR_SIZE;
}

// === basename === 原 FUN_0400ac64 @ 0x0400ac64
// 来源: reversed/uboot_funcs/FUN_0400ac64.c | 分类: uboot
// 语义: 返回路径最后一个 '/' 之后的文件名部分 (返回 fname+1); 从串尾向前扫描,
//       无 '/' 时越过起点返回 (fname = path-1, 与上游 nfs.c basename 一致)。
char *basename(char *path)
{
	char *cur;                       /* 原字符指针暂存, 当前扫描位 */
	char *fname;
	int len;

	len = ub_strlen(path);           /* 原 FUN_04005b98 (实参 path, Ghidra 未显式示出) */
	fname = path + len - 1;
	do {
		cur = fname;
		if (cur < path) {
			return cur;              /* 无 '/' → 返回 path-1 */
		}
		fname = cur - 1;
	} while (*cur != '/');
	return cur + 1;
}

// === rpc_req === 原 FUN_0400ad48 @ 0x0400ad48
// 来源: reversed/uboot_funcs/FUN_0400ad48.c | 分类: uboot
// 语义: 构造 RPC 调用包头 (id/type/rpcvers/prog/vers/proc, 共 6 字 = 0x18 字节)
//       并拷贝调用参数 data (datalen 个 32 位字), 头 + 数据整体拷入
//       net_tx_packet + net_eth_hdr_size() + IP_UDP_HDR_SIZE(0x1c) 后经
//       net_send_udp_packet 发往 NFS 服务器。端口选择: prog==PROG_PORTMAP →
//       SUNRPC_PORT(111); prog==PROG_MOUNT → nfs_server_mount_port; 其余 →
//       nfs_server_port。id 为自增序号 rpc_id (原 DAT_0400ae28 间接)。
//       注: htonl 的实现形态按反编译原样保留 (prog/proc 的字节反转仅保留低
//       位移位部分, 上游 v2016.07 为完整 htonl)。
void rpc_req(int rpc_prog, int rpc_proc, uint32_t *data, int datalen)
{
	uint32_t pkt_hdr[4];                 /* 原栈 -0x810 起 16 字节: id/type/rpcvers/prog */
	uint32_t vers;                       /* 原栈 -0x800 (包头偏移 0x10): 程序版本 */
	uint32_t proc;                       /* 原栈 -0x7fc (包头偏移 0x14): 过程号 */
	uint8_t data_area[2024];             /* 原栈 -0x7f8 起 2024 字节: 调用参数区 (包头偏移 0x18) */
	uint32_t id;
	int pktlen;
	int sport;
	int eth_hdr_size;
	uint8_t *tx_base;                    /* 原整型暂存: net_tx_packet 值 */

	id = rpc_id + 1;
	rpc_id = id;
	pkt_hdr[0] = id * 0x1000000 | (id >> 8 & 0xff) << 0x10 |
	             (id >> 0x10 & 0xff) << 8 | id >> 0x18;    /* htonl(id) */
	pkt_hdr[1] = 0;                                          /* htonl(MSG_CALL=0) */
	pkt_hdr[2] = 0x2000000;                                  /* htonl(2): rpcvers 2 (网络序, LE 存储 0x02000000) */
	vers = 0x2000000;                                        /* htonl(2): 程序版本 2 */
	pkt_hdr[3] = DAT_0400ae2c | rpc_prog << 0x18;            /* htonl(rpc_prog) — 见 DAT_0400ae2c 注 */
	proc = rpc_proc << 0x18;                                 /* htonl(rpc_proc) (proc < 256 时等价) */

	if (datalen != 0) {
		ub_memcpy(data_area, data, datalen * 4);
	}
	pktlen = datalen * 4 + 0x18;
	tx_base = (uint8_t *)net_tx_packet;                      /* 原 *DAT_0400ae30 */
	eth_hdr_size = net_eth_hdr_size();
	ub_memcpy(tx_base + eth_hdr_size + IP_UDP_HDR_SIZE, pkt_hdr, pktlen);  /* 头+参数整体 (原从包头首字起 0x18+datalen*4) */

	if (rpc_prog == PROG_PORTMAP) {                          /* 原 DAT_0400ae34 */
		sport = SUNRPC_PORT;                                 /* 0x6f */
	}
	else {
		if (rpc_prog == PROG_MOUNT) {                        /* 原 DAT_0400ae34 + 5 (100005) */
			sport = nfs_server_mount_port;                   /* 原 *DAT_0400ae38 */
		}
		else {
			sport = nfs_server_port;                         /* 原 *DAT_0400ae3c */
		}
	}
	net_send_udp_packet(net_server_ethaddr, nfs_server_ip, sport,
	                    nfs_our_port, pktlen);               /* 实参字面量: DAT_0400ae48/DAT_0400ae44/DAT_0400ae40 */
	return;
}

// === nfs_mount_req === 原 FUN_0400ae4c @ 0x0400ae4c
// 来源: reversed/uboot_funcs/FUN_0400ae4c.c | 分类: uboot
// 语义: 构造 MOUNT_ADDENTRY(1) 请求: 先 rpc_add_credentials 填 AUTH_UNIX 凭证,
//       再写 htonl(pathlen) 与路径串 (4 字节对齐补零), len 为 32 位字数, 交
//       rpc_req(PROG_MOUNT=100005, 原 DAT_0400aee0, 1, data, len) 发送。
//       注: 反编译中 pathlen<0 的符号修正分支为死代码 (strlen ≥ 0), 已折叠;
//       memset 补零与上游 *(p + pathlen/4) = 0 等价。
void nfs_mount_req(char *path)
{
	uint32_t data[1024];                 /* 原栈 -0x1018 起 4096 字节 */
	uint32_t *p;                         /* 原 32 位字指针 */
	uint32_t pathlen;                    /* 原无符号整型: strlen(path) */

	pathlen = ub_strlen(path);
	p = rpc_add_credentials(data);
	/* *p++ = htonl(pathlen): 32 位字节反转 */
	*p = pathlen << 0x18 | (pathlen >> 8 & 0xff) << 0x10 |
	     (pathlen >> 0x10 & 0xff) << 8 | pathlen >> 0x18;
	p = p + 1;
	if ((pathlen & 3) != 0) {
		p[pathlen / 4] = 0;              /* 尾部对齐补零 (p 已前移一字, 与上游 *(p + pathlen/4) = 0 一致) */
	}
	ub_memcpy(p, path, pathlen);
	p = p + (pathlen + 3) / 4;
	rpc_req(PROG_MOUNT, MOUNT_ADDENTRY, data,
	        (int)((uint8_t *)p - (uint8_t *)data) >> 2);
	return;
}

// === nfs_readlink_req === 原 FUN_0400b068 @ 0x0400b068
// 来源: reversed/uboot_funcs/FUN_0400b068.c | 分类: uboot
// 语义: 构造 NFS_READLINK(5) 请求: rpc_add_credentials 后拷入 32 字节文件句柄
//       (filefh, NFS_FHSIZE), len 为 32 位字数, 交 rpc_req(PROG_NFS=100003,
//       原 DAT_0400b0bc, 5, data, len) 发送。
void nfs_readlink_req(void)
{
	uint32_t data[1024];                 /* 原栈 -0x1010 起 4100 字节 */
	uint32_t *p;

	p = rpc_add_credentials(data);
	ub_memcpy(p, filefh, NFS_FHSIZE);    /* 原 DAT_0400b0b8 */
	rpc_req(PROG_NFS, NFS_READLINK, data,
	        (int)((uint8_t *)p + NFS_FHSIZE - (uint8_t *)data) >> 2);
	return;
}

// === aeabi_uidiv === 原 FUN_0400c66c @ 0x0400c66c
// 来源: reversed/uboot_funcs/FUN_0400c66c.c | 分类: uboot
// 语义: libgcc AEABI 无符号整除 __aeabi_uidiv, 返回 dividend / divisor 的商。
//       特例: 除数 1 → 返回被除数; 除数 0 → 走除零陷阱 (ub_div0_hang);
//       被除数 ≤ 除数 → 商 0/1; 除数为 2 的幂 → 移位; 一般情形: 归一化后
//       按每轮 4 位的 srt2 除法恢复商 (LZCOUNT 为 ARM CLZ 指令)。
uint32_t aeabi_uidiv(uint32_t dividend, uint32_t divisor)   /* 原第一参=被除数, 第二参=除数 */
{
	uint32_t denom;                      /* 归一化除数 */
	uint32_t quotient_bit;               /* 本轮商位掩码 */
	uint32_t quotient;                   /* 累计商 */
	uint32_t shift;

	if (divisor - 1 == 0) {
		return dividend;                 /* 除 1 */
	}
	if (divisor == 0) {
		ub_div0_hang();                  /* 原 thunk_FUN_04004844 → FUN_04004844 (ub_div0_hang) */
		return 0;
	}
	if (dividend <= divisor) {
		return (uint32_t)(dividend == divisor);   /* 被除数 ≤ 除数: 商为 0/1 */
	}
	if ((divisor & (divisor - 1)) == 0) {
		/* 除数为 2 的幂: 归一化移位即可 */
		return dividend >> ((31 - __builtin_clz(divisor)) & 0xff);
	}
	/* 归一化: 把除数对齐到被除数的最高位 */
	shift = (__builtin_clz(divisor) - __builtin_clz(dividend)) & 0xffU;
	denom = divisor << shift;
	quotient_bit = 1 << shift;
	quotient = 0;
	while (1) {
		if (denom <= dividend) {
			dividend = dividend - denom;
			quotient = quotient | quotient_bit;
		}
		if ((denom >> 1) <= dividend) {
			dividend = dividend - (denom >> 1);
			quotient = quotient | quotient_bit >> 1;
		}
		if ((denom >> 2) <= dividend) {
			dividend = dividend - (denom >> 2);
			quotient = quotient | quotient_bit >> 2;
		}
		if ((denom >> 3) <= dividend) {
			dividend = dividend - (denom >> 3);
			quotient = quotient | quotient_bit >> 3;
		}
		if (dividend == 0) {
			break;
		}
		quotient_bit = quotient_bit >> 4;
		if (quotient_bit == 0) {
			break;
		}
		denom = denom >> 4;
	}
	return quotient;
}

// === aeabi_idivmod === 原 FUN_0400c7ec @ 0x0400c7ec
// 来源: reversed/uboot_funcs/FUN_0400c7ec.c | 分类: uboot
// 语义: libgcc AEABI 有符号"除+取模"入口 __aeabi_idivmod: 全部逻辑委托给
//       FUN_0400c708 (aeabi_idiv, name_map); 被除数/除数经 r0/r1 直通, 商在
//       r0、余数在 r1 返回 (调用约定), 本函数无局部逻辑。
void aeabi_idivmod(void)
{
	aeabi_idiv();
	return;
}

/* ============================================================================
 * 覆盖率表 (uboot_b1 后 22/22)
 * ==========================================================================
 * #  函数名                   原地址        行起始  来源反编译
 * 1  arp_receive              0x04007efc  见上  reversed/uboot_funcs/FUN_04007efc.c
 * 2  eth_get_uclass_priv      0x04008c70  见上  reversed/uboot_funcs/FUN_04008c70.c
 * 3  on_ethaddr               0x04008c90  见上  reversed/uboot_funcs/FUN_04008c90.c
 * 4  eth_set_current_to_next  0x04008f30  见上  reversed/uboot_funcs/FUN_04008f30.c
 * 5  eth_get_dev              0x04008f68  见上  reversed/uboot_funcs/FUN_04008f68.c
 * 6  eth_set_dev              0x04008fa0  见上  reversed/uboot_funcs/FUN_04008fa0.c
 * 7  eth_pre_unbind           0x04008fe0  见上  reversed/uboot_funcs/FUN_04008fe0.c
 * 8  eth_halt_state_only      0x04009118  见上  reversed/uboot_funcs/FUN_04009118.c
 * 9  eth_parse_enetaddr       0x04009550  见上  reversed/uboot_funcs/FUN_04009550.c
 * 10 eth_setenv_enetaddr      0x04009608  见上  reversed/uboot_funcs/FUN_04009608.c
 * 11 eth_getenv_enetaddr_by_index 0x0400963c 见上 reversed/uboot_funcs/FUN_0400963c.c
 * 12 eth_common_init          0x040096d4  见上  reversed/uboot_funcs/FUN_040096d4.c
 * 13 eth_get_name             0x0400972c  见上  reversed/uboot_funcs/FUN_0400972c.c
 * 14 eth_set_current          0x04009864  见上  reversed/uboot_funcs/FUN_04009864.c
 * 15 net_set_icmp_handler     0x0400a03c  见上  reversed/uboot_funcs/FUN_0400a03c.c
 * 16 net_set_ether            0x0400a41c  见上  reversed/uboot_funcs/FUN_0400a41c.c
 * 17 basename                 0x0400ac64  见上  reversed/uboot_funcs/FUN_0400ac64.c
 * 18 rpc_req                  0x0400ad48  见上  reversed/uboot_funcs/FUN_0400ad48.c
 * 19 nfs_mount_req            0x0400ae4c  见上  reversed/uboot_funcs/FUN_0400ae4c.c
 * 20 nfs_readlink_req         0x0400b068  见上  reversed/uboot_funcs/FUN_0400b068.c
 * 21 aeabi_uidiv              0x0400c66c  见上  reversed/uboot_funcs/FUN_0400c66c.c
 * 22 aeabi_idivmod            0x0400c7ec  见上  reversed/uboot_funcs/FUN_0400c7ec.c
 *
 * 未消 Ghidra 残留说明:
 * - 0x03FDxxxx 调用桩以 fun_ 前缀占位 (命名待补), 语义推断见声明区注释;
 * - DAT_040080f0/0400ae2c 等未知全局/立即数按约定保留原名并注释;
 * - FUN_04008f30/040096d4 原反编译遇 bad instruction 截断, 已标注补齐依据。
 */

