//==================== FUN_00059fec @ 00059fec ====================

int FUN_00059fec(int *param_1)

{
  int iVar1;
  
  if (param_1 == (int *)0x0) {
    return 0;
  }
  if (*param_1 == 1) {
    iVar1 = param_1[3];
  }
  else {
    iVar1 = 0;
  }
  return iVar1;
}

