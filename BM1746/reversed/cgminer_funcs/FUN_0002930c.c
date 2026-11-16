//==================== FUN_0002930c @ 0002930c ====================

void FUN_0002930c(pthread_rwlock_t *param_1,char *param_2,int param_3)

{
  int iVar1;
  int *piVar2;
  char acStack_1010 [4100];
  
  iVar1 = pthread_rwlock_unlock(param_1);
  if (iVar1 != 0) {
    piVar2 = __errno_location();
    snprintf(acStack_1010,0x1000,"WTF RWLOCK ERROR ON UNLOCK! errno=%d in %s %s():%d",*piVar2,
             "util.c",param_2,param_3);
    FUN_00038730(3,acStack_1010,1);
    FUN_00016ca8(1);
  }
  return;
}

