//==================== FUN_00026f50 @ 00026f50 ====================

void FUN_00026f50(void)

{
  undefined4 *puVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  undefined4 *puVar6;
  int iVar7;
  char *pcVar8;
  double dVar9;
  longlong param3;
  double dVar10;
  char cStack_1031;
  char local_1030 [4];
  char acStack_102c [4];
  uint local_1028;
  char acStack_1024 [4];
  undefined3 local_1020;
  char cStack_101d;
  char acStack_101c [4];
  uint local_1018;
  char acStack_1014 [4];
  char local_1010 [4];
  undefined3 uStack_100c;
  char cStack_1009;
  char acStack_1008 [4];
  char acStack_1004 [4];
  char local_1000 [4];
  undefined3 uStack_ffc;
  char cStack_ff9;
  undefined2 local_ff8;
  
  DAT_0007bac8 = 1;
  immedok(DAT_0007b430,1);
  FUN_00018ad8();
  iVar4 = 0;
LAB_00026fa0:
  do {
    if (DAT_0007b198 <= iVar4) {
LAB_00026fac:
      snprintf(local_1030,0x1000,"\nCurrent pool management strategy: %s\n",
               (char *)(&DAT_00075e10)[DAT_00076efc]);
      FUN_00018bc0(local_1030);
      if (DAT_00076efc == 2) {
        snprintf(local_1030,0x1000,"Set to rotate every %d minutes\n",DAT_0007b558);
        FUN_00018bc0(local_1030);
      }
      pcVar8 = "disabled";
      if (DAT_0007b625 != 0) {
        pcVar8 = "enabled";
      }
      snprintf(local_1030,0x1000,"[F]ailover only %s\n",pcVar8);
      FUN_00018bc0(local_1030);
      local_1030[0] = 'P';
      local_1030[1] = 'o';
      local_1030[2] = 'o';
      local_1030[3] = 'l';
      acStack_102c[0] = ' ';
      acStack_102c[1] = '[';
      acStack_102c[2] = 'A';
      acStack_102c[3] = ']';
      local_1028._0_1_ = 'd';
      local_1028._1_1_ = 'd';
      local_1028._2_1_ = ' ';
      local_1028._3_1_ = '[';
      acStack_1024[0] = 'R';
      acStack_1024[1] = ']';
      acStack_1024[2] = 'e';
      acStack_1024[3] = 'm';
      local_1020._0_1_ = 'o';
      local_1020._1_1_ = 'v';
      local_1020._2_1_ = 'e';
      cStack_101d = ' ';
      acStack_101c[0] = '[';
      acStack_101c[1] = 'D';
      acStack_101c[2] = ']';
      acStack_101c[3] = 'i';
      local_1018._0_1_ = 's';
      local_1018._1_1_ = 'a';
      local_1018._2_1_ = 'b';
      local_1018._3_1_ = 'l';
      acStack_1014[0] = 'e';
      acStack_1014[1] = ' ';
      acStack_1014[2] = '[';
      acStack_1014[3] = 'E';
      local_1010[0] = ']';
      local_1010[1] = 'n';
      local_1010[2] = 'a';
      local_1010[3] = 'b';
      uStack_100c._0_1_ = 'l';
      uStack_100c._1_1_ = 'e';
      uStack_100c._2_1_ = ' ';
      cStack_1009 = '[';
      acStack_1008[0] = 'Q';
      acStack_1008[1] = ']';
      acStack_1008[2] = 'u';
      acStack_1008[3] = 'o';
      acStack_1004[0] = 't';
      acStack_1004[1] = 'a';
      acStack_1004[2] = ' ';
      acStack_1004[3] = 'c';
      local_1000[0] = 'h';
      local_1000[1] = 'a';
      local_1000[2] = 'n';
      local_1000[3] = 'g';
      _uStack_ffc = CONCAT13(cStack_ff9,0xa65);
      FUN_00018bc0(local_1030);
      local_1030[0] = '[';
      local_1030[1] = 'C';
      local_1030[2] = ']';
      local_1030[3] = 'h';
      acStack_102c[0] = 'a';
      acStack_102c[1] = 'n';
      acStack_102c[2] = 'g';
      acStack_102c[3] = 'e';
      local_1028._0_1_ = ' ';
      local_1028._1_1_ = 'm';
      local_1028._2_1_ = 'a';
      local_1028._3_1_ = 'n';
      acStack_1024[0] = 'a';
      acStack_1024[1] = 'g';
      acStack_1024[2] = 'e';
      acStack_1024[3] = 'm';
      local_1020._0_1_ = 'e';
      local_1020._1_1_ = 'n';
      local_1020._2_1_ = 't';
      cStack_101d = ' ';
      acStack_101c[0] = 's';
      acStack_101c[1] = 't';
      acStack_101c[2] = 'r';
      acStack_101c[3] = 'a';
      local_1018._0_1_ = 't';
      local_1018._1_1_ = 'e';
      local_1018._2_1_ = 'g';
      local_1018._3_1_ = 'y';
      acStack_1014[0] = ' ';
      acStack_1014[1] = '[';
      acStack_1014[2] = 'S';
      acStack_1014[3] = ']';
      local_1010[0] = 'w';
      local_1010[1] = 'i';
      local_1010[2] = 't';
      local_1010[3] = 'c';
      uStack_100c._0_1_ = 'h';
      uStack_100c._1_1_ = ' ';
      uStack_100c._2_1_ = 'p';
      cStack_1009 = 'o';
      acStack_1008[0] = 'o';
      acStack_1008[1] = 'l';
      acStack_1008[2] = ' ';
      acStack_1008[3] = '[';
      acStack_1004[0] = 'I';
      acStack_1004[1] = ']';
      acStack_1004[2] = 'n';
      acStack_1004[3] = 'f';
      local_1000[0] = 'o';
      local_1000[1] = 'r';
      local_1000[2] = 'm';
      local_1000[3] = 'a';
      uStack_ffc._0_1_ = 't';
      uStack_ffc._1_1_ = 'i';
      uStack_ffc._2_1_ = 'o';
      cStack_ff9 = 'n';
      local_ff8 = 10;
      FUN_00018bc0(local_1030);
      local_1030[0] = 'O';
      local_1030[1] = 'r';
      local_1030[2] = ' ';
      local_1030[3] = 'p';
      acStack_102c[0] = 'r';
      acStack_102c[1] = 'e';
      acStack_102c[2] = 's';
      acStack_102c[3] = 's';
      local_1028._0_1_ = ' ';
      local_1028._1_1_ = 'a';
      local_1028._2_1_ = 'n';
      local_1028._3_1_ = 'y';
      acStack_1024[0] = ' ';
      acStack_1024[1] = 'o';
      acStack_1024[2] = 't';
      acStack_1024[3] = 'h';
      local_1020._0_1_ = 'e';
      local_1020._1_1_ = 'r';
      local_1020._2_1_ = ' ';
      cStack_101d = 'k';
      acStack_101c[0] = 'e';
      acStack_101c[1] = 'y';
      acStack_101c[2] = ' ';
      acStack_101c[3] = 't';
      local_1018._0_1_ = 'o';
      local_1018._1_1_ = ' ';
      local_1018._2_1_ = 'c';
      local_1018._3_1_ = 'o';
      acStack_1014[0] = 'n';
      acStack_1014[1] = 't';
      acStack_1014[2] = 'i';
      acStack_1014[3] = 'n';
      local_1010[0] = 'u';
      local_1010[1] = 'e';
      local_1010[2] = '\n';
      local_1010[3] = '\0';
      FUN_00018bc0(local_1030);
      FUN_00018b44();
      cStack_1031 = wgetch(stdscr);
      iVar4 = strncasecmp(&cStack_1031,"a",1);
      if (iVar4 == 0) {
        FUN_00026d10(1);
        iVar4 = 0;
        goto LAB_00026fa0;
      }
      iVar4 = strncasecmp(&cStack_1031,"r",1);
      if (iVar4 == 0) {
        if (DAT_0007b198 < 2) {
          pcVar8 = "Cannot remove last pool";
LAB_00027404:
          local_1030 = *(char (*) [4])pcVar8;
          acStack_102c = *(char (*) [4])(pcVar8 + 4);
          local_1028 = *(uint *)(pcVar8 + 8);
          acStack_1024 = *(char (*) [4])(pcVar8 + 0xc);
          _local_1020 = *(undefined4 *)(pcVar8 + 0x10);
          acStack_101c = *(char (*) [4])(pcVar8 + 0x14);
          FUN_00018bc0(local_1030);
          goto LAB_00026fac;
        }
        iVar4 = FUN_000260d0("Select pool number");
        if ((-1 < iVar4) && (iVar4 < DAT_0007b198)) {
          iVar7 = *(int *)(DAT_0007b438 + iVar4 * 4);
          iVar4 = FUN_0001da58();
          if (iVar7 == iVar4) {
            FUN_00019bf8(0);
            iVar4 = FUN_0001da58();
          }
          else {
            iVar4 = FUN_0001da58();
          }
          if (iVar7 != iVar4) {
            iVar3 = *(int *)(iVar7 + 0x84);
            iVar4 = 0;
            *(undefined4 *)(iVar7 + 0x84) = 0;
            if (iVar3 == 1) {
              DAT_0007b5e4 = DAT_0007b5e4 + -1;
            }
            FUN_00015ac8(iVar7);
            goto LAB_00026fa0;
          }
          local_1030[0] = 'U';
          local_1030[1] = 'n';
          local_1030[2] = 'a';
          local_1030[3] = 'b';
          acStack_102c[0] = 'l';
          acStack_102c[1] = 'e';
          acStack_102c[2] = ' ';
          acStack_102c[3] = 't';
          local_1028._0_1_ = 'o';
          local_1028._1_1_ = ' ';
          local_1028._2_1_ = 'r';
          local_1028._3_1_ = 'e';
          acStack_1024[0] = 'm';
          acStack_1024[1] = 'o';
          acStack_1024[2] = 'v';
          acStack_1024[3] = 'e';
          local_1020._0_1_ = ' ';
          local_1020._1_1_ = 'p';
          local_1020._2_1_ = 'o';
          cStack_101d = 'o';
          acStack_101c[0] = 'l';
          acStack_101c[1] = ' ';
          acStack_101c[2] = 'd';
          acStack_101c[3] = 'u';
          local_1018._0_1_ = 'e';
          local_1018._1_1_ = ' ';
          local_1018._2_1_ = 't';
          local_1018._3_1_ = 'o';
          acStack_1014[0] = ' ';
          acStack_1014[1] = 'a';
          acStack_1014[2] = 'c';
          acStack_1014[3] = 't';
          local_1010[0] = 'i';
          local_1010[1] = 'v';
          local_1010[2] = 'i';
          local_1010[3] = 't';
          uStack_100c._0_1_ = 'y';
          uStack_100c._1_1_ = '\n';
          uStack_100c._2_1_ = '\0';
          FUN_00018bc0(local_1030);
          goto LAB_00026fac;
        }
      }
      else {
        iVar4 = strncasecmp(&cStack_1031,"s",1);
        if (iVar4 == 0) {
          iVar4 = FUN_000260d0("Select pool number");
          if ((iVar4 < 0) || (DAT_0007b198 <= iVar4)) goto LAB_00027124;
          iVar7 = *(int *)(DAT_0007b438 + iVar4 * 4);
          if (*(int *)(iVar7 + 0x84) != 1) {
            *(undefined4 *)(iVar7 + 0x84) = 1;
            DAT_0007b5e4 = DAT_0007b5e4 + 1;
          }
LAB_00027470:
          iVar4 = 0;
          FUN_00019bf8(iVar7);
          goto LAB_00026fa0;
        }
        iVar4 = strncasecmp(&cStack_1031,"d",1);
        if (iVar4 == 0) {
          if (DAT_0007b5e4 < 2) {
            local_1030[0] = 'C';
            local_1030[1] = 'a';
            local_1030[2] = 'n';
            local_1030[3] = 'n';
            acStack_102c[0] = 'o';
            acStack_102c[1] = 't';
            acStack_102c[2] = ' ';
            acStack_102c[3] = 'd';
            local_1028._0_1_ = 'i';
            local_1028._1_1_ = 's';
            local_1028._2_1_ = 'a';
            local_1028._3_1_ = 'b';
            acStack_1024[0] = 'l';
            acStack_1024[1] = 'e';
            acStack_1024[2] = ' ';
            acStack_1024[3] = 'l';
            local_1020._0_1_ = 'a';
            local_1020._1_1_ = 's';
            local_1020._2_1_ = 't';
            cStack_101d = ' ';
            acStack_101c[0] = 'p';
            acStack_101c[1] = 'o';
            acStack_101c[2] = 'o';
            acStack_101c[3] = 'l';
            local_1018 = local_1018 & 0xffffff00;
            FUN_00018bc0(local_1030);
            goto LAB_00026fac;
          }
          iVar4 = FUN_000260d0("Select pool number");
          if ((iVar4 < 0) || (DAT_0007b198 <= iVar4)) goto LAB_00027124;
          iVar7 = *(int *)(DAT_0007b438 + iVar4 * 4);
          if (*(int *)(iVar7 + 0x84) == 1) {
            DAT_0007b5e4 = DAT_0007b5e4 + -1;
          }
          iVar4 = 0;
          *(undefined4 *)(iVar7 + 0x84) = 0;
          iVar3 = FUN_0001da58();
          if (iVar7 == iVar3) {
            FUN_00019bf8(0);
            goto LAB_00026fa0;
          }
LAB_000271d8:
          iVar4 = 0;
          if (0 < DAT_0007b198) goto LAB_000271e8;
          goto LAB_00026fac;
        }
        iVar4 = strncasecmp(&cStack_1031,"e",1);
        if (iVar4 == 0) {
          iVar4 = FUN_000260d0("Select pool number");
          if ((-1 < iVar4) && (iVar4 < DAT_0007b198)) {
            iVar7 = *(int *)(DAT_0007b438 + iVar4 * 4);
            if (*(int *)(iVar7 + 0x84) != 1) {
              *(undefined4 *)(iVar7 + 0x84) = 1;
              DAT_0007b5e4 = DAT_0007b5e4 + 1;
            }
            iVar3 = *(int *)(iVar7 + 4);
            iVar4 = FUN_0001da58();
            if (iVar3 < *(int *)(iVar4 + 4)) goto LAB_00027470;
            goto LAB_000271d8;
          }
        }
        else {
          iVar4 = strncasecmp(&cStack_1031,"c",1);
          if (iVar4 == 0) {
            puVar1 = (undefined4 *)&DAT_00075e0c;
            iVar4 = 0;
            do {
              puVar1 = puVar1 + 1;
              iVar7 = iVar4 + 1;
              snprintf(local_1030,0x1000,"%d: %s\n",iVar4,(char *)*puVar1);
              FUN_00018bc0(local_1030);
              iVar4 = iVar7;
            } while (iVar7 != 5);
            uVar2 = FUN_000260d0("Select strategy number type");
            if (uVar2 < 5) {
              if ((uVar2 != 2) ||
                 (DAT_0007b558 = FUN_000260d0("Select interval in minutes"), DAT_0007b558 < 10000))
              {
                iVar4 = 0;
                DAT_00076efc = uVar2;
                FUN_00019bf8();
                goto LAB_00026fa0;
              }
              DAT_0007b558 = 0;
            }
          }
          else {
            iVar4 = strncasecmp(&cStack_1031,"i",1);
            if (iVar4 != 0) {
              iVar4 = strncasecmp(&cStack_1031,"q",1);
              if (iVar4 == 0) {
                iVar4 = FUN_000260d0("Select pool number");
                if ((iVar4 < 0) || (DAT_0007b198 <= iVar4)) goto LAB_00027124;
                iVar7 = *(int *)(DAT_0007b438 + iVar4 * 4);
                iVar4 = FUN_000260d0("Set quota");
                if (iVar4 < 0) {
                  pcVar8 = "Invalid negative quota\n";
                  goto LAB_00027404;
                }
                *(int *)(iVar7 + 0x38) = iVar4;
                iVar4 = 0;
                FUN_000152e0();
              }
              else {
                iVar4 = strncasecmp(&cStack_1031,"f",1);
                if (iVar4 != 0) {
                  FUN_00018ad8();
                  immedok(DAT_0007b430,0);
                  DAT_0007bac8 = 0;
                  return;
                }
                DAT_0007b625 = DAT_0007b625 ^ 1;
                iVar4 = 0;
              }
              goto LAB_00026fa0;
            }
            iVar4 = FUN_000260d0("Select pool number");
            if ((-1 < iVar4) && (iVar4 < DAT_0007b198)) {
              iVar7 = *(int *)(DAT_0007b438 + iVar4 * 4);
              iVar4 = FUN_00017fe4();
              if (iVar4 != 0) {
                snprintf(local_1030,0x1000,"Pool: %s\n",*(char **)(iVar7 + 0xc4));
                FUN_00015890(local_1030);
                iVar4 = *(int *)(iVar7 + 0x20);
                if (iVar4 != 0) {
                  pcVar8 = "S";
                  if (iVar4 < 2) {
                    pcVar8 = "";
                  }
                  snprintf(local_1030,0x1000,"SOLVED %d BLOCK%s!\n",iVar4,pcVar8);
                  FUN_00015890(local_1030);
                }
                if (*(char *)(iVar7 + 0x298) == '\0') {
                  pcVar8 = "Does not have";
                  if (*(int *)(iVar7 + 0x8c) != 0) {
                    pcVar8 = "Has";
                  }
                  snprintf(local_1030,0x1000,"%s own long-poll support\n",pcVar8);
                  FUN_00015890(local_1030);
                }
                snprintf(local_1030,0x1000," Queued work requests: %d\n",*(int *)(iVar7 + 0x94));
                FUN_00015890(local_1030);
                param3 = VectorAdd(*(undefined8 *)(iVar7 + 8),*(undefined8 *)(iVar7 + 0x10),8);
                snprintf(local_1030,0x1000," Share submissions: %lld\n",param3);
                FUN_00015890(local_1030);
                snprintf(local_1030,0x1000," Accepted shares: %lld\n",*(longlong *)(iVar7 + 8));
                FUN_00015890(local_1030);
                snprintf(local_1030,0x1000," Rejected shares: %lld\n",*(longlong *)(iVar7 + 0x10));
                FUN_00015890(local_1030);
                snprintf(local_1030,0x1000," Accepted difficulty shares: %1.f\n",
                         *(double *)(iVar7 + 0x68));
                FUN_00015890(local_1030);
                snprintf(local_1030,0x1000," Rejected difficulty shares: %1.f\n",
                         *(double *)(iVar7 + 0x70));
                FUN_00015890(local_1030);
                uVar5 = *(uint *)(iVar7 + 8);
                iVar3 = *(int *)(iVar7 + 0xc);
                uVar2 = *(uint *)(iVar7 + 0x10);
                iVar4 = *(int *)(iVar7 + 0x14);
                if ((uVar5 != 0 || iVar3 != 0) || (uVar2 != 0 || iVar4 != 0)) {
                  dVar9 = (double)FUN_0005cb64((int)((ulonglong)uVar2 * 100),
                                               iVar4 * 100 + (int)((ulonglong)uVar2 * 100 >> 0x20));
                  dVar10 = (double)FUN_0005cb64(uVar5 + uVar2,
                                                iVar3 + iVar4 + (uint)CARRY4(uVar5,uVar2));
                  snprintf(local_1030,0x1000," Reject ratio: %.1f%%\n",dVar9 / dVar10);
                  FUN_00015890(local_1030);
                }
                snprintf(local_1030,0x1000," Items worked on: %d\n",*(int *)(iVar7 + 0x44));
                FUN_00015890(local_1030);
                snprintf(local_1030,0x1000," Discarded work due to new blocks: %d\n",
                         *(int *)(iVar7 + 0x9c));
                FUN_00015890(local_1030);
                snprintf(local_1030,0x1000," Stale submissions discarded due to new blocks: %d\n",
                         *(int *)(iVar7 + 0x98));
                FUN_00015890(local_1030);
                snprintf(local_1030,0x1000," Unable to get work from server occasions: %d\n",
                         *(int *)(iVar7 + 0xa0));
                FUN_00015890(local_1030);
                snprintf(local_1030,0x1000," Submitting work remotely delay occasions: %d\n\n",
                         *(int *)(iVar7 + 0xa4));
                FUN_00015890(local_1030);
                iVar4 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b7a8);
                if (iVar4 != 0) {
                  FUN_0000b5c4("unlock_curses",0xb72);
                }
                (*DAT_00075ed8)();
              }
              goto LAB_00026fac;
            }
          }
        }
      }
LAB_00027124:
      local_1030[0] = 'I';
      local_1030[1] = 'n';
      local_1030[2] = 'v';
      local_1030[3] = 'a';
      acStack_102c[0] = 'l';
      acStack_102c[1] = 'i';
      acStack_102c[2] = 'd';
      acStack_102c[3] = ' ';
      local_1028._0_1_ = 's';
      local_1028._1_1_ = 'e';
      local_1028._2_1_ = 'l';
      local_1028._3_1_ = 'e';
      acStack_1024[0] = 'c';
      acStack_1024[1] = 't';
      acStack_1024[2] = 'i';
      acStack_1024[3] = 'o';
      local_1020._0_1_ = 'n';
      local_1020._1_1_ = '\n';
      local_1020._2_1_ = '\0';
      FUN_00018bc0(local_1030);
      goto LAB_00026fac;
    }
LAB_000271e8:
    puVar6 = *(undefined4 **)(DAT_0007b438 + iVar4 * 4);
    puVar1 = (undefined4 *)FUN_0001da58();
    if (puVar6 == puVar1) {
      wattr_on(DAT_0007b430,0x200000,0);
    }
    if (puVar6[0x21] != 1) {
      wattr_on(DAT_0007b430,0x100000,0);
    }
    snprintf(local_1030,0x1000,"%d: ",*puVar6);
    FUN_00018bc0(local_1030);
    iVar7 = puVar6[0x21];
    if (iVar7 == 1) {
      local_1030[0] = 'E';
      local_1030[1] = 'n';
      local_1030[2] = 'a';
      local_1030[3] = 'b';
      acStack_102c[0] = 'l';
      acStack_102c[1] = 'e';
      acStack_102c[2] = 'd';
      acStack_102c[3] = ' ';
      local_1028 = local_1028 & 0xffffff00;
      FUN_00018bc0(local_1030);
    }
    else if (iVar7 == 0) {
      local_1030[0] = 'D';
      local_1030[1] = 'i';
      local_1030[2] = 's';
      local_1030[3] = 'a';
      acStack_102c[0] = 'b';
      acStack_102c[1] = 'l';
      acStack_102c[2] = 'e';
      acStack_102c[3] = 'd';
      local_1028 = CONCAT22(local_1028._2_2_,0x20);
      FUN_00018bc0(local_1030);
    }
    else if (iVar7 == 2) {
      local_1030[0] = 'R';
      local_1030[1] = 'e';
      local_1030[2] = 'j';
      local_1030[3] = 'e';
      acStack_102c[0] = 'c';
      acStack_102c[1] = 't';
      acStack_102c[2] = 'i';
      acStack_102c[3] = 'n';
      local_1028 = CONCAT13(local_1028._3_1_,0x2067);
      FUN_00018bc0(local_1030);
    }
    iVar4 = iVar4 + 1;
    pcVar8 = "Alive";
    if (*(char *)((int)puVar6 + 0x81) != '\0') {
      pcVar8 = "Dead";
    }
    snprintf(local_1030,0x1000,"%s Quota %d Prio %d: %s  User:%s\n",pcVar8,puVar6[0xe],puVar6[1],
             (char *)puVar6[0x31],(char *)puVar6[0x33]);
    FUN_00018bc0(local_1030);
    wattr_off(DAT_0007b430,0x300000,0);
  } while( true );
}

