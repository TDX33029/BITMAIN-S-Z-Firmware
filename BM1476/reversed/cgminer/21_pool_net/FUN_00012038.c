//==================== FUN_00012038 @ 00012038 ====================

int FUN_00012038(int param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  char acStack_1008 [4100];
  
  if (DAT_0007b198 < 1) {
LAB_0000b340:
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
      snprintf(acStack_1008,0x1000,"WTF No pool %d found!",param_1);
      FUN_00038730(3,acStack_1008,0);
    }
    return DAT_0007b438[param_1];
  }
  iVar2 = *DAT_0007b438;
  if (*(int *)(iVar2 + 4) != param_1) {
    iVar3 = 0;
    piVar1 = DAT_0007b438;
    do {
      iVar3 = iVar3 + 1;
      if (iVar3 == DAT_0007b198) goto LAB_0000b340;
      piVar1 = piVar1 + 1;
      iVar2 = *piVar1;
    } while (*(int *)(iVar2 + 4) != param_1);
  }
  return iVar2;
}

