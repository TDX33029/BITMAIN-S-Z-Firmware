//==================== FUN_00057ea4 @ 00057ea4 ====================

int FUN_00057ea4(int param_1,byte *param_2)

{
  int extraout_r1;
  uint uVar1;
  int iVar2;
  byte *pbVar3;
  
  uVar1 = (uint)*param_2;
  if (uVar1 == 0) {
    iVar2 = 0x1505;
  }
  else {
    iVar2 = 0x1505;
    pbVar3 = param_2;
    do {
      iVar2 = uVar1 + iVar2 * 0x21;
      pbVar3 = pbVar3 + 1;
      uVar1 = (uint)*pbVar3;
    } while (uVar1 != 0);
  }
  FUN_0005c6f0(iVar2,*(undefined4 *)(&DAT_0006cb74 + *(int *)(param_1 + 8) * 4));
  iVar2 = FUN_0005791c(param_1,*(int *)(param_1 + 4) + extraout_r1 * 8,param_2,iVar2);
  if (iVar2 != 0) {
    iVar2 = iVar2 + 4;
  }
  return iVar2;
}

