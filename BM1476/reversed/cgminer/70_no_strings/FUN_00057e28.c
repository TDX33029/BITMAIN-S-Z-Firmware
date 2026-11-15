//==================== FUN_00057e28 @ 00057e28 ====================

void FUN_00057e28(undefined4 *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  FUN_00057994();
  iVar3 = *(int *)(&DAT_0006cb74 + param_1[2] * 4);
  if (iVar3 != 0) {
    iVar1 = param_1[1];
    iVar2 = 0;
    do {
      *(undefined4 **)(iVar1 + iVar2 + 4) = param_1 + 3;
      *(undefined4 **)(iVar1 + iVar2) = param_1 + 3;
      iVar2 = iVar2 + 8;
    } while (iVar2 != iVar3 * 8);
  }
  param_1[4] = param_1 + 3;
  param_1[3] = param_1 + 3;
  *param_1 = 0;
  return;
}

