//==================== FUN_00017e20 @ 00017e20 ====================

char * FUN_00017e20(char *param_1)

{
  char *pcVar1;
  size_t sVar2;
  size_t sVar3;
  long param4;
  int *piVar4;
  char acStack_1018 [4100];
  
  pcVar1 = strchr(param_1,0x3b);
  if (pcVar1 == (char *)0x0) {
    pcVar1 = "No semicolon separated quota;URL pair found";
  }
  else {
    sVar2 = strlen(param_1);
    *pcVar1 = '\0';
    sVar3 = strlen(param_1);
    if (sVar3 == 0) {
      pcVar1 = "No parameter for quota found";
    }
    else if ((int)(~sVar3 + sVar2) < 1) {
      pcVar1 = "No parameter for URL found";
    }
    else {
      param4 = strtol(param_1,(char **)0x0,10);
      if (param4 < 0) {
        pcVar1 = "Invalid negative parameter for quota set";
      }
      else {
        piVar4 = (int *)FUN_00017db8(param4);
        FUN_00017100(piVar4,param_1 + sVar3 + 1);
        piVar4[0xe] = param4;
        if ((DAT_00078e09 != '\0') &&
           (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
          snprintf(acStack_1018,0x1000,"Setting pool %d to quota %d",*piVar4,param4);
          FUN_00038730(7,acStack_1018,0);
        }
        FUN_000152e0();
        pcVar1 = (char *)0x0;
      }
    }
  }
  return pcVar1;
}

