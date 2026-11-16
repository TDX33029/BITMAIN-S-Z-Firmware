//==================== FUN_00018068 @ 00018068 ====================

void FUN_00018068(void)

{
  int iVar1;
  undefined4 in_r3;
  int iVar2;
  
  if (DAT_00075dd0 == '\0') {
    return;
  }
  iVar1 = FUN_00017fe4();
  if (iVar1 == 0) {
    return;
  }
  werase(stdscr);
  if (DAT_0007b7c0 == 0) {
    iVar1 = -1;
  }
  else {
    iVar1 = *(short *)(DAT_0007b7c0 + 6) + 1;
  }
  DAT_00077020 = DAT_0007701c;
  if (LINES + -1 <= DAT_0007701c) {
    DAT_00077020 = LINES + -2;
  }
  DAT_00077024 = DAT_00077020;
  wresize(DAT_0007b7c0,DAT_00077020,iVar1,DAT_00077020,in_r3);
  if (DAT_0007bc90 == 0) {
    iVar2 = -1;
    iVar1 = iVar2;
  }
  else {
    iVar1 = *(short *)(DAT_0007bc90 + 4) + 1;
    iVar2 = *(short *)(DAT_0007bc90 + 6) + 1;
  }
  wresize(DAT_0007b430,iVar1 - DAT_00077024,iVar2,iVar1,in_r3);
  mvwin(DAT_0007b430,DAT_00077024,0);
  iVar1 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b7a8);
  if (iVar1 != 0) {
    FUN_0000b5c4("unlock_curses",0xb72);
  }
  (*DAT_00075ed8)();
  return;
}

