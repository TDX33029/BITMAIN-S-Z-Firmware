//==================== FUN_0000b928 @ 0000b928 ====================

/* WARNING: Heritage AFTER dead removal. Example location: r0x00078950 : 0x0000dfcc */
/* WARNING: Heritage AFTER dead removal. Example location: s0xffffefd4 : 0x0000d7fc */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */
/* WARNING: Restarted to delay deadcode elimination for space: stack */

void FUN_0000b928(int param_1,undefined4 *param_2)

{
  byte *pbVar1;
  byte *pbVar2;
  byte *pbVar3;
  byte *pbVar4;
  byte *pbVar5;
  byte *pbVar6;
  byte *pbVar7;
  byte bVar8;
  char cVar9;
  undefined4 uVar10;
  long lVar11;
  void *pvVar12;
  undefined4 uVar13;
  char *pcVar14;
  char *pcVar15;
  size_t sVar16;
  uint uVar17;
  undefined4 *puVar18;
  FILE *pFVar19;
  int *piVar20;
  size_t sVar21;
  __sighandler_t p_Var22;
  __sighandler_t p_Var23;
  int iVar24;
  undefined4 *puVar25;
  uint uVar26;
  int *piVar27;
  char *pcVar28;
  undefined4 uVar29;
  code *extraout_r3;
  code *pcVar30;
  uint uVar31;
  int *piVar32;
  int extraout_r3_00;
  int iVar33;
  int iVar34;
  byte *pbVar35;
  undefined *puVar36;
  undefined *puVar37;
  byte *pbVar38;
  FILE **ppFVar39;
  uint uVar40;
  int iVar41;
  byte *pbVar42;
  undefined1 *puVar43;
  int iVar44;
  uint uVar45;
  code **extraout_r12;
  code **ppcVar46;
  size_t *psVar47;
  size_t *psVar48;
  bool bVar49;
  undefined1 local_3210 [16];
  char acStackY_3200 [4104];
  char local_21f8 [4];
  char acStackY_21f4 [4];
  undefined1 local_21f0;
  undefined1 auStackY_2004 [3588];
  int *local_11d0;
  uint local_11cc;
  byte *local_11c8;
  int aiStack_11c4 [2];
  _union_1051 _Stack_11bc;
  sigset_t sStack_11b8;
  int local_1138;
  char acStack_1130 [256];
  int local_1030;
  int local_102c;
  uint local_1028;
  char acStack_1024 [4];
  undefined4 local_1020;
  undefined4 local_101c;
  uint local_1018;
  uint local_1014;
  undefined4 local_1010;
  uint local_100c;
  undefined1 local_1008;
  undefined4 uStack_1004;
  undefined4 local_1000;
  char acStack_ffc [4044];
  byte abStack_30 [4];
  
  psVar48 = (size_t *)(auStackY_2004 + 0xe04);
  DAT_00078de8 = 0x2b61;
  DAT_00078dea = 0;
  DAT_00078ce8._0_1_ = 'c';
  DAT_00078ce8._1_1_ = 'g';
  DAT_00078ce8._2_1_ = 'm';
  DAT_00078ce8._3_1_ = 'i';
  DAT_00078cec._0_1_ = 'n';
  DAT_00078cec._1_1_ = 'e';
  DAT_00078cec._2_1_ = 'r';
  DAT_00078cec._3_1_ = '.';
  DAT_00078cf0._0_1_ = 'l';
  DAT_00078cf0._1_1_ = 'o';
  DAT_00078cf0._2_1_ = 'g';
  DAT_00078cf0._3_1_ = '\0';
  DAT_00078ce0 = 0;
  aiStack_11c4[0] = param_1;
  lVar11 = sysconf(0x54);
  ppcVar46 = extraout_r12;
  pcVar30 = extraout_r3;
  if (lVar11 == 1) {
    pcVar30 = sched_yield;
    ppcVar46 = &DAT_00075ed8;
  }
  if (lVar11 == 1) {
    *ppcVar46 = pcVar30;
  }
  pvVar12 = malloc((aiStack_11c4[0] + 1) * 4);
  DAT_00077290 = pvVar12;
  if (0 < aiStack_11c4[0]) {
    iVar34 = 0;
    iVar41 = 0;
    do {
      iVar41 = iVar41 + 1;
      uVar13 = __strdup(*(undefined4 *)((int)param_2 + iVar34));
      *(undefined4 *)((int)pvVar12 + iVar34) = uVar13;
      iVar34 = iVar34 + 4;
    } while (iVar41 < aiStack_11c4[0]);
  }
  ppFVar39 = (FILE **)0x0;
  *(undefined4 *)((int)pvVar12 + aiStack_11c4[0] * 4) = 0;
  FUN_00017990(&DAT_0007b384,&DAT_0005db00,0x29c5);
  FUN_00017990(&DAT_0007b85c,&DAT_0005db00,0x29c6);
  FUN_00017990(&DAT_0007b7a8,&DAT_0005db00,0x29c7);
  pbVar35 = (byte *)&DAT_00076bd8;
  FUN_000179e8(&DAT_0007ba10,&DAT_0005db00,0x29c8);
  FUN_00017990(&DAT_0007ad4c,&DAT_0005db00,0x29c9);
  FUN_00017990(&DAT_000770b8,&DAT_0005db00,0x29ca);
  FUN_000179e8(&DAT_0007b9d8,&DAT_0005db00,0x29cb);
  FUN_00017990(&DAT_00077030,&DAT_0005db00,0x29cc);
  FUN_000179bc(&DAT_00077294,&DAT_0005db00,0x29cd);
  FUN_000179bc(&DAT_0007b2d8,&DAT_0005db00,0x29ce);
  FUN_000179bc(&DAT_0007b650,&DAT_0005db00,0x29cf);
  FUN_000179bc(&DAT_0007b700,&DAT_0005db00,0x29d0);
  FUN_00017990(&DAT_00077060,&DAT_0005db00,0x29d2);
  iVar34 = pthread_cond_init((pthread_cond_t *)&DAT_00077078,(pthread_condattr_t *)0x0);
  psVar47 = (size_t *)(auStackY_2004 + 0xe04);
  if (iVar34 != 0) goto LAB_0000e168;
  FUN_00017990(&DAT_0007b62c,&DAT_0005db00,0x29d6);
  iVar34 = pthread_cond_init((pthread_cond_t *)&DAT_0007bae0,(pthread_condattr_t *)0x0);
  if (iVar34 == 0) {
    iVar34 = pthread_cond_init((pthread_cond_t *)&DAT_0007b1a0,(pthread_condattr_t *)0x0);
    ppFVar39 = (FILE **)0x0;
    psVar47 = (size_t *)(auStackY_2004 + 0xe04);
    if (iVar34 != 0) goto LAB_0000e1ac;
    DAT_0007b620 = FUN_0002a168();
    if (DAT_0007b620 == 0) {
      local_1028._0_1_ = 'o';
      local_1028._1_1_ = ' ';
      local_1028._2_1_ = 'c';
      local_1028._3_1_ = 'r';
      acStack_1024[0] = 'e';
      acStack_1024[1] = 'a';
      acStack_1024[2] = 't';
      acStack_1024[3] = 'e';
      local_1020._0_1_ = ' ';
      local_1020._1_1_ = 'g';
      local_1020._2_1_ = 'e';
      local_1020._3_1_ = 't';
      local_101c._0_1_ = 'q';
      local_101c._1_1_ = '\0';
      ppFVar39 = (FILE **)0x0;
      psVar48 = (size_t *)(auStackY_2004 + 0xe04);
      goto LAB_0000da08;
    }
    DAT_0007704c = (pthread_mutex_t *)(DAT_0007b620 + 0xc);
    snprintf(&DAT_00076bf4,0x100,"%s %s","cgminer","4.9.0");
    _Stack_11bc.sa_handler = FUN_0001795c;
    local_1138 = iVar34;
    sigemptyset(&sStack_11b8);
    sigaction(0xf,(sigaction *)&_Stack_11bc,(sigaction *)&DAT_0007b108);
    sigaction(2,(sigaction *)&_Stack_11bc,(sigaction *)&DAT_0007b94c);
    signal(0xd,(__sighandler_t)0x1);
    local_21f8[0] = '/';
    local_21f8[1] = 'u';
    local_21f8[2] = 's';
    local_21f8[3] = 'r';
    acStackY_21f4[0] = '/';
    acStackY_21f4[1] = 'b';
    acStackY_21f4[2] = 'i';
    acStackY_21f4[3] = 'n';
    DAT_0007bc74 = local_21f8;
    psVar48 = (size_t *)local_3210;
    DAT_0007b8b4 = acStackY_3200;
    local_21f0 = 0;
    pcVar14 = (char *)__strdup(*param_2);
    pcVar28 = DAT_0007b8b4;
    pcVar15 = dirname(pcVar14);
    strcpy(pcVar28,pcVar15);
    free(pcVar14);
    pcVar28 = DAT_0007b8b4;
    sVar16 = strlen(DAT_0007b8b4);
    (pcVar28 + sVar16)[0] = '/';
    (pcVar28 + sVar16)[1] = '\0';
    DAT_00077028 = 8;
    DAT_0007701c = 9;
    DAT_00077024 = 10;
    pbVar35 = calloc(0x68,1);
    if (pbVar35 == (byte *)0x0) goto LAB_0000e228;
  }
  else {
    local_1030._0_1_ = 'F';
    local_1030._1_1_ = 'a';
    local_1030._2_1_ = 'i';
    local_1030._3_1_ = 'l';
    local_102c._0_1_ = 'e';
    local_102c._1_1_ = 'd';
    local_102c._2_1_ = ' ';
    local_102c._3_1_ = 't';
    local_1028._0_1_ = 'o';
    local_1028._1_1_ = ' ';
    local_1028._2_1_ = 'p';
    local_1028._3_1_ = 't';
    acStack_1024[0] = 'h';
    acStack_1024[1] = 'r';
    acStack_1024[2] = 'e';
    acStack_1024[3] = 'a';
    local_1020._0_1_ = 'd';
    local_1020._1_1_ = '_';
    local_1020._2_1_ = 'c';
    local_1020._3_1_ = 'o';
    local_101c._0_1_ = 'n';
    local_101c._1_1_ = 'd';
    local_101c._2_1_ = '_';
    local_101c._3_1_ = 'i';
    local_1018._0_1_ = 'n';
    local_1018._1_1_ = 'i';
    local_1018._2_1_ = 't';
    local_1018._3_1_ = ' ';
    local_1014._0_1_ = 'r';
    local_1014._1_1_ = 'e';
    local_1014._2_1_ = 's';
    local_1014._3_1_ = 't';
    local_1010._0_1_ = 'a';
    local_1010._1_1_ = 'r';
    local_1010._2_1_ = 't';
    local_1010._3_1_ = '_';
    local_100c._0_1_ = 'c';
    local_100c._1_1_ = 'o';
    local_100c._2_1_ = 'n';
    local_100c._3_1_ = 'd';
    local_1008 = 0;
    FUN_00038730(3,&local_1030,1);
    FUN_0001d8fc(1,0);
LAB_0000e228:
    local_1030._0_1_ = 'm';
    local_1030._1_1_ = 'a';
    local_1030._2_1_ = 'i';
    local_1030._3_1_ = 'n';
    local_102c._0_1_ = ' ';
    local_102c._1_1_ = 'O';
    local_102c._2_1_ = 'O';
    local_102c._3_1_ = 'M';
    local_1028 = local_1028 & 0xffffff00;
    FUN_00038730(3,&local_1030,1);
    FUN_00016ca8(1);
  }
  iVar34 = 0x24;
  do {
    sVar16 = strlen((char *)pbVar35);
    iVar34 = iVar34 + -1;
    (pbVar35 + sVar16)[0] = '0';
    (pbVar35 + sVar16)[1] = '\0';
  } while (iVar34 != 0);
  pbVar35[0x4c] = 0;
  pbVar35[0x4d] = 0;
  pbVar35[0x4e] = 0;
  pbVar35[0x4f] = 0;
  *(byte **)(pbVar35 + 0x58) = pbVar35;
  sVar16 = strlen((char *)pbVar35);
  pbVar42 = DAT_000772b4;
  bVar49 = DAT_000772b4 == (byte *)0x0;
  *(size_t *)(pbVar35 + 0x5c) = sVar16;
  if (bVar49) {
    pbVar35[0x48] = 0;
    pbVar35[0x49] = 0;
    pbVar35[0x4a] = 0;
    pbVar35[0x4b] = 0;
    DAT_000772b4 = pbVar35;
    puVar18 = malloc(0x2c);
    *(undefined4 **)(pbVar35 + 0x44) = puVar18;
    if (puVar18 == (undefined4 *)0x0) {
LAB_0000da38:
                    /* WARNING: Subroutine does not return */
      exit(-1);
    }
    memset(puVar18,0,0x2c);
    puVar18[4] = pbVar35 + 0x44;
    puVar18[1] = 0x20;
    puVar18[2] = 5;
    puVar18[5] = 0x44;
    pvVar12 = malloc(0x180);
    *puVar18 = pvVar12;
    if (pvVar12 == (void *)0x0) goto LAB_0000da38;
    memset(pvVar12,0,0x180);
    puVar18[10] = 0xa0111fe1;
    pbVar42 = pbVar35;
  }
  else {
    *(byte **)(*(int *)(*(int *)(pbVar42 + 0x44) + 0x10) + 8) = pbVar35;
    *(int *)(pbVar35 + 0x48) =
         *(int *)(*(int *)(pbVar42 + 0x44) + 0x10) - *(int *)(*(int *)(pbVar42 + 0x44) + 0x14);
    *(byte **)(*(int *)(pbVar42 + 0x44) + 0x10) = pbVar35 + 0x44;
  }
  uVar45 = 0xfeedbeef;
  pvVar12 = (void *)0x9e3779b9;
  uVar40 = 0x9e3779b9;
  *(int *)(*(int *)(pbVar42 + 0x44) + 0xc) = *(int *)(*(int *)(pbVar42 + 0x44) + 0xc) + 1;
  uVar13 = *(undefined4 *)(pbVar42 + 0x44);
  pbVar35[0x60] = 0xef;
  pbVar35[0x61] = 0xbe;
  pbVar35[0x62] = 0xed;
  pbVar35[99] = 0xfe;
  local_11c8 = pbVar35 + 0x44;
  *(undefined4 *)local_11c8 = uVar13;
  uVar17 = strlen((char *)pbVar35);
  pbVar38 = pbVar35;
  for (; 0xb < uVar17; uVar17 = uVar17 - 0xc) {
    pbVar1 = pbVar38 + 2;
    pbVar2 = pbVar38 + 1;
    pbVar3 = pbVar38 + 6;
    pbVar4 = pbVar38 + 5;
    bVar8 = *pbVar38;
    pbVar5 = pbVar38 + 4;
    pbVar6 = pbVar38 + 3;
    pbVar7 = pbVar38 + 7;
    uVar45 = (uint)pbVar38[10] * 0x10000 + (uint)pbVar38[9] * 0x100 + (uint)pbVar38[8] +
             (uint)pbVar38[0xb] * 0x1000000 + uVar45;
    pbVar38 = pbVar38 + 0xc;
    iVar34 = (uint)*pbVar3 * 0x10000 + (uint)*pbVar4 * 0x100 + (uint)*pbVar5 +
             (uint)*pbVar7 * 0x1000000 + (int)pvVar12;
    uVar40 = ((((uint)*pbVar1 * 0x10000 + (uint)*pbVar2 * 0x100 + (uint)bVar8 +
               (uint)*pbVar6 * 0x1000000) - uVar45) - iVar34) + uVar40 ^ uVar45 >> 0xd;
    uVar26 = (iVar34 - uVar45) - uVar40 ^ uVar40 << 8;
    uVar31 = (uVar45 - uVar40) - uVar26 ^ uVar26 >> 0xd;
    uVar40 = (uVar40 - uVar26) - uVar31 ^ uVar31 >> 0xc;
    uVar45 = (uVar26 - uVar31) - uVar40 ^ uVar40 << 0x10;
    uVar26 = (uVar31 - uVar40) - uVar45 ^ uVar45 >> 5;
    uVar40 = (uVar40 - uVar45) - uVar26 ^ uVar26 >> 3;
    pvVar12 = (void *)((uVar45 - uVar26) - uVar40 ^ uVar40 << 10);
    uVar45 = (uVar26 - uVar40) - (int)pvVar12 ^ (uint)pvVar12 >> 0xf;
    *(uint *)(pbVar35 + 0x60) = uVar45;
  }
  sVar16 = strlen((char *)pbVar35);
  *(uint *)(pbVar35 + 0x60) = uVar45 + sVar16;
  switch(uVar17) {
  case 1:
    uVar45 = *(uint *)(pbVar35 + 0x60);
    while( true ) {
      uVar40 = uVar40 + *pbVar38;
LAB_0000bf14:
      uVar26 = (uVar40 - (int)pvVar12) - uVar45 ^ uVar45 >> 0xd;
      uVar17 = (int)pvVar12 + (-uVar26 - uVar45) ^ uVar26 << 8;
      uVar31 = (uVar45 - uVar26) - uVar17 ^ uVar17 >> 0xd;
      uVar26 = (uVar26 - uVar17) - uVar31 ^ uVar31 >> 0xc;
      uVar45 = (uVar17 - uVar31) - uVar26 ^ uVar26 << 0x10;
      uVar31 = (uVar31 - uVar26) - uVar45 ^ uVar45 >> 5;
      uVar17 = (uVar26 - uVar45) - uVar31 ^ uVar31 >> 3;
      uVar45 = (uVar45 - uVar31) - uVar17 ^ uVar17 << 10;
      uVar45 = (uVar31 - uVar17) - uVar45 ^ uVar45 >> 0xf;
      *(uint *)(pbVar35 + 0x60) = uVar45;
      iVar34 = (uVar45 & (*(int **)(pbVar42 + 0x44))[1] - 1U) * 0xc;
      iVar41 = **(int **)(pbVar42 + 0x44) + iVar34;
      *(int *)(iVar41 + 4) = *(int *)(iVar41 + 4) + 1;
      uVar13 = *(undefined4 *)(**(int **)(pbVar42 + 0x44) + iVar34);
      pbVar35[0x50] = 0;
      pbVar35[0x51] = 0;
      pbVar35[0x52] = 0;
      pbVar35[0x53] = 0;
      *(undefined4 *)(pbVar35 + 0x54) = uVar13;
      piVar27 = *(int **)(pbVar42 + 0x44);
      if (*(int *)(*piVar27 + iVar34) != 0) {
        *(byte **)(*(int *)(*piVar27 + iVar34) + 0xc) = local_11c8;
        piVar27 = *(int **)(pbVar42 + 0x44);
      }
      *(byte **)(*piVar27 + iVar34) = local_11c8;
      if (((uint)((*(int *)(**(int **)(pbVar42 + 0x44) + iVar34 + 8) + 1) * 10) <=
           *(uint *)(**(int **)(pbVar42 + 0x44) + iVar34 + 4)) &&
         (pbVar38 = *(byte **)(pbVar35 + 0x44), *(int *)(pbVar38 + 0x24) != 1)) {
        uVar40 = *(int *)(pbVar38 + 4) * 0x18;
        pvVar12 = malloc(uVar40);
        if (pvVar12 == (void *)0x0) goto LAB_0000da38;
        memset(pvVar12,0,uVar40);
        *(uint *)(pbVar38 + 0x18) =
             (uint)((*(int *)(pbVar38 + 4) * 2 - 1U & *(uint *)(pbVar38 + 0xc)) != 0) +
             (*(uint *)(pbVar38 + 0xc) >> (*(int *)(pbVar38 + 8) + 1U & 0xff));
        *(undefined4 *)(*(int *)(pbVar35 + 0x44) + 0x1c) = 0;
        piVar27 = *(int **)(pbVar35 + 0x44);
        uVar45 = piVar27[1];
        if (uVar45 != 0) {
          local_11c8 = (byte *)0x0;
          local_11cc = 0;
          do {
            pbVar38 = *(byte **)(local_11c8 + *piVar27);
            if (pbVar38 != (byte *)0x0) {
              while( true ) {
                pbVar42 = *(byte **)(pbVar38 + 0x10);
                uVar17 = uVar45 * 2 - 1 & *(uint *)(pbVar38 + 0x1c);
                uVar40 = uVar17 * 3;
                uVar45 = *(int *)((int)pvVar12 + uVar17 * 0xc + 4) + 1;
                *(uint *)((int)pvVar12 + uVar17 * 0xc + 4) = uVar45;
                if ((uint)piVar27[6] < uVar45) {
                  piVar27[7] = piVar27[7] + 1;
                  uVar13 = FUN_0005c648(*(undefined4 *)((int)pvVar12 + uVar17 * 0xc + 4),
                                        *(undefined4 *)(*(int *)(pbVar35 + 0x44) + 0x18));
                  *(undefined4 *)((int)pvVar12 + uVar17 * 0xc + 8) = uVar13;
                }
                pbVar38[0xc] = 0;
                pbVar38[0xd] = 0;
                pbVar38[0xe] = 0;
                pbVar38[0xf] = 0;
                *(undefined4 *)(pbVar38 + 0x10) = *(undefined4 *)((int)pvVar12 + uVar17 * 0xc);
                iVar34 = *(int *)((int)pvVar12 + uVar17 * 0xc);
                if (iVar34 != 0) {
                  *(byte **)(iVar34 + 0xc) = pbVar38;
                }
                *(byte **)((int)pvVar12 + uVar17 * 0xc) = pbVar38;
                if (pbVar42 == (byte *)0x0) break;
                piVar27 = *(int **)(pbVar35 + 0x44);
                uVar45 = piVar27[1];
                pbVar38 = pbVar42;
              }
              piVar27 = *(int **)(pbVar35 + 0x44);
            }
            uVar45 = piVar27[1];
            local_11cc = local_11cc + 1;
            local_11c8 = local_11c8 + 0xc;
          } while (local_11cc < uVar45);
        }
        free((void *)*piVar27);
        *(int *)(*(int *)(pbVar35 + 0x44) + 4) = *(int *)(*(int *)(pbVar35 + 0x44) + 4) << 1;
        *(int *)(*(int *)(pbVar35 + 0x44) + 8) = *(int *)(*(int *)(pbVar35 + 0x44) + 8) + 1;
        **(undefined4 **)(pbVar35 + 0x44) = pvVar12;
        iVar34 = *(int *)(pbVar35 + 0x44);
        if (*(uint *)(iVar34 + 0xc) >> 1 < *(uint *)(iVar34 + 0x1c)) {
          iVar41 = *(int *)(iVar34 + 0x20) + 1;
        }
        else {
          iVar41 = 0;
        }
        *(int *)(iVar34 + 0x20) = iVar41;
        if (1 < *(uint *)(*(int *)(pbVar35 + 0x44) + 0x20)) {
          *(undefined4 *)(*(int *)(pbVar35 + 0x44) + 0x24) = 1;
        }
      }
      strcpy(&DAT_0007bb28,(char *)pbVar35);
      pbVar42 = abStack_30;
      DAT_0007b440 = &DAT_0007b440;
      DAT_0007b444 = &DAT_0007b440;
      FUN_0005bbb8(&DAT_00075600,"Options for both config file and command line");
      FUN_0005bbb8(&DAT_00075e24,"Options for command line only");
      FUN_0005bc84(aiStack_11c4,param_2,&LAB_00011c44);
      if (aiStack_11c4[0] == 1) break;
      local_1030._0_1_ = 'U';
      local_1030._1_1_ = 'n';
      local_1030._2_1_ = 'e';
      local_1030._3_1_ = 'x';
      local_102c._0_1_ = 'p';
      local_102c._1_1_ = 'e';
      local_102c._2_1_ = 'c';
      local_102c._3_1_ = 't';
      local_1028._0_1_ = 'e';
      local_1028._1_1_ = 'd';
      local_1028._2_1_ = ' ';
      local_1028._3_1_ = 'e';
      acStack_1024[0] = 'x';
      acStack_1024[1] = 't';
      acStack_1024[2] = 'r';
      acStack_1024[3] = 'a';
      local_1020._0_1_ = ' ';
      local_1020._1_1_ = 'c';
      local_1020._2_1_ = 'o';
      local_1020._3_1_ = 'm';
      local_101c._0_1_ = 'm';
      local_101c._1_1_ = 'a';
      local_101c._2_1_ = 'n';
      local_101c._3_1_ = 'd';
      local_1018._0_1_ = 'l';
      local_1018._1_1_ = 'i';
      local_1018._2_1_ = 'n';
      local_1018._3_1_ = 'e';
      local_1014._0_1_ = ' ';
      local_1014._1_1_ = 'a';
      local_1014._2_1_ = 'r';
      local_1014._3_1_ = 'g';
      local_1010._0_1_ = 'u';
      local_1010._1_1_ = 'm';
      local_1010._2_1_ = 'e';
      local_1010._3_1_ = 'n';
      local_100c = CONCAT13(local_100c._3_1_,0x7374);
      FUN_00038730(3,&local_1030,1);
      FUN_0001d8fc(1,0);
switchD_0000be78_caseD_2:
      uVar45 = *(uint *)(pbVar35 + 0x60);
LAB_0000bf04:
      uVar40 = uVar40 + (uint)pbVar38[1] * 0x100;
    }
    if (DAT_00076f20 == '\0') {
      DAT_00076f24 = malloc(0x1000);
      FUN_00016150();
      iVar34 = access(DAT_00076f24,4);
      if (iVar34 == 0) {
        FUN_00016d98();
      }
      else {
        free(DAT_00076f24);
        DAT_00076f24 = (char *)0x0;
      }
    }
    if ((DAT_0007730d != '\0') || (DAT_00076ffc != 0)) {
      if (DAT_0007b859 != '\0') {
        local_1030._0_1_ = 'C';
        local_1030._1_1_ = 'a';
        local_1030._2_1_ = 'n';
        local_1030._3_1_ = 'n';
        local_102c._0_1_ = 'o';
        local_102c._1_1_ = 't';
        local_102c._2_1_ = ' ';
        local_102c._3_1_ = 'u';
        local_1028._0_1_ = 's';
        local_1028._1_1_ = 'e';
        local_1028._2_1_ = ' ';
        local_1028._3_1_ = 'b';
        acStack_1024[0] = 'e';
        acStack_1024[1] = 'n';
        acStack_1024[2] = 'c';
        acStack_1024[3] = 'h';
        local_1020._0_1_ = 'm';
        local_1020._1_1_ = 'a';
        local_1020._2_1_ = 'r';
        local_1020._3_1_ = 'k';
        local_101c._0_1_ = ' ';
        local_101c._1_1_ = 'm';
        local_101c._2_1_ = 'o';
        local_101c._3_1_ = 'd';
        local_1018._0_1_ = 'e';
        local_1018._1_1_ = ' ';
        local_1018._2_1_ = 'w';
        local_1018._3_1_ = 'i';
        local_1014._0_1_ = 't';
        local_1014._1_1_ = 'h';
        local_1014._2_1_ = ' ';
        local_1014._3_1_ = 's';
        local_1010._0_1_ = 'c';
        local_1010._1_1_ = 'r';
        local_1010._2_1_ = 'y';
        local_1010._3_1_ = 'p';
        local_100c = CONCAT22(local_100c._2_2_,0x74);
        FUN_00038730(3,&local_1030,1);
        FUN_00016ca8(1);
      }
      iVar34 = FUN_00017a3c();
      puVar18 = malloc(0xff);
      if (DAT_00076ffc == 0) {
        pcVar28 = "Benchmark";
      }
      else {
        pcVar28 = "Benchfile";
      }
      *(undefined4 **)(iVar34 + 0xc4) = puVar18;
      uVar13 = *(undefined4 *)(pcVar28 + 4);
      uVar29 = *(undefined4 *)(pcVar28 + 8);
      *puVar18 = *(undefined4 *)pcVar28;
      puVar18[1] = uVar13;
      *(short *)(puVar18 + 2) = (short)uVar29;
      uVar13 = *(undefined4 *)(iVar34 + 0xc4);
      *(undefined4 *)(iVar34 + 0xcc) = uVar13;
      *(undefined4 *)(iVar34 + 0xd0) = uVar13;
      *(undefined4 *)(iVar34 + 200) = uVar13;
      *(undefined4 *)(iVar34 + 0x270) = uVar13;
      strncpy((char *)(iVar34 + 0x30),"?",7);
      *(undefined1 *)(iVar34 + 0x37) = 0;
      if (*(int *)(iVar34 + 0x84) != 1) {
        *(undefined4 *)(iVar34 + 0x84) = 1;
        DAT_0007b5e4 = DAT_0007b5e4 + 1;
      }
      *(undefined1 *)(iVar34 + 0x81) = 0;
      DAT_00078ca9 = 1;
      iVar34 = 0;
      do {
        iVar41 = iVar34 + 1;
        FUN_00029830(&DAT_0007731c + iVar34 * 0xa0,
                     "000000029c6bf469abe4ad37605c097a860cff3cf5c1ef4377618f74000000000000000082b1514e7b6565941e5824f084292164ec5f97e7ea20c494bd96e524d478977b536dd2261900896c8b10020000000080000000000000000000000000000000000000000000000000000000000000000000000000000000008002000064e4e3becc01064d808269b330f40f4de82dc92e894d635025daa3e2e2c410b4"
                     + iVar34 * 0x144,0xa0);
        FUN_00029830(&DAT_00077d1c + iVar34 * 0xa0,
                     "00000002c01f502cb3e9fdb053230ec12a4954c1021a6b35862b5e29000000000000000084d1b83ae44057025e8c5b5756b44f04df5fffe4a7a30e5c12d12a97a7a4c2ea536dce431900896cb6d60e00000000800000000000000000000000000000000000000000000000000000000000000000000000000000000080020000d08f7e14c50dad77dc238b4db2901a0578e657b1954779ab9cd82a73829edf7f"
                     + iVar34 * 0x144,0xa0);
        iVar34 = iVar41;
      } while (iVar41 != 0x10);
      FUN_00016244(0,&DAT_00078730);
    }
LAB_0000c524:
    local_11d0 = &local_1030;
    puVar37 = &DAT_00078bd0;
    if (DAT_00076bf0 != (char *)0x0) {
      pFVar19 = fopen(DAT_00076bf0,"rb");
      memset(acStack_1130,0,0x100);
      if (pFVar19 == (FILE *)0x0) {
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
          snprintf((char *)local_11d0,0x1000,"Open miner version file %s error",DAT_00076bf0);
LAB_0000d908:
          FUN_00038730(3,local_11d0,0);
          goto LAB_0000c658;
        }
LAB_0000c67c:
        if ((DAT_00078e08 == '\0') && (DAT_000766c4 < 3)) goto LAB_0000c6d8;
      }
      else {
        sVar16 = fread(acStack_1130,1,0x100,pFVar19);
        if ((int)sVar16 < 1) {
          if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
            *psVar48 = sVar16;
            snprintf((char *)local_11d0,0x1000,"Read miner version file %s error %d",DAT_00076bf0,
                     *psVar48);
            goto LAB_0000d908;
          }
          goto LAB_0000c67c;
        }
        pcVar28 = strchr(acStack_1130,10);
        pcVar15 = acStack_1130;
        pcVar14 = &DAT_00078750;
        if (pcVar28 != (char *)0x0) {
          memcpy(&DAT_00078750,pcVar15,(int)pcVar28 - (int)pcVar15);
          pcVar14 = &DAT_00078850;
          pcVar15 = pcVar28 + 1;
        }
        strcpy(pcVar14,pcVar15);
        sVar16 = strlen(&DAT_00078750);
        if (*(char *)((int)&DAT_0007874c + sVar16 + 3) == '\n') {
          *(undefined1 *)((int)&DAT_0007874c + sVar16 + 3) = 0;
        }
        sVar16 = strlen(&DAT_00078750);
        if (*(char *)((int)&DAT_0007874c + sVar16 + 3) == '\r') {
          *(undefined1 *)((int)&DAT_0007874c + sVar16 + 3) = 0;
        }
        sVar16 = strlen(&DAT_00078850);
        if ((&DAT_0007884f)[sVar16] == '\n') {
          (&DAT_0007884f)[sVar16] = 0;
        }
        sVar16 = strlen(&DAT_00078850);
        if ((&DAT_0007884f)[sVar16] == '\r') {
          (&DAT_0007884f)[sVar16] = 0;
        }
LAB_0000c658:
        if (DAT_0007ad48 == '\0') goto LAB_0000c67c;
      }
      *psVar48 = (size_t)&DAT_00078850;
      snprintf((char *)local_11d0,0x1000,"Miner compile time: %s type: %s",&DAT_00078750,
               (char *)*psVar48);
      FUN_00038730(3,local_11d0,0);
    }
