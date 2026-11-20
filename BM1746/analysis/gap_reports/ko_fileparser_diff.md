# BM1746 ko / FileParser 差异报告

- 仓库根：`/d/Document/tempPrj/BITMAIN-S-Z-Firmware`
- 分析对象范围：`rootfs_1` vs `sdcard/rootfs_sd` 的两个 ko；`rootfs_1` vs `rootfs_2` 的 `FileParser`
- 证据来源：`file`、`md5sum`、`readelf -h/-S/-s/-d/-l`、`readelf -p .modinfo`、`strings`、`diff`，并参考既有反编译索引与升级脚本
- 既有反编译：`BM1746/reversed/ko_bitmain_axi/_func_index.txt`（5 函数）、`BM1746/reversed/ko_fpga_mem/_func_index.txt`（5 函数）、`BM1746/reversed/fileparser_funcs/_func_index.txt`（42 函数，均对应 `rootfs_1` 版本）

## 1. bitmain_axi.ko（rootfs_1 vs rootfs_sd）

| 项目 | rootfs_1 | rootfs_sd (sdcard) |
|---|---|---|
| 路径 | `BM1746/rootfs_1/lib/modules/bitmain_axi.ko` | `BM1746/sdcard/rootfs_sd/lib/modules/bitmain_axi.ko` |
| 大小 / MD5 | 7428 B / `5d00a362d368efc009dd79aa53723a0d` | 7587 B / `f2517c6f9554babeb1cfca7ac55aed2d` |
| ELF | REL, ARM EABI5, not stripped, BuildID `178cddb4...` | REL, ARM EABI5, not stripped, BuildID `70280de7...` |
| vermagic (`.modinfo`) | `4.6.0-xilinx-g03c746f7 SMP preempt mod_unload modversions ARMv7 p2v8` | `4.6.0-xilinx-g0387054-dirty SMP preempt mod_unload modversions ARMv7 p2v8` |
| depends / license | `depends=` / `Dual BSD/GPL` | 同左 |
| description | `75303c0` | 无 `description` 字段 |
| 编译器线索 (`.comment`) | `GCC: (Linaro GCC 7.2-2017.11) 7.2.1 20171011` | `GCC: (crosstool-NG linaro-1.13.1-4.7-2012.11-20121123 - Linaro GCC 2012.11) 4.7.3 20121106 (prerelease)` |
| 源文件/符号 | `axi_fpga.c`；定义函数：`axi_fpga_dev_open/release/mmap/init/exit` + `init_module/cleanup_module` | 同源文件与同名函数集合 |
| 关键字符串 | `axi_fpga_dev`, `axi_fpga_vir_mem`, `In axi fpga driver!`, `Err:failed in creating axi fpga class.`, `!!!*base_vir_addr = 0x%x`, `AXI fpga dev virtual address is 0x%x`, `request_mem_region ...`, `ioremap ...`, `axi_fpga_dev_mmap error!` | 与 rootfs_1 一致（无新增设备名/流程字符串） |
| ioctl/mmap 线索 | 可见 `axi_fpga_dev_mmap` / `remap_pfn_range`；未见 ioctl 相关字符串或 `unlocked_ioctl` 符号名 | 同左 |
| 已知函数大小（符号表） | `open=8, mmap=92, release=8, init=428, exit=104` | `open=8, mmap=84, release=8, init=424, exit=104`（布局顺序略异） |
| 额外差异 | — | 多导入 `__aeabi_unwind_cpp_pr1`；`__versions` 段更大（1408 vs 1344） |

**差异摘要**：两者为同一驱动的不同构建（内核 git hash/dirty 状态、工具链、BuildID、`description` 元数据不同），功能/接口层字符串与符号集合一致；仅存在少量代码尺寸与节布局差异，未发现新增设备节点名、mmap 路径或 ioctl 命令线索。

## 2. fpga_mem_driver.ko（rootfs_1 vs rootfs_sd）

