/*
 * miner-monitor 守护进程 — 核心逻辑 (第 1/2 部分, 批次 mon_core_b1)
 * =============================================================
 * 原始二进制: BM1746/sdcard/rootfs_sd/usr/bin/miner-monitor (ARM 32-bit, 基址 0x8000)
 *
 * 模块功能概述:
 *   miner-monitor 是矿机上的 TLS/TCP 上报守护进程:
 *   - main() 创建单例 pid 文件锁, 建立到服务器的连接 (可选 SSL), 注册信号处理器
 *     并驱动 "接收 → 环形队列 → 命令线程/应答线程" 主循环;
 *   - 通信包格式: 包头 "00" + 2 字节命令码 ("01".."07" 请求 / "fX" 应答)
 *     + 2 字节大端长度 + nonce/body; 包记录 0x100f 字节 (p2 的 pkg_slot_t);
 *   - minermonitor.conf (JSON, monitor_conf_get_value / conf_update_api_switch);
 *   - L_cmd 命令链表 (cmd_list_*, 见 monitor_core_p2.c);
 *   - cgminer.conf 矿池变更链 (parse_pools_string → apply_pool_changes → save);
 *   - 内嵌精简 cJSON (json_*, 见 cjson.c, 批次 mon_cjson_b1)。
 *
 * 跨批次共享约定 (与 monitor_core_p2.c 对齐):
 *   - 共享全局变量集中在 monitor_core_p2.c 定义 (g_conf_path/g_running/g_sockfd/
 *     g_recv_pkg 等); 本文件仅 extern 引用, 并补充 p2 未覆盖的 g_pthread_ret。
 *   - 包记录类型 pkg_slot_t / 队列类型 pkg_queue_t 定义在 p2 (匿名 typedef),
 *     本文件以布局完全相同的镜像类型 mon_slot_t / mon_queue_t 引用, 字段名一致。
 *   - 收/发分发函数指针 p2 为 pkg_xfer_fn, 本文件镜像为 mon_xfer_fn (同型)。
 *   - 本文件提供给 p2 的函数签名与 p2 的 extern 声明逐字一致 (无 const 修饰)。
 *   - json_* 函数属于 monitor/cjson.c (批次 mon_cjson_b1), 命名取自
 *     rewritten/name_map/monitor.tsv。cJSON 节点布局按字节偏移访问:
 *     +0x00 next | +0x04 prev | +0x08 child | +0x0C type
 *     +0x10 valuestring | +0x14 valueint | +0x18 valuedouble | +0x20 string
 *   - p2 的 pkg_queue_push(q, pkg) 直接复制 0x100f 记录; 原二进制中 main 以
 *     "12 字节头经寄存器 + 0x1003 体经栈" 的隐式约定传参, 两者落槽内容等价,
 *     main 内保留了对 g_recv_pkg+0xC 的栈上预载 memcpy (原样)。
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <fcntl.h>          /* flock */
#include <sys/file.h>       /* LOCK_EX / LOCK_NB */
#include <sys/stat.h>       /* mkdir */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <assert.h>

/* ------------------------------------------------------------------ */
/* 命名常量 (魔数来源: 反编译 + 字符串证据)                            */
/* ------------------------------------------------------------------ */

#define MON_PKG_BODY_SIZE      0x1003   /* 包体长度 (main 栈上预载长度) */
#define MON_MAIN_FULL_MODULUS  0x14     /* main 满队列判断模数 (20; push 内部为 9, 原始即不一致) */

#define MON_MAX_CONF_VARS      0x28     /* 通用配置最多 40 个变量 */
#define MON_CONF_NAME_LEN      0x81     /* 变量名缓冲 129 字节 */
#define MON_CONF_VALUE_LEN     0x201    /* 变量值缓冲 513 字节 */
#define MON_CONF_LINE_GETS     0x302    /* fgets 长度 770 */
#define MON_CONF_LINE_LIMIT    0x301    /* 超过 769 视为过长跳过 */

#define MON_API_BUF_INIT       0xffdd   /* callapi 首次 malloc 65501 */
#define MON_API_CHUNK          0xffdc   /* callapi 单次 recv 长度 65500 */

#define MON_LOCAL_PORT_BUF     12       /* port_local 取值缓冲 */
#define MON_LOCAL_IP_BUF       44       /* get_local_ip 缓冲 */
#define MON_LOGIN_IP_BUF       40       /* send_login_pkg 的 IP 区 */
#define MON_LOGIN_UID_BUF      60       /* send_login_pkg 的 UID 值区 (0x3c) */

#define MON_TCP_RETRY_MAX      6        /* restart_cgminer 连接重试基值 */

#define MON_POOL_REC_SIZE      0xa4     /* 矿池记录步长 */
#define MON_POOL_OFF_IDX       0x00     /* 矿池序号字符串偏移 */
#define MON_POOL_OFF_URL       0x04     /* url 偏移 (80B) */
#define MON_POOL_OFF_USER      0x54     /* user 偏移 (40B) */
#define MON_POOL_OFF_PASS      0x7c     /* pass 偏移 (40B) */
#define MON_POOL_OFF_COUNT     0x1ec    /* 矿池计数 (int) 偏移 */

/* cgminer.conf 结构 (原 DAT_00026d6c, p2 的 DAT_00026d6c) 各字段偏移 */
#define CGM_OFF_API_LISTEN     0x1ec    /* api-listen (int) */
#define CGM_OFF_API_NETWORK    0x1f0    /* api-network (int) */
#define CGM_OFF_NOBEEPER       0x1f4    /* bitmain-nobeeper (int) */
#define CGM_OFF_NOTEMPOVERCTRL 0x1f8    /* bitmain-notempoverctrl (int) */
#define CGM_OFF_API_ALLOW      0x1fc    /* api-allow (10B 字符串) */
#define CGM_OFF_FREQ           0x206    /* bitmain-freq (15B 字符串) */
#define CGM_OFF_VOLTAGE        0x215    /* bitmain-voltage (13B 字符串) */

/* cJSON 节点字段字节偏移 (与 cjson.c 的节点布局一致, 由反编译确认) */
#define JSON_OFF_VALUESTRING   0x10
#define JSON_OFF_VALUEINT      0x14
#define JSON_OFF_STRING        0x20

#define CTYPE_SPACE            0x2000   /* __ctype_b_loc 表中空白类标志位 */
#define MON_TYPE_ID            7        /* send_login_pkg 上报的 TYPE 值 */

/* ------------------------------------------------------------------ */
/* 类型 (p2 匿名 typedef 的布局镜像; 字段名与 p2 一致)                 */
/* ------------------------------------------------------------------ */

typedef struct {                    /* 镜像 p2 pkg_slot_t: 服务器协议包记录 (0x100f 字节) */
    char pkg_head[3];               /* +0x0000: "00\0" */
    char ser_cmd[3];                /* +0x0003: 命令串 "XY\0" */
    char ser_len[2];                /* +0x0006: 长度域 (接收路径未写) */
    char head_pad[8];               /* +0x0008: 原两段 dword (原样保留) */
    char body[0xff8];               /* +0x0010: 包体 (原 DAT_0001e460 起 = g_recv_pkg+0xC 起 0x1003) */
    char nonce[7];                  /* +0x1008: 包 nonce (原 DAT_0001f45c / DAT_0001e44c 落位) */
} mon_slot_t;

typedef struct {                    /* 镜像 p2 pkg_queue_t: 包环形队列控制块 (16 字节) */
    void *slots;                    /* +0x00: malloc(36999) 槽基址 (9 槽 × 0x100f) */
    int   head;                     /* +0x04: 读索引 */
    int   tail;                     /* +0x08: 写索引 */
    int   pad;                      /* +0x0c: 未用 */
} mon_queue_t;

/* 收/发分发函数指针 (同 p2 pkg_xfer_fn): main 按 ssl_connect 结果选择
 * SSL_read/SSL_write 或 recv/send 包装 (本文件末尾 4 个静态包装) */
typedef int (*mon_xfer_fn)(SSL *ssl, int sockfd, void *buf, int len);

struct cmd_node;                    /* L_cmd 结点 (p2 定义: {data, next}) */

/* ------------------------------------------------------------------ */
/* 共享全局变量 — p2 (monitor_core_p2.c) 集中定义, 本文件仅 extern 引用 */
/* ------------------------------------------------------------------ */

extern char *g_conf_path;           /* 原 DAT_0001b2f0 "/config/minermonitor.conf" */
extern char *g_pid_path;            /* 原 DAT_0001b2f8 "/tmp/miner-monitor/pid" */
extern int   g_running;             /* 原 DAT_0001b2ec (.data=1): 主循环运行标志 */
extern int   g_r_connect;           /* 原 DAT_0001b380 (.data=1): TCP 在线标志 */
extern int   g_web_switch_off;      /* 原 DAT_0001b394: Web 开关 (1 → 程序退出) */
extern FILE *g_pid_fp;              /* 原 DAT_0001c3c8: pid 文件句柄 */
extern int   g_var_num;             /* 原 DAT_0001b3a0: 通用配置已解析变量数 */
extern mon_queue_t g_cmd_queue;     /* 原 DAT_0001b3a8: 命令包队列 */
extern mon_queue_t g_respond_queue; /* 原 DAT_0001d410: 应答包队列 */
extern mon_slot_t g_recv_pkg;       /* 原 DAT_0001e454 (0x100f): 服务器接收记录 */
extern mon_slot_t g_err_pkg;        /* 原 DAT_0001c3d4 (0x100f): 断线复位用记录 */
extern mon_slot_t g_cmd_pkg;        /* 原 DAT_0001d444 (0x100f): 命令记录 (nonce=原 DAT_0001e44c) */
extern struct cmd_node *g_cmd_list_head;    /* 原 DAT_0001d420: L_cmd 哨兵 */
extern pthread_t g_tid_switch;              /* 原 DAT_0001d404: switch_monitor_thread 线程 */
extern pthread_t g_tid_cmd_deal;            /* 原 DAT_0001f464: cmd_deal_thread 线程 */
extern pthread_t g_tid_respond;             /* 原 DAT_0001c3cc: respond_queue_thread 线程 */
extern mon_xfer_fn g_recv_fn;       /* 原 DAT_0001d434: 接收分发函数指针 */
extern mon_xfer_fn g_send_fn;       /* 原 DAT_0001d43c: 发送分发函数指针 */
extern int   g_last_send_ret;       /* 原 DAT_00026f90: 最近一次发送返回值 */
extern int   g_last_recv_ret;       /* 原 DAT_00026f94: 最近一次接收返回值 */
extern int   g_sockfd;              /* 原 DAT_000258bc: 服务器连接 fd */
extern SSL  *g_ssl;                 /* 原 DAT_00026f98: SSL* (ssl_connect 创建) */
extern char  DAT_00026ae0[];        /* 原 DAT_00026ae0 (0x28c): UCI 映像; +0x244 = 原 DAT_00026d24 */
extern char  DAT_00024490[];        /* 原 DAT_00024490: conf 键名数组 (步进 0x81) */
extern char  DAT_0001f468[];        /* 原 DAT_0001f468: conf 值数组 (步进 0x201) */
extern char  g_api_uid[];           /* 原 DAT_00026ab8: 命令体首段 (设备 ID, p2 名 g_api_uid) */

