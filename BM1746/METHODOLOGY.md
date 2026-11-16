# Bitmain 矿机固件逆向方法论手册

> 本文是 Z15 Pro / BM1746 完整逆向过程的方法论沉淀，面向"分析其他型号"的场景。
> 每一步都标注了：当时怎么做的、为什么这么做、踩了什么坑、在其他型号上怎么复用。
> 配套产物：`reversed/`（全部函数+分类）、`bm1746/`（协议报告）、`tables/`（常量表）。

---

# 第 0 章 总路线图（先背下来这张图）

```
原始固件文件 (.bmu / .tar.gz / .img)
   │
   ├─ 1. 容器层：读懂自描述结构，零猜测切片
   │      BMU 头 → RSA公钥 → 成员表 → 7个成员
   │
   ├─ 2. 启动链层：BOOT.bin → bootgen分区表 → FSBL / 位流 / (uImage)
   │      确认主控 SoC 型号 (zynq7007 / amlogic / allwinner ...)
   │
   ├─ 3. 文件系统层：ext2/squashfs 手写解析器，挖配置
   │      topol.conf → 芯片型号/链数/间隔  ← 这是"逆向目标"的宣言书
   │      S70cgminer → 找真正干活的二进制  ← 找不到就说明缺包
   │
   ├─ 4. 找齐"真正干活的二进制"
   │      升级包缺 → 找完整包/SD恢复镜像/opkg数据库
   │      没有就先逆向现有最大 ELF + 内核模块
   │
   ├─ 5. Ghidra 批量反编译（headless + 自定义脚本）
   │      DumpAll(单文件) + ExportFuncs(单函数+调用图+字符串索引)
   │
   ├─ 6. 字符串锚点法定位
   │      __func__ 名字 → 函数体 → 数据表 → 寄存器码 → 调用图闭合
   │
   ├─ 7. 常量验证（不过验证的东西不写进报告）
   │      CRC 表全量复算 / PLL 公式逐条验算 / 帧长多处印证
   │
   └─ 8. 交付组织：按功能分类 + 报告 + 重写路线图
```

---

# 第 1 章 容器层：BMU 包解剖

## 1.1 第一步永远是"读头"，不是"搜工具"

拿到 `FR-1.9(260427-Z15 Pro).bmu`（19,333,763 字节），第一件事是**用 Python 把前 4KB 结构化打印出来**，而不是去找现成的解包工具（Bitmain 的容器格式没有现成工具，而且大部分固件格式本质上都是"头+表+数据"三板斧）。

```python
import struct
d = open("xxx.bmu","rb").read()
# 先看整体熵和特征
print(d[:64].hex())
# 结构化试探：u32 队列
for off in range(0, 0x400, 4):
    v = struct.unpack_from('<I', d, off)[0]
    ...
```

**读出了什么：**
- `0x000`: 版本 u32 0x0126 + ASCII 日期 "20260427"
- `0x01D..0x1DB`: 464 字节高熵块 → 长度是 RSA-2048 公钥 DER 的典型尺寸，base64 解码验证头尾 → 确认是 PEM 公钥
- `0x410`: 256 字节 = 签名块（后面 `miner.pem.sig` 字符串佐证）
- `0x518`: **规律结构** `[u8 idx][u32 BE size]` 重复 7 次 → 成员表

## 1.2 关键思想：容器是自描述的，偏移零猜测

成员表本身给出了每个成员的偏移和长度：

```
[0] BOOT.bin          size 0x2A8CC0  @0x000800
[1] devicetree.dtb    size 0x001F4E  @0x2A94C0
[2] uImage            size 0x3DE8E8  @0x2AB40E
[3] minerfs.image.gz  size 0x7CFD8F  @0x689CF6
[4] update.image.gz   size 0x4156CA  @0x0E59A85
[5] crl.tar.gz        size 0x000228  @0x126F14F
[6] miner.btm.tar.gz  size 0x00070C  @0x126F377
```

按表精确切片到 `parts/`，尾部 2048 字节随机填充另存为 footer。**验证方法**：每个切片用 `file`/gunzip -t/dtb magic 交叉验证类型，全部命中即证明表读对了。

## 1.3 其他型号的复用要点

