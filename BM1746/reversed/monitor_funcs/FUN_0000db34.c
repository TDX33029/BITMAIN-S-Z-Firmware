//==================== FUN_0000db34 @ 0000db34 ====================

void FUN_0000db34(int param_1,undefined4 param_2)

{
  int iVar1;
  int *piVar2;
  
  iVar1 = FUN_0000c5b4(param_2);
  if (iVar1 == 0) {
    return;
  }
  piVar2 = *(int **)(param_1 + 8);
  if (*(int **)(param_1 + 8) == (int *)0x0) {
    *(int *)(param_1 + 8) = iVar1;
    return;
  }
  while ((int *)*piVar2 != (int *)0x0) {
    piVar2 = (int *)*piVar2;
  }
  *piVar2 = iVar1;
  *(int **)(iVar1 + 4) = piVar2;
  return;
}

