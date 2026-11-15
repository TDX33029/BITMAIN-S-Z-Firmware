//==================== FUN_00040dec @ 00040dec ====================

void FUN_00040dec(void)

{
  int iVar1;
  char acStack_1008 [4096];
  
  FUN_0003d000(6,0);
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    snprintf(acStack_1008,0x1000,"%s: erase_loop = %d\n","erase_PIC16F1704_app_flash_new",0x4c);
    FUN_00038730(5,acStack_1008,0);
  }
  iVar1 = 0x4c;
  do {
    FUN_0003da84();
    iVar1 = iVar1 + -1;
  } while (iVar1 != 0);
  return;
}

