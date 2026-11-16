//==================== FUN_0000f7e8 @ 0000f7e8 ====================

undefined4 FUN_0000f7e8(int param_1,int param_2)

{
  undefined4 uVar1;
  int iVar2;
  char acStack_1008 [4100];
  
  iVar2 = *(int *)(param_1 + 0x24);
  if (*(int *)(iVar2 + 0xec) == param_2) {
    uVar1 = 0;
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (uVar1 = 0, 6 < DAT_000766c4)))) {
      snprintf(acStack_1008,0x1000,"%s %d duplicate share detected as HW error",
               *(char **)(*(int *)(iVar2 + 4) + 8),*(int *)(iVar2 + 8));
      FUN_00038730(7,acStack_1008,0);
      uVar1 = 0;
    }
  }
  else {
    *(int *)(iVar2 + 0xec) = param_2;
    uVar1 = 1;
  }
  return uVar1;
}

