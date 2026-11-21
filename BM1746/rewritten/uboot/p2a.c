// ============================================================================
// uboot/p2a.c — U-Boot 2016.07 反编译转写 (半批 uboot_b2 前 24 函数)
// 地址范围: 0x040029f4 .. 0x04007678
// 命名事实源: rewritten/name_map/uboot.tsv
// 语义规则: rewritten/CONVENTIONS.md
// ============================================================================

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/*
 * struct slre — U-Boot v2016.07 include/slre.h (Beer-ware), 布局已由反编译
 * 栈帧证实: auStack_234[528] + local_24(err_str) 共 532 字节。
 */
typedef struct ub_slre {
	uint8_t		code[256];
	uint8_t		data[256];
	int32_t		code_size;
	int32_t		data_size;
	int32_t		num_caps;	/* 括号对数量 */
	int32_t		anchored;	/* 必须从头匹配 */
	const char	*err_str;	/* 编译失败信息 */
} ub_slre;

/*
 * 正则虚拟机操作码 — 值取自反编译指令流 (include/slre.h v2016.07 enum 顺序):
 * code[] 中 0x0f.. 为字符类转义 (\d\w\s 等) 的扩展操作码, 本文件按不透明值传递。
 */
enum ub_slre_op {
	UB_RE_END = 0,		UB_RE_BRANCH = 1,	UB_RE_ANY = 2,
	UB_RE_EXACT = 3,	UB_RE_ANYOF = 4,	UB_RE_ANYBUT = 5,
	UB_RE_OPEN = 6,		UB_RE_CLOSE = 7,	UB_RE_BOL = 8,
	UB_RE_EOL = 9,		UB_RE_STAR = 10,	UB_RE_PLUS = 11,
	UB_RE_STAR_QUEST = 12,	UB_RE_PLUS_QUEST = 13,	UB_RE_QUEST = 14
};

/* ----------------------------------------------------------------------------
 * 模块内部前向声明区
 * -------------------------------------------------------------------------- */

/* --- 本文件内函数 (按地址序定义, 此处前向声明) --- */
static void ub_sha256_final(uint32_t *ctx, uint8_t *digest);
static int  ub_un_02d018(void *param_1, void *param_2);
static int  ub_un_02f050(uint32_t *dst, uint32_t count, uint32_t limit);
static void ub_hdelete_entry(int *htab, int *filled_p, uint32_t *ep, int idx);
static int  ub_match_filter(const char **key_pp, const char **data_pp,
                            uint32_t flag, int argc, const char *const *argv);
static void ub_hdestroy(int *htab);
static void ub_regex_parse(ub_slre *r, const char **re);
static int  ub_regex_compile(ub_slre *r, const char *re);
static int  ub_strcmp(const char *s1, const char *s2);
static uint32_t ub_crc32_le(uint32_t crc, const uint8_t *buf, uint32_t len);
static char *ub_strstr(char *haystack, char *needle);
static uint64_t ub_timer_read(void);
static void ub_udelay_raw(uint32_t usec);
static char *ub_put_dec(char *buf, uint64_t num);
static void ub_number_fmt(char *buf, const char *end, uint32_t num_lo,
                          uint32_t num_hi, uint32_t base, int32_t size,
                          int32_t precision, uint32_t type);
static int  ub_parse_hex_u32(const char *str, uint32_t *out);

/* --- 非本文件函数 — 命名来自 rewritten/name_map/uboot.tsv (定义于 p1a/p3a) --- */
extern void ub_sha256_update(uint32_t *ctx, const void *data, uint32_t len);
extern int  ub_str_match(uint32_t flag, const char *str, const char *pat,
                         const void *slre);
extern uint32_t ub_printf(const char *fmt, ...);
extern void ub_memset(void *dst, uint8_t fill, uint32_t len);
extern void ub_memcpy(void *dst, const void *src, uint32_t len);
/* 正则族被调函数 (定义于 p3a.c, 布局与 ub_slre 同一) */
extern int  ub_regex_escape_decode(const char **re);   /* 返回 16 位: hi=类标记, lo=字符 */
extern void ub_regex_emit_quant(uint8_t *prog, int pos, uint8_t op);
extern void ub_regex_prog_insert(uint8_t *prog, int pos, int len);
extern char *ub_strchr(char *s, char c);
/* vsprintf 族被调函数 (定义于 p1a.c) */
extern uint32_t ub_div64_rem(uint32_t *n, uint32_t base);	/* __div64_32: 商写回 *n, 返回余数 */
extern char *ub_put_dec_helper(char *buf, uint32_t val);	/* put_dec_trunc: 1~5 位, 前导零抑制 */
/* __func__ 自报名函数 (定义于后批 p2b/p3b, b3 半批) */
extern uint32_t simple_strtoul(const char *str, char **endp, int base);
/* 定时器频率获取 (get_tbclk 语义, 无实参形态; Ghidra 未恢复签名), 命名待补 */
extern uint32_t fun_03fb2008();

/* u-boot 全局数据指针: ARM ABI 下 u-boot 以 r9 传递 gd (原反编译 unaff_r9) */
register uint8_t *gd_r9 __asm__("r9");

/* --- 跨区 thunk: Ghidra 以 func_0x 原样引用, name_map 未收录 → 占位命名 --- */
/* 释放器: hashtable.c 中按 free() 语义使用 (1 指针实参), 命名待补 */
extern void fun_03fc8570(void *ptr);
/* 查表/取串辅助: 同一目标被以 0/4 个实参两种形态调用 (Ghidra 未恢复签名),
 * 故用无原型旧式声明, 命名待补 */
extern char *fun_03ffae84();
/* div0 处理器中的两步调用 (打印类/未知类), 命名待补 */
extern void fun_03fc7fbc(const char *s);
extern void fun_03fbf43c(int code);
/* 按 32 字节对齐分配 (memalign(32, size) 语义), 命名待补 */
extern void *fun_03fc9318(int align, uint32_t size);

