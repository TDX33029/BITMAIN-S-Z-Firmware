//==================== FUN_0005b544 @ 0005b544 ====================

char * FUN_0005b544(int param_1,int *param_2)

{
  char cVar1;
  char *pcVar2;
  int iVar3;
  int iVar4;
  char *pcVar5;
  
  iVar4 = param_1 + *param_2;
  cVar1 = *(char *)(param_1 + *param_2);
  if (cVar1 == ' ' || cVar1 == '=') {
    pcVar2 = (char *)0x0;
  }
  else {
    iVar3 = 0;
    if (cVar1 == '\0') {
      pcVar2 = (char *)0x0;
    }
    else {
      cVar1 = *(char *)(iVar4 + 2);
      pcVar2 = (char *)(iVar4 + 2);
      if (cVar1 == '\0') {
        iVar3 = 0;
      }
      else if (((cVar1 != '|') && (cVar1 != '=')) && (cVar1 != ' ')) {
        pcVar5 = pcVar2;
        while( true ) {
          pcVar5 = pcVar5 + 1;
          cVar1 = *pcVar5;
          iVar3 = iVar3 + 1;
          if ((cVar1 == '\0') || (cVar1 == '|')) break;
          if ((cVar1 == '=') || (cVar1 == ' ')) break;
        }
      }
      *param_2 = iVar3;
    }
  }
  return pcVar2;
}

