#!/usr/bin/env python3
"""batch_plan.py — 按类别+行数规划 cgminer 转写批次 (≤2500 源行/批)
输出: rewritten/batch_plan.tsv  (batch_id, module_file, category, fun, src, lines)
仅读 reversed/, 不修改。
"""
import os, sys, glob

ROOT = r"D:\Document\tempPrj\BITMAIN-S-Z-Firmware\BM1746"
CG = os.path.join(ROOT, "reversed", "cgminer")
OUT = os.path.join(ROOT, "rewritten", "batch_plan.tsv")

# 模块文件规划: category -> [(module_file, sub_filter or None)]
# sub_filter: None=全部; 函数名集合由 --split 指定文件提供(可选), 这里按行数贪心切分
CATS = [
    "10_uart_protocol", "12_freq_pll", "13_voltage_pic", "14_temp_fan",
    "15_fpga_axi", "16_chain_mgmt", "20_job_mining", "21_pool_net",
    "22_json_rpc", "30_api_status", "32_i2c_sensor", "40_config",
    "41_logging", "50_crypto_hash", "60_util_util", "70_no_strings",
]
CAT_SHORT = {
    "10_uart_protocol": "uart", "12_freq_pll": "freq", "13_voltage_pic": "pic",
    "14_temp_fan": "fan", "15_fpga_axi": "axi", "16_chain_mgmt": "chain",
    "20_job_mining": "job", "21_pool_net": "pool", "22_json_rpc": "json",
    "30_api_status": "api", "32_i2c_sensor": "i2c", "40_config": "cfg",
    "41_logging": "log", "50_crypto_hash": "hash", "60_util_util": "util",
    "70_no_strings": "nostr",
}
BUDGET = 2500  # 每批源行上限

def main():
    rows = []  # (cat, fun, src, lines)
    for cat in CATS:
        for f in sorted(glob.glob(os.path.join(CG, cat, "*.c"))):
            fun = os.path.splitext(os.path.basename(f))[0]
            with open(f, "r", encoding="utf-8", errors="replace") as fh:
                lines = sum(1 for _ in fh)
            rows.append((cat, fun, os.path.relpath(f, ROOT).replace("\\", "/"), lines))
    rows.sort(key=lambda r: (CATS.index(r[0]), -r[3]))

    # 每类贪心装箱: 大函数优先独占或组批
    batches = {}
    from collections import defaultdict
    bycat = defaultdict(list)
    for r in rows:
        bycat[r[0]].append(r)
    for cat, items in bycat.items():
        items = sorted(items, key=lambda r: -r[3])
        b = 0
        cur = 0
        for it in items:
            if cur + it[3] > BUDGET and cur > 0:
                b += 1
                cur = 0
            key = (cat, b)
            batches.setdefault(key, []).append(it)
            cur += it[3]

    # 独立模块文件命名: 类内批次>=2 时加 _N 后缀
    cat_batch_count = defaultdict(int)
    for (cat, b) in batches:
        cat_batch_count[cat] = max(cat_batch_count[cat], b + 1)
    module_of = {}
    for (cat, b) in sorted(batches.keys()):
        if cat_batch_count[cat] == 1:
            module_of[(cat, b)] = f"cgminer/{CAT_SHORT[cat]}.c"
        else:
            module_of[(cat, b)] = f"cgminer/{CAT_SHORT[cat]}_p{b+1}.c"

    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, "w", encoding="utf-8", newline="\n") as out:
        out.write("# batch\tmodule\tcategory\tfun\tsrc\tlines\n")
        for (cat, b) in sorted(batches.keys(), key=lambda k: (CATS.index(k[0]), k[1])):
            batch_id = f"{CAT_SHORT[cat]}_b{b+1}"
            for (c, fun, src, lines) in sorted(batches[(cat, b)], key=lambda r: r[1]):
                out.write(f"{batch_id}\t{module_of[(cat,b)]}\t{cat}\t{fun}\t{src}\t{lines}\n")

    nb = len(batches)
    total = len(rows)
    total_lines = sum(r[3] for r in rows)
    print(f"batches={nb} functions={total} total_lines={total_lines}")
    for (cat, b) in sorted(batches.keys(), key=lambda k: (CATS.index(k[0]), k[1])):
        its = batches[(cat, b)]
        print(f"  {CAT_SHORT[cat]}_b{b+1}: {module_of[(cat,b)]}  n={len(its)} lines={sum(i[3] for i in its)}")

if __name__ == "__main__":
    sys.exit(main())
