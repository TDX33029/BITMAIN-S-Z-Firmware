/* MinGW 语法冒烟专用桩: getaddrinfo 族, ARM Linux 常量 */
#ifndef STUB_NETDB_H
#define STUB_NETDB_H
#include <stdint.h>
#include <stddef.h>
#include "sys/socket.h"
struct hostent { char *h_name; char **h_aliases; int h_addrtype; int h_length; char **h_addr_list; };
struct addrinfo {
    int ai_flags; int ai_family; int ai_socktype; int ai_protocol;
    socklen_t ai_addrlen; struct sockaddr *ai_addr; char *ai_canonname;
    struct addrinfo *ai_next;
};
#ifndef AI_PASSIVE
#define AI_PASSIVE 0x0001
#endif
#ifndef AI_CANONNAME
#define AI_CANONNAME 0x0002
#endif
#ifndef AI_NUMERICHOST
#define AI_NUMERICHOST 0x0004
#endif
#ifndef AI_V4MAPPED
#define AI_V4MAPPED 0x0008
#endif
#ifndef AI_ADDRCONFIG
#define AI_ADDRCONFIG 0x0020
#endif
#ifndef AI_NUMERICSERV
#define AI_NUMERICSERV 0x0400
#endif
#ifndef NI_NUMERICHOST
#define NI_NUMERICHOST 1
#endif
#ifndef NI_NUMERICSERV
#define NI_NUMERICSERV 2
#endif
extern int getaddrinfo(const char *, const char *, const struct addrinfo *, struct addrinfo **);
extern void freeaddrinfo(struct addrinfo *);
extern struct hostent *gethostbyname(const char *);
extern int getnameinfo(const struct sockaddr *, socklen_t, char *, socklen_t, char *, socklen_t, int);
extern const char *gai_strerror(int);
#endif
