//==================== FUN_0000b450 @ 0000b450 ====================

int FUN_0000b450(undefined4 param_1)

{
  int iVar1;
  uint uVar2;
  byte *pbVar3;
  int iVar4;
  int iVar5;
  byte *pbVar6;
  int iVar7;
  undefined1 auStack_14c [7];
  byte bStack_145;
  byte local_144 [31];
  byte abStack_125 [261];
  
  iVar4 = -0x7ff1f80;
  iVar7 = 0;
  FUN_0000af74();
  do {
    pbVar3 = local_144;
    do {
      iVar1 = FUN_0000b254(iVar4);
      iVar5 = iVar4;
      if (iVar1 == 1) {
        iVar5 = iVar4 + 4;
      }
      pbVar6 = pbVar3 + 1;
      iVar4 = iVar5 + 4;
      iVar5 = FUN_0000b3d4(iVar5,pbVar3);
      if (iVar5 != 0) {
        return iVar5;
      }
      pbVar3 = pbVar6;
    } while (pbVar6 != abStack_125);
    uVar2 = FUN_0000b160(local_144,auStack_14c);
    if (uVar2 < 0x1f) {
      local_144[uVar2] = local_144[uVar2] + 1 & 1;
    }
    pbVar6 = &bStack_145;
    pbVar3 = abStack_125 + iVar7;
    do {
      pbVar6 = pbVar6 + 1;
      pbVar3 = pbVar3 + 1;
      *pbVar3 = *pbVar6;
    } while (local_144 + 0x19 != pbVar6);
    iVar7 = iVar7 + 0x1a;
  } while (iVar7 != 0x104);
  FUN_0000bed4(abStack_125 + 1,param_1,0x100);
  return 0;
}

