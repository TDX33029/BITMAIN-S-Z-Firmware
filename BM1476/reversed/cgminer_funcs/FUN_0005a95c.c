//==================== FUN_0005a95c @ 0005a95c ====================

undefined4 FUN_0005a95c(undefined4 param_1,int param_2,int param_3)

{
  int iVar1;
  undefined4 uVar2;
  
  if ((param_2 != 0) && (iVar1 = FUN_00059c8c(param_2,0xffffffff), iVar1 != 0)) {
    uVar2 = FUN_0005a820(param_1,param_2,param_3);
    return uVar2;
  }
  if ((param_3 != 0) && (*(int *)(param_3 + 4) != -1)) {
    iVar1 = *(int *)(param_3 + 4) + -1;
    *(int *)(param_3 + 4) = iVar1;
    if (iVar1 == 0) {
      FUN_0005a3e4(param_3);
    }
  }
  return 0xffffffff;
}

