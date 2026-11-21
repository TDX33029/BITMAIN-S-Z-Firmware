/**
 * fileparser.c — FileParser (/usr/bin/FileParser) 语义化转写
 *
 * 原始模块 : BM1746 rootfs /usr/bin/FileParser
 *            (ARM EABI5 动态链接 ELF, 入口 0x10839; 链接 libcrypto/OpenSSL)
 * 功能定位 : 固件包解析/烧写 CLI — RSA 签名验证 (PEM RSA 公钥 + SHA256)
 *            1) BMU ('&' 魔数) 固件包校验/拆包: -f / -s / -p / -x
 *            2) sigimg nand 镜像按分区表拆分:   -n
 *            3) 生成 256 字节 0xff 文件:        -q
 * 命名事实 : rewritten/name_map/fileparser.tsv (42 函数全命名)
 * 覆盖     : 42 个函数, 见文件末尾覆盖率表
 *
 * ============================ 用法 (字符串原文) ============================
 *   Useage:\r
 *   \t%s [option] [paramaters]\n\n
 *   \tOption:{-f} {-s} {-p} {-n} {-x} {-q}\n
 *   \t\t-f [minerType] [fileName] [rootPublicKeyFile]:
 *   \t\t\tOnly Check If Filename was Valided.\n
 *   \t\t-s [minerType] [fileName] [rootPublicKeyFile]:
 *   \t\t\tCheck If Filename Was Valided, and Splite Bmu To "/tmp/tmpfw/"
 *   \t\t-p [minerType] [fileName] [rootPublicKeyFile]:
 *   \t\t\tCheck If Filename Was Valided, and Dump BmuComments
 *   \t\t-x [minerType] [fileName] [rootPublicKeyFile]:
 *   \t\t\tCheck If Filename Was FullSize BMU
 *   \t\t-n [nandBinFile]: Splite SigImg To Single File To "/tmp/tmpNand/"
 *   \t\t-q: Generate A 256Bytes 0xff File To "/tmp/256BFF"
 *   \tReturns: 0: Well Done! / Others: Something Wrong!
 * 选项实现为 switch 逐级直落: 'x'→'p'→'s'→'f' (见 main)。
 *
 * ============================ 返回码汇总 ============================
 *   main:            1=无参数 2=选项未以'-'开头 3=文件名超长/参数错 4=pem名超长
 *                    5=命令不支持
 *   fp_gen_256bff:   1=建文件失败
 *   fp_split_sigimg: 1=nand 镜像打开失败 2=输出文件创建失败
 *   fp_process_bmu:  6=文件<0x800 7=打开失败 8=非 Btmu 文件 9=content 位图不匹配
 *                    10=总长不匹配 11=根公钥打开失败 12=根公钥读取失败
 *                    13=miner.pem 验签失败 14=非整包 15/16=miner.pem(.sig) 落盘失败
 *                    17=miner.pem 公钥加载失败 18=包尾签名验签失败 30=miner_type 不匹配
 *
 * ============================ BMU 包头布局 (反编译实测) ============================
 *   +0x000 : 1B  '&' 魔数
 *   +0x002 : 8B  miner_type 的 64 位哈希 (fp_cityhash64, 小端存储)
 *   +0x00b : 2B  content 位图 (大端) — bit i 置位表示含第 i 类成员文件
 *   +0x00d : 8B  fw 版本 (原样写入 /usr/bin/fw_version)
 *   +0x016 : 2B  miner.pem 长度 (大端)
 *   +0x018 : miner.pem 数据 (PEM 文本; 公钥加载读窗固定 0x400 字节)
 *   +0x418 : 0x100 B RSA-2048 签名 — 根公钥 (命令行 pemName) 验
 *            SHA256( miner.pem[0 .. pem_len) )
 *   +0x518 : 1B  成员文件数 N
 *   +0x519 : 4B  大端总长 (仅用于错误提示 "FileSize Should Be")
 *   +0x51d : N*5B 成员表: type(1B) + size(4B 大端); type→文件名见 fp_extract_member
 *   文件尾 : 最后 0x100 B = RSA-2048 签名 — miner.pem 公钥 (包头内嵌) 验
 *            SHA256( 0x00*32 ‖ SHA256(成员 i)×N ‖ SHA256(成员 i 的 .sig)×N )
 *
 * ============================ 验证流程 (fp_process_bmu) ============================
 *   1. stat 文件大小 ≥ 0x800; fopen "rb"; 读 0x800 头; 校验 '&' 魔数
 *   2. fp_cityhash64(miner_type) == 包头 +0x002 处 8 字节
 *   3. content 位图置位个数 == 包头 +0x518
 *   4. (count+9)*0x100 + Σ 成员 size == 实际文件大小
 *   5. fp_verify_sig_sha256: 根公钥验 +0x418 签名 (对 miner.pem)
 *   6. 写 /usr/bin/fw_version; (可选 -s/-x) 落盘 /tmp/tmpfw/miner.pem(.sig)
 *   7. 顺序抽取各成员与 .sig (边抽边 SHA256) → 成员哈希区 [0]=0, [1..N], [N+1..2N]
 *   8. fseek(-0x100, SEEK_END) 读包尾签名; SHA256(哈希区) 用 miner.pem 公钥 RSA_verify
 *
 * 说明: 本文件为语义转写产物, 不独立编译; 字符串字面量/魔数一律原样保留。
 */

#include "../include/common_types.h"

/* ==================== 本文件常量 ==================== */

/* BMU 包头偏移/尺寸 (来源: fp_process_bmu 反编译实测) */
#define BMU_HDR_MAGIC        0x000u  /* '&' 魔数字节 */
#define BMU_HDR_TYPE_HASH    0x002u  /* 8B miner_type 哈希 */
#define BMU_HDR_CONTENT      0x00bu  /* 2B content 位图 (大端) */
#define BMU_HDR_FW_VERSION   0x00du  /* 8B fw 版本 */
#define BMU_HDR_PEM_LEN      0x016u  /* 2B miner.pem 长度 (大端) */
#define BMU_HDR_PEM          0x018u  /* miner.pem 数据区起点 */
#define BMU_PEM_READ_WINDOW  0x400u  /* 公钥 PEM 加载窗口 (BIO_new_mem_buf/落盘读长) */
#define BMU_HDR_SIG          0x418u  /* miner.pem 签名区 */
#define BMU_HDR_FILE_COUNT   0x518u  /* 1B 成员文件数 */
#define BMU_HDR_TOTAL_SIZE   0x519u  /* 4B 大端总长 (仅提示用) */
#define BMU_HDR_FILE_TABLE   0x51du  /* 成员表: type(1B) + size(4B 大端) */
#define BMU_FILE_ENTRY_SIZE  5u      /* 成员表每项字节数 */
#define BMU_HEADER_SIZE      0x800u  /* 头部整体大小 (整包哈希覆盖范围) */
#define BMU_COMMENT_OFFSET   0x550u  /* 包注释 (puts 打印) */

/* OpenSSL 常量 */
#define FP_NID_SHA256        0x2a0   /* NID_sha256 = 672 (RSA_verify type 参数) */
#define FP_SHA256_DIGEST_LEN 0x20u   /* SHA256 摘要长度 */
#define FP_RSA_SIG_LEN       0x100u  /* RSA-2048 签名字节数 (256B) */
#define FP_SEEK_END          2       /* fseek whence = SEEK_END */

/* 64 位哈希乘子 (farmhash/CityHash 家族 k0/k1/k2, 反编译乘数常量确证) */
#define FP_K0 0xc3a5c85c97cb3127ULL  /* k0 */
#define FP_K1 0xb492b66fbe98f273ULL  /* k1 */
#define FP_K2 0x9ae16a3b2f90404fULL  /* k2 */
#define FP_CITYHASH_SEED 0x51ULL     /* 81 — farmhash na Hash64 固定 seed */

/* 其它 */
#define FP_TMPFW_DIR      "/tmp/tmpfw/"   /* -s 拆包输出目录 */
#define FP_IO_CHUNK       0x400u          /* 成员抽取 IO 块大小 */
#define FP_NAME_BUF_LEN   0x80u           /* fp_extract_member 路径缓冲 */
#define FP_COMMENT_LEN    0x100u          /* BMU 包注释长度 */

/* ==================== 类型还原 ==================== */

/* OpenSSL 1.0.x SHA256_CTX: h[8]+Nl+Nh+data[16]+num+md_len = 112 字节 (栈窗实测) */
typedef struct { uint8_t opaque[112]; } fp_sha256_ctx_t;

/* sigimg 分区表项 (fp_split_sigimg, 步长 0x84): 大小字段基址 = 原 DAT_0002507c,
 * 名字数组基址 = 0x25080 (即 +4)。ZEC 反编译版 (MD5 f4d8cabd…) 共 15 项;
 * BM1746/rootfs_2 同构副本表基址 0x2307c, 实测内容:
 *   #0  0x100  /tmp/tmpNand/devicetree.dtb.sig    #8  0x100 /tmp/tmpNand/miner.btm.sig
 *   #1  0x100  /tmp/unused                        #9  0x100 /tmp/unused
 *   #2  0x100  /tmp/tmpNand/uImage.sig            #10 0x100 /tmp/tmpNand/crl.sig
 *   #3  0x100  /tmp/unused                        #11 0x100 /tmp/unused
 *   #4  0x100  /tmp/tmpNand/minerfs.image.gz.sig  #12 0x1c00 /tmp/tmpNand/7kreserve
 *   #5  0x100  /tmp/unused                        #13 0x800  /tmp/tmpNand/miner.btm
 *   #6  0x100  /tmp/tmpNand/update.image.gz.sig   #14 0x2800 /tmp/tmpNand/crl
 *   #7  0x100  /tmp/unused
 * (BM1746/rootfs_1 为旧布局: 10 项, 表基址 0x24f10, 首项 /tmp/unused) */
typedef struct {
    uint32_t size;       /* 从 nand 镜像顺序读出并写入对应文件的字节数 */
    char     name[0x80]; /* 输出路径 */
} fp_sigimg_entry_t;
#define FP_SIGIMG_TABLE ((const fp_sigimg_entry_t *)0x0002507c) /* 原 DAT_0002507c */

