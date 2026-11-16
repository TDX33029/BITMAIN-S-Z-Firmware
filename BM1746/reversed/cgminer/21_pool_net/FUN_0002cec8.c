//==================== FUN_0002cec8 @ 0002cec8 ====================

undefined4 FUN_0002cec8(int param_1)

{
  int iVar1;
  
  if (*(char *)(param_1 + 0x299) != '\0') {
    FUN_0002aee0();
  }
  iVar1 = FUN_0002b8e8(param_1);
  if ((iVar1 == 0) ||
     (((*(char *)(param_1 + 0x25c) != '\0' && (iVar1 = FUN_0002e69c(param_1), iVar1 == 0)) ||
      (iVar1 = FUN_0002ea38(param_1), iVar1 == 0)))) {
    FUN_0001fa14(param_1);
    return 0;
  }
  FUN_0001fd64(param_1);
  return 1;
}

