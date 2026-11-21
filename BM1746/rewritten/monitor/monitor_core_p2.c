/* ============================================================================
 * monitor_core_p2.c — miner-monitor 守护进程核心逻辑 (第 2 部分 / 共 2 部分)
 *
 * 来源二进制: sdcard/rootfs_sd/usr/bin/miner-monitor (ARM 32-bit, stripped)
 * 覆盖批次:   rewritten/batch_monitor.tsv 中 batch=mon_core_b2 的 56 个函数
 * 配套文件:   monitor_core_p1.c (main/conf 解析/登录/getstatus/cgminer.conf JSON)
 *             cjson.c (FUN_0000c244..FUN_0000dfd8, cJSON 实现)
 *
 * 主题分区 (按功能块):
 *   [1] C 运行时桩    : entry/_init/_fini/init_array 等 ELF 启动收尾代码
 *   [2] 信号与定时器  : SIGPIPE/SIGINT 处理、itimer、信号掩码
 *   [3] 进程与配置工具: daemonize、字符串/键值行解析、conf 键值表
 *   [4] 包队列        : 9 槽 × 0x100f 环形队列 push/pop/foreach 与包记录复位
 *   [5] SSL/TCP 网络  : 证书打印、SSL 连接、明文 TCP 连接、服务器收包分发
 *   [6] 服务器收发    : 帧发送 (包头+命令+长度)、响应检查、状态上报
 *   [7] 配置更新链路  : deal_cmd → parse_pools → 池参数落盘 (uci/json 两路)
 *   [8] L_cmd 命令链表: 已下发命令的 nonce 登记/配对删除
 *   [9] 工作线程      : 命令处理线程 / 响应处理线程 / api-switch 监视线程
 *
 * 服务器协议帧 (收, 见 recv_server_pkg 于 p1):
 *   +0  2 字节包头 "00" (NUL 结尾) → 记录 pkg_head
 *   +2  2 字节命令串 "XY" (NUL 结尾) → 记录 ser_cmd (+3 起, 与包头共享边界)
 *   +4  2 字节 sum (大端) = 后随数据长度 → 仅入栈临时, 不写入记录
 *   +6  sum 字节数据 → 落位于记录 +0x1008 (日志称 pkgnonce, L_cmd 以此配对)
 *
 * 包记录 pkg_slot_t (0x100f 字节, bss 内四个实例及队列槽位共用该布局):
 *   +0x0000 pkg_head[3]  "00\0"
 *   +0x0003 ser_cmd[3]   命令串 "XY\0" (如 "01"/"06"/"f5")
 *   +0x0006 ser_len[2]   长度域 (接收路径未写, 恒 0; 打印用)
 *   +0x0008 head_pad[8]  原 c/d 两段 dword, 接收路径未写, 恒 0
 *                        (conf 更新命令以 +8 为包体串扫描基址, 见 deal_cmd)
 *   +0x0010 body[0xff8]  包体 (原按值传参的"栈续传区")
 *   +0x1008 nonce[7]     包 nonce — 服务器下发数据实际落位, L_cmd 配对键
 *
 * 说明: 原始固件将 0x100f 字节记录按 AAPCS 拆分传参 (头 12/16 字节进寄存器,
 * 其余 0x1003/0xfff 字节随栈续传), Ghidra 将其渲染为 3~5 个独立参数与调用点
 * 的 memcpy 暂存; 本转写统一以 pkg_slot_t * 传递记录, 语义等价
 * (被暂存/清零的均为传参载体副本, 对记录本体无可观察影响, 详见各函数注释)。
 * ============================================================================ */

#define _GNU_SOURCE 1

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/conf.h>
#include <openssl/engine.h>
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/comp.h>
#include <openssl/x509.h>

/* ---------------------------------------------------------------------------
 * 常量 (魔数命名; 协议/尺寸类魔数均来自反编译与 ELF 数据段核实)
 * ------------------------------------------------------------------------- */
#define PKG_SLOT_SIZE       0x100f              /* 包记录/队列槽位字节数 */
#define PKG_QUEUE_SLOTS     9                   /* 环形队列槽位数 (push/pop/foreach 模数) */
#define PKG_QUEUE_FULL_MOD  20                  /* 0x14: 主循环/分发线程判满模数, 与槽位数不一致, 原样保留 */
#define PKG_NONCE_OFF       0x1008              /* 记录内 nonce 偏移 */
#define PKG_BODY_OFF        0x10                /* 记录内包体偏移 */
#define L_CMD_FULL_LIMIT    0x15                /* L_cmd 计数告警阈值 (count >= 0x15 提示) */
#define L_CMD_NODE_SIZE     8                   /* L_cmd 结点 {data,next} 尺寸 */
#define L_CMD_MAX_COUNT     0x14                /* "L_cmd is full!" 判定上界 (0x15 之前不打) */
#define REVBODY_SIZE        0x1000              /* g_revbody 回发缓冲尺寸 (复位 memset 长度) */
#define UID_FIELD_SIZE      0x28                /* g_api_uid 区域尺寸 (0x26ab8..0x26ae0) */
#define SEND_STATE_BUF_SIZE 0x3c                /* send_cgminer_state 载荷缓冲 */
#define MAC_STR_SIZE        0x28                /* get_iface_mac MAC 串缓冲 */
#define IFREQ_COPY_LEN      0x14                /* get_local_ip strncpy 长度 (原样, 超出 ifr_name) */
#define CONF_KEY_STRIDE     0x81                /* conf 键名数组项步进 */
#define CONF_VAL_STRIDE     0x201               /* conf 值数组项步进 */
#define CONF_KEY_AREA_SIZE  0x1428              /* conf 键名数组整体尺寸 (41×0x81) */
#define POOL_STRIDE         0xa4                /* 池参数项步进 (uci/json conf 与服务器解析区) */
#define POOL_URL_OFF        0x04                /* 池项内 url 偏移 (uci/json conf) */
#define POOL_USER_OFF       0x54                /* 池项内 user 偏移 */
#define POOL_PW_OFF         0x7c                /* 池项内 pw 偏移 */
#define POOL_COUNT_OFF      0x1ec               /* conf 缓冲内池数偏移 (int) */
#define TIMER_SEC_30        30                  /* set_itimer(0x1e,0x1e) */
#define SLEEP_QUEUE_FULL    5                   /* 队列判满等待 */
#define SLEEP_BAD_PKG       1                   /* 坏包等待 */
#define SLEEP_OFFLINE_POLL  30                  /* 0x1e: 离线轮询间隔 */
#define SLEEP_SWITCH_POLL   15                  /* 0xf: api-switch 轮询间隔 */
#define SIGALRM_HDLR_ADDR   fun_0000ab74        /* 原 0xab74: SIGALRM 处理函数, 命名待补 */

/* ---------------------------------------------------------------------------
 * 包记录与队列类型
 * ------------------------------------------------------------------------- */
typedef struct {                    /* 服务器协议包记录 (0x100f 字节), 布局见文件头 */
    char pkg_head[3];               /* +0x0000: "00\0" */
    char ser_cmd[3];                /* +0x0003: 命令串 "XY\0" */
    char ser_len[2];                /* +0x0006: 长度域 (接收路径未写, 恒 0) */
    char head_pad[8];               /* +0x0008: 原 c/d 两段 dword, 恒 0; conf 更新串基址 */
    char body[0xff8];               /* +0x0010: 包体 (按值传参的栈续传区) */
    char nonce[7];                  /* +0x1008: 包 nonce (服务器下发数据落位) */
} pkg_slot_t;                       /* 合计 0x100f 字节 */

typedef struct {                    /* 包环形队列控制块 (16 字节) */
    void *slots;                    /* +0x00: malloc(9*0x100f)=36999 字节槽位首址 */
    int   head;                     /* +0x04: 读指针 (pop) */
    int   tail;                     /* +0x08: 写指针 (push) */
    int   pad;                      /* +0x0c: 未用 (响应队列处被周期清 0) */
} pkg_queue_t;

typedef struct cmd_node {           /* L_cmd 已下发命令结点 */
    pkg_slot_t      *data;          /* +0x00: malloc(0x100f) 命令记录副本 */
    struct cmd_node *next;          /* +0x04 */
} cmd_node_t;

/* 明文/SSL 收发统一入口: 由 main 按 ssl_connect 结果选择包装函数 (原 0x10c84/0x10cac/0x10cd4/0x10d04 系列) */
typedef int (*pkg_xfer_fn)(SSL *ssl, int sockfd, void *buf, int len);

/* ---------------------------------------------------------------------------
 * 全局变量 (与 monitor_core_p1.c 共享; 本文件集中定义, 注释给出原始地址)
 * ------------------------------------------------------------------------- */
char  *g_conf_path        = "/config/minermonitor.conf";  /* 原 DAT_0001b2f0 (.data) */
char  *g_pid_path         = "/tmp/miner-monitor/pid";     /* 原 DAT_0001b2f8 (.data) */
char  *g_cgminer_conf_path = "/config/cgminer.conf";      /* 原 PTR_0001b36c (.data, 值=0x11658) */

int    g_running        = 1;   /* 原 DAT_0001b2ec (.data=1): 主循环运行标志, SIGINT 清 0 */
int    g_r_connect      = 1;   /* 原 DAT_0001b380 (.data=1): TCP 连接状态 (日志名 r_connect) */
int    g_web_switch_off;       /* 原 DAT_0001b394 (.bss): api-switch=off 置 1 */
int    DAT_0001b2f4     = 1;   /* 原 DAT_0001b2f4 (.data=1): 全局, 语义待定, 各工作线程处置 0 */

int    g_var_num;              /* 原 DAT_0001b3a0: conf 键值对个数 (打印名 g_var_num) */

FILE  *g_pid_fp;               /* 原 DAT_0001c3c8: pid 文件句柄 (flock/fclose) */
char  *DAT_0001c3d0;           /* 原 DAT_0001c3d0 (.bss): UCI 配置文件路径; 全局, 语义待定
                                  (赋值点未见于反编译, conf_get_value/save_cgminer_uci_conf 使用) */

pkg_queue_t g_cmd_queue;       /* 原 DAT_0001b3a8: 命令队列 (head=原 DAT_0001b3ac, tail=原 DAT_0001b3b0) */
pkg_queue_t g_respond_queue;   /* 原 DAT_0001d410: 响应队列 (head=原 DAT_0001d414, tail=原 DAT_0001d418) */

pkg_slot_t  g_respond_pkg;     /* 原 DAT_0001b3b8 (0x100f 字节): 响应队列弹出记录 (nonce=原 DAT_0001c3c0) */
pkg_slot_t  g_err_pkg;         /* 原 DAT_0001c3d4 (0x100f 字节): 断线/出错路径复位用记录 */
pkg_slot_t  g_cmd_pkg;         /* 原 DAT_0001d444 (0x100f 字节): 命令队列弹出记录 (nonce=原 DAT_0001e44c) */
pkg_slot_t  g_recv_pkg;        /* 原 DAT_0001e454 (0x100f 字节): 服务器接收记录 (nonce=原 DAT_0001f45c) */

