/*
 * rewritten/cgminer/api_p1a.c — cgminer API 层第 1 批 A 组 (语义化转写)
 *
 * 来源: reversed/cgminer/30_api_status/FUN_0001b8b4.c ~ FUN_0003305c.c
 * 范围: 8 个函数 — 统计清零 (zero_stats) + API 基础设施
 *       (escape_string / check_connect / send_result / mcast) +
 *       api_data 环链表 (api_add_data_full / print_data) + message() 应答封装
 *
 * 事实还原 (全部以反编译偏移 / name_map 为证):
 *   - zero_stats 与上游 cgminer.c 同名函数一致: 清零 best_diff/best_share 并
 *     遍历 pools 清 pool->best_diff。
 *   - escape_string / check_connect / send_result / mcast / api_add_data_full /
 *     print_data / message 均与 api.c 同名函数一致。
 *   - api_add_data_full 按 type 枚举 (0..26, 0xe=bool 1 字节等) 拷贝数据并
 *     构造 prev/next 双向环形链表; NULL 数据以 NULLSTR "(null)" 兜底。
 *   - print_data 输出 JSON ("{"…"}") 与文本 ("|" 分隔) 两种形态, type 未知时
 *     打 "API: unknown2 data type %d ignored"。
 *   - message() 依 codes[] 表 (DAT_000760fc, 16 字节/项:
 *     {severity, code, 参数类型, 消息串}) 查 severity(I/S/W/E) 与参数并经
 *     print_data 输出 {"STATUS":[ … ]} 应答; Description = opt_api_description
 *     (默认 "cgminer 4.9.0")。
 *
 * 注: 下方 extern 声明均取自 name_map 语义名, 定义分布在其余批次文件;
 *     最终应上移至 rewritten/include/*.h 共享。
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <pthread.h>     /* pthread_mutex_t/lock/unlock (DAT_0007b384 全局锁) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>      /* __ctype_toupper_loc (toupper 表常量折叠) */
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>   /* struct timeval */
#include <sys/select.h> /* fd_set/fd_bits (send_result 内联 FD_ZERO/FD_SET) */

/* ================= 外部声明 (name_map 语义名; 定义在其余批次文件) ================= */
extern void    cgtime(struct timeval *tv);                    /* FUN_0002a84c: gettimeofday 包装 */
extern void    copy_timeval(void *dst, const void *src);      /* FUN_0002a530: 8 字节 timeval 拷贝 dst<-src */
extern void    zero_bestshare(void);                          /* FUN_000160b4 (cgminer.c) */
extern void   *get_devices(int index);                        /* FUN_0001b83c: 按索引返回设备 (cgpu) */
extern void    mutex_lock_fail_cgminer(const char *fn, int line);    /* FUN_0000b414 */
extern void    mutex_unlock_fail_cgminer(const char *fn, int line);  /* FUN_0000b5c4 */
extern void    applog(uint32_t prio, char *buf, ...); /* FUN_00038730 */
extern void    quit(int status);                                     /* FUN_00016ca8: 不返回 */
extern unsigned int cgsleep_ms(unsigned int ms);                    /* FUN_0002a92c (r0 直通) */
/* mcast 组播配置全局 (名称语义由上游 opt_api_* 结构锚定) */
extern int     DAT_00075ebc;         /* opt_api_mcast_port */
extern char   *DAT_00075ec8;         /* opt_api_mcast_addr */
extern char   *DAT_00075ec4;         /* opt_api_mcast_code ("FTW") */
extern int     DAT_00075ed0;         /* opt_api_port */
extern char   *DAT_00075ec0;         /* opt_api_mcast_des */

/* ================= 类型 ================= */

/* api_add_* 链表节点 (布局由本文件 api_add_data_full 偏移证实;
 * 定义与 api_p2.c 一致) */
typedef struct api_data api_data_t;
struct api_data {
	int        type;   /* +0x00: api_data_type 枚举 (0=ESCAPE 1=STRING 2=CONST
		            * 3=UINT8 4=UINT16 5=SHORT 6=INT 7=UINT 8=UINT32 9=INT32
		            * 10=UINT64 11=INT64 12=DOUBLE 13=ELAPSED 14=UTILITY(bool)
		            * 15=TIMEVAL 16=TIME 17=MHS 18=MHTOTAL 19=TEMP 20=UTILITY2
		            * 24=DIFF 25=PERCENT …) */
	const char *name;  /* +0x04 */
	char      *data;   /* +0x08 (按 type 拷贝/借用) */
	int        copy;   /* +0x0C: bool, data 是否已拷贝 */
	api_data_t *prev;  /* +0x10: 环链表 — 头节点 prev 恒指向尾 */
	api_data_t *next;  /* +0x14 */
};

extern int     DAT_0007b198;         /* total_pools: 已配置矿池数 */
extern void  **DAT_0007b438;         /* pools: 矿池指针数组基址 */
extern int     DAT_0007b720;         /* total_devices: 设备总数 */
extern void  (*DAT_00075ed8)(void);  /* 全局函数指针: ELF 初值 noop_hook (FUN_00028ae0, 恒 0) */

/* 全局统计/滚动窗口数据 (zero_stats 清零目标), 布局语义待定 */
extern int     DAT_000770d0[];       /* timeval total_start (cgtime 写入) */
extern int     DAT_000770d8[];       /* timeval total_runtime */
extern uint32_t DAT_000770e0[];      /* 滚动窗口数组起点 */
extern uint32_t DAT_00077140;        /* 滚动窗口数组终点(不含) */
extern int     DAT_00077144;
/* zero_stats 清零的全局统计标量 (多数为 8 字节成对字段, 转写用 uint64_t 别名写) */
extern int     DAT_0007bc98, DAT_0007bb10, DAT_0007b2f8, DAT_0007bc88, DAT_0007acc0;
extern int     DAT_0007b8a8, DAT_0007bb20, DAT_00075de8, DAT_000755f8, DAT_0007ae68;
extern int     DAT_0007b648, DAT_0007b64c, DAT_0007b8a4, DAT_0007b5e8, DAT_0007b4cc;
extern int     DAT_0007b670, DAT_0007b43c, DAT_0007b428, DAT_0007b8b8, DAT_0007b420;
extern int     DAT_0007b424, DAT_0007b6f8, DAT_0007b6fc, DAT_0007bc80, DAT_0007bc84;
extern int     DAT_0007b7c8, DAT_0007b7cc, DAT_0007b100;
extern int     DAT_0007b384[];       /* 全局互斥锁 (设备统计锁) */
/* API 访问控制全局 (api()/check_connect 共用) */
extern char   *DAT_00078c98;         /* opt_api_allow: "--api-allow" 配置串 (NULL=未配置) */
extern char    DAT_0007b7c4;         /* 非 0 = UNRESTRICTED 读访问模式标志 */
extern int     DAT_00078cb0;         /* ipaccess 有效条目数 */
extern void   *DAT_00078cb4;         /* ipaccess 表基址 (calloc(n,0x24): 16B 模式+16B 掩码+1B 组) */
extern void   *DAT_00078cd4;         /* StrBufs 分配池句柄 (_k_new_list 返回; +8=互斥锁指针) */
/* print_data 依赖的 api.c 锁/池包装 */
extern void    _mutex_lock(const char *fn, int line);       /* FUN_0000b774 */
extern void    _mutex_unlock(const char *fn, int line);     /* FUN_0000b84c */
extern void    api_rw_unlock(const char *fn, int line);     /* FUN_0000b7e0 */
extern int     k_get(void *pool, const char *file, const char *fn, int line);          /* FUN_000391fc */
extern void    _k_add_head(void *pool, int item, const char *file, const char *fn, int line); /* FUN_00039370 */
extern void    add_item_buf(int item, const char *str);     /* FUN_000306b4: 追加到 item+0xC 缓冲 */
/* message() 依赖: codes[] 表与 api_add_* 一行包装 */
extern int     DAT_000760fc[];      /* codes[] 表基址 — 16B/项 {severity, code, params, msg};
                                     * 表尾哨兵项 severity == 4 (SEVERITY_FAIL) */
