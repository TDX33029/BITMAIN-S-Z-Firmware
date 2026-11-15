//==================== FUN_0000c654 @ 0000c654 ====================

void FUN_0000c654(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x139);
    DAT_00028e84 = 1;
    return;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x18) = 0x10;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x18) = 0;
    return;
  }
  FUN_000014e4(s_0___P___p_0001728e + 0x12,0x13a);
  DAT_00028e84 = 1;
  return;
}

