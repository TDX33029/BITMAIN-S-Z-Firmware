//==================== FUN_0002f280 @ 0002f280 ====================

void FUN_0002f280(sem_t *param_1,char *param_2,char *param_3,int param_4)

{
  int iVar1;
  int *piVar2;
  char acStack_1018 [4100];
  
  do {
    iVar1 = sem_wait(param_1);
    if (iVar1 == 0) {
      return;
    }
    piVar2 = __errno_location();
  } while (*piVar2 == 4);
  snprintf(acStack_1018,0x1000,"Failed to sem_wait errno=%d cgsem=0x%p in %s %s():%d",*piVar2,
           param_1,param_2,param_3,param_4);
  FUN_00038730(3,acStack_1018,1);
  FUN_00016ca8(1);
  return;
}

