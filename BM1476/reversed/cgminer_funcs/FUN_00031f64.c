//==================== FUN_00031f64 @ 00031f64 ====================

void FUN_00031f64(int param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 extraout_r2;
  
  if (*(char *)(param_1 + 0xd) == '\0') {
    if (param_2 == 0) {
      return;
    }
  }
  else {
    FUN_000307c8(param_1,"]");
    *(undefined1 *)(param_1 + 0xd) = 0;
    if (param_2 == 0) {
      return;
    }
  }
  FUN_000307c8(param_1,&DAT_00066afc);
  FUN_000307c8(param_1,"]",extraout_r2,param_4);
  return;
}

