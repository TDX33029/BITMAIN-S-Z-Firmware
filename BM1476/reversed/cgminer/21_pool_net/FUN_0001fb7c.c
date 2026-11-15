//==================== FUN_0001fb7c @ 0001fb7c ====================

undefined4 FUN_0001fb7c(void)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007ba10);
  if (iVar1 != 0) {
    FUN_0000b414("cp_prio",0x1b7c);
  }
  iVar1 = pthread_rwlock_rdlock((pthread_rwlock_t *)&DAT_0007ba28);
  if (iVar1 != 0) {
    FUN_0000b69c("cp_prio",0x1b7c);
  }
  iVar1 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007ba10);
  if (iVar1 != 0) {
    FUN_0000b5c4("cp_prio",0x1b7c);
  }
  uVar2 = *(undefined4 *)(DAT_0007705c + 4);
  iVar1 = pthread_rwlock_unlock((pthread_rwlock_t *)&DAT_0007ba28);
  if (iVar1 != 0) {
    FUN_0000b630("cp_prio",0x1b7e);
  }
  (*DAT_00075ed8)();
  return uVar2;
}

