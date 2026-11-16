//==================== FUN_0002ace0 @ 0002ace0 ====================

char * FUN_0002ace0(char *param_1,int param_2)

{
  int iVar1;
  char *__dest;
  char *__src;
  char *pcVar2;
  int iVar3;
  size_t __n;
  char acStack_1020 [4100];
  
  pcVar2 = "http:";
  iVar3 = 0;
  __n = 5;
  *(undefined4 *)(param_2 + 0xd8) = 0;
  while( true ) {
    iVar1 = strncmp(param_1,pcVar2,__n);
    if (iVar1 == 0) {
      pcVar2 = strchr(param_1,0x7c);
      if (pcVar2 != (char *)0x0) {
        *pcVar2 = '\0';
        __dest = malloc((size_t)(pcVar2 + ((1 - (int)param_1) - __n)));
        *(char **)(param_2 + 0xd8) = __dest;
        if (__dest == (char *)0x0) {
          snprintf(acStack_1020,0x1000,"Failed to malloc rpc_proxy in %s %s():%d","util.c",
                   "get_proxy",0x371);
          FUN_00038730(3,acStack_1020,1);
          FUN_00016ca8(1);
          __dest = *(char **)(param_2 + 0xd8);
        }
        __src = param_1 + __n;
        param_1 = pcVar2 + 1;
        strcpy(__dest,__src);
        FUN_0002ab54(*(undefined4 *)(param_2 + 0xd8),param_2 + 0x274,param_2 + 0x278);
        *(undefined4 *)(param_2 + 0xd4) = (&DAT_00064e40)[iVar3 * 2];
      }
      return param_1;
    }
    iVar3 = iVar3 + 1;
    pcVar2 = *(char **)(&UNK_00064e3c + iVar3 * 8);
    if (pcVar2 == (char *)0x0) break;
    __n = strlen(pcVar2);
  }
  return param_1;
}

