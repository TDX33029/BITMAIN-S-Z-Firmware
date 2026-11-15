#!/bin/bash
# Batch import+analyze+export functions for all RE targets.
export JAVA_HOME=$(ls -d "D:/Document/tempPrj/BITMAIN-S19Hyd-151T-Firmware/Firmware/S19/analysis/tools/"jdk-25* | head -1)
GH="D:/Document/tempPrj/BITMAIN-S19Hyd-151T-Firmware/Firmware/S19/analysis/tools/ghidra_12.2_PUBLIC"
BASE="D:/Document/tempPrj/ZEC/_unpack"
SCRIPTS="$BASE/analysis/scripts"
PROJ="$BASE/analysis/ghidra_proj"
run() { # run <projdir> <projname> <importfile|null> <procname|-> <outdir> [extra loader args...]
  local pd="$1" pn="$2" imp="$3" proc="$4" out="$5"; shift 5
  local extra=()
  if [ "$imp" != "-" ]; then
    extra=(-import "$imp" -processor ARM:LE:32:v7 -cspec default -loader BinaryLoader -loader-baseAddr "$6")
  else
    extra=(-process "$proc" -noanalysis)
  fi
  "$GH/support/analyzeHeadless.bat" "$pd" "$pn" "${extra[@]}" \
    -scriptPath "$SCRIPTS" -postScript ExportFuncs.java "$out" 2>&1 | tail -3
}
# 1. cgminer (own project, ELF has proper base 0x8000)
mkdir -p "$BASE/analysis/ghidra_proj_cgminer"
run "$BASE/analysis/ghidra_proj_cgminer" CG -import "$BASE/bm1746/cgminer_z15_bm1746.elf" - "D:/Document/tempPrj/ZEC/_unpack/reversed/cgminer_funcs" 0x00008000
# 2. FileParser (rootfs_1)
run "$PROJ" ZECproj -import "$BASE/rootfs_1/usr/bin/FileParser" - "D:/Document/tempPrj/ZEC/_unpack/reversed/fileparser_funcs" 0x00008000
# 3. miner-monitor (SD rootfs)
run "$PROJ" ZECproj -import "$BASE/sdcard/rootfs_sd/usr/bin/miner-monitor" - "D:/Document/tempPrj/ZEC/_unpack/reversed/monitor_funcs" 0x00008000
# 4. kernel modules already analyzed in ZECproj: re-export per-function
run "$PROJ" ZECproj - bitmain_axi.ko "D:/Document/tempPrj/ZEC/_unpack/reversed/ko_bitmain_axi"
run "$PROJ" ZECproj - ph2_raw.bin "D:/Document/tempPrj/ZEC/_unpack/reversed/uboot_funcs"
# 5. FSBL already analyzed: re-export per-function
run "$PROJ" ZECproj - fsbl_ph1_raw.bin "D:/Document/tempPrj/ZEC/_unpack/reversed/fsbl_funcs"
echo ALL-DONE