/* struct stat64 — 本工具只读 st_size。原码在缓冲 +0x30 处取 32 位大小
 * (帧 -0xa0 处), 故此处按相同相对偏移建模; 其余字段本工具不读。 */
typedef struct {
    uint8_t  head[0x30];  /* stat64 其余字段 (dev/ino/mode/nlink/uid/gid/rdev/…) */
    uint32_t st_size;     /* 文件大小 (32 位; st_size 低字) */
} fp_stat64_t;

/* ==================== 导入函数 (PLT 桩, 以真实名调用) ====================
 * 0x10708-0x1082c 的 20 个 PLT 桩 = 真实导入函数 (命名阶段确证), 见各横幅;
 * 另有未导出 PLT 桩 (反编译以 thunk 占位): fclose / memset /
 * RSA_verify / __xstat64(0x107e4) / mcount(0x1072c) / __gmon_start__,
 * 同样按真实名直接调用。桩体均为 "按 GOT 槽间接跳转", C 侧用导入原型表示。 */

/* ---- libc ---- */
void    abort(void);                                   /* GOT 0x2500c */
unsigned long err_get_error(void);                     /* GOT 0x25010 (ERR_get_error) */
void    __libc_start_main(int (*main)(int, char **), int argc, char **argv,
                          void (*init)(int, char **, char **), void (*fini)(void),
                          void (*rtld_fini)(void *), void *stack_end); /* GOT 0x25014 */
int     fseek(void *stream, long offset, int whence);  /* GOT 0x25028 */
void   *pem_read_bio_rsa_pubkey(void *bp, char **x, void *cb, void *u); /* GOT 0x2502c (PEM_read_bio_RSAPublicKey) */
int     sha256_final(uint8_t *md, fp_sha256_ctx_t *ctx);              /* GOT 0x25030 (SHA256_Final) */
size_t  strlen(const char *s);                         /* GOT 0x25034 */
void   *memcpy(void *dst, const void *src, size_t n);  /* GOT 0x25038 */
void   *fopen64(const char *path, const char *mode);   /* GOT 0x2503c (glibc fopen64 == fopen) */
void   *bio_new_mem_buf(const void *buf, int len);     /* GOT 0x25040 (BIO_new_mem_buf) */
char   *strcpy(char *dst, const char *src);            /* GOT 0x25044 */
int     printf(const char *fmt, ...);                  /* GOT 0x25048 */
char   *err_error_string(unsigned long e, char *buf);  /* GOT 0x2504c (ERR_error_string) */
size_t  fwrite(const void *ptr, size_t size, size_t nmemb, void *stream); /* GOT 0x25050 */
int     puts(const char *s);                           /* GOT 0x25058 */
void    rsa_free(void *rsa);                           /* GOT 0x2505c (RSA_free) */
int     sha256_update(fp_sha256_ctx_t *ctx, const void *data, size_t len); /* GOT 0x25060 (SHA256_Update) */
int     bio_free(void *bio);                           /* GOT 0x25064 (BIO_free) */
int     sha256_init(fp_sha256_ctx_t *ctx);             /* GOT 0x25068 (SHA256_Init) */
size_t  fread(void *ptr, size_t size, size_t nmemb, void *stream);        /* GOT 0x2506c */
/* ---- 未导出 PLT 桩 ---- */
int     fclose(void *stream);                          /* 反编译 thunk_EXT_FUN_706d742e */
void   *memset(void *dst, int c, size_t n);            /* 反编译 thunk_EXT_FUN_756e752e */
int     RSA_verify(int type, const unsigned char *m, unsigned int m_len,
                   const unsigned char *sigbuf, unsigned int siglen, void *rsa); /* thunk_EXT_FUN_00646572 */
int     __xstat64(int ver, const char *path, void *stat_buf);  /* 0x107e4 @plt */
void    mcount(void);                                  /* 0x1072c @plt, gprof 探针 (GOT 0x25070) */
void    __gmon_start__(void);                          /* 弱符号, gprof 启动钩子 */
char   *strncpy(char *dst, const char *src, size_t n); /* 编译器内联内建 (非独立 PLT 调用) */

/* __libc_csu_fini: csu 尾段 (0x13c28, "bx lr" 式空桩), 仅作 entry 实参占位 */
void __libc_csu_fini(void);

/* ==================== 本文件函数前向声明 ==================== */
int      main(int argc, char **argv);
void     fp_print_usage(const char *prog);
uint32_t fp_split_sigimg(const char *nand_bin);
uint32_t fp_gen_256bff(void);
uint32_t fp_verify_sig_sha256(const void *pem_data, uint32_t pem_len,
                              const uint8_t *sig, const void *root_pem_buf);
uint32_t fp_extract_member(void *f_src, uint32_t size, uint32_t *hash_out,
                           uint8_t file_type, char is_sig, char dump);
uint32_t fp_cityhash_fetch32(const void *p);
uint64_t fp_cityhash_fetch64(const void *p);
uint64_t fp_cityhash_shiftmix(uint64_t val);
uint64_t fp_cityhash_rotate64(uint64_t val, uint32_t shift);
uint64_t fp_cityhash_hash_len16(uint64_t u, uint64_t v, uint64_t mul);
uint64_t fp_cityhash_hash_len0to16(const uint8_t *s, uint32_t len);
uint64_t fp_cityhash_hash_len17to32(const char *s, uint32_t len);
uint64_t fp_cityhash_hash_len33to64(const char *s, uint32_t len);
void     fp_cityhash_weak_len32_mix(uint64_t out[2], uint64_t w, uint64_t x, uint64_t y,
                                    uint64_t z, uint64_t a, uint64_t b);
void     fp_cityhash_weak_len32(uint64_t out[2], const char *s, uint64_t a, uint64_t b);
uint64_t fp_cityhash64(const char *s, uint32_t len);
uint32_t fp_process_bmu(const char *bmu_path, const char *miner_type,
                        const char *root_pem_path, uint8_t *buf,
                        char flag_dump_pem, char flag_print_comment, char flag_full_check);
void     __libc_csu_init(int argc, char **argv, char **envp);
void     fp_stat_file(const char *path, fp_stat64_t *stat_buf);

/* ==================== 1. crt / 进程骨架 ==================== */

// === _init === 原 FUN_000106e8 @ 0x000106e8
// 来源: reversed/fileparser_funcs/FUN_000106e8.c | 分类: crt
// 语义: .init 段入口 (crtbegin)。反编译报指令混叠 (目标落入数据/桩区) 无法逐句还原;
//       机器码实测: push {r3,lr}; bl 0x10868; pop {r3,pc} — 0x10868 为 gprof 探针桩:
//       r2 = GOT[0x25070] (mcount 槽), 为 0 直接返回, 否则尾调 mcount@plt (0x1072c)。
//       即本 _init 仅做 profiling 挂钩探测, 无其他初始化语义。
void _init(void)
{
	if (0 != ((void **)0x25070)[0]) { /* GOT mcount 槽, 运行期由 gprof 填写 */
		mcount();                     /* 原 0x1072c @plt 尾调 */
	}
}

// === abort === 原 FUN_00010708 @ 0x00010708
// 来源: reversed/fileparser_funcs/FUN_00010708.c | 分类: PLT
// 语义: PLT 桩 → GOT[0x2500c]; 真实导入 <stdlib.h> abort — 异常终止进程
//       (_start 中 __libc_start_main 若返回则调用)。
void abort(void);

// === err_get_error === 原 FUN_00010714 @ 0x00010714
// 来源: reversed/fileparser_funcs/FUN_00010714.c | 分类: PLT
// 语义: PLT 桩 → GOT[0x25010]; OpenSSL ERR_get_error — 取线程错误队列最早错误码
//       (随后交 err_error_string 转成文本打印 "OpenSSL error: %s")。
unsigned long err_get_error(void);

// === __libc_start_main === 原 FUN_00010720 @ 0x00010720
// 来源: reversed/fileparser_funcs/FUN_00010720.c | 分类: PLT
// 语义: PLT 桩 → GOT[0x25014]; glibc __libc_start_main — 初始化 libc/线程/exit 链
//       后调用 main(argc, argv, envp), 返回值交 exit。
void __libc_start_main(int (*main)(int, char **), int argc, char **argv,
                       void (*init)(int, char **, char **), void (*fini)(void),
                       void (*rtld_fini)(void *), void *stack_end);

// === fseek === 原 FUN_0001075c @ 0x0001075c
// 来源: reversed/fileparser_funcs/FUN_0001075c.c | 分类: PLT
// 语义: PLT 桩 → GOT[0x25028]; fseek — 移动文件读写位置
//       (fp_process_bmu 中 fseek(-0x100, SEEK_END) 读包尾 RSA 签名)。
int fseek(void *stream, long offset, int whence);

// === pem_read_bio_rsa_pubkey === 原 FUN_00010768 @ 0x00010768
// 来源: reversed/fileparser_funcs/FUN_00010768.c | 分类: PLT
// 语义: PLT 桩 → GOT[0x2502c]; OpenSSL PEM_read_bio_RSAPublicKey — 从 BIO 解析
//       PEM "BEGIN RSA PUBLIC KEY" 文本为 RSA 结构 (失败返回 NULL 并压错误队列)。
void *pem_read_bio_rsa_pubkey(void *bp, char **x, void *cb, void *u);

// === sha256_final === 原 FUN_00010774 @ 0x00010774
// 来源: reversed/fileparser_funcs/FUN_00010774.c | 分类: PLT
// 语义: PLT 桩 → GOT[0x25030]; OpenSSL SHA256_Final — 结束摘要计算, 输出 32 字节
//       哈希到 md (调用序: SHA256_Final(md, ctx))。
int sha256_final(uint8_t *md, fp_sha256_ctx_t *ctx);

// === strlen === 原 FUN_00010780 @ 0x00010780
// 来源: reversed/fileparser_funcs/FUN_00010780.c | 分类: PLT
// 语义: PLT 桩 → GOT[0x25034]; strlen — 串长 (miner_type 哈希输入/路径拼接/参数长度检查)。
size_t strlen(const char *s);

