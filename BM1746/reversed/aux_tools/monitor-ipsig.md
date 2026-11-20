# monitor-ipsig

## 基本信息
- 路径（主版本）: `BM1746/rootfs_1/usr/bin/monitor-ipsig`
- 大小: 9216 字节
- MD5: `1a08e517f249a9a3045fc653006add0f`
- 文件类型: `ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux-armhf.so.3, missing section headers at 9648`
- 入口点: `0x109c9`（Thumb CRT）；推断 `main` 位于 `0x10958`（ARM 代码）
- ELF 头/段: `readelf -h/-l` 显示 9 program headers；`readelf -d` 条目异常/不完整且 section headers 越界（`missing section headers`）
- 动态依赖（字符串/符号）: `libc.so.6`（socket/bind/sendto/recvfrom/select/ioctl/setsockopt/inet_ntoa/fopen/fscanf/gettimeofday/usleep/system/access/strcpy/strncmp 等），解释器 `/lib/ld-linux-armhf.so.3`

## 用途与系统角色
- **IP/信号监控守护进程**（init 描述 `Monitor-ipsig daemon`），用于在局域网内上报/交互矿机网络标识信息，并配合按键/GPIO 状态处理。
- 由 `BM1746/rootfs_1/etc/init.d/S65monitor-ipsig` 以 `start-stop-daemon -b -S -x /usr/bin/monitor-ipsig` 后台启动，停止时 `killall -9 monitor-ipsig`。
- 典型用于厂内/运维发现设备 IP/MAC（IP reporter 类行为），不直接控制算力核心。

## 命令行/接口/关键路径
- 命令行: init 脚本无参数启动；二进制字符串中未见 usage/help 文案，推测无复杂 CLI。
- 网络接口（字符串证据）: `socket`、`set socket error...`、`bind error...`、`get IP error`、`send error....`、`send ipmac:\n%s\n`、`send ack OK!\n `、`read error....`、`rev:\n%s\t`、`send_ack("1");` / `send_ack("0");`、`FAILD`、`Time Out`；网卡名 `eth0`。
- GPIO/按键接口（字符串证据）:
  - 通用写值: `echo %d > /sys/class/gpio/gpio%d/value`（`0x115e0`）
  - 导出/读写节点: `/sys/class/gpio/export`、`/sys/class/gpio/gpio%d`、`/sys/class/gpio/gpio%d/value`
  - 错误: `Open read gpio/export`、`File Write Error!`、`Open read recovery button failure`
  - 状态提示: `Key Down!!!!!`
- 编译标识: `monitor-ipsig compile %s--%s\n`，内嵌时间/日期 `23:54:23`、`Apr 27 2026`（`0x11780/0x1178c/0x11798`）。

## 核心逻辑概述（基于字符串/反汇编证据）
- `0x10958` (main, ARM): 启动即打印编译信息（引用 `0x11798/0x1178c/0x11780`），依次调用初始化/子功能（`0x10fe8`、`0x114a8` 等）；进入主循环：
  - 周期/条件检查按键状态函数 `0x114a8`，返回触发时执行提示/处理并再次进入检测流程；
  - 调用 `0x110ec` 构造并发送 IP/MAC 信息，调用 `0x11274` 等待/接收响应；根据结果循环重试或进入下一轮。
- `0x10fe8`: 创建 UDP socket（`socket(AF_INET,SOCK_DGRAM,...)` 痕迹），`setsockopt`（含广播/复用相关，失败报 `set socket error...`），`bind`（失败报 `bind error...`），配置目标地址（反汇编中写入端口/地址常量到 sockaddr 结构），保存 socket fd 到全局（`0x220a4`）。
- `0x110ec`: 通过 `ioctl` 获取 `eth0` 网络信息并格式化 MAC/IP 相关报文（字符串 `get IP error`、`send ipmac:\n%s\n`），`sendto` 发送（失败 `send error....`），成功路径打印 `send ack OK!` 等。
- `0x11274`: 使用 `select`/`recvfrom` 等待响应（超时/错误对应 `Time Out`/`read error....`），解析收到内容（`rev:\n%s\t`）并依据 `send_ack("1")`/`send_ack("0")`/`FAILD` 逻辑返回状态。
- `0x114a8`: GPIO 按键检测/消抖与 LED 指示序列（多次 `usleep`，调用 GPIO 读写辅助），检测到按下输出 `Key Down!!!!!` 并返回触发状态。
- 反汇编方法: `readelf -h/-l` 定位 LOAD 段，Capstone 对入口/`0x10958` 及关键子函数按 ARM 模式反汇编（缺 section headers 导致 objdump 不可用）。

## 与挖矿主链路的关系
- 不参与 cgminer/godminer 启动、算力板初始化、nonce/任务处理；属于外设/运维发现辅助守护进程。
- 停止该进程通常仅影响 IP 上报/发现与相关按键指示行为，不影响已运行矿机的核心哈希计算链路。
- **不阻塞单颗 BM1746 固件二次开发**。

## 与其他版本差异（sdcard/rootfs_sd）
- 对应文件: `BM1746/sdcard/rootfs_sd/usr/bin/monitor-ipsig`
  - 大小/MD5: 9148 字节 / `3a109798c3a266b6a4e8bff775b655d4`
  - 类型: `ELF32 ARM ... for GNU/Linux 2.6.32, BuildID 554971..., stripped`（有完整 section headers，动态段正常）
- 功能同类: 同样实现 socket 监听/收发、`eth0` IP/MAC 上报、GPIO 导出与按键/LED 控制、`Key Down!!!!!` 等核心字符串一致。
- 主要差异线索:
  - 编译时间: sdcard 为 `Jun 16 2020 14:30:46`（旧 SD 恢复镜像），rootfs_1 为 `Apr 27 2026 23:54:23`。
  - LED/GPIO 操作方式: sdcard 字符串含硬编码 `echo 0/1 >/sys/class/gpio/gpio941/value`、`gpio942`，并直接访问 `/sys/class/gpio/gpio943`；rootfs_1 采用格式化写值 `echo %d > /sys/class/gpio/gpio%d/value`，更通用。
  - 构建环境/ABI 标志略有差异（GNU/Linux 2.6.32 vs 缺少对应 BuildID 字段的 stripped 变体）。