| 容器特征 | 识别方法 |
|---|---|
| Bitmain BMU (.bmu) | 文件头 ASCII 日期 + RSA 公钥 + `[u8][u32BE]` 成员表 |
| Antminer 老式 .tar.gz | 直接 tar tf 看 |
| Xilinx BOOT.bin | bootgen 分区表 @0x0C80 附近，分区头含 image id (0x240/0x250/0x260) |
| Amlogic | `.aml` 头 / 魔数 `AMLC` |
| Inno/微比特 | 各自魔数，先 `binwalk` 扫一遍魔数，再手读结构 |

**通用口诀**：binwalk 扫魔数给线索 → 手工验证结构 → 找到"表"就找到了切割清单。绝不基于猜的偏移切数据。

---

# 第 2 章 启动链层：BOOT.bin 三层拆解

## 2.1 bootgen 分区表

BOOT.bin 内部结构（Xilinx 系列）：
- 0xC80 附近：分区头表，每项含 image id + 数据偏移 + 长度
- PH0 id 0x240 = FSBL（33KB）
- PH1 id 0x250 = 应用 ELF（这里叫 `Zynq7007_miner.elf`，521KB）
- PH2 id 0x260 = PL 位流（加载地址 0x4000000 = PL 侧）

**FSBL 内嵌 BIF 描述符字符串**是主控铁证："Zynq7007_miner.elf"/"Zynq7007_miner.bit" 直接写着型号。这一步的意义：

1. **确认主控**（Zynq-7007S），后续所有寄存器分析都在这个框架内
2. **发现 FSBL 里藏着两个宝贝**：ps7_init 配置表（540 条寄存器序列）和内嵌 PL 位流
3. **发现 u-boot 主体不在 BOOT.bin 里**——分区表只有 3 项，这直接决定了后面"启动链分析只做到 FSBL 为止"

## 2.2 位流的独立价值

PL 位流（254KB, 7z007s 尺寸特征）单独保存为 `Zynq7007_bitstream.bin`。Xilinx PCAP 格式有自己的包结构（Type1/Type2 包、寄存器地址+值对），后续用脚本解析出 `pcap_regs.json`——**位流就是 PL 侧的"ps7_init"**，重写固件时 PL 部分要么原样复用要么重新综合。

## 2.3 其他型号的复用要点

- 非 Xilinx 平台（S19 系列 Amlogic 等）：BOOT 层换成 u-boot + ATF + kernel 三件套，思路一样——**找分区表 → 切片 → file 验证 → 找最大的应用二进制**
- FSBL/uboot 阶段的配置表（类似 ps7_init）永远值得提取，它是"PS/SoC 侧寄存器初始化的权威答案"

---

# 第 3 章 文件系统层：手写 ext2 解析器

## 3.1 为什么手写

Git Bash on Windows 无法 mount ext2，`7z` 只能解小文件或部分结构。**手写解析器 150 行 Python**：
1. 超级块 @1024：读 magic 0xEF53、inode 数、每组 inode 数、块大小
2. 组描述符表：每组 inode 表位置
3. inode 解析：i_block[15] = 12 直接块 + 1 一级 + 1 二级 + 1 三级间接
4. 目录遍历：dirent = u32 inode + u16 reclen + u8 namelen + name

## 3.2 踩过的坑（经典，记住）

- **dirent 开头是 u32 inode**，我第一版按两个 u16 解析 → 全部错位。教训：先看 ext2 规范的字段宽度，别按感觉打包
- 超级块字段偏移读错（s_inodes_per_group）→ 除零崩溃。教训：解析器先在已知小镜像上测试

## 3.3 挖什么：目标清单

| 文件 | 价值 |
|---|---|
| `/etc/subtype` | 机器码/主控确认 ("zynq7007_BEZ36501") |
| `/etc/topol.conf` | **芯片型号/链数/每链芯片数/地址间隔/电压/传感器型号** —— 逆向目标宣言书 |
| `/etc/cgminer.conf.factory` | 出厂频率/电压默认值 |
| `/etc/init.d/S70cgminer` | **真正干活的二进制路径**（/usr/bin/godminer） |
| `/lib/modules/*.ko` | 内核模块（寄存器窗口的物理地址） |
| `/usr/lib/opkg/info/*` | 已安装软件包清单（发现"缺包"的线索） |

## 3.4 决定性发现：缺包

S70cgminer 指向 `/usr/bin/godminer`，但这个文件**不在 minerfs 里**。结合 devicetree 里的 NAND 分区表（upgrade-file 分区 0x7000000），推断：升级包只刷系统底座，矿机应用由"完整包"单独提供。

**推论**：要拿到 ASIC 驱动代码，必须找到另一个包。这就是后面转向 SD 恢复镜像的原因。

