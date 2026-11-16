//==================== FUN_00015ac8 @ 00015ac8 ====================

void FUN_00015ac8(int *param_1)

{
  int iVar1;
  int *piVar3;
  int *piVar2;
  int *piVar4;
  int iVar5;
  int iVar6;
  
  iVar5 = DAT_0007b198;
  iVar6 = DAT_0007b198 + -1;
  if (0 < DAT_0007b198) {
    piVar4 = DAT_0007b438 + DAT_0007b198;
    piVar2 = DAT_0007b438;
    do {
      piVar3 = piVar2 + 1;
      iVar1 = *(int *)(*piVar2 + 4);
      if (param_1[1] < iVar1) {
        *(int *)(*piVar2 + 4) = iVar1 + -1;
      }
      piVar2 = piVar3;
    } while (piVar3 != piVar4);
  }
  if (*param_1 < iVar6) {
    *(int *)DAT_0007b438[iVar6] = *param_1;
    DAT_0007b438[*param_1] = DAT_0007b438[iVar6];
    iVar5 = DAT_0007b198;
  }
  *param_1 = iVar5;
  *(undefined1 *)((int)param_1 + 0x89) = 1;
  DAT_0007b198 = DAT_0007b198 + -1;
  return;
}

