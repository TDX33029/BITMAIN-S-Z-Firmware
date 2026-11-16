//==================== FUN_0002a300 @ 0002a300 ====================

int FUN_0002a300(int *param_1,timespec *param_2)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  pthread_mutex_t *__mutex;
  int iVar4;
  
  __mutex = (pthread_mutex_t *)(param_1 + 3);
  iVar1 = pthread_mutex_lock(__mutex);
  if (iVar1 != 0) {
    FUN_0000b708("tq_pop",0x506);
  }
  piVar3 = (int *)*param_1;
  if (param_1 == piVar3) {
    if (param_2 == (timespec *)0x0) {
      iVar1 = pthread_cond_wait((pthread_cond_t *)(param_1 + 10),__mutex);
    }
    else {
      iVar1 = pthread_cond_timedwait((pthread_cond_t *)(param_1 + 10),__mutex,param_2);
    }
    if (iVar1 != 0) {
      iVar1 = 0;
      goto LAB_0002a354;
    }
    piVar3 = (int *)*param_1;
    if (param_1 == piVar3) {
      iVar1 = 0;
      goto LAB_0002a354;
    }
  }
  piVar2 = (int *)piVar3[1];
  iVar4 = *piVar3;
  iVar1 = piVar3[-1];
  *(int **)(iVar4 + 4) = piVar2;
  *piVar2 = iVar4;
  *piVar3 = 0;
  piVar3[1] = 0;
  free(piVar3 + -1);
LAB_0002a354:
  FUN_00029388(__mutex,"tq_pop",0x519);
  (*DAT_00075ed8)();
  return iVar1;
}