/* 本文件补充定义 (p2 未覆盖): */
int g_pthread_ret;                  /* 原 DAT_0001b398: pthread_create 返回值暂存 (仅 main 使用) */

/* ------------------------------------------------------------------ */
/* 跨批次被调函数声明 (批次 mon_cjson_b1 = cjson.c / mon_core_b2 = monitor_core_p2.c) */
/* ------------------------------------------------------------------ */

/* ---- cjson.c (mon_cjson_b1) ---- */
extern void *json_parse(char *text);                                   /* FUN_0000d9d8 */
extern int   json_get_array_size(void *item);                          /* FUN_0000d9fc */
extern void *json_get_array_item(void *item, int index);               /* FUN_0000da28 */
extern void *json_get_object_item(void *item, const char *key);        /* FUN_0000da50 */
extern void  json_delete(void *item);                                  /* FUN_0000d864 */
extern char *json_print(void *item);                                   /* FUN_0000d9e4 */
extern void *json_create_object(void);                                 /* FUN_0000de4c */
extern void *json_create_string(const char *str);                      /* FUN_0000de04 */
extern void *json_create_true(void);                                   /* FUN_0000dd80 */
extern void *json_create_false(void);                                  /* FUN_0000dd98 */
extern void *json_create_array(void);                                  /* FUN_0000de34 */
extern void  json_add_item_to_object(void *object, const char *key, void *item); /* FUN_0000dac8 */
extern void  json_add_item_to_array(void *array, void *item);          /* FUN_0000da90 */

/* ---- monitor_core_p2.c (mon_core_b2; 签名与 p2 前置声明逐字一致) ---- */
extern void  init_signal_mask(void);                                   /* FUN_0000afe8 */
extern void  setup_timer_signals(int unused_sec);                      /* FUN_0000ae1c */
extern void  set_itimer(time_t interval_sec, time_t value_sec);        /* FUN_0000ace4 */
extern int   sock_connect_server(void);                                /* FUN_0000aa14 */
extern int   ssl_connect(void);                                        /* FUN_0000a938 */
extern int   recv_serpkg_struct(void);                                 /* FUN_0000adb0 */
extern int   init_pkg_queues(mon_queue_t *cmd_q, mon_queue_t *resp_q); /* FUN_0000e444 */
extern int   clear_pkg_queue(mon_queue_t *q);                          /* FUN_0000e4cc */
extern bool  pkg_queue_push(mon_queue_t *q, const mon_slot_t *pkg);    /* FUN_0000e598 */
extern void  pkg_globals_reset(const mon_slot_t *pkg);                 /* FUN_0000e7f4 */
extern int   send_server_pkg(char *ser_cmd, unsigned int body_len);    /* FUN_0000ecd0 */
extern void  str_rstrip_char(char *str, unsigned int ch);              /* FUN_0000b208 */
extern int   parse_kv_line(char **line, char **key, char **value);     /* FUN_0000b250 */
extern char *gen_pkg_nonce(char *out);                                 /* FUN_0000e1f8 */
extern int   get_local_ip(char *iface, char *ip_out);                  /* FUN_0000e284 */
extern char *get_iface_mac(char *out, char *iface);                    /* FUN_0000e340 */
extern void  cmd_list_init(void *unused_head);                         /* FUN_00010d34 */
extern void  sigint_term_handler(void);                                /* FUN_0000a3ec (无参形态) */
extern int   switch_monitor_thread(void);                              /* FUN_0000a7f4 */
extern void  cmd_deal_thread(void);                                    /* FUN_0000a680 */
extern void  respond_queue_thread(void);                               /* FUN_0000a514 */

/* ------------------------------------------------------------------ */
/* 本文件内部函数前向声明                                              */
/* ------------------------------------------------------------------ */

static int  mon_ssl_recv(SSL *ssl, int fd, void *buf, int len);
static int  mon_ssl_send(SSL *ssl, int fd, void *buf, int len);
static int  mon_tcp_recv(SSL *ssl, int fd, void *buf, int len);
static int  mon_tcp_send(SSL *ssl, int fd, void *buf, int len);

void tcp_state(int sig);            /* 供 main 的 signal() 注册 */
int  create_pid_file(void);
int  recv_server_pkg(void);
int  send_getstatus_res(char *cmd);
char *monitor_conf_get_value(char *conf_path, char *key, char *value_out);
/* 其余函数定义见后文 (均按原地址顺序排列) */

/* ------------------------------------------------------------------ */
/* 匿名收发包装 (原二进制中以函数指针引用的代码块, 无独立 FUN_ 导出)    */
/* ------------------------------------------------------------------ */

// === mon_ssl_recv (匿名包装) === 原 匿名代码块 @ 0x00010c84
// 来源: reversed/monitor_funcs/ (匿名代码块, 地址位于 FUN_00010bfc 区间内, 反汇编确认)
// 语义: g_r_connect==1 时执行 SSL_read(ssl, buf, len), 否则返回 -1
static int mon_ssl_recv(SSL *ssl, int fd, void *buf, int len)
{
    (void)fd;   /* 原始包装忽略 fd 实参 */
    if (g_r_connect == 1) {
        return SSL_read(ssl, buf, len);
    }
    return -1;
}

// === mon_ssl_send (匿名包装) === 原 匿名代码块 @ 0x00010cac
// 来源: reversed/monitor_funcs/ (匿名代码块, 反汇编确认)
// 语义: g_r_connect==1 时执行 SSL_write(ssl, buf, len), 否则返回 -1
static int mon_ssl_send(SSL *ssl, int fd, void *buf, int len)
{
    (void)fd;
    if (g_r_connect == 1) {
        return SSL_write(ssl, buf, len);
    }
    return -1;
}

// === mon_tcp_recv (匿名包装) === 原 匿名代码块 @ 0x00010cd4
// 来源: reversed/monitor_funcs/ (匿名代码块, 反汇编确认)
// 语义: g_r_connect==1 时执行 recv(fd, buf, len, 0), 否则返回 -1 (忽略 ssl 实参)
static int mon_tcp_recv(SSL *ssl, int fd, void *buf, int len)
{
    (void)ssl;
    if (g_r_connect == 1) {
        return (int)recv(fd, buf, (size_t)len, 0);
    }
    return -1;
}

// === mon_tcp_send (匿名包装) === 原 匿名代码块 @ 0x00010d04
// 来源: reversed/monitor_funcs/ (匿名代码块, 反汇编确认)
// 语义: g_r_connect==1 时执行 send(fd, buf, len, 0), 否则返回 -1 (忽略 ssl 实参)
static int mon_tcp_send(SSL *ssl, int fd, void *buf, int len)
{
    (void)ssl;
    if (g_r_connect == 1) {
        return (int)send(fd, buf, (size_t)len, 0);
    }
    return -1;
}

/* ------------------------------------------------------------------ */
/* 函数定义 (按原地址顺序)                                             */
/* ------------------------------------------------------------------ */

// === main === 原 FUN_00009fa0 @ 0x00009fa0
// 来源: reversed/monitor_funcs/FUN_00009fa0.c | 分类: monitor_funcs
// 语义: 守护进程入口。pid 文件单例检查 → 初始化信号/队列/命令链表 → 建立服务器
//       连接 (sock_connect_server + ssl_connect, 失败降级明文) → 注册信号处理器 →
//       创建命令/应答线程 → 主循环: 队列未满时收包并按命令类型 ('f'=应答) 入队。
//       原始入口只使用 argc (argv/envp 未用); 带参数启动时先 sleep 40s。
int main(int argc, char *argv[], char *envp[])
{
    (void)argv;
    (void)envp;
    /* 包体栈副本 (0x1008): 原二进制中 pkg_queue_push 以 "12B 头经寄存器 + 0x1003B
     * 体经栈" 的隐式约定从调用方栈上读取包体, 本 memcpy 即预载动作; p2 的
     * push(q, pkg) 直接复制 g_recv_pkg, 落槽内容等价 (原样保留)。 */
    char body_copy[0x1008];
    int ret;

    if (create_pid_file() == 0) {
        puts("process exist!");
        return 1;
    }
    if (argc > 1) {
        sleep(40);                          /* 0x28: 带参数启动时延迟 40s */
    }
    init_signal_mask();
    init_pkg_queues(&g_cmd_queue, &g_respond_queue);
    cmd_list_init(g_cmd_list_head);         /* 原样传 DAT_0001d420 当前值 (NULL) */

    if (pthread_create(&g_tid_switch, NULL,
                       (void *(*)(void *))switch_monitor_thread, NULL) != 0) {
        puts("Create pthread error!");
        return 1;
    }
    sleep(1);
    if (g_web_switch_off == 1) {
        puts("Web  Switch is OFF!");
        exit(1);
    }

    while ((ret = sock_connect_server()) == -1) {
        sleep(30);                          /* 0x1e: 服务器连接失败每 30s 重试 */
    }
    ret = ssl_connect();
    if (ret == -1) {
        /* SSL 握手失败: 降级为明文收发并重新连接 TCP */
        g_recv_fn = mon_tcp_recv;
        g_send_fn = mon_tcp_send;
        puts("encrypt connect failed.");
        close(g_sockfd);
        sock_connect_server();
    }
    else {
        g_recv_fn = mon_ssl_recv;
        g_send_fn = mon_ssl_send;
    }

    setup_timer_signals(30);                /* 0x1e */
    signal(10, tcp_state);                  /* SIGUSR1: callapi 报告本地 API 断线 */
    signal(12, tcp_state);                  /* SIGUSR2: 重连后恢复通知 */
    signal(2, (void (*)(int))sigint_term_handler);      /* SIGINT (原处理器无参形态) */
    signal(15, (void (*)(int))sigint_term_handler);     /* SIGTERM */

    g_pthread_ret = pthread_create(&g_tid_cmd_deal, NULL,
                                   (void *(*)(void *))cmd_deal_thread, NULL);
    if (g_pthread_ret == 0) {
        g_pthread_ret = pthread_create(&g_tid_respond, NULL,
                                       (void *(*)(void *))respond_queue_thread, NULL);
        if (g_pthread_ret == 0) {
            while (g_running != 0) {
                /* 队列满判断: 模数 20 与 push 内部的 9 不一致, 原样保留 */
                if (((g_cmd_queue.tail + 1) % MON_MAIN_FULL_MODULUS == g_cmd_queue.head) ||
                    ((g_respond_queue.tail + 1) % MON_MAIN_FULL_MODULUS == g_respond_queue.head)) {
                    puts("the queue is full");
                    sleep(5);
                }
                else {
                    ret = recv_serpkg_struct();
                    if (ret == -1) {
                        puts("Bad Package REV!");
                        sleep(1);
                    }
                    else if (g_recv_pkg.ser_cmd[0] == 'f') {    /* 应答类命令 → 应答队列 */
                        puts("this is respond ");
                        memcpy(body_copy, (char *)&g_recv_pkg + 0xC, MON_PKG_BODY_SIZE);
                        pkg_queue_push(&g_respond_queue, &g_recv_pkg);
                    }
                    else {                              /* 命令类 → 命令队列 */
                        puts("this is command");
                        memcpy(body_copy, (char *)&g_recv_pkg + 0xC, MON_PKG_BODY_SIZE);
                        pkg_queue_push(&g_cmd_queue, &g_recv_pkg);
                    }
                }
            }
            return 1;
        }
    }
    puts("Create pthread error!");
    return 1;
}

