//==================== FUN_0000d300 @ 0000d300 ====================

uint FUN_0000d300(int param_1)

{
  uint uVar1;
  
  if (param_1 == 0) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x434);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x4480000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    uVar1 = *(uint *)(*(int *)(param_1 + 4) + 0x14);
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x4490000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    return uVar1 & 0x7fe1 | *(int *)(*(int *)(param_1 + 4) + 0x14) << 0x10;
  }
  FUN_000014e4(s_0___P___p_0001728e + 0x12,0x435);
  DAT_00028e84 = 1;
  return 0;
}

