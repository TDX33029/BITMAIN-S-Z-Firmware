//==================== FUN_04009118 @ 04009118 ====================

void FUN_04009118(void)

{
  int iVar1;
  
  iVar1 = FUN_04008f68();
  if (iVar1 == 0) {
    return;
  }
  if ((*(uint *)(iVar1 + 0x48) & 1) != 0) {
    **(undefined4 **)(iVar1 + 0x28) = 1;
  }
  return;
}

