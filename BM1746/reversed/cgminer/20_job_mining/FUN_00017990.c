//==================== FUN_00017990 @ 00017990 ====================

void FUN_00017990(pthread_mutex_t *param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  
  iVar1 = pthread_mutex_init(param_1,(pthread_mutexattr_t *)0x0);
  if (iVar1 == 0) {
    return;
  }
  FUN_0000b4ec(param_2,param_3);
  return;
}

