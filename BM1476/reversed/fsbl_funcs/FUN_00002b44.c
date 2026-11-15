//==================== FUN_00002b44 @ 00002b44 ====================

uint FUN_00002b44(undefined4 *param_1,uint param_2)

{
  byte bVar1;
  uint uVar2;
  byte *pbVar4;
  byte *pbVar3;
  byte *pbVar5;
  bool bVar6;
  bool bVar7;
  bool bVar8;
  
  pbVar5 = (byte *)*param_1;
  if (pbVar5 != (byte *)0x0) {
    pbVar4 = pbVar5;
    do {
      pbVar3 = pbVar4 + 1;
      bVar1 = *pbVar4;
      bVar8 = 0x39 < bVar1;
      bVar6 = bVar1 != 0x3a;
      if (bVar6) {
        bVar8 = 0x1f < bVar1;
      }
      bVar7 = bVar1 != 0x20;
      if (bVar8 && (bVar6 && bVar7)) {
        param_2 = 1;
      }
      if (!bVar8 || (!bVar6 || !bVar7)) {
        param_2 = 0;
      }
      pbVar4 = pbVar3;
    } while (bVar8 && (bVar6 && bVar7));
    if (bVar1 != 0x3a) {
      return param_2;
    }
    uVar2 = *pbVar5 - 0x30;
    if ((((uVar2 & 0xff) < 10) && (pbVar3 == pbVar5 + 2)) && ((int)uVar2 < 2)) {
      *param_1 = pbVar3;
      return uVar2;
    }
  }
  return 0xffffffff;
}

