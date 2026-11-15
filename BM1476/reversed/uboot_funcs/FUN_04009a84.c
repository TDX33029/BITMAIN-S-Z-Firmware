//==================== FUN_04009a84 @ 04009a84 ====================

void FUN_04009a84(void)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  uint uVar4;
  
  piVar1 = DAT_04009af0;
  if (*DAT_04009af0 != 0) {
    uVar4 = DAT_04009af4 & 0xffffffe0;
    *DAT_04009af8 = uVar4;
    piVar2 = DAT_04009afc;
    *DAT_04009afc = uVar4 + 0x600;
    piVar2[1] = uVar4 + 0xc00;
    piVar2[2] = uVar4 + 0x1200;
    piVar2[3] = uVar4 + 0x1800;
    FUN_04007ce4();
    FUN_040098f4();
    *piVar1 = 0;
  }
  iVar3 = FUN_04008f68();
  if (iVar3 == 0) {
    return;
  }
  FUN_04009a00();
  return;
}

