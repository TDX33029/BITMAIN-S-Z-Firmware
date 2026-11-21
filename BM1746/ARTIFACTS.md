# BM1746 逆向工作区产物说明（ARTIFACTS）

| 元信息 | 内容 |
|---|---|
| 版本 | v1.0 |
| 日期 | 2026-11-20 |
| 基准二进制 | analysis\cgminer_z15_bm1746.elf（md5 `709cc6f0f12c0d14987e07a13c492c02`，422 KB，ARM 静态 ELF；本工作区全部 FUN_000xxxxx 地址以它为基准，PT_LOAD: vaddr 0x8000 → 文件偏移 0） |
| 目标设备 | Antminer Z15 Pro（BM1746 ASIC / Xilinx Zynq-7007S，Equihash） |
| 读者 | 基于**本工作区**做"从零构建单颗 BM1746 工作固件"二次开发的工程师 |
| 验证状态摘要 | CRC16-CCITT 表 256/256 PASS；频率表 115/115 PASS；TX 任务帧 146B+CRC 大端@0x90 PASS；RX 0x5D=93B、91B 子块 CRC@+0x59 PASS；5bit checksum 重实现 PASS；RX 前导实测 AA 55（FPGA FIFO 字序，与 TX 55 AA 相对）；5 处文档错误已于 2026-11-20 修正（详见第 6 节） |

