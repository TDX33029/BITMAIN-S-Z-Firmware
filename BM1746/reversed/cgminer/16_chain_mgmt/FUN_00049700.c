//==================== FUN_00049700 @ 00049700 ====================

void FUN_00049700(undefined4 param_1,undefined4 param_2)

{
  char acStack_1010 [4100];
  
  pthread_mutex_lock((pthread_mutex_t *)&DAT_00079134);
  FUN_00046434(param_1,param_2);
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_00079134);
  FUN_000481b4();
  FUN_0002a92c(100);
  FUN_0004952c(param_2,1);
  FUN_0002a92c(200);
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    snprintf(acStack_1010,0x1000,"%s DONE!","recheck_asic_num");
    FUN_00038730(5,acStack_1010,0);
  }
  return;
}