// === memcpy === 原 FUN_0001078c @ 0x0001078c
// 来源: reversed/fileparser_funcs/FUN_0001078c.c | 分类: PLT
// 语义: PLT 桩 → GOT[0x25038]; memcpy — 复制内存 (包头哈希取 8B / 包注释取 0x100B /
//       fp_cityhash_fetch64 的非对齐 8B 读)。
void *memcpy(void *dst, const void *src, size_t n);

// === fopen64 === 原 FUN_00010798 @ 0x00010798
// 来源: reversed/fileparser_funcs/FUN_00010798.c | 分类: PLT
// 语义: PLT 桩 → GOT[0x2503c]; glibc fopen64 — 打开文件 (大文件偏移版 fopen)。
//       本工具模式串: "rb" (0x13f00) / "wb" (0x13f24) / "r" (0x14290) / "w" (0x142dc)。
void *fopen64(const char *path, const char *mode);

// === bio_new_mem_buf === 原 FUN_000107a4 @ 0x000107a4
// 来源: reversed/fileparser_funcs/FUN_000107a4.c | 分类: PLT
// 语义: PLT 桩 → GOT[0x25040]; OpenSSL BIO_new_mem_buf — 把内存缓冲包装为只读 BIO
//       (喂给 pem_read_bio_rsa_pubkey 解析公钥)。
void *bio_new_mem_buf(const void *buf, int len);

// === strcpy === 原 FUN_000107b0 @ 0x000107b0
// 来源: reversed/fileparser_funcs/FUN_000107b0.c | 分类: PLT
// 语义: PLT 桩 → GOT[0x25044]; strcpy — 复制命令行参数到本地缓冲 (main)。
char *strcpy(char *dst, const char *src);

// === printf === 原 FUN_000107bc @ 0x000107bc
// 来源: reversed/fileparser_funcs/FUN_000107bc.c | 分类: PLT
// 语义: PLT 桩 → GOT[0x25048]; printf — 带格式日志输出 (stderr/stdout 不区分)。
int printf(const char *fmt, ...);

// === err_error_string === 原 FUN_000107c8 @ 0x000107c8
// 来源: reversed/fileparser_funcs/FUN_000107c8.c | 分类: PLT
// 语义: PLT 桩 → GOT[0x2504c]; OpenSSL ERR_error_string — 错误码转文本 (buf=NULL
//       时返回静态缓冲), 配合 "OpenSSL error: %s" 打印。
char *err_error_string(unsigned long e, char *buf);

// === fwrite === 原 FUN_000107d4 @ 0x000107d4
// 来源: reversed/fileparser_funcs/FUN_000107d4.c | 分类: PLT
// 语义: PLT 桩 → GOT[0x25050]; fwrite — 写文件 (fw 版本/miner.pem/成员抽取落盘)。
size_t fwrite(const void *ptr, size_t size, size_t nmemb, void *stream);

// === puts === 原 FUN_000107f0 @ 0x000107f0
// 来源: reversed/fileparser_funcs/FUN_000107f0.c | 分类: PLT
// 语义: PLT 桩 → GOT[0x25058]; puts — 无格式行输出 (用法/状态提示)。
int puts(const char *s);

// === rsa_free === 原 FUN_000107fc @ 0x000107fc
// 来源: reversed/fileparser_funcs/FUN_000107fc.c | 分类: PLT
// 语义: PLT 桩 → GOT[0x2505c]; OpenSSL RSA_free — 释放 RSA 结构。
void rsa_free(void *rsa);

// === sha256_update === 原 FUN_00010808 @ 0x00010808
// 来源: reversed/fileparser_funcs/FUN_00010808.c | 分类: PLT
// 语义: PLT 桩 → GOT[0x25060]; OpenSSL SHA256_Update — 喂入待摘要数据。
int sha256_update(fp_sha256_ctx_t *ctx, const void *data, size_t len);

// === bio_free === 原 FUN_00010814 @ 0x00010814
// 来源: reversed/fileparser_funcs/FUN_00010814.c | 分类: PLT
// 语义: PLT 桩 → GOT[0x25064]; OpenSSL BIO_free — 释放 BIO (公钥解析后)。
int bio_free(void *bio);

// === sha256_init === 原 FUN_00010820 @ 0x00010820
// 来源: reversed/fileparser_funcs/FUN_00010820.c | 分类: PLT
// 语义: PLT 桩 → GOT[0x25068]; OpenSSL SHA256_Init — 初始化摘要上下文。
int sha256_init(fp_sha256_ctx_t *ctx);

// === fread === 原 FUN_0001082c @ 0x0001082c
// 来源: reversed/fileparser_funcs/FUN_0001082c.c | 分类: PLT
// 语义: PLT 桩 → GOT[0x2506c]; fread — 读文件 (BMU 头 0x800 / 成员抽取 / 包尾签名)。
size_t fread(void *ptr, size_t size, size_t nmemb, void *stream);

// === entry === 原 FUN_00010838 @ 0x00010838
// 来源: reversed/fileparser_funcs/entry.c | 分类: crt
// 语义: ELF 入口 _start (ARM glibc)。反编译体因代码与字面池混叠报坏指令数据
//       (halt_baddata 截断), 无法逐句还原; 机器码 (0x10838-0x1085b) 实测语义:
//         fp = lr = 0; pop {r1} 取 argc; r2 = sp 即 argv; push {argv} 占 stack_end;
//         push {r0} (动态链接器传入的 rtld_fini); 0x13c29(csu_fini) 入栈;
//         r0 = 0x13808 (main), r3 = 0x13bed (__libc_csu_init) — 三个字面量
//         已在 0x1085c-0x10864 字面池核实; BLX 0x10720 = __libc_start_main;
//         紧随 BLX 0x10708 = abort (仅当 __libc_start_main 返回, 异常路径)。
//       argc/argv/rtld_fini/stack_end 来自进程入口栈与寄存器, C 形参无法直接表达,
//       以下以占位实参表达调用形状。
void entry(void)
{
	/* 原反编译调用形状: __libc_start_main(main, argc, argv, 0x13bed, 0x13c29, rtld_fini) */
	__libc_start_main(main, 0 /*argc: 内核栈顶*/, (char **)0 /*argv: sp+4*/,
	                  __libc_csu_init /*0x13bed*/, __libc_csu_fini /*0x13c29*/,
	                  (void (*)(void *))0 /*rtld_fini: 动态链接器传入*/, (void *)0 /*stack_end*/);
	abort(); /* __libc_start_main 不应返回 */
}

/* ==================== 2. CLI 子命令 ==================== */

// === fp_print_usage === 原 FUN_000108f8 @ 0x000108f8
// 来源: reversed/fileparser_funcs/FUN_000108f8.c | 分类: cli
// 语义: 打印用法与返回码说明 (字符串原文, 含逐项 -f/-s/-p/-x/-n/-q 说明)。
//       无副作用, 不返回状态。
void fp_print_usage(const char *prog)
{
	puts("Useage:\r");
	printf("\t%s [option] [paramaters]\n\n", prog);
	puts("\tOption:{-f} {-s} {-p} {-n} {-x} {-q}\n");
	puts("\t\t-f [minerType] [fileName] [rootPublicKeyFile]: \n\t\t\tOnly Check If Filename was Valided.\n");
	puts("\t\t-s [minerType] [fileName] [rootPublicKeyFile]: \n\t\t\tCheck If Filename Was Valided, and Splite Bmu To \"/tmp/tmpfw/\"\n");
	puts("\t\t-p [minerType] [fileName] [rootPublicKeyFile]: \n\t\t\tCheck If Filename Was Valided, and Dump BmuComments\n");
	puts("\t\t-x [minerType] [fileName] [rootPublicKeyFile]: \n\t\t\tCheck If Filename Was FullSize BMU\n");
	puts("\t\t-n [nandBinFile]: \n\t\t\tSplite SigImg To Single File To \"/tmp/tmpNand/\"\n");
	puts("\t\t-q: \n\t\t\tGenerate A 256Bytes 0xff File To \"/tmp/256BFF\"\n");
	puts("\tReturns:");
	puts("\t\t0: \n\t\t\tWell Done!\n");
	puts("\t\tOthers: \n\t\t\tSomething Wrong!\n");
}

// === fp_split_sigimg === 原 FUN_000109a8 @ 0x000109a8
// 来源: reversed/fileparser_funcs/FUN_000109a8.c | 分类: cli
// 语义: -n 子命令 — 顺序读 nand 镜像, 按 sigimg 分区表 (FP_SIGIMG_TABLE, 15 项)
//       逐项 fread size 字节并 fwrite 到表项同名文件 ("wb"), 目录 /tmp/tmpNand/。
//       返回: 0=完成 1=镜像打开失败 2=输出文件创建失败 (此时已关镜像)。
uint32_t fp_split_sigimg(const char *nand_bin)
{
	uint32_t head;        /* 原帧 -0x8014: IO 缓冲首 4 字节 */
	uint8_t  tail[32764]; /* 原帧 -0x8010 */
	void    *f_in;        /* nand 镜像 */
	void    *f_out;       /* 输出文件 */
	uint32_t entry_size;
	int      i;

	head = 0;
	memset(tail, 0, 0x7ffc);
	f_in = fopen64(nand_bin, "rb");
	if (f_in == 0) {
		puts("Load Nand Image File Failed!");
		return 1;
	}
	for (i = 0; i < 0xf; i++) {
		entry_size = FP_SIGIMG_TABLE[i].size; /* 原 *(u32*)(0x2507c + i*0x84) */
		fread(&head, entry_size, 1, f_in);
		f_out = fopen64(FP_SIGIMG_TABLE[i].name, "wb"); /* 原 0x25080 + i*0x84 */
		if (f_out == 0) {
			printf("Try To Write To File \'%s\' Failed!\n", FP_SIGIMG_TABLE[i].name);
			if (f_in != 0) {
				fclose(f_in);
			}
			return 2;
		}
		fwrite(&head, entry_size, 1, f_out);
		if (f_out != 0) {
			fclose(f_out);
		}
	}
	if (f_in != 0) {
		fclose(f_in);
	}
	return 0;
}

