//==================== FUN_00017a3c @ 00017a3c ====================

int * FUN_00017a3c(void)

{
  void *__ptr;
  int *piVar1;
  int iVar2;
  int iVar3;
  char local_1010 [4];
  char acStack_100c [4];
  char acStack_1008 [4];
  char acStack_1004 [4];
  char local_1000 [4];
  char acStack_ffc [4];
  char acStack_ff8 [4];
  char acStack_ff4 [4];
  undefined2 local_ff0 [2];
  char acStack_fec [4060];
  
  piVar1 = calloc(0x750,1);
  if (piVar1 == (int *)0x0) {
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
    local_1000[1] = 'p';
    local_1000[2] = 'o';
    local_1000[3] = 'o';
    acStack_ffc[0] = 'l';
    acStack_ffc[1] = ' ';
    acStack_ffc[2] = 'i';
    acStack_ffc[3] = 'n';
    acStack_ff8[0] = ' ';
    acStack_ff8[1] = 'a';
    acStack_ff8[2] = 'd';
    acStack_ff8[3] = 'd';
    acStack_ff4[0] = '_';
    acStack_ff4[1] = 'p';
    acStack_ff4[2] = 'o';
    acStack_ff4[3] = 'o';
    local_ff0[0] = 0x6c;
    FUN_00038730(3,local_1010,1);
    FUN_00016ca8(1);
  }
  __ptr = DAT_0007b438;
  iVar2 = DAT_0007b198;
  iVar3 = DAT_0007b198 + 2;
  piVar1[1] = DAT_0007b198;
  *piVar1 = iVar2;
  DAT_0007b438 = realloc(__ptr,iVar3 * 4);
  *(int **)((int)DAT_0007b438 + DAT_0007b198 * 4) = piVar1;
  DAT_0007b198 = DAT_0007b198 + 1;
  iVar2 = pthread_mutex_init((pthread_mutex_t *)(piVar1 + 0x37),(pthread_mutexattr_t *)0x0);
  if (iVar2 != 0) {
    FUN_0000b4ec("add_pool",0x2ec);
  }
  iVar2 = pthread_cond_init((pthread_cond_t *)(piVar1 + 0x52),(pthread_condattr_t *)0x0);
  if (iVar2 != 0) {
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
    acStack_1008[2] = 'p';
    acStack_1008[3] = 't';
    acStack_1004[0] = 'h';
    acStack_1004[1] = 'r';
    acStack_1004[2] = 'e';
    acStack_1004[3] = 'a';
    local_1000[0] = 'd';
    local_1000[1] = '_';
    local_1000[2] = 'c';
    local_1000[3] = 'o';
    acStack_ffc[0] = 'n';
    acStack_ffc[1] = 'd';
    acStack_ffc[2] = '_';
    acStack_ffc[3] = 'i';
    acStack_ff8[0] = 'n';
    acStack_ff8[1] = 'i';
    acStack_ff8[2] = 't';
    acStack_ff8[3] = ' ';
    acStack_ff4[0] = 'i';
    acStack_ff4[1] = 'n';
    acStack_ff4[2] = ' ';
    acStack_ff4[3] = 'a';
    local_ff0[0]._0_1_ = 'd';
    local_ff0[0]._1_1_ = 'd';
    local_ff0[1]._0_1_ = '_';
    local_ff0[1]._1_1_ = 'p';
    acStack_fec[0] = 'o';
    acStack_fec[1] = 'o';
    acStack_fec[2] = 'l';
    acStack_fec[3] = '\0';
    FUN_00038730(3,local_1010,1);
    FUN_00016ca8(1);
  }
  iVar2 = pthread_mutex_init((pthread_mutex_t *)(piVar1 + 0x3d),(pthread_mutexattr_t *)0x0);
  if (iVar2 != 0) {
    FUN_0000b4ec("add_pool",0x2ef);
  }
  iVar2 = pthread_rwlock_init((pthread_rwlock_t *)(piVar1 + 0x43),(pthread_rwlockattr_t *)0x0);
  if (iVar2 != 0) {
    FUN_0000b558("add_pool",0x2ef);
  }
  iVar2 = pthread_mutex_init((pthread_mutex_t *)(piVar1 + 0xb6),(pthread_mutexattr_t *)0x0);
  if (iVar2 != 0) {
    FUN_0000b4ec("add_pool",0x2f0);
  }
  iVar2 = pthread_mutex_init((pthread_mutex_t *)(piVar1 + 0xbf),(pthread_mutexattr_t *)0x0);
  if (iVar2 != 0) {
    FUN_0000b4ec("add_pool",0x2f1);
  }
  iVar2 = pthread_rwlock_init((pthread_rwlock_t *)(piVar1 + 0xc5),(pthread_rwlockattr_t *)0x0);
  if (iVar2 != 0) {
    FUN_0000b558("add_pool",0x2f1);
  }
  piVar1[0x5e] = (int)(piVar1 + 0x5e);
  piVar1[0x5f] = (int)(piVar1 + 0x5e);
  piVar1[0x30] = (int)"{\"method\": \"getwork\", \"params\": [], \"id\":0}\n";
  piVar1[0x2a] = -1;
  piVar1[0x36] = 0;
  piVar1[0xe] = 1;
  FUN_000152e0();
  return piVar1;
}

