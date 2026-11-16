BM1746 串口寄存器操作完整报告 (来源: SD 恢复镜像 cgminer-z15, git branch "bm1746")
================================================================================
二进制: bm1746/cgminer_z15_bm1746.elf (422244 B, ARM32 ELF, driver-btm-zcash.c 编译产物)
反编译: bm1746/cgminer_all_decomp.c (644 函数)
SD 镜像: ANTMINER-Z15-user-800M-202007031139_6134 (V0.1.18, 2020-07-03, Zynq7000 ✓ 1746 平台)
注: 原提取时 cgminer 被杀软误杀, 已关闭杀软后直接从 ext2 镜像按 inode 恢复, 字节完整。

〇、系统调用面 (PL/FPGA 寄存器, mmap /dev/axi_fpga_dev 0x40000000, 0x1200 字节)
--------------------------------------------------------------------------------
FUN_0003b704: fd=open("/dev/axi_fpga_dev"); base=mmap(0,0x1200,RW,MAP_SHARED,fd,0)
  同时 open("/dev/fpga_mem") mmap 0x1000000 (工作下发用 DDR 窗口)
FUN_0003ba70 init_fpga:      base[0x80] = 0x8080800F; 轮询等待 bit31==0; 然后 base[0] |= 0x60000000 (硬件版本读出)
FUN_0003b994:                FPGA version = base[0] & 0xFFFF (打印 "%04X")
FUN_0003bb44 set_baud_pl(bt8d): base[0x3C] = bt8d|bt8d<<8|bt8d<<16|bt8d<<24  (4 链各 8bit)
FUN_0003bb78 check_how_many_uart_data_in_fpga(uart):
   uart0 RX 计数 = (base[0x1F0]>>16)&0x3FFFFFF; base[0x1F0]&0x3FF
   uart1: base[0x1F4]? (实际代码 500=0x1F4) ... uart4..9: base[0x1F8], base[0x1FC], base[0x200]
FUN_0003c66c/0003c708: base[0x30] = 命令(等 bit31 清零再写); 读回 = base[0x30]&0xFF
   (命令 0x02000000|cmd -> FPGA 内部命令通道)
FUN_0003c150 uart_send(chain, buf, len): 等 base[0x180+0x04*grp] 的 TX 空闲位(uart0..3: 位8/16/24/0
   即 +0x180,0x184,0x188,+0x180+0), 检查 base[0x190..0x18C 区](0x65,0x67,0x69,0x6B,0x6D,...0x77 的
   状态字), 然后 32 位字序交换后写 TX FIFO
reset_chain (FUN_00046434) 复位脉冲: base[0x34] = 1<<chain (拉低) -> sleep -> base[0x34]=0 (释放)

〇-1、目标频率分析 (结论: 800 MHz)
--------------------------------------------------------------------------------
证据链 (全部来自 cgminer_z15_bm1746.elf 反编译 + 配置):
1. SD 镜像文件名: ANTMINER-Z15-user-800M-202007031139_6134 ("user-800M")
2. etc/cgminer.conf.factory: "bitmain-freq" : "800", "bitmain-voltage" : "780"
3. 扫频线程 create_bitmain_scan_freq_pthread (FUN_000498f4 调用者) 入口 FUN_00047354:
   第一句 FUN_00043e4c(800) => 对全部在位链 set_frequency_chain(chain, 800) (0x0C 广播帧)
   随后每链参数: freq=800, 目标 hash_rate=0x32f (815), 扫频窗口 775(0x307)..815 MHz
4. 配置兜底逻辑 (bitmain_ZCASH_init, 0x4a2d8 附近):
   bitmain-freq 为 0/未配置/800 时 => DAT_0009d6f8 = 0x32000 (800<<8, 同样落到 800)
   其余值按配置写入; fan 降速阈值也用 DAT_0009d6f8 低位 (100%=满速封顶)
5. 频率表 (freq_table.txt) 含 800MHz=0x00800221, 表上限 900MHz(0x00480121)
FR-1.9 NAND 包 bitmain-freq="0" + bitmain-freq-level="100" => 走同一 800M 默认分支
频率→寄存器: FUN_000435e0 查表 0x69204 (115条×16B), 命中槽位取 pll 字段写 reg 0x0C;
            未命中默认取槽位 0x40 (587MHz@0x005E0221);
            每链生效频率存 DAT_0009d705+chain*2 (= 0x9d4b5+(chain+0x128)*2, status 显示用)