**其他型号复用**：S19 的固件包里 godminer 是带符号的完整 ELF（ 甚至能直接看到函数名），Z15 的被拆走了。先 opkg 列表 + find 大文件，快速判断"驱动在不在包里"。

---

# 第 4 章 找齐干活的二进制：SD 恢复镜像

## 4.1 判定平台

SD 镜像解包出 `fw.tar.gz` → BOOT.bin / dtb / uImage / uramdisk.image.gz(100MB ext2)。
判定 BM1746 平台的三条证据：
1. uImage/dtb 字符串 "Zynq7000"
2. uramdisk 里 cgminer 的 git branch 字符串 = **"bm1746"**
3. 文件名 ANTMINER-Z15-user-800M + topol.conf chip_type 0x1746

## 4.2 杀软问题与 inode 抢救

提取时 cgminer 被杀软落盘时吃掉（目录树里有名字没内容）。解法：
1. 用户关闭杀软
2. **不重新走目录树**，直接在 ext2 原始镜像里定位 cgminer 的 inode → 按块链读出原始字节 → md5 验证完整

**教训**：中间产物被第三方进程破坏时，回到最原始数据层重提取，并带哈希验证。

## 4.3 其他型号复用

- Bitmain 各型号几乎都有 SD 恢复镜像流出，里面的 cgminer/godminer 往往**更老但更完整**
- opkg 数据库（/usr/lib/opkg/status）列出全部包名，对照磁盘找缺失项
- 如果两个包都拿不到完整驱动：**升级包的 update.image.gz 里的 FileParser + 升级脚本 + 任何 ELF 都有价值**（签名链/烧写流程/分区布局），先逆向有的部分

---

# 第 5 章 Ghidra 批量反编译

## 5.1 环境与调用

```
Ghidra 12.2 + jdk-25，headless 模式：

analyzeHeadless.bat <项目目录> <项目名> \
  -import <ELF> \
  -processor ARM:LE:32:v7 -cspec default \
  -loader BinaryLoader -loader-baseAddr 0x04000000 \   # 只有裸 bin 需要
  -scriptPath <脚本目录> \
  -postScript DumpAll.java <输出目录>
```

**踩过的坑（全部记录在案）：**
- `-cspec gcc` 对 ARM:LE:32:v7 不支持 → 用 `-cspec default`
- 裸二进制（.bin）必须给 `-loader-baseAddr`，否则指针/表全乱
- Windows GBK 编码下 subprocess 输出 → `errors='replace'`
- 项目里已有程序时用 `-process <程序名> -noanalysis` 免得重复分析
- Ghidra 脚本是 Java，**API 签名因版本而异**（isUndefined() 不存在、CreateStringCmd.applyTo(program) 不带 monitor、getReferenceIterator 参数不同）——报错看 Ghidra 打印的第一条 error 行，缺什么补什么 import

## 5.2 两个自定义脚本

**DumpAll.java**：所有函数 → 单个 all_decomp.c，每函数带 `//==== FUN_xxx @ addr ====` 头。用途：grep 全文检索。

**ExportFuncs.java**（主力）：
1. 每函数一个 .c 文件（文件名=函数名）
2. `_callgraph.txt`：调用边 `A -> B`（从 getCalledFunctions 取）
3. `_string_xref.txt`：字符串 → 引用它的函数（遍历已定义字符串，getReferencesTo）
4. `_func_index.txt`：函数 → 地址

**为什么每函数一个文件**：后续人工阅读、AI 辅助分析、按类归档都以此为单元；单文件 1.5MB 没法工作。

## 5.3 其他型号复用

- Zynq/ARM 系：`ARM:LE:32:v7` + ELF 基址自动
- 裸 bin（uboot、固件段）：必须 BinaryLoader + baseAddr，常见基址 0x40000000/0x8000
- Cortex-M 老平台：换 processor 型号即可，流程完全一样
- **对内核模块 .ko**：直接导入，Ghidra 识别 ELF reloc，符号没 strip 时连函数名都是真的

---

# 第 6 章 字符串锚点法（核心方法论）

## 6.1 原理

C 编译的固件有三个"送分"来源：
1. **`__func__` / `__FUNCTION__` 宏**：每个函数把自己的名字编进 .rodata → grep 到名字就 grep 到了函数
2. **snprintf/printf 日志格式串**：参数序 = 函数签名
3. **错误消息**：把分支语义写得明明白白

## 6.2 实战流程（以 PLL/频率为例，全程复盘）

