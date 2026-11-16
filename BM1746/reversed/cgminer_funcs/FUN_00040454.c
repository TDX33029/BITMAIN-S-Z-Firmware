//==================== FUN_00040454 @ 00040454 ====================

void FUN_00040454(void)

{
  int iVar1;
  int param3;
  char *pcVar2;
  byte abStack_1024 [4100];
  
  pcVar2 = &DAT_0009d4bb;
  param3 = 0;
  abStack_1024[0] = 0;
  abStack_1024[1] = 0;
  abStack_1024[2] = 0;
  abStack_1024[3] = 0;
  do {
    pcVar2 = pcVar2 + 1;
    if (*pcVar2 == '\x01') {
      pthread_mutex_lock((pthread_mutex_t *)&DAT_000790d0);
      DAT_0007909c = param3;
      iVar1 = FUN_00040128(abStack_1024 + param3);
      FUN_0002a92c(100);
      pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790d0);
      if (iVar1 == 1) {
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
          snprintf((char *)(abStack_1024 + 4),0x1000,"chain%d pic version %02x\n",param3,
                   (uint)abStack_1024[param3]);
          FUN_00038730(5,abStack_1024 + 4,0);
        }
      }
      else if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
        snprintf((char *)(abStack_1024 + 4),0x1000,"chain%d pic version read failed\n",param3);
        FUN_00038730(5,abStack_1024 + 4,0);
      }
    }
    param3 = param3 + 1;
  } while (param3 != 4);
  return;
}

