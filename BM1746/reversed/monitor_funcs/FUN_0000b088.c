//==================== FUN_0000b088 @ 0000b088 ====================

undefined4 FUN_0000b088(void)

{
  int iVar1;
  size_t __size;
  char *__name;
  char *pcVar2;
  FILE *__stream;
  __pid_t _Var3;
  
  iVar1 = access(DAT_0001b2f8,0);
  pcVar2 = DAT_0001b2f8;
  if (iVar1 == 0) {
    DAT_0001c3c8 = fopen(DAT_0001b2f8,"w");
    __stream = (FILE *)0x0;
    if (DAT_0001c3c8 != (FILE *)0x0) {
      iVar1 = fileno(DAT_0001c3c8);
      iVar1 = flock(iVar1,6);
      if (iVar1 == 0) {
        _Var3 = getpid();
        fprintf(DAT_0001c3c8,"The thread executing miner-monitor:%d",_Var3);
        puts("exist,write and lock!");
        return 1;
      }
      __stream = DAT_0001c3c8;
    }
    fclose(__stream);
    return 0;
  }
  __size = strlen(DAT_0001b2f8);
  __name = malloc(__size);
  memcpy(__name,pcVar2,__size + 1);
  pcVar2 = __name;
  while (pcVar2 = strchr(pcVar2 + 1,0x2f), pcVar2 != (char *)0x0) {
    *pcVar2 = '\0';
    iVar1 = access(__name,0);
    if (iVar1 != 0) {
      mkdir(__name,0x1ff);
    }
    *pcVar2 = '/';
  }
  DAT_0001c3c8 = fopen(DAT_0001b2f8,"w");
  if (DAT_0001c3c8 != (FILE *)0x0) {
    puts("created!");
    if (DAT_0001c3c8 != (FILE *)0x0) {
      iVar1 = fileno(DAT_0001c3c8);
      iVar1 = flock(iVar1,2);
      if (iVar1 == 0) {
        _Var3 = getpid();
        fprintf(DAT_0001c3c8,"The thread executing miner-monitor:%d",_Var3);
        puts("locked!");
        return 1;
      }
    }
  }
  return 1;
}

