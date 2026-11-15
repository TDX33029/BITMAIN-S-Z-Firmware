//==================== FUN_04006044 @ 04006044 ====================

char * FUN_04006044(char *param_1,char *param_2)

{
  char cVar1;
  char *pcVar2;
  int iVar3;
  int iVar4;
  char cVar5;
  char *pcVar6;
  
  if (*param_2 == '\0') {
    return param_1;
  }
  pcVar2 = param_2;
  do {
    pcVar2 = pcVar2 + 1;
  } while (*pcVar2 != '\0');
  iVar4 = (int)pcVar2 - (int)param_2;
  if (iVar4 == 0) {
    return param_1;
  }
  cVar1 = *param_1;
  if (cVar1 == '\0') {
    iVar3 = 0;
  }
  else {
    pcVar2 = param_1;
    do {
      pcVar2 = pcVar2 + 1;
    } while (*pcVar2 != '\0');
    iVar3 = (int)pcVar2 - (int)param_1;
  }
  if (iVar4 <= iVar3) {
    while( true ) {
      iVar3 = iVar3 + -1;
      pcVar2 = param_1;
      pcVar6 = param_2;
      cVar5 = *param_2;
      while (cVar1 == cVar5) {
        if (param_1 + iVar4 + -1 == pcVar2) {
          return param_1;
        }
        pcVar2 = pcVar2 + 1;
        pcVar6 = pcVar6 + 1;
        cVar5 = *pcVar6;
        cVar1 = *pcVar2;
      }
      if (iVar3 < iVar4) break;
      param_1 = param_1 + 1;
      cVar1 = *param_1;
    }
  }
  return (char *)0x0;
}

