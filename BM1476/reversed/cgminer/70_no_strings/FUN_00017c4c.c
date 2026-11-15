//==================== FUN_00017c4c @ 00017c4c ====================

char * FUN_00017c4c(undefined4 param_1)

{
  char *pcVar1;
  void *pvVar2;
  int iVar3;
  
  if ((DAT_00077008 == 0) && (DAT_0007700c == 0)) {
    DAT_00077010 = DAT_00077010 + 1;
    if (DAT_0007b198 < DAT_00077010) {
      FUN_00017a3c();
    }
    iVar3 = *(int *)(DAT_0007b438 + (DAT_00077010 + -1) * 4);
    pcVar1 = (char *)__strdup(param_1);
    FUN_0005b1d4(param_1,iVar3 + 200);
    pcVar1 = strtok(pcVar1,":");
    *(char **)(iVar3 + 0xcc) = pcVar1;
    if (pcVar1 == (char *)0x0) {
      pcVar1 = "Failed to find : delimited user info";
    }
    else {
      pcVar1 = strtok((char *)0x0,":");
      *(char **)(iVar3 + 0xd0) = pcVar1;
      if (pcVar1 == (char *)0x0) {
        pvVar2 = calloc(1,1);
        *(void **)(iVar3 + 0xd0) = pvVar2;
        pcVar1 = (char *)0x0;
      }
      else {
        pcVar1 = (char *)0x0;
      }
    }
  }
  else {
    pcVar1 = "Use only user + pass or userpass, but not both";
  }
  return pcVar1;
}

