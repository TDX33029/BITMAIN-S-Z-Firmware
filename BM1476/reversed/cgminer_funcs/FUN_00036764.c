//==================== FUN_00036764 @ 00036764 ====================

/* WARNING: Restarted to delay deadcode elimination for space: ram */

void FUN_00036764(undefined4 param_1)

{
  uint uVar1;
  undefined4 *puVar2;
  int *__ptr;
  undefined4 *puVar3;
  undefined1 *puVar4;
  undefined4 *puVar5;
  int iVar6;
  size_t sVar7;
  byte *pbVar8;
  char *pcVar9;
  byte *pbVar10;
  int iVar11;
  char *pcVar12;
  byte *pbVar13;
  byte *pbVar14;
  ushort **ppuVar15;
  byte *pbVar16;
  long lVar17;
  int iVar18;
  time_t tVar19;
  ssize_t param3;
  uint *puVar20;
  int iVar21;
  time_t tVar22;
  int *piVar23;
  int *piVar24;
  uint *__s;
  undefined4 uVar25;
  byte bVar26;
  __int32_t *p_Var27;
  bool bVar28;
  char *unaff_r4;
  byte *param2;
  addrinfo *paVar29;
  char **ppcVar30;
  uint uVar31;
  uint *puVar32;
  bool bVar33;
  bool bVar34;
  undefined8 uVar35;
  byte *local_53bc;
  __int32_t **local_53b0;
  char *local_53a8;
  char *local_5394;
  byte *local_5388;
  byte bStack_535e;
  byte bStack_535d;
  char *pcStack_535c;
  socklen_t sStack_5358;
  addrinfo *paStack_5354;
  undefined4 uStack_5350;
  char acStack_534c [12];
  addrinfo aStack_5340;
  char acStack_5320 [100];
  sockaddr asStack_52bc [8];
  undefined1 auStack_523c [252];
  __pthread_unwind_buf_t a_Stack_5140 [6];
  char local_5028 [44];
  undefined2 uStack_4ffc;
  undefined1 local_4ffa;
  uint local_4028;
  char acStack_4024 [36];
  undefined2 local_4000;
  undefined4 local_2028;
  char acStack_2024 [20];
  undefined4 local_2010;
  undefined2 local_200c;
  
  uVar25 = DAT_00075ed0;
  __ptr = malloc(4);
  bVar33 = DAT_0007b7d1 == '\0';
  *__ptr = -1;
  if (bVar33) {
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      snprintf((char *)&local_2028,0x1000,"API not running%s"," - API will not be available");
      FUN_00038730(7,&local_2028,0);
    }
    goto LAB_0003681c;
  }
  puVar3 = malloc(0x10);
  puVar4 = malloc(0x10000);
  puVar3[1] = puVar4;
  puVar3[2] = puVar4;
  *(undefined1 *)(puVar3 + 3) = 1;
  *puVar3 = 0x10000;
  *puVar4 = 0;
  *(undefined1 *)((int)puVar3 + 0xd) = 0;
  puVar5 = malloc(0xc);
  puVar2 = DAT_00078cd0;
  bVar33 = DAT_00078cd0 == (undefined4 *)0x0;
  *puVar5 = puVar3;
  if (bVar33) {
    puVar5[1] = puVar5;
    puVar5[2] = puVar5;
    DAT_00078cd0 = puVar5;
  }
  else {
    puVar5[2] = puVar2;
    puVar5[1] = puVar2[1];
    puVar2[1] = puVar5;
    *(undefined4 **)(puVar5[1] + 8) = puVar5;
  }
  iVar6 = pthread_mutex_init((pthread_mutex_t *)&DAT_00078cb8,(pthread_mutexattr_t *)0x0);
  if (iVar6 != 0) {
    piVar23 = __errno_location();
    unaff_r4 = "api";
    snprintf((char *)&local_2028,0x1000,"Failed to pthread_mutex_init errno=%d in %s %s():%d",
             *piVar23,"api.c","api",0x14b1);
    FUN_00038730(3,&local_2028,1);
    FUN_00016ca8(1);
  }
  iVar6 = __sigsetjmp(a_Stack_5140,0);
  if (iVar6 == 0) {
    __pthread_register_cancel(a_Stack_5140);
    pcVar9 = DAT_0007b944;
    DAT_00078cdc = param_1;
    if (DAT_0007b944 == (char *)0x0) {
      sVar7 = 1;
      pcVar9 = "";
    }
    else {
      sVar7 = strlen(DAT_0007b944);
      sVar7 = sVar7 + 1;
    }
    local_5388 = malloc(sVar7);
    if (local_5388 == (byte *)0x0) {
      local_2028._0_1_ = 'F';
      local_2028._1_1_ = 'a';
      local_2028._2_1_ = 'i';
      local_2028._3_1_ = 'l';
      builtin_strncpy(acStack_2024,"ed to malloc ipgroup",0x14);
      local_2010._0_1_ = 's';
      local_2010._1_1_ = ' ';
      local_2010._2_1_ = 'b';
      local_2010._3_1_ = 'u';
      local_200c = 0x66;
      FUN_00038730(3,&local_2028,1);
      FUN_00016ca8(1);
      strcpy((char *)0x0,pcVar9);
      local_53b0 = __ctype_toupper_loc();
    }
    else {
      memcpy(local_5388,pcVar9,sVar7);
      if (*local_5388 != 0) {
        local_53b0 = __ctype_toupper_loc();
        pbVar13 = local_5388;
LAB_00036974:
        local_53bc = (byte *)strchr((char *)pbVar13,0x2c);
        pbVar8 = local_53bc;
        if (local_53bc != (byte *)0x0) {
          pbVar8 = local_53bc + 1;
          *local_53bc = 0;
        }
        if (local_53bc != (byte *)0x0) {
          local_53bc = pbVar8;
        }
        if (pbVar13[1] != 0x3a) {
          pcVar9 = strchr((char *)pbVar13,0x3a);
          if (pcVar9 != (char *)0x0) {
            *pcVar9 = '\0';
          }
          snprintf((char *)&local_4028,0x1000,"API invalid group name \'%s\'",(char *)pbVar13);
          FUN_00038730(3,&local_4028,1);
          FUN_00016ca8(1);
        }
        p_Var27 = *local_53b0;
        bVar26 = *(byte *)(p_Var27 + *pbVar13);
        uVar31 = (uint)bVar26;
        if ((p_Var27[uVar31] < p_Var27[0x41]) || (p_Var27[0x5a] < p_Var27[uVar31])) {
          snprintf((char *)&local_4028,0x1000,"API invalid group name \'%c\'",*pbVar13);
          FUN_00038730(3,&local_4028,1);
          FUN_00016ca8(1);
          p_Var27 = *local_53b0;
        }
        if (uVar31 == p_Var27[0x57]) {
          snprintf((char *)&local_4028,0x1000,"API group name can\'t be \'%c\'",bVar26);
          FUN_00038730(3,&local_4028,1);
          FUN_00016ca8(1);
          p_Var27 = *local_53b0;
        }
        if (uVar31 == p_Var27[0x52]) {
          snprintf((char *)&local_4028,0x1000,"API group name can\'t be \'%c\'",bVar26);
          FUN_00038730(3,&local_4028,1);
          FUN_00016ca8(1);
          p_Var27 = *local_53b0;
        }
        if (*(int *)(&DAT_0007bd24 + (p_Var27[uVar31] - p_Var27[0x41]) * 4) != 0) {
          snprintf((char *)&local_4028,0x1000,"API duplicate group name \'%c\'",*pbVar13);
          FUN_00038730(3,&local_4028,1);
          FUN_00016ca8(1);
        }
        bVar33 = false;
        local_53a8 = (char *)((int)&local_2028 + 1);
        local_2028._0_1_ = '|';
        local_2028._1_1_ = '\0';
        if ((pbVar13 + 2 != (byte *)0x0) && (pbVar13[2] != 0)) {
          pbVar13 = pbVar13 + 2;
          do {
            pbVar10 = (byte *)strchr((char *)pbVar13,0x3a);
            pbVar8 = pbVar10;
            if (pbVar10 != (byte *)0x0) {
              pbVar8 = pbVar10 + 1;
              *pbVar10 = 0;
            }
            if ((*pbVar13 == 0x2a) && (pbVar13[1] == 0)) {
              bVar33 = true;
            }
            else {
              if (DAT_0007652c != (char *)0x0) {
                ppcVar30 = &DAT_0007652c;
                iVar6 = 0;
                pcVar9 = DAT_0007652c;
                do {
                  iVar11 = strcasecmp((char *)pbVar13,pcVar9);
                  if (iVar11 == 0) {
                    sprintf(local_5028,"|%s|",pcVar9);
                    pcVar9 = strstr((char *)&local_2028,local_5028);
                    if (pcVar9 == (char *)0x0) {
                      pcVar9 = (&DAT_0007652c)[iVar6 * 3];
                      strcpy(local_53a8,pcVar9);
                      sVar7 = strlen(pcVar9);
                      local_53a8[sVar7] = '|';
                      local_53a8[sVar7 + 1] = '\0';
                      local_53a8 = local_53a8 + sVar7 + 1;
                    }
                    goto joined_r0x00036c10;
                  }
                  ppcVar30 = ppcVar30 + 3;
                  pcVar9 = *ppcVar30;
                  iVar6 = iVar6 + 1;
                } while (pcVar9 != (char *)0x0);
              }
              snprintf((char *)&local_4028,0x1000,"API unknown command \'%s\' in group \'%c\'",
                       (char *)pbVar13,bVar26);
              FUN_00038730(3,&local_4028,1);
              FUN_00016ca8(1);
            }
joined_r0x00036c10:
            if ((pbVar8 == (byte *)0x0) || (*pbVar8 == 0)) goto LAB_00036c14;
            pbVar13 = pbVar8;
          } while( true );
        }
        goto LAB_00036cbc;
      }
      local_53b0 = __ctype_toupper_loc();
    }
LAB_00036e40:
    ppcVar30 = (char **)&DAT_000760fc;
    if (DAT_0007652c != (char *)0x0) {
      ppcVar30 = &DAT_0007652c;
    }
    local_2028._0_1_ = '|';
    local_2028._1_1_ = '\0';
    if (DAT_0007652c != (char *)0x0) {
      pcVar9 = DAT_0007652c;
      pcVar12 = (char *)((int)&local_2028 + 1);
      do {
        while (*(char *)(ppcVar30 + 2) != '\0') {
          ppcVar30 = ppcVar30 + 3;
          pcVar9 = *ppcVar30;
          if (pcVar9 == (char *)0x0) goto LAB_00036ed0;
        }
        strcpy(pcVar12,pcVar9);
        sVar7 = strlen(pcVar9);
        pcVar12[sVar7] = '|';
        pcVar12[sVar7 + 1] = '\0';
        ppcVar30 = ppcVar30 + 3;
        pcVar9 = *ppcVar30;
        pcVar12 = pcVar12 + sVar7 + 1;
      } while (pcVar9 != (char *)0x0);
    }
LAB_00036ed0:
    p_Var27 = *local_53b0;
    iVar6 = p_Var27[0x52];
    if (iVar6 + 0x80U < 0x180) {
      iVar6 = p_Var27[iVar6];
    }
    iVar11 = p_Var27[0x41];
    sVar7 = strlen((char *)&local_2028);
    unaff_r4 = malloc(sVar7 + 1);
    *(char **)(&DAT_0007bd24 + (iVar6 - iVar11) * 4) = unaff_r4;
    if (unaff_r4 == (char *)0x0) goto LAB_00038268;
  }
  else {
    FUN_0003243c(__ptr);
    __pthread_unwind_next(a_Stack_5140);
LAB_00038268:
    local_4028._0_1_ = 'F';
    local_4028._1_1_ = 'a';
    local_4028._2_1_ = 'i';
    local_4028._3_1_ = 'l';
    builtin_strncpy(acStack_4024,"ed to malloc noprivgroup commands bu",0x24);
    local_4000 = 0x66;
    FUN_00038730(3,&local_4028,1);
    FUN_00016ca8(1);
  }
  strcpy(unaff_r4,(char *)&local_2028);
  free(local_5388);
  pcVar9 = DAT_00078c98;
  if (DAT_00078c98 != (char *)0x0) {
    sVar7 = strlen(DAT_00078c98);
    pbVar13 = malloc(sVar7 + 1);
    if (pbVar13 == (byte *)0x0) {
      local_2028._0_1_ = 'F';
      local_2028._1_1_ = 'a';
      local_2028._2_1_ = 'i';
      local_2028._3_1_ = 'l';
      builtin_strncpy(acStack_2024,"ed to malloc ipacces",0x14);
      local_2010._0_1_ = 's';
      local_2010._1_1_ = ' ';
      local_2010._2_1_ = 'b';
      local_2010._3_1_ = 'u';
      local_200c = 0x66;
      FUN_00038730(3,&local_2028,1);
      FUN_00016ca8(1);
      pcVar9 = DAT_00078c98;
    }
    strcpy((char *)pbVar13,pcVar9);
    pbVar8 = pbVar13 + -1;
    sVar7 = 1;
    while( true ) {
      pbVar8 = pbVar8 + 1;
      if (*pbVar8 == 0) break;
      if (*pbVar8 == 0x2c) {
        sVar7 = sVar7 + 1;
      }
    }
    DAT_00078cb4 = calloc(sVar7,0x24);
    if (DAT_00078cb4 == (void *)0x0) {
      local_2028._0_1_ = 'F';
      local_2028._1_1_ = 'a';
      local_2028._2_1_ = 'i';
      local_2028._3_1_ = 'l';
      builtin_strncpy(acStack_2024,"ed to calloc ipacces",0x14);
      local_2010._0_1_ = 's';
      local_2010._1_1_ = '\0';
      FUN_00038730(3,&local_2028,1);
      FUN_00016ca8(1);
    }
    iVar6 = 0;
    DAT_00078cb0 = 0;
    if ((pbVar13 == (byte *)0x0) || (bVar26 = *pbVar13, bVar26 == 0)) {
      free(pbVar13);
    }
    else {
      pbVar8 = pbVar13;
      do {
        if (bVar26 == 0x20 || bVar26 == 9) {
          do {
            pbVar8 = pbVar8 + 1;
            bVar26 = *pbVar8;
          } while (bVar26 == 0x20 || bVar26 == 9);
        }
        if (bVar26 == 0x2c) {
          pbVar10 = pbVar8 + 1;
          goto LAB_000370cc;
        }
        pbVar14 = (byte *)strchr((char *)pbVar8,0x2c);
        pbVar10 = pbVar14;
        if (pbVar14 != (byte *)0x0) {
          pbVar10 = pbVar14 + 1;
          *pbVar14 = 0;
        }
        ppuVar15 = __ctype_b_loc();
        p_Var27 = *local_53b0;
        bVar26 = *(byte *)(p_Var27 + 0x52);
        if ((((*ppuVar15)[*pbVar8] & 0x400) != 0) && (pbVar8[1] == 0x3a)) {
          iVar11 = p_Var27[*pbVar8];
          if ((iVar11 == p_Var27[0x57]) ||
             (*(int *)(&DAT_0007bd24 + (iVar11 - p_Var27[0x41]) * 4) != 0)) {
            bVar26 = (byte)iVar11;
          }
          pbVar8 = pbVar8 + 2;
        }
        pbVar14 = (byte *)((int)DAT_00078cb4 + iVar6 * 0x24);
        pbVar14[0x20] = bVar26;
        bVar26 = *pbVar8;
        if ((((bVar26 == 0x30) && (pbVar8[1] == 0x2f)) && (pbVar8[2] == 0x30)) && (pbVar8[3] == 0))
        {
          iVar11 = 0;
          pbVar8 = pbVar14 + -1;
          do {
            iVar11 = iVar11 + 1;
            pbVar8[1] = 0;
            pbVar8[0x11] = 0;
            pbVar8 = pbVar8 + 1;
          } while (iVar11 != 0x10);
LAB_000370c4:
          iVar6 = iVar6 + 1;
          DAT_00078cb0 = iVar6;
        }
        else {
          pbVar16 = (byte *)strchr((char *)pbVar8,0x2f);
          if (pbVar16 == (byte *)0x0) {
            iVar11 = 0;
            pbVar16 = pbVar14;
            do {
              iVar11 = iVar11 + 1;
              pbVar16[0x10] = 0xff;
              pbVar16 = pbVar16 + 1;
            } while (iVar11 != 0x10);
            sVar7 = strlen((char *)pbVar8);
            bVar26 = *pbVar8;
            pbVar16 = pbVar8 + sVar7;
          }
          if ((bVar26 == 0x5b) && (pbVar16[-1] == 0x5d)) {
            bVar33 = true;
            param2 = pbVar8 + 1;
            *pbVar8 = 0;
            pbVar16[-1] = 0;
          }
          else {
            bVar33 = false;
            param2 = pbVar8;
          }
          if (*pbVar16 == 0) {
LAB_0003722c:
            iVar11 = 0;
            do {
              pbVar14[iVar11] = 0;
              iVar11 = iVar11 + 1;
            } while (iVar11 != 0x10);
            if (bVar33) {
              iVar11 = inet_pton(10,(char *)param2,pbVar14);
            }
            else {
              sprintf((char *)&local_2028,"::ffff:%s",(char *)param2);
              iVar6 = DAT_00078cb0;
              pbVar14 = (byte *)((int)DAT_00078cb4 + DAT_00078cb0 * 0x24);
              iVar11 = inet_pton(10,(char *)&local_2028,pbVar14);
            }
            if (iVar11 == 1) {
              iVar11 = 0;
              do {
                iVar11 = iVar11 + 1;
                *pbVar14 = *pbVar14 & pbVar14[0x10];
                pbVar14 = pbVar14 + 1;
              } while (iVar11 != 0x10);
              goto LAB_000370c4;
            }
          }
          else {
            *pbVar16 = 0;
            lVar17 = strtol((char *)(pbVar16 + 1),(char **)0x0,10);
            if (0 < lVar17) {
              if (bVar33) {
                iVar11 = 0;
              }
              else {
                iVar11 = 0x60;
              }
              iVar11 = lVar17 + iVar11;
              if (iVar11 < 0x81) {
                iVar21 = 0;
                pbVar8 = pbVar14;
                do {
                  iVar21 = iVar21 + 1;
                  pbVar8[0x10] = 0;
                  pbVar8 = pbVar8 + 1;
                } while (iVar21 != 0x10);
                uVar31 = 7;
                iVar21 = 0;
                while (0 < iVar11) {
                  iVar18 = iVar21;
                  if (uVar31 == 0) {
                    iVar18 = iVar21 + 1;
                  }
                  iVar11 = iVar11 + -1;
                  uVar1 = uVar31 & 0xff;
                  if (uVar31 == 0) {
                    uVar31 = 7;
                  }
                  else {
                    uVar31 = uVar31 - 1;
                  }
                  pbVar14[iVar21 + 0x10] = pbVar14[iVar21 + 0x10] | (byte)(1 << uVar1);
                  iVar21 = iVar18;
                }
                goto LAB_0003722c;
              }
            }
          }
        }
LAB_000370cc:
        if (pbVar10 == (byte *)0x0) break;
        bVar26 = *pbVar10;
        pbVar8 = pbVar10;
      } while (bVar26 != 0);
      free(pbVar13);
      if (iVar6 != 0) goto LAB_000370f4;
    }
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
      snprintf((char *)&local_2028,0x1000,"API not running (no valid IPs specified)%s",
               " - API will not be available");
      FUN_00038730(4,&local_2028,0);
    }
