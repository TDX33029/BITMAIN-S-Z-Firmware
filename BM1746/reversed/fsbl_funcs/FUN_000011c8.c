//==================== FUN_000011c8 @ 000011c8 ====================

undefined4 FUN_000011c8(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(s_0___P___p_0001720a + 0x16,0xbc);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    return *(undefined4 *)(*(int *)(param_1 + 4) + 0xc);
  }
  FUN_000014e4(s_0___P___p_0001720a + 0x16,0xbd);
  DAT_00028e84 = 1;
  return 0;
}

