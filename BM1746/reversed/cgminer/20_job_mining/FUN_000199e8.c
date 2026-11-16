//==================== FUN_000199e8 @ 000199e8 ====================

undefined4 FUN_000199e8(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  bool bVar1;
  int iVar2;
  
  iVar2 = FUN_00017fe4();
  if (iVar2 == 0) {
    return 0;
  }
  bVar1 = 1 < param_1 - 3U;
  if (DAT_0007bac8 == '\0') {
    wprintw(DAT_0007b430,"%s%s\n",param_2,param_3,param_4);
    if (bVar1) goto LAB_00019a6c;
  }
  else {
    if (bVar1) goto LAB_00019a6c;
    wprintw(DAT_0007b430,"%s%s\n",param_2,param_3,param_4);
  }
  if (DAT_0007b430 == 0) {
    iVar2 = -1;
  }
  else {
    iVar2 = *(short *)(DAT_0007b430 + 4) + 1;
  }
  wtouchln(DAT_0007b430,0,iVar2,1);
  wrefresh(DAT_0007b430);
LAB_00019a6c:
  iVar2 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b7a8);
  if (iVar2 != 0) {
    FUN_0000b5c4("unlock_curses",0xb72);
  }
  (*DAT_00075ed8)();
  return 1;
}

