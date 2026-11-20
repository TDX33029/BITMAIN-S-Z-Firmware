# BM1746 devicetree.dtb 分析报告

## 1. 基本信息

| 项目 | parts/devicetree.dtb | sdcard/devicetree.dtb |
|---|---|---|
| 路径 | `D:/Document/tempPrj/BITMAIN-S-Z-Firmware/BM1746/parts/devicetree.dtb` | `D:/Document/tempPrj/BITMAIN-S-Z-Firmware/BM1746/sdcard/devicetree.dtb` |
| 大小(bytes) | 8014 | 7578 |
| MD5 | `ad826c24ff83f4c32ea104ad1558bd95` | `6b288d495fbb91197b27580f12bd392d` |
| FDT version | 17 | 17 |
| last_comp_version | 16 | 16 |
| boot CPU (boot_cpuid_phys) | 0 | 0 |
| totalsize | 8014 | 7578 |
| off_dt_struct | 56 | 56 |
| size_dt_struct (结构块) | 6996 | 6560 |
| off_dt_strings | 7052 | 6616 |
| size_dt_strings (字符串块) | 962 | 962 |
| off_mem_rsvmap | 40 | 40 |
| /memreserve/ 条目数 | 0 | 0 |
| 节点总数 | 47 | 43 |

`file` 命令输出：
- parts: `Device Tree Blob version 17, size=8014, boot CPU=0, string block size=962, DT structure block size=6996`
- sdcard: `Device Tree Blob version 17, size=7578, boot CPU=0, string block size=962, DT structure block size=6560`

## 2. 关键节点摘要

### 根节点 / 关键属性
- `compatible`: parts=`"xlnx,zynq-7000"` ; sdcard=`"xlnx,zynq-7000"`
- `model`: parts=`"Xilinx Zynq"` ; sdcard=`"Xilinx Zynq"`
- `#address-cells`: parts=`<1>` ; sdcard=`<1>`
- `#size-cells`: parts=`<1>` ; sdcard=`<1>`

### memory
- parts: `{'device_type': '"memory"', 'reg': '<0x0 0x10000000>', 'path': '/memory@0'}`
- sdcard: `{'device_type': '"memory"', 'reg': '<0x0 0x10000000>', 'path': '/memory@0'}`

### chosen / bootargs
- parts: `{'linux,stdout-path': '"/amba@0/serial@e0001000"'}`
- sdcard: `{'linux,stdout-path': '"/amba@0/serial@e0001000"'}`

### aliases
- parts: `{'ethernet0': '"/amba@0/ethernet@e000b000"', 'serial0': '"/amba@0/serial@e0000000"'}`
- sdcard: `{'ethernet0': '"/amba@0/ethernet@e000b000"', 'serial0': '"/amba@0/serial@e0000000"'}`

### UART / 串口相关节点

**parts:**
- `/amba@0/serial@e0000000` compatible=`<0x786c6e78 0x2c787561 0x72747073 0x63646e 0x732c7561 0x72742d72 0x31703800>` reg=`<0xe0000000 0x1000>` interrupts=`<0x0 0x1b 0x4>` interrupt-parent=`<3>` current-speed=`<115200>` clocks=`<0x2 0x17 0x2 0x28>`

**sdcard:**
- `/amba@0/serial@e0000000` compatible=`<0x786c6e78 0x2c787561 0x72747073 0x63646e 0x732c7561 0x72742d72 0x31703800>` reg=`<0xe0000000 0x1000>` interrupts=`<0x0 0x1b 0x4>` interrupt-parent=`<3>` current-speed=`<115200>` clocks=`<0x2 0x17 0x2 0x28>`

### AXI / PL / amba 相关节点

