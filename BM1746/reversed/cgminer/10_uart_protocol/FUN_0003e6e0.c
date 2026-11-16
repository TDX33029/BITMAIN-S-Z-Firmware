//==================== FUN_0003e6e0 @ 0003e6e0 ====================

void FUN_0003e6e0(uint param_1,byte *param_2,int param_3)

{
  undefined1 *puVar1;
  int param3;
  int param4;
  char *pcVar2;
  char acStack_1028 [4100];
  
  pcVar2 = &DAT_0009d4bb;
  param3 = 0;
  do {
    pcVar2 = pcVar2 + 1;
    if (*pcVar2 == '\x01') {
      param4 = 0;
      pthread_mutex_lock((pthread_mutex_t *)&DAT_000790d0);
      DAT_0007909c = param3;
      do {
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
          snprintf(acStack_1028,0x1000,
                   "chain%d asic%d, set data slave:%02x addr:%02x %02x %02x, len=%d\n",param3,param4
                   ,(uint)(byte)(&DAT_00076b48)[param4],param_1,(uint)*param_2,(uint)param_2[1],
                   param_3);
          FUN_00038730(5,acStack_1028,0);
        }
        puVar1 = &DAT_00076b48 + param4;
        param4 = param4 + 1;
        FUN_0003c7e0(*puVar1,param_1,param_2,param_3);
        FUN_0002a92c(200);
      } while (param4 != 3);
      pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790d0);
    }
    param3 = param3 + 1;
  } while (param3 != 4);
  return;
}

