//==================== FUN_00020088 @ 00020088 ====================

/* WARNING: Restarted to delay deadcode elimination for space: ram */

undefined4 FUN_00020088(int *param_1)

{
  char cVar1;
  byte bVar2;
  pthread_t __th;
  int iVar3;
  char *param3;
  int *piVar4;
  int *piVar5;
  uint uVar6;
  time_t tVar7;
  int iVar8;
  char *pcVar9;
  size_t __n;
  uint uVar10;
  int iVar11;
  uint uVar12;
  uint uVar13;
  int iVar14;
  int *piVar15;
  undefined4 uVar16;
  undefined4 *puVar17;
  void *__ptr;
  double *pdVar18;
  void *__ptr_00;
  bool bVar19;
  uint in_fpscr;
  double dVar20;
  ulonglong param5;
  double dVar21;
  int *local_1274;
  uint uStack_1268;
  timeval tStack_1264;
  char acStack_125c [16];
  char acStack_124c [20];
  char acStack_1238 [32];
  char local_1218 [4];
  undefined2 local_1214;
  undefined1 local_1212;
  undefined1 auStack_1211 [29];
  char acStack_11f4 [60];
  undefined4 uStack_11b8;
  fd_set local_11b4;
  int aiStack_1134 [23];
  char acStack_10d8 [160];
  uint local_1038 [4];
  uint local_1028;
  uint local_1024;
  uint local_1020;
  uint local_101c;
  char local_1018 [4];
  undefined1 local_1014;
  
  __th = pthread_self();
  pthread_detach(__th);
  snprintf(acStack_125c,0x10,"%d/RStratum",*param_1);
  FUN_0002f0b4(acStack_125c);
  do {
    while( true ) {
      if (*(char *)((int)param_1 + 0x89) != '\0') {
        FUN_0002aee0(param_1);
        return 0;
      }
      iVar3 = FUN_0002ae10();
      if ((iVar3 == 0) && (iVar3 = FUN_0001ffec(param_1), iVar3 == 0)) {
        FUN_0002aee0(param_1);
        FUN_000182f4(param_1);
        FUN_000185c8(param_1);
        while ((iVar3 = FUN_0001ffec(param_1), iVar3 == 0 &&
               ((param_1[0x21] == 0 ||
                (((piVar5 = (int *)FUN_0001da58(), param_1 != piVar5 && (DAT_00076efc != 3)) &&
                 (DAT_00076efc != 4))))))) {
          iVar3 = pthread_mutex_lock((pthread_mutex_t *)&DAT_00077060);
          if (iVar3 != 0) {
            FUN_0000b414("wait_lpcurrent",0x2420);
          }
          pthread_cond_wait((pthread_cond_t *)&DAT_00077078,(pthread_mutex_t *)&DAT_00077060);
          iVar3 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_00077060);
          if (iVar3 != 0) {
            FUN_0000b5c4("wait_lpcurrent",0x2422);
          }
          (*DAT_00075ed8)();
        }
        while( true ) {
          iVar3 = FUN_0002cec8(param_1);
          uVar16 = 30000;
          if (iVar3 != 0) break;
          if (*(char *)((int)param_1 + 0x89) != '\0') {
            return 0;
          }
          if (DAT_0007b5e4 < 2) {
            uVar16 = 3000;
          }
          FUN_0002a92c(uVar16);
        }
      }
      puVar17 = &uStack_11b8;
      do {
        puVar17 = puVar17 + 1;
        *puVar17 = 0;
      } while (puVar17 != local_11b4.fds_bits + 0x1f);
      iVar11 = param_1[0x99];
      tStack_1264.tv_usec = 0;
      iVar3 = iVar11 + 0x1f;
      if (-1 < iVar11) {
        iVar3 = iVar11;
      }
      tStack_1264.tv_sec = 0x5a;
      local_11b4.fds_bits[iVar3 >> 5] =
           local_11b4.fds_bits[iVar3 >> 5] | 1 << (iVar11 % 0x20 & 0xffU);
      iVar3 = FUN_0002ae10(param_1);
      if ((iVar3 != 0) ||
         (iVar3 = select(param_1[0x99] + 1,&local_11b4,(fd_set *)0x0,(fd_set *)0x0,&tStack_1264),
         0 < iVar3)) break;
      if (DAT_00078e09 != '\0') {
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)) {
          snprintf((char *)local_1038,0x1000,"Stratum select failed on pool %d with value %d",
                   *param_1,iVar3);
          FUN_00038730(7,local_1038,0);
          goto LAB_00020c58;
        }
LAB_00020c64:
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)) {
          snprintf((char *)local_1038,0x1000,"Stratum connection to pool %d interrupted",*param_1);
          FUN_00038730(7,local_1038,0);
        }
      }
