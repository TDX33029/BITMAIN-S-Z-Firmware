#!/usr/bin/env python3
"""coverage.py — 转写覆盖率与质量校验

用法:
  python -I tools/coverage.py names          # 校验 name_map/*.tsv 全覆盖
  python -I tools/coverage.py transcribed    # 校验 rewritten/ 内函数横幅 vs 索引全集
  python -I tools/coverage.py remnants       # 扫描 rewritten/ 中 Ghidra 残留 (uVar/local_/LAB_...)
  python -I tools/coverage.py all            # 以上全部

判定:
  - 横幅格式: 行内含 "=== <名> === 原 FUN_<8位地址>" (CONVENTIONS.md §1)
  - 每个 _func_index.txt 条目按 (addr,name) 对覆盖: 横幅命中 addr 或 name 之一即可
    (处理命名特例 _init/entry 等 与 ko 两模块地址重叠)
  - uboot 5 个仅索引无源条目豁免 (UBOOT_NO_SOURCE)
"""
import os, re, sys, glob
from collections import Counter, defaultdict

ROOT = r"D:\Document\tempPrj\BITMAIN-S-Z-Firmware\BM1746"
REV = os.path.join(ROOT, "reversed")
RWR = os.path.join(ROOT, "rewritten")
NM = os.path.join(RWR, "name_map")

BIN_MAP = {
    "cgminer": "cgminer",
    "fsbl": "fsbl_funcs",
    "uboot": "uboot_funcs",
    "monitor": "monitor_funcs",
    "fileparser": "fileparser_funcs",
    "ko": None,
}
KO_DIRS = ["ko_bitmain_axi", "ko_fpga_mem"]
UBOOT_NO_SOURCE = {"04004930", "04004bd4", "040069f0", "04009a20", "04009c68"}

BANNER_RE = re.compile(
    r"===\s*([A-Za-z_][A-Za-z0-9_]*)(?:\s*\([^)]*\))?\s*===\s*原\s*FUN_([0-9a-fA-F]{8})")
REMNANT_RE = re.compile(
    r"\b(uVar\d+|iVar\d+|sVar\d+|lVar\d+|pVar\d+|pcVar\d+|local_[0-9a-fA-F_]+|"
    r"acStack_[0-9a-f_]+|auStack_[0-9a-f_]+|puStack_[0-9a-f_]+|uStack_[0-9a-f_]+|"
    r"extraout_\w+|undefined\d|uRam[0-9a-fA-F]+(?![a-zA-Z0-9_]))")
# LAB_xxx 不列入: CONVENTIONS §5 允许将复杂 goto 保留为 LAB_ 命名标签 (只能作 label/goto 出现)
LABEL_DEF_RE = re.compile(r"^\s*([A-Za-z_][A-Za-z0-9_]*)\s*:")
GOTO_RE = re.compile(r"\bgoto\s+[A-Za-z_][A-Za-z0-9_]*")

def strip_c_comments(lines):
    """C 注释剥离 (跨行块注释状态机), 返回与输入等长的纯代码行列表"""
    out, in_block = [], False
    for line in lines:
        res, i, n = [], 0, len(line)
        while i < n:
            if in_block:
                j = line.find("*/", i)
                if j == -1:
                    i = n
                else:
                    in_block = False
                    i = j + 2
            else:
                j, k = line.find("//", i), line.find("/*", i)
                if j != -1 and (k == -1 or j < k):
                    break
                if k != -1:
                    res.append(line[i:k])
                    in_block = True
                    i = k + 2
                else:
                    res.append(line[i:])
                    i = n
        out.append("".join(res))
    return out

def load_pairs(path):
    """_func_index.txt -> {(addr8, name)} 集合 + {addr: name}"""
    pairs, amap = set(), {}
    with open(path, encoding="utf-8", errors="replace") as f:
        for line in f:
            parts = line.split()
            if len(parts) >= 2 and re.match(r"^[0-9a-fA-F]{8}$", parts[0]):
                pairs.add((parts[0].lower(), parts[1]))
                amap[parts[0].lower()] = parts[1]
    return pairs, amap

def universe(sub):
    if sub == "ko":
        pairs, amap = set(), {}
        for d in KO_DIRS:
            p, m = load_pairs(os.path.join(REV, d, "_func_index.txt"))
            pairs |= p; amap.update(m)
        return pairs, amap, set()
    pairs, amap = load_pairs(os.path.join(REV, BIN_MAP[sub], "_func_index.txt"))
    nosrc = UBOOT_NO_SOURCE if sub == "uboot" else set()
    return pairs, amap, nosrc

def load_name_map(path):
    out = {}
    with open(path, encoding="utf-8", errors="replace") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            parts = line.split("\t")
            if len(parts) >= 2:
                addr = parts[0].replace("FUN_", "").lower()
                out[addr] = (parts[1], parts[2] if len(parts) > 2 else "")
    return out

