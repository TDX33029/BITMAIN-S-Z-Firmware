//==================== FUN_000413f0 @ 000413f0 ====================

void FUN_000413f0(void)

{
  int param3;
  char *pcVar1;
  char acStack_1020 [4100];
  
  pcVar1 = &DAT_0009d4bb;
  param3 = 0;
  do {
    pcVar1 = pcVar1 + 1;
    if (*pcVar1 == '\x01') {
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
        snprintf(acStack_1020,0x1000,"chain%d %s",param3,
                 "every_chain_jump_from_loader_to_app_PIC16F1704_new");
        FUN_00038730(5,acStack_1020,0);
      }
      pthread_mutex_lock((pthread_mutex_t *)&DAT_000790d0);
      DAT_0007909c = param3;
      FUN_0003ddd0();
      FUN_0002a92c(100);
      pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790d0);
    }
    param3 = param3 + 1;
  } while (param3 != 4);
  FUN_0002a92c(500);
  return;
}

