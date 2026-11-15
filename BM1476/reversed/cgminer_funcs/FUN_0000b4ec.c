//==================== FUN_0000b4ec @ 0000b4ec ====================

void FUN_0000b4ec(char *param_1,int param_2)

{
  int *piVar1;
  char acStack_1010 [4100];
  
  piVar1 = __errno_location();
  snprintf(acStack_1010,0x1000,"Failed to pthread_mutex_init errno=%d in %s %s():%d",*piVar1,
           "cgminer.c",param_1,param_2);
  FUN_00038730(3,acStack_1010,1);
  FUN_00016ca8(1);
  return;
}

