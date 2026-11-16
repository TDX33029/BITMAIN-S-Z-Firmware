//==================== FUN_00018ad8 @ 00018ad8 ====================

void FUN_00018ad8(void)

{
  int iVar1;
  
  iVar1 = FUN_00017fe4();
  if (iVar1 == 0) {
    return;
  }
  werase(stdscr);
  wclear(DAT_0007b430);
  iVar1 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b7a8);
  if (iVar1 != 0) {
    FUN_0000b5c4("unlock_curses",0xb72);
  }
  (*DAT_00075ed8)();
  return;
}

