//==================== FUN_0000dd80 @ 0000dd80 ====================

void FUN_0000dd80(void)

{
  int iVar1;
  
  iVar1 = FUN_0000c540();
  if (iVar1 != 0) {
    *(undefined4 *)(iVar1 + 0xc) = 1;
  }
  return;
}

