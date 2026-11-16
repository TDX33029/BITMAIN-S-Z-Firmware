BMU 固件 ASIC 初始化分析报告 (Z15 Pro / Zynq-7007S)
================================================================
分析对象: FR-1.9(260427-Z15 Pro).bmu 解包产物
工具: Ghidra 12.2 (headless) + 自研表解析
产物目录: analysis/out/

一、启动与初始化代码的分布 (证据链)
------------------------------------------------------------------
1) BOOT.bin 成员内 (analysis/fsbl_ph1_raw.bin, 521KB):
   = Bitmain 定制 FSBL ("Zynq7007_miner.elf", 2021-05-25, Xilinx FSBL 2016 基线)
   - main() @0xE138: 读 SLCR_BOOT_MODE (0xF800025C), NAND 启动 (boot mode==4)
   - ps7_init 配置表 @0x10234..0x125CC (540 条, 含 3 次重复调用)
   - 内嵌 PL 位流 @0x21E30 (7z007s, 254KB, PCAP 字节序)
   - FsblHookBefore/AfterBitstreamDload 等钩子字符串存在但无调用 (空钩子, 原厂样板)
2) bitmain_axi.ko (rootfs_1/lib/modules, 7.4KB ELF reloc):
   - init: request_mem_region+ioremap(0x40000000, 0x1400)  => PL AXI 寄存器窗口
   - fops: open/release/mmap; mmap 用 remap_pfn_range 把 0x40000000 (pfn 0x40000) 映到用户态
   - /dev/axi_fpga_dev = 用户态直接读写 PL 寄存器通道 (ASIC 链控制面)
3) fpga_mem_driver.ko: 参数 fpga_mem_offset_addr=0x3F000000/0x1F000000/0x0F000000 (按 DDR 大小), 把 DDR 顶部窗口给 PL DMA 用
4) godminer (用户态 ASIC 驱动主体): 本包缺失 (S70cgminer test -x 兜底) - ASIC 链路复位/时钟/波特率等具体序列在此二进制, 需配合升级包获取
5) U-Boot 主体: 不在 BOOT.bin 分区表内 (BOOT.bin 仅 FSBL+认证段+rodata); FSBL 直接 NAND 引导, u-boot 为外部镜像

二、PS 寄存器级初始化序列 (ps7_init 表, 寄存器清晰可复核)
------------------------------------------------------------------
记录格式: [op u32][addr u32][mask u32][val u32], op=0x22 直接写/0x33 掩码写
完整 540 条见 out/ps7_init_full.txt / ps7_final.json。核心阶段:

1. SLCR 解锁/上锁
   SLCR_UNLOCK  (0xF8000008) <= 0xDF0D
   SLCR_LOCK    (0xF8000004) <= 0x767B   (每阶段前后配对)
   SLCR+0x240   : (r&~0xFFFFFFFF)|0      (WDT? 关闭)

2. DDRIOB 初始化 (0xF8000B44..0xF8000B70 区)
   +0x44,+0x48,+0x50,+0x58: (r&~0xFFF)|0x600/0x672/0x674/0x600  (input enable/delay)
   +0x4C,+0x54:            (r&~0xFFF)|0x800
   +0x5C: = 0x0018C61C  (drive strength/slew)
   +0x60..0x68: = 0x00F9861C ×3 (DQS/data 端接)
   +0x6C: (r&~0x7FFF)|0x220
   +0x70: (r&~0x21)|0x21 -> |0x20 -> (r&~0x7FFFFFF)|0x823  (DDRIOB 使能序列)

3. MIO 引脚配置 (0xF8000700.., 每 pin 掩码 0x3FFF)
   MIO00 = 0x1610  MIO01 = 0x1600  MIO02..08 = 0x0610 (UART0/QSPI 组)
   MIO09..14 = 0x1610/0x1611   MIO15 = 0x1600   (SD0)
   MIO28..33 = 0x1680 (USB0)    MIO34..39 = 0x1600/0x16E1 (USB1/GEM?)
   MIO48,49 = 0x1600           MIO52,53 = 0x1640 (GEM1/MDIO?)
   MIO76 = (r&~0x003F003F)|0x00220037
   MIO 三态寄存器 0xF8000900: (r&~0xF)|0xF  (启动初期 MIO0..3 输入)

