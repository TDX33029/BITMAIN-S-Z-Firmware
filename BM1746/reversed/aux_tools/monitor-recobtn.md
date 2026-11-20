# monitor-recobtn

## 基本信息
- 路径（主版本）: `BM1746/rootfs_1/usr/bin/monitor-recobtn`
- 大小: 5532 字节
- MD5: `794be2a6d004ee449846b3652088cda5`
- 文件类型: `ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux-armhf.so.3, for GNU/Linux 3.2.0, BuildID[sha1]=8a3c2a613e3e29091d6e226478f18a0a00704c0d, stripped`
- 入口点: `0x10881`（Thumb CRT）；推断 `main` 位于 `0x106a0`（ARM 代码）
- ELF 头/段: `readelf -h/-l/-d` 正常（9 program headers，含 `GNU_RELRO`；动态段显示 `NEEDED libc.so.6`，有 `.hash/.dynsym/.dynstr/.plt/.got` 等段映射）
- 动态依赖: `libc.so.6`（fopen/__isoc99_fscanf/fclose/access/gettimeofday/usleep/fork/wait/execvp/system/puts/fwrite/exit/abort 等）

## 用途与系统角色
- **恢复键监控守护进程**（`Monitor-recobtn daemon`），检测硬件恢复按钮按压时序，触发恢复/恢复出厂配置脚本。
- 由 `BM1746/rootfs_1/etc/init.d/S66monitor-recobtn` 启动：`start-stop-daemon -b -S -x /usr/bin/monitor-recobtn -- /usr/bin/factory_config_reset.sh`；停止 `killall -9 monitor-recobtn`。
- 关联脚本: `/usr/bin/factory_config_reset.sh`（实际执行恢复出厂配置并重启）。

## 命令行/接口/关键路径
- 命令行: 运行时接收一个参数 `ARGS="/usr/bin/factory_config_reset.sh"`（由 init 传入）；主逻辑在确认按键动作后对该路径执行恢复动作（`execvp`/子进程路径）。
- GPIO/sysfs 接口（字符串/调用证据）:
  - 导出与方向/值操作通过 `system`+echo 及文件读写：`echo %d > /sys/class/gpio/gpio%d/value`（通用格式字符串），`/sys/class/gpio/export`、`/sys/class/gpio/gpio%d`、`/sys/class/gpio/gpio%d/value`
  - 错误: `Open read gpio/export`、`File Write Error!`、`Open read recovery button failure`
- 行为提示字符串: `Detect recovery button push off`、`recovery button off over 3S, wait push on`、`recovery button on`、`error occured.`、`child complete`、`recovery factory complete`
- 编译标识: `monitor-recobtn compile %s--%s\n`，内嵌 `23:54:24`、`Apr 27 2026`。

## 核心逻辑概述（基于字符串/反汇编证据）
- `0x106a0` (main, ARM): 启动打印编译信息（`printf/puts` 路径引用编译时间字符串），初始化/读取 GPIO 状态；主循环逻辑（反汇编可见大量分支、`gettimeofday`/`usleep` 调用与状态比较）：
  1. 读取恢复按键 GPIO 值，检测“按键释放/按下”状态变化；检测到释放打印 `Detect recovery button push off`。
  2. 使用 `gettimeofday` 计时，若释放持续超过约 3 秒则打印 `recovery button off over 3S, wait push on` 并等待再次按下。
  3. 检测到按下打印 `recovery button on`，`fork` 子进程执行恢复命令：`argc>1` 时对子进程路径执行 `execvp(argv[1], &argv[1])`（反汇编中 `0x10858` 附近分支），父进程 `wait` 等待。
  4. 根据子进程/执行结果输出 `error occured.`、`child complete`、`recovery factory complete`，并继续循环监控。
- GPIO 访问辅助函数（`0x109c0` 附近被反复调用）: 打开 `/sys/class/gpio/gpio%d/value` 读取按键电平（失败打印 `Open read recovery button failure`）；另有通过 `system("echo %d > ...")` 控制 LED/指示的路径。
- 反汇编方法: 虽为 stripped，但保留完整 program/section headers；`objdump` 仅因架构识别限制失败，改用 `readelf -l/-d` + Capstone ARM 对 `0x106a0` 起始区域与关键分支反汇编，结合字符串锚点还原状态机。

## 与挖矿主链路的关系
- 不参与挖矿协议、频率/电压控制或 hashboard 数据通路；属于**运维恢复/安全维护**功能。
- 触发时会调用 `factory_config_reset.sh`（停止 dropbear/cgminer、清理 `/config` 中除 mac/sn 等保留项、重启系统），会中断挖矿，但这是人工/按键恢复行为而非正常算力流程组件。
- **不阻塞单颗 BM1746 固件二次开发**（可替换为自定义恢复策略或移除）。

## 与其他版本差异（sdcard/rootfs_sd）
- 对应文件: `BM1746/sdcard/rootfs_sd/usr/bin/monitor-recobtn`
  - 大小/MD5: 5136 字节 / `11daa0756ee824efa528908ef48a1d3c`
  - 类型: `ELF32 ARM ... for GNU/Linux 2.6.32, BuildID 1577dde..., stripped`，结构完整
- 对应 init: `BM1746/sdcard/rootfs_sd/etc/init.d/monitor-recobtn` 同样传入 `ARGS="/usr/bin/factory_config_reset.sh"`，并额外预配置 LED/恢复键 GPIO（`recover=921`, `red_led=941`, `green_led=942`，export/direction）。
- 功能同类: 核心字符串与流程一致（检测按键释放>3s、等待按下、fork/exec 恢复脚本、完成提示）。
- 主要差异线索:
  - 编译时间: sdcard `Jun 16 2020 14:30:27` vs rootfs_1 `Apr 27 2026 23:54:24`。
  - GPIO 访问/提示实现细节不同: sdcard 字符串含硬编码 `echo high/low >/sys/class/gpio/gpio941/942/direction` 与直接读 `/sys/class/gpio/gpio921/value`；rootfs_1 采用通用 `echo %d > /sys/class/gpio/gpio%d/value` 与参数化 gpio 路径。
  - sdcard `factory_config_reset.sh` 保留 `/config/mac` 并重启；rootfs_1 版本保留 `mac|sn` 且在恢复前停 `S50dropbear/S70cgminer`、`sync`、延时后 `reboot -f`（行为更激进/完整）。
