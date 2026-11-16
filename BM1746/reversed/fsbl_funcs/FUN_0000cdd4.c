//==================== FUN_0000cdd4 @ 0000cdd4 ====================

void FUN_0000cdd4(int param_1,uint param_2)

{
  bool bVar1;
  
  if (param_1 == 0) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x368);
    DAT_00028e84 = 1;
    return;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) != 0x11111111) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x369);
    DAT_00028e84 = 1;
    return;
  }
  bVar1 = 7 < param_2;
  if (param_2 != 8) {
    bVar1 = 3 < param_2;
  }
  if (bVar1 && (param_2 != 8 && param_2 != 4)) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x36b);
    DAT_00028e84 = 1;
    return;
  }
  *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x4410000;
  *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
  *(uint *)(*(int *)(param_1 + 4) + 0x10) =
       *(uint *)(*(int *)(param_1 + 4) + 0x14) & 0xfff | (param_2 & 0xf) << 0xc | 0x8410000;
  return;
}

