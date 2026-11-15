//==================== FUN_0005a8a0 @ 0005a8a0 ====================

undefined4 FUN_0005a8a0(int *param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  if ((((param_1 == (int *)0x0) || (*param_1 != 0)) || (param_2 == (int *)0x0)) || (*param_2 != 0))
  {
    return 0xffffffff;
  }
  FUN_00059e9c(param_2);
  iVar1 = FUN_00059f14();
  while( true ) {
    if (iVar1 == 0) {
      return 0;
    }
    iVar2 = FUN_00059f20(iVar1 + -0x10);
    if (iVar2 == 0) break;
    iVar3 = FUN_00059e20(param_1,iVar1);
    if (iVar3 == 0) {
      if (*(int *)(iVar2 + 4) != -1) {
        *(int *)(iVar2 + 4) = *(int *)(iVar2 + 4) + 1;
      }
      FUN_0005a820(param_1,iVar1,iVar2);
    }
    FUN_00059ee8(param_2,iVar1 + -0x10);
    iVar1 = FUN_00059f14();
  }
  return 0;
}

