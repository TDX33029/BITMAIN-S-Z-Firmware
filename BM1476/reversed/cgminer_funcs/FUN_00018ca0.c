//==================== FUN_00018ca0 @ 00018ca0 ====================

undefined1 FUN_00018ca0(uint *param_1,int param_2)

{
  char cVar1;
  undefined1 uVar2;
  int iVar3;
  uint uVar4;
  char *pcVar5;
  undefined1 auStack_1028 [8];
  undefined1 auStack_1020 [8];
  char local_1018 [4];
  char acStack_1014 [4];
  char acStack_1010 [4];
  char acStack_100c [4];
  char local_1008 [4];
  char acStack_1004 [4];
  undefined3 uStack_1000;
  char cStack_ffd;
  char acStack_ffc [4];
  undefined1 local_ff8;
  undefined4 uStack_ff4;
  undefined4 uStack_ff0;
  undefined4 uStack_fec;
  undefined4 local_fe8;
  undefined4 uStack_fe4;
  undefined4 uStack_fe0;
  undefined2 uStack_fdc;
  undefined1 local_fda;
  
  if ((char)param_1[0xbe] == '\0') {
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      pcVar5 = "Testing pool %s";
      goto LAB_000190ec;
    }
  }
  else if ((DAT_00078e09 != '\0') &&
          (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
    pcVar5 = "Retrieving block template from pool %s";
LAB_000190ec:
    snprintf(local_1018,0x1000,pcVar5,param_1[0x31]);
    FUN_00038730(7,local_1018,0);
  }
  if ((char)param_1[0xa6] != '\0') goto LAB_00018d50;
  if (DAT_00075dd4 == 0) {
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
      return 0;
    }
    local_1018[0] = 'C';
    local_1018[1] = 'U';
    local_1018[2] = 'R';
    local_1018[3] = 'L';
    acStack_1014[0] = ' ';
    acStack_1014[1] = 'i';
    acStack_1014[2] = 'n';
    acStack_1014[3] = 'i';
    acStack_1010[0] = 't';
    acStack_1010[1] = 'i';
    acStack_1010[2] = 'a';
    acStack_1010[3] = 'l';
    acStack_100c[0] = 'i';
    acStack_100c[1] = 's';
    acStack_100c[2] = 'a';
    acStack_100c[3] = 't';
    local_1008[0] = 'i';
    local_1008[1] = 'o';
    local_1008[2] = 'n';
    local_1008[3] = ' ';
    acStack_1004[0] = 'f';
    acStack_1004[1] = 'a';
    acStack_1004[2] = 'i';
    acStack_1004[3] = 'l';
    _uStack_1000 = CONCAT13(cStack_ffd,0x6465);
    FUN_00038730(3,local_1018,0);
    return 0;
  }
  if (*(char *)((int)param_1 + 0x83) == '\0') {
    if (DAT_00078e09 == '\0') {
      *(undefined1 *)((int)param_1 + 0x83) = 0;
    }
    else if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7)) {
      *(undefined1 *)((int)param_1 + 0x83) = 0;
      if ((char)param_1[0xbe] == '\0') {
        if ((char)param_1[0xe7] == '\0') {
LAB_00019004:
          if ((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) {
            if (DAT_000766c4 < 7) goto LAB_00018d0c;
            pcVar5 = "No GBT coinbase + append support found, using getwork protocol";
          }
          else {
            pcVar5 = "No GBT coinbase + append support found, using getwork protocol";
          }
LAB_00018f64:
          local_1018 = *(char (*) [4])pcVar5;
          acStack_1014 = *(char (*) [4])(pcVar5 + 4);
          acStack_1010 = *(char (*) [4])(pcVar5 + 8);
          acStack_100c = *(char (*) [4])(pcVar5 + 0xc);
          local_1008 = *(char (*) [4])(pcVar5 + 0x10);
          acStack_1004 = *(char (*) [4])(pcVar5 + 0x14);
          _uStack_1000 = *(undefined4 *)(pcVar5 + 0x18);
          acStack_ffc = *(char (*) [4])(pcVar5 + 0x1c);
          uStack_ff4 = *(undefined4 *)(pcVar5 + 0x24);
          uStack_ff0 = *(undefined4 *)(pcVar5 + 0x28);
          uStack_fec = *(undefined4 *)(pcVar5 + 0x2c);
          local_ff8 = (undefined1)*(undefined4 *)(pcVar5 + 0x20);
          local_fe8 = *(undefined4 *)(pcVar5 + 0x30);
          uStack_fe4 = *(undefined4 *)(pcVar5 + 0x34);
          uStack_fe0 = *(undefined4 *)(pcVar5 + 0x38);
          uStack_fdc = (undefined2)*(undefined4 *)(pcVar5 + 0x3c);
          local_fda = (undefined1)((uint)*(undefined4 *)(pcVar5 + 0x3c) >> 0x10);
          FUN_00038730(7,local_1018,0);
        }
        else {
LAB_00018fbc:
          if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)) {
            memcpy(local_1018,"GBT coinbase without append found, switching to GBT solo protocol",
                   0x42);
            FUN_00038730(7,local_1018,0);
          }
        }
      }
      else {
LAB_00018f34:
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)) {
          pcVar5 = "GBT coinbase + append support found, switching to GBT protocol";
          goto LAB_00018f64;
        }
      }
    }
    else {
      local_1018[0] = 'P';
      local_1018[1] = 'r';
      local_1018[2] = 'o';
      local_1018[3] = 'b';
      acStack_1014[0] = 'i';
      acStack_1014[1] = 'n';
      acStack_1014[2] = 'g';
      acStack_1014[3] = ' ';
      acStack_1010[0] = 'f';
      acStack_1010[1] = 'o';
      acStack_1010[2] = 'r';
      acStack_1010[3] = ' ';
      acStack_100c[0] = 'G';
      acStack_100c[1] = 'B';
      acStack_100c[2] = 'T';
      acStack_100c[3] = ' ';
      local_1008[0] = 's';
      local_1008[1] = 'u';
      local_1008[2] = 'p';
      local_1008[3] = 'p';
      acStack_1004[0] = 'o';
      acStack_1004[1] = 'r';
      acStack_1004[2] = 't';
      acStack_1004[3] = '\0';
      FUN_00038730(7,local_1018,0);
      cVar1 = DAT_00078e09;
      *(undefined1 *)((int)param_1 + 0x83) = 0;
      if ((char)param_1[0xbe] == '\0') {
        if ((char)param_1[0xe7] == '\0') {
          if (cVar1 != '\0') goto LAB_00019004;
        }
        else if (cVar1 != '\0') goto LAB_00018fbc;
      }
      else if (cVar1 != '\0') goto LAB_00018f34;
    }
  }