def check_names():
    ok = True
    for sub in BIN_MAP:
        pairs, amap, nosrc = universe(sub)
        need = len(pairs)
        nmpath = os.path.join(NM, f"{sub}.tsv")
        if not os.path.exists(nmpath):
            print(f"[names] MISSING {nmpath} (need {need})")
            ok = False
            continue
        nm = load_name_map(nmpath)
        missing = sorted(set(amap) - set(nm) - nosrc)
        extra = sorted(set(nm) - set(amap))
        names = [v[0] for v in nm.values()]
        dups = [n for n, c in Counter(names).items() if c > 1 and sub != "ko"]
        bad = [n for n in names if not re.match(r"^[A-Za-z_][A-Za-z0-9_]*$", n)]
        # 与索引名冲突检查: 语义名不应与另一条目的 ghidra 名混淆 (仅提示)
        status = "OK" if not (missing or extra or dups or bad) else "FAIL"
        if status == "FAIL":
            ok = False
        print(f"[names] {sub}: need={need} map={len(nm)} missing={len(missing)} "
              f"extra={len(extra)} dup_names={len(dups)} bad_id={len(bad)} -> {status}")
        for a in missing[:10]:
            print(f"        missing FUN_{a} ({amap[a]})")
        for n in dups[:10]:
            print(f"        dup: {n}")
        for n in bad[:10]:
            print(f"        bad id: {n}")
    return ok

def iter_target_files(sub):
    base = os.path.join(RWR, sub)
    for p in sorted(glob.glob(os.path.join(base, "**", "*.c"), recursive=True) +
                    glob.glob(os.path.join(base, "**", "*.h"), recursive=True)):
        yield p

def check_transcribed():
    ok = True
    for sub in BIN_MAP:
        pairs, amap, nosrc = universe(sub)
        found_pair = []   # (name, addr, loc) 每条横幅
        n_files = 0
        for p in iter_target_files(sub):
            n_files += 1
            with open(p, encoding="utf-8", errors="replace") as f:
                for ln, line in enumerate(f, 1):
                    m = BANNER_RE.search(line)
                    if m:
                        found_pair.append((m.group(1), m.group(2).lower(),
                                           f"{os.path.relpath(p, RWR)}:{ln}"))
        missing, multi, warn = [], [], []
        for addr, name in sorted(pairs):
            if addr in nosrc:
                continue
            exact = [loc for (n, a, loc) in found_pair if a == addr and n == name]
            by_addr = [loc for (n, a, loc) in found_pair if a == addr]
            by_name = [loc for (n, a, loc) in found_pair if n == name]
            hits = exact or by_addr or by_name
            if not hits:
                missing.append(f"FUN_{addr}({name})")
            elif len(exact) > 1:
                multi.append(f"FUN_{addr}({name}): {exact}")
            elif len(hits) > 1:
                warn.append(f"FUN_{addr}({name}): {hits}")
        exempt = sorted(nosrc)
        status = "OK" if not missing and not multi else "FAIL"
        if status == "FAIL":
            ok = False
        print(f"[trans] {sub}: need={len(pairs)-len(nosrc)} banners={len(found_pair)} "
              f"files={n_files} missing={len(missing)} dup={len(multi)} warn={len(warn)} "
              f"exempt_nosrc={len(exempt)} -> {status}")
        for m_ in missing[:10]:
            print(f"        missing {m_}")
        for m_ in multi[:5]:
            print(f"        dup {m_}")
        for w in warn[:5]:
            print(f"        warn {w}")
    return ok

def check_remnants():
    ok = True
    total_hits = 0
    for sub in BIN_MAP:
        hits = []
        for p in iter_target_files(sub):
            with open(p, encoding="utf-8", errors="replace") as f:
                code_lines = strip_c_comments(f.readlines())
            for ln, line in enumerate(code_lines, 1):
                if not line.strip():
                    continue
                # 标签定义可连续出现 (a: b: stmt;), 剥离后再扫; goto 目标必为标签, 一并剥离
                while True:
                    m = LABEL_DEF_RE.match(line)
                    if not m:
                        break
                    line = line[m.end():]
                line = GOTO_RE.sub("goto", line)
                m = REMNANT_RE.search(line)
                if m:
                    hits.append(f"{os.path.relpath(p, RWR)}:{ln}: {m.group(0)}: {line.strip()[:90]}")
        total_hits += len(hits)
        if hits:
            ok = False
        print(f"[remn] {sub}: {len(hits)}")
        for h in hits[:8]:
            print(f"        {h}")
    print(f"[remn] total={total_hits} -> {'OK' if ok else 'FAIL'}")
    return ok

if __name__ == "__main__":
    cmd = sys.argv[1] if len(sys.argv) > 1 else "all"
    rc = 0
    if cmd in ("names", "all"):
        rc |= 0 if check_names() else 1
    if cmd in ("transcribed", "all"):
        rc |= 0 if check_transcribed() else 1
    if cmd in ("remnants", "all"):
        rc |= 0 if check_remnants() else 1
    sys.exit(rc)
