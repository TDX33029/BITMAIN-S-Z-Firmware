//==================== FUN_04009864 @ 04009864 ====================

void FUN_04009864(void)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  
  piVar1 = DAT_040098e4;
  iVar3 = func_0x03fbeb6c();
  piVar2 = DAT_040098e8;
  if ((*piVar1 == 0) || (*DAT_040098e8 != iVar3)) {
    iVar4 = func_0x03fbefb0(DAT_040098ec);
    *piVar2 = iVar3;
    *piVar1 = iVar4;
  }
  if (*piVar1 == 0) {
    iVar3 = func_0x03fbefb0(DAT_040098f0);
    if ((iVar3 == 0) || (iVar3 = FUN_04009008(), iVar3 == 0)) {
      iVar3 = 0;
    }
  }
  else {
    iVar3 = FUN_04009008();
  }
  FUN_04008fa0(iVar3);
  FUN_04009754();
  return;
}