LAB_00018d0c:
  FUN_0002a84c(auStack_1028);
  FUN_0002a84c(auStack_1020);
  pcVar5 = (char *)param_1[0x96];
  if ((pcVar5 == (char *)0x0) || (DAT_00077054 != '\0')) {
LAB_00018d38:
    iVar3 = FUN_0002b8e8(param_1);
    if (iVar3 == 0) {
      if ((DAT_00078e09 != '\0') &&
         (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
        snprintf(local_1018,0x1000,"FAILED to retrieve work from pool %u %s",*param_1,
                 (char *)param_1[0x31]);
        FUN_00038730(7,local_1018,0);
      }
      if (((param_2 == 0) && (*(char *)((int)param_1 + 0x81) == '\0')) &&
         ((DAT_0007ad48 != '\0' || ((DAT_00078e08 != '\0' || (3 < DAT_000766c4)))))) {
        snprintf(local_1018,0x1000,"Pool %u slow/down or URL or credentials invalid",*param_1);
        FUN_00038730(4,local_1018,0);
      }
      return 0;
    }
  }
  else {
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      snprintf(local_1018,0x1000,"Testing pool %d stratum %s",*param_1,pcVar5);
      FUN_00038730(7,local_1018,0);
      pcVar5 = (char *)param_1[0x96];
    }
    FUN_0002aa68(param_1,pcVar5);
    iVar3 = FUN_0002ab54(param_1[0x96],param_1 + 0x9c,param_1 + 0x98);
    if ((iVar3 == 0) || (iVar3 = FUN_0002b8e8(param_1), iVar3 == 0)) goto LAB_00018d38;
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      snprintf(local_1018,0x1000,"Switching pool %d %s to %s",*param_1,(char *)param_1[0x31],
               (char *)param_1[0x96]);
      FUN_00038730(7,local_1018,0);
    }
    if (param_1[0x31] == 0) {
      uVar4 = __strdup(param_1[0x96]);
      param_1[0x31] = uVar4;
    }
  }
  *(undefined1 *)(param_1 + 0xa6) = 1;
