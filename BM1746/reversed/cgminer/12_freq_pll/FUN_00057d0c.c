//==================== FUN_00057d0c @ 00057d0c ====================

undefined4 FUN_00057d0c(int *param_1,byte *param_2)

{
  int extraout_r1;
  uint uVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  byte *pbVar7;
  
  uVar1 = (uint)*param_2;
  if (uVar1 == 0) {
    iVar4 = 0x1505;
  }
  else {
    iVar4 = 0x1505;
    pbVar7 = param_2;
    do {
      iVar4 = uVar1 + iVar4 * 0x21;
      pbVar7 = pbVar7 + 1;
      uVar1 = (uint)*pbVar7;
    } while (uVar1 != 0);
  }
  FUN_0005c6f0(iVar4,*(undefined4 *)(&DAT_0006cb74 + param_1[2] * 4));
  iVar6 = param_1[1];
  iVar5 = iVar6 + extraout_r1 * 8;
  iVar4 = FUN_0005791c(param_1,iVar5,param_2,iVar4);
  if (iVar4 == 0) {
    return 0xffffffff;
  }
  iVar2 = iVar4 + 4;
  if (iVar2 == *(int *)(iVar6 + extraout_r1 * 8)) {
    if (iVar2 == *(int *)(iVar5 + 4)) {
      piVar3 = param_1 + 3;
      *(int **)(iVar5 + 4) = piVar3;
    }
    else {
      piVar3 = *(int **)(iVar4 + 8);
    }
    *(int **)(iVar6 + extraout_r1 * 8) = piVar3;
  }
  else if (iVar2 == *(int *)(iVar5 + 4)) {
    *(undefined4 *)(iVar5 + 4) = *(undefined4 *)(iVar4 + 4);
  }
  iVar6 = *(int *)(iVar4 + 4);
  iVar5 = *(int *)(iVar4 + 0xc);
  *(undefined4 *)(iVar6 + 4) = *(undefined4 *)(iVar4 + 8);
  **(int **)(iVar4 + 8) = iVar6;
  if (((iVar5 != 0) && (*(int *)(iVar5 + 4) != -1)) &&
     (iVar6 = *(int *)(iVar5 + 4) + -1, *(int *)(iVar5 + 4) = iVar6, iVar6 == 0)) {
    FUN_0005a3e4();
  }
  FUN_00059594(iVar4);
  *param_1 = *param_1 + -1;
  return 0;
}

