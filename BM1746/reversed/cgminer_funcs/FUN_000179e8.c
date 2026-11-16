//==================== FUN_000179e8 @ 000179e8 ====================

void FUN_000179e8(pthread_mutex_t *param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  
  iVar1 = pthread_mutex_init(param_1,(pthread_mutexattr_t *)0x0);
  if (iVar1 != 0) {
    FUN_0000b4ec(param_2,param_3);
  }
  iVar1 = pthread_rwlock_init((pthread_rwlock_t *)(param_1 + 1),(pthread_rwlockattr_t *)0x0);
  if (iVar1 == 0) {
    return;
  }
  FUN_0000b558(param_2,param_3);
  return;
}

