//==================== FUN_00006d40 @ 00006d40 ====================

undefined4 FUN_00006d40(int param_1)

{
  byte bVar1;
  ushort uVar2;
  ushort uVar3;
  ushort uVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  undefined4 uVar8;
  uint uVar9;
  undefined1 uVar10;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_0001725c,0x167);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 0x24) != 0x11111111) {
    FUN_000014e4(&DAT_0001725c,0x168);
    DAT_00028e84 = 1;
    return 0;
  }
  *(undefined4 *)(param_1 + 0x14) = 4;
  if ((((*(char *)(param_1 + 0x36) != '\x02') ||
       ((*(uint *)(param_1 + 0x28) & 0xc0000000) != 0x40000000)) && (*(int *)(param_1 + 0xc) != 0))
     && ((*(uint *)(*(int *)(param_1 + 4) + 0x24) & 0x10000) == 0)) {
    return 1;
  }
  iVar5 = FUN_00006bcc(param_1,0,0);
  if (iVar5 != 0) {
    return 1;
  }
  uVar6 = FUN_00006bcc(param_1,0x800,0x1aa);
  if ((uVar6 & 0xfffffffd) != 0) {
    return 1;
  }
  iVar5 = *(int *)(param_1 + 4);
  if (uVar6 == 2) {
    *(undefined1 *)(iVar5 + 0x2f) = 2;
    iVar5 = *(int *)(param_1 + 4);
    bVar1 = *(byte *)(iVar5 + 0x2f);
    while ((bVar1 & 2) != 0) {
      bVar1 = *(byte *)(iVar5 + 0x2f);
    }
  }
  if (*(int *)(iVar5 + 0x10) == 0x1aa) {
    uVar10 = 2;
  }
  else {
    uVar10 = 1;
  }
  *(undefined1 *)(param_1 + 0x35) = uVar10;
  do {
    iVar5 = FUN_00006bcc(param_1,0x3700,0);
    uVar8 = 0x40ff8000;
    if (iVar5 != 0) {
      return 1;
    }
    if (*(char *)(param_1 + 0x36) == '\x02') {
      if (*(int *)(param_1 + 0x14) == 8) {
        uVar8 = 0x41ff8000;
      }
      else {
        uVar8 = 0x40ff8000;
      }
    }
    iVar5 = FUN_00006bcc(param_1,0xa900,uVar8,0);
    if (iVar5 != 0) {
      return 1;
    }
    uVar6 = *(uint *)(*(int *)(param_1 + 4) + 0x10);
  } while (-1 < (int)uVar6);
  if ((uVar6 & 0x40000000) != 0) {
    *(undefined4 *)(param_1 + 0x30) = 1;
  }
  if ((uVar6 & 0x1000000) != 0) {
    *(undefined1 *)(param_1 + 0x3c) = 1;
    iVar5 = FUN_00006bcc(param_1,0xb00,0);
    iVar7 = *(int *)(param_1 + 4);
    uVar6 = *(uint *)(iVar7 + 0x24);
    while ((uVar6 & 0x1f00000) != 0) {
      uVar6 = *(uint *)(iVar7 + 0x24);
    }
    *(ushort *)(iVar7 + 0x2c) = *(ushort *)(iVar7 + 0x2c) & 0xfffa;
    *(ushort *)(iVar7 + 0x3e) = *(ushort *)(iVar7 + 0x3e) | 8;
    func_0xffffe2ac(&LAB_00001388);
    iVar7 = *(int *)(param_1 + 4);
    if ((*(ushort *)(iVar7 + 0x3e) & 8) == 0) {
      return 1;
    }
    *(ushort *)(iVar7 + 0x2c) = *(ushort *)(iVar7 + 0x2c) | 1;
    uVar2 = *(ushort *)(iVar7 + 0x2c);
    while ((uVar2 & 2) == 0) {
      uVar2 = *(ushort *)(iVar7 + 0x2c);
    }
    *(ushort *)(iVar7 + 0x2c) = *(ushort *)(iVar7 + 0x2c) | 4;
    func_0xffffe2ac(1000);
    uVar6 = *(uint *)(*(int *)(param_1 + 4) + 0x24);
    while ((uVar6 & 0x1f00000) != 0x1f00000) {
      uVar6 = *(uint *)(*(int *)(param_1 + 4) + 0x24);
    }
    if (iVar5 != 0) {
      return 1;
    }
  }
  iVar5 = FUN_00006bcc(param_1,0x200,0);
  if (iVar5 == 0) {
    iVar5 = *(int *)(param_1 + 4);
    uVar2 = *(ushort *)(iVar5 + 0x14);
    uVar3 = *(ushort *)(iVar5 + 0x18);
    uVar4 = *(ushort *)(iVar5 + 0x1c);
    *(uint *)(param_1 + 0x40) = (uint)*(ushort *)(iVar5 + 0x10);
    *(uint *)(param_1 + 0x44) = (uint)uVar2;
    *(uint *)(param_1 + 0x48) = (uint)uVar3;
    *(uint *)(param_1 + 0x4c) = (uint)uVar4;
    do {
      iVar5 = FUN_00006bcc(param_1,0x300,0);
      if (iVar5 != 0) {
        return 1;
      }
      uVar6 = *(uint *)(*(int *)(param_1 + 4) + 0x10) & 0xffff0000;
      *(uint *)(param_1 + 0x50) = uVar6;
    } while (uVar6 == 0);
    iVar5 = FUN_00006bcc(param_1,0x900);
    if (iVar5 == 0) {
      iVar5 = *(int *)(param_1 + 4);
      uVar9 = *(uint *)(iVar5 + 0x14);
      uVar6 = (*(uint *)(iVar5 + 0x1c) & 0xffffff) >> 0x16;
      if (uVar6 == 0) {
        *(uint *)(param_1 + 100) =
             ((((*(uint *)(iVar5 + 0x18) & 3) << 10 | uVar9 >> 0x16) + 1 <<
              ((uVar9 & 0x3ff) >> 7) + 2) << ((*(uint *)(iVar5 + 0x18) & 0xfff) >> 8)) >> 9;
        return 0;
      }
      if (uVar6 == 1) {
        *(uint *)(param_1 + 100) = (((uVar9 & 0x3fffffff) >> 8) + 1) * 0x400;
      }
      return 0;
    }
  }
  return 1;
}

