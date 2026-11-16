//==================== FUN_0002aee0 @ 0002aee0 ====================

void FUN_0002aee0(int *param_1)

{
  int iVar1;
  int *piVar2;
  char acStack_1010 [4100];
  
  if ((DAT_00078e09 != '\0') &&
     (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
    snprintf(acStack_1010,0x1000,"Closing socket for stratum pool %d",*param_1);
    FUN_00038730(7,acStack_1010,0);
  }
  iVar1 = pthread_mutex_lock((pthread_mutex_t *)(param_1 + 0xb6));
  if (iVar1 != 0) {
    FUN_0000b708("suspend_stratum",0xd1a);
  }
  FUN_00028ae8(param_1);
  *(undefined1 *)((int)param_1 + 0x29b) = 0;
  *(undefined1 *)((int)param_1 + 0x299) = 0;
  if (param_1[0x99] != 0) {
    close(param_1[0x99]);
  }
  param_1[0x99] = 0;
  iVar1 = pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0xb6));
  if (iVar1 != 0) {
    piVar2 = __errno_location();
    snprintf(acStack_1010,0x1000,"WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d",*piVar2,
             "util.c","suspend_stratum",0xd1c);
    FUN_00038730(3,acStack_1010,1);
    FUN_00016ca8(1);
  }
  (*DAT_00075ed8)();
  return;
}