LAB_0003681c:
    free(__ptr);
    return;
  }
LAB_000370f4:
  iVar11 = (int)(short)uVar25;
  FUN_0002a92c(DAT_00075dcc * 1000);
  sprintf(acStack_534c,"%d",iVar11);
  aStack_5340.ai_flags = 1;
  aStack_5340.ai_family = 0;
  aStack_5340.ai_socktype = 0;
  aStack_5340.ai_protocol = 0;
  aStack_5340.ai_addrlen = 0;
  aStack_5340.ai_addr = (sockaddr *)0x0;
  aStack_5340.ai_canonname = (char *)0x0;
  aStack_5340.ai_next = (addrinfo *)0x0;
  iVar6 = getaddrinfo(DAT_00075ecc,acStack_534c,&aStack_5340,&paStack_5354);
  if (iVar6 != 0) {
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
      snprintf((char *)&local_2028,0x1000,"API failed to resolve %s",DAT_00075ecc);
LAB_000371e8:
      FUN_00038730(3,&local_2028,0);
    }
    goto LAB_0003681c;
  }
  if (paStack_5354 == (addrinfo *)0x0) {
    iVar6 = *__ptr;
  }
  else {
    paVar29 = paStack_5354;
    do {
      iVar6 = socket(paStack_5354->ai_family,1,0);
      *__ptr = iVar6;
      if (0 < iVar6) goto LAB_000374ec;
      paVar29 = paVar29->ai_next;
    } while (paVar29 != (addrinfo *)0x0);
  }
  if (iVar6 == -1) {
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
      piVar23 = __errno_location();
      pcVar9 = strerror(*piVar23);
      snprintf((char *)&local_2028,0x1000,"API initialisation failed (%s)%s",pcVar9,
               " - API will not be available");
      FUN_00038730(3,&local_2028,0);
    }
    freeaddrinfo(paStack_5354);
    free(__ptr);
    return;
  }
  paVar29 = (addrinfo *)0x0;
