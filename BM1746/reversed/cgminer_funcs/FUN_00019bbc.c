//==================== FUN_00019bbc @ 00019bbc ====================

void FUN_00019bbc(pthread_rwlock_t *param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  
  iVar1 = pthread_rwlock_unlock(param_1);
  if (iVar1 != 0) {
    FUN_0000b630(param_2,param_3);
  }
  (*DAT_00075ed8)();
  return;
}