4. PLL 配置 (ps_clk=33.333MHz)
   ARM_PLL_CTRL (0xF8000100): FDIV=0x28 (40) -> 1.333GHz, 后 bit4/BYPASS 处理
   DDR_PLL_CTRL (0xF8000104): FDIV=0x20 (32) -> 1.066GHz (DDR 533MHz)
   IO_PLL_CTRL  (0xF8000108): FDIV=0x30 (48) -> 1.600GHz
   IO_PLL_CFG (0x120): (r&~0x1F003F30)|0x1F000200
   ARM_PLL_CFG(0x140)=(r&~0x03F03F71)|0x00500141; DDR_PLL_CFG(0x148)|=0x1001; IO_PLL_CFG(0x150/0x154)|=0x1001
   ARM_CLK_CTRL (0x114): (r&~0x3FFFF0)|0x12C220  (CPU=ARMPLL/2? divisors 2,2,2? -> 666MHz? 按 div 解码: CPU_621x4 off)
   DDR_CLK_CTRL (0x118): (r&~0x3FFFF0)|0x0FA240
   DCI_CLK_CTRL(0x138)|=0x11; GEM0_CLK_CTRL(0x140)|=0x00500141; SDIO(0x150/154)|=0x1001; SPI(0x168)|=0x801; CAN(0x170)|=0x00400800
   APER_CLK_CTRL (0x12C): (r&~0x01FFCCCD)|0x015C044D  (开 DMA/USB0/USB1/GEM0/GEM1/SDIO0/SDIO1/UART0/UART1/SPI0/SPI1/FCAP? 时钟)

5. DDRC 控制器 (0xF8006000.., ~90 条): 时序参数/refresh/bank addr 等 (533MHz DDR3), 最后 DDRC+0x000 |= 0x85 (启动)
6. 收尾
   UART0.BRGR (0xE0000018): (r&~0xFFFF)|0x7C  -> 115200 @ IO PLL? (cd=124)
   UART0.CR (0xE0000000): (r&~0x1FF)|0x17 (使能 TX/RX)
   UART0.MR (0xE0000004): (r&~0x3FF)|0x20 (8N1, 无 modem)
   0xE000D000: (r&~0x80000)|0x80000  (非标地址, Bitmain 调试/预留)
   DEVCFG_CTRL (0xF8007000): (r&~0x20000000)|0 (PCAP 速率位清零)
   SCS SysTick: SYST_RVR(0xE000E014)<=0x449144; SYST_CVR(0xE000E018)清; SYST_CSR(0xE000E010)<=0x02400000 (ENABLE|CLKSOURCE, 无中断)

三、PL/ASIC 相关 (寄存器面)
------------------------------------------------------------------
- FSBL 通过 PCAP (devcfg 0xF8007000) DMA 下载内嵌位流 -> PL 实现矿链 AXI 接口
- Linux 态: /dev/axi_fpga_dev 映射物理 0x40000000..0x40001400 (5KB) = PL 寄存器:
 godminer (缺失) 通过该窗口完成: 链路使能/复位、ASIC 时钟(SPI 波特率)、温度/电压轮询
- 重任链条: FSBL(PS+PL 加载) -> [u-boot 外部] -> kernel -> insmod bitmain_axi.ko -> godminer(不在本包)

四、要拿到完整 ASIC 初始化序列还需要
------------------------------------------------------------------
1. godminer 二进制 (在 minerfs 运行后的 /usr/bin, 或随 update.bmu 的 minerfs 差分包)
2. 或 PL 位流对应的 HDL/寄存器手册 (AXI->SPI 链控制器)
本包内 PS 侧初始化已 100% 提取并注释 (见 out/ps7_init_full.txt)。


五、BM1746 (矿链 ASIC) 专项结论 [2026-11-16 增补]
==================================================================
1) BM1746 = Z15 Pro 的 Equihash 矿链 ASIC (topol.conf: chip_type=0x1746)。
   链路拓扑: chain_num=3 (3 条串行链), 每链 chain_asic_num=6, 每片 1 大核+1 小核,
   asic_addr_interval=42 (串口帧内 chip 地址步进), 频率/电压可调 (AWP12 电源, I2C 0x10,
   检查电压 1.2V, gpio 907)。

