//==================== FUN_000579f4 @ 000579f4 ====================

undefined4 FUN_000579f4(undefined4 *param_1)

{
  int iVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  
  *param_1 = 0;
  param_1[2] = 0;
  iVar1 = FUN_0005956c(0x28);
  param_1[1] = iVar1;
  if (iVar1 == 0) {
    return 0xffffffff;
  }
  puVar2 = param_1 + 3;
  param_1[4] = puVar2;
  param_1[3] = puVar2;
  iVar4 = *(int *)(&DAT_0006cb74 + param_1[2] * 4);
  if (iVar4 != 0) {
    iVar3 = 0;
    do {
      *(undefined4 **)(iVar1 + iVar3 + 4) = puVar2;
      *(undefined4 **)(iVar1 + iVar3) = puVar2;
      iVar3 = iVar3 + 8;
    } while (iVar3 != iVar4 << 3);
  }
  return 0;
}

