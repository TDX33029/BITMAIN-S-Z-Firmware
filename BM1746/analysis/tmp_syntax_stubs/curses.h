/* MinGW 语法冒烟专用桩: WINDOW 类型 + 本固件所用 curses 函数集 */
#ifndef STUB_CURSES_H
#define STUB_CURSES_H
#include <stdbool.h>
typedef struct { int _stub; } WINDOW;
typedef unsigned int chtype;
extern WINDOW *stdscr;
extern int LINES;
extern int COLS;
extern int cbreak(void);
extern int noecho(void);
extern int echo(void);
extern int halfdelay(int);
extern int leaveok(WINDOW *, bool);
extern int mvwin(WINDOW *, int, int);
extern int wresize(WINDOW *, int, int);
extern int wattr_on(WINDOW *, chtype, void *);
extern int wattr_off(WINDOW *, chtype, void *);
extern int werase(WINDOW *);
extern int wprintw(WINDOW *, const char *, ...);
extern int mvwprintw(WINDOW *, int, int, const char *, ...);
extern int wrefresh(WINDOW *);
extern int wtouchln(WINDOW *, int, int, int);
extern int wgetch(WINDOW *);
extern int wgetnstr(WINDOW *, char *, int);
#endif
