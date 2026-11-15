//==================== FUN_04008fa0 @ 04008fa0 ====================

void FUN_04008fa0(int param_1)

{
  int iVar1;
  int *piVar2;
  
  if (param_1 != 0) {
    if ((*(uint *)(param_1 + 0x48) & 1) != 0) goto LAB_04008fd0;
    iVar1 = func_0x03fd031c();
    *DAT_04008fdc = iVar1;
    if (iVar1 == 0) goto LAB_04008fd0;
  }
  param_1 = 0;
LAB_04008fd0:
  piVar2 = (int *)FUN_04008c70();
  *piVar2 = param_1;
  return;
}

