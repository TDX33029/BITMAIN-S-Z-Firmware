//==================== FUN_0005a820 @ 0005a820 ====================

undefined4 FUN_0005a820(int *param_1,int param_2,int *param_3)

{
  int iVar1;
  
  if (param_3 != (int *)0x0) {
    if (((param_1 != (int *)0x0 && param_2 != 0) && (*param_1 == 0)) && (param_1 != param_3)) {
      iVar1 = param_1[7];
      param_1[7] = iVar1 + 1;
      iVar1 = FUN_00057a8c(param_1 + 2,param_2,iVar1,param_3);
      if (iVar1 == 0) {
        return 0;
      }
    }
    if ((param_3[1] != -1) && (iVar1 = param_3[1] + -1, param_3[1] = iVar1, iVar1 == 0)) {
      FUN_0005a3e4(param_3);
    }
  }
  return 0xffffffff;
}

