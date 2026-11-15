//==================== FUN_0001c668 @ 0001c668 ====================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0001c668(void)

{
  int iVar1;
  uint uVar2;
  char *param4;
  int iVar3;
  uint uVar4;
  int iVar5;
  uint in_fpscr;
  double dVar6;
  double dVar7;
  double dVar8;
  double dVar9;
  double dVar10;
  int local_1150;
  char acStack_1140 [256];
  undefined4 local_1040;
  char acStack_103c [4];
  char acStack_1038 [4];
  char acStack_1034 [4];
  char local_1030 [4];
  char acStack_102c [4];
  char acStack_1028 [4];
  char acStack_1024 [4];
  ushort local_1020;
  undefined1 local_101e;
  
  dVar7 = DAT_000755f8;
  iVar3 = DAT_00077148 - DAT_000770d0;
  if (DAT_0007714c - DAT_000770d4 < 0) {
    iVar3 = iVar3 + -1;
  }
  dVar10 = (double)FUN_0005cb64(DAT_0007b420,DAT_0007b424);
  dVar8 = (double)FUN_0005cb64(DAT_0007b100,DAT_0007b104);
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
    local_1040._0_1_ = '\n';
    local_1040._1_1_ = 'S';
    local_1040._2_1_ = 'u';
    local_1040._3_1_ = 'm';
    acStack_103c[0] = 'm';
    acStack_103c[1] = 'a';
    acStack_103c[2] = 'r';
    acStack_103c[3] = 'y';
    acStack_1038[0] = ' ';
    acStack_1038[1] = 'o';
    acStack_1038[2] = 'f';
    acStack_1038[3] = ' ';
    acStack_1034[0] = 'r';
    acStack_1034[1] = 'u';
    acStack_1034[2] = 'n';
    acStack_1034[3] = 't';
    local_1030[0] = 'i';
    local_1030[1] = 'm';
    local_1030[2] = 'e';
    local_1030[3] = ' ';
    acStack_102c[0] = 's';
    acStack_102c[1] = 't';
    acStack_102c[2] = 'a';
    acStack_102c[3] = 't';
    acStack_1028[0] = 'i';
    acStack_1028[1] = 's';
    acStack_1028[2] = 't';
    acStack_1028[3] = 'i';
    acStack_1024[0] = 'c';
    acStack_1024[1] = 's';
    acStack_1024[2] = ':';
    acStack_1024[3] = '\n';
    local_1020 = local_1020 & 0xff00;
    FUN_00038730(4,&local_1040,0);
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
      snprintf((char *)&local_1040,0x1000,"Started at %s",&DAT_00077260);
      FUN_00038730(4,&local_1040,0);
    }
  }
  if (DAT_0007b198 == 1) {
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
      snprintf((char *)&local_1040,0x1000,"Pool: %s",*(char **)(*DAT_0007b438 + 0xc4));
      FUN_00038730(4,&local_1040,0);
      goto LAB_0001c7d4;
    }
  }
  else {
LAB_0001c7d4:
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
      snprintf((char *)&local_1040,0x1000,"Runtime: %d hrs : %d mins : %d secs",iVar3 / 0xe10,
               (iVar3 % 0xe10) / 0x3c,iVar3 % 0x3c);
      FUN_00038730(4,&local_1040,0);
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
        snprintf((char *)&local_1040,0x1000,"Average hashrate: %.1f Mhash/s",
                 _DAT_0007acc0 / DAT_000755f8);
        FUN_00038730(4,&local_1040,0);
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
          snprintf((char *)&local_1040,0x1000,"Solved blocks: %d",DAT_0007b8b8);
          FUN_00038730(4,&local_1040,0);
          if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
            snprintf((char *)&local_1040,0x1000,"Best share difficulty: %s",(char *)&DAT_00075dc4);
            FUN_00038730(4,&local_1040,0);
            if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
              snprintf((char *)&local_1040,0x1000,"Share submissions: %lld",
                       CONCAT44(DAT_0007b6fc + DAT_0007b424 +
                                (uint)CARRY4(DAT_0007b6f8,DAT_0007b420),DAT_0007b6f8 + DAT_0007b420)
                      );
              FUN_00038730(4,&local_1040,0);
              if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
                snprintf((char *)&local_1040,0x1000,"Accepted shares: %lld",
                         CONCAT44(DAT_0007b424,DAT_0007b420));
                FUN_00038730(4,&local_1040,0);
                if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
                  snprintf((char *)&local_1040,0x1000,"Rejected shares: %lld",
                           CONCAT44(DAT_0007b6fc,DAT_0007b6f8));
                  FUN_00038730(4,&local_1040,0);
                  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
                    snprintf((char *)&local_1040,0x1000,"Accepted difficulty shares: %1.f",
                             (double)CONCAT44(DAT_0007b5e8._4_4_,(undefined4)DAT_0007b5e8));
                    FUN_00038730(4,&local_1040,0);
                    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
                      snprintf((char *)&local_1040,0x1000,"Rejected difficulty shares: %1.f",
                               (double)CONCAT44(DAT_0007b670._4_4_,(undefined4)DAT_0007b670));
                      FUN_00038730(4,&local_1040,0);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  iVar1 = DAT_0007b6fc;
  uVar4 = DAT_0007b6f8;
  iVar3 = DAT_0007b424;
  uVar2 = DAT_0007b420;
  if ((DAT_0007b420 == 0 && DAT_0007b424 == 0) && (DAT_0007b6f8 == 0 && DAT_0007b6fc == 0)) {
LAB_0001cc10:
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
      snprintf((char *)&local_1040,0x1000,"Hardware errors: %d",DAT_0007b8a4);
      FUN_00038730(4,&local_1040,0);
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
        snprintf((char *)&local_1040,0x1000,"Utility (accepted shares / min): %.2f/min",
                 (dVar10 / dVar7) * 60.0);
        FUN_00038730(4,&local_1040,0);
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
          snprintf((char *)&local_1040,0x1000,"Work Utility (diff1 shares solved / min): %.2f/min\n"
                   ,(dVar8 / dVar7) * 60.0);
          FUN_00038730(4,&local_1040,0);
          if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
            snprintf((char *)&local_1040,0x1000,
                     "Stale submissions discarded due to new blocks: %lld",
                     CONCAT44(DAT_0007bc84,DAT_0007bc80));
            FUN_00038730(4,&local_1040,0);
            if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
              snprintf((char *)&local_1040,0x1000,"Unable to get work from server occasions: %d",
                       DAT_0007b43c);
              FUN_00038730(4,&local_1040,0);
              if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
                snprintf((char *)&local_1040,0x1000,"Work items generated locally: %d",DAT_0007b4cc)
                ;
                FUN_00038730(4,&local_1040,0);
                if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
                  snprintf((char *)&local_1040,0x1000,"Submitting work remotely delay occasions: %d"
                           ,DAT_0007ae68);
                  FUN_00038730(4,&local_1040,0);
                  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
                    snprintf((char *)&local_1040,0x1000,"New blocks detected on network: %d\n",
                             DAT_0007bb1c);
                    FUN_00038730(4,&local_1040,0);
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0' || (3 < DAT_000766c4)))) {
    dVar6 = (double)FUN_0005cb64((int)((ulonglong)DAT_0007b6f8 * 100),
                                 DAT_0007b6fc * 100 + (int)((ulonglong)DAT_0007b6f8 * 100 >> 0x20));
    dVar9 = (double)FUN_0005cb64(uVar2 + uVar4,iVar3 + iVar1 + (uint)CARRY4(uVar2,uVar4));
    snprintf((char *)&local_1040,0x1000,"Reject ratio: %.1f%%",dVar6 / dVar9);
    FUN_00038730(4,&local_1040,0);
    goto LAB_0001cc10;
  }
  if (1 < DAT_0007b198) {
    local_1150 = 0;
    do {
      iVar3 = DAT_0007b438[local_1150];
      if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 4)) {
        iVar1 = *(int *)(iVar3 + 0x20);
        if (iVar1 == 0) goto LAB_0001d354;
LAB_0001ceec:
        if ((DAT_00078e08 != '\0') || (3 < DAT_000766c4)) goto LAB_0001d30c;
      }
      else {
        snprintf((char *)&local_1040,0x1000,"Pool: %s",*(char **)(iVar3 + 0xc4));
        FUN_00038730(4,&local_1040,0);
        iVar1 = *(int *)(iVar3 + 0x20);
        if (iVar1 != 0) {
          if (DAT_0007ad48 == '\0') goto LAB_0001ceec;
LAB_0001d30c:
          param4 = "S";
          if (iVar1 < 2) {
            param4 = "";
          }
          snprintf((char *)&local_1040,0x1000,"SOLVED %d BLOCK%s!",iVar1,param4);
          FUN_00038730(4,&local_1040,0);
        }
        if (DAT_0007ad48 == '\0') {
LAB_0001d354:
          if ((DAT_00078e08 == '\0') && (DAT_000766c4 < 4)) goto LAB_0001cf0c;
        }
        snprintf((char *)&local_1040,0x1000," Share submissions: %lld",
                 CONCAT44(*(int *)(iVar3 + 0x14) + *(int *)(iVar3 + 0xc) +
                          (uint)CARRY4(*(uint *)(iVar3 + 0x10),*(uint *)(iVar3 + 8)),
                          *(uint *)(iVar3 + 0x10) + *(uint *)(iVar3 + 8)));
        FUN_00038730(4,&local_1040,0);
        if ((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0' || (3 < DAT_000766c4)))) {
          snprintf((char *)&local_1040,0x1000," Accepted shares: %lld",*(longlong *)(iVar3 + 8));
          FUN_00038730(4,&local_1040,0);
          if ((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0' || (3 < DAT_000766c4)))) {
            snprintf((char *)&local_1040,0x1000," Rejected shares: %lld",*(longlong *)(iVar3 + 0x10)
                    );
            FUN_00038730(4,&local_1040,0);
            if ((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0' || (3 < DAT_000766c4)))) {
              snprintf((char *)&local_1040,0x1000," Accepted difficulty shares: %1.f",
                       *(double *)(iVar3 + 0x68));
              FUN_00038730(4,&local_1040,0);
              if ((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0' || (3 < DAT_000766c4)))) {
                snprintf((char *)&local_1040,0x1000," Rejected difficulty shares: %1.f",
                         *(double *)(iVar3 + 0x70));
                FUN_00038730(4,&local_1040,0);
              }
            }
          }
        }
      }
LAB_0001cf0c:
      uVar2 = *(uint *)(iVar3 + 8);
      iVar1 = *(int *)(iVar3 + 0xc);
      if ((uVar2 == 0 && iVar1 == 0) && (*(int *)(iVar3 + 0x10) == 0 && *(int *)(iVar3 + 0x14) == 0)
         ) {
LAB_0001cfb4:
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
          snprintf((char *)&local_1040,0x1000," Items worked on: %d",*(int *)(iVar3 + 0x44));
          FUN_00038730(4,&local_1040,0);
          if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
            snprintf((char *)&local_1040,0x1000," Stale submissions discarded due to new blocks: %d"
                     ,*(int *)(iVar3 + 0x98));
            FUN_00038730(4,&local_1040,0);
            if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
              snprintf((char *)&local_1040,0x1000," Unable to get work from server occasions: %d",
                       *(int *)(iVar3 + 0xa0));
              FUN_00038730(4,&local_1040,0);
              if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
                snprintf((char *)&local_1040,0x1000,
                         " Submitting work remotely delay occasions: %d\n",*(int *)(iVar3 + 0xa4));
                FUN_00038730(4,&local_1040,0);
              }
            }
          }
        }
      }
      else if ((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0' || (3 < DAT_000766c4)))) {
        uVar4 = *(uint *)(iVar3 + 0x10);
        iVar5 = *(int *)(iVar3 + 0x14);
        dVar7 = (double)FUN_0005cb64((int)((ulonglong)uVar4 * 100),
                                     iVar5 * 100 + (int)((ulonglong)uVar4 * 100 >> 0x20));
        dVar8 = (double)FUN_0005cb64(uVar2 + uVar4,iVar1 + iVar5 + (uint)CARRY4(uVar2,uVar4));
        snprintf((char *)&local_1040,0x1000," Reject ratio: %.1f%%",dVar7 / dVar8);
        FUN_00038730(4,&local_1040,0);
        goto LAB_0001cfb4;
      }
      local_1150 = local_1150 + 1;
    } while (local_1150 < DAT_0007b198);
  }
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
    local_1040._0_1_ = 'S';
    local_1040._1_1_ = 'u';
    local_1040._2_1_ = 'm';
    local_1040._3_1_ = 'm';
    acStack_103c[0] = 'a';
    acStack_103c[1] = 'r';
    acStack_103c[2] = 'y';
    acStack_103c[3] = ' ';
    acStack_1038[0] = 'o';
    acStack_1038[1] = 'f';
    acStack_1038[2] = ' ';
    acStack_1038[3] = 'p';
    acStack_1034[0] = 'e';
    acStack_1034[1] = 'r';
    acStack_1034[2] = ' ';
    acStack_1034[3] = 'd';
    local_1030[0] = 'e';
    local_1030[1] = 'v';
    local_1030[2] = 'i';
    local_1030[3] = 'c';
    acStack_102c[0] = 'e';
    acStack_102c[1] = ' ';
    acStack_102c[2] = 's';
    acStack_102c[3] = 't';
    acStack_1028[0] = 'a';
    acStack_1028[1] = 't';
    acStack_1028[2] = 'i';
    acStack_1028[3] = 's';
    acStack_1024[0] = 't';
    acStack_1024[1] = 'i';
    acStack_1024[2] = 'c';
    acStack_1024[3] = 's';
    local_1020 = 0xa3a;
    local_101e = 0;
    FUN_00038730(4,&local_1040,0);
  }
  if (0 < DAT_0007b720) {
    iVar3 = 0;
    do {
      iVar1 = iVar3 + 1;
      iVar3 = FUN_0001b83c(iVar3);
      *(undefined **)(*(int *)(iVar3 + 4) + 0x14) = &DAT_0000f798;
      *(undefined **)(*(int *)(iVar3 + 4) + 0x18) = &DAT_0000f79c;
      FUN_00017744(acStack_1140,0xff,iVar3);
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
        snprintf((char *)&local_1040,0x1000,"%s",acStack_1140);
        FUN_00038730(4,&local_1040,0);
      }
      iVar3 = iVar1;
    } while (iVar1 < DAT_0007b720);
  }
  if (DAT_00077288 == 0) {
LAB_0001d2ac:
    if (DAT_0007ad48 == '\0') goto LAB_0001d2b8;
  }
  else {
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
      snprintf((char *)&local_1040,0x1000,"Mined %.0f accepted shares of %d requested\n",
               (double)CONCAT44(DAT_0007b5e8._4_4_,(undefined4)DAT_0007b5e8),DAT_00077288);
      FUN_00038730(4,&local_1040,0);
      dVar7 = (double)CONCAT44(DAT_0007b5e8._4_4_,(undefined4)DAT_0007b5e8);
      dVar8 = (double)VectorSignedToFloat(DAT_00077288,(byte)(in_fpscr >> 0x16) & 3);
      if (dVar7 < dVar8) {
        if (DAT_0007ad48 == '\0') goto LAB_0001d260;
        goto LAB_0001d280;
      }
      goto LAB_0001d2ac;
    }
    dVar7 = (double)CONCAT44(DAT_0007b5e8._4_4_,(undefined4)DAT_0007b5e8);
    dVar8 = (double)VectorSignedToFloat(DAT_00077288,(byte)(in_fpscr >> 0x16) & 3);
    if (dVar7 < dVar8) {
LAB_0001d260:
      if ((DAT_00078e08 == '\0') && (DAT_000766c4 < 4)) goto LAB_0001d2e0;
LAB_0001d280:
      snprintf((char *)&local_1040,0x1000,"WARNING - Mined only %.0f shares of %d requested.",dVar7,
               DAT_00077288);
      FUN_00038730(4,&local_1040,0);
      goto LAB_0001d2ac;
    }
LAB_0001d2b8:
    if ((DAT_00078e08 == '\0') && (DAT_000766c4 < 4)) goto LAB_0001d2e0;
  }
  local_1040._0_1_ = ' ';
  local_1040._1_1_ = '\0';
  FUN_00038730(4,&local_1040,0);
LAB_0001d2e0:
  fflush(stderr);
  fflush(stdout);
  return;
}