LAB_00018d50:
  iVar3 = FUN_00018834(param_1,(int)param_1 + 0x29a);
  if (iVar3 == 0) {
    iVar3 = FUN_0002b8e8(param_1);
    if ((iVar3 == 0) || (iVar3 = FUN_0002ea38(param_1), iVar3 == 0)) {
      FUN_0002b8e4(param_1);
      FUN_00018c2c(param_1,(int)param_1 + 0x29a);
      uVar2 = 0;
    }
    else {
      FUN_0002b8e4(param_1);
      DAT_0007bc79 = 1;
      iVar3 = pthread_create(param_1 + 0xb4,(pthread_attr_t *)0x0,FUN_00021fe8,param_1);
      if (iVar3 != 0) {
        local_1018[0] = 'F';
        local_1018[1] = 'a';
        local_1018[2] = 'i';
        local_1018[3] = 'l';
        acStack_1014[0] = 'e';
        acStack_1014[1] = 'd';
        acStack_1014[2] = ' ';
        acStack_1014[3] = 't';
        acStack_1010[0] = 'o';
        acStack_1010[1] = ' ';
        acStack_1010[2] = 'c';
        acStack_1010[3] = 'r';
        acStack_100c[0] = 'e';
        acStack_100c[1] = 'a';
        acStack_100c[2] = 't';
        acStack_100c[3] = 'e';
        local_1008[0] = ' ';
        local_1008[1] = 's';
        local_1008[2] = 't';
        local_1008[3] = 'r';
        acStack_1004[0] = 'a';
        acStack_1004[1] = 't';
        acStack_1004[2] = 'u';
        acStack_1004[3] = 'm';
        uStack_1000._0_1_ = ' ';
        uStack_1000._1_1_ = 's';
        uStack_1000._2_1_ = 't';
        cStack_ffd = 'h';
        acStack_ffc[0] = 'r';
        acStack_ffc[1] = 'e';
        acStack_ffc[2] = 'a';
        acStack_ffc[3] = 'd';
        local_ff8 = 0;
        FUN_00038730(3,local_1018,1);
        FUN_00016ca8(1);
      }
      iVar3 = pthread_create(param_1 + 0xb5,(pthread_attr_t *)0x0,FUN_00020088,param_1);
      if (iVar3 == 0) {
        uVar2 = 1;
      }
      else {
        local_1018[0] = 'F';
        local_1018[1] = 'a';
        local_1018[2] = 'i';
        local_1018[3] = 'l';
        acStack_1014[0] = 'e';
        acStack_1014[1] = 'd';
        acStack_1014[2] = ' ';
        acStack_1014[3] = 't';
        acStack_1010[0] = 'o';
        acStack_1010[1] = ' ';
        acStack_1010[2] = 'c';
        acStack_1010[3] = 'r';
        acStack_100c[0] = 'e';
        acStack_100c[1] = 'a';
        acStack_100c[2] = 't';
        acStack_100c[3] = 'e';
        local_1008[0] = ' ';
        local_1008[1] = 's';
        local_1008[2] = 't';
        local_1008[3] = 'r';
        acStack_1004[0] = 'a';
        acStack_1004[1] = 't';
        acStack_1004[2] = 'u';
        acStack_1004[3] = 'm';
        uStack_1000._0_1_ = ' ';
        uStack_1000._1_1_ = 'r';
        uStack_1000._2_1_ = 't';
        cStack_ffd = 'h';
        acStack_ffc[0] = 'r';
        acStack_ffc[1] = 'e';
        acStack_ffc[2] = 'a';
        acStack_ffc[3] = 'd';
        local_ff8 = 0;
        FUN_00038730(3,local_1018,1);
        FUN_00016ca8(1);
        uVar2 = 1;
      }
    }
  }
  else {
    uVar2 = *(undefined1 *)((int)param_1 + 0x299);
  }
  return uVar2;
}