LAB_00020cb8:
      param_1[0x28] = param_1[0x28] + 1;
      DAT_0007b43c = DAT_0007b43c + 1;
      iVar3 = pthread_mutex_lock((pthread_mutex_t *)(param_1 + 0x3d));
      if (iVar3 != 0) {
        FUN_0000b414("supports_resume",0x1bbd);
      }
      iVar3 = pthread_rwlock_rdlock((pthread_rwlock_t *)(param_1 + 0x43));
      if (iVar3 != 0) {
        FUN_0000b69c("supports_resume",0x1bbd);
      }
      iVar3 = pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0x3d));
      if (iVar3 != 0) {
        FUN_0000b5c4("supports_resume",0x1bbd);
      }
      iVar11 = param_1[0xa5];
      iVar3 = pthread_rwlock_unlock((pthread_rwlock_t *)(param_1 + 0x43));
      if (iVar3 != 0) {
        FUN_0000b630("supports_resume",0x1bbf);
      }
      (*DAT_00075ed8)();
      if ((iVar11 == 0) || (DAT_0007b644 != '\0')) {
        FUN_000182f4(param_1);
      }
      FUN_000185c8(param_1);
      piVar5 = (int *)FUN_0001da58();
      if (param_1 == piVar5) {
        FUN_00017210();
      }
      while (iVar3 = FUN_0002cec8(param_1), iVar3 == 0) {
        if (*(char *)((int)param_1 + 0x89) != '\0') {
          return 0;
        }
        FUN_0002a92c(30000);
      }
    }
    param3 = (char *)FUN_0002b040(param_1);
    if (param3 == (char *)0x0) {
LAB_00020c58:
      if (DAT_00078e09 != '\0') goto LAB_00020c64;
      goto LAB_00020cb8;
    }
    FUN_0001fd64(param_1);
    iVar3 = FUN_0002cf40(param_1,param3);
    if (iVar3 == 0) {
      iVar3 = FUN_00059230(param3,0,aiStack_1134);
      if (iVar3 == 0) {
        if (DAT_00078e09 != '\0') {
          if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)) {
            snprintf((char *)local_1038,0x1000,"JSON decode failed(%d): %s",aiStack_1134[0],
                     acStack_10d8);
            FUN_00038730(7,local_1038,0);
            goto LAB_00020ed0;
          }
LAB_00020edc:
          if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)) {
            snprintf((char *)local_1038,0x1000,"Unknown stratum msg: %s",param3);
            FUN_00038730(7,local_1038,0);
          }
        }
      }
      else {
        local_1274 = (int *)FUN_00059e20(iVar3,"result");
        piVar4 = (int *)FUN_00059e20(iVar3,"error");
        piVar5 = (int *)FUN_00059e20(iVar3,&DAT_00061ca0);
        if ((piVar5 == (int *)0x0) || (*piVar5 == 7)) {
          if (piVar4 == (int *)0x0) {
            pcVar9 = malloc(0x11);
            if (pcVar9 != (char *)0x0) {
              builtin_strncpy(pcVar9,"(unknown reason)",0x11);
            }
          }
          else {
            pcVar9 = (char *)FUN_00057854(piVar4,3);
          }
          if ((DAT_00078e09 != '\0') &&
             (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
            snprintf((char *)local_1038,0x1000,"JSON-RPC non method decode failed: %s",pcVar9);
            FUN_00038730(7,local_1038,0);
          }
          bVar19 = false;
          free(pcVar9);
        }
        else {
          uStack_1268 = FUN_0005a248();
          iVar11 = pthread_mutex_lock((pthread_mutex_t *)&DAT_00077030);
          if (iVar11 != 0) {
            FUN_0000b414("parse_stratum_response",0x1b09);
          }
          piVar5 = DAT_00077048;
          if (DAT_00077048 == (int *)0x0) {
            __ptr_00 = (void *)0x0;
          }
          else {
            puVar17 = (undefined4 *)*DAT_00077048;
            uVar12 = (uStack_1268 & 0xff) + 0x112410d + (uStack_1268 & 0xff000000) +
                     (uStack_1268 >> 0x10 & 0xff) * 0x10000 + (uStack_1268 >> 8 & 0xff) * 0x100 ^
                     0x7f76d;
            uVar10 = 0x9f49bac6 - uVar12 ^ uVar12 << 8;
            uVar6 = (-0x112410d - uVar12) - uVar10 ^ uVar10 >> 0xd;
            __ptr = (void *)*puVar17;
            uVar12 = (uVar12 - uVar10) - uVar6 ^ uVar6 >> 0xc;
            uVar10 = (uVar10 - uVar6) - uVar12 ^ uVar12 << 0x10;
            uVar13 = (uVar6 - uVar12) - uVar10 ^ uVar10 >> 5;
            uVar6 = (uVar12 - uVar10) - uVar13 ^ uVar13 >> 3;
            uVar12 = (uVar10 - uVar13) - uVar6 ^ uVar6 << 10;
            iVar11 = *(int *)((int)__ptr +
                             (((uVar13 - uVar6) - uVar12 ^ uVar12 >> 0xf) & puVar17[1] - 1) * 0xc);
            if (iVar11 == 0) {
              __ptr_00 = (void *)0x0;
            }
            else {
              __ptr_00 = (void *)(iVar11 - puVar17[5]);
            }
            for (; __ptr_00 != (void *)0x0;
                __ptr_00 = (void *)(*(int *)((int)__ptr_00 + 0x10) - puVar17[5])) {
              if ((*(int *)((int)__ptr_00 + 0x18) == 4) &&
                 (iVar11 = memcmp(*(void **)((int)__ptr_00 + 0x14),&uStack_1268,4), iVar11 == 0)) {
                iVar11 = *(int *)((int)__ptr_00 + 4);
                if ((iVar11 == 0) && (*(int *)((int)__ptr_00 + 8) == 0)) {
                  free(__ptr);
                  free((void *)*piVar5);
                  DAT_00077048 = (int *)0x0;
                }
                else {
                  if (__ptr_00 == (void *)(puVar17[4] - puVar17[5])) {
                    puVar17[4] = iVar11 + puVar17[5];
                    iVar11 = *(int *)((int)__ptr_00 + 4);
                  }
                  if (iVar11 == 0) {
                    piVar15 = *(int **)((int)__ptr_00 + 8);
                    piVar5 = piVar15;
                    DAT_00077048 = piVar15;
                  }
                  else {
                    *(undefined4 *)(iVar11 + *(int *)(*piVar5 + 0x14) + 8) =
                         *(undefined4 *)((int)__ptr_00 + 8);
                    piVar15 = *(int **)((int)__ptr_00 + 8);
                  }
                  if (piVar15 != (int *)0x0) {
                    *(undefined4 *)((int)piVar15 + *(int *)(*piVar5 + 0x14) + 4) =
                         *(undefined4 *)((int)__ptr_00 + 4);
                  }
                  uVar6 = ((int *)*piVar5)[1] - 1U & *(uint *)((int)__ptr_00 + 0x1c);
                  iVar11 = *(int *)*piVar5 + uVar6 * 0xc;
                  *(int *)(iVar11 + 4) = *(int *)(iVar11 + 4) + -1;
                  if (__ptr_00 == *(void **)(*(int *)*piVar5 + uVar6 * 0xc)) {
                    *(undefined4 *)(*(int *)*piVar5 + uVar6 * 0xc) =
                         *(undefined4 *)((int)__ptr_00 + 0x10);
                  }
                  if (*(int *)((int)__ptr_00 + 0xc) != 0) {
                    *(undefined4 *)(*(int *)((int)__ptr_00 + 0xc) + 0x10) =
                         *(undefined4 *)((int)__ptr_00 + 0x10);
                  }
                  if (*(int *)((int)__ptr_00 + 0x10) != 0) {
                    *(undefined4 *)(*(int *)((int)__ptr_00 + 0x10) + 0xc) =
                         *(undefined4 *)((int)__ptr_00 + 0xc);
                  }
                  *(int *)(*piVar5 + 0xc) = *(int *)(*piVar5 + 0xc) + -1;
                }
                param_1[0xbd] = param_1[0xbd] + -1;
                break;
              }
              if (*(int *)((int)__ptr_00 + 0x10) == 0) {
                __ptr_00 = (void *)0x0;
                break;
              }
            }
          }
          iVar11 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_00077030);
          if (iVar11 != 0) {
            FUN_0000b5c4("parse_stratum_response",0x1b10);
          }
          (*DAT_00075ed8)();
          if (__ptr_00 == (void *)0x0) {
            if (local_1274 == (int *)0x0) {
              bVar19 = false;
            }
            else {
              iVar11 = pthread_mutex_lock((pthread_mutex_t *)(param_1 + 0x3d));
              if (iVar11 != 0) {
                FUN_0000b414("parse_stratum_response",0x1b1a);
              }
              iVar11 = pthread_rwlock_rdlock((pthread_rwlock_t *)(param_1 + 0x43));
              if (iVar11 != 0) {
                FUN_0000b69c("parse_stratum_response",0x1b1a);
              }
              iVar11 = pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0x3d));
              if (iVar11 != 0) {
                FUN_0000b5c4("parse_stratum_response",0x1b1a);
              }
              dVar20 = *(double *)(param_1 + 0x1cc);
              iVar11 = pthread_rwlock_unlock((pthread_rwlock_t *)(param_1 + 0x43));
              if (iVar11 != 0) {
                FUN_0000b630("parse_stratum_response",0x1b1c);
              }
              (*DAT_00075ed8)();
              if (*local_1274 == 5) {
                if ((DAT_00078e09 != '\0') &&
                   (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
                  snprintf((char *)local_1038,0x1000,"Accepted untracked stratum share from pool %d"
                           ,*param_1);
                  FUN_00038730(7,local_1038,0);
                }
                iVar11 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007ad4c);
                if (iVar11 != 0) {
                  FUN_0000b414("parse_stratum_response",0x1b24);
                }
                uVar6 = param_1[2];
                param_1[2] = uVar6 + 1;
                param_1[3] = param_1[3] + (uint)(0xfffffffe < uVar6);
                bVar19 = 0xfffffffe < DAT_0007b420;
                DAT_0007b420 = DAT_0007b420 + 1;
                DAT_0007b424 = DAT_0007b424 + (uint)bVar19;
                DAT_0007b5e8 = dVar20 + DAT_0007b5e8;
                *(double *)(param_1 + 0x1a) = *(double *)(param_1 + 0x1a) + dVar20;
                iVar11 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007ad4c);
                if (iVar11 != 0) {
                  FUN_0000b5c4("parse_stratum_response",0x1b29);
                }
              }
              else {
                if ((DAT_00078e09 != '\0') &&
                   (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
                  snprintf((char *)local_1038,0x1000,"Rejected untracked stratum share from pool %d"
                           ,*param_1);
                  FUN_00038730(7,local_1038,0);
                }
                iVar11 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007ad4c);
                if (iVar11 != 0) {
                  FUN_0000b414("parse_stratum_response",0x1b2f);
                }
                uVar6 = param_1[4];
                param_1[4] = uVar6 + 1;
                param_1[5] = param_1[5] + (uint)(0xfffffffe < uVar6);
                bVar19 = 0xfffffffe < DAT_0007b6f8;
                DAT_0007b6f8 = DAT_0007b6f8 + 1;
                DAT_0007b6fc = DAT_0007b6fc + (uint)bVar19;
                DAT_0007b670 = dVar20 + DAT_0007b670;
                *(double *)(param_1 + 0x1c) = *(double *)(param_1 + 0x1c) + dVar20;
                iVar11 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007ad4c);
                if (iVar11 != 0) {
                  FUN_0000b5c4("parse_stratum_response",0x1b34);
                }
              }
              bVar19 = false;
              (*DAT_00075ed8)();
            }
          }
          else {
            iVar11 = *(int *)((int)__ptr_00 + 0x24);
            tVar7 = time((time_t *)0x0);
            if ((DAT_00078e09 != '\0') &&
               (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
              snprintf((char *)local_1038,0x1000,"Pool %d stratum share result lag time %d seconds",
                       **(int **)(iVar11 + 0x6d4),tVar7 - *(int *)((int)__ptr_00 + 0x30));
              FUN_00038730(7,local_1038,0);
            }
            uVar6 = *(uint *)(iVar11 + 0xdc);
            iVar14 = 0;
            local_1038[0] =
                 uVar6 << 0x18 | (uVar6 >> 8 & 0xff) << 0x10 | (uVar6 >> 0x10 & 0xff) << 8 |
                 uVar6 >> 0x18;
            uVar6 = *(uint *)(iVar11 + 0xd8);
            local_1038[1] =
                 uVar6 << 0x18 | (uVar6 >> 8 & 0xff) << 0x10 | (uVar6 >> 0x10 & 0xff) << 8 |
                 uVar6 >> 0x18;
            uVar6 = *(uint *)(iVar11 + 0xd4);
            local_1038[2] =
                 uVar6 << 0x18 | (uVar6 >> 8 & 0xff) << 0x10 | (uVar6 >> 0x10 & 0xff) << 8 |
                 uVar6 >> 0x18;
            uVar6 = *(uint *)(iVar11 + 0xd0);
            local_1038[3] =
                 uVar6 << 0x18 | (uVar6 >> 8 & 0xff) << 0x10 | (uVar6 >> 0x10 & 0xff) << 8 |
                 uVar6 >> 0x18;
            uVar6 = *(uint *)(iVar11 + 0xcc);
            local_1028 = uVar6 << 0x18 | (uVar6 >> 8 & 0xff) << 0x10 | (uVar6 >> 0x10 & 0xff) << 8 |
                         uVar6 >> 0x18;
            uVar6 = *(uint *)(iVar11 + 200);
            local_1024 = uVar6 << 0x18 | (uVar6 >> 8 & 0xff) << 0x10 | (uVar6 >> 0x10 & 0xff) << 8 |
                         uVar6 >> 0x18;
            uVar6 = *(uint *)(iVar11 + 0xc4);
            local_1020 = uVar6 << 0x18 | (uVar6 >> 8 & 0xff) << 0x10 | (uVar6 >> 0x10 & 0xff) << 8 |
                         uVar6 >> 0x18;
            uVar6 = *(uint *)(iVar11 + 0xc0);
            local_101c = uVar6 << 0x18 | (uVar6 >> 8 & 0xff) << 0x10 | (uVar6 >> 0x10 & 0xff) << 8 |
                         uVar6 >> 0x18;
            do {
              if (*(char *)((int)local_1038 + iVar14) != '\0') break;
              iVar14 = iVar14 + 1;
            } while (iVar14 != 0x1d);
            pdVar18 = (double *)(iVar11 + 0x748);
            uVar6 = *(uint *)((int)local_1038 + iVar14);
            dVar20 = round(*pdVar18);
            param5 = FUN_0005cc48(SUB84(dVar20,0),(int)((ulonglong)dVar20 >> 0x20));
            FUN_00011580(*(undefined4 *)(iVar11 + 0x6b8),*(undefined4 *)(iVar11 + 0x6bc),local_1218,
                         0x10,0);
            pcVar9 = "";
            if (*(char *)(iVar11 + 0x6eb) != '\0') {
              pcVar9 = " BLOCK!";
            }
            snprintf(acStack_11f4,0x40,"%08lx Diff %s/%llu%s",
                     uVar6 << 0x18 | (uVar6 >> 8 & 0xff) << 0x10 | (uVar6 >> 0x10 & 0xff) << 8 |
                     uVar6 >> 0x18,local_1218,param5,pcVar9);
            piVar5 = *(int **)(iVar11 + 0x6d4);
            iVar14 = FUN_0001b124(*(undefined4 *)(iVar11 + 0x6d0));
            iVar14 = *(int *)(iVar14 + 0x24);
            if ((local_1274 == (int *)0x0) ||
               ((*local_1274 != 5 && ((*(char *)(iVar11 + 0x714) == '\0' || (*local_1274 != 7))))))
            {
              iVar8 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007ad4c);
              if (iVar8 != 0) {
                FUN_0000b414("share_result",0xdcb);
              }
              *(int *)(iVar14 + 0x28) = *(int *)(iVar14 + 0x28) + 1;
              uVar6 = piVar5[4];
              piVar5[4] = uVar6 + 1;
              piVar5[5] = piVar5[5] + (uint)(0xfffffffe < uVar6);
              bVar19 = 0xfffffffe < DAT_0007b6f8;
              DAT_0007b6f8 = DAT_0007b6f8 + 1;
              DAT_0007b6fc = DAT_0007b6fc + (uint)bVar19;
              *(double *)(iVar14 + 0xd0) = *(double *)(iVar14 + 0xd0) + *pdVar18;
              dVar20 = *pdVar18;
              piVar5[6] = piVar5[6] + 1;
              DAT_0007b670 = dVar20 + DAT_0007b670;
              *(double *)(piVar5 + 0x1c) = *(double *)(piVar5 + 0x1c) + dVar20;
              iVar8 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007ad4c);
              if (iVar8 != 0) {
                FUN_0000b5c4("share_result",0xdd3);
              }
              (*DAT_00075ed8)();
              if ((DAT_00078e09 != '\0') &&
                 (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
                local_1038[0]._0_1_ = 'P';
                local_1038[0]._1_1_ = 'R';
                local_1038[0]._2_1_ = 'O';
                local_1038[0]._3_1_ = 'O';
                local_1038[1]._0_1_ = 'F';
                local_1038[1]._1_1_ = ' ';
                local_1038[1]._2_1_ = 'O';
                local_1038[1]._3_1_ = 'F';
                local_1038[2]._0_1_ = ' ';
                local_1038[2]._1_1_ = 'W';
                local_1038[2]._2_1_ = 'O';
                local_1038[2]._3_1_ = 'R';
                local_1038[3]._0_1_ = 'K';
                local_1038[3]._1_1_ = ' ';
                local_1038[3]._2_1_ = 'R';
                local_1038[3]._3_1_ = 'E';
                local_1028._0_1_ = 'S';
                local_1028._1_1_ = 'U';
                local_1028._2_1_ = 'L';
                local_1028._3_1_ = 'T';
                local_1024._0_1_ = ':';
                local_1024._1_1_ = ' ';
                local_1024._2_1_ = 'f';
                local_1024._3_1_ = 'a';
                local_1020._0_1_ = 'l';
                local_1020._1_1_ = 's';
                local_1020._2_1_ = 'e';
                local_1020._3_1_ = ' ';
                local_101c._0_1_ = '(';
                local_101c._1_1_ = 'b';
                local_101c._2_1_ = 'o';
                local_101c._3_1_ = 'o';
                local_1018[0] = 'o';
                local_1018[1] = 'o';
                local_1018[2] = 'o';
                local_1018[3] = ')';
                local_1014 = 0;
                FUN_00038730(7,local_1038,0);
              }
              if ((DAT_0007bc78 == '\0') && (cVar1 = DAT_0007bb18, DAT_0007bb18 == '\0')) {
                local_1218[0] = 'r';
                local_1218[1] = 'e';
                local_1218[2] = 'j';
                local_1218[3] = 'e';
                local_1212 = 0;
                local_1214 = 0x7463;
                memset(auStack_1211,0,0x1d);
                acStack_1238[0] = cVar1;
                if (DAT_0007b198 < 2) {
                  acStack_124c[0] = '\0';
                }
                else {
                  snprintf(acStack_124c,0x14,"pool %d",**(int **)(iVar11 + 0x6d4));
                }
                if (*(char *)(iVar11 + 0x714) == '\0') {
                  local_1274 = (int *)FUN_00059e20(iVar3,"reject-reason");
                }
                if (local_1274 == (int *)0x0) {
                  if ((*(char *)(iVar11 + 0x6ec) != '\0') && (piVar4 != (int *)0x0)) {
                    if (*piVar4 == 1) {
                      piVar4 = (int *)FUN_0005a008(piVar4);
                      if ((piVar4 != (int *)0x0) && (*piVar4 == 2)) goto LAB_000214ac;
                    }
                    else if (*piVar4 == 2) {
LAB_000214ac:
                      pcVar9 = (char *)FUN_0005a178(piVar4);
                      snprintf(acStack_1238,0x1f," (%s)",pcVar9);
                    }
                  }
                }
                else {
                  pcVar9 = (char *)FUN_0005a178(local_1274);
                  __n = strlen(pcVar9);
                  acStack_1238[0] = ' ';
                  acStack_1238[1] = 0x28;
                  if (0x1b < __n) {
                    __n = 0x1c;
                  }
                  memcpy(acStack_1238 + 2,pcVar9,__n);
                  acStack_1238[__n + 3] = '\0';
                  acStack_1238[__n + 2] = ')';
                  memcpy(auStack_1211,pcVar9,__n);
                  local_1212 = 0x3a;
                  auStack_1211[__n] = 0;
                }
                if ((DAT_00078e09 != '\0') &&
                   (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
                  snprintf((char *)local_1038,0x1000,"Rejected %s %s %d %s%s %s%s",acStack_11f4,
                           *(char **)(*(int *)(iVar14 + 4) + 8),*(int *)(iVar14 + 8),acStack_124c,
                           acStack_1238,"","");
                  FUN_00038730(7,local_1038,0);
                }
                FUN_0001b600(local_1218,iVar11);
              }
              iVar14 = piVar5[6];
              if (((10 < iVar14) && (*(char *)(iVar11 + 0x6e9) == '\0')) &&
                 ((DAT_0007b250 != '\0' && (1 < DAT_0007b5e4)))) {
                dVar20 = (double)VectorSignedToFloat(iVar14,(byte)(in_fpscr >> 0x16) & 3);
                dVar21 = (double)FUN_0005cb64(DAT_0007b420,DAT_0007b424);
                dVar21 = (dVar21 / DAT_000755f8) * 60.0 * 3.0;
                uVar6 = in_fpscr & 0xfffffff | (uint)(dVar20 < dVar21) << 0x1f |
                        (uint)(dVar20 == dVar21) << 0x1e;
                in_fpscr = uVar6 | (uint)(NAN(dVar20) || NAN(dVar21)) << 0x1c;
                bVar2 = (byte)(uVar6 >> 0x18);
                if (!(bool)(bVar2 >> 6 & 1) && bVar2 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
                  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
                    snprintf((char *)local_1038,0x1000,
                             "Pool %d rejected %d sequential shares, disabling!",*piVar5,iVar14);
                    FUN_00038730(4,local_1038,0);
                  }
                  if (piVar5[0x21] == 1) {
                    DAT_0007b5e4 = DAT_0007b5e4 + -1;
                  }
                  piVar5[0x21] = 2;
                  piVar4 = (int *)FUN_0001da58();
                  if (piVar5 == piVar4) {
                    FUN_00019bf8(0);
                  }
                  piVar5[6] = 0;
                }
              }
            }
            else {
              iVar8 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007ad4c);
              if (iVar8 != 0) {
                FUN_0000b414("share_result",0xd99);
              }
              *(int *)(iVar14 + 0x24) = *(int *)(iVar14 + 0x24) + 1;
              uVar6 = piVar5[2];
              piVar5[2] = uVar6 + 1;
              piVar5[3] = piVar5[3] + (uint)(0xfffffffe < uVar6);
              bVar19 = 0xfffffffe < DAT_0007b420;
              DAT_0007b420 = DAT_0007b420 + 1;
              DAT_0007b424 = DAT_0007b424 + (uint)bVar19;
              *(double *)(iVar14 + 200) = *(double *)(iVar14 + 200) + *pdVar18;
              DAT_0007b5e8 = *pdVar18 + DAT_0007b5e8;
              *(double *)(piVar5 + 0x1a) = *(double *)(piVar5 + 0x1a) + *pdVar18;
              iVar8 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007ad4c);
              if (iVar8 != 0) {
                FUN_0000b5c4("share_result",0xda0);
              }
              (*DAT_00075ed8)();
              piVar5[6] = 0;
              *(int *)(iVar14 + 0xd8) = *piVar5;
              tVar7 = time((time_t *)0x0);
              bVar19 = DAT_00078e09 != '\0';
              *(time_t *)(iVar14 + 0xdc) = tVar7;
              uVar16 = *(undefined4 *)(iVar11 + 0x74c);
              *(int *)(iVar14 + 0xe0) = *(int *)pdVar18;
              *(undefined4 *)(iVar14 + 0xe4) = uVar16;
              piVar5[0x60] = tVar7;
              iVar8 = *(int *)(iVar11 + 0x74c);
              piVar5[0x62] = *(int *)pdVar18;
              piVar5[99] = iVar8;
              if ((bVar19) &&
                 (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
                local_1038[0]._0_1_ = 'P';
                local_1038[0]._1_1_ = 'R';
                local_1038[0]._2_1_ = 'O';
                local_1038[0]._3_1_ = 'O';
                local_1038[1]._0_1_ = 'F';
                local_1038[1]._1_1_ = ' ';
                local_1038[1]._2_1_ = 'O';
                local_1038[1]._3_1_ = 'F';
                local_1038[2]._0_1_ = ' ';
                local_1038[2]._1_1_ = 'W';
                local_1038[2]._2_1_ = 'O';
                local_1038[2]._3_1_ = 'R';
                local_1038[3]._0_1_ = 'K';
                local_1038[3]._1_1_ = ' ';
                local_1038[3]._2_1_ = 'R';
                local_1038[3]._3_1_ = 'E';
                local_1028._0_1_ = 'S';
                local_1028._1_1_ = 'U';
                local_1028._2_1_ = 'L';
                local_1028._3_1_ = 'T';
                local_1024._0_1_ = ':';
                local_1024._1_1_ = ' ';
                local_1024._2_1_ = 't';
                local_1024._3_1_ = 'r';
                local_1020._0_1_ = 'u';
                local_1020._1_1_ = 'e';
                local_1020._2_1_ = ' ';
                local_1020._3_1_ = '(';
                local_101c._0_1_ = 'y';
                local_101c._1_1_ = 'a';
                local_101c._2_1_ = 'y';
                local_101c._3_1_ = '!';
                local_1018[0] = '!';
                local_1018[1] = '!';
                local_1018[2] = ')';
                local_1018[3] = '\0';
                FUN_00038730(7,local_1038,0);
              }
              if ((DAT_0007bc78 == '\0') && (DAT_0007bb18 == '\0')) {
                if (DAT_0007b198 < 2) {
                  if ((DAT_00078e09 != '\0') &&
                     (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
                    snprintf((char *)local_1038,0x1000,"Accepted %s %s %d %s%s",acStack_11f4,
                             *(char **)(*(int *)(iVar14 + 4) + 8),*(int *)(iVar14 + 8),"","");
                    FUN_00038730(7,local_1038,0);
                  }
                }
                else if ((DAT_00078e09 != '\0') &&
                        (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4))))
                {
                  snprintf((char *)local_1038,0x1000,"Accepted %s %s %d pool %d %s%s",acStack_11f4,
                           *(char **)(*(int *)(iVar14 + 4) + 8),*(int *)(iVar14 + 8),
                           **(int **)(iVar11 + 0x6d4),"","");
                  FUN_00038730(7,local_1038,0);
                }
              }
              FUN_0001b600("accept",iVar11);
              if (DAT_00077288 != 0) {
                dVar20 = (double)VectorSignedToFloat(DAT_00077288,(byte)(in_fpscr >> 0x16) & 3);
                uVar6 = in_fpscr & 0xfffffff | (uint)(dVar20 == DAT_0007b5e8) << 0x1e;
                in_fpscr = uVar6 | (uint)(DAT_0007b5e8 <= dVar20) << 0x1d;
                if (!(bool)((byte)(in_fpscr >> 0x1d) & 1) || SUB41(uVar6 >> 0x1e,0)) {
                  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
                    snprintf((char *)local_1038,0x1000,
                             "Successfully mined %d accepted shares as requested and exiting.",
                             DAT_00077288);
                    FUN_00038730(4,local_1038,0);
                  }
                  FUN_000178f8();
                  goto LAB_00020aec;
                }
              }
              if (piVar5[0x21] == 2) {
                if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 4)) {
LAB_000216ac:
                  piVar5[0x21] = 1;
                  DAT_0007b5e4 = DAT_0007b5e4 + 1;
                }
                else {
                  snprintf((char *)local_1038,0x1000,
                           "Rejecting pool %d now accepting shares, re-enabling!",*piVar5);
                  FUN_00038730(4,local_1038,0);
                  if (piVar5[0x21] != 1) goto LAB_000216ac;
                }
                FUN_00019bf8(0);
              }
              if (*(char *)(iVar11 + 0x6eb) != '\0') {
                FUN_00017210();
              }
            }
LAB_00020aec:
            bVar19 = true;
            FUN_00015780(*(undefined4 *)((int)__ptr_00 + 0x24));
            *(undefined4 *)((int)__ptr_00 + 0x24) = 0;
            free(__ptr_00);
          }
        }
        if ((*(int *)(iVar3 + 4) != -1) &&
           (iVar11 = *(int *)(iVar3 + 4) + -1, *(int *)(iVar3 + 4) = iVar11, iVar11 == 0)) {
          FUN_0005a3e4(iVar3);
        }
        if (bVar19) goto LAB_000201c0;
LAB_00020ed0:
        if (DAT_00078e09 != '\0') goto LAB_00020edc;
      }
    }
    else {
LAB_000201c0:
      if ((char)param_1[0xae] != '\0') {
        iVar3 = FUN_0001a3d4();
        *(undefined1 *)(param_1 + 0xae) = 0;
        FUN_0001acc4(param_1,iVar3);
        *(undefined1 *)(iVar3 + 0x6e8) = 1;
        FUN_0001db0c(iVar3);
        FUN_00015780(iVar3);
      }
    }
    free(param3);
  } while( true );
}

