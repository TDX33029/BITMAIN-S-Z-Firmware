//==================== FUN_0000f8a4 @ 0000f8a4 ====================

char * FUN_0000f8a4(char *param_1)

{
  size_t sVar1;
  char *pcVar2;
  undefined4 *puVar3;
  char *pcVar4;
  char *pcVar5;
  char *pcVar6;
  char cVar7;
  
  sVar1 = strlen(param_1);
  pcVar2 = malloc(sVar1 * 2 + 1);
  puVar3 = malloc(8);
  cVar7 = *param_1;
  *puVar3 = pcVar2;
  puVar3[1] = DAT_00076bd8;
  DAT_00076bd8 = puVar3;
  pcVar6 = pcVar2;
  if (cVar7 != '\0') {
    pcVar4 = param_1 + -1;
    do {
      pcVar5 = pcVar6;
      if (cVar7 == '\\' || cVar7 == '\"') {
        pcVar5 = pcVar6 + 1;
        *pcVar6 = '\\';
      }
      pcVar6 = pcVar5 + 1;
      *pcVar5 = pcVar4[1];
      cVar7 = pcVar4[2];
      pcVar4 = pcVar4 + 1;
    } while (cVar7 != '\0');
  }
  *pcVar6 = '\0';
  return pcVar2;
}

