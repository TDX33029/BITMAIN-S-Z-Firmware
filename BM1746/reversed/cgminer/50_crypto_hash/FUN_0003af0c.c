//==================== FUN_0003af0c @ 0003af0c ====================

void FUN_0003af0c(pthread_rwlock_t *param_1)

{
  int iVar1;
  int *piVar2;
  char acStack_1008 [4100];
  
  iVar1 = pthread_rwlock_init(param_1,(pthread_rwlockattr_t *)0x0);
  if (iVar1 != 0) {
    piVar2 = __errno_location();
    snprintf(acStack_1008,0x1000,"Failed to pthread_rwlock_init errno=%d in %s %s():%d",*piVar2,
             "driver-btm-zcash.c","bitmain_ZCASH_prepare",0x1d4b);
    FUN_00038730(3,acStack_1008,1);
    FUN_00016ca8(1);
  }
  return;
}

