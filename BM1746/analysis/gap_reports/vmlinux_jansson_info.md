# vmlinux.bin 与 libjansson 补充信息

日期：2026-11-20  
目的：补齐此前“未反编译/未细查”资产的可识别元信息，降低后续定位成本；不做完整反编译。

## 1. vmlinux.bin

| 项目 | 值 |
|---|---|
| 路径 | `BM1746/analysis/vmlinux.bin` |
| 大小 | 11,750,816 B（11.75 MB） |
| md5 | `6c3ef57db0267f77e45ed2316c6893b9` |
| 同源文件 | `BM1746/parts/inner/extra_gz/0x2af3e1.gz.out`（同尺寸同 md5） |
| file 类型 | `data`（裸 ARM 内核 Image，非标准 ELF 容器） |
| 内核版本字符串 | `Linux version 4.6.0-xilinx-g03c746f7 (jenkins@nomd-nomd-fwc-bj) (gcc version 7.2.1 20171011 (Linaro GCC 7.2-2017.11) ) #14 SMP PREEMPT Mon Apr 27 23:35:25 CST 2026` |

**提取方式**：`file` + `strings -n 8` / `strings | grep "Linux version"`。

**结论**：
- 该内核与 2026 构建链（FR-1.9 / rootfs_1 编译时间线）一致，版本为 Xilinx 定制 4.6.0（git tag `g03c746f7`）。
- 当前二次开发聚焦“单颗 BM1746 ASIC 协议/驱动与应用链路”；ko 模块语义已由 `reversed/ko_bitmain_axi/`、`reversed/ko_fpga_mem/` 覆盖，vmlinux 完整反编译优先级低，仅在需要追溯内核子系统行为（如特定驱动框架/调度/内存映射细节）时再做。

## 2. libjansson.so.4.5.0（SD rootfs）

| 项目 | 值 |
|---|---|
| 路径 | `BM1746/sdcard/rootfs_sd/usr/lib/libjansson.so.4.5.0` |
| 大小 | 34,704 B |
| md5 | `44278ad45d2acad41fe3ee09bb6dff15` |
| file 类型 | `ELF 32-bit LSB shared object, ARM, EABI5, dynamically linked, stripped` |
| SONAME | `libjansson.so.4` |
| 依赖 | `libc.so.6` |
| BuildID | `f6f26795554f25e91d43c3c7b899b0248fa3e4da` |
| 备注 | 同目录 `libjansson.so.4` 为 0 字节占位（`d41d8cd98f00b204e9800998ecf8427e`），实际载荷在 `.so.4.5.0` |

**导出能力（strings/readelf 摘要）**：包含标准 jansson API 符号，如 `json_loads/json_loadf/json_load_file/json_loadb`、`json_dump*`、`json_object_*`、`json_array_*`、`json_string*`、`json_integer*`、`json_real*`、`json_true/false/null`、`json_delete`、`json_equal/copy/deep_copy` 等，版本为 2.x 系列常见 ABI（soname .4）。

**结论**：
- cgminer/stratum 与 API 侧 JSON 处理依赖 jansson；该库为通用第三方组件，无 Bitmain 私有逻辑，无需反编译。二次开发可直接使用兼容 soname/ABI 的 jansson（或在新环境静态链接/替换同 API 实现）。

## 3. 证据

- `file`、`readelf -h -d -s`、`strings` 命令输出（2026-11-20，本工作区 Git Bash 环境）
- 关联文档：`BM1746/ARTIFACTS.md`、`BM1746/reversed/README.md`
