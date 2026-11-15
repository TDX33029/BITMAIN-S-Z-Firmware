//==================== FUN_000559d0 @ 000559d0 ====================

int FUN_000559d0(int param_1)

{
  ssize_t sVar1;
  char *pcVar2;
  char *pcVar3;
  size_t param2;
  int iVar4;
  int iVar5;
  size_t sVar6;
  int *piVar7;
  int iVar8;
  uint uVar9;
  char acStack_7050 [4];
  socklen_t sStack_704c;
  char acStack_7048 [8];
  undefined4 uStack_7040;
  undefined4 local_703c;
  sockaddr sStack_7038;
  char local_7028 [4];
  char acStack_7024 [4];
  undefined2 local_7020;
  char acStack_6060 [56];
  char local_6028 [4];
  char acStack_6024 [4];
  char acStack_6020 [4];
  char acStack_601c [4];
  undefined2 local_6018 [2];
  undefined3 uStack_6014;
  char cStack_6011;
  uint local_6010;
  char acStack_600c [4];
  char local_6008 [4];
  char acStack_6004 [4];
  uint local_6000;
  char acStack_5ffc [14292];
  undefined4 local_2828;
  undefined4 uStack_2824;
  undefined4 uStack_2820;
  undefined4 uStack_281c;
  undefined4 local_2818;
  uint local_2814;
  undefined4 uStack_2810;
  undefined2 local_280c [2];
  uint local_2808;
  char acStack_2804 [4];
  char acStack_2800 [4];
  char local_27fc [10200];
  
  uStack_7040 = 3;
  local_703c = 0;
  memset(local_7028,0,0x1000);
  sStack_704c = 0;
  acStack_7050[0] = '\r';
  pcVar3 = acStack_5ffc + 0xfd4;
  builtin_strncpy(acStack_7048,"\r\n\r\n",5);
  acStack_7050[2] = 0;
  iVar8 = 0;
  acStack_7050[1] = 10;
  memset(pcVar3,0,0x2800);
  do {
    if (DAT_0007acb8 != '\0') goto LAB_00055b50;
    sVar1 = recvfrom(param_1,pcVar3,0x27ff - iVar8,0,&sStack_7038,&sStack_704c);
    iVar8 = iVar8 + sVar1;
    if (sVar1 < 1) {
      close(param_1);
      if (DAT_00078e09 == '\0') {
        return 0;
      }
      if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7)) {
        return 0;
      }
      local_2828._0_1_ = ':';
      local_2828._1_1_ = 's';
      local_2828._2_1_ = 't';
      local_2828._3_1_ = 'a';
      uStack_2824._0_1_ = 't';
      uStack_2824._1_1_ = 'u';
      uStack_2824._2_1_ = 's';
      uStack_2824._3_1_ = 'S';
      uStack_2820._0_1_ = 'e';
      uStack_2820._1_1_ = 'r';
      uStack_2820._2_1_ = 'v';
      uStack_2820._3_1_ = 'i';
      uStack_281c._0_1_ = 'c';
      uStack_281c._1_1_ = 'e';
      uStack_281c._2_1_ = 'T';
      uStack_281c._3_1_ = 'h';
      local_2818._0_1_ = 'r';
      local_2818._1_1_ = 'e';
      local_2818._2_1_ = 'a';
      local_2818._3_1_ = 'd';
      local_2814._0_1_ = ' ';
      local_2814._1_1_ = 'r';
      local_2814._2_1_ = 'e';
      local_2814._3_1_ = 'c';
      uStack_2810._0_1_ = 'v';
      uStack_2810._1_1_ = 'f';
      uStack_2810._2_1_ = 'r';
      uStack_2810._3_1_ = 'o';
      local_280c[0]._0_1_ = 'm';
      local_280c[0]._1_1_ = '<';
      local_280c[1]._0_1_ = '=';
      local_280c[1]._1_1_ = '0';
      local_2808 = local_2808 & 0xffffff00;
      FUN_00038730(7,&local_2828,0);
      return 0;
    }
    pcVar3 = acStack_5ffc + iVar8 + 0xfd4;
    if (0x27fe < iVar8) {
      close(param_1);
      if (DAT_00078e09 == '\0') {
        return 0;
      }
      if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7)) {
        return 0;
      }
      local_2828._0_1_ = 'B';
      local_2828._1_1_ = 'U';
      local_2828._2_1_ = 'F';
      local_2828._3_1_ = 'S';
      uStack_2824._0_1_ = 'I';
      uStack_2824._1_1_ = 'Z';
      uStack_2824._2_1_ = 'E';
      uStack_2824._3_1_ = ' ';
      uStack_2820._0_1_ = 'i';
      uStack_2820._1_1_ = 's';
      uStack_2820._2_1_ = ' ';
      uStack_2820._3_1_ = 't';
      uStack_281c._0_1_ = 'o';
      uStack_281c._1_1_ = 'o';
      uStack_281c._2_1_ = ' ';
      uStack_281c._3_1_ = 's';
      local_2818._0_1_ = 'm';
      local_2818._1_1_ = 'a';
      local_2818._2_1_ = 'l';
      local_2818._3_1_ = 'l';
      local_2814 = CONCAT22(local_2814._2_2_,0x21);
      FUN_00038730(7,&local_2828,0);
      return 0;
    }
    pcVar2 = strstr(acStack_5ffc + 0xfd4,acStack_7048);
  } while (pcVar2 == (char *)0x0);
  if (DAT_00078e09 != '\0') {
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7)) {
LAB_00055e78:
      if (6 < DAT_000766c4) {
LAB_00055b7c:
        snprintf((char *)&local_2828,0x1000,"get http=%s",acStack_5ffc + 0xfd4);
        FUN_00038730(7,&local_2828,0);
      }
    }
    else {
      local_2828._0_1_ = 'f';
      local_2828._1_1_ = 'i';
      local_2828._2_1_ = 'n';
      local_2828._3_1_ = 'd';
      uStack_2824._0_1_ = ' ';
      uStack_2824._1_1_ = 'h';
      uStack_2824._2_1_ = 't';
      uStack_2824._3_1_ = 't';
      uStack_2820._0_1_ = 'p';
      uStack_2820._1_1_ = ' ';
      uStack_2820._2_1_ = 'r';
      uStack_2820._3_1_ = 'e';
      uStack_281c._0_1_ = 'q';
      uStack_281c._1_1_ = 'u';
      uStack_281c._2_1_ = 'e';
      uStack_281c._3_1_ = 's';
      local_2818._0_1_ = 't';
      local_2818._1_1_ = ' ';
      local_2818._2_1_ = 'e';
      local_2818._3_1_ = 'n';
      local_2814._0_1_ = 'd';
      local_2814._1_1_ = ' ';
      local_2814._2_1_ = 'f';
      local_2814._3_1_ = 'l';
      uStack_2810._0_1_ = 'a';
      uStack_2810._1_1_ = 'g';
      uStack_2810._2_1_ = '!';
      uStack_2810._3_1_ = '\0';
      FUN_00038730(7,&local_2828,0);
LAB_00055b50:
      if (DAT_00078e09 != '\0') {
        if ((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) goto LAB_00055e78;
        goto LAB_00055b7c;
      }
    }
  }
  if ((((acStack_5ffc[0xfd4] == 'G') && (acStack_5ffc[0xfd5] == 'E')) &&
      (acStack_5ffc[0xfd6] == 'T')) && (acStack_5ffc[0xfd7] == ' ')) {
    pcVar3 = strstr(acStack_5ffc + 0xfd8,"/rate");
    if (pcVar3 == (char *)0x0) {
      pcVar3 = strstr(acStack_5ffc + 0xfd8,"/productName");
      if (pcVar3 == (char *)0x0) {
        close(param_1);
        if (DAT_00078e09 == '\0') {
          return 0;
        }
        if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7)) {
          return 0;
        }
        local_2828._0_1_ = 's';
        local_2828._1_1_ = 't';
        local_2828._2_1_ = 'a';
        local_2828._3_1_ = 't';
        uStack_2824._0_1_ = 'u';
        uStack_2824._1_1_ = 's';
        uStack_2824._2_1_ = 'S';
        uStack_2824._3_1_ = 'e';
        uStack_2820._0_1_ = 'r';
        uStack_2820._1_1_ = 'v';
        uStack_2820._2_1_ = 'i';
        uStack_2820._3_1_ = 'c';
        uStack_281c._0_1_ = 'e';
        uStack_281c._1_1_ = 'T';
        uStack_281c._2_1_ = 'h';
        uStack_281c._3_1_ = 'r';
        local_2818._0_1_ = 'e';
        local_2818._1_1_ = 'a';
        local_2818._2_1_ = 'd';
        local_2818._3_1_ = ' ';
        local_2814._0_1_ = 'e';
        local_2814._1_1_ = 'x';
        local_2814._2_1_ = 'i';
        local_2814._3_1_ = 't';
        uStack_2810._0_1_ = ' ';
        uStack_2810._1_1_ = 'f';
        uStack_2810._2_1_ = 'o';
        uStack_2810._3_1_ = 'r';
        local_280c[0]._0_1_ = ' ';
        local_280c[0]._1_1_ = 'E';
        local_280c[1]._0_1_ = 'r';
        local_280c[1]._1_1_ = 'r';
        local_2808._0_1_ = 'o';
        local_2808._1_1_ = 'r';
        local_2808._2_1_ = ' ';
        local_2808._3_1_ = 'c';
        acStack_2804[0] = 'm';
        acStack_2804[1] = 'd';
        acStack_2804[2] = '!';
        acStack_2804[3] = '\0';
        FUN_00038730(7,&local_2828,0);
        return 0;
      }
      if ((DAT_00078e09 != '\0') &&
         (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
        local_2828._0_1_ = 'c';
        local_2828._1_1_ = 'm';
        local_2828._2_1_ = 'd';
        local_2828._3_1_ = ' ';
        uStack_2824._0_1_ = ':';
        uStack_2824._1_1_ = ' ';
        uStack_2824._2_1_ = 'g';
        uStack_2824._3_1_ = 'e';
        uStack_2820._0_1_ = 't';
        uStack_2820._1_1_ = ' ';
        uStack_2820._2_1_ = 'm';
        uStack_2820._3_1_ = 'i';
        uStack_281c._0_1_ = 'n';
        uStack_281c._1_1_ = 'e';
        uStack_281c._2_1_ = 'r';
        uStack_281c._3_1_ = ' ';
        local_2818._0_1_ = 't';
        local_2818._1_1_ = 'y';
        local_2818._2_1_ = 'p';
        local_2818._3_1_ = 'e';
        local_2814 = local_2814 & 0xffffff00;
        FUN_00038730(7,&local_2828,0);
      }
      snprintf(local_7028,0x1000,&DAT_00078850);
    }
    else {
      if ((DAT_00078e09 != '\0') &&
         (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
        local_2828._0_1_ = 'c';
        local_2828._1_1_ = 'm';
        local_2828._2_1_ = 'd';
        local_2828._3_1_ = ' ';
        uStack_2824._0_1_ = ':';
        uStack_2824._1_1_ = ' ';
        uStack_2824._2_1_ = 'g';
        uStack_2824._3_1_ = 'e';
        uStack_2820._0_1_ = 't';
        uStack_2820._1_1_ = ' ';
        uStack_2820._2_1_ = 'r';
        uStack_2820._3_1_ = 'a';
        uStack_281c._0_1_ = 't';
        uStack_281c._1_1_ = 'e';
        uStack_281c._2_1_ = '\0';
        FUN_00038730(7,&local_2828,0);
      }
      if (DAT_00076b98 == 1) {
        snprintf(local_7028,0x1000,"%d",420000);
      }
      else if (DAT_00076b98 == 2) {
        snprintf(local_7028,0x1000,"%s:%s","searchfailed","search_failed_info");
      }
      else {
        local_7028[0] = 's';
        local_7028[1] = 'e';
        local_7028[2] = 'a';
        local_7028[3] = 'r';
        acStack_7024[0] = 'c';
        acStack_7024[1] = 'h';
        acStack_7024[2] = 'i';
        acStack_7024[3] = 'n';
        local_7020 = 0x67;
      }
    }
    iVar8 = setsockopt(param_1,1,0x15,&uStack_7040,8);
    if (iVar8 == 0) {
      iVar8 = setsockopt(param_1,1,0x14,&uStack_7040,8);
      if (iVar8 == 0) {
        param2 = strlen(local_7028);
        iVar8 = sprintf((char *)&local_2828,"HTTP/1.0  200  OK%s",acStack_7050);
        iVar4 = sprintf(acStack_5ffc + iVar8 + 0x37d4,"Server: SearchFreqServer%s",acStack_7050);
        iVar5 = sprintf(acStack_5ffc + iVar8 + iVar4 + 0x37d4,"Cache-Control: no-cache%s",
                        acStack_7050);
        iVar5 = iVar5 + iVar8 + iVar4;
        iVar8 = sprintf(acStack_5ffc + iVar5 + 0x37d4,"Pragma: no-cache%s",acStack_7050);
        iVar8 = iVar8 + iVar5;
        iVar4 = sprintf(acStack_5ffc + iVar8 + 0x37d4,"Content-Type: text/plain%s",acStack_7050);
        iVar4 = iVar4 + iVar8;
        iVar8 = sprintf(acStack_5ffc + iVar4 + 0x37d4,"Content-Length: %d%s",param2,acStack_7050);
        sprintf(acStack_5ffc + iVar8 + iVar4 + 0x37d4,"Connection: Keep-Alive%s",acStack_7048);
        if ((DAT_00078e09 != '\0') &&
           (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
          local_6028[0] = 's';
          local_6028[1] = 'e';
          local_6028[2] = 'n';
          local_6028[3] = 'd';
          acStack_6024[0] = ' ';
          acStack_6024[1] = 'h';
          acStack_6024[2] = 't';
          acStack_6024[3] = 't';
          acStack_6020[0] = 'p';
          acStack_6020[1] = ' ';
          acStack_6020[2] = 'r';
          acStack_6020[3] = 'e';
          acStack_601c[0] = 's';
          acStack_601c[1] = 'p';
          acStack_601c[2] = 'o';
          acStack_601c[3] = 'n';
          local_6018[0]._0_1_ = 's';
          local_6018[0]._1_1_ = 'e';
          local_6018[1]._0_1_ = '.';
          local_6018[1]._1_1_ = '.';
          uStack_6014._0_1_ = '.';
          uStack_6014._1_1_ = '\n';
          uStack_6014._2_1_ = '\0';
          FUN_00038730(7,local_6028,0);
        }
        if (DAT_0007acb8 == '\0') {
          sVar6 = strlen((char *)&local_2828);
          uVar9 = 0;
          do {
            sVar1 = send(param_1,acStack_5ffc + uVar9 + 0x37d4,sVar6 - uVar9,0);
            if (sVar1 == -1) {
              piVar7 = __errno_location();
              if (*piVar7 != 0xb) {
LAB_00056154:
                close(param_1);
                if (DAT_00078e09 == '\0') {
                  return 0;
                }
                if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7)) {
                  return 0;
                }
                local_6028[0] = 's';
                local_6028[1] = 't';
                local_6028[2] = 'a';
                local_6028[3] = 't';
                acStack_6024[0] = 'u';
                acStack_6024[1] = 's';
                acStack_6024[2] = 'S';
                acStack_6024[3] = 'e';
                acStack_6020[0] = 'r';
                acStack_6020[1] = 'v';
                acStack_6020[2] = 'i';
                acStack_6020[3] = 'c';
                acStack_601c[0] = 'e';
                acStack_601c[1] = 'T';
                acStack_601c[2] = 'h';
                acStack_601c[3] = 'r';
                local_6018[0]._0_1_ = 'e';
                local_6018[0]._1_1_ = 'a';
                local_6018[1]._0_1_ = 'd';
                local_6018[1]._1_1_ = ' ';
                uStack_6014._0_1_ = 's';
                uStack_6014._1_1_ = 'e';
                uStack_6014._2_1_ = 'n';
                cStack_6011 = 'd';
                local_6010._0_1_ = ' ';
                local_6010._1_1_ = 'h';
                local_6010._2_1_ = 't';
                local_6010._3_1_ = 't';
                acStack_600c[0] = 'p';
                acStack_600c[1] = ' ';
                acStack_600c[2] = 'r';
                acStack_600c[3] = 'e';
                local_6008[0] = 's';
                local_6008[1] = 'p';
                local_6008[2] = 'o';
                local_6008[3] = 'n';
                acStack_6004[0] = 's';
                acStack_6004[1] = 'e';
                acStack_6004[2] = ' ';
                acStack_6004[3] = 'e';
                local_6000._0_1_ = 'r';
                local_6000._1_1_ = 'r';
                local_6000._2_1_ = 'o';
                local_6000._3_1_ = 'r';
                pcVar2 = acStack_5ffc;
                pcVar3 = acStack_6060;
                goto LAB_00055c34;
              }
              if ((DAT_00078e09 != '\0') &&
                 (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
                local_6028[0] = 's';
                local_6028[1] = 't';
                local_6028[2] = 'a';
                local_6028[3] = 't';
                acStack_6024[0] = 'u';
                acStack_6024[1] = 's';
                acStack_6024[2] = 'S';
                acStack_6024[3] = 'e';
                acStack_6020[0] = 'r';
                acStack_6020[1] = 'v';
                acStack_6020[2] = 'i';
                acStack_6020[3] = 'c';
                acStack_601c[0] = 'e';
                acStack_601c[1] = 'T';
                acStack_601c[2] = 'h';
                acStack_601c[3] = 'r';
                local_6018[0]._0_1_ = 'e';
                local_6018[0]._1_1_ = 'a';
                local_6018[1]._0_1_ = 'd';
                local_6018[1]._1_1_ = ' ';
                uStack_6014._0_1_ = 's';
                uStack_6014._1_1_ = 'e';
                uStack_6014._2_1_ = 'n';
                cStack_6011 = 'd';
                local_6010._0_1_ = ' ';
                local_6010._1_1_ = 'h';
                local_6010._2_1_ = 't';
                local_6010._3_1_ = 't';
                acStack_600c[0] = 'p';
                acStack_600c[1] = ' ';
                acStack_600c[2] = 't';
                acStack_600c[3] = 'i';
                local_6008[0] = 'm';
                local_6008[1] = 'e';
                local_6008[2] = 'o';
                local_6008[3] = 'u';
                acStack_6004[0] = 't';
                acStack_6004[1] = ',';
                acStack_6004[2] = ' ';
                acStack_6004[3] = 't';
                local_6000._0_1_ = 'r';
                local_6000._1_1_ = 'y';
                local_6000._2_1_ = ' ';
                local_6000._3_1_ = 'a';
                acStack_5ffc[0] = 'g';
                acStack_5ffc[1] = 'a';
                acStack_5ffc[2] = 'i';
                acStack_5ffc[3] = 'n';
                acStack_5ffc[4] = '.';
                acStack_5ffc[5] = '.';
                acStack_5ffc[6] = '.';
                acStack_5ffc[7] = '\0';
                FUN_00038730(7,local_6028,0);
              }
              usleep(100000);
            }
            else {
              if (sVar1 < 1) goto LAB_00056154;
              uVar9 = uVar9 + sVar1;
            }
            sVar6 = strlen((char *)&local_2828);
          } while ((uVar9 < sVar6) && (DAT_0007acb8 == '\0'));
        }
        if ((DAT_00078e09 != '\0') &&
           (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
          local_6028[0] = 's';
          local_6028[1] = 'e';
          local_6028[2] = 'n';
          local_6028[3] = 'd';
          acStack_6024[0] = ' ';
          acStack_6024[1] = 'h';
          acStack_6024[2] = 't';
          acStack_6024[3] = 't';
          acStack_6020[0] = 'p';
          acStack_6020[1] = ' ';
          acStack_6020[2] = 'd';
          acStack_6020[3] = 'a';
          acStack_601c[0] = 't';
          acStack_601c[1] = 'a';
          acStack_601c[2] = '.';
          acStack_601c[3] = '.';
          local_6018[0] = 0x2e;
          FUN_00038730(7,local_6028,0);
        }
        iVar8 = 0;
        while( true ) {
          sVar1 = send(param_1,local_7028 + iVar8,param2 - iVar8,0);
          if ((DAT_00078e09 != '\0') &&
             (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
            snprintf(local_6028,0x1000,"send http data ret=%d",sVar1);
            FUN_00038730(7,local_6028,0);
          }
          if (sVar1 < 1) break;
          iVar8 = iVar8 + sVar1;
          if (((int)param2 <= iVar8) || (DAT_0007acb8 != '\0')) goto LAB_000562e0;
        }
        if (DAT_00078e09 == '\0') goto LAB_00056350;
        if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7))
        goto LAB_00056300;
        local_6028[0] = 's';
        local_6028[1] = 't';
        local_6028[2] = 'a';
        local_6028[3] = 't';
        acStack_6024[0] = 'u';
        acStack_6024[1] = 's';
        acStack_6024[2] = 'S';
        acStack_6024[3] = 'e';
        acStack_6020[0] = 'r';
        acStack_6020[1] = 'v';
        acStack_6020[2] = 'i';
        acStack_6020[3] = 'c';
        acStack_601c[0] = 'e';
        acStack_601c[1] = 'T';
        acStack_601c[2] = 'h';
        acStack_601c[3] = 'r';
        local_6018[0]._0_1_ = 'e';
        local_6018[0]._1_1_ = 'a';
        local_6018[1]._0_1_ = 'd';
        local_6018[1]._1_1_ = ' ';
        uStack_6014._0_1_ = 's';
        uStack_6014._1_1_ = 'e';
        uStack_6014._2_1_ = 'n';
        cStack_6011 = 'd';
        local_6010._0_1_ = ' ';
        local_6010._1_1_ = 'h';
        local_6010._2_1_ = 't';
        local_6010._3_1_ = 't';
        acStack_600c[0] = 'p';
        acStack_600c[1] = ' ';
        acStack_600c[2] = 'd';
        acStack_600c[3] = 'a';
        local_6008[0] = 't';
        local_6008[1] = 'a';
        local_6008[2] = ' ';
        local_6008[3] = 'e';
        acStack_6004[0] = 'r';
        acStack_6004[1] = 'r';
        acStack_6004[2] = 'o';
        acStack_6004[3] = 'r';
        local_6000 = local_6000 & 0xffffff00;
        FUN_00038730(7,local_6028,0);
LAB_000562e0:
        if (DAT_00078e09 != '\0') {
          if (DAT_0007ad48 == '\0') {
LAB_00056300:
            if ((DAT_00078e08 == '\0') && (DAT_000766c4 < 7)) goto LAB_00056350;
          }
          local_6028[0] = 'o';
          local_6028[1] = 'n';
          local_6028[2] = 'e';
          local_6028[3] = ' ';
          acStack_6024[0] = 'c';
          acStack_6024[1] = 'l';
          acStack_6024[2] = 'i';
          acStack_6024[3] = 'e';
          acStack_6020[0] = 'n';
          acStack_6020[1] = 't';
          acStack_6020[2] = ' ';
          acStack_6020[3] = 'd';
          acStack_601c[0] = 'i';
          acStack_601c[1] = 's';
          acStack_601c[2] = 'c';
          acStack_601c[3] = 'o';
          local_6018[0]._0_1_ = 'n';
          local_6018[0]._1_1_ = 'n';
          local_6018[1]._0_1_ = 'e';
          local_6018[1]._1_1_ = 'c';
          uStack_6014._0_1_ = 't';
          uStack_6014._1_1_ = 'e';
          uStack_6014._2_1_ = 'd';
          cStack_6011 = '!';
          local_6010 = local_6010 & 0xffffff00;
          FUN_00038730(7,local_6028,0);
        }
LAB_00056350:
        close(param_1);
        return iVar8;
      }
      close(param_1);
      if ((DAT_00078e09 != '\0') &&
         (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
        pcVar3 = "setsockopt SO_RCVTIMEO failed";
        goto LAB_00055e30;
      }
    }
    else {
      close(param_1);
      if ((DAT_00078e09 != '\0') &&
         (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
        pcVar3 = "setsockopt SO_SNDTIMEO failed";
LAB_00055e30:
        local_2828 = *(undefined4 *)pcVar3;
        uStack_2824 = *(undefined4 *)(pcVar3 + 4);
        uStack_2820 = *(undefined4 *)(pcVar3 + 8);
        uStack_281c = *(undefined4 *)(pcVar3 + 0xc);
        local_2818 = *(undefined4 *)(pcVar3 + 0x10);
        local_2814 = *(uint *)(pcVar3 + 0x14);
        uStack_2810 = *(undefined4 *)(pcVar3 + 0x18);
        local_280c[0] = (short)*(undefined4 *)(pcVar3 + 0x1c);
        FUN_00038730(7,&local_2828,0);
        return 0;
      }
    }
  }
  else {
    close(param_1);
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      local_2828._0_1_ = 's';
      local_2828._1_1_ = 't';
      local_2828._2_1_ = 'a';
      local_2828._3_1_ = 't';
      uStack_2824._0_1_ = 'u';
      uStack_2824._1_1_ = 's';
      uStack_2824._2_1_ = 'S';
      uStack_2824._3_1_ = 'e';
      uStack_2820._0_1_ = 'r';
      uStack_2820._1_1_ = 'v';
      uStack_2820._2_1_ = 'i';
      uStack_2820._3_1_ = 'c';
      uStack_281c._0_1_ = 'e';
      uStack_281c._1_1_ = 'T';
      uStack_281c._2_1_ = 'h';
      uStack_281c._3_1_ = 'r';
      local_2818._0_1_ = 'e';
      local_2818._1_1_ = 'a';
      local_2818._2_1_ = 'd';
      local_2818._3_1_ = ' ';
      local_2814._0_1_ = 'n';
      local_2814._1_1_ = 'o';
      local_2814._2_1_ = 't';
      local_2814._3_1_ = ' ';
      uStack_2810._0_1_ = 's';
      uStack_2810._1_1_ = 'u';
      uStack_2810._2_1_ = 'p';
      uStack_2810._3_1_ = 'p';
      local_280c[0]._0_1_ = 'o';
      local_280c[0]._1_1_ = 'r';
      local_280c[1]._0_1_ = 't';
      local_280c[1]._1_1_ = ' ';
      local_2808._0_1_ = 'h';
      local_2808._1_1_ = 't';
      local_2808._2_1_ = 't';
      local_2808._3_1_ = 'p';
      acStack_2804[0] = ' ';
      acStack_2804[1] = 'c';
      acStack_2804[2] = 'o';
      acStack_2804[3] = 'm';
      acStack_2800[0] = 'm';
      acStack_2800[1] = 'a';
      acStack_2800[2] = 'n';
      acStack_2800[3] = 'd';
      pcVar2 = local_27fc;
      pcVar3 = acStack_5ffc + 0x379c;
LAB_00055c34:
      *pcVar2 = '\0';
      FUN_00038730(7,pcVar3 + 0x38,0);
      return 0;
    }
  }
  return 0;
}

