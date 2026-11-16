//==================== FUN_00000ca0 @ 00000ca0 ====================

uint FUN_00000ca0(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(s_0___P___p_0001720a + 10,0x26d);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    return ~*(uint *)(*(int *)(param_1 + 4) + 0x14) >> 0x1f;
  }
  FUN_000014e4(s_0___P___p_0001720a + 10,0x26e);
  DAT_00028e84 = 1;
  return 0;
}

