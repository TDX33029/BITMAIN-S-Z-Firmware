//==================== FUN_0003085c @ 0003085c ====================

void FUN_0003085c(undefined4 param_1,void *param_2,int param_3,char *param_4)

{
  void *__ptr;
  
  if (*param_4 == '\0') {
    if (param_3 == 0) goto LAB_00030884;
    FUN_000307c8(param_1,&DAT_00060764);
    __ptr = (void *)FUN_00030500(param_2,1);
  }
  else {
    if (param_3 == 0) {
      *param_4 = '\0';
LAB_00030884:
      __ptr = (void *)FUN_00030500(param_2,param_3);
      FUN_000307c8(param_1,&DAT_00066aac);
      FUN_000307c8(param_1,__ptr);
      FUN_000307c8(param_1,&DAT_000606fc);
      goto joined_r0x000308bc;
    }
    FUN_000307c8(param_1,&DAT_00066aa8);
    *param_4 = '\0';
    __ptr = (void *)FUN_00030500(param_2,1);
  }
  FUN_000307c8(param_1,&DAT_00060810);
  FUN_000307c8(param_1,__ptr);
  FUN_000307c8(param_1,&DAT_0006712c);
joined_r0x000308bc:
  if (__ptr == param_2) {
    return;
  }
  free(__ptr);
  return;
}

