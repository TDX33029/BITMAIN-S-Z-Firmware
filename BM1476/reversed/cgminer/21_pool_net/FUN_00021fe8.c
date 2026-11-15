//==================== FUN_00021fe8 @ 00021fe8 ====================

/* WARNING: Restarted to delay deadcode elimination for space: ram */

undefined4 FUN_00021fe8(int *param_1)

{
  char cVar1;
  pthread_t __th;
  int iVar2;
  int *piVar3;
  time_t tVar4;
  void *pvVar5;
  char *param8;
  int iVar6;
  size_t sVar7;
  uint uVar8;
  undefined4 uVar9;
  uint uVar10;
  int iVar11;
  uint uVar12;
  uint uVar13;
  int *piVar14;
  int iVar15;
  undefined4 *puVar16;
  bool bVar17;
  uint local_2044;
  char acStack_2038 [16];
  char acStack_2028 [4096];
  char local_1028 [4];
  char acStack_1024 [4];
  char acStack_1020 [4];
  char acStack_101c [4];
  char local_1018 [4];
  char acStack_1014 [4];
  char acStack_1010 [4];
  undefined2 local_100c [2];
  char local_1008 [4];
  char acStack_1004 [4];
  char acStack_1000 [4];
  undefined2 local_ffc [2];
  undefined2 local_ff8 [2];
  undefined2 local_ff4;
  
  __th = pthread_self();
  pthread_detach(__th);
  snprintf(acStack_2038,0x10,"%d/SStratum",*param_1);
  FUN_0002f0b4(acStack_2038);
  iVar2 = FUN_0002a168();
  param_1[0xbc] = iVar2;
  if (iVar2 == 0) {
    local_1028[0] = 'F';
    local_1028[1] = 'a';
    local_1028[2] = 'i';
    local_1028[3] = 'l';
    acStack_1024[0] = 'e';
    acStack_1024[1] = 'd';
    acStack_1024[2] = ' ';
    acStack_1024[3] = 't';
    acStack_1020[0] = 'o';
    acStack_1020[1] = ' ';
    acStack_1020[2] = 'c';
    acStack_1020[3] = 'r';
    acStack_101c[0] = 'e';
    acStack_101c[1] = 'a';
    acStack_101c[2] = 't';
    acStack_101c[3] = 'e';
    local_1018[0] = ' ';
    local_1018[1] = 's';
    local_1018[2] = 't';
    local_1018[3] = 'r';
    acStack_1014[0] = 'a';
    acStack_1014[1] = 't';
    acStack_1014[2] = 'u';
    acStack_1014[3] = 'm';
    acStack_1010[0] = '_';
    acStack_1010[1] = 'q';
    acStack_1010[2] = ' ';
    acStack_1010[3] = 'i';
    local_100c[0]._0_1_ = 'n';
    local_100c[0]._1_1_ = ' ';
    local_100c[1]._0_1_ = 's';
    local_100c[1]._1_1_ = 't';
    local_1008[0] = 'r';
    local_1008[1] = 'a';
    local_1008[2] = 't';
    local_1008[3] = 'u';
    acStack_1004[0] = 'm';
    acStack_1004[1] = '_';
    acStack_1004[2] = 's';
    acStack_1004[3] = 't';
    acStack_1000[0] = 'h';
    acStack_1000[1] = 'r';
    acStack_1000[2] = 'e';
    acStack_1000[3] = 'a';
    local_ffc[0] = 100;
    FUN_00038730(3,local_1028,1);
    FUN_00016ca8(1);
  }
  memset(acStack_2028,0,0x1000);
  cVar1 = *(char *)((int)param_1 + 0x89);
  do {
    if (cVar1 != '\0') {
      FUN_0002a230(param_1[0xbc]);
      return 0;
    }
    iVar2 = FUN_0002a300(param_1[0xbc],0);
    if (iVar2 == 0) {
      local_1028[0] = 'S';
      local_1028[1] = 't';
      local_1028[2] = 'r';
      local_1028[3] = 'a';
      acStack_1024[0] = 't';
      acStack_1024[1] = 'u';
      acStack_1024[2] = 'm';
      acStack_1024[3] = ' ';
      acStack_1020[0] = 'q';
      acStack_1020[1] = ' ';
      acStack_1020[2] = 'r';
      acStack_1020[3] = 'e';
      acStack_101c[0] = 't';
      acStack_101c[1] = 'u';
      acStack_101c[2] = 'r';
      acStack_101c[3] = 'n';
      local_1018[0] = 'e';
      local_1018[1] = 'd';
      local_1018[2] = ' ';
      local_1018[3] = 'e';
      acStack_1014[0] = 'm';
      acStack_1014[1] = 'p';
      acStack_1014[2] = 't';
      acStack_1014[3] = 'y';
      acStack_1010[0] = ' ';
      acStack_1010[1] = 'w';
      acStack_1010[2] = 'o';
      acStack_1010[3] = 'r';
      local_100c[0] = 0x6b;
      FUN_00038730(3,local_1028,1);
      FUN_00016ca8(1);
    }
    piVar3 = calloc(0x34,1);
    if (piVar3 == (int *)0x0) {
      snprintf(local_1028,0x1000,"%s: calloc() failed on sshare.","stratum_sthread");
      FUN_00038730(3,local_1028,1);
      FUN_00016ca8(1);
    }
    tVar4 = time((time_t *)0x0);
    piVar3[9] = iVar2;
    piVar3[0xb] = tVar4;
    pvVar5 = (void *)FUN_00029730(iVar2 + 0x14c,0x20);
    param8 = (char *)FUN_00029730(iVar2 + 0x16c,0x543);
    iVar6 = pthread_mutex_lock((pthread_mutex_t *)&DAT_00077030);
    if (iVar6 != 0) {
      FUN_0000b414("stratum_sthread",0x1c78);
    }
    iVar6 = DAT_0007b0f8 + 1;
    piVar3[10] = DAT_0007b0f8;
    DAT_0007b0f8 = iVar6;
    iVar6 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_00077030);
    if (iVar6 != 0) {
      FUN_0000b5c4("stratum_sthread",0x1c7b);
    }
    (*DAT_00075ed8)();
    sVar7 = strlen(*(char **)(iVar2 + 0x710));
    snprintf(acStack_2028,0x1000,
             "{\"id\": %d, \"method\": \"mining.submit\", \"params\": [\"%s\", \"%s\", \"%s\", \"%s\", \"%s\"]}"
             ,piVar3[10],(char *)param_1[0x33],*(char **)(iVar2 + 0x6f0),*(char **)(iVar2 + 0x704),
             (char *)((int)pvVar5 + sVar7),param8);
    free(pvVar5);
    free(param8);
    while( true ) {
      iVar6 = piVar3[0xb];
      tVar4 = time((time_t *)0x0);
      if (iVar6 + 0x77 < tVar4) goto LAB_0002265c;
      sVar7 = strlen(acStack_2028);
      iVar6 = FUN_0002b5c8(param_1,acStack_2028,sVar7);
      if (iVar6 != 0) break;
      iVar6 = FUN_00018834(param_1,param_1 + 0x20);
      if ((iVar6 == 0) && (iVar6 = FUN_0001ffec(param_1), iVar6 != 0)) {
        if ((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0' || (3 < DAT_000766c4)))) {
          snprintf(local_1028,0x1000,"Pool %d stratum share submission failure",*param_1);
          FUN_00038730(4,local_1028,0);
        }
        DAT_0007ae68 = DAT_0007ae68 + 1;
        param_1[0x29] = param_1[0x29] + 1;
      }
      if (DAT_0007b644 != '\0') {
        if ((DAT_00078e09 != '\0') &&
           (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
          local_1028[0] = 'L';
          local_1028[1] = 'o';
          local_1028[2] = 'w';
          local_1028[3] = 'm';
          acStack_1024[0] = 'e';
          acStack_1024[1] = 'm';
          acStack_1024[2] = ' ';
          acStack_1024[3] = 'o';
          acStack_1020[0] = 'p';
          acStack_1020[1] = 't';
          acStack_1020[2] = 'i';
          acStack_1020[3] = 'o';
          acStack_101c[0] = 'n';
          acStack_101c[1] = ' ';
          acStack_101c[2] = 'p';
          acStack_101c[3] = 'r';
          local_1018[0] = 'e';
          local_1018[1] = 'v';
          local_1018[2] = 'e';
          local_1018[3] = 'n';
          acStack_1014[0] = 't';
          acStack_1014[1] = 's';
          acStack_1014[2] = ' ';
          acStack_1014[3] = 'r';
          acStack_1010[0] = 'e';
          acStack_1010[1] = 's';
          acStack_1010[2] = 'u';
          acStack_1010[3] = 'b';
          local_100c[0]._0_1_ = 'm';
          local_100c[0]._1_1_ = 'i';
          local_100c[1]._0_1_ = 't';
          local_100c[1]._1_1_ = 't';
          local_1008[0] = 'i';
          local_1008[1] = 'n';
          local_1008[2] = 'g';
          local_1008[3] = ' ';
          acStack_1004[0] = 's';
          acStack_1004[1] = 't';
          acStack_1004[2] = 'r';
          acStack_1004[3] = 'a';
          acStack_1000[0] = 't';
          acStack_1000[1] = 'u';
          acStack_1000[2] = 'm';
          acStack_1000[3] = ' ';
          local_ffc[0]._0_1_ = 's';
          local_ffc[0]._1_1_ = 'h';
          local_ffc[1]._0_1_ = 'a';
          local_ffc[1]._1_1_ = 'r';
          local_ff8[0] = 0x65;
          FUN_00038730(7,local_1028,0);
        }
        goto LAB_0002265c;
      }
      iVar6 = pthread_mutex_lock((pthread_mutex_t *)(param_1 + 0x3d));
      if (iVar6 != 0) {
        FUN_0000b414("stratum_sthread",0x1ca2);
      }
      iVar6 = pthread_rwlock_rdlock((pthread_rwlock_t *)(param_1 + 0x43));
      if (iVar6 != 0) {
        FUN_0000b69c("stratum_sthread",0x1ca2);
      }
      iVar6 = pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0x3d));
      if (iVar6 != 0) {
        FUN_0000b5c4("stratum_sthread",0x1ca2);
      }
      if ((char *)param_1[0x9f] == (char *)0x0) {
        iVar6 = 0;
      }
      else {
        uVar8 = strcmp(*(char **)(iVar2 + 0x710),(char *)param_1[0x9f]);
        iVar6 = 1 - uVar8;
        if (1 < uVar8) {
          iVar6 = 0;
        }
      }
      iVar11 = pthread_rwlock_unlock((pthread_rwlock_t *)(param_1 + 0x43));
      if (iVar11 != 0) {
        FUN_0000b630("stratum_sthread",0x1ca4);
      }
      (*DAT_00075ed8)();
      if (iVar6 == 0) {
        if ((DAT_00078e09 != '\0') &&
           (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
          local_1028[0] = 'N';
          local_1028[1] = 'o';
          local_1028[2] = ' ';
          local_1028[3] = 'm';
          acStack_1024[0] = 'a';
          acStack_1024[1] = 't';
          acStack_1024[2] = 'c';
          acStack_1024[3] = 'h';
          acStack_1020[0] = 'i';
          acStack_1020[1] = 'n';
          acStack_1020[2] = 'g';
          acStack_1020[3] = ' ';
          acStack_101c[0] = 's';
          acStack_101c[1] = 'e';
          acStack_101c[2] = 's';
          acStack_101c[3] = 's';
          local_1018[0] = 'i';
          local_1018[1] = 'o';
          local_1018[2] = 'n';
          local_1018[3] = ' ';
          acStack_1014[0] = 'i';
          acStack_1014[1] = 'd';
          acStack_1014[2] = ' ';
          acStack_1014[3] = 'f';
          acStack_1010[0] = 'o';
          acStack_1010[1] = 'r';
          acStack_1010[2] = ' ';
          acStack_1010[3] = 'r';
          local_100c[0]._0_1_ = 'e';
          local_100c[0]._1_1_ = 's';
          local_100c[1]._0_1_ = 'u';
          local_100c[1]._1_1_ = 'b';
          local_1008[0] = 'm';
          local_1008[1] = 'i';
          local_1008[2] = 't';
          local_1008[3] = 't';
          acStack_1004[0] = 'i';
          acStack_1004[1] = 'n';
          acStack_1004[2] = 'g';
          acStack_1004[3] = ' ';
          acStack_1000[0] = 's';
          acStack_1000[1] = 't';
          acStack_1000[2] = 'r';
          acStack_1000[3] = 'a';
          local_ffc[0]._0_1_ = 't';
          local_ffc[0]._1_1_ = 'u';
          local_ffc[1]._0_1_ = 'm';
          local_ffc[1]._1_1_ = ' ';
          local_ff8[0]._0_1_ = 's';
          local_ff8[0]._1_1_ = 'h';
          local_ff8[1]._0_1_ = 'a';
          local_ff8[1]._1_1_ = 'r';
          local_ff4 = 0x65;
          FUN_00038730(7,local_1028,0);
        }
        goto LAB_0002265c;
      }
      sleep(5);
    }
    iVar2 = pthread_mutex_lock((pthread_mutex_t *)&DAT_00077030);
    if (iVar2 != 0) {
      FUN_0000b414("stratum_sthread",0x1c8a);
    }
    piVar3[2] = 0;
    piVar3[5] = (int)(piVar3 + 10);
    piVar14 = DAT_00077048;
    piVar3[6] = 4;
    if (piVar14 == (int *)0x0) {
      piVar3[1] = 0;
      DAT_00077048 = piVar3;
      puVar16 = malloc(0x2c);
      *piVar3 = (int)puVar16;
      if (puVar16 == (undefined4 *)0x0) goto LAB_000227ec;
      *puVar16 = 0;
      puVar16[3] = 0;
      puVar16[4] = 0;
      puVar16[5] = 0;
      puVar16[6] = 0;
      puVar16[7] = 0;
      puVar16[8] = 0;
      puVar16[9] = 0;
      puVar16[10] = 0;
      puVar16[4] = piVar3;
      puVar16[5] = 0;
      puVar16[1] = 0x20;
      puVar16[2] = 5;
      pvVar5 = malloc(0x180);
      *puVar16 = pvVar5;
      if (pvVar5 == (void *)0x0) goto LAB_000227ec;
      memset(pvVar5,0,0x180);
      puVar16[10] = 0xa0111fe1;
      piVar14 = piVar3;
    }
    else {
      *(int **)(*(int *)(*piVar14 + 0x10) + 8) = piVar3;
      piVar3[1] = *(int *)(*piVar14 + 0x10) - *(int *)(*piVar14 + 0x14);
      *(int **)(*piVar14 + 0x10) = piVar3;
      puVar16 = (undefined4 *)*piVar14;
    }
    puVar16[3] = puVar16[3] + 1;
    uVar12 = *(byte *)(piVar3 + 10) + 0x112410d + (uint)*(byte *)((int)piVar3 + 0x2b) * 0x1000000 +
             (uint)*(byte *)((int)piVar3 + 0x2a) * 0x10000 +
             (uint)*(byte *)((int)piVar3 + 0x29) * 0x100 ^ 0x7f76d;
    uVar10 = 0x9f49bac6 - uVar12 ^ uVar12 << 8;
    *piVar3 = *piVar14;
    uVar8 = (-0x112410d - uVar12) - uVar10 ^ uVar10 >> 0xd;
    uVar12 = (uVar12 - uVar10) - uVar8 ^ uVar8 >> 0xc;
    uVar10 = (uVar10 - uVar8) - uVar12 ^ uVar12 << 0x10;
    uVar13 = (uVar8 - uVar12) - uVar10 ^ uVar10 >> 5;
    uVar8 = (uVar12 - uVar10) - uVar13 ^ uVar13 >> 3;
    uVar12 = (uVar10 - uVar13) - uVar8 ^ uVar8 << 10;
    uVar8 = (uVar13 - uVar8) - uVar12 ^ uVar12 >> 0xf;
    piVar3[7] = uVar8;
    uVar8 = uVar8 & ((int *)*piVar14)[1] - 1U;
    iVar2 = uVar8 * 0xc;
    iVar6 = *(int *)*piVar14 + iVar2;
    *(int *)(iVar6 + 4) = *(int *)(iVar6 + 4) + 1;
    iVar6 = *(int *)(*(int *)*piVar14 + uVar8 * 0xc);
    piVar3[3] = 0;
    piVar3[4] = iVar6;
    iVar6 = *(int *)*piVar14;
    iVar11 = *(int *)(iVar6 + uVar8 * 0xc);
    if (iVar11 != 0) {
      *(int **)(iVar11 + 0xc) = piVar3;
      iVar6 = *(int *)*piVar14;
    }
    *(int **)(iVar6 + iVar2) = piVar3;
    if (((uint)((*(int *)(*(int *)*piVar14 + iVar2 + 8) + 1) * 10) <=
         *(uint *)(*(int *)*piVar14 + iVar2 + 4)) && (iVar2 = *piVar3, *(int *)(iVar2 + 0x24) != 1))
    {
      sVar7 = *(int *)(iVar2 + 4) * 0x18;
      pvVar5 = malloc(sVar7);
      if (pvVar5 == (void *)0x0) {
LAB_000227ec:
                    /* WARNING: Subroutine does not return */
        exit(-1);
      }
      memset(pvVar5,0,sVar7);
      iVar6 = 0;
      *(uint *)(iVar2 + 0x18) =
           (uint)((*(int *)(iVar2 + 4) * 2 - 1U & *(uint *)(iVar2 + 0xc)) != 0) +
           (*(uint *)(iVar2 + 0xc) >> (*(int *)(iVar2 + 8) + 1U & 0xff));
      *(undefined4 *)(*piVar3 + 0x1c) = 0;
      piVar14 = (int *)*piVar3;
      uVar8 = piVar14[1];
      if (uVar8 != 0) {
        local_2044 = 0;
        do {
          if (*(int *)(*piVar14 + iVar6) != 0) {
            iVar2 = *(int *)(*piVar14 + iVar6);
            while( true ) {
              iVar11 = *(int *)(iVar2 + 0x10);
              uVar12 = uVar8 * 2 - 1 & *(uint *)(iVar2 + 0x1c);
              uVar8 = *(int *)((int)pvVar5 + uVar12 * 0xc + 4) + 1;
              *(uint *)((int)pvVar5 + uVar12 * 0xc + 4) = uVar8;
              if ((uint)piVar14[6] < uVar8) {
                piVar14[7] = piVar14[7] + 1;
                uVar9 = FUN_0005c648(*(undefined4 *)((int)pvVar5 + uVar12 * 0xc + 4),
                                     *(undefined4 *)(*piVar3 + 0x18));
                *(undefined4 *)((int)pvVar5 + uVar12 * 0xc + 8) = uVar9;
              }
              *(undefined4 *)(iVar2 + 0xc) = 0;
              *(undefined4 *)(iVar2 + 0x10) = *(undefined4 *)((int)pvVar5 + uVar12 * 0xc);
              iVar15 = *(int *)((int)pvVar5 + uVar12 * 0xc);
              if (iVar15 != 0) {
                *(int *)(iVar15 + 0xc) = iVar2;
              }
              *(int *)((int)pvVar5 + uVar12 * 0xc) = iVar2;
              piVar14 = (int *)*piVar3;
              if (iVar11 == 0) break;
              uVar8 = piVar14[1];
              iVar2 = iVar11;
            }
            uVar8 = piVar14[1];
          }
          iVar6 = iVar6 + 0xc;
          local_2044 = local_2044 + 1;
        } while (local_2044 < uVar8);
      }
      free((void *)*piVar14);
      *(int *)(*piVar3 + 4) = *(int *)(*piVar3 + 4) << 1;
      *(int *)(*piVar3 + 8) = *(int *)(*piVar3 + 8) + 1;
      *(void **)*piVar3 = pvVar5;
      iVar2 = *piVar3;
      if (*(uint *)(iVar2 + 0xc) >> 1 < *(uint *)(iVar2 + 0x1c)) {
        iVar6 = *(int *)(iVar2 + 0x20) + 1;
      }
      else {
        iVar6 = 0;
      }
      *(int *)(iVar2 + 0x20) = iVar6;
      if (1 < *(uint *)(*piVar3 + 0x20)) {
        *(undefined4 *)(*piVar3 + 0x24) = 1;
      }
    }
    param_1[0xbd] = param_1[0xbd] + 1;
    iVar2 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_00077030);
    if (iVar2 != 0) {
      FUN_0000b5c4("stratum_sthread",0x1c8d);
    }
    (*DAT_00075ed8)();
    iVar2 = FUN_00018c2c(param_1,param_1 + 0x20);
    if ((iVar2 != 0) && (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4))))
    {
      snprintf(local_1028,0x1000,"Pool %d communication resumed, submitting work",*param_1);
      FUN_00038730(4,local_1028,0);
    }
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      local_1028[0] = 'S';
      local_1028[1] = 'u';
      local_1028[2] = 'c';
      local_1028[3] = 'c';
      acStack_1024[0] = 'e';
      acStack_1024[1] = 's';
      acStack_1024[2] = 's';
      acStack_1024[3] = 'f';
      acStack_1020[0] = 'u';
      acStack_1020[1] = 'l';
      acStack_1020[2] = 'l';
      acStack_1020[3] = 'y';
      acStack_101c[0] = ' ';
      acStack_101c[1] = 's';
      acStack_101c[2] = 'u';
      acStack_101c[3] = 'b';
      local_1018[0] = 'm';
      local_1018[1] = 'i';
      local_1018[2] = 't';
      local_1018[3] = 't';
      acStack_1014[0] = 'e';
      acStack_1014[1] = 'd';
      acStack_1014[2] = ',';
      acStack_1014[3] = ' ';
      acStack_1010[0] = 'a';
      acStack_1010[1] = 'd';
      acStack_1010[2] = 'd';
      acStack_1010[3] = 'i';
      local_100c[0]._0_1_ = 'n';
      local_100c[0]._1_1_ = 'g';
      local_100c[1]._0_1_ = ' ';
      local_100c[1]._1_1_ = 't';
      local_1008[0] = 'o';
      local_1008[1] = ' ';
      local_1008[2] = 's';
      local_1008[3] = 't';
      acStack_1004[0] = 'r';
      acStack_1004[1] = 'a';
      acStack_1004[2] = 't';
      acStack_1004[3] = 'u';
      acStack_1000[0] = 'm';
      acStack_1000[1] = '_';
      acStack_1000[2] = 's';
      acStack_1000[3] = 'h';
      local_ffc[0]._0_1_ = 'a';
      local_ffc[0]._1_1_ = 'r';
      local_ffc[1]._0_1_ = 'e';
      local_ffc[1]._1_1_ = 's';
      local_ff8[0]._0_1_ = ' ';
      local_ff8[0]._1_1_ = 'd';
      local_ff8[1]._0_1_ = 'b';
      local_ff8[1]._1_1_ = '\0';
      FUN_00038730(7,local_1028,0);
    }
LAB_0002265c:
    tVar4 = time((time_t *)0x0);
    bVar17 = DAT_00078e09 != '\0';
    piVar3[0xc] = tVar4;
    if ((bVar17) && (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      snprintf(local_1028,0x1000,"Pool %d stratum share submission lag time %d seconds",*param_1,
               tVar4 - piVar3[0xb]);
      FUN_00038730(7,local_1028,0);
    }
    memset(acStack_2028,0,0x1000);
    cVar1 = *(char *)((int)param_1 + 0x89);
  } while( true );
}

