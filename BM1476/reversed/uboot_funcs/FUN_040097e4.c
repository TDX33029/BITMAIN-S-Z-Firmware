//==================== FUN_040097e4 @ 040097e4 ====================

void FUN_040097e4(int param_1)

{
  int *piVar1;
  int iVar2;
  
  iVar2 = func_0x03fbefb0(DAT_04009854);
  if ((iVar2 != 0) && (iVar2 = FUN_04005a60(iVar2,DAT_04009860), iVar2 == 0)) {
    return;
  }
  iVar2 = FUN_04008f68();
  piVar1 = DAT_04009858;
  if (iVar2 == 0) {
    return;
  }
  if (param_1 != 0) {
    iVar2 = FUN_04008f68();
    *piVar1 = iVar2;
  }
  FUN_04008f30();
  FUN_04009754();
  iVar2 = FUN_04008f68();
  if (*piVar1 == iVar2) {
    *DAT_0400985c = 1;
  }
  return;
}