LAB_0000c6d8:
    if (DAT_00076be0 != (char *)0x0) {
      DAT_00078ce0 = 1;
      strcpy((char *)&DAT_00078ce8,DAT_00076be0);
      if (DAT_00076bec != (char *)0x0) {
        strcpy((char *)&DAT_00078de8,DAT_00076bec);
      }
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
        *psVar48 = (size_t)&DAT_00078de8;
        snprintf((char *)local_11d0,0x1000,"Log file path: %s Open flag: %s",(char *)&DAT_00078ce8,
                 (char *)*psVar48);
        FUN_00038730(3,local_11d0,0);
      }
    }
    pcVar28 = DAT_00076be8;
    ppFVar39 = (FILE **)0x0;
    if (DAT_00076be8 == (char *)0x0) goto LAB_0000cc0c;
    pcVar14 = acStack_1130;
    memset(pcVar14,0,0x100);
    if (DAT_00076be4 == (char *)0x0) {
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
        snprintf((char *)local_11d0,0x1000,"Log work path: %s",pcVar28);
        FUN_00038730(3,local_11d0,0);
      }
    }
    else {
      if (*DAT_00076be4 == '\0') {
        local_1030._0_1_ = 'L';
        local_1030._1_1_ = 'o';
        local_1030._2_1_ = 'g';
        local_1030._3_1_ = ' ';
        local_102c._0_1_ = 'w';
        local_102c._1_1_ = 'o';
        local_102c._2_1_ = 'r';
        local_102c._3_1_ = 'k';
        local_1028._0_1_ = ' ';
        local_1028._1_1_ = 'a';
        local_1028._2_1_ = 's';
        local_1028._3_1_ = 'i';
        builtin_strncpy(acStack_1024,"c nu",4);
        local_1020._0_1_ = 'm';
        local_1020._1_1_ = ' ';
        local_1020._2_1_ = 'e';
        local_1020._3_1_ = 'm';
        local_101c._0_1_ = 'p';
        local_101c._1_1_ = 't';
        local_101c._2_1_ = 'y';
        local_101c._3_1_ = '\0';
        FUN_00038730(3,local_11d0,1);
        FUN_00016ca8(1);
      }
      DAT_00078950 = strtol(DAT_00076be4,(char **)0x0,10);
      if ((DAT_00078950 != 0x20 && DAT_00078950 != 1) && (DAT_00078950 != 0x40)) {
        local_1030._0_1_ = 'L';
        local_1030._1_1_ = 'o';
        local_1030._2_1_ = 'g';
        local_1030._3_1_ = ' ';
        local_102c._0_1_ = 'w';
        local_102c._1_1_ = 'o';
        local_102c._2_1_ = 'r';
        local_102c._3_1_ = 'k';
        local_1028._0_1_ = ' ';
        local_1028._1_1_ = 'a';
        local_1028._2_1_ = 's';
        local_1028._3_1_ = 'i';
        builtin_strncpy(acStack_1024,"c nu",4);
        local_1020._0_1_ = 'm';
        local_1020._1_1_ = ' ';
        local_1020._2_1_ = 'm';
        local_1020._3_1_ = 'u';
        local_101c._0_1_ = 's';
        local_101c._1_1_ = 't';
        local_101c._2_1_ = ' ';
        local_101c._3_1_ = 'b';
        local_1018._0_1_ = 'e';
        local_1018._1_1_ = ' ';
        local_1018._2_1_ = '1';
        local_1018._3_1_ = ',';
        local_1014._0_1_ = ' ';
        local_1014._1_1_ = '3';
        local_1014._2_1_ = '2';
        local_1014._3_1_ = ',';
        local_1010._0_1_ = ' ';
        local_1010._1_1_ = '6';
        local_1010._2_1_ = '4';
        local_1010._3_1_ = '\0';
        FUN_00038730(3,local_11d0,1);
        FUN_00016ca8(1);
      }
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
        *psVar48 = (size_t)DAT_00076be4;
        snprintf((char *)local_11d0,0x1000,"Log work path: %s Asic num: %s",DAT_00076be8,
                 (char *)*psVar48);
        FUN_00038730(3,local_11d0,0);
      }
    }
    sprintf(pcVar14,"%s.txt",DAT_00076be8);
    DAT_00078954 = fopen(pcVar14,"a+");
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
      snprintf((char *)local_11d0,0x1000,"Log work open file %s",pcVar14);
      FUN_00038730(3,local_11d0,0);
    }
    ppFVar39 = (FILE **)&DAT_00078bd0;
    iVar34 = 1;
    if (DAT_00078950 == 1) goto LAB_0000df54;
    if ((DAT_00078950 == 0x20 || DAT_00078950 == 0x40) && (-1 < DAT_00078950)) {
      ppFVar39 = &DAT_00078954;
      iVar34 = 0;
      do {
        iVar41 = DAT_00078950;
        *psVar48 = iVar34;
        sprintf(pcVar14,"%s%02d_%02d.txt",DAT_00076be8,iVar41,*psVar48);
        pFVar19 = fopen(pcVar14,"a+");
        bVar49 = DAT_0007ad48 != '\0';
        ppFVar39 = ppFVar39 + 1;
        *ppFVar39 = pFVar19;
        iVar41 = DAT_00078950;
        if (((bVar49) || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
          *psVar48 = (size_t)pcVar14;
          snprintf((char *)local_11d0,0x1000,"Log work open asic %d file %s",iVar41,(char *)*psVar48
                  );
          FUN_00038730(3,local_11d0,0);
        }
        iVar34 = iVar34 + 1;
      } while (iVar34 <= DAT_00078950);
    }
LAB_0000cb58:
    if (DAT_00078a5c != '\0') {
      pcVar28 = &DAT_00078a5c;
      iVar34 = 0;
      do {
        sprintf(pcVar14,"%s_diff_%02d.txt",DAT_00076be8,iVar34);
        pFVar19 = fopen(pcVar14,"a+");
        bVar49 = DAT_0007ad48 != '\0';
        ppFVar39 = (FILE **)((int)pcVar28 + 4);
        *ppFVar39 = pFVar19;
        if (((bVar49) || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
          snprintf((char *)local_11d0,0x1000,"Log work open diff file %s",pcVar14);
          FUN_00038730(3,local_11d0,0);
        }
        iVar34 = iVar34 + 1;
        pcVar28 = (char *)ppFVar39;
      } while (iVar34 != 0x41);
    }
LAB_0000cc0c:
    if (DAT_0007bb18 == '\0') {
      if (DAT_00075dd0 != '\0') {
        FUN_00027ba0();
      }
    }
    else {
      DAT_00075dd0 = '\0';
    }
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
      snprintf((char *)local_11d0,0x1000,"Started %s",&DAT_00076bf4);
      FUN_00038730(4,local_11d0,0);
    }
    if (DAT_00076f24 != (char *)0x0) {
      if ((DAT_00078e09 != '\0') &&
         (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
        snprintf((char *)local_11d0,0x1000,"Loaded configuration file %s",DAT_00076f24);
        FUN_00038730(7,local_11d0,0);
      }
      if (DAT_00076ff4 == -1) {
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
          local_1030._0_1_ = 'E';
          local_1030._1_1_ = 'r';
          local_1030._2_1_ = 'r';
          local_1030._3_1_ = 'o';
          local_102c._0_1_ = 'r';
          local_102c._1_1_ = ' ';
          local_102c._2_1_ = 'i';
          local_102c._3_1_ = 'n';
          local_1028._0_1_ = ' ';
          local_1028._1_1_ = 'c';
          local_1028._2_1_ = 'o';
          local_1028._3_1_ = 'n';
          builtin_strncpy(acStack_1024,"figu",4);
          local_1020._0_1_ = 'r';
          local_1020._1_1_ = 'a';
          local_1020._2_1_ = 't';
          local_1020._3_1_ = 'i';
          local_101c._0_1_ = 'o';
          local_101c._1_1_ = 'n';
          local_101c._2_1_ = ' ';
          local_101c._3_1_ = 'f';
          local_1018._0_1_ = 'i';
          local_1018._1_1_ = 'l';
          local_1018._2_1_ = 'e';
          local_1018._3_1_ = ',';
          local_1014._0_1_ = ' ';
          local_1014._1_1_ = 'p';
          local_1014._2_1_ = 'a';
          local_1014._3_1_ = 'r';
          local_1010._0_1_ = 't';
          local_1010._1_1_ = 'i';
          local_1010._2_1_ = 'a';
          local_1010._3_1_ = 'l';
          local_100c._0_1_ = 'l';
          local_100c._1_1_ = 'y';
          local_100c._2_1_ = ' ';
          local_100c._3_1_ = 'l';
          local_1008 = 0x6f;
          uStack_1004._0_1_ = 'd';
          uStack_1004._1_1_ = '.';
          uStack_1004._2_1_ = '\0';
          FUN_00038730(4,local_11d0,0);
        }
        if ((DAT_00075dd0 != '\0') &&
           (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)))) {
          local_1030._0_1_ = 'S';
          local_1030._1_1_ = 't';
          local_1030._2_1_ = 'a';
          local_1030._3_1_ = 'r';
          local_102c._0_1_ = 't';
          local_102c._1_1_ = ' ';
          local_102c._2_1_ = 'c';
          local_102c._3_1_ = 'g';
          local_1028._0_1_ = 'm';
          local_1028._1_1_ = 'i';
          local_1028._2_1_ = 'n';
          local_1028._3_1_ = 'e';
          builtin_strncpy(acStack_1024,"r wi",4);
          local_1020._0_1_ = 't';
          local_1020._1_1_ = 'h';
          local_1020._2_1_ = ' ';
          local_1020._3_1_ = '-';
          local_101c._0_1_ = 'T';
          local_101c._1_1_ = ' ';
          local_101c._2_1_ = 't';
          local_101c._3_1_ = 'o';
          local_1018._0_1_ = ' ';
          local_1018._1_1_ = 's';
          local_1018._2_1_ = 'e';
          local_1018._3_1_ = 'e';
          local_1014._0_1_ = ' ';
          local_1014._1_1_ = 'w';
          local_1014._2_1_ = 'h';
          local_1014._3_1_ = 'a';
          local_1010._0_1_ = 't';
          local_1010._1_1_ = ' ';
          local_1010._2_1_ = 'f';
          local_1010._3_1_ = 'a';
          local_100c._0_1_ = 'i';
          local_100c._1_1_ = 'l';
          local_100c._2_1_ = 'e';
          local_100c._3_1_ = 'd';
          local_1008 = 0x20;
          uStack_1004._0_1_ = 'l';
          uStack_1004._1_1_ = 'o';
          uStack_1004._2_1_ = 'a';
          uStack_1004._3_1_ = 'd';
          local_1000._0_1_ = '.';
          local_1000._1_1_ = '\0';
          FUN_00038730(4,local_11d0,0);
        }
      }
      else if (DAT_00076ff4 == 0) {
        if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 4)) {
LAB_0000cd9c:
          if (DAT_000766c4 < 4) goto LAB_0000cdec;
        }
        else {
          local_1030._0_1_ = 'F';
          local_1030._1_1_ = 'a';
          local_1030._2_1_ = 't';
          local_1030._3_1_ = 'a';
          local_102c._0_1_ = 'l';
          local_102c._1_1_ = ' ';
          local_102c._2_1_ = 'J';
          local_102c._3_1_ = 'S';
          local_1028._0_1_ = 'O';
          local_1028._1_1_ = 'N';
          local_1028._2_1_ = ' ';
          local_1028._3_1_ = 'e';
          builtin_strncpy(acStack_1024,"rror",4);
          local_1020._0_1_ = ' ';
          local_1020._1_1_ = 'i';
          local_1020._2_1_ = 'n';
          local_1020._3_1_ = ' ';
          local_101c._0_1_ = 'c';
          local_101c._1_1_ = 'o';
          local_101c._2_1_ = 'n';
          local_101c._3_1_ = 'f';
          local_1018._0_1_ = 'i';
          local_1018._1_1_ = 'g';
          local_1018._2_1_ = 'u';
          local_1018._3_1_ = 'r';
          local_1014._0_1_ = 'a';
          local_1014._1_1_ = 't';
          local_1014._2_1_ = 'i';
          local_1014._3_1_ = 'o';
          local_1010._0_1_ = 'n';
          local_1010._1_1_ = ' ';
          local_1010._2_1_ = 'f';
          local_1010._3_1_ = 'i';
          local_100c._0_1_ = 'l';
          local_100c._1_1_ = 'e';
          local_100c._2_1_ = '.';
          local_100c._3_1_ = '\0';
          FUN_00038730(4,local_11d0,0);
          if ((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) goto LAB_0000cd9c;
        }
        local_1030._0_1_ = 'C';
        local_1030._1_1_ = 'o';
        local_1030._2_1_ = 'n';
        local_1030._3_1_ = 'f';
        local_102c._0_1_ = 'i';
        local_102c._1_1_ = 'g';
        local_102c._2_1_ = 'u';
        local_102c._3_1_ = 'r';
        local_1028._0_1_ = 'a';
        local_1028._1_1_ = 't';
        local_1028._2_1_ = 'i';
        local_1028._3_1_ = 'o';
        builtin_strncpy(acStack_1024,"n fi",4);
        local_1020._0_1_ = 'l';
        local_1020._1_1_ = 'e';
        local_1020._2_1_ = ' ';
        local_1020._3_1_ = 'c';
        local_101c._0_1_ = 'o';
        local_101c._1_1_ = 'u';
        local_101c._2_1_ = 'l';
        local_101c._3_1_ = 'd';
        local_1018._0_1_ = ' ';
        local_1018._1_1_ = 'n';
        local_1018._2_1_ = 'o';
        local_1018._3_1_ = 't';
        local_1014._0_1_ = ' ';
        local_1014._1_1_ = 'b';
        local_1014._2_1_ = 'e';
        local_1014._3_1_ = ' ';
        local_1010._0_1_ = 'u';
        local_1010._1_1_ = 's';
        local_1010._2_1_ = 'e';
        local_1010._3_1_ = 'd';
        local_100c = CONCAT22(local_100c._2_2_,0x2e);
        FUN_00038730(4,local_11d0,0);
      }
