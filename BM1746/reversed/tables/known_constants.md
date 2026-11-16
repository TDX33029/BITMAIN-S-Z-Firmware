# cgminer 已识别常量/表数据 (从 ELF 数据段直接提取)

## I2C 每 ASIC 地址表 DAT_00076b48 (0x76b48)

前 16 字节: ['0x50', '0x54', '0x58', '0x01', '0xff', '0xff', '0xff', '0xff', '0x14', '0x00', '0x00', '0x00', '0x11', '0x51', '0x11', '0x02']

## 频率表 0x69204: 见 freq_table_full.csv + pll_decode.md (115/115 公式验证通过)

## PLL 公式: fout = 25MHz * N / (P*Q), N=pll[31:16], P=pll[7:4], Q=pll[11:8]

## 关键协议字符串 (AA55/PIC 命令相关)

| 文件偏移 | 字符串 |
|---|---|
| 0x5b8ec | `Please check the details from the list below of the servers you have input` |
| 0x5f098 | `,"CHECK":[` |
| 0x5f0a4 | `CHECK,` |
| 0x5ffb0 | `Missing check cmd` |
| 0x5ffc4 | `Check command` |
| 0x60208 | `Hotplug check set to %ds` |
| 0x60a60 | `check_how_many_uart_data_in_fpga` |
| 0x60a84 | `read_uart_data_in_fpga` |
| 0x60a9c | `clear_uart_send_fifo` |
| 0x60ab4 | `clear_uart_rx_fifo` |
| 0x60ac8 | `uart_send` |
| 0x60cec | `set_PIC16F1704_flash_pointer_new` |
| 0x60d10 | `send_data_to_PIC16F1704_new` |
| 0x60d2c | `read_PIC16F1704_flash_pointer_new` |
| 0x60d50 | `read_PIC16F1704_flash_data_new` |
| 0x60d70 | `erase_PIC16F1704_flash_new` |
| 0x60d8c | `write_data_into_PIC16F1704_flash_new` |
| 0x60db4 | `jump_from_loader_to_app_PIC16F1704_new` |
| 0x60ddc | `reset_PIC16F1704_pic_new` |
| 0x60e14 | `set_PIC16F1704_voltage_new` |
| 0x60e30 | `write_hash_ID_PIC16F1704_new` |
| 0x60e50 | `read_hash_id_PIC16F1704_new` |
| 0x60e6c | `enable_PIC16F1704_dc_dc_new` |
| 0x60e88 | `disable_PIC16F1704_dc_dc_new` |
| 0x60ea8 | `heart_beat_PIC16F1704_new` |
| 0x60ec4 | `pic_heart_beat_func_new` |
| 0x60edc | `get_PIC16F1704_software_version_new` |
| 0x60f00 | `get_PIC16F1704_voltage_new` |
| 0x60f1c | `write_temperature_offset_PIC16F1704_new` |
| 0x60f44 | `read_temperature_offset_PIC16F1704_new` |
| 0x60f6c | `erase_PIC16F1704_app_flash_new` |
| 0x60f8c | `PIC1704_update_pic_app_program_new` |
| 0x60fb0 | `every_chain_reset_PIC16F1704_pic_new` |
| 0x60fd8 | `every_chain_jump_from_loader_to_app_PIC16F1704_new` |
| 0x6100c | `every_chain_disable_PIC16F1704_dc_dc_new` |
| 0x61038 | `every_chain_enable_PIC16F1704_dc_dc_new` |
| 0x61060 | `send_heart_beat_to_every_chain` |
| 0x61080 | `check_whether_need_update_pic_program` |
| 0x610a8 | `save_freq_PIC16F1704_new` |
| 0x610c4 | `get_PIC16F1704_freq_new` |
