//==================== FUN_00000c1c @ 00000c1c ====================

undefined4 FUN_00000c1c(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(s_0___P___p_0001720a + 10,0x252);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    return *(undefined4 *)(*(int *)(param_1 + 4) + 0x80);
  }
  FUN_000014e4(s_0___P___p_0001720a + 10,0x253);
  DAT_00028e84 = 1;
  return 0;
}

