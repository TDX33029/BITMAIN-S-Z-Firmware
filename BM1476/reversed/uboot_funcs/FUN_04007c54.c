//==================== FUN_04007c54 @ 04007c54 ====================

undefined4 FUN_04007c54(byte *param_1)

{
  byte *pbVar1;
  byte *pbVar2;
  undefined4 uVar3;
  int iVar4;
  
  iVar4 = FUN_04005b98();
  pbVar2 = param_1 + iVar4 + -1;
  if (pbVar2 <= param_1) {
    return 0xffffffff;
  }
  if ((*(byte *)(DAT_04007c50 + (uint)param_1[iVar4 + -1]) & 4) != 0) {
    pbVar1 = param_1 + iVar4 + -2;
    do {
      pbVar2 = pbVar1;
      if (pbVar2 == param_1) {
        return 0xffffffff;
      }
      pbVar1 = pbVar2 + -1;
    } while ((*(byte *)(DAT_04007c50 + (uint)*pbVar2) & 4) != 0);
  }
  uVar3 = FUN_04007718(pbVar2 + 1,0,10);
  return uVar3;
}

