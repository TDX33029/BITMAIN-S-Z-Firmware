#!/usr/bin/env python3
"""merge_name_maps.py — 合并 cgminer_A/B/C/D.tsv → cgminer.tsv 并校验

用法: python -I tools/merge_name_maps.py
规则:
  - 各分片按 function_map.txt 分类互斥, 地址不重叠; 若重叠以 __func__/anchor basis 优先
  - 校验: 全覆盖 644 / 无重名 / id 合法 / 与 _func_index 对齐
"""
import os, re, sys, glob
from collections import Counter

ROOT = r"D:\Document\tempPrj\BITMAIN-S-Z-Firmware\BM1746"
NM = os.path.join(ROOT, "rewritten", "name_map")
IDX = os.path.join(ROOT, "reversed", "cgminer", "_func_index.txt")
OUT = os.path.join(NM, "cgminer.tsv")
SHARDS = ["cgminer_A.tsv", "cgminer_B1.tsv", "cgminer_B2.tsv", "cgminer_B3.tsv", "cgminer_B4.tsv",
          "cgminer_C.tsv", "cgminer_C2.tsv", "cgminer_C3.tsv", "cgminer_D.tsv"]
PRIO = {"__func__": 5, "anchor": 4, "logstring": 3, "body": 2, "callgraph": 1, "fallback": 0, "passthrough": 6}

def load_func_index():
    out = {}
    for line in open(IDX, encoding="utf-8", errors="replace"):
        p = line.split()
        if len(p) >= 2 and re.match(r"^[0-9a-fA-F]{8}$", p[0]):
            out[p[0].lower()] = p[1]
    return out

def load_shard(path):
    rows = []
    for line in open(path, encoding="utf-8", errors="replace"):
        line = line.rstrip("\n")
        if not line.strip() or line.startswith("#"):
            continue
        parts = line.split("\t")
        if len(parts) >= 2:
            addr = parts[0].replace("FUN_", "").lower()
            name = parts[1].strip()
            basis = parts[2].strip() if len(parts) > 2 else "fallback"
            rows.append((addr, name, basis))
    return rows

def main():
    idx = load_func_index()
    merged = {}   # addr -> (name, basis, shard)
    conflicts = []
    for shard in SHARDS:
        path = os.path.join(NM, shard)
        if not os.path.exists(path):
            print(f"[merge] MISSING shard {shard}")
            continue
        for addr, name, basis in load_shard(path):
            if addr in merged:
                old_name, old_basis, old_shard = merged[addr]
                if name != old_name:
                    if PRIO.get(basis, 0) > PRIO.get(old_basis, 0):
                        merged[addr] = (name, basis, shard)
                        conflicts.append(f"addr {addr}: {old_shard}:{old_name}({old_basis}) -> {shard}:{name}({basis})")
                    else:
                        conflicts.append(f"addr {addr}: keep {old_shard}:{old_name}({old_basis}), ignore {shard}:{name}({basis})")
            else:
                merged[addr] = (name, basis, shard)
    missing = sorted(set(idx) - set(merged))
    extra = sorted(set(merged) - set(idx))
    names = [v[0] for v in merged.values()]
    dups = [n for n, c in Counter(names).items() if c > 1]
    bad = [n for n in names if not re.match(r"^[A-Za-z_][A-Za-z0-9_]*$", n)]
    print(f"[merge] index={len(idx)} merged={len(merged)} missing={len(missing)} extra={len(extra)} "
          f"dup={len(dups)} bad={len(bad)} conflicts={len(conflicts)}")
    for m in missing[:15]:
        print(f"        missing FUN_{m} ({idx[m]})")
    for d in dups[:15]:
        print(f"        dup name: {d}")
    for b in bad[:15]:
        print(f"        bad id: {b}")
    for c in conflicts[:15]:
        print(f"        conflict: {c}")
    if missing or extra or dups or bad:
        sys.exit(1)
    with open(OUT, "w", encoding="utf-8", newline="\n") as f:
        f.write(f"# cgminer name_map: addr<TAB>name<TAB>basis  (merged from {', '.join(SHARDS)})\n")
        for addr in sorted(merged):
            name, basis, shard = merged[addr]
            f.write(f"FUN_{addr}\t{name}\t{basis}\n")
    print(f"[merge] wrote {OUT}: {len(merged)} rows")

if __name__ == "__main__":
    main()
