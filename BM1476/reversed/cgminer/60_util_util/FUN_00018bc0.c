//==================== FUN_00018bc0 @ 00018bc0 ====================

void FUN_00018bc0(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = FUN_00017fe4();
  if (iVar1 == 0) {
    return;
  }
  wprintw(DAT_0007b430,"%s",param_1);
  iVar1 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b7a8);
  if (iVar1 != 0) {
    FUN_0000b5c4("unlock_curses",0xb72);
  }
  (*DAT_00075ed8)();
  return;
}