extern int     DAT_00076100[];      /* codes[0].code (@DAT_000760fc+4) */
extern int     DAT_00076104[];      /* codes[0].params (@DAT_000760fc+8) */
extern char   *DAT_00076108[];      /* codes[0].msg 格式串 (@DAT_000760fc+12) */
extern char   *DAT_00075ed4;        /* opt_api_description (默认 "cgminer 4.9.0") */
extern time_t  DAT_00078cd8;        /* "When" 时间戳全局 (api_add_time 读取) */
extern api_data_t *api_add_string(api_data_t *root, const char *name, char *data, int copy);   /* FUN_00032e1c */
extern api_data_t *api_add_time(api_data_t *root, const char *name, time_t *data, int copy);   /* FUN_00033038 */
extern api_data_t *api_add_int(api_data_t *root, const char *name, int *data, int copy);       /* FUN_00032ed0 */
extern api_data_t *api_add_escape(api_data_t *root, const char *name, const char *data, int copy); /* FUN_00032df8 */
extern int     io_add(void *io_data, const char *str);       /* FUN_000307c8 */
/* 全库统一内联日志守卫位 (json.c 横幅已证: DAT_00078e09 && (DAT_0007ad48 ||
 * DAT_00078e08 || N < DAT_000766c4), N 随 applog 级别取 level-1); 精确语义待定 */
extern char    DAT_00078e09;         /* 守卫总开关 */
extern char    DAT_0007ad48;         /* 守卫条件 A */
extern char    DAT_00078e08;         /* 守卫条件 B */
extern int     DAT_000766c4;         /* 级别阈值比较值 */

// === zero_stats === 原 FUN_0001b8b4
// 来源: reversed/cgminer/30_api_status/FUN_0001b8b4.c | 分类: 30_api_status
// 语义: 清零全局最优难度/份额统计并遍历所有 pool 清 best_diff (调 zero_bestshare)
// 行为: 以当前时刻为统计零点 (total_start / total_runtime), 清空全部全局统计
//       计数、滚动窗口数组、每 pool 统计 (offset +0x08~+0x18c 多字段), 再调
//       zero_bestshare(); 然后逐设备: 复制零点时刻到设备 +0x170, 锁全局互斥锁
//       DAT_0007b384 清设备统计字段, 调 noop_hook 指针与设备驱动回调
//       (device->drv [+4] 槽位 +0x60)。与 cgminer.c zero_stats 一致。
//       锁失败分别走 mutex_lock_fail_cgminer / mutex_unlock_fail_cgminer
//       (携带原码 "zero_stats" 与源行号 0x16f2 / 0x16fd)。
void zero_stats(void)
{
	int dev_idx;
	int i;
	uint32_t *roll;                          /* 原 v7: 滚动窗口数组游标 */
	void **pool_p;                           /* 原 v2: pools[] 游标 */
	void **pool_end;                         /* 原 v5 */
	uint8_t *pool;                           /* 原 v3/v4: pool 指针 (字节寻址) */
	int rc;                                  /* 原 v1: 锁返回值 */
	int dev;                                 /* 原 v4: 设备指针 */

	cgtime((struct timeval *)&DAT_000770d0);             /* total_start = now */
	copy_timeval(&DAT_000770d8, &DAT_000770d0);          /* total_runtime = total_start */

	/* ---- 全局 64 位统计清零 (均以相邻双字为一组) ---- */
	*(uint64_t *)&DAT_0007bc98 = 0;
	*(uint64_t *)&DAT_0007bb10 = 0;
	*(uint64_t *)&DAT_0007b2f8 = 0;
	*(uint64_t *)&DAT_0007bc88 = 0;
	*(uint64_t *)&DAT_0007acc0 = 0;

	/* ---- 滚动窗口数组: DAT_000770e0 ~ DAT_00077140 (0x60 字节, 12 对字) ---- */
	roll = &DAT_000770e0;
	do {
		roll[0] = 0;
		roll[1] = 0;
		roll += 2;
	} while (roll != &DAT_00077140);

	DAT_00077144 = 1;                        /* 窗口满标志复位为 1 */
	DAT_0007b8a8 = 0;
	DAT_0007bb20 = 0;
	*(uint64_t *)&DAT_00075de8 = 0x3ff0000000000000ULL;  /* double 1.0 */
	*(uint64_t *)&DAT_000755f8 = 0x3ff0000000000000ULL;  /* double 1.0 */
	DAT_00077140 = 0;
	DAT_0007ae68 = 0;
	DAT_0007b648 = 0;
	DAT_0007b64c = 0;
	DAT_0007b8a4 = 0;
	DAT_0007b5e8 = 0;
	DAT_0007b4cc = 0;
	DAT_0007b670 = 0;
	DAT_0007b43c = 0;
	DAT_0007b428 = 0;
	DAT_0007b8b8 = 0;
	DAT_0007b420 = 0;
	DAT_0007b424 = 0;
	DAT_0007b6f8 = 0;
	DAT_0007b6fc = 0;
	DAT_0007bc80 = 0;
	DAT_0007bc84 = 0;
	DAT_0007b7c8 = 0;
	DAT_0007b7cc = 0;
	*(uint64_t *)&DAT_0007b100 = 0;

	/* ---- 每 pool 统计清零: 遍历 pools[] 指针数组 ---- */
	if (0 < DAT_0007b198) {                  /* total_pools */
		pool_end = &DAT_0007b438[DAT_0007b198];
		pool_p = &DAT_0007b438[0];
		do {
			pool = (uint8_t *)*pool_p++;
			/* pool->getfail_occurs/remotefail_occurs 等 6 计数 (+0x68~+0x7c) */
			*(uint32_t *)(pool + 0x68) = 0;
			*(uint32_t *)(pool + 0x6c) = 0;
			*(uint32_t *)(pool + 0x70) = 0;
			*(uint32_t *)(pool + 0x74) = 0;
			*(uint32_t *)(pool + 0x78) = 0;
			*(uint32_t *)(pool + 0x7c) = 0;
			*(uint32_t *)(pool + 0x188) = 0;
			*(uint32_t *)(pool + 0x18c) = 0;
			*(uint32_t *)(pool + 0x94) = 0;
			/* pool->diff1/accepted/rejected/stale 等 (+0x08~+0x14) */
			*(uint32_t *)(pool + 8) = 0;
			*(uint32_t *)(pool + 0xc) = 0;
			*(uint32_t *)(pool + 0x10) = 0;
			*(uint32_t *)(pool + 0x14) = 0;
			*(uint32_t *)(pool + 0x98) = 0;
			*(uint32_t *)(pool + 0x9c) = 0;
			*(uint32_t *)(pool + 0xa0) = 0;
			*(uint32_t *)(pool + 0xa4) = 0;
			*(uint32_t *)(pool + 0x180) = 0;
			*(uint32_t *)(pool + 0x28) = 0;
			*(uint32_t *)(pool + 0x2c) = 0;
		} while (pool_p != pool_end);
	}

	zero_bestshare();                        /* 原 FUN_000160b4 */

	/* ---- 逐设备清零 ---- */
	if (DAT_0007b720 < 1) {                  /* total_devices */
		return;
	}
	i = 0;
	do {
		dev_idx = i + 1;
		dev = (int)get_devices(i);
		copy_timeval((void *)(dev + 0x170), &DAT_000770d0); /* dev->零点时刻 */

		rc = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007b384);
		if (rc != 0) {
			mutex_lock_fail_cgminer("zero_stats", 0x16f2);
		}
		*(uint32_t *)(dev + 0x50) = 0;
		*(uint32_t *)(dev + 0x54) = 0;
		*(uint32_t *)(dev + 0x24) = 0;
		*(uint64_t *)(dev + 0xc0) = 0;
		*(uint32_t *)(dev + 0x58) = 0;
		*(uint32_t *)(dev + 0x5c) = 0;
		*(uint32_t *)(dev + 0x28) = 0;
		*(uint32_t *)(dev + 200) = 0;        /* +0xc8 */
		*(uint32_t *)(dev + 0xcc) = 0;
		*(uint32_t *)(dev + 0x2c) = 0;
		*(uint32_t *)(dev + 0xd0) = 0;
		*(uint32_t *)(dev + 0xd4) = 0;
		*(uint32_t *)(dev + 0xdc) = 0;
		*(uint32_t *)(dev + 0xe0) = 0;
		*(uint32_t *)(dev + 0xe4) = 0;
		rc = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b384);
		if (rc != 0) {
			mutex_unlock_fail_cgminer("zero_stats", 0x16fd);
		}

		(*DAT_00075ed8)();                   /* 空操作钩子 (按机器码保留) */
		/* 设备驱动统计复位回调: *(dev->drv [+4] -> 槽 +0x60) */
		((void (*)(int))*(uint32_t *)(*(uint32_t *)(dev + 4) + 0x60))(dev);
		i = dev_idx;
	} while (dev_idx < DAT_0007b720);
	return;
}

