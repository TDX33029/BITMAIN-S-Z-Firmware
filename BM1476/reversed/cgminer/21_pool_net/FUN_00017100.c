//==================== FUN_00017100 @ 00017100 ====================

void FUN_00017100(int param_1,undefined4 param_2)

{
  char *__s1;
  int iVar1;
  char *__dest;
  char local_1010 [4];
  char acStack_100c [4];
  char acStack_1008 [4];
  char acStack_1004 [4];
  char local_1000 [4];
  char acStack_ffc [4];
  undefined2 uStack_ff8;
  undefined1 local_ff6;
  
  __s1 = (char *)FUN_0002ace0(param_2,param_1);
  iVar1 = FUN_0001557c(param_1,__s1);
  if (iVar1 == 0) {
    FUN_0005b1d4(__s1,param_1 + 0xc4);
    iVar1 = strncmp(__s1,"http://",7);
    if ((iVar1 != 0) && (iVar1 = strncmp(__s1,"https://",8), iVar1 != 0)) {
      __dest = malloc(0x100);
      if (__dest == (char *)0x0) {
        local_1010[0] = 'F';
        local_1010[1] = 'a';
        local_1010[2] = 'i';
        local_1010[3] = 'l';
        acStack_100c[0] = 'e';
        acStack_100c[1] = 'd';
        acStack_100c[2] = ' ';
        acStack_100c[3] = 't';
        acStack_1008[0] = 'o';
        acStack_1008[1] = ' ';
        acStack_1008[2] = 'm';
        acStack_1008[3] = 'a';
        acStack_1004[0] = 'l';
        acStack_1004[1] = 'l';
        acStack_1004[2] = 'o';
        acStack_1004[3] = 'c';
        local_1000[0] = ' ';
        local_1000[1] = 'h';
        local_1000[2] = 't';
        local_1000[3] = 't';
        acStack_ffc[0] = 'p';
        acStack_ffc[1] = 'i';
        acStack_ffc[2] = 'n';
        acStack_ffc[3] = 'p';
        uStack_ff8 = 0x7475;
        local_ff6 = 0;
        FUN_00038730(3,local_1010,1);
        FUN_00016ca8(1);
      }
      builtin_strncpy(__dest,"stratum+tcp://",0xf);
      strncat(__dest,__s1,0xf2);
      FUN_0001557c(param_1,__dest);
    }
  }
  return;
}

