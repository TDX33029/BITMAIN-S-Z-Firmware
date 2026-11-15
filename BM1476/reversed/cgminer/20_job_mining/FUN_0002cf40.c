//==================== FUN_0002cf40 @ 0002cf40 ====================

uint FUN_0002cf40(int *param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  char *pcVar5;
  char *pcVar6;
  undefined4 uVar7;
  size_t sVar8;
  char *param6;
  char *__s;
  char *__s_00;
  size_t sVar9;
  char *pcVar10;
  char *pcVar11;
  void *pvVar12;
  uint uVar13;
  char *unaff_r11;
  pthread_mutex_t *__mutex;
  pthread_rwlock_t *__rwlock;
  bool bVar14;
  bool bVar15;
  uint in_fpscr;
  undefined4 extraout_s1;
  double param4;
  double dVar16;
  undefined8 in_stack_ffffdd88;
  undefined8 uVar17;
  uint uStack_2240;
  int iStack_223c;
  int iStack_2238;
  int aiStack_2234 [23];
  char acStack_21d8 [160];
  timeval tStack_2138;
  uint local_2130;
  uint local_212c;
  uint local_2128;
  uint local_2124;
  uint local_2120;
  uint local_211c;
  char local_2038 [4];
  char acStack_2034 [4];
  char acStack_2030 [4];
  undefined2 local_202c [2];
  char local_2028 [4];
  char acStack_2024 [4];
  char acStack_2020 [4];
  char acStack_201c [4];
  char acStack_2018 [4];
  undefined2 local_2014;
  
  pcVar5 = (char *)((ulonglong)in_stack_ffffdd88 >> 0x20);
  if (param_2 == 0) {
    return 0;
  }
  iVar1 = FUN_00059230(param_2,0,aiStack_2234);
  if (iVar1 == 0) {
    if (DAT_00078e09 == '\0') {
      return 0;
    }
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7)) {
      return 0;
    }
    snprintf(local_2038,0x1000,"JSON decode failed(%d): %s",aiStack_2234[0],acStack_21d8);
    FUN_00038730(7,local_2038,0);
    return 0;
  }
  iVar2 = FUN_00059e20(iVar1,"method");
  if (iVar2 == 0) {
LAB_0002d0d8:
    uVar13 = 0;
    goto LAB_0002cfe4;
  }
  piVar3 = (int *)FUN_00059e20(iVar1,"error");
  piVar4 = (int *)FUN_00059e20(iVar1,"params");
  if ((piVar3 != (int *)0x0) && (*piVar3 != 7)) {
    pcVar5 = (char *)FUN_00057854(piVar3,3);
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      snprintf(local_2038,0x1000,"JSON-RPC method decode failed: %s",pcVar5);
      FUN_00038730(7,local_2038,0);
    }
    uVar13 = 0;
    free(pcVar5);
    goto LAB_0002cfe4;
  }
  pcVar6 = (char *)FUN_0005a178(iVar2);
  if (pcVar6 == (char *)0x0) goto LAB_0002d0d8;
  iVar2 = strncasecmp(pcVar6,"mining.notify",0xd);
  if (iVar2 != 0) {
    iVar2 = strncasecmp(pcVar6,"mining.set_extranonce",0x15);
    if (iVar2 == 0) {
      uVar13 = FUN_000299ac(param_1,piVar4);
      goto LAB_0002cfe4;
    }
    iVar2 = strncasecmp(pcVar6,"mining.set_difficulty",0x15);
    if (iVar2 != 0) {
      iVar2 = strncasecmp(pcVar6,"client.reconnect",0x10);
      if (iVar2 == 0) {
        memset(&tStack_2138,0,0xff);
        FUN_0005a008(piVar4,0);
        pcVar5 = (char *)FUN_0005a178();
        if (pcVar5 == (char *)0x0) {
          pcVar5 = (char *)param_1[0x9c];
LAB_0002df74:
          FUN_0005a008(piVar4,1);
          pcVar6 = (char *)FUN_0005a178();
          if (pcVar6 == (char *)0x0) {
            pcVar6 = (char *)param_1[0x98];
          }
          snprintf((char *)&tStack_2138,0xfe,"%s:%s",pcVar5,pcVar6);
          iVar2 = FUN_0002ab54(&tStack_2138,&iStack_223c,&iStack_2238);
          if (iVar2 != 0) {
            if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
              snprintf(local_2038,0x1000,"Stratum reconnect requested from pool %d to %s",*param_1,
                       (char *)&tStack_2138);
              FUN_00038730(4,local_2038,0);
            }
            FUN_000185c8(param_1);
            iVar2 = pthread_mutex_lock((pthread_mutex_t *)(param_1 + 0xb6));
            if (iVar2 != 0) {
              FUN_0000b708("parse_reconnect",0xa51);
            }
            FUN_00028ae8(param_1);
            *(undefined1 *)((int)param_1 + 0x29b) = 0;
            *(undefined1 *)((int)param_1 + 0x299) = 0;
            if (param_1[0x99] != 0) {
              close(param_1[0x99]);
            }
            pvVar12 = (void *)param_1[0x9c];
            param_1[0x99] = 0;
            param_1[0x96] = iStack_223c;
            param_1[0x9c] = iStack_223c;
            free(pvVar12);
            pvVar12 = (void *)param_1[0x98];
            param_1[0x98] = iStack_2238;
            free(pvVar12);
            iVar2 = pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0xb6));
            if (iVar2 != 0) {
              piVar3 = __errno_location();
              snprintf(local_2038,0x1000,"WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d",*piVar3
                       ,"util.c","parse_reconnect",0xa5a);
              FUN_00038730(3,local_2038,1);
              FUN_00016ca8(1);
            }
            (*DAT_00075ed8)();
            uVar13 = FUN_0002cec8(param_1);
            goto LAB_0002cfe4;
          }
        }
        else {
          pcVar6 = (char *)param_1[0x9c];
          pcVar10 = strchr(pcVar6,0x2e);
          if (pcVar10 == (char *)0x0) {
            if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
              pcVar10 = "Denied stratum reconnect request for pool without domain \'%s\'";
LAB_0002dc04:
              uVar13 = 0;
              snprintf(local_2038,0x1000,pcVar10,pcVar6);
              FUN_00038730(3,local_2038,0);
              goto LAB_0002cfe4;
            }
          }
          else {
            pcVar11 = strchr(pcVar5,0x2e);
            if (pcVar11 == (char *)0x0) {
              if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
                pcVar10 = "Denied stratum reconnect request to url without domain \'%s\'";
                pcVar6 = pcVar5;
                goto LAB_0002dc04;
              }
            }
            else {
              iVar2 = strcmp(pcVar10,pcVar11);
              if (iVar2 == 0) goto LAB_0002df74;
              if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
                pcVar10 = "Denied stratum reconnect request to non-matching domain url \'%s\'";
                goto LAB_0002dc04;
              }
            }
          }
        }
      }
      else {
        iVar2 = strncasecmp(pcVar6,"client.get_version",0x12);
        if (iVar2 == 0) {
          iVar2 = FUN_00059e20(iVar1,&DAT_00061ca0);
          uVar13 = 0;
          if (iVar2 == 0) goto LAB_0002cfe4;
          FUN_00059e20(iVar1,&DAT_00061ca0);
          uVar7 = FUN_0005a248();
          pcVar5 = "{\"id\": %d, \"result\": \"cgminer/4.9.0\", \"error\": null}";
LAB_0002d2b0:
          sprintf(local_2038,pcVar5,uVar7);
          sVar8 = strlen(local_2038);
          uVar13 = FUN_0002b5c8(param_1,local_2038,sVar8);
          goto LAB_0002cfe4;
        }
        iVar2 = strncasecmp(pcVar6,"client.show_message",0x13);
        if (iVar2 == 0) {
          if ((piVar4 != (int *)0x0) && (*piVar4 == 1)) {
            FUN_0005a008(piVar4);
            iVar2 = FUN_0005a178();
            if (iVar2 != 0) {
              if ((DAT_00078e09 != '\0') &&
                 (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
                uVar17 = CONCAT44(pcVar5,iVar2);
                iVar2 = *param_1;
                pcVar5 = "Pool %d message: %s";
                goto LAB_0002d3e0;
              }
              goto LAB_0002d3f4;
            }
          }
        }
        else {
          iVar2 = strncasecmp(pcVar6,"mining.ping",0xb);
          if (iVar2 == 0) {
            if ((DAT_00078e09 != '\0') &&
               (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
              snprintf(local_2038,0x1000,"Pool %d ping",*param_1);
              FUN_00038730(7,local_2038,0);
            }
            iVar2 = FUN_00059e20(iVar1,&DAT_00061ca0);
            if (iVar2 != 0) {
              FUN_00059e20(iVar1,&DAT_00061ca0);
              uVar7 = FUN_0005a248();
              pcVar5 = "{\"id\": %d, \"result\": \"pong\", \"error\": null}";
              goto LAB_0002d2b0;
            }
          }
          else {
            iVar2 = strncasecmp(pcVar6,"mining.set_target",0x11);
            if (iVar2 == 0) {
              pvVar12 = (void *)FUN_000292f4(piVar4);
              if (pvVar12 != (void *)0x0) {
                FUN_00029830(&tStack_2138,pvVar12,0x20);
                iVar2 = pthread_mutex_lock((pthread_mutex_t *)(param_1 + 0x3d));
                if (iVar2 != 0) {
                  FUN_0000b708("parse_target",0xa9a);
                }
                iVar2 = pthread_rwlock_wrlock((pthread_rwlock_t *)(param_1 + 0x43));
                if (iVar2 != 0) {
                  piVar3 = __errno_location();
                  snprintf(local_2038,0x1000,"WTF WRLOCK ERROR ON LOCK! errno=%d in %s %s():%d",
                           *piVar3,"util.c","parse_target",0xa9a);
                  FUN_00038730(3,local_2038,1);
                  FUN_00016ca8(1);
                }
                param_1[0x19] =
                     tStack_2138.tv_sec << 0x18 | ((uint)tStack_2138.tv_sec >> 8 & 0xff) << 0x10 |
                     ((uint)tStack_2138.tv_sec >> 0x10 & 0xff) << 8 |
                     (uint)tStack_2138.tv_sec >> 0x18;
                param_1[0x13] =
                     local_2120 << 0x18 | (local_2120 >> 8 & 0xff) << 0x10 |
                     (local_2120 >> 0x10 & 0xff) << 8 | local_2120 >> 0x18;
                param_1[0x14] =
                     local_2124 << 0x18 | (local_2124 >> 8 & 0xff) << 0x10 |
                     (local_2124 >> 0x10 & 0xff) << 8 | local_2124 >> 0x18;
                param_1[0x15] =
                     local_2128 << 0x18 | (local_2128 >> 8 & 0xff) << 0x10 |
                     (local_2128 >> 0x10 & 0xff) << 8 | local_2128 >> 0x18;
                param_1[0x16] =
                     local_212c << 0x18 | (local_212c >> 8 & 0xff) << 0x10 |
                     (local_212c >> 0x10 & 0xff) << 8 | local_212c >> 0x18;
                param_1[0x12] =
                     local_211c << 0x18 | (local_211c >> 8 & 0xff) << 0x10 |
                     (local_211c >> 0x10 & 0xff) << 8 | local_211c >> 0x18;
                param_1[0x17] =
                     local_2130 << 0x18 | (local_2130 >> 8 & 0xff) << 0x10 |
                     (local_2130 >> 0x10 & 0xff) << 8 | local_2130 >> 0x18;
                param_1[0x18] =
                     tStack_2138.tv_usec << 0x18 | ((uint)tStack_2138.tv_usec >> 8 & 0xff) << 0x10 |
                     ((uint)tStack_2138.tv_usec >> 0x10 & 0xff) << 8 |
                     (uint)tStack_2138.tv_usec >> 0x18;
                FUN_0002930c((pthread_rwlock_t *)(param_1 + 0x43),"parse_target",0xa9d);
                iVar2 = pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0x3d));
                if (iVar2 != 0) {
                  piVar3 = __errno_location();
                  snprintf(local_2038,0x1000,"WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d",
                           *piVar3,"util.c","parse_target",0xa9d);
                  FUN_00038730(3,local_2038,1);
                  FUN_00016ca8(1);
                }
                uVar13 = 1;
                (*DAT_00075ed8)();
                free(pvVar12);
                goto LAB_0002cfe4;
              }
              if ((DAT_00078e09 != '\0') &&
                 (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
                local_2038[0] = 'p';
                local_2038[1] = 'a';
                local_2038[2] = 'r';
                local_2038[3] = 's';
                acStack_2034[0] = 'e';
                acStack_2034[1] = '_';
                acStack_2034[2] = 't';
                acStack_2034[3] = 'a';
                acStack_2030[0] = 'r';
                acStack_2030[1] = 'g';
                acStack_2030[2] = 'e';
                acStack_2030[3] = 't';
                local_202c[0]._0_1_ = ':';
                local_202c[0]._1_1_ = ' ';
                local_202c[1]._0_1_ = 'M';
                local_202c[1]._1_1_ = 'i';
                local_2028[0] = 's';
                local_2028[1] = 's';
                local_2028[2] = 'i';
                local_2028[3] = 'n';
                acStack_2024[0] = 'g';
                acStack_2024[1] = ' ';
                acStack_2024[2] = 'a';
                acStack_2024[3] = 'n';
                acStack_2020[0] = ' ';
                acStack_2020[1] = 'a';
                acStack_2020[2] = 'r';
                acStack_2020[3] = 'r';
                acStack_201c[0] = 'a';
                acStack_201c[1] = 'y';
                acStack_201c[2] = ' ';
                acStack_201c[3] = 'v';
                acStack_2018[0] = 'a';
                acStack_2018[1] = 'l';
                acStack_2018[2] = 'u';
                acStack_2018[3] = 'e';
                local_2014 = 0x2e;
                FUN_00038730(7,local_2038,0);
              }
            }
          }
        }
      }
      goto LAB_0002d0d8;
    }
    FUN_0005a008(piVar4,0);
    uVar7 = FUN_0005a378();
    param4 = (double)CONCAT44(extraout_s1,uVar7);
    in_fpscr = in_fpscr & 0xfffffff;
    if (param4 == 0.0) {
      uVar13 = 0;
      goto LAB_0002cfe4;
    }
    iVar2 = pthread_mutex_lock((pthread_mutex_t *)(param_1 + 0x3d));
    if (iVar2 != 0) {
      FUN_0000b708("parse_diff",0x9b3);
    }
    iVar2 = pthread_rwlock_wrlock((pthread_rwlock_t *)(param_1 + 0x43));
    if (iVar2 != 0) {
      piVar3 = __errno_location();
      pcVar5 = "parse_diff";
      snprintf(local_2038,0x1000,"WTF WRLOCK ERROR ON LOCK! errno=%d in %s %s():%d",*piVar3,"util.c"
               ,"parse_diff",0x9b3);
      FUN_00038730(3,local_2038,1);
      FUN_00016ca8(1);
    }
    dVar16 = *(double *)(param_1 + 0x1cc);
    *(double *)(param_1 + 0x1cc) = param4;
    iVar2 = pthread_rwlock_unlock((pthread_rwlock_t *)(param_1 + 0x43));
    if (iVar2 != 0) {
      piVar3 = __errno_location();
      pcVar5 = "parse_diff";
      snprintf(local_2038,0x1000,"WTF RWLOCK ERROR ON UNLOCK! errno=%d in %s %s():%d",*piVar3,
               "util.c","parse_diff",0x9b6);
      FUN_00038730(3,local_2038,1);
      FUN_00016ca8(1);
    }
    iVar2 = pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0x3d));
    if (iVar2 != 0) {
      piVar3 = __errno_location();
      pcVar5 = "parse_diff";
      snprintf(local_2038,0x1000,"WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d",*piVar3,
               "util.c","parse_diff",0x9b6);
      FUN_00038730(3,local_2038,1);
      FUN_00016ca8(1);
    }
    (*DAT_00075ed8)();
    if (param4 == dVar16) {
      if ((DAT_00078e09 != '\0') &&
         (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
        uVar13 = 1;
        snprintf(local_2038,0x1000,"Pool %d difficulty set to %f",*param_1,param4);
        FUN_00038730(7,local_2038,0);
        goto LAB_0002cfe4;
      }
    }
    else {
      uVar7 = VectorFloatToSigned(param4,3);
      dVar16 = (double)VectorSignedToFloat(uVar7,(byte)((in_fpscr & 0xfffffff) >> 0x16) & 3);
      if (param4 == dVar16) {
        if ((DAT_00078e09 != '\0') &&
           (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
          uVar17 = CONCAT44(pcVar5,uVar7);
          pcVar5 = "Pool %d difficulty changed to %d";
          iVar2 = *param_1;
LAB_0002d3e0:
          snprintf(local_2038,0x1000,pcVar5,iVar2,uVar17);
          goto LAB_0002d3e4;
        }
      }
      else if ((DAT_00078e09 != '\0') &&
              (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
        snprintf(local_2038,0x1000,"Pool %d difficulty changed to %.1f",*param_1,param4);
LAB_0002d3e4:
        FUN_00038730(7,local_2038,0);
      }
    }
LAB_0002d3f4:
    uVar13 = 1;
    goto LAB_0002cfe4;
  }
  pcVar5 = (char *)FUN_000292f4(piVar4);
  pcVar6 = (char *)FUN_000292f4(piVar4,1);
  pcVar10 = (char *)FUN_000292f4(piVar4,2);
  pcVar11 = (char *)FUN_000292f4(piVar4,3);
  param6 = (char *)FUN_000292f4(piVar4,4);
  __s = (char *)FUN_000292f4(piVar4,5);
  __s_00 = (char *)FUN_000292f4(piVar4,6);
  iVar2 = FUN_0005a008(piVar4,7);
  if (iVar2 == 0) {
    bVar15 = false;
  }
  else {
    piVar3 = (int *)FUN_0005a008(piVar4,7);
    bVar15 = *piVar3 == 5;
  }
  if (pcVar5 == (char *)0x0 || pcVar10 == (char *)0x0) {
    if (pcVar5 != (char *)0x0) goto LAB_0002dcb0;
LAB_0002dae8:
    if (pcVar10 != (char *)0x0) {
      free(pcVar10);
    }
    if (param6 != (char *)0x0) {
      free(param6);
    }
    if (pcVar11 != (char *)0x0) {
      free(pcVar11);
    }
    if (pcVar6 != (char *)0x0) {
      free(pcVar6);
    }
    if (__s_00 != (char *)0x0) {
      free(__s_00);
    }
    if (__s == (char *)0x0) {
      bVar15 = false;
    }
    else {
      bVar15 = false;
      free(__s);
    }
  }
  else {
    if (((pcVar11 == (char *)0x0 || param6 == (char *)0x0) ||
        (pcVar6 == (char *)0x0 || __s_00 == (char *)0x0)) || (__s == (char *)0x0)) {
LAB_0002dcb0:
      free(pcVar5);
      goto LAB_0002dae8;
    }
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      local_2038[0] = 'V';
      local_2038[1] = 'a';
      local_2038[2] = 'l';
      local_2038[3] = 'i';
      acStack_2034[0] = 'd';
      acStack_2034[1] = ' ';
      acStack_2034[2] = 'N';
      acStack_2034[3] = 'o';
      acStack_2030[0] = 't';
      acStack_2030[1] = 'i';
      acStack_2030[2] = 'f';
      acStack_2030[3] = 'y';
      local_202c[0] = 10;
      FUN_00038730(7,local_2038,0);
    }
    __mutex = (pthread_mutex_t *)(param_1 + 0x3d);
    iVar2 = pthread_mutex_lock(__mutex);
    if (iVar2 != 0) {
      FUN_0000b708("parse_notify_equihash",0x85e);
    }
    __rwlock = (pthread_rwlock_t *)(param_1 + 0x43);
    iVar2 = pthread_rwlock_wrlock(__rwlock);
    if (iVar2 != 0) {
      piVar3 = __errno_location();
      snprintf(local_2038,0x1000,"WTF WRLOCK ERROR ON LOCK! errno=%d in %s %s():%d",*piVar3,"util.c"
               ,"parse_notify_equihash",0x85e);
      FUN_00038730(3,local_2038,1);
      FUN_00016ca8(1);
    }
    free((void *)param_1[0xa8]);
    free((void *)param_1[0xa9]);
    free((void *)param_1[0xab]);
    free((void *)param_1[0xac]);
    free((void *)param_1[0xad]);
    param_1[0xa8] = (int)pcVar5;
    param_1[0xa9] = (int)pcVar10;
    param_1[0xab] = (int)pcVar6;
    param_1[0xac] = (int)__s_00;
    param_1[0xad] = (int)__s;
    *(bool *)(param_1 + 0xae) = bVar15;
    if (0.0 < *(double *)(param_1 + 0x1ce)) {
      *(double *)(param_1 + 0xb2) = *(double *)(param_1 + 0x1ce);
    }
    if (bVar15) {
      param_1[0xa2] = 0;
      param_1[0xa3] = 0;
    }
    sVar8 = strlen(pcVar6);
    sVar9 = strlen(pcVar10);
    param_1[0x1d0] = sVar9 + sVar8;
    iVar2 = (int)(sVar9 + sVar8) / 2;
    sVar8 = strlen(__s);
    sVar9 = strlen(__s_00);
    param_1[0x1d0] = iVar2;
    bVar14 = DAT_00078e09 != '\0';
    sVar8 = ((sVar9 >> 1) + (sVar8 >> 1) + 0x54 + iVar2) * 2 + 1;
    param_1[0xb0] = sVar8;
    if ((bVar14) && (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      snprintf(local_2038,0x1000,"%s: pool->swork.header_len = %d","parse_notify_equihash",sVar8);
      FUN_00038730(7,local_2038,0);
      sVar8 = param_1[0xb0];
    }
    if ((sVar8 & 3) != 0) {
      sVar8 = (sVar8 & 0xfffffffc) + 4;
      param_1[0xb0] = sVar8;
    }
    unaff_r11 = malloc(sVar8);
    if (unaff_r11 == (char *)0x0) {
      snprintf(local_2038,0x1000,"%s: Failed to malloc header. in %s %s():%d",
               "parse_notify_equihash","util.c","parse_notify_equihash",0x88c);
      FUN_00038730(3,local_2038,1);
      FUN_00016ca8(1);
      sVar8 = param_1[0xb0];
    }
    snprintf(unaff_r11,sVar8,"%s%s%s%s%s%s%s",(char *)param_1[0xab],(char *)param_1[0xa9],pcVar11,
             param6,(char *)param_1[0xad],(char *)param_1[0xac],
             "0000000000000000000000000000000000000000");
    iVar2 = FUN_00029830(param_1 + 0x191,unaff_r11,0x80);
    if (iVar2 == 0) {
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
        snprintf(local_2038,0x1000,"%s: Failed to convert header to header_bin, got %s",
                 "parse_notify_equihash",unaff_r11);
        FUN_00038730(4,local_2038,0);
      }
      FUN_0002930c(__rwlock,"parse_notify_equihash",0x89d);
      FUN_00029388(__mutex,"parse_notify_equihash",0x89d);
      (*DAT_00075ed8)();
      FUN_0001fb44(param_1);
      goto LAB_0002dcb0;
    }
    param_1[0x25] = param_1[0x25] + 1;
    bVar14 = 0xfffffffe < DAT_0007b648;
    DAT_0007b648 = DAT_0007b648 + 1;
    DAT_0007b64c = DAT_0007b64c + (uint)bVar14;
    iVar2 = pthread_rwlock_unlock(__rwlock);
    if (iVar2 != 0) {
      piVar3 = __errno_location();
      snprintf(local_2038,0x1000,"WTF RWLOCK ERROR ON UNLOCK! errno=%d in %s %s():%d",*piVar3,
               "util.c","parse_notify_equihash",0x8a6);
      FUN_00038730(3,local_2038,1);
      FUN_00016ca8(1);
    }
    FUN_00029388(__mutex,"parse_notify_equihash",0x8a6);
    (*DAT_00075ed8)();
    piVar3 = (int *)FUN_0001da58();
    if (param_1 == piVar3) {
      DAT_0007b8ac = 1;
    }
    if ((DAT_0007b380 != '\0') && (DAT_00078e09 != '\0')) {
      if ((DAT_0007ad48 == '\0') && ((DAT_00078e08 == '\0' && (DAT_000766c4 < 7)))) {
LAB_0002d7ec:
        if ((DAT_00078e08 == '\0') && (DAT_000766c4 < 7)) {
LAB_0002d85c:
          if ((DAT_00078e08 == '\0') && (DAT_000766c4 < 7)) {
LAB_0002d8cc:
            if ((DAT_00078e08 == '\0') && (DAT_000766c4 < 7)) {
LAB_0002d93c:
              if ((DAT_00078e08 == '\0') && (DAT_000766c4 < 7)) {
LAB_0002d9ac:
                if ((DAT_00078e08 == '\0') && (DAT_000766c4 < 7)) {
LAB_0002da1c:
                  if ((DAT_00078e08 == '\0') && (DAT_000766c4 < 7)) {
LAB_0002da38:
                    if ((DAT_00078e08 != '\0') || (6 < DAT_000766c4)) {
LAB_0002da58:
                      pcVar5 = "yes";
                      if (!bVar15) {
                        pcVar5 = "no";
                      }
                      snprintf(local_2038,0x1000,"clean: %s",pcVar5);
                      FUN_00038730(7,local_2038,0);
                    }
                  }
                  else {
LAB_0002e328:
                    snprintf(local_2038,0x1000,"ntime: %s",__s);
                    FUN_00038730(7,local_2038,0);
                    if (DAT_00078e09 != '\0') {
                      if (DAT_0007ad48 == '\0') goto LAB_0002da38;
                      goto LAB_0002da58;
                    }
                  }
                }
                else {
LAB_0002d9cc:
                  snprintf(local_2038,0x1000,"nbit: %s",__s_00);
                  FUN_00038730(7,local_2038,0);
                  if (DAT_00078e09 != '\0') {
                    if (DAT_0007ad48 == '\0') goto LAB_0002da1c;
                    goto LAB_0002e328;
                  }
                }
              }
              else {
LAB_0002d95c:
                snprintf(local_2038,0x1000,"reserved: %s",param6);
                FUN_00038730(7,local_2038,0);
                if (DAT_00078e09 != '\0') {
                  if (DAT_0007ad48 == '\0') goto LAB_0002d9ac;
                  goto LAB_0002d9cc;
                }
              }
            }
            else {
LAB_0002d8ec:
              snprintf(local_2038,0x1000,"merkle: %s",pcVar11);
              FUN_00038730(7,local_2038,0);
              if (DAT_00078e09 != '\0') {
                if (DAT_0007ad48 == '\0') goto LAB_0002d93c;
                goto LAB_0002d95c;
              }
            }
          }
          else {
LAB_0002d87c:
            snprintf(local_2038,0x1000,"prev_hash: %s",pcVar10);
            FUN_00038730(7,local_2038,0);
            if (DAT_00078e09 != '\0') {
              if (DAT_0007ad48 == '\0') goto LAB_0002d8cc;
              goto LAB_0002d8ec;
            }
          }
        }
        else {
LAB_0002d80c:
          snprintf(local_2038,0x1000,"version: %s",pcVar6);
          FUN_00038730(7,local_2038,0);
          if (DAT_00078e09 != '\0') {
            if (DAT_0007ad48 == '\0') goto LAB_0002d85c;
            goto LAB_0002d87c;
          }
        }
      }
      else {
        snprintf(local_2038,0x1000,"job_id: %s",pcVar5);
        FUN_00038730(7,local_2038,0);
        if (DAT_00078e09 != '\0') {
          if (DAT_0007ad48 == '\0') goto LAB_0002d7ec;
          goto LAB_0002d80c;
        }
      }
    }
    if (DAT_00078ca8 == '\0') {
      FUN_00029830(&uStack_2240,__s,4);
      gettimeofday(&tStack_2138,(__timezone_ptr_t)0x0);
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
        snprintf(local_2038,0x1000,"stime.tv_sec %lu, block_ntime %lu\n",tStack_2138.tv_sec,
                 uStack_2240);
        FUN_00038730(5,local_2038,0);
      }
      if (tStack_2138.tv_sec + 0xe10U < uStack_2240) {
        tStack_2138.tv_sec = uStack_2240;
        settimeofday(&tStack_2138,(timezone *)0x0);
      }
      DAT_00078ca8 = '\x01';
    }
    bVar15 = true;
  }
  if (unaff_r11 != (char *)0x0) {
    free(unaff_r11);
  }
  if (!bVar15) {
    *(undefined1 *)((int)param_1 + 0x29b) = 0;
  }
  else {
    *(undefined1 *)((int)param_1 + 0x29b) = 1;
  }
  uVar13 = (uint)bVar15;
LAB_0002cfe4:
  if (*(int *)(iVar1 + 4) == -1) {
    return uVar13;
  }
  iVar2 = *(int *)(iVar1 + 4) + -1;
  *(int *)(iVar1 + 4) = iVar2;
  if (iVar2 == 0) {
    FUN_0005a3e4(iVar1);
    return uVar13;
  }
  return uVar13;
}

