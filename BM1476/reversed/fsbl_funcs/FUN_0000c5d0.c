//==================== FUN_0000c5d0 @ 0000c5d0 ====================

undefined4 FUN_0000c5d0(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x11d);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    return *(undefined4 *)(*(int *)(param_1 + 4) + 0x18);
  }
  FUN_000014e4(s_0___P___p_0001728e + 0x12,0x11e);
  DAT_00028e84 = 1;
  return 0;
}

