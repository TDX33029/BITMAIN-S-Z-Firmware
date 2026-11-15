BMU 固件解包报告 — FR-1.9(260427-Z15 Pro).bmu
================================================
文件: FR-1.9(260427-Z15 Pro).bmu  (19,333,763 字节 / 0x1270283)
设备: Antminer Z15 Pro (机器码 BEZ36501, Equihash 矿机)
主控: Xilinx Zynq-7007S (xc7z007s), NAND 启动

一、容器格式 (BMU / Bitmain Update)
  0x000  BMU 头: 版本 0x0126, 日期 "20260427", RSA-2048 根公钥 (PEM, 0x1D..0x1DB)
  0x410  256B 签名 (miner.pem.sig), 0x518 起: 成员表 (count=7, 每项 [u8 idx][u32 BE size])
  0x800  成员数据区, 末尾 2048B 随机数据 (footer)

二、成员表 (0x51D 起, 7 项)
  [0] BOOT.bin          0x2A8CC0  @0x000800
  [1] devicetree.dtb    0x001F4E  @0x2A94C0
  [2] uImage            0x3DE8E8  @0x2AB40E
  [3] minerfs.image.gz  0x7CFD8F  @0x689CF6
  [4] update.image.gz   0x4156CA  @0x0E59A85
  [5] crl.tar.gz        0x000228  @0x126F14F
  [6] miner.btm.tar.gz  0x00070C  @0x126F377

三、BOOT.bin 内部 (Xilinx bootgen, FSBL 内嵌 BIF 描述)
  FSBL 分区表 @rel 0xC80: 3 个分区
    PH0 id 0x240: FSBL           data @rel 0x05C0  size 0x8004 (33KB)
    PH1 id 0x250: Zynq7007_miner.elf  data @rel 0x8780  size 0x7F2E8 (521KB, ARM 代码+内嵌位流 @0x21E30)
    PH2 id 0x260: Zynq7007_miner.bit  data @rel 0x87C20  size 0x22553 (140KB, 加载地址 0x4000000=PL)
  描述符 (dword 反序存储): "Zynq7007_miner.elf" / "Zynq7007_miner.bit" / "u-boot.elf"
  u-boot 区: rel ~0x27C000..0x2A8CC0 (U-Boot 2016.07-g4c04e6f-dirty), 自带 dtb (xlnx,zynq-microzed)

四、devicetree.dtb (内核用)
  compatible: xlnx,zynq-7000
  ps7-nand MTD 分区: BOOT.bin-dts-marker-kernel 0x0+0x2800000 / ramfs 0x2800000+0x2000000 /
  configs 0x4800000+0x800000 / sig 0x5000000+0x200000 / reserve1 / upgrade-ramfs 0x6000000+0x1000000 /
  upgrade-file 0x7000000+0x3800000 / reserve2 0xA800000+0x5800000

五、uImage
  Linux-4.6.0-xilinx-g03c746f7, zImage 未压缩, load/entry 0x8000, CRC 校验通过

六、minerfs.image.gz (主 rootfs, 32MB ext2)
  /etc/subtype = "zynq7007_BEZ36501"   <-- 明确标注 zynq7007
  /etc/topol.conf: machine BEZ36501, processor.type ZYNQ7007
  /etc/init.d/S70cgminer -> /usr/bin/godminer (本包未含该二进制, 由升级分区/现场组件提供)
  编译时间: Mon Apr 27 23:35:40 CST 2026, "Antminer Z15 Pro / Equihash"

七、update.image.gz (升级用 ramfs, 32MB ext2)
  /etc/init.d/Update.sh: 解析 update.bmu, 校验签名后烧写 NAND:
    mtd0=BOOT.bin(3 份备份)+dtb+uImage, mtd1=minerfs, mtd3=sigImage, mtd5=update fs
  工具: /usr/bin/FileParser (-s/-f/-p/-n/-x/-q), openssl 签名校验
  FileParser 字符串确认 BMU 成员名与 miner_type 校验机制

八、crl.tar.gz / miner.btm.tar.gz
  crl (10KB) + crl.sig; miner.btm (2KB) + miner.btm.sig (miner.btm = MAC OS X AppleDouble 包装的二进制)

