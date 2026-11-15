//==================== FUN_00008798 @ 00008798 ====================

bool FUN_00008798(int param_1)

{
  int iVar1;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017264,0x319);
    DAT_00028e84 = 1;
    return false;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 0x24) != 0x11111111) {
    FUN_000014e4(&DAT_00017264,0x31a);
    DAT_00028e84 = 1;
    return false;
  }
  iVar1 = FUN_00006bcc(param_1,0x3700,*(undefined4 *)(param_1 + 0x50),0);
  if (iVar1 == 0) {
    iVar1 = FUN_00006bcc(param_1,0xaa00,0);
    return iVar1 != 0;
  }
  return true;
}

