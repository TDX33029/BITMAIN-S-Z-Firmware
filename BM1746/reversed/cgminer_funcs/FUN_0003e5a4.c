//==================== FUN_0003e5a4 @ 0003e5a4 ====================

void FUN_0003e5a4(int param_1)

{
  int param4;
  ushort *puVar1;
  bool bVar2;
  ushort uStack_102a;
  char acStack_1028 [4100];
  
  param4 = 0;
  puVar1 = (ushort *)(param_1 * 6 + 0x790b6);
  do {
    uStack_102a = 0;
    FUN_0003e564((&DAT_00076b48)[param4],&uStack_102a);
    bVar2 = DAT_0007ad48 != '\0';
    puVar1 = puVar1 + 1;
    *puVar1 = uStack_102a;
    if (((bVar2) || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
      snprintf(acStack_1028,0x1000,"chain %d asic = %d vol=%u\n",param_1,param4,(uint)uStack_102a);
      FUN_00038730(5,acStack_1028,0);
    }
    param4 = param4 + 1;
  } while (param4 != 3);
  return;
}

