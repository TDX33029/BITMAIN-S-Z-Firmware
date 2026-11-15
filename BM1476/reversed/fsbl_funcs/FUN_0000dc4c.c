//==================== FUN_0000dc4c @ 0000dc4c ====================

uint FUN_0000dc4c(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x643);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x4400000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    return (*(uint *)(*(int *)(param_1 + 4) + 0x14) & 0x3ff) >> 9;
  }
  FUN_000014e4(s_0___P___p_0001728e + 0x12,0x644);
  DAT_00028e84 = 1;
  return 0;
}

