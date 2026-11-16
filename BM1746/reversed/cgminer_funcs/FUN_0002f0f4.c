//==================== FUN_0002f0f4 @ 0002f0f4 ====================

void FUN_0002f0f4(sem_t *param_1,char *param_2,char *param_3,int param_4)

{
  int param3;
  int *piVar1;
  char acStack_1018 [4100];
  
  param3 = sem_init(param_1,0,0);
  if (param3 != 0) {
    piVar1 = __errno_location();
    snprintf(acStack_1018,0x1000,"Failed to sem_init ret=%d errno=%d in %s %s():%d",param3,*piVar1,
             param_2,param_3,param_4);
    FUN_00038730(3,acStack_1018,1);
    FUN_00016ca8(1);
  }
  return;
}