电压→寄存器: bitmain-voltage=780 => PIC16F1704 命令 0x10 编码 0x15 偏置, 且每 ASIC 经
            reg 0x20 (I2C 间接) 写 ISL 电源芯片: 合法窗 [700,800], 越界回落 700 (0x2BC)
            反编译参考: FUN_0003ea50 (set+回读校验±4mV), FUN_0003eec8 (every_asic_set_isl_voltage)

〇-2、频率值的全部修改点与设置规则 (谁改 freq、改成多少)
--------------------------------------------------------------------------------
per-ASIC 当前频率数组: DAT_0007c224[chain*3+asic] (4链×3组), 显示值在 status 区 (chain+0x128)*2
(1) 配置解析 (bitmain_ZCASH_init): bitmain-freq => DAT_0009d6f8
    0/800/未配置 => 800; 其它值原样; freq-level 控制扫频开关 (DAT_0009d6f7)
(2) 启动广播: FUN_00043e4c(800) 或配置值 => set_frequency_chain 全链 reg 0x0C
(3) 扫频线程 FUN_00047354 (freq-level>=100? 触发): 起 800 基准, 窗口 [775,815] (0x307..0x32f),
    对每 (chain,asic组) 用 set_freq_asic 逐组试频: nonce 错误率 + OX 状态判定,
    有效则二分逼近边界 (up/down), 全链 avg>800 或到达 825(0x339) 上限 => 停
(4) 保存: scan_freq_save_freq (FUN_000470a8) 写 /config/chips_freq.config (12 个十进制数一行)
    下次启动 FUN_00046cbc 读回: 值在 [775,900] 内采用, 越界一律回落 800 ("turbo mode")
    PIC 也可记忆频率 (save_freq_PIC16F1704_new / get_PIC16F1704_freq_new)
(5) 运行期重装: bitmain_ZCASH_reinit_chain (FUN_0004791c) 用 DAT_0009d705[chain] 重发 0x0C
最终唯一写入口: FUN_000435e0 (整链广播帧) / FUN_00043734 (单组帧), 帧值 = 频率表 pll 字段
表: 0x69204, 115条×16B [freq_MHz][..][..][pll], 未命中兜底槽位 0x40 (587MHz)

一、BM1746 串口帧格式 (命令帧, FUN_0003cdcc 构帧, 11 字节)
--------------------------------------------------------------------------------
偏移  长度  内容
0..1   2   前导 0xAA 0x55
2      1   帧类型 = 0x41 | (读写位<<4): 0x41=广播写(所有芯片), 0x51=单芯片写(param_2&1<<4)
3      1   (帧类型域第二字节, 与类型相关)
4      1   芯片地址 (chip_addr * asic_addr_interval; topol.conf: interval=42)
5      1   寄存器/命令码 (见下表)
6..9   4   32 位值 (字节内交换后大端发送)
10     1   校验: 低 5 位 = FUN_0003cc6c 4-bit 校验(lane 校验), 高 3 位保留
发送前置: 0x41 类型 = 广播; 单芯片寻址时 bit4 置位并给芯片地址。

二、BM1746 寄存器/命令码表 (cgminer 内使用的全部写操作)
--------------------------------------------------------------------------------
码    函数                          语义                     值域
0x0C  set_frequency_chain(chain, f) PLL 分频值(整链广播)    频率表 pll 字段
      set_freq_asic(chain, chip, f) 单芯片 PLL               同上 (chip 寻址帧)
0x14  set_ticket_mask_chain(chain)  ticket mask              8bit
0x1C  set_baud(chain, bt8d)         UART 波特率分频          (bt8d&0x1F)<<8 | 保留位
      enable_read_temperature_from_asic_chain: reg 0x1C = 0x4060|保留(开温度读回, 每芯片)
