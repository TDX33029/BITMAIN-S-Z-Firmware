//==================== FUN_0000c6dc @ 0000c6dc ====================

uint FUN_0000c6dc(int param_1,uint param_2)

{
  bool bVar1;
  bool bVar2;
  
  if (param_1 == 0) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x164);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) != 0x11111111) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x165);
    DAT_00028e84 = 1;
    return 0;
  }
  bVar2 = 5 < param_2;
  bVar1 = param_2 == 6;
  if (6 < param_2) {
    bVar2 = 0x11 < param_2 - 0xd;
    bVar1 = param_2 - 0xd == 0x12;
  }
  if (bVar2 && !bVar1) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x168);
    DAT_00028e84 = 1;
    return 0;
  }
  *(uint *)(*(int *)(param_1 + 4) + 0x10) = param_2 << 0x10 | 0x4000000;
  *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
  return *(uint *)(*(int *)(param_1 + 4) + 0x14) & 0xffff;
}

