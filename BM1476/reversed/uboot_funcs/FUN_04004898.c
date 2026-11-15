//==================== FUN_04004898 @ 04004898 ====================

byte * FUN_04004898(byte *param_1)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  byte *pbVar5;
  
  iVar2 = DAT_0400492c;
  bVar1 = *(byte *)(DAT_0400492c + (uint)*param_1);
  while ((bVar1 & 0x20) != 0) {
    param_1 = param_1 + 1;
    bVar1 = *(byte *)(DAT_0400492c + (uint)*param_1);
  }
  iVar3 = FUN_04005b98(param_1);
  if (iVar3 != 0) {
    uVar4 = iVar3 - 1;
    pbVar5 = param_1 + uVar4;
    if ((!CARRY4((uint)param_1,uVar4)) && ((*(byte *)(iVar2 + (uint)param_1[uVar4]) & 0x20) != 0)) {
      do {
        pbVar5 = pbVar5 + -1;
        if (pbVar5 == param_1 + -1) break;
      } while ((*(byte *)(iVar2 + (uint)*pbVar5) & 0x20) != 0);
    }
    pbVar5[1] = 0;
  }
  return param_1;
}

