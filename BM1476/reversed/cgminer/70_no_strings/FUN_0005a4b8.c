//==================== FUN_0005a4b8 @ 0005a4b8 ====================

undefined4 FUN_0005a4b8(int *param_1)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  
  if ((param_1 == (int *)0x0) || (*param_1 != 1)) {
    return 0xffffffff;
  }
  uVar2 = param_1[3];
  if (uVar2 != 0) {
    uVar4 = 0;
    do {
      iVar1 = *(int *)(param_1[4] + uVar4 * 4);
      uVar4 = uVar4 + 1;
      if (((iVar1 != 0) && (iVar3 = *(int *)(iVar1 + 4) + -1, *(int *)(iVar1 + 4) != -1)) &&
         (*(int *)(iVar1 + 4) = iVar3, iVar3 == 0)) {
        FUN_0005a3e4();
        uVar2 = param_1[3];
      }
    } while (uVar4 < uVar2);
  }
  param_1[3] = 0;
  return 0;
}

