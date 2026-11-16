//==================== FUN_0003c66c @ 0003c66c ====================

void FUN_0003c66c(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = 0x16;
  do {
    if (*(int *)(DAT_00078e10 + 0x30) < 0) {
      *(undefined4 *)(DAT_00078e10 + 0x30) = param_1;
      return;
    }
    usleep(5000);
    iVar1 = iVar1 + -1;
  } while (iVar1 != 0);
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    FUN_0003ade0();
  }
  return;
}

