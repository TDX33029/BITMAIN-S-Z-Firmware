//==================== FUN_0000b8b8 @ 0000b8b8 ====================

void FUN_0000b8b8(int param_1)

{
  int *piVar1;
  char acStack_1008 [4096];
  
  piVar1 = __errno_location();
  snprintf(acStack_1008,0x1000,"Failed to pthread_mutex_init errno=%d in %s %s():%d",*piVar1,
           "driver-btm-zcash.c","bitmain_ZCASH_prepare",param_1);
  FUN_00038730(3,acStack_1008,1);
  FUN_00016ca8(1);
  return;
}

