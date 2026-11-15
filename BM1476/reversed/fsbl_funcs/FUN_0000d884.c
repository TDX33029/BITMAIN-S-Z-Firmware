//==================== FUN_0000d884 @ 0000d884 ====================

void FUN_0000d884(int param_1,uint param_2,uint param_3)

{
  if (param_1 == 0) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x583);
    DAT_00028e84 = 1;
    return;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) != 0x11111111) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x584);
    DAT_00028e84 = 1;
    return;
  }
  if (param_2 < 0x10) {
    *(uint *)(*(int *)(param_1 + 4) + 0x10) = param_3 | 0x8000000 | (param_2 + 0x50) * 0x10000;
    return;
  }
  FUN_000014e4(s_0___P___p_0001728e + 0x12,0x585);
  DAT_00028e84 = 1;
  return;
}