**parts:**
- `/` compatible=`"xlnx,zynq-7000"`
- `/amba@0` compatible=`<0x786c6e78 0x2c707337 0x2d617869 0x2d696e74 0x6572636f 0x6e6e6563 0x742d312e 0x30302e61 0x73696d 0x706c652d 0x62757300>`
- `/amba@0/efuse@f800d000` compatible=`"xlnx,zynq-efuse"` reg=`<0xf800d000 0x20>`
- `/amba@0/ethernet@e000b000` compatible=`[63 64 6e 73 2c 7a 79 6e 71 2d 67 65 6d 00 63 64 6e 73 2c 67 65 6d 00]` reg=`<0xe000b000 0x1000>` interrupts=`<0x0 0x16 0x4>` status=`"okay"`
- `/amba@0/ethernet@e000b000/mdio` 
- `/amba@0/ethernet@e000b000/mdio/phy@0` reg=`<0x0>`
- `/amba@0/ps7-afi@f8008000` compatible=`"xlnx,ps7-afi-1.00.a"` reg=`<0xf8008000 0x1000>`
- `/amba@0/ps7-afi@f8009000` compatible=`"xlnx,ps7-afi-1.00.a"` reg=`<0xf8009000 0x1000>`
- `/amba@0/ps7-afi@f800a000` compatible=`"xlnx,ps7-afi-1.00.a"` reg=`<0xf800a000 0x1000>`
- `/amba@0/ps7-afi@f800b000` compatible=`"xlnx,ps7-afi-1.00.a"` reg=`<0xf800b000 0x1000>`
- `/amba@0/ps7-ddrc@f8006000` compatible=`"xlnx,zynq-ddrc-1.0"` reg=`<0xf8006000 0x1000>`
- `/amba@0/ps7-dev-cfg@f8007000` compatible=`"xlnx,zynq-devcfg-1.0"` reg=`<0xf8007000 0x100>` interrupts=`<0x0 0x8 0x4>`
- `/amba@0/ps7-dma@f8003000` compatible=`<0x61726d2c 0x7072696d 0x6563656c 0x6c006172 0x6d2c706c 0x33333000>` reg=`<0xf8003000 0x1000>` interrupts=`<0x0 0xd 0x4 0x0 0xe 0x4 0x0 0xf 0x4 0x0 0x10 0x4 0x0 0x11 0x4 0x0 0x28 0x4 0x0 0x29 0x4 0x0 0x2a 0x4 0x0 0x2b 0x4>`
- `/amba@0/ps7-globaltimer@f8f00200` compatible=`"arm,cortex-a9-global-timer"` reg=`<0xf8f00200 0x100>` interrupts=`<0x1 0xb 0x301>`
- `/amba@0/ps7-gpio@e000a000` compatible=`"xlnx,zynq-gpio-1.0"` reg=`<0xe000a000 0x1000>` interrupts=`<0x0 0x14 0x4>`
- `/amba@0/ps7-iop-bus-config@e0200000` compatible=`"xlnx,ps7-iop-bus-config-1.00.a"` reg=`<0xe0200000 0x1000>`
- `/amba@0/ps7-ocmc@f800c000` compatible=`"xlnx,zynq-ocmc-1.0"` reg=`<0xf800c000 0x1000>` interrupts=`<0x0 0x3 0x4>`
- `/amba@0/ps7-pl310@f8f02000` compatible=`"arm,pl310-cache"` reg=`<0xf8f02000 0x1000>` interrupts=`<0x0 0x2 0x4>`
- `/amba@0/ps7-qspi-linear@fc000000` compatible=`"xlnx,ps7-qspi-linear-1.00.a"` reg=`<0xfc000000 0x1000000>`
- `/amba@0/ps7-scugic@f8f01000` compatible=`[61 72 6d 2c 63 6f 72 74 65 78 2d 61 39 2d 67 69 63 00 61 72 6d 2c 67 69 63 00]` reg=`<0xf8f01000 0x1000 0xf8f00100 0x100>`
- `/amba@0/ps7-scutimer@f8f00600` compatible=`"arm,cortex-a9-twd-timer"` reg=`<0xf8f00600 0x20>` interrupts=`<0x1 0xd 0x301>`
- `/amba@0/ps7-scuwdt@f8f00620` compatible=`"xlnx,ps7-scuwdt-1.00.a"` reg=`<0xf8f00620 0xe0>` interrupts=`<0x1 0xe 0x301>`
- `/amba@0/ps7-sdio@e0100000` compatible=`"arasan,sdhci-8.9a"` reg=`<0xe0100000 0x1000>` interrupts=`<0x0 0x18 0x4>`
- `/amba@0/ps7-slcr@f8000000` compatible=`[78 6c 6e 78 2c 7a 79 6e 71 2d 73 6c 63 72 00 73 79 73 63 6f 6e 00]` reg=`<0xf8000000 0x1000>`
- `/amba@0/ps7-slcr@f8000000/clkc@100` compatible=`"xlnx,ps7-clkc"` reg=`<0x100 0x100>`
- `/amba@0/ps7-smcc@e000e000` compatible=`"arm,pl353-smc-r2p1"` reg=`<0xe000e000 0x1000>` interrupts=`<0x0 0x12 0x4>`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000` compatible=`"arm,pl353-nand-r2p1"` reg=`<0xe1000000 0x1000000>`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@BOOT.bin-dts-marker-kernel` reg=`<0x0 0x2800000>`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@configs` reg=`<0x4800000 0x800000>`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@ramfs` reg=`<0x2800000 0x2000000>`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@reserve1` reg=`<0x5200000 0xe00000>`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@reserve2` reg=`<0xa800000 0x5800000>`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@sig` reg=`<0x5000000 0x200000>`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@upgrade-file` reg=`<0x7000000 0x3800000>`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@upgrade-ramfs` reg=`<0x6000000 0x1000000>`
- `/amba@0/ps7-ttc@f8001000` compatible=`"cdns,ttc"` reg=`<0xf8001000 0x1000>` interrupts=`<0x0 0xa 0x4 0x0 0xb 0x4 0x0 0xc 0x4>`
- `/amba@0/ps7-usb@e0002000` compatible=`[78 6c 6e 78 2c 70 73 37 2d 75 73 62 2d 31 2e 30 30 2e 61 00 78 6c 6e 78 2c 7a 79 6e 71 2d 75 73 62 2d 31 2e 30 30 2e 61 00]` reg=`<0xe0002000 0x1000>` interrupts=`<0x0 0x15 0x4>`
- `/amba@0/ps7-xadc@f8007100` compatible=`"xlnx,zynq-xadc-1.00.a"` reg=`<0xf8007100 0x20>` interrupts=`<0x0 0x7 0x4>`
- `/amba@0/ps7_i2c@e0005000` compatible=`"cdns,i2c-r1p10"` reg=`<0xe0005000 0x1000>` interrupts=`<0x0 0x30 0x4>` status=`"okay"`
- `/amba@0/serial@e0000000` compatible=`<0x786c6e78 0x2c787561 0x72747073 0x63646e 0x732c7561 0x72742d72 0x31703800>` reg=`<0xe0000000 0x1000>` interrupts=`<0x0 0x1b 0x4>`

**sdcard:**
- `/` compatible=`"xlnx,zynq-7000"`
- `/amba@0` compatible=`<0x786c6e78 0x2c707337 0x2d617869 0x2d696e74 0x6572636f 0x6e6e6563 0x742d312e 0x30302e61 0x73696d 0x706c652d 0x62757300>`
- `/amba@0/efuse@f800d000` compatible=`"xlnx,zynq-efuse"` reg=`<0xf800d000 0x20>`
- `/amba@0/ethernet@e000b000` compatible=`[63 64 6e 73 2c 7a 79 6e 71 2d 67 65 6d 00 63 64 6e 73 2c 67 65 6d 00]` reg=`<0xe000b000 0x1000>` interrupts=`<0x0 0x16 0x4>` status=`"okay"`
- `/amba@0/ethernet@e000b000/mdio` 
- `/amba@0/ethernet@e000b000/mdio/phy@0` reg=`<0x0>`
- `/amba@0/ps7-afi@f8008000` compatible=`"xlnx,ps7-afi-1.00.a"` reg=`<0xf8008000 0x1000>`
- `/amba@0/ps7-afi@f8009000` compatible=`"xlnx,ps7-afi-1.00.a"` reg=`<0xf8009000 0x1000>`
- `/amba@0/ps7-afi@f800a000` compatible=`"xlnx,ps7-afi-1.00.a"` reg=`<0xf800a000 0x1000>`
- `/amba@0/ps7-afi@f800b000` compatible=`"xlnx,ps7-afi-1.00.a"` reg=`<0xf800b000 0x1000>`
- `/amba@0/ps7-ddrc@f8006000` compatible=`"xlnx,zynq-ddrc-1.0"` reg=`<0xf8006000 0x1000>`
- `/amba@0/ps7-dev-cfg@f8007000` compatible=`"xlnx,zynq-devcfg-1.0"` reg=`<0xf8007000 0x100>` interrupts=`<0x0 0x8 0x4>`
- `/amba@0/ps7-dma@f8003000` compatible=`<0x61726d2c 0x7072696d 0x6563656c 0x6c006172 0x6d2c706c 0x33333000>` reg=`<0xf8003000 0x1000>` interrupts=`<0x0 0xd 0x4 0x0 0xe 0x4 0x0 0xf 0x4 0x0 0x10 0x4 0x0 0x11 0x4 0x0 0x28 0x4 0x0 0x29 0x4 0x0 0x2a 0x4 0x0 0x2b 0x4>`
- `/amba@0/ps7-globaltimer@f8f00200` compatible=`"arm,cortex-a9-global-timer"` reg=`<0xf8f00200 0x100>` interrupts=`<0x1 0xb 0x301>`
- `/amba@0/ps7-gpio@e000a000` compatible=`"xlnx,zynq-gpio-1.0"` reg=`<0xe000a000 0x1000>` interrupts=`<0x0 0x14 0x4>`
- `/amba@0/ps7-iop-bus-config@e0200000` compatible=`"xlnx,ps7-iop-bus-config-1.00.a"` reg=`<0xe0200000 0x1000>`
- `/amba@0/ps7-ocmc@f800c000` compatible=`"xlnx,zynq-ocmc-1.0"` reg=`<0xf800c000 0x1000>` interrupts=`<0x0 0x3 0x4>`
- `/amba@0/ps7-pl310@f8f02000` compatible=`"arm,pl310-cache"` reg=`<0xf8f02000 0x1000>` interrupts=`<0x0 0x2 0x4>`
- `/amba@0/ps7-qspi-linear@fc000000` compatible=`"xlnx,ps7-qspi-linear-1.00.a"` reg=`<0xfc000000 0x1000000>`
- `/amba@0/ps7-scugic@f8f01000` compatible=`[61 72 6d 2c 63 6f 72 74 65 78 2d 61 39 2d 67 69 63 00 61 72 6d 2c 67 69 63 00]` reg=`<0xf8f01000 0x1000 0xf8f00100 0x100>`
- `/amba@0/ps7-scutimer@f8f00600` compatible=`"arm,cortex-a9-twd-timer"` reg=`<0xf8f00600 0x20>` interrupts=`<0x1 0xd 0x301>`
- `/amba@0/ps7-scuwdt@f8f00620` compatible=`"xlnx,ps7-scuwdt-1.00.a"` reg=`<0xf8f00620 0xe0>` interrupts=`<0x1 0xe 0x301>`
- `/amba@0/ps7-sdio@e0100000` compatible=`"arasan,sdhci-8.9a"` reg=`<0xe0100000 0x1000>` interrupts=`<0x0 0x18 0x4>`
- `/amba@0/ps7-slcr@f8000000` compatible=`[78 6c 6e 78 2c 7a 79 6e 71 2d 73 6c 63 72 00 73 79 73 63 6f 6e 00]` reg=`<0xf8000000 0x1000>`
- `/amba@0/ps7-slcr@f8000000/clkc@100` compatible=`"xlnx,ps7-clkc"` reg=`<0x100 0x100>`
- `/amba@0/ps7-smcc@e000e000` compatible=`"arm,pl353-smc-r2p1"` reg=`<0xe000e000 0x1000>` interrupts=`<0x0 0x12 0x4>`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000` compatible=`"arm,pl353-nand-r2p1"` reg=`<0xe1000000 0x1000000>`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@BOOT.bin-env-dts-kernel` reg=`<0x0 0x2800000>`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@configs` reg=`<0x4800000 0x800000>`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@ramfs` reg=`<0x2800000 0x2000000>`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@ramfs-bak` reg=`<0x6000000 0x2000000>`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@reserve` reg=`<0x5000000 0x1000000>`
- `/amba@0/ps7-ttc@f8001000` compatible=`"cdns,ttc"` reg=`<0xf8001000 0x1000>` interrupts=`<0x0 0xa 0x4 0x0 0xb 0x4 0x0 0xc 0x4>`
- `/amba@0/ps7-usb@e0002000` compatible=`[78 6c 6e 78 2c 70 73 37 2d 75 73 62 2d 31 2e 30 30 2e 61 00 78 6c 6e 78 2c 7a 79 6e 71 2d 75 73 62 2d 31 2e 30 30 2e 61 00]` reg=`<0xe0002000 0x1000>` interrupts=`<0x0 0x15 0x4>`
- `/amba@0/ps7-xadc@f8007100` compatible=`"xlnx,zynq-xadc-1.00.a"` reg=`<0xf8007100 0x20>` interrupts=`<0x0 0x7 0x4>`
- `/amba@0/serial@e0000000` compatible=`<0x786c6e78 0x2c787561 0x72747073 0x63646e 0x732c7561 0x72742d72 0x31703800>` reg=`<0xe0000000 0x1000>` interrupts=`<0x0 0x1b 0x4>`

### MTD / NAND / Flash / 分区相关节点

**parts:**
- `/amba@0/ps7-smcc@e000e000` compatible=`"arm,pl353-smc-r2p1"` reg=`<0xe000e000 0x1000>`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000` compatible=`"arm,pl353-nand-r2p1"` reg=`<0xe1000000 0x1000000>`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@BOOT.bin-dts-marker-kernel` reg=`<0x0 0x2800000>` label=`"BOOT.bin-dts-marker-kernel"`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@configs` reg=`<0x4800000 0x800000>` label=`"configs"`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@ramfs` reg=`<0x2800000 0x2000000>` label=`"ramfs"`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@reserve1` reg=`<0x5200000 0xe00000>` label=`"reserve1"`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@reserve2` reg=`<0xa800000 0x5800000>` label=`"reserve2"`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@sig` reg=`<0x5000000 0x200000>` label=`"sig"`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@upgrade-file` reg=`<0x7000000 0x3800000>` label=`"upgrade-file"`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@upgrade-ramfs` reg=`<0x6000000 0x1000000>` label=`"upgrade-ramfs"`