cmd_node_t *g_cmd_list_head;   /* 原 DAT_0001d420: L_cmd 哨兵头结点 */
int         g_cmd_list_count;  /* 原 DAT_0001d440: L_cmd 结点计数 */

pthread_t   g_tid_switch;      /* 原 DAT_0001d404: switch_monitor_thread 线程 id */
pthread_t   g_tid_cmd_deal;    /* 原 DAT_0001f464: cmd_deal_thread 线程 id */
pthread_t   g_tid_respond;     /* 原 DAT_0001c3cc: respond_queue_thread 线程 id */

pkg_xfer_fn g_recv_fn;         /* 原 DAT_0001d434: 收包包装 (SSL_read / recv) */
pkg_xfer_fn g_send_fn;         /* 原 DAT_0001d43c: 发包包装 (SSL_write / send) */
int         g_last_send_ret;   /* 原 DAT_00026f90: 最近一次 send 返回值 */
int         g_last_recv_ret;   /* 原 DAT_00026f94: 最近一次 recv 返回值 */
int         g_sockfd;          /* 原 DAT_000258bc: 服务器 socket fd */
SSL        *g_ssl;             /* 原 DAT_00026f98 */
SSL_CTX    *g_ssl_ctx;         /* 原 DAT_00026f8c */

short       DAT_000258b8;      /* 原 DAT_000258b8: 全局, 语义待定 (复位清 2 字节) */
short       DAT_000258c0;      /* 原 DAT_000258c0: 全局, 语义待定 (复位清 2 字节) */
char        DAT_000258c2;      /* 原 DAT_000258c2: 全局, 语义待定 (复位清 1 字节) */
char        g_revbody[REVBODY_SIZE];  /* 原 DAT_00025ab4: 收包体回发缓冲 (send_revbody_back 读取) */
short       DAT_00026ab4;      /* 原 DAT_00026ab4: 全局, 语义待定 (复位清 2 字节) */
char        DAT_00026ab6;      /* 原 DAT_00026ab6: 全局, 语义待定 (复位清 1 字节) */
char        g_api_uid[UID_FIELD_SIZE]; /* 原 DAT_00026ab8: api-uid (parse_pools_string 写入) */

char        DAT_000258c4[0x1f0]; /* 原 DAT_000258c4: 服务器下发池参数解析区
                                      池 i (i=0..2): +i*0xa4+0x00 池序号串(0/1/2), +0x04 url,
                                      +0x54 user, +0x7c pw; +0x1ec 池数 (int) */
char        DAT_00026ae0[0x28c]; /* 原 DAT_00026ae0: UCI cgminer.conf 内存映像
                                      池 i: +i*0xa4+{0x04 url, 0x54 user, 0x7c pw};
                                      +0x1ec api_allow, +0x200 more_options, +0x20a freq,
                                      +0x219 voltage, +0x223 pool_balance, +0x22d nobeeper,
                                      +0x237 notempoverctrl (均为字符串) */
char        DAT_00026d6c[0x224]; /* 原 DAT_00026d6c: /config/cgminer.conf (JSON) 内存映像, 布局同上 */

char        DAT_0001f468[0x5028];  /* 原 DAT_0001f468: conf 值数组, 步进 0x201 (0x1f468..0x24490) */
char        DAT_00024490[CONF_KEY_AREA_SIZE]; /* 原 DAT_00024490: conf 键名数组, 步进 0x81 (41 项) */

char        g_dtors_done;      /* 原 DAT_0001b390: do_global_dtors_aux 已执行标志 */

/* ---------------------------------------------------------------------------
 * 外部函数声明 — monitor_core_p1.c / cjson.c 提供 (签名以实现方文件为准)
 * ------------------------------------------------------------------------- */
extern int   recv_server_pkg(void);                       /* 原 FUN_0000e8dc */
extern char *monitor_conf_get_value(char *conf_path, char *key, char *value_buf); /* 原 FUN_0000f990 */
extern int   conf_update_api_switch(char *conf_path, char *auth_value);           /* 原 FUN_0000fa54 */
extern void  send_login_pkg(char *cmd_str);               /* 原 FUN_0000edfc */
extern int   send_getstatus_res(char *ser_cmd);           /* 原 FUN_0000f108 */
extern int   restart_cgminer(void);                       /* 原 FUN_0000f4c0 */
extern char *parse_pools_string(char *pool_str, char *pool_conf);   /* 原 FUN_0000feb0, 返回 pool_conf */
extern char *load_cgminer_conf_json(char *json_path, char *conf_buf); /* 原 FUN_00010004, 返回 conf_buf */
extern char *apply_pool_changes(char *srv_pools, char *json_conf);          /* 原 FUN_00010474, 返回 json_conf */
extern void  save_cgminer_conf_json(char *conf_buf, char *json_path);       /* 原 FUN_00010210 */
extern int   parse_conf_file(char *conf_path);            /* 原 FUN_0000b46c */
extern int   main(int argc, char **argv, char **envp);    /* 原 FUN_00009fa0 (p1) */
/* SIGALRM 处理函数 (原 0xab74, name_map 未收录) */
extern void fun_0000ab74(void); /* 命名待补 */
/* C 运行时桩 (entry/libc_csu_init 使用) */
extern void __gmon_start__(void);                         /* gprof 剖析启动桩 (弱符号) */
extern int  __libc_start_main(int (*main_fn)(int, char **, char **), int argc, char **argv,
                              void (*init)(int, char **, char **), void (*fini)(void),
                              void (*rtld_fini)(void));   /* 原 _start 调用形态 (Ghidra 视图, 略第 7 参 stack_end) */

/* ---------------------------------------------------------------------------
 * 本文件内函数前置声明
 * ------------------------------------------------------------------------- */
void call_gmon_start(void);
void deregister_tm_clones(void);
void _INIT_0(void);
void _FINI_0(void);
int  _init(void);
void _fini(void);
void libc_csu_init(int argc, char **argv, char **envp);
void entry(uint32_t rtld_fini, uint32_t argc, char *stack_argv0);
void sigpipe_handler(void);
void sigint_term_handler(void);
void set_itimer(time_t interval_sec, time_t value_sec);
void setup_timer_signals(int unused_sec);
void init_signal_mask(void);
void respond_queue_thread(void);
void cmd_deal_thread(void);
int  switch_monitor_thread(void);
void print_peer_certificate(SSL *ssl);
int  ssl_connect(void);
int  sock_connect_server(void);
int  recv_serpkg_struct(void);
void pkg_recv_dispatch(void);
void daemonize(void);
void str_rstrip_char(char *s, uint32_t c);
int  parse_kv_line(char **line, char **key, char **value);
void print_conf_vars(void);
char *conf_get_value(char *key);
char *gen_pkg_nonce(char *out);
int  get_local_ip(char *iface, char *ip_out);
char *get_iface_mac(char *out, char *iface);
int  init_pkg_queues(pkg_queue_t *cmd_q, pkg_queue_t *resp_q);
int  free_pkg_queue(pkg_queue_t *q);
int  clear_pkg_queue(pkg_queue_t *q);
bool pkg_queue_copy_slot(const char *slots, int src_idx, int cmp_idx, void *dst);
bool pkg_queue_push(pkg_queue_t *q, const pkg_slot_t *pkg);
int  pkg_queue_pop(pkg_queue_t *q, pkg_slot_t *out);
int  pkg_queue_foreach(const char *slots, int head, int tail,
                       void (*cb)(const pkg_slot_t *slot));
void print_pkg_node(const pkg_slot_t *slot);
bool pkg_queue_not_empty(pkg_queue_t *q);
void pkg_globals_reset(const pkg_slot_t *pkg);
void check_server_respond(void);
int  send_server_pkg(char *ser_cmd, uint32_t body_len);
int  send_revbody_back(const pkg_slot_t *pkg);
int  update_cgminer_conf(const pkg_slot_t *pkg);
int  update_cgminer_uci_conf(const pkg_slot_t *pkg);
void send_cgminer_state(char *ser_cmd, char *result);
int  deal_cmd(pkg_slot_t *pkg);
char *load_cgminer_uci_conf(char *uci_conf);
char *apply_uci_pool_changes(char *srv_pools, char *uci_conf);
void save_cgminer_uci_conf(char *uci_conf);
int  write_auth_tmp_file(char *auth_str);
void cmd_list_init(void *unused_head);
cmd_node_t *cmd_list_find(const pkg_slot_t *pkg, cmd_node_t *head);
cmd_node_t *cmd_list_find_prev(const pkg_slot_t *pkg, cmd_node_t *prev);
void cmd_list_delete(const pkg_slot_t *pkg, cmd_node_t *head);
void cmd_list_add(const pkg_slot_t *pkg, cmd_node_t *unused_head, cmd_node_t *head);
void cmd_list_free(cmd_node_t *head);

/* ==========================================================================
 * [1] C 运行时桩 (ELF 启动/收尾)
 * ======================================================================== */

// === call_gmon_start === 原 FUN_0000a2e8 @ 0x0000a2e8
// 来源: reversed/monitor_funcs/FUN_0000a2e8.c | 分类: monitor
// 语义: _init 转发调用 gprof 剖析启动桩 __gmon_start__ (弱符号)。
//       原反编译已将 "非空判跳" 块裁剪为不可达, 故此处为无条件调用 (原样保留)。
void call_gmon_start(void)
{
    __gmon_start__();
}

// === deregister_tm_clones === 原 FUN_0000a30c @ 0x0000a30c
// 来源: reversed/monitor_funcs/FUN_0000a30c.c | 分类: monitor
// 语义: TLS 克隆注销桩; 原反编译全部代码块被标记不可达后消除, 函数体为空 (原样保留)。
void deregister_tm_clones(void)
{
}

// === sigpipe_handler === 原 FUN_0000a3e0 @ 0x0000a3e0
// 来源: reversed/monitor_funcs/FUN_0000a3e0.c | 分类: monitor
// 语义: SIGPIPE (13) 处理: 仅打印提示, 不退出 (send/recv 断管后由上层判定离线)。
void sigpipe_handler(void)
{
    puts("send or recv: Broken pipe");
}

// === sigint_term_handler === 原 FUN_0000a3ec @ 0x0000a3ec
// 来源: reversed/monitor_funcs/FUN_0000a3ec.c | 分类: monitor
// 语义: SIGINT(2)/SIGTERM(15) 处理: 解锁并关闭 pid 文件 → 释放 L_cmd → 停线程 →
//       释放两个包队列 → SSL/OpenSSL 全量清理 → exit(0)。
void sigint_term_handler(void)
{
    int ret;

    puts("oops! stop by ctrl+c or kill!!!----------------------------------------");
    ret = flock(fileno(g_pid_fp), LOCK_UN);              /* 8 = LOCK_UN */
    if (ret == 0)
        puts("unlock fd_pid_conf succeed!");
    else
        puts("unlock fd_pid_conf failed!");
    fclose(g_pid_fp);
    cmd_list_free(g_cmd_list_head);
    g_running = 0;
    pthread_join(g_tid_switch, NULL);
    pthread_join(g_tid_cmd_deal, NULL);
    pthread_join(g_tid_respond, NULL);
    free_pkg_queue(&g_cmd_queue);
    free_pkg_queue(&g_respond_queue);
    SSL_shutdown(g_ssl);
    SSL_free(g_ssl);
    close(g_sockfd);
    SSL_CTX_free(g_ssl_ctx);
    CONF_modules_free();
    ERR_remove_state(0);
    ENGINE_cleanup();
    CONF_modules_unload(1);
    ERR_free_strings();
    EVP_cleanup();
    CRYPTO_cleanup_all_ex_data();
    sk_free((_STACK *)SSL_COMP_get_compression_methods());
    exit(0);
}

