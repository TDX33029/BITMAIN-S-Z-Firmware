//==================== FUN_0003896c @ 0003896c ====================

void FUN_0003896c(int param_1)

{
  int iVar1;
  int *piVar2;
  char acStack_1008 [4096];
  
  iVar1 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b7a8);
  if (iVar1 != 0) {
    piVar2 = __errno_location();
    snprintf(acStack_1008,0x1000,"WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d",*piVar2,
             "logging.c","my_log_curses",param_1);
    FUN_00038730(3,acStack_1008,1);
    FUN_00016ca8(1);
  }
  return;
}

