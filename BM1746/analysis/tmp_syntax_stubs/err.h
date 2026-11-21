/* MinGW 语法冒烟专用桩: glibc err/warn 族 */
#ifndef STUB_ERR_H
#define STUB_ERR_H
extern void err(int, const char *, ...);
extern void errx(int, const char *, ...);
extern void warn(const char *, ...);
extern void warnx(const char *, ...);
#endif