/* --- 未知全局 (Ghidra 命名) --- */
extern const char DAT_04002ba4[]; /* 全局, 传入 sha256_update 的填充块 (0x80,0,0,...), 语义待定 */
extern const char DAT_04002d4c[]; /* 全局, 语义待定 */
extern const char DAT_04002d50[]; /* 全局, 语义待定 */
extern const char DAT_040032fc[]; /* printf 格式串, 上游对应 "Error compiling regex: %s\n", 语义待定 */
extern const char DAT_040042a8[]; /* printf 格式串, 语义待定 */
extern const char DAT_040042a4[]; /* printf 实参 (串/缓冲), 语义待定 */
extern const char DAT_0400485c[]; /* div0 处理器使用的串, 语义待定 */
extern const char DAT_040051b0[]; /* err_str 错误串常量, 上游对应 "RE is too long (code overflow)", 语义待定 */
extern const char DAT_040051b4[]; /* err_str 错误串常量, 上游对应 "Jump offset is too big", 语义待定 */
extern const char DAT_040056fc[]; /* 元字符集字符串 (exact() 停止字符表), 上游对应 "|^$.[()\\?*+", 语义待定 */
extern const char DAT_04005700[]; /* err_str 错误串常量, 上游对应 "RE is too long (data overflow)", 语义待定 */
extern const char DAT_04005704[]; /* err_str 错误串常量, 上游对应 "Unbalanced brackets", 语义待定 */
extern const char DAT_04005708[]; /* err_str 错误串常量, 上游对应 "RE is too long (code overflow)", 语义待定 */
extern const char DAT_0400570c[]; /* err_str 错误串常量, 上游对应 "Jump offset is too big", 语义待定 */
extern const char DAT_04005710[]; /* err_str 错误串常量, 上游对应 "No closing ']' bracket", 语义待定 */
extern const char DAT_04005714[]; /* err_str 错误串常量, 上游对应 "No closing bracket", 语义待定 */
extern const char DAT_04005838[]; /* err_str 错误串常量, 上游对应 "RE is too long (code overflow)", 语义待定 */
extern const char DAT_040069ec[]; /* 数字表字符串, 上游对应 "0123456789ABCDEF" (OR 0x20 转小写), 语义待定 */
extern const uint32_t DAT_04006690; /* 64 位常量低字 (=99999, 由 put_dec 算法推得), 语义待定 */
extern const uint32_t DAT_04006694; /* 64 位常量高字 (=0, 由 put_dec 算法推得), 语义待定 */
extern const uint32_t DAT_04005bfbc[]; /* CRC-32 查找表 (256 项, 字面量池 @0x0405bfbc 引用), 语义待定 */

// ============================================================================
// 函数转写区 (按地址升序)
// ============================================================================

// === ub_sha256_final === 原 FUN_040029f4 @ 0x040029f4
// 来源: reversed/uboot_funcs/FUN_040029f4.c | 分类: uboot
// 语义: SHA-256 收尾 — 填充至块边界 (0x80,0,0...), 追加 64 位大端总比特长度,
//       再把 8 个状态字 (ctx+8 起, 各 4 字节大端) 写入 32 字节摘要。
//       上下文布局与上游 lib/sha256.c sha256_context 一致:
//       total[2]@0 / state[8]@8 / buffer[64]@0x28。
static void ub_sha256_final(uint32_t *ctx, uint8_t *digest)
{
	uint32_t total_lo = ctx[0];
	uint32_t total_hi = ctx[1];
	uint32_t rem = total_lo & 0x3f;
	uint32_t pad = (rem < 0x38) ? 0x38 - rem : 0x78 - rem;
	uint32_t high = (total_lo >> 29) | (total_hi << 3);
	uint32_t low = total_lo << 3;
	uint8_t msglen[8];
	int i;

	/* 总比特长度 (total[1]:total[0] << 3) 大端 8 字节 — 与原逐字节移位拼装等价 */
	msglen[0] = (uint8_t)(high >> 24);
	msglen[1] = (uint8_t)(high >> 16);
	msglen[2] = (uint8_t)(high >> 8);
	msglen[3] = (uint8_t)high;
	msglen[4] = (uint8_t)(low >> 24);
	msglen[5] = (uint8_t)(low >> 16);
	msglen[6] = (uint8_t)(low >> 8);
	msglen[7] = (uint8_t)low;

	ub_sha256_update(ctx, DAT_04002ba4, pad);
	ub_sha256_update(ctx, msglen, 8);

	/* 8 个状态字 (ctx 字 2..9, 即字节 8..0x27) 逐字大端拷出 */
	for (i = 0; i < 8; i++) {
		uint32_t w = ctx[2 + i];
		digest[4 * i + 0] = (uint8_t)(w >> 24);
		digest[4 * i + 1] = (uint8_t)(w >> 16);
		digest[4 * i + 2] = (uint8_t)(w >> 8);
		digest[4 * i + 3] = (uint8_t)w;
	}
}

// === ub_un_02d018 === 原 FUN_04002d18 @ 0x04002d18
// 来源: reversed/uboot_funcs/FUN_04002d18.c | 分类: uboot
// 语义: 经外部查表 thunk 取串; 取不到返回真, 否则与全局串比较相等才返回真。
//       (Ghidra 未恢复实参类型与全局语义, 命名回退)
static int ub_un_02d018(void *param_1, void *param_2)
{
	char *result = fun_03ffae84(param_1, param_2, DAT_04002d4c, 0);

	if (result == 0)
		return 1;
	return ub_strcmp(result, DAT_04002d50) == 0;
}

// === ub_un_02f050 === 原 FUN_04002f50 @ 0x04002f50
// 来源: reversed/uboot_funcs/FUN_04002f50.c | 分类: uboot
// 语义: 经外部 thunk 取得大端 uint32 数组首地址, 逐字字节序反转 (BE→LE)
//       拷入 dst 共 count 字; 空间不足或源为空返回负错误码。
//       注: Ghidra 将本函数签名恢复为 (void), dst/count/limit 实参实际来自
//       寄存器/栈 (对应原 in_r3 / in_stack_00000000 / local_14), 形参顺序为转写重建。
static int ub_un_02f050(uint32_t *dst, uint32_t count, uint32_t limit)
{
	const uint32_t *src = (const uint32_t *)fun_03ffae84(); /* Ghidra 丢失实参 */
	uint32_t i;

	if (src == 0)
		return -1;	/* 0xffffffff */
	if (limit < count * 4)
		return -12;	/* 0xfffffff4, 语义: -ENOMEM */
	for (i = 0; i < count; i++) {
		uint32_t v = src[i];
		dst[i] = v << 24 | (v >> 8 & 0xff) << 16 | (v >> 16 & 0xff) << 8 | v >> 24;
	}
	return 0;
}

// === ub_hdelete_entry === 原 FUN_040030a8 @ 0x040030a8
// 来源: reversed/uboot_funcs/FUN_040030a8.c | 分类: uboot
// 语义: 删除 env 哈希表项 (对应 U-Boot v2016.07 lib/hashtable.c 的 _hdelete):
//       释放 entry 的 key/data, 清 callback/flags, 槽位 used 置 -1 (删除标记),
//       已用计数减一。_ENTRY 表项 stride 0x14 = { int used; ENTRY entry; },
//       ENTRY = { key, data, callback, flags } (include/search.h)。
static void ub_hdelete_entry(int *htab, int *filled_p, uint32_t *ep, int idx)
{
	fun_03fc8570((void *)ep[0]);	/* free(ep->key) */
	fun_03fc8570((void *)ep[1]);	/* free(ep->data) */
	ep[2] = 0;			/* ep->callback = NULL */
	ep[3] = 0;			/* ep->flags = 0 */
	*(uint32_t *)(htab[0] + idx * 0x14) = 0xffffffff;	/* table[idx].used = -1 */
	*filled_p = *filled_p - 1;	/* htab->filled-- */
}

