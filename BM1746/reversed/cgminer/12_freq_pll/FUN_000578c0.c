//==================== FUN_000578c0 @ 000578c0 ====================

void FUN_000578c0(int param_1,undefined4 *param_2,int *param_3)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = (int *)*param_2;
  if ((piVar2 == (int *)(param_1 + 0xc)) && (piVar2 == (int *)param_2[1])) {
    iVar1 = *(int *)(param_1 + 0xc);
    param_3[1] = (int)piVar2;
    *param_3 = iVar1;
    iVar1 = *(int *)(param_1 + 0xc);
    *(int **)(param_1 + 0xc) = param_3;
    *(int **)(iVar1 + 4) = param_3;
    param_2[1] = param_3;
    *param_2 = param_3;
    return;
  }
  *param_3 = *piVar2;
  param_3[1] = (int)piVar2;
  iVar1 = *piVar2;
  *piVar2 = (int)param_3;
  *(int **)(iVar1 + 4) = param_3;
  *param_2 = param_3;
  return;
}

