//==================== FUN_00034dc0 @ 00034dc0 ====================

void FUN_00034dc0(undefined4 param_1,undefined4 param_2,char *param_3,undefined4 param_4)

{
  FILE *__stream;
  char *pcVar1;
  char acStack_1018 [4096];
  
  if ((param_3 == (char *)0x0) || (*param_3 == '\0')) {
    FUN_00016150();
    param_3 = acStack_1018;
  }
  __stream = fopen(param_3,"w");
  if (__stream == (FILE *)0x0) {
    pcVar1 = (char *)FUN_00030500(param_3,param_4);
    FUN_0003305c(param_1,0x2b,0,pcVar1,param_4);
    if (pcVar1 != param_3) {
      free(pcVar1);
    }
  }
  else {
    FUN_00015b64();
    fclose(__stream);
    pcVar1 = (char *)FUN_00030500(param_3,param_4);
    FUN_0003305c(param_1,0x2c,0,pcVar1,param_4);
    if (pcVar1 != param_3) {
      free(pcVar1);
    }
  }
  return;
}

