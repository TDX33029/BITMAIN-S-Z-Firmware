//==================== FUN_00011814 @ 00011814 ====================

void FUN_00011814(undefined4 param_1,uint param_2)

{
  int iVar1;
  uint param3;
  char acStack_1408 [1024];
  char acStack_1008 [4096];
  
  iVar1 = 4;
  param3 = 0;
  do {
    iVar1 = iVar1 + -1;
    param3 = (param_2 & 0xff) + param3 * 0x100;
    param_2 = param_2 >> 8;
  } while (iVar1 != 0);
  FUN_000296d8(acStack_1408,param_1,0x80);
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
    snprintf(acStack_1008,0x1000,"BENCHFILE nonce %u=0x%08x for work=%s",param3,param3,acStack_1408)
    ;
    FUN_00038730(3,acStack_1008,0);
  }
  return;
}

