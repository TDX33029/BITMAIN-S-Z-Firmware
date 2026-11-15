//==================== FUN_0003bb00 @ 0003bb00 ====================

undefined4 FUN_0003bb00(int param_1)

{
  if (param_1 == 1) {
    return 1;
  }
  if (param_1 == 0) {
    return 0;
  }
  if (param_1 == 0x1a) {
    return 0x1a;
  }
  printf("%s: Don\'t support ASIC baud = %d, error!!!\n","asic_baud_to_fpga_baud",param_1);
  return 0x35;
}

