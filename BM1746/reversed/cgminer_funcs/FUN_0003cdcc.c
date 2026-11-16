//==================== FUN_0003cdcc @ 0003cdcc ====================

undefined4
FUN_0003cdcc(undefined4 *param_1,byte param_2,undefined1 param_3,undefined1 param_4,uint param_5)

{
  byte bVar1;
  undefined4 local_1c;
  undefined1 local_18;
  undefined1 local_17;
  ushort local_16;
  ushort uStack_14;
  byte local_12;
  
  uStack_14 = (ushort)((param_5 << 0x18) >> 0x10) | (ushort)(param_5 >> 8) & 0xff;
  local_16 = (ushort)((param_5 >> 0x10 & 0xff) << 8) | (ushort)(byte)(param_5 >> 0x18);
  local_1c = CONCAT22((ushort)(byte)((param_2 & 1) << 4 | 0x41),0xaa55);
  local_1c = CONCAT13(9,(undefined3)local_1c);
  local_12 = local_12 & 0x1f;
  local_18 = param_3;
  local_17 = param_4;
  bVar1 = FUN_0003cc6c((int)&local_1c + 2,0x40);
  *param_1 = local_1c;
  param_1[1] = CONCAT22(local_16,CONCAT11(local_17,local_18));
  *(ushort *)(param_1 + 2) = uStack_14;
  *(byte *)((int)param_1 + 10) = local_12 & 0xe0 | bVar1 & 0x1f;
  return 0xb;
}

