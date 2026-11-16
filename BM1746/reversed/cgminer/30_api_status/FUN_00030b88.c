//==================== FUN_00030b88 @ 00030b88 ====================

void FUN_00030b88(int param_1,int param_2,int param_3)

{
  size_t sVar1;
  int param3;
  size_t param3_00;
  int *piVar2;
  size_t param3_01;
  char *param5;
  __suseconds_t *p_Var3;
  int param4;
  char *pcVar4;
  int iVar5;
  int iVar6;
  timeval tStack_10b0;
  fd_set local_10a8;
  char acStack_1028 [4100];
  
  pcVar4 = *(char **)(param_1 + 4);
  if (*(char *)(param_1 + 0xd) != '\0') {
    sVar1 = strlen(pcVar4);
    (pcVar4 + sVar1)[0] = ']';
    (pcVar4 + sVar1)[1] = '\0';
  }
  if (param_3 != 0) {
    sVar1 = strlen(pcVar4);
    builtin_strncpy(pcVar4 + sVar1,",\"id\":1}",9);
  }
  sVar1 = strlen(pcVar4);
  param3_01 = sVar1 + 1;
  if ((DAT_00078e09 != '\0') &&
     (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
    param5 = "...";
    if ((int)sVar1 < 0xb) {
      param5 = "";
    }
    snprintf(acStack_1028,0x1000,"API: send reply: (%d) \'%.10s%s\'",param3_01,pcVar4,param5);
    FUN_00038730(7,acStack_1028,0);
  }
  iVar6 = param_2 + 0x1f;
  if (-1 < param_2) {
    iVar6 = param_2;
  }
  param4 = 0;
  iVar5 = 0;
  sVar1 = param3_01;
  do {
    while( true ) {
      if (4 < iVar5 || (int)sVar1 < 1) {
        return;
      }
      p_Var3 = &tStack_10b0.tv_usec;
      tStack_10b0.tv_sec = 0;
      tStack_10b0.tv_usec = (__suseconds_t)&DAT_0000c350;
      do {
        p_Var3 = p_Var3 + 1;
        *p_Var3 = 0;
      } while (p_Var3 != local_10a8.fds_bits + 0x1f);
      local_10a8.fds_bits[iVar6 >> 5] =
           1 << (param_2 % 0x20 & 0xffU) | local_10a8.fds_bits[iVar6 >> 5];
      param3 = select(param_2 + 1,(fd_set *)0x0,&local_10a8,(fd_set *)0x0,&tStack_10b0);
      if (param3 < 1) {
        if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 4)) {
          return;
        }
        snprintf(acStack_1028,0x1000,"API: send select failed (%d)",param3);
        FUN_00038730(4,acStack_1028,0);
        return;
      }
      param3_00 = send(param_2,pcVar4,sVar1,0);
      param4 = param4 + 1;
      if ((int)param3_00 < 0) break;
      if (param4 == 1) {
        if (param3_00 == sVar1) {
          if ((DAT_00078e09 != '\0') &&
             (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
            snprintf(acStack_1028,0x1000,"API: sent all of %d first go",param3_00);
            goto LAB_00030ec4;
          }
          goto LAB_00030ed8;
        }
        if ((DAT_00078e09 != '\0') &&
           (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
          snprintf(acStack_1028,0x1000,"API: sent %d of %d first go",param3_00,sVar1);
          goto LAB_00030d78;
        }
LAB_00030d8c:
        sVar1 = sVar1 - param3_00;
        pcVar4 = pcVar4 + param3_00;
        if (param3_00 == 0) goto LAB_00030d9c;
      }
      else {
        if (param3_00 != sVar1) {
          if ((DAT_00078e09 != '\0') &&
             (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
            snprintf(acStack_1028,0x1000,"API: sent %d of remaining %d (sendc=%d)",param3_00,sVar1,
                     param4);
LAB_00030d78:
            FUN_00038730(7,acStack_1028,0);
          }
          goto LAB_00030d8c;
        }
        if ((DAT_00078e09 != '\0') &&
           (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
          snprintf(acStack_1028,0x1000,"API: sent all of remaining %d (sendc=%d)",param3_00,param4);
LAB_00030ec4:
          FUN_00038730(7,acStack_1028,0);
        }
LAB_00030ed8:
        sVar1 = sVar1 - param3_00;
        pcVar4 = pcVar4 + param3_00;
      }
    }
    piVar2 = __errno_location();
    if (*piVar2 != 0xb) {
      if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 4)) {
        return;
      }
      pcVar4 = strerror(*piVar2);
      snprintf(acStack_1028,0x1000,"API: send (%d:%d) failed: %s",param3_01,param3_01 - sVar1,pcVar4
              );
      FUN_00038730(4,acStack_1028,0);
      return;
    }
LAB_00030d9c:
    iVar5 = iVar5 + 1;
  } while( true );
}

