//==================== FUN_0000e75c @ 0000e75c ====================

void FUN_0000e75c(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 local_10;
  undefined4 uStack_c;
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  local_10 = param_1;
  uStack_c = param_2;
  uStack_8 = param_3;
  uStack_4 = param_4;
  printf("first_2byte=%s  ser_cmd=%s ser_len=%s body=%s\n",(char *)&local_10,
         (char *)((int)&local_10 + 3),(char *)((int)&uStack_c + 2),(char *)&uStack_8);
  return;
}