LAB_000374ec:
  uStack_5350 = 1;
  iVar6 = setsockopt(iVar6,1,2,&uStack_5350,4);
  if (((iVar6 < 0) && (DAT_00078e09 != '\0')) &&
     ((DAT_0007ad48 != '\0' || ((DAT_00078e08 != '\0' || (6 < DAT_000766c4)))))) {
    piVar23 = __errno_location();
    pcVar9 = strerror(*piVar23);
    snprintf((char *)&local_2028,0x1000,"API setsockopt SO_REUSEADDR failed (ignored): %s",pcVar9);
    FUN_00038730(7,&local_2028,0);
  }
  tVar19 = time((time_t *)0x0);
  while (iVar6 = bind(*__ptr,paVar29->ai_addr,paVar29->ai_addrlen), iVar6 < 0) {
    piVar23 = __errno_location();
    pcVar9 = strerror(*piVar23);
    tVar22 = time((time_t *)0x0);
    if (0x3d < tVar22 - tVar19) {
      freeaddrinfo(paStack_5354);
      if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3))
      goto LAB_0003681c;
      snprintf((char *)&local_2028,0x1000,"API bind to port %d failed (%s)%s",iVar11,pcVar9,
               " - API will not be available");
      goto LAB_000371e8;
    }
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
      snprintf((char *)&local_2028,0x1000,"API bind to port %d failed - trying again in 30sec",
               iVar11);
      FUN_00038730(4,&local_2028,0);
    }
    FUN_0002a92c(30000);
  }
  freeaddrinfo(paStack_5354);
  iVar6 = listen(*__ptr,100);
  if (iVar6 < 0) {
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
      piVar23 = __errno_location();
      pcVar9 = strerror(*piVar23);
      snprintf((char *)&local_2028,0x1000,"API3 initialisation failed (%s)%s",pcVar9,
               " - API will not be available");
      FUN_00038730(3,&local_2028,0);
    }
    close(*__ptr);
    goto LAB_0003681c;
  }
  if (DAT_00078c98 == (char *)0x0) {
    if (DAT_0007b7c4 == '\0') {
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
        pcVar9 = "API running in local read access mode on port %d (%d)";
        iVar6 = *__ptr;
        goto LAB_00037600;
      }
    }
    else if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
      pcVar9 = "API running in UNRESTRICTED read access mode on port %d (%d)";
      iVar6 = *__ptr;
