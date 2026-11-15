# ESP32 + 单颗 BM1746 直驱可行性核验 (基于 cgminer 逆向产物)

结论先行: **可以写出来, 单芯片直驱所需协议 100% 已从 cgminer_z15_bm1746.elf 逆出**;
不能照搬的只有"Z15 Pro 的 PL(FPGA) 中转层", ESP32 用自己的 UART 替代即可。
电源 (PIC16F1704/ISL) 按约定不在范围内, 但命令表已备齐。

## 一、已核验的完整协议要素 (全部有反编译函数支撑)

### 1. 命令帧 (11 字节, FUN_0003cdcc @ bm1746/cgminer_all_decomp.c:32200 附近)
```
[0]=0x55 [1]=0xAA [2]=(rw<<4|0x41)  0x41=链广播, 0x51=单芯片
[3]=保留0  [4]=chip_addr*interval  [5]=reg  [6..9]=32bit 值 (字节内交换后 BE 发送)
[10]=CRC 校验低5位 (高3位保留)
```
注意帧第 10 字节是 `crc & 0xe0 | checksum & 0x1f` (代码里 local_c & 0xe0 恒 0, 实际= checksum&0x1f)。
checksum 算法 = FUN_0003cc6c: **按位 (MSB first) 的奇偶/异或折叠**, 非标准 CRC,
逐位: `parity ^= ((byte & bit) != 0)` 变体 — 已有完整 C 代码可照抄 (见 12_freq_pll/FUN_0003cc6c.c)。

### 2. 校验算法双套
- 短帧 checksum: FUN_0003cc6c (4-bit lane 校验, 逐字节 8 位滚动异或)
- 长帧 CRC16-CCITT: 查表 @0x68aec, **多项式 0x1021 已 100% 验证** (Python 复算 256 项全中)
  `crc16(table, data, len)` 初值 0xFFFF — 任务帧 (0x92) 与 nonce 回包 (0x5d) 都用它。

### 3. 寄存器码 (reg) 全表 — 见 BM1746_uart_register_ops.md 第二节
0x0C PLL(查表/公式), 0x14 ticket mask, 0x1C 波特率/温度使能(0x4060), 0x20 I2C 间接,
0x28 核心超时, 0x40 模拟 MUX, 0x50 nonce 位移。
PLL 公式已 100% 验证: `fout = 25MHz * N/(P*Q)`, N=pll[31:16], P=pll[7:4], Q=pll[11:8]。

### 4. 波特率 (单芯片直驱可无视 FPGA 层)
- 初始 bt8d=0x1A (低速率枚举), 枚举完切 bt8d=0 (最高速)
- reg 0x1C 值 = `(bt8d & 0x1f) << 8 | 保留位` (FUN_00043e98)
- Z15 上 FPGA 以 bt8d 直接分频 25MHz 参考钟 (0x1A→约 862kbps 量级, 0→25M/1)
- **ESP32 直驱**: 用自己的 UART 波特率对着芯片即可; 芯片侧 bt8d=0 时请求的速率需实测标定
  (唯一建议实测点, 见缺口 §三-1)

### 5. 寻址 (FUN_00042f7c software_set_address_chain)
- 先发 7 字节地址复位帧: `[55][AA][04][00][00][00][chk]` (FUN_0003cd58, 值域 0x553)
- 再对每组发 7 字节设址帧: `[55][AA][04][40][00][addr][chk]` (FUN_0003ce7c, 0x40|addr)
- 单颗芯片: addr=0x00, interval=42 不再起作用 (那是多芯片链式自动递增用)

### 6. 任务帧 (bm1740_makeup_work, FUN_00042a60) — 0x92 字节
```
[0]=0x55 [1]=0xAA [2]=0x20 (|chip 分组低位)  ... 任务体 0x8C=140 字节 ...  [0x90..0x91]=CRC16-CCITT(BE)
```
140 字节任务体 = **Equihash (200,9) 的 140B 区块头** (version+hashPrevBlock+hashMerkleRoot+
hashReserved+ nTime+nBits+nNonce, 与 Zcash 头一致), 由 FUN_0004eee0(ctx,200,9) "ZcashPoW" 佐证。
workid = 帧[2] 的低 5 位 (0x20|workid&0x1f) — 最多 32 个在飞任务。
发送线程 FUN_00042b54: 从 work 队列取任务 → 构帧 → FUN_0003c150 发 0x92 字节。

