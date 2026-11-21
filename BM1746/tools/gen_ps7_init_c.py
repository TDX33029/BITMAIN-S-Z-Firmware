#!/usr/bin/env python3
"""gen_ps7_init_c.py — 从 analysis/out/ps7_final.json 生成 rewritten/fsbl/ps7_init.c
机器可读源: [op, addr, value, mask, pc_offset] (op: W=写, MW=掩码写)
生成确定性 C 表, 由 FSBL 转写代理引用; 不做任何语义推断。
"""
import json, os, sys

ROOT = r"D:\Document\tempPrj\BITMAIN-S-Z-Firmware\BM1746"
SRC = os.path.join(ROOT, "analysis", "out", "ps7_final.json")
DST = os.path.join(ROOT, "rewritten", "fsbl", "ps7_init.c")

def main():
    ops = json.load(open(SRC, encoding="utf-8"))
    lines = []
    lines.append("""/**
 * ps7_init.c — Zynq PS 初始化寄存器序列 (540 条, 机器生成)
 *
 * 数据源: analysis/out/ps7_final.json  <- reversed/fsbl_funcs 内嵌 ps7_init 序列
 * (每条目 [op, addr, value, mask, pc_offset]; pc 为 FSBL ELF 内代码地址)
 * op: PS7_WRITE = 直接写 value; PS7_MASK_WRITE = (r & ~mask) | value
 *
 * 对照文本: analysis/out/ps7_init_full.txt (符号化注释版)
 * 关键锚点: SLCR_UNLOCK 0xF8000008<=0xDF0D / SLCR_LOCK 0xF8000004<=0x767B 成对出现
 *          DDRC 初始化 243 条; DDRIOB 0xF8000B70..; JTAG DAP 解锁魔数 0xC5ACCE55
 */
#include "../include/common_types.h"

typedef enum {
    PS7_WRITE = 0,
    PS7_MASK_WRITE = 1,
} ps7_op_t;

typedef struct {
    ps7_op_t  op;
    uint32_t  addr;
    uint32_t  value;
    uint32_t  mask;   /* 仅 PS7_MASK_WRITE 使用 */
    uint32_t  pc;     /* FSBL ELF 内源地址, 追溯用 */
} ps7_op_entry_t;

#define PS7_INIT_ENTRY_COUNT  %d

static const ps7_op_entry_t g_ps7_init_table[PS7_INIT_ENTRY_COUNT] = {
""" % len(ops))
    for op, addr, val, mask, pc in ops:
        if op == "W":
            lines.append(f"    {{ PS7_WRITE,      0x{addr:08X}, 0x{val:08X}, 0, 0x{pc:X} }},")
        elif op == "MW":
            lines.append(f"    {{ PS7_MASK_WRITE, 0x{addr:08X}, 0x{val:08X}, 0x{mask:08X}, 0x{pc:X} }},")
        else:
            sys.exit(f"unknown op: {op}")
    lines.append("""};

/**
 * ps7_apply_init — 按序执行全部 PS 初始化寄存器写 (等价原 ps7_init 调用序列)
 * 原始执行函数见 fsbl_main.c (由命名映射确认后引用)
 */
void ps7_apply_init(void)
{
    for (uint32_t i = 0; i < PS7_INIT_ENTRY_COUNT; i++) {
        const ps7_op_entry_t *e = &g_ps7_init_table[i];
        volatile uint32_t *reg = (volatile uint32_t *)(uintptr_t)e->addr;
        if (e->op == PS7_WRITE) {
            *reg = e->value;
        } else {
            *reg = (*reg & ~e->mask) | (e->value & e->mask);
        }
    }
}

/* 覆盖率: 本文件不含函数横幅 (数据表), 由 fsbl 转写批次在其模块表内登记 ps7_init 相关 FUN */""")
    os.makedirs(os.path.dirname(DST), exist_ok=True)
    with open(DST, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(lines) + "\n")
    print(f"wrote {DST}: {len(ops)} entries")

if __name__ == "__main__":
    main()