// === tcp_state === 原 FUN_0000ad18 @ 0x0000ad18
// 来源: reversed/monitor_funcs/FUN_0000ad18.c | 分类: monitor_funcs
// 语义: SIGUSR1/SIGUSR2 处理器。SIGUSR1 (callapi 连接失败): 停 itimer, 置离线,
//       关 fd, 清空两个队列。SIGUSR2: 清队列后用 TCP_INFO 检查连接状态, 断线则
//       复位收发全局、重新 sock_connect_server + ssl_connect (失败降级明文)。
void tcp_state(int sig)
{
    char stack_pad[128];                    /* 原始帧 128 字节填充区, 未使用 (保留布局) */
    char offline_bak[0xfff];                /* 断线时对 g_err_pkg.body 起始 0xfff 字节的栈上备份 */
    char tcp_info[108];                     /* struct tcp_info 原始字节 (首字节 = tcpi_state) */
    socklen_t info_len;
    int ret;

    printf("receive signal :%d\n", sig);
    if (sig == 10) {                        /* SIGUSR1 */
        set_itimer(6000000, 6000000);       /* 停用实时定时器 (超大间隔) */
        g_r_connect = 0;
        close(g_sockfd);
        clear_pkg_queue(&g_cmd_queue);
        clear_pkg_queue(&g_respond_queue);
        return;
    }
    if (sig != 12) {                        /* 只处理 SIGUSR2 */
        return;
    }
    clear_pkg_queue(&g_cmd_queue);
    clear_pkg_queue(&g_respond_queue);
    set_itimer(30, 30);
    info_len = 0x68;                        /* 104 = sizeof(struct tcp_info) */
    if (g_web_switch_off == 1) {
        puts("Web  Switch is OFF!");
        exit(1);
    }
    getsockopt(g_sockfd, 6 /*IPPROTO_TCP*/, 0xb /*TCP_INFO*/, tcp_info, &info_len);
    if (tcp_info[0] == '\x01') {            /* tcpi_state == TCP_ESTABLISHED */
        puts("Connect to normal!!!");
    }
    else {
        puts("Off Line!!! in tcp_state()");
        g_r_connect = 0;
        memcpy(offline_bak, g_err_pkg.body, 0xfff);     /* 原 &DAT_0001c3e4 (g_err_pkg+0x10 起 0xfff 至记录末尾) */
        pkg_globals_reset(&g_err_pkg);                  /* 原 pkg_globals_reset(DAT_0001c3d4..c3e0) */
        memset(DAT_00026ae0 + 0x244, 0, 0x46);          /* 原 &DAT_00026d24, 70 字节 */
        ret = sock_connect_server();
        if (ret == -1) {
            puts("tcp_state, sock_connect_server failed!");
        }
        else {
            ret = ssl_connect();
            if (ret == -1) {
                g_recv_fn = mon_tcp_recv;
                g_send_fn = mon_tcp_send;
                close(g_sockfd);
                sock_connect_server();
                puts("encrypt connect failed.");
            }
            else {
                g_recv_fn = mon_ssl_recv;
                g_send_fn = mon_ssl_send;
            }
        }
    }
    return;
}

// === create_pid_file === 原 FUN_0000b088 @ 0x0000b088
// 来源: reversed/monitor_funcs/FUN_0000b088.c | 分类: monitor_funcs
// 语义: 单例锁。pid 文件已存在时以 "w" 打开并尝试 LOCK_EX|LOCK_NB: 成功则写入
//       进程号并返回 1 (程序继续), 失败 fclose 后返回 0 (main 报 "process exist!")。
//       文件不存在时逐级 mkdir 后以 LOCK_EX 锁定并写 pid, 恒返回 1。
int create_pid_file(void)
{
    int ret;
    size_t len;
    char *path;
    char *slash;
    FILE *fp;

    if (access(g_pid_path, 0 /*F_OK*/) == 0) {
        g_pid_fp = fopen(g_pid_path, "w");
        fp = NULL;
        if (g_pid_fp != NULL) {
            ret = fileno(g_pid_fp);
            ret = flock(ret, 6 /*LOCK_EX|LOCK_NB*/);
            if (ret == 0) {
                fprintf(g_pid_fp, "The thread executing miner-monitor:%d", (int)getpid());
                puts("exist,write and lock!");
                return 1;
            }
            fp = g_pid_fp;
        }
        fclose(fp);                         /* 原样保留: fopen 失败时 fclose(NULL) */
        return 0;
    }

    /* pid 文件不存在: 按路径逐级创建目录 (原样保留 malloc(len) 但拷贝 len+1 字节) */
    len = strlen(g_pid_path);
    path = malloc(len);
    memcpy(path, g_pid_path, len + 1);
    slash = path;
    while ((slash = strchr(slash + 1, '/')) != NULL) {
        *slash = '\0';
        if (access(path, 0) != 0) {
            mkdir(path, 0x1ff /*0777*/);
        }
        *slash = '/';
    }
    g_pid_fp = fopen(g_pid_path, "w");
    if (g_pid_fp != NULL) {
        puts("created!");
        ret = fileno(g_pid_fp);
        ret = flock(ret, 2 /*LOCK_EX*/);
        if (ret == 0) {
            fprintf(g_pid_fp, "The thread executing miner-monitor:%d", (int)getpid());
            puts("locked!");
            return 1;
        }
    }
    return 1;                               /* flock 失败也返回 1 (原样) */
}

// === parse_option_line === 原 FUN_0000b340 @ 0x0000b340
// 来源: reversed/monitor_funcs/FUN_0000b340.c | 分类: monitor_funcs
// 语义: 解析 uci 风格行 "option <name> '<value>'"。跳过行首空白, 名称止于首个
//       空白 (原地截断), 值必须由单引号包裹 (原地截断右引号)。成功返回 0,
//       失败返回 0xffffffff。*cursor 不会被回写 (原样)。
unsigned parse_option_line(char **cursor, char **name_out, char **value_out)
{
    unsigned char *cur = (unsigned char *)*cursor;   /* 前进指针 */
    unsigned char *prev;                             /* 滞后指针 */
    const unsigned short *ct;
    unsigned char c;

    if (cur == NULL) {
        return 0xffffffffU;
    }
    ct = *__ctype_b_loc();
    /* ① 跳过行首空白: 退出时 prev = 第一个非空白字符 */
    do {
        prev = cur;
        cur = prev + 1;
    } while ((ct[*prev] & CTYPE_SPACE) != 0);

    if ((*prev != '\0') && (strncmp((char *)prev, "option", 6) == 0)) {
        c = prev[6];
        cur = prev + 6;
        if ((ct[c] & CTYPE_SPACE) != 0) {
            /* ② "option " 带空白: 跳过全部空白, cur = 名称首字符 */
            prev = prev + 7;
            do {
                cur = prev;
                c = *cur;
                prev = cur + 1;
            } while ((ct[c] & CTYPE_SPACE) != 0);
        }
        if (c != '\0') {
            *name_out = (char *)cur;
            /* ③ 扫描名称: 退出时 cur = 名称后的第一个空白, prev = 名称末字符 */
            do {
                prev = cur;
                cur = prev + 1;
                if (*cur == '\0') {
                    return 0xffffffffU;
                }
            } while ((ct[*cur] & CTYPE_SPACE) == 0);
            *cur = '\0';                    /* 原地截断名称 */
            cur = prev + 2;                 /* 空白后的首字符 */
            /* ④ 跳过空白: 退出时 c = 值首字符 (期望 '\'', cur = 引号后一字符) */
            do {
                prev = cur + 1;
                c = *cur;
                if (c == '\0') {
                    return 0xffffffffU;
                }
                cur = prev;
            } while ((ct[c] & CTYPE_SPACE) != 0);
            if (c == 0x27) {                /* '\'' */
                char *close;
                *value_out = (char *)cur;
                close = strchr((char *)cur, 0x27);
                if (close != NULL) {
                    *close = '\0';
                    return ct[c] & CTYPE_SPACE;   /* 引号非空白 → 返回 0 表示成功 */
                }
                /* 无右引号 → 落到函数末尾返回失败 (原样) */
            }
        }
    }
    return 0xffffffffU;
}

// === parse_conf_file === 原 FUN_0000b46c @ 0x0000b46c
// 来源: reversed/monitor_funcs/FUN_0000b46c.c | 分类: monitor_funcs
// 语义: 解析 "option <name> '<value>'" 配置文件。跳过空白/'#' 注释/"config" 段头
//       行, 变量写入 DAT_00024490/DAT_0001f468 (p2 的 conf 键/值数组, 上限 40 条),
//       值尾部 '/' 被 str_rstrip_char 去除。返回 g_var_num。
//       fopen 失败仅打印后仍执行 fclose (原样保留)。
int parse_conf_file(char *conf_path)
{
    FILE *fp;
    char *err;
    char *parse_cursor;                     /* 传入解析器的游标 (原样不被回写) */
    char *name;
    char *value;
    size_t len;
    const unsigned short *ct;
    unsigned char *p;
    int lineno;
    int ret;
    char line[776];                         /* 776 字节行缓冲 */

    fp = fopen(conf_path, "r");
    if (fp == NULL) {
        err = strerror(errno);
        printf("can\'t open \'%s\' as config file: %s\n", conf_path, err);
    }
    else {
        lineno = 0;
        for (;;) {
            if (fgets(line, MON_CONF_LINE_GETS, fp) == NULL) {
                break;
            }
            lineno++;
            len = strlen(line);
            if (len >= MON_CONF_LINE_LIMIT) {
                printf("line too long, conf line skipped %s, line %d\n", conf_path, lineno);
                continue;
            }
            if (len == 0) {
                continue;
            }
            ct = *__ctype_b_loc();
            p = (unsigned char *)line;
            /* 跳过行首空白 (受 len 约束); 整行空白则直接读下一行 */
            while (len != 0) {
                if ((ct[*p] & CTYPE_SPACE) == 0) {
                    break;
                }
                len--;
                p++;
            }
            if (len == 0) {
                continue;
            }
            if ((*p != 0x23 /*'#'*/) && (strncmp((char *)p, "config", 6) != 0)) {
                memcpy(line, p, len);       /* 去掉行首空白 */
                line[len] = '\0';
                parse_cursor = line;
                ret = parse_option_line(&parse_cursor, &name, &value);
                if (ret == 0) {
                    if (g_var_num < MON_MAX_CONF_VARS) {
                        len = strlen(name);
                        if (len < MON_CONF_NAME_LEN) {
                            len = strlen(value);
                            if (len < MON_CONF_VALUE_LEN) {
                                strncpy(DAT_00024490 + g_var_num * MON_CONF_NAME_LEN,
                                        name, MON_CONF_NAME_LEN);
                                str_rstrip_char(value, '/');
                                strncpy(DAT_0001f468 + g_var_num * MON_CONF_VALUE_LEN,
                                        value, MON_CONF_VALUE_LEN);
                                g_var_num++;
                            }
                            else {
                                printf("value to long %s, line %d:%d\n", conf_path, lineno,
                                       (int)(parse_cursor - line));
                            }
                        }
                        else {
                            printf("var name to long %s, line %d:%d\n", conf_path, lineno,
                                   (int)(parse_cursor - line));
                        }
                    }
                    else {
                        printf("too many vars in  %s, line %d:%d\n", conf_path, lineno,
                               (int)(parse_cursor - line));
                    }
                }
                else {
                    printf("error parsing %s, line %d:%d\n", conf_path, lineno,
                           (int)(parse_cursor - line));
                }
            }
            /* '#' 注释 / "config" 段头 → 跳过, 继续读下一行 */
        }
    }
    fclose(fp);                             /* 原样保留: fopen 失败时 fclose(NULL) */
    return g_var_num;
}

