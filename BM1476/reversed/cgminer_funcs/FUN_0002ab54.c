//==================== FUN_0002ab54 @ 0002ab54 ====================

undefined4 FUN_0002ab54(char *param_1,undefined4 *param_2,undefined4 *param_3)

{
  bool bVar1;
  char *pcVar2;
  char *pcVar3;
  char *pcVar4;
  size_t sVar5;
  undefined4 uVar6;
  size_t sVar7;
  int iVar8;
  undefined2 local_130;
  undefined1 local_12e;
  char acStack_128 [260];
  
  *param_2 = param_1;
  pcVar2 = strstr(param_1,"//");
  if (pcVar2 != (char *)0x0) {
    param_1 = pcVar2 + 2;
  }
  pcVar2 = strchr(param_1,0x5b);
  pcVar3 = strchr(param_1,0x5d);
  if (pcVar3 == (char *)0x0 || pcVar2 == (char *)0x0) {
    bVar1 = false;
  }
  else {
    bVar1 = true;
    pcVar4 = pcVar3;
    if (pcVar2 < pcVar3) goto LAB_0002abb8;
  }
  pcVar4 = param_1;
LAB_0002abb8:
  pcVar4 = strchr(pcVar4,0x3a);
  if (pcVar4 == (char *)0x0) {
    sVar7 = strlen(param_1);
    pcVar4 = (char *)0x0;
    iVar8 = 0;
  }
  else {
    sVar7 = (int)pcVar4 - (int)param_1;
    sVar5 = strlen(param_1);
    iVar8 = ~sVar7 + sVar5;
    if (iVar8 < 1) {
      return 0;
    }
    pcVar4 = pcVar4 + 1;
  }
  if ((int)sVar7 < 1) {
    return 0;
  }
  if ((bVar1) && (pcVar2 < pcVar3)) {
    sVar7 = sVar7 - 2;
    param_1 = param_1 + 1;
  }
  snprintf(acStack_128,0xfe,"%.*s",sVar7,param_1);
  if (iVar8 == 0) {
    local_130 = 0x3038;
    local_12e = 0;
  }
  else {
    snprintf((char *)&local_130,6,"%.*s",iVar8,pcVar4);
    pcVar2 = strchr((char *)&local_130,0x2f);
    if (pcVar2 != (char *)0x0) {
      *pcVar2 = '\0';
    }
  }
  uVar6 = __strdup(&local_130);
  *param_3 = uVar6;
  uVar6 = __strdup(acStack_128);
  *param_2 = uVar6;
  return 1;
}