九、关于 "是否含针对 zynq7007 的分包固件"
  结论: 本 .bmu 整包即为 Zynq-7007S 专用固件, 无 7020 双版本。
  证据:
  1) BOOT.bin 的 FSBL BIF 描述符明确列出 "Zynq7007_miner.elf" / "Zynq7007_miner.bit"
  2) 主 rootfs /etc/subtype = "zynq7007_BEZ36501"; topol.conf processor.type = "ZYNQ7007"
  3) 全文件检索无任何 "Zynq7020"/"7z020" 固件组件 (仅 u-boot 通用芯片名列表含 7z020)
  4) 7z007s 位流尺寸 (约 254KB) 与 PL 分区一致
  如需 7020 版本, 应为另一个独立的 .bmu 包, 不在本文件内。

十、SD 恢复镜像与 BM1746 目标频率 (补充分析)
  SD 镜像: ANTMINER-Z15-user-800M-202007031139_6134 (V0.1.18, 2020-07-03)
  平台判定: Zynq7000 ✓ / cgminer git branch "bm1746" (chip_type 0x1746) => 确认 BM1746 平台
  目标频率: 800 MHz, 证据闭环:
    1) 镜像文件名 "user-800M"
    2) cgminer.conf.factory: "bitmain-freq":"800", "bitmain-voltage":"780"
    3) 扫频线程入口 FUN_00047354 首句 FUN_00043e4c(800): 对全部在位链
       set_frequency_chain(chain,800) (reg 0x0C 广播帧; 扫频窗口 775..815 MHz, 基准 800)
    4) 配置兜底: bitmain-freq 为 0/800/未配置 => 默认 800 (0x32000 = 800<<8)
    5) FR-1.9 NAND 包 freq=0 + freq-level=100 走同一 800M 默认分支
  频率写入口: UART 广播帧 reg 0x0C, PLL 值查 0x69204 频率表 (800MHz=0x00800221)
  电压路径: bitmain-voltage=780 => PIC 命令 0x10 (0x15 偏置编码) + 每 ASIC reg 0x20
           I2C 间接写 ISL 电源芯片 (合法窗 [700,800]mV, 越界回落 700)
  详细见: bm1746/BM1746_uart_register_ops.md (帧格式/寄存器码/PIC 命令/完整序列)

十一、最终产物索引 (整理后)
  SUMMARY.md                              本文件
  reversed/README.md                      ★ 逆向总索引 (全部函数按功能分类 + 重写路线图)
  reversed/cgminer/                       cgminer 644 函数 × 16 功能类 (每函数一文件)
  reversed/fsbl_funcs/                    FSBL 165 函数 (ps7_init/PCAP/NAND)
  reversed/fileparser_funcs/              FileParser 42 函数 (BMU 解析烧写)
  reversed/monitor_funcs/                 miner-monitor 112 函数 (监控守护)
  reversed/uboot_funcs/                   U-Boot 140 函数 (基址 0x40000000)
  reversed/ko_bitmain_axi/ ko_fpga_mem/   两个内核模块 (各 5 函数)
  reversed/tables/                        频率表/PLL公式/I2C表/拓扑/出厂配置/函数名目录
  reversed/protocols/                     stratum 协议字符串
  bm1746/BM1746_uart_register_ops.md      BM1746 寄存器/帧/频率/电压完整报告 (核心交付)
  bm1746/cgminer_z15_bm1746.elf           SD 镜像恢复的 cgminer (422244B, md5 709cc6f0..)
  bm1746/cgminer_all_decomp.c             cgminer Ghidra 全量反编译 (644 函数)
  bm1746/freq_table.txt                   BM1746 频率表 115 条 (100..900MHz)
  analysis/out/ASIC_INIT_REPORT.md        FSBL/PS 侧初始化分析 (ps7_init 540 条)
  analysis/out/ps7_init_full.txt          ps7_init 寄存器表全文 (亦见 ps7_final.json)
  analysis/out/all_decomp.c               FSBL (Zynq7007_miner.elf) 反编译
  analysis/out/pcap_regs.json             PL 位流 PCAP 寄存器写解析 (亦见 pcap_packet_log.txt)
  parts/                                  BMU 成员原件 (BOOT.bin/dtb/uImage/*.image.gz...)
  rootfs_1/ rootfs_2/                     minerfs / update-ramfs 解包
  sdcard/                                 SD 恢复镜像原始成员 (BOOT.bin/dtb/uImage/uramdisk...)
  sdcard/rootfs_sd/                       SD uramdisk ext2 解包 (含 cgminer.conf.factory)