// === escape_string === 原 FUN_00030500
// 来源: reversed/cgminer/30_api_status/FUN_00030500.c | 分类: 30_api_status
// 语义: 复制字符串并对特殊字符加 '\' 转义: '\\' 恒转义; '"' 仅 JSON 模式
//       (isjson) 转义; '|'/','/'=' 仅文本模式转义。无可转义字符时直接返回
//       原指针 (不拷贝)。与 api.c escape_string 一致。
// 注: 反编译输出第一遍 '"' 分支的跳转条件与反汇编相反 (0x305a0 为 beq, 仅
//     !isjson 走跳过路径), 缓冲长度 = len+1+count 与第二遍写 2 字节的算术
//     一致性均以反汇编为准: '"' 仅 isjson 计数并转义。
char *escape_string(char *string, int isjson)
{
	size_t total;                            /* 原 sVar1: malloc 尺寸 */
	char *buf;                               /* 原 pbVar2: 返回值 (默认原指针) */
	char *src;                               /* 原 pbVar3 */
	char *dst;                               /* 原 pbVar6 */
	char *dst_next;                          /* 原 pbVar5 */
	char c;                                  /* 原 bVar7/bVar4: 当前字符 */
	int esc_count;                           /* 原 v8: 需转义字符数 */
	char errbuf[4100];                       /* 原 栈@0x1020 */

	c = *string;
	buf = string;
	if (c != '\0') {
		esc_count = 0;
		src = string;
		/* ---- 第一遍: 统计需转义字符数 ---- */
		do {
			while (c != '=') {               /* 0x3d */
				if ('=' < c) {
					if (c != '\\') {
						if (c != '|') goto next_char;   /* 原 LAB_00030544: 跳过 */
						break;                          /* '|' 汇合 */
					}
					goto count_it;                      /* 原 LAB_00030574: '\\' 恒计数 */
				}
				if (c == '"') {          /* 0x22: 仅 isjson 计数 */
					if (isjson == 0) goto next_char;
					goto count_it;
				}
				else if (c == ',') {     /* 0x2c: 汇合 */
					break;
				}
next_char:
				src++;
				c = *src;
				if (c == '\0') goto done_scan;      /* 原 LAB_00030584 */
			}
			/* '=', ',', '|' 汇合: 仅 !isjson 计数 */
			if (isjson != 0) goto next_char;
count_it:
			src++;
			c = *src;
			esc_count++;
		} while (c != '\0');
done_scan:
		if (esc_count != 0) {
			total = strlen(string);
			total = esc_count + 1 + total;
			buf = malloc(total);
			dst = buf;
			src = string;
			if (buf == NULL) {
				snprintf(errbuf, 0x1000, "Failed to malloc escape buf %d in %s %s():%d",
					 (int)total, "api.c", "escape_string", 0x348);
				applog(3, errbuf, 1);
				quit(1);
				/* quit() 不返回; 以下拷贝路径按原码保留 (不可达) */
				c = *string;
			}
			/* ---- 第二遍: 拷贝并按模式转义 ---- */
			while (c != '\0') {
				if (c == '=') {          /* 0x3d */
esc_check:                       /* 原 LAB_00030618 */
					if (isjson == 0) {   /* 文本模式: 写 '\' 再写字符 */
esc_write:                       /* 原 LAB_00030620 */
						*dst = '\\';     /* 0x5c */
						c = *src;
						dst++;
					}
copy_char:                       /* 原 LAB_000305e8 */
					dst_next = dst + 1;
					*dst = c;
				}
				else {
					if (c < '=') {
						if (c == '"') {  /* 0x22: 仅 isjson 写 '\' 再写字符 */
							if (isjson != 0) goto esc_write;   /* 原 LAB_00030620 */
						}
						else if (c == ',') {
							goto esc_check;
						}
						goto copy_char;
					}
					if (c != '\\') {
						if (c != '|') goto copy_char;
						goto esc_check;  /* '|': 仅文本模式转义 */
					}
					/* '\\': 写 '\' + 字符本身, 共 2 字节 */
					*dst = '\\';
					dst[1] = *src;
					dst_next = dst + 2;
				}
				string++;
				dst = dst_next;
				src++;
				c = *string;
			}
			*dst = '\0';
		}
	}
	return buf;
}

// === check_connect === 原 FUN_000309e8
// 来源: reversed/cgminer/30_api_status/FUN_000309e8.c | 分类: 30_api_status
// 语义: 判定 API 连接是否允许 — getnameinfo 取对端数字 IP, IPv4 映射为
//       "::ffff:%s" 后 inet_pton(AF_INET6) 展开; 默认组置 'R' (toupper 表
//       常量折叠)。opt_api_allow (DAT_00078c98) 未配置时: 127.0.0.1 或
//       UNRESTRICTED 标志 (DAT_0007b7c4) 直通, 其余仅 ::1 环回放行; 已配置
//       时逐条匹配 ipaccess 表 (16B 模式 & 16B 掩码 == 对端地址) 并取组字符。
// 返回: 1=允许 (组字符写入 *group; *ipaddr = malloc 的 46 字节主机串)
int check_connect(const void *saddr, char **ipaddr, char *group)
{
	uint8_t *p_entry;                        /* 原 pbVar4: 表游标 (基址-1) */
	uint8_t *entry;                          /* 条目首址 */
	char *host;                              /* 原 __host */
	const int32_t *ctype_up;                 /* 原 pp_Var2 */
	int i;                                   /* 原 v6: strcmp 结果 / 条目计数 */
	int k;                                   /* 16 字节比较游标 */
	int matched;                             /* 原 bVar7 */
	uint32_t addr[4];                        /* 原 栈@0x50/4c/48/44: 16 字节 IPv6 地址 */

	host = malloc(0x2e);                     /* 46 字节: "xxx.xxx.xxx.xxx" 上限 */
	*ipaddr = host;
	getnameinfo((const struct sockaddr *)saddr, 0x80, host, 0x2e, NULL, 0, 1);   /* 1 = NI_NUMERICHOST */
	if (((const struct sockaddr *)saddr)->sa_family == 2) {   /* AF_INET: 拼 IPv4-mapped 形式 */
		char mapped[36];                     /* 原 栈@0x40 */
		sprintf(mapped, "::ffff:%s", host);
		inet_pton(10, mapped, addr);         /* 10 = AF_INET6 */
	}
	else {
		inet_pton(10, host, addr);
	}
	ctype_up = *__ctype_toupper_loc();
	*group = (char)ctype_up[0x52];           /* 编译器常量折叠 → 'R' (默认只读组) */

	if (DAT_00078c98 == NULL) {              /* 未配置 --api-allow */
		if (DAT_0007b7c4 == '\0' && strcmp(host, "127.0.0.1") != 0) {
			/* 非本机/非全开放: 仅 ::1 环回放行 (addr 前 12 字节全 0 且尾字节 1) */
			matched = (addr[0] == 0 && addr[1] == 0 &&
				   addr[2] == 0 && addr[3] == 0x01000000);
		}
		else {
			matched = 1;                     /* 127.0.0.1 或 UNRESTRICTED 模式直通 */
		}
	}
	else {
		matched = false;
		if (0 < DAT_00078cb0) {
			i = 0;
			p_entry = (uint8_t *)DAT_00078cb4 - 1;
			do {
				entry = p_entry + 1;
				/* 逐字节: pattern[k] == mask[k] & addr[k] (掩码 16B @条目+0x10) */
				for (k = 0; k < 16; k++) {
					if (entry[k] != ((uint8_t *)addr)[k] & entry[0x10 + k])
						break;
				}
				if (k == 16) {
					*group = entry[0x20];    /* 组字符 @条目+0x20 */
					return 1;
				}
				i++;
				p_entry += 0x24;             /* 条目步长 36 字节 */
			} while (i != DAT_00078cb0);
		}
	}
	return matched;
}

