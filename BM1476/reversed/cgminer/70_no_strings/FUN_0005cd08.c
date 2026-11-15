//==================== FUN_0005cd08 @ 0005cd08 ====================

undefined8 FUN_0005cd08(uint param_1,uint param_2,uint param_3,uint param_4)

{
  longlong lVar1;
  int iVar2;
  int iVar3;
  int extraout_r1;
  int extraout_r1_00;
  int extraout_r1_01;
  uint extraout_r1_02;
  int extraout_r1_03;
  int extraout_r1_04;
  int extraout_r1_05;
  int extraout_r1_06;
  int iVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  int iVar13;
  bool bVar14;
  
  if ((int)param_2 < 0) {
    uVar8 = -param_1;
    uVar11 = -(param_2 + (param_1 != 0));
    uVar6 = 0xffffffff;
  }
  else {
    uVar6 = 0;
    uVar8 = param_1;
    uVar11 = param_2;
  }
  uVar5 = param_3;
  if ((int)param_4 < 0) {
    uVar5 = -param_3;
    param_4 = -(param_4 + (param_3 != 0));
    uVar6 = ~uVar6;
  }
  if (param_4 == 0) {
    if (uVar5 <= uVar11) {
      if (uVar5 == 0) {
        uVar5 = FUN_0005c648(1,0);
      }
      iVar4 = LZCOUNT(uVar5);
      if (iVar4 == 0) {
        iVar4 = uVar11 - uVar5;
        uVar7 = 1;
      }
      else {
        uVar5 = uVar5 << iVar4;
        uVar12 = uVar11 >> (0x20U - iVar4 & 0xff);
        uVar7 = uVar5 >> 0x10;
        uVar9 = uVar8 >> (0x20U - iVar4 & 0xff) | uVar11 << iVar4;
        uVar8 = uVar8 << iVar4;
        FUN_0005c6f0(uVar12,uVar7);
        iVar4 = FUN_0005c648(uVar12,uVar7);
        uVar12 = uVar9 >> 0x10 | extraout_r1_01 << 0x10;
        uVar11 = (uVar5 & 0xffff) * iVar4;
        iVar2 = iVar4;
        if (uVar12 < uVar11) {
          bVar14 = CARRY4(uVar12,uVar5);
          uVar12 = uVar12 + uVar5;
          iVar2 = iVar4 + -1;
          if ((!bVar14) && (uVar12 < uVar11)) {
            uVar12 = uVar12 + uVar5;
            iVar2 = iVar4 + -2;
          }
        }
        FUN_0005c6f0(uVar12 - uVar11,uVar7,uVar11,uVar12,param_1,param_2,param_3);
        param_1 = extraout_r1_02;
        uVar11 = FUN_0005c648(uVar12 - uVar11,uVar7);
        uVar12 = uVar9 & 0xffff | param_1 << 0x10;
        uVar9 = (uVar5 & 0xffff) * uVar11;
        uVar7 = uVar11;
        if (uVar12 < uVar9) {
          bVar14 = CARRY4(uVar12,uVar5);
          uVar12 = uVar12 + uVar5;
          uVar7 = uVar11 - 1;
          if ((!bVar14) && (uVar12 < uVar9)) {
            uVar12 = uVar12 + uVar5;
            uVar7 = uVar11 - 2;
          }
        }
        iVar4 = uVar12 - uVar9;
        uVar7 = uVar7 | iVar2 << 0x10;
      }
      uVar11 = uVar5 >> 0x10;
      FUN_0005c6f0(iVar4,uVar11);
      iVar2 = FUN_0005c648(iVar4,uVar11);
      uVar9 = uVar8 >> 0x10 | extraout_r1_03 << 0x10;
      uVar12 = (uVar5 & 0xffff) * iVar2;
      iVar4 = iVar2;
      if (uVar9 < uVar12) {
        bVar14 = CARRY4(uVar9,uVar5);
        uVar9 = uVar9 + uVar5;
        iVar4 = iVar2 + -1;
        if ((!bVar14) && (uVar9 < uVar12)) {
          uVar9 = uVar9 + uVar5;
          iVar4 = iVar2 + -2;
        }
      }
      FUN_0005c6f0(uVar9 - uVar12,uVar11,uVar12,uVar9,param_1,param_2,param_3);
      iVar2 = extraout_r1_04;
      uVar11 = FUN_0005c648(uVar9 - uVar12,uVar11);
      uVar9 = uVar8 & 0xffff | iVar2 << 0x10;
      uVar12 = (uVar5 & 0xffff) * uVar11;
      uVar8 = uVar11;
      if (uVar9 < uVar12) {
        uVar8 = uVar11 - 1;
        if ((!CARRY4(uVar9,uVar5)) && (uVar9 + uVar5 < uVar12)) {
          uVar8 = uVar11 - 2;
        }
      }
      uVar11 = uVar8 | iVar4 << 0x10;
      goto LAB_0005d140;
    }
    iVar4 = LZCOUNT(uVar5);
    uVar7 = uVar5;
    if (iVar4 != 0) {
      uVar7 = uVar5 << iVar4;
      uVar5 = uVar8 >> (0x20U - iVar4 & 0xff);
      uVar8 = uVar8 << iVar4;
      uVar11 = uVar5 | uVar11 << iVar4;
    }
    uVar12 = uVar7 >> 0x10;
    FUN_0005c6f0(uVar11,uVar12,uVar5,iVar4,param_1,param_2,param_3);
    iVar2 = FUN_0005c648(uVar11,uVar12);
    uVar5 = uVar8 >> 0x10 | extraout_r1 << 0x10;
    uVar11 = (uVar7 & 0xffff) * iVar2;
    iVar4 = iVar2;
    if (uVar5 < uVar11) {
      bVar14 = CARRY4(uVar5,uVar7);
      uVar5 = uVar5 + uVar7;
      iVar4 = iVar2 + -1;
      if ((!bVar14) && (uVar5 < uVar11)) {
        uVar5 = uVar5 + uVar7;
        iVar4 = iVar2 + -2;
      }
    }
    FUN_0005c6f0(uVar5 - uVar11,uVar12,uVar11,uVar5,param_1,param_2,param_3);
    uVar5 = FUN_0005c648(uVar5 - uVar11,uVar12);
    uVar12 = uVar8 & 0xffff | extraout_r1_00 << 0x10;
    uVar8 = (uVar7 & 0xffff) * uVar5;
    uVar11 = uVar5;
    if (uVar12 < uVar8) {
      uVar11 = uVar5 - 1;
      if ((!CARRY4(uVar12,uVar7)) && (uVar12 + uVar7 < uVar8)) {
        uVar11 = uVar5 - 2;
      }
    }
    uVar11 = uVar11 | iVar4 << 0x10;
LAB_0005d130:
    uVar7 = 0;
  }
  else {
    if (param_4 <= uVar11) {
      iVar4 = LZCOUNT(param_4);
      if (iVar4 != 0) {
        uVar12 = 0x20 - iVar4;
        uVar7 = uVar11 >> (uVar12 & 0xff);
        uVar9 = uVar5 >> (uVar12 & 0xff) | param_4 << iVar4;
        uVar10 = uVar8 >> (uVar12 & 0xff) | uVar11 << iVar4;
        uVar11 = uVar9 >> 0x10;
        FUN_0005c6f0(uVar7,uVar11,uVar7,param_4,param_1,uVar7,param_3);
        iVar2 = extraout_r1_05;
        iVar3 = FUN_0005c648(uVar7,uVar11);
        uVar7 = uVar10 >> 0x10 | iVar2 << 0x10;
        uVar12 = (uVar9 & 0xffff) * iVar3;
        iVar2 = iVar3;
        if (uVar7 < uVar12) {
          bVar14 = CARRY4(uVar7,uVar9);
          uVar7 = uVar7 + uVar9;
          iVar2 = iVar3 + -1;
          if ((!bVar14) && (uVar7 < uVar12)) {
            uVar7 = uVar7 + uVar9;
            iVar2 = iVar3 + -2;
          }
        }
        iVar13 = uVar7 - uVar12;
        FUN_0005c6f0(iVar13,uVar11);
        iVar3 = extraout_r1_06;
        uVar7 = FUN_0005c648(iVar13,uVar11);
        uVar12 = uVar10 & 0xffff | iVar3 << 0x10;
        uVar10 = (uVar9 & 0xffff) * uVar7;
        uVar11 = uVar7;
        if (uVar12 < uVar10) {
          bVar14 = CARRY4(uVar12,uVar9);
          uVar12 = uVar12 + uVar9;
          uVar11 = uVar7 - 1;
          if ((!bVar14) && (uVar12 < uVar10)) {
            uVar12 = uVar12 + uVar9;
            uVar11 = uVar7 - 2;
          }
        }
        uVar11 = uVar11 | iVar2 << 0x10;
        uVar12 = uVar12 - uVar10;
        lVar1 = (ulonglong)(uVar5 << iVar4) * (ulonglong)uVar11;
        uVar5 = (uint)((ulonglong)lVar1 >> 0x20);
        if ((uVar5 < uVar12 || uVar5 == uVar12) &&
           (uVar7 = 0, (uint)lVar1 <= uVar8 << iVar4 || uVar5 != uVar12)) goto LAB_0005d140;
        uVar11 = uVar11 - 1;
        goto LAB_0005d130;
      }
      bVar14 = param_4 <= uVar11;
      if (uVar11 <= param_4) {
        bVar14 = uVar5 <= uVar8;
      }
      if (bVar14) {
        uVar7 = 0;
        uVar11 = 1;
        goto LAB_0005d140;
      }
    }
    uVar7 = 0;
    uVar11 = uVar7;
  }
LAB_0005d140:
  if (uVar6 != 0) {
    bVar14 = uVar11 != 0;
    uVar11 = -uVar11;
    uVar7 = -(uVar7 + bVar14);
  }
  return CONCAT44(uVar7,uVar11);
}

