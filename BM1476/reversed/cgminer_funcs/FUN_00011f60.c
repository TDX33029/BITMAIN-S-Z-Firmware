//==================== FUN_00011f60 @ 00011f60 ====================

void FUN_00011f60(int param_1)

{
  char *param3;
  int iVar1;
  uint uVar2;
  undefined1 auStack_1078 [32];
  uint local_1058 [20];
  char acStack_1008 [4096];
  
  iVar1 = 0;
  do {
    uVar2 = *(uint *)(param_1 + iVar1);
    *(uint *)((int)local_1058 + iVar1) =
         uVar2 << 0x18 | (uVar2 >> 8 & 0xff) << 0x10 | (uVar2 >> 0x10 & 0xff) << 8 | uVar2 >> 0x18;
    iVar1 = iVar1 + 4;
  } while (iVar1 != 0x50);
  FUN_0003049c(local_1058,0x50,auStack_1078);
  FUN_0003049c(auStack_1078,0x20,param_1 + 0xc0);
  param3 = (char *)FUN_00029730(param_1 + 0xc0,0x20);
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    snprintf(acStack_1008,0x1000,"regen:%s",param3);
    FUN_00038730(5,acStack_1008,0);
  }
  free(param3);
  return;
}

