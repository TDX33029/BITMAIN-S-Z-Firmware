//==================== FUN_000179bc @ 000179bc ====================

void FUN_000179bc(pthread_rwlock_t *param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  
  iVar1 = pthread_rwlock_init(param_1,(pthread_rwlockattr_t *)0x0);
  if (iVar1 == 0) {
    return;
  }
  FUN_0000b558(param_2,param_3);
  return;
}

