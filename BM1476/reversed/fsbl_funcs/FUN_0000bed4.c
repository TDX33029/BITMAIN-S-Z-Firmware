//==================== FUN_0000bed4 @ 0000bed4 ====================

void FUN_0000bed4(int param_1,int param_2,uint param_3)

{
  byte *pbVar1;
  byte *pbVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  bool bVar8;
  
  pbVar1 = (byte *)(param_2 + -1);
  uVar6 = param_3 >> 3;
  if ((param_3 & 7) != 0) {
    uVar6 = (param_3 >> 3) + 1;
  }
  pbVar2 = pbVar1;
  while (pbVar1 + uVar6 != pbVar2) {
    pbVar2 = pbVar2 + 1;
    *pbVar2 = 0;
  }
  if (param_3 == 0) {
    return;
  }
  iVar5 = 0;
  do {
    pbVar1 = pbVar1 + 1;
    uVar3 = (uint)*pbVar1;
    uVar4 = param_3 - 1;
    uVar7 = 0;
    uVar6 = uVar4;
    pbVar2 = (byte *)(param_1 + iVar5);
    do {
      param_3 = uVar6;
      bVar8 = uVar7 == uVar4;
      iVar5 = (int)(pbVar2 + 1) - param_1;
      uVar6 = uVar7 & 0xff;
      uVar7 = uVar7 + 1;
      uVar3 = uVar3 | (*pbVar2 & 1) << uVar6 & 0xff;
      *pbVar1 = (byte)uVar3;
      if (bVar8) {
        return;
      }
      uVar6 = param_3 - 1;
      pbVar2 = pbVar2 + 1;
    } while (uVar7 != 8);
  } while( true );
}

