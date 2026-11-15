//==================== FUN_0400aff0 @ 0400aff0 ====================

void FUN_0400aff0(uint param_1,uint param_2)

{
  int iVar1;
  undefined1 auStack_1018 [4100];
  
  iVar1 = FUN_0400ac9c(auStack_1018);
  FUN_04005ea4(iVar1,DAT_0400b060,0x20);
  *(uint *)(iVar1 + 0x20) =
       param_1 << 0x18 | (param_1 >> 8 & 0xff) << 0x10 | (param_1 >> 0x10 & 0xff) << 8 |
       param_1 >> 0x18;
  *(undefined4 *)(iVar1 + 0x28) = 0;
  *(uint *)(iVar1 + 0x24) =
       param_2 << 0x18 | (param_2 >> 8 & 0xff) << 0x10 | (param_2 >> 0x10 & 0xff) << 8 |
       param_2 >> 0x18;
  FUN_0400ad48(DAT_0400b064,6,auStack_1018,(iVar1 + 0x2c) - (int)auStack_1018 >> 2);
  return;
}

