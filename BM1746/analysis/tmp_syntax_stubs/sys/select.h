/* MinGW 语法冒烟专用桩: fd_set/FD_* 与 select */
#ifndef STUB_SYS_SELECT_H
#define STUB_SYS_SELECT_H
#include <stdint.h>
#include <stddef.h>
#include <sys/time.h>
#ifndef FD_SETSIZE
#define FD_SETSIZE 1024
#endif
typedef struct { uint32_t fds_bits[FD_SETSIZE / 32]; } fd_set;
#define FD_ZERO(p)  do { unsigned __i; for (__i = 0; __i < (FD_SETSIZE / 32); __i++) ((p)->fds_bits)[__i] = 0; } while (0)
#define FD_SET(d,p) (((p)->fds_bits)[(d) / 32] |= 1u << ((d) % 32))
#define FD_CLR(d,p) (((p)->fds_bits)[(d) / 32] &= ~(1u << ((d) % 32)))
#define FD_ISSET(d,p) ((((const uint32_t *)(p)->fds_bits)[(d) / 32] >> ((d) % 32)) & 1u)
extern int select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
#endif