**sdcard:**
- `/amba@0/ps7-smcc@e000e000` compatible=`"arm,pl353-smc-r2p1"` reg=`<0xe000e000 0x1000>`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000` compatible=`"arm,pl353-nand-r2p1"` reg=`<0xe1000000 0x1000000>`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@BOOT.bin-env-dts-kernel` reg=`<0x0 0x2800000>` label=`"BOOT.bin-env-dts-kernel"`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@configs` reg=`<0x4800000 0x800000>` label=`"configs"`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@ramfs` reg=`<0x2800000 0x2000000>` label=`"ramfs"`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@ramfs-bak` reg=`<0x6000000 0x2000000>` label=`"ramfs-bak"`
- `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@reserve` reg=`<0x5000000 0x1000000>` label=`"reserve"`

### I2C 节点

**parts:**
- `/amba@0/ps7_i2c@e0005000` compatible=`"cdns,i2c-r1p10"` reg=`<0xe0005000 0x1000>` status=`"okay"`

**sdcard:**
- (未发现)

### GPIO 节点

**parts:**
- `/amba@0/ps7-gpio@e000a000` compatible=`"xlnx,zynq-gpio-1.0"` reg=`<0xe000a000 0x1000>`

**sdcard:**
- `/amba@0/ps7-gpio@e000a000` compatible=`"xlnx,zynq-gpio-1.0"` reg=`<0xe000a000 0x1000>`

### Ethernet/网络节点

**parts:**
- `/amba@0/ethernet@e000b000` compatible=`[63 64 6e 73 2c 7a 79 6e 71 2d 67 65 6d 00 63 64 6e 73 2c 67 65 6d 00]` reg=`<0xe000b000 0x1000>` status=`"okay"`
- `/amba@0/ethernet@e000b000/mdio` 
- `/amba@0/ethernet@e000b000/mdio/phy@0` reg=`<0x0>`

**sdcard:**
- `/amba@0/ethernet@e000b000` compatible=`[63 64 6e 73 2c 7a 79 6e 71 2d 67 65 6d 00 63 64 6e 73 2c 67 65 6d 00]` reg=`<0xe000b000 0x1000>` status=`"okay"`
- `/amba@0/ethernet@e000b000/mdio` 
- `/amba@0/ethernet@e000b000/mdio/phy@0` reg=`<0x0>`

## 3. 关键差异

- 新增节点（sdcard 相对 parts）: 3
  - `+ /amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@BOOT.bin-env-dts-kernel`
  - `+ /amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@ramfs-bak`
  - `+ /amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@reserve`
- 删除节点（sdcard 相对 parts）: 7
  - `- /amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@BOOT.bin-dts-marker-kernel`
  - `- /amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@reserve1`
  - `- /amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@reserve2`
  - `- /amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@sig`
  - `- /amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@upgrade-file`
  - `- /amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@upgrade-ramfs`
  - `- /amba@0/ps7_i2c@e0005000`
- 属性有差异的公共节点: 0

### MTD 分区布局差异（聚焦）
- parts:
  - `/amba@0/ps7-smcc@e000e000 | label=None | reg=<0xe000e000 0x1000> | compatible="arm,pl353-smc-r2p1"`
  - `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000 | label=None | reg=<0xe1000000 0x1000000> | compatible="arm,pl353-nand-r2p1"`
  - `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@BOOT.bin-dts-marker-kernel | label="BOOT.bin-dts-marker-kernel" | reg=<0x0 0x2800000> | compatible=None`
  - `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@configs | label="configs" | reg=<0x4800000 0x800000> | compatible=None`
  - `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@ramfs | label="ramfs" | reg=<0x2800000 0x2000000> | compatible=None`
  - `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@reserve1 | label="reserve1" | reg=<0x5200000 0xe00000> | compatible=None`
  - `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@reserve2 | label="reserve2" | reg=<0xa800000 0x5800000> | compatible=None`
  - `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@sig | label="sig" | reg=<0x5000000 0x200000> | compatible=None`
  - `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@upgrade-file | label="upgrade-file" | reg=<0x7000000 0x3800000> | compatible=None`
  - `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@upgrade-ramfs | label="upgrade-ramfs" | reg=<0x6000000 0x1000000> | compatible=None`
- sdcard:
  - `/amba@0/ps7-smcc@e000e000 | label=None | reg=<0xe000e000 0x1000> | compatible="arm,pl353-smc-r2p1"`
  - `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000 | label=None | reg=<0xe1000000 0x1000000> | compatible="arm,pl353-nand-r2p1"`
  - `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@BOOT.bin-env-dts-kernel | label="BOOT.bin-env-dts-kernel" | reg=<0x0 0x2800000> | compatible=None`
  - `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@configs | label="configs" | reg=<0x4800000 0x800000> | compatible=None`
  - `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@ramfs | label="ramfs" | reg=<0x2800000 0x2000000> | compatible=None`
  - `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@ramfs-bak | label="ramfs-bak" | reg=<0x6000000 0x2000000> | compatible=None`
  - `/amba@0/ps7-smcc@e000e000/ps7-nand@e1000000/partition@reserve | label="reserve" | reg=<0x5000000 0x1000000> | compatible=None`

### bootargs 差异（聚焦）
- parts: `(none)`
- sdcard: `(none)`

## 4. 与挖矿/单芯片二次开发相关的结论

### UART / 串口
- UART 节点、基址(`reg`)、中断(`interrupts`)与状态来自“UART / 串口相关节点”小节；波特率线索可关注 `current-speed`、`clock-frequency`/`xlnx,clock-freq` 属性及 bootargs 中的 `console=` 参数。
- 结论：可据此确定早期引导与 Linux 控制台串口硬件地址与中断路由，为裸机/最小系统 bring-up 提供依据。

### AXI / PL 相关
- AXI/PL/amba 相关节点反映 PS-PL 互联与 FPGA 侧外设映射；若存在自定义 IP（如矿机控制/哈希加速相关）通常表现为 `xlnx,*` compatible 与固定 MMIO `reg`。
- 结论：这些节点是 Zynq PS 通过 AXI 访问 PL 逻辑的线索；对“单颗 BM1746 直驱”评估时需区分 ASIC 控制总线与 Zynq PL 中转逻辑，DTS 仅证明 PS 侧可见映射。

### NAND / MTD 分区与启动链路
- MTD/分区节点及 bootargs 中的 `root=`/`mtdparts=`/`ubi` 等参数决定内核从何处加载 rootfs 与各固件分区布局。
- parts 与 sdcard 的分区/bootargs 差异对应不同启动介质或部署形态（板载固件 vs SD 卡/恢复镜像），影响升级、刷写与故障恢复流程。

### 对“单颗 BM1746 直驱（脱离 Zynq PL）”的意义
- 该 devicetree 描述的是 Zynq PS/Linux 对板级外设与 PL 映射的视图，不直接描述 BM1746 ASIC 本身的寄存器/通信协议；BM1746 控制通常经由 SPI/UART/特定总线或 PL 桥接，需结合驱动、bitstream 与硬件原理图确认。
- 若 DTS 中未见明确命名的 BM1746/算力链节点，则仅凭 DTB 无法得出直驱所需的引脚复用、时序与协议细节；DTS 的主要价值在于界定 PS 侧资源与当前系统对 PL 的依赖边界。
- 因此：DTB 分析可辅助识别控制主机侧接口与启动约束，但“脱离 Zynq PL 直驱 BM1746”仍需额外证据（PL bitstream 功能、总线拓扑、ASIC 通信协议/驱动实现）。

## 5. 证据路径
- `D:/Document/tempPrj/BITMAIN-S-Z-Firmware/BM1746/parts/devicetree.dtb`
- `D:/Document/tempPrj/BITMAIN-S-Z-Firmware/BM1746/sdcard/devicetree.dtb`
- `D:/Document/tempPrj/BITMAIN-S-Z-Firmware/BM1746/analysis/dtb/parts_devicetree.dts.txt`
- `D:/Document/tempPrj/BITMAIN-S-Z-Firmware/BM1746/analysis/dtb/sdcard_devicetree.dts.txt`
- `D:/Document/tempPrj/BITMAIN-S-Z-Firmware/BM1746/analysis/gap_reports/devicetree_analysis.md`
- `BM1746/analysis/dtb/ (输出目录)`
- `file 命令: parts/devicetree.dtb -> Device Tree Blob version 17, size=8014, boot CPU=0, string block size=962, DT structure block size=6996`
- `file 命令: sdcard/devicetree.dtb -> Device Tree Blob version 17, size=7578, boot CPU=0, string block size=962, DT structure block size=6560`
- `md5sum: ad826c24ff83f4c32ea104ad1558bd95 (parts), 6b288d495fbb91197b27580f12bd392d (sdcard)`
- `解析脚本(临时): C:/Users/Lenovo/AppData/Local/Temp/parse_bm1746_dtb.py (/tmp/parse_bm1746_dtb.py)`
