//==================== FUN_0001fc34 @ 0001fc34 ====================

void FUN_0001fc34(int *param_1)

{
  int iVar1;
  int iVar2;
  char acStack_1010 [4100];
  
  iVar1 = DAT_00076efc;
  param_1[7] = 0;
  if ((iVar1 == 0) && (iVar2 = param_1[1], iVar1 = FUN_0001fb7c(), iVar2 < iVar1)) {
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
      snprintf(acStack_1010,0x1000,"Pool %d %s alive, testing stability",*param_1,
               (char *)param_1[0x31]);
      FUN_00038730(4,acStack_1010,0);
    }
  }
  else if ((DAT_00078e09 != '\0') &&
          (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
    snprintf(acStack_1010,0x1000,"Pool %d %s alive",*param_1,(char *)param_1[0x31]);
    FUN_00038730(7,acStack_1010,0);
  }
  return;
}

