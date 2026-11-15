//==================== FUN_00059ee8 @ 00059ee8 ====================

int * FUN_00059ee8(int *param_1,int param_2)

{
  int *piVar1;
  
  if (((param_1 == (int *)0x0) || (*param_1 != 0)) || (param_2 == 0)) {
    return (int *)0x0;
  }
  piVar1 = *(int **)(param_2 + 4);
  if (piVar1 == param_1 + 5) {
    piVar1 = (int *)0x0;
  }
  return piVar1;
}

