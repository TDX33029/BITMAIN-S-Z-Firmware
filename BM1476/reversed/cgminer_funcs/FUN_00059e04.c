//==================== FUN_00059e04 @ 00059e04 ====================

int FUN_00059e04(int *param_1)

{
  int iVar1;
  
  if (param_1 == (int *)0x0) {
    return 0;
  }
  if (*param_1 == 0) {
    iVar1 = param_1[2];
  }
  else {
    iVar1 = 0;
  }
  return iVar1;
}

