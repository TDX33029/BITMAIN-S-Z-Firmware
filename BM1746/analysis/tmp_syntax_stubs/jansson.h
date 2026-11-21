/* MinGW 语法冒烟专用桩: jansson 类型 (函数由转写文件自带 extern 声明) */
#ifndef STUB_JANSSON_H
#define STUB_JANSSON_H
#include <stdint.h>
#include <stddef.h>
typedef enum {
    JSON_OBJECT = 0,
    JSON_ARRAY = 1,
    JSON_STRING = 2,
    JSON_INTEGER = 3,
    JSON_REAL = 4,
    JSON_TRUE = 5,
    JSON_FALSE = 6,
    JSON_NULL = 7
} json_type;
typedef struct json_t { json_type type; uint32_t refcount; } json_t;
typedef struct { char text[256]; int line; int column; int position; char source[80]; } json_error_t;
#endif
