# BM1746 辅助用户态工具逆向索引（aux_tools）

> 范围：对 BM1746 工作区中此前未深入说明的 6 个辅助用户态工具做静态逆向（rootfs_1 主版本，辅以 sdcard/rootfs_sd 对比线索）。分析方法：`file`、`readelf -h/-l/-d`、`strings -n 4`、`objdump -d`（对缺 section header/ARM 架构识别失败的 ELF 回退为 Capstone 6.0.0 对 PT_LOAD 代码段/入口/main 反汇编），并结合 `BM1746/SUMMARY.md`、`BM1746/analysis/out`、init 脚本与调用脚本线索。

## 汇总表

| 工具 | rootfs_1 路径 | 大小/MD5 | 一句话功能 | 系统调用方式/接口 | 是否阻塞单颗 BM1746 固件二次开发 | sdcard 对应版本 | 文档 |
|---|---|---:|---|---|---|---|---|
| `cgminer-api` | `usr/bin/cgminer-api` | 9216B / `41cd93a7b8b5b98efbf77910ac8ff2d7` | 本地 cgminer/godminer TCP API 命令行客户端（默认 `summary@127.0.0.1:4028`） | CLI `[command [ip/host [port]]]`，`--help/-h/-?/-o`；脚本调用如 `updateporc.sh` 发 reload | **否**（可用自写 API 客户端/TCP 替代） | `sdcard/rootfs_sd/usr/bin/cgminer-api` (5900B, 不同实现/更早构建) | [cgminer-api.md](cgminer-api.md) |
| `monitor-ipsig` | `usr/bin/monitor-ipsig` | 9216B / `1a08e517f249a9a3045fc653006add0f` | UDP IP/MAC 上报与按键/GPIO 监控守护（IP reporter 类） | `S65monitor-ipsig` 后台启动，无 CLI 参数；`eth0`+UDP 收发、`/sys/class/gpio/*` | **否**（运维发现辅助，非算力路径） | `sdcard/rootfs_sd/usr/bin/monitor-ipsig` (9148B, GPIO 处理/编译时间不同) | [monitor-ipsig.md](monitor-ipsig.md) |
| `monitor-recobtn` | `usr/bin/monitor-recobtn` | 5532B / `794be2a6d004ee449846b3652088cda5` | 恢复按键监控守护，长按/按键时序触发恢复出厂脚本 | `S66monitor-recobtn` 启动并传参 `/usr/bin/factory_config_reset.sh`；监控 GPIO sysfs，fork+exec 脚本 | **否**（恢复策略可替换；触发会中断运行但非正常挖矿组件） | `sdcard/rootfs_sd/usr/bin/monitor-recobtn` (5136B, init 预配 GPIO 不同) | [monitor-recobtn.md](monitor-recobtn.md) |
| `antlogin` | `usr/sbin/antlogin` | 5120B / `fb2a152f42702dcac27c0240fe921acd` | 固定执行 `execlp("login","login","-f","root",NULL)` 的本地登录辅助程序 | 无参数解析；可能作为受控登录入口（仓库内未检索到显式 init/inittab 调用） | **否**（仅影响维护登录方式） | **未找到** | [antlogin.md](antlogin.md) |
| `update-daemon` | `usr/sbin/update-daemon` | 8192B / `788a2031aeaa2e57539d1ead3312788e` | 本地升级 IPC 守护/客户端：监听 `127.0.0.1:22322` 收请求并 `system("/usr/sbin/updateporc.sh %s")` | `S67update-daemon` 启动 `/usr/sbin/daemons &`（仓库中 daemons 为 0B，疑同体/链接）；argv0 `daemonc/daemons` 分模式 | **部分相关，不阻塞**（核心算力非必需；兼容原厂升级链路时需等价实现） | **未找到**（SD 无同款升级守护） | [update-daemon.md](update-daemon.md) |
| `id2mac` | `sbin/id2mac` | 6512B / `a380ac2d354f33bf85772ecc095dc6ff` | 基于 `/dev/urandom`/标识派生并输出 MAC 地址（供网络脚本设置接口 MAC） | `S38network` / sdcard `network.sh` 以 ``mac=`id2mac` `` 调用；stdout 输出 `%02x:%02x:%02x:%02x:%02x:%02x` | **否**（可替换为固定/eeprom MAC 方案） | `sdcard/rootfs_sd/usr/bin/id2mac` (6876B, 同源算法/路径与打包不同) | [id2mac.md](id2mac.md) |

## 关键调用与脚本线索

- `etc/init.d/S65monitor-ipsig` -> `/usr/bin/monitor-ipsig`
- `etc/init.d/S66monitor-recobtn` -> `/usr/bin/monitor-recobtn -- /usr/bin/factory_config_reset.sh`
- `etc/init.d/S67update-daemon` -> `/usr/sbin/daemons &`（与 `update-daemon` 协同/同体存疑）
- `etc/init.d/S38network` -> `id2mac`
- `/usr/sbin/updateporc.sh` -> 解析/验签/刷写或 pool/root 配置更新，末尾可调用 `cgminer-api {"command":"reload","new_api":true}`
- `/usr/bin/factory_config_reset.sh` -> 停服务、清理 `/config`（保留 mac/sn）、重启
- `/etc/init.d/S52miner_act` 也可根据 `/tmp/miner_act` 状态触发重启/恢复/重启 cgminer 等运维动作

## 分析说明

- rootfs_1 中多数目标为 **缺 section headers** 的 stripped ARM ELF，`objdump -d` 在本机 mingw 工具链下不可用；已通过 `readelf -l` 确定 PT_LOAD 代码段并用 **Capstone 6.0.0 (Python, Thumb/ARM)** 对入口、`main` 及关键子函数反汇编，结合字符串交叉引用还原核心行为。
- sdcard 对应版本普遍为较早构建（`GNU/Linux 2.6.32`，2020 年编译时间字符串）且多保留完整节头；功能同类但 GPIO 处理细节、报错文案或安装路径存在差异，已在各工具文档中注明。
- `BM1746/SUMMARY.md` 与 `BM1746/analysis/out` 主要覆盖 BMU 容器、PS/PL 初始化、cgminer 主挖矿链路；本批 6 个工具均处于主算力链路外或仅为运维/升级辅助。