// === _INIT_0 (frame_dummy) === 原 FUN_0000a3ac @ 0x0000a3ac
// 来源: reversed/monitor_funcs/_INIT_0.c | 分类: monitor
// 语义: .init_array 首项 (frame_dummy); 原反编译中 register_tm_clones 等块被
//       标记不可达后消除, 函数体为空 (原样保留)。
void _INIT_0(void)
{
}

// === _FINI_0 (do_global_dtors_aux) === 原 FUN_0000a384 @ 0x0000a384
// 来源: reversed/monitor_funcs/_FINI_0.c | 分类: monitor
// 语义: 退出期全局析构桩: 以 g_dtors_done 守卫保证 deregister_tm_clones 仅执行一次。
void _FINI_0(void)
{
    if (g_dtors_done == '\0') {
        deregister_tm_clones();
        g_dtors_done = 1;
    }
}

// === _init === 原 FUN_00009a58 @ 0x00009a58
// 来源: reversed/monitor_funcs/_init.c | 分类: monitor
// 语义: ELF .init 节入口: 转发调用 call_gmon_start; 返回值原为寄存器残留, 无消费者。
int _init(void)
{
    call_gmon_start();
    return 0;
}

// === _fini === 原 FUN_00011048 @ 0x00011048
// 来源: reversed/monitor_funcs/_fini.c | 分类: monitor
// 语义: ELF .fini 节入口, 函数体为空 (原样保留)。
void _fini(void)
{
}

// === libc_csu_init === 原 FUN_00010fe0 @ 0x00010fe0
// 来源: reversed/monitor_funcs/FUN_00010fe0.c | 分类: monitor
// 语义: glibc csu 初始化: 先调 _init(), 再遍历 .init_array (原 0x1b000 起,
//       本固件仅 1 项) 以 (argc, argv, envp) 调用各初始化函数。
void libc_csu_init(int argc, char **argv, char **envp)
{
    int i;

    _init();                          /* 原以 argc 传参但 _init 未使用 */
    for (i = 0; i != 1; i++) {        /* 原循环计数由 (init_end-init_start)/4 化简为 1 */
        void (**init_fn)(int, char **, char **)
            = (void (**)(int, char **, char **))0x0001b000; /* 原指针初值 0x1affc+4 (指向 .init_array) */
        init_fn[i](argc, argv, envp);
    }
}

// === entry (_start) === 原 FUN_0000a2ac @ 0x0000a2ac
// 来源: reversed/monitor_funcs/entry.c | 分类: monitor
// 语义: ELF 入口: __libc_start_main(main, argc, &栈上argv区, libc_csu_init,
//       _fini, rtld_fini) 后永不返回; abort 为不可达兜底 (原样保留)。
void entry(uint32_t rtld_fini, uint32_t argc, char *stack_argv0)
{
    __libc_start_main(main, (int)argc, &stack_argv0,   /* 第 3 参取栈上 argv/envp 区首址 */
                      libc_csu_init,
                      (void (*)(void))0x00011044 /* 原 &DAT_00011044 (_fini 入口) */,
                      (void (*)(void))(uintptr_t)rtld_fini);
    abort();
}

/* ==========================================================================
 * [2] 信号与定时器
 * ======================================================================== */

// === set_itimer === 原 FUN_0000ace4 @ 0x0000ace4
// 来源: reversed/monitor_funcs/FUN_0000ace4.c | 分类: monitor
// 语义: 配置 ITIMER_REAL: 周期间隔=interval_sec, 首次触发=value_sec (秒, 微秒清 0)。
void set_itimer(time_t interval_sec, time_t value_sec)
{
    struct itimerval timer;

    timer.it_interval.tv_usec = 0;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = interval_sec;
    timer.it_value.tv_sec = value_sec;
    setitimer(ITIMER_REAL, &timer, NULL);
}

// === setup_timer_signals === 原 FUN_0000ae1c @ 0x0000ae1c
// 来源: reversed/monitor_funcs/FUN_0000ae1c.c | 分类: monitor
// 语义: 启动 30 秒周期 REAL 定时器并挂 SIGALRM(14)/SIGPIPE(13) 处理函数。
//       原调用另携参数 (main 传入 0x1e, 函数未使用; 按值传参续传残留), 略。
void setup_timer_signals(int unused_sec)
{
    (void)unused_sec;
    set_itimer(TIMER_SEC_30, TIMER_SEC_30);              /* 原 set_itimer(0x1e, 0x1e) */
    signal(SIGALRM, (void (*)(int))SIGALRM_HDLR_ADDR);   /* 原 signal(0xe, 0xab74) 命名待补 */
    signal(SIGPIPE, sigpipe_handler);                    /* 原 signal(0xd, ...) */
}

// === init_signal_mask === 原 FUN_0000afe8 @ 0x0000afe8
// 来源: reversed/monitor_funcs/FUN_0000afe8.c | 分类: monitor
// 语义: 构造全信号掩码并放行 SIGINT(2)/SIGPIPE(13); 注意原函数未调用
//       sigprocmask 安装, 掩码构造后即弃 (原样保留)。
void init_signal_mask(void)
{
    sigset_t mask;

    sigemptyset(&mask);
    sigfillset(&mask);
    sigdelset(&mask, SIGINT);      /* 原 sigdelset(&mask, 2) */
    sigdelset(&mask, SIGPIPE);     /* 原 sigdelset(&mask, 0xd) */
}

/* ==========================================================================
 * [3] 进程与配置工具 (守护化 / 字符串与键值行解析 / conf 键值表)
 * ======================================================================== */

// === daemonize === 原 FUN_0000b020 @ 0x0000b020
// 来源: reversed/monitor_funcs/FUN_0000b020.c | 分类: monitor
// 语义: 双 fork 守护化: 父/祖父退出, 孙进程 setsid 后关闭 0..255 全部描述符,
//       chdir("/root/test") 并 umask(0); 任一 fork 失败 exit(1)。
void daemonize(void)
{
    pid_t pid;

    pid = fork();
    if (pid < 0)
        exit(1);                      /* 原 0xb074 标签 */
    if (pid == 0) {
        setsid();
        pid = fork();
        if (pid < 1) {
            if (pid == 0) {
                int fd = 0;
                int next;
                do {
                    next = fd + 1;
                    close(fd);
                    fd = next;
                } while (next != 0x100);      /* 关闭 0..255 */
                chdir("/root/test");
                umask(0);
                return;                       /* 孙进程继续执行 */
            }
            exit(1);                          /* fork 失败 */
        }
    }
    exit(0);                                  /* 父进程/祖父进程退出 */
}

// === str_rstrip_char === 原 FUN_0000b208 @ 0x0000b208
// 来源: reversed/monitor_funcs/FUN_0000b208.c | 分类: monitor
// 语义: 去除字符串尾部所有指定字符 c (逐个以 NUL 覆盖), 原地修改。
void str_rstrip_char(char *s, uint32_t c)
{
    size_t len = strlen(s);
    char *p;

    if (len == 0)
        return;
    p = s + len;
    if ((unsigned char)p[-1] != (unsigned char)c)    /* 尾字符不是 c: 不动 */
        return;
    while (len > 0 && (unsigned char)p[-1] == (unsigned char)c) {
        p[-1] = '\0';
        len--;
        p--;
    }
}

// === parse_kv_line === 原 FUN_0000b250 @ 0x0000b250
// 来源: reversed/monitor_funcs/FUN_0000b250.c | 分类: monitor
// 语义: 解析一行 "key = \"value\"" (原为 parse_conf_file_kv 的行内扫描器):
//   - 跳过行首空白; 键终止于空白或 '=' (该字符被改写为 NUL);
//   - 值必须以 '"' 开始, 截至下一个 '"';
//   - *line 只读不推进; *key/*value 输出键/值首址;
//   - 返回: 值前最后一个被跳过字符的 isspace 标志 (0x2000 或 0),
//     空行/无值/无引号/无闭引号返回 -1 (0xffffffff)。
int parse_kv_line(char **line, char **key, char **value)
{
    unsigned char *p = (unsigned char *)*line;
    int space_flag;

    if (p == NULL)
        return -1;
    while (isspace(*p))                             /* 原 __ctype_b 0x2000 = _ISSpace */
        p++;
    if (*p == '\0')                                 /* 全空白行 */
        return -1;
    *key = (char *)p;
    while (!isspace(*p) && *p != '=') {             /* 键扫描: 至空白/'=' 止 */
        p++;
        if (*p == '\0')
            return -1;
    }
    *p = '\0';                                      /* 键终止符 (空格或 '=') 改写为 NUL */
    for (;;) {
        unsigned char c = p[1];
        if (c == '\0')                              /* 无值段 */
            break;
        space_flag = isspace(c) ? 0x2000 : 0;       /* 原取 __ctype_b 表项 & 0x2000 */
        if (!isspace(c) && c != '=') {
            char *quote;
            if (c != '"')                           /* 值必须以引号开始 */
                return -1;
            *value = (char *)(p + 2);               /* 越过开引号 */
            quote = strchr((char *)(p + 2), '"');
            if (quote == NULL)
                return -1;
            *quote = '\0';                          /* 闭引号改写为 NUL */
            return space_flag;
        }
        p++;                                        /* 继续跳过空白 / '=' */
    }
    return -1;
}

// === print_conf_vars === 原 FUN_0000b9c4 @ 0x0000b9c4 /* 无调用方, 疑旧路径 */
// 来源: reversed/monitor_funcs/FUN_0000b9c4.c | 分类: monitor
// 语义: 打印 conf 键值表全部 g_var_num 项 ("键 = 值"), 键步进 0x81, 值步进 0x201。
void print_conf_vars(void)
{
    int i;

    printf("g_var_num == %d\n", g_var_num);
    if (g_var_num < 1)
        return;
    for (i = 0; i < g_var_num; i++)
        printf("%s = %s\n", &DAT_00024490[i * CONF_KEY_STRIDE],
                            &DAT_0001f468[i * CONF_VAL_STRIDE]);
}

// === conf_get_value === 原 FUN_0000ba78 @ 0x0000ba78
// 来源: reversed/monitor_funcs/FUN_0000ba78.c | 分类: monitor
// 语义: 重新解析 UCI conf (DAT_0001c3d0) 建表后按键 (忽略大小写) 查值:
//   命中 → 清表 (计数归 0, 键名区清零) 并返回值首址 (值区不受清表影响);
//   未命中 → 清表并打印 "get %s failed", 返回 NULL。
char *conf_get_value(char *key)
{
    int i, n;

    parse_conf_file(DAT_0001c3d0);
    n = g_var_num;
    if (n > 0) {
        for (i = 0; i != n; i++) {
            if (strcasecmp(&DAT_00024490[i * CONF_KEY_STRIDE], key) == 0) {
                g_var_num = 0;
                memset(DAT_00024490, 0, CONF_KEY_AREA_SIZE);
                return &DAT_0001f468[i * CONF_VAL_STRIDE];
            }
        }
    }
    memset(DAT_00024490, 0, CONF_KEY_AREA_SIZE);
    g_var_num = 0;
    printf("get %s failed\n", key);
    return NULL;
}

