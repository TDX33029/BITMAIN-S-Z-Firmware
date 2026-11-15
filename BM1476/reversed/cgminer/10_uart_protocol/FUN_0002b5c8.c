//==================== FUN_0002b5c8 @ 0002b5c8 ====================

int FUN_0002b5c8(int *param_1,char *param_2,undefined4 param_3)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  uint uVar4;
  char local_1018 [4];
  char acStack_1014 [4];
  char acStack_1010 [4];
  char acStack_100c [4];
  char local_1008 [4];
  char acStack_1004 [4];
  char acStack_1000 [4];
  undefined3 uStack_ffc;
  char cStack_ff9;
  char local_ff8 [4];
  char acStack_ff4 [4];
  char acStack_ff0 [4];
  char acStack_fec [4];
  undefined2 local_fe8;
  
  if (((DAT_0007b380 != '\0') && (DAT_00078e09 != '\0')) &&
     ((DAT_0007ad48 != '\0' || ((DAT_00078e08 != '\0' || (6 < DAT_000766c4)))))) {
    snprintf(local_1018,0x1000,"SEND: %s",param_2);
    FUN_00038730(7,local_1018,0);
  }
  iVar1 = pthread_mutex_lock((pthread_mutex_t *)(param_1 + 0xb6));
  if (iVar1 != 0) {
    FUN_0000b708("stratum_send",0x765);
  }
  if (*(char *)((int)param_1 + 0x299) == '\0') {
    uVar4 = 3;
    iVar1 = 0;
  }
  else {
    uVar4 = FUN_00029404(param_1,param_2,param_3);
    iVar1 = 1 - uVar4;
    if (1 < uVar4) {
      iVar1 = 0;
    }
  }
  iVar2 = pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0xb6));
  if (iVar2 != 0) {
    piVar3 = __errno_location();
    snprintf(local_1018,0x1000,"WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d",*piVar3,"util.c",
             "stratum_send",0x768);
    FUN_00038730(3,local_1018,1);
    FUN_00016ca8(1);
  }
  (*DAT_00075ed8)();
  if (uVar4 == 2) {
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
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
      acStack_1010[2] = 's';
      acStack_1010[3] = 'e';
      acStack_100c[0] = 'n';
      acStack_100c[1] = 'd';
      acStack_100c[2] = ' ';
      acStack_100c[3] = 'i';
      local_1008[0] = 'n';
      local_1008[1] = ' ';
      local_1008[2] = 's';
      local_1008[3] = 't';
      acStack_1004[0] = 'r';
      acStack_1004[1] = 'a';
      acStack_1004[2] = 't';
      acStack_1004[3] = 'u';
      acStack_1000[0] = 'm';
      acStack_1000[1] = '_';
      acStack_1000[2] = 's';
      acStack_1000[3] = 'e';
      _uStack_ffc = CONCAT13(cStack_ff9,0x646e);
      FUN_00038730(7,local_1018,0);
      FUN_0002aee0(param_1);
      return iVar1;
    }
  }
  else {
    if (uVar4 == 3) {
      if (DAT_00078e09 == '\0') {
        return iVar1;
      }
      if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7)) {
        return iVar1;
      }
      local_1018[0] = 'S';
      local_1018[1] = 't';
      local_1018[2] = 'r';
      local_1018[3] = 'a';
      acStack_1014[0] = 't';
      acStack_1014[1] = 'u';
      acStack_1014[2] = 'm';
      acStack_1014[3] = ' ';
      acStack_1010[0] = 's';
      acStack_1010[1] = 'e';
      acStack_1010[2] = 'n';
      acStack_1010[3] = 'd';
      acStack_100c[0] = ' ';
      acStack_100c[1] = 'f';
      acStack_100c[2] = 'a';
      acStack_100c[3] = 'i';
      local_1008[0] = 'l';
      local_1008[1] = 'e';
      local_1008[2] = 'd';
      local_1008[3] = ' ';
      acStack_1004[0] = 'd';
      acStack_1004[1] = 'u';
      acStack_1004[2] = 'e';
      acStack_1004[3] = ' ';
      acStack_1000[0] = 't';
      acStack_1000[1] = 'o';
      acStack_1000[2] = ' ';
      acStack_1000[3] = 'n';
      uStack_ffc._0_1_ = 'o';
      uStack_ffc._1_1_ = ' ';
      uStack_ffc._2_1_ = 'p';
      cStack_ff9 = 'o';
      local_ff8[0] = 'o';
      local_ff8[1] = 'l';
      local_ff8[2] = ' ';
      local_ff8[3] = 's';
      acStack_ff4[0] = 't';
      acStack_ff4[1] = 'r';
      acStack_ff4[2] = 'a';
      acStack_ff4[3] = 't';
      acStack_ff0[0] = 'u';
      acStack_ff0[1] = 'm';
      acStack_ff0[2] = '_';
      acStack_ff0[3] = 'a';
      acStack_fec[0] = 'c';
      acStack_fec[1] = 't';
      acStack_fec[2] = 'i';
      acStack_fec[3] = 'v';
      local_fe8 = 0x65;
      FUN_00038730(7,local_1018,0);
      return iVar1;
    }
    if (uVar4 != 1) {
      return iVar1;
    }
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      snprintf(local_1018,0x1000,"Write select failed on pool %d sock",*param_1);
      FUN_00038730(7,local_1018,0);
    }
  }
  FUN_0002aee0(param_1);
  return iVar1;
}