// === fp_gen_256bff === 原 FUN_00010b9c @ 0x00010b9c
// 来源: reversed/fileparser_funcs/FUN_00010b9c.c | 分类: cli
// 语义: -q 子命令 — 生成 /tmp/256BFF: 256 字节全 0xff (烧写/擦除占位文件)。
//       返回: 0=完成 1=建文件失败。
uint32_t fp_gen_256bff(void)
{
	uint32_t head;    /* 原帧 -0x110: 256B 块首 4 字节 */
	uint8_t  tail[252]; /* 原帧 -0x10c */
	void    *f_out;
	int      i;

	f_out = fopen64("/tmp/256BFF", "wb");
	if (f_out == 0) {
		puts("GenFile Failed!");
		return 1;
	}
	head = 0;
	memset(tail, 0, 0xfc);
	for (i = 0; i < 0x100; i++) {
		/* 原码 tail[i-4] 下标: 自 head 起连填 256 字节 0xff */
		((uint8_t *)&head)[i] = 0xff;
	}
	fwrite(&head, 0x100, 1, f_out);
	fclose(f_out);
	return 0;
}

// === fp_verify_sig_sha256 === 原 FUN_00010c70 @ 0x00010c70
// 来源: reversed/fileparser_funcs/FUN_00010c70.c | 分类: crypto
// 语义: BMU 包头内嵌签名验证 — 用根公钥 PEM (root_pem_buf, 读窗 0x400) 验
//       RSA-2048 签名 sig (0x100B) 对数据 pem[0..pem_len) 的 SHA256 摘要。
//       流程: BIO_new_mem_buf → PEM_read_bio_RSAPublicKey → SHA256 全三段 →
//       RSA_verify(NID_sha256=0x2a0, hash, 0x20, sig, 0x100, rsa) → RSA_free/BIO_free。
//       返回: 0=验签通过 0xc=根公钥解析失败 0xd=验签失败。
uint32_t fp_verify_sig_sha256(const void *pem_data, uint32_t pem_len,
                              const uint8_t *sig, const void *root_pem_buf)
{
	uint8_t         hash[FP_SHA256_DIGEST_LEN]; /* 原帧 -0x34 */
	fp_sha256_ctx_t ctx;                        /* 原帧 -0xa4 */
	void           *bio;
	void           *rsa;
	uint32_t        ret;
	int             verify_ret;
	unsigned long   err_code;

	bio = 0;
	rsa = 0;
	bio = bio_new_mem_buf(root_pem_buf, 0x400);
	rsa = pem_read_bio_rsa_pubkey(bio, 0, 0, 0);
	if (rsa == 0) {
		err_code = err_get_error();
		printf("OpenSSL error: %s\n", err_error_string(err_code, 0));
		puts("Read Root PubK Failed!");
		ret = 0xc;
	}
	else {
		sha256_init(&ctx);
		sha256_update(&ctx, pem_data, pem_len);
		sha256_final(hash, &ctx);
		verify_ret = RSA_verify(FP_NID_SHA256, hash, FP_SHA256_DIGEST_LEN,
		                        sig, FP_RSA_SIG_LEN, rsa);
		if (rsa != 0) {
			rsa_free(rsa);
		}
		if (bio != 0) {
			bio_free(bio);
		}
		if (verify_ret == 1) {
			ret = 0;
		}
		else {
			err_code = err_get_error();
			printf("OpenSSL error: %s\n", err_error_string(err_code, 0));
			puts("Check miner.pem Failed!");
			ret = 0xd;
		}
	}
	return ret;
}

// === fp_extract_member === 原 FUN_00010de8 @ 0x00010de8
// 来源: reversed/fileparser_funcs/FUN_00010de8.c | 分类: core
// 语义: 从 BMU 包顺序抽取一个成员 (或其 .sig): 以 FP_IO_CHUNK 分块 fread,
//       边读边 SHA256 (整块/尾块按实读长度), 结果 32B 哈希写 hash_out[8] 并以
//       第 8 个字为返回值。dump 置位时按 type 拼路径 "/tmp/tmpfw/<名>[.sig]" 落盘。
//       type→文件名: 0=BOOT.bin 1=devicetree.dtb 2=uImage 3=minerfs.image.gz
//       4=update.image.gz 5=crl.tar.gz 6=miner.btm.tar.gz 7=reserve 9=datafile
//       (type=8 或其它 → 目录名后无文件名, 原码如此)。
uint32_t fp_extract_member(void *f_src, uint32_t size, uint32_t *hash_out,
                           uint8_t file_type, char is_sig, char dump)
{
	char            path[FP_NAME_BUF_LEN]; /* 原帧 -0x524 */
	uint8_t         member_hash[FP_SHA256_DIGEST_LEN]; /* 原帧 -0x4a4..-0x488 (8×u32) */
	fp_sha256_ctx_t ctx;                   /* 原帧 -0x484 */
	uint32_t        io_head;               /* 原帧 -0x414: IO 缓冲首 4 字节 */
	uint8_t         io_tail[1020];         /* 原帧 -0x410 */
	int             read_total;            /* 原帧 -0xc */
	int             read_now;              /* 原帧 -0x14 */
	void           *f_out;                 /* 原帧 -0x10 */
	int             name_len;

	io_head = 0;
	memset(io_tail, 0, 0x3fc);
	sha256_init(&ctx);
	read_total = 0;
	read_now = 0;
	f_out = 0;
	if (dump != '\0') {
		strncpy(path, FP_TMPFW_DIR, 0xc);       /* 含结束符, 12 字节 */
		memset(path + 0xc, 0, 0x74);            /* 清空剩余 116 字节 */
		switch (file_type) {
		case 0:
			name_len = strlen(path);
			strncpy(path + name_len, "BOOT.bin", 9);
			break;
		case 1:
			name_len = strlen(path);
			strncpy(path + name_len, "devicetree.dtb", 0xf);
			break;
		case 2:
			name_len = strlen(path);
			strncpy(path + name_len, "uImage", 7);
			break;
		case 3:
			name_len = strlen(path);
			strncpy(path + name_len, "minerfs.image.gz", 0x11);
			break;
		case 4:
			name_len = strlen(path);
			strncpy(path + name_len, "update.image.gz", 0x10);
			break;
		case 5:
			name_len = strlen(path);
			strncpy(path + name_len, "crl.tar.gz", 0xb);
			break;
		case 6:
			name_len = strlen(path);
			strncpy(path + name_len, "miner.btm.tar.gz", 0x11);
			break;
		case 7:
			name_len = strlen(path);
			strncpy(path + name_len, "reserve", 8);
			break;
		case 9:
			name_len = strlen(path);
			strncpy(path + name_len, "datafile", 9);
			break;
		}
		if (is_sig != '\0') {
			name_len = strlen(path);
			strncpy(path + name_len, ".sig", 5);
		}
		printf("fileName:\'%s\', size:[%d]\r\n", path, size);
		f_out = fopen64(path, "wb");
		if (f_out == 0) {
			printf("Create File \'%s\' Failed!\r\n", path);
		}
	}
	while (FP_IO_CHUNK < (uint32_t)(size - read_total)) {
		read_now = (int)fread(&io_head, 1, FP_IO_CHUNK, f_src);
		read_total = read_total + read_now;
		sha256_update(&ctx, &io_head, FP_IO_CHUNK); /* 原码固定喂整块 0x400 */
		if ((dump != '\0') && (f_out != 0)) {
			fwrite(&io_head, 1, FP_IO_CHUNK, f_out);
		}
	}
	read_now = (int)fread(&io_head, 1, (uint32_t)size - read_total, f_src);
	sha256_update(&ctx, &io_head, read_now);
	if ((dump != '\0') && (f_out != 0)) {
		fwrite(&io_head, 1, read_now, f_out);
	}
	member_hash[0] = 0;
	member_hash[1] = 0;
	member_hash[2] = 0;
	member_hash[3] = 0;
	member_hash[4] = 0;
	member_hash[5] = 0;
	member_hash[6] = 0;
	member_hash[7] = 0;
	sha256_final(member_hash, &ctx);
	if ((dump != '\0') && (f_out != 0)) {
		fclose(f_out);
	}
	hash_out[0] = ((uint32_t *)member_hash)[0];
	hash_out[1] = ((uint32_t *)member_hash)[1];
	hash_out[2] = ((uint32_t *)member_hash)[2];
	hash_out[3] = ((uint32_t *)member_hash)[3];
	hash_out[4] = ((uint32_t *)member_hash)[4];
	hash_out[5] = ((uint32_t *)member_hash)[5];
	hash_out[6] = ((uint32_t *)member_hash)[6];
	hash_out[7] = ((uint32_t *)member_hash)[7];
	return hash_out[7];
}

/* ==================== 3. 64 位哈希 (fp_cityhash64 = farmhash na v1.0, seed=81) ====================
 * 命名沿 CityHash64 家族; 算法与 farmhash 2014-03-31 初始版 farmhashna::Hash64
 * (固定 seed=81) 逐常量吻合, 已数值验证: y=seed*k1+113=0x226bb95b4e64b6d4、
 * shiftmix 输入 y*k2+113=0x919387da22096bdd、mul=k1+(z&0xff)<<1 等均一致。
 * 以下保留反编译位运算语义 (32 位双字搬运合并为等价 64 位运算)。 */

// === fp_cityhash_fetch32 === 原 FUN_00011460 @ 0x00011460
// 来源: reversed/fileparser_funcs/FUN_00011460.c | 分类: cityhash
// 语义: 非对齐取 4 字节 (ARM 下经 ldr 直接读)。
uint32_t fp_cityhash_fetch32(const void *p)
{
	return *(const uint32_t *)p;
}

// === fp_cityhash_fetch64 === 原 FUN_00011490 @ 0x00011490
// 来源: reversed/fileparser_funcs/FUN_00011490.c | 分类: cityhash
// 语义: 非对齐取 8 字节 — 原码经 memcpy(GOT 0x25038) 拷入局部再按 u64 返回。
uint64_t fp_cityhash_fetch64(const void *p)
{
	uint64_t val;
	memcpy(&val, p, 8);
	return val;
}

