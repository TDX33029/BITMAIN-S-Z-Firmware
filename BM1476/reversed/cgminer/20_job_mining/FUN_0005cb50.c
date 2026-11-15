//==================== FUN_0005cb50 @ 0005cb50 ====================

ulonglong FUN_0005cb50(uint param_1,uint param_2)

{
  byte bVar1;
  byte bVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  int iVar10;
  bool bVar11;
  bool bVar12;
  
  if (param_1 == 0 && param_2 == 0) {
    return CONCAT44(param_2,param_1);
  }
  iVar10 = 0x432;
  uVar9 = 0;
  if (param_2 >> 0x16 != 0) {
    iVar10 = 3;
    if (param_2 >> 0x19 != 0) {
      iVar10 = 6;
    }
    if (param_2 >> 0x1c != 0) {
      iVar10 = iVar10 + 3;
    }
    iVar10 = iVar10 - ((int)param_2 >> 0x1f);
    uVar9 = param_1 << (0x20U - iVar10 & 0xff);
    param_1 = param_1 >> iVar10 | param_2 << (0x20U - iVar10 & 0xff);
    param_2 = param_2 >> iVar10;
    iVar10 = iVar10 + 0x432;
  }
  if (0xfffff < param_2) {
    if (0x1fffff < param_2) {
      bVar1 = (byte)param_2;
      param_2 = param_2 >> 1;
      bVar2 = (byte)param_1;
      param_1 = (uint)(bVar1 & 1) << 0x1f | param_1 >> 1;
      uVar9 = (uint)(bVar2 & 1) << 0x1f | uVar9 >> 1;
      iVar10 = iVar10 + 1;
      if (0xffbfffff < (uint)(iVar10 * 0x200000)) {
        return 0x7ff0000000000000;
      }
    }
LAB_0005c930:
    bVar11 = 0x7fffffff < uVar9;
    if (uVar9 == 0x80000000) {
      bVar11 = (bool)((byte)param_1 & 1);
    }
    return CONCAT44(param_2 + iVar10 * 0x100000 + (uint)CARRY4(param_1,(uint)bVar11),
                    param_1 + bVar11);
  }
  bVar1 = (byte)(uVar9 >> 0x1f);
  uVar9 = uVar9 << 1;
  uVar3 = param_1 * 2;
  bVar11 = CARRY4(param_1,param_1);
  param_1 = param_1 * 2 + (uint)bVar1;
  param_2 = param_2 * 2 + (uint)(bVar11 || CARRY4(uVar3,(uint)bVar1));
  iVar10 = iVar10 + -1;
  if ((param_2 & 0x100000) != 0) goto LAB_0005c930;
  uVar3 = param_1;
  uVar4 = param_2;
  if (param_2 == 0) {
    uVar3 = 0;
    uVar4 = param_1;
  }
  iVar7 = LZCOUNT(uVar4);
  if (param_2 == 0) {
    iVar7 = iVar7 + 0x20;
  }
  uVar8 = iVar7 - 0xb;
  bVar12 = SBORROW4(uVar8,0x20);
  uVar5 = iVar7 - 0x2b;
  bVar11 = uVar5 == 0;
  uVar6 = uVar5;
  if ((int)uVar8 < 0x20) {
    bVar12 = SCARRY4(uVar5,0xc);
    uVar6 = iVar7 - 0x1f;
    bVar11 = uVar6 == 0;
    if (!bVar11 && -0xd < (int)uVar5) {
      uVar3 = uVar4 << (uVar8 & 0xff);
      uVar4 = uVar4 >> (0xc - uVar6 & 0xff);
      goto LAB_0005c9b0;
    }
    uVar5 = uVar8;
  }
  if (bVar11 || (int)uVar6 < 0 != bVar12) {
    uVar9 = 0x20 - uVar5;
  }
  uVar4 = uVar4 << (uVar5 & 0xff);
  if (bVar11 || (int)uVar6 < 0 != bVar12) {
    uVar4 = uVar4 | uVar3 >> (uVar9 & 0xff);
    uVar3 = uVar3 << (uVar5 & 0xff);
  }
LAB_0005c9b0:
  if ((int)uVar8 <= iVar10) {
    return CONCAT44(uVar4 + (iVar10 - uVar8) * 0x100000,uVar3);
  }
  uVar9 = ~(iVar10 - uVar8);
  if (0x1e < (int)uVar9) {
    return (ulonglong)(uVar4 >> (uVar9 - 0x1f & 0xff));
  }
  if (uVar9 - 0x13 == 0 || (int)(uVar9 - 0x1f) < -0xc) {
    uVar9 = uVar9 + 1;
    return CONCAT44(uVar4 >> (uVar9 & 0xff),uVar3 >> (uVar9 & 0xff) | uVar4 << (0x20 - uVar9 & 0xff)
                   );
  }
  uVar9 = 0xc - (uVar9 - 0x13);
  return (ulonglong)(uVar3 >> (0x20 - uVar9 & 0xff) | uVar4 << (uVar9 & 0xff));
}