/* ==========================================================================
 * [4] 包队列 (9 槽 × 0x100f 环形队列) 与包记录复位
 * ======================================================================== */

// === gen_pkg_nonce === 原 FUN_0000e1f8 @ 0x0000e1f8
// 来源: reversed/monitor_funcs/FUN_0000e1f8.c | 分类: monitor
// 语义: 以 gettimeofday 微秒值生成 6 位十进制 nonce (不足 6 位左补 '0'),
//       写入 out (原 DAT_0001e44c: 弹出命令记录的 nonce 字段) 并返回 out。
char *gen_pkg_nonce(char *out)
{
    char usec_str[8];
    struct timeval tv;
    uint32_t pad;
    size_t len;

    gettimeofday(&tv, NULL);
    sprintf(usec_str, "%ld", tv.tv_usec);
    memset(out, 0, 7);                              /* 原逐字节置 out[0..6] = 0 */
    len = strlen(usec_str);
    pad = (uint32_t)(6 - len) & 0xff;               /* 原样 & 0xff 截断 */
    if (pad != 0)
        memset(out, '0', pad);
    strcat(out, usec_str);
    return out;
}

// === get_local_ip === 原 FUN_0000e284 @ 0x0000e284
// 来源: reversed/monitor_funcs/FUN_0000e284.c | 分类: monitor
// 语义: SIOCGIFADDR 取指定网卡 IPv4 点分串写入 ip_out; 成功返回 1,
//       socket 失败/ioctl 失败返回 -1 (错误路径不 close, 原样保留)。
int get_local_ip(char *iface, char *ip_out)
{
    struct ifreq ifr;
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (fd == -1) {
        perror("socket");
        return -1;
    }
    strncpy(ifr.ifr_name, iface, IFREQ_COPY_LEN);   /* 原样拷 0x14 字节 (含越界段) */
    ifr.ifr_name[15] = '0';                         /* 原帧内 +0x0f 字节置 0x30 ('0') */
    if (ioctl(fd, SIOCGIFADDR, &ifr) < 0) {
        printf(":No Such Device %s/n", iface);      /* 原串即为 "/n" 结尾 */
        return -1;
    }
    strcpy(ip_out, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
    close(fd);
    return 1;
}

// === get_iface_mac === 原 FUN_0000e340 @ 0x0000e340
// 来源: reversed/monitor_funcs/FUN_0000e340.c | 分类: monitor
// 语义: SIOCGIFHWADDR 取网卡 MAC, 组装 "MAC=XX:XX:XX:XX:XX:XX," 到 out
//       (首 4 字节原以 dword 0x3d43414d 即 "MAC=" 写入); 返回 out。
//       socket/ioctl 失败时 perror 后 exit(0) (原样保留)。
char *get_iface_mac(char *out, char *iface)
{
    char *mac_str = calloc(MAC_STR_SIZE, 1);
    struct ifreq ifr;
    int fd;
    unsigned char *mac;
    char *end;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        exit(0);
    }
    strcpy(ifr.ifr_name, iface);
    if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0) {
        perror("ioctl");
        exit(0);
    }
    close(fd);
    mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;  /* 原栈帧 ifr+18 (sockaddr sa_data) */
    sprintf(mac_str, "%02X:%02X:%02X:%02X:%02X:%02X",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    *(uint32_t *)out = 0x3d43414d;                  /* "MAC=" (小端四字节, 原样) */
    end = stpcpy(out + 4, mac_str);
    end[0] = ',';
    end[1] = '\0';
    free(mac_str);
    return out;
}

// === init_pkg_queues === 原 FUN_0000e444 @ 0x0000e444
// 来源: reversed/monitor_funcs/FUN_0000e444.c | 分类: monitor
// 语义: 为命令/响应两个队列各 malloc 36999 (=9×0x100f) 字节槽位区并清读写指针;
//       任一 malloc 失败 exit(0) (原样保留); 成功返回 1。
int init_pkg_queues(pkg_queue_t *cmd_q, pkg_queue_t *resp_q)
{
    cmd_q->slots = malloc(PKG_QUEUE_SLOTS * PKG_SLOT_SIZE);   /* 36999 字节 */
    if (cmd_q->slots != NULL) {
        cmd_q->tail = 0;                                      /* 原顺序: [2] 先于 [1] */
        cmd_q->head = 0;
        resp_q->slots = malloc(PKG_QUEUE_SLOTS * PKG_SLOT_SIZE);
        if (resp_q->slots != NULL) {
            resp_q->tail = 0;
            resp_q->head = 0;
            return 1;
        }
    }
    exit(0);
}

// === free_pkg_queue === 原 FUN_0000e49c @ 0x0000e49c
// 来源: reversed/monitor_funcs/FUN_0000e49c.c | 分类: monitor
// 语义: 释放队列槽位区并复位控制块; 返回 1。
int free_pkg_queue(pkg_queue_t *q)
{
    if (q->slots != NULL)
        free(q->slots);
    q->slots = NULL;
    q->tail = 0;                    /* 原顺序: [2]=0, [1]=0 */
    q->head = 0;
    return 1;
}

// === clear_pkg_queue === 原 FUN_0000e4cc @ 0x0000e4cc
// 来源: reversed/monitor_funcs/FUN_0000e4cc.c | 分类: monitor
// 语义: 仅清空队列读写指针 (不释放槽位区); 返回 1。
int clear_pkg_queue(pkg_queue_t *q)
{
    q->tail = 0;                    /* 原 +8 先清 */
    q->head = 0;                    /* 原 +4 后清 */
    return 1;
}

// === pkg_queue_copy_slot === 原 FUN_0000e54c @ 0x0000e54c /* 无调用方, 疑旧路径 */
// 来源: reversed/monitor_funcs/FUN_0000e54c.c | 分类: monitor
// 语义: 若 cmp_idx != src_idx 则把槽位区第 src_idx 项 (0x100f 字节) 拷到 dst;
//       返回是否发生了拷贝。
bool pkg_queue_copy_slot(const char *slots, int src_idx, int cmp_idx, void *dst)
{
    if (cmp_idx != src_idx)
        memcpy(dst, slots + (size_t)src_idx * PKG_SLOT_SIZE, PKG_SLOT_SIZE);
    return cmp_idx != src_idx;
}

// === pkg_queue_push === 原 FUN_0000e598 @ 0x0000e598
// 来源: reversed/monitor_funcs/FUN_0000e598.c | 分类: monitor
// 语义: 记录入队: 若 (tail+1)%9 != head 将整条 0x100f 记录拷入 tail 槽并推进 tail;
//       返回是否入队成功 (队满返回 false)。原实现以寄存器+栈续传按值收记录,
//       此处以指针承载同一 0x100f 字节区, 语义等价。
bool pkg_queue_push(pkg_queue_t *q, const pkg_slot_t *pkg)
{
    bool ok = (q->tail + 1) % PKG_QUEUE_SLOTS != q->head;

    if (ok) {
        memcpy((char *)q->slots + (size_t)q->tail * PKG_SLOT_SIZE, pkg, PKG_SLOT_SIZE);
        q->tail = (q->tail + 1) % PKG_QUEUE_SLOTS;
    }
    return ok;
}

// === pkg_queue_pop === 原 FUN_0000e62c @ 0x0000e62c
// 来源: reversed/monitor_funcs/FUN_0000e62c.c | 分类: monitor
// 语义: 出队: 队空 (head==tail) 返回 0; 否则将 head 槽 0x100f 字节拷到 out,
//       推进 head, 返回 1。
int pkg_queue_pop(pkg_queue_t *q, pkg_slot_t *out)
{
    if (q->head == q->tail)
        return 0;
    memcpy(out, (char *)q->slots + (size_t)q->head * PKG_SLOT_SIZE, PKG_SLOT_SIZE);
    q->head = (q->head + 1) % PKG_QUEUE_SLOTS;
    return 1;
}

// === pkg_queue_foreach === 原 FUN_0000e698 @ 0x0000e698
// 来源: reversed/monitor_funcs/FUN_0000e698.c | 分类: monitor
// 语义: 遍历 [head, tail) 槽位, 对每条记录调用回调 (原按寄存器传 16 字节头,
//       此处传槽位指针, 回调内字段偏移不变); 末尾必 putchar('\n'); 返回 1。
int pkg_queue_foreach(const char *slots, int head, int tail,
                      void (*cb)(const pkg_slot_t *slot))
{
    while (head != tail) {
        cb((const pkg_slot_t *)(slots + (size_t)head * PKG_SLOT_SIZE));
        head = (head + 1) % PKG_QUEUE_SLOTS;
    }
    putchar('\n');                  /* 原 putchar(10) */
    return 1;
}

// === print_pkg_node === 原 FUN_0000e75c @ 0x0000e75c
// 来源: reversed/monitor_funcs/FUN_0000e75c.c | 分类: monitor
// 语义: 打印包记录四段: 包头(+0)/命令(+3)/长度(+6)/body(+8)。
//       body 段指向 +0x08 (head_pad), 接收路径恒为空串, 原样保留。
void print_pkg_node(const pkg_slot_t *slot)
{
    printf("first_2byte=%s  ser_cmd=%s ser_len=%s body=%s\n",
           slot->pkg_head, slot->ser_cmd, slot->ser_len, slot->head_pad);
}

// === pkg_queue_not_empty === 原 FUN_0000e7d8 @ 0x0000e7d8
// 来源: reversed/monitor_funcs/FUN_0000e7d8.c | 分类: monitor
// 语义: 队列非空判定: head != tail。
bool pkg_queue_not_empty(pkg_queue_t *q)
{
    return q->head != q->tail;
}

// === pkg_globals_reset === 原 FUN_0000e7f4 @ 0x0000e7f4
// 来源: reversed/monitor_funcs/FUN_0000e7f4.c | 分类: monitor
// 语义: 复位"当前包"相关全局状态: 清 revbody 缓冲 (0x25ab4, 0x1000 字节)、
//       清 0x258b8/0x258c0/0x258c2/0x26ab4/0x26ab6 与 uid 区 0x26ab8..0x26ae0。
//       注: 原实现按值收 16 字节包头(寄存器)+0xff8 包体+7 字节 nonce(栈续传),
//       并把落在本帧栈上的包体副本 memset 0 — 该副本仅为传参载体, 与包记录
//       本体互不影响, 故此处不再复制/写回记录; 参数仅为保留调用形状。
void pkg_globals_reset(const pkg_slot_t *pkg)
{
    (void)pkg;

    DAT_000258c0 = 0;               /* 原 strh r4,[0x258c0] */
    DAT_000258c2 = 0;               /* 原 strb r4,[0x258c2] */
    DAT_00026ab4 = 0;               /* 原 strh r4,[0x26ab4] */
    DAT_00026ab6 = 0;               /* 原 strb r4,[0x26ab6] */
    DAT_000258b8 = 0;               /* 原 strh r4,[0x258b8] */
    memset(g_revbody, 0, REVBODY_SIZE);
    memset(g_api_uid, 0, UID_FIELD_SIZE);   /* 原以 10 个 dword 存 0 清 0x26ab8..0x26ae0 */
}