// === fp_cityhash_shiftmix === 原 FUN_000114c8 @ 0x000114c8
// 来源: reversed/fileparser_funcs/FUN_000114c8.c | 分类: cityhash
// 语义: ShiftMix(v) = v ^ (v >> 47); 原反编译按 (lo,hi) 两 32 位参数表达:
//       hi 不变, lo ^= hi>>15 — 与 64 位式等价 (47 = 32+15)。
uint64_t fp_cityhash_shiftmix(uint64_t val)
{
	return val ^ (val >> 47);
}

// === fp_cityhash_rotate64 === 原 FUN_00011514 @ 0x00011514
// 来源: reversed/fileparser_funcs/FUN_00011514.c | 分类: cityhash
// 语义: 64 位循环右移 Rotate64(val, shift) = (val >> shift) | (val << (64-shift));
//       原反编译: inv = -shift & 0x3f 后做 32 位双字搬运, 等价于下式
//       (实参旋转量均为 0x12..0x2c 区间常数, 行为一致)。
uint64_t fp_cityhash_rotate64(uint64_t val, uint32_t shift)
{
	uint32_t inv;

	inv = (0u - shift) & 0x3f; /* = (64-shift)&63 */
	return (val >> (shift & 0x3f)) | (val << inv);
}

// === fp_cityhash_hash_len16 === 原 FUN_00011598 @ 0x00011598
// 来源: reversed/fileparser_funcs/FUN_00011598.c | 分类: cityhash
// 语义: Murmur 风格 16 字节长度哈希 (farmhash na HashLen16):
//       a = (u^v)*mul; a ^= a>>47; b = (v^a)*mul; b ^= b>>47; return b*mul。
//       原反编译以 umull/umaal 32 位部分积拼 64 位乘法, 语义同上。
uint64_t fp_cityhash_hash_len16(uint64_t u, uint64_t v, uint64_t mul)
{
	uint64_t a;
	uint64_t b;

	a = (u ^ v) * mul;
	a ^= (a >> 47);
	b = (v ^ a) * mul;
	b ^= (b >> 47);
	return b * mul;
}

// === fp_cityhash_hash_len0to16 === 原 FUN_000116c8 @ 0x000116c8
// 来源: reversed/fileparser_funcs/FUN_000116c8.c | 分类: cityhash
// 语义: 长度 0..16 字符串哈希 (farmhash na HashLen0to16):
//   ≥8:  mul=k2+len*2; a=F64+k2; b=F64(s+len-8);
//        c=rot37(b)*mul+a; d=(rot25(a)+b)*mul; return HashLen16(c,d,mul)
//   ≥4:  mul=k2+len*2; return HashLen16(len+(F32<<3), F32(s+len-4), mul)
//   >0:  y=s[0]|s[len>>1]<<8; z=len+s[len-1]<<2; return ShiftMix(y*k2^z*k0)*k2
//   =0:  return k2
uint64_t fp_cityhash_hash_len0to16(const uint8_t *s, uint32_t len)
{
	uint64_t mul;
	uint64_t a;
	uint64_t b;
	uint64_t c;
	uint64_t d;
	uint32_t y;
	uint32_t z;

	if (len >= 8) {
		mul = FP_K2 + (uint64_t)len * 2;
		a = fp_cityhash_fetch64(s) + FP_K2;
		b = fp_cityhash_fetch64(s + len - 8);
		c = fp_cityhash_rotate64(b, 37) * mul + a;
		d = (fp_cityhash_rotate64(a, 25) + b) * mul;
		return fp_cityhash_hash_len16(c, d, mul);
	}
	if (len >= 4) {
		mul = FP_K2 + (uint64_t)len * 2;
		a = fp_cityhash_fetch32(s);
		return fp_cityhash_hash_len16(len + ((uint64_t)a << 3),
		                              (uint64_t)fp_cityhash_fetch32(s + len - 4), mul);
	}
	if (len > 0) {
		y = (uint32_t)s[0] + ((uint32_t)s[len >> 1] << 8);
		z = len + ((uint32_t)s[len - 1] << 2);
		return fp_cityhash_shiftmix((uint64_t)y * FP_K2 ^ (uint64_t)z * FP_K0) * FP_K2;
	}
	return FP_K2;
}

// === fp_cityhash_hash_len17to32 === 原 FUN_00011a00 @ 0x00011a00
// 来源: reversed/fileparser_funcs/FUN_00011a00.c | 分类: cityhash
// 语义: 长度 17..32 字符串哈希 (farmhash na HashLen17to32):
//   mul=k2+len*2; a=F64(s)*k1; b=F64(s+8); c=F64(s+len-8)*mul; d=F64(s+len-16)*k2;
//   return HashLen16(rot43(a+b)+rot30(c)+d, a+rot18(b+k2)+c, mul)
//   (原反编译第二参实序为 c+rot18(b+k2)+a, 加法交换等价)。
uint64_t fp_cityhash_hash_len17to32(const char *s, uint32_t len)
{
	uint64_t mul;
	uint64_t a;
	uint64_t b;
	uint64_t c;
	uint64_t d;

	mul = FP_K2 + (uint64_t)len * 2;
	a = fp_cityhash_fetch64(s) * FP_K1;
	b = fp_cityhash_fetch64(s + 8);
	c = fp_cityhash_fetch64(s + len - 8) * mul;
	d = fp_cityhash_fetch64(s + len - 16) * FP_K2;
	return fp_cityhash_hash_len16(
	           fp_cityhash_rotate64(a + b, 43) + fp_cityhash_rotate64(c, 30) + d,
	           a + fp_cityhash_rotate64(b + FP_K2, 18) + c, mul);
}

// === fp_cityhash_hash_len33to64 === 原 FUN_00011be0 @ 0x00011be0
// 来源: reversed/fileparser_funcs/FUN_00011be0.c | 分类: cityhash
// 语义: 长度 33..64 字符串哈希 (farmhash na HashLen33to64, 2014 初始版 —
//       与 v1.0.3 的 vf/vs 形态及 v1.1 final 的 bswap 形态均不同, 以反编译为准):
//   mul=k2+len*2; a=F64(s)*k2; b=F64(s+8); c=F64(s+len-8)*mul; d=F64(s+len-16)*k2;
//   y=rot43(a+b)+rot30(c)+d; z=HashLen16(y, a+rot18(b+k2)+c, mul);
//   e=F64(s+16)*mul; f=F64(s+24);
//   g=(y+F64(s+len-32))*mul; h=(z+F64(s+len-24))*mul;
//   return HashLen16(rot43(e+f)+rot30(g)+h, e+rot18(f+a)+g, mul)
uint64_t fp_cityhash_hash_len33to64(const char *s, uint32_t len)
{
	uint64_t mul;
	uint64_t a;
	uint64_t b;
	uint64_t c;
	uint64_t d;
	uint64_t y;
	uint64_t z;
	uint64_t e;
	uint64_t f;
	uint64_t g;
	uint64_t h;

	mul = FP_K2 + (uint64_t)len * 2;
	a = fp_cityhash_fetch64(s) * FP_K2;
	b = fp_cityhash_fetch64(s + 8);
	c = fp_cityhash_fetch64(s + len - 8) * mul;
	d = fp_cityhash_fetch64(s + len - 16) * FP_K2;
	y = fp_cityhash_rotate64(a + b, 43) + fp_cityhash_rotate64(c, 30) + d;
	z = fp_cityhash_hash_len16(y, a + fp_cityhash_rotate64(b + FP_K2, 18) + c, mul);
	e = fp_cityhash_fetch64(s + 16) * mul;
	f = fp_cityhash_fetch64(s + 24);
	g = (y + fp_cityhash_fetch64(s + len - 32)) * mul;
	h = (z + fp_cityhash_fetch64(s + len - 24)) * mul;
	return fp_cityhash_hash_len16(
	           fp_cityhash_rotate64(e + f, 43) + fp_cityhash_rotate64(g, 30) + h,
	           e + fp_cityhash_rotate64(f + a, 18) + g, mul);
}

// === fp_cityhash_weak_len32_mix === 原 FUN_00011fbc @ 0x00011fbc
// 来源: reversed/fileparser_funcs/FUN_00011fbc.c | 分类: cityhash
// 语义: WeakHashLen32WithSeeds 数值版 (farmhash na): 快而糙的 32 字节 16B 摘要。
//   a += w; b = rot21(b+a+z); c = a; a += x; a += y; b += rot44(a);
//   out = { a+z, b+c }
//   (原反编译 14 形参为 Ghidra 64 位寄存器拆分噪声, 实参 6×u64; 尾 3 槽未用)
void fp_cityhash_weak_len32_mix(uint64_t out[2], uint64_t w, uint64_t x, uint64_t y,
                                uint64_t z, uint64_t a, uint64_t b)
{
	uint64_t c;

	a += w;
	b = fp_cityhash_rotate64(b + a + z, 21);
	c = a;
	a += x;
	a += y;
	b += fp_cityhash_rotate64(a, 44);
	out[0] = a + z;
	out[1] = b + c;
}

// === fp_cityhash_weak_len32 === 原 FUN_000120dc @ 0x000120dc
// 来源: reversed/fileparser_funcs/FUN_000120dc.c | 分类: cityhash
// 语义: WeakHashLen32WithSeeds 缓冲版 — 取 s[0..32) 四个 u64 后调 weak_len32_mix。
void fp_cityhash_weak_len32(uint64_t out[2], const char *s, uint64_t a, uint64_t b)
{
	fp_cityhash_weak_len32_mix(out,
	                           fp_cityhash_fetch64(s),
	                           fp_cityhash_fetch64(s + 8),
	                           fp_cityhash_fetch64(s + 16),
	                           fp_cityhash_fetch64(s + 24),
	                           a, b);
}

