//==================== FUN_0000d62c @ 0000d62c ====================

uint FUN_0000d62c(int param_1)

{
  uint uVar1;
  
  if (param_1 == 0) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x4f6);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x44c0000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    uVar1 = *(uint *)(*(int *)(param_1 + 4) + 0x14);
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x44d0000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    return uVar1 & 0x800 | *(int *)(*(int *)(param_1 + 4) + 0x14) << 0x10;
  }
  FUN_000014e4(s_0___P___p_0001728e + 0x12,0x4f7);
  DAT_00028e84 = 1;
  return 0;
}