// === send_result === 原 FUN_00030b88
// 来源: reversed/cgminer/30_api_status/FUN_00030b88.c | 分类: 30_api_status
// 语义: 发送 API 应答 — io_data->close (+0x0d) 置位则补 ']'; isjson 则追加
//       ",\"id\":1}"; 记 "API: send reply" 日志后循环 select(50ms 可写) +
//       send(), 部分发送推进游标重试; EAGAIN (0xb) 重试上限 4 次, 其余错误
//       记 "API: send (%d:%d) failed" 后返回。与 api.c send_result 一致。
void send_result(void *io_data, int sock, int isjson)
{
	size_t total_len;                        /* 原 param3_01 */
	size_t nsent;                            /* 原 param3_00: send 返回值 */
	size_t len;                              /* 原 sVar1: 尚未发送字节数 */
	int retries;                             /* 原 v5: EAGAIN 重试计数, 上限 4 */
	int sends;                               /* 原 param4: send() 调用次数 */
	int rc;                                  /* 原 param3: select 返回值 */
	int fd_idx;                              /* 原 v6: fds_bits 索引 (负值除法规整) */
	char *ptr;                               /* 原 v4: 发送缓冲游标 */
	char *dots;
	struct timeval tv;                       /* 原 tStack_10b0 */
	fd_set writefds;                         /* 原 栈@0x10a8 */
	char logbuf[4100];                       /* 原 栈@0x1028 */

	ptr = *(char **)((uint8_t *)io_data + 4);            /* io_data->ptr */
	if (*(char *)((uint8_t *)io_data + 0xd) != '\0') {   /* io_data->close: 补 ']' */
		len = strlen(ptr);
		ptr[len] = ']';
		ptr[len + 1] = '\0';
	}
	if (isjson != 0) {
		len = strlen(ptr);
		memcpy(ptr + len, ",\"id\":1}", 9);  /* 原 builtin_strncpy(…,9): 8 字符 + NUL */
	}
	total_len = strlen(ptr) + 1;
	if (DAT_00078e09 != '\0' &&
	    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 6 < DAT_000766c4)) {
		dots = "...";
		if ((int)total_len < 0xb)            /* 尾部不足 11 字节即完整可见 */
			dots = "";
		snprintf(logbuf, 0x1000, "API: send reply: (%d) '%.10s%s'",
			 (int)total_len, ptr, dots);
		applog(7, logbuf, 0);
	}
	retries = 0;
	sends = 0;
	len = total_len;
	do {
		while (true) {
			if (4 < retries || (int)len < 1)
				return;
			/* 50ms 可写等待 (原码内联展开 FD_ZERO/FD_SET) */
			fd_idx = (sock < 0) ? sock + 0x1f : sock;
			tv.tv_sec = 0;
			tv.tv_usec = 50000;              /* 0xC350 = 50ms */
			FD_ZERO(&writefds);
			writefds.fds_bits[fd_idx >> 5] |= 1 << (sock % 0x20);
			rc = select(sock + 1, NULL, &writefds, NULL, &tv);
			if (rc < 1) {
				if (DAT_0007ad48 == '\0' && DAT_00078e08 == '\0' && DAT_000766c4 < 4)
					return;
				snprintf(logbuf, 0x1000, "API: send select failed (%d)", rc);
				applog(4, logbuf, 0);
				return;
			}
			nsent = send(sock, ptr, len, 0);
			sends++;
			if ((int)nsent < 0)
				break;                       /* → 下方 errno 处理 */
			if (sends == 1) {
				if (nsent == len) {
					if (DAT_00078e09 != '\0' &&
					    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' ||
					     6 < DAT_000766c4)) {
						snprintf(logbuf, 0x1000, "API: sent all of %d first go",
							 (int)nsent);
						applog(7, logbuf, 0);
					}
					len -= nsent;            /* 原 LAB_00030ed8 */
					ptr += nsent;
					continue;
				}
				if (DAT_00078e09 != '\0' &&
				    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' ||
				     6 < DAT_000766c4)) {
					snprintf(logbuf, 0x1000, "API: sent %d of %d first go",
						 (int)nsent, (int)len);
					applog(7, logbuf, 0);    /* 原 LAB_00030d78 */
				}
			}
			else if (nsent != len) {
				if (DAT_00078e09 != '\0' &&
				    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' ||
				     6 < DAT_000766c4)) {
					snprintf(logbuf, 0x1000, "API: sent %d of remaining %d (sendc=%d)",
						 (int)nsent, (int)len, sends);
					applog(7, logbuf, 0);    /* 原 LAB_00030d78 */
				}
			}
			else {
				if (DAT_00078e09 != '\0' &&
				    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' ||
				     6 < DAT_000766c4)) {
					snprintf(logbuf, 0x1000, "API: sent all of remaining %d (sendc=%d)",
						 (int)nsent, sends);
					applog(7, logbuf, 0);    /* 原 LAB_00030ec4 */
				}
				len -= nsent;                /* 原 LAB_00030ed8 */
				ptr += nsent;
				continue;
			}
			/* 原 LAB_00030d8c: 部分发送, 推进游标后继续 */
			len -= nsent;
			ptr += nsent;
			if (nsent == 0)
				goto retry_inc;              /* 原 LAB_00030d9c: 直接计重试, 跳过 errno 检查 */
		}
		/* send() < 0: 仅 EAGAIN (0xb) 可重试 */
		if (errno != EAGAIN) {
			if (DAT_0007ad48 == '\0' && DAT_00078e08 == '\0' && DAT_000766c4 < 4)
				return;
			snprintf(logbuf, 0x1000, "API: send (%d:%d) failed: %s",
				 (int)total_len, (int)(total_len - len), strerror(errno));
			applog(4, logbuf, 0);
			return;
		}
retry_inc:       /* 原 LAB_00030d9c */
		retries++;
	} while (true);
}

