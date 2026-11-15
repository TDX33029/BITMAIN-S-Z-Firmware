//==================== FUN_0005cb64 @ 0005cb64 ====================

ulonglong FUN_0005cb64(uint param_1,uint param_2)

{
  byte bVar1;
  byte bVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  int iVar9;
  uint uVar10;
  uint uVar11;
  bool bVar12;
  bool bVar13;
  
  if (param_1 == 0 && param_2 == 0) {
    return CONCAT44(param_2,param_1);
  }
  uVar10 = param_2 & 0x80000000;
  uVar4 = param_2;
  if ((int)uVar10 < 0) {
    bVar12 = param_1 != 0;
    param_1 = -param_1;
    uVar4 = -(param_2 + bVar12);
  }
  iVar9 = 0x432;
  uVar11 = 0;
  if (uVar4 >> 0x16 != 0) {
    iVar9 = 3;
    if (uVar4 >> 0x19 != 0) {
      iVar9 = 6;
    }
    if (uVar4 >> 0x1c != 0) {
      iVar9 = iVar9 + 3;
    }
    iVar9 = iVar9 - ((int)uVar4 >> 0x1f);
    uVar11 = param_1 << (0x20U - iVar9 & 0xff);
    param_1 = param_1 >> iVar9 | uVar4 << (0x20U - iVar9 & 0xff);
    uVar4 = uVar4 >> iVar9;
    iVar9 = iVar9 + 0x432;
  }
  if (0xfffff < uVar4) {
    if (0x1fffff < uVar4) {
      bVar1 = (byte)uVar4;
      uVar4 = uVar4 >> 1;
      bVar2 = (byte)param_1;
      param_1 = (uint)(bVar1 & 1) << 0x1f | param_1 >> 1;
      uVar11 = (uint)(bVar2 & 1) << 0x1f | uVar11 >> 1;
      iVar9 = iVar9 + 1;
      if (0xffbfffff < (uint)(iVar9 * 0x200000)) {
        return (ulonglong)(uVar10 | 0x7ff00000) << 0x20;
      }
    }
LAB_0005c930:
    bVar12 = 0x7fffffff < uVar11;
    if (uVar11 == 0x80000000) {
      bVar12 = (bool)((byte)param_1 & 1);
    }
    return CONCAT44(uVar4 + iVar9 * 0x100000 + (uint)CARRY4(param_1,(uint)bVar12) | uVar10,
                    param_1 + bVar12);
  }
  bVar1 = (byte)(uVar11 >> 0x1f);
  uVar11 = uVar11 << 1;
  uVar3 = param_1 * 2;
  bVar12 = CARRY4(param_1,param_1);
  param_1 = param_1 * 2 + (uint)bVar1;
  uVar4 = uVar4 * 2 + (uint)(bVar12 || CARRY4(uVar3,(uint)bVar1));
  iVar9 = iVar9 + -1;
  if ((uVar4 & 0x100000) != 0) goto LAB_0005c930;
  uVar3 = param_1;
  uVar5 = uVar4;
  if (uVar4 == 0) {
    uVar3 = 0;
    uVar5 = param_1;
  }
  iVar7 = LZCOUNT(uVar5);
  if (uVar4 == 0) {
    iVar7 = iVar7 + 0x20;
  }
  uVar8 = iVar7 - 0xb;
  bVar13 = SBORROW4(uVar8,0x20);
  uVar4 = iVar7 - 0x2b;
  bVar12 = uVar4 == 0;
  uVar6 = uVar4;
  if ((int)uVar8 < 0x20) {
    bVar13 = SCARRY4(uVar4,0xc);
    uVar6 = iVar7 - 0x1f;
    bVar12 = uVar6 == 0;
    if (!bVar12 && -0xd < (int)uVar4) {
      uVar3 = uVar5 << (uVar8 & 0xff);
      uVar5 = uVar5 >> (0xc - uVar6 & 0xff);
      goto LAB_0005c9b0;
    }
    uVar4 = uVar8;
  }
  if (bVar12 || (int)uVar6 < 0 != bVar13) {
    uVar11 = 0x20 - uVar4;
  }
  uVar5 = uVar5 << (uVar4 & 0xff);
  if (bVar12 || (int)uVar6 < 0 != bVar13) {
    uVar5 = uVar5 | uVar3 >> (uVar11 & 0xff);
    uVar3 = uVar3 << (uVar4 & 0xff);
  }
LAB_0005c9b0:
  if ((int)uVar8 <= iVar9) {
    return CONCAT44(uVar5 + (iVar9 - uVar8) * 0x100000 | uVar10,uVar3);
  }
  uVar4 = ~(iVar9 - uVar8);
  if (0x1e < (int)uVar4) {
    return CONCAT44(param_2,uVar5 >> (uVar4 - 0x1f & 0xff)) & 0x80000000ffffffff;
  }
  if (uVar4 - 0x13 == 0 || (int)(uVar4 - 0x1f) < -0xc) {
    uVar4 = uVar4 + 1;
    return CONCAT44(uVar10 | uVar5 >> (uVar4 & 0xff),
                    uVar3 >> (uVar4 & 0xff) | uVar5 << (0x20 - uVar4 & 0xff));
  }
  uVar4 = 0xc - (uVar4 - 0x13);
  return CONCAT44(param_2,uVar3 >> (0x20 - uVar4 & 0xff) | uVar5 << (uVar4 & 0xff)) &
         0x80000000ffffffff;
}