LAB_0000cdec:
      free(DAT_00076f24);
      DAT_00076f24 = (char *)0x0;
    }
    pcVar28 = DAT_0007bc74;
    sVar16 = strlen(DAT_0007bc74);
    (pcVar28 + sVar16)[0] = '/';
    (pcVar28 + sVar16)[1] = '\0';
    if (DAT_0007b858 != '\0') {
      DAT_00078e08 = '\x01';
    }
    if (DAT_00075df0 < 0) {
      if (DAT_0007b859 == '\0') {
        DAT_00075df0 = 0x3c;
      }
      else {
        DAT_00075df0 = 0x1e;
      }
    }
    DAT_00078b64 = 8;
    DAT_0007b0fc = calloc(8,0x40);
    if (DAT_0007b0fc == (void *)0x0) goto LAB_0000e128;
    pcVar14 = (char *)0x0;
    DAT_00078b68 = 0;
    FUN_00027cd0(&DAT_0007b728);
    FUN_00027cd0(&DAT_0007acc8);
    FUN_00027cd0(&DAT_0007b7d8);
    FUN_00027cd0(&DAT_0007b448);
    FUN_00027cd0(&DAT_00076ac8);
    FUN_00027cd0(&DAT_0007bca0);
    FUN_00027cd0(&DAT_0007b4d8);
    FUN_00027cd0(&DAT_0007b678);
    FUN_00027cd0(&DAT_0007ba48);
    FUN_00027cd0(&DAT_0007ad68);
    FUN_00027cd0(&DAT_0007bb70);
    FUN_00027cd0(&DAT_0007ae70);
    FUN_00027cd0(&DAT_0007b300);
    FUN_00027cd0(&DAT_0007b258);
    FUN_00027cd0(&DAT_0007af70);
    FUN_00027cd0(&DAT_0007b8c0);
    FUN_00027cd0(&DAT_0007b078);
    FUN_00027cd0(&DAT_0007aff0);
    FUN_00027cd0(&DAT_0007b560);
    FUN_00027cd0(&DAT_0007b3a0);
    FUN_00027cd0(&DAT_0007b1d0);
    FUN_00027cd0(&DAT_0007bbf0);
    FUN_00027cd0(&DAT_0007aef0);
    FUN_00027cd0(&DAT_0007ade8);
    (*DAT_0007b734)(0);
    (*DAT_0007acd4)(0);
    (*DAT_0007b7e4)(0);
    (*DAT_0007b454)(0);
    (*(code *)PTR_FUN_00076ad4)(0);
    (*DAT_0007bcac)(0);
    (*DAT_0007b4e4)(0);
    (*DAT_0007b684)(0);
    (*DAT_0007ba54)(0);
    (*DAT_0007ad74)(0);
    (*DAT_0007bb7c)(0);
    (*DAT_0007ae7c)(0);
    (*DAT_0007b30c)(0);
    (*DAT_0007b264)(0);
    (*DAT_0007af7c)(0);
    (*DAT_0007b8cc)(0);
    (*DAT_0007b084)(0);
    (*DAT_0007affc)(0);
    (*DAT_0007b56c)(0);
    (*DAT_0007b3ac)(0);
    (*DAT_0007b1dc)(0);
    (*DAT_0007bbfc)(0);
    (*DAT_0007aefc)(0);
    (*DAT_0007adf4)(0);
    DAT_0007bb6c = 0;
    if (0 < DAT_0007b720) {
      do {
        piVar27 = DAT_0007b628 + (int)pcVar14;
        pcVar14 = pcVar14 + 1;
        FUN_00027f10(*piVar27);
      } while ((int)pcVar14 < DAT_0007b720);
    }
    if (DAT_0007b720 == 0) goto LAB_0000df08;
    DAT_0007702c = DAT_0007b720;
    if (DAT_00076f00 == (char *)0x0) {
      FUN_0001ab24(&DAT_0007b700,"load_temp_cutoffs",0x480);
      iVar41 = DAT_0007b720;
      piVar27 = DAT_0007b628;
      iVar34 = 0;
      while (iVar34 < iVar41) {
        piVar20 = piVar27 + iVar34;
        iVar34 = iVar34 + 1;
        if (*(int *)(*piVar20 + 0xb8) == 0) {
          *(undefined4 *)(*piVar20 + 0xb8) = 0x5f;
        }
      }
      FUN_00019bbc(&DAT_0007b700,"load_temp_cutoffs",0x486);
    }
    else {
      pcVar28 = strtok(DAT_00076f00,",");
      uVar40 = 0;
      if (pcVar28 == (char *)0x0) goto LAB_0000de30;
      iVar34 = 0;
      do {
        if (DAT_0007b720 <= iVar34) {
          local_1030._0_1_ = 'T';
          local_1030._1_1_ = 'o';
          local_1030._2_1_ = 'o';
          local_1030._3_1_ = ' ';
          local_102c._0_1_ = 'm';
          local_102c._1_1_ = 'a';
          local_102c._2_1_ = 'n';
          local_102c._3_1_ = 'y';
          local_1028._0_1_ = ' ';
          local_1028._1_1_ = 'v';
          local_1028._2_1_ = 'a';
          local_1028._3_1_ = 'l';
          builtin_strncpy(acStack_1024,"ues ",4);
          local_1020._0_1_ = 'p';
          local_1020._1_1_ = 'a';
          local_1020._2_1_ = 's';
          local_1020._3_1_ = 's';
          local_101c._0_1_ = 'e';
          local_101c._1_1_ = 'd';
          local_101c._2_1_ = ' ';
          local_101c._3_1_ = 't';
          local_1018._0_1_ = 'o';
          local_1018._1_1_ = ' ';
          local_1018._2_1_ = 's';
          local_1018._3_1_ = 'e';
          local_1014._0_1_ = 't';
          local_1014._1_1_ = ' ';
          local_1014._2_1_ = 't';
          local_1014._3_1_ = 'e';
          local_1010._0_1_ = 'm';
          local_1010._1_1_ = 'p';
          local_1010._2_1_ = ' ';
          local_1010._3_1_ = 'c';
          local_100c._0_1_ = 'u';
          local_100c._1_1_ = 't';
          local_100c._2_1_ = 'o';
          local_100c._3_1_ = 'f';
          local_1008 = 0x66;
          FUN_00038730(3,local_11d0,1);
          FUN_00016ca8(1);
        }
        uVar45 = strtol(pcVar28,(char **)0x0,10);
        if (200 < uVar45) {
          local_1030._0_1_ = 'I';
          local_1030._1_1_ = 'n';
          local_1030._2_1_ = 'v';
          local_1030._3_1_ = 'a';
          local_102c._0_1_ = 'l';
          local_102c._1_1_ = 'i';
          local_102c._2_1_ = 'd';
          local_102c._3_1_ = ' ';
          local_1028._0_1_ = 'v';
          local_1028._1_1_ = 'a';
          local_1028._2_1_ = 'l';
          local_1028._3_1_ = 'u';
          builtin_strncpy(acStack_1024,"e pa",4);
          local_1020._0_1_ = 's';
          local_1020._1_1_ = 's';
          local_1020._2_1_ = 'e';
          local_1020._3_1_ = 'd';
          local_101c._0_1_ = ' ';
          local_101c._1_1_ = 't';
          local_101c._2_1_ = 'o';
          local_101c._3_1_ = ' ';
          local_1018._0_1_ = 's';
          local_1018._1_1_ = 'e';
          local_1018._2_1_ = 't';
          local_1018._3_1_ = ' ';
          local_1014._0_1_ = 't';
          local_1014._1_1_ = 'e';
          local_1014._2_1_ = 'm';
          local_1014._3_1_ = 'p';
          local_1010._0_1_ = ' ';
          local_1010._1_1_ = 'c';
          local_1010._2_1_ = 'u';
          local_1010._3_1_ = 't';
          local_100c._0_1_ = 'o';
          local_100c._1_1_ = 'f';
          local_100c._2_1_ = 'f';
          local_100c._3_1_ = '\0';
          FUN_00038730(3,local_11d0,1);
          FUN_00016ca8(1);
        }
        FUN_0001ab24(&DAT_0007b700,"load_temp_cutoffs",0x479);
        piVar27 = DAT_0007b628 + iVar34;
        iVar34 = iVar34 + 1;
        *(uint *)(*piVar27 + 0xb8) = uVar45;
        FUN_00019bbc(&DAT_0007b700,"load_temp_cutoffs",0x47b);
        pcVar28 = strtok((char *)0x0,",");
      } while (pcVar28 != (char *)0x0);
      uVar40 = 1;
      if (iVar34 == 1) goto LAB_0000de48;
    }
