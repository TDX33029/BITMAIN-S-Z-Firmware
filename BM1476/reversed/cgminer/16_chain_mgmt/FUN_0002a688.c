//==================== FUN_0002a688 @ 0002a688 ====================

void FUN_0002a688(int *param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = *param_1 + *param_2;
  *param_1 = iVar1;
  iVar2 = param_1[1] + param_2[1];
  param_1[1] = iVar2;
  if (999999999 < iVar2) {
    do {
      iVar1 = iVar1 + 1;
      iVar2 = iVar2 + -1000000000;
    } while (999999999 < iVar2);
    *param_1 = iVar1;
    param_1[1] = iVar2;
  }
  if (iVar2 < 0) {
    iVar1 = *param_1;
    do {
      iVar1 = iVar1 + -1;
      iVar2 = iVar2 + 1000000000;
    } while (iVar2 < 0);
    *param_1 = iVar1;
    param_1[1] = iVar2;
  }
  return;
}

