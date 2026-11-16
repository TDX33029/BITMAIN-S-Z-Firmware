//==================== FUN_0002f304 @ 0002f304 ====================

undefined4 FUN_0002f304(sem_t *param_1,int param_2,char *param_3,char *param_4,int param_5)

{
  int iVar1;
  int *piVar2;
  timespec tStack_1030;
  undefined4 uStack_1028;
  int local_1024;
  undefined4 uStack_1020;
  int local_101c;
  char acStack_1018 [4100];
  
  FUN_0002a84c(&uStack_1020);
  local_1024 = local_101c * 1000;
  uStack_1028 = uStack_1020;
  FUN_0002a600(&tStack_1030,local_1024,param_2,param_2 >> 0x1f);
  while( true ) {
    FUN_0002a688(&tStack_1030,&uStack_1028);
    iVar1 = sem_timedwait(param_1,&tStack_1030);
    if (iVar1 == 0) {
      return 0;
    }
    piVar2 = __errno_location();
    iVar1 = *piVar2;
    if (iVar1 == 0x6e) break;
    if (iVar1 != 4) {
      snprintf(acStack_1018,0x1000,"Failed to sem_timedwait errno=%d cgsem=0x%p in %s %s():%d",iVar1
               ,param_1,param_3,param_4,param_5);
      FUN_00038730(3,acStack_1018,1);
      FUN_00016ca8(1);
      return 0;
    }
  }
  return 0x6e;
}

