//==================== FUN_0000e1f8 @ 0000e1f8 ====================

char * FUN_0000e1f8(char *param_1)

{
  size_t sVar1;
  uint __n;
  char acStack_18 [8];
  timeval tStack_10;
  
  gettimeofday(&tStack_10,(__timezone_ptr_t)0x0);
  sprintf(acStack_18,"%ld",tStack_10.tv_usec);
  *param_1 = '\0';
  param_1[1] = '\0';
  param_1[2] = '\0';
  param_1[3] = '\0';
  param_1[4] = '\0';
  param_1[5] = '\0';
  param_1[6] = '\0';
  sVar1 = strlen(acStack_18);
  __n = 6 - sVar1 & 0xff;
  if (__n != 0) {
    memset(param_1,0x30,__n);
  }
  strcat(param_1,acStack_18);
  return param_1;
}

