//==================== FUN_0000c314 @ 0000c314 ====================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_0000c314(undefined2 *param_1,undefined2 *param_2,undefined4 param_3)

{
  undefined2 uVar1;
  
  if (param_1 == (undefined2 *)0x0) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x6e);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (param_2 == (undefined2 *)0x0) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x6f);
    DAT_00028e84 = 1;
    return 0;
  }
  uVar1 = *param_2;
  *(undefined4 *)(param_1 + 2) = param_3;
  *param_1 = uVar1;
  _DAT_f8007034 = 0x757bdf0d;
  **(uint **)(param_1 + 2) = **(uint **)(param_1 + 2) | 0x80ff0000;
  *(undefined4 *)(*(int *)(param_1 + 2) + 0x18) = 0;
  *(undefined4 *)(param_1 + 4) = 0x11111111;
  return 0;
}

