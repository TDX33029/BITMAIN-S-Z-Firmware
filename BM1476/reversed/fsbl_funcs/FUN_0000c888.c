//==================== FUN_0000c888 @ 0000c888 ====================

uint FUN_0000c888(int param_1,uint param_2)

{
  bool bVar1;
  bool bVar2;
  
  if (param_1 == 0) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x1b4);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) != 0x11111111) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x1b5);
    DAT_00028e84 = 1;
    return 0;
  }
  bVar2 = 9 < param_2;
  bVar1 = param_2 == 10;
  if (10 < param_2) {
    bVar2 = 1 < param_2 - 0xc;
    bVar1 = param_2 - 0xc == 2;
  }
  if (bVar2 && !bVar1) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x1b8);
    DAT_00028e84 = 1;
    return 0;
  }
  *(uint *)(*(int *)(param_1 + 4) + 0x10) = (param_2 + 0x20) * 0x10000 | 0x4000000;
  *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
  return *(uint *)(*(int *)(param_1 + 4) + 0x14) & 0xffff;
}

