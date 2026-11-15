//==================== FUN_0003107c @ 0003107c ====================

/* WARNING: Restarted to delay deadcode elimination for space: stack */

void FUN_0003107c(void)

{
  int iVar1;
  int iVar2;
  time_t tVar3;
  size_t sVar4;
  uint __n;
  char *pcVar5;
  uint param3;
  int iVar6;
  time_t tVar7;
  long param4;
  ssize_t param4_00;
  int *piVar8;
  sockaddr *psVar9;
  char *pcVar10;
  addrinfo *paVar11;
  char *param4_01;
  int local_1908;
  char *local_1904;
  undefined1 uStack_18fd;
  socklen_t sStack_18fc;
  char *pcStack_18f8;
  addrinfo *paStack_18f4;
  undefined4 uStack_18f0;
  char local_18ec [4];
  char acStack_18e8 [4];
  undefined1 local_18e4;
  char acStack_18e0 [12];
  char acStack_18d4 [12];
  addrinfo local_18c8;
  sockaddr asStack_18a8 [7];
  char acStack_1829 [10];
  char acStack_181f [1015];
  undefined4 local_1428;
  undefined4 local_1424;
  undefined4 uStack_1420;
  undefined4 uStack_141c;
  undefined4 local_1418;
  char local_1028 [4];
  char acStack_1024 [4];
  char acStack_1020 [4];
  char acStack_101c [4];
  char local_1018 [4];
  char acStack_1014 [4];
  char acStack_1010 [4];
  undefined2 local_100c [2];
  undefined2 uStack_1008;
  undefined1 local_1006;
  
  local_18ec[0] = 'c';
  local_18ec[1] = 'g';
  local_18ec[2] = 'm';
  local_18ec[3] = 'i';
  acStack_18e8[0] = 'n';
  acStack_18e8[1] = 'e';
  acStack_18e8[2] = 'r';
  acStack_18e8[3] = '-';
  local_18e4 = 0;
  sprintf(acStack_18e0,"%d",DAT_00075ebc);
  local_18c8.ai_flags = 0;
  local_18c8.ai_family = 0;
  local_18c8.ai_socktype = 0;
  local_18c8.ai_protocol = 0;
  local_18c8.ai_addrlen = 0;
  local_18c8.ai_addr = (sockaddr *)0x0;
  local_18c8.ai_canonname = (char *)0x0;
  local_18c8.ai_next = (addrinfo *)0x0;
  iVar1 = getaddrinfo(DAT_00075ec8,acStack_18e0,&local_18c8,&paStack_18f4);
  if (iVar1 != 0) {
    local_1028[0] = 'I';
    local_1028[1] = 'n';
    local_1028[2] = 'v';
    local_1028[3] = 'a';
    acStack_1024[0] = 'l';
    acStack_1024[1] = 'i';
    acStack_1024[2] = 'd';
    acStack_1024[3] = ' ';
    acStack_1020[0] = 'A';
    acStack_1020[1] = 'P';
    acStack_1020[2] = 'I';
    acStack_1020[3] = ' ';
    acStack_101c[0] = 'M';
    acStack_101c[1] = 'u';
    acStack_101c[2] = 'l';
    acStack_101c[3] = 't';
    local_1018[0] = 'i';
    local_1018[1] = 'c';
    local_1018[2] = 'a';
    local_1018[3] = 's';
    acStack_1014[0] = 't';
    acStack_1014[1] = ' ';
    acStack_1014[2] = 'A';
    acStack_1014[3] = 'd';
    acStack_1010[0] = 'd';
    acStack_1010[1] = 'r';
    acStack_1010[2] = 'e';
    acStack_1010[3] = 's';
    local_100c[0] = 0x73;
    FUN_00038730(3,local_1028,1);
    FUN_00016ca8(1);
  }
  paVar11 = paStack_18f4;
  if (paStack_18f4 == (addrinfo *)0x0) {
LAB_000314fc:
    freeaddrinfo(paStack_18f4);
    paVar11 = (addrinfo *)0x0;
    iVar1 = -1;
    local_1028[0] = 'A';
    local_1028[1] = 'P';
    local_1028[2] = 'I';
    local_1028[3] = ' ';
    acStack_1024[0] = 'm';
    acStack_1024[1] = 'c';
    acStack_1024[2] = 'a';
    acStack_1024[3] = 's';
    acStack_1020[0] = 't';
    acStack_1020[1] = ' ';
    acStack_1020[2] = 'c';
    acStack_1020[3] = 'o';
    acStack_101c[0] = 'u';
    acStack_101c[1] = 'l';
    acStack_101c[2] = 'd';
    acStack_101c[3] = ' ';
    local_1018[0] = 'n';
    local_1018[1] = 'o';
    local_1018[2] = 't';
    local_1018[3] = ' ';
    acStack_1014[0] = 'o';
    acStack_1014[1] = 'p';
    acStack_1014[2] = 'e';
    acStack_1014[3] = 'n';
    acStack_1010[0] = ' ';
    acStack_1010[1] = 's';
    acStack_1010[2] = 'o';
    acStack_1010[3] = 'c';
    local_100c[0]._0_1_ = 'k';
    local_100c[0]._1_1_ = 'e';
    local_100c[1]._0_1_ = 't';
    local_100c[1]._1_1_ = '\0';
    FUN_00038730(3,local_1028,1);
    FUN_00016ca8(1);
  }
  else {
    do {
      iVar1 = socket(paStack_18f4->ai_family,2,0);
      if (0 < iVar1) goto LAB_00031148;
      paVar11 = paVar11->ai_next;
    } while (paVar11 != (addrinfo *)0x0);
    if (iVar1 == -1) goto LAB_000314fc;
  }
LAB_00031148:
  uStack_18f0 = 1;
  iVar2 = setsockopt(iVar1,1,2,&uStack_18f0,4);
  if (iVar2 < 0) {
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) goto LAB_000315a8;
    piVar8 = __errno_location();
    pcVar10 = strerror(*piVar8);
    pcVar5 = "API mcast setsockopt SO_REUSEADDR failed (%s)%s";
  }
  else {
    tVar3 = time((time_t *)0x0);
    while (iVar2 = bind(iVar1,paVar11->ai_addr,paVar11->ai_addrlen), iVar2 < 0) {
      piVar8 = __errno_location();
      pcVar10 = strerror(*piVar8);
      tVar7 = time((time_t *)0x0);
      if (0x3d < tVar7 - tVar3) {
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
          snprintf(local_1028,0x1000,"API mcast bind to port %d failed (%s)%s",DAT_00075ebc,pcVar10,
                   " - API multicast listener will not be available");
          FUN_00038730(3,local_1028,0);
        }
        goto LAB_000315a8;
      }
      FUN_0002a92c(30000);
    }
    if (paVar11->ai_family == 2) {
      local_1424 = 0;
      local_1428 = *(undefined4 *)(paVar11->ai_addr->sa_data + 2);
      iVar2 = setsockopt(iVar1,0,0x23,&local_1428,8);
    }
    else {
      if (paVar11->ai_family != 10) goto LAB_000311c0;
      psVar9 = paVar11->ai_addr;
      local_1428 = *(undefined4 *)(psVar9->sa_data + 6);
      local_1424 = *(undefined4 *)(psVar9->sa_data + 10);
      uStack_1420._0_2_ = psVar9[1].sa_family;
      uStack_1420._2_1_ = psVar9[1].sa_data[0];
      uStack_1420._3_1_ = psVar9[1].sa_data[1];
      uStack_141c = *(undefined4 *)(psVar9[1].sa_data + 2);
      local_1418 = 0;
      iVar2 = setsockopt(iVar1,0x29,0x14,&local_1428,0x14);
    }
    if (-1 < iVar2) {
LAB_000311c0:
      local_1904 = (char *)&local_1428;
      freeaddrinfo(paStack_18f4);
      pcVar10 = DAT_00075ec4;
      sVar4 = strlen(DAT_00075ec4);
      __n = sVar4 + 9;
      pcVar5 = malloc(sVar4 + 10);
      if (pcVar5 == (char *)0x0) {
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
        acStack_1020[2] = 'm';
        acStack_1020[3] = 'a';
        acStack_101c[0] = 'l';
        acStack_101c[1] = 'l';
        acStack_101c[2] = 'o';
        acStack_101c[3] = 'c';
        local_1018[0] = ' ';
        local_1018[1] = 'm';
        local_1018[2] = 'c';
        local_1018[3] = 'a';
        acStack_1014[0] = 's';
        acStack_1014[1] = 't';
        acStack_1014[2] = ' ';
        acStack_1014[3] = 'e';
        acStack_1010[0] = 'x';
        acStack_1010[1] = 'p';
        acStack_1010[2] = 'e';
        acStack_1010[3] = 'c';
        local_100c[0]._0_1_ = 't';
        local_100c[0]._1_1_ = '_';
        local_100c[1]._0_1_ = 'c';
        local_100c[1]._1_1_ = 'o';
        uStack_1008 = 0x6564;
        local_1006 = 0;
        FUN_00038730(3,local_1028,1);
        FUN_00016ca8(1);
        pcVar10 = DAT_00075ec4;
      }
      param4_01 = acStack_1829 + 1;
      iVar2 = 0;
      snprintf(pcVar5,sVar4 + 10,"%s%s-",local_18ec,pcVar10);
      local_1908 = -1;
LAB_00031238:
      do {
        while( true ) {
          while( true ) {
            while( true ) {
              do {
                while( true ) {
                  iVar2 = iVar2 + 1;
                  FUN_0002a92c(1000);
                  sStack_18fc = 0x80;
                  param3 = recvfrom(iVar1,param4_01,0x3ff,0,asStack_18a8,&sStack_18fc);
                  if (-1 < (int)param3) break;
                  if ((DAT_00078e09 != '\0') &&
                     (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
                    piVar8 = __errno_location();
                    pcVar10 = strerror(*piVar8);
                    snprintf(local_1028,0x1000,"API mcast failed count=%d (%s) (%d)",iVar2,pcVar10,
                             iVar1);
                    FUN_00038730(7,local_1028,0);
                  }
                }
                iVar6 = FUN_000309e8(asStack_18a8,&pcStack_18f8,&uStack_18fd);
                if ((DAT_00078e09 != '\0') &&
                   (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
                  pcVar10 = "Ignored";
                  if (iVar6 != 0) {
                    pcVar10 = "Accepted";
                  }
                  snprintf(local_1028,0x1000,"API mcast from %s - %s",pcStack_18f8,pcVar10);
                  FUN_00038730(7,local_1028,0);
                }
              } while (iVar6 == 0);
              param4_01[param3] = '\0';
              if ((param3 != 0) && (acStack_1829[param3] == '\n')) {
                param3 = param3 - 1;
                param4_01[param3] = '\0';
              }
              getnameinfo(asStack_18a8,sStack_18fc,(char *)0x0,0,acStack_18d4,10,1);
              if ((DAT_00078e09 != '\0') &&
                 (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
                snprintf(local_1028,0x1000,"API mcast request rep=%d (%s) from [%s]:%s",param3,
                         param4_01,pcStack_18f8,acStack_18d4);
                FUN_00038730(7,local_1028,0);
              }
              if ((__n < param3) && (iVar6 = memcmp(param4_01,pcVar5,__n), iVar6 == 0)) break;
              if ((DAT_00078e09 != '\0') &&
                 (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
                local_1028[0] = 'A';
                local_1028[1] = 'P';
                local_1028[2] = 'I';
                local_1028[3] = ' ';
                acStack_1024[0] = 'm';
                acStack_1024[1] = 'c';
                acStack_1024[2] = 'a';
                acStack_1024[3] = 's';
                acStack_1020[0] = 't';
                acStack_1020[1] = ' ';
                acStack_1020[2] = 'r';
                acStack_1020[3] = 'e';
                acStack_101c[0] = 'q';
                acStack_101c[1] = 'u';
                acStack_101c[2] = 'e';
                acStack_101c[3] = 's';
                local_1018[0] = 't';
                local_1018[1] = ' ';
                local_1018[2] = 'w';
                local_1018[3] = 'a';
                acStack_1014[0] = 's';
                acStack_1014[1] = ' ';
                acStack_1014[2] = 'n';
                acStack_1014[3] = 'o';
                acStack_1010[0] = ' ';
                acStack_1010[1] = 'g';
                acStack_1010[2] = 'o';
                acStack_1010[3] = 'o';
                local_100c[0] = 100;
                FUN_00038730(7,local_1028,0);
              }
            }
            pcVar10 = param4_01 + __n;
            param4 = strtol(pcVar10,(char **)0x0,10);
            if (param4 - 1U < 0xffff) break;
            if ((DAT_00078e09 != '\0') &&
               (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
              snprintf(local_1028,0x1000,"API mcast request ignored - invalid port (%s)",pcVar10);
              FUN_00038730(7,local_1028,0);
            }
          }
          if ((DAT_00078e09 != '\0') &&
             (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
            snprintf(local_1028,0x1000,"API mcast request OK port %s=%d",pcVar10,param4);
            FUN_00038730(7,local_1028,0);
          }
          iVar6 = getaddrinfo(pcStack_18f8,pcVar10,&local_18c8,&paStack_18f4);
          paVar11 = paStack_18f4;
          if (iVar6 != 0) goto code_r0x00031768;
          if ((paStack_18f4 != (addrinfo *)0x0) &&
             (local_1908 = socket(paStack_18f4->ai_family,2,0), iVar1 < 1)) break;
          if (local_1908 == -1) goto LAB_00031954;
LAB_0003181c:
          snprintf(local_1904,0x400,"cgm-FTW-%d-%s",DAT_00075ed0,DAT_00075ec0);
          sVar4 = strlen(local_1904);
          param4_00 = sendto(local_1908,local_1904,sVar4 + 1,0,paVar11->ai_addr,paVar11->ai_addrlen)
          ;
          freeaddrinfo(paStack_18f4);
          if (param4_00 < 0) {
            if ((DAT_00078e09 != '\0') &&
               (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
              piVar8 = __errno_location();
              pcVar10 = strerror(*piVar8);
              snprintf(local_1028,0x1000,"API mcast send reply failed (%s) (%d)",pcVar10,local_1908)
              ;
              FUN_00038730(7,local_1028,0);
            }
          }
          else if ((DAT_00078e09 != '\0') &&
                  (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
            snprintf(local_1028,0x1000,"API mcast send reply (%s) succeeded (%d) (%d)",local_1904,
                     param4_00,local_1908);
            FUN_00038730(7,local_1028,0);
          }
          close(local_1908);
        }
        while (paVar11 = paVar11->ai_next, paVar11 != (addrinfo *)0x0) {
          local_1908 = socket(paStack_18f4->ai_family,2,0);
        }
        paVar11 = (addrinfo *)0x0;
        if (local_1908 != -1) goto LAB_0003181c;
LAB_00031954:
        freeaddrinfo(paStack_18f4);
      } while (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3));
      pcVar10 = "API mcast could not open socket to client %s";
      goto LAB_000317c0;
    }
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) goto LAB_000315a8;
    piVar8 = __errno_location();
    pcVar10 = strerror(*piVar8);
    pcVar5 = "API mcast join failed (%s)%s";
  }
  snprintf(local_1028,0x1000,pcVar5,pcVar10," - API multicast listener will not be available");
  FUN_00038730(3,local_1028,0);
LAB_000315a8:
  close(iVar1);
  return;
code_r0x00031768:
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
    pcVar10 = "Invalid client address %s";
LAB_000317c0:
    snprintf(local_1028,0x1000,pcVar10,pcStack_18f8);
    FUN_00038730(3,local_1028,0);
  }
  goto LAB_00031238;
}

