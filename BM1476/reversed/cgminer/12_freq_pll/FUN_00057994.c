//==================== FUN_00057994 @ 00057994 ====================

void FUN_00057994(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar2 = *(int *)(param_1 + 0x10);
  if (iVar2 == param_1 + 0xc) {
    return;
  }
  do {
    iVar1 = *(int *)(iVar2 + 8);
    iVar3 = iVar2 + -4;
    iVar2 = *(int *)(iVar2 + 4);
    if (iVar1 != 0) {
      iVar4 = *(int *)(iVar1 + 4) + -1;
      if ((*(int *)(iVar1 + 4) != -1) && (*(int *)(iVar1 + 4) = iVar4, iVar4 == 0)) {
        FUN_0005a3e4();
      }
    }
    FUN_00059594(iVar3);
  } while (iVar2 != param_1 + 0xc);
  return;
}

