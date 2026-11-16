//==================== FUN_0003243c @ 0003243c ====================

void FUN_0003243c(int *param_1)

{
  int *piVar1;
  int iVar2;
  int *__ptr;
  int *piVar3;
  
  iVar2 = pthread_mutex_lock((pthread_mutex_t *)&DAT_00078cb8);
  if (iVar2 != 0) {
    FUN_0000b774("tidyup",0x120c);
  }
  DAT_00078cac = 1;
  if (*param_1 != -1) {
    shutdown(*param_1,2);
    close(*param_1);
    *param_1 = -1;
  }
  if (DAT_00078cb4 != (void *)0x0) {
    free(DAT_00078cb4);
    DAT_00078cb4 = (void *)0x0;
  }
  piVar1 = DAT_00078cd0;
  if (DAT_00078cd0 != (int *)0x0) {
    __ptr = DAT_00078cd0;
    do {
      piVar3 = (int *)__ptr[2];
      free(*(void **)(*__ptr + 4));
      free((void *)*__ptr);
      free(__ptr);
      __ptr = piVar3;
    } while (piVar1 != piVar3);
    DAT_00078cd0 = (int *)0x0;
  }
  iVar2 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_00078cb8);
  if (iVar2 != 0) {
    FUN_0000b84c("tidyup",0x1221);
  }
  (*DAT_00075ed8)();
  return;
}

