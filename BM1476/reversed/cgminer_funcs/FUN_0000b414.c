//==================== FUN_0000b414 @ 0000b414 ====================

void FUN_0000b414(char *param_1,int param_2)

{
  int *piVar1;
  char acStack_1010 [4100];
  
  piVar1 = __errno_location();
  snprintf(acStack_1010,0x1000,"WTF MUTEX ERROR ON LOCK! errno=%d in %s %s():%d",*piVar1,"cgminer.c"
           ,param_1,param_2);
  FUN_00038730(3,acStack_1010,1);
  FUN_00016ca8(1);
  return;
}

