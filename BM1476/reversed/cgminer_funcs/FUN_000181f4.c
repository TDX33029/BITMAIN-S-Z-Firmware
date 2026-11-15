//==================== FUN_000181f4 @ 000181f4 ====================

int FUN_000181f4(int param_1,uint param_2,undefined4 param_3)

{
  int iVar1;
  int iVar2;
  int local_20;
  int local_1c;
  timespec local_18;
  
  FUN_0002a84c(&local_20,param_2,param_3,param_2 * 0x10624dd3);
  local_18.tv_sec = param_2 / 1000 + local_20;
  local_1c = (param_2 / 1000) * -1000000 + param_2 * 1000 + local_1c;
  if (999999 < local_1c) {
    local_18.tv_sec = local_18.tv_sec + 1;
    local_1c = local_1c + -1000000;
  }
  local_18.tv_nsec = local_1c * 1000;
  iVar1 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007b62c);
  if (iVar1 != 0) {
    FUN_0000b414("restart_wait",0x1481);
  }
  if (*(char *)(param_1 + 0x3e) == '\0') {
    iVar1 = pthread_cond_timedwait
                      ((pthread_cond_t *)&DAT_0007bae0,(pthread_mutex_t *)&DAT_0007b62c,&local_18);
  }
  else {
    iVar1 = 0;
  }
  iVar2 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b62c);
  if (iVar2 != 0) {
    FUN_0000b5c4("restart_wait",0x1486);
  }
  (*DAT_00075ed8)();
  return iVar1;
}