/* ==========================================================================
 * [5] SSL/TCP 网络连接与接收分发
 * ======================================================================== */

// === print_peer_certificate === 原 FUN_0000a8a0 @ 0x0000a8a0
// 来源: reversed/monitor_funcs/FUN_0000a8a0.c | 分类: monitor
// 语义: 打印 SSL 对端证书主题/签发者; 无证书时打印提示并返回。
void print_peer_certificate(SSL *ssl)
{
    X509 *cert;
    X509_NAME *name;
    char *oneline;

    cert = SSL_get_peer_certificate(ssl);
    if (cert == NULL) {
        puts("No certificate information\uff01");     /* 原串含全角'！' (0xefbc81) */
        return;
    }
    puts("A digital certificate information:");
    name = X509_get_subject_name(cert);
    oneline = X509_NAME_oneline(name, NULL, 0);
    printf("certificate: %s\n", oneline);
    free(oneline);
    name = X509_get_issuer_name(cert);
    oneline = X509_NAME_oneline(name, NULL, 0);
    printf("Issued by the: %s\n", oneline);
    free(oneline);
    X509_free(cert);
}

// === ssl_connect === 原 FUN_0000a938 @ 0x0000a938
// 来源: reversed/monitor_funcs/FUN_0000a938.c | 分类: monitor
// 语义: TLSv1 客户端握手: 建 SSL_CTX/SSL 并在 g_sockfd 上 SSL_connect;
//       成功且套件非 "(NONE)" 时打印加密套件、打印对端证书并返回 1;
//       CTX 创建失败 exit(1); 握手失败/无套件打印错误栈返回 -1。
int ssl_connect(void)
{
    const SSL_METHOD *meth;
    const SSL_CIPHER *cipher;
    const char *cipher_name;
    int ret;

    SSL_library_init();
    OPENSSL_add_all_algorithms_noconf();
    SSL_load_error_strings();
    meth = TLSv1_client_method();
    g_ssl_ctx = SSL_CTX_new(meth);
    if (g_ssl_ctx == NULL) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
    g_ssl = SSL_new(g_ssl_ctx);
    SSL_set_fd(g_ssl, g_sockfd);
    ret = SSL_connect(g_ssl);
    if (ret != -1) {
        cipher = SSL_get_current_cipher(g_ssl);
        cipher_name = SSL_CIPHER_get_name(cipher);
        if (strcmp("(NONE)", cipher_name) != 0) {
            cipher = SSL_get_current_cipher(g_ssl);       /* 原样二次获取 */
            printf("Connected with %s encryption\n", SSL_CIPHER_get_name(cipher));
            print_peer_certificate(g_ssl);
            return 1;
        }
    }
    ERR_print_errors_fp(stderr);
    return -1;
}

// === sock_connect_server === 原 FUN_0000aa14 @ 0x0000aa14
// 来源: reversed/monitor_funcs/FUN_0000aa14.c | 分类: monitor
// 语义: 明文 TCP 连接服务器: 从 minermonitor.conf 读 "ip_server" (格式 ip:port),
//       域名解析后 connect; 成功置 g_r_connect=1, 失败置 0; 返回 connect 返回值。
//       各解析/系统调用失败处 perror 后 exit(1) (原样保留, 无 ':' 时解引用 NULL)。
int sock_connect_server(void)
{
    char host_buf[60];
    char *colon;
    long port;
    uint16_t port_be;
    struct hostent *he;
    struct sockaddr_in sa;
    int ret;

    if (g_web_switch_off == 1) {
        puts("Web  Switch is OFF!");
        exit(1);
    }
    memset(host_buf, 0, 0x3c);
    monitor_conf_get_value(g_conf_path, "ip_server", host_buf);
    colon = strchr(host_buf, ':');
    *colon = '\0';                                  /* 原样: 无 ':' 时为 NULL 解引用 */
    port = strtol(colon + 1, NULL, 10);
    he = gethostbyname(host_buf);
    if (he == NULL) {
        perror("gethostbyname");
        exit(1);
    }
    g_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (g_sockfd == -1) {
        perror("socket");
        exit(1);
    }
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;                        /* 原 sa_family = 2 */
    /* 原式: (port>>8)&0xff | (port<<8) — 端口大端序 (htons 等价), 按原式保留 */
    port_be = (uint16_t)(((uint32_t)port >> 8) & 0xff | (uint16_t)(port << 8));
    memcpy(&sa.sin_port, &port_be, 2);
    memcpy(&sa.sin_addr, *he->h_addr_list, 4);      /* h_addr_list[0] 4 字节 */
    memset(&sa.sin_zero, 0, sizeof(sa.sin_zero));   /* 原 sa_data[6..13] = 0 */
    ret = connect(g_sockfd, (struct sockaddr *)&sa, 0x10);
    if (ret == -1) {
        puts("connect failed");
        g_r_connect = 0;
    } else {
        g_r_connect = 1;
        puts("server connected");
    }
    return ret;
}

// === recv_serpkg_struct === 原 FUN_0000adb0 @ 0x0000adb0
// 来源: reversed/monitor_funcs/FUN_0000adb0.c | 分类: monitor
// 语义: 收一条服务器记录 (转调 recv_server_pkg, p1):
//   返回>0 正常; 返回 -1 原样上抛; 返回 0 (离线) → 打印提示后进入
//   "已连接则反复收 / 离线则 sleep(30) 后再收" 循环, 直到收到 (>0) 返回 1。
int recv_serpkg_struct(void)
{
    int ret = recv_server_pkg();

    if (ret != 0) {
        if (ret == -1)
            return -1;
        return 1;
    }
    puts("OFF LINE !!!!Please Wait!!! in recv_serpkg_struct() in client");
    do {
        while (g_r_connect != 0) {
            ret = recv_server_pkg();
            if (ret > 0)
                return 1;
        }
        sleep(SLEEP_OFFLINE_POLL);
        ret = recv_server_pkg();
    } while (ret < 1);
    return 1;
}

// === pkg_recv_dispatch === 原 FUN_0000ae50 @ 0x0000ae50 /* 无调用方, 疑旧路径 */
// 来源: reversed/monitor_funcs/FUN_0000ae50.c | 分类: monitor
// 语义: 收包分发主循环 (main 内联同型循环): 两队列判满 (模 20, 与槽位数 9
//       不一致, 原样保留) 则等待; 否则收包, nonce 首字符为 'f' 入响应队列,
//       否则入命令队列; 坏包 (-1) 等待 1 秒。
//       注: 原 push 前的 memcpy(暂存包体续传) 为按值传参载体, 等价省略。
void pkg_recv_dispatch(void)
{
    while (g_running != '\0') {
        if ((g_cmd_queue.tail + 1) % PKG_QUEUE_FULL_MOD == g_cmd_queue.head ||
            (g_respond_queue.tail + 1) % PKG_QUEUE_FULL_MOD == g_respond_queue.head) {
            puts("the queue is full");
            sleep(SLEEP_QUEUE_FULL);
        } else {
            int ret = recv_serpkg_struct();
            if (ret == -1) {
                puts("Bad Package REV!");
                sleep(SLEEP_BAD_PKG);
            } else if (g_recv_pkg.ser_cmd[0] == 'f') {
                puts("this is respond ");
                pkg_queue_push(&g_respond_queue, &g_recv_pkg);
            } else {
                puts("this is command");
                pkg_queue_push(&g_cmd_queue, &g_recv_pkg);
            }
        }
    }
}

/* ==========================================================================
 * [6] 服务器收发 (帧发送 / 响应检查 / 状态上报)
 * ======================================================================== */

// === check_server_respond === 原 FUN_0000eb78 @ 0x0000eb78
// 来源: reversed/monitor_funcs/FUN_0000eb78.c | 分类: monitor
// 语义: 检查弹出响应记录 (g_respond_pkg) 的命令串 "f1".."f7":
//   F1..F4/F6/F7 → 打印后 exit(0) (F6 额外打印 "F6 exit!!!!!!");
//   仅 F5 → 打印后 return (继续运行);
//   其余 (非 'f' 前缀 / 二段不为 0 / 未匹配) → 打印原始响应后 exit(0)。
void check_server_respond(void)
{
    if (g_respond_pkg.ser_cmd[0] == 'f') {
        if (g_respond_pkg.ser_cmd[1] == '1') {
            if (g_respond_pkg.ser_cmd[2] == '\0') {
                puts("Server respond  F1\n");
                exit(0);
            }
        } else if (g_respond_pkg.ser_cmd[1] == '2') {
            if (g_respond_pkg.ser_cmd[2] == '\0') {
                puts("Server respond  F2\n");
                exit(0);
            }
        } else if (g_respond_pkg.ser_cmd[1] == '3') {
            if (g_respond_pkg.ser_cmd[2] == '\0') {
                puts("Server respond  F3\n");
                exit(0);
            }
        } else if (g_respond_pkg.ser_cmd[1] == '4') {
            if (g_respond_pkg.ser_cmd[2] == '\0') {
                puts("Server respond  F4\n");
                exit(0);
            }
        } else if (g_respond_pkg.ser_cmd[1] == '5') {
            if (g_respond_pkg.ser_cmd[2] == '\0') {
                puts("Server respond  F5\n");
                return;                               /* 仅 F5 存活 */
            }
        } else if (g_respond_pkg.ser_cmd[1] == '6') {
            if (g_respond_pkg.ser_cmd[2] == '\0') {
                puts("Server respond  F6\n");
                puts("F6 exit!!!!!!");
                exit(0);
            }
        } else if (g_respond_pkg.ser_cmd[1] == '7' && g_respond_pkg.ser_cmd[2] == '\0') {
            puts("Server respond  F7\n");
            exit(0);
        }
    }
    printf("respond rev : %s", g_respond_pkg.ser_cmd);
    exit(0);
}

// === send_server_pkg === 原 FUN_0000ecd0 @ 0x0000ecd0
// 来源: reversed/monitor_funcs/FUN_0000ecd0.c | 分类: monitor
// 语义: 发送帧头: 2 字节包头 "00" + 命令串 (strlen) + 长度高字节 + 长度低字节;
//       任段返回 -1 即 perror 后 exit(1); 全部成功返回 1。
int send_server_pkg(char *ser_cmd, uint32_t body_len)
{
    uint8_t len_hi = (uint8_t)(body_len >> 8);
    uint8_t len_lo = (uint8_t)body_len;
    size_t cmd_len;

    printf("body_len1=%d\n", (body_len >> 8) & 0xff);
    printf("body_len2=%d\n", body_len & 0xff);
    g_last_send_ret = g_send_fn(g_ssl, g_sockfd, "00", 2);   /* 原 &DAT_00011b8c ("00") */
    if (g_last_send_ret == -1) {
        perror("send head");
        exit(1);
    }
    cmd_len = strlen(ser_cmd);
    g_last_send_ret = g_send_fn(g_ssl, g_sockfd, (void *)ser_cmd, (int)cmd_len);
    if (g_last_send_ret == -1) {
        perror("send");
        exit(1);
    }
    g_last_send_ret = g_send_fn(g_ssl, g_sockfd, &len_hi, 1);
    if (g_last_send_ret == -1) {
        perror("send");
        exit(1);
    }
    g_last_send_ret = g_send_fn(g_ssl, g_sockfd, &len_lo, 1);
    if (g_last_send_ret == -1) {
        perror("send");
        exit(1);
    }
    return 1;
}