// === parse_conf_file_kv === 原 FUN_0000b6d4 @ 0x0000b6d4
// 来源: reversed/monitor_funcs/FUN_0000b6d4.c | 分类: monitor_funcs
// 语义: 与 parse_conf_file 相同的存储逻辑, 但使用 "key = \"value\"" 行解析器
//       parse_kv_line, 且只跳过 '#' 注释 (无 "config" 段头检查)。
int parse_conf_file_kv(char *conf_path)
{
    FILE *fp;
    char *err;
    char *parse_cursor;
    char *name;
    char *value;
    size_t len;
    const unsigned short *ct;
    unsigned char *p;
    int lineno;
    int ret;
    char line[776];

    fp = fopen(conf_path, "r");
    if (fp == NULL) {
        err = strerror(errno);
        printf("can\'t open \'%s\' as config file: %s\n", conf_path, err);
    }
    else {
        lineno = 0;
        for (;;) {
            if (fgets(line, MON_CONF_LINE_GETS, fp) == NULL) {
                break;
            }
            lineno++;
            len = strlen(line);
            if (len >= MON_CONF_LINE_LIMIT) {
                printf("line too long, conf line skipped %s, line %d\n", conf_path, lineno);
                continue;
            }
            if (len == 0) {
                continue;
            }
            ct = *__ctype_b_loc();
            p = (unsigned char *)line;
            while (len != 0) {
                if ((ct[*p] & CTYPE_SPACE) == 0) {
                    break;
                }
                len--;
                p++;
            }
            if (len == 0) {
                continue;
            }
            if (*p != 0x23 /*'#'*/) {
                memcpy(line, p, len);
                line[len] = '\0';
                parse_cursor = line;
                ret = parse_kv_line(&parse_cursor, &name, &value);
                if (ret == 0) {
                    if (g_var_num < MON_MAX_CONF_VARS) {
                        len = strlen(name);
                        if (len < MON_CONF_NAME_LEN) {
                            len = strlen(value);
                            if (len < MON_CONF_VALUE_LEN) {
                                strncpy(DAT_00024490 + g_var_num * MON_CONF_NAME_LEN,
                                        name, MON_CONF_NAME_LEN);
                                str_rstrip_char(value, '/');
                                strncpy(DAT_0001f468 + g_var_num * MON_CONF_VALUE_LEN,
                                        value, MON_CONF_VALUE_LEN);
                                g_var_num++;
                            }
                            else {
                                printf("value to long %s, line %d:%d\n", conf_path, lineno,
                                       (int)(parse_cursor - line));
                            }
                        }
                        else {
                            printf("var name to long %s, line %d:%d\n", conf_path, lineno,
                                   (int)(parse_cursor - line));
                        }
                    }
                    else {
                        printf("too many vars in  %s, line %d:%d\n", conf_path, lineno,
                               (int)(parse_cursor - line));
                    }
                }
                else {
                    printf("error parsing %s, line %d:%d\n", conf_path, lineno,
                           (int)(parse_cursor - line));
                }
            }
        }
    }
    fclose(fp);
    return g_var_num;
}

// === load_json_pairs === 原 FUN_0000ba9c @ 0x0000ba9c
// 来源: reversed/monitor_funcs/FUN_0000ba9c.c | 分类: monitor_funcs
// 语义: 读取 JSON 文件 (扁平对象 {"名":"值",...}), 把每个成员的 key/value 拷入
//       两个 malloc 的 char* 数组并经出参返回。返回成员个数; fopen 失败或空文件
//       返回 0 (空文件路径不 fclose, 原样保留)。节点字段按偏移访问:
//       +0x10 valuestring, +0x20 string。
size_t load_json_pairs(const char *path, char ***names_out, char ***values_out)
{
    FILE *fp;
    size_t size;
    char *text;
    void *root;
    size_t count;
    size_t idx;
    size_t key_len;
    size_t val_len;
    void *item;

    fp = fopen(path, "rb+");
    if (fp == NULL) {
        puts("open error0");
        return 0;
    }
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    if (size == 0) {
        return 0;                           /* 原样保留: 此路径未 fclose */
    }
    fseek(fp, 0, SEEK_SET);
    text = malloc(size + 100);
    fread(text, 1, size, fp);
    fclose(fp);

    root = json_parse(text);
    count = (size_t)json_get_array_size(root);   /* 实为对象成员数 (child 链长) */
    *names_out = malloc(count * sizeof(char *));
    *values_out = malloc(count * sizeof(char *));
    if ((int)count > 0) {
        idx = 0;
        do {
            item = json_get_array_item(root, (int)idx);
            key_len = strlen(*(char **)((char *)item + JSON_OFF_STRING));
            item = json_get_array_item(root, (int)idx);
            val_len = strlen(*(char **)((char *)item + JSON_OFF_VALUESTRING));
            (*names_out)[idx] = malloc(key_len + 1);
            (*values_out)[idx] = malloc(val_len + 1);
            item = json_get_array_item(root, (int)idx);
            strcpy((*names_out)[idx], *(char **)((char *)item + JSON_OFF_STRING));
            item = json_get_array_item(root, (int)idx);
            strcpy((*values_out)[idx], *(char **)((char *)item + JSON_OFF_VALUESTRING));
            /* 原样保留: strcpy 后再写一次终止符 (位置相同) */
            (*names_out)[idx][key_len] = '\0';
            (*values_out)[idx][val_len] = '\0';
            idx++;
        } while (idx != count);
    }
    json_delete(root);
    free(text);
    return count;
}

// === format_api_result === 原 FUN_0000bc44 @ 0x0000bc44
// 来源: reversed/monitor_funcs/FUN_0000bc44.c | 分类: monitor_funcs
// 语义: 按 dataformat_*.conf 的 (key,value) 对格式化 cgminer API 应答。
//       param_3 以 '|' 分段: 首段被丢弃 (原始 strrchr 在截断后的首段上执行,
//       恒为空操作, 原样保留); 后续各段以 ',' 或先出现的 '|' 再分割成
//       "key" 与 "key=value" (要求含 '='), key 命中 JSON 时向 param_2 追加
//       "json值=段内值" 并以段分隔符连接。返回 param_2。
//       注意: 段内含 ',' 而无后续 '|' 时原始代码会对 NULL 执行 strncpy (保留)。
char *format_api_result(const char *json_file, char *result, char *resp)
{
    char tmp[104];                          /* sprintf 缓冲 (memset 前 100 字节) */
    char sep[4];                            /* 仅被 strncpy/strncat 读写 1 字节的分隔符暂存 */
    char **names;                           /* JSON key 数组 */
    char **values;                          /* JSON value 数组 */
    int count;
    char *rest;                             /* 段迭代游标 */
    char *next_pos;                         /* 段分隔符后的下一位置 */
    char *val_pos;                          /* '=' 后的段内值 */
    char *comma;
    char *bar;
    char *eq;
    char next_c;
    unsigned use_bar;
    int i;
    size_t len;

    names = NULL;
    values = NULL;
    count = (int)load_json_pairs(json_file, &names, &values);
    printf("sum = %d\n", count);
    memset(tmp, 0, 100);

    /* 首段分割: rest = 第一个 '|' 之后; 若无 '|' 则直接收尾返回 */
    bar = strchr(resp, 0x7c /*'|'*/);
    rest = bar;
    if (bar != NULL) {
        rest = bar + 1;
        *bar = '\0';
    }
    bar = strrchr(resp, 0x7c);              /* 原样保留: 作用于已截断的首段, 恒 NULL */
    if (bar != NULL) {
        *bar = '\0';
    }
    if ((rest == NULL) || (*rest == '\0')) {
        goto format_done;                   /* 原 0x0000bdac */
    }

    for (;;) {
        /* 每段选择分隔符: 优先 ',' 除非 '|' 出现得更早 (或段内无 ',') */
        comma = strchr(rest, 0x2c /*','*/);
        bar = strchr(rest, 0x7c);
        use_bar = (comma == NULL) ? 1U : 0U;
        if (bar < comma) {
            use_bar |= 1U;
        }
        if (use_bar == 0U) {
            strncpy(sep, comma, 1);
            next_pos = comma + 1;
            *comma = '\0';
        }
        else {
            strncpy(sep, bar, 1);           /* bar 可能为 NULL (原始缺陷, 保留) */
            next_pos = NULL;
            if (bar != NULL) {
                next_pos = bar + 1;
                *bar = '\0';
            }
        }
        eq = strchr(rest, 0x3d /*'='*/);
        val_pos = eq;
        if (eq != NULL) {
            val_pos = eq + 1;
            *eq = '\0';
        }
        /* 在 JSON 名称表中查找当前 key */
        if (count > 0) {
            for (i = 0; i != count; i++) {
                if (strcmp(names[i], rest) == 0) {
                    sprintf(tmp, "%s=%s", values[i], val_pos);
                    strcat(result, tmp);
                    if ((next_pos == NULL) || (*next_pos == '\0')) {
                        goto format_done;
                    }
                    strncat(result, sep, 1);
                    next_c = *next_pos;
                    goto format_next;
                }
            }
        }
        if (next_pos == NULL) {
            goto format_done;
        }
        next_c = *next_pos;
format_next:
        if (next_c == '\0') {
            goto format_done;
        }
        rest = next_pos;
    }

format_done:                                /* 原 0x0000bdac: 收尾 (同时覆盖入口空段路径) */
    len = strlen(result);
    if (result[len - 1] == ',' || result[len - 1] == '|') {
        result[len - 1] = '\0';             /* 原样保留: result 为空串时读 result[-1] */
    }
    if (count > 0) {
        for (i = 0; i != count; i++) {
            if (names[i] != NULL) {
                free(names[i]);
            }
            if (values[i] != NULL) {
                free(values[i]);
            }
        }
    }
    if (names != NULL) {
        free(names);
    }
    if (values != NULL) {
        free(values);
    }
    printf("after format :%s\n\n", result);
    return result;
}

