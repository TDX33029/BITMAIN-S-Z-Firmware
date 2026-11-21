/* MinGW 语法冒烟专用桩: mmap 常量与原型 */
#ifndef STUB_SYS_MMAN_H
#define STUB_SYS_MMAN_H
#include <stddef.h>
#ifndef PROT_READ
#define PROT_READ  1
#endif
#ifndef PROT_WRITE
#define PROT_WRITE 2
#endif
#ifndef PROT_EXEC
#define PROT_EXEC  4
#endif
#ifndef MAP_SHARED
#define MAP_SHARED 1
#endif
#ifndef MAP_PRIVATE
#define MAP_PRIVATE 2
#endif
#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS 0x20
#endif
#ifndef MAP_FAILED
#define MAP_FAILED ((void *)-1)
#endif
extern void *mmap(void *, size_t, int, int, int, long);
extern int   munmap(void *, size_t);
#endif
