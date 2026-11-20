# update-daemon

## 基本信息
- 路径（主版本）: `BM1746/rootfs_1/usr/sbin/update-daemon`
- 大小: 8192 字节
- MD5: `788a2031aeaa2e57539d1ead3312788e`
- 文件类型: `ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux-armhf.so.3, missing section headers at 9628`
- 入口点: `0x10b85`（Thumb CRT）；推断 `main` 位于 `0x10930`（ARM 代码）
- ELF 头/段: `readelf -h/-l` 显示 9 program headers；`readelf -d` 仅 `NULL` 且 section headers 越界（`missing section headers`）
- 动态依赖（字符串/符号）: `libpthread.so.0`、`libc.so.6`（socket/bind/listen/accept/connect/send/recv/shutdown/pthread_create/pthread_exit/system/access/strstr/inet_addr/snprintf/fwrite/__assert_fail 等）

## 用途与系统角色
- **本地更新守护/客户端二合一工具**，用于通过本机回环 TCP 通道接收升级触发请求并调用升级处理脚本。
- 启动方式: `BM1746/rootfs_1/etc/init.d/S67update-daemon` 执行 `/usr/sbin/daemons &`（后台）。仓库中 `BM1746/rootfs_1/usr/sbin/daemons` 为 **0 字节文件**；同目录存在 `update-daemon` 二进制，且守护逻辑字符串含 `daemons`/`daemonc`，推测实际设备上 `daemons` 为 `update-daemon` 的拷贝或符号链接（以 argv[0] 选择模式）。
- 与升级链路配合的脚本: `/usr/sbin/updateporc.sh`（接收文件路径参数，解析 BMU/pool/root 等更新包并执行校验、写 nand、reload 等）。

## 命令行/接口/关键路径
- 监听地址/端口（字符串/反汇编证据）: `127.0.0.1`（`0x11104`）、`22322`（`0x11110`）；日志 `Will Listening To Port[%d]`（`0x11168`）。
- 模式相关字符串: `daemonc`（`0x11134`）、`daemons`（`0x1113c`）。`main` 根据 `argv[0]`/参数分支：非 `daemonc/daemons` 形式且带文件参数时走客户端发送路径（调用 `0x10e90`）。
- 关键路径/命令: `/usr/sbin/updateporc.sh %s`（`0x11054`，由服务端处理线程通过 `system()` 执行）。
- 进程/线程与网络: `socket/bind/listen/accept` 服务端循环；每连接 `pthread_create` 创建处理线程；客户端路径创建 socket、`connect` 到 `127.0.0.1:22322` 并 `send/recv`。
- 运行时日志/错误: `ProcessServer`、`New Thread!`、`Recv:'%s'`、`File does not exist: %s`、`Invalid client content.`、`system err!`、`system status:[%d]`、`Shell Exec Successfully!`、`Shell Execute Failed, Ret[%d]`、`Other Exit Ret[%d]`、`Come On WUHAN`、`socket err:`、`connect err:`、`bind error!`、`listen error!`、`waiting for new connection...`、`Accept error!`、`A new connection occurs!`、`pthread_create error!`、`ret != -1`、`Server shuts down`、`Cannot Find Command!`。

## 核心逻辑概述（基于字符串/反汇编证据）
- `0x10930` (main, ARM): 
  - 读取 `argv[0]` 并与 `"daemonc"`、`"daemons"` 比较（`0x11134/0x1113c`）；若为 `daemons`/服务端模式则初始化 TCP server：`socket` -> `bind(127.0.0.1:22322)` -> `listen` -> 循环 `accept`，对每个连接 `pthread_create` 启动处理线程（失败输出 `pthread_create error!`，成功打印 `A new connection occurs!`/`New Thread!`）。
  - 若命中客户端分支（如 `argv[1]` 提供待处理文件/命令参数），调用 `0x10e90` 向本地守护发送请求后退出。
  - 各步骤均有断言/错误分支（`sockfd_server != -1`、`bind error!`、`listen error!` 等）。
- `0x10e90` (客户端发送函数, ARM): 构造目标地址 `127.0.0.1:22322`，`socket/connect`（错误 `socket err:`/`connect err:`），将参数内容/文件路径通过 `send` 发送，`recv` 读取响应（缓冲区约 0x400），校验返回码（成功路径返回 0，失败返回负值），用于与守护进程交互。
- 服务端请求处理（由线程入口调用，`ProcessServer` 字符串可见）: 接收客户端数据（`Recv:'%s'`），做基本校验（`access` 检查文件存在 -> `File does not exist: %s`；内容合法性 -> `Invalid client content.`），拼接并执行 `system("/usr/sbin/updateporc.sh %s")`；依据 `system` 返回值输出 `Shell Exec Successfully!` / `Shell Execute Failed, Ret[%d]` / `Other Exit Ret[%d]` / `system err!`。
- 反汇编方法: `readelf -h/-l` 确认可执行段；`objdump` 因缺 section headers 失败；使用 Capstone ARM 对 `0x10930`、`0x10e90` 等地址反汇编，并结合 rodata 字符串定位分支语义。

## 与挖矿主链路的关系
- 不直接驱动 ASIC/算力，但属于**固件升级与配置下发链路**的本地 IPC 前端：通过 22322 本地端口接收请求并触发 `updateporc.sh`，后者可解包 BMU、验签、刷写 NAND、或在 pool/root 更新后调用 `cgminer-api reload` 并影响矿池配置生效。
- 对“挖矿算力实时路径”非必需；对“在线/本地升级与部分运维更新流程”重要。
- **部分相关但通常不阻塞单颗 BM1746 固件二次开发**：若自建升级机制可绕过该守护；若需兼容原厂升级入口/脚本生态则需保留或替换等价功能。

## 与其他版本差异（sdcard/rootfs_sd）
- 在 `BM1746/sdcard/rootfs_sd` 中**未找到 `update-daemon`/`daemons` 二进制**（检索无结果）；SD 镜像更偏向恢复/运行环境，不包含同款 NAND 在线升级守护。
- 无法做同名二进制级对比。可对比的关联脚本差异: rootfs_1 的 `updateporc.sh` 支持 `{type:pool/root/update}` 多分支处理及 `cgminer-api reload`；SD 侧未见对应升级守护调用链。
