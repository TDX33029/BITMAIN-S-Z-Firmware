//==================== FUN_00029e48 @ 00029e48 ====================

undefined1 * FUN_00029e48(char *param_1,int *param_2)

{
  size_t __n;
  undefined1 *puVar1;
  char local_1018 [4];
  char acStack_1014 [4];
  char acStack_1010 [4];
  char acStack_100c [4];
  char local_1008 [4];
  char acStack_1004 [4];
  char acStack_1000 [4];
  char acStack_ffc [4];
  undefined2 uStack_ff8;
  undefined1 local_ff6;
  
  __n = strlen(param_1);
  puVar1 = malloc(__n + 9);
  if (puVar1 == (undefined1 *)0x0) {
    local_1018[0] = 'F';
    local_1018[1] = 'a';
    local_1018[2] = 'i';
    local_1018[3] = 'l';
    acStack_1014[0] = 'e';
    acStack_1014[1] = 'd';
    acStack_1014[2] = ' ';
    acStack_1014[3] = 't';
    acStack_1010[0] = 'o';
    acStack_1010[1] = ' ';
    acStack_1010[2] = 'm';
    acStack_1010[3] = 'a';
    acStack_100c[0] = 'l';
    acStack_100c[1] = 'l';
    acStack_100c[2] = 'o';
    acStack_100c[3] = 'c';
    local_1008[0] = ' ';
    local_1008[1] = 'r';
    local_1008[2] = 'e';
    local_1008[3] = 't';
    acStack_1004[0] = ' ';
    acStack_1004[1] = 'i';
    acStack_1004[2] = 'n';
    acStack_1004[3] = ' ';
    acStack_1000[0] = 's';
    acStack_1000[1] = 'e';
    acStack_1000[2] = 'r';
    acStack_1000[3] = '_';
    acStack_ffc[0] = 's';
    acStack_ffc[1] = 't';
    acStack_ffc[2] = 'r';
    acStack_ffc[3] = 'i';
    uStack_ff8 = 0x676e;
    local_ff6 = 0;
    FUN_00038730(3,local_1018,1);
    FUN_00016ca8(1);
  }
  if (__n < 0xfd) {
    *puVar1 = (char)__n;
    memcpy(puVar1 + 1,param_1,__n);
    *param_2 = __n + 1;
  }
  else if (__n < 0x10000) {
    *(ushort *)(puVar1 + 1) = (ushort)(__n >> 8) & 0xff | (ushort)(__n << 8);
    *puVar1 = 0xfd;
    memcpy(puVar1 + 3,param_1,__n);
    *param_2 = __n + 3;
  }
  else {
    *puVar1 = 0xfe;
    *(size_t *)(puVar1 + 1) =
         __n << 0x18 | (__n >> 8 & 0xff) << 0x10 | (__n >> 0x10 & 0xff) << 8 | __n >> 0x18;
    memcpy(puVar1 + 5,param_1,__n);
    *param_2 = __n + 5;
  }
  return puVar1;
}