LAB_0000d430:
    if (0 < DAT_0007b720) {
      piVar20 = DAT_0007b628 + DAT_0007b720;
      piVar27 = DAT_0007b628;
      do {
        piVar32 = piVar27 + 1;
        *(undefined4 *)(*piVar27 + 0x138) = 99999999;
        piVar27 = piVar32;
      } while (piVar32 != piVar20);
    }
    if (DAT_0007b940 == '\0') {
      DAT_0007701c = DAT_0007701c + DAT_0007702c;
      DAT_00077024 = DAT_0007701c + 1;
      FUN_00018068();
    }
    if (DAT_0007b198 == 0) {
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
        local_1030._0_1_ = 'N';
        local_1030._1_1_ = 'e';
        local_1030._2_1_ = 'e';
        local_1030._3_1_ = 'd';
        local_102c._0_1_ = ' ';
        local_102c._1_1_ = 't';
        local_102c._2_1_ = 'o';
        local_102c._3_1_ = ' ';
        local_1028._0_1_ = 's';
        local_1028._1_1_ = 'p';
        local_1028._2_1_ = 'e';
        local_1028._3_1_ = 'c';
        builtin_strncpy(acStack_1024,"ify ",4);
        local_1020._0_1_ = 'a';
        local_1020._1_1_ = 't';
        local_1020._2_1_ = ' ';
        local_1020._3_1_ = 'l';
        local_101c._0_1_ = 'e';
        local_101c._1_1_ = 'a';
        local_101c._2_1_ = 's';
        local_101c._3_1_ = 't';
        local_1018._0_1_ = ' ';
        local_1018._1_1_ = 'o';
        local_1018._2_1_ = 'n';
        local_1018._3_1_ = 'e';
        local_1014._0_1_ = ' ';
        local_1014._1_1_ = 'p';
        local_1014._2_1_ = 'o';
        local_1014._3_1_ = 'o';
        local_1010._0_1_ = 'l';
        local_1010._1_1_ = ' ';
        local_1010._2_1_ = 's';
        local_1010._3_1_ = 'e';
        local_100c._0_1_ = 'r';
        local_100c._1_1_ = 'v';
        local_100c._2_1_ = 'e';
        local_100c._3_1_ = 'r';
        local_1008 = 0x2e;
        FUN_00038730(4,local_11d0,0);
      }
      if ((DAT_00075dd0 == '\0') || (iVar34 = FUN_00026d10(0), iVar34 == 0)) {
        local_1030._0_1_ = 'P';
        local_1030._1_1_ = 'o';
        local_1030._2_1_ = 'o';
        local_1030._3_1_ = 'l';
        local_102c._0_1_ = ' ';
        local_102c._1_1_ = 's';
        local_102c._2_1_ = 'e';
        local_102c._3_1_ = 't';
        local_1028._0_1_ = 'u';
        local_1028._1_1_ = 'p';
        local_1028._2_1_ = ' ';
        local_1028._3_1_ = 'f';
        builtin_strncpy(acStack_1024,"aile",4);
        local_1020._0_1_ = 'd';
        local_1020._1_1_ = '\0';
        FUN_00038730(3,local_11d0,1);
        FUN_0001d8fc(1,0);
      }
    }
    if (DAT_0007b198 < 1) goto LAB_0000d76c;
    uVar40 = 0;