// === fp_cityhash64 === 原 FUN_00012180 @ 0x00012180
// 来源: reversed/fileparser_funcs/FUN_00012180.c | 分类: cityhash
// 语义: miner_type 64 位哈希入口 — farmhash na v1.0 Hash64 (固定 seed=0x51):
//   len<17 → hash_len0to16; len<33 → hash_len17to32; len<65 → hash_len33to64;
//   否则 64 字节分块主循环 (状态 x,y,z + v/w 二元组, rot37/42/33 乘 k1),
//   尾块再过一遍 (mul=k1+(z&0xff)<<1, w.first+=尾长, rot 乘数换 mul, v.first*9),
//   最终 HashLen16(HashLen16(v0,w0,mul)+ShiftMix(y)*k0+z,
//                  HashLen16(v1,w1,mul)+x, mul)。
//   主循环逐句与反编译一致: x^=w1; y+=v0+F64(s+40); z=rot33(z+w0)*k1;
//   v=Weak32(s, v1*k1, x+w0); w=Weak32(s+32, z+w1, y+F64(s+16)); swap(z,x)。
uint64_t fp_cityhash64(const char *s, uint32_t len)
{
	uint64_t x;
	uint64_t y;
	uint64_t z;
	uint64_t v[2]; /* v[0]=v.first, v[1]=v.second (原 @0x60/@0x58) */
	uint64_t w[2]; /* w[0]=w.first, w[1]=w.second (原 @0x70/@0x68) */
	const char *end;     /* 原帧 -0x44: 分块区末尾 */
	const char *last64;  /* 原帧 -0x48: 输入最后 64 字节窗口起点 */
	uint64_t mul;
	uint64_t t;

	if (len < 0x11) {
		return fp_cityhash_hash_len0to16((const uint8_t *)s, len);
	}
	if (len < 0x21) {
		return fp_cityhash_hash_len17to32(s, len);
	}
	if (len < 0x41) {
		return fp_cityhash_hash_len33to64(s, len);
	}

	/* 初始状态 (seed=0x51): y=seed*k1+113, z=ShiftMix(y*k2+113)*k2, v=w={0,0} */
	x = FP_CITYHASH_SEED;
	y = FP_CITYHASH_SEED * FP_K1 + 113;
	z = fp_cityhash_shiftmix(y * FP_K2 + 113) * FP_K2;
	v[0] = 0;
	v[1] = 0;
	w[0] = 0;
	w[1] = 0;
	x = x * FP_K2 + fp_cityhash_fetch64(s); /* 原: F64(s) + 0x51*k2 */

	/* 原码: end = s + (len-1 & ~63), last64 = end + (len-1 & 63) - 63 */
	end = s + (uint32_t)((len - 1) & ~(uint32_t)63);
	last64 = end + ((len - 1) & 63) - 63;

	do {
		x = fp_cityhash_rotate64(x + y + v[0] + fp_cityhash_fetch64(s + 8), 37) * FP_K1;
		y = fp_cityhash_rotate64(y + v[1] + fp_cityhash_fetch64(s + 48), 42) * FP_K1;
		x ^= w[1];
		y += v[0] + fp_cityhash_fetch64(s + 40);
		z = fp_cityhash_rotate64(z + w[0], 33) * FP_K1;
		fp_cityhash_weak_len32(v, s, v[1] * FP_K1, x + w[0]);
		fp_cityhash_weak_len32(w, s + 32, z + w[1], y + fp_cityhash_fetch64(s + 16));
		t = z; /* std::swap(z, x) — 原码经寄存器携带 (lVar10/lVar11) 实现 */
		z = x;
		x = t;
		s += 64;
	} while (s != end);

	mul = FP_K1 + ((z & 0xff) << 1); /* 原: k1 + (z*2 & 0x1fe) */
	s = last64;
	w[0] += (uint32_t)(len - 1) & 63;
	v[0] += w[0];
	w[0] += v[0];
	x = fp_cityhash_rotate64(x + y + v[0] + fp_cityhash_fetch64(s + 8), 37) * mul;
	y = fp_cityhash_rotate64(y + v[1] + fp_cityhash_fetch64(s + 48), 42) * mul;
	x ^= w[1] * 9;
	y += v[0] * 9 + fp_cityhash_fetch64(s + 40);
	z = fp_cityhash_rotate64(z + w[0], 33) * mul;
	fp_cityhash_weak_len32(v, s, v[1] * mul, x + w[0]);
	fp_cityhash_weak_len32(w, s + 32, z + w[1], y + fp_cityhash_fetch64(s + 16));
	t = z; /* std::swap(z, x) */
	z = x;
	x = t;
	return fp_cityhash_hash_len16(
	           fp_cityhash_hash_len16(v[0], w[0], mul) + fp_cityhash_shiftmix(y) * FP_K0 + z,
	           fp_cityhash_hash_len16(v[1], w[1], mul) + x,
	           mul);
}

/* ==================== 4. BMU 包处理 ==================== */

