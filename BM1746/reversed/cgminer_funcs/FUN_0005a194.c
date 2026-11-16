//==================== FUN_0005a194 @ 0005a194 ====================

undefined4 FUN_0005a194(int *param_1,int param_2)

{
  int iVar1;
  
  if ((((param_1 == (int *)0x0) || (*param_1 != 2)) || (param_2 == 0)) ||
     (iVar1 = FUN_000595b4(param_2), iVar1 == 0)) {
    return 0xffffffff;
  }
  FUN_00059594(param_1[2]);
  param_1[2] = iVar1;
  return 0;
}

