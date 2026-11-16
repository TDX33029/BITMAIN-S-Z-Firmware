//==================== FUN_0005b5ec @ 0005b5ec ====================

char * FUN_0005b5ec(uint *param_1,int *param_2)

{
  int *piVar1;
  char cVar2;
  int *piVar3;
  char *pcVar4;
  int iVar5;
  int *piVar6;
  char *pcVar7;
  uint uVar8;
  
  *param_1 = 0;
  if (DAT_0009d774 == 0) {
LAB_0005b6bc:
    pcVar4 = (char *)0x0;
  }
  else {
    piVar6 = DAT_0009d77c;
    if (DAT_0009d77c[1] == 8) {
      uVar8 = 1;
      piVar3 = DAT_0009d77c;
      do {
        piVar6 = piVar3 + 7;
        *param_1 = uVar8;
        if (DAT_0009d774 <= uVar8) goto LAB_0005b6bc;
        piVar1 = piVar3 + 8;
        uVar8 = uVar8 + 1;
        piVar3 = piVar6;
      } while (*piVar1 == 8);
    }
    pcVar4 = (char *)(*piVar6 + 1);
    cVar2 = *(char *)(*piVar6 + 1);
    iVar5 = 0;
    if (cVar2 != '\0') {
      if (((cVar2 == '|') || (cVar2 == '=')) || (cVar2 == ' ')) {
        iVar5 = 0;
      }
      else {
        iVar5 = 0;
        pcVar7 = pcVar4;
        while( true ) {
          pcVar7 = pcVar7 + 1;
          cVar2 = *pcVar7;
          iVar5 = iVar5 + 1;
          if ((cVar2 == '\0') || (cVar2 == '|')) break;
          if ((cVar2 == '=') || (cVar2 == ' ')) break;
        }
      }
    }
    *param_2 = iVar5;
  }
  return pcVar4;
}

