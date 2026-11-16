//==================== FUN_0001fd64 @ 0001fd64 ====================

void FUN_0001fd64(int *param_1)

{
  int iVar1;
  char acStack_1008 [4096];
  
  iVar1 = FUN_00018c2c(param_1,(int)param_1 + 0x81);
  if (iVar1 != 0) {
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      snprintf(acStack_1008,0x1000,"Stratum connection to pool %d resumed",*param_1);
      FUN_00038730(7,acStack_1008,0);
    }
    FUN_0001fc34(param_1);
  }
  return;
}

