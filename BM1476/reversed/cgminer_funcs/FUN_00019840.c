//==================== FUN_00019840 @ 00019840 ====================

void FUN_00019840(void)

{
  int iVar1;
  
  iVar1 = pthread_mutex_lock(DAT_0007704c);
  if (iVar1 != 0) {
    FUN_0000b414("wake_gws",0x1454);
  }
  pthread_cond_signal((pthread_cond_t *)&DAT_0007b1a0);
  iVar1 = pthread_mutex_unlock(DAT_0007704c);
  if (iVar1 != 0) {
    FUN_0000b5c4("wake_gws",0x1456);
  }
  (*DAT_00075ed8)();
  return;
}

