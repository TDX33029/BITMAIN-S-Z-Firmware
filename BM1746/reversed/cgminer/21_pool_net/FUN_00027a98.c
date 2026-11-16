//==================== FUN_00027a98 @ 00027a98 ====================

undefined4 FUN_00027a98(void)

{
  int iVar1;
  char local_11 [5];
  
  pthread_setcanceltype(1,(int *)0x0);
  FUN_0002f0b4("Input");
  if (DAT_00077018 != '\0') {
    do {
      while( true ) {
        local_11[0] = wgetch(stdscr);
        iVar1 = strncasecmp(local_11,"q",1);
        if (iVar1 == 0) {
          FUN_000178f8();
          return 0;
        }
        iVar1 = strncasecmp(local_11,"d",1);
        if (iVar1 != 0) break;
        FUN_000260fc();
        if (DAT_0007bb18 != '\0') goto LAB_00027b58;
      }
      iVar1 = strncasecmp(local_11,"p",1);
      if (iVar1 == 0) {
        FUN_00026f50();
      }
      else {
        iVar1 = strncasecmp(local_11,"s",1);
        if (iVar1 == 0) {
          FUN_000267b0();
        }
      }
    } while (DAT_0007bb18 == '\0');
LAB_00027b58:
    FUN_00019adc();
  }
  return 0;
}

