/* MinGW 语法冒烟专用 prelude (-include 强制注入):
 * 目标平台 (ARM Linux / 内核) 真实存在的符号, 仅冒烟环境缺失 */
#ifndef STUB_PRELUDE_H
#define STUB_PRELUDE_H
#include <stdint.h>
typedef struct { uint32_t __bits[32]; } __stub_sigset_t;   /* glibc sigset_t = 128B */
typedef __stub_sigset_t sigset_t;       /* monitor 显式用 sigset_t 类型名 */
struct sigaction {
    union { void (*sa_handler)(int); void (*sa_sigaction)(int, void *, void *); };  /* glibc 匿名联合 */
    __stub_sigset_t sa_mask;
    int sa_flags;
    void (*sa_restorer)(void);
};
typedef uint32_t dev_t;                 /* 内核 dev_t (32 位目标) */
/* -DNO_OLDNAMES 屏蔽了 MinGW 旧名区, pid_t/off_t 由本桩补齐 */
typedef int pid_t;
typedef long off_t;
extern const unsigned short **__ctype_b_loc(void);   /* glibc ctype 表定位 */
extern const int **__ctype_toupper_loc(void);        /* glibc toupper 表定位 */
extern char *__strdup(const char *);    /* glibc strdup 内部名 (axi.c) */
extern int mkdir(const char *, unsigned int);   /* POSIX 2 参 (冒烟需 -DNOOLDNAMES 屏蔽 MinGW 1 参旧名) */
extern int kill(int, int);              /* glibc signal.h */
extern int sigemptyset(void *);         /* 实为 sigset_t*, 取 void* 规避桩类型名 */
extern int sigfillset(void *);
extern int sigprocmask(int, const void *, void *);
extern long sysconf(int);               /* glibc unistd.h */
extern int *__errno_location(void);     /* glibc errno tloc */
extern void __assert_fail(const char *, const char *, unsigned int, const char *);  /* glibc assert */
/* Linux 内核字符设备最小骨架 (ko 模块 fops 初始化器所需) */
struct module;
struct inode;
struct file;
struct vm_area_struct {
    unsigned long vm_start;
    unsigned long vm_end;
    unsigned long vm_flags;        /* +0x28 */
    unsigned int  vm_page_prot;    /* +0x24 */
};
extern int remap_pfn_range(struct vm_area_struct *, unsigned long, unsigned long,
                           unsigned long, unsigned long);
extern int printk(const char *, ...);
#ifndef VM_IO
#define VM_IO         0x00004000u
#ifndef LOCK_UN
#define LOCK_UN 8
struct stat {                        /* MinGW 无 struct stat; cfg.c 仅整体传址, 无成员访问 */
    uint32_t st_dev; uint32_t st_ino; unsigned int st_mode; unsigned int st_nlink;
    unsigned int st_uid; unsigned int st_gid; uint32_t st_size;
    uint32_t st_atime; uint32_t st_mtime; uint32_t st_ctime;
};
extern int __xstat(int, const char *, struct stat *);   /* glibc stat 包装 (_STAT_VER=3) */
#endif
#ifndef SIGPIPE
#define SIGPIPE 13
#endif
#ifndef SIGALRM
#define SIGALRM 14
#endif
#include <sys/time.h>            /* mingw 自带 struct timeval */
struct itimerval { struct timeval it_interval; struct timeval it_value; };
#ifndef ITIMER_REAL
#define ITIMER_REAL   0
#endif
#ifndef ITIMER_VIRTUAL
#define ITIMER_VIRTUAL 1
#endif
#ifndef ITIMER_PROF
#define ITIMER_PROF   2
#endif
#endif
#ifndef VM_DONTEXPAND
#define VM_DONTEXPAND 0x00040000u
#endif
#ifndef VM_DONTDUMP
#define VM_DONTDUMP   0x04000000u
#endif
struct cdev {
    struct module *owner;
    const struct file_operations *ops;
    dev_t dev;
    unsigned int count;
};
struct file_operations {
    struct module *owner;
    int (*mmap)(struct file *, struct vm_area_struct *);
    int (*open)(struct inode *, struct file *);
    int (*release)(struct inode *, struct file *);
};
#define THIS_MODULE ((struct module *)0)
#endif
