//==================== FUN_0005d160 @ 0005d160 ====================

undefined8 FUN_0005d160(uint param_1,uint param_2,uint param_3,uint param_4)

{
  longlong lVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  int extraout_r1;
  int extraout_r1_00;
  int extraout_r1_01;
  int extraout_r1_02;
  int extraout_r1_03;
  int extraout_r1_04;
  int extraout_r1_05;
  int extraout_r1_06;
  uint uVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  int iVar13;
  bool bVar14;
  
  if (param_4 == 0) {
    if (param_3 <= param_2) {
      uVar3 = param_1;
      uVar4 = param_2;
      uVar10 = param_3;
      if (param_3 == 0) {
        param_3 = FUN_0005c648(1,0);
      }
      iVar7 = LZCOUNT(param_3);
      if (iVar7 == 0) {
        iVar7 = param_2 - param_3;
        uVar12 = 1;
        uVar8 = param_1;
      }
      else {
        param_3 = param_3 << iVar7;
        uVar6 = param_2 >> (0x20U - iVar7 & 0xff);
        uVar8 = param_1 << iVar7;
        uVar12 = param_3 >> 0x10;
        uVar11 = param_1 >> (0x20U - iVar7 & 0xff) | param_2 << iVar7;
        FUN_0005c6f0(uVar6,uVar12);
        iVar7 = FUN_0005c648(uVar6,uVar12);
        uVar9 = uVar11 >> 0x10 | extraout_r1_01 << 0x10;
        uVar6 = (param_3 & 0xffff) * iVar7;
        iVar2 = iVar7;
        if (uVar9 < uVar6) {
          bVar14 = CARRY4(uVar9,param_3);
          uVar9 = uVar9 + param_3;
          iVar2 = iVar7 + -1;
          if ((!bVar14) && (uVar9 < uVar6)) {
            uVar9 = uVar9 + param_3;
            iVar2 = iVar7 + -2;
          }
        }
        FUN_0005c6f0(uVar9 - uVar6,uVar12,uVar6,uVar9,uVar3,uVar4,uVar10);
        uVar6 = FUN_0005c648(uVar9 - uVar6,uVar12);
        uVar9 = uVar11 & 0xffff | extraout_r1_02 << 0x10;
        uVar11 = (param_3 & 0xffff) * uVar6;
        uVar12 = uVar6;
        if (uVar9 < uVar11) {
          bVar14 = CARRY4(uVar9,param_3);
          uVar9 = uVar9 + param_3;
          uVar12 = uVar6 - 1;
          if ((!bVar14) && (uVar9 < uVar11)) {
            uVar9 = uVar9 + param_3;
            uVar12 = uVar6 - 2;
          }
        }
        iVar7 = uVar9 - uVar11;
        uVar12 = uVar12 | iVar2 << 0x10;
      }
      uVar6 = param_3 >> 0x10;
      FUN_0005c6f0(iVar7,uVar6);
      iVar2 = FUN_0005c648(iVar7,uVar6);
      uVar11 = uVar8 >> 0x10 | extraout_r1_03 << 0x10;
      uVar9 = (param_3 & 0xffff) * iVar2;
      iVar7 = iVar2;
      if (uVar11 < uVar9) {
        bVar14 = CARRY4(uVar11,param_3);
        uVar11 = uVar11 + param_3;
        iVar7 = iVar2 + -1;
        if ((!bVar14) && (uVar11 < uVar9)) {
          uVar11 = uVar11 + param_3;
          iVar7 = iVar2 + -2;
        }
      }
      FUN_0005c6f0(uVar11 - uVar9,uVar6,uVar9,uVar11,uVar3,uVar4,uVar10);
      uVar4 = FUN_0005c648(uVar11 - uVar9,uVar6);
      uVar10 = uVar8 & 0xffff | extraout_r1_04 << 0x10;
      uVar8 = (param_3 & 0xffff) * uVar4;
      uVar3 = uVar4;
      if (uVar10 < uVar8) {
        uVar3 = uVar4 - 1;
        if ((!CARRY4(uVar10,param_3)) && (uVar10 + param_3 < uVar8)) {
          uVar3 = uVar4 - 2;
        }
      }
      uVar3 = uVar3 | iVar7 << 0x10;
      goto LAB_0005d534;
    }
    iVar7 = LZCOUNT(param_3);
    uVar12 = param_2;
    uVar3 = param_1;
    uVar4 = param_3;
    if (iVar7 != 0) {
      uVar4 = param_3 << iVar7;
      uVar3 = param_1 << iVar7;
      uVar12 = param_1 >> (0x20U - iVar7 & 0xff) | param_2 << iVar7;
    }
    uVar10 = uVar4 >> 0x10;
    FUN_0005c6f0(uVar12,uVar10,param_3,iVar7,param_1,param_2,param_3);
    iVar2 = FUN_0005c648(uVar12,uVar10);
    uVar8 = uVar3 >> 0x10 | extraout_r1 << 0x10;
    uVar12 = (uVar4 & 0xffff) * iVar2;
    iVar7 = iVar2;
    if (uVar8 < uVar12) {
      bVar14 = CARRY4(uVar8,uVar4);
      uVar8 = uVar8 + uVar4;
      iVar7 = iVar2 + -1;
      if ((!bVar14) && (uVar8 < uVar12)) {
        uVar8 = uVar8 + uVar4;
        iVar7 = iVar2 + -2;
      }
    }
    FUN_0005c6f0(uVar8 - uVar12,uVar10,uVar12,uVar8,param_1,param_2,param_3);
    uVar12 = FUN_0005c648(uVar8 - uVar12,uVar10);
    uVar10 = uVar3 & 0xffff | extraout_r1_00 << 0x10;
    uVar8 = (uVar4 & 0xffff) * uVar12;
    uVar3 = uVar12;
    if (uVar10 < uVar8) {
      uVar3 = uVar12 - 1;
      if ((!CARRY4(uVar10,uVar4)) && (uVar10 + uVar4 < uVar8)) {
        uVar3 = uVar12 - 2;
      }
    }
    uVar3 = uVar3 | iVar7 << 0x10;
LAB_0005d524:
    uVar12 = 0;
  }
  else {
    if (param_4 <= param_2) {
      iVar7 = LZCOUNT(param_4);
      if (iVar7 != 0) {
        uVar3 = 0x20 - iVar7;
        uVar4 = param_2 >> (uVar3 & 0xff);
        uVar12 = param_3 >> (uVar3 & 0xff) | param_4 << iVar7;
        uVar10 = param_1 >> (uVar3 & 0xff) | param_2 << iVar7;
        uVar3 = uVar12 >> 0x10;
        FUN_0005c6f0(uVar4,uVar3,param_3,param_4,param_1,param_2,param_3);
        iVar2 = extraout_r1_05;
        iVar5 = FUN_0005c648(uVar4,uVar3);
        uVar8 = uVar10 >> 0x10 | iVar2 << 0x10;
        uVar4 = (uVar12 & 0xffff) * iVar5;
        iVar2 = iVar5;
        if (uVar8 < uVar4) {
          bVar14 = CARRY4(uVar8,uVar12);
          uVar8 = uVar8 + uVar12;
          iVar2 = iVar5 + -1;
          if ((!bVar14) && (uVar8 < uVar4)) {
            uVar8 = uVar8 + uVar12;
            iVar2 = iVar5 + -2;
          }
        }
        iVar13 = uVar8 - uVar4;
        FUN_0005c6f0(iVar13,uVar3);
        iVar5 = extraout_r1_06;
        uVar4 = FUN_0005c648(iVar13,uVar3);
        uVar10 = uVar10 & 0xffff | iVar5 << 0x10;
        uVar8 = (uVar12 & 0xffff) * uVar4;
        uVar3 = uVar4;
        if (uVar10 < uVar8) {
          bVar14 = CARRY4(uVar10,uVar12);
          uVar10 = uVar10 + uVar12;
          uVar3 = uVar4 - 1;
          if ((!bVar14) && (uVar10 < uVar8)) {
            uVar10 = uVar10 + uVar12;
            uVar3 = uVar4 - 2;
          }
        }
        uVar3 = uVar3 | iVar2 << 0x10;
        uVar10 = uVar10 - uVar8;
        lVar1 = (ulonglong)(param_3 << iVar7) * (ulonglong)uVar3;
        uVar4 = (uint)((ulonglong)lVar1 >> 0x20);
        if ((uVar4 < uVar10 || uVar4 == uVar10) &&
           (uVar12 = 0, (uint)lVar1 <= param_1 << iVar7 || uVar4 != uVar10)) goto LAB_0005d534;
        uVar3 = uVar3 - 1;
        goto LAB_0005d524;
      }
      bVar14 = param_4 <= param_2;
      if (param_2 <= param_4) {
        bVar14 = param_3 <= param_1;
      }
      if (bVar14) {
        uVar3 = 1;
        uVar12 = 0;
        goto LAB_0005d534;
      }
    }
    uVar3 = 0;
    uVar12 = uVar3;
  }
LAB_0005d534:
  return CONCAT44(uVar12,uVar3);
}