### 第 1 步：全量 strings

```python
import re
d = open("cgminer.elf","rb").read()
for m in re.finditer(rb'[\x20-\x7e]{5,}', d):
    print(hex(m.start()), m.group().decode())
```
输出里直接看到：
```
0x611f0 set_frequency_chain
0x61934 set_freq_asic
0x61aa8 create_bitmain_scan_freq_pthread
0x639fc /config/chips_freq.config
0x60e14 set_PIC16F1704_voltage_new
```
**10 分钟内，协议地图成形**：频率、电压、PIC、扫频、持久化，五大功能域全部有名字。

### 第 2 步：字符串 → 函数反查

在反编译文本里 grep `"set_frequency_chain"`，命中 `FUN_000435e0`：
```c
snprintf(acStack_1010, 0x1000, "chainid%d %s freq=%d", param_1,
         "set_frequency_chain", param_2);
```
日志串直接给出签名：**f(chain_id, freq_MHz)**。

### 第 3 步：函数体内找数据引用

紧邻日志的就是查表循环：
```c
iVar2 = 0;
do {
    iVar1 = iVar2 * 4;              // u32 索引步进
    iVar3 = iVar2 << 4;             // 字节步进 = 16
    iVar2 = iVar2 + 1;
    if (param_2 == (&DAT_00069204)[iVar1]) goto LAB_00043694;
} while (iVar2 != 0x73);            // 0x73 = 115 次
iVar3 = 0x40;                       // 未命中兜底槽位 0x40
```
**两个数字定表**：步长 16 字节、循环 115 次 → `0x69204` 处有 115×16B 的表。兜底槽位 0x40 也要记录（后面算出是 587MHz）。

### 第 4 步：虚拟地址 → 文件偏移

Ghidra 报的是虚拟地址，读文件必须换算：
```python
def v2o(v):
    for i in range(e_phnum):
        t, off, va, _, fsz = unpack('<IIIII', phoff+i*32)
        if t == 1 and va <= v < va+fsz:
            return off + (v - va)
```
0x69204 → 文件 0x61204。**这步不做，读到的全是错位数据。**

### 第 5 步：Python 抠表

```python
o = v2o(0x69204)
for i in range(115):
    f, b, c, pll = struct.unpack_from('<IIII', elf, o + i*16)
```
第一列立刻认出是频率（100,125,...,900），第四列是给寄存器的值——**表自我解释**。field_b/field_c 暂时存疑（后来也没完全用上，诚实标注）。

### 第 6 步：定位寄存器号

表命中后取值 + 构帧：
```c
DAT_00076b58 = *(undefined4 *)((int)&DAT_00069210 + iVar3);  // 0x69210 = 表基址+0x0C
FUN_0003cdcc(&frame, 1, 0, 0xc, DAT_00076b58);               // 第4参数 = reg 码
FUN_0003c150(chain, &frame, 0xb);                            // 11 字节串口发送
```
构帧函数 `FUN_0003cdcc(buf, rw, chip, reg, val)` 的参数序**不是猜的**：grep 全部几十个调用点，第 4 参数反复出现 0x0C/0x14/0x1C/0x20/0x28/0x40/0x50，每个都对应一个语义明确的函数（set_frequency/ticket_mask/baud/i2c/timeout/mux/nonce_shift）——**寄存器码表靠调用点对齐锁定**。

### 第 7 步：破解 PLL 公式

观察表：800M=0x00800221 与 400M=0x00800241 高 16 位相同 → 高半字独立于低半字。
假设 `fout = 25MHz × N/(P×Q)`，N=pll[31:16]、P=pll[7:4]、Q=pll[11:8]（25MHz 是 Bitmain 惯用参考钟）。
Python 对 **115 条全部验算，115/115 通过，最大误差 <1.5MHz** → 公式成立。
**这条纪律贯穿始终：不过全量验证的"破解"不写进报告。**

### 第 8 步：确认目标频率（多证据合围）

1. 文件名 `user-800M`
2. `cgminer.conf.factory`: `"bitmain-freq":"800"`
3. 扫频线程入口 `FUN_00047354` 第一句 `FUN_00043e4c(800)`
4. 兜底逻辑 `freq==0 || freq==800 → 800`（解释了 NAND 包 freq=0 的含义）

四条独立证据同一答案 → 800MHz 结论可写入报告。

### 第 9 步：调用图双向闭合

