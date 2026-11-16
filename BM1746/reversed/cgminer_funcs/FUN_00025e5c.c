//==================== FUN_00025e5c @ 00025e5c ====================

undefined4 FUN_00025e5c(int param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  char acStack_1010 [4100];
  
  FUN_0001a730();
  iVar1 = FUN_00029f54(param_2 + 0xc0,param_2 + 0xa0);
  if (iVar1 == 0) {
    if (DAT_00078e09 == '\0') {
      uVar2 = 0;
    }
    else if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (uVar2 = 0, 6 < DAT_000766c4)) {
      snprintf(acStack_1010,0x1000,"%s %s %d: Share above target","submit_tested_work",
               *(char **)(*(int *)(*(int *)(param_1 + 0x24) + 4) + 8),
               *(int *)(*(int *)(param_1 + 0x24) + 8));
      FUN_00038730(7,acStack_1010,0);
      uVar2 = 0;
    }
  }
  else {
    FUN_0001a44c(param_2,0);
    FUN_000256a4();
    uVar2 = 1;
  }
  return uVar2;
}

