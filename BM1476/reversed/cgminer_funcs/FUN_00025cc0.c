//==================== FUN_00025cc0 @ 00025cc0 ====================

undefined4 FUN_00025cc0(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  char acStack_1018 [4096];
  
  iVar1 = FUN_0001a3d4();
  FUN_00015684(iVar1,param_2,param_4);
  iVar2 = FUN_0001654c(iVar1,param_3);
  if (iVar2 == 0) {
    FUN_00015780(iVar1);
    FUN_000189a4(param_1);
    uVar3 = 0;
  }
  else {
    FUN_0001a730(param_1,iVar1);
    if ((DAT_00076ffc != 0) && (DAT_0007871c != '\0')) {
      FUN_00011814(iVar1,param_3);
    }
    iVar2 = FUN_00029f54(iVar1 + 0xc0,iVar1 + 0xa0);
    if (iVar2 == 0) {
      FUN_00015780(iVar1);
      if ((DAT_00078e09 == '\0') ||
         (((DAT_0007ad48 == '\0' && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7)))) {
        uVar3 = 1;
      }
      else {
        snprintf(acStack_1018,0x1000,"%s %d: Share above target",
                 *(char **)(*(int *)(*(int *)(param_1 + 0x24) + 4) + 8),
                 *(int *)(*(int *)(param_1 + 0x24) + 8));
        FUN_00038730(7,acStack_1018,0);
        uVar3 = 1;
      }
    }
    else {
      FUN_000256a4();
      uVar3 = 1;
    }
  }
  return uVar3;
}

