//==================== FUN_0003b994 @ 0003b994 ====================

void FUN_0003b994(void)

{
  char acStack_1010 [4096];
  
  *DAT_00078e10 = *DAT_00078e10 | 0x60000000;
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    snprintf(acStack_1010,0x1000,"FPGA version: %04X",*DAT_00078e10 & 0xffff);
    FUN_00038730(5,acStack_1010,0);
  }
  return;
}

