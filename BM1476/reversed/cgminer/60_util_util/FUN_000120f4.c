//==================== FUN_000120f4 @ 000120f4 ====================

void FUN_000120f4(void)

{
  int iVar1;
  undefined4 in_r3;
  char acStack_1008 [4096];
  undefined4 uStack_8;
  
  uStack_8 = in_r3;
  iVar1 = nice(10);
  if (iVar1 != 0) {
    return;
  }
  if (DAT_00078e09 == '\0') {
    return;
  }
  if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7)) {
    return;
  }
  builtin_strncpy(acStack_1008,"Unable to set thread to low priority",0x25);
  FUN_00038730(7,acStack_1008,0);
  return;
}

