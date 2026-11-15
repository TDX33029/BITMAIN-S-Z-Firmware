//==================== FUN_00057a8c @ 00057a8c ====================

undefined4 FUN_00057a8c(uint *param_1,byte *param_2,int param_3,int param_4)

{
  uint *puVar1;
  int iVar2;
  size_t sVar3;
  int *piVar4;
  uint uVar5;
  int extraout_r1;
  int extraout_r1_00;
  int *piVar6;
  int iVar7;
  uint *puVar8;
  uint uVar9;
  int iVar10;
  uint *puVar11;
  uint uVar12;
  byte *pbVar13;
  
  uVar5 = *(uint *)(&DAT_0006cb74 + param_1[2] * 4);
  if (*param_1 < uVar5) {
    uVar9 = param_1[1];
  }
  else {
    FUN_00059594(param_1[1]);
    uVar5 = param_1[2];
    param_1[2] = uVar5 + 1;
    iVar7 = *(int *)(&DAT_0006cb74 + (uVar5 + 1) * 4);
    uVar9 = FUN_0005956c(iVar7 << 3);
    param_1[1] = uVar9;
    if (uVar9 == 0) {
      return 0xffffffff;
    }
    uVar5 = *(uint *)(&DAT_0006cb74 + param_1[2] * 4);
    if (uVar5 != 0) {
      iVar2 = 0;
      do {
        *(uint **)(uVar9 + iVar2 + 4) = param_1 + 3;
        *(uint **)(uVar9 + iVar2) = param_1 + 3;
        iVar2 = iVar2 + 8;
      } while (iVar2 != uVar5 << 3);
    }
    puVar11 = (uint *)param_1[4];
    puVar1 = param_1 + 3;
    *puVar1 = (uint)puVar1;
    param_1[4] = (uint)puVar1;
    if (puVar11 != puVar1) {
      while( true ) {
        FUN_0005c6f0(puVar11[-1],iVar7);
        puVar8 = (uint *)puVar11[1];
        FUN_000578c0(param_1,uVar9 + extraout_r1_00 * 8,puVar11);
        if (puVar1 == puVar8) break;
        uVar9 = param_1[1];
        puVar11 = puVar8;
      }
      uVar9 = param_1[1];
      uVar5 = *(uint *)(&DAT_0006cb74 + param_1[2] * 4);
    }
  }
  uVar12 = (uint)*param_2;
  if (uVar12 == 0) {
    iVar7 = 0x1505;
  }
  else {
    iVar7 = 0x1505;
    pbVar13 = param_2;
    do {
      iVar7 = uVar12 + iVar7 * 0x21;
      pbVar13 = pbVar13 + 1;
      uVar12 = (uint)*pbVar13;
    } while (uVar12 != 0);
  }
  FUN_0005c6f0(iVar7,uVar5);
  iVar10 = uVar9 + extraout_r1 * 8;
  iVar2 = FUN_0005791c(param_1,iVar10,param_2,iVar7);
  if (iVar2 == 0) {
    sVar3 = strlen((char *)param_2);
    piVar4 = (int *)FUN_0005956c(sVar3 + 0x15);
    if (piVar4 == (int *)0x0) {
      return 0xffffffff;
    }
    *piVar4 = iVar7;
    piVar4[4] = param_3;
    strcpy((char *)(piVar4 + 5),(char *)param_2);
    piVar6 = piVar4 + 1;
    piVar4[2] = (int)piVar6;
    piVar4[3] = param_4;
    piVar4[1] = (int)piVar6;
    FUN_000578c0(param_1,iVar10,piVar6);
    *param_1 = *param_1 + 1;
    return 0;
  }
  iVar7 = *(int *)(iVar2 + 0xc);
  if (((iVar7 != 0) && (*(int *)(iVar7 + 4) != -1)) &&
     (iVar10 = *(int *)(iVar7 + 4) + -1, *(int *)(iVar7 + 4) = iVar10, iVar10 == 0)) {
    FUN_0005a3e4();
  }
  *(int *)(iVar2 + 0xc) = param_4;
  return 0;
}

