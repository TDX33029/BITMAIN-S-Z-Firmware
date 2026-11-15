# 逆向产物总索引 — Z15 Pro (BM1746) 固件完全逆向

> 目标: 支撑固件重写。本目录 `reversed/` 存放全部功能函数（按二进制分目录、按功能分子文件夹）、
> 常量表、协议记录。`analysis/out/` 存放 PS 级初始化报告。上游解包物在 `parts/ rootfs_1/ rootfs_2/ sdcard/`。

## 一、产物地图

```
reversed/
├── cgminer/                    ← 核心: BM1746 ASIC 驱动 (644 函数, 16 功能类)
│   ├── 00_INDEX.md             分类索引 (每函数一个 .c 文件)
│   ├── _callgraph.txt          全部调用边 (1821 条)
│   ├── _string_xref.txt        字符串→函数引用 (1453 条)
│   ├── _func_index.txt         函数→地址 (644 条)
│   ├── 10_uart_protocol/       串口收发 FIFO (11)
│   ├── 12_freq_pll/            频率/PLL/扫频 (99)
│   ├── 13_voltage_pic/         电压/PIC16F1704 电源 (39)
│   ├── 14_temp_fan/            温度/风扇 PWM (18)
│   ├── 15_fpga_axi/            FPGA AXI 窗口 (10)
│   ├── 16_chain_mgmt/          链复位/寻址/波特率/寄存器写 (44)
│   ├── 20_job_mining/          挖矿任务/nonce (162)
│   ├── 21_pool_net/            矿池/stratum/socket (61)
│   ├── 22_json_rpc/            JSON-RPC (6)
│   ├── 30_api_status/          API/状态 (46)
│   ├── 32_i2c_sensor/          I2C 传感 (3)
│   ├── 40_config/  41_logging/ 50_crypto_hash/ 60_util_util/ 70_no_strings/
├── fsbl_funcs/                 FSBL "Zynq7007_miner.elf" (165 函数 + _classified/)
│   ├── ps7_init 540 条寄存器序列 → ../analysis/out/ps7_init_full.txt
│   └── 内嵌 PL 位流 @0x21E30 (254KB, 7z007s)
├── fileparser_funcs/           FileParser BMU 解析/烧写 (42 函数 + _classified/)
├── monitor_funcs/              miner-monitor 监控守护 (112 函数 + _classified/)
├── uboot_funcs/                U-Boot 2016.07 (140 函数, 基址 0x40000000)
├── ko_bitmain_axi/             bitmain_axi.ko (5 函数, mmap 0x40000000 窗口)
├── ko_fpga_mem/                fpga_mem_driver.ko (5 函数, DDR 窗口)
├── cgminer_funcs/              cgminer 原始平铺导出 (全部单文件, 644)
├── tables/                     常量表/配置事实 (见下)
└── protocols/                  协议字符串 (stratum 103 条)
```

## 二、tables/ — 重写固件直接可用的数据

| 文件 | 内容 |
|---|---|
| `freq_table_full.csv` | BM1746 频率表 115 条 (100~900MHz, 槽位/freq/pll) |
| `pll_decode.md` | **PLL 公式 (115/115 验证)**: `fout = 25MHz × N/(P×Q)`, N=pll[31:16], P=pll[7:4], Q=pll[11:8], bit0=1 |
| `topol.conf` | 拓扑: 3 链×6 ASIC, asic_addr_interval=42, chip_type 0x1746, AWP12 1.2V, PIC pic1704, TMP451, AT24C02D |
| `cgminer.conf.factory.sd` / `.nand` | 出厂配置 (SD: freq=800/volt=780; NAND: freq=0→800, level=100) |
| `known_constants.md` | I2C 地址表 0x76b48、PIC 全命令函数名表 |
| `function_name_catalog.txt` | 208 个驱动函数自报名 (`__func__` 字符串) — 重写时的 API 清单 |
| `subtype` | "zynq7007_BEZ36501" (机器码校验用) |