- 向上：谁调 FUN_000435e0？→ FUN_00043e4c（全链包装）、FUN_0004791c（重初始化，用每链数组 DAT_0009d705[chain]）
- 向下：它写哪个寄存器？→ reg 0x0C，值 = 表[slot].pll
- 每链数组的寻址巧合：`(&DAT_0009d4b5 + (chain+0x128)*2)` 换算 = `0x9d705 + chain*2` → Ghidra 对同一内存给了两个标签，对齐后全局内存地图又准确了一块

## 6.3 更多锚点案例（复盘用）

| 目标 | 锚点字符串 | 位置 |
|---|---|---|
| 串口帧构造 | `"set_frequency_chain"`（反查） | FUN_0003cdcc |
| 枚举/计数 | `"check_asic_reg"`、`"chain J%d has %d ASIC"` | FUN_0004791c |
| nonce 处理 | `"receive a error nonce"`、`"nonce fifo full"` | FUN_0004a754 |
| 扫频 | `"scan freq done, avg freq %d max %d, reboot cgminer"` | FUN_00047354 |
| 持久化 | `/config/chips_freq.config` | FUN_00046cbc/470a8 |
| PIC 命令 | `set_PIC16F1704_voltage_new` 等 25 个函数名 | 0x60ce0 区 |
| 哈希 | `"ZcashPoW"`、SHA256 常量 0x243185be | FUN_0004eee0 |
| 升级工具 | FileParser 的 usage 字符串 `-s/-f/-p/-n/-x` | FUN_000108f8 |

## 6.4 帧格式还原的标准动作

拿到构帧函数后：
1. **逐字段标注**：`[0x55][0xAA][type][rsv][addr][reg][value BE][checksum]`
2. **找收发对偶**：TX 的构帧函数 vs RX 的分发循环（0xAA55 前缀过滤 + 类型高 4 位 switch）
3. **长度定界**：发送长度 0xb / 0x92 / 0x5d，接收缓冲循环取 0x5d / 9
4. **校验算法**：短帧逐位异或（FUN_0003cc6c），长帧 CRC16-CCITT（查表 0x68aec）
5. **CRC 表验证**：Python 生成标准 CRC16-CCITT 表，与 ELF 数据逐项比对（256/256 中）

---

# 第 7 章 全局内存地图（Ghidra DAT_ 全景）

反编译里满地 `DAT_xxxxxx`。处置方法：
1. **按地址排序**所有 DAT_ 引用，聚类出：链信息数组（0x9d4bc..）、每 ASIC 状态（0x7c130..0x7c254）、配置缓存（0x9d6f2..）、FPGA 窗口指针（DAT_00078e10）
2. **寻址巧合对齐**：两个不同表达式算出同一地址（如 0x9d705 = 0x9d4b5+(0+0x128)*2）→ 标签合并，地图补全
3. **只记录"有语义的"**：能从调用点推断用途的才进报告，纯临时变量不管

---

# 第 8 章 验证纪律

| 断言 | 验证方式 | 结果 |
|---|---|---|
| BMU 成员表切片正确 | 每片 file/gunzip -t/dtb magic | 全过 |
| CRC16 多项式 | 256 项全量复算 vs ELF 数据 | 256/256 |
| PLL 公式 | 115 条逐条验算 | 115/115, <1.5MHz |
| 任务帧=140B Equihash 头 | "ZcashPoW" 串 + N=200,K=9 参数 + 帧长三处独立印证 | 闭环 |
| 目标频率 800M | 文件名+配置+代码硬编码+兜底逻辑 | 4 证据 |
| cgminer 提取完整 | md5（绕过杀软后） | 与源镜像一致 |
| nonce 帧块结构 | workid 连续性校验代码 + CRC 位置 | 结构级 |

**未完全解决的诚实标注**：0x5B 块内 bit 级字段、bt8d=0 的精确波特率、field_b/field_c 语义——都写进"缺口"章节，不冒充已解决。

---

# 第 9 章 交付组织（对"换型号"最重要的一章）

