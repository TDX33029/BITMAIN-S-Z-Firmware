//==================== FUN_0005bc84 @ 0005bc84 ====================

int FUN_0005bc84(undefined4 param_1,undefined4 *param_2,undefined4 param_3)

{
  uint uVar1;
  int iVar2;
  undefined4 local_1c [2];
  
  DAT_0009d780 = *param_2;
  local_1c[0] = 0;
  do {
    uVar1 = FUN_0005bdec(param_1,param_2,local_1c,param_3);
  } while (uVar1 == 1);
  iVar2 = 1 - uVar1;
  if (1 < uVar1) {
    iVar2 = 0;
  }
  return iVar2;
}

