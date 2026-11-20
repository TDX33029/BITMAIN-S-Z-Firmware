# id2mac

## 基本信息
- 路径（主版本）: `BM1746/rootfs_1/sbin/id2mac`
- 大小: 6512 字节
- MD5: `a380ac2d354f33bf85772ecc095dc6ff`
- 文件类型: `ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux-armhf.so.3, for GNU/Linux 3.2.0, BuildID[sha1]=cc703daae74bf43de8cb125b39f4805651ddad94, stripped`
- 入口点: `0x10469`（Thumb CRT）；`main` 位于 `0x10438`（ARM）
- ELF 头/段: `readelf -h/-l/-d` 正常（9 program headers；动态段 `NEEDED libc.so.6`，含 `.hash/.dynsym/.dynstr/.plt/.got/.init_array/.fini_array` 等）
- 动态依赖: `libc.so.6`（fopen/fread/fclose/sprintf/puts/strlen/abort/__libc_start_main 等，字符串及 PLT 证据）

## 用途与系统角色
- **根据设备标识/随机源生成或派生 MAC 地址并输出**的工具，供网络初始化脚本获取 MAC。
- 调用点:
  - `BM1746/rootfs_1/etc/init.d/S38network`: `mac=`id2mac``，随后写入 `/etc/mac` 并用于网络配置。
  - `BM1746/sdcard/rootfs_sd/etc/init.d/network.sh`: 同样 `mac=`id2mac``。
- 属于系统启动早期网络配置依赖组件之一。

## 命令行/接口/关键路径
- 命令行: `main` 未见参数解析，直接执行生成逻辑并打印结果。
- 关键输入/常量（字符串 rodata）:
  - `/dev/urandom`（`0x10d4c`）：随机种子来源
  - `CPUID`（`0x10cfc`）：可能用于读取 CPU/板级标识（getenv/系统信息路径）
  - 模板 `xxxxxxxxxxxx4xxxyxxxxxxxxxxxxxxx`（`0x10d24`）：UUID/标识模板（含 version/variant 位占位）
  - 字符集 `0123456789abcdef`（`0x10d5c`）
  - 输出格式 `%02x:%02x:%02x:%02x:%02x:%02x`（`0x10d04`）
- 输出: 向 stdout 打印 MAC 地址字符串（`puts`/`sprintf` 路径）。

## 核心逻辑概述（基于字符串/反汇编证据）
- `0x10438` (main, ARM): 在栈上分配缓冲区，依次调用
  - `0x106c8(buf)`：生成/填充 16 字节（或等效）中间标识数据；
  - `0x105b0(buf, outbuf)`：将中间数据转换为 MAC 字符串；
  - `puts(outbuf)` 输出；返回 0。
- `0x106c8`（反汇编可见大量位运算/置换与循环）:
  - 若全局缓存未初始化，则尝试从 `/dev/urandom` 读取 16 字节随机数（`fopen` mode `"rb"` at `0x10d48`，路径 `0x10d4c`，`fread` 16 字节，失败则回退/返回错误）；将随机态作为内部状态保存。
  - 对内部 128-bit 状态执行类 xorshift/混洗更新，产生当前轮输出字节序列。
  - 按模板 `xxxxxxxxxxxx4xxxyxxxxxxxxxxxxxxx`（`0x10d24`）与 hex 字符集 `0123456789abcdef`（`0x10d5c`）将状态 nibble 映射为字符（含将特定位置设置为版本 `4`、variant `y` 语义的模板驱动逻辑），写入调用方缓冲区并以 NUL 结尾。
- `0x105b0`（反汇编可见 `CPUID` 相关分支与 CRC/哈希式计算）:
  - 尝试获取 `CPUID`/环境或系统标识（`getenv("CPUID")` 痕迹，字符串 `0x10cfc`）；若存在则拷贝/使用该标识，否则走备用路径。
  - 对标识数据执行校验和/哈希计算（循环中使用查表异或与移位累积，生成 32-bit/多字节摘要），取结果字节组合成 6 字节 MAC。
  - 使用 `sprintf(out, "%02x:%02x:%02x:%02x:%02x:%02x", ...)`（格式串 `0x10d04`）格式化输出；代码中对首字节做本地管理地址位调整倾向（`and reg, #0xfe` 等）以生成单播/本地分配 MAC。
- 反汇编方法: ELF 结构完整但 stripped；`readelf` 提取段/动态信息，Capstone ARM 对 `main/0x106c8/0x105b0` 反汇编并结合 rodata 字符串与常量表引用还原算法骨架。

## 与挖矿主链路的关系
- 不参与算力、挖矿协议或 ASIC 控制；但影响**网络接口 MAC 配置**，间接关系到设备联网、运维访问与可能的网络身份识别。
- 若 MAC 生成异常可能导致网络不可达，从而影响矿机接入矿池；但工具本身不在 hash 计算路径上。
- **基本不阻塞单颗 BM1746 固件二次开发**（可替换为固定 MAC、eeprom/otp 读取或其他生成策略）。

## 与其他版本差异（sdcard/rootfs_sd）
- 对应文件: `BM1746/sdcard/rootfs_sd/usr/bin/id2mac`
  - 大小/MD5: 6876 字节 / `dcfa656b83d740cf6bfdb376f910f11b`
  - 类型: `ELF32 ARM ... for GNU/Linux 2.6.32, BuildID 084344..., stripped`，结构完整
  - 安装信息: `var/lib/opkg/{status,info/id2mac.*}` 记录 `Package: id2mac`, `id2mac version 1.0-r0`, 文件路径 `/usr/bin/id2mac`（注意路径与 rootfs_1 的 `/sbin/id2mac` 不同）。
- 功能同类: 字符串高度一致（`/dev/urandom`、`%02x:%02x:%02x:%02x:%02x:%02x`、UUID 模板、`0123456789abcdef`），推断同源实现/相近算法。
- 差异线索: 二进制大小与链接环境不同（2.6.32 vs 3.2.0），sdcard 版 `CPUID` 明文字符串未在简单 strings 中同样显现但整体逻辑族相同；rootfs_1 路径位于 `/sbin` 且被 NAND rootfs 网络脚本调用，sdcard 位于 `/usr/bin` 且为 opkg 包管理。
