//==================== FUN_040076e8 @ 040076e8 ====================

uint FUN_040076e8(void)

{
  byte bVar1;
  byte *pbVar2;
  undefined4 *puVar3;
  uint extraout_r2;
  uint uVar4;
  uint uVar5;
  byte *pbVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  undefined8 uVar10;
  undefined4 auStack_14 [3];
  
  func_0x03fc7fbc();
  uVar10 = FUN_040076b8();
  auStack_14[0] = (undefined4)((ulonglong)uVar10 >> 0x20);
  FUN_040075a0((int)uVar10,auStack_14);
  uVar10 = FUN_040076b8();
  puVar3 = (undefined4 *)((ulonglong)uVar10 >> 0x20);
  pbVar2 = (byte *)uVar10;
  uVar5 = (uint)*pbVar2;
  uVar4 = extraout_r2;
  if (uVar5 == 0x30) {
    uVar8 = (uint)pbVar2[1];
    pbVar6 = pbVar2 + 1;
    if (uVar8 != 0x78) {
LAB_040077b4:
      bVar1 = *(byte *)(DAT_04007800 + uVar8);
      uVar5 = uVar8;
      if (extraout_r2 == 0) {
        uVar4 = 8;
      }
      goto LAB_04007744;
    }
    uVar5 = (uint)pbVar2[2];
    uVar7 = (uint)*(byte *)(DAT_04007800 + uVar5);
    if ((*(byte *)(DAT_04007800 + uVar5) & 0x44) == 0) goto LAB_040077b4;
    pbVar6 = pbVar2 + 2;
    uVar4 = 0x10;
  }
  else {
    if (extraout_r2 == 0) {
      uVar4 = 10;
    }
    bVar1 = *(byte *)(DAT_04007800 + uVar5);
    pbVar6 = pbVar2;
LAB_04007744:
    uVar7 = (uint)bVar1;
    uVar8 = uVar7 & 0x44;
    if ((bVar1 & 0x44) == 0) goto LAB_04007794;
  }
  uVar8 = 0;
  do {
    uVar9 = uVar5 - 0x30;
    if ((uVar7 & 4) == 0) {
      uVar9 = uVar5 - 0x37;
      if ((uVar7 & 2) != 0) {
        uVar9 = (uVar5 - 0x20 & 0xff) - 0x37;
      }
    }
    if (uVar4 <= uVar9) break;
    pbVar6 = pbVar6 + 1;
    uVar5 = (uint)*pbVar6;
    uVar8 = uVar4 * uVar8 + uVar9;
    uVar7 = (uint)*(byte *)(DAT_04007800 + uVar5);
  } while ((*(byte *)(DAT_04007800 + uVar5) & 0x44) != 0);
LAB_04007794:
  if (puVar3 != (undefined4 *)0x0) {
    *puVar3 = pbVar6;
  }
  return uVar8;
}