// === mcast === 原 FUN_0003107c
// 来源: reversed/cgminer/30_api_status/FUN_0003107c.c | 分类: 30_api_status
// 语义: API 组播监听线程 — getaddrinfo(DAT_00075ec8=opt_api_mcast_addr,
//       端口 DAT_00075ebc) 建组播 UDP socket, SO_REUSEADDR, bind 重试 61s,
//       加入组播组 (IPv4: IPPROTO_IP/IP_ADD_MEMBERSHIP(0x23), IPv6:
//       IPPROTO_IPV6(0x29)/IPV6_JOIN_GROUP(0x14)); 循环收 "cgminer-<code>-<端口>"
//       请求 (check_connect 过滤来源), 校验端口 [1,65535] 后对请求者回发
//       "cgm-FTW-%d-%s" (DAT_00075ed0=opt_api_port, DAT_00075ec0=opt_api_mcast_des)。
//       与 api.c mcast() 一致; 上游遗留缺陷按机器码保留 (reply socket 家族恒取
//       res 头节点、break 判 mcast_sock 而非 reply_sock)。
// 日志门控: LOG_ERR(3) 级 = DAT_0007ad48 || DAT_00078e08 || 2 < DAT_000766c4;
//           LOG_DEBUG(7) 级 = 另需 DAT_00078e09 (见 send_result 守卫注释)。
void mcast(void)
{
	int mcast_sock;                          /* 原 v1: 组播监听 socket */
	int reply_sock;                          /* 原 栈@0x1908: 对客户端应答 socket */
	long reply_port;                         /* 原 param4: strtol 结果 */
	ssize_t n;                               /* 原 param3/param4_00: recvfrom/sendto 字节数 */
	int rc;                                  /* 原 v2 (前半): getaddrinfo/setsockopt/bind 返回值 */
	int count;                               /* 原 v2 (后半): recvfrom 尝试计数 */
	int addrok;                              /* 原 v6 (循环段): check_connect 结果 */
	size_t len;                              /* 原 sVar4 */
	size_t expect_code_len;                  /* 原 __n */
	char *expect_code;                       /* 原 v5 */
	char *code;                              /* 原 v10: opt_api_mcast_code / 端口串游标 */
	char *errstr;                            /* 原 v10 (错误消息复用) */
	time_t bind_start;                       /* 原 tVar3 */
	time_t now;                              /* 原 tVar7 */
	int optval;                              /* 原 uStack_18f0 */
	struct addrinfo hints;                   /* 原 栈@0x18c8 */
	struct addrinfo *res;                    /* 原 paStack_18f4 */
	struct addrinfo *ai;                     /* 原 paVar11 */
	struct addrinfo *client;                 /* ai_next 遍历游标 (上游 client) */
	struct sockaddr_storage came_from;       /* 原 asStack_18a8 */
	socklen_t came_from_siz;                 /* 原 sStack_18fc */
	char *connectaddr;                       /* 原 pcStack_18f8: check_connect 输出 (malloc 46B) */
	char connect_group;                      /* 原 uStack_18fd */
	char expect[] = "cgminer-";              /* 原 栈@0x18ec+栈@0x18e8+栈@0x18e4 */
	char port_s[12];                         /* 原 栈@0x18e0 */
	char came_from_port[12];                 /* 原 栈@0x18d4 */
	char buf[1025];                          /* 原 栈@0x1829 起 (含 1 字节头) */
	char *bufp;                              /* 原 param4_01 = buf+1: 收包数据区 */
	char logbuf[4100];                       /* 原 栈@0x1028 */
	char replybuf[1024];                     /* 原 栈@0x1904 = &栈@0x1428 起 0x400 字节 */
	struct ip_mreq grp4;                     /* 原 栈@0x1428/1424 (AF_INET 分支) */
	struct ipv6_mreq grp6;                   /* 原 栈@0x1428~栈@0x1418 (AF_INET6 分支) */

	bufp = buf + 1;
	code = DAT_00075ec4;                     /* opt_api_mcast_code */
	sprintf(port_s, "%d", DAT_00075ebc);     /* opt_api_mcast_port */

	memset(&hints, 0, sizeof(hints));        /* ai_family = AF_UNSPEC (0) */
	rc = getaddrinfo(DAT_00075ec8, port_s, &hints, &res);   /* opt_api_mcast_addr */
	if (rc != 0) {
		applog(3, "Invalid API MulticastAddress", 1);
		quit(1);
	}
	ai = res;
	if (res == NULL) {                       /* 原 LAB_000314fc */
		freeaddrinfo(res);
		ai = NULL;
		mcast_sock = -1;
		applog(3, "API mcast could not open socket", 1);
		quit(1);
	}
	else {
		do {
			mcast_sock = socket(res->ai_family, 2, 0);   /* 2 = SOCK_DGRAM (家族恒取头节点, 上游同) */
			if (0 < mcast_sock)
				break;                            /* 原 LAB_00031148 */
			ai = ai->ai_next;
		} while (ai != NULL);
		if (mcast_sock == -1) {                  /* INVSOCK */
			freeaddrinfo(res);                   /* 原 LAB_000314fc */
			ai = NULL;
			mcast_sock = -1;
			applog(3, "API mcast could not open socket", 1);
			quit(1);
		}
	}
	optval = 1;
	rc = setsockopt(mcast_sock, 1, 2, &optval, 4);       /* SOL_SOCKET, SO_REUSEADDR */
	if (rc < 0) {
		if (DAT_0007ad48 == '\0' && DAT_00078e08 == '\0' && DAT_000766c4 < 3)
			goto die;                             /* 原 LAB_000315a8 */
		errstr = strerror(errno);
		snprintf(logbuf, 0x1000, "API mcast setsockopt SO_REUSEADDR failed (%s)%s",
			 errstr, " - API multicast listener will not be available");
		applog(3, logbuf, 0);
		goto die;
	}
	/* ---- bind 重试: 总时限 61 秒 (旧监听方未完全释放场景) ---- */
	bind_start = time(NULL);
	while (rc = bind(mcast_sock, ai->ai_addr, ai->ai_addrlen), rc < 0) {
		errstr = strerror(errno);
		now = time(NULL);
		if (0x3d < now - bind_start) {
			if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 2 < DAT_000766c4) {
				snprintf(logbuf, 0x1000, "API mcast bind to port %d failed (%s)%s",
					 DAT_00075ebc, errstr,
					 " - API multicast listener will not be available");
				applog(3, logbuf, 0);
			}
			goto die;                             /* 原 LAB_000315a8 */
		}
		cgsleep_ms(30000);
	}
	if (ai->ai_family == 2) {                /* AF_INET: IP_ADD_MEMBERSHIP */
		grp4.imr_multiaddr.s_addr = *(uint32_t *)((uint8_t *)ai->ai_addr + 4);  /* sin_addr @ sa_data+2 */
		grp4.imr_interface.s_addr = 0;       /* INADDR_ANY */
		rc = setsockopt(mcast_sock, 0, 0x23, &grp4, 8);   /* IPPROTO_IP, IP_ADD_MEMBERSHIP */
	}
	else {
		if (ai->ai_family != 10)             /* AF_INET6 之外: 跳过加组, 直接进入监听 */
			goto after_join;                 /* 原 LAB_000311c0 */
		/* sin6_addr @ sa_data+6 (字节 8..23, 原 4 个字装载) */
		grp6.ipv6mr_multiaddr = *(struct in6_addr *)((uint8_t *)ai->ai_addr + 8);
		grp6.ipv6mr_interface = 0;
		rc = setsockopt(mcast_sock, 0x29, 0x14, &grp6, 0x14);  /* IPPROTO_IPV6, IPV6_JOIN_GROUP */
	}
	if (-1 < rc) {
after_join:                                  /* 原 LAB_000311c0 */
		freeaddrinfo(res);
		code = DAT_00075ec4;
		len = strlen(DAT_00075ec4);
		expect_code_len = len + 9;
		expect_code = malloc(len + 10);
		if (expect_code == NULL) {
			applog(3, "Failed to malloc mcast expect_code", 1);
			quit(1);
			/* quit() 不返回; 其后按原码保留 (不可达) */
			code = DAT_00075ec4;
		}
		snprintf(expect_code, len + 10, "%s%s-", expect, code);
		bufp = buf + 1;
		count = 0;
		reply_sock = -1;
		/* ---- 主循环: 收组播请求并应答 (原 LAB_00031238; 正常不退出) ---- */
		while (true) {
			do {
				while (true) {
					count++;
					cgsleep_ms(1000);
					came_from_siz = 0x80;     /* sizeof(sockaddr_storage) */
					n = recvfrom(mcast_sock, bufp, 0x3ff, 0,
						     (struct sockaddr *)&came_from, &came_from_siz);
					if (-1 < (int)n)
						break;
					if (DAT_00078e09 != '\0' &&
					    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' ||
					     6 < DAT_000766c4)) {
						errstr = strerror(errno);
						snprintf(logbuf, 0x1000, "API mcast failed count=%d (%s) (%d)",
							 count, errstr, mcast_sock);
						applog(7, logbuf, 0);
					}
				}
				addrok = check_connect((struct sockaddr *)&came_from,
						       &connectaddr, &connect_group);
				if (DAT_00078e09 != '\0' &&
				    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' ||
				     6 < DAT_000766c4)) {
					snprintf(logbuf, 0x1000, "API mcast from %s - %s",
						 connectaddr, addrok != 0 ? "Accepted" : "Ignored");
					applog(7, logbuf, 0);
				}
			} while (addrok == 0);        /* 来源被拒 → 继续收包 */

			bufp[n] = '\0';
			if ((n != 0) && (bufp[n - 1] == '\n')) {   /* 原 栈@0x1829[n] */
				n--;
				bufp[n] = '\0';
			}
			getnameinfo((struct sockaddr *)&came_from, came_from_siz,
				    NULL, 0, came_from_port, 10, 1);   /* 1 = NI_NUMERICHOST */
			if (DAT_00078e09 != '\0' &&
			    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 6 < DAT_000766c4)) {
				snprintf(logbuf, 0x1000, "API mcast request rep=%d (%s) from [%s]:%s",
					 (int)n, bufp, connectaddr, came_from_port);
				applog(7, logbuf, 0);
			}
			/* 期望码匹配 + 端口校验层: 不合规则回到收包 (原 C/B 层) */
			while (true) {
				if ((size_t)n > expect_code_len &&
				    memcmp(bufp, expect_code, expect_code_len) == 0) {
					code = bufp + expect_code_len;
					reply_port = strtol(code, NULL, 10);
					if (reply_port - 1U < 0xffff)      /* 端口 ∈ [1,65535] */
						break;                         /* → 应答路径 */
					if (DAT_00078e09 != '\0' &&
					    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' ||
					     6 < DAT_000766c4)) {
						snprintf(logbuf, 0x1000,
							 "API mcast request ignored - invalid port (%s)", code);
						applog(7, logbuf, 0);
					}
				}
				else {
					if (DAT_00078e09 != '\0' &&
					    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' ||
					     6 < DAT_000766c4)) {
						applog(7, "API mcast request was no good", 0);
					}
				}
				goto recv_again;          /* 原 C/B 层回跳: 重收包 (count++/sleep) */
			}
			break;                        /* 原 B 层 break: 有效端口 */
