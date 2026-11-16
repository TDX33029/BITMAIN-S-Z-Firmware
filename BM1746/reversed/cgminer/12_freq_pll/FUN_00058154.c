//==================== FUN_00058154 @ 00058154 ====================

undefined4 FUN_00058154(undefined4 *param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 uVar2;
  
  *param_1 = param_2;
  param_1[1] = param_3;
  *(undefined1 *)(param_1 + 2) = 0;
  param_1[4] = 0;
  param_1[5] = 0;
  param_1[7] = 0;
  param_1[9] = 0;
  param_1[6] = 1;
  iVar1 = FUN_0005961c(param_1 + 10);
  if (iVar1 == 0) {
    param_1[0xd] = 0xffffffff;
    uVar2 = 0;
  }
  else {
    uVar2 = 0xffffffff;
  }
  return uVar2;
}

