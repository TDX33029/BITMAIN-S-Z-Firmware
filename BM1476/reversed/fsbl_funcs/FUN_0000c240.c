//==================== FUN_0000c240 @ 0000c240 ====================

bool FUN_0000c240(int param_1)

{
  int iVar1;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_0001728c,0x69);
    DAT_00028e84 = 1;
    return false;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    FUN_0000c654();
    FUN_0000d884(param_1,1,0x55);
    iVar1 = FUN_0000d938(param_1,1);
    FUN_0000c654(param_1);
    return iVar1 != 0x55;
  }
  FUN_000014e4(&DAT_0001728c,0x6a);
  DAT_00028e84 = 1;
  return false;
}

