//==================== FUN_0002b8e8 @ 0002b8e8 ====================

int FUN_0002b8e8(int *param_1)

{
  bool bVar1;
  byte bVar2;
  bool bVar3;
  char cVar4;
  int iVar5;
  undefined4 uVar6;
  uint uVar7;
  int iVar8;
  int *piVar9;
  size_t sVar10;
  void *pvVar11;
  int *piVar12;
  char *pcVar13;
  long lVar14;
  int iVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  addrinfo *paVar18;
  __fd_mask *p_Var19;
  int local_3380;
  char *local_337c;
  char *local_3378;
  addrinfo *paStack_3360;
  undefined4 uStack_335c;
  socklen_t sStack_3358;
  int iStack_3354;
  timeval tStack_3350;
  addrinfo local_3348;
  int aiStack_3328 [23];
  char acStack_32cc [160];
  fd_set local_322c [4];
  char local_3028 [4];
  char acStack_3024 [4];
  char acStack_3020 [4];
  char acStack_301c [4];
  char local_3018 [4];
  char acStack_3014 [4];
  undefined2 local_3010 [2];
  char local_300c [4];
  char local_3008 [4];
  uint local_3004;
  uint local_3000;
  char acStack_2ffc [4];
  undefined2 uStack_2ff8;
  undefined1 local_2ff6;
  char acStack_2028 [8196];
  
  local_3380 = 0;
  bVar2 = 0;
  bVar3 = false;
  do {
    iVar5 = pthread_mutex_lock((pthread_mutex_t *)(param_1 + 0xb6));
    if (iVar5 != 0) {
      FUN_0000b708("setup_stratum_socket",0xc5f);
    }
    *(undefined1 *)((int)param_1 + 0x299) = 0;
    if (param_1[0x99] != 0) {
      close(param_1[0x99]);
    }
    param_1[0x99] = 0;
    iVar5 = pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0xb6));
    if (iVar5 != 0) {
      piVar9 = __errno_location();
      snprintf(local_3028,0x1000,"WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d",*piVar9,
               "util.c","setup_stratum_socket",0xc64);
      FUN_00038730(3,local_3028,1);
      FUN_00016ca8(1);
    }
    (*DAT_00075ed8)();
    iVar5 = DAT_00078b78;
    local_3348.ai_family = 0;
    local_3348.ai_flags = 0;
    local_3348.ai_protocol = 0;
    local_3348.ai_addrlen = 0;
    local_3348.ai_addr = (sockaddr *)0x0;
    local_3348.ai_canonname = (char *)0x0;
    local_3348.ai_next = (addrinfo *)0x0;
    local_3348.ai_socktype = 1;
    if (param_1[0x36] == 0) {
      if (DAT_00078b78 != 0) {
        param_1[0x36] = DAT_00078b78;
        FUN_0002ab54(iVar5,param_1 + 0x9d,param_1 + 0x9e);
        param_1[0x35] = 3;
        if (param_1[0x36] != 0) goto LAB_0002bb50;
      }
      local_337c = (char *)param_1[0x9c];
      local_3378 = (char *)param_1[0x98];
    }
    else {
LAB_0002bb50:
      local_337c = (char *)param_1[0x9d];
      local_3378 = (char *)param_1[0x9e];
    }
    iVar5 = getaddrinfo(local_337c,local_3378,&local_3348,&paStack_3360);
    if (iVar5 == 0) {
      paVar18 = paStack_3360;
      if (paStack_3360 == (addrinfo *)0x0) {
LAB_0002be64:
        if ((DAT_00078e09 != '\0') &&
           (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
          snprintf(local_3028,0x1000,"Failed to connect to stratum on %s:%s",local_337c,local_3378);
          FUN_00038730(7,local_3028,0);
        }
        freeaddrinfo(paStack_3360);
        goto LAB_0002b994;
      }
LAB_0002bc24:
      iVar5 = socket(paVar18->ai_family,paVar18->ai_socktype,paVar18->ai_protocol);
      if (iVar5 == -1) {
        if ((DAT_00078e09 != '\0') &&
           (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
          pcVar13 = local_3028;
          uVar6._0_1_ = 'F';
          uVar6._1_1_ = 'a';
          uVar6._2_1_ = 'i';
          uVar6._3_1_ = 'l';
          uVar16._0_1_ = 'e';
          uVar16._1_1_ = 'd';
          uVar16._2_1_ = ' ';
          uVar16._3_1_ = 's';
          uVar17._0_1_ = 'o';
          uVar17._1_1_ = 'c';
          uVar17._2_1_ = 'k';
          uVar17._3_1_ = 'e';
LAB_0002bc00:
          *(undefined4 *)pcVar13 = uVar6;
          *(undefined4 *)(pcVar13 + 4) = uVar16;
          *(undefined4 *)(pcVar13 + 8) = uVar17;
          pcVar13[0xc] = 't';
          pcVar13[0xd] = '\0';
          FUN_00038730(7,local_3028,0);
        }
        goto LAB_0002bc18;
      }
      uVar7 = fcntl(iVar5,3,0);
      fcntl(iVar5,4,uVar7 | 0x800);
      iVar8 = connect(iVar5,paVar18->ai_addr,paVar18->ai_addrlen);
      if (iVar8 == -1) {
        tStack_3350.tv_sec = 1;
        tStack_3350.tv_usec = 0;
        piVar9 = __errno_location();
        if (*piVar9 == 0x73) {
          iVar8 = iVar5 + 0x1f;
          if (-1 < iVar5) {
            iVar8 = iVar5;
          }
          p_Var19 = local_322c[0].fds_bits + (iVar8 >> 5);
          uVar7 = 0;
LAB_0002bcec:
          if (uVar7 < 0x20) {
LAB_0002bdb8:
            local_322c[0].fds_bits[uVar7] = 0;
            uVar7 = uVar7 + 1;
            goto LAB_0002bcec;
          }
          *p_Var19 = 1 << (iVar5 % 0x20 & 0xffU) | *p_Var19;
          iVar8 = select(iVar5 + 1,(fd_set *)0x0,local_322c,(fd_set *)0x0,&tStack_3350);
          if (iVar8 < 1) {
            uVar7 = 0;
            if ((iVar8 == 0) || (*piVar9 != 4)) goto LAB_0002bd38;
            goto LAB_0002bdb8;
          }
          if ((*p_Var19 >> (iVar5 % 0x20 & 0xffU) & 1U) != 0) {
            sStack_3358 = 4;
            iVar8 = getsockopt(iVar5,1,4,&iStack_3354,&sStack_3358);
            if ((iVar8 == 0) && (iStack_3354 == 0)) {
              if ((DAT_00078e09 != '\0') &&
                 (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
                local_3028[0] = 'S';
                local_3028[1] = 'u';
                local_3028[2] = 'c';
                local_3028[3] = 'c';
                acStack_3024[0] = 'e';
                acStack_3024[1] = 'e';
                acStack_3024[2] = 'd';
                acStack_3024[3] = 'e';
                acStack_3020[0] = 'd';
                acStack_3020[1] = ' ';
                acStack_3020[2] = 'd';
                acStack_3020[3] = 'e';
                acStack_301c[0] = 'l';
                acStack_301c[1] = 'a';
                acStack_301c[2] = 'y';
                acStack_301c[3] = 'e';
                local_3018[0] = 'd';
                local_3018[1] = ' ';
                local_3018[2] = 'c';
                local_3018[3] = 'o';
                acStack_3014[0] = 'n';
                acStack_3014[1] = 'n';
                acStack_3014[2] = 'e';
                acStack_3014[3] = 'c';
                local_3010[0] = 0x74;
                FUN_00038730(7,local_3028,0);
              }
              goto LAB_0002bf8c;
            }
          }
LAB_0002bd38:
          close(iVar5);
          if ((DAT_00078e09 != '\0') &&
             (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
            local_3028[0] = 'S';
            local_3028[1] = 'e';
            local_3028[2] = 'l';
            local_3028[3] = 'e';
            acStack_3024[0] = 'c';
            acStack_3024[1] = 't';
            acStack_3024[2] = ' ';
            acStack_3024[3] = 't';
            acStack_3020[0] = 'i';
            acStack_3020[1] = 'm';
            acStack_3020[2] = 'e';
            acStack_3020[3] = 'o';
            acStack_301c[0] = 'u';
            acStack_301c[1] = 't';
            acStack_301c[2] = '/';
            acStack_301c[3] = 'f';
            pcVar13 = local_3018;
            uVar6._0_1_ = 'a';
            uVar6._1_1_ = 'i';
            uVar6._2_1_ = 'l';
            uVar6._3_1_ = 'e';
            uVar16._0_1_ = 'd';
            uVar16._1_1_ = ' ';
            uVar16._2_1_ = 'c';
            uVar16._3_1_ = 'o';
            uVar17._0_1_ = 'n';
            uVar17._1_1_ = 'n';
            uVar17._2_1_ = 'e';
            uVar17._3_1_ = 'c';
            goto LAB_0002bc00;
          }
LAB_0002bc18:
          paVar18 = paVar18->ai_next;
        }
        else {
          close(iVar5);
          if ((DAT_00078e09 == '\0') ||
             (((DAT_0007ad48 == '\0' && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7))))
          goto LAB_0002bc18;
          local_3028[0] = 'F';
          local_3028[1] = 'a';
          local_3028[2] = 'i';
          local_3028[3] = 'l';
          acStack_3024[0] = 'e';
          acStack_3024[1] = 'd';
          acStack_3024[2] = ' ';
          acStack_3024[3] = 's';
          acStack_3020[0] = 'o';
          acStack_3020[1] = 'c';
          acStack_3020[2] = 'k';
          acStack_3020[3] = ' ';
          acStack_301c[0] = 'c';
          acStack_301c[1] = 'o';
          acStack_301c[2] = 'n';
          acStack_301c[3] = 'n';
          local_3018[0] = 'e';
          local_3018[1] = 'c';
          local_3018[2] = 't';
          local_3018[3] = '\0';
          FUN_00038730(7,local_3028,0);
          paVar18 = paVar18->ai_next;
        }
        if (paVar18 == (addrinfo *)0x0) goto LAB_0002be64;
        goto LAB_0002bc24;
      }
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
        local_3028[0] = 'S';
        local_3028[1] = 'u';
        local_3028[2] = 'c';
        local_3028[3] = 'c';
        acStack_3024[0] = 'e';
        acStack_3024[1] = 'e';
        acStack_3024[2] = 'd';
        acStack_3024[3] = 'e';
        acStack_3020[0] = 'd';
        acStack_3020[1] = ' ';
        acStack_3020[2] = 'i';
        acStack_3020[3] = 'm';
        acStack_301c[0] = 'm';
        acStack_301c[1] = 'e';
        acStack_301c[2] = 'd';
        acStack_301c[3] = 'i';
        local_3018[0] = 'a';
        local_3018[1] = 't';
        local_3018[2] = 'e';
        local_3018[3] = ' ';
        acStack_3014[0] = 'c';
        acStack_3014[1] = 'o';
        acStack_3014[2] = 'n';
        acStack_3014[3] = 'n';
        local_3010[0]._0_1_ = 'e';
        local_3010[0]._1_1_ = 'c';
        local_3010[1]._0_1_ = 't';
        local_3010[1]._1_1_ = '\0';
        FUN_00038730(4,local_3028,0);
      }
LAB_0002bf8c:
      FUN_00028b9c(iVar5);
      freeaddrinfo(paStack_3360);
      if (param_1[0x36] == 0) goto LAB_0002c0d0;
      switch(param_1[0x35]) {
      case 0:
        iVar8 = FUN_00028bec(param_1,iVar5,0);
        if (iVar8 == 0) goto LAB_0002b994;
        iVar8 = param_1[0x9a];
        goto joined_r0x0002c0d8;
      case 1:
        iVar8 = FUN_00028bec(param_1,iVar5,1);
        break;
      case 2:
        iVar8 = FUN_00028fc8(param_1,iVar5,0);
        break;
      case 3:
      case 5:
        local_322c[0].fds_bits[0]._0_1_ = 5;
        local_322c[0].fds_bits[0]._1_1_ = 1;
        local_322c[0].fds_bits[0]._2_1_ = 0;
        if ((DAT_00078e09 != '\0') &&
           (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
          snprintf(local_3028,0x1000,"Attempting to negotiate with %s:%s SOCKS5 proxy",
                   (char *)param_1[0x9d],(char *)param_1[0x9e]);
          FUN_00038730(7,local_3028,0);
        }
        send(iVar5,local_322c,3,0);
        iVar8 = FUN_00028bc4(iVar5);
        if ((iVar8 != 5) || (uVar7 = FUN_00028bc4(iVar5), uVar7 != local_322c[0].fds_bits[0]._2_1_))
        {
LAB_0002c360:
          if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
            snprintf(local_3028,0x1000,"Bad response from %s:%s SOCKS5 server",(char *)param_1[0x9d]
                     ,(char *)param_1[0x9e]);
            FUN_00038730(4,local_3028,0);
          }
          goto LAB_0002b994;
        }
        pcVar13 = (char *)param_1[0x9c];
        local_322c[0].fds_bits[0]._1_1_ = 1;
        local_322c[0].fds_bits[0]._0_1_ = 5;
        local_322c[0].fds_bits[0]._2_1_ = 0;
        local_322c[0].fds_bits[0]._3_1_ = 3;
        sVar10 = strlen(pcVar13);
        if (0xfe < (int)sVar10) {
          sVar10 = 0xff;
        }
        local_322c[0].fds_bits[1]._0_1_ = (undefined1)sVar10;
        memcpy((void *)((int)local_322c[0].fds_bits + 5),pcVar13,sVar10);
        lVar14 = strtol((char *)param_1[0x98],(char **)0x0,10);
        *(char *)((int)local_322c[0].fds_bits + sVar10 + 6) = (char)lVar14;
        *(char *)((int)local_322c[0].fds_bits + sVar10 + 5) = (char)((uint)lVar14 >> 8);
        send(iVar5,local_322c,sVar10 + 7,0);
        iVar8 = FUN_00028bc4(iVar5);
        if ((iVar8 != 5) || (iVar8 = FUN_00028bc4(iVar5), iVar8 != 0)) goto LAB_0002c360;
        FUN_00028bc4(iVar5);
        cVar4 = FUN_00028bc4(iVar5);
        if (cVar4 == '\x01') {
          FUN_00028bc4(iVar5);
          FUN_00028bc4(iVar5);
          FUN_00028bc4(iVar5);
          FUN_00028bc4(iVar5);
          goto LAB_0002ca28;
        }
        if (cVar4 != '\x03') goto LAB_0002c360;
        iVar15 = FUN_00028bc4(iVar5);
        iVar8 = 0;
        if (0 < iVar15) {
          do {
            iVar8 = iVar8 + 1;
            FUN_00028bc4(iVar5);
          } while (iVar8 != iVar15);
        }
LAB_0002ca28:
        FUN_00028bc4(iVar5);
        FUN_00028bc4(iVar5);
        if ((DAT_00078e09 != '\0') &&
           (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
          snprintf(local_3028,0x1000,"Success negotiating with %s:%s SOCKS5 proxy",
                   (char *)param_1[0x9d],(char *)param_1[0x9e]);
          FUN_00038730(7,local_3028,0);
        }
LAB_0002c0d0:
        iVar8 = param_1[0x9a];
joined_r0x0002c0d8:
        if (iVar8 == 0) {
          pvVar11 = calloc(0x2000,1);
          param_1[0x9a] = (int)pvVar11;
          if (pvVar11 == (void *)0x0) {
            snprintf(local_3028,0x1000,"Failed to calloc pool sockbuf in %s %s():%d","util.c",
                     "setup_stratum_socket",0xcef);
            FUN_00038730(3,local_3028,1);
            FUN_00016ca8(1);
          }
          param_1[0x9b] = 0x2000;
        }
        param_1[0x99] = iVar5;
        uStack_335c = 1;
        sStack_3358 = 0x2d;
        iStack_3354 = 0x1e;
        uVar7 = fcntl(iVar5,3,0);
        fcntl(iVar5,4,uVar7 | 0x800);
        setsockopt(iVar5,1,9,&uStack_335c,4);
        if (DAT_0007bd20 == '\0') {
          fcntl(iVar5,2,1);
        }
        setsockopt(iVar5,6,1,&uStack_335c,4);
        setsockopt(iVar5,6,6,&uStack_335c,4);
        setsockopt(iVar5,6,4,&sStack_3358,4);
        setsockopt(iVar5,6,5,&iStack_3354,4);
        iVar5 = DAT_0007b0f8;
        if (bVar3) {
          FUN_000295a8(param_1);
          iVar5 = DAT_0007b0f8;
          DAT_0007b0f8 = DAT_0007b0f8 + 1;
          sprintf(acStack_2028,"{\"id\": %d, \"method\": \"mining.subscribe\", \"params\": []}",
                  iVar5);
        }
        else if ((char *)param_1[0xa5] == (char *)0x0) {
          DAT_0007b0f8 = DAT_0007b0f8 + 1;
          sprintf(acStack_2028,
                  "{\"id\":%d, \"method\":\"mining.subscribe\", \"params\":[\"cgminer/4.9.0\", null, \"%s\", \"%s\"]}"
                  ,iVar5,(char *)param_1[0x9c],(char *)param_1[0x98]);
        }
        else {
          DAT_0007b0f8 = DAT_0007b0f8 + 1;
          sprintf(acStack_2028,
                  "{\"id\": %d, \"method\": \"mining.subscribe\", \"params\": [\"cgminer/4.9.0\", \"%s\"]}"
                  ,iVar5,(char *)param_1[0xa5]);
        }
        sVar10 = strlen(acStack_2028);
        iVar5 = FUN_00029404(param_1,acStack_2028,sVar10);
        if (iVar5 != 0) {
          if ((DAT_00078e09 == '\0') ||
             (((DAT_0007ad48 == '\0' && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7)))) {
LAB_0002c500:
            bVar1 = true;
          }
          else {
            bVar1 = true;
            local_3028[0] = 'F';
            local_3028[1] = 'a';
            local_3028[2] = 'i';
            local_3028[3] = 'l';
            acStack_3024[0] = 'e';
            acStack_3024[1] = 'd';
            acStack_3024[2] = ' ';
            acStack_3024[3] = 't';
            acStack_3020[0] = 'o';
            acStack_3020[1] = ' ';
            acStack_3020[2] = 's';
            acStack_3020[3] = 'e';
            acStack_301c[0] = 'n';
            acStack_301c[1] = 'd';
            acStack_301c[2] = ' ';
            acStack_301c[3] = 's';
            local_3018[0] = ' ';
            local_3018[1] = 'i';
            local_3018[2] = 'n';
            local_3018[3] = ' ';
            acStack_3014[0] = 'i';
            acStack_3014[1] = 'n';
            acStack_3014[2] = 'i';
            acStack_3014[3] = 't';
            local_3010[0]._0_1_ = 'i';
            local_3010[0]._1_1_ = 'a';
            local_3010[1]._0_1_ = 't';
            local_3010[1]._1_1_ = 'e';
            local_300c[0] = '_';
            local_300c[1] = 's';
            local_300c[2] = 't';
            local_300c[3] = 'r';
            local_3008[0] = 'a';
            local_3008[1] = 't';
            local_3008[2] = 'u';
            local_3008[3] = 'm';
            local_3004 = local_3004 & 0xffffff00;
            FUN_00038730(7,local_3028,0);
          }
          goto LAB_0002b998;
        }
        iVar5 = FUN_00028afc(param_1,0x3c);
        if (iVar5 == 0) {
          if ((DAT_00078e09 == '\0') ||
             (((DAT_0007ad48 == '\0' && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7))))
          goto LAB_0002c500;
          bVar1 = true;
          local_3028[0] = 'T';
          local_3028[1] = 'i';
          local_3028[2] = 'm';
          local_3028[3] = 'e';
          acStack_3024[0] = 'd';
          acStack_3024[1] = ' ';
          acStack_3024[2] = 'o';
          acStack_3024[3] = 'u';
          acStack_3020[0] = 't';
          acStack_3020[1] = ' ';
          acStack_3020[2] = 'w';
          acStack_3020[3] = 'a';
          acStack_301c[0] = 'i';
          acStack_301c[1] = 't';
          acStack_301c[2] = 'i';
          acStack_301c[3] = 'n';
          local_3018[0] = 'g';
          local_3018[1] = ' ';
          local_3018[2] = 'f';
          local_3018[3] = 'o';
          acStack_3014[0] = 'r';
          acStack_3014[1] = ' ';
          acStack_3014[2] = 'r';
          acStack_3014[3] = 'e';
          local_3010[0]._0_1_ = 's';
          local_3010[0]._1_1_ = 'p';
          local_3010[1]._0_1_ = 'o';
          local_3010[1]._1_1_ = 'n';
          local_300c[0] = 's';
          local_300c[1] = 'e';
          local_300c[2] = ' ';
          local_300c[3] = 'i';
          local_3008[0] = 'n';
          local_3008[1] = ' ';
          local_3008[2] = 'i';
          local_3008[3] = 'n';
          local_3004._0_1_ = 'i';
          local_3004._1_1_ = 't';
          local_3004._2_1_ = 'i';
          local_3004._3_1_ = 'a';
          local_3000._0_1_ = 't';
          local_3000._1_1_ = 'e';
          local_3000._2_1_ = '_';
          local_3000._3_1_ = 's';
          acStack_2ffc[0] = 't';
          acStack_2ffc[1] = 'r';
          acStack_2ffc[2] = 'a';
          acStack_2ffc[3] = 't';
          uStack_2ff8 = 0x6d75;
          local_2ff6 = 0;
          FUN_00038730(7,local_3028,0);
        }
        else {
          pvVar11 = (void *)FUN_0002b040(param_1);
          if (pvVar11 == (void *)0x0) {
            bVar1 = true;
          }
          else {
            local_3380 = FUN_00059230(pvVar11,0,aiStack_3328);
            free(pvVar11);
            if (local_3380 == 0) {
              if (DAT_00078e09 == '\0') {
                bVar3 = true;
                bVar1 = true;
              }
              else if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7)) {
                bVar3 = true;
                bVar1 = true;
              }
              else {
                snprintf(local_3028,0x1000,"JSON decode failed(%d): %s",aiStack_3328[0],acStack_32cc
                        );
LAB_0002cc54:
                bVar1 = true;
                FUN_00038730(7,local_3028,0);
                bVar3 = true;
              }
            }
            else {
              piVar9 = (int *)FUN_00059e20(local_3380,"result");
              piVar12 = (int *)FUN_00059e20(local_3380,"error");
              if ((piVar9 == (int *)0x0) || (*piVar9 == 7)) {
                if (piVar12 != (int *)0x0) goto LAB_0002c750;
                pcVar13 = malloc(0x11);
                if (pcVar13 != (char *)0x0) {
                  builtin_strncpy(pcVar13,"(unknown reason)",0x11);
                }
              }
              else {
                if ((piVar12 == (int *)0x0) || (*piVar12 == 7)) {
                  iVar5 = FUN_000299ac(param_1,piVar9,1);
                  if (iVar5 == 0) {
                    if ((DAT_00078e09 != '\0') &&
                       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
                      snprintf(local_3028,0x1000,"%s: Failed to get parse extranonce.",
                               "initiate_stratum");
                      goto LAB_0002cc54;
                    }
                    bVar1 = true;
                    bVar3 = true;
                    goto LAB_0002b998;
                  }
                  if (param_1[0x96] == 0) {
                    param_1[0x96] = param_1[0x9c];
                  }
                  cVar4 = DAT_0007b380;
                  *(undefined1 *)((int)param_1 + 0x299) = 1;
                  param_1[0x1cc] = 0;
                  param_1[0x1cd] = 0x3ff00000;
                  if (((cVar4 != '\0') && (DAT_00078e09 != '\0')) &&
                     ((DAT_0007ad48 != '\0' || ((DAT_00078e08 != '\0' || (6 < DAT_000766c4)))))) {
                    snprintf(local_3028,0x1000,
                             "Pool %d confirmed mining.subscribe with extranonce1 %s extran2size %d"
                             ,*param_1,(char *)param_1[0x9f],param_1[0xa4]);
                    FUN_00038730(7,local_3028,0);
                  }
                  iVar8 = DAT_0007b0f8;
                  if ((char)param_1[0x97] != '\0') {
                    DAT_0007b0f8 = DAT_0007b0f8 + 1;
                    sprintf(acStack_2028,
                            "{\"id\": %d, \"method\": \"mining.extranonce.subscribe\", \"params\": []}"
                            ,iVar8);
                    sVar10 = strlen(acStack_2028);
                    FUN_0002b5c8(param_1,acStack_2028,sVar10);
                    pvVar11 = (void *)FUN_0002b040(param_1);
                    if (pvVar11 == (void *)0x0) {
                      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4))
                      {
                        local_3028[0] = 'r';
                        local_3028[1] = 'e';
                        local_3028[2] = 'c';
                        local_3028[3] = 'v';
                        acStack_3024[0] = ' ';
                        acStack_3024[1] = 'm';
                        acStack_3024[2] = 'i';
                        acStack_3024[3] = 'n';
                        acStack_3020[0] = 'i';
                        acStack_3020[1] = 'n';
                        acStack_3020[2] = 'g';
                        acStack_3020[3] = '.';
                        acStack_301c[0] = 'e';
                        acStack_301c[1] = 'x';
                        acStack_301c[2] = 't';
                        acStack_301c[3] = 'r';
                        local_3018[0] = 'a';
                        local_3018[1] = 'n';
                        local_3018[2] = 'o';
                        local_3018[3] = 'n';
                        acStack_3014[0] = 'c';
                        acStack_3014[1] = 'e';
                        acStack_3014[2] = '.';
                        acStack_3014[3] = 's';
                        local_3010[0]._0_1_ = 'u';
                        local_3010[0]._1_1_ = 'b';
                        local_3010[1]._0_1_ = 's';
                        local_3010[1]._1_1_ = 'c';
                        local_300c[0] = 'r';
                        local_300c[1] = 'i';
                        local_300c[2] = 'b';
                        local_300c[3] = 'e';
                        local_3008[0] = ' ';
                        local_3008[1] = 'f';
                        local_3008[2] = 'a';
                        local_3008[3] = 'i';
                        local_3004._0_1_ = 'l';
                        local_3004._1_1_ = 'e';
                        local_3004._2_1_ = 'd';
                        local_3004._3_1_ = '\n';
                        local_3000 = local_3000 & 0xffffff00;
                        FUN_00038730(3,local_3028,0);
                      }
                    }
                    else {
                      free(pvVar11);
                    }
                  }
                  goto LAB_0002c600;
                }
LAB_0002c750:
                pcVar13 = (char *)FUN_00057854(piVar12,3);
              }
              if ((DAT_00078e09 != '\0') &&
                 (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
                snprintf(local_3028,0x1000,"JSON-RPC decode failed: %s",pcVar13);
                FUN_00038730(7,local_3028,0);
              }
              bVar1 = true;
              free(pcVar13);
              bVar3 = true;
            }
          }
        }
        goto LAB_0002b998;
      case 4:
        iVar8 = FUN_00028fc8(param_1,iVar5,1);
        break;
      default:
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
          snprintf(local_3028,0x1000,"Unsupported proxy type for %s:%s",(char *)param_1[0x9d],
                   (char *)param_1[0x9e]);
          FUN_00038730(4,local_3028,0);
        }
        goto LAB_0002b994;
      }
      if (iVar8 != 0) goto LAB_0002c0d0;
      goto LAB_0002b994;
    }
    if (*(char *)((int)param_1 + 0x83) == '\0') {
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
        snprintf(local_3028,0x1000,"Failed to resolve (?wrong URL) %s:%s",local_337c,local_3378);
        FUN_00038730(4,local_3028,0);
      }
      *(undefined1 *)((int)param_1 + 0x83) = 1;
    }
    else if ((DAT_00078e09 != '\0') &&
            (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      snprintf(local_3028,0x1000,"Failed to getaddrinfo for %s:%s",local_337c,local_3378);
      FUN_00038730(7,local_3028,0);
    }
LAB_0002b994:
    bVar1 = false;
LAB_0002b998:
    if (!(bool)(bVar3 & (bVar2 ^ 1))) {
      if ((DAT_00078e09 != '\0') &&
         (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
        local_3028[0] = 'I';
        local_3028[1] = 'n';
        local_3028[2] = 'i';
        local_3028[3] = 't';
        acStack_3024[0] = 'i';
        acStack_3024[1] = 'a';
        acStack_3024[2] = 't';
        acStack_3024[3] = 'e';
        acStack_3020[0] = ' ';
        acStack_3020[1] = 's';
        acStack_3020[2] = 't';
        acStack_3020[3] = 'r';
        acStack_301c[0] = 'a';
        acStack_301c[1] = 't';
        acStack_301c[2] = 'u';
        acStack_301c[3] = 'm';
        local_3018[0] = ' ';
        local_3018[1] = 'f';
        local_3018[2] = 'a';
        local_3018[3] = 'i';
        acStack_3014[0] = 'l';
        acStack_3014[1] = 'e';
        acStack_3014[2] = 'd';
        acStack_3014[3] = '\0';
        FUN_00038730(7,local_3028,0);
      }
      if (bVar1) {
        FUN_0002aee0(param_1);
      }
      if (local_3380 == 0) {
        iVar5 = 0;
      }
      else {
        iVar5 = 0;
LAB_0002c600:
        if ((*(int *)(local_3380 + 4) != -1) &&
           (iVar8 = *(int *)(local_3380 + 4) + -1, *(int *)(local_3380 + 4) = iVar8, iVar8 == 0)) {
          FUN_0005a3e4(local_3380);
        }
      }
      return iVar5;
    }
    iVar5 = pthread_mutex_lock((pthread_mutex_t *)(param_1 + 0x3d));
    if (iVar5 != 0) {
      FUN_0000b708("initiate_stratum",0xe18);
    }
    iVar5 = pthread_rwlock_wrlock((pthread_rwlock_t *)(param_1 + 0x43));
    if (iVar5 != 0) {
      piVar9 = __errno_location();
      snprintf(local_3028,0x1000,"WTF WRLOCK ERROR ON LOCK! errno=%d in %s %s():%d",*piVar9,"util.c"
               ,"initiate_stratum",0xe18);
      FUN_00038730(3,local_3028,1);
      FUN_00016ca8(1);
    }
    free((void *)param_1[0xa5]);
    free((void *)param_1[0x9f]);
    param_1[0x9f] = 0;
    param_1[0xa5] = 0;
    iVar5 = pthread_rwlock_unlock((pthread_rwlock_t *)(param_1 + 0x43));
    if (iVar5 != 0) {
      piVar9 = __errno_location();
      snprintf(local_3028,0x1000,"WTF RWLOCK ERROR ON UNLOCK! errno=%d in %s %s():%d",*piVar9,
               "util.c","initiate_stratum",0xe1c);
      FUN_00038730(3,local_3028,1);
      FUN_00016ca8(1);
    }
    iVar5 = pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0x3d));
    if (iVar5 != 0) {
      piVar9 = __errno_location();
      snprintf(local_3028,0x1000,"WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d",*piVar9,
               "util.c","initiate_stratum",0xe1c);
      FUN_00038730(3,local_3028,1);
      FUN_00016ca8(1);
    }
    (*DAT_00075ed8)();
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      local_3028[0] = 'F';
      local_3028[1] = 'a';
      local_3028[2] = 'i';
      local_3028[3] = 'l';
      acStack_3024[0] = 'e';
      acStack_3024[1] = 'd';
      acStack_3024[2] = ' ';
      acStack_3024[3] = 't';
      acStack_3020[0] = 'o';
      acStack_3020[1] = ' ';
      acStack_3020[2] = 'r';
      acStack_3020[3] = 'e';
      acStack_301c[0] = 's';
      acStack_301c[1] = 'u';
      acStack_301c[2] = 'm';
      acStack_301c[3] = 'e';
      local_3018[0] = ' ';
      local_3018[1] = 's';
      local_3018[2] = 't';
      local_3018[3] = 'r';
      acStack_3014[0] = 'a';
      acStack_3014[1] = 't';
      acStack_3014[2] = 'u';
      acStack_3014[3] = 'm';
      local_3010[0]._0_1_ = ',';
      local_3010[0]._1_1_ = ' ';
      local_3010[1]._0_1_ = 't';
      local_3010[1]._1_1_ = 'r';
      local_300c[0] = 'y';
      local_300c[1] = 'i';
      local_300c[2] = 'n';
      local_300c[3] = 'g';
      local_3008[0] = ' ';
      local_3008[1] = 'a';
      local_3008[2] = 'f';
      local_3008[3] = 'r';
      local_3004._0_1_ = 'e';
      local_3004._1_1_ = 's';
      local_3004._2_1_ = 'h';
      local_3004._3_1_ = '\0';
      FUN_00038730(7,local_3028,0);
    }
    if (((local_3380 != 0) && (*(int *)(local_3380 + 4) != -1)) &&
       (iVar5 = *(int *)(local_3380 + 4) + -1, *(int *)(local_3380 + 4) = iVar5, iVar5 == 0)) {
      FUN_0005a3e4(local_3380);
    }
    bVar2 = 1;
  } while( true );
}

