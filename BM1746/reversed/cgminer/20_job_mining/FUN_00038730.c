//==================== FUN_00038730 @ 00038730 ====================

void FUN_00038730(uint param_1,char *param_2,undefined4 param_3)

{
  FILE *__s;
  tm *ptVar1;
  int iVar2;
  size_t sVar3;
  uint local_64;
  uint local_60;
  uint local_5c;
  char acStack_58 [64];
  
  local_60 = (uint)DAT_0007ad48;
  if (local_60 == 0) {
    local_5c = local_60;
    FUN_0002a84c(&local_60);
    local_64 = local_60;
    ptVar1 = localtime((time_t *)&local_64);
    snprintf(acStack_58,0x40," [%d-%02d-%02d %02d:%02d:%02d] ",ptVar1->tm_year + 0x76c,
             ptVar1->tm_mon + 1,ptVar1->tm_mday,ptVar1->tm_hour,ptVar1->tm_min,ptVar1->tm_sec);
    iVar2 = fileno(stderr);
    iVar2 = isatty(iVar2);
    if (iVar2 == 0) {
      fprintf(stderr,"%s%s\n",acStack_58,param_2);
      fflush(stderr);
    }
    if ((DAT_00078ce0 != '\0') &&
       ((DAT_00078ce4 != (FILE *)0x0 ||
        (DAT_00078ce4 = fopen((char *)&DAT_00078ce8,(char *)&DAT_00078de8),
        DAT_00078ce4 != (FILE *)0x0)))) {
      __s = DAT_00078ce4;
      sVar3 = strlen(acStack_58);
      fwrite(acStack_58,sVar3,1,__s);
      sVar3 = strlen(param_2);
      fwrite(param_2,sVar3,1,DAT_00078ce4);
      fwrite("\n",1,1,DAT_00078ce4);
      fflush(DAT_00078ce4);
    }
    FUN_000389f4(param_1,acStack_58,param_2,param_3);
  }
  else {
    syslog(param_1 | 0x80,"%s",param_2);
  }
  return;
}

