//==================== FUN_00046434 @ 00046434 ====================

void FUN_00046434(int param_1,uint param_2)

{
  char acStack_1028 [4100];
  
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
    snprintf(acStack_1028,0x1000,"%s chainid %d","reset_chain",param_2);
    FUN_00038730(4,acStack_1028,0);
  }
  pthread_mutex_lock((pthread_mutex_t *)&DAT_000790d0);
  DAT_0007909c = param_2;
  FUN_0003fc24();
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790d0);
  (&DAT_000790ec)[param_2] = 0;
  (&DAT_00079118)[param_2] = 0;
  (&DAT_000790fc)[param_2] = 1;
  FUN_0002a3dc(param_1 + param_2 * 0x40 + 0x27a);
  FUN_0002a3dc(param_1 + param_2 * 0x40 + 0x37a);
  DAT_00076b4b = 0;
  pthread_mutex_lock((pthread_mutex_t *)&DAT_000790d0);
  DAT_0007909c = param_2;
  FUN_0003df78();
  FUN_0002a92c(1000);
  FUN_0003ddd0();
  FUN_0002a92c(1000);
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790d0);
  DAT_00076b4b = 1;
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    snprintf(acStack_1028,0x1000,"%s %d","reset_hash_board_low",param_2);
    FUN_00038730(5,acStack_1028,0);
  }
  *(int *)(DAT_00078e10 + 0x34) = 1 << (param_2 & 0xff);
  FUN_0002a92c(100);
  pthread_mutex_lock((pthread_mutex_t *)&DAT_000790d0);
  DAT_0007909c = param_2;
  FUN_0003fa7c();
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790d0);
  sleep(1);
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    snprintf(acStack_1028,0x1000,"%s %d","reset_hash_board_high",param_2);
    FUN_00038730(5,acStack_1028,0);
  }
  *(undefined4 *)(DAT_00078e10 + 0x34) = 0;
  (&DAT_000790fc)[param_2] = 0;
  FUN_000445e8(param_2,param_1);
  FUN_0003bb44(0x1a);
  FUN_0002a92c(100);
  FUN_00043e98(param_2,0);
  FUN_0002a92c(1000);
  FUN_0003bb44(0);
  FUN_0002a92c(100);
  return;
}

