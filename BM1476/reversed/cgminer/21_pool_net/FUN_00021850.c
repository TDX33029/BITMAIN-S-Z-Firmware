//==================== FUN_00021850 @ 00021850 ====================

void FUN_00021850(void)

{
  uint uVar1;
  time_t tVar2;
  int *piVar3;
  pthread_mutex_t *__mutex;
  char *param4;
  int *piVar4;
  int *piVar5;
  int *piVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  int *piVar10;
  bool bVar11;
  int iVar12;
  double dVar13;
  int local_1054;
  int aiStack_1048 [2];
  int aiStack_1040 [2];
  char acStack_1038 [4100];
  
  pthread_setcanceltype(1,(int *)0x0);
  FUN_0002f0b4("Watchpool");
  FUN_000120f4();
  local_1054 = 0;
  do {
    local_1054 = local_1054 + 1;
    FUN_0002a84c(aiStack_1048);
    if (0x14 < local_1054) {
      local_1054 = 0;
    }
    if (0 < DAT_0007b198) {
      iVar8 = 0;
LAB_000218e8:
      do {
        piVar6 = *(int **)(DAT_0007b438 + iVar8 * 4);
        if ((DAT_0007730d == '\0') && (DAT_00076ffc == 0)) {
          __mutex = (pthread_mutex_t *)(piVar6 + 0x37);
          FUN_0002a84c(aiStack_1040);
          iVar12 = pthread_mutex_lock(__mutex);
          if (iVar12 != 0) {
            FUN_0000b414("reap_curl",0x251f);
          }
          piVar10 = piVar6 + 0x5e;
          piVar4 = (int *)*piVar10;
          if (piVar10 == piVar4) {
            iVar12 = 0;
            iVar7 = pthread_mutex_unlock(__mutex);
          }
          else {
            iVar7 = piVar6[0x50];
            iVar12 = 0;
            if (1 < iVar7) {
              iVar12 = 0;
              piVar5 = (int *)*piVar4;
              do {
                piVar3 = piVar5;
                if (300 < aiStack_1040[0] - piVar4[2]) {
                  piVar6[0x50] = iVar7 + -1;
                  piVar5 = (int *)piVar4[1];
                  iVar7 = *piVar4;
                  iVar12 = iVar12 + 1;
                  *(int **)(iVar7 + 4) = piVar5;
                  *piVar5 = iVar7;
                  *piVar4 = 0;
                  piVar4[1] = 0;
                  free(piVar4 + -1);
                }
                if (piVar10 == piVar3) break;
                iVar7 = piVar6[0x50];
                piVar5 = (int *)*piVar3;
                piVar4 = piVar3;
              } while (1 < iVar7);
            }
            iVar7 = pthread_mutex_unlock(__mutex);
          }
          if (iVar7 != 0) {
            FUN_0000b5c4("reap_curl",0x252d);
          }
          (*DAT_00075ed8)();
          if (((iVar12 != 0) && (DAT_00078e09 != '\0')) &&
             ((DAT_0007ad48 != '\0' || ((DAT_00078e08 != '\0' || (6 < DAT_000766c4)))))) {
            param4 = "s";
            if (iVar12 == 1) {
              param4 = "";
            }
            snprintf(acStack_1038,0x1000,"Reaped %d curl%s from pool %d",iVar12,param4,*piVar6);
            FUN_00038730(7,acStack_1038,0);
          }
          tVar2 = time((time_t *)0x0);
          iVar12 = pthread_mutex_lock((pthread_mutex_t *)&DAT_00077030);
          if (iVar12 != 0) {
            FUN_0000b414("prune_stratum_shares",0x253c);
          }
          if (DAT_00077048 == (int *)0x0) {
            uVar1 = 0;
          }
          else {
            uVar1 = 0;
            piVar4 = (int *)DAT_00077048[2];
            piVar10 = DAT_00077048;
            while( true ) {
              piVar5 = piVar4;
              piVar4 = DAT_00077048;
              if ((piVar6 == *(int **)(piVar10[9] + 0x6d4)) && (piVar10[0xb] + 0x78 < tVar2)) {
                iVar12 = piVar10[1];
                if ((iVar12 == 0) && (piVar10[2] == 0)) {
                  free(*(void **)*DAT_00077048);
                  free((void *)*piVar4);
                  DAT_00077048 = (int *)0x0;
                }
                else {
                  iVar7 = *DAT_00077048;
                  if (piVar10 == (int *)(*(int *)(iVar7 + 0x10) - *(int *)(iVar7 + 0x14))) {
                    *(int *)(iVar7 + 0x10) = iVar12 + *(int *)(iVar7 + 0x14);
                    iVar12 = piVar10[1];
                  }
                  if (iVar12 == 0) {
                    piVar3 = (int *)piVar10[2];
                    piVar4 = piVar3;
                    DAT_00077048 = piVar3;
                  }
                  else {
                    *(int *)(iVar12 + *(int *)(*piVar4 + 0x14) + 8) = piVar10[2];
                    piVar3 = (int *)piVar10[2];
                  }
                  if (piVar3 != (int *)0x0) {
                    *(int *)((int)piVar3 + *(int *)(*piVar4 + 0x14) + 4) = piVar10[1];
                  }
                  uVar9 = ((int *)*piVar4)[1] - 1U & piVar10[7];
                  iVar12 = *(int *)*piVar4 + uVar9 * 0xc;
                  *(int *)(iVar12 + 4) = *(int *)(iVar12 + 4) + -1;
                  if (piVar10 == *(int **)(*(int *)*piVar4 + uVar9 * 0xc)) {
                    *(int *)(*(int *)*piVar4 + uVar9 * 0xc) = piVar10[4];
                  }
                  if (piVar10[3] != 0) {
                    *(int *)(piVar10[3] + 0x10) = piVar10[4];
                  }
                  if (piVar10[4] != 0) {
                    *(int *)(piVar10[4] + 0xc) = piVar10[3];
                  }
                  *(int *)(*piVar4 + 0xc) = *(int *)(*piVar4 + 0xc) + -1;
                }
                uVar1 = uVar1 + 1;
                FUN_00015780(piVar10[9]);
                piVar10[9] = 0;
                free(piVar10);
              }
              if (piVar5 == (int *)0x0) break;
              piVar4 = (int *)piVar5[2];
              piVar10 = piVar5;
            }
          }
          iVar12 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_00077030);
          if (iVar12 != 0) {
            FUN_0000b5c4("prune_stratum_shares",0x2547);
          }
          (*DAT_00075ed8)();
          if (uVar1 != 0) {
            if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
              snprintf(acStack_1038,0x1000,
                       "Lost %d shares due to no stratum share response from pool %d",uVar1,*piVar6)
              ;
              FUN_00038730(4,acStack_1038,0);
            }
            piVar6[0x26] = piVar6[0x26] + uVar1;
            bVar11 = CARRY4(DAT_0007bc80,uVar1);
            DAT_0007bc80 = DAT_0007bc80 + uVar1;
            DAT_0007bc84 = DAT_0007bc84 + ((int)uVar1 >> 0x1f) + (uint)bVar11;
          }
        }
        if (local_1054 == 0x14) {
          uVar1 = piVar6[0x2e];
          uVar9 = piVar6[10];
          dVar13 = (double)FUN_0005cb64(uVar9 - uVar1,
                                        piVar6[0xb] - (((int)uVar1 >> 0x1f) + (uint)(uVar9 < uVar1))
                                       );
          piVar6[0x2e] = uVar9;
          dVar13 = (*(double *)(piVar6 + 0x2c) + dVar13 * 0.63) / 1.63;
          iVar12 = VectorFloatToSigned(dVar13,3);
          *(double *)(piVar6 + 0x2c) = dVar13;
          piVar6[0x2f] = iVar12;
        }
        if ((piVar6[0x21] == 0) || ((char)piVar6[0x4f] != '\0')) {
LAB_000218d8:
          iVar8 = iVar8 + 1;
          if (DAT_0007b198 <= iVar8) break;
          goto LAB_000218e8;
        }
        if (*(char *)((int)piVar6 + 0x81) == '\0') {
LAB_000218cc:
          if (((DAT_00076efc == 0) && (iVar7 = piVar6[1], iVar12 = FUN_0001fb7c(), iVar7 < iVar12))
             && (300 < aiStack_1048[0] - piVar6[0x2a])) {
            if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
              snprintf(acStack_1038,0x1000,"Pool %d %s stable for 5 mins",*piVar6,
                       (char *)piVar6[0x31]);
              FUN_00038730(4,acStack_1038,0);
            }
            FUN_00019bf8(0);
          }
          goto LAB_000218d8;
        }
        if (aiStack_1048[0] - piVar6[0x2a] < 0x1f) goto LAB_000218d8;
        iVar12 = FUN_00018ca0(piVar6,1);
        if ((iVar12 == 0) || (iVar12 = FUN_00018c2c(piVar6,(int)piVar6 + 0x81), iVar12 == 0)) {
          FUN_0002a84c(piVar6 + 0x2a);
        }
        else {
          FUN_0001fc34(piVar6);
        }
        if (*(char *)((int)piVar6 + 0x81) == '\0') goto LAB_000218cc;
        iVar8 = iVar8 + 1;
      } while (iVar8 < DAT_0007b198);
    }
    iVar8 = FUN_0001da58();
    if (*(char *)(iVar8 + 0x81) != '\0') {
      FUN_00019bf8(0);
    }
    if ((DAT_00076efc == 2) && (DAT_0007b558 * 0x3c < aiStack_1048[0] - DAT_00077310)) {
      FUN_0002a84c(&DAT_00077310);
      FUN_00019bf8(0);
    }
    FUN_0002a92c(30000);
  } while( true );
}

