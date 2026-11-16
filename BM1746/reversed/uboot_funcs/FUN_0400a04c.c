//==================== FUN_0400a04c @ 0400a04c ====================

undefined4 FUN_0400a04c(int param_1,undefined4 param_2)

{
  longlong lVar1;
  undefined4 uVar2;
  
  if (param_1 == 0) {
    *DAT_0400a098 = 0;
    return 0;
  }
  *DAT_0400a098 = param_2;
  uVar2 = FUN_040061e4(0);
  *DAT_0400a09c = uVar2;
  lVar1 = (ulonglong)DAT_0400a0a0 * (ulonglong)(uint)(param_1 * 1000);
  *DAT_0400a0a4 = (uint)((ulonglong)lVar1 >> 0x26);
  return (int)lVar1;
}

