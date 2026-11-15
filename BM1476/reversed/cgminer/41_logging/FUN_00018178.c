//==================== FUN_00018178 @ 00018178 ====================

void FUN_00018178(void)

{
  int iVar1;
  
  if (DAT_0007b940 == '\0') {
    iVar1 = DAT_00077028 + DAT_0007702c;
  }
  else {
    iVar1 = DAT_00077028;
  }
  DAT_0007701c = iVar1 + 1;
  DAT_00077024 = iVar1 + 2;
  iVar1 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b7a8);
  if (iVar1 != 0) {
    FUN_0000b5c4("unlock_curses",0xb72);
  }
  (*DAT_00075ed8)();
  FUN_00018068();
  return;
}

