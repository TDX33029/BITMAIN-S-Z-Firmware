//==================== FUN_0005664c @ 0005664c ====================

undefined4 FUN_0005664c(void)

{
  int iVar1;
  char cVar2;
  socklen_t sStack_1050;
  socklen_t sStack_104c;
  sockaddr sStack_1048;
  sockaddr sStack_1038;
  char local_1028 [4];
  char acStack_1024 [4];
  char acStack_1020 [4];
  char acStack_101c [4];
  char local_1018 [4];
  char acStack_1014 [4];
  uint local_1010;
  char acStack_100c [4];
  char local_1008 [4];
  char acStack_1004 [4];
  undefined3 uStack_1000;
  char cStack_ffd;
  undefined3 local_ffc;
  char cStack_ff9;
  undefined2 uStack_ff8;
  undefined1 local_ff6;
  
  sleep(0x1e);
  do {
    DAT_00076b9c = socket(2,1,6);
    if (DAT_00076b9c < 0) {
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
        local_1028[0] = 's';
        local_1028[1] = 'o';
        local_1028[2] = 'c';
        local_1028[3] = 'k';
        acStack_1024[0] = 'e';
        acStack_1024[1] = 't';
        acStack_1024[2] = ' ';
        acStack_1024[3] = 'c';
        acStack_1020[0] = 'r';
        acStack_1020[1] = 'e';
        acStack_1020[2] = 'a';
        acStack_1020[3] = 't';
        acStack_101c[0] = 'i';
        acStack_101c[1] = 'n';
        acStack_101c[2] = 'g';
        acStack_101c[3] = ' ';
        local_1018[0] = 'f';
        local_1018[1] = 'a';
        local_1018[2] = 'i';
        local_1018[3] = 'l';
        acStack_1014[0] = 'e';
        acStack_1014[1] = 'd';
        acStack_1014[2] = ',';
        acStack_1014[3] = ' ';
        local_1010._0_1_ = 't';
        local_1010._1_1_ = 'r';
        local_1010._2_1_ = 'y';
        local_1010._3_1_ = ' ';
        acStack_100c[0] = 'a';
        acStack_100c[1] = 'g';
        acStack_100c[2] = 'a';
        acStack_100c[3] = 'i';
        local_1008[0] = 'n';
        local_1008[1] = ' ';
        local_1008[2] = 'a';
        local_1008[3] = 'f';
        acStack_1004[0] = 't';
        acStack_1004[1] = 'e';
        acStack_1004[2] = 'r';
        acStack_1004[3] = ' ';
        uStack_1000._0_1_ = '1';
        uStack_1000._1_1_ = '0';
        uStack_1000._2_1_ = 's';
        cStack_ffd = '.';
        local_ffc._0_1_ = '.';
        local_ffc._1_1_ = '.';
        local_ffc._2_1_ = '\0';
        FUN_00038730(5,local_1028,0);
      }
      sleep(10);
    }
    else {
      sStack_104c = 1;
      iVar1 = setsockopt(DAT_00076b9c,1,2,&sStack_104c,4);
      if ((iVar1 < 0) && (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4))))
      {
        local_1028[0] = '*';
        local_1028[1] = '*';
        local_1028[2] = '*';
        local_1028[3] = '*';
        acStack_1024[0] = '*';
        acStack_1024[1] = '*';
        acStack_1024[2] = '*';
        acStack_1024[3] = '*';
        acStack_1020[0] = '*';
        acStack_1020[1] = '*';
        acStack_1020[2] = '*';
        acStack_1020[3] = '*';
        acStack_101c[0] = '*';
        acStack_101c[1] = '*';
        acStack_101c[2] = '*';
        acStack_101c[3] = '*';
        local_1018[0] = '*';
        local_1018[1] = '*';
        local_1018[2] = '*';
        local_1018[3] = 's';
        acStack_1014[0] = 'e';
        acStack_1014[1] = 't';
        acStack_1014[2] = 's';
        acStack_1014[3] = 'o';
        local_1010._0_1_ = 'c';
        local_1010._1_1_ = 'k';
        local_1010._2_1_ = 'o';
        local_1010._3_1_ = 'p';
        acStack_100c[0] = 't';
        acStack_100c[1] = '(';
        acStack_100c[2] = 'S';
        acStack_100c[3] = 'O';
        local_1008[0] = '_';
        local_1008[1] = 'R';
        local_1008[2] = 'E';
        local_1008[3] = 'U';
        acStack_1004[0] = 'S';
        acStack_1004[1] = 'E';
        acStack_1004[2] = 'A';
        acStack_1004[3] = 'D';
        uStack_1000._0_1_ = 'D';
        uStack_1000._1_1_ = 'R';
        uStack_1000._2_1_ = ')';
        cStack_ffd = ' ';
        local_ffc._0_1_ = 'f';
        local_ffc._1_1_ = 'a';
        local_ffc._2_1_ = 'i';
        cStack_ff9 = 'l';
        uStack_ff8 = 0x6465;
        local_ff6 = 0;
        FUN_00038730(4,local_1028,0);
      }
      sStack_1048.sa_family = 2;
      sStack_1048.sa_data[0] = '\x17';
      sStack_1048.sa_data[1] = -0x54;
      sStack_1048.sa_data[2] = '\0';
      sStack_1048.sa_data[3] = '\0';
      sStack_1048.sa_data[4] = '\0';
      sStack_1048.sa_data[5] = '\0';
      sStack_1048.sa_data[6] = '\0';
      sStack_1048.sa_data[7] = '\0';
      sStack_1048.sa_data[8] = '\0';
      sStack_1048.sa_data[9] = '\0';
      sStack_1048.sa_data[10] = '\0';
      sStack_1048.sa_data[0xb] = '\0';
      sStack_1048.sa_data[0xc] = '\0';
      sStack_1048.sa_data[0xd] = '\0';
      iVar1 = bind(DAT_00076b9c,&sStack_1048,0x10);
      if (iVar1 < 0) {
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
          local_1028[0] = 'h';
          local_1028[1] = 't';
          local_1028[2] = 't';
          local_1028[3] = 'p';
          acStack_1024[0] = ' ';
          acStack_1024[1] = 'p';
          acStack_1024[2] = 'o';
          acStack_1024[3] = 'r';
          acStack_1020[0] = 't';
          acStack_1020[1] = ' ';
          acStack_1020[2] = 'b';
          acStack_1020[3] = 'i';
          acStack_101c[0] = 'n';
          acStack_101c[1] = 'd';
          acStack_101c[2] = ' ';
          acStack_101c[3] = 'f';
          local_1018[0] = 'a';
          local_1018[1] = 'i';
          local_1018[2] = 'l';
          local_1018[3] = 'e';
          acStack_1014[0] = 'd';
          acStack_1014[1] = '!';
          acStack_1014[2] = ' ';
          acStack_1014[3] = 't';
          local_1010._0_1_ = 'r';
          local_1010._1_1_ = 'y';
          local_1010._2_1_ = ' ';
          local_1010._3_1_ = 'a';
          acStack_100c[0] = 'g';
          acStack_100c[1] = 'a';
          acStack_100c[2] = 'i';
          acStack_100c[3] = 'n';
          local_1008[0] = ' ';
          local_1008[1] = 'a';
          local_1008[2] = 'f';
          local_1008[3] = 't';
          acStack_1004[0] = 'e';
          acStack_1004[1] = 'r';
          acStack_1004[2] = ' ';
          acStack_1004[3] = '1';
          uStack_1000._0_1_ = '0';
          uStack_1000._1_1_ = 's';
          uStack_1000._2_1_ = '.';
          cStack_ffd = '.';
          local_ffc._0_1_ = '.';
          local_ffc._1_1_ = '\0';
          FUN_00038730(5,local_1028,0);
        }
      }
      else {
        iVar1 = listen(DAT_00076b9c,100);
        if (-1 < iVar1) break;
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
          local_1028[0] = 'h';
          local_1028[1] = 't';
          local_1028[2] = 't';
          local_1028[3] = 'p';
          acStack_1024[0] = ' ';
          acStack_1024[1] = 'l';
          acStack_1024[2] = 'i';
          acStack_1024[3] = 's';
          acStack_1020[0] = 't';
          acStack_1020[1] = 'e';
          acStack_1020[2] = 'n';
          acStack_1020[3] = ' ';
          acStack_101c[0] = 'f';
          acStack_101c[1] = 'a';
          acStack_101c[2] = 'i';
          acStack_101c[3] = 'l';
          local_1018[0] = 'e';
          local_1018[1] = 'd';
          local_1018[2] = '!';
          local_1018[3] = ' ';
          acStack_1014[0] = 't';
          acStack_1014[1] = 'r';
          acStack_1014[2] = 'y';
          acStack_1014[3] = ' ';
          local_1010._0_1_ = 'a';
          local_1010._1_1_ = 'g';
          local_1010._2_1_ = 'a';
          local_1010._3_1_ = 'i';
          acStack_100c[0] = 'n';
          acStack_100c[1] = ' ';
          acStack_100c[2] = 'a';
          acStack_100c[3] = 'f';
          local_1008[0] = 't';
          local_1008[1] = 'e';
          local_1008[2] = 'r';
          local_1008[3] = ' ';
          acStack_1004[0] = '1';
          acStack_1004[1] = '0';
          acStack_1004[2] = 's';
          acStack_1004[3] = '.';
          uStack_1000._0_1_ = '.';
          uStack_1000._1_1_ = '.';
          uStack_1000._2_1_ = '\0';
          FUN_00038730(5,local_1028,0);
        }
      }
      close(DAT_00076b9c);
      DAT_00076b9c = -1;
      sleep(10);
    }
  } while (DAT_0007acb8 == '\0');
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    local_1028[0] = 's';
    local_1028[1] = 't';
    local_1028[2] = 'a';
    local_1028[3] = 'r';
    acStack_1024[0] = 't';
    acStack_1024[1] = ' ';
    acStack_1024[2] = 'l';
    acStack_1024[3] = 'i';
    acStack_1020[0] = 's';
    acStack_1020[1] = 't';
    acStack_1020[2] = 'e';
    acStack_1020[3] = 'n';
    acStack_101c[0] = ' ';
    acStack_101c[1] = 'o';
    acStack_101c[2] = 'n';
    acStack_101c[3] = ' ';
    local_1018[0] = '6';
    local_1018[1] = '0';
    local_1018[2] = '6';
    local_1018[3] = '0';
    acStack_1014[0] = ' ';
    acStack_1014[1] = '.';
    acStack_1014[2] = '.';
    acStack_1014[3] = '.';
    local_1010 = local_1010 & 0xffffff00;
    FUN_00038730(5,local_1028,0);
  }
  do {
    if (DAT_0007acb8 != '\0') {
LAB_00056860:
      close(DAT_00076b9c);
      DAT_00076b9c = 0xffffffff;
      return 0;
    }
    cVar2 = '\0';
    do {
      if (cVar2 != '\0') goto LAB_00056860;
      usleep(10000);
      sStack_1050 = 0x10;
      iVar1 = accept(DAT_00076b9c,&sStack_1038,&sStack_1050);
      cVar2 = DAT_0007acb8;
    } while (iVar1 == -1);
    if (DAT_0007acb8 != '\0') {
      close(iVar1);
      goto LAB_00056860;
    }
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      snprintf(local_1028,0x1000,"one client connected sock=%d",iVar1);
      FUN_00038730(7,local_1028,0);
    }
    FUN_000559d0(iVar1);
  } while( true );
}