LAB_00037600:
      snprintf((char *)&local_2028,0x1000,pcVar9,iVar11,iVar6);
      FUN_00038730(4,&local_2028,0);
    }
  }
  else if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
    pcVar9 = "API running in IP access mode on port %d (%d)";
    iVar6 = *__ptr;
    goto LAB_00037600;
  }
  if (DAT_0007b624 != '\0') {
    FUN_0003669c();
  }
  DAT_00078cd4 = FUN_00038f3c("StrBufs",0xc,2,0,0,"api.c",&DAT_00066a40,0x1521);
  if (DAT_00078cac == '\0') {
    bVar33 = false;
    piVar23 = (int *)0x0;
    do {
      sStack_5358 = 0x80;
      iVar6 = accept(*__ptr,asStack_52bc,&sStack_5358);
      if (iVar6 < 0) {
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
          piVar23 = __errno_location();
          pcVar9 = strerror(*piVar23);
          snprintf(local_5028,0x1000,"API failed (%s)%s (%d)",pcVar9," - API will not be available",
                   *__ptr);
          FUN_00038730(3,local_5028,0);
        }
        break;
      }
      iVar11 = FUN_000309e8(asStack_52bc,&pcStack_535c,&bStack_535e);
      if ((DAT_00078e09 != '\0') &&
         (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
        if (iVar11 == 0) {
          pcVar9 = "Ignored";
        }
        else {
          pcVar9 = "Accepted";
        }
        snprintf(local_5028,0x1000,"API: connection from %s - %s",pcStack_535c,pcVar9);
        FUN_00038730(7,local_5028,0);
      }
      if (iVar11 != 0) {
        param3 = recv(iVar6,&local_4028,0x1fff,0);
        if (param3 < 0) {
          local_4028 = local_4028 & 0xffffff00;
          if ((DAT_00078e09 != '\0') &&
             (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
            piVar24 = __errno_location();
            pcVar9 = strerror(*piVar24);
            snprintf(local_5028,0x1000,"API: recv failed: %s",pcVar9);
            FUN_00038730(7,local_5028,0);
          }
        }
        else {
          *(undefined1 *)((int)&local_4028 + param3) = 0;
          if ((DAT_00078e09 != '\0') &&
             (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
            snprintf(local_5028,0x1000,"API: recv command: (%d) \'%s\'",param3,(char *)&local_4028);
            FUN_00038730(7,local_5028,0);
          }
          bVar34 = false;
          DAT_00078cd8 = time((time_t *)0x0);
          puVar3[2] = (undefined1 *)puVar3[1];
          *(undefined1 *)puVar3[1] = 0;
          *(undefined1 *)((int)puVar3 + 0xd) = 0;
          if ((char)local_4028 != '{') {
            pcVar9 = strchr((char *)&local_4028,0x7c);
            if (pcVar9 == (char *)0x0) {
              __s = &local_4028;
              bVar34 = false;
              pcVar12 = (char *)0x0;
            }
            else {
              pcVar12 = pcVar9 + 1;
              *pcVar9 = '\0';
              __s = &local_4028;
            }
            goto LAB_000378e0;
          }
          piVar23 = (int *)FUN_000592c4(&local_4028,param3,0,auStack_523c);
          if ((piVar23 == (int *)0x0) || (*piVar23 != 0)) {
            uVar25 = 0x17;
LAB_00038404:
            FUN_0003305c(puVar3,uVar25,0,0,1);
            FUN_00030b88(puVar3,iVar6,1);
            bVar34 = piVar23 != (int *)0x0;
            bVar28 = true;
          }
          else {
            piVar24 = (int *)FUN_00059e20(piVar23,"command");
            if (piVar24 == (int *)0x0) {
              uVar25 = 0x18;
              goto LAB_00038404;
            }
            if (*piVar24 != 2) {
              uVar25 = 0xe;
              goto LAB_00038404;
            }
            __s = (uint *)FUN_0005a178();
            piVar24 = (int *)FUN_00059e20(piVar23,"parameter");
            if (piVar24 == (int *)0x0) {
LAB_000386ac:
              pcVar12 = (char *)0x0;
              bVar34 = true;
            }
            else {
              iVar11 = *piVar24;
              if (iVar11 == 2) {
                pcVar12 = (char *)FUN_0005a178();
                bVar34 = true;
              }
              else if (iVar11 == 3) {
                iVar11 = FUN_0005a248();
                bVar34 = true;
                pcVar12 = (char *)&local_2028;
                sprintf(pcVar12,"%d",iVar11);
              }
              else {
                if (iVar11 != 4) goto LAB_000386ac;
                uVar35 = FUN_0005a2f8();
                pcVar12 = (char *)&local_2028;
                bVar34 = true;
                sprintf(pcVar12,"%f",(int)uVar35,(int)((ulonglong)uVar35 >> 0x20));
              }
            }
LAB_000378e0:
            pcVar9 = strchr((char *)__s,0x2b);
            if (pcVar9 == (char *)0x0) {
              bStack_535d = 0;
              bVar33 = false;
              local_5394 = (char *)0x0;
            }
            else {
              bStack_535d = 1;
              sVar7 = strlen((char *)__s);
              local_5394 = malloc(sVar7 + 3);
              if (local_5394 == (char *)0x0) {
                snprintf(local_5028,0x1000,"OOM cmdsbuf in %s %s():%d","api.c","api",0x1590);
                FUN_00038730(3,local_5028,1);
                FUN_00016ca8(1);
              }
              bVar33 = true;
              pcVar12 = (char *)0x0;
              local_5394[0] = '|';
              local_5394[1] = '\0';
            }
            puVar32 = __s;
            do {
              if (bVar33) {
                puVar20 = (uint *)strchr((char *)__s,0x2b);
                puVar32 = puVar20;
                if (puVar20 != (uint *)0x0) {
                  puVar32 = (uint *)((int)puVar20 + 1);
                  *(char *)puVar20 = '\0';
                }
                if ((char)*__s != '\0') goto LAB_000379d4;
LAB_00037a3c:
                __s = puVar32;
                puVar32 = __s;
              }
              else {
LAB_000379d4:
                if (DAT_0007652c != (char *)0x0) {
                  ppcVar30 = &DAT_0007652c;
                  iVar11 = 0;
                  pcVar9 = DAT_0007652c;
                  do {
                    iVar21 = strcmp((char *)__s,pcVar9);
                    if (iVar21 == 0) {
                      sprintf(acStack_5320,"|%s|",__s);
                      if (bVar33) {
                        pcVar9 = strstr(local_5394,acStack_5320);
                        if (pcVar9 != (char *)0x0) goto LAB_00037a3c;
                        sVar7 = strlen(local_5394);
                        pcVar9 = stpcpy(local_5394 + sVar7,(char *)__s);
                        pcVar9[0] = '|';
                        pcVar9[1] = '\0';
                        FUN_0003085c(puVar3,__s,bVar34,&bStack_535d);
                        if ((&DAT_00076535)[iVar11 * 0xc] == '\0') {
                          FUN_0003305c(puVar3,0x2d,0,(&DAT_0007652c)[iVar11 * 3],bVar34);
                          FUN_00031f64(puVar3,bVar34);
                          goto LAB_00037a3c;
                        }
                      }
                      p_Var27 = *local_53b0;
                      uVar31 = (uint)bStack_535e;
                      if ((p_Var27[uVar31] == p_Var27[0x57]) ||
                         (pcVar9 = strstr(*(char **)(&DAT_0007bd24 +
                                                    (p_Var27[uVar31] - p_Var27[0x41]) * 4),
                                          acStack_5320), pcVar9 != (char *)0x0)) {
                        (*(code *)(&DAT_00076530)[iVar11 * 3])(puVar3,iVar6,pcVar12,bVar34,uVar31);
                      }
                      else {
                        FUN_0003305c(puVar3,0x2d,0,(&DAT_0007652c)[iVar11 * 3],bVar34);
                        if ((DAT_00078e09 != '\0') &&
                           (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4))
                           )) {
                          snprintf(local_5028,0x1000,
                                   "API: access denied to \'%s\' for \'%s\' command",pcStack_535c,
                                   (&DAT_0007652c)[iVar11 * 3]);
                          FUN_00038730(7,local_5028,0);
                        }
                      }
                      if (!bVar33) goto LAB_00037ca8;
                      goto LAB_00037d40;
                    }
                    ppcVar30 = ppcVar30 + 3;
                    pcVar9 = *ppcVar30;
                    iVar11 = iVar11 + 1;
                  } while (pcVar9 != (char *)0x0);
                }
                if (bVar33) {
                  FUN_0003085c(puVar3,__s,bVar34,&bStack_535d);
                  FUN_0003305c(puVar3,0xe,0,0,bVar34);
LAB_00037d40:
                  FUN_00031f64(puVar3,bVar34);
                  goto LAB_00037a3c;
                }
                FUN_0003305c(puVar3,0xe,0,0,bVar34);
LAB_00037ca8:
                FUN_00030b88(puVar3,iVar6,bVar34);
              }
              if (__s == (uint *)0x0) {
                bVar28 = false;
              }
              else {
                bVar28 = bVar33;
              }
            } while (bVar28);
            bVar28 = bVar34;
          }
          if (bVar33) {
            FUN_00030b88(puVar3,iVar6,bVar28);
          }
          if (((bVar34 != false) && (*piVar23 == 0)) &&
             ((piVar23[1] != -1 && (iVar11 = piVar23[1] + -1, piVar23[1] = iVar11, iVar11 == 0)))) {
            FUN_0005a3e4(piVar23);
          }
        }
      }
      close(iVar6);
    } while (DAT_00078cac == '\0');
  }
  __pthread_unregister_cancel(a_Stack_5140);
  FUN_0003243c(__ptr);
  free(__ptr);
  if ((DAT_00078e09 != '\0') &&
     (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
    if (DAT_00078cae == '\0') {
      if (DAT_00078cad == '\0') {
        if (DAT_00078cac == '\0') {
          pcVar9 = "UNKNOWN!";
        }
        else {
          pcVar9 = "BYE";
        }
      }
      else {
        pcVar9 = "RESTART";
      }
    }
    else {
      pcVar9 = "QUIT";
    }
    snprintf(local_5028,0x1000,"API: terminating due to: %s",pcVar9);
    FUN_00038730(7,local_5028,0);
  }
  iVar6 = pthread_mutex_lock((pthread_mutex_t *)&DAT_00078cb8);
  if (iVar6 != 0) {
    FUN_0000b774(&DAT_00066a40,0x15f3);
  }
  if (DAT_00078cad == '\0') {
    if (DAT_00078cae == '\0') goto LAB_00037ed8;
    iVar6 = FUN_0002f174(a_Stack_5140,0,FUN_00032340,a_Stack_5140);
    if (iVar6 != 0) {
      FUN_0003220c(&DAT_00078cb8,&DAT_00066a40,0x1602);
      builtin_strncpy(local_5028,"API failed to initiate a clean quit - aborti",0x2c);
      uStack_4ffc = 0x676e;
      local_4ffa = 0;
      FUN_00038730(3,local_5028,1);
      FUN_00016ca8(1);
    }
  }
  else {
    iVar6 = FUN_0002f174(a_Stack_5140,0,FUN_00032248,a_Stack_5140);
    if (iVar6 != 0) {
      FUN_0003220c(&DAT_00078cb8,&DAT_00066a40,0x15f9);
      builtin_strncpy(local_5028,"API failed to initiate a restart - aborting",0x2c);
      FUN_00038730(3,local_5028,1);
      FUN_00016ca8(1);
    }
  }
  pthread_detach(a_Stack_5140[0].__cancel_jmp_buf[0].__cancel_jmp_buf[3]);
LAB_00037ed8:
  iVar6 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_00078cb8);
  if (iVar6 != 0) {
    FUN_0000b84c(&DAT_00066a40,0x1608);
  }
  (*DAT_00075ed8)();
  return;
