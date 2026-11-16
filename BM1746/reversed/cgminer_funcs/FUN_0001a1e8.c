//==================== FUN_0001a1e8 @ 0001a1e8 ====================

int FUN_0001a1e8(void)

{
  int iVar1;
  int iVar2;
  
  iVar1 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007ba10);
  if (iVar1 != 0) {
    FUN_0000b414("total_work_inc",0x8e1);
  }
  iVar1 = pthread_rwlock_wrlock((pthread_rwlock_t *)&DAT_0007ba28);
  if (iVar1 != 0) {
    FUN_0000b480("total_work_inc",0x8e1);
  }
  iVar1 = DAT_000770a8;
  DAT_000770a8 = DAT_000770a8 + 1;
  iVar2 = pthread_rwlock_unlock((pthread_rwlock_t *)&DAT_0007ba28);
  if (iVar2 != 0) {
    FUN_0000b630("total_work_inc",0x8e3);
  }
  iVar2 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007ba10);
  if (iVar2 != 0) {
    FUN_0000b5c4("total_work_inc",0x8e3);
  }
  (*DAT_00075ed8)();
  return iVar1;
}

