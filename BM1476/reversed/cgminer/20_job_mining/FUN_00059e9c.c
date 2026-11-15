//==================== FUN_00059e9c @ 00059e9c ====================

int * FUN_00059e9c(int *param_1)

{
  int *piVar1;
  
  if ((param_1 == (int *)0x0) || (*param_1 != 0)) {
    return (int *)0x0;
  }
  piVar1 = (int *)param_1[6];
  if (piVar1 == param_1 + 5) {
    piVar1 = (int *)0x0;
  }
  return piVar1;
}

