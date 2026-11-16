//==================== FUN_000388e4 @ 000388e4 ====================

void FUN_000388e4(void)

{
  int iVar1;
  int *piVar2;
  char acStack_1008 [4100];
  
  iVar1 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007b7a8);
  if (iVar1 != 0) {
    piVar2 = __errno_location();
    snprintf(acStack_1008,0x1000,"WTF MUTEX ERROR ON LOCK! errno=%d in %s %s():%d",*piVar2,
             "logging.c","my_log_curses",0x2f);
    FUN_00038730(3,acStack_1008,1);
    FUN_00016ca8(1);
  }
  return;
}

