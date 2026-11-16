//==================== FUN_0005a6e8 @ 0005a6e8 ====================

undefined4 FUN_0005a6e8(int *param_1,int *param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  
  if (param_2 != (int *)0x0) {
    if ((((param_1 != (int *)0x0) && (*param_1 == 1)) && (param_1 != param_2)) &&
       (iVar1 = FUN_00059d20(param_1,1,1,param_4,param_4), iVar1 != 0)) {
      iVar1 = param_1[3];
      *(int **)(param_1[4] + iVar1 * 4) = param_2;
      param_1[3] = iVar1 + 1;
      return 0;
    }
    if (param_2[1] != -1) {
      iVar1 = param_2[1] + -1;
      param_2[1] = iVar1;
      if (iVar1 == 0) {
        FUN_0005a3e4(param_2);
      }
    }
  }
  return 0xffffffff;
}

