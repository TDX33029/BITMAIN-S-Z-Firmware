//==================== FUN_00059e68 @ 00059e68 ====================

undefined4 FUN_00059e68(int *param_1)

{
  if ((param_1 == (int *)0x0) || (*param_1 != 0)) {
    return 0xffffffff;
  }
  FUN_00057e28(param_1 + 2);
  param_1[7] = 0;
  return 0;
}