// === send_revbody_back === 原 FUN_0000f064 @ 0x0000f064 /* 无调用方, 疑旧路径 */
// 来源: reversed/monitor_funcs/FUN_0000f064.c | 分类: monitor
// 语义: 把 g_revbody 中缓存的内容按帧回发服务器: 先发包头/命令 (首参为包记录
//       首段, 即 "00"), 长度 = strlen(g_revbody); 再发缓冲本体。
//       首参按调用形态推断为包记录 (p1..p4 = 记录头按值), 此处以记录指针承载。
int send_revbody_back(const pkg_slot_t *pkg)
{
    size_t len = strlen(g_revbody);

    printf("02revbody_size=%d\n", (int)len);
    send_server_pkg(pkg->pkg_head, (uint32_t)len);
    g_last_send_ret = g_send_fn(g_ssl, g_sockfd, g_revbody, (int)len);
    if (g_last_send_ret == -1) {
        perror("send");
        exit(1);
    }
    printf("send what I rev:\n%s\n", g_revbody);
    return 1;
}

// === send_cgminer_state === 原 FUN_0000f69c @ 0x0000f69c
// 来源: reversed/monitor_funcs/FUN_0000f69c.c | 分类: monitor
// 语义: 上报 cgminer 状态: 刷新命令记录 nonce (g_cmd_pkg.nonce), 组包
//       "<nonce>ID=<uid>,result=<result>" (0x3c 字节 calloc) 以命令 ser_cmd
//       发帧头后发送载荷; 发送失败仅 perror, 打印载荷后释放。
void send_cgminer_state(char *ser_cmd, char *result)
{
    char *buf = calloc(SEND_STATE_BUF_SIZE, 1);
    char *p;
    size_t len;

    p = stpcpy(buf, gen_pkg_nonce(g_cmd_pkg.nonce));     /* 原 &DAT_0001e44c */
    memcpy(p, "ID=", 4);                                 /* 原 builtin_strncpy (4 字节含 NUL) */
    p = stpcpy(p + 3, g_api_uid);
    p = stpcpy(p, ",result=");                           /* 原 strncpy 8 字节 + stpcpy 于 +8, 等价 */
    p = stpcpy(p, result);
    len = (size_t)(p - buf);                             /* 原 stpcpy 返回指针减缓冲首址 */
    send_server_pkg(ser_cmd, (uint32_t)len);
    g_last_send_ret = g_send_fn(g_ssl, g_sockfd, buf, (int)len);
    if (g_last_send_ret == -1)
        perror("send");
    printf("send cgminer state :%s\n", buf);
    free(buf);
}

/* ==========================================================================
 * [7] 配置更新链路 (deal_cmd → 池参数解析 → uci/json conf 落盘)
 * ======================================================================== */

// === update_cgminer_conf === 原 FUN_0000f410 @ 0x0000f410
// 来源: reversed/monitor_funcs/FUN_0000f410.c | 分类: monitor
// 语义: 处理 "06" 命令 (更新 /config/cgminer.conf): 解析包体串中的池参数段
//       (解析基址 = 记录+8, 即 head_pad 起; 该 8 字节在接收路径恒 0, 原样保留),
//       读出现有 JSON conf → 按服务器池序号合并 → 回写 JSON conf。
int update_cgminer_conf(const pkg_slot_t *pkg)
{
    char *srv_pools = parse_pools_string((char *)&pkg->head_pad[0], DAT_000258c4);
    char *json_conf = load_cgminer_conf_json(g_cgminer_conf_path, DAT_00026d6c);

    json_conf = apply_pool_changes(srv_pools, json_conf);
    save_cgminer_conf_json(json_conf, g_cgminer_conf_path);
    return 1;
}

// === update_cgminer_uci_conf === 原 FUN_0000f470 @ 0x0000f470 /* 无调用方, 疑旧路径 */
// 来源: reversed/monitor_funcs/FUN_0000f470.c | 分类: monitor
// 语义: 处理 UCI 路配置更新 (与 update_cgminer_conf 同构): 解析池参数段
//       (基址 = 记录+8) → 读 UCI conf 内存映像 (DAT_00026ae0) → 按序号合并 →
//       回写 UCI conf 文件 (save_cgminer_uci_conf 经 r0 隐式承接合并结果指针)。
int update_cgminer_uci_conf(const pkg_slot_t *pkg)
{
    char *srv_pools = parse_pools_string((char *)&pkg->head_pad[0], DAT_000258c4);
    char *uci_conf = load_cgminer_uci_conf(DAT_00026ae0);

    uci_conf = apply_uci_pool_changes(srv_pools, uci_conf);
    save_cgminer_uci_conf(uci_conf);
    return 1;
}

// === deal_cmd === 原 FUN_0000f854 @ 0x0000f854
// 来源: reversed/monitor_funcs/FUN_0000f854.c | 分类: monitor
// 语义: 命令分发 (命令串 = 记录 ser_cmd, 第 9 字节为 NUL 判定两字符命令):
//   "01" → send_login_pkg("10") 登录;
//   "06" (ser_cmd[0]=='0', ser_cmd[1]=='6') → 更新 cgminer.conf 并
//         重启 cgminer, 成功/失败经 send_cgminer_state("60", "S"/"F") 上报;
//   其余 → send_getstatus_res(ser_cmd) 走状态查询路径;
//   成功路径末尾复位包全局 (原 0xf898 处标签); 登录与状态路径失败返回 0 且不复位。
//   原 0xfff 包体栈续传复制为传参载体, 等价省略。
int deal_cmd(pkg_slot_t *pkg)
{
    if (pkg->ser_cmd[0] == '0') {
        if (pkg->ser_cmd[1] == '1' && pkg->head_pad[1] == '\0') {
            send_login_pkg("10");                     /* 原 &DAT_0001163c */
            goto pkg_done;                            /* 原 0xf898 处标签 */
        }
        if (pkg->ser_cmd[1] == '6' && pkg->head_pad[1] == '\0') {
            update_cgminer_conf(pkg);
            if (restart_cgminer() != 1) {
                send_cgminer_state("60", "F");        /* 原 &DAT_00011650 / &DAT_00011d94 */
                puts("restart cgminer FAILD ");
                return 0;                             /* 失败不复位包全局 */
            }
            send_cgminer_state("60", "S");            /* 原 &DAT_00011d78 */
            puts("restart cgminer OK OK ");
            goto pkg_done;
        }
    }
    if (send_getstatus_res(pkg->ser_cmd) == 0) {
        puts("after send_getstatus_res()");
        return 0;
    }
pkg_done:                                             /* 原 0xf898 处标签 */
    pkg_globals_reset(pkg);
    return 1;
}

// === load_cgminer_uci_conf === 原 FUN_00010710 @ 0x00010710
// 来源: reversed/monitor_funcs/FUN_00010710.c | 分类: monitor
// 语义: 经 conf_get_value 读 UCI 键值表, 将 16 项按固定偏移拷入 uci_conf 内存
//       映像 (DAT_00026ae0, 布局见全局注释); conf_get_value 可能返回 NULL,
//       strcpy 原样无防护; 返回 uci_conf。
char *load_cgminer_uci_conf(char *uci_conf)
{
    strcpy(uci_conf + POOL_URL_OFF,        conf_get_value("pool1url"));
    strcpy(uci_conf + POOL_USER_OFF,       conf_get_value("pool1user"));
    strcpy(uci_conf + POOL_PW_OFF,         conf_get_value("pool1pw"));
    strcpy(uci_conf + POOL_STRIDE + POOL_URL_OFF,  conf_get_value("pool2url"));
    strcpy(uci_conf + POOL_STRIDE + POOL_USER_OFF, conf_get_value("pool2user"));
    strcpy(uci_conf + POOL_STRIDE + POOL_PW_OFF,   conf_get_value("pool2pw"));
    strcpy(uci_conf + 2 * POOL_STRIDE + POOL_URL_OFF,  conf_get_value("pool3url"));
    strcpy(uci_conf + 2 * POOL_STRIDE + POOL_USER_OFF, conf_get_value("pool3user"));
    strcpy(uci_conf + 2 * POOL_STRIDE + POOL_PW_OFF,   conf_get_value("pool3pw"));
    strcpy(uci_conf + POOL_COUNT_OFF,      conf_get_value("api_allow"));
    strcpy(uci_conf + 0x200,               conf_get_value("more_options"));
    strcpy(uci_conf + 0x20a,               conf_get_value("freq"));
    strcpy(uci_conf + 0x219,               conf_get_value("voltage"));
    strcpy(uci_conf + 0x223,               conf_get_value("pool_balance"));
    strcpy(uci_conf + 0x22d,               conf_get_value("bitmain_nobeeper"));
    strcpy(uci_conf + 0x237,               conf_get_value("bitmain_notempoverctrl"));
    return uci_conf;
}

