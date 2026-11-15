//==================== FUN_00022b44 @ 00022b44 ====================

undefined4 FUN_00022b44(int param_1,int param_2)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  char *pcVar6;
  int aiStack_1028 [2];
  undefined4 local_1020;
  undefined4 uStack_101c;
  undefined4 uStack_1018;
  undefined4 uStack_1014;
  undefined4 local_1010;
  undefined4 uStack_100c;
  uint local_1008;
  undefined4 uStack_1004;
  uint local_1000;
  undefined4 uStack_ffc;
  undefined2 local_ff8;
  
  if ((DAT_0007730d == '\0') && (DAT_00076ffc == 0)) {
    if (*(int *)(param_1 + 0x720) == DAT_000770ac) {
      iVar5 = *(int *)(param_1 + 0x6d4);
      iVar4 = *(int *)(param_1 + 0x6e4);
      if (*(int *)(param_1 + 0x6e4) <= DAT_00075df0) {
        iVar4 = DAT_00075df4;
      }
      if ((param_2 == 0) && (*(char *)(iVar5 + 0x298) != '\0')) {
        if ((*(char *)(iVar5 + 0x299) == '\0') || (*(char *)(iVar5 + 0x29b) == '\0')) {
          if ((DAT_00078e09 != '\0') &&
             (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
            local_1020._0_1_ = 'W';
            local_1020._1_1_ = 'o';
            local_1020._2_1_ = 'r';
            local_1020._3_1_ = 'k';
            uStack_101c._0_1_ = ' ';
            uStack_101c._1_1_ = 's';
            uStack_101c._2_1_ = 't';
            uStack_101c._3_1_ = 'a';
            uStack_1018._0_1_ = 'l';
            uStack_1018._1_1_ = 'e';
            uStack_1018._2_1_ = ' ';
            uStack_1018._3_1_ = 'd';
            uStack_1014._0_1_ = 'u';
            uStack_1014._1_1_ = 'e';
            uStack_1014._2_1_ = ' ';
            uStack_1014._3_1_ = 't';
            local_1010._0_1_ = 'o';
            local_1010._1_1_ = ' ';
            local_1010._2_1_ = 's';
            local_1010._3_1_ = 't';
            uStack_100c._0_1_ = 'r';
            uStack_100c._1_1_ = 'a';
            uStack_100c._2_1_ = 't';
            uStack_100c._3_1_ = 'u';
            local_1008._0_1_ = 'm';
            local_1008._1_1_ = ' ';
            local_1008._2_1_ = 'i';
            local_1008._3_1_ = 'n';
            uStack_1004._0_1_ = 'a';
            uStack_1004._1_1_ = 'c';
            uStack_1004._2_1_ = 't';
            uStack_1004._3_1_ = 'i';
            local_1000 = CONCAT13(local_1000._3_1_,0x6576);
            FUN_00038730(7,&local_1020,0);
            return 1;
          }
        }
        else {
          iVar2 = pthread_mutex_lock((pthread_mutex_t *)(iVar5 + 0xf4));
          if (iVar2 != 0) {
            FUN_0000b414("stale_work",0x1347);
          }
          iVar2 = pthread_rwlock_rdlock((pthread_rwlock_t *)(iVar5 + 0x10c));
          if (iVar2 != 0) {
            FUN_0000b69c("stale_work",0x1347);
          }
          iVar2 = pthread_mutex_unlock((pthread_mutex_t *)(iVar5 + 0xf4));
          if (iVar2 != 0) {
            FUN_0000b5c4("stale_work",0x1347);
          }
          iVar2 = strcmp(*(char **)(param_1 + 0x6f0),*(char **)(iVar5 + 0x2a0));
          iVar3 = pthread_rwlock_unlock((pthread_rwlock_t *)(iVar5 + 0x10c));
          if (iVar3 != 0) {
            FUN_0000b630("stale_work",0x134a);
          }
          (*DAT_00075ed8)();
          if (iVar2 == 0) goto LAB_00022cd8;
          if ((DAT_00078e09 != '\0') &&
             (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
            pcVar6 = "Work stale due to stratum job_id mismatch";
            goto LAB_00022e24;
          }
        }
      }
      else {
LAB_00022cd8:
        iVar2 = VectorFloatToSigned(*(double *)(iVar5 + 0x1d8) * 5.0 + 1.0,3);
        iVar4 = iVar4 - iVar2;
        FUN_0002a84c(aiStack_1028);
        if (iVar4 < 5) {
          iVar4 = 5;
        }
        if (aiStack_1028[0] - *(int *)(param_1 + 0x6d8) < iVar4) {
          if ((((DAT_0007b625 == '\0') || (param_2 != 0)) ||
              (iVar4 = FUN_0001da58(), iVar5 == iVar4)) ||
             (((*(char *)(param_1 + 0x6ea) != '\0' || (DAT_00076efc == 3)) || (DAT_00076efc == 4))))
          goto LAB_00022b74;
          if ((DAT_00078e09 != '\0') &&
             (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
            pcVar6 = "Work stale due to fail only pool mismatch";
LAB_00022e24:
            local_1020 = *(undefined4 *)pcVar6;
            uStack_101c = *(undefined4 *)(pcVar6 + 4);
            uStack_1018 = *(undefined4 *)(pcVar6 + 8);
            uStack_1014 = *(undefined4 *)(pcVar6 + 0xc);
            local_1010 = *(undefined4 *)(pcVar6 + 0x10);
            uStack_100c = *(undefined4 *)(pcVar6 + 0x14);
            local_1008 = *(uint *)(pcVar6 + 0x18);
            uStack_1004 = *(undefined4 *)(pcVar6 + 0x1c);
            local_1000 = *(uint *)(pcVar6 + 0x20);
            uStack_ffc = *(undefined4 *)(pcVar6 + 0x24);
            local_ff8 = (undefined2)*(undefined4 *)(pcVar6 + 0x28);
            FUN_00038730(7,&local_1020,0);
            return 1;
          }
        }
        else if ((DAT_00078e09 != '\0') &&
                (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
          local_1020._0_1_ = 'W';
          local_1020._1_1_ = 'o';
          local_1020._2_1_ = 'r';
          local_1020._3_1_ = 'k';
          uStack_101c._0_1_ = ' ';
          uStack_101c._1_1_ = 's';
          uStack_101c._2_1_ = 't';
          uStack_101c._3_1_ = 'a';
          uStack_1018._0_1_ = 'l';
          uStack_1018._1_1_ = 'e';
          uStack_1018._2_1_ = ' ';
          uStack_1018._3_1_ = 'd';
          uStack_1014._0_1_ = 'u';
          uStack_1014._1_1_ = 'e';
          uStack_1014._2_1_ = ' ';
          uStack_1014._3_1_ = 't';
          local_1010._0_1_ = 'o';
          local_1010._1_1_ = ' ';
          local_1010._2_1_ = 'e';
          local_1010._3_1_ = 'x';
          uStack_100c._0_1_ = 'p';
          uStack_100c._1_1_ = 'i';
          uStack_100c._2_1_ = 'r';
          uStack_100c._3_1_ = 'y';
          local_1008 = local_1008 & 0xffffff00;
          FUN_00038730(7,&local_1020,0);
          return 1;
        }
      }
    }
    else if ((DAT_00078e09 != '\0') &&
            (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      local_1020._0_1_ = 'W';
      local_1020._1_1_ = 'o';
      local_1020._2_1_ = 'r';
      local_1020._3_1_ = 'k';
      uStack_101c._0_1_ = ' ';
      uStack_101c._1_1_ = 's';
      uStack_101c._2_1_ = 't';
      uStack_101c._3_1_ = 'a';
      uStack_1018._0_1_ = 'l';
      uStack_1018._1_1_ = 'e';
      uStack_1018._2_1_ = ' ';
      uStack_1018._3_1_ = 'd';
      uStack_1014._0_1_ = 'u';
      uStack_1014._1_1_ = 'e';
      uStack_1014._2_1_ = ' ';
      uStack_1014._3_1_ = 't';
      local_1010._0_1_ = 'o';
      local_1010._1_1_ = ' ';
      local_1010._2_1_ = 'b';
      local_1010._3_1_ = 'l';
      uStack_100c._0_1_ = 'o';
      uStack_100c._1_1_ = 'c';
      uStack_100c._2_1_ = 'k';
      uStack_100c._3_1_ = ' ';
      local_1008._0_1_ = 'm';
      local_1008._1_1_ = 'i';
      local_1008._2_1_ = 's';
      local_1008._3_1_ = 'm';
      uStack_1004._0_1_ = 'a';
      uStack_1004._1_1_ = 't';
      uStack_1004._2_1_ = 'c';
      uStack_1004._3_1_ = 'h';
      local_1000 = local_1000 & 0xffffff00;
      FUN_00038730(7,&local_1020,0);
      return 1;
    }
    uVar1 = 1;
  }
  else {
LAB_00022b74:
    uVar1 = 0;
  }
  return uVar1;
}

