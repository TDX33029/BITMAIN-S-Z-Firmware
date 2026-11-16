//==================== FUN_0003eec8 @ 0003eec8 ====================

void FUN_0003eec8(int param_1)

{
  ushort uVar1;
  int param8;
  int param4;
  ushort *puVar2;
  char acStack_1028 [4100];
  
  puVar2 = (ushort *)(param_1 * 6 + 0x790b6);
  param4 = 0;
  do {
    puVar2 = puVar2 + 1;
    uVar1 = *puVar2;
    param8 = FUN_0003ea50((&DAT_00076b48)[param4],(uint)uVar1);
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
      snprintf(acStack_1028,0x1000,"chain%d asic%d addr %02x, %s set vol %d, get vol %d",param_1,
               param4,(uint)(byte)(&DAT_00076b48)[param4],"every_asic_set_isl_voltage",(uint)uVar1,
               param8);
      FUN_00038730(5,acStack_1028,0);
    }
    param4 = param4 + 1;
    FUN_0002a92c(200);
  } while (param4 != 3);
  return;
}