> 说明：本文所有路径均为相对 `BM1746\` 的工作区相对路径。函数地址均为 cgminer 基准 ELF（上述 md5）中的虚拟地址。除标注"见索引"外，所有函数语义均经过反编译抽读或三轮审计字节级复核。

---

## 1. 工作区总览

整个工作区是 **Antminer Z15 Pro (BM1746/Zynq-7007S) 固件逆向工作区**：从原始升级包解包（original/parts/sdcard）→ 分析与 Ghidra 工程（analysis）→ 反编译函数库与常量表（reversed）→ 协议/可行性报告（bm1746），四层递进支撑固件重写。

| 目录 | 体量 | 用途 |
|---|---|---|
| original\ | 35M | 原始固件：2026 NAND 升级整包 FR-1.9 BMU 容器 + 2020 SD 恢复包（fw.tar.gz 及其签名） |
| parts\ | 31M | BMU 容器按成员表零猜测切片产物：BOOT.bin、内核、位流、FSBL、两个 rootfs 镜像等 |
| sdcard\ | 152M | SD 恢复镜像内容（V0.1.18, 2020）：BOOT.bin/uImage/uramdisk（100M ext2）+ rootfs_sd\ 解包树 |
| rootfs_1\ | 15M | 2026 NAND 主文件系统解包（minerfs.image.gz） |
| rootfs_2\ | 8.7M | 2026 升级 ramfs 解包（update.image.gz） |
| analysis\ | 100M | 基准 ELF、裸 bin、vmlinux、Ghidra 工程（2 个 .rep）、7 个 Java 导出器、out\ 分析产物、dtb\ DTS 转储、gap_reports\ 缺口分析报告 |
| reversed\ | 7.2M | 反编译函数库：1758 个 .c（按二进制分目录、cgminer 再按 16 功能类分子目录）+ 常量表 + 协议字符串 + 索引 |
| bm1746\ | 1.5M | 核心报告：串口寄存器操作完整报告、单芯片可行性核验、频率表、cgminer 全量单文件反编译 |

---

## 2. 原始固件资产（original\、parts\、sdcard\）

### 2.1 original\ — 原始入手文件

| 文件 | 格式 | 角色 | md5 前 8 位 |
|---|---|---|---|
| FR-1.9(260427-Z15 Pro).bmu | BMU 容器（头 0x0126、日期 20260427，内含 RSA-2048 根公钥 @0x1D..0x1DB、256B 签名 @0x410、7 项成员表 @0x51D、成员数据 @0x800、尾部 2048B 随机 footer） | 2026 NAND 升级整包（19.3M），升级入口文件 | 7c632caa |
| ANTMINER-Z15-user-800M-202007031139_6134\fw.tar.gz | gzip tar（内含 BOOT.bin/devicetree.dtb/uImage/uramdisk.image.gz/fileinfo/md5_info/runme.sh/version） | SD 恢复包主体（17.1M，V0.1.18, 2020-07-03, "user-800M"） | 80b179c7 |
| ANTMINER-…\runme.sh（+ .sig 512B） | shell | SD 卡自动执行脚本 | abc235ef |
| ANTMINER-…\cert.pem（+ .sig 512B） | PEM 证书 | 签名链校验证书 | 0b5e598a |
| ANTMINER-…\version_number | 8 字节 | 包版本号 | f0ae1c3b |

#### 2.2.0 FR-1.9 BMU 成员表（0x51D 起 7 项，重打包升级包时按此布局）

| idx | 成员 | 大小 (B) | 偏移 |
|---|---|---|---|
| 0 | BOOT.bin | 0x2A8CC0 | 0x000800 |
| 1 | devicetree.dtb | 0x001F4E | 0x2A94C0 |
| 2 | uImage | 0x3DE8E8 | 0x2AB40E |
| 3 | minerfs.image.gz | 0x7CFD8F | 0x689CF6 |
| 4 | update.image.gz | 0x4156CA | 0x0E59A85 |
| 5 | crl.tar.gz | 0x000228 | 0x126F14F |
| 6 | miner.btm.tar.gz | 0x00070C | 0x126F377 |

### 2.2 parts\ — BMU 成员切片与 BOOT.bin 内部结构（2026 升级链路）

| 文件 | 格式 | 角色 | md5 前 8 位 |
|---|---|---|---|
| BOOT.bin | Xilinx bootgen 容器（FSBL 分区表 @rel 0xC80：PH0=FSBL 33KB / PH1=Zynq7007_miner.elf 521KB（内嵌位流 @0x21E30, 254KB）/ PH2=Zynq7007_miner.bit 140KB 加载至 0x4000000=PL；尾部 u-boot 2016.07 自带 dtb） | 启动第一阶段（2.7M，NAND mtd0） | ac40608b |
| BOOT_FSBL.bin | bootgen PH0 分区 | FSBL 单分区抽出件（32,772 B） | e602c799 |
| Zynq7007_miner.elf | ARM ELF | FSBL 主程序（ps7_init + PCAP 位流加载） | e83652a8 |
| Zynq7007_bitstream.bin | Xilinx 位流（7z007s） | PL 固件（254,400 B，PH1 内嵌副本） | 3a9c0374 |
| Zynq7007_miner.bit.bin | PH2 位流分区 | PL 固件（140,627 B，加载地址 0x4000000） | 52d9da43 |
| devicetree.dtb | FDT（compatible xlnx,zynq-7000；ps7-nand MTD 分区表：BOOT/kernel 0x0+0x2800000、ramfs 0x2800000+0x2000000、configs 0x4800000+0x800000、sig、upgrade-ramfs 0x6000000+0x1000000 等） | 内核设备树 | ad826c24 |
| uImage | uImage + gzip（Linux-4.6.0-xilinx-g03c746f7，zImage，load/entry 0x8000） | 内核（4.1M） | 55e20452 |
| minerfs.image.gz | gzip → ext2 | 主 rootfs（32MB ext2，编译时间 2026-04-27 23:35:40 CST，"Antminer Z15 Pro / Equihash"，subtype=zynq7007_BEZ36501） | e9a09abd |
| update.image.gz | gzip → ext2 | 升级 ramfs（32MB ext2，含 Update.sh 烧写流程 + FileParser 工具） | 33fd7b12 |
| crl.tar.gz | gzip tar（crl 10,240B + crl.sig） | 证书吊销列表 | 48b3f74a |
| miner.btm.tar.gz | gzip tar（miner.btm 2,048B（MAC AppleDouble 包装的二进制）+ .sig） | miner.btm 组件 | c4bd3fbb |
| footer_0x800.bin | 原始字节 | BMU 尾部 2048B 随机数据 | 未记录 |
| inner\extra_gz\0x2af3e1.gz.out | gzip 解包产物 | vmlinux（ARM 内核 Image，11,750,816 B） | 6c3ef57d |
| inner\extra_gz\0x126f14f.gz.out / 0x126f377.gz.out | gzip 解包产物 | crl 包 / miner.btm 包原始切片 | 未记录 |

### 2.3 sdcard\ — SD 恢复镜像（2020，V0.1.18）

| 文件 | 格式 | 角色 | md5 前 8 位 |
|---|---|---|---|
| BOOT.bin | bootgen 容器 | SD 恢复用启动镜像（与 parts\BOOT.bin **不同**：2,743,932 B） | fc91abeb |
| uImage | uImage + gzip | SD 恢复用内核（4,070,312 B） | 75f8c0c7 |
| devicetree.dtb | FDT | SD 恢复用设备树 | 6b288d49（取自 md5_info） |
| uramdisk.image.gz | gzip → ext2 | 恢复 ramfs 压缩包（12,772,650 B） | fd8ac886 |
| uramdisk.ext2.img | ext2 | 解包后的 ramfs（100M）；逆向所用 cgminer ELF 即从 SD ext2 镜像按 inode 恢复（见 bm1746\BM1746_uart_register_ops.md 卷首注） | 2755a7e5 |
| version / fileinfo / md5_info / runme.sh(+sig) | 文本/脚本 | 包元数据与校验清单 | 468ada8c / 未记录 / 3838cb07 / 114329ae |
| rootfs_sd\ | ext2 解包树 | 2020 SD 卡系统（详见 2.4） | — |

### 2.4 三代 rootfs 构建（同名二进制 md5 全不同，不可互替）

| 构建代 | 目录 | 出身 | 年代/版本 | 关键二进制 md5（前 8 位） |
|---|---|---|---|---|
| rootfs_1 | rootfs_1\ | minerfs.image.gz（NAND 主 fs，mtd1） | 2026（FR-1.9，编译 2026-04-27） | cgminer-api `41cd93a7`、FileParser `f4d8cabd`、bitmain_axi.ko `5d00a362`、fpga_mem_driver.ko `fc136134`；S70cgminer 指向 /usr/bin/godminer（**该二进制不在本包**，由升级分区/现场组件提供）；antlogin、update-daemon、id2mac、monitor-ipsig、monitor-recobtn 均在 |
| rootfs_2 | rootfs_2\ | update.image.gz（升级 ramfs，mtd5） | 2026 | FileParser `56358ea4`；etc\init.d\Update.sh 为 BMU 烧写流程入口（mtd0 三份备份+dtb+uImage / mtd1 minerfs / mtd3 sigImage / mtd5 update fs）；无 lib\modules |
| rootfs_sd | sdcard\rootfs_sd\ | SD 恢复镜像 SD 卡分区 | 2020 V0.1.18 | miner-monitor `1679ed0d`、cgminer-api `3ad30016`、bitmain_axi.ko `f2517c6f`、fpga_mem_driver.ko `990edb34`；etc\init.d\cgminer.sh 中 DAEMON=/usr/bin/cgminer，opkg 清单（var\lib\opkg\info\cgminer-z15.list）声明 /usr/bin/cgminer，但该二进制未在解包树中（cgminer 从 uramdisk ext2 按 inode 恢复，见 analysis\） |

三代同名二进制（cgminer-api、FileParser、两个 .ko）md5 互不相同：三套是**不同构建**，重写时应以 2026 链路（rootfs_1）为基线、以 2020 SD 链路（rootfs_sd）为协议参照。

---

## 3. analysis\（分析与 Ghidra 工程）

### 3.1 基准二进制

| 文件 | 说明 | md5 前 8 位 |
|---|---|---|
| cgminer_z15_bm1746.elf | **本工作区地址基准**。2026-11-20 从 D:\Document\tempPrj\ZEC\_unpack\bm1746\ 归档入库；422,244 B；ARM 静态 ELF；SD 恢复镜像 cgminer-z15（git branch "bm1746"，driver-btm-zcash.c 编译产物）；Ghidra CG.rep 的分析对象 | 709cc6f0 |
| vmlinux.bin | ARM 内核 Image，11,750,816 B（11.75M），与 parts\inner\extra_gz\0x2af3e1.gz.out 同尺寸同 md5；**未反编译** | 6c3ef57d |
| fsbl_ph1_raw.bin | FSBL 主程序裸 bin（= parts\Zynq7007_miner.elf） | e83652a8 |
| ph0_raw.bin / ph2_raw.bin | bootgen PH0（FSBL 头段）/ PH2（PL 位流分区）裸 bin | e602c799 / 52d9da43 |
| uboot_raw.bin / uboot2.bin / uboot3.bin | U-Boot 2016.07 三个切片/还原件；**uboot2.bin 为 uboot_funcs 反编译来源** | 3aa8a7f3 / 892e183b / 5a438212 |
| minerfs_recheck.img | 主 rootfs 复核用 ext2 镜像（32M） | 7d75025d |

### 3.2 Ghidra 工程与导出脚本

| 路径 | 内容 |
|---|---|
| ghidra_proj\ZECproj.gpr / ZECproj.rep | 覆盖 ko（bitmain_axi、fpga_mem）+ FSBL + FileParser + monitor 的工程，可复查这四个二进制的任何函数 |
| ghidra_proj_cgminer\CG.gpr / CG.rep | cgminer 专属工程（基准 ELF 的全部 644 函数可复查） |
| scripts\ | 7 个 Ghidra Java 导出器：DumpAll（单文件全量）、ExportFuncs（单函数 .c + 调用图 + 字符串索引，reversed\ 各目录即其产物）、DumpRegWrites、DumpHooks、DefineStrings、FindStrs、XrefDump；run_all_exports.sh 为 headless 批处理入口 |

### 3.3 out\ 分析产物

| 文件 | 内容 |
|---|---|
| ASIC_INIT_REPORT.md | PS/FSBL 级初始化分析（SLCR 解锁 0xDF0D/锁定 0x767B、DDRIOB、MIO、时钟、PCAP 加载位流） |
| ps7_init_full.txt + ps7_final.json | FSBL ps7_init 540 条寄存器序列全量导出（FSBL 重写的直接输入） |
| reg_writes.txt | 寄存器写序列关联的反编译片段 |
| pcap_packet_log.txt + pcap_regs.json | PCAP（PL 配置口）写操作日志——**全部为 FPGA 位流 Type1 配置帧，无任何真实 ASIC 通信帧**（真实 ASIC 帧语料缺失的判据即出于此） |
| strings_map.txt | FSBL 字符串 → 函数引用映射 |
| all_decomp.c | FSBL 全量单文件反编译（检索用） |
| out_axi\、out_uboot\、out_uboot2\、out_uboot3\ | 各二进制 all_decomp.c 平铺反编译（检索用） |

### 3.4 dtb\ 与 gap_reports\（2026-11-20 缺口处理新增）

| 路径 | 内容 |
|---|---|
| dtb\parts_devicetree.dts.txt | parts\devicetree.dtb（8014B, ad826c24）的 FDT v17 结构化转储：根/compatible/memory/chosen、UART0（0xe0000000, IRQ 27, 115200）、amba/PS 外设、SMCC+NAND 分区布局 |
| dtb\sdcard_devicetree.dts.txt | sdcard\devicetree.dtb（7578B, 6b288d49）的同类转储 |
| gap_reports\devicetree_analysis.md | 两份 DTB 对比：sdcard 删 I2C@e0005000 与 upgrade/sig/reserve 分区，改为 BOOT.bin-env-dts-kernel/reserve/ramfs-bak 布局；结论：DTB 反映 PS/PL 可见映射，不含 BM1746 ASIC 寄存器/协议，单芯片直驱仍需 bitstream/总线拓扑/原理图证据 |
| gap_reports\ko_fileparser_diff.md | rootfs_sd vs rootfs_1 两个 ko（同源重编译，设备名/mmap/open-release-init-exit 一致，vermagic/工具链不同不可混装）、rootfs_2 vs rootfs_1 FileParser（`-s/-n/-q` 升级路径一致，rootfs_1 多 miner_type/hash 校验字符串）；结论：接口语义兼容，通常无需补反编译 |
| gap_reports\vmlinux_jansson_info.md | vmlinux.bin 元信息（Linux 4.6.0-xilinx-g03c746f7, 2026-04-27 构建，与 parts\inner\extra_gz\0x2af3e1.gz.out 同源）；sdcard libjansson.so.4.5.0（SONAME libjansson.so.4，标准 jansson API，通用依赖无需反编译） |

---

## 4. reversed\（反编译函数库，1758 个 .c）

### 4.1 目录组织与来源二进制

组织方式：按来源二进制分目录；cgminer 再按 16 功能类分子目录（reversed\cgminer\），并保留一份原始平铺导出（reversed\cgminer_funcs\）；其余二进制各带 _classified\ 分类索引。

| 目录 | 来源二进制 | md5 前 8 位 | .c 数量 | 组织 |
|---|---|---|---|---|
| cgminer\ | cgminer_z15_bm1746.elf | 709cc6f0 | 644 | 16 功能类子目录 + 00_INDEX.md + _callgraph.txt（1821 条）+ _string_xref.txt（1453 条）+ _func_index.txt（644 条） |
| cgminer_funcs\ | 同上（平铺导出） | 709cc6f0 | 644 | 单文件 FUN_000xxxxx.c，全部关键函数引用均指向此目录 |
| fsbl_funcs\ | Zynq7007_miner.elf | e83652a8 | 165 | + _classified\（00_INDEX.md + function_map.txt）；ps7_init 540 条 → analysis\out\ps7_init_full.txt；内嵌 PL 位流 @0x21E30 |
| uboot_funcs\ | uboot2.bin（U-Boot 2016.07） | 892e183b | 140 | 基址 0x40000000 |
| monitor_funcs\ | miner-monitor（rootfs_sd） | 1679ed0d | 113 | + _classified\ |
| fileparser_funcs\ | FileParser（rootfs_1） | f4d8cabd | 42 | + _classified\；BMU 解析/烧写流程 |
| ko_bitmain_axi\ | bitmain_axi.ko（rootfs_1） | 5d00a362 | 5 | init_module / cleanup_module / axi_fpga_dev_open / axi_fpga_dev_mmap / axi_fpga_dev_release —— /dev/axi_fpga_dev mmap 物理窗口 0x40000000 |
| ko_fpga_mem\ | fpga_mem_driver.ko（rootfs_1） | fc136134 | 5 | init_module / cleanup_module / open / mmap / release —— /dev/fpga_mem DDR 窗口（工作下发用） |
| tables\ | — | — | — | 常量表/配置事实（见 4.4） |
| protocols\ | — | — | — | stratum_strings.txt（103 条：地址 → 符号名，如 gen_stratum_work @0x55888） |
| _classified\ | cgminer | 709cc6f0 | — | 00_INDEX.md（644 函数分类索引）+ function_map.txt（644 行 TSV：函数地址 → 类别） |

合计 644+644+165+140+113+42+5+5 = **1758 个 .c**。其余未逐一详述的函数请查：reversed\cgminer\00_INDEX.md、reversed\_classified\function_map.txt、各 _classified\function_map.txt、reversed\cgminer\_func_index.txt。

### 4.2 cgminer 16 功能类分布（reversed\cgminer\）

| 子目录 | 职能 | 函数数 |
|---|---|---|
| 10_uart_protocol | 串口收发与 FIFO（PL AXI UART 硬件层） | 11 |
| 12_freq_pll | 频率/PLL/扫频 | 99 |
| 13_voltage_pic | 电压/PIC16F1704 电源管理 | 39 |
| 14_temp_fan | 温度传感器与风扇 PWM | 18 |
| 15_fpga_axi | FPGA/AXI 寄存器窗口 | 10 |
| 16_chain_mgmt | 链复位/寻址/波特率/寄存器写 | 44 |
| 20_job_mining | 挖矿任务构造/下发/nonce 上报 | 162 |
| 21_pool_net | 矿池连接/stratum/socket | 61 |
| 22_json_rpc | JSON-RPC 解析与构造 | 6 |
| 30_api_status | API/状态统计（cgminer-api） | 46 |
| 32_i2c_sensor | I2C 传感器访问（TMP451/AT24C02D 等） | 3 |
| 40_config | 配置文件解析 | 17 |
| 41_logging | 日志系统 | 9 |
| 50_crypto_hash | SHA256/哈希算法 | 12 |
| 60_util_util | 通用工具（队列/线程/内存） | 35 |
| 70_no_strings | 无字符串特征叶函数 | 72 |

### 4.3 核心：单芯片固件五阶段关键函数表

每行：函数地址/语义名 → .c 路径（均在 reversed\cgminer_funcs\）→ 执行的操作。

#### 阶段一：初始化

| 函数 | .c 路径 | 执行的操作 |
|---|---|---|
| FUN_00049cc8 `bitmain_ZCASH_init` | reversed\cgminer_funcs\FUN_00049cc8.c | 主初始化序：mmap/init_fpga → 设 PL 波特率 bt8d=0x1A(132) → PIC 复位+心跳保活 → PIC 断电/上电链 → tty_init → set_baud → 软件设址 → check_asic_num/recheck_asic_num → 写 ticket mask/timeout/频率 → 启动 4 个监控线程 |
| FUN_0004791c | reversed\cgminer_funcs\FUN_0004791c.c | 初始化 13 步序列定义（check_asic_reg 族入口；重 init 时复用） |
| FUN_00042f7c `software_set_address_chain` | reversed\cgminer_funcs\FUN_00042f7c.c | 链设址：先发复位帧 `55 AA 53 05 00 00 chk`，再逐芯片发设址帧 `55 AA 40 05 addr 00 chk`（addr 间隔 42） |
| FUN_00046434 `reset_chain` | reversed\cgminer_funcs\FUN_00046434.c | 链复位：FPGA 寄存器 base[0x34] 置 1<<chain 拉低 → sleep → 清零释放；尾部内含切高速（写 reg 0x1C、bt8d=0），**先于设址执行** |
| FUN_000435e0 `set_frequency_chain` | reversed\cgminer_funcs\FUN_000435e0.c | 写 PLL 寄存器 0x0C：在表 0x69204（115 条×16B）中按目标频率线性查找，命中取槽位 pll 字段经 FUN_0003cdcc 构 11B 帧下发；**未命中默认取槽位 0x40（587MHz，0x005E0221）**（经反编译直接复核，与 BM1746_uart_register_ops.md 一致） |
| FUN_000498f4 | reversed\cgminer_funcs\FUN_000498f4.c | 芯片计数探测/扫频线程创建入口（check_asic_num 侧；探测链路 FUN_000498f4 → FUN_0004791c） |

#### 阶段二：任务下发

| 函数 | .c 路径 | 执行的操作 |
|---|---|---|
| FUN_0001acc4 `gen_stratum_work` | reversed\cgminer_funcs\FUN_0001acc4.c | 生成 140B 工作头（work+0xe0）：pool 模板 0x644 + merkle 0x14c + nonce2 0x15e 拼装；workid 取全局计数 DAT_000770a8 |
| FUN_00042b54 | reversed\cgminer_funcs\FUN_00042b54.c | 任务下发线程：帧[3]=workid&0x7f、帧[2]恒 0x20；128 槽映射表按完整 workid 索引（回绕覆盖）；每帧间 500us 节流 |
| FUN_00042a60 `bm1740_makeup_work` | reversed\cgminer_funcs\FUN_00042a60.c | 任务帧 CRC16 追加：查表 @0x68aec，覆盖字节 [2..0x8F]，结果大端写入 @0x90（任务帧总长 146B = 0x92） |
| FUN_0003cdcc | reversed\cgminer_funcs\FUN_0003cdcc.c | 11B 命令帧构造：`55 AA` + 类型字节 + byte[3]=9 + 32bit 值（大端）+ 5bit 校验 |

#### 阶段三：nonce 接收

| 函数 | .c 路径 | 执行的操作 |
|---|---|---|
| FUN_0004b0c0 | reversed\cgminer_funcs\FUN_0004b0c0.c | RX 分发：帧类型 0xE_ → 93B nonce 包；0xCC → pm_handle；其它 → reg_handle |
| FUN_0004a754 `nonce_handle`/`bm1740_verify_nonce_integrality` | reversed\cgminer_funcs\FUN_0004a754.c | 0x5D nonce 包解析：chipid=addr/42；16×91B 子块逐块 CRC16（@+0x59，大端）校验；workid 经 base+workid*4+0x76 映射表取回并与帧内比对；合法 nonce 经 0x54D 记录入容量 100 的 FIFO |

#### 阶段四：Equihash 校验（host 侧做）

| 函数 | .c 路径 | 执行的操作 |
|---|---|---|
| FUN_0003af90 `bitmain_scanhash` | reversed\cgminer_funcs\FUN_0003af90.c | scanhash 主循环（兼统计出口）：调 FUN_0004eee0 生成 (200,9) Equihash 参数解 → FUN_00054f8c 组 140B → FUN_00055930 双 SHA256 预筛 → 命中后 FUN_0004eaf4 全量验证 → FUN_00025e28/000256a4 提交 share |
| FUN_0004eee0 | reversed\cgminer_funcs\FUN_0004eee0.c | Equihash (200,9) 求解器 |
| FUN_00054f8c | reversed\cgminer_funcs\FUN_00054f8c.c | 组装 140B 校验输入块 |
| FUN_00055930 | reversed\cgminer_funcs\FUN_00055930.c | 双 SHA256 快速预筛 |
| FUN_0004eaf4 | reversed\cgminer_funcs\FUN_0004eaf4.c | Equihash 解全量验证 |
| FUN_00025e28 / FUN_000256a4 | reversed\cgminer_funcs\FUN_00025e28.c、FUN_000256a4.c | share 提交（入队/上报） |

#### 阶段五：矿池（stratum 客户端）

| 函数 | .c 路径 | 执行的操作 |
|---|---|---|
| FUN_0002b8e8 `initiate_stratum` | reversed\cgminer_funcs\FUN_0002b8e8.c | 3 种 subscribe 变体协商 + extranonce 订阅 |
| FUN_0002ea38 `authorize` | reversed\cgminer_funcs\FUN_0002ea38.c | mining.authorize 授权 |
| FUN_0002cf40 `parse_notify_equihash` | reversed\cgminer_funcs\FUN_0002cf40.c | mining.notify（Equihash 变体）解析入队 |
| FUN_00021fe8 | reversed\cgminer_funcs\FUN_00021fe8.c | mining.submit 构造：[worker, job_id, ntime, nonce2, solution（3B 前缀 + 1344B 索引）] |
| FUN_0002b5c8 / FUN_0002b040 | reversed\cgminer_funcs\FUN_0002b5c8.c、FUN_0002b040.c | socket send / recv |
| FUN_0002aee0 / FUN_00018ca0 / FUN_00021850 | reversed\cgminer_funcs\FUN_0002aee0.c、FUN_00018ca0.c、FUN_00021850.c | 断线重连/恢复链路 |
| FUN_00019bf8 `switch_pools` | reversed\cgminer_funcs\FUN_00019bf8.c | 矿池切换 |

#### 支撑函数

| 函数 | .c 路径 | 执行的操作 |
|---|---|---|
| FUN_0003cc6c | reversed\cgminer_funcs\FUN_0003cc6c.c | 5bit checksum：4-lane XOR（任务帧帧尾→0x0A） |
| FUN_00045ec0 / FUN_00045d88 / FUN_00045b50 | reversed\cgminer_funcs\ 同名 .c | 温控/风扇 PWM 三件套（目标温度、PWM 占空比、阈值联动） |
| FUN_0003d000 .. FUN_000414d8（PIC 族） | reversed\cgminer_funcs\FUN_0003d000.c 等 39 个 | PIC16F1704 电源管理：`[55 AA len cmd args]`+回显；0x10=电压（0x15 偏置）、0x15/1A/1B=DC-DC 开关、0x06=loader→app、0x07=PIC 复位、心跳保活；**自制供电方案可整体跳过** |
| FUN_0004791c / FUN_00049700 | reversed\cgminer_funcs\ 同名 .c | 重 init 入口（运行期重新走初始化） |
| FUN_00019fc8 | reversed\cgminer_funcs\FUN_00019fc8.c | workid 超时管理（超时任务槽位回收） |

### 4.4 tables\（重写固件直接可用的数据）

| 文件 | 内容 |
|---|---|
| freq_table_full.csv | BM1746 频率表 115 条（100~900MHz：slot/freq/field_b/field_c/pll_value），已与二进制 0x69204 表逐条比对一致 |
| pll_decode.md | PLL 公式（115/115 验证）：fout = 25MHz × N/(P×Q)，N=pll[31:16]、P=pll[7:4]、Q=pll[11:8]、bit0=1；800MHz=0x00800221 |
| topol.conf | 拓扑：chain_num=3、chain_asic_num=6、asic_addr_interval=42、chip_type 0x1746、AWP12 电源 1.2V、PIC pic1704、TMP451、AT24C02D |
| cgminer.conf.factory.sd / .nand | 出厂配置（SD：freq=800/volt=780；NAND：freq=0→走 800 默认，level=100） |
| known_constants.md | I2C 每 ASIC 地址表 0x76b48（前 16 字节实测值）、PIC 全命令函数名表 |
| function_name_catalog.txt | 208 个驱动函数自报名（`__func__` 字符串）——重写时的 API 清单 |
| subtype | "zynq7007_BEZ36501"（机器码校验用） |

### 4.5 缺口资产处理状态（2026-11-20 更新，均不阻塞挖矿链）

此前“未反编译/未分析”资产已按可行性完成处理：辅助工具做静态逆向说明、ko/FileParser 做版本对比、vmlinux/jansson 提取元信息、DTB 完成转储与差异分析。未逐函数反编译的不影响单颗 BM1746 挖矿主链路二次开发。

| 资产 | 出处 / 关键版本 | 处理结果 | 产物 |
|---|---|---|---|
| cgminer-api | rootfs_1 `41cd93a7` / sd `3ad30016` | 静态分析完成：本地 cgminer TCP API CLI（默认 127.0.0.1:4028）；服务端逻辑由 cgminer 30_api_status 覆盖；不阻塞 | reversed\aux_tools\cgminer-api.md、00_INDEX.md |
| monitor-ipsig | rootfs_1 `1a08e517` / sd `3a109798` | 静态分析完成：UDP IP/MAC 上报与按键/GPIO 监控守护（S65）；非算力路径 | reversed\aux_tools\monitor-ipsig.md |
| monitor-recobtn | rootfs_1 `794be2a6` / sd `11daa075` | 静态分析完成：恢复按键监控触发 factory reset（S66）；非算力路径 | reversed\aux_tools\monitor-recobtn.md |
| antlogin | rootfs_1 `fb2a152f` | 静态分析完成：固定 `login -f root` 登录辅助；仅影响维护通道 | reversed\aux_tools\antlogin.md |
| update-daemon | rootfs_1 `788a2031` | 静态分析完成：127.0.0.1:22322 IPC，调用 `updateporc.sh`；兼容原厂升级链路时需等价实现，但不阻塞核心挖矿 | reversed\aux_tools\update-daemon.md |
| id2mac | rootfs_1 `a380ac2d` / sd `dcfa656b` | 静态分析完成：派生并输出 MAC（S38network）；可替换 | reversed\aux_tools\id2mac.md |
| rootfs_sd 版 bitmain_axi.ko / fpga_mem_driver.ko | sd `f2517c6f` / `990edb34`（对照 rootfs_1 `5d00a362` / `fc136134`） | 版本对比完成：同源重编译，设备名/mmap/open-release-init-exit 与 rootfs_1 一致，无 ioctl 线索；vermagic/工具链不同不可混装；通常无需补反编译 | analysis\gap_reports\ko_fileparser_diff.md，参考 reversed\ko_bitmain_axi\、reversed\ko_fpga_mem\ |
| FileParser（rootfs_2） | rootfs_2 `56358ea4`（对照 rootfs_1 `f4d8cabd`） | 版本对比完成：`-s/-n/-q` 与 Update.sh 升级路径一致，rootfs_1 多 miner_type/hash 校验字符串；通常无需优先补反编译 | analysis\gap_reports\ko_fileparser_diff.md，参考 reversed\fileparser_funcs\ |
| devicetree.dtb（parts/sdcard） | parts `ad826c24` / sd `6b288d49` | 解析与对比完成：UART0 0xe0000000/115200、MTD 分区差异、PS 外设映射；不含 BM1746 ASIC 直驱所需细节 | analysis\dtb\*.dts.txt、analysis\gap_reports\devicetree_analysis.md |
| vmlinux.bin | analysis `6c3ef57d` | 元信息提取完成：Linux 4.6.0-xilinx-g03c746f7（2026-04-27）；ko 语义已覆盖，完整反编译优先级低 | analysis\gap_reports\vmlinux_jansson_info.md |
| libjansson.so.4.5.0 | sdcard/rootfs_sd `44278ad4` | 元信息提取完成：标准 jansson API（soname .4），通用第三方依赖无需反编译 | analysis\gap_reports\vmlinux_jansson_info.md |

---

## 5. bm1746\（核心报告与表）

| 文件 | 内容 | 状态 |
|---|---|---|
| BM1746_uart_register_ops.md | 串口帧格式/寄存器码全表（0x0C=PLL、0x14=ticket mask（出厂 0x10）、0x1C=baud/温度使能、0x20=I2C 间接、0x28=核心超时（默认 84000）、0x40=模拟 MUX、0x50=nonce 位移）/PIC 命令/频率规则（配置 0/800/缺省→800；扫频线程 775~815 起步二分逼近，判据为 nonce 错误率+OX 状态，结果写 /config/chips_freq.config 合法窗 [775,900]）/PL 窗口寄存器（0x80=init 0x8080800F、0x34=链复位、0x3C=波特率、0x180+4n=TX FIFO、0x1F0+4n=RX 计数）/PS 级要点 | 2026-11-20 已修订（5 处字节级修正，见第 6 节） |
| ESP32_single_chip_feasibility.md | 单芯片（ESP32）直驱 BM1746 可行性核验：已核验的 9 类协议要素（命令帧/双校验/寄存器码/波特率/寻址/任务帧 0x92B/nonce 回包/初始化序列/芯片计数探测）全部有反编译函数支撑 + 5 个必实现模块 + 已知缺口 + 最小固件骨架伪码 | 已修订 |
| freq_table.txt | 115 条频率表（freq_MHz → pll_value，写 reg 0x0C）；与 reversed\tables\freq_table_full.csv 同源 | 已比对一致 |
| cgminer_all_decomp.c | cgminer 644 函数全量单文件反编译（1.5MB，检索用；分类版在 reversed\cgminer\） | 基准 md5 对应 |

---

## 6. 字节级验证状态表

| 验证项 | 结果 | 说明 |
|---|---|---|
| CRC16-CCITT (0x1021, 0xFFFF) 查表 | PASS 256/256 | 表 @0x68aec 全表复算一致（reversed\cgminer_funcs\FUN_00042a60.c） |
| 频率表 | PASS 115/115 | 表 0x69204 逐条比对；800MHz → 0x00800221 |
| TX 任务帧 | PASS | 146B（0x92），CRC16 大端 @0x90，5bit 帧尾 @0x0A，帧[2]恒 0x20、帧[3]=workid&0x7f |
| RX 0x5D nonce 包 | PASS | 93B 总长；16×91B 子块，逐块 CRC16 大端 @+0x59；workid 经 base+workid*4+0x76 取回 |
| 5bit checksum 重实现 | PASS | 4-lane XOR（FUN_0003cc6c） |
| RX 前导 | 已复核 | **RX 字节流前导实测为 AA 55（FPGA FIFO 字序），与 TX `55 AA` 相对** |
| 真实 ASIC 帧语料 | 缺失 | analysis\out\pcap_packet_log.txt 全为 FPGA 位流 Type1 配置帧，无真实 ASIC 通信帧——协议结论全部来自反编译推证而非抓包 |
| 文档修正 | 5 处 | 2026-11-20 修正：① 设址帧字节（`55 AA 53 05 00 00 chk`/`55 AA 40 05 addr 00 chk`）② 0x41/0x51 语义对调（0x51=链广播写 rw=1、0x41=带芯片地址读）③ workid 帧[3]&0x7f ④ 初始化步序/timeout 84000/ticket 0x10（切高速在 reset_chain 内部、先于设址）⑤ RX 前导 |

---

## 7. 二次开发用途映射（单芯片重写路线）

### 7.1 五阶段 → 所需文件映射

| 阶段 | 目标 | 所需文件（全部已在工作区） |
|---|---|---|
| 1. PS 启动底座 | FSBL 重写（ps7_init + PCAP 加载） | reversed\fsbl_funcs\、analysis\out\ps7_init_full.txt（540 条）、analysis\out\ps7_final.json、analysis\out\ASIC_INIT_REPORT.md、parts\Zynq7007_miner.elf |
| 2. PL 固件 | 位流提取/重生成（7z007s） | parts\Zynq7007_bitstream.bin、parts\Zynq7007_miner.bit.bin、analysis\out\pcap_regs.json |
| 3. 内核+驱动 | bitmain_axi.ko / fpga_mem_driver.ko 重写（各仅 5 函数，语义已明） | reversed\ko_bitmain_axi\、reversed\ko_fpga_mem\、rootfs_1\lib\modules\*.ko（5d00a362/fc136134） |
| 4. ASIC 驱动层 | 串口帧/PLL/PIC/链管理（单芯片直驱的核心） | reversed\cgminer\10~16 类、reversed\tables\、bm1746\BM1746_uart_register_ops.md、bm1746\ESP32_single_chip_feasibility.md |
| 5. 业务+矿池 | 任务下发/nonce 回收/stratum/API | reversed\cgminer\20/21/22/30 类、reversed\protocols\stratum_strings.txt、reversed\cgminer\50_crypto_hash（Equihash host 校验）、rootfs_1\etc\cgminer.conf.factory |

### 7.2 单芯片直驱需改写的 AXI/板级函数清单

单芯片方案没有 Zynq PL FPGA，凡是走 mmap /dev/axi_fpga_dev 窗口或 PIC 电源板的函数都要换实现：

| 函数 | .c 路径（reversed\cgminer_funcs\） | 现实现 | 单芯片改写方向 |
|---|---|---|---|
| FUN_0003c150 | FUN_0003c150.c | TX FIFO 写（base[0x180+4×grp]，32 位字序交换后写） | 改为直写目标 UART/软串口 |
| FUN_0003c5e8 / FUN_0003bb78 / FUN_0003bd44 | 同名 .c | RX 计数读取与取数（base[0x1F0..0x200]） | 改为直读 UART RX |
| FUN_0003c0a0 / FUN_0003bf0c | 同名 .c | RX FIFO 读出侧封装 | 同上，去 FPGA 字序交换 |
| FUN_0003b704 / FUN_0003ba70 | 同名 .c | mmap /dev/axi_fpga_dev + /dev/fpga_mem；init_fpga 写 base[0x80]=0x8080800F、base[0]\|=0x60000000 | 单芯片无 PL：删除或替换为本地 UART 初始化 |
| FUN_0003bb44 | FUN_0003bb44.c | set_baud_pl：base[0x3C]=bt8d 四链各 8bit | 改为配置本机 UART 分频（bt8d=0 的主机侧波特率见 7.5 未决项） |
| FUN_0003c66c / FUN_0003c708 | 同名 .c | FPGA 命令通道（base[0x30] 写命令/读回，等 bit31 清零） | 单芯片无命令通道：删除或映射到本地状态寄存器 |
| FUN_00046434 | FUN_00046434.c | 链复位走 FPGA base[0x34]=1<<chain | 改为 GPIO 拉低/释放复位脚；尾部切高速（reg 0x1C bt8d=0）保留 |
| FUN_00045d88 / FUN_00045ec0 / FUN_00045b50 | 同名 .c | 风扇 PWM（经 FPGA/PIC） | 改为本地 PWM 输出 |
| FUN_000445e8 | FUN_000445e8.c | RX 收包线程骨架 | 保留线程模型，换数据源 |
| PIC 族（FUN_0003d000..000414d8） | 同名 .c | PIC16F1704 供电管理 | 自制供电可整体删除 |

### 7.3 可直接复用的纯协议函数（不经 AXI 窗口，原样移植）

| 类别 | 函数 |
|---|---|
| 构帧/校验 | FUN_0003cdcc（11B 命令帧）、FUN_0003cc6c（5bit checksum）、FUN_00042a60（任务帧 CRC16@0x90 大端） |
| 初始化语义 | FUN_00042f7c（设址帧序列）、FUN_000435e0（PLL 查表写 0x0C）、FUN_0004791c（13 步定义） |
| 任务链路 | FUN_0001acc4（gen_stratum_work）、FUN_00042b54（下发线程语义：帧[3]=workid&0x7f、帧[2]=0x20、128 槽映射+回绕、500us 节流）、FUN_00019fc8（workid 超时） |
| nonce 解析 | FUN_0004b0c0（RX 分发）、FUN_0004a754（0x5D/93B/91B 子块解析） |
| Equihash 校验 | FUN_0003af90 → FUN_0004eee0 / FUN_00054f8c / FUN_00055930 / FUN_0004eaf4 / FUN_00025e28 / FUN_000256a4 |
| 矿池 | FUN_0002b8e8、FUN_0002ea38、FUN_0002cf40、FUN_00021fe8、FUN_0002b5c8/0002b040、FUN_0002aee0/00018ca0/00021850、FUN_00019bf8 |
| 数据表 | reversed\tables\freq_table_full.csv（115 条）、pll_decode.md 公式、known_constants.md |

### 7.4 裁剪注意

| 事项 | 说明 |
|---|---|
| cgpu 注册 | cgminer 仅注册 1 个 cgpu：FUN_0003a940 `bitmain_ZCASH_detect`（reversed\cgminer_funcs\FUN_0003a940.c）——单芯片版本天然匹配，无需改动 detect 框架 |
| chain 槽位 | 代码中 chain 状态数组 DAT_0009d4bc 为 4 槽（FUN_0003bb44 波特率寄存器也按 4 链×8bit 组织）；单芯片裁为 1，遍历/波特率/复位均按单链索引 |
| pools 与 chain 解耦 | pools[] 结构与 chain 无关，矿池层裁剪不影响驱动层 |
| topol.conf 参照 | 出厂拓扑 chain_num=3×chain_asic_num=6、asic_addr_interval=42（chip_type 0x1746）；单芯片即 chain_num=1、asic_num=1，设址帧 addr 用 0，interval 逻辑可留作兼容 |
| PIC/温控 | 自制供电跳过 PIC 族后，电压/心跳相关返回值需在 init 序中短路（FUN_00049cc8 主序中 PIC 断/上电段） |

### 7.5 未决事项

| 事项 | 影响 | 状态 |
|---|---|---|
| bt8d=0 时的主机侧波特率 | FUN_00046434 尾部切高速写 reg 0x1C bt8d=0 后，BM1746 芯片端高速波特率值未从主机代码侧推出 | 需上机实测（硬件环境缺口，工作区内无法闭环） |
| 0x5B 子块 bit 级遥测 | RX 帧中 0x5B 类型子块的逐 bit 含义未解析 | 不影响 nonce 路径（nonce 走 0x5D/93B），留待后续 |

---

## 附：与既有索引的关系

- 每函数级速查：reversed\cgminer\00_INDEX.md（带相对链接）、reversed\_classified\function_map.txt（644 行 TSV）、reversed\cgminer\_func_index.txt / _callgraph.txt / _string_xref.txt
- 辅助工具静态分析：reversed\aux_tools\00_INDEX.md（cgminer-api、monitor-ipsig、monitor-recobtn、antlogin、update-daemon、id2mac）
- DTB 转储与缺口报告：analysis\dtb\*.dts.txt、analysis\gap_reports\devicetree_analysis.md、analysis\gap_reports\ko_fileparser_diff.md、analysis\gap_reports\vmlinux_jansson_info.md
- 方法论与解包细节：METHODOLOGY.md、SUMMARY.md（BMU 容器格式 §一~§九）
- 协议细节（帧格式/PIC 命令/频率规则）：bm1746\BM1746_uart_register_ops.md
- 单芯片可行性论证：bm1746\ESP32_single_chip_feasibility.md

## 8. rewritten\（语义化 C 全量转写，1114 函数）

对 `reversed\` 全部 6 个二进制（7 产物）1,114 个函数的语义化 C 转写，100% 完成落盘。
规则见 `rewritten\CONVENTIONS.md`；总索引见 `rewritten\README.md`。

### 8.1 完成度

| 目录 | 函数 | 行数 | 说明 |
|---|---|---|---|
| rewritten\cgminer\ | 644 | 38,670 | 33 个模块文件（uart/axi/i2c/pic/fan/chain/freq×4/job×9/pool×3/json/api×3/cfg/log/util/hash/nostr） |
| rewritten\fsbl\ | 165 | 9,936 | p1a-p4 + ps7_init.c（540 条 PS 寄存器序列, 由 analysis\out\ps7_final.json 生成） |
| rewritten\uboot\ | 140 | 6,297 | p1a-p3b（另 5 条仅索引无源条目豁免, 记录于 coverage.py） |
| rewritten\monitor\ | 113 | 5,008 | cjson.c + monitor_core_p1/p2 |
| rewritten\fileparser\ | 42 | 1,329 | farmhash na::Hash64 (2172 golden 向量验证通过) |
| rewritten\ko\ | 10 | 420 | bitmain_axi.c + fpga_mem.c |
| rewritten\include\ | — | 241 | common_types.h / bm1746_regs.h / fpga_axi.h |
| rewritten\name_map\ | 1114 | — | FUN_地址→语义名→依据 冻结事实源（6 个正式 tsv + 分片过程件） |

### 8.2 校验状态

- 覆盖率脚本 `tools\coverage.py all`：names 6/6 OK、transcribed 6/6 OK（1114/1114）、Ghidra 残留 0。
- gcc 语法冒烟：53 个翻译单元全部通过（`-fsyntax-only -std=gnu99`；MinGW 桩头
  `analysis\tmp_syntax_stubs\` 仅服务语法检查——POSIX/内核/openssl/curses 头缺失补桩 +
  prelude 注入 glibc 符号；`-DNO_OLDNAMES` 屏蔽 MinGW 1 参 mkdir）。
- 机器码级抽查：BLAKE2b（gcc 8192/8192 + 已知答案）、SHA-256（NIST 向量）、scrypt（hashlib
  对拍）、farmhash（2172 golden）、CRC16、__udivsi3（capstone 裁决）、4-bit lane 校验和
  （3000 随机码流 vs 寄存器模型）等，逐批次详见各文件头部注释与批次报告。
- 阶段 3 冒烟中修复的真实转写缺陷约 30 处（注释内 `*/` 早闭、do-while 作用域、跨文件签名
  不一致、DAT_ 缺 extern、fsbl p2b/p4 部分实参调用形态、fat_find_volume/rsa_mont_mul 按
  反编译重对齐等），全部修正后三关复绿。

### 8.3 工具链

| 工具 | 用途 |
|---|---|
| tools\coverage.py | names/transcribed/remnants 三模校验（横幅配对 + Ghidra 残留扫描） |
| tools\merge_name_maps.py | cgminer 9 分片命名合并（__func__>anchor>logstring 优先级） |
| tools\batch_plan.py | 按类别+≤2500 行装箱生成 27 个转写批次 |
| tools\gen_ps7_init_c.py | ps7_final.json → ps7_init.c 确定性生成 |

### 8.4 已知边界

- 不保证可链接：纯数据 blob（opt_config_table、ctype 表等）以 extern 引用未转写数据段。
- 反编译体受损函数（halt_baddata 占位）按原样转写为挂死入口，不虚构语义。
- Ghidra 丢失实参的调用点以"部分原型函数指针"形态保留并注明寄存器未定值（不虚构补参）。
