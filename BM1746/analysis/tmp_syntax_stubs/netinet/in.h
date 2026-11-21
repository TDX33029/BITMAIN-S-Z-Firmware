/* MinGW 语法冒烟专用桩: 复用 socket 桩类型, 数值为 ARM Linux 原值 */
#ifndef STUB_NETINET_IN_H
#define STUB_NETINET_IN_H
#include "../sys/socket.h"
#ifndef INADDR_ANY
#define INADDR_ANY 0x00000000u
struct sockaddr_storage { uint16_t ss_family; char __pad[126]; };
struct in6_addr { uint8_t s6_addr[16]; };
struct ip_mreq { struct in_addr imr_multiaddr; struct in_addr imr_interface; };
struct ipv6_mreq { struct in6_addr ipv6mr_multiaddr; unsigned int ipv6mr_interface; };
#endif
#ifndef INADDR_NONE
#define INADDR_NONE 0xFFFFFFFFu
#endif
#ifndef INADDR_LOOPBACK
#define INADDR_LOOPBACK 0x7F000001u
#endif
#ifndef INADDR_BROADCAST
#define INADDR_BROADCAST 0xFFFFFFFFu
#endif
#ifndef IP_TOS
#define IP_TOS 1
#endif
#endif
