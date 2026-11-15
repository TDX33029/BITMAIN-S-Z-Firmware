//==================== FUN_00045d88 @ 00045d88 ====================

void FUN_00045d88(int param_1)

{
  int iVar1;
  uint uVar2;
  
  iVar1 = DAT_00078e10;
  if (param_1 < 0x28) {
    param_1 = 0x28;
  }
  else if (100 < param_1) {
    uVar2 = 0x640000;
    DAT_0009d6ca = 100;
    goto LAB_00045da4;
  }
  if (DAT_00079126 == '\0') {
    DAT_0009d6ca = (undefined1)param_1;
    uVar2 = 100U - param_1 | param_1 << 0x10;
  }
  else {
    uVar2 = 0x640000;
    DAT_0009d6ca = 100;
  }
LAB_00045da4:
  *(uint *)(DAT_00078e10 + 0x84) = uVar2;
  *(uint *)(iVar1 + 0xa0) = uVar2;
  return;
}

