//==================== FUN_00003e38 @ 00003e38 ====================

int FUN_00003e38(int *param_1)

{
  int iVar1;
  char *pcVar2;
  char *pcVar4;
  char *pcVar3;
  int *piVar5;
  int iVar6;
  
  iVar6 = *param_1;
  iVar1 = FUN_00003d50();
  if (iVar1 == 0) {
    do {
      if ((param_1[6] != *(int *)(iVar6 + 0x2c)) &&
         (iVar1 = FUN_00002f3c(iVar6,param_1[6]), iVar1 != 0)) break;
      pcVar2 = (char *)param_1[7];
      if (*pcVar2 == '\0' || *pcVar2 == -0x1b) {
        if (param_1[6] != *(int *)(iVar6 + 0x2c)) {
          iVar1 = FUN_00002f3c(iVar6);
          if (iVar1 != 0) {
            return iVar1;
          }
          pcVar2 = (char *)param_1[7];
        }
        pcVar4 = pcVar2;
        do {
          pcVar3 = pcVar4 + 1;
          *pcVar4 = '\0';
          pcVar4 = pcVar3;
        } while (pcVar2 + 0x20 != pcVar3);
        piVar5 = (int *)param_1[7];
        *piVar5 = param_1[8];
        piVar5[1] = param_1[9];
        *(char *)(piVar5 + 2) = (char)param_1[10];
        *(undefined1 *)((int)piVar5 + 9) = *(undefined1 *)((int)param_1 + 0x29);
        *(undefined1 *)((int)piVar5 + 10) = *(undefined1 *)((int)param_1 + 0x2a);
        *(undefined1 *)(iVar6 + 3) = 1;
        return 0;
      }
      iVar1 = FUN_00003b44(param_1,1);
    } while (iVar1 == 0);
  }
  if (iVar1 == 4) {
    iVar1 = 7;
  }
  return iVar1;
}

