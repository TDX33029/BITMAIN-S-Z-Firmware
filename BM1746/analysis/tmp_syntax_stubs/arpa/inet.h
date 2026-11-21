/* MinGW 语法冒烟专用桩: 字节序与地址转换 (ARM Linux) */
#ifndef STUB_ARPA_INET_H
#define STUB_ARPA_INET_H
#include <stdint.h>
#include "sys/socket.h"
extern uint32_t htonl(uint32_t);
extern uint16_t htons(uint16_t);
extern uint32_t ntohl(uint32_t);
extern uint16_t ntohs(uint16_t);
extern uint32_t inet_addr(const char *);
extern char *inet_ntoa(struct in_addr);
extern int inet_aton(const char *, struct in_addr *);
extern const char *inet_ntop(int, const void *, char *, unsigned int);
extern int inet_pton(int, const char *, void *);
#endif
