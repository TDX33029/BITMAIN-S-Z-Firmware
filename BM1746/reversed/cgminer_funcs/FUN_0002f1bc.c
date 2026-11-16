//==================== FUN_0002f1bc @ 0002f1bc ====================

void FUN_0002f1bc(sem_t *param_1,char *param_2,char *param_3,int param_4)

{
  int iVar1;
  int *piVar2;
  char acStack_1018 [4100];
  
  iVar1 = sem_post(param_1);
  if (iVar1 != 0) {
    piVar2 = __errno_location();
    snprintf(acStack_1018,0x1000,"Failed to sem_post errno=%d cgsem=0x%p in %s %s():%d",*piVar2,
             param_1,param_2,param_3,param_4);
    FUN_00038730(3,acStack_1018,1);
    FUN_00016ca8(1);
  }
  return;
}

