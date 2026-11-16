//==================== FUN_00008994 @ 00008994 ====================

undefined4 FUN_00008994(int param_1,undefined4 param_2)

{
  int iVar1;
  
  iVar1 = FUN_00006bcc(param_1,0x600,param_2,0);
  if (iVar1 != 0) {
    return 1;
  }
  iVar1 = *(int *)(param_1 + 4);
  do {
    if ((*(ushort *)(iVar1 + 0x30) & 0x8000) != 0) {
      *(undefined2 *)(iVar1 + 0x32) = 0xf3ff;
      return 1;
    }
  } while ((*(ushort *)(iVar1 + 0x30) & 2) == 0);
  *(undefined2 *)(iVar1 + 0x30) = 2;
  return 0;
}

