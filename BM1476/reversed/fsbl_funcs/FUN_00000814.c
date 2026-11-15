//==================== FUN_00000814 @ 00000814 ====================

undefined4 FUN_00000814(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(s_0___P___p_0001720a + 10,0x175);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    return *(undefined4 *)(*(int *)(param_1 + 4) + 4);
  }
  FUN_000014e4(s_0___P___p_0001720a + 10,0x176);
  DAT_00028e84 = 1;
  return 0;
}

