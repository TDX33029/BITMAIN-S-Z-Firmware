//==================== FUN_0000ace4 @ 0000ace4 ====================

void FUN_0000ace4(__time_t param_1,__time_t param_2)

{
  itimerval local_18;
  
  local_18.it_interval.tv_usec = 0;
  local_18.it_value.tv_usec = 0;
  local_18.it_interval.tv_sec = param_1;
  local_18.it_value.tv_sec = param_2;
  setitimer(ITIMER_REAL,&local_18,(itimerval *)0x0);
  return;
}