// === callapi === 原 FUN_0000bea4 @ 0x0000bea4
// 来源: reversed/monitor_funcs/FUN_0000bea4.c | 分类: monitor_funcs
// 语义: 向 host:port 发送 request 文本并回收应答 (cgminer 本地 API 调用)。
//       connect 失败首次触发 SIGUSR1 (10) 并每 5s 重试直至成功; 若曾失败,
//       成功后触发 SIGUSR2 (12) 并返回 ""。malloc(0xffdd) 失败 /
//       realloc 失败触发 __assert_fail("buf","cgminer_api.c")。
//       解析/socket 失败返回 NULL (buf 泄漏, 原样保留)。
char *callapi(const char *request, const char *host, int port)
{
    char *buf;
    struct hostent *he;
    int fd;
    ssize_t n;
    int got;
    int retried;
    size_t cap;
    char *err;
    struct sockaddr_in addr;
    unsigned short port_be;

    buf = malloc(MON_API_BUF_INIT);
    if (buf == NULL) {
        __assert_fail("buf", "cgminer_api.c", 139 /*0x8b*/, "callapi");
    }
    he = gethostbyname(host);
    if (he == NULL) {
        printf("Failed to resolve host %s\n", host);
        buf = NULL;                         /* 原样保留: buf 泄漏 */
        return buf;
    }
    fd = socket(2 /*AF_INET*/, 1 /*SOCK_STREAM*/, 0);
    if (fd == -1) {
        err = strerror(errno);
        buf = NULL;
        printf("Socket initialisation failed: %s\n", err);
        return buf;
    }
    port_be = (unsigned short)(((unsigned int)port >> 8) & 0xff) |
              (unsigned short)(port << 8);  /* == htons(port) */
    memset(addr.sin_zero, 0, 8);            /* sa_data[6..13] */
    addr.sin_addr = *(struct in_addr *)*he->h_addr_list;
    addr.sin_port = port_be;                /* sa_data[0..1] */
    addr.sin_family = 2;                    /* AF_INET */

    retried = 0;
    while (connect(fd, (struct sockaddr *)&addr, 0x10) < 0) {
        if (retried == 0) {
            puts("Call API error.Disconnect with server!");
            raise(10);                      /* SIGUSR1 → tcp_state */
        }
        retried = 1;
        puts("Call API Failed wait 5s !!");
        sleep(5);
    }
    if (retried) {
        buf = "";
        close(fd);
        raise(12);                          /* SIGUSR2 → tcp_state */
        return buf;
    }

    n = send(fd, request, strlen(request), 0);
    if (n < 0) {
        err = strerror(errno);
        printf("Send failed: %s\n", err);
    }
    else {
        puts("Send succed");
        buf[0] = '\0';
        cap = MON_API_CHUNK;
        got = 0;
        for (;;) {
            puts("begin to Recv");
            n = recv(fd, buf + got, MON_API_CHUNK, 0);
            printf("Recved n: %d\n", (int)n);
            if (n < 0) {
                err = strerror(errno);
                printf("Recv failed: %s\n", err);
                break;
            }
            if (n == 0) {
                break;
            }
            got += (int)n;
            buf[got] = '\0';
            if (cap < (size_t)((unsigned int)got + MON_API_CHUNK)) {
                cap <<= 1;
                buf = realloc(buf, cap);
                if (buf == NULL) {
                    __assert_fail("buf", "cgminer_api.c", 187 /*0xbb*/, "callapi");
                }
            }
        }
        close(fd);
        printf("the result of callapi about %s: %s\n\n", request, buf);
    }
    return buf;
}

// === parse_hex4 === 原 FUN_0000c130 @ 0x0000c130
// 来源: reversed/monitor_funcs/FUN_0000c130.c | 分类: monitor_funcs
// 语义: 把 4 个十六进制字符 ('0'-'9','A'-'F','a'-'f') 解析为 int; 任一字符
//       非法立即返回 0。算术形式与原指令一致 (unsigned 截断语义)。
int parse_hex4(const unsigned char *p)
{
    unsigned int c;
    unsigned int v;
    int r;

    c = (unsigned int)p[0];
    v = c - 0x30;
    if ((v & 0xff) > 9) {                   /* 非 '0'..'9' */
        if (c - 0x41 < 6) {                 /* 'A'..'F' */
            v = c - 0x37;
        }
        else {
            if (5 < c - 0x61) {             /* 非 'a'..'f' */
                return 0;
            }
            v = c - 0x57;
        }
    }
    r = (int)(v * 0x10);

    c = (unsigned int)p[1];
    if (((c - 0x30) & 0xff) < 10) {
        r = (int)(c - 0x30) + r;
    }
    else if (c - 0x41 < 6) {
        r = (int)(c - 0x37) + r;
    }
    else {
        if (5 < c - 0x61) {
            return 0;
        }
        r = (int)(c - 0x57) + r;
    }

    c = (unsigned int)p[2];
    v = c - 0x30;
    if ((v & 0xff) > 9) {
        if (c - 0x41 < 6) {
            v = c - 0x37;
        }
        else {
            if (5 < c - 0x61) {
                return 0;
            }
            v = c - 0x57;
        }
    }
    r = (int)(v + (unsigned int)r * 0x10) * 0x10;   /* (v2 + r*16) * 16 */

    c = (unsigned int)p[3];
    if (((c - 0x30) & 0xff) < 10) {
        return (int)(c - 0x30) + r;
    }
    if (c - 0x41 < 6) {
        return (int)(c - 0x37) + r;
    }
    if (c - 0x61 < 6) {
        return (int)(c - 0x57) + r;
    }
    return 0;
}

// === recv_server_pkg === 原 FUN_0000e8dc @ 0x0000e8dc
// 来源: reversed/monitor_funcs/FUN_0000e8dc.c | 分类: monitor_funcs
// 语义: 从服务器收一包到 g_recv_pkg: 2B 包头 (须为 "00", pkg_head) → 2B 命令码
//       (ser_cmd, 第 3 字节补 NUL) → 2B 大端长度 sum → sum 字节 nonce (nonce 字段)。
//       返回 1=成功, 0=离线/收满 0 字节 (置 g_r_connect=0 并复位全局),
//       -1=包错误。头部接收失败时整体复位 (备份 g_err_pkg 体 + pkg_globals_reset
//       + 清空两队列)。
int recv_server_pkg(void)
{
    unsigned char sum_buf[2];               /* 2 字节大端长度暂存 */
    char unused[104];                       /* 原始帧局部, 仅 memset (原样保留) */
    char offline_bak[4096];                 /* 0xfff 字节断线备份 (原样保留) */
    char head_c3;
    unsigned int sum;
    int n;

    memset(unused, 0, 100);                 /* 原始局部清零, 原样保留 */
    printf("r_connect = %d.\n", g_r_connect);
    g_last_recv_ret = g_recv_fn(g_ssl, g_sockfd, g_recv_pkg.pkg_head, 2);
    if (g_last_recv_ret == -1) {
        puts("recv data failed1.");
        g_r_connect = 0;
        memcpy(offline_bak, g_err_pkg.body, 0xfff);     /* 原 &DAT_0001c3e4 */
        pkg_globals_reset(&g_err_pkg);                  /* 原 pkg_globals_reset(DAT_0001c3d4..c3e0) */
        clear_pkg_queue(&g_cmd_queue);
        clear_pkg_queue(&g_respond_queue);
        return 0;
    }
    printf("recv package.pkg_head:%s.\n", g_recv_pkg.pkg_head);
    n = g_last_recv_ret;
    g_recv_pkg.pkg_head[n] = '\0';          /* 按实收字节数截断 */
    head_c3 = g_recv_pkg.pkg_head[2];
    if (n == 0) {
        puts("the first two byte not 00   it is 0");
        return 0;
    }
    if ((g_recv_pkg.pkg_head[0] == '0') && (g_recv_pkg.pkg_head[1] == '0') &&
        (g_recv_pkg.pkg_head[2] == '\0')) {
        g_last_recv_ret = g_recv_fn(g_ssl, g_sockfd, g_recv_pkg.ser_cmd, 2);
        if (g_last_recv_ret == -1) {
            g_r_connect = 0;
            return 0;
        }
        if (g_last_recv_ret == 0) {
            return 0;
        }
        if (g_last_recv_ret == 2) {
            g_recv_pkg.ser_cmd[2] = head_c3;    /* 命令串终止符 (head_c3 恒为 0) */
            printf("rev from server cmd = \n%s\n", g_recv_pkg.ser_cmd);
            g_last_recv_ret = g_recv_fn(g_ssl, g_sockfd, sum_buf, 2);
            if (g_last_recv_ret == -1) {
                g_r_connect = 0;
                return 0;
            }
            if (g_last_recv_ret == 0) {
                return 0;
            }
            if (g_last_recv_ret == 2) {
                sum = ((unsigned int)sum_buf[0] << 8) | (unsigned int)sum_buf[1];   /* 大端 */
                printf("rev from server sum=\n%d\n", sum);
                if (sum == 0) {
                    g_recv_pkg.nonce[0] = (char)sum_buf[1];     /* 原样保留 */
                    return 1;
                }
                n = g_recv_fn(g_ssl, g_sockfd, g_recv_pkg.nonce, (int)sum);
                g_last_recv_ret = n;
                if (n == -1) {
                    g_r_connect = 0;
                    g_last_recv_ret = n;
                    return 0;
                }
                g_recv_pkg.nonce[sum] = '\0';
                printf("recvbytes:%d,sum:%d,pkgnonce=%s\n", g_last_recv_ret, sum, g_recv_pkg.nonce);
                if (g_last_recv_ret == 0) {
                    g_r_connect = 0;
                    return 0;
                }
                if (sum == (unsigned int)g_last_recv_ret) {
                    return 1;
                }
                puts("PAKGE ERROR!!!!!");
            }
        }
    }
    return -1;
}

