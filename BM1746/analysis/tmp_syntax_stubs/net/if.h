/* MinGW 语法冒烟专用桩: ifreq 与网卡 ioctl (ARM Linux 值) */
#ifndef STUB_NET_IF_H
#define STUB_NET_IF_H
#include "sys/socket.h"
struct ifreq {
    char ifr_name[16];
    union {
        struct sockaddr ifru_addr;
        struct sockaddr ifru_hwaddr;
        char ifru_data[24];
    } ifr_ifru;
};
#define ifr_addr   ifr_ifru.ifru_addr
#define ifr_hwaddr ifr_ifru.ifru_hwaddr
#define ifr_data   ifr_ifru.ifru_data
#ifndef SIOCGIFADDR
#define SIOCGIFADDR   0x8915
#endif
#ifndef SIOCGIFHWADDR
#define SIOCGIFHWADDR 0x8927
#endif
#endif
