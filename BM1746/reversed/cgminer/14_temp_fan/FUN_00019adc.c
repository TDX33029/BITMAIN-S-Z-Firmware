//==================== FUN_00019adc @ 00019adc ====================

void FUN_00019adc(void)

{
  int iVar1;
  undefined4 in_r3;
  
  pthread_mutex_trylock((pthread_mutex_t *)&DAT_0007b7a8);
  if (DAT_00077018 == '\0') {
    iVar1 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b7a8);
  }
  else {
    DAT_00075dd0 = 0;
    DAT_00077018 = '\0';
    leaveok(DAT_0007b430,0,&DAT_000755f0,&DAT_00076bd8,in_r3);
    leaveok(DAT_0007b7c0,0);
    leaveok(DAT_0007bc90,0);
    nocbreak();
    echo();
    delwin(DAT_0007b430);
    delwin(DAT_0007b7c0);
    delwin(DAT_0007bc90);
    endwin();
    iVar1 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b7a8);
  }
  if (iVar1 != 0) {
    FUN_0000b5c4("unlock_curses",0xb72);
  }
  (*DAT_00075ed8)();
  return;
}

