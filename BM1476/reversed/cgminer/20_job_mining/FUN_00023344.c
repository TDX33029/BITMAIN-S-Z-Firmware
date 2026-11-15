//==================== FUN_00023344 @ 00023344 ====================

/* WARNING: Restarted to delay deadcode elimination for space: ram */

undefined4 FUN_00023344(void)

{
  char cVar1;
  int iVar2;
  int *piVar3;
  time_t tVar4;
  char *pcVar5;
  size_t sVar6;
  undefined4 extraout_r1;
  int iVar7;
  int *piVar8;
  int iVar9;
  int *piVar10;
  char *pcVar11;
  int iVar12;
  undefined4 uVar13;
  int iVar14;
  bool bVar15;
  bool bVar16;
  uint in_fpscr;
  double dVar17;
  undefined8 uVar18;
  double dVar19;
  double dVar20;
  undefined1 auStack_1184 [8];
  undefined4 local_117c;
  char acStack_1178 [4];
  undefined1 local_1174;
  char acStack_1170 [16];
  int aiStack_1160 [4];
  char acStack_1150 [256];
  char local_1050 [4];
  undefined3 local_104c;
  char cStack_1049;
  char acStack_1048 [4];
  char acStack_1044 [4];
  char local_1040 [4];
  char acStack_103c [4];
  char acStack_1038 [4];
  char acStack_1034 [4];
  uint local_1030;
  char acStack_102c [4];
  char acStack_1028 [4];
  char acStack_1024 [4];
  char acStack_1020 [4];
  undefined2 local_101c;
  
  pthread_setcanceltype(1,(int *)0x0);
  FUN_0002f0b4("Watchdog");
  FUN_000120f4();
  FUN_0002a84c(&DAT_00077310);
  do {
    sleep(2);
    FUN_00022f88();
    FUN_0001bba4(0xffffffff,extraout_r1,0,0);
    iVar2 = FUN_00017fe4();
    if (iVar2 != 0) {
      if (DAT_0007bc90 != 0) {
        iVar12 = (int)*(short *)(DAT_0007bc90 + 4);
        iVar14 = iVar12 + 1;
        iVar9 = *(short *)(DAT_0007bc90 + 6) + 1;
        bVar16 = SBORROW4(iVar9,0x4f);
        iVar2 = *(short *)(DAT_0007bc90 + 6) + -0x4e;
        bVar15 = iVar9 == 0x4f;
        if (0x4f < iVar9) {
          bVar16 = SBORROW4(iVar14,0x18);
          iVar2 = iVar12 + -0x17;
          bVar15 = iVar14 == 0x18;
        }
        if (!bVar15 && iVar2 < 0 == bVar16) {
          if ((DAT_00077020 + 2 < iVar14) && (DAT_00077020 < DAT_0007701c)) {
            DAT_00077020 = DAT_0007701c;
            if (iVar12 <= DAT_0007701c) {
              DAT_00077020 = iVar12 + -1;
            }
            DAT_00077024 = DAT_00077020 + 1;
            mvwin(DAT_0007b430,DAT_00077024,0);
            wresize(DAT_0007b7c0,DAT_00077020,iVar9);
          }
          if (DAT_0007b430 == 0) {
            iVar12 = -1;
            iVar2 = iVar12;
          }
          else {
            iVar2 = *(short *)(DAT_0007b430 + 4) + 1;
            iVar12 = *(short *)(DAT_0007b430 + 6) + 1;
          }
          if (iVar9 != iVar12 || iVar14 - DAT_00077024 != iVar2) {
            wresize(DAT_0007b430,iVar14 - DAT_00077024,iVar9);
          }
        }
      }
      iVar2 = FUN_0001da58();
      if (DAT_00077318 == '\0') {
        uVar13 = 0x50;
      }
      else {
        uVar13 = 100;
      }
      wattr_on(DAT_0007b7c0,0x200000);
      snprintf(local_1050,0x100," cgminer version 4.9.0 - Started: %s",&DAT_00077260);
      mvwprintw(DAT_0007b7c0,0,0,"%s",local_1050);
      wattr_off(DAT_0007b7c0,0x200000,0);
      iVar12 = wmove(DAT_0007b7c0,1,0);
      if (iVar12 != -1) {
        whline(DAT_0007b7c0,0x2d,uVar13);
      }
      snprintf(local_1050,0x100," %s",&DAT_00077160);
      mvwprintw(DAT_0007b7c0,2,0,"%s",local_1050);
      wclrtoeol(DAT_0007b7c0);
      iVar12 = DAT_0007b8a4;
      if (DAT_00077318 == '\0') {
        if (DAT_00077154 == '\0') {
          FUN_0005cb64(DAT_0007b100,DAT_0007b104,(undefined4)DAT_0007b670,DAT_0007b670._4_4_,
                       (undefined4)DAT_0007b5e8,DAT_0007b5e8._4_4_,(undefined4)DAT_0007b670,
                       DAT_0007b670._4_4_,DAT_0007b8a4);
          snprintf(local_1050,0x100," A:%.0f  R:%.0f  HW:%d  WU:%.1f/m");
          mvwprintw(DAT_0007b7c0,3,0,"%s",local_1050);
        }
        else {
          iVar12 = FUN_000188a8();
          snprintf(local_1050,0x100," ST: %d  SS: %lld  NB: %d  LW: %d  GF: %d  RF: %d",iVar12,
                   CONCAT44(DAT_0007bc84,DAT_0007bc80),DAT_0007bb1c,DAT_0007b4cc,DAT_0007b43c,
                   DAT_0007ae68);
          mvwprintw(DAT_0007b7c0,3,0,"%s",local_1050);
        }
      }
      else {
        dVar17 = (double)CONCAT44(DAT_0007b5e8._4_4_,(undefined4)DAT_0007b5e8);
        dVar20 = (double)CONCAT44(DAT_0007b670._4_4_,(undefined4)DAT_0007b670);
        dVar19 = (double)FUN_0005cb64(DAT_0007b100,DAT_0007b104);
        dVar19 = dVar19 / DAT_000755f8;
        iVar14 = FUN_000188a8();
        snprintf(local_1050,0x100,
                 " A:%.0f  R:%.0f  HW:%d  WU:%.1f/m | ST: %d  SS: %lld  NB: %d  LW: %d  GF: %d  RF: %d"
                 ,dVar17,dVar20,iVar12,dVar19 * 60.0,iVar14,CONCAT44(DAT_0007bc84,DAT_0007bc80),
                 DAT_0007bb1c,DAT_0007b4cc,DAT_0007b43c,DAT_0007ae68);
        mvwprintw(DAT_0007b7c0,3,0,"%s",local_1050);
      }
      wclrtoeol(DAT_0007b7c0);
      if ((DAT_00076efc - 3U < 2) && (1 < DAT_0007b198)) {
        pcVar11 = "out";
        if (DAT_0007bc79 != 0) {
          pcVar11 = "";
        }
        snprintf(local_1050,0x100," Connected to multiple pools with%s block change notify",pcVar11)
        ;
LAB_00024440:
        mvwprintw(DAT_0007b7c0,4,0,"%s",local_1050);
      }
      else {
        if (*(char *)(iVar2 + 0x298) != '\0') {
          snprintf(local_1050,0x100," Connected to %s diff %s with stratum as user %s",
                   *(char **)(iVar2 + 0x270),(char *)(iVar2 + 0x30),*(char **)(iVar2 + 0xcc));
          goto LAB_00024440;
        }
        pcVar5 = (char *)(uint)DAT_0007bc79;
        pcVar11 = pcVar5;
        if (pcVar5 == (char *)0x0) {
          pcVar11 = "out";
        }
        if (pcVar5 != (char *)0x0) {
          pcVar11 = "";
        }
        pcVar5 = "LP";
        if (*(char *)(iVar2 + 0x2f8) != '\0') {
          pcVar5 = "GBT";
        }
        snprintf(local_1050,0x100," Connected to %s diff %s with%s %s as user %s",
                 *(char **)(iVar2 + 0x270),(char *)(iVar2 + 0x30),pcVar11,pcVar5,
                 *(char **)(iVar2 + 0xcc));
        mvwprintw(DAT_0007b7c0,4,0,"%s",local_1050);
      }
      wclrtoeol(DAT_0007b7c0);
      snprintf(local_1050,0x100," Block: %s...  Diff:%s  Started: %s  Best share: %s   ",
               &DAT_00077300,&DAT_000772b8,&DAT_000772e0,(char *)&DAT_00075dc4);
      mvwprintw(DAT_0007b7c0,5,0,"%s",local_1050);
      iVar2 = wmove(DAT_0007b7c0,6,0);
      if (iVar2 != -1) {
        whline(DAT_0007b7c0,0x2d,uVar13);
      }
      iVar2 = wmove(DAT_0007b7c0,DAT_00077020 + -1,0);
      if (iVar2 != -1) {
        whline(DAT_0007b7c0,0x2d,uVar13);
      }
      local_1050[0] = '[';
      local_1050[1] = 'P';
      local_1050[2] = ']';
      local_1050[3] = 'o';
      local_104c._0_1_ = 'o';
      local_104c._1_1_ = 'l';
      local_104c._2_1_ = ' ';
      cStack_1049 = 'm';
      acStack_1048[0] = 'a';
      acStack_1048[1] = 'n';
      acStack_1048[2] = 'a';
      acStack_1048[3] = 'g';
      acStack_1044[0] = 'e';
      acStack_1044[1] = 'm';
      acStack_1044[2] = 'e';
      acStack_1044[3] = 'n';
      local_1040[0] = 't';
      local_1040[1] = ' ';
      local_1040[2] = '[';
      local_1040[3] = 'S';
      acStack_103c[0] = ']';
      acStack_103c[1] = 'e';
      acStack_103c[2] = 't';
      acStack_103c[3] = 't';
      acStack_1038[0] = 'i';
      acStack_1038[1] = 'n';
      acStack_1038[2] = 'g';
      acStack_1038[3] = 's';
      acStack_1034[0] = ' ';
      acStack_1034[1] = '[';
      acStack_1034[2] = 'D';
      acStack_1034[3] = ']';
      local_1030._0_1_ = 'i';
      local_1030._1_1_ = 's';
      local_1030._2_1_ = 'p';
      local_1030._3_1_ = 'l';
      acStack_102c[0] = 'a';
      acStack_102c[1] = 'y';
      acStack_102c[2] = ' ';
      acStack_102c[3] = 'o';
      acStack_1028[0] = 'p';
      acStack_1028[1] = 't';
      acStack_1028[2] = 'i';
      acStack_1028[3] = 'o';
      acStack_1024[0] = 'n';
      acStack_1024[1] = 's';
      acStack_1024[2] = ' ';
      acStack_1024[3] = '[';
      acStack_1020[0] = 'Q';
      acStack_1020[1] = ']';
      acStack_1020[2] = 'u';
      acStack_1020[3] = 'i';
      local_101c = 0x74;
      mvwprintw(DAT_0007b7c0,DAT_00077028 + -1,1,"%s",local_1050);
      if (0 < DAT_0007b720) {
        iVar12 = 0;
        iVar2 = iVar12;
        do {
          while (iVar9 = FUN_0001b83c(iVar12), iVar14 = iVar2, iVar9 == 0) {
LAB_000241cc:
            iVar12 = iVar12 + 1;
            iVar2 = iVar14;
            if (DAT_0007b720 <= iVar12) goto LAB_00024218;
          }
          iVar14 = iVar2 + 1;
          if (((DAT_0007b940 == '\0') && (iVar7 = iVar2 + DAT_00077028, iVar7 < LINES + -1)) &&
             (iVar2 < DAT_0007702c)) {
            if (*(int *)(iVar9 + 0x170) == 0) {
              dVar17 = DAT_000755f8;
            }
            else {
              FUN_0002a84c(auStack_1184);
              dVar17 = (double)FUN_0002aa24(auStack_1184,iVar9 + 0x170);
              iVar7 = iVar2 + DAT_00077028;
            }
            iVar2 = DAT_0007b7c0;
            uVar18 = *(undefined8 *)(iVar9 + 0xc0);
            in_fpscr = in_fpscr & 0xfffffff;
            dVar20 = (double)VectorSignedToFloat(*(undefined4 *)(iVar9 + 0x24),
                                                 (byte)(in_fpscr >> 0x16) & 3);
            if (dVar17 < 1.0) {
              dVar17 = 1.0;
            }
            *(double *)(iVar9 + 0x58) = (dVar20 / dVar17) * 60.0;
            wmove(iVar2,iVar7,0);
            FUN_00011548(iVar12,&DAT_00075df8);
            if (*(char **)(iVar9 + 0x1c) == (char *)0x0) {
              sprintf((char *)&local_117c,"%-8d",*(undefined4 *)(iVar9 + 8));
            }
            else {
              local_1174 = 0;
              local_117c = 0x20202020;
              acStack_1178[0] = ' ';
              acStack_1178[1] = ' ';
              acStack_1178[2] = ' ';
              acStack_1178[3] = ' ';
              strncpy((char *)&local_117c,*(char **)(iVar9 + 0x1c),8);
            }
            snprintf(local_1050,0x100," %*d: %s %-8s: ",DAT_00075df8,iVar12,
                     *(char **)(*(int *)(iVar9 + 4) + 8),(char *)&local_117c);
            wprintw(DAT_0007b7c0,"%s",local_1050);
            acStack_1150[0] = '\0';
            (**(code **)(*(int *)(iVar9 + 4) + 0x14))(acStack_1150,0x100,iVar9);
            sVar6 = strlen(acStack_1150);
            if (sVar6 < 0x17) {
              strncat(acStack_1150,"                                        ",0x17 - sVar6);
            }
            snprintf(local_1050,0x100,"%s | ",acStack_1150);
            wprintw(DAT_0007b7c0,"%s",local_1050);
            if (*(int *)(iVar9 + 0x60) == 2) {
              pcVar11 = "DEAD  ";
LAB_000242b4:
              local_1050 = *(char (*) [4])pcVar11;
              local_104c = (undefined3)*(undefined4 *)(pcVar11 + 4);
              wprintw(DAT_0007b7c0,"%s",local_1050);
            }
            else {
              if (*(int *)(iVar9 + 0x60) == 1) {
                pcVar11 = "SICK  ";
                goto LAB_000242b4;
              }
              if (*(int *)(iVar9 + 0x20) == 1) {
                pcVar11 = "OFF   ";
                goto LAB_000242b4;
              }
              if (*(int *)(iVar9 + 0x20) == 2) {
                pcVar11 = "REST  ";
                goto LAB_000242b4;
              }
              dVar20 = (double)FUN_0005cb64((int)uVar18,(int)((ulonglong)uVar18 >> 0x20));
              dVar20 = (dVar20 / dVar17) * 60.0;
              if (DAT_00077318 == '\0') {
                if (DAT_00077154 == '\0') {
                  dVar17 = (*(double *)(iVar9 + 0x50) / dVar17) * 1000000.0;
                  uVar18 = FUN_0005cc48(SUB84(dVar17,0),(int)((ulonglong)dVar17 >> 0x20));
                  FUN_00011580((int)uVar18,(int)((ulonglong)uVar18 >> 0x20),acStack_1170,0x10,4);
                  dVar17 = *(double *)(iVar9 + 0x30) * 1000000.0;
                  uVar18 = FUN_0005cc48(SUB84(dVar17,0),(int)((ulonglong)dVar17 >> 0x20));
                  FUN_00011580((int)uVar18,(int)((ulonglong)uVar18 >> 0x20),aiStack_1160,0x10,4);
                  uVar13 = VectorFloatToSigned(dVar20,3);
                  FUN_00011548(uVar13,&DAT_00075dfc);
                  snprintf(local_1050,0x100,"%6s / %6sh/s WU:%*.1f/m",(char *)aiStack_1160,
                           acStack_1170,DAT_00075dfc + 2,dVar20);
                  wprintw(DAT_0007b7c0,"%s",local_1050);
                }
                else {
                  FUN_00011514((float)*(double *)(iVar9 + 200),&DAT_00075e00);
                  FUN_00011514((float)*(double *)(iVar9 + 0xd0),&DAT_00075e04);
                  FUN_00011548(*(undefined4 *)(iVar9 + 0x2c),&DAT_00075e08);
                  snprintf(local_1050,0x100,"A:%*.0f R:%*.0f HW:%*d",DAT_00075e00,
                           *(double *)(iVar9 + 200),DAT_00075e04,*(double *)(iVar9 + 0xd0),
                           DAT_00075e08,*(int *)(iVar9 + 0x2c));
                  wprintw(DAT_0007b7c0,"%s",local_1050);
                }
              }
              else {
                dVar17 = (*(double *)(iVar9 + 0x50) / dVar17) * 1000000.0;
                uVar18 = FUN_0005cc48(SUB84(dVar17,0),(int)((ulonglong)dVar17 >> 0x20));
                FUN_00011580((int)uVar18,(int)((ulonglong)uVar18 >> 0x20),acStack_1170,0x10,4);
                dVar17 = *(double *)(iVar9 + 0x30) * 1000000.0;
                uVar18 = FUN_0005cc48(SUB84(dVar17,0),(int)((ulonglong)dVar17 >> 0x20));
                FUN_00011580((int)uVar18,(int)((ulonglong)uVar18 >> 0x20),aiStack_1160,0x10,4);
                uVar13 = VectorFloatToSigned(dVar20,3);
                FUN_00011548(uVar13,&DAT_00075dfc);
                FUN_00011514((float)*(double *)(iVar9 + 200),&DAT_00075e00);
                FUN_00011514((float)*(double *)(iVar9 + 0xd0),&DAT_00075e04);
                FUN_00011548(*(undefined4 *)(iVar9 + 0x2c),&DAT_00075e08);
                snprintf(local_1050,0x100,"%6s / %6sh/s WU:%*.1f/m A:%*.0f R:%*.0f HW:%*d",
                         aiStack_1160,acStack_1170,DAT_00075dfc + 2,dVar20,DAT_00075e00);
                wprintw(DAT_0007b7c0,"%s",local_1050);
              }
            }
            acStack_1150[0] = '\0';
            (**(code **)(*(int *)(iVar9 + 4) + 0x18))(acStack_1150,0x100,iVar9);
            snprintf(local_1050,0x100,"%s",acStack_1150);
            wprintw(DAT_0007b7c0,"%s",local_1050);
            wclrtoeol(DAT_0007b7c0);
            goto LAB_000241cc;
          }
          iVar12 = iVar12 + 1;
          iVar2 = iVar14;
        } while (iVar12 < DAT_0007b720);
      }
LAB_00024218:
      if (DAT_0007b7c0 == 0) {
        iVar2 = -1;
      }
      else {
        iVar2 = *(short *)(DAT_0007b7c0 + 4) + 1;
      }
      wtouchln(DAT_0007b7c0,0,iVar2,1);
      wrefresh(DAT_0007b7c0);
      if (DAT_0007b430 == 0) {
        iVar2 = -1;
      }
      else {
        iVar2 = *(short *)(DAT_0007b430 + 4) + 1;
      }
      wtouchln(DAT_0007b430,0,iVar2,1);
      wrefresh(DAT_0007b430);
      iVar2 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b7a8);
      if (iVar2 != 0) {
        FUN_0000b5c4("unlock_curses",0xb72);
      }
      (*DAT_00075ed8)();
    }
    FUN_0002a84c(aiStack_1160);
    if (DAT_0007b194 == '\0') {
      iVar2 = FUN_000118d4();
      if (iVar2 == 0) {
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
          snprintf(local_1050,0x1000,"Pausing execution as per stop time %02d:%02d scheduled",
                   DAT_0007b5fc,DAT_0007b5f8);
          FUN_00038730(4,local_1050,0);
        }
        if (DAT_0007b874 == '\0') {
          local_1050[0] = 'T';
          local_1050[1] = 'e';
          local_1050[2] = 'r';
          local_1050[3] = 'm';
          local_104c._0_1_ = 'i';
          local_104c._1_1_ = 'n';
          local_104c._2_1_ = 'a';
          cStack_1049 = 't';
          acStack_1048[0] = 'i';
          acStack_1048[1] = 'n';
          acStack_1048[2] = 'g';
          acStack_1048[3] = ' ';
          acStack_1044[0] = 'e';
          acStack_1044[1] = 'x';
          acStack_1044[2] = 'e';
          acStack_1044[3] = 'c';
          local_1040[0] = 'u';
          local_1040[1] = 't';
          local_1040[2] = 'i';
          local_1040[3] = 'o';
          acStack_103c[0] = 'n';
          acStack_103c[1] = ' ';
          acStack_103c[2] = 'a';
          acStack_103c[3] = 's';
          acStack_1038[0] = ' ';
          acStack_1038[1] = 'p';
          acStack_1038[2] = 'l';
          acStack_1038[3] = 'a';
          acStack_1034[0] = 'n';
          acStack_1034[1] = 'n';
          acStack_1034[2] = 'e';
          acStack_1034[3] = 'd';
          local_1030 = local_1030 & 0xffffff00;
          FUN_00038730(3,local_1050,1);
          FUN_00016ca8(0);
          return 0;
        }
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
          snprintf(local_1050,0x1000,"Will restart execution as scheduled at %02d:%02d",DAT_0007b880
                   ,DAT_0007b87c);
          FUN_00038730(4,local_1050,0);
        }
        DAT_0007b194 = '\x01';
        iVar2 = pthread_rwlock_rdlock((pthread_rwlock_t *)&DAT_0007b650);
        if (iVar2 != 0) {
          FUN_0000b69c("watchdog_thread",0x25f9);
        }
        if (0 < DAT_0007bb6c) {
          piVar8 = DAT_0007b5e0 + DAT_0007bb6c;
          piVar3 = DAT_0007b5e0;
          do {
            piVar10 = piVar3 + 1;
            *(undefined1 *)(*piVar3 + 0x3c) = 1;
            piVar3 = piVar10;
          } while (piVar10 != piVar8);
        }
        iVar2 = pthread_rwlock_unlock((pthread_rwlock_t *)&DAT_0007b650);
        if (iVar2 != 0) {
          FUN_0000b630("watchdog_thread",0x25fc);
        }
        (*DAT_00075ed8)();
      }
      else if (DAT_0007b194 != '\0') goto LAB_000233e4;
    }
    else {
LAB_000233e4:
      iVar2 = FUN_000118d4();
      if (iVar2 != 0) {
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
          snprintf(local_1050,0x1000,"Restarting execution as per start time %02d:%02d scheduled",
                   DAT_0007b880,DAT_0007b87c);
          FUN_00038730(4,local_1050,0);
        }
        if ((DAT_0007b5f0 != '\0') &&
           (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)))) {
          snprintf(local_1050,0x1000,"Will pause execution as scheduled at %02d:%02d",DAT_0007b5fc,
                   DAT_0007b5f8);
          FUN_00038730(4,local_1050,0);
        }
        iVar2 = 0;
        DAT_0007b194 = '\0';
        if (0 < DAT_0007bb6c) {
          do {
            iVar12 = iVar2 + 1;
            piVar3 = (int *)FUN_0001b124(iVar2);
            if (*(int *)(piVar3[9] + 0x20) != 1) {
              bVar15 = DAT_00078e09 != '\0';
              *(undefined1 *)(piVar3 + 0xf) = 0;
              if ((bVar15) &&
                 (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
                snprintf(local_1050,0x1000,"Pushing sem post to thread %d",*piVar3);
                FUN_00038730(7,local_1050,0);
              }
              FUN_0002f1bc(piVar3 + 4,"cgminer.c","watchdog_thread",0x2612);
            }
            iVar2 = iVar12;
          } while (iVar12 < DAT_0007bb6c);
        }
      }
    }
    if (0 < DAT_0007b720) {
      iVar2 = 0;
      do {
        iVar12 = FUN_0001b83c(iVar2);
        iVar14 = **(int **)(iVar12 + 0x98);
        if (iVar14 != 0) {
          (**(code **)(*(int *)(iVar12 + 4) + 0x20))();
          snprintf(acStack_1150,8,"%s %d",*(char **)(*(int *)(iVar12 + 4) + 8),*(int *)(iVar12 + 8))
          ;
          cVar1 = DAT_0007ad48;
          if ((*(char *)(iVar14 + 0x3d) == '\0') && (*(int *)(iVar12 + 0x20) != 1)) {
            iVar9 = *(int *)(iVar12 + 0x60);
            if (iVar9 == 0) {
              if (aiStack_1160[0] - *(int *)(iVar14 + 0x2c) < 0x79) {
LAB_00023904:
                if ((0x3c < aiStack_1160[0] - *(int *)(iVar14 + 0x34)) &&
                   ((iVar9 - 1U < 2 && (FUN_0002a84c(iVar14 + 0x34), DAT_00075e0c != '\0')))) {
LAB_000237c4:
                  FUN_00016c88(iVar12);
                }
              }
              else {
                *(undefined8 *)(iVar12 + 0x30) = 0;
                cVar1 = DAT_0007ad48;
                *(undefined4 *)(iVar12 + 0x60) = 1;
                if (((cVar1 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
                  snprintf(local_1050,0x1000,"%s: Idle for more than 60 seconds, declaring SICK!",
                           acStack_1150);
                  FUN_00038730(3,local_1050,0);
                }
                FUN_0002a84c(iVar14 + 0x34);
                FUN_0002ed88(iVar12,3);
                if (DAT_00075e0c != '\0') {
                  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
                    snprintf(local_1050,0x1000,"%s: Attempting to restart",acStack_1150);
                    FUN_00038730(3,local_1050,0);
                  }
                  goto LAB_000237c4;
                }
              }
            }
            else {
              iVar7 = aiStack_1160[0] - *(int *)(iVar14 + 0x2c);
              if (iVar7 < 0x78) {
                if ((iVar9 != 4) &&
                   (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)))) {
                  snprintf(local_1050,0x1000,"%s: Recovered, declaring WELL!",acStack_1150);
                  FUN_00038730(3,local_1050,0);
                }
                *(undefined4 *)(iVar12 + 0x60) = 0;
                tVar4 = time((time_t *)0x0);
                *(time_t *)(iVar12 + 0xf0) = tVar4;
              }
              else {
                if ((iVar9 != 1) || (iVar7 < 0x259)) goto LAB_00023904;
                *(undefined4 *)(iVar12 + 0x60) = 2;
                if (((cVar1 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
                  snprintf(local_1050,0x1000,
                           "%s: Not responded for more than 10 minutes, declaring DEAD!",
                           acStack_1150);
                  FUN_00038730(3,local_1050,0);
                }
                FUN_0002a84c(iVar14 + 0x34);
                FUN_0002ed88(iVar12,4);
              }
            }
          }
        }
        iVar2 = iVar2 + 1;
      } while (iVar2 < DAT_0007b720);
    }
  } while( true );
}

