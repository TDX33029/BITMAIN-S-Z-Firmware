//==================== FUN_0002f4f0 @ 0002f4f0 ====================

void FUN_0002f4f0(void *param_1,void *param_2,size_t param_3,char *param_4,char *param_5,int param_6
                 )

{
  char acStack_1010 [4096];
  
  if (-1 < (int)(param_3 - 1)) {
    memcpy(param_1,param_2,param_3);
    return;
  }
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
    snprintf(acStack_1010,0x1000,"ERR: Asked to memcpy %u bytes from %s %s():%d",param_3,param_4,
             param_5,param_6);
    FUN_00038730(3,acStack_1010,0);
  }
  return;
}

