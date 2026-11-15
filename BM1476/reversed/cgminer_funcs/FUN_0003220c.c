//==================== FUN_0003220c @ 0003220c ====================

void FUN_0003220c(pthread_mutex_t *param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  
  iVar1 = pthread_mutex_unlock(param_1);
  if (iVar1 != 0) {
    FUN_0000b84c(param_2,param_3);
  }
  (*DAT_00075ed8)();
  return;
}

