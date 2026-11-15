//==================== FUN_0004e198 @ 0004e198 ====================

/* WARNING: Restarted to delay deadcode elimination for space: stack */

undefined1 FUN_0004e198(int *param_1,undefined4 param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  void *pvVar7;
  byte *pbVar8;
  int extraout_r1;
  int extraout_r1_00;
  uint *puVar9;
  size_t sVar10;
  byte bVar11;
  undefined1 uVar12;
  size_t sVar13;
  uint uVar14;
  size_t sVar15;
  char *pcVar16;
  int iVar17;
  undefined4 *puVar18;
  int iVar19;
  int iVar20;
  size_t __n;
  uint *puVar21;
  size_t sVar22;
  uint uVar23;
  uint local_1610;
  size_t local_1608;
  size_t local_1604;
  int local_15f8;
  undefined4 *local_15f4;
  undefined4 *local_15ec;
  int local_15dc;
  size_t local_15d8;
  undefined4 local_15cc;
  undefined4 local_15c8;
  undefined4 local_15c4;
  undefined4 local_15c0;
  undefined4 local_15bc;
  undefined4 local_15b8;
  undefined4 local_15b4;
  undefined4 local_15b0;
  undefined2 local_15ac;
  byte bStack_15a9;
  byte local_15a8 [32];
  undefined1 auStack_1588 [31];
  byte bStack_1569;
  byte local_1568 [1344];
  uint local_1028 [512];
  uint local_828 [512];
  uint uStack_28;
  
  uVar14 = param_1[1];
  iVar19 = *param_1;
  iVar1 = param_1[2];
  iVar17 = uVar14 + 1;
  iVar2 = FUN_0005c710(iVar19,iVar17);
  iVar3 = iVar2 + 1 << (uVar14 & 0xff);
  DAT_00076b90 = 3;
  iVar4 = iVar3 + 7;
  if (-1 < iVar3) {
    iVar4 = iVar3;
  }
  FUN_0004df94(param_2,iVar4 >> 3,local_1028,0x800,iVar2 + 1,1);
  local_15c8 = 0;
  local_15c4 = 0;
  local_15c0 = 0;
  local_15bc = 0;
  local_15b8 = 0;
  local_15b4 = 0;
  local_15b0 = 0;
  local_15ac = 0;
  local_15f4 = malloc(0x4400);
  local_15ec = malloc(0x4400);
  puVar9 = local_1028;
  local_15a8[0] = 0;
  local_15a8[1] = 0;
  local_15a8[2] = 0;
  local_15a8[3] = 0;
  local_15a8[4] = 0;
  local_15a8[5] = 0;
  local_15a8[6] = 0;
  local_15a8[7] = 0;
  local_15a8[8] = 0;
  local_15a8[9] = 0;
  local_15a8[10] = 0;
  local_15a8[0xb] = 0;
  local_15a8[0xc] = 0;
  local_15a8[0xd] = 0;
  local_15a8[0xe] = 0;
  local_15a8[0xf] = 0;
  local_15a8[0x10] = 0;
  local_15a8[0x11] = 0;
  local_15a8[0x12] = 0;
  local_15a8[0x13] = 0;
  local_15a8[0x14] = 0;
  local_15a8[0x15] = 0;
  local_15a8[0x16] = 0;
  local_15a8[0x17] = 0;
  local_15a8[0x18] = 0;
  local_15a8[0x19] = 0;
  local_15a8[0x1a] = 0;
  local_15a8[0x1b] = 0;
  local_15a8[0x1c] = 0;
  local_15a8[0x1d] = 0;
  iVar3 = 0;
  while (iVar3 = iVar3 + 1, iVar3 != 0x200) {
    if (*puVar9 == puVar9[1]) {
LAB_0004e488:
      uVar12 = 1;
      goto LAB_0004e430;
    }
    puVar21 = local_1028 + iVar3;
    iVar4 = iVar3;
    while (iVar4 = iVar4 + 1, iVar4 != 0x200) {
      puVar21 = puVar21 + 1;
      if (*puVar9 == *puVar21) goto LAB_0004e488;
    }
    puVar9 = puVar9 + 1;
  }
  bVar11 = 0;
  iVar3 = iVar2 + 7;
  if (iVar3 < 0) {
    iVar3 = iVar2 + 0xe;
  }
  __n = (iVar3 >> 3) * iVar17;
  iVar4 = FUN_0005c710(0x200,iVar19);
  iVar3 = iVar19 + 7;
  if (-1 < iVar19) {
    iVar3 = iVar19;
  }
  iVar17 = iVar19 * iVar4;
  if (iVar17 < 0) {
    iVar17 = iVar17 + 7;
  }
  iVar20 = 0;
  puVar18 = local_15f4;
  do {
    uVar14 = local_1028[iVar20];
    uVar14 = uVar14 << 0x18 | (uVar14 >> 8 & 0xff) << 0x10 | (uVar14 >> 0x10 & 0xff) << 8 |
             uVar14 >> 0x18;
    uVar5 = FUN_0005c710(uVar14,iVar4);
    FUN_0004df30(iVar1,uVar5,local_828,iVar17 >> 3);
    FUN_0005c7ec(uVar14,iVar4);
    iVar6 = iVar19 * extraout_r1;
    if (iVar6 < 0) {
      iVar6 = iVar6 + 7;
    }
    FUN_0004df94((int)local_828 + (iVar6 >> 3),iVar3 >> 3,local_1568,__n,iVar2,0);
    if (__n != 0) {
      sVar22 = 0;
      while( true ) {
        local_15a8[sVar22 - 0x20] = bVar11 ^ local_1568[sVar22];
        if (__n == sVar22 + 1) break;
        bVar11 = local_15a8[sVar22 - 0x1f];
        sVar22 = sVar22 + 1;
      }
      bVar11 = (byte)local_15c8;
    }
    iVar6 = iVar20 + 1;
    memcpy(puVar18,local_1568,__n);
    *(uint *)(__n + (int)puVar18) = local_1028[iVar20];
    puVar18 = (undefined4 *)((int)puVar18 + __n + 4);
    iVar20 = iVar6;
  } while (iVar6 != 0x200);
  pbVar8 = (byte *)((int)&local_15c8 + 1);
  while( true ) {
    if (bVar11 != 0) goto code_r0x0004e42c;
    if (pbVar8 + 1 == &bStack_15a9) break;
    bVar11 = *pbVar8;
    pbVar8 = pbVar8 + 1;
  }
  local_15d8 = 4;
  local_15dc = 9;
  local_1608 = 0x200;
  local_15ac = 0;
  local_15c8 = 0;
  local_15c4 = 0;
  local_15c0 = 0;
  local_15bc = 0;
  local_15b8 = 0;
  local_15b4 = 0;
  local_15b0 = 0;
  while( true ) {
    puVar18 = local_15f4;
    sVar22 = local_15d8 + __n;
    qsort(local_15f4,local_1608,sVar22,(__compar_fn_t)&LAB_0004df84);
    if (local_1608 == 0) {
      sVar15 = local_1608;
      sVar10 = local_15d8 << 1;
    }
    else {
      uVar14 = 0;
      sVar15 = 0;
      sVar13 = sVar22;
      do {
        if (__n != 0) {
          sVar10 = 0;
          do {
            local_15a8[sVar10] =
                 *(byte *)((int)local_15f4 + sVar10 + sVar13) ^
                 *(byte *)((int)local_15f4 + sVar10 + sVar22 * uVar14);
            sVar10 = sVar10 + 1;
          } while (__n != sVar10);
        }
        iVar20 = (local_15d8 * 2 + __n) * ((int)uVar14 >> 1);
        sVar15 = sVar15 + 1;
        iVar6 = iVar20 + __n;
        memcpy((undefined4 *)((int)local_15ec + iVar20),local_15a8,__n);
        memcpy((void *)((int)local_15ec + iVar6),(void *)((int)local_15f4 + sVar22 * uVar14 + __n),
               local_15d8);
        iVar20 = sVar13 + __n;
        sVar13 = sVar13 + sVar22 * 2;
        memcpy((void *)((int)local_15ec + iVar6 + local_15d8),(void *)((int)local_15f4 + iVar20),
               local_15d8);
        uVar14 = sVar15 * 2;
        sVar10 = local_15d8 * 2;
      } while (uVar14 < local_1608);
    }
    local_15d8 = sVar10;
    local_15dc = local_15dc + -1;
    DAT_00076b90 = DAT_00076b90 + 3;
    if (local_15dc == 0) break;
    local_15f4 = local_15ec;
    local_15ec = puVar18;
    local_1608 = sVar15;
  }
  iVar20 = 0;
  do {
    uVar14 = *(uint *)((int)local_15ec + iVar20 + __n);
    *(uint *)((int)local_828 + iVar20) =
         uVar14 << 0x18 | (uVar14 >> 8 & 0xff) << 0x10 | (uVar14 >> 0x10 & 0xff) << 8 |
         uVar14 >> 0x18;
    iVar20 = iVar20 + 4;
  } while (iVar20 != 0x800);
  uVar14 = 0;
  do {
    iVar20 = 0;
    do {
      puVar9 = local_828 + iVar20;
      iVar20 = iVar20 + (2 << (uVar14 & 0xff));
      FUN_0004e134(puVar9,1 << (uVar14 & 0xff));
    } while (iVar20 < 0x200);
    uVar14 = uVar14 + 1;
  } while (uVar14 != 9);
  puVar9 = local_828;
  puVar18 = local_15ec;
  do {
    uVar14 = *puVar9;
    uVar5 = FUN_0005c710(uVar14,iVar4);
    FUN_0004df30(iVar1,uVar5,local_1568,iVar17 >> 3);
    FUN_0005c7ec(uVar14,iVar4);
    iVar20 = iVar19 * extraout_r1_00;
    if (iVar20 < 0) {
      iVar20 = iVar20 + 7;
    }
    FUN_0004df94(local_1568 + (iVar20 >> 3),iVar3 >> 3,auStack_1588,__n,iVar2,0);
    puVar9 = puVar9 + 1;
    memcpy(puVar18,auStack_1588,__n);
    *(undefined4 *)(__n + (int)puVar18) = local_15cc;
    pvVar7 = (void *)FUN_00029730(puVar18,0x22);
    puVar18 = (undefined4 *)((int)puVar18 + __n + 4);
    free(pvVar7);
  } while (puVar9 != &uStack_28);
  local_1604 = 4;
  local_15f8 = 9;
  local_1610 = 0x200;
  while( true ) {
    puVar18 = local_15ec;
    if (local_1610 == 0) {
      uVar14 = local_1610;
      sVar22 = local_1604 << 1;
    }
    else {
      iVar4 = local_1604 + __n;
      uVar23 = 0;
      uVar14 = 0;
      iVar3 = iVar4;
      do {
        if (__n != 0) {
          sVar22 = 0;
          do {
            local_15a8[sVar22] =
                 *(byte *)((int)local_15ec + sVar22 + iVar3) ^
                 *(byte *)((int)local_15ec + sVar22 + iVar4 * uVar23);
            sVar22 = sVar22 + 1;
          } while (__n != sVar22);
        }
        iVar1 = (local_1604 * 2 + __n) * ((int)uVar23 >> 1);
        uVar14 = uVar14 + 1;
        iVar2 = iVar1 + __n;
        memcpy((undefined4 *)((int)local_15f4 + iVar1),local_15a8,__n);
        memcpy((void *)((int)local_15f4 + iVar2),(void *)((int)local_15ec + iVar4 * uVar23 + __n),
               local_1604);
        iVar1 = iVar3 + __n;
        iVar3 = iVar3 + iVar4 * 2;
        memcpy((void *)((int)local_15f4 + iVar2 + local_1604),(void *)((int)local_15ec + iVar1),
               local_1604);
        uVar23 = uVar14 * 2;
        sVar22 = local_1604 * 2;
      } while (uVar23 < local_1610);
    }
    local_1604 = sVar22;
    local_15f8 = local_15f8 + -1;
    DAT_00076b90 = DAT_00076b90 + 3;
    if (local_15f8 == 0) break;
    local_15ec = local_15f4;
    local_15f4 = puVar18;
    local_1610 = uVar14;
  }
  pbVar8 = &bStack_1569;
  uVar14 = 0;
  iVar3 = 0;
  uVar23 = 0;
  do {
    iVar4 = iVar3 + __n;
    if (uVar23 < 8) {
      iVar3 = iVar3 + 4;
      uVar23 = uVar23 + 0x15;
      uVar14 = (*(byte *)((int)local_15f4 + iVar4 + 1) & 0x1f) << 0x10 |
               (uint)*(byte *)((int)local_15f4 + iVar4 + 2) << 8 | uVar14 << 0x15 |
               (uint)*(byte *)((int)local_15f4 + iVar4 + 3);
    }
    uVar23 = uVar23 - 8;
    pbVar8 = pbVar8 + 1;
    *pbVar8 = (byte)(uVar14 >> (uVar23 & 0xff));
  } while (pbVar8 != local_1568 + 0x53f);
  pvVar7 = (void *)FUN_00029730(local_1568,0x540);
  pcVar16 = (char *)((int)&local_15cc + 3);
  free(pvVar7);
  pvVar7 = (void *)FUN_00029730(local_15f4,local_1604 + __n);
  free(pvVar7);
  local_15c8 = *local_15f4;
  local_15c4 = local_15f4[1];
  local_15c0 = local_15f4[2];
  local_15bc = local_15f4[3];
  local_15b8 = local_15f4[4];
  local_15b4 = local_15f4[5];
  local_15b0 = local_15f4[6];
  local_15ac = (short)local_15f4[7];
  while (pcVar16 = pcVar16 + 1, *pcVar16 == '\0') {
    uVar12 = 0;
    if (pcVar16 == (char *)((int)&local_15ac + 1)) goto LAB_0004e430;
  }
  uVar12 = 3;
LAB_0004e430:
  free(local_15f4);
  free(local_15ec);
  return uVar12;
code_r0x0004e42c:
  uVar12 = 2;
  goto LAB_0004e430;
}

