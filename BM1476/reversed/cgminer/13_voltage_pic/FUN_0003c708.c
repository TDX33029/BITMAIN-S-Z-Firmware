//==================== FUN_0003c708 @ 0003c708 ====================

uint FUN_0003c708(uint param_1)

{
  uint uVar1;
  int iVar2;
  
  FUN_0003c66c(param_1 | 0x2000000);
  iVar2 = 0x16;
  do {
    usleep(5000);
    uVar1 = *(uint *)(DAT_00078e10 + 0x30);
    if ((int)uVar1 < 0) {
      return uVar1 & 0xff;
    }
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 5)) {
    uVar1 = 0xff;
  }
  else {
    uVar1 = FUN_0003ae24(uVar1);
  }
  return uVar1;
}

