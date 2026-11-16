//==================== FUN_0001da58 @ 0001da58 ====================

undefined4 FUN_0001da58(void)

{
  undefined4 uVar1;
  int iVar2;
  
  iVar2 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007ba10);
  if (iVar2 != 0) {
    FUN_0000b414("current_pool",0x31c);
  }
  iVar2 = pthread_rwlock_rdlock((pthread_rwlock_t *)&DAT_0007ba28);
  if (iVar2 != 0) {
    FUN_0000b69c("current_pool",0x31c);
  }
  iVar2 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007ba10);
  if (iVar2 != 0) {
    FUN_0000b5c4("current_pool",0x31c);
  }
  uVar1 = DAT_0007705c;
  iVar2 = pthread_rwlock_unlock((pthread_rwlock_t *)&DAT_0007ba28);
  if (iVar2 != 0) {
    FUN_0000b630("current_pool",0x31e);
  }
  (*DAT_00075ed8)();
  return uVar1;
}

