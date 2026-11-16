//==================== FUN_0001f17c @ 0001f17c ====================

void FUN_0001f17c(int param_1)

{
  size_t __size;
  pthread_mutex_t *__mutex;
  int iVar1;
  uint uVar2;
  void *pvVar3;
  undefined4 *puVar4;
  int *piVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  undefined4 uVar9;
  int iVar10;
  uint uVar11;
  uint uVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  int iVar17;
  int iVar18;
  int iVar19;
  int iVar20;
  int local_1030;
  uint local_102c;
  char acStack_1028 [4100];
  
  if ((DAT_00078e09 != '\0') &&
     (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
    snprintf(acStack_1028,0x1000,"Pushing work from pool %d to hash queue",
             **(int **)(param_1 + 0x6d4));
    FUN_00038730(7,acStack_1028,0);
  }
  *(undefined4 *)(param_1 + 0x720) = DAT_000770ac;
  FUN_0001db0c(param_1);
  __mutex = DAT_0007704c;
  *(int *)(*(int *)(param_1 + 0x6d4) + 0x44) = *(int *)(*(int *)(param_1 + 0x6d4) + 0x44) + 1;
  iVar1 = pthread_mutex_lock(__mutex);
  if (iVar1 != 0) {
    FUN_0000b414("hash_push",0x159d);
  }
  iVar1 = DAT_00077050;
  if ((*(char *)(param_1 + 0x6e1) == '\0') && (*(int *)(param_1 + 0x6e4) != 0)) {
    DAT_00077058 = DAT_00077058 + 1;
  }
  if (*(char *)(DAT_0007b620 + 8) != '\0') {
LAB_0001f708:
    pthread_cond_broadcast((pthread_cond_t *)(DAT_0007b620 + 0x28));
    iVar1 = pthread_mutex_unlock(DAT_0007704c);
    if (iVar1 != 0) {
      FUN_0000b5c4("hash_push",0x15a8);
    }
    (*DAT_00075ed8)();
    return;
  }
  *(undefined4 *)(param_1 + 0x730) = 0;
  *(int *)(param_1 + 0x73c) = param_1 + 0x724;
  *(undefined4 *)(param_1 + 0x740) = 4;
  if (iVar1 == 0) {
    *(undefined4 *)(param_1 + 0x72c) = 0;
    DAT_00077050 = param_1;
    puVar4 = malloc(0x2c);
    *(undefined4 **)(param_1 + 0x728) = puVar4;
    if (puVar4 == (undefined4 *)0x0) goto LAB_0001f8f0;
    *puVar4 = 0;
    puVar4[3] = 0;
    puVar4[4] = 0;
    puVar4[5] = 0;
    puVar4[6] = 0;
    puVar4[7] = 0;
    puVar4[8] = 0;
    puVar4[9] = 0;
    puVar4[10] = 0;
    puVar4[4] = param_1 + 0x728;
    puVar4[1] = 0x20;
    puVar4[2] = 5;
    puVar4[5] = 0x728;
    pvVar3 = malloc(0x180);
    iVar1 = DAT_00077050;
    *puVar4 = pvVar3;
    if ((void *)**(undefined4 **)(iVar1 + 0x728) == (void *)0x0) goto LAB_0001f8f0;
    memset((void *)**(undefined4 **)(iVar1 + 0x728),0,0x180);
    iVar1 = DAT_00077050;
    *(undefined4 *)(*(int *)(DAT_00077050 + 0x728) + 0x28) = 0xa0111fe1;
  }
  else {
    *(int *)(*(int *)(*(int *)(iVar1 + 0x728) + 0x10) + 8) = param_1;
    *(int *)(param_1 + 0x72c) =
         *(int *)(*(int *)(iVar1 + 0x728) + 0x10) - *(int *)(*(int *)(iVar1 + 0x728) + 0x14);
    *(int *)(*(int *)(iVar1 + 0x728) + 0x10) = param_1 + 0x728;
  }
  *(int *)(*(int *)(iVar1 + 0x728) + 0xc) = *(int *)(*(int *)(iVar1 + 0x728) + 0xc) + 1;
  uVar11 = *(byte *)(param_1 + 0x724) + 0x112410d + (uint)*(byte *)(param_1 + 0x727) * 0x1000000 +
           (uint)*(byte *)(param_1 + 0x726) * 0x10000 + (uint)*(byte *)(param_1 + 0x725) * 0x100 ^
           0x7f76d;
  puVar4 = (undefined4 *)(param_1 + 0x728);
  *puVar4 = *(undefined4 *)(iVar1 + 0x728);
  uVar7 = 0x9f49bac6 - uVar11 ^ uVar11 << 8;
  uVar2 = (-0x112410d - uVar11) - uVar7 ^ uVar7 >> 0xd;
  uVar11 = (uVar11 - uVar7) - uVar2 ^ uVar2 >> 0xc;
  uVar7 = (uVar7 - uVar2) - uVar11 ^ uVar11 << 0x10;
  uVar12 = (uVar2 - uVar11) - uVar7 ^ uVar7 >> 5;
  uVar2 = (uVar11 - uVar7) - uVar12 ^ uVar12 >> 3;
  uVar11 = (uVar7 - uVar12) - uVar2 ^ uVar2 << 10;
  uVar2 = (uVar12 - uVar2) - uVar11 ^ uVar11 >> 0xf;
  *(uint *)(param_1 + 0x744) = uVar2;
  iVar13 = (uVar2 & (*(int **)(iVar1 + 0x728))[1] - 1U) * 0xc;
  iVar8 = **(int **)(iVar1 + 0x728) + iVar13;
  *(int *)(iVar8 + 4) = *(int *)(iVar8 + 4) + 1;
  uVar9 = *(undefined4 *)(**(int **)(iVar1 + 0x728) + iVar13);
  *(undefined4 *)(param_1 + 0x734) = 0;
  *(undefined4 *)(param_1 + 0x738) = uVar9;
  iVar8 = **(int **)(iVar1 + 0x728);
  if (*(int *)(iVar8 + iVar13) != 0) {
    *(undefined4 **)(*(int *)(iVar8 + iVar13) + 0xc) = puVar4;
    iVar8 = **(int **)(iVar1 + 0x728);
  }
  *(undefined4 **)(iVar8 + iVar13) = puVar4;
  iVar13 = **(int **)(DAT_00077050 + 0x728) + iVar13;
  iVar1 = DAT_00077050;
  if (((uint)((*(int *)(iVar13 + 8) + 1) * 10) <= *(uint *)(iVar13 + 4)) &&
     (*(int *)(*(int *)(param_1 + 0x728) + 0x24) != 1)) {
    __size = *(int *)(*(int *)(param_1 + 0x728) + 4) * 0x18;
    pvVar3 = malloc(__size);
    if (pvVar3 == (void *)0x0) {
LAB_0001f8f0:
                    /* WARNING: Subroutine does not return */
      exit(-1);
    }
    memset(pvVar3,0,__size);
    iVar1 = *(int *)(param_1 + 0x728);
    *(uint *)(iVar1 + 0x18) =
         (uint)((*(int *)(iVar1 + 4) * 2 - 1U & *(uint *)(iVar1 + 0xc)) != 0) +
         (*(uint *)(iVar1 + 0xc) >> (*(int *)(iVar1 + 8) + 1U & 0xff));
    *(undefined4 *)(*(int *)(param_1 + 0x728) + 0x1c) = 0;
    piVar5 = *(int **)(param_1 + 0x728);
    uVar2 = piVar5[1];
    if (uVar2 != 0) {
      local_1030 = 0;
      local_102c = 0;
      do {
        iVar1 = *(int *)(*piVar5 + local_1030);
        if (*(int *)(*piVar5 + local_1030) != 0) {
          while( true ) {
            iVar13 = *(int *)(iVar1 + 0x10);
            uVar11 = uVar2 * 2 - 1 & *(uint *)(iVar1 + 0x1c);
            uVar2 = *(int *)((int)pvVar3 + uVar11 * 0xc + 4) + 1;
            *(uint *)((int)pvVar3 + uVar11 * 0xc + 4) = uVar2;
            if ((uint)piVar5[6] < uVar2) {
              piVar5[7] = piVar5[7] + 1;
              uVar9 = FUN_0005c648(*(undefined4 *)((int)pvVar3 + uVar11 * 0xc + 4),
                                   *(undefined4 *)(*(int *)(param_1 + 0x728) + 0x18));
              *(undefined4 *)((int)pvVar3 + uVar11 * 0xc + 8) = uVar9;
            }
            *(undefined4 *)(iVar1 + 0xc) = 0;
            *(undefined4 *)(iVar1 + 0x10) = *(undefined4 *)((int)pvVar3 + uVar11 * 0xc);
            iVar8 = *(int *)((int)pvVar3 + uVar11 * 0xc);
            if (iVar8 != 0) {
              *(int *)(iVar8 + 0xc) = iVar1;
            }
            *(int *)((int)pvVar3 + uVar11 * 0xc) = iVar1;
            piVar5 = *(int **)(param_1 + 0x728);
            if (iVar13 == 0) break;
            uVar2 = piVar5[1];
            iVar1 = iVar13;
          }
          uVar2 = piVar5[1];
        }
        local_102c = local_102c + 1;
        local_1030 = local_1030 + 0xc;
      } while (local_102c < uVar2);
    }
    free((void *)*piVar5);
    *(int *)(*(int *)(param_1 + 0x728) + 4) = *(int *)(*(int *)(param_1 + 0x728) + 4) << 1;
    *(int *)(*(int *)(param_1 + 0x728) + 8) = *(int *)(*(int *)(param_1 + 0x728) + 8) + 1;
    **(undefined4 **)(param_1 + 0x728) = pvVar3;
    iVar1 = DAT_00077050;
    iVar13 = *(int *)(param_1 + 0x728);
    if (*(uint *)(iVar13 + 0xc) >> 1 < *(uint *)(iVar13 + 0x1c)) {
      iVar8 = *(int *)(iVar13 + 0x20) + 1;
    }
    else {
      iVar8 = 0;
    }
    *(int *)(iVar13 + 0x20) = iVar8;
    if (1 < *(uint *)(*(int *)(param_1 + 0x728) + 0x20)) {
      *(undefined4 *)(*(int *)(param_1 + 0x728) + 0x24) = 1;
    }
    if (iVar1 == 0) goto LAB_0001f708;
  }
  iVar13 = 1;
  iVar8 = iVar1 + 0x728;
LAB_0001f5cc:
  iVar20 = 0;
  uVar2 = 0;
  iVar6 = 0;
  iVar18 = iVar8;
LAB_0001f5dc:
  uVar2 = uVar2 + 1;
  if (iVar13 == 0) {
    iVar10 = 0;
    iVar14 = iVar18;
  }
  else if (*(int *)(iVar18 + 8) == 0) {
    iVar10 = 1;
    iVar14 = 0;
  }
  else {
    iVar8 = *(int *)(*(int *)(iVar1 + 0x728) + 0x14);
    iVar10 = 1;
    iVar14 = *(int *)(iVar18 + 8) + iVar8;
    if (iVar14 != 0) {
      do {
        if (iVar10 == iVar13) break;
        iVar14 = *(int *)(iVar14 + 8);
        iVar10 = iVar10 + 1;
        if (iVar14 == 0) break;
        iVar14 = iVar14 + iVar8;
      } while (iVar14 != 0);
    }
  }
  iVar17 = iVar13;
  iVar8 = iVar6;
LAB_0001f63c:
  do {
    iVar6 = iVar18;
    iVar15 = iVar10;
    if (iVar10 == 0) goto LAB_0001f6b0;
LAB_0001f644:
    iVar18 = iVar6;
    if (iVar17 == 0 || iVar14 == 0) {
      if (iVar6 != 0) {
        iVar18 = *(int *)(iVar6 + 8);
        if (iVar18 != 0) {
          iVar18 = iVar18 + *(int *)(*(int *)(iVar1 + 0x728) + 0x14);
        }
      }
    }
    else {
      iVar16 = *(int *)(*(int *)(iVar1 + 0x728) + 0x14);
      if (0 < *(int *)(iVar6 + -iVar16 + 0x6d8) - *(int *)(iVar14 + -iVar16 + 0x6d8)) {
        iVar19 = *(int *)(iVar14 + 8);
        iVar10 = iVar15;
        if (iVar19 == 0) goto LAB_0001f750;
        iVar19 = iVar19 + iVar16;
        goto LAB_0001f750;
      }
      if (iVar6 != 0) {
        iVar18 = *(int *)(iVar6 + 8);
        if (iVar18 != 0) {
          iVar18 = iVar18 + iVar16;
        }
      }
    }
    iVar10 = iVar15 + -1;
    iVar19 = iVar14;
    if (iVar20 == 0) {
      iVar10 = iVar15 + -1;
      iVar8 = iVar6;
      iVar20 = iVar6;
      if (iVar6 != 0) {
        iVar15 = 0;
        goto LAB_0001f6a4;
      }
      goto LAB_0001f63c;
    }
    while (iVar14 = iVar19, iVar6 != 0) {
      *(int *)(iVar20 + 8) = iVar6 - *(int *)(*(int *)(iVar1 + 0x728) + 0x14);
      iVar15 = iVar20 - *(int *)(*(int *)(iVar1 + 0x728) + 0x14);
      iVar20 = iVar6;
LAB_0001f6a4:
      while( true ) {
        *(int *)(iVar20 + 4) = iVar15;
        iVar6 = iVar18;
        iVar15 = iVar10;
        if (iVar10 != 0) goto LAB_0001f644;
LAB_0001f6b0:
        if (iVar14 == 0 || iVar17 == 0) {
          iVar6 = iVar8;
          iVar18 = iVar14;
          if (iVar14 != 0) goto LAB_0001f5dc;
          if (iVar20 != 0) {
            *(undefined4 *)(iVar20 + 8) = 0;
          }
          if (1 < uVar2) {
            iVar13 = iVar13 << 1;
            if (iVar8 != 0) goto LAB_0001f5cc;
            iVar20 = iVar8;
          }
          *(int *)(*(int *)(iVar1 + 0x728) + 0x10) = iVar20;
          DAT_00077050 = iVar8 - *(int *)(*(int *)(iVar1 + 0x728) + 0x14);
          goto LAB_0001f708;
        }
        iVar19 = iVar10;
        if (*(int *)(iVar14 + 8) != 0) {
          iVar19 = *(int *)(iVar14 + 8) + *(int *)(*(int *)(iVar1 + 0x728) + 0x14);
        }
LAB_0001f750:
        iVar6 = iVar14;
        iVar17 = iVar17 + -1;
        if (iVar20 != 0) break;
        iVar15 = 0;
        iVar8 = iVar6;
        iVar14 = iVar19;
        iVar20 = iVar6;
      }
    }
    *(undefined4 *)(iVar20 + 8) = 0;
    iVar20 = iVar6;
  } while( true );
}

