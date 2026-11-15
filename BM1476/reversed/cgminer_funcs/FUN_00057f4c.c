//==================== FUN_00057f4c @ 00057f4c ====================

void FUN_00057f4c(int param_1,undefined4 param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = *(int *)(param_1 + 8);
  if ((iVar1 != 0) && (*(int *)(iVar1 + 4) != -1)) {
    iVar2 = *(int *)(iVar1 + 4) + -1;
    *(int *)(iVar1 + 4) = iVar2;
    if (iVar2 == 0) {
      FUN_0005a3e4();
      *(undefined4 *)(param_1 + 8) = param_2;
      return;
    }
  }
  *(undefined4 *)(param_1 + 8) = param_2;
  return;
}

