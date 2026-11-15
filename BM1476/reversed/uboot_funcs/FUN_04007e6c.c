//==================== FUN_04007e6c @ 04007e6c ====================

undefined4 FUN_04007e6c(void)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  
  if (*DAT_04007ee8 == 0) {
    return 0;
  }
  iVar3 = FUN_040061e4(0);
  piVar2 = DAT_04007ef0;
  piVar1 = DAT_04007eec;
  if (5000 < (uint)(iVar3 - *DAT_04007eec)) {
    iVar4 = *DAT_04007ef0;
    *DAT_04007ef0 = iVar4 + 1;
    if (iVar4 + 1 < 5) {
      *piVar1 = iVar3;
      FUN_04007de8();
    }
    else {
      func_0x03fc7fbc(DAT_04007ef4);
      *piVar2 = 0;
      *DAT_04007ef8 = 3;
    }
  }
  return 1;
}

