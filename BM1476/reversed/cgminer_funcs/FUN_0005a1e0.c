//==================== FUN_0005a1e0 @ 0005a1e0 ====================

undefined4 FUN_0005a1e0(undefined4 param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 extraout_r2;
  
  if ((param_2 == 0) || (iVar1 = FUN_00059c8c(param_2,0xffffffff), iVar1 == 0)) {
    return 0xffffffff;
  }
  uVar2 = FUN_0005a194(param_1,param_2,extraout_r2,param_4);
  return uVar2;
}

