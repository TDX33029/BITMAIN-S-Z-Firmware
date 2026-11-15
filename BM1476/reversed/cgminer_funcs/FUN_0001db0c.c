//==================== FUN_0001db0c @ 0001db0c ====================

undefined4 FUN_0001db0c(int param_1)

{
  byte bVar1;
  int iVar2;
  size_t sVar3;
  uint uVar4;
  byte *pbVar5;
  byte *pbVar6;
  undefined4 *puVar7;
  void *pvVar8;
  tm *ptVar9;
  uint uVar10;
  byte *pbVar11;
  byte *pbVar12;
  uint uVar13;
  byte *pbVar14;
  byte *pbVar15;
  int iVar16;
  char *pcVar17;
  undefined4 uVar18;
  byte *pbVar19;
  int *piVar20;
  int *piVar21;
  byte *pbVar22;
  int iVar23;
  byte *pbVar24;
  int iVar25;
  byte *pbVar26;
  uint uVar27;
  uint uVar28;
  byte *pbVar29;
  uint in_fpscr;
  double dVar30;
  undefined8 uVar31;
  double dVar32;
  ulonglong in_d17;
  uint local_10a8;
  byte *local_10a0;
  time_t tStack_1098;
  int local_1094;
  int local_1090;
  int local_108c;
  int local_1088;
  int local_1084;
  int local_1080;
  int local_107c;
  int local_1078;
  byte local_1074 [68];
  char local_1030 [4];
  char acStack_102c [4];
  char acStack_1028 [4];
  char acStack_1024 [4];
  char local_1020 [4];
  undefined2 local_101c [2];
  char acStack_1018 [4];
  undefined2 local_1014 [2];
  char local_1010 [4];
  char acStack_100c [4];
  char acStack_1008 [4];
  char acStack_1004 [4];
  char acStack_1000 [4];
  undefined2 uStack_ffc;
  undefined1 local_ffa;
  
  piVar21 = *(int **)(param_1 + 0x6d4);
  if (*(char *)(param_1 + 0x6ea) == '\0') {
    local_1094 = *(int *)(param_1 + 0x20);
    local_1090 = *(int *)(param_1 + 0x1c);
    local_108c = *(int *)(param_1 + 0x18);
    local_1088 = *(int *)(param_1 + 0x14);
    local_1080 = *(int *)(param_1 + 0xc);
    local_1084 = *(int *)(param_1 + 0x10);
    local_1078 = *(int *)(param_1 + 4);
    local_107c = *(int *)(param_1 + 8);
    FUN_000296d8(local_1074,&local_1094,0x20);
    iVar2 = pthread_rwlock_rdlock((pthread_rwlock_t *)&DAT_00077294);
    if (iVar2 != 0) {
      FUN_0000b69c("block_exists",0x14e3);
    }
    pbVar5 = DAT_000772b4;
    iVar2 = 0;
    if (DAT_000772b4 != (byte *)0x0) {
      sVar3 = strlen((char *)local_1074);
      uVar27 = 0x9e3779b9;
      pbVar14 = local_1074;
      uVar10 = 0x9e3779b9;
      uVar4 = 0xfeedbeef;
      for (uVar13 = sVar3; 0xb < uVar13; uVar13 = uVar13 - 0xc) {
        pbVar24 = pbVar14 + 2;
        pbVar22 = pbVar14 + 1;
        pbVar29 = pbVar14 + 10;
        pbVar11 = pbVar14 + 9;
        bVar1 = *pbVar14;
        iVar2 = (uint)pbVar14[6] * 0x10000 + (uint)pbVar14[5] * 0x100 + (uint)pbVar14[4] +
                (uint)pbVar14[7] * 0x1000000 + uVar27;
        pbVar15 = pbVar14 + 8;
        pbVar19 = pbVar14 + 3;
        pbVar6 = pbVar14 + 0xb;
        pbVar14 = pbVar14 + 0xc;
        uVar4 = (uint)*pbVar29 * 0x10000 + (uint)*pbVar11 * 0x100 + (uint)*pbVar15 +
                (uint)*pbVar6 * 0x1000000 + uVar4;
        uVar10 = ((((uint)*pbVar24 * 0x10000 + (uint)*pbVar22 * 0x100 + (uint)bVar1 +
                   (uint)*pbVar19 * 0x1000000) - iVar2) - uVar4) + uVar10 ^ uVar4 >> 0xd;
        uVar28 = (iVar2 - uVar4) - uVar10 ^ uVar10 << 8;
        uVar27 = (uVar4 - uVar10) - uVar28 ^ uVar28 >> 0xd;
        uVar10 = (uVar10 - uVar28) - uVar27 ^ uVar27 >> 0xc;
        uVar28 = (uVar28 - uVar27) - uVar10 ^ uVar10 << 0x10;
        uVar4 = (uVar27 - uVar10) - uVar28 ^ uVar28 >> 5;
        uVar10 = (uVar10 - uVar28) - uVar4 ^ uVar4 >> 3;
        uVar27 = (uVar28 - uVar4) - uVar10 ^ uVar10 << 10;
        uVar4 = (uVar4 - uVar10) - uVar27 ^ uVar27 >> 0xf;
      }
      uVar4 = uVar4 + sVar3;
      switch(uVar13) {
      case 0xb:
        uVar4 = uVar4 + (uint)pbVar14[10] * 0x1000000;
      case 10:
        uVar4 = uVar4 + (uint)pbVar14[9] * 0x10000;
      case 9:
        uVar4 = uVar4 + (uint)pbVar14[8] * 0x100;
      case 8:
        uVar27 = uVar27 + (uint)pbVar14[7] * 0x1000000;
      case 7:
        uVar27 = uVar27 + (uint)pbVar14[6] * 0x10000;
      case 6:
        uVar27 = uVar27 + (uint)pbVar14[5] * 0x100;
      case 5:
        uVar27 = uVar27 + pbVar14[4];
      case 4:
        uVar10 = uVar10 + (uint)pbVar14[3] * 0x1000000;
      case 3:
        uVar10 = uVar10 + (uint)pbVar14[2] * 0x10000;
      case 2:
        uVar10 = uVar10 + (uint)pbVar14[1] * 0x100;
      case 1:
        uVar10 = uVar10 + *pbVar14;
      default:
        piVar20 = *(int **)(pbVar5 + 0x44);
        uVar10 = (uVar10 - uVar27) - uVar4 ^ uVar4 >> 0xd;
        uVar13 = (uVar27 - uVar4) - uVar10 ^ uVar10 << 8;
        uVar4 = (uVar4 - uVar10) - uVar13 ^ uVar13 >> 0xd;
        uVar27 = (uVar10 - uVar13) - uVar4 ^ uVar4 >> 0xc;
        uVar10 = (uVar13 - uVar4) - uVar27 ^ uVar27 << 0x10;
        uVar4 = (uVar4 - uVar27) - uVar10 ^ uVar10 >> 5;
        uVar27 = (uVar27 - uVar10) - uVar4 ^ uVar4 >> 3;
        uVar10 = (uVar10 - uVar4) - uVar27 ^ uVar27 << 10;
        iVar23 = *(int *)(*piVar20 +
                         (((uVar4 - uVar27) - uVar10 ^ uVar10 >> 0xf) & piVar20[1] - 1U) * 0xc);
        iVar2 = 0;
        if (iVar23 == 0) goto LAB_0001de10;
      }
      do {
        iVar2 = iVar23 - piVar20[5];
LAB_0001de10:
        if ((iVar2 == 0) ||
           ((sVar3 == *(size_t *)(iVar2 + 0x5c) &&
            (iVar23 = memcmp(*(void **)(iVar2 + 0x58),local_1074,sVar3), iVar23 == 0)))) break;
        iVar23 = *(int *)(iVar2 + 0x54);
        iVar2 = 0;
      } while (iVar23 != 0);
    }
    iVar23 = pthread_rwlock_unlock((pthread_rwlock_t *)&DAT_00077294);
    if (iVar23 != 0) {
      FUN_0000b630("block_exists",0x14e5);
    }
    (*DAT_00075ed8)();
    if (iVar2 == 0) {
      pbVar5 = calloc(0x68,1);
      if (pbVar5 == (byte *)0x0) {
        local_1030[0] = 't';
        local_1030[1] = 'e';
        local_1030[2] = 's';
        local_1030[3] = 't';
        acStack_102c[0] = '_';
        acStack_102c[1] = 'w';
        acStack_102c[2] = 'o';
        acStack_102c[3] = 'r';
        acStack_1028[0] = 'k';
        acStack_1028[1] = '_';
        acStack_1028[2] = 'c';
        acStack_1028[3] = 'u';
        acStack_1024[0] = 'r';
        acStack_1024[1] = 'r';
        acStack_1024[2] = 'e';
        acStack_1024[3] = 'n';
        local_1020[0] = 't';
        local_1020[1] = ' ';
        local_1020[2] = 'O';
        local_1020[3] = 'O';
        local_101c[0] = 0x4d;
        FUN_00038730(3,local_1030,1);
        FUN_00016ca8(1);
      }
      strcpy((char *)pbVar5,(char *)local_1074);
      iVar2 = DAT_0007bb1c + 1;
      *(int *)(pbVar5 + 100) = DAT_0007bb1c;
      DAT_0007bb1c = iVar2;
      iVar2 = pthread_rwlock_wrlock((pthread_rwlock_t *)&DAT_00077294);
      if (iVar2 != 0) {
        FUN_0000b480("test_work_current",0x1526);
      }
      pbVar14 = DAT_000772b4;
      if (DAT_000772b4 == (byte *)0x0) {
        pbVar5[0x4c] = 0;
        pbVar5[0x4d] = 0;
        pbVar5[0x4e] = 0;
        pbVar5[0x4f] = 0;
        *(byte **)(pbVar5 + 0x58) = pbVar5;
        local_10a0 = pbVar14;
        sVar3 = strlen((char *)pbVar5);
        *(size_t *)(pbVar5 + 0x5c) = sVar3;
LAB_0001e2c8:
        pbVar5[0x48] = 0;
        pbVar5[0x49] = 0;
        pbVar5[0x4a] = 0;
        pbVar5[0x4b] = 0;
        DAT_000772b4 = pbVar5;
        puVar7 = malloc(0x2c);
        *(undefined4 **)(pbVar5 + 0x44) = puVar7;
        if (puVar7 == (undefined4 *)0x0) goto LAB_0001f11c;
        *puVar7 = 0;
        puVar7[3] = 0;
        puVar7[4] = 0;
        puVar7[5] = 0;
        puVar7[6] = 0;
        puVar7[7] = 0;
        puVar7[8] = 0;
        puVar7[9] = 0;
        puVar7[10] = 0;
        puVar7[1] = 0x20;
        puVar7[4] = pbVar5 + 0x44;
        puVar7[2] = 5;
        puVar7[5] = 0x44;
        pvVar8 = malloc(0x180);
        *puVar7 = pvVar8;
        if (pvVar8 == (void *)0x0) goto LAB_0001f11c;
        memset(pvVar8,0,0x180);
        puVar7[10] = 0xa0111fe1;
        pbVar14 = pbVar5;
      }
      else {
        if (6 < *(uint *)(*(int *)(DAT_000772b4 + 0x44) + 0xc)) {
          pbVar14 = (byte *)0x1;
          pbVar24 = DAT_000772b4 + 0x44;
LAB_0001e07c:
          pbVar22 = (byte *)0x0;
          pbVar29 = (byte *)0x0;
          uVar27 = 0;
LAB_0001e08c:
          uVar27 = uVar27 + 1;
          if (pbVar14 == (byte *)0x0) {
            pbVar11 = (byte *)0x0;
            pbVar15 = pbVar24;
          }
          else if (*(int *)(pbVar24 + 8) == 0) {
            pbVar11 = (byte *)0x1;
            pbVar15 = (byte *)0x0;
          }
          else {
            pbVar11 = (byte *)0x1;
            pbVar15 = (byte *)(*(int *)(pbVar24 + 8) +
                              *(int *)(*(int *)(DAT_000772b4 + 0x44) + 0x14));
            if (pbVar15 != (byte *)0x0) {
              while (pbVar11 != pbVar14) {
                pbVar15 = *(byte **)(pbVar15 + 8);
                pbVar11 = pbVar11 + 1;
                if ((pbVar15 == (byte *)0x0) ||
                   (pbVar15 = pbVar15 + *(int *)(*(int *)(DAT_000772b4 + 0x44) + 0x14),
                   pbVar15 == (byte *)0x0)) break;
              }
            }
          }
          pbVar19 = pbVar14;
LAB_0001e0ec:
          do {
            pbVar6 = pbVar24;
            pbVar12 = pbVar11;
            if (pbVar11 == (byte *)0x0) goto LAB_0001e160;
LAB_0001e0f4:
            pbVar24 = pbVar6;
            if (pbVar19 == (byte *)0x0 || pbVar15 == (byte *)0x0) {
              if ((pbVar6 != (byte *)0x0) &&
                 (pbVar24 = *(byte **)(pbVar6 + 8), pbVar24 != (byte *)0x0)) {
                pbVar24 = pbVar24 + *(int *)(*(int *)(DAT_000772b4 + 0x44) + 0x14);
              }
            }
            else {
              iVar2 = *(int *)(*(int *)(DAT_000772b4 + 0x44) + 0x14);
              if (0 < *(int *)(pbVar6 + -iVar2 + 100) - *(int *)(pbVar15 + -iVar2 + 100)) {
                pbVar26 = *(byte **)(pbVar15 + 8);
                pbVar11 = pbVar12;
                if (pbVar26 == (byte *)0x0) goto LAB_0001e390;
                pbVar26 = pbVar26 + iVar2;
                goto LAB_0001e390;
              }
              if ((pbVar6 != (byte *)0x0) &&
                 (pbVar24 = *(byte **)(pbVar6 + 8), pbVar24 != (byte *)0x0)) {
                pbVar24 = pbVar24 + iVar2;
              }
            }
            pbVar11 = pbVar12 + -1;
            pbVar26 = pbVar15;
            if (pbVar29 == (byte *)0x0) {
              pbVar11 = pbVar12 + -1;
              pbVar22 = pbVar6;
              pbVar29 = pbVar6;
              if (pbVar6 != (byte *)0x0) {
                iVar2 = 0;
                goto LAB_0001e154;
              }
              goto LAB_0001e0ec;
            }
            while (pbVar15 = pbVar26, pbVar6 != (byte *)0x0) {
              *(int *)(pbVar29 + 8) = (int)pbVar6 - *(int *)(*(int *)(DAT_000772b4 + 0x44) + 0x14);
              iVar2 = (int)pbVar29 - *(int *)(*(int *)(DAT_000772b4 + 0x44) + 0x14);
              pbVar29 = pbVar6;
LAB_0001e154:
              while( true ) {
                *(int *)(pbVar29 + 4) = iVar2;
                pbVar6 = pbVar24;
                pbVar12 = pbVar11;
                if (pbVar11 != (byte *)0x0) goto LAB_0001e0f4;
LAB_0001e160:
                if (pbVar15 == (byte *)0x0 || pbVar19 == (byte *)0x0) {
                  pbVar24 = pbVar15;
                  if (pbVar15 != (byte *)0x0) goto LAB_0001e08c;
                  if (pbVar29 != (byte *)0x0) {
                    pbVar29[8] = 0;
                    pbVar29[9] = 0;
                    pbVar29[10] = 0;
                    pbVar29[0xb] = 0;
                  }
                  if (1 < uVar27) {
                    pbVar14 = (byte *)((int)pbVar14 << 1);
                    pbVar24 = pbVar22;
                    if (pbVar22 != (byte *)0x0) goto LAB_0001e07c;
                    pbVar29 = pbVar22;
                  }
                  *(byte **)(*(int *)(DAT_000772b4 + 0x44) + 0x10) = pbVar29;
                  pbVar22 = pbVar22 + -*(int *)(*(int *)(DAT_000772b4 + 0x44) + 0x14);
                  iVar2 = *(int *)(pbVar22 + 0x48);
                  local_10a0 = *(byte **)(pbVar22 + 100);
                  DAT_000772b4 = pbVar22;
                  if ((iVar2 == 0) && (*(int *)(pbVar22 + 0x4c) == 0)) {
                    free((void *)**(undefined4 **)(pbVar22 + 0x44));
                    free(*(void **)(pbVar22 + 0x44));
                    pbVar14 = (byte *)0x0;
                    DAT_000772b4 = (byte *)0x0;
                  }
                  else {
                    iVar23 = *(int *)(pbVar22 + 0x44);
                    if (pbVar22 == (byte *)(*(int *)(iVar23 + 0x10) - *(int *)(iVar23 + 0x14))) {
                      *(int *)(iVar23 + 0x10) = iVar2 + *(int *)(iVar23 + 0x14);
                      iVar2 = *(int *)(pbVar22 + 0x48);
                    }
                    if (iVar2 == 0) {
                      pbVar24 = *(byte **)(pbVar22 + 0x4c);
                      pbVar14 = pbVar24;
                      DAT_000772b4 = pbVar24;
                    }
                    else {
                      *(undefined4 *)(iVar2 + *(int *)(*(int *)(pbVar22 + 0x44) + 0x14) + 8) =
                           *(undefined4 *)(pbVar22 + 0x4c);
                      pbVar24 = *(byte **)(pbVar22 + 0x4c);
                      pbVar14 = pbVar22;
                    }
                    if (pbVar24 != (byte *)0x0) {
                      *(undefined4 *)(pbVar24 + *(int *)(*(int *)(pbVar14 + 0x44) + 0x14) + 4) =
                           *(undefined4 *)(pbVar22 + 0x48);
                    }
                    iVar2 = ((*(int **)(pbVar14 + 0x44))[1] - 1U & *(uint *)(pbVar22 + 0x60)) * 0xc;
                    iVar23 = **(int **)(pbVar14 + 0x44) + iVar2;
                    *(int *)(iVar23 + 4) = *(int *)(iVar23 + 4) + -1;
                    if (*(byte **)(**(int **)(pbVar14 + 0x44) + iVar2) == pbVar22 + 0x44) {
                      *(undefined4 *)(**(int **)(pbVar14 + 0x44) + iVar2) =
                           *(undefined4 *)(pbVar22 + 0x54);
                    }
                    if (*(int *)(pbVar22 + 0x50) != 0) {
                      *(undefined4 *)(*(int *)(pbVar22 + 0x50) + 0x10) =
                           *(undefined4 *)(pbVar22 + 0x54);
                    }
                    if (*(int *)(pbVar22 + 0x54) != 0) {
                      *(undefined4 *)(*(int *)(pbVar22 + 0x54) + 0xc) =
                           *(undefined4 *)(pbVar22 + 0x50);
                    }
                    *(int *)(*(int *)(pbVar14 + 0x44) + 0xc) =
                         *(int *)(*(int *)(pbVar14 + 0x44) + 0xc) + -1;
                  }
                  free(pbVar22);
                  *(byte **)(pbVar5 + 0x58) = pbVar5;
                  pbVar5[0x4c] = 0;
                  pbVar5[0x4d] = 0;
                  pbVar5[0x4e] = 0;
                  pbVar5[0x4f] = 0;
                  sVar3 = strlen((char *)pbVar5);
                  *(size_t *)(pbVar5 + 0x5c) = sVar3;
                  if (pbVar14 != (byte *)0x0) goto LAB_0001e518;
                  goto LAB_0001e2c8;
                }
                pbVar26 = pbVar11;
                if (*(int *)(pbVar15 + 8) != 0) {
                  pbVar26 = (byte *)(*(int *)(pbVar15 + 8) +
                                    *(int *)(*(int *)(DAT_000772b4 + 0x44) + 0x14));
                }
LAB_0001e390:
                pbVar6 = pbVar15;
                pbVar19 = pbVar19 + -1;
                if (pbVar29 != (byte *)0x0) break;
                iVar2 = 0;
                pbVar22 = pbVar6;
                pbVar15 = pbVar26;
                pbVar29 = pbVar6;
              }
            }
            pbVar29[8] = 0;
            pbVar29[9] = 0;
            pbVar29[10] = 0;
            pbVar29[0xb] = 0;
            pbVar29 = pbVar6;
          } while( true );
        }
        *(byte **)(pbVar5 + 0x58) = pbVar5;
        pbVar5[0x4c] = 0;
        pbVar5[0x4d] = 0;
        pbVar5[0x4e] = 0;
        pbVar5[0x4f] = 0;
        sVar3 = strlen((char *)pbVar5);
        local_10a0 = (byte *)0x0;
        *(size_t *)(pbVar5 + 0x5c) = sVar3;
LAB_0001e518:
        *(byte **)(*(int *)(*(int *)(pbVar14 + 0x44) + 0x10) + 8) = pbVar5;
        *(int *)(pbVar5 + 0x48) =
             *(int *)(*(int *)(pbVar14 + 0x44) + 0x10) - *(int *)(*(int *)(pbVar14 + 0x44) + 0x14);
        *(byte **)(*(int *)(pbVar14 + 0x44) + 0x10) = pbVar5 + 0x44;
        puVar7 = *(undefined4 **)(pbVar14 + 0x44);
      }
      uVar10 = 0xfeedbeef;
      puVar7[3] = puVar7[3] + 1;
      uVar18 = *(undefined4 *)(pbVar14 + 0x44);
      pbVar5[0x60] = 0xef;
      pbVar5[0x61] = 0xbe;
      pbVar5[0x62] = 0xed;
      pbVar5[99] = 0xfe;
      uVar4 = 0x9e3779b9;
      pbVar24 = pbVar5 + 0x44;
      *(undefined4 *)pbVar24 = uVar18;
      sVar3 = strlen((char *)pbVar5);
      pbVar22 = pbVar5;
      uVar27 = sVar3;
      if (sVar3 < 0xc) {
        uVar13 = 0x9e3779b9;
      }
      else {
        uVar13 = 0x9e3779b9;
        do {
          uVar27 = uVar27 - 0xc;
          pbVar29 = pbVar22 + 2;
          pbVar11 = pbVar22 + 1;
          pbVar15 = pbVar22 + 6;
          pbVar19 = pbVar22 + 5;
          bVar1 = *pbVar22;
          pbVar6 = pbVar22 + 4;
          pbVar12 = pbVar22 + 3;
          pbVar26 = pbVar22 + 7;
          uVar10 = (uint)pbVar22[10] * 0x10000 + (uint)pbVar22[9] * 0x100 + (uint)pbVar22[8] +
                   (uint)pbVar22[0xb] * 0x1000000 + uVar10;
          pbVar22 = pbVar22 + 0xc;
          iVar2 = (uint)*pbVar15 * 0x10000 + (uint)*pbVar19 * 0x100 + (uint)*pbVar6 +
                  (uint)*pbVar26 * 0x1000000 + uVar4;
          uVar28 = ((((uint)*pbVar29 * 0x10000 + (uint)*pbVar11 * 0x100 + (uint)bVar1 +
                     (uint)*pbVar12 * 0x1000000) - uVar10) - iVar2) + uVar13 ^ uVar10 >> 0xd;
          uVar4 = (iVar2 - uVar10) - uVar28 ^ uVar28 << 8;
          uVar13 = (uVar10 - uVar28) - uVar4 ^ uVar4 >> 0xd;
          uVar10 = (uVar28 - uVar4) - uVar13 ^ uVar13 >> 0xc;
          uVar4 = (uVar4 - uVar13) - uVar10 ^ uVar10 << 0x10;
          uVar28 = (uVar13 - uVar10) - uVar4 ^ uVar4 >> 5;
          uVar13 = (uVar10 - uVar4) - uVar28 ^ uVar28 >> 3;
          uVar4 = (uVar4 - uVar28) - uVar13 ^ uVar13 << 10;
          uVar10 = (uVar28 - uVar13) - uVar4 ^ uVar4 >> 0xf;
          *(uint *)(pbVar5 + 0x60) = uVar10;
        } while (0xb < uVar27);
        sVar3 = strlen((char *)pbVar5);
      }
      uVar10 = sVar3 + uVar10;
      *(uint *)(pbVar5 + 0x60) = uVar10;
      switch(uVar27) {
      case 0xb:
        uVar10 = uVar10 + (uint)pbVar22[10] * 0x1000000;
        *(uint *)(pbVar5 + 0x60) = uVar10;
      case 10:
        uVar10 = uVar10 + (uint)pbVar22[9] * 0x10000;
        *(uint *)(pbVar5 + 0x60) = uVar10;
      case 9:
        uVar10 = uVar10 + (uint)pbVar22[8] * 0x100;
        *(uint *)(pbVar5 + 0x60) = uVar10;
      case 8:
        uVar4 = uVar4 + (uint)pbVar22[7] * 0x1000000;
      case 7:
        uVar4 = uVar4 + (uint)pbVar22[6] * 0x10000;
      case 6:
        uVar4 = uVar4 + (uint)pbVar22[5] * 0x100;
      case 5:
        uVar4 = uVar4 + pbVar22[4];
      case 4:
        uVar13 = uVar13 + (uint)pbVar22[3] * 0x1000000;
      case 3:
        uVar13 = uVar13 + (uint)pbVar22[2] * 0x10000;
      case 2:
        uVar13 = uVar13 + (uint)pbVar22[1] * 0x100;
      case 1:
        uVar13 = uVar13 + *pbVar22;
      }
      uVar27 = (uVar13 - uVar4) - uVar10 ^ uVar10 >> 0xd;
      uVar4 = (uVar4 - uVar10) - uVar27 ^ uVar27 << 8;
      uVar13 = (uVar10 - uVar27) - uVar4 ^ uVar4 >> 0xd;
      uVar27 = (uVar27 - uVar4) - uVar13 ^ uVar13 >> 0xc;
      uVar10 = (uVar4 - uVar13) - uVar27 ^ uVar27 << 0x10;
      uVar4 = (uVar13 - uVar27) - uVar10 ^ uVar10 >> 5;
      uVar27 = (uVar27 - uVar10) - uVar4 ^ uVar4 >> 3;
      uVar10 = (uVar10 - uVar4) - uVar27 ^ uVar27 << 10;
      uVar27 = (uVar4 - uVar27) - uVar10 ^ uVar10 >> 0xf;
      *(uint *)(pbVar5 + 0x60) = uVar27;
      iVar2 = (uVar27 & (*(int **)(pbVar14 + 0x44))[1] - 1U) * 0xc;
      iVar23 = **(int **)(pbVar14 + 0x44) + iVar2;
      *(int *)(iVar23 + 4) = *(int *)(iVar23 + 4) + 1;
      uVar18 = *(undefined4 *)(**(int **)(pbVar14 + 0x44) + iVar2);
      pbVar5[0x50] = 0;
      pbVar5[0x51] = 0;
      pbVar5[0x52] = 0;
      pbVar5[0x53] = 0;
      *(undefined4 *)(pbVar5 + 0x54) = uVar18;
      iVar23 = **(int **)(pbVar14 + 0x44);
      if (*(int *)(iVar23 + iVar2) != 0) {
        *(byte **)(*(int *)(iVar23 + iVar2) + 0xc) = pbVar24;
        iVar23 = **(int **)(pbVar14 + 0x44);
      }
      *(byte **)(iVar23 + iVar2) = pbVar24;
      if (((uint)((*(int *)(**(int **)(pbVar14 + 0x44) + iVar2 + 8) + 1) * 10) <=
           *(uint *)(**(int **)(pbVar14 + 0x44) + iVar2 + 4)) &&
         (iVar2 = *(int *)(pbVar5 + 0x44), *(int *)(iVar2 + 0x24) != 1)) {
        sVar3 = *(int *)(iVar2 + 4) * 0x18;
        pvVar8 = malloc(sVar3);
        if (pvVar8 == (void *)0x0) {
LAB_0001f11c:
                    /* WARNING: Subroutine does not return */
          exit(-1);
        }
        memset(pvVar8,0,sVar3);
        *(uint *)(iVar2 + 0x18) =
             (uint)((*(int *)(iVar2 + 4) * 2 - 1U & *(uint *)(iVar2 + 0xc)) != 0) +
             (*(uint *)(iVar2 + 0xc) >> (*(int *)(iVar2 + 8) + 1U & 0xff));
        *(undefined4 *)(*(int *)(pbVar5 + 0x44) + 0x1c) = 0;
        piVar20 = *(int **)(pbVar5 + 0x44);
        uVar27 = piVar20[1];
        if (uVar27 != 0) {
          iVar2 = 0;
          local_10a8 = 0;
          do {
            iVar23 = *(int *)(*piVar20 + iVar2);
            if (*(int *)(*piVar20 + iVar2) != 0) {
              while( true ) {
                iVar25 = *(int *)(iVar23 + 0x10);
                uVar10 = uVar27 * 2 - 1 & *(uint *)(iVar23 + 0x1c);
                uVar27 = *(int *)((int)pvVar8 + uVar10 * 0xc + 4) + 1;
                *(uint *)((int)pvVar8 + uVar10 * 0xc + 4) = uVar27;
                if ((uint)piVar20[6] < uVar27) {
                  piVar20[7] = piVar20[7] + 1;
                  uVar18 = FUN_0005c648(*(undefined4 *)((int)pvVar8 + uVar10 * 0xc + 4),
                                        *(undefined4 *)(*(int *)(pbVar5 + 0x44) + 0x18));
                  *(undefined4 *)((int)pvVar8 + uVar10 * 0xc + 8) = uVar18;
                }
                *(undefined4 *)(iVar23 + 0xc) = 0;
                *(undefined4 *)(iVar23 + 0x10) = *(undefined4 *)((int)pvVar8 + uVar10 * 0xc);
                iVar16 = *(int *)((int)pvVar8 + uVar10 * 0xc);
                if (iVar16 != 0) {
                  *(int *)(iVar16 + 0xc) = iVar23;
                }
                *(int *)((int)pvVar8 + uVar10 * 0xc) = iVar23;
                piVar20 = *(int **)(pbVar5 + 0x44);
                if (iVar25 == 0) break;
                uVar27 = piVar20[1];
                iVar23 = iVar25;
              }
              uVar27 = piVar20[1];
            }
            iVar2 = iVar2 + 0xc;
            local_10a8 = local_10a8 + 1;
          } while (local_10a8 < uVar27);
        }
        free((void *)*piVar20);
        *(int *)(*(int *)(pbVar5 + 0x44) + 4) = *(int *)(*(int *)(pbVar5 + 0x44) + 4) << 1;
        *(int *)(*(int *)(pbVar5 + 0x44) + 8) = *(int *)(*(int *)(pbVar5 + 0x44) + 8) + 1;
        **(undefined4 **)(pbVar5 + 0x44) = pvVar8;
        iVar2 = *(int *)(pbVar5 + 0x44);
        if (*(uint *)(iVar2 + 0xc) >> 1 < *(uint *)(iVar2 + 0x1c)) {
          iVar23 = *(int *)(iVar2 + 0x20) + 1;
        }
        else {
          iVar23 = 0;
        }
        *(int *)(iVar2 + 0x20) = iVar23;
        if (1 < *(uint *)(*(int *)(pbVar5 + 0x44) + 0x20)) {
          *(undefined4 *)(*(int *)(pbVar5 + 0x44) + 0x24) = 1;
        }
      }
      uVar27 = *(uint *)(param_1 + 0x48);
      uVar31 = VectorShiftLeft(0xffff,in_d17 & 0xffffffffffff0000 |
                                      (ulonglong)((0x1d - (uint)*(byte *)(param_1 + 0x48)) * 8) &
                                      0xffff,8,1);
      dVar32 = (double)FUN_0005cb50((int)uVar31,(int)((ulonglong)uVar31 >> 0x20));
      dVar30 = (double)VectorSignedToFloat((uVar27 >> 8 & 0xff) << 0x10 |
                                           (uVar27 >> 0x10 & 0xff) << 8 | uVar27 >> 0x18,
                                           (byte)(in_fpscr >> 0x16) & 3);
      dVar32 = dVar32 / dVar30;
      if (dVar32 != DAT_00075de0) {
        uVar31 = FUN_0005cc48(SUB84(dVar32,0),(int)((ulonglong)dVar32 >> 0x20));
        FUN_00011580((int)uVar31,(int)((ulonglong)uVar31 >> 0x20),&DAT_000772b8,8,0);
        DAT_00075de0 = dVar32;
        if ((DAT_00078e09 != '\0') &&
           (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
          snprintf(local_1030,0x1000,"Network diff set to %s",&DAT_000772b8);
          FUN_00038730(7,local_1030,0);
        }
      }
      iVar2 = pthread_rwlock_unlock((pthread_rwlock_t *)&DAT_00077294);
      if (iVar2 != 0) {
        FUN_0000b630("test_work_current",0x1536);
      }
      (*DAT_00075ed8)();
      if (((local_10a0 != (byte *)0x0) && (DAT_00078e09 != '\0')) &&
         ((DAT_0007ad48 != '\0' || ((DAT_00078e08 != '\0' || (6 < DAT_000766c4)))))) {
        snprintf(local_1030,0x1000,"Deleted block %d from database",(int)local_10a0);
        FUN_00038730(7,local_1030,0);
      }
      iVar2 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007b9d8);
      if (iVar2 != 0) {
        FUN_0000b414("set_curblock",0x14cc);
      }
      iVar2 = pthread_rwlock_wrlock((pthread_rwlock_t *)&DAT_0007b9f0);
      if (iVar2 != 0) {
        FUN_0000b480("set_curblock",0x14cc);
      }
      FUN_0002a84c(&DAT_0007b070);
      strcpy(&DAT_0007bb28,(char *)local_1074);
      tStack_1098 = DAT_0007b070;
      DAT_000772c0 = local_1094;
      DAT_000772c4 = local_1090;
      DAT_000772c8 = local_108c;
      DAT_000772cc = local_1088;
      DAT_000772d0 = local_1084;
      DAT_000772d4 = local_1080;
      DAT_000772d8 = local_107c;
      DAT_000772dc = local_1078;
      ptVar9 = localtime(&tStack_1098);
      snprintf(&DAT_000772e0,0x20,"[%02d:%02d:%02d]",ptVar9->tm_hour,ptVar9->tm_min,ptVar9->tm_sec);
      iVar2 = pthread_rwlock_unlock((pthread_rwlock_t *)&DAT_0007b9f0);
      if (iVar2 != 0) {
        FUN_0000b630("set_curblock",0x14d1);
      }
      iVar2 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b9d8);
      if (iVar2 != 0) {
        FUN_0000b5c4("set_curblock",0x14d1);
      }
      (*DAT_00075ed8)();
      iVar2 = 0;
      do {
        if ((&DAT_0007bb28)[iVar2] != '0') break;
        iVar2 = iVar2 + 1;
      } while (iVar2 != 0x39);
      strncpy(&DAT_00077300,&DAT_0007bb28 + iVar2,8);
      DAT_00077308 = 0;
      if ((DAT_00078e09 != '\0') &&
         (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
        snprintf(local_1030,0x1000,"New block: %s... diff %s",&DAT_0007bb28,&DAT_000772b8);
        FUN_00038730(7,local_1030,0);
      }
      iVar2 = DAT_0007bb1c;
      piVar21[0x8e] = local_1094;
      piVar21[0x8f] = local_1090;
      piVar21[0x90] = local_108c;
      piVar21[0x91] = local_1088;
      piVar21[0x92] = local_1084;
      piVar21[0x93] = local_1080;
      piVar21[0x94] = local_107c;
      piVar21[0x95] = local_1078;
      if (iVar2 != 1) {
        DAT_000770ac = DAT_000770ac + 1;
        *(int *)(param_1 + 0x720) = DAT_000770ac;
        if (*(char *)(param_1 + 0x6e8) == '\0') {
          if (DAT_0007bc79 == '\0') {
            if ((((char)piVar21[0xe7] == '\0') && (DAT_00078e09 != '\0')) &&
               ((DAT_0007ad48 != '\0' || ((DAT_00078e08 != '\0' || (6 < DAT_000766c4)))))) {
              local_1030[0] = 'N';
              local_1030[1] = 'e';
              local_1030[2] = 'w';
              local_1030[3] = ' ';
              acStack_102c[0] = 'b';
              acStack_102c[1] = 'l';
              acStack_102c[2] = 'o';
              acStack_102c[3] = 'c';
              acStack_1028[0] = 'k';
              acStack_1028[1] = ' ';
              acStack_1028[2] = 'd';
              acStack_1028[3] = 'e';
              acStack_1024[0] = 't';
              acStack_1024[1] = 'e';
              acStack_1024[2] = 'c';
              acStack_1024[3] = 't';
              local_1020[0] = 'e';
              local_1020[1] = 'd';
              local_1020[2] = ' ';
              local_1020[3] = 'o';
              local_101c[0]._0_1_ = 'n';
              local_101c[0]._1_1_ = ' ';
              local_101c[1]._0_1_ = 'n';
              local_101c[1]._1_1_ = 'e';
              acStack_1018[0] = 't';
              acStack_1018[1] = 'w';
              acStack_1018[2] = 'o';
              acStack_1018[3] = 'r';
              local_1014[0] = 0x6b;
              FUN_00038730(7,local_1030,0);
            }
          }
          else if ((((char)piVar21[0xe7] == '\0') && (DAT_00078e09 != '\0')) &&
                  ((DAT_0007ad48 != '\0' || ((DAT_00078e08 != '\0' || (6 < DAT_000766c4)))))) {
            local_1030[0] = 'N';
            local_1030[1] = 'e';
            local_1030[2] = 'w';
            local_1030[3] = ' ';
            acStack_102c[0] = 'b';
            acStack_102c[1] = 'l';
            acStack_102c[2] = 'o';
            acStack_102c[3] = 'c';
            acStack_1028[0] = 'k';
            acStack_1028[1] = ' ';
            acStack_1028[2] = 'd';
            acStack_1028[3] = 'e';
            acStack_1024[0] = 't';
            acStack_1024[1] = 'e';
            acStack_1024[2] = 'c';
            acStack_1024[3] = 't';
            local_1020[0] = 'e';
            local_1020[1] = 'd';
            local_1020[2] = ' ';
            local_1020[3] = 'o';
            local_101c[0]._0_1_ = 'n';
            local_101c[0]._1_1_ = ' ';
            local_101c[1]._0_1_ = 'n';
            local_101c[1]._1_1_ = 'e';
            acStack_1018[0] = 't';
            acStack_1018[1] = 'w';
            acStack_1018[2] = 'o';
            acStack_1018[3] = 'r';
            local_1014[0]._0_1_ = 'k';
            local_1014[0]._1_1_ = ' ';
            local_1014[1]._0_1_ = 'b';
            local_1014[1]._1_1_ = 'e';
            local_1010[0] = 'f';
            local_1010[1] = 'o';
            local_1010[2] = 'r';
            local_1010[3] = 'e';
            acStack_100c[0] = ' ';
            acStack_100c[1] = 'p';
            acStack_100c[2] = 'o';
            acStack_100c[3] = 'o';
            acStack_1008[0] = 'l';
            acStack_1008[1] = ' ';
            acStack_1008[2] = 'n';
            acStack_1008[3] = 'o';
            acStack_1004[0] = 't';
            acStack_1004[1] = 'i';
            acStack_1004[2] = 'f';
            acStack_1004[3] = 'i';
            acStack_1000[0] = 'c';
            acStack_1000[1] = 'a';
            acStack_1000[2] = 't';
            acStack_1000[3] = 'i';
            uStack_ffc = 0x6e6f;
            local_ffa = 0;
            FUN_00038730(7,local_1030,0);
          }
        }
        else if (*(char *)(param_1 + 0x6ec) == '\0') {
          if ((DAT_00078e09 != '\0') &&
             (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
            pcVar17 = "";
            if (*(char *)(param_1 + 0x714) != '\0') {
              pcVar17 = "GBT ";
            }
            snprintf(local_1030,0x1000,"%sLONGPOLL from pool %d detected new block",pcVar17,
                     **(int **)(param_1 + 0x6d4));
            FUN_00038730(7,local_1030,0);
          }
        }
        else if ((DAT_00078e09 != '\0') &&
                (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
          snprintf(local_1030,0x1000,"Stratum from pool %d detected new block",*piVar21);
          FUN_00038730(7,local_1030,0);
        }
        FUN_00017210();
        *(undefined1 *)(param_1 + 0x6e8) = 0;
        return 1;
      }
      uVar18 = 0;
    }
    else {
      iVar2 = memcmp(piVar21 + 0x8e,&local_1094,0x20);
      if (iVar2 == 0) {
        uVar18 = 1;
      }
      else {
        iVar2 = memcmp(&local_1094,&DAT_000772c0,0x20);
        if (iVar2 == 0) {
          if ((DAT_00078e09 != '\0') &&
             (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
            snprintf(local_1030,0x1000,"Pool %d now up to date",*piVar21);
            FUN_00038730(7,local_1030,0);
          }
          uVar18 = 1;
          piVar21[0x8e] = local_1094;
          piVar21[0x8f] = local_1090;
          piVar21[0x90] = local_108c;
          piVar21[0x91] = local_1088;
          piVar21[0x92] = local_1084;
          piVar21[0x93] = local_1080;
          piVar21[0x94] = local_107c;
          piVar21[0x95] = local_1078;
        }
        else {
          uVar18 = 0;
          if ((DAT_00078e09 != '\0') &&
             (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (uVar18 = 0, 6 < DAT_000766c4))))
          {
            uVar18 = 0;
            snprintf(local_1030,0x1000,"Stale data from pool %d",*piVar21);
            FUN_00038730(7,local_1030,0);
          }
        }
      }
      if (*(char *)(param_1 + 0x6e8) != '\0') {
        uVar27 = DAT_00076efc - 3;
        DAT_000770ac = DAT_000770ac + 1;
        *(int *)(param_1 + 0x720) = DAT_000770ac;
        if ((uVar27 < 2) ||
           (iVar23 = *(int *)(param_1 + 0x6d4), iVar2 = FUN_0001da58(), iVar23 == iVar2)) {
          if (*(char *)(param_1 + 0x6ec) == '\0') {
            if ((DAT_00078e09 != '\0') &&
               (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
              pcVar17 = "";
              if (*(char *)(param_1 + 0x714) != '\0') {
                pcVar17 = "GBT ";
              }
              snprintf(local_1030,0x1000,"%sLONGPOLL from pool %d requested work restart",pcVar17,
                       **(int **)(param_1 + 0x6d4));
              FUN_00038730(7,local_1030,0);
            }
          }
          else if ((DAT_00078e09 != '\0') &&
                  (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
            snprintf(local_1030,0x1000,"Stratum from pool %d requested work restart",*piVar21);
            FUN_00038730(7,local_1030,0);
          }
          FUN_00017210();
        }
      }
    }
    *(undefined1 *)(param_1 + 0x6e8) = 0;
  }
  else {
    uVar18 = 1;
  }
  return uVar18;
}

