//==================== FUN_000467b0 @ 000467b0 ====================

void FUN_000467b0(void)

{
  undefined4 uVar1;
  uint param4;
  char acStack_1008 [4096];
  
  uVar1 = FUN_000466a4(3);
  DAT_0009d6f3 = FUN_0005c710(0x100,uVar1);
  param4 = (uint)DAT_0009d6f3;
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    snprintf(acStack_1008,0x1000,"%s:addrInterval = %d","calculate_address_interval",param4);
    FUN_00038730(5,acStack_1008,0);
    param4 = (uint)DAT_0009d6f3;
  }
  DAT_00079131 = (char)param4;
  DAT_00079132 = (char)(param4 << 1);
  DAT_00079130 = 0;
  return;
}

