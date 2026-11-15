//==================== FUN_0000ca3c @ 0000ca3c ====================

uint FUN_0000ca3c(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x20a);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x4400000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    return (*(uint *)(*(int *)(param_1 + 4) + 0x14) & 0x3fff) >> 0xc;
  }
  FUN_000014e4(s_0___P___p_0001728e + 0x12,0x20b);
  DAT_00028e84 = 1;
  return 0;
}

