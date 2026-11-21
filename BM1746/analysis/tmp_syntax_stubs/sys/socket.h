/* MinGW 语法冒烟专用桩: 仅满足 -fsyntax-only, 数值为 ARM Linux 原值 (文件的本地定义优先) */
#ifndef STUB_SYS_SOCKET_H
#define STUB_SYS_SOCKET_H
#include <stdint.h>
#include <stddef.h>
typedef uint32_t socklen_t;
struct sockaddr { uint16_t sa_family; char sa_data[14]; };
struct in_addr  { uint32_t s_addr; };
struct sockaddr_in { uint16_t sin_family; uint16_t sin_port; struct in_addr sin_addr; uint8_t sin_zero[8]; };
struct linger { int l_onoff; int l_linger; };
#ifndef AF_UNIX
#define AF_UNIX 1
#endif
#ifndef AF_UNSPEC
#define AF_UNSPEC 0
#endif
#ifndef AF_INET
#define AF_INET 2
#endif
#ifndef AF_INET6
#define AF_INET6 10
#endif
#ifndef SOCK_STREAM
#define SOCK_STREAM 1
#endif
#ifndef SOCK_DGRAM
#define SOCK_DGRAM 2
#endif
#ifndef SOCK_RAW
#define SOCK_RAW 3
#endif
#ifndef SOL_SOCKET
#define SOL_SOCKET 1
#endif
#ifndef SO_REUSEADDR
#define SO_REUSEADDR 2
#endif
#ifndef SO_KEEPALIVE
#define SO_KEEPALIVE 9
#endif
#ifndef SO_ERROR
#define SO_ERROR 4
#endif
#ifndef SO_BROADCAST
#define SO_BROADCAST 6
#endif
#ifndef SO_SNDBUF
#define SO_SNDBUF 7
#endif
#ifndef SO_RCVBUF
#define SO_RCVBUF 8
#endif
#ifndef SO_LINGER
#define SO_LINGER 13
#endif
#ifndef SO_RCVTIMEO
#define SO_RCVTIMEO 20
#endif
#ifndef SO_SNDTIMEO
#define SO_SNDTIMEO 21
#endif
#ifndef MSG_PEEK
#define MSG_PEEK 2
#endif
#ifndef MSG_DONTWAIT
#define MSG_DONTWAIT 0x80
#endif
#ifndef MSG_WAITALL
#define MSG_WAITALL 0x100
#endif
#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0x400
#endif
#ifndef SHUT_RD
#define SHUT_RD 0
#endif
#ifndef SHUT_WR
#define SHUT_WR 1
#endif
#ifndef SHUT_RDWR
#define SHUT_RDWR 2
#endif
#ifndef IPPROTO_TCP
#define IPPROTO_TCP 6
#endif
#ifndef TCP_NODELAY
#define TCP_NODELAY 1
#endif
#ifndef TCP_KEEPIDLE
#define TCP_KEEPIDLE 4
#endif
#ifndef TCP_KEEPINTVL
#define TCP_KEEPINTVL 5
#endif
#ifndef TCP_KEEPCNT
#define TCP_KEEPCNT 6
#endif
extern int  socket(int, int, int);
extern int  connect(int, const struct sockaddr *, socklen_t);
extern int  bind(int, const struct sockaddr *, socklen_t);
extern int  listen(int, int);
extern int  accept(int, struct sockaddr *, socklen_t *);
extern int  send(int, const void *, size_t, int);
extern int  recv(int, void *, size_t, int);
extern int  setsockopt(int, int, int, const void *, socklen_t);
extern int  getsockopt(int, int, int, void *, socklen_t *);
extern int  shutdown(int, int);
#endif
/* ---- 追加: 数据报收发 (uart.c 监听口) ---- */
extern int recvfrom(int, void *, size_t, int, struct sockaddr *, socklen_t *);
extern int sendto(int, const void *, size_t, int, const struct sockaddr *, socklen_t);