| 项目 | rootfs_1 | rootfs_sd (sdcard) |
|---|---|---|
| 路径 | `BM1746/rootfs_1/lib/modules/fpga_mem_driver.ko` | `BM1746/sdcard/rootfs_sd/lib/modules/fpga_mem_driver.ko` |
| 大小 / MD5 | 7916 B / `fc136134f281da0bd6a03b50fd194a7f` | 8098 B / `990edb3456f71c8e2a7a405af6e97c57` |
| ELF | REL, ARM EABI5, not stripped, BuildID `138471bc...` | REL, ARM EABI5, not stripped, BuildID `19d7a228...` |
| vermagic (`.modinfo`) | `4.6.0-xilinx-g03c746f7 SMP preempt mod_unload modversions ARMv7 p2v8` | `4.6.0-xilinx-g0387054-dirty SMP preempt mod_unload modversions ARMv7 p2v8` |
| depends / license | `depends=` / `Dual BSD/GPL` | 同左 |
| description | `2129149` | 无 `description` 字段 |
| 模块参数 | `parmtype=fpga_mem_offset_addr:int`；符号含 `__param_fpga_mem_offset_addr` / `param_ops_int` | 同左（参数名与类型一致） |
| 编译器线索 (`.comment`) | GCC 7.2.1 (Linaro 2017.11) | GCC 4.7.3 (crosstool-NG Linaro 2012.11 prerelease) |
| 源文件/符号 | `fpga_mem.c`；定义函数：`fpga_mem_open/release/mmap/init/exit` + `init_module/cleanup_module` | 同源文件与同名函数集合 |
| 关键字符串 | `fpga_mem`, `fpga_vir_mem`, `In fpga mem driver!`, `Err:failed in creating fpga mem class.`, `fpga mem virtual address is 0x%x`, `fpga_mem_mmap error!`, `request_mem_region ...`, `ioremap ...` | 与 rootfs_1 一致 |
| ioctl/mmap 线索 | 可见 `fpga_mem_mmap` / `remap_pfn_range`；未见 ioctl 相关字符串或命令号 | 同左 |
| 已知函数大小（符号表） | `open=8, mmap=104, release=8, init=440, exit=112` | `open=8, mmap=100, release=8, init=436, exit=112` |
| 额外差异 | — | 多导入 `__aeabi_unwind_cpp_pr1`；`__versions` 更大（1472 vs 1408） |

**差异摘要**：同样是同一驱动针对不同内核/工具链的重编译。设备名 `fpga_mem`、模块参数 `fpga_mem_offset_addr`、mmap 接口与初始化/错误路径字符串保持一致；未见 ioctl 接口迹象。

## 3. FileParser（rootfs_1 vs rootfs_2）

| 项目 | rootfs_1 | rootfs_2（升级 ramfs 版） |
|---|---|---|
| 路径 | `BM1746/rootfs_1/usr/bin/FileParser` | `BM1746/rootfs_2/usr/bin/FileParser` |
| 大小 / MD5 | 23552 B / `f4d8cabd84af2c149e14c4c9b1a46ee0` | 15720 B / `56358ea4885bf8836a13c13ede685c94` |
| ELF | EXEC ARM EABI5 hard-float，dynlinked，interp `/lib/ld-linux-armhf.so.3`；**节头表损坏/缺失**（`readelf -S/-s/-d` 报错或动态段异常），文件尾非标准 | EXEC ARM EABI5 hard-float，dynlinked，`for GNU/Linux 3.2.0`，BuildID `7a2f27d0...`，**stripped**，节/段/动态段完整 |
| 依赖/符号 | strings 可见 `libcrypto.so.1.1`、`libc.so.6` 及 OpenSSL(RSA/SHA256/BIO/PEM/ERR)、文件 IO 等导入名 | `NEEDED libcrypto.so.1.1`, `libc.so.6`；dynsym 仅导入符号，能力集一致（RSA_verify/SHA256/PEM_read_bio_RSA_PUBKEY 等） |
| CLI/行为字符串（共有） | `-f/-s/-p/-x/-n/-q`；`-s` 拆 BMU 到 `/tmp/tmpfw/`；`-n` 拆 sigImg 到 `/tmp/tmpNand/`；`-q` 生成 `/tmp/256BFF`；`/usr/bin/fw_version`；校验/输出：`Check miner.pem Failed!`、`Check pem payload failed!`、`Load Pem Failed!`、`Check File Sig failed!`、`Dump Miner.pem(.sig) Failed!` 等 | 同左（参数语义与主要工作路径字符串一致） |
| BMU/镜像内容线索（共有） | `'%s' Not A Btmu File!`；成员名：`BOOT.bin devicetree.dtb uImage minerfs.image.gz update.image.gz crl.tar.gz miner.btm.tar.gz`；签名后缀 `.sig`；nand 拆分输出：`/tmp/tmpNand/{devicetree.dtb.sig,uImage.sig,minerfs.image.gz.sig,update.image.gz.sig,miner.btm.sig,crl.sig,7kreserve,miner.btm,crl}` 及 `/tmp/unused` | 同左 |
| 字符串差异（rootfs_1 独有） | `miner_type %s`、`miner_type_hash %16llx, bmu_hash %16llx`、`input miner_type and bmu miner type donot match!` | **缺失**上述三条 miner_type/hash 校验相关字符串 |
| 烧写/mtd 路径 | 二进制字符串中未直接出现 `/dev/mtd*`；实际 NAND 擦写由升级脚本完成（见下） | 同左 |

### 3.1 rootfs_2 在升级流程中的角色（`rootfs_2/etc/init.d/Update.sh`）

