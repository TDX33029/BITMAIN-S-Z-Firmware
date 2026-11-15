//==================== FUN_0002ee78 @ 0002ee78 ====================

char * FUN_0002ee78(char *param_1,char *param_2)

{
  size_t sVar1;
  size_t sVar2;
  char *__dest;
  char acStack_1010 [4096];
  
  sVar1 = strlen(param_2);
  __dest = param_1;
  if (sVar1 != 0) {
    sVar2 = 0;
    if (param_1 != (char *)0x0) {
      sVar2 = strlen(param_1);
    }
    sVar2 = sVar1 + 1 + sVar2;
    if ((sVar2 & 3) != 0) {
      sVar2 = (sVar2 & 0xfffffffc) + 4;
    }
    __dest = malloc(sVar2);
    if (__dest == (char *)0x0) {
      snprintf(acStack_1010,0x1000,"Failed to malloc in %s %s():%d","util.c","realloc_strcat",0xe79)
      ;
      FUN_00038730(3,acStack_1010,1);
      FUN_00016ca8(1);
    }
    if (param_1 == (char *)0x0) {
      strcpy(__dest,param_2);
    }
    else {
      sprintf(__dest,"%s%s",param_1,param_2);
      free(param_1);
    }
  }
  return __dest;
}

