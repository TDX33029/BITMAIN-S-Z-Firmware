//==================== FUN_00015264 @ 00015264 ====================

void FUN_00015264(char *param_1,size_t param_2,time_t *param_3)

{
  tm *ptVar1;
  time_t local_14 [2];
  
  local_14[0] = *param_3;
  ptVar1 = localtime(local_14);
  snprintf(param_1,param_2,"[%d-%02d-%02d %02d:%02d:%02d]",ptVar1->tm_year + 0x76c,
           ptVar1->tm_mon + 1,ptVar1->tm_mday,ptVar1->tm_hour,ptVar1->tm_min,ptVar1->tm_sec);
  return;
}

