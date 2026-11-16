//==================== FUN_0001ab24 @ 0001ab24 ====================

void FUN_0001ab24(pthread_rwlock_t *param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  
  iVar1 = pthread_rwlock_rdlock(param_1);
  if (iVar1 == 0) {
    return;
  }
  FUN_0000b69c(param_2,param_3);
  return;
}