// === fp_process_bmu === 原 FUN_00012c40 @ 0x00012c40
// 来源: reversed/fileparser_funcs/FUN_00012c40.c | 分类: core
// 语义: BMU 包核心校验/拆包 (见文件头"验证流程")。flag_dump_pem (-s/-x) 落盘
//       /tmp/tmpfw/miner.pem(.sig) 并抽取成员文件; flag_print_comment (-p) 打印
//       +0x550 处包注释; flag_full_check (-x) 额外要求 content 位图高 7 位全 1。
//       返回错误码 (0=完成); 各失败分支按原码关闭 f_in (部分分支故意泄漏, 原样保留)。
uint32_t fp_process_bmu(const char *bmu_path, const char *miner_type,
                        const char *root_pem_path, uint8_t *buf,
                        char flag_dump_pem, char flag_print_comment, char flag_full_check)
{
	fp_stat64_t     st;               /* 原帧 -0xd0/-0xa0: stat64 + st_size */
	uint32_t        file_size;        /* 原帧 -0x2c */
	void           *f_in;             /* 原帧 -0x30 */
	uint64_t        bmu_hash;         /* 原帧 -0x38 (u64) */
	uint64_t        hdr_hash;         /* 原帧 -0xd8/-0xd4 (u64) */
	uint16_t        content_mask;     /* 原帧 -0x3a (大端位图) */
	uint8_t         n_files;          /* 原帧 -0x11: 位 1 计数 */
	uint32_t        total_calc;       /* 原帧 -0x1c: (count+9)*0x100 + Σsize */
	uint32_t        total_declared;   /* 原帧 -0x40: +0x519 大端 (仅提示用) */
	uint32_t        member_size;      /* 原帧 -0x68/-0x64 */
	uint32_t        i;
	void           *f_root;           /* 原帧 -0x44 */
	uint32_t        root_pem_head;    /* 原帧 -0x4d8: 根公钥缓冲首 4 字节 */
	uint8_t         root_pem_tail[1020]; /* 原帧 -0x4d4 (合计 0x400) */
	uint32_t        pem_len;          /* 原帧 -0x48: +0x16 大端 */
	uint32_t        verify_ret;       /* 原帧 -0x4c (函数返回值) */
	void           *f_ver;            /* 原帧 -0x50 */
	void           *f_pem;            /* 原帧 -0x54 */
	uint8_t         hash_area[1056];  /* 原帧 -0x8f8 (0x420): [0]=整头哈希, [1..N]=成员哈希, [N+1..2N]=sig 哈希 */
	fp_sha256_ctx_t hdr_ctx;          /* 原帧 -0x968 */
	fp_sha256_ctx_t area_ctx;         /* 原帧 -0xaf8 */
	uint32_t        sig_end_head;     /* 原帧 -0xa68: 包尾签名缓冲首 4 字节 */
	uint8_t         sig_end_tail[252]; /* 原帧 -0xa64 (合计 0x100) */
	uint32_t        member_hash[8];   /* 原帧 -0xa88..-0xa6c: 哈希区摘要 */
	void           *bio;              /* 原帧 -0x58 */
	void           *rsa;              /* 原帧 -0x5c */
	int             rsa_ret;          /* 原帧 -0x60 */
	uint32_t        comment_head;     /* 原帧 -0xbfc */
	uint8_t         comment_tail[256]; /* 原帧 -0xbf8 */
	unsigned long   err_code;

	/* 1) 文件大小 ≥ 0x800 */
	fp_stat_file(bmu_path, &st);
	file_size = st.st_size;
	if (file_size < 0x800) {
		printf("File \'%s\' Not Enough %d, Something Wrong!\n", bmu_path, 0x800);
		return 6;
	}

	/* 2) 打开并读 0x800 头 */
	f_in = fopen64(bmu_path, "rb");
	if (f_in == 0) {
		printf("Read File \'%s\' Failed!\n", bmu_path);
		return 7;
	}
	fread(buf, 0x800, 1, f_in);
	if (buf[BMU_HDR_MAGIC] != '&') {
		printf("\'%s\' Not A Btmu File!\n", bmu_path);
		if (f_in != 0) {
			fclose(f_in);
		}
		return 8;
	}

	/* 3) miner_type 哈希比对 (fp_cityhash64 vs 包头 +0x002 的 8 字节) */
	printf("miner_type %s\n", miner_type);
	bmu_hash = fp_cityhash64(miner_type, (uint32_t)strlen(miner_type));
	memcpy(&hdr_hash, buf + BMU_HDR_TYPE_HASH, 8);
	printf("miner_type_hash %16llx, bmu_hash %16llx\n", bmu_hash, hdr_hash);
	if (bmu_hash != hdr_hash) {
		puts("input miner_type and bmu miner type donot match!");
		if (f_in != 0) {
			fclose(f_in);
		}
		return 0x1e;
	}

	/* 4) content 位图 (大端 u16 @0x0b); -x 额外要求 & 0xfe00 == 0xfe00 (整包) */
	content_mask = (uint16_t)(((uint16_t)buf[0xb] << 8) | buf[0xc]);
	if (flag_full_check != '\0') {
		printf("content:%x\n", content_mask);
		if ((content_mask & 0xfe00) != 0xfe00) {
			puts("This Package Was Not Full Package!");
			return 0x14;
		}
	}

	/* 5) 位 1 计数须等于包头 +0x518 的成员数 */
	n_files = 0;
	for (i = 0; i < 0x10; i++) {
		if (((content_mask >> i) & 1u) != 0) {
			n_files = n_files + 1;
		}
	}
	if (n_files != buf[BMU_HDR_FILE_COUNT]) {
		printf("Content Doesn\'t Match![%d][%d]\n", buf[BMU_HDR_FILE_COUNT], n_files);
		if (f_in != 0) {
			fclose(f_in);
		}
		return 9;
	}

	/* 6) 总长校验: (count+9)*0x100 + Σ成员 size == 实际文件大小 */
	total_calc = ((uint32_t)buf[BMU_HDR_FILE_COUNT] + 9) * 0x100;
	total_declared = ((uint32_t)buf[BMU_HDR_TOTAL_SIZE] << 0x18) |
	                 ((uint32_t)buf[BMU_HDR_TOTAL_SIZE + 1] << 0x10) |
	                 ((uint32_t)buf[BMU_HDR_TOTAL_SIZE + 2] << 8) |
	                 (uint32_t)buf[BMU_HDR_TOTAL_SIZE + 3];
	for (i = 0; i < (uint32_t)n_files; i++) {
		member_size = ((uint32_t)buf[i * BMU_FILE_ENTRY_SIZE + 0x51e] << 0x18) |
		              ((uint32_t)buf[i * BMU_FILE_ENTRY_SIZE + 0x51f] << 0x10) |
		              ((uint32_t)buf[i * BMU_FILE_ENTRY_SIZE + 0x520] << 8) |
		              (uint32_t)buf[i * BMU_FILE_ENTRY_SIZE + 0x521];
		total_calc = total_calc + member_size;
		printf("file[%d] size:[%d]\n", i, member_size);
	}
	if (total_calc != file_size) {
		printf("Check FileSize Failed, FileSize Should Be [%d]Bytes, But It Was [%d] Bytes, And Total Says[%d]\n",
		       total_declared, file_size, total_calc);
		if (f_in != 0) {
			fclose(f_in);
		}
		return 10;
	}

	/* 7) 根公钥验 miner.pem 签名 (+0x418, 对 +0x18 起 pem_len 字节) */
	f_root = fopen64(root_pem_path, "r");
	if (f_root == 0) {
		printf("Cannot Open Root PublicKey \'%s\'!\n", root_pem_path);
		if (f_in != 0) {
			fclose(f_in);
		}
		return 0xb;
	}
	root_pem_head = 0;
	memset(root_pem_tail, 0, 0x3fc);
	fread(&root_pem_head, 0x400, 1, f_root);
	fclose(f_root);
	pem_len = (uint32_t)(((uint32_t)buf[BMU_HDR_PEM_LEN] << 8) | buf[BMU_HDR_PEM_LEN + 1]);
	verify_ret = fp_verify_sig_sha256(buf + BMU_HDR_PEM, pem_len,
	                                  buf + BMU_HDR_SIG, &root_pem_head);
	if (verify_ret != 0) {
		printf("Check pem payload failed! ret:[%d]\n", verify_ret);
		if (f_in != 0) {
			fclose(f_in);
		}
		return verify_ret;
	}

	/* 8) 写 /usr/bin/fw_version (包头 +0x0d 的 8 字节) */
	f_ver = fopen64("/usr/bin/fw_version", "w");
	if (f_ver != 0) {
		printf("fw version data: %02X %02X %02X %02X %02X %02X %02X %02X\n",
		       buf[0xd], buf[0xe], buf[0xf], buf[0x10],
		       buf[0x11], buf[0x12], buf[0x13], buf[0x14]);
		fwrite(buf + BMU_HDR_FW_VERSION, 1, 8, f_ver);
		fclose(f_ver);
	}

	/* 9) (-s/-x) 落盘 miner.pem 与 .sig */
	if (flag_dump_pem != '\0') {
		f_pem = fopen64("/tmp/tmpfw/miner.pem", "w");
		if (f_pem == 0) {
			puts("Dump Miner.pem Failed!\r");
			return 0xf;
		}
		fwrite(buf + BMU_HDR_PEM, 1, pem_len, f_pem);
		fclose(f_pem);
		f_pem = fopen64("/tmp/tmpfw/miner.pem.sig", "w");
		if (f_pem == 0) {
			puts("Dump Miner.pem.sig Failed!\r");
			return 0x10;
		}
		fwrite(buf + BMU_HDR_SIG, 1, 0x100, f_pem);
		fclose(f_pem);
	}

	/* 10) 哈希区: [0]=SHA256(头 0x800B), [1..N]=成员哈希, [N+1..2N]=成员 .sig 哈希 */
	memset(hash_area, 0, 0x420);
	sha256_init(&hdr_ctx);
	sha256_update(&hdr_ctx, buf, BMU_HEADER_SIZE);
	sha256_final(hash_area, &hdr_ctx);
	for (i = 0; i < (uint32_t)n_files; i++) {
		member_size = ((uint32_t)buf[i * BMU_FILE_ENTRY_SIZE + 0x51e] << 0x18) |
		              ((uint32_t)buf[i * BMU_FILE_ENTRY_SIZE + 0x51f] << 0x10) |
		              ((uint32_t)buf[i * BMU_FILE_ENTRY_SIZE + 0x520] << 8) |
		              (uint32_t)buf[i * BMU_FILE_ENTRY_SIZE + 0x521];
		fp_extract_member(f_in, member_size,
		          (uint32_t *)(hash_area + (i + 1) * FP_SHA256_DIGEST_LEN),
		          buf[i * BMU_FILE_ENTRY_SIZE + BMU_HDR_FILE_TABLE], 0, flag_dump_pem);
	}
	for (i = 0; i < (uint32_t)n_files; i++) {
		fp_extract_member(f_in, FP_RSA_SIG_LEN,
		          (uint32_t *)(hash_area + (n_files + 1 + i) * FP_SHA256_DIGEST_LEN),
		          buf[i * BMU_FILE_ENTRY_SIZE + BMU_HDR_FILE_TABLE], 1, flag_dump_pem);
	}

	/* 11) 读包尾 0x100B RSA 签名, 对哈希区摘要做验签准备 */
	sig_end_head = 0;
	memset(sig_end_tail, 0, 0xfc);
	fseek(f_in, -(long)FP_RSA_SIG_LEN, FP_SEEK_END);
	fread(&sig_end_head, 0x100, 1, f_in);
	member_hash[0] = 0;
	member_hash[1] = 0;
	member_hash[2] = 0;
	member_hash[3] = 0;
	member_hash[4] = 0;
	member_hash[5] = 0;
	member_hash[6] = 0;
	member_hash[7] = 0;
	sha256_init(&area_ctx);
	sha256_update(&area_ctx, hash_area, (uint32_t)n_files * 0x40 + 0x20);
	sha256_final((uint8_t *)member_hash, &area_ctx);

	/* 12) miner.pem 公钥 (读窗固定 0x400) 验包尾签名 */
	bio = bio_new_mem_buf(buf + BMU_HDR_PEM, 0x400);
	rsa = pem_read_bio_rsa_pubkey(bio, 0, 0, 0);
	if (rsa == 0) {
		err_code = err_get_error();
		printf("OpenSSL error: %s\n", err_error_string(err_code, 0));
		puts("Load Pem Failed!");
		verify_ret = 0x11;
	}
	else {
		rsa_ret = RSA_verify(FP_NID_SHA256, (const unsigned char *)member_hash,
		                     FP_SHA256_DIGEST_LEN, (const unsigned char *)&sig_end_head,
		                     FP_RSA_SIG_LEN, rsa);
		if (rsa != 0) {
			rsa_free(rsa);
		}
		if (bio != 0) {
			bio_free(bio);
		}
		if (rsa_ret == 1) {
			if (f_in != 0) {
				fclose(f_in);
			}
			f_in = 0;
			puts("All Done!\r");
			if (flag_print_comment != '\0') {
				puts("This Comment Of This Package:");
				comment_head = 0;
				memset(comment_tail, 0, 0xfd);
				memcpy(&comment_head, buf + BMU_COMMENT_OFFSET, FP_COMMENT_LEN);
				puts((const char *)&comment_head);
			}
			verify_ret = 0;
		}
		else {
			/* 原码此分支不关 f_in (句柄泄漏按原样保留) */
			puts("Check File Sig failed!\r");
			err_code = err_get_error();
			printf("OpenSSL error: %s\n", err_error_string(err_code, 0));
			verify_ret = 0x12;
		}
	}
	return verify_ret;
}

