//==================== FUN_000260fc @ 000260fc ====================

void FUN_000260fc(void)

{
  int iVar1;
  uint param3;
  char *pcVar2;
  char *param11;
  char *param3_00;
  char *pcVar3;
  char *param3_01;
  char *param4;
  char *param7;
  char *param5;
  char *param8;
  char *param6;
  char *param9;
  char cStack_1029;
  char local_1028 [4];
  char acStack_1024 [4];
  char acStack_1020 [4];
  char acStack_101c [4];
  undefined3 local_1018;
  char cStack_1015;
  char acStack_1014 [4];
  char acStack_1010 [4];
  uint local_100c;
  char local_1008 [4];
  char acStack_1004 [4];
  char acStack_1000 [4];
  uint local_ffc;
  char acStack_ff8 [4];
  undefined1 local_ff4;
  
  DAT_0007bac8 = 1;
  immedok(DAT_0007b430,1);
  FUN_00018ad8();
LAB_00026140:
  do {
    while( true ) {
      while( true ) {
        while( true ) {
          pcVar3 = "disabled";
          local_1028[0] = '[';
          local_1028[1] = 'N';
          local_1028[2] = ']';
          local_1028[3] = 'o';
          acStack_1024[0] = 'r';
          acStack_1024[1] = 'm';
          acStack_1024[2] = 'a';
          acStack_1024[3] = 'l';
          acStack_1020[0] = ' ';
          acStack_1020[1] = '[';
          acStack_1020[2] = 'C';
          acStack_1020[3] = ']';
          acStack_101c[0] = 'l';
          acStack_101c[1] = 'e';
          acStack_101c[2] = 'a';
          acStack_101c[3] = 'r';
          param3_01 = "enabled";
          local_1018._0_1_ = ' ';
          local_1018._1_1_ = '[';
          local_1018._2_1_ = 'S';
          cStack_1015 = ']';
          acStack_1014[0] = 'i';
          acStack_1014[1] = 'l';
          acStack_1014[2] = 'e';
          acStack_1014[3] = 'n';
          acStack_1010[0] = 't';
          acStack_1010[1] = ' ';
          acStack_1010[2] = 'm';
          acStack_1010[3] = 'o';
          local_100c._0_1_ = 'd';
          local_100c._1_1_ = 'e';
          local_100c._2_1_ = ' ';
          local_100c._3_1_ = '(';
          local_1008[0] = 'd';
          local_1008[1] = 'i';
          local_1008[2] = 's';
          local_1008[3] = 'a';
          acStack_1004[0] = 'b';
          acStack_1004[1] = 'l';
          acStack_1004[2] = 'e';
          acStack_1004[3] = ' ';
          acStack_1000[0] = 'a';
          acStack_1000[1] = 'l';
          acStack_1000[2] = 'l';
          acStack_1000[3] = ' ';
          local_ffc._0_1_ = 'o';
          local_ffc._1_1_ = 'u';
          local_ffc._2_1_ = 't';
          local_ffc._3_1_ = 'p';
          acStack_ff8[0] = 'u';
          acStack_ff8[1] = 't';
          acStack_ff8[2] = ')';
          acStack_ff8[3] = '\n';
          local_ff4 = 0;
          FUN_00018bc0(local_1028);
          param3_00 = "off";
          pcVar2 = "on";
          if (DAT_0007b858 == 0) {
            param4 = param3_00;
          }
          else {
            param4 = pcVar2;
          }
          if (DAT_0007bc78 == 0) {
            param5 = param3_00;
          }
          else {
            param5 = pcVar2;
          }
          if (DAT_00078e08 == 0) {
            param6 = param3_00;
          }
          else {
            param6 = pcVar2;
          }
          if (DAT_0007b380 == 0) {
            param7 = param3_00;
          }
          else {
            param7 = pcVar2;
          }
          if (DAT_0007b85a == 0) {
            param8 = param3_00;
          }
          else {
            param8 = pcVar2;
          }
          if (DAT_0007b940 == 0) {
            param9 = param3_00;
          }
          else {
            param9 = pcVar2;
          }
          if (DAT_00078e09 != 0) {
            param3_00 = pcVar2;
          }
          if (DAT_00077155 == 0) {
            pcVar2 = pcVar3;
          }
          else {
            pcVar2 = param3_01;
          }
          if (DAT_00077318 == 0) {
            param11 = pcVar3;
          }
          else {
            param11 = param3_01;
          }
          snprintf(local_1028,0x1000,
                   "[D]ebug:%s\n[P]er-device:%s\n[Q]uiet:%s\n[V]erbose:%s\n[R]PC debug:%s\n[W]orkTime details:%s\nco[M]pact: %s\n[T]oggle status switching:%s\nw[I]descreen:%s\n[Z]ero statistics\n[L]og interval:%d\n"
                   ,param3_00,param4,param5,param6,param7,param8,param9,pcVar2,param11,DAT_00075dcc)
          ;
          FUN_00018bc0(local_1028);
          local_1028[0] = 'S';
          local_1028[1] = 'e';
          local_1028[2] = 'l';
          local_1028[3] = 'e';
          acStack_1024[0] = 'c';
          acStack_1024[1] = 't';
          acStack_1024[2] = ' ';
          acStack_1024[3] = 'a';
          acStack_1020[0] = 'n';
          acStack_1020[1] = ' ';
          acStack_1020[2] = 'o';
          acStack_1020[3] = 'p';
          acStack_101c[0] = 't';
          acStack_101c[1] = 'i';
          acStack_101c[2] = 'o';
          acStack_101c[3] = 'n';
          local_1018._0_1_ = ' ';
          local_1018._1_1_ = 'o';
          local_1018._2_1_ = 'r';
          cStack_1015 = ' ';
          acStack_1014[0] = 'a';
          acStack_1014[1] = 'n';
          acStack_1014[2] = 'y';
          acStack_1014[3] = ' ';
          acStack_1010[0] = 'o';
          acStack_1010[1] = 't';
          acStack_1010[2] = 'h';
          acStack_1010[3] = 'e';
          local_100c._0_1_ = 'r';
          local_100c._1_1_ = ' ';
          local_100c._2_1_ = 'k';
          local_100c._3_1_ = 'e';
          local_1008[0] = 'y';
          local_1008[1] = ' ';
          local_1008[2] = 't';
          local_1008[3] = 'o';
          acStack_1004[0] = ' ';
          acStack_1004[1] = 'r';
          acStack_1004[2] = 'e';
          acStack_1004[3] = 't';
          acStack_1000[0] = 'u';
          acStack_1000[1] = 'r';
          acStack_1000[2] = 'n';
          acStack_1000[3] = '\n';
          local_ffc = local_ffc & 0xffffff00;
          FUN_00018bc0(local_1028);
          FUN_00018b44();
          cStack_1029 = wgetch(stdscr);
          iVar1 = strncasecmp(&cStack_1029,"q",1);
          if (iVar1 != 0) break;
          DAT_0007bc78 = DAT_0007bc78 ^ 1;
          if (DAT_0007bc78 == 0) {
            param3_01 = pcVar3;
          }
          snprintf(local_1028,0x1000,"Quiet mode %s\n",param3_01);
          FUN_00018bc0(local_1028);
        }
        iVar1 = strncasecmp(&cStack_1029,"v",1);
        if (iVar1 != 0) break;
        DAT_00078e08 = DAT_00078e08 ^ 1;
        if (DAT_00078e08 != 0) {
          DAT_0007bc78 = 0;
          pcVar3 = param3_01;
        }
        snprintf(local_1028,0x1000,"Verbose mode %s\n",pcVar3);
        FUN_00018bc0(local_1028);
      }
      iVar1 = strncasecmp(&cStack_1029,"n",1);
      if (iVar1 != 0) break;
      DAT_00078e09 = 0;
      local_1028[0] = 'O';
      local_1028[1] = 'u';
      local_1028[2] = 't';
      local_1028[3] = 'p';
      acStack_1024[0] = 'u';
      acStack_1024[1] = 't';
      acStack_1024[2] = ' ';
      acStack_1024[3] = 'm';
      acStack_1020[0] = 'o';
      acStack_1020[1] = 'd';
      acStack_1020[2] = 'e';
      acStack_1020[3] = ' ';
      acStack_101c[0] = 'r';
      acStack_101c[1] = 'e';
      acStack_101c[2] = 's';
      acStack_101c[3] = 'e';
      DAT_00078e08 = 0;
      DAT_0007bc78 = 0;
      DAT_0007b380 = 0;
      local_1018._0_1_ = 't';
      local_1018._1_1_ = ' ';
      local_1018._2_1_ = 't';
      cStack_1015 = 'o';
      acStack_1014[0] = ' ';
      acStack_1014[1] = 'n';
      acStack_1014[2] = 'o';
      acStack_1014[3] = 'r';
      acStack_1010[0] = 'm';
      acStack_1010[1] = 'a';
      acStack_1010[2] = 'l';
      acStack_1010[3] = '\n';
      DAT_0007b940 = 0;
      DAT_0007b858 = 0;
      local_100c = local_100c & 0xffffff00;
      FUN_00018bc0(local_1028);
      iVar1 = FUN_00017fe4();
joined_r0x00026430:
      if (iVar1 != 0) {
        FUN_00018178();
      }
    }
    iVar1 = strncasecmp(&cStack_1029,"d",1);
    if (iVar1 == 0) {
      DAT_00078e09 = DAT_00078e09 ^ 1;
      if (DAT_00078e09 != 0) {
        DAT_0007bc78 = 0;
        pcVar3 = param3_01;
      }
      DAT_00078e08 = DAT_00078e09;
      snprintf(local_1028,0x1000,"Debug mode %s\n",pcVar3);
      FUN_00018bc0(local_1028);
      goto LAB_00026140;
    }
    iVar1 = strncasecmp(&cStack_1029,"m",1);
    if (iVar1 == 0) {
      DAT_0007b940 = DAT_0007b940 ^ 1;
      if (DAT_0007b940 == 0) {
        param3_01 = pcVar3;
      }
      snprintf(local_1028,0x1000,"Compact mode %s\n",param3_01);
      FUN_00018bc0(local_1028);
      iVar1 = FUN_00017fe4();
      goto joined_r0x00026430;
    }
    iVar1 = strncasecmp(&cStack_1029,"p",1);
    if (iVar1 == 0) {
      DAT_0007b858 = DAT_0007b858 ^ 1;
      if (DAT_0007b858 == 0) {
        param3_01 = pcVar3;
      }
      DAT_00078e08 = DAT_0007b858;
      snprintf(local_1028,0x1000,"Per-device stats %s\n",param3_01);
      FUN_00018bc0(local_1028);
    }
    else {
      iVar1 = strncasecmp(&cStack_1029,"r",1);
      if (iVar1 == 0) {
        DAT_0007b380 = DAT_0007b380 ^ 1;
        if (DAT_0007b380 != 0) {
          DAT_0007bc78 = 0;
          pcVar3 = param3_01;
        }
        snprintf(local_1028,0x1000,"RPC protocol debugging %s\n",pcVar3);
        FUN_00018bc0(local_1028);
      }
      else {
        iVar1 = strncasecmp(&cStack_1029,"c",1);
        if (iVar1 == 0) {
LAB_000266d8:
          FUN_00018ad8();
LAB_000266dc:
          immedok(DAT_0007b430,0);
          DAT_0007bac8 = 0;
          return;
        }
        iVar1 = strncasecmp(&cStack_1029,"l",1);
        if (iVar1 == 0) {
          param3 = FUN_000260d0("Interval in seconds");
          if (param3 < 10000) {
            DAT_00075dcc = param3;
            snprintf(local_1028,0x1000,"Log interval set to %d seconds\n",param3);
            FUN_00018bc0(local_1028);
          }
          else {
            local_1028[0] = 'I';
            local_1028[1] = 'n';
            local_1028[2] = 'v';
            local_1028[3] = 'a';
            acStack_1024[0] = 'l';
            acStack_1024[1] = 'i';
            acStack_1024[2] = 'd';
            acStack_1024[3] = ' ';
            acStack_1020[0] = 's';
            acStack_1020[1] = 'e';
            acStack_1020[2] = 'l';
            acStack_1020[3] = 'e';
            acStack_101c[0] = 'c';
            acStack_101c[1] = 't';
            acStack_101c[2] = 'i';
            acStack_101c[3] = 'o';
            _local_1018 = CONCAT13(cStack_1015,0xa6e);
            FUN_00018bc0(local_1028);
          }
        }
        else {
          iVar1 = strncasecmp(&cStack_1029,"s",1);
          if (iVar1 == 0) {
            DAT_0007bb18 = 1;
            goto LAB_000266dc;
          }
          iVar1 = strncasecmp(&cStack_1029,"w",1);
          if (iVar1 == 0) {
            DAT_0007b85a = DAT_0007b85a ^ 1;
            pcVar3 = "enabled";
            if (DAT_0007b85a == 0) {
              pcVar3 = "disabled";
            }
            snprintf(local_1028,0x1000,"WorkTime details %s\n",pcVar3);
            FUN_00018bc0(local_1028);
          }
          else {
            iVar1 = strncasecmp(&cStack_1029,"t",1);
            if (iVar1 == 0) {
              DAT_00077155 = DAT_00077155 ^ 1;
            }
            else {
              iVar1 = strncasecmp(&cStack_1029,"i",1);
              if (iVar1 == 0) {
                DAT_00077318 = DAT_00077318 ^ 1;
              }
              else {
                iVar1 = strncasecmp(&cStack_1029,"z",1);
                if (iVar1 != 0) goto LAB_000266d8;
                FUN_0001b8b4();
              }
            }
          }
        }
      }
    }
  } while( true );
}