// === apply_uci_pool_changes === 原 FUN_000108b4 @ 0x000108b4
// 来源: reversed/monitor_funcs/FUN_000108b4.c | 分类: monitor
// 语义: 把服务器池参数 (srv_pools, DAT_000258c4) 按 UCI 映像目标位次合并:
//   池数==1 → idx=strtol(池0序号), 池0 三串拷入 uci_conf+idx*0xa4+{4,0x54,0x7c};
//   池数==2 → 池0/池1 序号各自 strtol, 分别拷入对应目标位次;
//   池数==3 → 三池三串拷入固定 +4/+0x54/+0x7c、+0xa8/+0xf8/+0x120、
//             +0x14c/+0x19c/+0x1c4 (序号不参与); 其余池数不处理; 返回 uci_conf。
char *apply_uci_pool_changes(char *srv_pools, char *uci_conf)
{
    int count = *(int *)(srv_pools + POOL_COUNT_OFF);

    if (count == 1) {
        long idx = strtol(srv_pools, NULL, 10);            /* 池0 序号串位于 +0 */
        strcpy(uci_conf + idx * POOL_STRIDE + POOL_URL_OFF,  srv_pools + POOL_URL_OFF);
        strcpy(uci_conf + idx * POOL_STRIDE + POOL_USER_OFF, srv_pools + POOL_USER_OFF);
        strcpy(uci_conf + idx * POOL_STRIDE + POOL_PW_OFF,   srv_pools + POOL_PW_OFF);
    } else if (count == 2) {
        long idx1 = strtol(srv_pools, NULL, 10);
        long idx2 = strtol(srv_pools + POOL_STRIDE, NULL, 10);
        strcpy(uci_conf + idx1 * POOL_STRIDE + POOL_URL_OFF,  srv_pools + POOL_URL_OFF);
        strcpy(uci_conf + idx1 * POOL_STRIDE + POOL_USER_OFF, srv_pools + POOL_USER_OFF);
        strcpy(uci_conf + idx1 * POOL_STRIDE + POOL_PW_OFF,   srv_pools + POOL_PW_OFF);
        strcpy(uci_conf + idx2 * POOL_STRIDE + POOL_URL_OFF,  srv_pools + POOL_STRIDE + POOL_URL_OFF);
        strcpy(uci_conf + idx2 * POOL_STRIDE + POOL_USER_OFF, srv_pools + POOL_STRIDE + POOL_USER_OFF);
        strcpy(uci_conf + idx2 * POOL_STRIDE + POOL_PW_OFF,   srv_pools + POOL_STRIDE + POOL_PW_OFF);
    } else if (count == 3) {
        strcpy(uci_conf + POOL_URL_OFF,              srv_pools + POOL_URL_OFF);
        strcpy(uci_conf + POOL_USER_OFF,             srv_pools + POOL_USER_OFF);
        strcpy(uci_conf + POOL_PW_OFF,               srv_pools + POOL_PW_OFF);
        strcpy(uci_conf + POOL_STRIDE + POOL_URL_OFF,  srv_pools + POOL_STRIDE + POOL_URL_OFF);
        strcpy(uci_conf + POOL_STRIDE + POOL_USER_OFF, srv_pools + POOL_STRIDE + POOL_USER_OFF);
        strcpy(uci_conf + POOL_STRIDE + POOL_PW_OFF,   srv_pools + POOL_STRIDE + POOL_PW_OFF);
        strcpy(uci_conf + 2 * POOL_STRIDE + POOL_URL_OFF,  srv_pools + 2 * POOL_STRIDE + POOL_URL_OFF);
        strcpy(uci_conf + 2 * POOL_STRIDE + POOL_USER_OFF, srv_pools + 2 * POOL_STRIDE + POOL_USER_OFF);
        strcpy(uci_conf + 2 * POOL_STRIDE + POOL_PW_OFF,   srv_pools + 2 * POOL_STRIDE + POOL_PW_OFF);
        return uci_conf;
    }
    return uci_conf;
}

// === save_cgminer_uci_conf === 原 FUN_00010a34 @ 0x00010a34
// 来源: reversed/monitor_funcs/FUN_00010a34.c | 分类: monitor
// 语义: 以 "rb+" 重写 UCI conf (DAT_0001c3d0): 先写 "config cgminer 'default'\n",
//       再按固定缩进/格式逐项 fprintf 池与选项, 尾部补 12 字节空白后关闭。
//       fopen 失败仅打印 "open error", 后续写入照旧执行 (原样保留, 不判空)。
//       注: 原 Ghidra 视图调用无参, 实际 r0 承接 apply_uci_pool_changes 的返回
//       (uci_conf), 此处显式传参。
void save_cgminer_uci_conf(char *uci_conf)
{
    FILE *fp = fopen(DAT_0001c3d0, "rb+");

    if (fp == NULL)
        puts("open error");
    fwrite("config cgminer 'default'\n", 1, 0x19, fp);
    fprintf(fp, "  option pool1url  '%s'\n", uci_conf + POOL_URL_OFF);
    fprintf(fp, "\toption pool1user '%s'\n", uci_conf + POOL_USER_OFF);
    fprintf(fp, "\toption pool1pw   '%s'\n", uci_conf + POOL_PW_OFF);
    fprintf(fp, "\toption pool2url  '%s'\n", uci_conf + POOL_STRIDE + POOL_URL_OFF);
    fprintf(fp, "\toption pool2user '%s'\n", uci_conf + POOL_STRIDE + POOL_USER_OFF);
    fprintf(fp, "\toption pool2pw   '%s'\n", uci_conf + POOL_STRIDE + POOL_PW_OFF);
    fprintf(fp, "\toption pool3url  '%s'\n", uci_conf + 2 * POOL_STRIDE + POOL_URL_OFF);
    fprintf(fp, "\toption pool3user '%s'\n", uci_conf + 2 * POOL_STRIDE + POOL_USER_OFF);
    fprintf(fp, "\toption pool3pw   '%s'\n", uci_conf + 2 * POOL_STRIDE + POOL_PW_OFF);
    fprintf(fp, "\toption api_allow  '%s'\n", uci_conf + POOL_COUNT_OFF);
    fprintf(fp, "\toption more_options '%s'\n", uci_conf + 0x200);
    fprintf(fp, "\toption freq   '%s'\n", uci_conf + 0x20a);
    fprintf(fp, "\toption voltage   '%s'\n", uci_conf + 0x219);
    fprintf(fp, "\toption pool_balance '%s'\n", uci_conf + 0x223);
    fprintf(fp, "\toption bitmain_nobeeper   '%s'\n", uci_conf + 0x22d);
    fprintf(fp, "\toption bitmain_notempoverctrl '%s'\n", uci_conf + 0x237);
    fwrite("\t           ", 1, 0xc, fp);
    fclose(fp);
}

// === write_auth_tmp_file === 原 FUN_00010bfc @ 0x00010bfc /* 无调用方, 疑旧路径 */
// 来源: reversed/monitor_funcs/FUN_00010bfc.c | 分类: monitor
// 语义: 写 "/etc/config/auth_tmp.txt": 参数为 "f5" 时原样写入, 否则写默认串
//       "fx" (原 &DAT_0001217c); 打开失败返回 -1, 成功返回 1。
int write_auth_tmp_file(char *auth_str)
{
    FILE *fp = fopen("/etc/config/auth_tmp.txt", "w+");

    if (fp == NULL)
        return -1;
    if (auth_str[0] == 'f' && auth_str[1] == '5' && auth_str[2] == '\0')
        fputs(auth_str, fp);
    else
        fwrite("fx", 1, 2, fp);
    fclose(fp);
    return 1;
}

/* ==========================================================================
 * [8] L_cmd 命令链表 (已下发命令 nonce 登记 / 配对删除)
 * ======================================================================== */

// === cmd_list_init === 原 FUN_00010d34 @ 0x00010d34
// 来源: reversed/monitor_funcs/FUN_00010d34.c | 分类: monitor
// 语义: 创建 L_cmd 哨兵头结点 (8 字节, 仅清 next, data 不初始化), 计数清 0,
//       挂到 g_cmd_list_head; malloc 失败 exit(0)。main 以 DAT_0001d420 值传参
//       但原函数不使用, 此处保留哑参数以匹配调用形状。
void cmd_list_init(void *unused_head)
{
    cmd_node_t *sentinel = malloc(L_CMD_NODE_SIZE);

    (void)unused_head;
    if (sentinel == NULL)
        exit(0);
    sentinel->next = NULL;          /* 原 *(int*)(p+4) = 0 */
    g_cmd_list_count = 0;
    g_cmd_list_head = sentinel;
}

// === cmd_list_find === 原 FUN_00010d90 @ 0x00010d90
// 来源: reversed/monitor_funcs/FUN_00010d90.c | 分类: monitor
// 语义: 在 L_cmd 中查找 nonce 与弹出记录 (pkg->nonce, 即记录 +0x1008) 相同的
//       结点; 命中返回结点指针, 未命中返回 NULL。
cmd_node_t *cmd_list_find(const pkg_slot_t *pkg, cmd_node_t *head)
{
    cmd_node_t *node = head->next;

    while (node != NULL) {
        if (strcmp(node->data->nonce, pkg->nonce) == 0)   /* 原 +0x1008 比较 */
            return node;
        node = node->next;
    }
    return NULL;
}

// === cmd_list_find_prev === 原 FUN_00010ddc @ 0x00010ddc
// 来源: reversed/monitor_funcs/FUN_00010ddc.c | 分类: monitor
// 语义: 返回"下一结点 nonce 与 pkg 匹配"的前驱结点 (供删除用);
//       无匹配时返回尾结点 (其 next 为 NULL)。
cmd_node_t *cmd_list_find_prev(const pkg_slot_t *pkg, cmd_node_t *prev)
{
    while (prev->next != NULL &&
           strcmp(prev->next->data->nonce, pkg->nonce) != 0)
        prev = prev->next;
    return prev;
}

// === cmd_list_delete === 原 FUN_00010e24 @ 0x00010e24
// 来源: reversed/monitor_funcs/FUN_00010e24.c | 分类: monitor
// 语义: 按 pkg->nonce 找前驱并摘除匹配结点: 打印被删命令/nonce, 释放记录副本
//       与结点, 计数减 1; 无匹配时打印提示并返回 (不减计数)。
void cmd_list_delete(const pkg_slot_t *pkg, cmd_node_t *head)
{
    cmd_node_t *prev = cmd_list_find_prev(pkg, head);
    cmd_node_t *node = prev->next;
    pkg_slot_t *data;

    if (node == NULL) {
        puts("there is no cmd for this response.");
        return;
    }
    data = node->data;
    prev->next = node->next;
    printf("delete one cmd:%s,nonce:%s.\n", data->ser_cmd, data->nonce);
    free(data);
    free(node);
    g_cmd_list_count--;
}

// === cmd_list_add === 原 FUN_00010ec0 @ 0x00010ec0
// 来源: reversed/monitor_funcs/FUN_00010ec0.c | 分类: monitor
// 语义: 头插登记一条已下发命令: 结点 8 字节 + 记录副本 0x100f 字节,
//       仅复制 ser_cmd (+3) 与 nonce (+0x1008) 两段 (其余保持 malloc 原值),
//       计数加 1 后 >= 0x15 时打印 "L_cmd is full!" (登记照常完成);
//       malloc 失败 exit(0)。第二参数与 head 同值传入但原函数未使用 (调用形状)。
void cmd_list_add(const pkg_slot_t *pkg, cmd_node_t *unused_head, cmd_node_t *head)
{
    cmd_node_t *node = malloc(L_CMD_NODE_SIZE);
    pkg_slot_t *data;

    (void)unused_head;
    if (node == NULL)
        exit(0);
    data = malloc(PKG_SLOT_SIZE);
    strcpy(data->ser_cmd, pkg->ser_cmd);            /* 原 +3 → +3 */
    strcpy(data->nonce, pkg->nonce);                /* 原 +0x1008 → +0x1008 */
    node->data = data;
    node->next = head->next;
    g_cmd_list_count++;
    head->next = node;
    if (g_cmd_list_count > L_CMD_MAX_COUNT)         /* 原 < 0x15 即返回 */
        puts("L_cmd is full!");
}

// === cmd_list_free === 原 FUN_00010f48 @ 0x00010f48
// 来源: reversed/monitor_funcs/FUN_00010f48.c | 分类: monitor
// 语义: 摘除并释放 L_cmd 全部结点 (记录副本+结点), 最后释放哨兵头结点。
void cmd_list_free(cmd_node_t *head)
{
    cmd_node_t *node = head->next;
    cmd_node_t *next;

    head->next = NULL;
    while (node != NULL) {
        next = node->next;
        free(node->data);
        free(node);
        node = next;
    }
    free(head);
}

/* ==========================================================================
 * [9] 工作线程 (响应处理 / 命令处理 / api-switch 监视)
 * ======================================================================== */