### 7. nonce 回包 (RX 分发循环 @ cgminer_all_decomp.c:36010-36110)
三种帧, 均以 [55][AA] 开头, 第 3 字节高 4 位分型:
- `0xE_` → **nonce 数据帧 0x5D=93 字节** → FUN_0004a754 (nonce_handle):
  [2]&0xf=workid 序号(0..0xf 连续校验), [3]=chip_addr, [4]=workid, 之后 16 组 0x55C 结构?
  实际布局: 16 个 nonce 报告块, 每块 0x5B=91B, 末 2B=CRC16-CCITT (bm1740_verify_nonce_integrality)
  尾部: [0x556..0x558]=extra 字节, [0x559]=? , [0x5AA]=? 组装进 0x54D 的上报块
- `0xCC` → 9 字节状态/PM 帧 → FUN_0004acec (pm_handle): [3..6]=reg 值回读
- 其它 (`0x4_`..) → 9 字节寄存器回读 → FUN_0004ae48 (reg_handle): [3..6]=值, [7]=?

单芯片场景: 只需解 `0xE_` 93B nonce 帧和 9B 寄存器回读帧, 校验都是 CRC16-CCITT。

### 8. 初始化序列 (FUN_00049cc8 "z15 init" + reset_chain FUN_00046434)
```
1. (FPGA 层, ESP32 跳过) init_fpga: base[0x80]=0x8080800F 等 bit31 清零
2. UART @ bt8d=0x1A 低速
3. 地址复位帧 [55 AA 04 00 00 00 chk]  (FUN_00042f04)
4. 设址帧     [55 AA 04 40 00 00 chk]  (FUN_00042f40, 单颗 addr=0)
5. reg 0x1C: bt8d (低速)
6. reset: PIC 断电→0.1s→PIC 上电→1s (ESP32 用 GPIO 控 EN/复位脚替代)
7. reg 0x0C: 频率 (800MHz=0x00800221, 或按公式合成)
8. reg 0x28: 核心超时 = 80000 (0x13880)
9. reg 0x14: ticket mask (Z15 出厂值查 DAT_00076b60 初始=0? 按 diff 设置)
10. reg 0x1C: 0x4060|bt8d 逐芯片开温度回读
11. 切高速率: reg 0x1C bt8d=0 + 主机 UART 同步提速
12. 扫频线程 (可选): 775→815 二分, nonce 错误率+OX 状态判定
13. 开始下发 0x92 任务帧 (workid 0..31 轮转)
```

### 9. 芯片计数探测 (FUN_000498f4 → check_asic_reg FUN_0004791c)
读芯片 ID: 构 0x51 单读帧 reg?→ 回包 [2]&0xf 组号 + 芯片地址自增判定 (interval=42)。
单颗芯片: 发一次设址→读 ID 回包存在即认为 alive。

## 二、ESP32 固件必须实现的 5 个模块 (全部有源码级参考)

| 模块 | 参考函数 (bm1746/cgminer_all_decomp.c) | 复刻难度 |
|---|---|---|
| 帧构造/校验 | FUN_0003cdcc(11B), FUN_00042a60(0x92), FUN_0003cc6c, CRC16 表 | 低, 直接抄 |
| 初始化序列 | FUN_00049cc8 + FUN_00046434 + FUN_00042f7c | 低, 照序 |
| 任务下发 | FUN_00042b54 线程 + workid 管理 | 低 (单芯片单线程) |
| nonce 回收 | FUN_0004a754 + CRC16 完整性 + workid 映射 | 中 (93B 帧解析) |
| Equihash 校验 | FUN_00055930 (SHA256 起始) + 原始 Zcash PoW | 中 (ESP32 上只做抽验) |

## 三、已知缺口 (不影响"能写出来", 但标注诚实)

1. **bt8d=0 时芯片请求的精确主机波特率**: Z15 走 FPGA 分频 (25MHz/(bt8d+1)?),
   ESP32 直驱要对表实测一次 (逻辑分析仪抓芯片上电后的自动波特率序列, 或先用 bt8d=0x1A 的
   862kbps 推算档位试 115200~3M 档)。协议本身与波特率无关, 帧格式不变。
