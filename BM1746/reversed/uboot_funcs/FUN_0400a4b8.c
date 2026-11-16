//==================== FUN_0400a4b8 @ 0400a4b8 ====================

undefined4 FUN_0400a4b8(int param_1,undefined4 param_2,ushort param_3)

{
  ushort uVar1;
  undefined4 uVar2;
  ushort uVar3;
  
  FUN_04005ea4(param_1,param_2,6);
  FUN_04005ea4(param_1 + 6,DAT_0400a530);
  uVar3 = *(ushort *)(param_1 + 0xc) >> 8 | *(ushort *)(param_1 + 0xc) << 8;
  uVar1 = (ushort)DAT_0400a534 & param_3 << 8 | param_3 >> 8;
  if (uVar3 == 0x8100) {
    *(ushort *)(param_1 + 0x10) = uVar1;
    return 0x12;
  }
  if (uVar3 < 0x5eb) {
    *(ushort *)(param_1 + 0x14) = uVar1;
    uVar2 = 0x16;
  }
  else {
    uVar2 = 0xe;
    *(ushort *)(param_1 + 0xc) = uVar1;
  }
  return uVar2;
}

