//==================== FUN_00028008 @ 00028008 ====================

undefined4 FUN_00028008(int param_1)

{
  byte *pbVar1;
  byte *pbVar2;
  byte *pbVar3;
  byte *pbVar4;
  byte *pbVar5;
  byte *pbVar6;
  byte *pbVar7;
  byte bVar8;
  size_t sVar9;
  byte *pbVar10;
  undefined4 uVar11;
  time_t tVar12;
  void *pvVar13;
  byte *__s;
  uint uVar14;
  uint uVar15;
  int iVar16;
  byte *pbVar17;
  uint uVar18;
  int iVar19;
  int iVar20;
  byte *pbVar21;
  undefined4 *puVar22;
  int *piVar23;
  uint uVar24;
  uint uVar25;
  int iVar26;
  uint local_2c;
  
  __s = DAT_00078728;
  if (DAT_00078728 == (byte *)0x0) {
    __s = malloc(0x28);
    uVar11 = **(undefined4 **)(*(int *)(param_1 + 4) + 8);
    __s[4] = 0;
    __s[5] = 0;
    __s[6] = 0;
    __s[7] = 0;
    *(undefined4 *)__s = uVar11;
    *(undefined4 *)(param_1 + 8) = 0;
    __s[0x10] = 0;
    __s[0x11] = 0;
    __s[0x12] = 0;
    __s[0x13] = 0;
    *(byte **)(__s + 0x1c) = __s;
    sVar9 = strlen((char *)__s);
    __s[0xc] = 0;
    __s[0xd] = 0;
    __s[0xe] = 0;
    __s[0xf] = 0;
    DAT_00078728 = __s;
    *(size_t *)(__s + 0x20) = sVar9;
    puVar22 = malloc(0x2c);
    *(undefined4 **)(__s + 8) = puVar22;
    if (puVar22 == (undefined4 *)0x0) goto LAB_000289d4;
    *puVar22 = 0;
    puVar22[3] = 0;
    pbVar17 = __s + 8;
    puVar22[4] = 0;
    puVar22[5] = 0;
    puVar22[6] = 0;
    puVar22[7] = 0;
    puVar22[8] = 0;
    puVar22[9] = 0;
    puVar22[10] = 0;
    puVar22[4] = pbVar17;
    puVar22[1] = 0x20;
    puVar22[2] = 5;
    puVar22[5] = 8;
    pvVar13 = malloc(0x180);
    *puVar22 = pvVar13;
    if (pvVar13 == (void *)0x0) goto LAB_000289d4;
    memset(pvVar13,0,0x180);
    puVar22[10] = 0xa0111fe1;
    pbVar10 = __s;
  }
  else {
    pbVar21 = *(byte **)(*(int *)(param_1 + 4) + 8);
    sVar9 = strlen((char *)pbVar21);
    uVar25 = 0x9e3779b9;
    uVar14 = 0xfeedbeef;
    uVar24 = 0x9e3779b9;
    pbVar17 = pbVar21;
    for (uVar15 = sVar9; 0xb < uVar15; uVar15 = uVar15 - 0xc) {
      pbVar10 = pbVar17 + 2;
      pbVar1 = pbVar17 + 1;
      pbVar2 = pbVar17 + 10;
      pbVar3 = pbVar17 + 9;
      bVar8 = *pbVar17;
      iVar26 = (uint)pbVar17[6] * 0x10000 + (uint)pbVar17[5] * 0x100 + (uint)pbVar17[4] +
               (uint)pbVar17[7] * 0x1000000 + uVar25;
      pbVar4 = pbVar17 + 8;
      pbVar5 = pbVar17 + 3;
      pbVar6 = pbVar17 + 0xb;
      pbVar17 = pbVar17 + 0xc;
      uVar14 = (uint)*pbVar2 * 0x10000 + (uint)*pbVar3 * 0x100 + (uint)*pbVar4 +
               (uint)*pbVar6 * 0x1000000 + uVar14;
      uVar24 = ((((uint)*pbVar10 * 0x10000 + (uint)*pbVar1 * 0x100 + (uint)bVar8 +
                 (uint)*pbVar5 * 0x1000000) - iVar26) - uVar14) + uVar24 ^ uVar14 >> 0xd;
      uVar25 = (iVar26 - uVar14) - uVar24 ^ uVar24 << 8;
      uVar14 = (uVar14 - uVar24) - uVar25 ^ uVar25 >> 0xd;
      uVar24 = (uVar24 - uVar25) - uVar14 ^ uVar14 >> 0xc;
      uVar25 = (uVar25 - uVar14) - uVar24 ^ uVar24 << 0x10;
      uVar14 = (uVar14 - uVar24) - uVar25 ^ uVar25 >> 5;
      uVar24 = (uVar24 - uVar25) - uVar14 ^ uVar14 >> 3;
      uVar25 = (uVar25 - uVar14) - uVar24 ^ uVar24 << 10;
      uVar14 = (uVar14 - uVar24) - uVar25 ^ uVar25 >> 0xf;
    }
    uVar14 = uVar14 + sVar9;
    switch(uVar15) {
    case 0xb:
      uVar14 = uVar14 + (uint)pbVar17[10] * 0x1000000;
    case 10:
      uVar14 = uVar14 + (uint)pbVar17[9] * 0x10000;
    case 9:
      uVar14 = uVar14 + (uint)pbVar17[8] * 0x100;
    case 8:
      uVar25 = uVar25 + (uint)pbVar17[7] * 0x1000000;
    case 7:
      uVar25 = uVar25 + (uint)pbVar17[6] * 0x10000;
    case 6:
      uVar25 = uVar25 + (uint)pbVar17[5] * 0x100;
    case 5:
      uVar25 = uVar25 + pbVar17[4];
    case 4:
      uVar24 = uVar24 + (uint)pbVar17[3] * 0x1000000;
    case 3:
      uVar24 = uVar24 + (uint)pbVar17[2] * 0x10000;
    case 2:
      uVar24 = uVar24 + (uint)pbVar17[1] * 0x100;
    case 1:
      uVar24 = uVar24 + *pbVar17;
    default:
      piVar23 = *(int **)(__s + 8);
      uVar24 = (uVar24 - uVar25) - uVar14 ^ uVar14 >> 0xd;
      uVar15 = (uVar25 - uVar14) - uVar24 ^ uVar24 << 8;
      uVar18 = (uVar14 - uVar24) - uVar15 ^ uVar15 >> 0xd;
      uVar25 = (uVar24 - uVar15) - uVar18 ^ uVar18 >> 0xc;
      uVar14 = (uVar15 - uVar18) - uVar25 ^ uVar25 << 0x10;
      uVar24 = (uVar18 - uVar25) - uVar14 ^ uVar14 >> 5;
      uVar25 = (uVar25 - uVar14) - uVar24 ^ uVar24 >> 3;
      uVar14 = (uVar14 - uVar24) - uVar25 ^ uVar25 << 10;
      iVar26 = *(int *)(*piVar23 +
                       (((uVar24 - uVar25) - uVar14 ^ uVar14 >> 0xf) & piVar23[1] - 1U) * 0xc);
      if (iVar26 != 0) {
        iVar26 = iVar26 - piVar23[5];
      }
    }
    for (; iVar26 != 0; iVar26 = *(int *)(iVar26 + 0x18) - piVar23[5]) {
      if ((sVar9 == *(size_t *)(iVar26 + 0x20)) &&
         (iVar16 = memcmp(*(void **)(iVar26 + 0x1c),pbVar21,sVar9), iVar16 == 0)) {
        iVar16 = *(int *)(iVar26 + 4) + 1;
        *(int *)(iVar26 + 4) = iVar16;
        *(int *)(param_1 + 8) = iVar16;
        goto LAB_000284a8;
      }
      if (*(int *)(iVar26 + 0x18) == 0) break;
    }
    pbVar10 = malloc(0x28);
    uVar11 = *(undefined4 *)pbVar21;
    pbVar10[4] = 0;
    pbVar10[5] = 0;
    pbVar10[6] = 0;
    pbVar10[7] = 0;
    pbVar17 = pbVar10 + 8;
    *(undefined4 *)pbVar10 = uVar11;
    *(undefined4 *)(param_1 + 8) = 0;
    pbVar10[0x10] = 0;
    pbVar10[0x11] = 0;
    pbVar10[0x12] = 0;
    pbVar10[0x13] = 0;
    *(byte **)(pbVar10 + 0x1c) = pbVar10;
    sVar9 = strlen((char *)pbVar10);
    *(size_t *)(pbVar10 + 0x20) = sVar9;
    *(byte **)(*(int *)(*(int *)(__s + 8) + 0x10) + 8) = pbVar10;
    *(int *)(pbVar10 + 0xc) =
         *(int *)(*(int *)(__s + 8) + 0x10) - *(int *)(*(int *)(__s + 8) + 0x14);
    *(byte **)(*(int *)(__s + 8) + 0x10) = pbVar17;
    puVar22 = *(undefined4 **)(__s + 8);
  }
  uVar14 = 0xfeedbeef;
  puVar22[3] = puVar22[3] + 1;
  uVar11 = *(undefined4 *)(__s + 8);
  pbVar10[0x24] = 0xef;
  pbVar10[0x25] = 0xbe;
  pbVar10[0x26] = 0xed;
  pbVar10[0x27] = 0xfe;
  uVar24 = 0x9e3779b9;
  *(undefined4 *)(pbVar10 + 8) = uVar11;
  sVar9 = strlen((char *)pbVar10);
  pbVar21 = pbVar10;
  uVar25 = sVar9;
  if (sVar9 < 0xc) {
    uVar15 = 0x9e3779b9;
  }
  else {
    uVar15 = 0x9e3779b9;
    do {
      uVar25 = uVar25 - 0xc;
      pbVar1 = pbVar21 + 2;
      pbVar2 = pbVar21 + 1;
      pbVar3 = pbVar21 + 6;
      pbVar4 = pbVar21 + 5;
      bVar8 = *pbVar21;
      pbVar5 = pbVar21 + 4;
      pbVar6 = pbVar21 + 3;
      pbVar7 = pbVar21 + 7;
      uVar14 = (uint)pbVar21[10] * 0x10000 + (uint)pbVar21[9] * 0x100 + (uint)pbVar21[8] +
               (uint)pbVar21[0xb] * 0x1000000 + uVar14;
      pbVar21 = pbVar21 + 0xc;
      iVar26 = (uint)*pbVar3 * 0x10000 + (uint)*pbVar4 * 0x100 + (uint)*pbVar5 +
               (uint)*pbVar7 * 0x1000000 + uVar24;
      uVar18 = ((((uint)*pbVar1 * 0x10000 + (uint)*pbVar2 * 0x100 + (uint)bVar8 +
                 (uint)*pbVar6 * 0x1000000) - uVar14) - iVar26) + uVar15 ^ uVar14 >> 0xd;
      uVar24 = (iVar26 - uVar14) - uVar18 ^ uVar18 << 8;
      uVar15 = (uVar14 - uVar18) - uVar24 ^ uVar24 >> 0xd;
      uVar14 = (uVar18 - uVar24) - uVar15 ^ uVar15 >> 0xc;
      uVar24 = (uVar24 - uVar15) - uVar14 ^ uVar14 << 0x10;
      uVar18 = (uVar15 - uVar14) - uVar24 ^ uVar24 >> 5;
      uVar15 = (uVar14 - uVar24) - uVar18 ^ uVar18 >> 3;
      uVar24 = (uVar24 - uVar18) - uVar15 ^ uVar15 << 10;
      uVar14 = (uVar18 - uVar15) - uVar24 ^ uVar24 >> 0xf;
      *(uint *)(pbVar10 + 0x24) = uVar14;
    } while (0xb < uVar25);
    sVar9 = strlen((char *)pbVar10);
  }
  uVar14 = sVar9 + uVar14;
  *(uint *)(pbVar10 + 0x24) = uVar14;
  switch(uVar25) {
  case 0xb:
    uVar14 = uVar14 + (uint)pbVar21[10] * 0x1000000;
    *(uint *)(pbVar10 + 0x24) = uVar14;
  case 10:
    uVar14 = uVar14 + (uint)pbVar21[9] * 0x10000;
    *(uint *)(pbVar10 + 0x24) = uVar14;
  case 9:
    uVar14 = uVar14 + (uint)pbVar21[8] * 0x100;
    *(uint *)(pbVar10 + 0x24) = uVar14;
  case 8:
    uVar24 = uVar24 + (uint)pbVar21[7] * 0x1000000;
  case 7:
    uVar24 = uVar24 + (uint)pbVar21[6] * 0x10000;
  case 6:
    uVar24 = uVar24 + (uint)pbVar21[5] * 0x100;
  case 5:
    uVar24 = uVar24 + pbVar21[4];
  case 4:
    uVar15 = uVar15 + (uint)pbVar21[3] * 0x1000000;
  case 3:
    uVar15 = uVar15 + (uint)pbVar21[2] * 0x10000;
  case 2:
    uVar15 = uVar15 + (uint)pbVar21[1] * 0x100;
  case 1:
    uVar15 = uVar15 + *pbVar21;
  }
  uVar25 = (uVar15 - uVar24) - uVar14 ^ uVar14 >> 0xd;
  uVar24 = (uVar24 - uVar14) - uVar25 ^ uVar25 << 8;
  uVar15 = (uVar14 - uVar25) - uVar24 ^ uVar24 >> 0xd;
  uVar25 = (uVar25 - uVar24) - uVar15 ^ uVar15 >> 0xc;
  uVar14 = (uVar24 - uVar15) - uVar25 ^ uVar25 << 0x10;
  uVar24 = (uVar15 - uVar25) - uVar14 ^ uVar14 >> 5;
  uVar25 = (uVar25 - uVar14) - uVar24 ^ uVar24 >> 3;
  uVar14 = (uVar14 - uVar24) - uVar25 ^ uVar25 << 10;
  uVar25 = (uVar24 - uVar25) - uVar14 ^ uVar14 >> 0xf;
  *(uint *)(pbVar10 + 0x24) = uVar25;
  iVar26 = (uVar25 & (*(int **)(__s + 8))[1] - 1U) * 0xc;
  iVar16 = **(int **)(__s + 8) + iVar26;
  *(int *)(iVar16 + 4) = *(int *)(iVar16 + 4) + 1;
  uVar11 = *(undefined4 *)(**(int **)(__s + 8) + iVar26);
  pbVar10[0x14] = 0;
  pbVar10[0x15] = 0;
  pbVar10[0x16] = 0;
  pbVar10[0x17] = 0;
  *(undefined4 *)(pbVar10 + 0x18) = uVar11;
  iVar16 = **(int **)(__s + 8);
  if (*(int *)(iVar16 + iVar26) != 0) {
    *(byte **)(*(int *)(iVar16 + iVar26) + 0xc) = pbVar17;
    iVar16 = **(int **)(__s + 8);
  }
  *(byte **)(iVar16 + iVar26) = pbVar17;
  if (((uint)((*(int *)(**(int **)(__s + 8) + iVar26 + 8) + 1) * 10) <=
       *(uint *)(**(int **)(__s + 8) + iVar26 + 4)) &&
     (iVar26 = *(int *)(pbVar10 + 8), *(int *)(iVar26 + 0x24) != 1)) {
    sVar9 = *(int *)(iVar26 + 4) * 0x18;
    pvVar13 = malloc(sVar9);
    if (pvVar13 == (void *)0x0) {
LAB_000289d4:
                    /* WARNING: Subroutine does not return */
      exit(-1);
    }
    memset(pvVar13,0,sVar9);
    iVar16 = 0;
    *(uint *)(iVar26 + 0x18) =
         (uint)((*(int *)(iVar26 + 4) * 2 - 1U & *(uint *)(iVar26 + 0xc)) != 0) +
         (*(uint *)(iVar26 + 0xc) >> (*(int *)(iVar26 + 8) + 1U & 0xff));
    *(undefined4 *)(*(int *)(pbVar10 + 8) + 0x1c) = 0;
    piVar23 = *(int **)(pbVar10 + 8);
    uVar25 = piVar23[1];
    if (uVar25 != 0) {
      local_2c = 0;
      do {
        iVar26 = *(int *)(*piVar23 + iVar16);
        if (*(int *)(*piVar23 + iVar16) != 0) {
          while( true ) {
            iVar20 = *(int *)(iVar26 + 0x10);
            uVar14 = uVar25 * 2 - 1 & *(uint *)(iVar26 + 0x1c);
            uVar25 = *(int *)((int)pvVar13 + uVar14 * 0xc + 4) + 1;
            *(uint *)((int)pvVar13 + uVar14 * 0xc + 4) = uVar25;
            if ((uint)piVar23[6] < uVar25) {
              piVar23[7] = piVar23[7] + 1;
              uVar11 = FUN_0005c648(*(undefined4 *)((int)pvVar13 + uVar14 * 0xc + 4),
                                    *(undefined4 *)(*(int *)(pbVar10 + 8) + 0x18));
              *(undefined4 *)((int)pvVar13 + uVar14 * 0xc + 8) = uVar11;
            }
            *(undefined4 *)(iVar26 + 0xc) = 0;
            *(undefined4 *)(iVar26 + 0x10) = *(undefined4 *)((int)pvVar13 + uVar14 * 0xc);
            iVar19 = *(int *)((int)pvVar13 + uVar14 * 0xc);
            if (iVar19 != 0) {
              *(int *)(iVar19 + 0xc) = iVar26;
            }
            *(int *)((int)pvVar13 + uVar14 * 0xc) = iVar26;
            piVar23 = *(int **)(pbVar10 + 8);
            if (iVar20 == 0) break;
            uVar25 = piVar23[1];
            iVar26 = iVar20;
          }
          uVar25 = piVar23[1];
        }
        iVar16 = iVar16 + 0xc;
        local_2c = local_2c + 1;
      } while (local_2c < uVar25);
    }
    free((void *)*piVar23);
    *(int *)(*(int *)(pbVar10 + 8) + 4) = *(int *)(*(int *)(pbVar10 + 8) + 4) << 1;
    *(int *)(*(int *)(pbVar10 + 8) + 8) = *(int *)(*(int *)(pbVar10 + 8) + 8) + 1;
    **(undefined4 **)(pbVar10 + 8) = pvVar13;
    iVar26 = *(int *)(pbVar10 + 8);
    if (*(uint *)(iVar26 + 0xc) >> 1 < *(uint *)(iVar26 + 0x1c)) {
      iVar16 = *(int *)(iVar26 + 0x20) + 1;
    }
    else {
      iVar16 = 0;
    }
    *(int *)(iVar26 + 0x20) = iVar16;
    if (1 < *(uint *)(*(int *)(pbVar10 + 8) + 0x20)) {
      *(undefined4 *)(*(int *)(pbVar10 + 8) + 0x24) = 1;
    }
  }
LAB_000284a8:
  iVar26 = pthread_rwlock_wrlock((pthread_rwlock_t *)&DAT_0007b700);
  if (iVar26 != 0) {
    FUN_0000b480("add_cgpu",0x28cc);
  }
  DAT_0007b628 = realloc(DAT_0007b628,(DAT_0007b720 + DAT_0007872c + 2) * 4);
  iVar26 = pthread_rwlock_unlock((pthread_rwlock_t *)&DAT_0007b700);
  if (iVar26 != 0) {
    FUN_0000b630("add_cgpu",0x28ce);
  }
  (*DAT_00075ed8)();
  iVar26 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007ad4c);
  if (iVar26 != 0) {
    FUN_0000b414("add_cgpu",0x28d0);
  }
  tVar12 = time((time_t *)0x0);
  *(time_t *)(param_1 + 0xe8) = tVar12;
  iVar26 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007ad4c);
  if (iVar26 != 0) {
    FUN_0000b5c4("add_cgpu",0x28d2);
  }
  (*DAT_00075ed8)();
  if (DAT_0007b7d0 == '\0') {
    *(int *)((int)DAT_0007b628 + DAT_0007b720 * 4) = param_1;
    DAT_0007b720 = DAT_0007b720 + 1;
  }
  else {
    iVar26 = DAT_0007872c + DAT_0007b720;
    DAT_0007872c = DAT_0007872c + 1;
    *(int *)((int)DAT_0007b628 + iVar26 * 4) = param_1;
  }
  if (DAT_0007702c < DAT_0007b720 - DAT_0007b8b0) {
    DAT_0007702c = DAT_0007b720 - DAT_0007b8b0;
  }
  return 1;
}

