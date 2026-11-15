//==================== FUN_000300b0 @ 000300b0 ====================

void FUN_000300b0(int param_1,int param_2,int param_3)

{
  int iVar1;
  uint *puVar2;
  int *piVar3;
  int *piVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  uint *puVar8;
  uint uVar9;
  int iVar10;
  int iVar11;
  uint uVar12;
  int local_154;
  uint local_144;
  uint local_140 [8];
  uint local_120 [64];
  
  if (0 < param_3) {
    local_154 = 0;
    do {
      puVar8 = local_140 + 7;
      iVar5 = param_2 + local_154 * 0x40;
      iVar11 = 0;
      do {
        iVar10 = iVar5 + iVar11 * 4;
        iVar1 = iVar11 * 4;
        iVar11 = iVar11 + 1;
        puVar8 = puVar8 + 1;
        *puVar8 = (uint)*(byte *)(iVar10 + 3) | (uint)*(byte *)(iVar5 + iVar1) << 0x18 |
                  (uint)*(byte *)(iVar10 + 2) << 8 | (uint)*(byte *)(iVar10 + 1) << 0x10;
      } while (iVar11 != 0x10);
      puVar8 = local_120;
      iVar5 = 0;
      do {
        uVar6 = puVar8[0xe];
        uVar9 = puVar8[1];
        piVar3 = (int *)((int)local_120 + iVar5);
        iVar5 = iVar5 + 4;
        puVar8[0x10] = puVar8[9] + *piVar3 +
                       ((uVar6 >> 0x11 | uVar6 << 0xf) ^ (uVar6 >> 0x13 | uVar6 << 0xd) ^
                       uVar6 >> 10) +
                       ((uVar9 >> 7 | uVar9 << 0x19) ^ (uVar9 >> 0x12 | uVar9 << 0xe) ^ uVar9 >> 3);
        puVar8 = puVar8 + 1;
      } while (iVar5 != 0xc0);
      puVar8 = &local_144;
      iVar5 = param_1;
      do {
        puVar2 = (uint *)(iVar5 + 0x88);
        iVar5 = iVar5 + 4;
        puVar8 = puVar8 + 1;
        *puVar8 = *puVar2;
      } while (puVar8 != local_140 + 7);
      iVar5 = 0;
      uVar6 = local_140[5];
      uVar9 = local_140[1];
      while( true ) {
        uVar12 = local_140[6];
        local_140[5] = local_140[4];
        local_140[1] = local_140[0];
        uVar7 = local_140[2];
        piVar3 = (int *)((int)local_120 + iVar5);
        piVar4 = (int *)((int)&DAT_00075fdc + iVar5);
        iVar5 = iVar5 + 4;
        iVar11 = *piVar3 + *piVar4 +
                 ((local_140[5] >> 6 | local_140[5] << 0x1a) ^
                  (local_140[5] >> 0xb | local_140[5] << 0x15) ^
                 (local_140[5] >> 0x19 | local_140[5] << 7)) +
                 (uVar12 & ~local_140[5] ^ uVar6 & local_140[5]) + local_140[7];
        local_140[4] = local_140[3] + iVar11;
        local_140[0] = ((uVar9 ^ uVar7) & local_140[1] ^ uVar7 & uVar9) +
                       ((local_140[1] >> 2 | local_140[1] << 0x1e) ^
                        (local_140[1] >> 0xd | local_140[1] << 0x13) ^
                       (local_140[1] >> 0x16 | local_140[1] << 10)) + iVar11;
        if (iVar5 == 0x100) break;
        local_140[2] = uVar9;
        local_140[6] = uVar6;
        uVar6 = local_140[5];
        local_140[3] = uVar7;
        local_140[7] = uVar12;
        uVar9 = local_140[1];
      }
      local_140[7] = uVar12;
      local_140[6] = uVar6;
      local_140[2] = uVar9;
      local_140[3] = uVar7;
      iVar5 = 0;
      iVar11 = param_1;
      uVar6 = local_140[0];
      while( true ) {
        *(uint *)(iVar11 + 0x88) = *(int *)(iVar11 + 0x88) + uVar6;
        iVar11 = iVar11 + 4;
        if (iVar5 + 4 == 0x20) break;
        uVar6 = *(uint *)((int)local_140 + iVar5 + 4);
        iVar5 = iVar5 + 4;
      }
      local_154 = local_154 + 1;
    } while (param_3 != local_154);
  }
  return;
}