recv_again: ;
		}
		if (DAT_00078e09 != '\0' &&
		    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 6 < DAT_000766c4)) {
			snprintf(logbuf, 0x1000, "API mcast request OK port %s=%d", code, reply_port);
			applog(7, logbuf, 0);
		}
		rc = getaddrinfo(connectaddr, code, &hints, &res);
		if (rc != 0) {
			if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 2 < DAT_000766c4) {
				snprintf(logbuf, 0x1000, "Invalid client address %s", connectaddr);
				applog(3, logbuf, 0);
			}
			goto recv_again;              /* 原 code_r0x00031768 → LAB_00031238 */
		}
		client = res;
		while (client != NULL) {
			reply_sock = socket(res->ai_family, 2, 0);   /* 上游缺陷: 家族恒取 res 头节点 */
			if (mcast_sock > 0)                          /* 上游缺陷: 判 mcast_sock 而非 reply_sock */
				break;
			client = client->ai_next;
		}
		if (reply_sock == -1) {
			freeaddrinfo(res);            /* 原 LAB_00031954 */
			if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 2 < DAT_000766c4) {
				snprintf(logbuf, 0x1000, "API mcast could not open socket to client %s",
					 connectaddr);
				applog(3, logbuf, 0);
			}
			goto recv_again;              /* 原 do-while 尾 (双路径) 均回 LAB_00031238 */
		}
		snprintf(replybuf, 0x400, "cgm-FTW-%d-%s", DAT_00075ed0, DAT_00075ec0);
		len = strlen(replybuf);
		n = sendto(reply_sock, replybuf, len + 1, 0, client->ai_addr, client->ai_addrlen);
		freeaddrinfo(res);
		if (n < 0) {
			if (DAT_00078e09 != '\0' &&
			    (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 6 < DAT_000766c4)) {
				errstr = strerror(errno);
				snprintf(logbuf, 0x1000, "API mcast send reply failed (%s) (%d)",
					 errstr, reply_sock);
				applog(7, logbuf, 0);
			}
		}
		else if (DAT_00078e09 != '\0' &&
			 (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 6 < DAT_000766c4)) {
			snprintf(logbuf, 0x1000, "API mcast send reply (%s) succeeded (%d) (%d)",
				 replybuf, (int)n, reply_sock);
			applog(7, logbuf, 0);
		}
		close(reply_sock);
		/* 原 LAB_00031238 回跳: 继续收包 */
		goto recv_again;
	}
	else {
		if (DAT_0007ad48 == '\0' && DAT_00078e08 == '\0' && DAT_000766c4 < 3)
			goto die;                             /* 原 LAB_000315a8 */
		errstr = strerror(errno);
		snprintf(logbuf, 0x1000, "API mcast join failed (%s)%s",
			 errstr, " - API multicast listener will not be available");
		applog(3, logbuf, 0);
		goto die;
	}
die:                                             /* 原 LAB_000315a8 */
	close(mcast_sock);
	return;
}

// === api_add_data_full === 原 FUN_00031cdc
// 来源: reversed/cgminer/30_api_status/FUN_00031cdc.c | 分类: 30_api_status
// 语义: 构造 api_data 节点 (0x18 字节) 并环链接入 root 尾 (root->prev 恒指尾,
//       尾->next 回指头)。name 经 strdup; data==NULL 兜底 "(null)" (NULLSTR)
//       且 type 置 2 (API_CONST); copy!=0 时按 type 枚举 (0..26) 拷贝数据:
//       0/1/2=字符串 strdup, 3/0xe=1 字节, 4/5=2 字节, 6-9/0x10/0x13/0x16/0x1a=
//       4 字节, 其余列出的=8 字节; 未知 type 记 "API: unknown1 data type %d
//       ignored" 并置 type=1、data="Unknown"。与 api.c api_add_data_full 一致。
api_data_t *api_add_data_full(api_data_t *root, const char *name, int type,
			      void *data, int copy)
{
	api_data_t *new;                         /* 原 v2 */
	api_data_t *head;                        /* 原 v1: 返回根 (root==NULL 时为新节点) */
	char *str;                               /* 原 v5 */
	uint16_t *v16;                           /* 原 v4 */
	uint32_t *v32;                           /* 原 v7 */
	uint32_t tmp;                            /* 原 v8: 8 字节拷贝防别名暂存 */
	size_t len;                              /* 原 sVar6 */
	char logbuf[4100];                       /* 原 栈@0x1020 */

	new = malloc(0x18);
	new->name = strdup(name);                /* 原 __strdup */
	new->type = type;
	head = root;
	if (root == NULL) {
		new->prev = new;                     /* 空链: 自环 */
		new->next = new;
		head = new;
	}
	if (root != NULL) {
		new->prev = head->prev;              /* 接到原尾之后 */
		head->prev = new;
		new->next = head;
		new->prev->next = new;               /* 原 *(new->prev + 0x14) = new */
	}
	new->copy = copy;
	if (data == NULL) {
		new->copy = 0;
		data = "(null)";                     /* NULLSTR */
		new->type = 2;                       /* API_CONST */
	}
	else if (copy != 0) {
		switch (type) {
		case 0:                              /* API_ESCAPE */
		case 1:                              /* API_STRING */
		case 2:                              /* API_CONST */
			len = strlen(data);
			str = malloc(len + 1);
			new->data = str;
			strcpy(str, data);
			return head;
		case 3:                              /* API_UINT8: malloc(4) 仅写 1 字节 */
			str = malloc(4);
			new->data = str;
			*(uint8_t *)str = *(uint8_t *)data;
			return head;
		case 4:                              /* API_UINT16 */
		case 5:                              /* API_SHORT */
			v16 = malloc(4);
			new->data = (char *)v16;
			*v16 = *(uint16_t *)data;
			return head;
		case 6:                              /* API_INT */
		case 7:                              /* API_UINT */
		case 8:                              /* API_UINT32 */
		case 9:                              /* API_INT32 */
		case 0x10:                           /* API_TIME (32 位 time_t) */
			v32 = malloc(4);
			new->data = (char *)v32;
			*v32 = *(uint32_t *)data;
			return head;
		case 10:                             /* API_UINT64 */
		case 11:                             /* API_INT64 */
			v32 = malloc(8);
			new->data = (char *)v32;
			tmp = *(uint32_t *)((char *)data + 4);
			*(uint32_t *)v32 = *(uint32_t *)data;
			v32[1] = tmp;
			return head;
		case 0xc:                            /* API_DOUBLE */
		case 0xd:                            /* API_ELAPSED */
		case 0x11:                           /* API_MHS */
		case 0x12:                           /* API_MHTOTAL */
		case 0x14:
		case 0x15:
		case 0x17:
		case 0x18:
		case 0x19:
			v32 = malloc(8);
			new->data = (char *)v32;
			tmp = *(uint32_t *)((char *)data + 4);
			*(uint32_t *)v32 = *(uint32_t *)data;
			v32[1] = tmp;
			return head;
		case 0xe:                            /* API_UTILITY: bool, malloc(1) 1 字节 */
			str = malloc(1);
			new->data = str;
			*(uint8_t *)str = *(uint8_t *)data;
			return head;
		case 0xf:                            /* API_TIMEVAL */
			v32 = malloc(8);
			new->data = (char *)v32;
			tmp = *(uint32_t *)((char *)data + 4);
			*(uint32_t *)v32 = *(uint32_t *)data;
			v32[1] = tmp;
			return head;
		case 0x13:                           /* API_TEMP */
		case 0x16:
		case 0x1a:
			v32 = malloc(4);
			new->data = (char *)v32;
			*v32 = *(uint32_t *)data;
			return head;
		}
		if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 2 < DAT_000766c4) {
			snprintf(logbuf, 0x1000, "API: unknown1 data type %d ignored", type);
			applog(3, logbuf, 0);
		}
		new->type = 1;
		new->copy = 0;
		new->data = "Unknown";
		return head;
	}
	new->data = data;
	return head;
}