0x20  read_i2c_reg(chain, chip, reg, type) / write (FUN_0004633c):
      值 = base[0x76B6C]&0xFF000000 | 0x01000000 | (i2c_reg&0x7F)<<17 | (type&0xFF)<<8 | data
      (0x20 = I2C 间接寄存器: 芯片内 I2C master, 访问电源芯片 ISL/温度 TMP451)
      type: 0/1/0x11/0xFE 四种 I2C 通道
0x28  set_core_timeout_chain(chain, timeout)  核心超时
0x40  set_analog_mux_control_chain(chain, sel) 模拟多路开关 (温度传感器通道选择, 3bit)
0x50  set_nonce_shift_chain(chain, shift)      nonce 位移 (值<<27)

三、频率表 (0x69204, 115 条, 每条 16 字节: [freq_MHz][?][?][pll_value])
--------------------------------------------------------------------------------
freq(MHz): pll_value (写入寄存器 0x0C)
100:0x00220241  125:0x00280241  150:0x00300241  175:0x00380241  200:0x00400241
225:0x00480241  250:0x00500241  275:0x00580241  300:0x00600241  325:0x00680241
350:0x00700241  375:0x00780241  400:0x00800241  ...  800:0x00800221 (本机目标点)
精确全表见 bm1746/freq_table.txt (100..900 MHz 共 115 条完整列出)
set_frequency_chain 未命中时默认取 0x40 槽位 = 587MHz@0x005E0221。

四、链路初始化完整序列 (runtime 流程, 依 cgminer 调用序)
--------------------------------------------------------------------------------
1. init_fpga: base[0x80]=0x8080800F, 等待完成, base[0]|=0x60000000
2. 每链 (chain 0..3, 存在性检查 &DAT_0009D4BC[chain]==1):
   a. reset_chain(chain):
      - PIC 命令 5/0x15/0x1A (disable DC-DC): 串口帧 [55][AA][05][15][1A], 回显 15,01 = 成功
      - base[0x34] = 1<<chain (链复位拉低), 等 100ms
      - PIC 命令 5/0x15/0x1B (enable DC-DC), sleep 1s
      - reset_hash_board_low/high (PIC 命令 4/7: [55][AA][04][07], 回 7,01 = PIC 自身复位)
      - base[0x34]=0 释放复位
      - tty_init_chain: 创建 rx/tx 线程; base[0x3C]=0x1A (初始 baud), 发 set_baud(reg 0x1C, 0)
   b. software_set_address_chain:
      - 发 set_address 帧序列 (FUN_0003CD58: [55][AA]...固定帧) 复位芯片地址计数器
      - 对 3 个芯片组: [55][AA][04?]+FUN_0003CE7C 帧 (0x40|param<<16...) 逐组设址
        (asic_addr_interval=42 => 地址 0,42,84 ...)
   c. check_every_chain_asic_number / check_every_chain_asic_pll: 读回校验芯片数/PLL
   d. set_core_timeout_chain(0x28), set_nonce_shift_chain(0x50), set_ticket_mask_chain(0x14)
   e. set_frequency_chain(0x0C 广播) 或 set_freq_asic 逐片
   f. enable_read_temperature_from_asic_chain: reg 0x1C=0x4060 逐芯片 (开温度)
   g. set_io_drive_strength_chain / set_analog_mux_control_chain(0x40)
3. PIC16F1704 (每链电源/复位管理器, 同一 UART 半双工命令):
   [55][AA][LEN][CMD][ARG...];  已知命令:
   4/06 jump loader->app (回 06,01)
   4/07 PIC reset        (回 07,01)
   5/15/1A disable DC-DC (回 15,01)
   5/15/1B enable  DC-DC (回 15,01)
   5/10 [电压Lo][电压Hi] set_voltage (回 10,01), 电压值 = param+0x15 编码两字节
   4/09? get PIC freq
   心跳: send_heart_beat_to_every_chain (FUN_000416EC)

五、与 FR-1.9 BMU 包的关系
--------------------------------------------------------------------------------
SD 包 (2020-07, V0.1.18) 与 BMU 包 (2026-04, FR-1.9) 同为 Zynq7000 平台;
SD 包携带完整 cgminer (BM1746 驱动); BMU 包只刷系统底座。
BM1746 寄存器操作 = 本报告 + cgminer ELF; FR-1.9 的 ps7_init 表负责 PS 侧。
