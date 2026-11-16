//==================== FUN_00029294 @ 00029294 ====================

undefined4 FUN_00029294(int *param_1,uint param_2)

{
  uint uVar1;
  int *piVar2;
  undefined4 uVar3;
  
  if ((((param_1 == (int *)0x0) || (*param_1 == 7)) || (*param_1 != 1)) ||
     (((uVar1 = FUN_00059fec(), uVar1 < param_2 ||
       (piVar2 = (int *)FUN_0005a008(param_1,param_2), piVar2 == (int *)0x0)) || (*piVar2 != 2)))) {
    return 0;
  }
  uVar3 = FUN_0005a178();
  return uVar3;
}

