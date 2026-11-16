//==================== FUN_04005b44 @ 04005b44 ====================

char * FUN_04005b44(char *param_1,char param_2)

{
  char cVar1;
  char *pcVar2;
  
  cVar1 = *param_1;
  pcVar2 = param_1;
  while (cVar1 != '\0') {
    pcVar2 = pcVar2 + 1;
    cVar1 = *pcVar2;
  }
  cVar1 = '\0';
  while( true ) {
    if (cVar1 == param_2) {
      return pcVar2;
    }
    pcVar2 = pcVar2 + -1;
    if (pcVar2 < param_1) break;
    cVar1 = *pcVar2;
  }
  return (char *)0x0;
}

