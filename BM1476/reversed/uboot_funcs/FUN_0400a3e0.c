//==================== FUN_0400a3e0 @ 0400a3e0 ====================

undefined4 FUN_0400a3e0(void)

{
  undefined4 uVar1;
  ushort uVar2;
  
  uVar2 = *DAT_0400a418 >> 8 | *DAT_0400a418 << 8;
  if (uVar2 == 0xffff) {
    uVar2 = 0xfff;
  }
  if ((uVar2 & 0xfff) == 0xfff) {
    uVar1 = 0xe;
  }
  else {
    uVar1 = 0x12;
  }
  return uVar1;
}