## 三、关键机制速查 (重写固件必须复刻的 8 件事)

1. **BM1746 UART 帧** (11B): `[AA 55][0x41|rw<<4][..][chip_addr][reg][32bit BE][4bit 校验]` — reg 0x0C=PLL, 0x14=ticket mask, 0x1C=baud/温度使能, 0x20=I2C 间接, 0x28=核心超时, 0x40=模拟 MUX, 0x50=nonce 位移
2. **PLL**: 寄存器 0x0C, 值查 115 条表 (25MHz 参考钟), 800MHz=0x00800221
3. **频率规则**: 配置 0/800/缺省→800; 扫频线程 775~815 起步, 二分逼近, nonce 错误率+OX 状态判据; 结果写 /config/chips_freq.config (合法窗 [775,900])
4. **链初始化序**: PIC 断电→链复位 (base[0x34])→PIC 上电→设址 (interval=42)→波特率 (0x3C=bt8d×4)→0x0C 频率→0x28 超时 (80000)→0x14 ticket mask→0x1C 温度使能
5. **PIC16F1704 命令** (同一 UART): `[55][AA][len][cmd][args]`+回显; 0x10=电压 (0x15 偏置), 0x15/1A/1B=DC-DC 开关, 0x06=loader→app, 0x07=PIC 复位, 心跳保活
6. **电压**: PIC 0x10 + 每 ASIC reg 0x20 (I2C 间接) 写 ISL, 合法窗 [700,800]mV, ±4mV 回读校验
7. **PL 窗口**: mmap /dev/axi_fpga_dev (物理 0x40000000, 0x1400B); 0x80=init 0x8080800F, 0x34=链复位, 0x3C=波特率, 0x180+4n=TX FIFO, 0x1F0+4n=RX 计数
8. **PS 级**: ps7_init 540 条 (SLCR 解锁 0xDF0D/锁定 0x767B、DDRIOB、MIO、时钟) — 启动前必须完整执行, 见 analysis/out/ASIC_INIT_REPORT.md

## 四、重写固件路线图 (建议顺序)

| 步骤 | 内容 | 数据来源 (已完成) |
|---|---|---|
| 1 | PS 启动底座: FSBL 重写 (ps7_init 表 + PCAP 加载) | fsbl_funcs/ + ps7_init_full.txt + pcap_regs.json |
| 2 | PL 固件: 位流提取/重生成 (7z007s) | Zynq7007_bitstream.bin + pcap_regs.json |
| 3 | 内核 + 驱动: bitmain_axi.ko / fpga_mem_driver.ko 重写 | ko_*/ (仅 5 函数, 语义已明) |
| 4 | **ASIC 驱动层** (串口帧/PLL/PIC/链管理) | cgminer/10~16 类 + tables/ + BM1746_uart_register_ops.md |
| 5 | 挖矿业务层 (任务下发/nonce 回收/分层) | cgminer/20 类 (162 函数) |
| 6 | 矿池层 (stratum 客户端/重连/难度) | cgminer/21+22 类 + protocols/stratum_strings.txt |
| 7 | 监控/API (cgminer-api 接口、miner-monitor) | cgminer/30 类 + monitor_funcs/ |
| 8 | 升级链路 (BMU 容器/签名/FileParser 流程) | fileparser_funcs/ + SUMMARY.md §容器格式 |

## 五、与既有报告的关系

- `../SUMMARY.md` — BMU 容器解包 + 频率分析总结
- `../bm1746/BM1746_uart_register_ops.md` — 串口协议/寄存器完整报告 (含帧格式、PIC 命令、频率修改点)
- `../analysis/out/ASIC_INIT_REPORT.md` — PS/FSBL 初始化分析
- `../bm1746/cgminer_all_decomp.c` — cgminer 全量单文件反编译 (检索用; 分类版在 cgminer/)
- `../analysis/ghidra_proj*/` — Ghidra 工程 (可复查任何函数)
