//==================== FUN_04009140 @ 04009140 ====================

undefined4 FUN_04009140(void)

{
  int iVar1;
  
  iVar1 = FUN_04008f68();
  if (iVar1 == 0) {
    return 0xffffffff;
  }
  iVar1 = FUN_04008f68();
  return *(undefined4 *)(iVar1 + 0x50);
}