// === send_login_pkg === 原 FUN_0000edfc @ 0x0000edfc
// 来源: reversed/monitor_funcs/FUN_0000edfc.c | 分类: monitor_funcs
// 语义: 登录上报 (命令 "10", 由 deal_cmd 触发)。组包 "nonce+MAC+IP=x,+UID=y,+
//       TYPE=7", 总长 & 0xff, 经 send_server_pkg 发包头/命令/长度后发送包体。
//       nonce 落位在 g_cmd_pkg.nonce (原 DAT_0001e44c)。
void send_login_pkg(char *ser_cmd)
{
    char ip_uid[104];                       /* [0..39]=IP 区, [40..99]=UID 值区 */
    char type_str[12];                      /* "TYPE=.." 字符串缓冲 */
    char *mac_buf;                          /* calloc(0x1e) */
    char *ip_str;                           /* calloc(0x19) */
    char *uid_str;                          /* calloc(0x3c) */
    char *uid_val;
    char *nonce;
    char *mac;
    char *pkg;
    char *end;
    size_t nonce_len;
    size_t mac_len;
    unsigned int pkg_len;
    int ret;

    memset(ip_uid, 0, MON_LOGIN_IP_BUF);            /* IP 区清零 */
    memset(type_str, 0, 10);                        /* 原始代码逐字节清零 10 字节 */
    memset(ip_uid + MON_LOGIN_IP_BUF, 0, MON_LOGIN_UID_BUF);    /* UID 值区清零 */
    mac_buf = calloc(0x1e, 1);
    ip_str = calloc(0x19, 1);
    uid_str = calloc(0x3c, 1);
    ret = get_local_ip("eth0", ip_uid);
    if (ret == 0) {
        puts("get ip error!");
    }
    sprintf(ip_str, "IP=%s,", ip_uid);
    strncpy(uid_str, "UID=", 5);
    uid_val = monitor_conf_get_value(g_conf_path, "api-uid", ip_uid + MON_LOGIN_IP_BUF);
    end = stpcpy(uid_str + strlen(uid_str), uid_val);
    end[0] = ',';
    end[1] = '\0';
    sprintf(type_str, "TYPE=%d", MON_TYPE_ID);
    nonce = gen_pkg_nonce(g_cmd_pkg.nonce);         /* 原 &DAT_0001e44c */
    nonce_len = strlen(nonce);
    mac = get_iface_mac(mac_buf, "eth0");
    mac_len = strlen(mac);
    pkg_len = (unsigned int)(mac_len + nonce_len + strlen(ip_str) + strlen(uid_str) +
                             strlen(type_str)) & 0xff;
    send_server_pkg(ser_cmd, pkg_len);
    pkg = calloc(pkg_len + 4, 1);
    end = stpcpy(pkg, g_cmd_pkg.nonce);
    end = stpcpy(end, mac);
    end = stpcpy(end, ip_str);
    end = stpcpy(end, uid_str);
    strcpy(end, type_str);
    g_last_send_ret = g_send_fn(g_ssl, g_sockfd, pkg, (int)pkg_len);
    if (g_last_send_ret == -1) {
        perror("send");
    }
    printf("send pakge str :%s\n", pkg);
    free(mac_buf);
    free(ip_str);
    free(uid_str);
    free(pkg);
    return;
}

// === send_getstatus_res === 原 FUN_0000f108 @ 0x0000f108
// 来源: reversed/monitor_funcs/FUN_0000f108.c | 分类: monitor_funcs
// 语义: 状态查询应答 (命令码 "01".."07")。按匹配项取三张并行表:
//       dataformat JSON 文件 / cgminer API 命令 / 服务器命令码 ("10".."70"),
//       callapi(cmd, 本机IP, port_local) 后经 format_api_result 组包, 发送
//       send_server_pkg(ser_cmd, body+nonce) + nonce (g_cmd_pkg.nonce) + body。
//       未匹配时三个指针保持未初始化并继续执行 (原始缺陷, 原样保留);
//       返回 0=无应答, 1=已发送。
int send_getstatus_res(char *cmd)
{
    /* 查询码表: [0] 不参与匹配 (扫描自索引 1 开始, 反汇编确认) */
    static const char *status_scan[8] = {
        "lcd", "01", "02", "03", "04", "05", "06", "07"
    };
    /* dataformat JSON 文件表 (与 status_scan[1..7] 平行) */
    static const char *status_fmt_file[7] = {
        "",
        "/config/dataformatconfig/dataformat_devs.conf",
        "/config/dataformatconfig/dataformat_summary.conf",
        "/config/dataformatconfig/dataformat_stats.conf",
        "/config/dataformatconfig/dataformat_pools.conf",
        "",
        "/config/dataformatconfig/dataformat_lcds.conf"
    };
    /* cgminer 本地 API 命令表 */
    static const char *status_api_cmd[7] = {
        "AUTH", "devs", "summary", "stats", "pools", "", "lcd"
    };
    /* 服务器命令码表 */
    static const char *status_ser_cmd[7] = {
        "10", "20", "30", "40", "50", "60", "70"
    };

    char *fmt_file;                         /* 未匹配时未初始化 (原样保留) */
    char *api_cmd;                          /* 未匹配时未初始化 (原样保留) */
    char *ser_cmd;                          /* 未匹配时未初始化 (原样保留) */
    char *body;
    char *resp;
    char *out;
    const char *s;
    char port_val[MON_LOCAL_PORT_BUF];
    char ip_buf[MON_LOCAL_IP_BUF];
    char *port_str;
    long port;
    size_t nonce_len;
    size_t body_len;
    int i;
    int ret;

    i = 0;
    do {
        if (strcmp(cmd, status_scan[i + 1]) == 0) {
            s = status_fmt_file[i];
            fmt_file = malloc(strlen(s) + 1);
            memset(fmt_file, 0, strlen(s) + 1);
            strcpy(fmt_file, s);
            s = status_api_cmd[i];
            api_cmd = malloc(strlen(s) + 1);
            memset(api_cmd, 0, strlen(s) + 1);
            strcpy(api_cmd, s);
            s = status_ser_cmd[i];
            ser_cmd = malloc(strlen(s) + 1);
            memset(ser_cmd, 0, strlen(s) + 1);
            strcpy(ser_cmd, s);
            break;
        }
        i++;
    } while (i != 7);
    /* 原始代码: 未匹配时不初始化 fmt_file/api_cmd/ser_cmd, 继续往下执行 */

    body = calloc(0x1000, 1);
    s = gen_pkg_nonce(g_cmd_pkg.nonce);             /* 原 &DAT_0001e44c */
    nonce_len = strlen(s);
    port_str = monitor_conf_get_value(g_conf_path, "port_local", port_val);
    port = strtol(port_str, NULL, 10);
    ret = get_local_ip("eth0", ip_buf);
    if (ret == 0) {
        puts("get IP error");
    }
    resp = callapi(api_cmd, ip_buf, (int)(short)port);
    if (*resp == '\0') {
        printf("nothing returned form callapi\n\n\n ");
        return 0;
    }
    printf("result of callapi:%s\n ", resp);
    out = format_api_result(fmt_file, body, resp);
    body_len = strlen(out);
    printf("state_len=%d\n", (int)(body_len + nonce_len));
    send_server_pkg(ser_cmd, (unsigned int)(body_len + nonce_len));
    g_last_send_ret = g_send_fn(g_ssl, g_sockfd, g_cmd_pkg.nonce, (int)nonce_len);
    if (g_last_send_ret == -1) {
        perror("send");
    }
    g_last_send_ret = g_send_fn(g_ssl, g_sockfd, out, (int)body_len);
    if (g_last_send_ret == -1) {
        perror("send");
    }
    printf("sendbytes=%d\n", g_last_send_ret);
    printf("send pakge str :%s\n", out);
    if (ser_cmd != NULL) {
        free(ser_cmd);
    }
    if (fmt_file != NULL) {
        free(fmt_file);
    }
    if (api_cmd != NULL) {
        free(api_cmd);
    }
    free(resp);
    free(body);
    return 1;
}

// === restart_cgminer === 原 FUN_0000f4c0 @ 0x0000f4c0
// 来源: reversed/monitor_funcs/FUN_0000f4c0.c | 分类: monitor_funcs
// 语义: 执行 "/etc/init.d/cgminer restart" (期间阻塞 SIGPIPE), 5s 后向本机
//       port_local 发起 TCP 连接探测 cgminer 是否恢复: 最多 6 次间隔 5s 重试,
//       第 7 次尝试无论成败均视为超时。返回 1=重启 OK, 0=解析/socket 失败,
//       0xff=重启超时。
int restart_cgminer(void)
{
    char port_val[MON_LOCAL_PORT_BUF];      /* port_local 值缓冲 */
    char ip_buf[40];                        /* 本机 IP 缓冲 */
    sigset_t pipe_set;                      /* SIGPIPE 屏蔽集 */
    struct sockaddr_in addr;
    struct hostent *he;
    char *port_str;
    char *err;
    long port;
    int fd;
    int ret;
    int retry;
    unsigned short port_be;

    port_str = monitor_conf_get_value(g_conf_path, "port_local", port_val);
    port = strtol(port_str, NULL, 10);
    ret = get_local_ip("eth0", ip_buf);
    if (ret == 0) {
        puts("get IP error");
    }
    sigemptyset(&pipe_set);
    sigaddset(&pipe_set, 0xd /*SIGPIPE*/);
    sigprocmask(0 /*SIG_BLOCK*/, &pipe_set, NULL);
    system("/etc/init.d/cgminer restart");
    sigprocmask(1 /*SIG_UNBLOCK*/, &pipe_set, NULL);
    sleep(5);
    he = gethostbyname(ip_buf);
    if (he == NULL) {
        puts("Failed to resolve host ");
        return 0;
    }
    fd = socket(2 /*AF_INET*/, 1 /*SOCK_STREAM*/, 0);
    if (fd == -1) {
        err = strerror(errno);
        printf("Socket initialisation failed: %s\n", err);
        return 0;
    }
    port_be = (unsigned short)(((unsigned long)port >> 8) & 0xff) |
              (unsigned short)(port << 8);  /* == htons(port) */
    memset(addr.sin_zero, 0, 8);
    addr.sin_addr = *(struct in_addr *)*he->h_addr_list;
    addr.sin_port = port_be;
    addr.sin_family = 2;
    retry = MON_TCP_RETRY_MAX;              /* 6: 首连 + 6 次重试, 第 7 次必超时 */
    while ((ret = connect(fd, (struct sockaddr *)&addr, 0x10)) < 0) {
        if (retry == 0) {
            close(fd);
            goto restart_timeout;           /* 原 0x0000f600 */
        }
        retry--;
        err = strerror(errno);
        printf("Cgminer!Socket connect failed: %s\n", err);
        puts("restart not well!!\nwait next 5s......");
        sleep(5);
    }
    close(fd);
    if (retry < 1) {
restart_timeout:                            /* 原 0x0000f600 */
        puts("restart timeout!");
        return 0xff;
    }
    puts("restart OK!");
    return 1;
}

// === monitor_conf_get_value === 原 FUN_0000f990 @ 0x0000f990
// 来源: reversed/monitor_funcs/FUN_0000f990.c | 分类: monitor_funcs
// 语义: 从 minermonitor.conf (JSON) 读取 key 键的字符串值并 strcpy 到 value_out,
//       返回 value_out。文件打不开只打印 (继续执行, 原样); 空文件返回 NULL。
//       键不存在时 json_get_object_item 返回 NULL 后解引用崩溃 (原样)。
char *monitor_conf_get_value(char *conf_path, char *key, char *value_out)
{
    FILE *fp;
    size_t size;
    char *text;
    void *root;
    void *item;

    fp = fopen(conf_path, "rb+");
    if (fp == NULL) {
        puts("open error1111");
    }
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    if (size == 0) {
        value_out = NULL;
        return value_out;
    }
    fseek(fp, 0, SEEK_SET);
    text = malloc(size + 100);
    fread(text, 1, size, fp);
    fclose(fp);
    root = json_parse(text);
    item = json_get_object_item(root, key);
    strcpy(value_out, *(char **)((char *)item + JSON_OFF_VALUESTRING));
    json_delete(root);
    free(text);
    return value_out;
}

