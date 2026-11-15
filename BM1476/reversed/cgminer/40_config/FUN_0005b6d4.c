//==================== FUN_0005b6d4 @ 0005b6d4 ====================

char * FUN_0005b6d4(int param_1,uint *param_2,int *param_3)

{
  char cVar1;
  char *pcVar2;
  char *pcVar3;
  uint uVar4;
  int iVar5;
  int *piVar6;
  
  uVar4 = *param_2;
  if (uVar4 < DAT_0009d774) {
    do {
      piVar6 = (int *)(DAT_0009d77c + uVar4 * 0x1c);
      if (piVar6[1] != 8) {
        if (param_1 == 0) {
          iVar5 = *piVar6;
          pcVar2 = (char *)(iVar5 + 1);
          cVar1 = *(char *)(iVar5 + 1);
          iVar5 = 0;
          if ((((cVar1 == '\0') || (cVar1 == '|')) || (cVar1 == '=')) || (cVar1 == ' '))
          goto LAB_0005b7b0;
          iVar5 = 0;
          pcVar3 = pcVar2;
          goto LAB_0005b7a0;
        }
        pcVar2 = (char *)FUN_0005b544(param_1,param_3);
        if (pcVar2 != (char *)0x0) {
          return pcVar2;
        }
        uVar4 = *param_2;
        param_1 = 0;
      }
      uVar4 = uVar4 + 1;
      *param_2 = uVar4;
    } while (uVar4 < DAT_0009d774);
  }
  return (char *)0x0;
LAB_0005b7a0:
  pcVar3 = pcVar3 + 1;
  cVar1 = *pcVar3;
  iVar5 = iVar5 + 1;
  if ((cVar1 == '\0') || (cVar1 == '|')) {
LAB_0005b7b0:
    *param_3 = iVar5;
    return pcVar2;
  }
  if ((cVar1 == '=') || (cVar1 == ' ')) goto LAB_0005b7b0;
  goto LAB_0005b7a0;
}