// === respond_queue_thread === 原 FUN_0000a514 @ 0x0000a514
// 来源: reversed/monitor_funcs/FUN_0000a514.c | 分类: monitor
// 语义: 响应处理线程: 运行标志开启时循环取空响应队列 — 弹出记录后遍历打印、
//       check_server_respond (F1..F7 逐项处理, F5 之外均 exit)、按 nonce 配对
//       L_cmd 结点 (找不到 exit(1)); 若原命令为 "01" 则把响应命令串写入
//       minermonitor.conf 的 auth 字段; 删除结点并复位包全局。
//       原 push/reset 前的 0xfff memcpy 为按值传参载体, 等价省略。
void respond_queue_thread(void)
{
    for (;;) {
        if (g_running == '\0')
            return;
        while (pkg_queue_not_empty(&g_respond_queue)) {
            cmd_node_t *node;

            g_respond_queue.pad = 0;                /* 原 DAT_0001d41c = 0 */
            if (pkg_queue_pop(&g_respond_queue, &g_respond_pkg) == 0) {
                puts("respond queue  empty");
            } else {
                pkg_queue_foreach(g_respond_queue.slots, g_respond_queue.head,
                                  g_respond_queue.tail, print_pkg_node);
                check_server_respond();
                printf("response node:%s,%s----------\n",
                       g_respond_pkg.ser_cmd, g_respond_pkg.nonce);
                node = cmd_list_find(&g_respond_pkg, g_cmd_list_head);
                if (node == NULL) {
                    puts("no cmd for responding!");
                    exit(1);
                }
                if (node->data->ser_cmd[0] == '0' &&
                    node->data->ser_cmd[1] == '1' &&
                    node->data->ser_cmd[2] == '\0') {
                    conf_update_api_switch(g_conf_path, g_respond_pkg.ser_cmd);
                }
                printf("Delete node:%s,%s----------\n",
                       g_respond_pkg.ser_cmd, g_respond_pkg.nonce);
                cmd_list_delete(&g_respond_pkg, g_cmd_list_head);
                pkg_globals_reset(&g_respond_pkg);
            }
            DAT_0001b2f4 = 0;
            if (g_running == '\0') {
                DAT_0001b2f4 = 0;
                return;
            }
        }
    }
}

// === cmd_deal_thread === 原 FUN_0000a680 @ 0x0000a680
// 来源: reversed/monitor_funcs/FUN_0000a680.c | 分类: monitor
// 语义: 命令处理线程: 运行标志开启时循环弹出命令队列 — 遍历打印 → deal_cmd
//       分发处理; 处理成功则登记 L_cmd (记录 ser_cmd+nonce) 并复位包全局;
//       队列空 sleep(1); api-switch 关闭 (g_web_switch_off==1) 时退出 exit(1)。
void cmd_deal_thread(void)
{
    for (;;) {
        int ret;

        if (g_running == '\0')
            return;
        puts("next command!");
        if (g_web_switch_off == 1)
            break;
        ret = pkg_queue_pop(&g_cmd_queue, &g_cmd_pkg);
        if (ret == 0) {
            puts("queue empty1");
            sleep(1);
        } else {
            printf("get a command head:%s!\n", g_cmd_pkg.pkg_head);
            printf("get a command:%s!\n", g_cmd_pkg.ser_cmd);
            pkg_queue_foreach(g_cmd_queue.slots, g_cmd_queue.head,
                              g_cmd_queue.tail, print_pkg_node);
            ret = deal_cmd(&g_cmd_pkg);
            if (ret != 0) {
                puts("after deal_cmd()");
                printf("get a command nonce:%s!\n", g_cmd_pkg.nonce);
                DAT_0001b2f4 = 0;
                cmd_list_add(&g_cmd_pkg, g_cmd_list_head, g_cmd_list_head);
                printf("new node:%s,%s==========\n", g_cmd_pkg.ser_cmd, g_cmd_pkg.nonce);
                pkg_globals_reset(&g_cmd_pkg);
            }
        }
    }
    puts("Web  Switch is OFF!");
    exit(1);
}

// === switch_monitor_thread === 原 FUN_0000a7f4 @ 0x0000a7f4
// 来源: reversed/monitor_funcs/FUN_0000a7f4.c | 分类: monitor
// 语义: api-switch 监视线程: 每 15 秒读 minermonitor.conf 的 "api-switch" 键,
//       值为 "off" 时置 g_web_switch_off=1 (命令线程/连接处据以退出);
//       运行标志关闭后打印提示并返回 0。conf 读取失败返回 NULL 时解引用崩溃,
//       原样保留无防护。
int switch_monitor_thread(void)
{
    char value[16];

    while (g_running != '\0') {
        char *v = monitor_conf_get_value(g_conf_path, "api-switch", value);

        if (v[0] == 'o' && v[1] == 'f' && v[2] == 'f' && v[3] == '\0')
            g_web_switch_off = 1;
        printf("key_switch = %s\n", value);
        sleep(SLEEP_SWITCH_POLL);                   /* 原 sleep(0xf) */
    }
    puts("will bi exited###################");
    return 0;
}

/* ==========================================================================
 * 覆盖率表 — batch=mon_core_b2 (56/56)
 * ==========================================================================
 * 原地址      反编译文件          语义名                    分区
 * FUN_0000a2e8 FUN_0000a2e8.c    call_gmon_start           [1] 运行时桩
 * FUN_0000a2ac entry             entry (_start)            [1] 运行时桩
 * FUN_0000a30c FUN_0000a30c.c    deregister_tm_clones      [1] 运行时桩
 * FUN_0000a3ac _INIT_0.c         _INIT_0 (frame_dummy)     [1] 运行时桩
 * FUN_0000a384 _FINI_0.c         _FINI_0 (do_global_dtors_aux) [1] 运行时桩
 * FUN_00009a58 _init.c           _init                     [1] 运行时桩
 * FUN_00011048 _fini.c           _fini                     [1] 运行时桩
 * FUN_00010fe0 FUN_00010fe0.c    libc_csu_init             [1] 运行时桩
 * FUN_0000a3e0 FUN_0000a3e0.c    sigpipe_handler           [2] 信号与定时器
 * FUN_0000a3ec FUN_0000a3ec.c    sigint_term_handler       [2] 信号与定时器
 * FUN_0000ace4 FUN_0000ace4.c    set_itimer                [2] 信号与定时器
 * FUN_0000ae1c FUN_0000ae1c.c    setup_timer_signals       [2] 信号与定时器
 * FUN_0000afe8 FUN_0000afe8.c    init_signal_mask          [2] 信号与定时器
 * FUN_0000b020 FUN_0000b020.c    daemonize                 [3] 进程与配置工具
 * FUN_0000b208 FUN_0000b208.c    str_rstrip_char           [3] 进程与配置工具
 * FUN_0000b250 FUN_0000b250.c    parse_kv_line             [3] 进程与配置工具
 * FUN_0000b9c4 FUN_0000b9c4.c    print_conf_vars (无调用方) [3] 进程与配置工具
 * FUN_0000ba78 FUN_0000ba78.c    conf_get_value            [3] 进程与配置工具
 * FUN_0000e1f8 FUN_0000e1f8.c    gen_pkg_nonce             [4] 包队列
 * FUN_0000e284 FUN_0000e284.c    get_local_ip              [4] 包队列
 * FUN_0000e340 FUN_0000e340.c    get_iface_mac             [4] 包队列
 * FUN_0000e444 FUN_0000e444.c    init_pkg_queues           [4] 包队列
 * FUN_0000e49c FUN_0000e49c.c    free_pkg_queue            [4] 包队列
 * FUN_0000e4cc FUN_0000e4cc.c    clear_pkg_queue           [4] 包队列
 * FUN_0000e54c FUN_0000e54c.c    pkg_queue_copy_slot (无调用方) [4] 包队列
 * FUN_0000e598 FUN_0000e598.c    pkg_queue_push            [4] 包队列
 * FUN_0000e62c FUN_0000e62c.c    pkg_queue_pop             [4] 包队列
 * FUN_0000e698 FUN_0000e698.c    pkg_queue_foreach         [4] 包队列
 * FUN_0000e75c FUN_0000e75c.c    print_pkg_node            [4] 包队列
 * FUN_0000e7d8 FUN_0000e7d8.c    pkg_queue_not_empty       [4] 包队列
 * FUN_0000e7f4 FUN_0000e7f4.c    pkg_globals_reset         [4] 包队列
 * FUN_0000a8a0 FUN_0000a8a0.c    print_peer_certificate    [5] SSL/TCP 网络
 * FUN_0000a938 FUN_0000a938.c    ssl_connect               [5] SSL/TCP 网络
 * FUN_0000aa14 FUN_0000aa14.c    sock_connect_server       [5] SSL/TCP 网络
 * FUN_0000adb0 FUN_0000adb0.c    recv_serpkg_struct        [5] SSL/TCP 网络
 * FUN_0000ae50 FUN_0000ae50.c    pkg_recv_dispatch (无调用方) [5] SSL/TCP 网络
 * FUN_0000eb78 FUN_0000eb78.c    check_server_respond      [6] 服务器收发
 * FUN_0000ecd0 FUN_0000ecd0.c    send_server_pkg           [6] 服务器收发
 * FUN_0000f064 FUN_0000f064.c    send_revbody_back (无调用方) [6] 服务器收发
 * FUN_0000f69c FUN_0000f69c.c    send_cgminer_state        [6] 服务器收发
 * FUN_0000f410 FUN_0000f410.c    update_cgminer_conf       [7] 配置更新链路
 * FUN_0000f470 FUN_0000f470.c    update_cgminer_uci_conf (无调用方) [7] 配置更新链路
 * FUN_0000f854 FUN_0000f854.c    deal_cmd                  [7] 配置更新链路
 * FUN_00010710 FUN_00010710.c    load_cgminer_uci_conf     [7] 配置更新链路
 * FUN_000108b4 FUN_000108b4.c    apply_uci_pool_changes    [7] 配置更新链路
 * FUN_00010a34 FUN_00010a34.c    save_cgminer_uci_conf     [7] 配置更新链路
 * FUN_00010bfc FUN_00010bfc.c    write_auth_tmp_file (无调用方) [7] 配置更新链路
 * FUN_00010d34 FUN_00010d34.c    cmd_list_init             [8] L_cmd 命令链表
 * FUN_00010d90 FUN_00010d90.c    cmd_list_find             [8] L_cmd 命令链表
 * FUN_00010ddc FUN_00010ddc.c    cmd_list_find_prev        [8] L_cmd 命令链表
 * FUN_00010e24 FUN_00010e24.c    cmd_list_delete           [8] L_cmd 命令链表
 * FUN_00010ec0 FUN_00010ec0.c    cmd_list_add              [8] L_cmd 命令链表
 * FUN_00010f48 FUN_00010f48.c    cmd_list_free             [8] L_cmd 命令链表
 * FUN_0000a514 FUN_0000a514.c    respond_queue_thread      [9] 工作线程
 * FUN_0000a680 FUN_0000a680.c    cmd_deal_thread           [9] 工作线程
 * FUN_0000a7f4 FUN_0000a7f4.c    switch_monitor_thread     [9] 工作线程
 * ========================================================================== */
