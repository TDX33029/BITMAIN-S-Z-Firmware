//==================== FUN_00055388 @ 00055388 ====================

undefined4
FUN_00055388(int param_1,int param_2,int param_3,int param_4,int param_5,int param_6,byte param_7)

{
  int iVar1;
  undefined1 auStack_188 [364];
  
  if ((((param_2 != 0 || param_5 == 0 && param_6 == 0) && (param_1 != 0)) &&
      (param_3 != 0 || param_7 == 0)) && ((param_4 - 1U < 0x40 && (param_7 < 0x41)))) {
    if (param_7 != 0) {
      iVar1 = FUN_00055098(auStack_188,param_4,param_3,param_7);
    }
    else {
      iVar1 = FUN_00054ed0(auStack_188,param_4);
    }
    if (-1 < iVar1) {
      FUN_00054f8c(auStack_188,param_2,param_5,param_6);
      FUN_000551d4(auStack_188,param_1,param_4);
      return 0;
    }
  }
  return 0xffffffff;
}

