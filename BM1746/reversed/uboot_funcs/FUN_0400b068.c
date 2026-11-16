//==================== FUN_0400b068 @ 0400b068 ====================

void FUN_0400b068(void)

{
  int iVar1;
  undefined1 auStack_1010 [4100];
  
  iVar1 = FUN_0400ac9c(auStack_1010);
  FUN_04005ea4(iVar1,DAT_0400b0b8,0x20);
  FUN_0400ad48(DAT_0400b0bc,5,auStack_1010,(iVar1 + 0x20) - (int)auStack_1010 >> 2);
  return;
}

