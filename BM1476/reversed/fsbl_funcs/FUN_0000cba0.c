//==================== FUN_0000cba0 @ 0000cba0 ====================

uint FUN_0000cba0(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x2e2);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x4410000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    return ~*(uint *)(*(int *)(param_1 + 4) + 0x14) & 0xf0f;
  }
  FUN_000014e4(s_0___P___p_0001728e + 0x12,0x2e3);
  DAT_00028e84 = 1;
  return 0;
}

