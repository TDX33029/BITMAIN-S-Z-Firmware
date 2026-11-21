# rewritten/ — BM1746 固件反编译产物语义化 C 转写

本目录是对 `reversed/` 中 7 个 Ghidra 反编译产物的**全量语义化 C 转写**：1,114 个函数全部落盘，
按功能块合并为独立模块文件，寄存器/魔数/协议常量采用已验证命名，中文注释解释语义意图。

**转写状态：100% 完成。** 校验三关全绿：

| 校验 | 结果 |
|---|---|
| 覆盖率（`tools/coverage.py all`） | names 6/6 OK · transcribed 6/6 OK（1114/1114，uboot 5 条无源豁免）· remnants 0 |
| gcc 语法冒烟（53 个翻译单元） | **53/53 通过**（`gcc -fsyntax-only -std=gnu99`，MinGW 桩头见下文） |
| 抽查复核 | 各批次代理机器码级验证（capstone 交叉核实）+ 阶段 3 专项复核 |

## 阅读路径

1. [CONVENTIONS.md](CONVENTIONS.md) — 9 条转写规则（横幅格式、类型映射、常量来源、goto 保留策略）
2. `name_map/*.tsv` — 命名事实源：`FUN_<地址> → 语义名 → 命名依据`，转写前冻结
3. `include/*.h` — 协议/寄存器常量头（数值均出自已验证文档，不臆造）

## 模块地图

| 目录 | 函数数 | 行数 | 内容 |
|---|---|---|---|
| `cgminer/` | 644 | 38,670 | 采矿主程序（bitmain ZCash fork, cgminer 4.9.0 基底） |
| `fsbl/` | 165 | 9,936 | Zynq FSBL（ps7_init 540 条寄存器序列 + FatFs 移植层 + SD/eMMC + RSA/SHA-256） |
| `uboot/` | 140+5 | 6,297 | U-Boot 2016.07 子集（net/nfs/tftp/dhcp/hashtable/regex/ctype） |
| `monitor/` | 113 | 5,008 | 状态监控守护（cJSON + stratum TLS + 网卡 ioctl） |
| `fileparser/` | 42 | 1,329 | farmhash 2014-03-31 na::Hash64（2172 golden 向量验证） |
| `ko/` | 10 | 420 | 内核模块 bitmain_axi.ko + fpga_mem.ko（AXI 窗口/DDR 工作下发） |
| `include/` | — | 241 | common_types.h（拓扑常量）· bm1746_regs.h（UART 帧/ASIC 寄存器/PLL/PIC）· fpga_axi.h（PL 窗口） |

### cgminer/ 模块文件

- **协议层**: `uart.c`(11) BM1746 ASIC 11 字节帧 · `axi.c`(10) PL 窗口 · `i2c.c`(3) 温感
- **硬件控制**: `pic.c`(39) PIC16F1704 电压/升级 · `fan.c`(18) 风扇 · `chain.c`(44) 链管理 · `freq_p1-p4.c`(99) 频率/PLL/扫描
- **挖掘主线**: `job_p1-p6.c`(162) main/workid/nonce/gwork · `pool_p1-p3.c`(61) stratum/网络 · `json.c`(6) jansson RPC
- **支撑**: `api_p1a-p2.c`(46) API 服务 · `cfg.c`(17) ccan/opt 配置 · `log.c`(9) · `util.c`(35) · `hash.c`(12) BLAKE2b/SHA-256/scrypt · `nostr.c`(72) 无字符串叶函数

## FUN_地址 → 语义名 对照

每个函数头部横幅：`// === <语义名> === 原 FUN_xxxxxxxx @ 0x地址`，可双向检索：

```bash
grep -rn "原 FUN_0003b704" rewritten/     # 地址 → 现名
grep -rn "=== init_fpga ===" rewritten/   # 现名 → 地址
grep "FUN_0003b704" rewritten/name_map/cgminer.tsv   # 命名依据
```

已知命名偏差（记录于 name_map 行内备注）：uboot FUN_040051b8 语义复核为 ub_regex_parse；
cgminer 15 处跨分片重名按函数体证据重命名（见 name_map/cgminer.tsv basis 列）。

## 语法冒烟说明（MinGW 环境）

冒烟命令（BM1746 根目录）：

```bash
gcc -fsyntax-only -std=gnu99 -fno-builtin -DNO_OLDNAMES \
  -Wno-implicit-function-declaration -Wno-incompatible-pointer-types -Wno-int-conversion \
  -Wno-builtin-declaration-mismatch \
  -I analysis/tmp_syntax_stubs -I rewritten/include \
  -include analysis/tmp_syntax_stubs/prelude.h <file>
```

- `analysis/tmp_syntax_stubs/`：MinGW 缺失的 POSIX/内核头桩（sys/socket.h、curses.h、openssl/*、
  netdb.h、netinet/in.h、net/if.h、jansson.h、err.h、syslog.h）+ `prelude.h`（glibc
  sigaction/__errno_location/__assert_fail、内核 file_operations/cdev、NO_OLDNAMES 下的
  pid_t/off_t）。桩数值均为 ARM Linux 原值；**仅用于语法检查，不参与语义**。
- `-DNO_OLDNAMES`：屏蔽 MinGW 1 参 mkdir 旧名，使 POSIX 2 参 `mkdir(path, mode)` 调用点成立。
- 目标平台为 ARM Linux（Zynq-7000），转写文件内的系统头引用对目标环境均为真实头。

## 转写原则摘要（详见 CONVENTIONS.md）

- 语义严格等价：Ghidra 伪变量全部消除，类型/常量按用法还原；不可恢复的寄存器实参**不虚构**，
  以注释存证（如 SupervisorCall 的 memcpy 残参）。
- 部分调用点参数缺失（Ghidra 仅显示部分实参）以函数指针部分原型形态保留，注明寄存器未定值。
- 复杂 goto 保留 LAB_ 命名标签（CONVENTIONS §5）。
- 每文件末尾附覆盖率表（全部已转写 FUN 地址清单）。

## 已知边界

- **不保证可链接**：语义重构以可读性+语义等价为目标；纯数据 blob（如 opt_config_table、
  ctype 表）以 extern 引用，未转写数据段内容。
- uboot 5 个仅索引无源条目豁免（`tools/coverage.py` UBOOT_NO_SOURCE）。
- 反编译体本身受损的函数（halt_baddata 占位）按原样转写为不可恢复挂死入口。
