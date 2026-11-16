//==================== FUN_0400799c @ 0400799c ====================

undefined8 FUN_0400799c(byte *param_1,undefined4 *param_2,uint param_3)

{
  byte bVar1;
  ulonglong uVar2;
  byte *pbVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  
  uVar6 = (uint)*param_1;
  if (uVar6 == 0x30) {
    uVar5 = (uint)param_1[1];
    pbVar3 = param_1 + 1;
    if (uVar5 != 0x78) {
LAB_04007a70:
      bVar1 = *(byte *)(DAT_04007ac0 + uVar5);
      uVar6 = uVar5;
      if (param_3 == 0) {
        param_3 = 8;
      }
      goto LAB_040079cc;
    }
    uVar6 = (uint)param_1[2];
    bVar1 = *(byte *)(DAT_04007ac0 + uVar6);
    if ((bVar1 & 0x44) == 0) goto LAB_04007a70;
    pbVar3 = param_1 + 2;
    param_3 = 0x10;
  }
  else {
    if (param_3 == 0) {
      param_3 = 10;
    }
    bVar1 = *(byte *)(DAT_04007ac0 + uVar6);
    pbVar3 = param_1;
LAB_040079cc:
    if ((bVar1 & 0x44) == 0) {
      uVar5 = 0;
      iVar4 = 0;
      goto LAB_04007a44;
    }
  }
  uVar5 = 0;
  iVar4 = 0;
  do {
    uVar7 = uVar6 - 0x30;
    if ((bVar1 & 4) == 0) {
      uVar7 = uVar6 - 0x37;
      if ((bVar1 & 2) != 0) {
        uVar7 = (uVar6 - 0x20 & 0xff) - 0x37;
      }
    }
    uVar2 = (ulonglong)uVar5;
    uVar8 = (uint)(param_3 * uVar2);
    if ((int)uVar7 < 0 || param_3 <= uVar7) break;
    pbVar3 = pbVar3 + 1;
    uVar6 = (uint)*pbVar3;
    uVar5 = uVar8 + uVar7;
    bVar1 = *(byte *)(DAT_04007ac0 + uVar6);
    iVar4 = param_3 * iVar4 + (int)(param_3 * uVar2 >> 0x20) + ((int)uVar7 >> 0x1f) +
            (uint)CARRY4(uVar8,uVar7);
  } while ((bVar1 & 0x44) != 0);
LAB_04007a44:
  if (param_2 != (undefined4 *)0x0) {
    *param_2 = pbVar3;
  }
  return CONCAT44(iVar4,uVar5);
}