// === print_data === 原 FUN_00032534
// 来源: reversed/cgminer/30_api_status/FUN_00032534.c | 分类: 30_api_status
// 语义: 遍历 api_data 环链表输出条目到 StrBufs 池暂存缓冲 (item+0xC 指向):
//       isjson → {"name":value,…}(字符串/转义值加引号), 文本 → name=value|
//       …, 分隔 ","; 结束符 isjson?"}":"|" 后经 io_add 追加到 io_data 并把
//       item 归还池。值按 type 格式化: 3=%u(1B) 4=%d(short) 5=%u(ushort)
//       6=%d 7/8=%u 9=0x%08x 10=%llu 11=%lld 12=%f 13=%.0f 14=bool
//       15=%ld.%06ld(TIMEVAL) 16=%lu 17/20/21=%.2f 18=%.4f 19=%.2f(float)
//       22/26=%.3f(float) 23=%.15f 24=%.8f 25=%.4f(×100)。未知 type 记
//       "API: unknown2 data type %d ignored" 并输出 Unknown。
// 锁序: 池互斥锁 *(DAT_00078cd4+8) + 其后紧跟的读写锁; 失败分别走
//       _mutex_lock/_mutex_unlock/api_rw_unlock; WRLOCK 错误 snprintf+quit(1)。
int print_data(void *io_data, api_data_t *root, int isjson, int precom)
{
	char valbuf[64];                         /* 原 栈@0x1070 */
	char logbuf[4100];                       /* 原 栈@0x1030 */
	int item;                                /* 原 v1: StrBufs 池 item */
	int rc;                                  /* 原 v1/v2: 锁返回值 */
	char *str;                               /* 原 v4 */
	pthread_mutex_t *mutex;                  /* 原 ppVar5 */
	api_data_t *node;                        /* 原 v3 */
	api_data_t *head;                        /* 原 param_2: 环游标 */
	api_data_t *prev;                        /* 原 rc/v2: 摘除节点时的 prev */

	mutex = *(pthread_mutex_t **)(DAT_00078cd4 + 8);
	rc = pthread_mutex_lock(mutex);
	if (rc != 0) {
		_mutex_lock("print_data", 0x4ac);
	}
	rc = pthread_rwlock_wrlock((pthread_rwlock_t *)(mutex + 1));
	if (rc != 0) {
		snprintf(logbuf, 0x1000, "WTF WRLOCK ERROR ON LOCK! errno=%d in %s %s():%d",
			 errno, "api.c", "print_data", 0x4ac);
		applog(3, logbuf, 1);
		quit(1);
	}
	item = k_get(DAT_00078cd4, "api.c", "print_data", 0x4ad);
	mutex = *(pthread_mutex_t **)(DAT_00078cd4 + 8);
	rc = pthread_rwlock_unlock((pthread_rwlock_t *)(mutex + 1));
	if (rc != 0) {
		api_rw_unlock("print_data", 0x4ae);
	}
	rc = pthread_mutex_unlock(mutex);
	if (rc != 0) {
		_mutex_unlock("print_data", 0x4ae);
	}
	(*DAT_00075ed8)();                       /* 空操作钩子 (按机器码保留) */
	/* item+0xC → 暂存缓冲; 缓冲+4 (已写长度) 清零 */
	*(int *)(*(int *)(item + 0xc) + 4) = 0;
	if (precom != 0) {
		add_item_buf(item, ",");             /* DAT_00060764 */
	}
	if (isjson != 0) {
		add_item_buf(item, "{");             /* DAT_00066aa8 */
	}
	head = root;
	if (root == NULL) {
		goto tail;                           /* 原 LAB_00032760 */
	}
	/* ---- 条目循环: 头 (文本 name= / JSON "name":) + 值 ---- */
	while (true) {
		if (isjson == 0) {                   /* 原 LAB_0003260c */
			add_item_buf(item, head->name);
			add_item_buf(item, "=");         /* DAT_00066fbc */
		}
		else {                               /* 原 LAB_00032714 */
			add_item_buf(item, "\"");        /* DAT_00060810 */
			add_item_buf(item, head->name);
			add_item_buf(item, "\"");
			add_item_buf(item, ":");         /* DAT_00065964 */
		}
		node = head;
		switch (node->type) {
		case 0:                              /* API_ESCAPE: escape_string(data, isjson) */
			str = escape_string(node->data, isjson);
			if (isjson == 0) {
				add_item_buf(item, str);
			}
			else {
				add_item_buf(item, "\"");
				add_item_buf(item, str);
				add_item_buf(item, "\"");
			}
			if (str != node->data) {         /* escape_string 原样返回时不释放 */
				free(str);
			}
			goto after_value;
		case 1:                              /* API_STRING */
		case 2:                              /* API_CONST */
			if (isjson == 0) {
				add_item_buf(item, node->data);
			}
			else {
				add_item_buf(item, "\"");
				add_item_buf(item, node->data);
				add_item_buf(item, "\"");
			}
			goto after_value;
		case 3:                              /* API_UINT8 */
			snprintf(valbuf, 0x40, "%u", *(uint8_t *)node->data);
			break;
		case 4:                              /* signed short (输出 %d) */
			snprintf(valbuf, 0x40, "%d", (int)*(short *)node->data);
			break;
		case 5:                              /* unsigned short (输出 %u) */
			snprintf(valbuf, 0x40, "%u", *(unsigned short *)node->data);
			break;
		case 6:                              /* API_INT */
			snprintf(valbuf, 0x40, "%d", *(int *)node->data);
			break;
		case 7:                              /* API_UINT */
		case 8:                              /* API_UINT32 */
			snprintf(valbuf, 0x40, "%u", *(uint32_t *)node->data);
			break;
		case 9:                              /* API_HEX32 (上游 api.h) */
			if (isjson == 0) {
				snprintf(valbuf, 0x40, "0x%08x", *(uint32_t *)node->data);
				add_item_buf(item, valbuf);
			}
			else {
				add_item_buf(item, "\"");
				snprintf(valbuf, 0x40, "0x%08x", *(uint32_t *)node->data);
				add_item_buf(item, valbuf);
				add_item_buf(item, "\"");
			}
			goto after_value;
		case 10:                             /* API_UINT64 */
			snprintf(valbuf, 0x40, "%llu", *(unsigned long long *)node->data);
			break;
		case 11:                             /* API_INT64 */
			snprintf(valbuf, 0x40, "%lld", *(long long *)node->data);
			break;
		case 12:                             /* API_DOUBLE */
			snprintf(valbuf, 0x40, "%f", *(double *)node->data);
			break;
		case 13:                             /* API_ELAPSED */
			snprintf(valbuf, 0x40, "%.0f", *(double *)node->data);
			break;
		case 14:                             /* API_UTILITY: bool */
			str = "false";
			if (*(char *)node->data != '\0') {
				str = "true";
			}
			snprintf(valbuf, 0x40, "%s", str);
			break;
		case 15:                             /* API_TIMEVAL: tv_sec.tv_usec */
			snprintf(valbuf, 0x40, "%ld.%06ld", *(long *)node->data,
				 ((long *)node->data)[1]);
			break;
		case 16:                             /* API_TIME (32 位 time_t) */
			snprintf(valbuf, 0x40, "%lu", *(unsigned long *)node->data);
			break;
		case 17:                             /* API_MHS */
		case 20:
		case 21:
			snprintf(valbuf, 0x40, "%.2f", *(double *)node->data);
			break;
		case 18:                             /* API_MHTOTAL */
			snprintf(valbuf, 0x40, "%.4f", *(double *)node->data);
			break;
		case 19:                             /* API_TEMP (float) */
			snprintf(valbuf, 0x40, "%.2f", (double)*(float *)node->data);
			break;
		case 22:                             /* (float, %.3f) */
		case 26:
			snprintf(valbuf, 0x40, "%.3f", (double)*(float *)node->data);
			break;
		case 23:
			snprintf(valbuf, 0x40, "%.15f", *(double *)node->data);
			break;
		case 24:
			snprintf(valbuf, 0x40, "%.8f", *(double *)node->data);
			break;
		case 25:                             /* (double × 100, %.4f) */
			snprintf(valbuf, 0x40, "%.4f", *(double *)node->data * 100.0);
			break;
		default:
			if (DAT_0007ad48 != '\0' || DAT_00078e08 != '\0' || 2 < DAT_000766c4) {
				snprintf(logbuf, 0x1000, "API: unknown2 data type %d ignored",
					 node->type);
				applog(3, logbuf, 0);
			}
			if (isjson == 0) {
				add_item_buf(item, "Unknown");
			}
			else {
				add_item_buf(item, "\"");
				add_item_buf(item, "Unknown");
				add_item_buf(item, "\"");
			}
			goto after_value;
		}
		add_item_buf(item, valbuf);
after_value:                                 /* 原 LAB_000326c4 */
		free(node->name);                    /* api_add_data_full strdup */
		if (node->copy == 0) {
			head = node->next;
		}
		else {
			free(node->data);                /* copy=1: data 已拷贝 */
			head = node->next;
		}
		if (head == node) {                  /* 尾节点自指 → 收尾 */
			free(node);
			goto tail;
		}
		/* 环链摘除当前节点 */
		prev = node->prev;
		head->prev = prev;
		prev->next = head;
		free(node);
		add_item_buf(item, ",");             /* DAT_00060764 */
	}                                        /* 下一条目头 */
tail:                                        /* 原 LAB_00032760 */
	if (isjson == 0) {
		str = "|";
	}
	else {
		str = "}";
	}
	add_item_buf(item, str);
	io_add(io_data, *(char **)(*(int *)(item + 0xc)));   /* 暂存缓冲 → io_data */
	/* ---- 归还 item 到 StrBufs 池 (原锁序) ---- */
	mutex = *(pthread_mutex_t **)(DAT_00078cd4 + 8);
	rc = pthread_mutex_lock(mutex);
	if (rc != 0) {
		_mutex_lock("print_data", 0x558);
	}
	rc = pthread_rwlock_wrlock((pthread_rwlock_t *)(mutex + 1));
	if (rc != 0) {
		snprintf(logbuf, 0x1000, "WTF WRLOCK ERROR ON LOCK! errno=%d in %s %s():%d",
			 errno, "api.c", "print_data", 0x558);
		applog(3, logbuf, 1);
		quit(1);
	}
	_k_add_head(DAT_00078cd4, item, "api.c", "print_data", 0x559);
	mutex = *(pthread_mutex_t **)(DAT_00078cd4 + 8);
	rc = pthread_rwlock_unlock((pthread_rwlock_t *)(mutex + 1));
	if (rc != 0) {
		api_rw_unlock("print_data", 0x55a);
	}
	rc = pthread_mutex_unlock(mutex);
	if (rc != 0) {
		_mutex_unlock("print_data", 0x55a);
	}
	(*DAT_00075ed8)();
	return 0;
}

