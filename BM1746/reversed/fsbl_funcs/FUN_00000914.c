//==================== FUN_00000914 @ 00000914 ====================

undefined4 FUN_00000914(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(s_0___P___p_0001720a + 10,0x1ad);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    return *(undefined4 *)(*(int *)(param_1 + 4) + 8);
  }
  FUN_000014e4(s_0___P___p_0001720a + 10,0x1ae);
  DAT_00028e84 = 1;
  return 0;
}

