//==================== FUN_000082b0 @ 000082b0 ====================

undefined4 FUN_000082b0(int param_1,uint param_2)

{
  ushort uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  int iVar7;
  bool bVar8;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017264,0x2a4);
    DAT_00028e84 = 1;
    return 0;
  }
  puVar5 = &DAT_00028e84;
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 0x24) != 0x11111111) {
    FUN_000014e4(&DAT_00017264,0x2a5);
    DAT_00028e84 = 1;
    return 0;
  }
  iVar7 = *(int *)(param_1 + 4);
  uVar6 = *(undefined4 *)(param_1 + 8);
  *(ushort *)(iVar7 + 0x2c) = *(ushort *)(iVar7 + 0x2c) & 0xfffa;
  bVar8 = *(char *)(param_1 + 0x36) == '\x02';
  if (!bVar8) {
    puVar5 = (undefined4 *)0x9;
  }
  uVar4 = 1;
  if (bVar8) {
    uVar4 = 1;
    do {
      uVar2 = FUN_00001a50(uVar6,uVar4);
      uVar3 = uVar4 & 0xffff;
      uVar4 = uVar4 + 1;
      if (uVar2 <= param_2) {
        *(ushort *)(iVar7 + 0x2c) =
             *(ushort *)(iVar7 + 0x2c) & 0x3f | 1 | (ushort)((uVar3 >> 9) << 6) |
             (ushort)((uVar3 >> 1) << 8);
        goto LAB_0000837c;
      }
    } while (uVar4 != 0x7ff);
  }
  else {
    do {
      uVar2 = FUN_00001a50(uVar6,uVar4);
      if (uVar2 <= param_2) {
        *(ushort *)(iVar7 + 0x2c) =
             *(ushort *)(iVar7 + 0x2c) & 0xff | 1 | (ushort)((uVar4 >> 1) << 8);
LAB_0000837c:
        uVar1 = *(ushort *)(iVar7 + 0x2c);
        while ((uVar1 & 2) == 0) {
          uVar1 = *(ushort *)(iVar7 + 0x2c);
        }
        *(ushort *)(iVar7 + 0x2c) = *(ushort *)(iVar7 + 0x2c) | 4;
        return 0;
      }
      puVar5 = (undefined4 *)((int)puVar5 + -1);
      uVar4 = (uVar4 & 0x7fff) << 1;
    } while (puVar5 != (undefined4 *)0x0);
  }
  return 1;
}

