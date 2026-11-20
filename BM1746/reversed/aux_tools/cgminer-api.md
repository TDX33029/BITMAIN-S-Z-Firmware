# cgminer-api

## 基本信息
- 路径（主版本）: `BM1746/rootfs_1/usr/bin/cgminer-api`
- 大小: 9216 字节
- MD5: `41cd93a7b8b5b98efbf77910ac8ff2d7`
- 文件类型: `ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux-armhf.so.3, missing section headers at 9604`
- 入口点: `0x10a49`（Thumb, CRT `_start`）；推断 `main` 位于 `0x11290`
- ELF 头/段: `readelf -h/-l` 可读（9 program headers: ARM_EXIDX/PHDR/INTERP/LOAD×2/DYNAMIC/NOTE/GNU_STACK/GNU_RELRO）；`readelf -d` 仅见 `SYMBOLIC/NULL` 且 section headers 越界（`missing section headers`）
- 动态依赖（strings/符号痕迹）: `libc.so.6`（socket/htons/connect/send/recv/setsockopt/gethostbyname/atoi/strcmp/strerror/malloc/realloc/fprintf 等），解释器 `/lib/ld-linux-armhf.so.3`

## 用途与系统角色
- cgminer/godminer 侧本地 **API 查询 CLI**，用于向矿机挖矿进程的 TCP API 端口发送命令并打印返回内容（标准 cgminer API 客户端形态）。
- 在本固件中由运维/脚本调用：如 `updateporc.sh` 的 pool 更新后执行 `cgminer-api {\"command\":\"reload\",\"new_api\":true}` 触发矿池配置重载（`BM1746/rootfs_1/usr/sbin/updateporc.sh`）。
- 非守护进程、非 init 启动项；属于辅助诊断/控制工具。

## 命令行/接口/关键路径
- 用法字符串: `usAge: %s [command [ip/host [port]]]`（地址 `0x11714`）
- 帮助判定: `--help`/`-h`/`-?`（`0x1170c/0x11708/0x11704`）
- 默认值（字符串/立即数证据）:
  - command: `summary`（`0x116f0`，main 中 `str [r11,#-8]`）
  - host: `127.0.0.1`（`0x116f8`，main 中 `str [r11,#-0xc]`）
  - port: `4028`（`0xfbc`，main 中 `strh [r11,#-0xe]`，cgminer 标准 API 端口）
- `-o` 选项: 置 `-o` 标志位（`0x1173c`，写全局显示模式标记 `0x22088`），影响输出/展示路径。
- 参数解析逻辑（main 反汇编）: 位置参数依次覆盖 command/host/port；port 经 `atoi` 解析并做有效性检查。
- 网络接口: `gethostbyname` 解析主机，`socket`+`connect` 连接，`setsockopt` 设置 `SO_LINGER/SO_SNDTIMEO/SO_RCVTIMEO`，`send` 发送请求，`recv` 循环接收并按需 `realloc` 缓冲区。

## 核心逻辑概述（基于字符串/反汇编证据）
- `0x11290` (main): 初始化默认 `command="summary"`, `host="127.0.0.1"`, `port=4028`；若 `argv[1]` 为 `--help/-h/-?` 则向 stderr 打印用法并返回；若为 `-o` 则设置显示标志并调整参数起始索引；按位置取 command/host/port（含 `atoi` 转端口）；调用 API 交互函数（`0x10ce4` 附近 callapi 路径）。
- API 交互/错误路径字符串（rodata）: `Couldn't get hostname: '%s'`(`0x11564`)、`Socket initialisation failed: %s`(`0x11584`)、`Socket connect failed: %s`(`0x115a8`)、`cgminer-api, setsocket SO_LINGER failed (%s)`(`0x115c4`)、`... SO_SNDTIMEO failed (%s)`(`0x115f4`)、`... SO_RCVTIMEO failed (%s)`(`0x11624`)、`cgminer api send failed: %s`(`0x11654`)、`cgminer api: OOM (%d)`、`cgminer realloc: OOM (%d)`(`0x11694`)、`Recv failed: %s`(`0x116a8`)、`Recv failed: api server close the socket`。
- 反汇编片段（`0x10d80+` 区域）显示: `gethostbyname` 结果填充 `sockaddr_in`、`connect`、多次 `setsockopt`、`send` 全量请求、`recv` 循环并处理连接关闭/错误，随后进入响应展示/格式化逻辑（存在 `display`/`callapi` 符号字符串）。
- `objdump -d` 无法识别（`file format not recognized`，缺 section headers），核心定位通过 `readelf -l` 确定可执行 LOAD 段并用 Capstone Thumb/ARM 对入口及 `0x11290+` 代码段反汇编完成。

## 与挖矿主链路的关系
- **不在 S70 启矿链路中**，不参与 ASIC 初始化/算力生产；仅作为与 cgminer/godminer API 的控制/查询通道。
- 被 `updateporc.sh` 用于 pool 配置更新后的 `reload`，属于运维辅助能力；缺失会影响该脚本的即时重载动作，但不改变矿机固件核心挖矿二进制本身。
- **不阻塞单颗 BM1746 固件二次开发**（可用自建 API 客户端或直接 TCP 4028 替代）。

## 与其他版本差异（sdcard/rootfs_sd）
- 对应文件: `BM1746/sdcard/rootfs_sd/usr/bin/cgminer-api`
  - 大小/MD5: 5900 字节 / `3ad300168b400aea19a410e1ecd0fca3`（不同于 rootfs_1）
  - 类型: `ELF32 ARM ... for GNU/Linux 2.6.32, BuildID 854c2a..., stripped`（保留 section headers，readelf -d 正常列出 NEEDED libc.so.6 等）
- 功能同类: 同为 cgminer API CLI，字符串显示 `summary`/`127.0.0.1`/`--help`/`usAge...`，支持 socket/connect/send/recv/gethostbyname。
- 接口/实现差异线索: sdcard 版错误文案更简短（如 `Send failed: %s`、`Err: OOM (%d)`、`Reply was '%s'`），无 rootfs_1 中细化的 `SO_LINGER/SO_SNDTIMEO/SO_RCVTIMEO` 失败提示与 `cgminer api ...` 前缀；编译目标/年代更早（GNU/Linux 2.6.32, 2020 年代 SD 恢复镜像环境）。
