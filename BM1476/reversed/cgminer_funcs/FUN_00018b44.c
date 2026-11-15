//==================== FUN_00018b44 @ 00018b44 ====================

void FUN_00018b44(void)

{
  int iVar1;
  
  iVar1 = FUN_00017fe4();
  if (iVar1 == 0) {
    return;
  }
  if (DAT_0007b430 == 0) {
    iVar1 = -1;
  }
  else {
    iVar1 = *(short *)(DAT_0007b430 + 4) + 1;
  }
  wtouchln(DAT_0007b430,0,iVar1,1);
  wrefresh(DAT_0007b430);
  iVar1 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b7a8);
  if (iVar1 != 0) {
    FUN_0000b5c4("unlock_curses",0xb72);
  }
  (*DAT_00075ed8)();
  return;
}

