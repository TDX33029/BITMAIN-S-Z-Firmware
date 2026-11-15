//==================== FUN_0002a47c @ 0002a47c ====================

void FUN_0002a47c(int *param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = *param_2;
  iVar2 = *param_1;
  *param_2 = iVar2 + iVar3;
  iVar1 = param_1[1] + param_2[1];
  param_2[1] = iVar1;
  if (999999 < iVar1) {
    *param_2 = iVar2 + iVar3 + 1;
    param_2[1] = iVar1 + -1000000;
  }
  return;
}

