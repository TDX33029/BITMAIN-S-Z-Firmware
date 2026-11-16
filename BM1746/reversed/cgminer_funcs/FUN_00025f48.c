//==================== FUN_00025f48 @ 00025f48 ====================

undefined4 FUN_00025f48(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  
  iVar1 = FUN_0000f7e8(param_1,param_3);
  if ((iVar1 == 0) || (iVar1 = FUN_0001654c(param_2,param_3), iVar1 == 0)) {
    FUN_000189a4(param_1);
    return 0;
  }
  FUN_00025e5c(param_1,param_2);
  if ((DAT_00076ffc == 0) || (DAT_0007871c == '\0')) {
    return 1;
  }
  FUN_00011814(param_2,param_3);
  return 1;
}

