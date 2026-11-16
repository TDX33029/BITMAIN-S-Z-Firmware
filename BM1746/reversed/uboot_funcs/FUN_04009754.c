//==================== FUN_04009754 @ 04009754 ====================

/* WARNING: Control flow encountered bad instruction data */

void FUN_04009754(void)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  
  iVar1 = func_0x03fbefb0(DAT_040097d8);
  iVar2 = func_0x03fbefb0(DAT_040097dc);
  if ((iVar2 != 0) && (iVar2 = FUN_04005a60(iVar2,DAT_040097e0), iVar2 == 0)) {
    return;
  }
  iVar2 = FUN_04008f68();
  if (iVar2 == 0) {
    if (iVar1 == 0) {
      return;
    }
  }
  else {
    if (iVar1 != 0) {
      uVar3 = FUN_0400972c();
      iVar1 = FUN_04005a60(iVar1,uVar3);
      if (iVar1 == 0) {
        return;
      }
    }
    FUN_0400972c();
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

