//==================== FUN_0400a41c @ 0400a41c ====================

undefined4 FUN_0400a41c(int param_1,undefined4 param_2,int param_3)

{
  ushort uVar1;
  uint uVar2;
  
  uVar2 = (uint)(*DAT_0400a4ac >> 8) | (*DAT_0400a4ac & 0xff) << 8;
  if (uVar2 == 0xffff) {
    uVar2 = 0xfff;
  }
  FUN_04005ea4(param_1,param_2,6);
  FUN_04005ea4(param_1 + 6,DAT_0400a4b0);
  uVar1 = (ushort)DAT_0400a4b4 & (ushort)(param_3 << 8) | (ushort)((uint)param_3 >> 8) & 0xff;
  if ((uVar2 & 0xfff) == 0xfff) {
    *(ushort *)(param_1 + 0xc) = uVar1;
    return 0xe;
  }
  *(undefined2 *)(param_1 + 0xc) = 0x81;
  *(ushort *)(param_1 + 0xe) = (ushort)((uVar2 & 0xff) << 8) | (ushort)((uVar2 & 0xfff) >> 8);
  *(ushort *)(param_1 + 0x10) = uVar1;
  return 0x12;
}

