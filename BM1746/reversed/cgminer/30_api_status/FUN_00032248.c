//==================== FUN_00032248 @ 00032248 ====================

undefined4 FUN_00032248(void)

{
  int iVar1;
  char local_1008 [4];
  char acStack_1004 [4];
  char acStack_1000 [4];
  char acStack_ffc [4];
  char local_ff8 [4];
  char acStack_ff4 [4080];
  
  iVar1 = pthread_mutex_lock((pthread_mutex_t *)&DAT_00078cb8);
  if (iVar1 != 0) {
    FUN_0000b774("restart_thread",0x1357);
  }
  iVar1 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_00078cb8);
  if (iVar1 != 0) {
    FUN_0000b84c("restart_thread",0x1358);
  }
  (*DAT_00075ed8)();
  if ((DAT_00078e09 != '\0') &&
     (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
    local_1008[0] = 'A';
    local_1008[1] = 'P';
    local_1008[2] = 'I';
    local_1008[3] = ':';
    acStack_1004[0] = ' ';
    acStack_1004[1] = 'r';
    acStack_1004[2] = 'e';
    acStack_1004[3] = 's';
    acStack_1000[0] = 't';
    acStack_1000[1] = 'a';
    acStack_1000[2] = 'r';
    acStack_1000[3] = 't';
    acStack_ffc[0] = 'i';
    acStack_ffc[1] = 'n';
    acStack_ffc[2] = 'g';
    acStack_ffc[3] = ' ';
    local_ff8[0] = 'c';
    local_ff8[1] = 'g';
    local_ff8[2] = 'm';
    local_ff8[3] = 'i';
    acStack_ff4[0] = 'n';
    acStack_ff4[1] = 'e';
    acStack_ff4[2] = 'r';
    acStack_ff4[3] = '\0';
    FUN_00038730(7,local_1008,0);
  }
  FUN_0001d7c8();
  return 0;
}

