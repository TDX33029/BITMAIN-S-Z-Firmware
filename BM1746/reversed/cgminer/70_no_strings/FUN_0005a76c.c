//==================== FUN_0005a76c @ 0005a76c ====================

undefined4 FUN_0005a76c(int *param_1,uint param_2,int *param_3)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  
  if (param_3 != (int *)0x0) {
    if ((((param_1 != (int *)0x0) && (*param_1 == 1)) && (param_1 != param_3)) &&
       (param_2 < (uint)param_1[3])) {
      piVar2 = (int *)(param_1[4] + param_2 * 4);
      iVar1 = *(int *)(param_1[4] + param_2 * 4);
      if ((iVar1 != 0) && (*(int *)(iVar1 + 4) != -1)) {
        iVar3 = *(int *)(iVar1 + 4) + -1;
        *(int *)(iVar1 + 4) = iVar3;
        if (iVar3 == 0) {
          FUN_0005a3e4();
          piVar2 = (int *)(param_1[4] + param_2 * 4);
        }
      }
      *piVar2 = (int)param_3;
      return 0;
    }
    if (param_3[1] != -1) {
      iVar1 = param_3[1] + -1;
      param_3[1] = iVar1;
      if (iVar1 == 0) {
        FUN_0005a3e4(param_3);
      }
    }
  }
  return 0xffffffff;
}