2) BM1746 的寄存器操作 (RT/BT/工作下发/温度读取) 全部为 **UART 串口协议帧**,
   由 **godminer 用户态程序** 组帧发出。本固件包内**没有** godminer 二进制:
   - 全 rootfs_1/rootfs_2 扫描无 godminer / 无任何含 BM1746 协议码的二进制
   - S70cgminer: DAEMON=/usr/bin/godminer, test -x 失败即静默退出
   - 结论: BM1746 寄存器级初始化序列无法从本 .bmu 提取

3) 串口物理通路 (本包可证):
   - PS UART0 (MIO2/3, 115200) = 控制台 (ps7_init 表内 BRGR=0x7C)
   - PS UART1 (MIO8/9, 同为 L3=UART) = 空闲/备用
   - 3 条矿链的 UART 由 PL (7z007s 位流) 提供, 位流帧数据带 DECOMP/RSA 保护:
     CTL0=0x501 (DEC 位置位) => 位流加密, 帧数据不可直接解出
   - godminer 读写 PL 寄存器窗口 = /dev/axi_fpga_dev (0x40000000..0x40001400, 5KB)
   - fpga_mem_driver.ko: PL DMA 使用的 DDR 顶窗 (0x0F/0x1F/0x3F000000 按 DDR 大小)

4) 若需 BM1746 串口寄存器序列, 需要以下之一:
   a. 运行中设备的 /usr/bin/godminer (约几 MB ARM ELF, 用本流程 1-2 小时可出寄存器级报告)
   b. 带 godminer 的完整 minerfs 差分升级包 (update.bmu 的 minerfs.image.gz 成员)
   c. 串口总线抓包 (逻辑分析仪接矿链 UART, 波特率通常 3MHz/12MHz 动态切换)

5) 本包内与 BM1746 相关的可用参数 (godminer 运行时读 topol.conf):
   chip_type=0x1746, addr_interval=42, cycle_cnt=1, domain=1, big/little core=1/1,
   chain=3×6, pic_mcu=pic1704(I2C), 传感器 TMP451×6, EEPROM AT24C02D(0x50)


六、"这个 .bmu 是不是完整固件?" [2026-11-16 答复]
==================================================================
是完整升级包, 但按比特大陆的分工它不包含矿工应用:

1) 包完整性验证:
   - 成员表 7 项全部提取成功, 尺寸与成员表逐一吻合 (BOOT.bin 2788544B 逐字节核对)
   - minerfs.image.gz 全文件系统重走读 (含三级间接块): 413 文件/53 目录/225 符号链接,
     共 692 inode, 与此前抽取一致 => 提取无损, 确实没有 godminer
   - uImage CRC 校验通过, 根公钥 + 双层 RSA 签名链完整

2) 为什么没有 godminer (Bitmain 的分区分工):
   NAND 分区表 (内核 dtb): BOOT.bin(0x0) / ramfs(0x2800000) / configs(0x4800000) /
   sig(0x5000000) / reserve1 / upgrade-ramfs(0x6000000) / upgrade-file(0x7000000) / reserve2
   - 本包 Update.sh 只写: mtd0(BOOT.bin×3 备份+dtb+uImage) / mtd1(ramfs=minerfs) /
     mtd3(sigImage) / mtd5(upgrade-ramfs)
   - **upgrade-file (0x7000000, 56MB) 从不被本流程擦写** => godminer 这类矿工应用放在
     upgrade-file/datafile 通道, 由另一种"全包"或在线升级下发, 跨系统升级保留
   - FileParser 内置成员名单里另有 'reserve' / 'datafile' 两个可选成员名, 本包均未携带,
     且有专门检测 "This Package Was Not Full Package!" / 'Check If Filename Was FullSize BMU'
     => Bitmain 明确区分 "全包/部分包", 本包属于刷系统的部分包 (system refresh)

3) 对 BM1746 逆向的影响:
   - PS/PL 引导链寄存器序列: 本包 100% 覆盖 (ps7_init 540 条 + PCAP 流程)
   - BM1746 串口协议寄存器序列: 在 godminer (upgrade-file 通道), 需单独获取
   - miner.btm (2KB, RSA 签名): opaque 表 (疑为芯片频率/参数表), 已解出并签名校验结构在案