```
BM1476/
├── SUMMARY.md                    # 容器+解包+频率结论总报告
├── original/                     # 原始固件存档（gitignore 大文件）
├── parts/                        # 容器成员原件
├── rootfs_1/ rootfs_2/           # 文件系统解包
├── sdcard/                       # SD 镜像解包
├── bm1746/
│   ├── BM1746_uart_register_ops.md   # 协议核心报告（帧/寄存器/PIC/频率）
│   ├── ESP32_single_chip_feasibility.md  # 可行性核验+最小固件骨架
│   ├── cgminer_z15_bm1746.elf        # 恢复的二进制
│   └── cgminer_all_decomp.c          # 全量反编译单文件
├── reversed/
│   ├── cgminer/                  # 644 函数 × 16 功能类
│   ├── fsbl_funcs/ fileparser_funcs/ monitor_funcs/ uboot_funcs/ ko_*/
│   ├── tables/                   # 频率表CSV + PLL公式 + I2C表 + 拓扑 + 函数名目录
│   ├── protocols/                # 协议字符串
│   └── README.md                 # 逆向总索引 + 重写路线图
└── analysis/                     # Ghidra 工程（本地）+ out/（报告与表）
```

**功能分类的半自动方法**（可复用到任何型号）：
1. 每函数的字符串引用按关键词规则加权打分（freq→12类、vol/pic→13类、temp/fan→14类…）
2. 无字符串函数用调用图传播（调用者类别的 2 票 + 被调者 1 票，≥2 票才归类）
3. 命名暗示兜底（FUN 名不含信息时跳过，保持 70_no_strings）
4. **人工抽查核心类别**（串口/频率/PIC）确认无误即可

---

# 第 10 章 换型号时的检查清单（S19/S21/BM1398 等）

1. [ ] 容器读头：BMU 老格式（.tar.gz 整包）直接 tar；新格式先读头找表
2. [ ] 主控确认：dtb/uboot 字符串 + SoC 特征
3. [ ] topol.conf / 等价配置：芯片型号、链数、间隔、电压、传感器
4. [ ] 找齐驱动二进制：cgminer/godminer（S19 系常在 minerfs 里，带符号！）
5. [ ] Ghidra 批量：DumpAll + ExportFuncs 两件套
6. [ ] strings 地图：__func__ 名字、/dev/ 节点、/config/ 路径、conf 键名
7. [ ] 锚点定位三件套：构帧函数 → 寄存器码表 → 数据表（频率表/电压表）
8. [ ] PIC 命令表：set/get_PIC_voltage + dc_dc + 心跳（各代 PIC 命令码有差异）
9. [ ] 验证纪律：CRC 表全量复算、频率表公式验算
10. [ ] 交付：报告 + 分类函数 + 表 + 重写路线图

**各型号已知差异速查**：
- BM1398/BM1370（S19 系）：驱动在 godminer ELF，带符号，函数名即文档；PIC 换代命令码不同；频率表在同代 cgminer 数据段
- BM1746（Z15/Z15 Pro）：本文全程；驱动在 SD 恢复镜像
- BM1485/BM1120 等老芯片：cgminer 开源版本可对照，逆向工作量减半
- 串口帧基本格式（55 AA + 类型 + 地址 + reg + 值 + 校验）跨代稳定，主要差异在 reg 码表与校验算法代次

---

# 附录 A：全套工具脚本清单

| 脚本/工具 | 位置 | 用途 |
|---|---|---|
| DumpAll.java | analysis/scripts/ | 全函数单文件导出 |
| ExportFuncs.java | analysis/scripts/ | 单函数+调用图+字符串索引 |
| DefineStrings.java | analysis/scripts/ | 裸 bin 先定义字符串再导出 |
| v2o() | 各 Python 段 | 虚拟地址→文件偏移 |
| ext2 解析器 | 会话内脚本 | 手写 inode/目录遍历 |
| BMU 切片器 | 会话内脚本 | 成员表→精确切片 |
| CRC/PLL 验算 | 会话内脚本 | 表全量复算 |

# 附录 B：踩坑全集

1. ext2 dirent 是 u32 inode 开头，不是两个 u16
2. Ghidra `-cspec gcc` 不支持 ARM v7 → default
3. 裸 bin 必须 -loader-baseAddr
4. Ghidra Java API 因版本变化（isUndefined/applyTo/getReferenceIterator）
5. 杀软会落盘吃掉可疑二进制 → 关杀软 + 按 inode 重提取 + md5
6. 虚拟地址直接当文件偏移读 = 全错
7. Ghidra 对同一地址的 DAT_ 标签可能因引用方式不同而不同（0x9d4b5+0x128*2 vs 0x9d705）
8. 反编译器的 catchall 规则会淹没真实分类（`.` 匹配一切 → 打分全输给它）
9. Windows GBK 控制台编码 → subprocess errors='replace'
10. `__func__` 字符串只覆盖有日志的函数；库函数/内联函数没有名字，需按调用图传播归类
