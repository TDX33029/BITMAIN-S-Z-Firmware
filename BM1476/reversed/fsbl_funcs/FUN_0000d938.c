//==================== FUN_0000d938 @ 0000d938 ====================

uint FUN_0000d938(int param_1,uint param_2)

{
  if (param_1 == 0) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x5a5);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) != 0x11111111) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x5a6);
    DAT_00028e84 = 1;
    return 0;
  }
  if (param_2 < 0x10) {
    *(uint *)(*(int *)(param_1 + 4) + 0x10) = (param_2 + 0x50) * 0x10000 | 0x4000000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    return *(uint *)(*(int *)(param_1 + 4) + 0x14) & 0xffff;
  }
  FUN_000014e4(s_0___P___p_0001728e + 0x12,0x5a7);
  DAT_00028e84 = 1;
  return 0;
}