// === main === 原 FUN_00013808 @ 0x00013808
// 来源: reversed/fileparser_funcs/FUN_00013808.c | 分类: cli
// 语义: 命令行分发 — 校验参数个数/格式并按 argv[1][1] 逐级直落:
//       'n'→fp_split_sigimg; 'q'→fp_gen_256bff;
//       'x'(flag_full_check=1)→'p'(flag_print_comment=1)→'s'(flag_dump_pem=1)
//       →'f'→fp_process_bmu; default → "Command Not Support!" + 用法。
//       返回: 1/2/3/4/5 = 参数/命令错误; 否则为子命令返回值。
int main(int argc, char **argv)
{
	uint32_t ret;              /* 原帧 -0x10 */
	char     flag_dump_pem;    /* 原帧 -0x9  (-s/-x: 拆包落盘) */
	char     flag_print_comment; /* 原帧 -0xa (-p: 打印包注释) */
	char     flag_full_check;  /* 原帧 -0xb  (-x: 整包校验) */
	char     miner_type[256];  /* 原帧 -0x110..-0x10 (256B: 首 4B + 0xfc) */
	char     file_name[128];   /* 原帧 -0x190..-0x110 (128B: 首 4B + 0x7c) */
	char     pem_name[128];    /* 原帧 -0x210..-0x190 (128B: 首 4B + 0x7c) */
	uint8_t  bmu_buf[2048];    /* 原帧 -0xa10 (0x800) */
	uint8_t  unused_tail[256]; /* 原帧 -0xb10: 仅清零 (0xfd), 后续未引用, 原样保留 */

	if (argc < 2) {
		fp_print_usage(argv[0]);
		ret = 1;
	}
	else if (argv[1][0] == '-') {
		((uint32_t *)miner_type)[0] = 0;  /* 原码首 4B 清零 */
		memset(miner_type + 4, 0, 0xfc);  /* 其余 0xfc 清零 (合计 256B) */
		((uint32_t *)file_name)[0] = 0;
		memset(file_name + 4, 0, 0x7c);
		((uint32_t *)pem_name)[0] = 0;
		memset(pem_name + 4, 0, 0x7c);
		memset(bmu_buf, 0, 0x800);
		flag_full_check = 0;
		flag_print_comment = '\0';
		flag_dump_pem = '\0';
		memset(unused_tail, 0, 0xfd);
		if (argc == 5) {
			if (0xff < strlen(argv[2])) {
				puts("fileName Too Long!");
				return 3;
			}
			strcpy(miner_type, argv[2]);
			if (0x7f < strlen(argv[3])) {
				puts("fileName Too Long!");
				return 3;
			}
			strcpy(file_name, argv[3]);
			if (0x7f < strlen(argv[4])) {
				puts("pemName Too Long!");
				return 4;
			}
			strcpy(pem_name, argv[4]);
		}
		else if (argc == 3) {
			if (0x7f < strlen(argv[2])) {
				puts("fileName Too Long!");
				return 3;
			}
			strcpy(file_name, argv[2]);
		}
		else if (argc != 2) {
			puts("Param Err!");
			fp_print_usage(argv[0]);
			return 3;
		}
		ret = 0;
		switch ((uint8_t)argv[1][1]) {
		default:
			puts("Command Not Support!");
			fp_print_usage(argv[0]);
			ret = 5;
			break;
		case 0x6e: /* 'n' */
			ret = fp_split_sigimg(file_name);
			break;
		case 0x71: /* 'q' */
			ret = fp_gen_256bff();
			break;
		case 0x78: /* 'x' — 置整包校验后直落 p/s/f */
			flag_full_check = '\x01';
			/* fall through */
		case 0x70: /* 'p' — 未被 -x 置位时置打印注释, 直落 s/f */
			if (flag_full_check == '\0') {
				flag_print_comment = '\x01';
			}
			/* fall through */
		case 0x73: /* 's' — 未被 -p 置位时置拆包落盘, 直落 f */
			if (flag_print_comment == '\0') {
				flag_dump_pem = 1;
			}
			/* fall through */
		case 0x66: /* 'f' */
			ret = fp_process_bmu(file_name, miner_type, pem_name, bmu_buf,
			                     flag_dump_pem, flag_print_comment, flag_full_check);
			break;
		}
	}
	else {
		fp_print_usage(argv[0]);
		ret = 2;
	}
	return ret;
}

// === __libc_csu_init === 原 FUN_00013bec @ 0x00013bec
// 来源: reversed/fileparser_funcs/FUN_00013bec.c | 分类: crt
// 语义: glibc csu 初始化 — 先调 _init (FUN_000106e8), 再遍历 .init_array
//       (原 DAT_00024f08 @ 0x24f08, 本二进制仅 1 个条目, 加载期由动态重定位
//       写入 frame_dummy), 以 (argc, argv, envp) 调用 (原反编译多出的
//       第 4/5 实参为 Ghidra 噪声, 机器码只传 r0-r3)。
void __libc_csu_init(int argc, char **argv, char **envp)
{
	void (**init_fn)(int, char **, char **); /* 原寄存器变量 */
	int i;                                   /* 原 iVar1 */

	_init();
	i = 0;
	init_fn = (void (**)(int, char **, char **))0x24f08; /* 原 &DAT_00024f08 (.init_array) */
	do {
		i = i + 1;
		(*init_fn)(argc, argv, envp);
		init_fn = init_fn + 1;
	} while (i != 1); /* 原码固定 1 次 (本二进制 init_array 长度) */
}

// === fp_stat_file === 原 FUN_00013c2c @ 0x00013c2c
// 来源: reversed/fileparser_funcs/FUN_00013c2c.c | 分类: core
// 语义: 取文件状态 — 原体尾调 0x107e4 @plt = __xstat64(3, path, buf)
//       (stat64 版本 3); fp_process_bmu 用其 st_size (缓冲 +0x30) 做 ≥0x800 检查。
void fp_stat_file(const char *path, fp_stat64_t *stat_buf)
{
	__xstat64(3, path, stat_buf);
}

/* ==================== 覆盖率表 (42/42) ====================
 * 原 FUN_000106e8 @ 0x000106e8 -> _init                     (reversed/fileparser_funcs/FUN_000106e8.c)
 * 原 FUN_00010708 @ 0x00010708 -> abort                     (reversed/fileparser_funcs/FUN_00010708.c)
 * 原 FUN_00010714 @ 0x00010714 -> err_get_error             (reversed/fileparser_funcs/FUN_00010714.c)
 * 原 FUN_00010720 @ 0x00010720 -> __libc_start_main         (reversed/fileparser_funcs/FUN_00010720.c)
 * 原 FUN_0001075c @ 0x0001075c -> fseek                     (reversed/fileparser_funcs/FUN_0001075c.c)
 * 原 FUN_00010768 @ 0x00010768 -> pem_read_bio_rsa_pubkey   (reversed/fileparser_funcs/FUN_00010768.c)
 * 原 FUN_00010774 @ 0x00010774 -> sha256_final              (reversed/fileparser_funcs/FUN_00010774.c)
 * 原 FUN_00010780 @ 0x00010780 -> strlen                    (reversed/fileparser_funcs/FUN_00010780.c)
 * 原 FUN_0001078c @ 0x0001078c -> memcpy                    (reversed/fileparser_funcs/FUN_0001078c.c)
 * 原 FUN_00010798 @ 0x00010798 -> fopen64                   (reversed/fileparser_funcs/FUN_00010798.c)
 * 原 FUN_000107a4 @ 0x000107a4 -> bio_new_mem_buf           (reversed/fileparser_funcs/FUN_000107a4.c)
 * 原 FUN_000107b0 @ 0x000107b0 -> strcpy                    (reversed/fileparser_funcs/FUN_000107b0.c)
 * 原 FUN_000107bc @ 0x000107bc -> printf                    (reversed/fileparser_funcs/FUN_000107bc.c)
 * 原 FUN_000107c8 @ 0x000107c8 -> err_error_string          (reversed/fileparser_funcs/FUN_000107c8.c)
 * 原 FUN_000107d4 @ 0x000107d4 -> fwrite                    (reversed/fileparser_funcs/FUN_000107d4.c)
 * 原 FUN_000107f0 @ 0x000107f0 -> puts                      (reversed/fileparser_funcs/FUN_000107f0.c)
 * 原 FUN_000107fc @ 0x000107fc -> rsa_free                  (reversed/fileparser_funcs/FUN_000107fc.c)
 * 原 FUN_00010808 @ 0x00010808 -> sha256_update             (reversed/fileparser_funcs/FUN_00010808.c)
 * 原 FUN_00010814 @ 0x00010814 -> bio_free                  (reversed/fileparser_funcs/FUN_00010814.c)
 * 原 FUN_00010820 @ 0x00010820 -> sha256_init               (reversed/fileparser_funcs/FUN_00010820.c)
 * 原 FUN_0001082c @ 0x0001082c -> fread                     (reversed/fileparser_funcs/FUN_0001082c.c)
 * 原 FUN_00010838 @ 0x00010838 -> entry                     (reversed/fileparser_funcs/entry.c)
 * 原 FUN_000108f8 @ 0x000108f8 -> fp_print_usage            (reversed/fileparser_funcs/FUN_000108f8.c)
 * 原 FUN_000109a8 @ 0x000109a8 -> fp_split_sigimg           (reversed/fileparser_funcs/FUN_000109a8.c)
 * 原 FUN_00010b9c @ 0x00010b9c -> fp_gen_256bff             (reversed/fileparser_funcs/FUN_00010b9c.c)
 * 原 FUN_00010c70 @ 0x00010c70 -> fp_verify_sig_sha256      (reversed/fileparser_funcs/FUN_00010c70.c)
 * 原 FUN_00010de8 @ 0x00010de8 -> fp_extract_member         (reversed/fileparser_funcs/FUN_00010de8.c)
 * 原 FUN_00011460 @ 0x00011460 -> fp_cityhash_fetch32       (reversed/fileparser_funcs/FUN_00011460.c)
 * 原 FUN_00011490 @ 0x00011490 -> fp_cityhash_fetch64       (reversed/fileparser_funcs/FUN_00011490.c)
 * 原 FUN_000114c8 @ 0x000114c8 -> fp_cityhash_shiftmix      (reversed/fileparser_funcs/FUN_000114c8.c)
 * 原 FUN_00011514 @ 0x00011514 -> fp_cityhash_rotate64      (reversed/fileparser_funcs/FUN_00011514.c)
 * 原 FUN_00011598 @ 0x00011598 -> fp_cityhash_hash_len16    (reversed/fileparser_funcs/FUN_00011598.c)
 * 原 FUN_000116c8 @ 0x000116c8 -> fp_cityhash_hash_len0to16 (reversed/fileparser_funcs/FUN_000116c8.c)
 * 原 FUN_00011a00 @ 0x00011a00 -> fp_cityhash_hash_len17to32 (reversed/fileparser_funcs/FUN_00011a00.c)
 * 原 FUN_00011be0 @ 0x00011be0 -> fp_cityhash_hash_len33to64 (reversed/fileparser_funcs/FUN_00011be0.c)
 * 原 FUN_00011fbc @ 0x00011fbc -> fp_cityhash_weak_len32_mix (reversed/fileparser_funcs/FUN_00011fbc.c)
 * 原 FUN_000120dc @ 0x000120dc -> fp_cityhash_weak_len32    (reversed/fileparser_funcs/FUN_000120dc.c)
 * 原 FUN_00012180 @ 0x00012180 -> fp_cityhash64             (reversed/fileparser_funcs/FUN_00012180.c)
 * 原 FUN_00012c40 @ 0x00012c40 -> fp_process_bmu            (reversed/fileparser_funcs/FUN_00012c40.c)
 * 原 FUN_00013808 @ 0x00013808 -> main                      (reversed/fileparser_funcs/FUN_00013808.c)
 * 原 FUN_00013bec @ 0x00013bec -> __libc_csu_init           (reversed/fileparser_funcs/FUN_00013bec.c)
 * 原 FUN_00013c2c @ 0x00013c2c -> fp_stat_file              (reversed/fileparser_funcs/FUN_00013c2c.c)
 */