// === message === 原 FUN_0003305c
// 来源: reversed/cgminer/30_api_status/FUN_0003305c.c | 分类: 30_api_status
// 语义: 组装 STATUS 应答 — isjson 先写 {"STATUS":[; 查 codes[] 表
//       (DAT_000760fc, 16B/项 {severity, code, params, msg}): severity==4
//       (SEVERITY_FAIL, 表尾哨兵) 或码未命中 → 兜底 severity "F"、Code=-1、
//       Msg=sprintf("%d",code); 命中 → severity 映射 2→'I' 3→'S' 1→'W' 其余
//       'E' (sev_str[2]={char,0}), 按 params 分支 sprintf 消息串。链 STATUS/
//       When(DAT_00078cd8)/Code/Msg/Description(DAT_00075ed4=opt_api_description)
//       经 print_data 输出; isjson 补 "]"。与 api.c message() 一致。
void message(void *io_data, int code, int param0, int param1, int isjson)
{
	int *codes;                              /* 原 v3: 表游标 */
	int *code_ptr;                           /* 原 v1: 下一项 code 字段地址 */
	int severity;                            /* 原 v5 */
	int idx;                                 /* 原 v6: 条目索引 */
	int param;                               /* 命中项 params (switch 依据) */
	api_data_t *root;                        /* 原 v2 */
	int default_code;                        /* 原 uStack_2024 */
	char sev_str[2];                         /* 原 uStack_2028 + 栈@0x2027 */
	char *fmt;                               /* DAT_00076108[idx*4] */
	char *boolstr;                           /* 原 v4 */
	char msgbuf[8192];                       /* 原 栈@0x2020 */

	if (isjson != 0) {
		io_add(io_data, "{\"STATUS\":[");
	}
	if (DAT_000760fc[0] == 4) {
		goto default_msg;                    /* 原 LAB_000330f4 (entry0 即哨兵) */
	}
	if (DAT_00076100[0] == code) {
		idx = 0;
		severity = DAT_000760fc[0];
	}
	else {
		idx = 0;
		codes = &DAT_000760fc[0];
		do {
			severity = codes[4];             /* 下一项 severity */
			idx++;
			if (severity == 4) {
				goto default_msg;            /* 表尾哨兵 (SEVERITY_FAIL) */
			}
			code_ptr = &codes[5];            /* 下一项 code */
			codes += 4;                      /* 步长 16 字节 */
		} while (*code_ptr != code);
	}
	/* ---- 命中已知码: severity 字符 + 按 params 分支格式化消息 ---- */
	if (severity == 2) {
		sev_str[0] = 'I';                    /* SEVERITY_INFO */
	}
	else if (severity == 3) {
		sev_str[0] = 'S';                    /* SEVERITY_SUCC */
	}
	else if (severity == 1) {
		sev_str[0] = 'W';                    /* SEVERITY_WARN */
	}
	else {
		sev_str[0] = 'E';                    /* SEVERITY_ERR */
	}
	sev_str[1] = '\0';
	param = DAT_00076104[idx * 4];
	fmt = DAT_00076108[idx * 4];
	switch (param) {
	case 0:
	case 1:
	case 2:
	case 0xe:
		sprintf(msgbuf, fmt, param0);
		break;
	default:
		strcpy(msgbuf, fmt);
		break;
	case 5:
		sprintf(msgbuf, fmt, DAT_0007b198);           /* total_pools */
		break;
	case 6:
		sprintf(msgbuf, fmt, param0, DAT_0007b198 - 1);
		break;
	case 7:
		sprintf(msgbuf, fmt);
		break;
	case 8:
		sprintf(msgbuf, fmt, "command");
		break;
	case 9:
		sprintf(msgbuf, fmt, param0,
			*(char **)((uint8_t *)((void **)DAT_0007b438)[param0] + 0xc4));  /* pools[param0]->rpc_url */
		break;
	case 10:
		sprintf(msgbuf, fmt, param1);
		break;
	case 0xb:
		sprintf(msgbuf, fmt, param0, param1);
		break;
	case 0xc:
		boolstr = "false";
		if (param0 != 0) {
			boolstr = "true";
		}
		sprintf(msgbuf, fmt, boolstr);
		break;
	case 0xd:
		sprintf(msgbuf, fmt, param1, param0);
		break;
	}
	root = api_add_string(NULL, "STATUS", sev_str, 0);
	root = api_add_time(root, "When", &DAT_00078cd8, 0);
	root = api_add_int(root, "Code", &code, 0);
	goto build_tail;
default_msg:                                     /* 原 LAB_000330f4 */
	root = api_add_string(NULL, "STATUS", "F", 0);
	root = api_add_time(root, "When", &DAT_00078cd8, 0);
	default_code = -1;
	root = api_add_int(root, "Code", &default_code, 0);
	sprintf(msgbuf, "%d", code);
build_tail:
	root = api_add_escape(root, "Msg", msgbuf, 0);
	root = api_add_escape(root, "Description", DAT_00075ed4, 0);
	print_data(io_data, root, isjson, 0);
	if (isjson != 0) {
		io_add(io_data, "]");
	}
	return;
}

/* ================= 覆盖率表 ================= */
//
// 原 FUN        语义名             源行  源文件                                       分类          状态
// FUN_0001b8b4  zero_stats         129  reversed/cgminer/30_api_status/FUN_0001b8b4.c    30_api_status  完成
// FUN_00030500  escape_string       99  reversed/cgminer/30_api_status/FUN_00030500.c    30_api_status  完成
// FUN_000309e8  check_connect       79  reversed/cgminer/30_api_status/FUN_000309e8.c    30_api_status  完成
// FUN_00030b88  send_result        131  reversed/cgminer/30_api_status/FUN_00030b88.c    30_api_status  完成
// FUN_0003107c  mcast              403  reversed/cgminer/30_api_status/FUN_0003107c.c    30_api_status  完成
// FUN_00031cdc  api_add_data_full  123  reversed/cgminer/30_api_status/FUN_00031cdc.c    30_api_status  完成
// FUN_00032534  print_data         236  reversed/cgminer/30_api_status/FUN_00032534.c    30_api_status  完成
// FUN_0003305c  message            113  reversed/cgminer/30_api_status/FUN_0003305c.c    30_api_status  完成
// 合计 8/8 (源合计 ~1313 行)