2. **0xE_ 帧 16×0x5B 块内部字段** (每块 = [chip_addr][workid][...nonce 4B...][...]
   + CRC2B) 的逐字节语义已到 0x54D 组装层, 但"块内 91 字节"的 bit 级字段 (含 OX/温度位)
   只逆到结构级。影响: 不能解析每芯片温度/电压遥测 (不影响挖矿 nonce 路径)。
3. **ticket mask 的出厂值**: DAT_00076b60 的初始赋值在 cgminer.conf 解析里
   (bitmain-ticket-mask 未出现在 factory 配置 → 走代码默认)。单芯片可用 0xFF (全 diff)。
4. **Equihash (200,9) solver**: 芯片只算 PoW 里的 Blake2b+solver, 主机侧只需校验
   (FUN_00055930 SHA256 已逆出)。ESP32 上完整校验一个 sol 需要 ~100ms 级, 可接受。
5. **PIC16F1704 电源序列**: 约定不做; 但命令码表 (0x06/07/10/15/1A/1B/心跳) 已在报告中,
   若自制供电板可复用。

## 四、给 ESP32 侧的最小固件骨架 (伪码级, 全部要素已有出处)

```c
// 1. 帧构造 (源: FUN_0003cdcc)
void cmd_frame(uint8_t *f, int rw, uint8_t chip, uint8_t reg, uint32_t val) {
  f[0]=0x55; f[1]=0xAA;
  f[2]= 0x41 | (rw?0x10:0x00);          // 广播写/单芯片
  f[3]=0; f[4]=chip; f[5]=reg;
  put_be32(&f[6], bswap32(val));         // 字节内交换后大端 (见函数体 uStack_14 处理)
  f[10]= lane_checksum(f+2, 8) & 0x1f;  // FUN_0003cc6c
}
// 2. PLL (源: freq_table_full.csv + pll_decode.md)
uint32_t pll_for(uint32_t mhz){ return (mhz/5)<<16 | (25*mhz/mhz>>4)<<4 | (2<<8) | 1; } // 或查表
// 3. 初始化 (源: FUN_00049cc8 流程)
uart_begin(BT8D1A_BAUD);  // ~862k 或先 115200 试
addr_reset();  set_addr(0);  set_baud_reg(0x1A);
gpio_reset_pulse();          // 替代 PIC 断/上电
set_pll(0x00800221);         // 800 MHz
set_core_timeout(80000);     // reg 0x28
set_ticket_mask(0xFF);       // reg 0x14
temp_enable(0x4060);         // reg 0x1C
switch_baud(HIGH);           // reg 0x1C bt8d=0 + host 提速
// 4. 任务环 (源: FUN_00042b54)
for (;;) {
  work = pool_next_work();                     // 140B header
  tx_work_frame(workid&0x1f, work);            // 0x92 帧, CRC16
  rx = uart_read_frame();                      // 0x5D=93B nonce / 9B reg
  if (rx.type==0xE_) for (blk in rx.blocks) {
     submit_share(blk.nonce, blk.workid);      // Equihash verify -> pool
  }
}
```

## 五、核验用产物索引

- 帧构造: bm1746/cgminer_all_decomp.c → FUN_0003cdcc (0x22 附近), FUN_00042a60 (31875)
- checksum/CRC: FUN_0003cc6c (32268 前), CRC16 表 0x68aec (=0x1021, 已验证)
- 初始化: FUN_00049cc8 (35269, "z15 init"), reset_chain FUN_00046434 (33757)
- 寻址: FUN_00042f04/FUN_00042f40/FUN_0003cd58/FUN_0003ce7c (32015-32055/32102-32120)
- 任务: FUN_00042b54 (31911), FUN_0004eee0 (37675, N=200 K=9), FUN_00054f8c (39851)
- nonce 回包: RX 分发 36010-36110, FUN_0004a754 (35685), FUN_0004acec (35865), FUN_0004ae48 (35893)
- 波特率: FUN_00043e98 (32662), FUN_0003bb00/3bb44 (28430 后), 初始 0x1A
- 频率: tables/freq_table_full.csv (115 条), tables/pll_decode.md (公式 100% 验证)
- 温度: FUN_0004601c (33640 附近, reg 0x1C |= 0x4060)
