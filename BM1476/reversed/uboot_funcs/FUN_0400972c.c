//==================== FUN_0400972c @ 0400972c ====================

undefined4 FUN_0400972c(void)

{
  int iVar1;
  
  iVar1 = FUN_04008f68();
  if (iVar1 == 0) {
    return DAT_04009750;
  }
  iVar1 = FUN_04008f68();
  return *(undefined4 *)(iVar1 + 4);
}