// === conf_update_api_switch === 原 FUN_0000fa54 @ 0x0000fa54
// 来源: reversed/monitor_funcs/FUN_0000fa54.c | 分类: monitor_funcs
// 语义: 更新 minermonitor.conf: 读出原 api-switch/api-uid/ip_server/port_server/
//       port_local 后重写全部键, 其中 "auth" 键写入 auth_value 新值。
//       返回 1=成功, 0=空文件。首次 fopen 失败则 puts+exit(1) (原样);
//       重开 "w+" 失败仅打印 (后续 fputs(NULL) 崩溃, 原样)。
int conf_update_api_switch(char *conf_path, char *auth_value)
{
    FILE *fp;
    size_t size;
    char *text;
    void *root;
    void *item;
    void *obj;
    void *node;
    char *printed;
    char sw_val[16];                        /* api-switch 原值 */
    char uid_val[60];                       /* api-uid 原值 */
    char ip_val[20];                        /* ip_server 原值 */
    char port_srv_val[12];                  /* port_server 原值 */
    char port_loc_val[12];                  /* port_local 原值 */

    fp = fopen(conf_path, "rb+");
    if (fp == NULL) {
        puts("open error");
        exit(1);
    }
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    if (size == 0) {
        return 0;                           /* 原样保留: 此路径未 fclose */
    }
    fseek(fp, 0, SEEK_SET);
    text = malloc(size + 100);
    fread(text, 1, size, fp);
    fclose(fp);
    root = json_parse(text);
    item = json_get_object_item(root, "api-switch");
    strcpy(sw_val, *(char **)((char *)item + JSON_OFF_VALUESTRING));
    item = json_get_object_item(root, "api-uid");
    strcpy(uid_val, *(char **)((char *)item + JSON_OFF_VALUESTRING));
    item = json_get_object_item(root, "ip_server");
    strcpy(ip_val, *(char **)((char *)item + JSON_OFF_VALUESTRING));
    item = json_get_object_item(root, "port_server");
    strcpy(port_srv_val, *(char **)((char *)item + JSON_OFF_VALUESTRING));
    item = json_get_object_item(root, "port_local");
    strcpy(port_loc_val, *(char **)((char *)item + JSON_OFF_VALUESTRING));
    json_delete(root);

    fp = fopen(conf_path, "w+");
    if (fp == NULL) {
        puts("open error");
    }
    obj = json_create_object();
    node = json_create_string(sw_val);
    json_add_item_to_object(obj, "api-switch", node);
    node = json_create_string(uid_val);
    json_add_item_to_object(obj, "api-uid", node);
    node = json_create_string(auth_value);
    json_add_item_to_object(obj, "auth", node);
    node = json_create_string(ip_val);
    json_add_item_to_object(obj, "ip_server", node);
    node = json_create_string(port_srv_val);
    json_add_item_to_object(obj, "port_server", node);
    node = json_create_string(port_loc_val);
    json_add_item_to_object(obj, "port_local", node);
    printed = json_print(obj);
    fputs(printed, fp);
    fclose(fp);
    json_delete(obj);
    free(printed);
    free(text);
    return 1;
}

// === conf_reset_api_switch === 原 FUN_0000fc84 @ 0x0000fc84
// 来源: reversed/monitor_funcs/FUN_0000fc84.c | 分类: monitor_funcs
// 语义: 复位 minermonitor.conf 的 api-switch: 读出原 "auth"/api-uid/ip_server/
//       port_server/port_local 后重写, api-switch 固定写 "off", "auth" 保持原值。
//       返回 1=成功, 0=空文件。fopen 失败仅打印 (后续对 NULL 文件操作崩溃, 原样)。
int conf_reset_api_switch(char *conf_path)
{
    FILE *fp;
    size_t size;
    char *text;
    void *root;
    void *item;
    void *obj;
    void *node;
    char *printed;
    char auth_val[16];                      /* 原 auth 值 */
    char uid_val[64];                       /* api-uid 原值 */
    char ip_val[20];                        /* ip_server 原值 */
    char port_srv_val[12];                  /* port_server 原值 */
    char port_loc_val[12];                  /* port_local 原值 */

    fp = fopen(conf_path, "rb+");
    if (fp == NULL) {
        puts("open error");
    }
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    if (size == 0) {
        return 0;
    }
    fseek(fp, 0, SEEK_SET);
    text = malloc(size + 100);
    fread(text, 1, size, fp);
    fclose(fp);
    root = json_parse(text);
    item = json_get_object_item(root, "auth");
    strcpy(auth_val, *(char **)((char *)item + JSON_OFF_VALUESTRING));
    item = json_get_object_item(root, "api-uid");
    strcpy(uid_val, *(char **)((char *)item + JSON_OFF_VALUESTRING));
    item = json_get_object_item(root, "ip_server");
    strcpy(ip_val, *(char **)((char *)item + JSON_OFF_VALUESTRING));
    item = json_get_object_item(root, "port_server");
    strcpy(port_srv_val, *(char **)((char *)item + JSON_OFF_VALUESTRING));
    item = json_get_object_item(root, "port_local");
    strcpy(port_loc_val, *(char **)((char *)item + JSON_OFF_VALUESTRING));
    json_delete(root);

    fp = fopen(conf_path, "w+");
    if (fp == NULL) {
        puts("open error");
    }
    obj = json_create_object();
    node = json_create_string("off");
    json_add_item_to_object(obj, "api-switch", node);
    node = json_create_string(uid_val);
    json_add_item_to_object(obj, "api-uid", node);
    node = json_create_string(auth_val);
    json_add_item_to_object(obj, "auth", node);
    node = json_create_string(ip_val);
    json_add_item_to_object(obj, "ip_server", node);
    node = json_create_string(port_srv_val);
    json_add_item_to_object(obj, "port_server", node);
    node = json_create_string(port_loc_val);
    json_add_item_to_object(obj, "port_local", node);
    printed = json_print(obj);
    fputs(printed, fp);
    fclose(fp);
    json_delete(obj);
    free(printed);
    free(text);
    return 1;
}

// === parse_pools_string === 原 FUN_0000feb0 @ 0x0000feb0
// 来源: reversed/monitor_funcs/FUN_0000feb0.c | 分类: monitor_funcs
// 语义: 解析矿池变更命令体。首个 ';'-segment 为设备 ID, 存入 g_api_uid
//       (原 DAT_00026ab8); 其后每个 ';'-segment 按 ',' 分字段, 以 4 态循环
//       状态机填入 pool_conf: 状态0=序号(记录+0x00, 并使 pool_conf+0x1ec 计数
//       自增) → 1=url(+0x04) → 2=user(+0x54) → 3=pass(+0x7c); 段内字段耗尽即
//       进入下一条记录, 状态跨 ';' 段延续。空段/空字段不推进状态。返回 pool_conf。
char *parse_pools_string(char *body, char *pool_table)
{
    char *seg;                              /* 当前 ';' 段起点 */
    char *next_seg;
    char *comma;
    char *after_comma;
    char *id_src;
    int rec;                                /* 记录序号 */
    int state;                              /* 字段状态机 */
    int off;                                /* 记录基偏移 */

    id_src = strchr(body, 0x3b /*';'*/);
    rec = 0;
    state = 0;
    *id_src = '\0';
    strcpy(g_api_uid, body);
    seg = id_src + 1;

    for (;;) {
        next_seg = strchr(seg, 0x3b);
        if (next_seg != NULL) {
            *next_seg = '\0';
            next_seg = next_seg + 1;
        }
        if (*seg == '\0') {
            goto pool_next_seg;             /* 空段直接跳过 */
        }
        /* 字段状态机 (状态跨段延续) */
        for (;;) {
            off = rec * MON_POOL_REC_SIZE;
            comma = strchr(seg, 0x2c /*','*/);
            after_comma = comma;
            if (comma != NULL) {
                after_comma = comma + 1;
                *comma = '\0';
            }
            if (*seg != '\0') {
                switch (state) {
                case 0:
                    strcpy(pool_table + off, seg);
                    state = 1;
                    (*(int *)(pool_table + MON_POOL_OFF_COUNT))++;
                    break;
                case 1:
                    state = 2;
                    strcpy(pool_table + off + MON_POOL_OFF_URL, seg);
                    break;
                case 2:
                    state = 3;
                    strcpy(pool_table + off + MON_POOL_OFF_USER, seg);
                    break;
                case 3:
                    strcpy(pool_table + off + MON_POOL_OFF_PASS, seg);
                    state = 0;
                    break;
                default:
                    puts("Pools Config Data is Wrong");
                }
            }
            if (after_comma == NULL) {
                rec++;                      /* 本段字段取尽 → 下一条记录 */
                break;
            }
            seg = after_comma;              /* 下一个 ',' 字段 */
        }
pool_next_seg:                              /* 原 0x0000ffa4 */
        if (next_seg == NULL) {
            return pool_table;
        }
        seg = next_seg;
    }
}

// === load_cgminer_conf_json === 原 FUN_00010004 @ 0x00010004
// 来源: reversed/monitor_funcs/FUN_00010004.c | 分类: monitor_funcs
// 语义: 读取 /config/cgminer.conf (JSON) 到 conf_buf: pools 数组每项的
//       url/user/pass 写入各记录 (+4/+0x54/+0x7c, 步长 0xa4, 不限条数, 原样),
//       api-listen/api-network/bitmain-nobeeper/bitmain-notempoverctrl 取
//       valueint, api-allow/bitmain-freq/bitmain-voltage 取 valuestring。
//       返回 conf_buf; 空文件返回 NULL。fopen 失败仅打印 (原样)。
char *load_cgminer_conf_json(char *conf_path, char *conf)
{
    FILE *fp;
    size_t size;
    char *text;
    void *root;
    void *item;
    void *pool;                             /* pools 链表游标 (child/next) */
    int rec;
    int rec_off;
    int flag;

    fp = fopen(conf_path, "rb+");
    if (fp == NULL) {
        puts("open error");
    }
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    if (size == 0) {
        conf = NULL;
        return conf;
    }
    fseek(fp, 0, SEEK_SET);
    text = malloc(size + 100);
    fread(text, 1, size, fp);
    fclose(fp);
    root = json_parse(text);
    item = json_get_object_item(root, "pools");
    json_get_array_size(item);              /* 原始代码调用但未使用返回值 (原样保留) */
    pool = *(void **)((char *)item + 8);    /* item->child: 第一个矿池对象 */
    if (pool != NULL) {
        rec = 0;
        do {
            item = json_get_object_item(pool, "url");
            rec_off = rec * MON_POOL_REC_SIZE;
            rec++;
            strcpy(conf + rec_off + MON_POOL_OFF_URL,
                   *(char **)((char *)item + JSON_OFF_VALUESTRING));
            item = json_get_object_item(pool, "user");
            strcpy(conf + rec_off + MON_POOL_OFF_USER,
                   *(char **)((char *)item + JSON_OFF_VALUESTRING));
            item = json_get_object_item(pool, "pass");
            strcpy(conf + rec_off + MON_POOL_OFF_PASS,
                   *(char **)((char *)item + JSON_OFF_VALUESTRING));
            pool = *(void **)pool;          /* item->next */
        } while (pool != NULL);
    }
    item = json_get_object_item(root, "api-listen");
    *(int *)(conf + CGM_OFF_API_LISTEN) = *(int *)((char *)item + JSON_OFF_VALUEINT);
    item = json_get_object_item(root, "api-network");
    *(int *)(conf + CGM_OFF_API_NETWORK) = *(int *)((char *)item + JSON_OFF_VALUEINT);
    item = json_get_object_item(root, "bitmain-nobeeper");
    *(int *)(conf + CGM_OFF_NOBEEPER) = *(int *)((char *)item + JSON_OFF_VALUEINT);
    item = json_get_object_item(root, "bitmain-notempoverctrl");
    *(int *)(conf + CGM_OFF_NOTEMPOVERCTRL) = *(int *)((char *)item + JSON_OFF_VALUEINT);
    item = json_get_object_item(root, "api-allow");
    strcpy(conf + CGM_OFF_API_ALLOW,
           *(char **)((char *)item + JSON_OFF_VALUESTRING));
    item = json_get_object_item(root, "bitmain-freq");
    strcpy(conf + CGM_OFF_FREQ,
           *(char **)((char *)item + JSON_OFF_VALUESTRING));
    item = json_get_object_item(root, "bitmain-voltage");
    strcpy(conf + CGM_OFF_VOLTAGE,
           *(char **)((char *)item + JSON_OFF_VALUESTRING));
    free(text);
    json_delete(root);
    return conf;
}

