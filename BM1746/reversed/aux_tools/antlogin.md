# antlogin

## 基本信息
- 路径（主版本）: `BM1746/rootfs_1/usr/sbin/antlogin`
- 大小: 5120 字节
- MD5: `fb2a152f42702dcac27c0240fe921acd`
- 文件类型: `ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux-armhf.so.3, missing section headers at 5424`
- 入口点: `0x10351`（Thumb CRT）；推断 `main` 位于 `0x10314`
- ELF 头/段: `readelf -h/-l` 显示 9 program headers；`readelf -d` 几乎为空（仅 `NULL`），且 section headers 越界（`missing section headers`）
- 动态依赖（字符串/符号）: `libc.so.6`（`execlp`、`abort`、`__libc_start_main`），解释器 `/lib/ld-linux-armhf.so.3`

## 用途与系统角色
- 极简登录辅助程序：直接以 `root` 身份无密码参数形式拉起 `login`（`login -f root` 语义）。
- 常见于 Bitmain 固件作为受控登录入口（如串口/getty/特定认证封装的目标程序），用于绕过常规交互登录流程进入 root shell；本仓库中未在 `etc/inittab`/init 脚本内直接检索到调用点（`grep antlogin` 仅命中二进制自身），但二进制位于 `usr/sbin` 表明其为系统登录链路组件候选。
- 不参与挖矿业务逻辑。

## 命令行/接口/关键路径
- 命令行: 未见参数解析；`main` 固定调用 `execlp`。
- 关键字符串（rodata, 通过 main 中 `movw/movt` 立即数解析）:
  - `0x10468`: `"login"`
  - `0x1045c`: `"root"`
  - `0x10464`: `"-f"`
- 调用形式（反汇编证据）: `execlp("login", "login", "-f", "root", NULL)`。

## 核心逻辑概述（基于字符串/反汇编证据）
- `0x10314` (main, Thumb/ARM 混合区按实际反汇编): 设置参数寄存器后调用 `execlp`@PLT（`bl 0x102e4`），参数为 `"login"`/`"-f"`/`"root"`；随后返回 0（若 `execlp` 成功通常不返回）。
- 实现无额外文件 I/O、无网络、无配置读取，仅依赖 PATH 中的 `login` 程序；失败时行为取决于 libc/`execlp`（可能返回并退出）。
- 证据获取: `file/readelf` + `strings` 提取符号与参数字符串；因缺 section headers，使用 Capstone 对入口及 `0x10314` 反汇编确认 `execlp` 调用及 rodata 指针。

## 与挖矿主链路的关系
- 与 BM1746 ASIC 初始化、cgminer/godminer 运行、算力/矿池通信均无直接关系。
- 仅影响设备本地登录/维护通道；若作为串口或特定服务登录 shell，替换/移除会改变运维访问方式。
- **不阻塞单颗 BM1746 固件二次开发**（二次开发通常关注挖矿与升级链路；登录策略可独立调整）。

## 与其他版本差异（sdcard/rootfs_sd）
- 在 `BM1746/sdcard/rootfs_sd` 中**未找到同名二进制**（`find ... -name antlogin` 无结果）。
- 因缺少对应版本样本，无法做字符串/行为级对比；可初步判断该工具为 rootfs_1（NAND 主固件）特有或在 SD 恢复镜像中未包含/命名不同。