LAB_00036c14:
  if ((bVar33) && (DAT_0007652c != (char *)0x0)) {
    pcVar9 = &DAT_00076534;
    do {
      while (*pcVar9 != '\0') {
LAB_00036c44:
        piVar23 = (int *)(pcVar9 + 4);
        pcVar9 = pcVar9 + 0xc;
        if (*piVar23 == 0) goto LAB_00036cbc;
      }
      sprintf(local_5028,"|%s|");
      pcVar12 = strstr((char *)&local_2028,local_5028);
      if (pcVar12 != (char *)0x0) goto LAB_00036c44;
      pcVar12 = *(char **)(pcVar9 + -8);
      strcpy(local_53a8,pcVar12);
      sVar7 = strlen(pcVar12);
      local_53a8[sVar7] = '|';
      local_53a8[sVar7 + 1] = '\0';
      piVar23 = (int *)(pcVar9 + 4);
      pcVar9 = pcVar9 + 0xc;
      local_53a8 = local_53a8 + sVar7 + 1;
    } while (*piVar23 != 0);
  }
LAB_00036cbc:
  iVar11 = (*local_53b0)[uVar31];
  iVar6 = (*local_53b0)[0x41];
  sVar7 = strlen((char *)&local_2028);
  pcVar9 = malloc(sVar7 + 1);
  *(char **)(&DAT_0007bd24 + (iVar11 - iVar6) * 4) = pcVar9;
  if (pcVar9 == (char *)0x0) {
    local_4028._0_1_ = 'F';
    local_4028._1_1_ = 'a';
    local_4028._2_1_ = 'i';
    local_4028._3_1_ = 'l';
    builtin_strncpy(acStack_4024,"ed to malloc group commands buf",0x20);
    FUN_00038730(3,&local_4028,1);
    FUN_00016ca8(1);
  }
  strcpy(pcVar9,(char *)&local_2028);
  if ((local_53bc == (byte *)0x0) || (*local_53bc == 0)) goto LAB_00036e40;
  pbVar13 = local_53bc;
  goto LAB_00036974;
}