// === ub_match_filter === 原 FUN_04003210 @ 0x04003210
// 来源: reversed/uboot_funcs/FUN_04003210.c | 分类: uboot
// 语义: 按模式串逐个匹配单个 env 表项 (对应 hashtable.c 的 match_entry):
//       每个模式先经正则编译 (失败则打印错误并整体返回 0), 然后
//       flag&0x10 (H_MATCH_KEY) 匹配 key、flag&0x20 (H_MATCH_DATA) 匹配 data,
//       任一命中返回 1, 全部未中返回 0。
//       调用方 (hexport_r) 传入 &entry.key / &entry.data。
static int ub_match_filter(const char **key_pp, const char **data_pp,
                           uint32_t flag, int argc, const char *const *argv)
{
	ub_slre slre;
	int arg;

	if (argc > 0) {
		for (arg = 0; arg < argc; arg++) {
			if (ub_regex_compile(&slre, argv[arg]) == 0) {
				ub_printf(DAT_040032fc, slre.err_str);
				return 0;
			}
			if ((flag & 0x10) != 0 &&
			    ub_str_match(flag, *key_pp, argv[arg], &slre) != 0)
				return 1;
			if ((flag & 0x20) != 0 &&
			    ub_str_match(flag, *data_pp, argv[arg], &slre) != 0)
				return 1;
		}
	}
	return 0;
}

// === ub_hdestroy === 原 FUN_0400339c @ 0x0400339c
// 来源: reversed/uboot_funcs/FUN_0400339c.c | 分类: uboot
// 语义: 销毁 env 哈希表 (对应 hashtable.c 的 hdestroy_r): 从槽位 1 到 size
//       (槽 0 保留) 遍历, used>0 的表项释放 key/data; 随后释放整张表并将
//       htab->table 置 NULL。本固件版本无 htab NULL 入参检查 (与反编译一致)。
static void ub_hdestroy(int *htab)
{
	uint32_t *table = (uint32_t *)htab[0];	/* htab->table, _ENTRY 数组 (每项 5 字) */
	uint32_t size = (uint32_t)htab[1];	/* htab->size */
	uint32_t i;

	if (size != 0) {
		for (i = 1; i <= size; i++) {
			uint32_t e = i * 5;	/* &table[i] (stride 0x14) */
			if (0 < (int)table[e]) {	/* entry.used > 0 */
				fun_03fc8570((void *)table[e + 1]);	/* free(entry.key) */
				fun_03fc8570((void *)table[e + 2]);	/* free(entry.data) */
			}
		}
	}
	/* free(htab->table): 反编译处实参被 Ghidra 丢弃, 依调用点寄存器即 table */
	fun_03fc8570((void *)table);
	htab[0] = 0;	/* htab->table = NULL (非 NULL 即表处于活动状态) */
}

// === ub_stub_printf0 === 原 FUN_0400428c @ 0x0400428c
// 来源: reversed/uboot_funcs/FUN_0400428c.c | 分类: uboot
// 语义: 打印一条带全局实参的消息后返回 0 (打印存根)。DAT 语义待定。
static int ub_stub_printf0(void)
{
	ub_printf(DAT_040042a8, DAT_040042a4);
	return 0;
}

// === ub_crc32_le === 原 FUN_0400461c @ 0x0400461c
// 来源: reversed/uboot_funcs/FUN_0400461c.c | 分类: uboot
// 语义: zlib 风格 CRC-32 核心 (查单表 DAT_04005bfbc): 未对齐前缀逐字节、
//       对齐主体按 4 字节字读入后连做 4 次查表 (crc^=word 再 4 次
//       crc = T[crc&0xff] ^ (crc>>8))、尾部逐字节。返回未终结异或的 crc
//       (调用方自行取反)。
//       注: 反编译产物丢失了循环体内的查表/异或运算与返回值, 此处算法按
//       0x0400461c-0x04004714 机器码逐指令核实。
static uint32_t ub_crc32_le(uint32_t crc, const uint8_t *buf, uint32_t len)
{
	uint32_t i, n;

	/* 未对齐前缀: 逐字节, len 耗尽即返回 */
	while (((uintptr_t)buf & 3) != 0) {
		if (len == 0)
			return crc;
		crc = DAT_04005bfbc[(*buf++ ^ crc) & 0xff] ^ (crc >> 8);
		len--;
	}
	/* 对齐主体: 每次一字 */
	for (i = 0, n = len >> 2; i < n; i++) {
		crc ^= *(const uint32_t *)buf;
		buf += 4;
		crc = DAT_04005bfbc[crc & 0xff] ^ (crc >> 8);
		crc = DAT_04005bfbc[crc & 0xff] ^ (crc >> 8);
		crc = DAT_04005bfbc[crc & 0xff] ^ (crc >> 8);
		crc = DAT_04005bfbc[crc & 0xff] ^ (crc >> 8);
	}
	/* 尾部不足 4 字节: 逐字节 */
	for (n = len & 3; n != 0; n--)
		crc = DAT_04005bfbc[(*buf++ ^ crc) & 0xff] ^ (crc >> 8);
	return crc;
}

// === ub_crc32_store === 原 FUN_04004714 @ 0x04004714
// 来源: reversed/uboot_funcs/FUN_04004714.c | 分类: uboot
// 语义: 计算标准 CRC-32 (初值/终结均取反, 即 zlib crc32(0,buf,len)) 并以
//       大端 4 字节写入 out (uImage 头部 CRC 字段字序)。
static void ub_crc32_store(const void *buf, uint32_t len, void *out)
{
	uint32_t crc = ub_crc32_le(0xffffffffU, buf, len);
	uint32_t be;

	crc = ~crc;
	be = crc << 24 | (crc >> 8 & 0xff) << 16 | (crc >> 16 & 0xff) << 8 | crc >> 24;
	ub_memcpy(out, &be, 4);
}

// === ub_div0_hang === 原 FUN_04004844 @ 0x04004844
// 来源: reversed/uboot_funcs/FUN_04004844.c | 分类: uboot
// 语义: 除零异常处理 (对应 libgcc __div0): 两步外部调用后死循环挂死。
//       第二个实参 0xffffffe2 = -30, 语义待定。
static void ub_div0_hang(void)
{
	fun_03fc7fbc(DAT_0400485c);
	fun_03fbf43c(0xffffffe2);
	for (;;) {
		/* 原始注释位: Ghidra "Do nothing block with infinite loop" */
	}
}

// === ub_memalign32 === 原 FUN_04004860 @ 0x04004860
// 来源: reversed/uboot_funcs/FUN_04004860.c | 分类: uboot
// 语义: 以 32 字节对齐分配 size 字节 (memalign(0x20, size) 语义);
//       当 flags 的 bit15 (0x8000) 置位时把分配区清零。返回分配指针。
static void *ub_memalign32(uint32_t size, uint32_t flags)
{
	void *ptr = fun_03fc9318(0x20, size);

	if ((flags & 0x8000) != 0)
		ub_memset(ptr, 0, size);
	return ptr;
}