LAB_0000d5d0:
    puVar43 = (undefined1 *)DAT_0007b438[uVar40];
    *(undefined4 *)(puVar43 + 0x1ac) = 99999999;
    *(undefined4 *)(puVar43 + 0x1d0) = 99999999;
    if (*(int *)(puVar43 + 200) == 0) {
      if (*(int *)(puVar43 + 0xd0) == 0) goto LAB_0000f5f4;
      do {
        pcVar28 = *(char **)(puVar43 + 0xcc);
        if (pcVar28 != (char *)0x0) {
          sVar16 = strlen(pcVar28);
          pcVar15 = *(char **)(puVar43 + 0xd0);
          sVar21 = strlen(pcVar15);
          sVar16 = sVar16 + sVar21 + 2;
          pcVar14 = malloc(sVar16);
          *(char **)(puVar43 + 200) = pcVar14;
          if (pcVar14 != (char *)0x0) goto code_r0x0000d648;
          local_1030._0_1_ = 'F';
          local_1030._1_1_ = 'a';
          local_1030._2_1_ = 'i';
          local_1030._3_1_ = 'l';
          local_102c._0_1_ = 'e';
          local_102c._1_1_ = 'd';
          local_102c._2_1_ = ' ';
          local_102c._3_1_ = 't';
          local_1028._0_1_ = 'o';
          local_1028._1_1_ = ' ';
          local_1028._2_1_ = 'm';
          local_1028._3_1_ = 'a';
          builtin_strncpy(acStack_1024,"lloc",4);
          local_1020._0_1_ = ' ';
          local_1020._1_1_ = 'u';
          local_1020._2_1_ = 's';
          local_1020._3_1_ = 'e';
          local_101c._0_1_ = 'r';
          local_101c._1_1_ = 'p';
          local_101c._2_1_ = 'a';
          local_101c._3_1_ = 's';
          local_1018 = CONCAT22(local_1018._2_2_,0x73);
          FUN_00038730(3,local_11d0,1);
          FUN_0001d8fc(1,0);
          puVar37 = (undefined *)0x0;
        }
        puVar18 = (undefined4 *)(puVar43 + 0xc4);
        puVar43 = auStackY_2004;
        *psVar48 = *puVar18;
        snprintf((char *)local_11d0,0x1000,"No login credentials supplied for pool %u %s",uVar40,
                 (char *)*psVar48);
        FUN_00038730(3,local_11d0,1);
        FUN_0001d8fc(1,pcVar28);
LAB_0000f5f4:
        pvVar12 = calloc(1,1);
        *(void **)(puVar43 + 0xd0) = pvVar12;
      } while( true );
    }
    goto LAB_0000d5b8;
  case 2:
    goto switchD_0000be78_caseD_2;
  case 3:
    uVar45 = *(uint *)(pbVar35 + 0x60);
    goto LAB_0000befc;
  case 4:
    uVar45 = *(uint *)(pbVar35 + 0x60);
    goto LAB_0000bef4;
  case 5:
    uVar45 = *(uint *)(pbVar35 + 0x60);
    goto LAB_0000beec;
  case 6:
    uVar45 = *(uint *)(pbVar35 + 0x60);
    goto LAB_0000bee4;
  case 7:
    uVar45 = *(uint *)(pbVar35 + 0x60);
    goto LAB_0000bedc;
  case 8:
    uVar45 = *(uint *)(pbVar35 + 0x60);
    goto LAB_0000bed4;
  case 9:
    iVar34 = *(int *)(pbVar35 + 0x60);
    break;
  case 0xb:
    *(uint *)(pbVar35 + 0x60) = uVar45 + sVar16 + (uint)pbVar38[10] * 0x1000000;
  case 10:
    iVar34 = *(int *)(pbVar35 + 0x60) + (uint)pbVar38[9] * 0x10000;
    *(int *)(pbVar35 + 0x60) = iVar34;
    break;
  default:
    uVar45 = *(uint *)(pbVar35 + 0x60);
    goto LAB_0000bf14;
  }
  uVar45 = iVar34 + (uint)pbVar38[8] * 0x100;
  *(uint *)(pbVar35 + 0x60) = uVar45;
LAB_0000bed4:
  pvVar12 = (void *)((int)pvVar12 + (uint)pbVar38[7] * 0x1000000);
LAB_0000bedc:
  pvVar12 = (void *)((int)pvVar12 + (uint)pbVar38[6] * 0x10000);
LAB_0000bee4:
  pvVar12 = (void *)((int)pvVar12 + (uint)pbVar38[5] * 0x100);
LAB_0000beec:
  pvVar12 = (void *)((int)pvVar12 + (uint)pbVar38[4]);
LAB_0000bef4:
  uVar40 = uVar40 + (uint)pbVar38[3] * 0x1000000;
LAB_0000befc:
  uVar40 = uVar40 + (uint)pbVar38[2] * 0x10000;
  goto LAB_0000bf04;
LAB_0000e128:
  local_1030._0_1_ = 'F';
  local_1030._1_1_ = 'a';
  local_1030._2_1_ = 'i';
  local_1030._3_1_ = 'l';
  local_102c._0_1_ = 'e';
  local_102c._1_1_ = 'd';
  local_102c._2_1_ = ' ';
  local_102c._3_1_ = 't';
  local_1028._0_1_ = 'o';
  local_1028._1_1_ = ' ';
  local_1028._2_1_ = 'c';
  local_1028._3_1_ = 'a';
  builtin_strncpy(acStack_1024,"lloc",4);
  local_1020._0_1_ = ' ';
  local_1020._1_1_ = 'c';
  local_1020._2_1_ = 'o';
  local_1020._3_1_ = 'n';
  local_101c._0_1_ = 't';
  local_101c._1_1_ = 'r';
  local_101c._2_1_ = 'o';
  local_101c._3_1_ = 'l';
  local_1018._0_1_ = '_';
  local_1018._1_1_ = 't';
  local_1018._2_1_ = 'h';
  local_1018._3_1_ = 'r';
  local_1014 = local_1014 & 0xffffff00;
  FUN_00038730(3,local_11d0,1);
  FUN_0001d8fc(1,0);
  psVar47 = psVar48;
LAB_0000e168:
  local_1030._0_1_ = 'F';
  local_1030._1_1_ = 'a';
  local_1030._2_1_ = 'i';
  local_1030._3_1_ = 'l';
  local_102c._0_1_ = 'e';
  local_102c._1_1_ = 'd';
  local_102c._2_1_ = ' ';
  local_102c._3_1_ = 't';
  local_1028._0_1_ = 'o';
  local_1028._1_1_ = ' ';
  local_1028._2_1_ = 'p';
  local_1028._3_1_ = 't';
  acStack_1024[0] = 'h';
  acStack_1024[1] = 'r';
  acStack_1024[2] = 'e';
  acStack_1024[3] = 'a';
  local_1020._0_1_ = 'd';
  local_1020._1_1_ = '_';
  local_1020._2_1_ = 'c';
  local_1020._3_1_ = 'o';
  local_101c._0_1_ = 'n';
  local_101c._1_1_ = 'd';
  local_101c._2_1_ = '_';
  local_101c._3_1_ = 'i';
  local_1018._0_1_ = 'n';
  local_1018._1_1_ = 'i';
  local_1018._2_1_ = 't';
  local_1018._3_1_ = ' ';
  local_1014._0_1_ = 'l';
  local_1014._1_1_ = 'p';
  local_1014._2_1_ = '_';
  local_1014._3_1_ = 'c';
  local_1010._0_1_ = 'o';
  local_1010._1_1_ = 'n';
  local_1010._2_1_ = 'd';
  local_1010._3_1_ = '\0';
  FUN_00038730(3,&local_1030,1);
  FUN_0001d8fc(1,ppFVar39);
LAB_0000e1ac:
  local_1028._0_1_ = 'o';
  local_1028._1_1_ = ' ';
  local_1028._2_1_ = 'p';
  local_1028._3_1_ = 't';
  acStack_1024[0] = 'h';
  acStack_1024[1] = 'r';
  acStack_1024[2] = 'e';
  acStack_1024[3] = 'a';
  local_1020._0_1_ = 'd';
  local_1020._1_1_ = '_';
  local_1020._2_1_ = 'c';
  local_1020._3_1_ = 'o';
  local_101c._0_1_ = 'n';
  local_101c._1_1_ = 'd';
  local_101c._2_1_ = '_';
  local_101c._3_1_ = 'i';
  local_1018._0_1_ = 'n';
  local_1018._1_1_ = 'i';
  local_1018._2_1_ = 't';
  local_1018._3_1_ = ' ';
  local_1014._0_1_ = 'g';
  local_1014._1_1_ = 'w';
  local_1014._2_1_ = 's';
  local_1014._3_1_ = '_';
  local_1010._0_1_ = 'c';
  local_1010._1_1_ = 'o';
  local_1010._2_1_ = 'n';
  local_1010._3_1_ = 'd';
  local_100c = local_100c & 0xffffff00;
  psVar48 = psVar47;
LAB_0000da08:
  local_102c._0_1_ = 'e';
  local_102c._1_1_ = 'd';
  local_102c._2_1_ = ' ';
  local_102c._3_1_ = 't';
  local_1030._0_1_ = 'F';
  local_1030._1_1_ = 'a';
  local_1030._2_1_ = 'i';
  local_1030._3_1_ = 'l';
  FUN_00038730(3,&local_1030,1);
  FUN_0001d8fc(1,ppFVar39);
  goto LAB_0000c524;
LAB_0000df08:
  local_1030._0_1_ = 'A';
  local_1030._1_1_ = 'l';
  local_1030._2_1_ = 'l';
  local_1030._3_1_ = ' ';
  local_102c._0_1_ = 'd';
  local_102c._1_1_ = 'e';
  local_102c._2_1_ = 'v';
  local_102c._3_1_ = 'i';
  local_1028._0_1_ = 'c';
  local_1028._1_1_ = 'e';
  local_1028._2_1_ = 's';
  local_1028._3_1_ = ' ';
  builtin_strncpy(acStack_1024,"disa",4);
  local_1020._0_1_ = 'b';
  local_1020._1_1_ = 'l';
  local_1020._2_1_ = 'e';
  local_1020._3_1_ = 'd';
  local_101c._0_1_ = ',';
  local_101c._1_1_ = ' ';
  local_101c._2_1_ = 'c';
  local_101c._3_1_ = 'a';
  local_1018._0_1_ = 'n';
  local_1018._1_1_ = 'n';
  local_1018._2_1_ = 'o';
  local_1018._3_1_ = 't';
  local_1014._0_1_ = ' ';
  local_1014._1_1_ = 'm';
  local_1014._2_1_ = 'i';
  local_1014._3_1_ = 'n';
  local_1010._0_1_ = 'e';
  local_1010._1_1_ = '!';
  FUN_00038730(3,local_11d0,1);
  FUN_0001d8fc(1,0);
  ppFVar39 = (FILE **)0x0;
  iVar34 = extraout_r3_00;
