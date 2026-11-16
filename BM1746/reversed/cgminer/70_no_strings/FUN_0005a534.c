//==================== FUN_0005a534 @ 0005a534 ====================

undefined4 FUN_0005a534(int *param_1,uint param_2)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  
  if (((param_1 == (int *)0x0) || (*param_1 != 1)) || (uVar3 = param_1[3], uVar3 <= param_2)) {
    return 0xffffffff;
  }
  iVar1 = *(int *)(param_1[4] + param_2 * 4);
  if (((iVar1 != 0) && (*(int *)(iVar1 + 4) != -1)) &&
     (iVar2 = *(int *)(iVar1 + 4) + -1, *(int *)(iVar1 + 4) = iVar2, iVar2 == 0)) {
    FUN_0005a3e4();
    uVar3 = param_1[3];
  }
  uVar3 = uVar3 - 1;
  if (param_2 < uVar3) {
    memmove((void *)(param_1[4] + param_2 * 4),(void *)(param_1[4] + (param_2 + 1) * 4),
            (uVar3 - param_2) * 4);
    uVar3 = param_1[3] - 1;
  }
  param_1[3] = uVar3;
  return 0;
}

