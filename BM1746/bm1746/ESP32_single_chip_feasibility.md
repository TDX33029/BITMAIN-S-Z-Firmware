# ESP32 + 单颗 BM1746 直驱可行性核验 (基于 cgminer 逆向产物)

结论先行: **可以写出来, 单芯片直驱所需协议 100% 已从 cgminer_z15_bm1746.elf 逆出**;
不能照搬的只有"Z15 Pro 的 PL(FPGA) 中转层", ESP32 用自己的 UART 替代即可。
电源 (PIC16F1704/ISL) 按约定不在范围内, 但命令表已备齐。

## 一、已核验的完整协议要素 (全部有反编译函数支撑)

### 1. 命令帧 (11 字节, FUN_0003cdcc @ bm1746/cgminer_all_decomp.c:32200 附近)
```
[0]=0x55 [1]=0xAA [2]=0x40|(rw<<4)|0x01  0x51=链广播写(rw=1, 不带芯片地址), 0x41=带芯片地址(读, rw=0)
[3]=固定0x09 (非"保留0")  [4]=chip_addr*interval  [5]=reg  [6..9]=32bit 值 (字节内交换后 BE 发送)
[10]=CRC 校验低5位 (高3位保留)
```
注意帧第 10 字节是 `crc & 0xe0 | checksum & 0x1f` (代码里 local_c & 0xe0 恒 0, 实际= checksum&0x1f)。
(已按字节级复核修正: 2026-11-20 — FUN_0003cdcc 反汇编确认 byte[3]=9 (strb #9@[sp+0xf]);
旧文 "0x41=链广播/0x51=单芯片" 两语义写反, 实为 0x51=链广播写、0x41=带芯片地址(读)。)
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
- 先发 7 字节地址复位帧: `[55][AA][53][05][00][00][chk]` (FUN_0003cd58)
- 再对每组发 7 字节设址帧: `[55][AA][40][05][addr][00][chk]` (FUN_0003ce7c, 设址字节在帧[4])
- 单颗芯片: addr=0x00, interval=42 不再起作用 (那是多芯片链式自动递增用)
(已按字节级复核修正: 2026-11-20 — 旧文 "[55][AA][04]…" 有误, 依 FUN_0003cd58/3ce7c 反汇编:
 复位帧=55 AA 53 05 00 00 chk, 设址帧=55 AA 40 05 addr 00 chk。)

### 6. 任务帧 (bm1740_makeup_work, FUN_00042a60) — 0x92 字节
```
[0]=0x55 [1]=0xAA [2]=0x20 (恒定, 机器码 bfi #5,#3 写入)  [3]=workid  ... 任务体 0x8C=140 字节 ...
[0x90..0x91]=CRC16-CCITT(BE)
```
140 字节任务体 = **Equihash (200,9) 的 140B 区块头** (version+hashPrevBlock+hashMerkleRoot+
hashReserved+ nTime+nBits+nNonce, 与 Zcash 头一致), 由 FUN_0004eee0(ctx,200,9) "ZcashPoW" 佐证。
workid = 帧[3] = work->id (全局计数器 DAT_000770a8, FUN_0001acc4/FUN_0001a1e8) & 0x7f, 0..127;
帧[2] 恒为 0x20。
发送线程 FUN_00042b54: 从 work 队列取任务 → 构帧 → FUN_0003c150 发 0x92 字节。
(已按字节级复核修正: 2026-11-20 — 旧文 "workid 低 5 位在帧[2]、在飞≤32" 有误。机器码
 0x42cb8/0x42ccc: `and r11, #0x7f` 后入帧[3]; 0x42d00: `bfi r12,r2,#5,#3` 写帧[2]=0x20。
 下发线程维护 workid→work 映射表: 槽位 = 链结构基址+(workid+0x1c)*4+6, 共 **128 槽**, 直接按
 完整 workid&0x7f 索引 (不取低位), 槽被占则 free 旧 work 再写新 (workid 回绕 mod 128);
 RX 侧 bitmain_scanhash (FUN_0003af90.c:97-99) 以 base+workid*4+0x76 同一偏移取回 work, 与 TX
 写入一致。在飞数量限制的真正来源 = 该 128 槽映射表回绕, 不是 32。)

### 7. nonce 回包 (RX 分发循环 @ cgminer_all_decomp.c:36010-36110)
三种帧均以帧头字节对开头; **RX 字节流实测前导为 AA 55** (FPGA FIFO 字序), 与 TX 的 55 AA 相对——
直驱解析时收发不能共用同一匹配器 (已按字节级复核修正: 2026-11-20)。第 3 字节高 4 位分型:
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
2. UART @ bt8d=0x1A 低速; 切高速 (reg 0x1C bt8d=0 → PL divider 0) 在 reset_chain 内部
   (FUN_00046434.c:48-53), 即先于设址, 不是最后一步
3. 地址复位帧 [55 AA 53 05 00 00 chk]  (FUN_0003cd58)
4. 设址帧     [55 AA 40 05 addr 00 chk]  (FUN_0003ce7c, 单颗 addr=0)
5. reg 0x1C: bt8d (低速枚举用)
6. reset: PIC 断电→0.1s→PIC 上电→1s (ESP32 用 GPIO 控 EN/复位脚替代)
7. reg 0x0C: 频率 (800MHz=0x00800221, 或按公式合成)
8. reg 0x28: 核心超时 = 84000 (0x14820, DAT_00076b74 静态初值; 另: 重装链路径 FUN_0004791c 传字面值 80000)
9. reg 0x14: ticket mask 出厂静态初值 = 0x10 (DAT_00076b60, 按 diff 设置)
10. reg 0x1C: 0x4060|bt8d 逐芯片开温度回读
11. (切高速已并入第 2 步: reset_chain 内部, 先于设址)
12. 扫频线程 (可选): 775→815 二分 (上限 825=0x339), nonce 错误率+OX 状态判定
13. 开始下发 0x92 任务帧 (workid = 计数器&0x7f, 0..127 回绕)
```
(已按字节级复核修正: 2026-11-20 — 切高速时序、超时 84000、ticket mask 0x10、帧字节序, 旧文
 "核心超时=80000 / ticket mask 初始=0? / 切高速在最后" 均有误。)

### 9. 芯片计数探测 (FUN_000498f4 → check_asic_reg FUN_0004791c)
读芯片 ID: 构 0x41 带芯片地址读帧 reg?→ 回包 [2]&0xf 组号 + 芯片地址自增判定 (interval=42)。
(已按字节级复核修正: 2026-11-20 — 旧文"0x51 单读帧"与修正后语义不符: 0x51=链广播写, 带地址读=0x41。)
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
3. **ticket mask 的出厂值**: 静态初值 = **0x10** (DAT_00076b60, ELF .data 实测 16; 旧文"初始=0"有误)。
   运行期经 FUN_00044004 (set_ticket_mask, reg 0x14) 按配置改写。单芯片可按 diff 需要
   设置 (如 0xFF 全 diff)。(已按字节级复核修正: 2026-11-20)
4. **Equihash (200,9) solver**: 芯片只算 PoW 里的 Blake2b+solver, 主机侧只需校验
   (FUN_00055930 SHA256 已逆出)。ESP32 上完整校验一个 sol 需要 ~100ms 级, 可接受。
5. **PIC16F1704 电源序列**: 约定不做; 但命令码表 (0x06/07/10/15/1A/1B/心跳) 已在报告中,
   若自制供电板可复用。

## 四、给 ESP32 侧的最小固件骨架 (伪码级, 全部要素已有出处)

```c
// 1. 帧构造 (源: FUN_0003cdcc)
void cmd_frame(uint8_t *f, int rw, uint8_t chip, uint8_t reg, uint32_t val) {
  f[0]=0x55; f[1]=0xAA;
  f[2]= 0x40 | (rw?0x10:0x00) | 0x01;   // 0x51=链广播写(rw=1), 0x41=带芯片地址(读)
  f[3]=9; f[4]=chip; f[5]=reg;           // byte[3] 固定 9 (机器码确认)
  put_be32(&f[6], bswap32(val));         // 字节内交换后大端 (见函数体 uStack_14 处理)
  f[10]= lane_checksum(f+2, 8) & 0x1f;  // FUN_0003cc6c
}
// 2. PLL (源: freq_table_full.csv + pll_decode.md)
uint32_t pll_for(uint32_t mhz){ return (mhz/5)<<16 | (25*mhz/mhz>>4)<<4 | (2<<8) | 1; } // 或查表
// 3. 初始化 (源: FUN_00049cc8 流程)
uart_begin(BT8D1A_BAUD);  // ~862k 或先 115200 试
set_baud_reg(0x1A);
gpio_reset_pulse();          // 替代 PIC 断/上电; reset_chain 尾部即切高速
switch_baud(HIGH);           // reg 0x1C bt8d=0 + host 提速 (真实时序在复位内部, 先于设址)
addr_reset();  set_addr(0);  // [55 AA 53 05 00 00 chk] / [55 AA 40 05 addr 00 chk]
set_pll(0x00800221);         // 800 MHz
set_core_timeout(84000);     // reg 0x28, DAT_00076b74 静态初值
set_ticket_mask(0x10);       // reg 0x14, 出厂静态初值 (按 diff 可调高)
temp_enable(0x4060);         // reg 0x1C
// 4. 任务环 (源: FUN_00042b54)
for (;;) {
  work = pool_next_work();                     // 140B header
  tx_work_frame(workid&0x7f, work);            // 0x92 帧, 帧[3]=workid 0..127, 帧[2] 恒 0x20
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

## 六、修订记录
- 日期: 2026-11-20
- 依据: cgminer_z15_bm1746.elf (md5 709cc6f0f12c0d14987e07a13c492c02) 机器码级复核 —
  CRC16 表 256/256 PASS、频率表 115/115 PASS、帧结构指令级确认 (capstone 反汇编,
  关键函数 FUN_0003cdcc/0003cd58/0003ce7c/00042b54/0004a754/0003af90/00046434)。
- 修正条目:
  1. §1/§9: 11B 命令帧 byte[3]=固定 9 (非"保留0"); 0x51=链广播写、0x41=带芯片地址(读),
     旧文两语义写反。
  2. §5/§8: 7B 帧 — 复位帧 [55 AA 53 05 00 00 chk]、设址帧 [55 AA 40 05 addr 00 chk],
     旧文 "[55][AA][04]…" 有误。
  3. §6: workid = 帧[3] = work->id(计数器 DAT_000770a8)&0x7f (0..127), 帧[2] 恒 0x20;
     映射表 128 槽 ((workid+0x1c)*4+6 直接索引), RX 侧 FUN_0003af90 同偏移取回;
     在飞上限 = 128 槽回绕, 非 32 — 旧文"workid 低 5 位在帧[2]、在飞≤32"有误。
  4. §8/§三-3: 切高速 (reg 0x1C bt8d=0 → divider 0) 在 reset_chain 内部 (FUN_00046434.c:48-53),
     先于设址; 核心超时 = 84000 (DAT_00076b74), ticket mask 出厂 = 0x10 (DAT_00076b60);
     旧文 80000/初始=0/切高速在最后 均有误。
  5. §7: RX 字节流实测前导为 AA 55 (FPGA FIFO 字序), 与 TX 55 AA 相对, 收发不能共用同一匹配器。