// === ub_regex_node_setlen === 原 FUN_04005170 @ 0x04005170
// 来源: reversed/uboot_funcs/FUN_04005170.c | 分类: uboot
// 语义: slre.c fixup_branch — 先向 code 区压入一字节 END(0x00) (code 溢出则
//       置 err_str), 再把 code[fixup] 补写为 code_size-(fixup-2) 的跳距
//       (超过 0xff 则置 err_str)。
//       注: 上游 v2016.07 有 if (fixup > 0) 外层守卫, 反编译未见 (调用点已保证)。
static void ub_regex_node_setlen(ub_slre *r, int fixup)
{
	int32_t n = r->code_size;

	if (n < 0x100) {
		/* emit(r, END) */
		r->code_size = n + 1;
		r->code[n] = 0;
		n++;
	} else {
		r->err_str = DAT_040051b0;
	}
	n = n - (fixup - 2);
	if (n < 0x100)
		r->code[fixup] = (uint8_t)n;
	else
		r->err_str = DAT_040051b4;
}

// === ub_regex_parse === 原 FUN_040051b8 @ 0x040051b8
// 来源: reversed/uboot_funcs/FUN_040051b8.c | 分类: uboot
// 语义: slre.c 的静态 compile() — 正则模式解析器, 把 **re 编译为 code[] 字节码。
//       code/data 区各 0x100 字节, 溢出只置 err_str (粘滞) 并继续解析, 不中断。
//       prev = 上一节点起始位置, 供后续量词回指 (仅 '.'/字面量/[..]/(..)/\\ 分支更新,
//       '$'/^/量词/| 分支不更新 — 与原指令一致)。错误串均为全局 DAT 常量。
// 注: 反编译已按 0x040051b8-0x04005700 机器码逐指令核对 (含寄存器 r6/r8/r10/r11)。
static void ub_regex_parse(ub_slre *r, const char **re)
{
	int32_t branch_start = r->code_size;	/* r8: 本层起始 (BRANCH 落点) */
	int32_t fixup = 0;			/* r10: 顶层 '|' 待回填占位位置 */
	int32_t num_caps_entry = r->num_caps;	/* [sp+4]: 入口捕获计数 (')' 平衡检查用) */
	int32_t prev = branch_start;		/* r6: 上一节点起始位置 */

	for (;;) {
		const char c = **re;
		*re += 1;

		if (c == '\0') {
			*re -= 1;			/* 原样回退, 交还上层 */
			return;
		}
		if (c == '$') {				/* emit EOL(9) */
			if (r->code_size < 0x100) {
				r->code[r->code_size] = UB_RE_EOL;
				r->code_size++;
			} else {
				r->err_str = DAT_04005708;
			}
			continue;
		}
		if (c == '(') {
			uint8_t cap_id;
			prev = r->code_size;
			r->num_caps = r->num_caps + 1;
			cap_id = (uint8_t)r->num_caps;
			/* emit OPEN(6), cap_id */
			if (r->code_size > 0xff) {
				r->err_str = DAT_04005708;
			} else {
				int32_t cs0 = r->code_size;
				r->code[cs0] = UB_RE_OPEN;
				r->code_size = cs0 + 1;
				if (r->code_size == 0x100)
					r->err_str = DAT_04005708;
				else {
					r->code[cs0 + 1] = cap_id;
					r->code_size = cs0 + 2;
				}
			}
			ub_regex_parse(r, re);		/* 递归编译括号内模式 */
			if (*(*re)++ != ')') {
				r->err_str = DAT_04005714;
				return;
			}
			/* emit CLOSE(7), cap_id — 溢出错后回主循环 (原 0x536c) */
			if (r->code_size > 0xff) {
				r->err_str = DAT_04005708;
			} else {
				int32_t cs0 = r->code_size;
				r->code[cs0] = UB_RE_CLOSE;
				r->code_size = cs0 + 1;
				if (r->code_size == 0x100)
					r->err_str = DAT_04005708;
				else {
					r->code[cs0 + 1] = cap_id;
					r->code_size = cs0 + 2;
				}
			}
			continue;
		}
		if (c == ')') {
			*re -= 1;
			if (fixup > 0)
				ub_regex_node_setlen(r, fixup);
			if (num_caps_entry == 0)
				r->err_str = DAT_04005704;
			return;
		}
		if (c == '.') {			/* emit ANY(2), 并更新 prev (0x5550) */
			prev = r->code_size;
			if (r->code_size < 0x100) {
				r->code[r->code_size] = UB_RE_ANY;
				r->code_size++;
			} else {
				r->err_str = DAT_04005708;
			}
			continue;
		}
		if (c == '?') {			/* prev 不更新 (原 0x531c 分支) */
			ub_regex_emit_quant((uint8_t *)r, prev, UB_RE_QUEST);
			continue;
		}
		if (c == '*' || c == '+') {
			uint8_t op = (c == '*') ? UB_RE_STAR : UB_RE_PLUS;
			if (**re == '?') {
				*re += 1;
				op += 2;		/* '*?'→12, '+?'→13 (懒惰量词) */
			}
			ub_regex_emit_quant((uint8_t *)r, prev, op);
			continue;
		}
		if (c == '^') {				/* emit BOL(8) */
			if (r->code_size < 0x100) {
				r->code[r->code_size] = UB_RE_BOL;
				r->code_size++;
			} else {
				r->err_str = DAT_04005708;
			}
			continue;
		}
		if (c == '|') {
			if (fixup > 0)
				ub_regex_node_setlen(r, fixup);
			ub_regex_prog_insert((uint8_t *)r, branch_start, 3);
			r->code[branch_start] = UB_RE_BRANCH;
			{
				int32_t len = r->code_size - branch_start;
				if (len < 0x100)
					r->code[branch_start + 1] = (uint8_t)len;
				else
					r->err_str = DAT_0400570c;
			}
			fixup = branch_start + 2;
			r->code[fixup] = 0xff;		/* 跳距占位, 由 fixup_branch 回填 */
			continue;
		}
		if (c == '\\') {
			int esc = ub_regex_escape_decode(re);	/* re 已被其推进 */
			int32_t ds;
			prev = r->code_size;
			if ((esc & 0xff00) != 0) {
				/* 单码字节: hi 位含类操作码, 直接入 code 区 */
				if (r->code_size > 0xff) {
					r->err_str = DAT_04005708;
				} else {
					r->code[r->code_size] = (uint8_t)(esc >> 8);
					r->code_size++;
				}
				continue;
			}
			/* 普通转义字符: emit {EXACT(3), data_size, 1} 后字符入 data 区 */
			ds = r->data_size;
			if (r->code_size > 0xff) {
				r->err_str = DAT_04005708;
			} else {
				int32_t cs0 = r->code_size;
				r->code[cs0] = UB_RE_EXACT;
				r->code_size = cs0 + 1;
				if (r->code_size == 0x100)
					r->err_str = DAT_04005708;
				else {
					r->code[cs0 + 1] = (uint8_t)ds;
					r->code_size = cs0 + 2;
					if (r->code_size == 0x100)
						r->err_str = DAT_04005708;
					else {
						r->code[cs0 + 2] = 1;
						r->code_size = cs0 + 3;
					}
				}
			}
			if (ds > 0xff)
				r->err_str = DAT_04005700;
			else {
				r->data[ds] = (uint8_t)(esc & 0xff);
				r->data_size = ds + 1;
			}
			continue;
		}
		if (c == '[') {
			/* anyof/anybut 内联: '[' 后可选 '^', 逐字符入 data 区直至 ']' */
			int op = UB_RE_ANYOF;
			int32_t old_ds = r->data_size;
			prev = r->code_size;
			if (**re == '^') {
				*re += 1;
				op = UB_RE_ANYBUT;
			}
			while (**re != '\0') {
				int32_t ch = (uint8_t)(**re);
				*re += 1;
				if (ch == '\\') {
					int esc = ub_regex_escape_decode(re);
					if ((esc & 0xff) != 0) {
						/* 单字节转义: 仅 lo 字节入 data 区 */
						if (r->data_size < 0x100) {
							r->data[r->data_size] = (uint8_t)esc;
							r->data_size++;
						} else {
							r->err_str = DAT_04005700;
						}
					} else if (r->data_size > 0xff) {
						r->err_str = DAT_04005700;
					} else {
						/* 双字节 (类转义): 先 lo(恒 0) 后 hi(类标记) */
						int32_t ds2 = r->data_size + 1;
						r->data_size = ds2;
						r->data[ds2 - 1] = 0;
						if (ds2 != 0x100) {
							r->data[ds2] = (uint8_t)(esc >> 8);
							r->data_size = ds2 + 1;
						} else {
							r->err_str = DAT_04005700;
						}
					}
				} else if (ch == ']') {
					/* 收尾: emit {op, old_ds, len}, 第三字节落位后回主循环 */
					if (r->code_size > 0xff) {
						r->err_str = DAT_04005708;
					} else if (r->code_size + 1 == 0x100) {
						r->code[r->code_size] = (uint8_t)op;
						r->code_size = r->code_size + 1;
						r->err_str = DAT_04005708;
					} else {
						int32_t cs0 = r->code_size;
						r->code[cs0] = (uint8_t)op;
						r->code_size = cs0 + 1;
						r->code[cs0 + 1] = (uint8_t)old_ds;
						r->code_size = cs0 + 2;
						if (cs0 + 2 == 0x100)
							r->err_str = DAT_04005708;
						else {
							r->code[cs0 + 2] = (uint8_t)(r->data_size - old_ds);
							r->code_size = cs0 + 3;
						}
					}
					goto dispatch;		/* 原 joined_r0x040054d0 → 0x51e0 */
				} else {
					if (r->data_size < 0x100) {
						r->data[r->data_size] = (uint8_t)ch;
						r->data_size++;
					} else {
						r->err_str = DAT_04005700;
					}
				}
			}
			r->err_str = DAT_04005710;
			continue;
		}
		/* 默认: exact() — 字面量串入 data 区后 emit {EXACT(3), old_ds, len} */
		{
			int32_t old_ds = r->data_size;
			*re -= 1;
			while (**re != '\0' &&
			       ub_strchr((char *)DAT_040056fc, **re) == NULL) {
				int32_t ch = (uint8_t)(**re);
				*re += 1;
				if (r->data_size < 0x100) {
					r->data[r->data_size] = (uint8_t)ch;
					r->data_size++;
				} else {
					r->err_str = DAT_04005700;
				}
			}
			prev = r->code_size;
			if (r->code_size > 0xff) {
				r->err_str = DAT_04005708;
			} else {
				int32_t cs0 = r->code_size;
				r->code[cs0] = UB_RE_EXACT;
				r->code_size = cs0 + 1;
				if (r->code_size == 0x100) {
					r->err_str = DAT_04005708;
				} else {
					int32_t len = r->data_size - old_ds;
					r->code[cs0 + 1] = (uint8_t)old_ds;
					r->code_size = cs0 + 2;
					if (cs0 + 2 == 0x100)
						r->err_str = DAT_04005708;
					else {
						r->code[cs0 + 2] = (uint8_t)len;
						r->code_size = cs0 + 3;
					}
				}
			}
		}
dispatch:	;	/* 原 LAB_040051e0 主循环 */
	}
}

