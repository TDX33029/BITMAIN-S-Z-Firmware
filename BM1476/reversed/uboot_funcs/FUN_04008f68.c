//==================== FUN_04008f68 @ 04008f68 ====================

int FUN_04008f68(void)

{
  int *piVar1;
  undefined4 uVar2;
  
  piVar1 = (int *)FUN_04008c70();
  if (*piVar1 == 0) {
    uVar2 = func_0x03fd1098(0x13,piVar1);
    *DAT_04008f9c = uVar2;
  }
  return *piVar1;
}

