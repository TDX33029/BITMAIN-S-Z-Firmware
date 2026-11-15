//==================== FUN_00041614 @ 00041614 ====================

void FUN_00041614(void)

{
  int iVar1;
  char *pcVar2;
  char local_1010 [4];
  char acStack_100c [4];
  char acStack_1008 [4];
  char acStack_1004 [4];
  char local_1000 [4];
  char acStack_ffc [4];
  char acStack_ff8 [4];
  char acStack_ff4 [4];
  char local_ff0 [4];
  char acStack_fec [4060];
  
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    local_1010[0] = 'e';
    local_1010[1] = 'v';
    local_1010[2] = 'e';
    local_1010[3] = 'r';
    acStack_100c[0] = 'y';
    acStack_100c[1] = '_';
    acStack_100c[2] = 'c';
    acStack_100c[3] = 'h';
    acStack_1008[0] = 'a';
    acStack_1008[1] = 'i';
    acStack_1008[2] = 'n';
    acStack_1008[3] = '_';
    acStack_1004[0] = 'e';
    acStack_1004[1] = 'n';
    acStack_1004[2] = 'a';
    acStack_1004[3] = 'b';
    local_1000[0] = 'l';
    local_1000[1] = 'e';
    local_1000[2] = '_';
    local_1000[3] = 'P';
    acStack_ffc[0] = 'I';
    acStack_ffc[1] = 'C';
    acStack_ffc[2] = '1';
    acStack_ffc[3] = '6';
    acStack_ff8[0] = 'F';
    acStack_ff8[1] = '1';
    acStack_ff8[2] = '7';
    acStack_ff8[3] = '0';
    acStack_ff4[0] = '4';
    acStack_ff4[1] = '_';
    acStack_ff4[2] = 'd';
    acStack_ff4[3] = 'c';
    local_ff0[0] = '_';
    local_ff0[1] = 'd';
    local_ff0[2] = 'c';
    local_ff0[3] = '_';
    acStack_fec[0] = 'n';
    acStack_fec[1] = 'e';
    acStack_fec[2] = 'w';
    acStack_fec[3] = '\0';
    FUN_00038730(5,local_1010,0);
  }
  pcVar2 = &DAT_0009d4bb;
  iVar1 = 0;
  do {
    pcVar2 = pcVar2 + 1;
    if (*pcVar2 == '\x01') {
      pthread_mutex_lock((pthread_mutex_t *)&DAT_000790d0);
      DAT_0007909c = iVar1;
      FUN_0003fa7c();
      FUN_0002a92c(200);
      pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790d0);
    }
    iVar1 = iVar1 + 1;
  } while (iVar1 != 4);
  return;
}

