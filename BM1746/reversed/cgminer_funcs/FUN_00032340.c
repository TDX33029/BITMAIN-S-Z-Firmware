//==================== FUN_00032340 @ 00032340 ====================

undefined4 FUN_00032340(void)

{
  int iVar1;
  char local_1008 [4];
  char acStack_1004 [4];
  char acStack_1000 [4];
  char acStack_ffc [4];
  char acStack_ff8 [4];
  undefined1 local_ff4;
  
  iVar1 = pthread_mutex_lock((pthread_mutex_t *)&DAT_00078cb8);
  if (iVar1 != 0) {
    FUN_0000b774("quit_thread",0x1349);
  }
  iVar1 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_00078cb8);
  if (iVar1 != 0) {
    FUN_0000b84c("quit_thread",0x134a);
  }
  (*DAT_00075ed8)();
  if ((DAT_00078e09 != '\0') &&
     (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
    local_1008[0] = 'A';
    local_1008[1] = 'P';
    local_1008[2] = 'I';
    local_1008[3] = ':';
    acStack_1004[0] = ' ';
    acStack_1004[1] = 'k';
    acStack_1004[2] = 'i';
    acStack_1004[3] = 'l';
    acStack_1000[0] = 'l';
    acStack_1000[1] = 'i';
    acStack_1000[2] = 'n';
    acStack_1000[3] = 'g';
    acStack_ffc[0] = ' ';
    acStack_ffc[1] = 'c';
    acStack_ffc[2] = 'g';
    acStack_ffc[3] = 'm';
    acStack_ff8[0] = 'i';
    acStack_ff8[1] = 'n';
    acStack_ff8[2] = 'e';
    acStack_ff8[3] = 'r';
    local_ff4 = 0;
    FUN_00038730(7,local_1008,0);
  }
  FUN_000178f8();
  return 0;
}

