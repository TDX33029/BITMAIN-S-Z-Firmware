//==================== FUN_0001fa14 @ 0001fa14 ====================

void FUN_0001fa14(int *param_1)

{
  int iVar1;
  int *piVar2;
  char acStack_1008 [4096];
  
  iVar1 = FUN_00018834(param_1,(int)param_1 + 0x81);
  if (iVar1 == 0) {
    FUN_0002a84c(param_1 + 0x2a);
    piVar2 = (int *)FUN_0001da58();
    if (piVar2 == param_1) {
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
        snprintf(acStack_1008,0x1000,"Pool %d %s not responding!",*piVar2,(char *)piVar2[0x31]);
        FUN_00038730(4,acStack_1008,0);
      }
      FUN_00019bf8(0);
    }
    else if ((DAT_00078e09 != '\0') &&
            (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      snprintf(acStack_1008,0x1000,"Pool %d %s failed to return work",*param_1,(char *)param_1[0x31]
              );
      FUN_00038730(7,acStack_1008,0);
    }
  }
  return;
}

