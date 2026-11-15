//==================== FUN_040090f8 @ 040090f8 ====================

int FUN_040090f8(void)

{
  int iVar1;
  
  iVar1 = FUN_04008f68();
  if (iVar1 == 0) {
    return 0;
  }
  iVar1 = FUN_04008f68();
  return *(int *)(iVar1 + 8) + 4;
}

