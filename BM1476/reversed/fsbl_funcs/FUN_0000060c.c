//==================== FUN_0000060c @ 0000060c ====================

void FUN_0000060c(int param_1,uint param_2)

{
  if (param_1 == 0) {
    FUN_000014e4(s_0___P___p_0001720a + 10,0xff);
    DAT_00028e84 = 1;
    return;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    **(uint **)(param_1 + 4) = param_2 | **(uint **)(param_1 + 4);
    return;
  }
  FUN_000014e4(s_0___P___p_0001720a + 10,0x100);
  DAT_00028e84 = 1;
  return;
}

