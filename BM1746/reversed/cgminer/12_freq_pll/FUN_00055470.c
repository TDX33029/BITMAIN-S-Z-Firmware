//==================== FUN_00055470 @ 00055470 ====================

void FUN_00055470(int param_1)

{
  byte *pbVar1;
  byte *pbVar2;
  byte *pbVar3;
  byte *pbVar4;
  uint uVar5;
  uint *puVar6;
  uint uVar7;
  int iVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  int iVar14;
  uint uVar15;
  uint uVar16;
  int iVar17;
  int local_d0;
  uint local_c0 [8];
  uint local_a0 [16];
  uint local_60 [16];
  
  puVar6 = local_c0 + 7;
  iVar8 = param_1;
  do {
    pbVar1 = (byte *)(iVar8 + 0x29);
    pbVar2 = (byte *)(iVar8 + 0x28);
    pbVar3 = (byte *)(iVar8 + 0x2b);
    pbVar4 = (byte *)(iVar8 + 0x2a);
    iVar8 = iVar8 + 4;
    puVar6 = puVar6 + 1;
    *puVar6 = (uint)*pbVar1 * 0x10000 + (uint)*pbVar2 * 0x1000000 + (uint)*pbVar3 +
              (uint)*pbVar4 * 0x100;
  } while (puVar6 != local_a0 + 0xf);
  iVar8 = 0;
  do {
    *(undefined4 *)((int)local_c0 + iVar8) = *(undefined4 *)(param_1 + iVar8);
    iVar8 = iVar8 + 4;
  } while (iVar8 != 0x20);
  local_d0 = 0;
  do {
    iVar8 = 0;
    uVar9 = 7;
    do {
      uVar15 = uVar9 - 1;
      uVar10 = local_c0[uVar9 - 3 & 7];
      uVar16 = local_c0[uVar15 & 7];
      uVar7 = uVar9 & 7;
      uVar5 = local_c0[uVar9 - 2 & 7];
      uVar11 = local_c0[uVar7];
      iVar17 = *(int *)(&DAT_0006c580 + iVar8 + local_d0 * 4);
      if (local_d0 == 0) {
        iVar14 = *(int *)((int)local_a0 + iVar8);
        *(int *)((int)local_60 + iVar8) = iVar14;
      }
      else {
        uVar13 = local_60[5 - uVar9 & 0xf];
        uVar12 = local_60[0xfff8 - uVar9 & 0xf];
        iVar14 = local_60[-uVar9 & 0xf] + *(int *)((int)local_60 + iVar8) +
                 ((uVar13 >> 0x11 | uVar13 << 0xf) ^ (uVar13 >> 0x13 | uVar13 << 0xd) ^ uVar13 >> 10
                 ) + ((uVar12 >> 7 | uVar12 << 0x19) ^ (uVar12 >> 0x12 | uVar12 << 0xe) ^
                     uVar12 >> 3);
        *(int *)((int)local_60 + iVar8) = iVar14;
      }
      uVar12 = uVar9 - 4 & 7;
      uVar5 = uVar11 + iVar17 +
              ((uVar10 >> 6 | uVar10 << 0x1a) ^ (uVar10 >> 0xb | uVar10 << 0x15) ^
              (uVar10 >> 0x19 | uVar10 << 7)) + ((uVar16 ^ uVar5) & uVar10 ^ uVar16) + iVar14;
      local_c0[uVar7] = uVar5;
      local_c0[uVar12] = uVar5 + local_c0[uVar12];
      uVar5 = local_c0[uVar9 - 7 & 7];
      iVar8 = iVar8 + 4;
      local_c0[uVar7] =
           ((uVar5 >> 2 | uVar5 << 0x1e) ^ (uVar5 >> 0xd | uVar5 << 0x13) ^
           (uVar5 >> 0x16 | uVar5 << 10)) +
           ((uVar5 | local_c0[uVar9 - 6 & 7]) & local_c0[uVar9 - 5 & 7] |
           uVar5 & local_c0[uVar9 - 6 & 7]) + local_c0[uVar7];
      uVar9 = uVar15;
    } while (uVar15 != 0xfffffff7);
    local_d0 = local_d0 + 0x10;
  } while (local_d0 != 0x40);
  iVar8 = 0;
  do {
    *(int *)(param_1 + iVar8) = *(int *)(param_1 + iVar8) + *(int *)((int)local_c0 + iVar8);
    iVar8 = iVar8 + 4;
  } while (iVar8 != 0x20);
  return;
}

