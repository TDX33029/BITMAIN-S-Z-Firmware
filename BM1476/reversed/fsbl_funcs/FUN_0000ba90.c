//==================== FUN_0000ba90 @ 0000ba90 ====================

undefined4 FUN_0000ba90(int param_1,int param_2,int param_3)

{
  int iVar1;
  uint extraout_r2;
  uint uVar2;
  byte local_12;
  char local_11;
  
  if (param_3 == 0) {
    return 0;
  }
  uVar2 = 0;
  while( true ) {
    iVar1 = FUN_0000ba3c(*(undefined1 *)(param_1 + uVar2),&local_11);
    if ((iVar1 != 0) ||
       (iVar1 = FUN_0000ba3c(*(undefined1 *)(param_1 + uVar2 + 1),&local_12), iVar1 != 0)) break;
    *(byte *)(param_2 + (uVar2 >> 1)) = local_12 | local_11 << 4;
    uVar2 = uVar2 + 2;
    if (extraout_r2 <= uVar2) {
      return 0;
    }
  }
  return 0x11;
}

