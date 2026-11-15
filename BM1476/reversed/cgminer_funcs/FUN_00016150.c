//==================== FUN_00016150 @ 00016150 ====================

void FUN_00016150(char *param_1)

{
  char *pcVar1;
  size_t sVar2;
  
  if ((DAT_00076bdc != (char *)0x0) && (*DAT_00076bdc != '\0')) {
    strcpy(param_1,DAT_00076bdc);
    return;
  }
  pcVar1 = getenv("HOME");
  if ((pcVar1 == (char *)0x0) || (pcVar1 = getenv("HOME"), *pcVar1 == '\0')) {
    *param_1 = '\0';
  }
  else {
    pcVar1 = getenv("HOME");
    pcVar1 = stpcpy(param_1,pcVar1);
    pcVar1[0] = '/';
    pcVar1[1] = '\0';
  }
  sVar2 = strlen(param_1);
  builtin_strncpy(param_1 + sVar2,".cgminer/",10);
  mkdir(param_1,0x1ff);
  sVar2 = strlen(param_1);
  builtin_strncpy(param_1 + sVar2,"cgminer.conf",0xd);
  return;
}