// === ub_regex_compile === 原 FUN_04005718 @ 0x04005718
// 来源: reversed/uboot_funcs/FUN_04005718.c | 分类: uboot
// 语义: slre_compile — 复位计数器/err_str, code[0..1] = {OPEN(6), 0},
//       循环调用 ub_regex_parse 直至模式尾; 存在顶层 BRANCH 时回填 code[4]
//       占位 (fixup_branch), 再 emit {CLOSE(7), 0} 与 END(0)。
//       返回 err_str == NULL (无错误)。0x100 边界任一步溢出 → 置错并返回 0。
static int ub_regex_compile(ub_slre *r, const char *re)
{
	const char *p = re;
	int32_t cs;

	r->anchored = 0;
	r->code_size = 0;
	r->err_str = 0;
	r->num_caps = 0;
	r->data_size = 0;

	if (re[0] == '^')
		r->anchored = 1;
	r->code_size = 2;
	r->code[0] = UB_RE_OPEN;	/* 原半字写入 {6, 0} */
	r->code[1] = 0;

	if (re[0] == '\0') {
		if (r->code[2] != UB_RE_BRANCH) {
			cs = 4;
			r->code[2] = UB_RE_CLOSE;	/* 原半字写入 {7, 0} */
			r->code[3] = 0;
			goto emit_end;
		}
		ub_regex_node_setlen(r, 4);
	} else {
		do {
			ub_regex_parse(r, &p);
		} while (*p != '\0');
		if (r->code[2] == UB_RE_BRANCH)
			ub_regex_node_setlen(r, 4);
	}

	cs = r->code_size;
	if (cs > 0xff)
		goto overflow;
	r->code[cs] = UB_RE_CLOSE;
	r->code_size = cs + 1;
	if (cs + 1 == 0x100)
		goto overflow;
	r->code[cs + 1] = 0;
	r->code_size = cs + 2;
	if (cs + 2 == 0x100)
		goto overflow;
emit_end:
	r->code_size = cs + 1;
	r->code[cs] = UB_RE_END;
	return r->err_str == 0;
overflow:
	r->err_str = DAT_04005838;
	return 0;
}

