//==================== FUN_00012250 @ 00012250 ====================

void FUN_00012250(int param_1)

{
  int *piVar2;
  int *piVar1;
  int iVar3;
  int iVar4;
  
  iVar4 = *(int *)(param_1 + 0xbc);
  if (0 < DAT_0007b198) {
    piVar1 = DAT_0007b438;
    do {
      while( true ) {
        piVar2 = piVar1 + 1;
        iVar3 = *piVar1;
        piVar1 = piVar2;
        if (((*(char *)(iVar3 + 0x81) != '\0') || (*(int *)(iVar3 + 0x84) != 1)) ||
           ((*(char *)(iVar3 + 0x298) != '\0' && (*(char *)(iVar3 + 0x299) == '\0')))) break;
        if (*(int *)(iVar3 + 0xbc) < iVar4) {
          iVar4 = *(int *)(iVar3 + 0xbc);
          param_1 = iVar3;
        }
        if (piVar2 == DAT_0007b438 + DAT_0007b198) goto LAB_000122cc;
      }
    } while (piVar2 != DAT_0007b438 + DAT_0007b198);
LAB_000122cc:
    iVar4 = *(int *)(param_1 + 0xbc);
  }
  *(int *)(param_1 + 0xbc) = iVar4 + 1;
  return;
}

