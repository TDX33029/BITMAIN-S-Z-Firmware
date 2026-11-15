//==================== FUN_0000d3cc @ 0000d3cc ====================

undefined4 FUN_0000d3cc(int param_1,uint param_2)

{
  int iVar1;
  
  if (param_1 == 0) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x463);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) != 0x11111111) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x464);
    DAT_00028e84 = 1;
    return 0;
  }
  iVar1 = FUN_0000ceb8();
  if (iVar1 == 0) {
    *(uint *)(*(int *)(param_1 + 4) + 0x10) = param_2 & 0x7fe0 | 0x84a0000;
    *(uint *)(*(int *)(param_1 + 4) + 0x10) = param_2 >> 0x10 | 0x84b0000;
    return 0;
  }
  return 1;
}

