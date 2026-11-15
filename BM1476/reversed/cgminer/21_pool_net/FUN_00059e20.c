//==================== FUN_00059e20 @ 00059e20 ====================

undefined4 FUN_00059e20(int *param_1,byte *param_2)

{
  undefined4 uVar1;
  int extraout_r1;
  uint uVar2;
  int iVar3;
  byte *pbVar4;
  
  if ((param_1 == (int *)0x0) || (*param_1 != 0)) {
    return 0;
  }
  uVar2 = (uint)*param_2;
  if (uVar2 == 0) {
    iVar3 = 0x1505;
  }
  else {
    iVar3 = 0x1505;
    pbVar4 = param_2;
    do {
      iVar3 = uVar2 + iVar3 * 0x21;
      pbVar4 = pbVar4 + 1;
      uVar2 = (uint)*pbVar4;
    } while (uVar2 != 0);
  }
  FUN_0005c6f0(iVar3,*(undefined4 *)(&DAT_0006cb74 + param_1[4] * 4));
  iVar3 = FUN_0005791c(param_1 + 2,param_1[3] + extraout_r1 * 8,param_2,iVar3);
  uVar1 = 0;
  if (iVar3 != 0) {
    uVar1 = *(undefined4 *)(iVar3 + 0xc);
  }
  return uVar1;
}

