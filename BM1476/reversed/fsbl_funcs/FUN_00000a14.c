//==================== FUN_00000a14 @ 00000a14 ====================

undefined4 FUN_00000a14(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(s_0___P___p_0001720a + 10,0x1e1);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    return *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
  }
  FUN_000014e4(s_0___P___p_0001720a + 10,0x1e2);
  DAT_00028e84 = 1;
  return 0;
}