// === save_cgminer_conf_json === 原 FUN_00010210 @ 0x00010210
// 来源: reversed/monitor_funcs/FUN_00010210.c | 分类: monitor_funcs
// 语义: 把 conf_buf 写回 /config/cgminer.conf (JSON): 固定写 3 条矿池
//       (url/user/pass); api-listen/api-network 为 1 写 true、为 0 写 false、
//       其他值不写; bitmain-nobeeper/bitmain-notempoverctrl 仅在为 1 时写 true;
//       api-allow/bitmain-freq/bitmain-voltage 恒写。fopen 失败仅打印 (原样)。
void save_cgminer_conf_json(char *conf, char *conf_path)
{
    FILE *fp;
    void *obj;
    void *pools;
    void *pool_obj;
    void *node;
    char *printed;
    int i;
    int rec_off;
    int flag;

    fp = fopen(conf_path, "w+");
    if (fp == NULL) {
        puts("open error");
    }
    obj = json_create_object();
    pools = json_create_array();
    json_add_item_to_object(obj, "pools", pools);
    i = 0;
    do {
        pool_obj = json_create_object();
        rec_off = i * MON_POOL_REC_SIZE;
        i++;
        json_add_item_to_array(pools, pool_obj);
        node = json_create_string(conf + rec_off + MON_POOL_OFF_URL);
        json_add_item_to_object(pool_obj, "url", node);
        node = json_create_string(conf + rec_off + MON_POOL_OFF_USER);
        json_add_item_to_object(pool_obj, "user", node);
        node = json_create_string(conf + rec_off + MON_POOL_OFF_PASS);
        json_add_item_to_object(pool_obj, "pass", node);
    } while (i != 3);

    flag = *(int *)(conf + CGM_OFF_API_LISTEN);
    if (flag == 1) {
        node = json_create_true();
        json_add_item_to_object(obj, "api-listen", node);
    }
    else if (flag == 0) {
        node = json_create_false();
        json_add_item_to_object(obj, "api-listen", node);
    }
    flag = *(int *)(conf + CGM_OFF_API_NETWORK);
    if (flag == 1) {
        node = json_create_true();
        json_add_item_to_object(obj, "api-network", node);
    }
    else if (flag == 0) {
        node = json_create_false();
        json_add_item_to_object(obj, "api-network", node);
    }
    flag = *(int *)(conf + CGM_OFF_NOBEEPER);
    if (flag == 1) {
        node = json_create_true();
        json_add_item_to_object(obj, "bitmain-nobeeper", node);
    }
    flag = *(int *)(conf + CGM_OFF_NOTEMPOVERCTRL);
    if (flag == 1) {
        node = json_create_true();
        json_add_item_to_object(obj, "bitmain-notempoverctrl", node);
    }
    node = json_create_string(conf + CGM_OFF_API_ALLOW);
    json_add_item_to_object(obj, "api-allow", node);
    node = json_create_string(conf + CGM_OFF_FREQ);
    json_add_item_to_object(obj, "bitmain-freq", node);
    node = json_create_string(conf + CGM_OFF_VOLTAGE);
    json_add_item_to_object(obj, "bitmain-voltage", node);
    printed = json_print(obj);
    fputs(printed, fp);
    fclose(fp);
    json_delete(obj);
    free(printed);
    return;
}

// === apply_pool_changes === 原 FUN_00010474 @ 0x00010474
// 来源: reversed/monitor_funcs/FUN_00010474.c | 分类: monitor_funcs
// 语义: 把 srv_pools (parse_pools_string 输出, 记录步长 0xa4: 序号@+0,
//       url@+4, user@+0x54, pass@+0x7c, 计数@+0x1ec) 按 pools_count 应用到
//       json_conf (3 条矿池布局): count=1 按 srv_pools[0] 的序号写对应槽;
//       count=2 依次处理前两条; count=3 原样拷贝三条的 url/user/pass。
//       返回 json_conf。
char *apply_pool_changes(char *new_pools, char *conf)
{
    long pool_no;
    long pool_no2;
    int count;

    printf("change->pools_count = %d\n", *(int *)(new_pools + MON_POOL_OFF_COUNT));
    count = *(int *)(new_pools + MON_POOL_OFF_COUNT);
    if (count == 1) {
        pool_no = strtol(new_pools, NULL, 10);
        if (pool_no == 1) {
            strcpy(conf + 0xa8, new_pools + MON_POOL_OFF_URL);
            strcpy(conf + 0xf8, new_pools + MON_POOL_OFF_USER);
            strcpy(conf + 0x120, new_pools + MON_POOL_OFF_PASS);
        }
        else if (pool_no == 2) {
            strcpy(conf + 0x14c, new_pools + MON_POOL_OFF_URL);
            strcpy(conf + 0x19c, new_pools + MON_POOL_OFF_USER);
            strcpy(conf + 0x1c4, new_pools + MON_POOL_OFF_PASS);
        }
        else if (pool_no == 0) {
            strcpy(conf + 0x4, new_pools + MON_POOL_OFF_URL);
            strcpy(conf + 0x54, new_pools + MON_POOL_OFF_USER);
            strcpy(conf + 0x7c, new_pools + MON_POOL_OFF_PASS);
        }
    }
    else if (count == 2) {
        pool_no = strtol(new_pools, NULL, 10);
        pool_no2 = strtol(new_pools + MON_POOL_REC_SIZE, NULL, 10);
        if (pool_no == 1) {
            strcpy(conf + 0xa8, new_pools + MON_POOL_OFF_URL);
            strcpy(conf + 0xf8, new_pools + MON_POOL_OFF_USER);
            strcpy(conf + 0x120, new_pools + MON_POOL_OFF_PASS);
        }
        else if (pool_no == 2) {
            strcpy(conf + 0x14c, new_pools + MON_POOL_OFF_URL);
            strcpy(conf + 0x19c, new_pools + MON_POOL_OFF_USER);
            strcpy(conf + 0x1c4, new_pools + MON_POOL_OFF_PASS);
        }
        else if (pool_no == 0) {
            strcpy(conf + 0x4, new_pools + MON_POOL_OFF_URL);
            strcpy(conf + 0x54, new_pools + MON_POOL_OFF_USER);
            strcpy(conf + 0x7c, new_pools + MON_POOL_OFF_PASS);
        }
        if (pool_no2 == 1) {
            strcpy(conf + 0xa8, new_pools + 0xa8);
            strcpy(conf + 0xf8, new_pools + 0xf8);
            strcpy(conf + 0x120, new_pools + 0x120);
        }
        else if (pool_no2 == 2) {
            strcpy(conf + 0x14c, new_pools + 0xa8);
            strcpy(conf + 0x19c, new_pools + 0xf8);
            strcpy(conf + 0x1c4, new_pools + 0x120);
        }
        else if (pool_no2 == 0) {
            strcpy(conf + 0x4, new_pools + 0xa8);
            strcpy(conf + 0x54, new_pools + 0xf8);
            strcpy(conf + 0x7c, new_pools + 0x120);
        }
    }
    else if (count == 3) {
        strcpy(conf + 0x4, new_pools + MON_POOL_OFF_URL);
        strcpy(conf + 0x54, new_pools + MON_POOL_OFF_USER);
        strcpy(conf + 0x7c, new_pools + MON_POOL_OFF_PASS);
        strcpy(conf + 0xa8, new_pools + 0xa8);
        strcpy(conf + 0xf8, new_pools + 0xf8);
        strcpy(conf + 0x120, new_pools + 0x120);
        strcpy(conf + 0x14c, new_pools + 0x14c);
        strcpy(conf + 0x19c, new_pools + 0x19c);
        strcpy(conf + 0x1c4, new_pools + 0x1c4);
        return conf;
    }
    return conf;
}

/* ------------------------------------------------------------------ */
/* 覆盖率表 (批次 mon_core_b1: 21/21)                                   */
/* ------------------------------------------------------------------ */
/*
 * 原 FUN_00009fa0  → main                    (0x00009fa0)
 * 原 FUN_0000ad18  → tcp_state               (0x0000ad18)
 * 原 FUN_0000b088  → create_pid_file         (0x0000b088)
 * 原 FUN_0000b340  → parse_option_line       (0x0000b340)
 * 原 FUN_0000b46c  → parse_conf_file         (0x0000b46c)
 * 原 FUN_0000b6d4  → parse_conf_file_kv      (0x0000b6d4)
 * 原 FUN_0000ba9c  → load_json_pairs         (0x0000ba9c)
 * 原 FUN_0000bc44  → format_api_result       (0x0000bc44)
 * 原 FUN_0000bea4  → callapi                 (0x0000bea4)
 * 原 FUN_0000c130  → parse_hex4              (0x0000c130)
 * 原 FUN_0000e8dc  → recv_server_pkg         (0x0000e8dc)
 * 原 FUN_0000edfc  → send_login_pkg          (0x0000edfc)
 * 原 FUN_0000f108  → send_getstatus_res      (0x0000f108)
 * 原 FUN_0000f4c0  → restart_cgminer         (0x0000f4c0)
 * 原 FUN_0000f990  → monitor_conf_get_value  (0x0000f990)
 * 原 FUN_0000fa54  → conf_update_api_switch  (0x0000fa54)
 * 原 FUN_0000fc84  → conf_reset_api_switch   (0x0000fc84)
 * 原 FUN_0000feb0  → parse_pools_string      (0x0000feb0)
 * 原 FUN_00010004  → load_cgminer_conf_json  (0x00010004)
 * 原 FUN_00010210  → save_cgminer_conf_json  (0x00010210)
 * 原 FUN_00010474  → apply_pool_changes      (0x00010474)
 * 附加 (无 FUN_ 导出, main/tcp_state 以函数指针引用的匿名包装):
 *   0x00010c84 → mon_ssl_recv, 0x00010cac → mon_ssl_send,
 *   0x00010cd4 → mon_tcp_recv, 0x00010d04 → mon_tcp_send
 */
