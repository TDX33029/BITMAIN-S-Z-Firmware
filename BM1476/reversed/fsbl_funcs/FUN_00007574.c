//==================== FUN_00007574 @ 00007574 ====================

undefined4 FUN_00007574(int param_1)

{
  ushort uVar1;
  ushort uVar2;
  ushort uVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_0001725c,0x6ce);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 0x24) != 0x11111111) {
    FUN_000014e4(&DAT_0001725c,0x6cf);
    DAT_00028e84 = 1;
    return 0;
  }
  if (((((*(char *)(param_1 + 0x36) == '\x02') &&
        ((*(uint *)(param_1 + 0x28) & 0xc0000000) == 0x40000000)) || (*(int *)(param_1 + 0xc) == 0))
      || ((*(uint *)(*(int *)(param_1 + 4) + 0x24) & 0x10000) != 0)) &&
     (iVar4 = FUN_00006bcc(param_1,0,0), iVar4 == 0)) {
    do {
      iVar4 = FUN_00006bcc(param_1,0x100,0x40ff8000,0);
      if (iVar4 != 0) {
        return 1;
      }
      uVar5 = *(uint *)(*(int *)(param_1 + 4) + 0x10);
    } while (-1 < (int)uVar5);
    if ((uVar5 & 0x40000000) != 0) {
      *(undefined4 *)(param_1 + 0x30) = 1;
    }
    iVar4 = FUN_00006bcc(param_1,0x200,0);
    if (iVar4 == 0) {
      iVar4 = *(int *)(param_1 + 4);
      uVar1 = *(ushort *)(iVar4 + 0x14);
      uVar2 = *(ushort *)(iVar4 + 0x18);
      uVar3 = *(ushort *)(iVar4 + 0x1c);
      *(uint *)(param_1 + 0x40) = (uint)*(ushort *)(iVar4 + 0x10);
      *(uint *)(param_1 + 0x44) = (uint)uVar1;
      *(uint *)(param_1 + 0x4c) = (uint)uVar3;
      *(uint *)(param_1 + 0x48) = (uint)uVar2;
      *(undefined4 *)(param_1 + 0x50) = 0x12340000;
      iVar4 = FUN_00006bcc(param_1,0x300,0x12340000);
      if ((iVar4 == 0) &&
         (iVar4 = FUN_00006bcc(param_1,0x900,*(undefined4 *)(param_1 + 0x50)), iVar4 == 0)) {
        iVar4 = *(int *)(param_1 + 4);
        uVar6 = *(uint *)(iVar4 + 0x14);
        uVar5 = *(uint *)(iVar4 + 0x18);
        *(char *)(param_1 + 0x35) = (char)((*(uint *)(iVar4 + 0x1c) & 0x3fffff) >> 0x12);
        *(uint *)(param_1 + 100) =
             ((((uVar5 & 3) << 10 | uVar6 >> 0x16) + 1 << ((uVar6 & 0x3ff) >> 7) + 2) <<
             ((uVar5 & 0xfff) >> 8)) >> 9;
        return 0;
      }
    }
  }
  return 1;
}