// === ub_strncpy === 原 FUN_040059ac @ 0x040059ac
// 来源: reversed/uboot_funcs/FUN_040059ac.c | 分类: uboot
// 语义: 复制至多 n 字节; 遇 NUL 提前结束且不做零填充 ( BSD 风格 strncpy)。
static void ub_strncpy(char *dst, const char *src, int n)
{
	int i;

	for (i = 0; i < n; i++) {
		dst[i] = src[i];
		if (dst[i] == '\0')
			return;
	}
}

// === ub_strcmp === 原 FUN_04005a60 @ 0x04005a60
// 来源: reversed/uboot_funcs/FUN_04005a60.c | 分类: uboot
// 语义: 字符串比较。返回值为差值的低 8 位带符号扩展 (原指令 sub + sxtb),
//       与标准 strcmp 的全宽差值不同, 调用方只判 0/正/负时等价。
static int ub_strcmp(const char *s1, const char *s2)
{
	const char *p = s1 - 1;
	int d;

	do {
		p++;
		d = (int8_t)((uint8_t)*p - (uint8_t)*s2);
		if (d != 0)
			return d;
		s2++;
	} while (*p != '\0');
	return 0;
}

// === ub_strncmp === 原 FUN_04005a90 @ 0x04005a90
// 来源: reversed/uboot_funcs/FUN_04005a90.c | 分类: uboot
// 语义: 比较至多 n 个字符, 遇 NUL 提前返回 0; 返回值同 ub_strcmp 为
//       低 8 位带符号扩展差值 (sub + sxtb)。
static int ub_strncmp(const char *s1, const char *s2, int n)
{
	const char *end;
	int d;

	if (n == 0)
		return 0;
	d = (int8_t)((uint8_t)*s1 - (uint8_t)*s2);
	if (d != 0)
		return d;
	if (*s1 == '\0')
		return 0;
	end = s2 + n;
	while (++s2 != end) {
		s1++;
		d = (int8_t)((uint8_t)*s1 - (uint8_t)*s2);
		if (d != 0)
			return d;
		if (*s1 == '\0')
			return 0;
	}
	return 0;
}

// === ub_strrchr === 原 FUN_04005b44 @ 0x04005b44
// 来源: reversed/uboot_funcs/FUN_04005b44.c | 分类: uboot
// 语义: 从结尾 NUL (含) 起反向查找首个等于 c 的字符 (标准 strrchr 语义);
//       未找到返回 NULL。
static char *ub_strrchr(const char *s, char c)
{
	const char *p = s;
	char cur = *p;

	while (cur != '\0') {
		p++;
		cur = *p;
	}
	cur = '\0';
	for (;;) {
		if (cur == c)
			return (char *)p;
		p--;
		if (p < s)
			return 0;
		cur = *p;
	}
}

// === ub_strstr === 原 FUN_04006044 @ 0x04006044
// 来源: reversed/uboot_funcs/FUN_04006044.c | 分类: uboot
// 语义: 子串查找 (标准 strstr): needle 为空返回 haystack; 否则逐一试
//       haystack 的 0..len(hay)-len(needle) 起点, 内层双指针同步比对,
//       窗口末字符相等即整窗命中返回起点; 全部未中返回 NULL。
static char *ub_strstr(char *haystack, char *needle)
{
	char *h, *n;
	char ch, nc;
	int needle_len, hay_len, remain;

	if (*needle == '\0')
		return haystack;
	n = needle;
	do {
		n++;
	} while (*n != '\0');
	needle_len = (int)(n - needle);
	if (needle_len == 0)
		return haystack;
	ch = *haystack;
	if (ch == '\0') {
		hay_len = 0;
	} else {
		h = haystack;
		do {
			h++;
		} while (*h != '\0');
		hay_len = (int)(h - haystack);
	}
	if (needle_len <= hay_len) {
		remain = hay_len;
		while (remain >= needle_len) {
			remain--;
			h = haystack;
			n = needle;
			nc = *n;
			while (ch == nc) {
				if (haystack + needle_len - 1 == h)
					return haystack;
				h++;
				n++;
				nc = *n;
				ch = *h;
			}
			if (remain < needle_len)
				break;
			haystack++;
			ch = *haystack;
		}
	}
	return NULL;
}

// === ub_timer_read === 原 FUN_040061b0 @ 0x040061b0
// 来源: reversed/uboot_funcs/FUN_040061b0.c | 分类: uboot
// 语义: 读取递减定时器计数 (MMIO 0xf8f00604, 原 uRamf8f00604) 并取反为递增刻度;
//       计数回绕 (本次 < 上次) 时 gd+0x88 (高 32 位) 加一, gd+0x8c 存本次值。
//       返回 64 位 {高:gd+0x88, 低:gd+0x8c}。
static uint64_t ub_timer_read(void)
{
	uint32_t now = ~*(volatile uint32_t *)0xf8f00604;

	if (now < *(volatile uint32_t *)(gd_r9 + 0x8c))
		*(uint32_t *)(gd_r9 + 0x88) = *(uint32_t *)(gd_r9 + 0x88) + 1;
	*(volatile uint32_t *)(gd_r9 + 0x8c) = now;
	return ((uint64_t)*(volatile uint32_t *)(gd_r9 + 0x88) << 32) | now;
}

// === ub_udelay_raw === 原 FUN_04006244 @ 0x04006244
// 来源: reversed/uboot_funcs/FUN_04006244.c | 分类: uboot
// 语义: 微秒级忙等 (udelay): 目标时刻 = 当前刻度 + 1 + usec*freq/1000000
//       (freq = fun_03fb2008(), 每秒刻度数), 反复读刻度直到达到目标。
//       乘积 64 位: 高字为 0 时用 32 位除法, 否则经 ub_div64_rem (商写回)。
static void ub_udelay_raw(uint32_t usec)
{
	uint64_t now = ub_timer_read();
	uint64_t prod = (uint64_t)fun_03fb2008() * (uint64_t)usec;
	uint32_t delay_lo, delay_hi;
	uint64_t target;

	if ((uint32_t)(prod >> 32) == 0) {
		delay_hi = 0;
		delay_lo = (uint32_t)prod / 1000000;
	} else {
		uint32_t n[2];
		n[0] = (uint32_t)prod;
		n[1] = (uint32_t)(prod >> 32);
		ub_div64_rem(n, 1000000);
		delay_lo = n[0];
		delay_hi = n[1];
	}
	target = now + 1 + ((uint64_t)delay_hi << 32 | delay_lo);
	do {
		now = ub_timer_read();
	} while (now < target);
}

