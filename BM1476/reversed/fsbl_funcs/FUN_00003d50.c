//==================== FUN_00003d50 @ 00003d50 ====================

undefined4 FUN_00003d50(undefined4 *param_1)

{
  byte *pbVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  byte *pbVar5;
  
  uVar2 = param_1[2];
  param_1[4] = 0;
  pbVar5 = (byte *)*param_1;
  uVar3 = uVar2;
  if ((uVar2 == 0) && ((*pbVar5 < 3 || (uVar3 = *(uint *)(pbVar5 + 0x24), uVar3 == 0)))) {
    if (*(short *)(pbVar5 + 8) == 0) {
      return 2;
    }
    iVar4 = *(int *)(pbVar5 + 0x24);
    param_1[6] = iVar4;
    uVar3 = uVar2;
  }
  else {
    if (*(ushort *)(pbVar5 + 10) == 0) {
      pbVar1 = pbVar5;
      while( true ) {
        uVar3 = FUN_000036fc(pbVar1,uVar3);
        if (uVar3 == 0xffffffff) {
          return 1;
        }
        if (uVar3 < 2) break;
        if (*(uint *)(pbVar5 + 0x14) <= uVar3) {
          return 2;
        }
        pbVar1 = (byte *)*param_1;
      }
      return 2;
    }
    if (*(int *)(pbVar5 + 0x14) - 2U <= uVar3 - 2) {
      param_1[5] = uVar3;
      param_1[6] = 0;
      return 2;
    }
    iVar4 = (uVar3 - 2) * (uint)*(ushort *)(pbVar5 + 10) + *(int *)(pbVar5 + 0x28);
    param_1[6] = iVar4;
  }
  param_1[5] = uVar3;
  if (iVar4 == 0) {
    return 2;
  }
  param_1[7] = pbVar5 + 0x40;
  return 0;
}

