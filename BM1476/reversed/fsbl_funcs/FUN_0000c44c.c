//==================== FUN_0000c44c @ 0000c44c ====================

undefined4 FUN_0000c44c(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0xc4);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    return **(undefined4 **)(param_1 + 4);
  }
  FUN_000014e4(s_0___P___p_0001728e + 0x12,0xc5);
  DAT_00028e84 = 1;
  return 0;
}

