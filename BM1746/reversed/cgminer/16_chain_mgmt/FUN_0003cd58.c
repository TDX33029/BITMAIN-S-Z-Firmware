//==================== FUN_0003cd58 @ 0003cd58 ====================

undefined4 FUN_0003cd58(undefined1 *param_1)

{
  byte bVar1;
  undefined4 local_10;
  byte local_c;
  
  local_c = 0;
  local_10 = 0x553;
  bVar1 = FUN_0003cc6c(&local_10,0x20);
  *param_1 = 0x55;
  param_1[1] = 0xaa;
  param_1[6] = local_c & 0xe0 | bVar1 & 0x1f;
  *(undefined4 *)(param_1 + 2) = local_10;
  return 7;
}

