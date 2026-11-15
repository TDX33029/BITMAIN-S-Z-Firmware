//==================== FUN_0400a644 @ 0400a644 ====================

undefined4
FUN_0400a644(undefined4 param_1,int param_2,undefined4 param_3,undefined4 param_4,int param_5)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  piVar1 = DAT_0400a728;
  iVar5 = *DAT_0400a728;
  if (iVar5 == 0) {
    uVar2 = 0xffffffff;
  }
  else {
    iVar4 = param_2;
    uVar2 = param_1;
    if (0xfffffffc < param_2 - 1U && param_2 != -2) {
      iVar4 = -1;
      uVar2 = DAT_0400a72c;
    }
    iVar3 = FUN_0400a41c(iVar5,uVar2,0x800,param_4,param_1,param_2);
    FUN_0400a5c0(iVar5 + iVar3,iVar4,param_3,param_4,param_5);
    iVar5 = FUN_04005ff4(uVar2,DAT_0400a730,6);
    if (iVar5 == 0) {
      *DAT_0400a734 = iVar4;
      *DAT_0400a738 = uVar2;
      *DAT_0400a73c = iVar3 + 0x1c + param_5;
      *DAT_0400a740 = 1;
      uVar2 = FUN_040061e4();
      *DAT_0400a744 = uVar2;
      FUN_04007de8();
      uVar2 = 1;
    }
    else {
      FUN_0400929c(*piVar1);
      uVar2 = 0;
    }
  }
  return uVar2;
}

