//==================== FUN_0003cef0 @ 0003cef0 ====================

undefined4 FUN_0003cef0(undefined4 *param_1,byte param_2,undefined1 param_3,undefined1 param_4)

{
  byte bVar1;
  undefined4 local_18;
  undefined2 local_14;
  byte local_12;
  
  local_12 = local_12 & 0x1f;
  local_14 = CONCAT11(param_4,param_3);
  local_18 = CONCAT22((ushort)(byte)((param_2 & 1) << 4 | 0x42),0xaa55);
  local_18 = CONCAT13(5,(undefined3)local_18);
  bVar1 = FUN_0003cc6c((int)&local_18 + 2,0x20);
  *(byte *)((int)param_1 + 6) = local_12 & 0xe0 | bVar1 & 0x1f;
  *param_1 = local_18;
  *(undefined2 *)(param_1 + 1) = local_14;
  return 7;
}

