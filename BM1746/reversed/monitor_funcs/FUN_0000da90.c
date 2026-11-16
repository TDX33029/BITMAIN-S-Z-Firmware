//==================== FUN_0000da90 @ 0000da90 ====================

void FUN_0000da90(int param_1,int param_2)

{
  int *piVar1;
  
  if (param_2 == 0) {
    return;
  }
  piVar1 = *(int **)(param_1 + 8);
  if (*(int **)(param_1 + 8) == (int *)0x0) {
    *(int *)(param_1 + 8) = param_2;
    return;
  }
  while ((int *)*piVar1 != (int *)0x0) {
    piVar1 = (int *)*piVar1;
  }
  *piVar1 = param_2;
  *(int **)(param_2 + 4) = piVar1;
  return;
}