`rootfs_1` 与 `rootfs_2` 的 `Update.sh` 内容完全一致（`diff` 无差异），流程为：
1. `FileParser -s S19 $1/update.bmu /etc/bitmain.pub`：校验并拆分 BMU 到 `/tmp/tmpfw/`。
2. 脚本内用 `openssl dgst -sha256 -verify` 对 `miner.pem`、各镜像 `*.sig`、`crl`、`miner.btm` 做二次验签。
3. `dd if=/dev/mtd3 ...` 导出 NAND sig 区后 `FileParser -n /tmp/nandBin` 拆分到 `/tmp/tmpNand/`；再 `FileParser -q` 生成 `/tmp/256BFF` 并重组 `sigImage`。
4. `flash_erase` + `nandwrite` 将 `sigImage`、`BOOT.bin`、`devicetree.dtb`、`uImage`、`minerfs.image.gz`、`update.image.gz` 写入对应 MTD 分区/偏移（含备份区）。

**角色结论**：`rootfs_2` 的 `FileParser` 是升级/恢复 ramfs 环境中的 BMU 拆包、签名载荷提取与 NAND sig 镜像拆装工具（`-s/-n/-q` 为脚本依赖的关键能力）；其在升级脚本中的调用方式与 `rootfs_1` 完全相同。

### 3.2 行为差异判断

- **共同点**：CLI 选项、BMU/sig 产物命名与目录、NAND sig 拆分布局、OpenSSL 1.1 依赖与签名校验相关能力表征一致；脚本级升级流程无差异。
- **主要差异**：`rootfs_1` 含 miner_type 与 BMU hash 匹配校验提示/逻辑线索，`rootfs_2` 未见对应字符串，且体积更小、为 stripped 正常 ELF；`rootfs_1` 版本存在节头损坏痕迹但 `rootfs_1/reversed/fileparser_funcs/` 已有 42 个函数的反编译产物可用。
- **含义**：`rootfs_2` 很可能是面向升级场景的精简构建（去除 miner_type 绑定检查或相关分支），但对本仓库 `Update.sh` 所覆盖的 `-s/-n/-q` 路径应保持兼容；是否存在更深层校验差异需对照反编译/动态验证确认。

## 4. 对“单颗 BM1746 从零固件”二次开发的影响结论

| 对象 | 接口/兼容性判断 | 是否需补充反编译 | 建议 |
|---|---|---|---|
| `sdcard/rootfs_sd` `bitmain_axi.ko` | 与 `rootfs_1` 已反编译版在**功能接口形态上一致**：字符设备名 `axi_fpga_dev`、mmap 路径（`axi_fpga_dev_mmap`/`remap_pfn_range`）、open/release/init/exit 流程与关键日志一致；**未发现 ioctl 接口线索**。但 vermagic（`g0387054-dirty` vs `g03c746f7`）与工具链不同，**不可与 rootfs_1 内核/模块 ABI 混装**。 | **通常不需要** | 若目标为 rootfs_1 对应内核环境，可直接以 `BM1746/reversed/ko_bitmain_axi/` 作为接口与行为参考；仅当需在 SD 卡/rootfs_sd 对应内核上定位实现差异或验证编译器导致的细节时，再对 SD 版补充反编译。 |
| `sdcard/rootfs_sd` `fpga_mem_driver.ko` | 与 `rootfs_1` 已反编译版在**功能接口形态上一致**：设备名 `fpga_mem`、模块参数 `fpga_mem_offset_addr:int`、mmap 路径与初始化/资源申请逻辑字符串一致；**未发现 ioctl 接口线索**。同样受 vermagic/工具链差异限制，不可跨环境混装。 | **通常不需要** | 以 `BM1746/reversed/ko_fpga_mem/` 参考即可满足多数二次开发理解；针对 SD 环境的精确行为验证才需补反编译。 |
| `rootfs_2` `FileParser` | 与 `rootfs_1` 在升级脚本使用的 `-s/-n/-q` 能力、BMU 产物/签名文件命名及 NAND sig 布局上**高度一致**；差异主要是缺少 miner_type/hash 匹配相关字符串与构建形态（更小、stripped、ELF 正常）。升级流程角色等同。 | **通常不需要优先补充** | 从零固件/升级链路分析优先依据 `BM1746/reversed/fileparser_funcs/`（rootfs_1，42 函数）与两版 `Update.sh`；仅当需要确认 rootfs_2 是否去除了 miner_type 绑定校验、或需在升级 ramfs 场景做逐指令级验证时，再对 rootfs_2 补充反编译。 |

**总体结论**：本次对比未发现 `rootfs_sd` ko 相对 `rootfs_1` 的新设备接口或新 ioctl/mmap 语义，也未发现 `rootfs_2` `FilePa
