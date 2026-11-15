//==================== FUN_00017fe4 @ 00017fe4 ====================

char FUN_00017fe4(void)

{
  int iVar1;
  
  iVar1 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007b7a8);
  if (iVar1 != 0) {
    FUN_0000b414("lock_curses",0xb77);
  }
  if (DAT_00077018 != '\0') {
    return DAT_00077018;
  }
  iVar1 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b7a8);
  if (iVar1 != 0) {
    FUN_0000b5c4("unlock_curses",0xb72);
  }
  (*DAT_00075ed8)();
  return '\0';
}

