//==================== FUN_0003f254 @ 0003f254 ====================

void FUN_0003f254(void)

{
  int param4;
  int param3;
  undefined2 *puVar1;
  char acStack_1020 [4096];
  
  puVar1 = &DAT_000790b8;
  param3 = 0;
  do {
    param4 = 0;
    do {
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
        snprintf(acStack_1020,0x1000,"chain %d asic %d vol %d\n",param3,param4,
                 (uint)(ushort)puVar1[param4]);
        FUN_00038730(5,acStack_1020,0);
      }
      param4 = param4 + 1;
    } while (param4 != 3);
    param3 = param3 + 1;
    puVar1 = puVar1 + 3;
  } while (param3 != 4);
  return;
}

