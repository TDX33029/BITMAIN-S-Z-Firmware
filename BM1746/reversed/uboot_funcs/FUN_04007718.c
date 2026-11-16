//==================== FUN_04007718 @ 04007718 ====================

uint FUN_04007718(byte *param_1,undefined4 *param_2,uint param_3)

{
  byte bVar1;
  uint uVar2;
  byte *pbVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  
  uVar2 = (uint)*param_1;
  if (uVar2 == 0x30) {
    uVar5 = (uint)param_1[1];
    pbVar3 = param_1 + 1;
    if (uVar5 != 0x78) {
LAB_040077b4:
      bVar1 = *(byte *)(DAT_04007800 + uVar5);
      uVar2 = uVar5;
      if (param_3 == 0) {
        param_3 = 8;
      }
      goto LAB_04007744;
    }
    uVar2 = (uint)param_1[2];
    uVar4 = (uint)*(byte *)(DAT_04007800 + uVar2);
    if ((*(byte *)(DAT_04007800 + uVar2) & 0x44) == 0) goto LAB_040077b4;
    pbVar3 = param_1 + 2;
    param_3 = 0x10;
  }
  else {
    if (param_3 == 0) {
      param_3 = 10;
    }
    bVar1 = *(byte *)(DAT_04007800 + uVar2);
    pbVar3 = param_1;
LAB_04007744:
    uVar4 = (uint)bVar1;
    uVar5 = uVar4 & 0x44;
    if ((bVar1 & 0x44) == 0) goto LAB_04007794;
  }
  uVar5 = 0;
  do {
    uVar6 = uVar2 - 0x30;
    if ((uVar4 & 4) == 0) {
      uVar6 = uVar2 - 0x37;
      if ((uVar4 & 2) != 0) {
        uVar6 = (uVar2 - 0x20 & 0xff) - 0x37;
      }
    }
    if (param_3 <= uVar6) break;
    pbVar3 = pbVar3 + 1;
    uVar2 = (uint)*pbVar3;
    uVar5 = param_3 * uVar5 + uVar6;
    uVar4 = (uint)*(byte *)(DAT_04007800 + uVar2);
  } while ((*(byte *)(DAT_04007800 + uVar2) & 0x44) != 0);
LAB_04007794:
  if (param_2 != (undefined4 *)0x0) {
    *param_2 = pbVar3;
  }
  return uVar5;
}