// === ub_put_dec === 原 FUN_04006520 @ 0x04006520
// 来源: reversed/uboot_funcs/FUN_04006520.c | 分类: uboot
// 语义: vsprintf put_dec (64 位十进制展开): num > 99999 (DAT_04006694:DAT_04006690)
//       时每次取 rem = num % 100000, 内联 put_dec_full 恒发射 5 位, num /= 100000;
//       剩余值 (<100000) 交 ub_put_dec_helper (put_dec_trunc, 1~5 位, 前导零抑制)。
//       返回发射结束位置 (number() 消费该值; 反编译呈 void 系 Ghidra 类型缺失)。
//       32 位快路径: num/100000 = (num>>5)/3125 (0xc35), 精确。
static char *ub_put_dec(char *buf, uint64_t num)
{
	uint32_t n[2];			/* 64 位值: n[0]=低, n[1]=高 (原 local_28/uStack_24 槽) */
	uint32_t rem;
	char *p = buf;

	n[0] = (uint32_t)num;
	n[1] = (uint32_t)(num >> 32);
	while ((n[1] > DAT_04006694) ||
	       (n[1] == DAT_04006694 && n[0] > DAT_04006690)) {
		if (n[1] == 0) {
			uint32_t q = (n[0] >> 5) / 0xc35;
			rem = n[0] - q * 100000;
			n[1] = 0;
			n[0] = q;
		} else {
			rem = ub_div64_rem(n, 100000);
		}
		/* put_dec_full 内联: 按 nibble 域 + Jones 修正恒发射 5 位十进制 */
		{
			uint32_t d3 = rem >> 12;
			uint32_t d2 = (rem & 0xfff) >> 8;
			uint32_t d1 = (rem & 0xff) >> 4;
			uint32_t s0 = (rem & 0xf) + (d1 + d2 + d3) * 6;
			uint32_t q0 = (s0 * 0xcd) >> 11;
			uint32_t s1 = d3 * 9 + d1 + d2 * 5 + q0;
			uint32_t q1 = (s1 * 0xcd) >> 11;
			uint32_t s2 = q1 + d2 * 2;
			uint32_t q2 = (s2 * 0xd) >> 7;
			uint32_t s3 = q2 + d3 * 4;
			uint32_t q3 = (s3 * 0xcd) >> 11;
			p[0] = (char)(s0 - q0 * 10 + '0');
			p[1] = (char)(s1 - q1 * 10 + '0');
			p[2] = (char)(s2 - q2 * 10 + '0');
			p[3] = (char)(s3 - q3 * 10 + '0');
			p[4] = (char)(q3 + '0');
			p += 5;
		}
	}
	return ub_put_dec_helper(p, n[0]);
}

// === ub_number_fmt === 原 FUN_04006698 @ 0x04006698
// 来源: reversed/uboot_funcs/FUN_04006698.c | 分类: uboot
// 语义: vsprintf number() — 64 位数值按 base (8/10/16) 格式化到 [buf,end):
//       符号 (SIGN/PLUS/SPACE)、前缀 "0x"/"0" (SPECIAL)、宽度/精度/ZEROPAD/LEFT
//       完整支持。标志位: ZEROPAD=1 SIGN=2 PLUS=4 SPACE=8 LEFT=0x10 SMALL=0x20
//       SPECIAL=0x40 (SMALL 位经 OR 0x20 将数字表转小写)。
//       反编译呈 void; 上游原型返回 buf (本固件调用方未消费返回值)。
static void ub_number_fmt(char *buf, const char *end, uint32_t num_lo,
                          uint32_t num_hi, uint32_t base, int32_t size,
                          int32_t precision, uint32_t type)
{
	uint8_t tmp[68];			/* 倒序数字缓冲 (原 local_64[68]) */
	uint32_t left = type & 0x10;		/* LEFT 标志 */
	uint8_t locase = (uint8_t)(type & 0x20);/* SMALL: 数字 OR 0x20 转小写 */
	int need_pfx = (base != 10) && ((type >> 6) & 1);	/* SPECIAL 且非十进制 */
	char sign;
	int32_t i;				/* 数字个数 (原 iVar4) */
	int32_t last;				/* 最高位下标 i-1 (原 iVar12) */
	int32_t k;				/* 填充剩余量 (原 iVar8) */
	char *p;

	if (left != 0)
		type &= ~1U;			/* LEFT 清除 ZEROPAD */

	if ((type & 0x2) == 0) {		/* SIGN 域: 计算符号并占位 */
		sign = 0;
	} else if ((int32_t)num_hi < 0) {	/* 64 位取负 */
		uint32_t carry = (num_lo != 0);
		num_lo = -num_lo;
		num_hi = -(num_hi + carry);
		size = size - 1;
		sign = '-';
	} else if ((type & 0x4) == 0) {		/* PLUS */
		if ((type & 0x8) == 0) {	/* SPACE */
			sign = 0;
		} else {
			size = size - 1;
			sign = ' ';
		}
	} else {
		size = size - 1;
		sign = '+';
	}

	if (need_pfx == 0) {
		if (num_lo != 0 || num_hi != 0) {
			if (base == 10) {
				char *e = ub_put_dec(tmp, (uint64_t)num_hi << 32 | num_lo);
				i = (int32_t)(e - (char *)tmp);
				last = i - 1;
				goto post;		/* 原 LAB_040067e4 */
			}
			goto shift_loop;		/* 原 LAB_04006738 */
		}
	} else if (base == 0x10) {
		size = size - 2;
		if (num_lo != 0 || num_hi != 0) {
			uint32_t mask = 0xf;
			uint32_t shift = 4;
			goto emit_digits;
		}
	} else {
		size = size - 1;
		if (num_lo != 0 || num_hi != 0)
			goto shift_loop;
	}
	/* num == 0: 单个 '0' */
	i = 1;
	last = 0;
	tmp[0] = 0x30;
	goto post;

shift_loop:					/* 原 LAB_04006738/04006754 */
	{
		uint32_t mask, shift;
		int32_t cnt;
		if (base == 0x10) {
			mask = 0xf;
			shift = 4;
		} else {
			mask = base - 1;
			shift = 3;
		}
		cnt = 0;
		for (;;) {
			uint32_t digit_val = num_lo & 0xff;
			uint32_t lo_shifted = (num_lo >> shift) | (num_hi << (32 - shift));
			tmp[cnt] = locase | DAT_040069ec[digit_val & mask];
			if (lo_shifted == 0 && (num_hi >> shift) == 0) {
				i = cnt + 1;
				last = cnt;
				break;
			}
			num_hi >>= shift;
			num_lo = lo_shifted;
			cnt++;
		}
	}
	goto post;

emit_digits:					/* 原 LAB_04006754 (十六进制直入) */
	{
		uint32_t mask = 0xf;
		uint32_t shift = 4;
		int32_t cnt = 0;
		for (;;) {
			uint32_t digit_val = num_lo & 0xff;
			uint32_t lo_shifted = (num_lo >> shift) | (num_hi << (32 - shift));
			tmp[cnt] = locase | DAT_040069ec[digit_val & mask];
			if (lo_shifted == 0 && (num_hi >> shift) == 0) {
				i = cnt + 1;
				last = cnt;
				break;
			}
			num_hi >>= shift;
			num_lo = lo_shifted;
			cnt++;
		}
	}

post:						/* 原 LAB_040067e4 */
	if (precision <= i)
		precision = i;
	size = size - precision;
	k = size - 1;
	p = buf;
	if ((type & 0x11) == 0) {		/* 前导空格 (!(ZEROPAD|LEFT)) */
		if (k < 0) {
			k = size - 2;
		} else {
			char *stop = p + size;
			while (p != stop) {
				if (p < end)
					*p = ' ';
				p++;
			}
			k = -2;
		}
	}
	if (sign != 0) {			/* 符号 (越界仍推进游标) */
		char *w = p + 1;
		if (p < end)
			*p = sign;
		p = w;
	}
	if (need_pfx != 0) {			/* 前缀 "0" / "0x" */
		if (p < end)
			*p = '0';
		p++;
		if (base == 0x10) {
			if (p < end)
				*p = (char)(locase | 0x58);	/* 'x'/'X' */
			p++;
		}
	}
	if (left == 0) {			/* 零填充/空格填充 (值前) */
		char pad = (type & 1) ? '0' : ' ';
		if (k < 0) {
			k = k - 1;
		} else {
			char *stop = p + k + 1;
			while (p != stop) {
				if (p < end)
					*p = pad;
				p++;
			}
			k = -2;
		}
	}
	if (i < precision) {			/* 精度补零 */
		char *stop = p + (precision - i);
		while (p != stop) {
			if (p < end)
				*p = '0';
			p++;
		}
	}
	if (last >= 0) {			/* 数字本体 (倒序发射) */
		int32_t j;
		for (j = last; j >= 0; j--) {
			if (p < end)
				*p = (char)tmp[j];
			p++;
		}
	}
	if (k >= 0) {				/* LEFT 后置空格 */
		char *stop = p + k + 1;
		while (p != stop) {
			if (p < end)
				*p = ' ';
			p++;
		}
	}
}

