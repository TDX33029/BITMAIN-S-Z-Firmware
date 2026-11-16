//==================== FUN_0004388c @ 0004388c ====================

void FUN_0004388c(void)

{
  uint *puVar1;
  char cVar2;
  int iVar3;
  undefined4 *puVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  char *pcVar9;
  uint *puVar10;
  uint uVar11;
  int *piVar12;
  undefined4 *puVar13;
  uint *puVar14;
  uint *puVar15;
  uint *puVar16;
  uint uVar17;
  int iVar18;
  int local_1094;
  char *local_1090;
  undefined4 *local_108c;
  uint local_1088 [24];
  char local_1028 [4];
  char acStack_1024 [4];
  char acStack_1020 [4];
  char acStack_101c [4];
  char local_1018 [4];
  char acStack_1014 [4];
  char acStack_1010 [4];
  undefined2 local_100c;
  
  puVar13 = &DAT_0007c130;
  local_1088[1] = 0;
  local_1088[0xd] = 0;
  iVar18 = 0;
  local_1088[2] = 0;
  puVar10 = local_1088 + 0xc;
  local_1088[0xe] = 0;
  local_1088[3] = 0;
  local_1088[0xf] = 0;
  local_1088[4] = 0;
  local_1088[0x10] = 0;
  local_1088[5] = 0;
  local_1088[0x11] = 0;
  local_1088[6] = 0;
  local_1088[0x12] = 0;
  local_1088[7] = 0;
  local_1088[0x13] = 0;
  local_1088[8] = 0;
  local_1088[0x14] = 0;
  local_1088[9] = 0;
  local_1088[0x15] = 0;
  local_1088[0] = 0;
  local_1088[0xc] = 0;
  local_1088[10] = 0;
  local_1088[0x16] = 0;
  pcVar9 = &DAT_0009d4bb;
  local_1088[0xb] = 0;
  local_1088[0x17] = 0;
  do {
    pcVar9 = pcVar9 + 1;
    if (*pcVar9 == '\x01') {
      iVar5 = puVar13[0x3e];
      iVar3 = puVar13[0x3d];
      *puVar10 = 0x307;
      iVar6 = puVar13[0x3f];
      puVar10[1] = 0x307;
      puVar10[2] = 0x307;
      *(short *)(&DAT_0009d4b5 + (iVar18 + 0x128) * 2) = (short)((iVar3 + iVar5 + iVar6) / 3);
    }
    iVar18 = iVar18 + 1;
    puVar10 = puVar10 + 3;
    puVar13 = puVar13 + 3;
  } while (iVar18 != 4);
  local_108c = &DAT_0007c130;
  do {
    while (DAT_000790ec == '\0') {
      FUN_0002a92c(500);
    }
    uVar8 = 0;
    local_1094 = 0;
    local_1090 = &DAT_0009d4bb;
    do {
      local_1090 = local_1090 + 1;
      if (*local_1090 == '\x01') {
        uVar17 = uVar8 & 0xff;
        uVar11 = 0;
        puVar14 = &DAT_0007c224 + uVar8 * 3;
        puVar10 = local_1088 + uVar8 * 3 + 0xc;
        puVar15 = local_1088 + uVar8 * 3 + 1;
        if (local_1088[uVar8 * 3] != 0) goto LAB_00043b40;
        do {
          uVar7 = *puVar14;
          puVar16 = puVar15;
          if (uVar7 < 0x307) {
            *puVar10 = uVar7;
            FUN_00043734(uVar17,uVar11 & 0xff,uVar7);
            puVar15[-1] = 1;
          }
          else if (uVar7 == 0x307) {
            *puVar10 = 0x307;
            FUN_00043734(uVar17,uVar11 & 0xff);
            puVar15[-1] = 1;
          }
          else {
            puVar4 = &DAT_00069204;
            iVar18 = 1;
            puVar13 = puVar4;
            do {
              puVar1 = puVar13 + 4;
              puVar13 = puVar13 + 4;
              if (uVar7 == *puVar1) {
                uVar7 = *puVar10;
                goto joined_r0x00043aac;
              }
              iVar18 = iVar18 + 1;
            } while (iVar18 != 0x73);
            uVar7 = *puVar10;
            iVar18 = -1;
joined_r0x00043aac:
            if (uVar7 == 100) {
              iVar3 = 0;
            }
            else {
              iVar3 = 1;
              do {
                puVar1 = puVar4 + 4;
                puVar4 = puVar4 + 4;
                if (uVar7 == *puVar1) goto LAB_00043ad4;
                iVar3 = iVar3 + 1;
              } while (iVar3 != 0x73);
              iVar3 = -1;
            }
LAB_00043ad4:
            if (iVar18 - iVar3 < 2) {
              iVar3 = iVar3 + 1;
            }
            else {
              iVar3 = iVar3 + 2;
            }
            uVar7 = (&DAT_00069204)[iVar3 * 4];
            *puVar10 = uVar7;
            FUN_00043734(uVar17,uVar11 & 0xff,uVar7);
            if (uVar7 == *puVar14) {
              puVar15[-1] = 1;
            }
          }
          while( true ) {
            uVar11 = uVar11 + 1;
            puVar10 = puVar10 + 1;
            puVar14 = puVar14 + 1;
            if (uVar11 == 3) goto LAB_000439e0;
            puVar15 = puVar16 + 1;
            if (*puVar16 == 0) break;
LAB_00043b40:
            local_1094 = local_1094 + 1;
            puVar16 = puVar15;
          }
        } while( true );
      }
LAB_000439e0:
      uVar8 = uVar8 + 1;
    } while (uVar8 != 4);
    if ((int)((uint)DAT_0009d6cb * 3) <= local_1094) {
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
        local_1028[0] = 'A';
        local_1028[1] = 'l';
        local_1028[2] = 'l';
        local_1028[3] = ' ';
        acStack_1024[0] = 'a';
        acStack_1024[1] = 's';
        acStack_1024[2] = 'i';
        acStack_1024[3] = 'c';
        acStack_1020[0] = 's';
        acStack_1020[1] = ' ';
        acStack_1020[2] = 'a';
        acStack_1020[3] = 'r';
        acStack_101c[0] = 'r';
        acStack_101c[1] = 'i';
        acStack_101c[2] = 'v';
        acStack_101c[3] = 'e';
        local_1018[0] = ' ';
        local_1018[1] = 't';
        local_1018[2] = 'a';
        local_1018[3] = 'r';
        acStack_1014[0] = 'g';
        acStack_1014[1] = 'e';
        acStack_1014[2] = 't';
        acStack_1014[3] = ' ';
        acStack_1010[0] = 'f';
        acStack_1010[1] = 'r';
        acStack_1010[2] = 'e';
        acStack_1010[3] = 'q';
        local_100c = 10;
        FUN_00038730(5,local_1028,0);
      }
      iVar18 = 0;
      pcVar9 = &DAT_0009d4bb;
      do {
        pcVar9 = pcVar9 + 1;
        if (*pcVar9 == '\x01') {
          (&DAT_0007c134)[iVar18 * 3] = 0;
          (&DAT_0007c194)[iVar18 * 3] = 0;
          (&DAT_0007c1f4)[iVar18 * 3] = 0x32f;
          (&DAT_0007c1c4)[iVar18 * 3] = 0x307;
          local_108c[2] = 0;
          local_108c[0x1a] = 0;
          local_108c[0x32] = 0x32f;
          local_108c[0x26] = 0x307;
          local_108c[3] = 0;
          local_108c[0x1b] = 0;
          local_108c[0x33] = 0x32f;
          local_108c[0x27] = 0x307;
        }
        local_108c = local_108c + 3;
        iVar18 = iVar18 + 1;
      } while (iVar18 != 4);
LAB_00043c8c:
      do {
        sleep(1);
        pthread_mutex_lock((pthread_mutex_t *)&DAT_00079100);
        pthread_cond_wait((pthread_cond_t *)&DAT_0007c078,(pthread_mutex_t *)&DAT_00079100);
        pthread_mutex_unlock((pthread_mutex_t *)&DAT_00079100);
      } while (DAT_00078e3c != '\0');
      pcVar9 = &DAT_0009d4bc;
      uVar8 = 0;
      cVar2 = DAT_0009d4bc;
      do {
        if (cVar2 == '\x01') {
          uVar11 = 0;
          piVar12 = &DAT_0007c194 + uVar8 * 3;
          iVar18 = *piVar12;
          do {
            if (iVar18 == 1) {
              uVar17 = piVar12[0x24];
              if ((uint)piVar12[0xc] < uVar17) {
                if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
                  snprintf(local_1028,0x1000,
                           "low freq[%d-%d]: cur freq %d, [%d, %d], invalid nonce %d, ox status %d\n"
                           ,uVar8,uVar11,uVar17,piVar12[0xc],piVar12[0x18],piVar12[-0x18],1);
                  FUN_00038730(5,local_1028,0);
                  uVar17 = piVar12[0x24];
                }
                if (uVar17 == 100) {
                  iVar18 = -1;
                }
                else {
                  puVar13 = &DAT_00069204;
                  iVar18 = 1;
                  do {
                    puVar10 = puVar13 + 4;
                    puVar13 = puVar13 + 4;
                    if (*puVar10 == uVar17) {
                      iVar18 = iVar18 + -1;
                      goto LAB_00043dd4;
                    }
                    iVar18 = iVar18 + 1;
                  } while (iVar18 != 0x73);
                  iVar18 = -2;
                }
LAB_00043dd4:
                iVar18 = (&DAT_00069204)[iVar18 * 4];
                piVar12[0x24] = iVar18;
                FUN_00043734(uVar8 & 0xff,uVar11 & 0xff,iVar18);
              }
            }
            uVar11 = uVar11 + 1;
            if (uVar11 == 3) break;
            piVar12 = piVar12 + 1;
            iVar18 = *piVar12;
          } while( true );
        }
        uVar8 = uVar8 + 1;
        if (uVar8 == 4) goto LAB_00043c8c;
        pcVar9 = pcVar9 + 1;
        cVar2 = *pcVar9;
      } while( true );
    }
    sleep(3);
  } while( true );
}