LAB_0000df54:
  sprintf(pcVar14,"%s%02d.txt",DAT_00076be8,iVar34);
  pFVar19 = fopen(pcVar14,"a+");
  bVar49 = DAT_0007ad48 != '\0';
  ppFVar39[-0x9e] = pFVar19;
  if (((bVar49) || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
    *psVar48 = (size_t)pcVar14;
    snprintf((char *)local_11d0,0x1000,"Log work open asic %d file %s",DAT_00078950,(char *)*psVar48
            );
    FUN_00038730(3,local_11d0,0);
  }
  goto LAB_0000cb58;
code_r0x0000d648:
  *psVar48 = (size_t)pcVar15;
  snprintf(pcVar14,sVar16,"%s:%s",pcVar28,(char *)*psVar48);
LAB_0000d5b8:
  uVar40 = uVar40 + 1;
  if (DAT_0007b198 <= (int)uVar40) goto LAB_0000d76c;
  goto LAB_0000d5d0;
LAB_0000d76c:
  DAT_0007705c = *DAT_0007b438;
  if (DAT_0007ad48 != '\0') {
    openlog("cgminer",1,8);
  }
  if (*(int *)(puVar37 + -100) != 0) {
    iVar34 = pipe(local_11d0);
    if (iVar34 < 0) {
      perror("pipe - failed to create pipe for --monitor");
                    /* WARNING: Subroutine does not return */
      exit(1);
    }
    fflush(stderr);
    iVar34 = dup2(local_102c,2);
    if (iVar34 < 0) {
      perror("dup2 - failed to alias stderr to write end of pipe for --monitor");
                    /* WARNING: Subroutine does not return */
      exit(1);
    }
    iVar34 = close(local_102c);
    if (iVar34 < 0) {
      perror("close - failed to close write end of pipe for --monitor");
                    /* WARNING: Subroutine does not return */
      exit(1);
    }
    p_Var22 = signal(0xd,(__sighandler_t)0x1);
    p_Var23 = signal(0xd,(__sighandler_t)0x1);
    if (p_Var23 == (__sighandler_t)0xffffffff || p_Var22 == (__sighandler_t)0xffffffff) {
      perror("signal - failed to edit signal mask for --monitor");
                    /* WARNING: Subroutine does not return */
      exit(1);
    }
    DAT_0007728c = fork();
    if (DAT_0007728c < 0) {
      perror("fork - failed to fork child process for --monitor");
                    /* WARNING: Subroutine does not return */
      exit(1);
    }
    if (DAT_0007728c == 0) {
      iVar34 = dup2(local_1030,0);
      if (iVar34 < 0) {
        perror("dup2 - in child, failed to alias read end of pipe to stdin for --monitor");
                    /* WARNING: Subroutine does not return */
        exit(1);
      }
      close(local_1030);
      *psVar48 = 0;
      execl("/bin/bash","/bin/bash",&DAT_000637bc,DAT_00078b6c);
      perror("execl - in child failed to exec user specified command for --monitor");
                    /* WARNING: Subroutine does not return */
      exit(1);
    }
    iVar34 = close(local_1030);
    if (iVar34 < 0) {
      perror("close - failed to close read end of pipe for --monitor");
                    /* WARNING: Subroutine does not return */
      exit(1);
    }
  }
  DAT_0007b5e0 = calloc(DAT_0007bb6c,4);
  if (DAT_0007b5e0 == (int *)0x0) {
    local_1030._0_1_ = 'F';
    local_1030._1_1_ = 'a';
    local_1030._2_1_ = 'i';
    local_1030._3_1_ = 'l';
    local_102c._0_1_ = 'e';
    local_102c._1_1_ = 'd';
    local_102c._2_1_ = ' ';
    local_102c._3_1_ = 't';
    local_1028._0_1_ = 'o';
    local_1028._1_1_ = ' ';
    local_1028._2_1_ = 'c';
    local_1028._3_1_ = 'a';
    builtin_strncpy(acStack_1024,"lloc",4);
    local_1020._0_1_ = ' ';
    local_1020._1_1_ = 'm';
    local_1020._2_1_ = 'i';
    local_1020._3_1_ = 'n';
    local_101c._0_1_ = 'i';
    local_101c._1_1_ = 'n';
    local_101c._2_1_ = 'g';
    local_101c._3_1_ = '_';
    local_1018._0_1_ = 't';
    local_1018._1_1_ = 'h';
    local_1018._2_1_ = 'r';
    local_1018._3_1_ = '\0';
    piVar27 = (int *)0x0;
  }
  else {
    iVar34 = 0;
    while( true ) {
      piVar27 = DAT_0007b5e0;
      if ((int)DAT_0007bb6c <= iVar34) {
        iVar24 = 0;
        iVar41 = 0;
        do {
          piVar27 = DAT_0007b438;
          iVar34 = DAT_0007b198;
          if (DAT_0007b720 <= iVar41) {
            if (DAT_0007730d != '\0') goto LAB_0000dc70;
            if (DAT_00076ffc != 0) goto LAB_0000dc70;
            iVar41 = 0;
            do {
              if (iVar34 <= iVar41) goto LAB_0000f09c;
              iVar24 = piVar27[iVar41];
              iVar41 = iVar41 + 1;
              *(undefined1 *)(iVar24 + 0x81) = 1;
              if (*(int *)(iVar24 + 0x84) != 1) {
                *(undefined4 *)(iVar24 + 0x84) = 1;
                DAT_0007b5e4 = DAT_0007b5e4 + 1;
              }
            } while( true );
          }
          iVar34 = 0;
          puVar36 = (undefined *)DAT_0007b628[iVar41];
          iVar44 = *(int *)(puVar36 + 0x94);
          pvVar12 = malloc((iVar44 + 1) * 4);
          *(void **)(puVar36 + 0x98) = pvVar12;
          *(undefined4 *)((int)pvVar12 + iVar44 * 4) = 0;
          *(undefined4 *)(puVar36 + 0x60) = 4;
          if (0 < *(int *)(puVar36 + 0x94)) {
            do {
              piVar27 = (int *)FUN_0001b124(iVar24);
              *piVar27 = iVar24;
              piVar27[9] = (int)puVar36;
              piVar27[1] = iVar34;
              iVar44 = (**(code **)(*(int *)(puVar36 + 4) + 0x2c))();
              if (iVar44 != 0) {
                iVar44 = FUN_0002f174(piVar27,0,FUN_00011e24,piVar27);
                if (iVar44 != 0) {
                  snprintf((char *)local_11d0,0x1000,"thread %d create failed",*piVar27);
                  FUN_00038730(3,local_11d0,1);
                  FUN_0001d8fc(1,0);
                  puVar37 = puVar36;
                  goto LAB_0000f19c;
                }
                *(int **)(*(int *)(puVar36 + 0x98) + iVar34 * 4) = piVar27;
                if (*(int *)(puVar36 + 0x20) != 1) {
                  if ((DAT_00078e09 != '\0') &&
                     (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
                    snprintf((char *)local_11d0,0x1000,"Pushing sem post to thread %d",*piVar27);
                    FUN_00038730(7,local_11d0,0);
                  }
                  FUN_0002f1bc(piVar27 + 4,"cgminer.c",&DAT_0005db00,0x2b5a);
                }
              }
              iVar34 = iVar34 + 1;
              iVar24 = iVar24 + 1;
            } while (iVar34 < *(int *)(puVar36 + 0x94));
          }
          iVar41 = iVar41 + 1;
        } while( true );
      }
      pvVar12 = calloc(1,0x40);
      piVar27[iVar34] = (int)pvVar12;
      piVar27 = (int *)0x0;
      if (DAT_0007b5e0[iVar34] == 0) break;
      iVar34 = iVar34 + 1;
    }
LAB_0000f19c:
    snprintf((char *)local_11d0,0x1000,"Failed to calloc mining_thr[%d]",iVar34);
  }
  FUN_00038730(3,local_11d0,1);
  FUN_0001d8fc(1,piVar27);
  do {
    iVar34 = 0x3c;
    do {
      sleep(1);
      if (DAT_0007730c != '\0') break;
      iVar34 = iVar34 + -1;
    } while (iVar34 != 0);
    iVar34 = 0;
    while (DAT_0007730c == '\0') {
      if (iVar34 == 0) {
        if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
LAB_0000f2bc:
          if (2 < DAT_000766c4) goto LAB_0000f2c8;
LAB_0000f528:
          if (2 < DAT_000766c4) goto LAB_0000f304;
        }
        else {
          local_1030._0_1_ = 'N';
          local_1030._1_1_ = 'o';
          local_1030._2_1_ = ' ';
          local_1030._3_1_ = 's';
          local_102c._0_1_ = 'e';
          local_102c._1_1_ = 'r';
          local_102c._2_1_ = 'v';
          local_102c._3_1_ = 'e';
          local_1028._0_1_ = 'r';
          local_1028._1_1_ = 's';
          local_1028._2_1_ = ' ';
          local_1028._3_1_ = 'w';
          builtin_strncpy(acStack_1024,"ere ",4);
          local_1020._0_1_ = 'f';
          local_1020._1_1_ = 'o';
          local_1020._2_1_ = 'u';
          local_1020._3_1_ = 'n';
          local_101c._0_1_ = 'd';
          local_101c._1_1_ = ' ';
          local_101c._2_1_ = 't';
          local_101c._3_1_ = 'h';
          local_1018._0_1_ = 'a';
          local_1018._1_1_ = 't';
          local_1018._2_1_ = ' ';
          local_1018._3_1_ = 'c';
          local_1014._0_1_ = 'o';
          local_1014._1_1_ = 'u';
          local_1014._2_1_ = 'l';
          local_1014._3_1_ = 'd';
          local_1010._0_1_ = ' ';
          local_1010._1_1_ = 'b';
          local_1010._2_1_ = 'e';
          local_1010._3_1_ = ' ';
          local_100c._0_1_ = 'u';
          local_100c._1_1_ = 's';
          local_100c._2_1_ = 'e';
          local_100c._3_1_ = 'd';
          local_1008 = 0x20;
          uStack_1004._0_1_ = 'g';
          uStack_1004._1_1_ = 'e';
          uStack_1004._2_1_ = 't';
          uStack_1004._3_1_ = ' ';
          local_1000._0_1_ = 'w';
          local_1000._1_1_ = 'o';
          local_1000._2_1_ = 'r';
          local_1000._3_1_ = 'k';
          builtin_strncpy(acStack_ffc," from.",7);
          FUN_00038730(3,local_11d0,0);
          if ((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) goto LAB_0000f2bc;
LAB_0000f2c8:
          memcpy(local_11d0,
                 "Please check the details from the list below of the servers you have input",0x4b);
          FUN_00038730(3,local_11d0,0);
          if ((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) goto LAB_0000f528;
LAB_0000f304:
          memcpy(local_11d0,
                 "Most likely you have input the wrong URL, forgotten to add a port, or have not set up workers"
                 ,0x5e);
          FUN_00038730(3,local_11d0,0);
        }
        for (iVar34 = 0; iVar34 < DAT_0007b198; iVar34 = iVar34 + 1) {
          iVar41 = DAT_0007b438[iVar34];
          if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
            *psVar48 = *(undefined4 *)(iVar41 + 0xc4);
            psVar48[1] = *(size_t *)(iVar41 + 0xcc);
            psVar48[2] = *(size_t *)(iVar41 + 0xd0);
            snprintf((char *)local_11d0,0x1000,"Pool: %d  URL: %s  User: %s  Password: %s",iVar34,
                     (char *)*psVar48,(char *)psVar48[1],(char *)psVar48[2]);
            FUN_00038730(4,local_11d0,0);
          }
        }
        if (DAT_00075dd0 != '\0') {
          if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
            iVar34 = 1;
            memcpy(local_11d0,
                   "Press any key to exit, or cgminer will wait indefinitely for an alive pool.",
                   0x4c);
            FUN_00038730(3,local_11d0,0);
            goto LAB_0000f460;
          }
          iVar34 = 1;
          goto LAB_0000f474;
        }
LAB_0000f4cc:
        local_1030._0_1_ = 'N';
        local_1030._1_1_ = 'o';
        local_1030._2_1_ = ' ';
        local_1030._3_1_ = 's';
        local_102c._0_1_ = 'e';
        local_102c._1_1_ = 'r';
        local_102c._2_1_ = 'v';
        local_102c._3_1_ = 'e';
        local_1028._0_1_ = 'r';
        local_1028._1_1_ = 's';
        local_1028._2_1_ = ' ';
        local_1028._3_1_ = 'c';
        builtin_strncpy(acStack_1024,"ould",4);
        local_1020._0_1_ = ' ';
        local_1020._1_1_ = 'b';
        local_1020._2_1_ = 'e';
        local_1020._3_1_ = ' ';
        local_101c._0_1_ = 'u';
        local_101c._1_1_ = 's';
        local_101c._2_1_ = 'e';
        local_101c._3_1_ = 'd';
        local_1018._0_1_ = '!';
        local_1018._1_1_ = ' ';
        local_1018._2_1_ = 'E';
        local_1018._3_1_ = 'x';
        local_1014._0_1_ = 'i';
        local_1014._1_1_ = 't';
        local_1014._2_1_ = 'i';
        local_1014._3_1_ = 'n';
        local_1010._0_1_ = 'g';
        local_1010._1_1_ = '.';
        FUN_00038730(3,local_11d0,1);
        FUN_0001d8fc(0,0);
      }
      else {
LAB_0000f460:
        if (DAT_00075dd0 == '\0') goto LAB_0000f4cc;
LAB_0000f474:
        if (DAT_0007b430 == 0) {
          iVar41 = -1;
        }
        else {
          iVar41 = *(short *)(DAT_0007b430 + 4) + 1;
        }
        wtouchln(DAT_0007b430,0,iVar41,1);
        wrefresh(DAT_0007b430);
        halfdelay(10);
        iVar41 = wgetch(stdscr);
        if (iVar41 != -1) goto LAB_0000f4cc;
      }
      cbreak();
    }
LAB_0000dc70:
    _DAT_0007acc0 = 0;
    DAT_0007acc4 = 0;
    puVar18 = &DAT_000770e0;
    do {
      puVar25 = puVar18 + 2;
      *puVar18 = 0;
      puVar18[1] = 0;
      iVar34 = DAT_0007b720;
      piVar27 = DAT_0007b628;
      puVar18 = puVar25;
    } while (puVar25 != &DAT_00077140);
    DAT_00077140 = 0;
    for (iVar41 = 0; iVar41 < iVar34; iVar41 = iVar41 + 1) {
      iVar24 = piVar27[iVar41];
      *(undefined4 *)(iVar24 + 0x50) = 0;
      *(undefined4 *)(iVar24 + 0x54) = 0;
      *(undefined4 *)(iVar24 + 0x30) = 0;
      *(undefined4 *)(iVar24 + 0x34) = 0;
    }
    FUN_0002a84c(&DAT_000770d0);
    FUN_0002a84c(&DAT_00077148);
    FUN_0002a84c(&DAT_000770d8);
    FUN_00015264(&DAT_00077260,0x28,&DAT_000770d0);
    pvVar12 = DAT_0007b0fc;
    DAT_000770b0 = 2;
    iVar34 = FUN_0002f174((int)DAT_0007b0fc + 0x80,0,FUN_00021850,0);
    if (iVar34 == 0) break;
    local_1030._0_1_ = 'w';
    local_1030._1_1_ = 'a';
    local_1030._2_1_ = 't';
    local_1030._3_1_ = 'c';
    local_102c._0_1_ = 'h';
    local_102c._1_1_ = 'p';
    local_102c._2_1_ = 'o';
    local_102c._3_1_ = 'o';
    local_1028._0_1_ = 'l';
    local_1028._1_1_ = ' ';
    local_1028._2_1_ = 't';
    local_1028._3_1_ = 'h';
    builtin_strncpy(acStack_1024,"read",4);
    local_1020._0_1_ = ' ';
    local_1020._1_1_ = 'c';
    local_1020._2_1_ = 'r';
    local_1020._3_1_ = 'e';
    local_101c._0_1_ = 'a';
    local_101c._1_1_ = 't';
    local_101c._2_1_ = 'e';
    local_101c._3_1_ = ' ';
    local_1018._0_1_ = 'f';
    local_1018._1_1_ = 'a';
    local_1018._2_1_ = 'i';
    local_1018._3_1_ = 'l';
    local_1014 = CONCAT22(local_1014._2_2_,0x6465);
    FUN_00038730(3,local_11d0,1);
    FUN_0001d8fc(1,0);
LAB_0000f09c:
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      local_1030._0_1_ = 'P';
      local_1030._1_1_ = 'r';
      local_1030._2_1_ = 'o';
      local_1030._3_1_ = 'b';
      local_102c._0_1_ = 'i';
      local_102c._1_1_ = 'n';
      local_102c._2_1_ = 'g';
      local_102c._3_1_ = ' ';
      local_1028._0_1_ = 'f';
      local_1028._1_1_ = 'o';
      local_1028._2_1_ = 'r';
      local_1028._3_1_ = ' ';
      builtin_strncpy(acStack_1024,"an a",4);
      local_1020._0_1_ = 'l';
      local_1020._1_1_ = 'i';
      local_1020._2_1_ = 'v';
      local_1020._3_1_ = 'e';
      local_101c._0_1_ = ' ';
      local_101c._1_1_ = 'p';
      local_101c._2_1_ = 'o';
      local_101c._3_1_ = 'o';
      local_1018 = CONCAT22(local_1018._2_2_,0x6c);
      FUN_00038730(7,local_11d0,0);
    }
    for (iVar34 = 0; iVar34 < DAT_0007b198; iVar34 = iVar34 + 1) {
      pvVar12 = (void *)DAT_0007b438[iVar34];
      *(undefined1 *)((int)pvVar12 + 0x13c) = 1;
      pthread_create((pthread_t *)((int)pvVar12 + 0x138),(pthread_attr_t *)0x0,
                     (__start_routine *)&LAB_0001fe0c,pvVar12);
    }
  } while( true );
  pthread_detach(*(pthread_t *)((int)pvVar12 + 0x8c));
  pvVar12 = DAT_0007b0fc;
  DAT_000770b4 = 3;
  iVar34 = FUN_0002f174((int)DAT_0007b0fc + 0xc0,0,FUN_00023344,0);
  if (iVar34 != 0) {
    local_1030._0_1_ = 'w';
    local_1030._1_1_ = 'a';
    local_1030._2_1_ = 't';
    local_1030._3_1_ = 'c';
    local_102c._0_1_ = 'h';
    local_102c._1_1_ = 'd';
    local_102c._2_1_ = 'o';
    local_102c._3_1_ = 'g';
    local_1028._0_1_ = ' ';
    local_1028._1_1_ = 't';
    local_1028._2_1_ = 'h';
    local_1028._3_1_ = 'r';
    builtin_strncpy(acStack_1024,"ead ",4);
    local_1020._0_1_ = 'c';
    local_1020._1_1_ = 'r';
    local_1020._2_1_ = 'e';
    local_1020._3_1_ = 'a';
    local_101c._0_1_ = 't';
    local_101c._1_1_ = 'e';
    local_101c._2_1_ = ' ';
    local_101c._3_1_ = 'f';
    local_1018._0_1_ = 'a';
    local_1018._1_1_ = 'i';
    local_1018._2_1_ = 'l';
    local_1018._3_1_ = 'e';
    local_1014 = CONCAT22(local_1014._2_2_,100);
    FUN_00038730(3,local_11d0,1);
    FUN_0001d8fc(1,0);
LAB_0000e2a0:
    iVar34 = nice(-10);
    if (((iVar34 == 0) && (DAT_00078e09 != '\0')) &&
       ((DAT_0007ad48 != '\0' || ((DAT_00078e08 != '\0' || (6 < DAT_000766c4)))))) {
      local_1030._0_1_ = 'U';
      local_1030._1_1_ = 'n';
      local_1030._2_1_ = 'a';
      local_1030._3_1_ = 'b';
      local_102c._0_1_ = 'l';
      local_102c._1_1_ = 'e';
      local_102c._2_1_ = ' ';
      local_102c._3_1_ = 't';
      local_1028._0_1_ = 'o';
      local_1028._1_1_ = ' ';
      local_1028._2_1_ = 's';
      local_1028._3_1_ = 'e';
      builtin_strncpy(acStack_1024,"t th",4);
      local_1020._0_1_ = 'r';
      local_1020._1_1_ = 'e';
      local_1020._2_1_ = 'a';
      local_1020._3_1_ = 'd';
      local_101c._0_1_ = ' ';
      local_101c._1_1_ = 't';
      local_101c._2_1_ = 'o';
      local_101c._3_1_ = ' ';
      local_1018._0_1_ = 'h';
      local_1018._1_1_ = 'i';
      local_1018._2_1_ = 'g';
      local_1018._3_1_ = 'h';
      local_1014._0_1_ = ' ';
      local_1014._1_1_ = 'p';
      local_1014._2_1_ = 'r';
      local_1014._3_1_ = 'i';
      local_1010._0_1_ = 'o';
      local_1010._1_1_ = 'r';
      local_1010._2_1_ = 'i';
      local_1010._3_1_ = 't';
      local_100c = CONCAT22(local_100c._2_2_,0x79);
      FUN_00038730(7,local_11d0,0);
    }
    local_11cc = 0;
LAB_0000e368:
    while( true ) {
      iVar34 = DAT_00075dd8;
      if (DAT_0007b8ac != '\0') {
        if ((DAT_00078e09 != '\0') &&
           (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
          local_1030._0_1_ = 'W';
          local_1030._1_1_ = 'o';
          local_1030._2_1_ = 'r';
          local_1030._3_1_ = 'k';
          local_102c._0_1_ = ' ';
          local_102c._1_1_ = 'u';
          local_102c._2_1_ = 'p';
          local_102c._3_1_ = 'd';
          local_1028._0_1_ = 'a';
          local_1028._1_1_ = 't';
          local_1028._2_1_ = 'e';
          local_1028._3_1_ = ' ';
          builtin_strncpy(acStack_1024,"mess",4);
          local_1020._0_1_ = 'a';
          local_1020._1_1_ = 'g';
          local_1020._2_1_ = 'e';
          local_1020._3_1_ = ' ';
          local_101c._0_1_ = 'r';
          local_101c._1_1_ = 'e';
          local_101c._2_1_ = 'c';
          local_101c._3_1_ = 'e';
          local_1018._0_1_ = 'i';
          local_1018._1_1_ = 'v';
          local_1018._2_1_ = 'e';
          local_1018._3_1_ = 'd';
          local_1014 = local_1014 & 0xffffff00;
          FUN_00038730(7,local_11d0,0);
        }
        FUN_0002a84c(&DAT_00076f0c);
        iVar41 = pthread_rwlock_rdlock((pthread_rwlock_t *)&DAT_0007b650);
        if (iVar41 != 0) {
          FUN_0000b69c("signal_work_update",0x14c2);
        }
        if (0 < (int)DAT_0007bb6c) {
          piVar20 = DAT_0007b5e0 + DAT_0007bb6c;
          piVar27 = DAT_0007b5e0;
          do {
            piVar32 = piVar27 + 1;
            *(undefined1 *)(*piVar27 + 0x3f) = 1;
            piVar27 = piVar32;
          } while (piVar32 != piVar20);
        }
        iVar41 = pthread_rwlock_unlock((pthread_rwlock_t *)&DAT_0007b650);
        if (iVar41 != 0) {
          FUN_0000b630("signal_work_update",0x14c5);
        }
        (*DAT_00075ed8)();
      }
      DAT_0007b8ac = '\0';
      FUN_0001da58();
      iVar41 = pthread_mutex_lock(DAT_0007704c);
      if (iVar41 != 0) {
        FUN_0000b414(&DAT_0005db00,0x2be3);
      }
      iVar41 = DAT_00077050;
      if (DAT_00077050 != 0) {
        iVar41 = *(int *)(*(int *)(DAT_00077050 + 0x728) + 0xc);
      }
      if (iVar34 < iVar41) {
        if ((DAT_00077056 != '\0') && (DAT_00075dd8 < DAT_00075ddc)) {
          DAT_00075dd8 = DAT_00075dd8 + 1;
          DAT_00077056 = '\0';
        }
        DAT_00077055 = 1;
        pthread_cond_wait((pthread_cond_t *)&DAT_0007b1a0,DAT_0007704c);
        iVar41 = DAT_00077050;
        if (DAT_00077050 != 0) {
          iVar41 = *(int *)(*(int *)(DAT_00077050 + 0x728) + 0xc);
        }
      }
      iVar24 = pthread_mutex_unlock(DAT_0007704c);
      if (iVar24 != 0) {
        FUN_0000b5c4(&DAT_0005db00,0x2bf5);
      }
      (*DAT_00075ed8)();
      if (iVar41 <= iVar34) break;
      if ((DAT_00077056 != '\0') && (DAT_00075dd8 < DAT_00075ddc)) {
        DAT_00075dd8 = DAT_00075dd8 + 1;
        DAT_00077056 = '\0';
      }
      DAT_00077055 = 1;
      local_11cc = FUN_000193b4(0);
      if (local_11cc != 0) {
        FUN_000159b4();
        local_11cc = 0;
      }
    }
    if (local_11cc != 0) {
      FUN_000159b4(local_11cc);
    }
    iVar41 = FUN_0001a3d4();
    pbVar35 = (byte *)FUN_0001da58();
    iVar34 = DAT_0007b198;
    if (DAT_00076efc == 4) {
      pbVar35 = (byte *)FUN_00012250();
    }
    else if (DAT_00076efc == 3) {
      if (DAT_0007b198 < 1) {
LAB_0000ec10:
        iVar24 = *(int *)(puVar37 + -0x5c);
        *(int *)(puVar37 + -0x5c) = iVar24 + 1;
        if (iVar34 <= iVar24 + 1) {
          DAT_00078b74 = 0;
        }
      }
      else if (*(int *)(*DAT_0007b438 + 0x3c) <= *(int *)(*DAT_0007b438 + 0x40)) {
        iVar24 = 0;
        piVar27 = DAT_0007b438;
LAB_0000ebec:
        iVar24 = iVar24 + 1;
        if (iVar24 != DAT_0007b198) goto LAB_0000ebd8;
        piVar20 = DAT_0007b438 + DAT_0007b198;
        piVar27 = DAT_0007b438;
        do {
          piVar32 = piVar27 + 1;
          *(undefined4 *)(*piVar27 + 0x40) = 0;
          piVar27 = piVar32;
        } while (piVar32 != piVar20);
        goto LAB_0000ec10;
      }
LAB_0000ec2c:
      iVar24 = 0;
      while (iVar44 = iVar24 + 1, iVar24 < iVar34) {
        iVar33 = *(int *)(puVar37 + -0x5c);
        piVar27 = (int *)DAT_0007b438[iVar33];
        iVar24 = piVar27[0x10];
        piVar27[0x10] = iVar24 + 1;
        if (iVar24 < piVar27[0xf]) {
          if (((*(char *)((int)piVar27 + 0x81) == '\0') && (piVar27[0x21] == 1)) &&
             (((char)piVar27[0xa6] == '\0' || (*(char *)((int)piVar27 + 0x299) != '\0'))))
          goto LAB_0000ece0;
          if (DAT_0007b625 != '\0') {
            iVar24 = FUN_00012038(0);
            iVar34 = DAT_0007b198;
            iVar33 = *(int *)(puVar37 + -0x5c);
            *(int *)(iVar24 + 0x40) = *(int *)(iVar24 + 0x40) + -1;
          }
        }
        *(int *)(puVar37 + -0x5c) = iVar33 + 1;
        iVar24 = iVar44;
        if (iVar34 <= iVar33 + 1) {
          DAT_00078b74 = 0;
        }
      }
      if (0 < iVar34) {
        iVar34 = 0;
        do {
          piVar27 = (int *)FUN_00012038(iVar34);
          if ((*(char *)((int)piVar27 + 0x81) == '\0') && (piVar27[0x21] == 1)) goto LAB_0000ece0;
          iVar34 = iVar34 + 1;
        } while (iVar34 < DAT_0007b198);
      }
    }
    goto LAB_0000e53c;
  }
  pthread_detach(*(pthread_t *)((int)pvVar12 + 0xcc));
  DAT_00076ef8 = 5;
  uVar40 = FUN_0002f174((int)DAT_0007b0fc + 0x140,0,FUN_00012164,(int)DAT_0007b0fc + 0x140);
  pvVar12 = DAT_0007b0fc;
  iVar34 = 0;
  if (uVar40 == 0) {
    *(undefined4 *)(puVar37 + -0x60) = 7;
    iVar34 = (int)pvVar12 + 0x1c0;
    iVar34 = FUN_0002f174(iVar34,0,FUN_00027a98,iVar34);
    if (iVar34 == 0) {
      pthread_detach(*(pthread_t *)((int)pvVar12 + 0x1cc));
      if (DAT_00078b64 == 8) goto LAB_0000e2a0;
      snprintf((char *)local_11d0,0x1000,"incorrect total_control_threads (%d) should be 8",
               DAT_00078b64);
      goto LAB_0000de1c;
    }
    local_1030._0_1_ = 'i';
    local_1030._1_1_ = 'n';
    local_1030._2_1_ = 'p';
    local_1030._3_1_ = 'u';
    local_102c._0_1_ = 't';
    local_102c._1_1_ = ' ';
    local_102c._2_1_ = 't';
    local_102c._3_1_ = 'h';
    local_1028._0_1_ = 'r';
    local_1028._1_1_ = 'e';
    local_1028._2_1_ = 'a';
    local_1028._3_1_ = 'd';
    builtin_strncpy(acStack_1024," cre",4);
    local_1020._0_1_ = 'a';
    local_1020._1_1_ = 't';
    local_1020._2_1_ = 'e';
    local_1020._3_1_ = ' ';
    local_101c._0_1_ = 'f';
    local_101c._1_1_ = 'a';
    local_101c._2_1_ = 'i';
    local_101c._3_1_ = 'l';
    local_1018 = CONCAT22(local_1018._2_2_,0x6465);
    FUN_00038730(3,local_11d0,1);
    FUN_0001d8fc(1,0);
  }
  local_1030._0_1_ = 'A';
  local_1030._1_1_ = 'P';
  local_1030._2_1_ = 'I';
  local_1030._3_1_ = ' ';
  local_102c._0_1_ = 't';
  local_102c._1_1_ = 'h';
  local_102c._2_1_ = 'r';
  local_102c._3_1_ = 'e';
  local_1028._0_1_ = 'a';
  local_1028._1_1_ = 'd';
  local_1028._2_1_ = ' ';
  local_1028._3_1_ = 'c';
  builtin_strncpy(acStack_1024,"reat",4);
  local_1020._0_1_ = 'e';
  local_1020._1_1_ = ' ';
  local_1020._2_1_ = 'f';
  local_1020._3_1_ = 'a';
  local_101c._0_1_ = 'i';
  local_101c._1_1_ = 'l';
  local_101c._2_1_ = 'e';
  local_101c._3_1_ = 'd';
  local_1018 = local_1018 & 0xffffff00;
LAB_0000de1c:
  FUN_00038730(3,local_11d0,1);
  FUN_0001d8fc(1,iVar34);
LAB_0000de30:
  uVar45 = uVar40;
LAB_0000de48:
  FUN_0001ab24(&DAT_0007b700,"load_temp_cutoffs",0x48c);
  iVar34 = DAT_0007b720;
  piVar27 = DAT_0007b628;
  for (; (int)uVar40 < iVar34; uVar40 = uVar40 + 1) {
    *(uint *)(piVar27[uVar40] + 0xb8) = uVar45;
  }
  FUN_00019bbc(&DAT_0007b700,"load_temp_cutoffs",0x48f);
  goto LAB_0000d430;
LAB_0000ebd8:
  piVar27 = piVar27 + 1;
  if (*(int *)(*piVar27 + 0x40) < *(int *)(*piVar27 + 0x3c)) goto LAB_0000ec2c;
  goto LAB_0000ebec;
LAB_0000ece0:
  pbVar35 = (byte *)piVar27;
LAB_0000e53c:
  if ((DAT_00078e09 != '\0') &&
     (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
    snprintf((char *)local_11d0,0x1000,"Selecting pool %d for work",*(int *)pbVar35);
    FUN_00038730(7,local_11d0,0);
  }
  cVar9 = (char)*(int *)((int)pbVar35 + 0x298);
joined_r0x0000e5c8:
  piVar27 = DAT_0007b438;
  if (cVar9 != '\0') {
LAB_0000e5cc:
    if ((*(char *)((int)pbVar35 + 0x299) == '\0') || (*(char *)((int)pbVar35 + 0x29b) == '\0')) {
      local_11c8 = (byte *)FUN_0001da58();
      iVar34 = DAT_0007b198;
      if (DAT_00076efc == 4) {
        local_11c8 = (byte *)FUN_00012250();
      }
      else if ((DAT_00076efc == 3) || (DAT_0007b625 == '\0')) {
        if (DAT_0007b198 < 1) {
LAB_0000e688:
          iVar24 = *(int *)(puVar37 + -0x5c);
          *(int *)(puVar37 + -0x5c) = iVar24 + 1;
          if (iVar34 <= iVar24 + 1) {
            DAT_00078b74 = 0;
          }
        }
        else if (*(int *)(*DAT_0007b438 + 0x3c) <= *(int *)(*DAT_0007b438 + 0x40)) {
          iVar24 = 0;
          piVar27 = DAT_0007b438;
LAB_0000e668:
          iVar24 = iVar24 + 1;
          if (iVar24 != DAT_0007b198) goto LAB_0000e654;
          piVar20 = DAT_0007b438 + DAT_0007b198;
          piVar27 = DAT_0007b438;
          do {
            piVar32 = piVar27 + 1;
            *(undefined4 *)(*piVar27 + 0x40) = 0;
            piVar27 = piVar32;
          } while (piVar32 != piVar20);
          goto LAB_0000e688;
        }
LAB_0000e69c:
        iVar24 = 0;
        while (iVar44 = iVar24 + 1, iVar24 < iVar34) {
          iVar33 = *(int *)(puVar37 + -0x5c);
          piVar27 = (int *)DAT_0007b438[iVar33];
          iVar24 = piVar27[0x10];
          piVar27[0x10] = iVar24 + 1;
          if (iVar24 < piVar27[0xf]) {
            if (((*(char *)((int)piVar27 + 0x81) == '\0') && (piVar27[0x21] == 1)) &&
               (((char)piVar27[0xa6] == '\0' || (*(char *)((int)piVar27 + 0x299) != '\0'))))
            goto LAB_0000e7e0;
            if (DAT_0007b625 != '\0') {
              iVar24 = FUN_00012038(0);
              iVar34 = DAT_0007b198;
              iVar33 = *(int *)(puVar37 + -0x5c);
              *(int *)(iVar24 + 0x40) = *(int *)(iVar24 + 0x40) + -1;
            }
          }
          *(int *)(puVar37 + -0x5c) = iVar33 + 1;
          iVar24 = iVar44;
          if (iVar34 <= iVar33 + 1) {
            DAT_00078b74 = 0;
          }
        }
        if (0 < iVar34) {
          iVar34 = 0;
          do {
            piVar27 = (int *)FUN_00012038(iVar34);
            if ((*(char *)((int)piVar27 + 0x81) == '\0') && (piVar27[0x21] == 1)) goto LAB_0000e7e0;
            iVar34 = iVar34 + 1;
          } while (iVar34 < DAT_0007b198);
        }
      }
      goto LAB_0000e7e8;
    }
    FUN_0001acc4(pbVar35,iVar41);
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      local_1030._0_1_ = 'G';
      local_1030._1_1_ = 'e';
      local_1030._2_1_ = 'n';
      local_1030._3_1_ = 'e';
      local_102c._0_1_ = 'r';
      local_102c._1_1_ = 'a';
      local_102c._2_1_ = 't';
      local_102c._3_1_ = 'e';
      local_1028._0_1_ = 'd';
      local_1028._1_1_ = ' ';
      local_1028._2_1_ = 's';
      local_1028._3_1_ = 't';
      builtin_strncpy(acStack_1024,"ratu",4);
      local_1020._0_1_ = 'm';
      local_1020._1_1_ = ' ';
      local_1020._2_1_ = 'w';
      local_1020._3_1_ = 'o';
      local_101c._0_1_ = 'r';
      local_101c._1_1_ = 'k';
      FUN_00038730(7,local_11d0,0);
    }
    local_11cc = 0;
    FUN_0001f17c(iVar41);
    goto LAB_0000e368;
  }
  if (DAT_00076ffc == 0) {
    local_11cc = iVar41;
    if (DAT_0007730d != '\0') {
      *(undefined8 *)(iVar41 + 0x748) = 0x4040000000000000;
      uVar10 = DAT_0007873c;
      uVar29 = DAT_00078738;
      uVar13 = DAT_00078734;
      *(undefined4 *)(iVar41 + 0xa0) = DAT_00078730;
      *(undefined4 *)(iVar41 + 0xa4) = uVar13;
      *(undefined4 *)(iVar41 + 0xa8) = uVar29;
      *(undefined4 *)(iVar41 + 0xac) = uVar10;
      uVar10 = DAT_0007874c;
      uVar29 = DAT_00078748;
      uVar13 = DAT_00078744;
      *(undefined4 *)(iVar41 + 0xb0) = DAT_00078740;
      *(undefined4 *)(iVar41 + 0xb4) = uVar13;
      *(undefined4 *)(iVar41 + 0xb8) = uVar29;
      *(undefined4 *)(iVar41 + 0xbc) = uVar10;
      *(undefined4 *)(iVar41 + 0x6c4) = 0;
      *(undefined1 *)(iVar41 + 0x6ea) = 1;
      *(int *)(iVar41 + 0x6d4) = *piVar27;
      FUN_0002a84c(iVar41 + 0x760);
      FUN_0002a530(iVar41 + 0x768,iVar41 + 0x760);
      bVar49 = DAT_00078e09 != '\0';
      *(undefined1 *)(iVar41 + 0x788) = 0x42;
      if ((bVar49) && (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
        local_1030._0_1_ = 'G';
        local_1030._1_1_ = 'e';
        local_1030._2_1_ = 'n';
        local_1030._3_1_ = 'e';
        local_102c._0_1_ = 'r';
        local_102c._1_1_ = 'a';
        local_102c._2_1_ = 't';
        local_102c._3_1_ = 'e';
        local_1028._0_1_ = 'd';
        local_1028._1_1_ = ' ';
        local_1028._2_1_ = 'b';
        local_1028._3_1_ = 'e';
        builtin_strncpy(acStack_1024,"nchm",4);
        local_1020._0_1_ = 'a';
        local_1020._1_1_ = 'r';
        local_1020._2_1_ = 'k';
        local_1020._3_1_ = ' ';
        local_101c._0_1_ = 'w';
        local_101c._1_1_ = 'o';
        local_101c._2_1_ = 'r';
        local_101c._3_1_ = 'k';
        local_1018 = local_1018 & 0xffffff00;
        FUN_00038730(7,local_11d0,0);
      }
      local_11cc = 0;
      FUN_0001f17c(iVar41);
    }
  }
  else {
    FUN_00017254();
    piVar27 = DAT_0007b438;
    *(undefined1 *)(iVar41 + 0x6ea) = 1;
    *(int *)(iVar41 + 0x6d4) = *piVar27;
    FUN_0002a84c(iVar41 + 0x760);
    FUN_0002a530(iVar41 + 0x768,iVar41 + 0x760);
    *(undefined1 *)(iVar41 + 0x788) = 0x42;
    FUN_00011a5c(0,iVar41);
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      local_1030._0_1_ = 'G';
      local_1030._1_1_ = 'e';
      local_1030._2_1_ = 'n';
      local_1030._3_1_ = 'e';
      local_102c._0_1_ = 'r';
      local_102c._1_1_ = 'a';
      local_102c._2_1_ = 't';
      local_102c._3_1_ = 'e';
      local_1028._0_1_ = 'd';
      local_1028._1_1_ = ' ';
      local_1028._2_1_ = 'b';
      local_1028._3_1_ = 'e';
      builtin_strncpy(acStack_1024,"nchf",4);
      local_1020._0_1_ = 'i';
      local_1020._1_1_ = 'l';
      local_1020._2_1_ = 'e';
      local_1020._3_1_ = ' ';
      local_101c._0_1_ = 'w';
      local_101c._1_1_ = 'o';
      local_101c._2_1_ = 'r';
      local_101c._3_1_ = 'k';
      local_1018 = local_1018 & 0xffffff00;
      FUN_00038730(7,local_11d0,0);
    }
    local_11cc = 0;
    FUN_0001f17c(iVar41);
  }
  goto LAB_0000e368;
LAB_0000e654:
  piVar27 = piVar27 + 1;
  if (*(int *)(*piVar27 + 0x40) < *(int *)(*piVar27 + 0x3c)) goto LAB_0000e69c;
  goto LAB_0000e668;
LAB_0000e7e0:
  local_11c8 = (byte *)piVar27;
LAB_0000e7e8:
  if ((DAT_00078e09 != '\0') &&
     (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
    snprintf((char *)local_11d0,0x1000,"Selecting pool %d for work",*(int *)local_11c8);
    FUN_00038730(7,local_11d0,0);
  }
  FUN_0002a92c(5000);
  if (pbVar35 != local_11c8) goto code_r0x0000e880;
  goto LAB_0000e5cc;
code_r0x0000e880:
  cVar9 = (char)*(int *)((int)local_11c8 + 0x298);
  pbVar35 = local_11c8;
  goto joined_r0x0000e5c8;
}