// === ub_parse_hex_u32 === 原 FUN_04007678 @ 0x04007678
// 来源: reversed/uboot_funcs/FUN_04007678.c | 分类: uboot
// 语义: 解析 16 进制 32 位值 (simple_strtoul(str,&end,16)); 串非空且解析止于
//       串尾 (endp 指向 NUL) 时返回真, 否则假。结果经 *out 传回。
static int ub_parse_hex_u32(const char *str, uint32_t *out)
{
	char *endp;
	uint32_t value;

	value = simple_strtoul(str, &endp, 0x10);
	*out = value;
	if (*str == '\0')
		return 0;
	return *endp == '\0';
}

/* ============================================================================
 * 覆盖率表 (batch=uboot_b2 按地址升序前 24 函数, 24/24)
 * 行号为本文件横幅所在行; 上游参照列为语义核实依据。
 * ==========================================================================
 *
 * | #  | 语义名                | 原地址      | 行   | 上游对应 (U-Boot v2016.07)          | 核实方式          |
 * |----|----------------------|-------------|------|-------------------------------------|-------------------|
 * | 1  | ub_sha256_final      | 0x040029f4  | 125  | lib/sha256.c sha256_finish          | ctx 布局+算法比对 |
 * | 2  | ub_un_02d018         | 0x04002d18  | 165  | (回退名, 外部查表+strcmp)           | 原样转写          |
 * | 3  | ub_un_02f050         | 0x04002f50  | 178  | (回退名, BE 数组转 LE 拷贝)         | 原样转写          |
 * | 4  | ub_hdelete_entry     | 0x040030a8  | 200  | lib/hashtable.c _hdelete            | 上游逐行比对      |
 * | 5  | ub_match_filter      | 0x04003210  | 216  | lib/hashtable.c match_entry         | 上游逐行比对      |
 * | 6  | ub_hdestroy          | 0x0400339c  | 246  | lib/hashtable.c hdestroy_r          | 上游逐行比对      |
 * | 7  | ub_stub_printf0      | 0x0400428c  | 271  | (打印存根)                          | 原样转写          |
 * | 8  | ub_crc32_le          | 0x0400461c  | 280  | lib/crc32.c crc32 核心              | 反汇编逐指令核实  |
 * | 9  | ub_crc32_store       | 0x04004714  | 314  | crc32 + 大端 4 字节存储             | 反编译+反汇编     |
 * | 10 | ub_div0_hang         | 0x04004844  | 328  | __div0 (打印后挂死)                 | 原样转写          |
 * | 11 | ub_memalign32        | 0x04004860  | 341  | memalign(32)+可选清零               | 原样转写          |
 * | 12 | ub_regex_node_setlen | 0x04005170  | 354  | lib/slre.c fixup_branch             | 上游逐行比对      |
 * | 13 | ub_regex_parse       | 0x040051b8  | 379  | lib/slre.c compile()                | 反汇编逐指令核实  |
 * | 14 | ub_regex_compile     | 0x04005718  | 660  | lib/slre.c slre_compile             | 反汇编逐指令核实  |
 * | 15 | ub_strncpy           | 0x040059ac  | 719  | lib/string.c strncpy (无零填充)     | 原样转写          |
 * | 16 | ub_strcmp            | 0x04005a60  | 733  | strcmp (返回值含 sxtb 语义)         | 反汇编核实        |
 * | 17 | ub_strncmp           | 0x04005a90  | 752  | strncmp (返回值含 sxtb 语义)        | 反汇编核实        |
 * | 18 | ub_strrchr           | 0x04005b44  | 780  | strrchr (含 NUL 位)                 | 原样转写          |
 * | 19 | ub_strstr            | 0x04006044  | 804  | strstr (窗口回退式)                 | 原样转写          |
 * | 20 | ub_timer_read        | 0x040061b0  | 858  | get_ticks (TTC 递减计数, gd 经 r9)  | 反汇编核实        |
 * | 21 | ub_udelay_raw        | 0x04006244  | 873  | __udelay (64 位乘除 + 忙等)         | 原样转写          |
 * | 22 | ub_put_dec           | 0x04006520  | 902  | lib/vsprintf.c put_dec              | 上游逐行比对      |
 * | 23 | ub_number_fmt        | 0x04006698  | 951  | lib/vsprintf.c number()             | 反汇编逐指令核实  |
 * | 24 | ub_parse_hex_u32     | 0x04007678  | 1146 | simple_strtoul 包装                 | 原样转写          |
 *
 * 注: 函数 8 的反编译产物丢失了循环体运算 (空循环/void 返回), 以 0x0400461c-
 *     0x04004714 机器码为准恢复; 13/23 亦经反汇编逐指令核对控制流与寄存器语义。
 *     name_map 曾将 13/14 记为同名 ub_regex_compile, 已按 p3a 修订消歧为
 *     ub_regex_parse (slre.c 静态 compile) / ub_regex_compile (slre_compile)。
 */
