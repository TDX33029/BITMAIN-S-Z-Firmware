//==================== FUN_0002a448 @ 0002a448 ====================

void FUN_0002a448(int *param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = *param_1;
  iVar1 = *param_2;
  *param_2 = iVar3 - iVar1;
  iVar2 = param_1[1] - param_2[1];
  param_2[1] = iVar2;
  if (iVar2 < 0) {
    *param_2 = (iVar3 - iVar1) + -1;
    param_2[1] = iVar2 + 1000000;
  }
  return;
}

