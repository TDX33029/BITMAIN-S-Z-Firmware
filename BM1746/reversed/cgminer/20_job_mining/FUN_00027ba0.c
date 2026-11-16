//==================== FUN_00027ba0 @ 00027ba0 ====================

void FUN_00027ba0(void)

{
  int iVar1;
  int iVar2;
  
  iVar1 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007b7a8);
  if (iVar1 != 0) {
    FUN_0000b414("lock_curses",0xb77);
  }
  if (DAT_00077018 == '\0') {
    DAT_0007bc90 = initscr();
    if (DAT_0007bc90 == 0) {
      iVar2 = -1;
      iVar1 = iVar2;
    }
    else {
      iVar1 = *(short *)(DAT_0007bc90 + 6) + 1;
      iVar2 = *(short *)(DAT_0007bc90 + 4) + 1;
    }
    DAT_0007b7c0 = newwin(DAT_0007701c,iVar1,0,0);
    leaveok(DAT_0007b7c0,1);
    DAT_0007b430 = newwin(iVar2 - DAT_00077024,0,DAT_00077024,0);
    idlok(DAT_0007b430,1);
    scrollok(DAT_0007b430,1);
    leaveok(DAT_0007b430,1);
    cbreak();
    noecho();
    DAT_00077018 = '\x01';
    DAT_00077020 = DAT_0007701c;
    iVar1 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b7a8);
  }
  else {
    iVar1 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b7a8);
  }
  if (iVar1 != 0) {
    FUN_0000b5c4("unlock_curses",0xb72);
  }
  (*DAT_00075ed8)();
  return;
}

