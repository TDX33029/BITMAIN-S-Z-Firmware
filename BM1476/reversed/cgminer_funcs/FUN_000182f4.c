//==================== FUN_000182f4 @ 000182f4 ====================

void FUN_000182f4(int *param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  uint uVar5;
  int *__ptr;
  int *piVar6;
  uint param3;
  bool bVar7;
  double dVar8;
  char acStack_1030 [4100];
  
  iVar2 = pthread_mutex_lock((pthread_mutex_t *)&DAT_00077030);
  if (iVar2 != 0) {
    FUN_0000b414("clear_stratum_shares",0x1b4a);
  }
  if (DAT_00077048 == (int *)0x0) {
    dVar8 = 0.0;
    param3 = 0;
  }
  else {
    param3 = 0;
    dVar8 = 0.0;
    __ptr = DAT_00077048;
    piVar6 = (int *)DAT_00077048[2];
    while( true ) {
      piVar1 = DAT_00077048;
      if (*(int **)(__ptr[9] + 0x6d4) == param_1) {
        iVar2 = __ptr[1];
        if ((iVar2 == 0) && (__ptr[2] == 0)) {
          free(*(void **)*DAT_00077048);
          free((void *)*piVar1);
          DAT_00077048 = (int *)0x0;
        }
        else {
          iVar3 = *DAT_00077048;
          if ((int *)(*(int *)(iVar3 + 0x10) - *(int *)(iVar3 + 0x14)) == __ptr) {
            *(int *)(iVar3 + 0x10) = iVar2 + *(int *)(iVar3 + 0x14);
            iVar2 = __ptr[1];
          }
          if (iVar2 == 0) {
            piVar4 = (int *)__ptr[2];
            piVar1 = piVar4;
            DAT_00077048 = piVar4;
          }
          else {
            *(int *)(iVar2 + *(int *)(*piVar1 + 0x14) + 8) = __ptr[2];
            piVar4 = (int *)__ptr[2];
          }
          if (piVar4 != (int *)0x0) {
            *(int *)((int)piVar4 + *(int *)(*piVar1 + 0x14) + 4) = __ptr[1];
          }
          uVar5 = ((int *)*piVar1)[1] - 1U & __ptr[7];
          iVar2 = *(int *)*piVar1 + uVar5 * 0xc;
          *(int *)(iVar2 + 4) = *(int *)(iVar2 + 4) + -1;
          if (*(int **)(*(int *)*piVar1 + uVar5 * 0xc) == __ptr) {
            *(int *)(*(int *)*piVar1 + uVar5 * 0xc) = __ptr[4];
          }
          if (__ptr[3] != 0) {
            *(int *)(__ptr[3] + 0x10) = __ptr[4];
          }
          if (__ptr[4] != 0) {
            *(int *)(__ptr[4] + 0xc) = __ptr[3];
          }
          *(int *)(*piVar1 + 0xc) = *(int *)(*piVar1 + 0xc) + -1;
        }
        param3 = param3 + 1;
        dVar8 = dVar8 + *(double *)(__ptr[9] + 0x748);
        FUN_00015780();
        __ptr[9] = 0;
        param_1[0xbd] = param_1[0xbd] + -1;
        free(__ptr);
      }
      if (piVar6 == (int *)0x0) break;
      __ptr = piVar6;
      piVar6 = (int *)piVar6[2];
    }
  }
  iVar2 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_00077030);
  if (iVar2 != 0) {
    FUN_0000b5c4("clear_stratum_shares",6999);
  }
  (*DAT_00075ed8)();
  if (param3 != 0) {
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
      snprintf(acStack_1030,0x1000,"Lost %d shares due to stratum disconnect on pool %d",param3,
               *param_1);
      FUN_00038730(4,acStack_1030,0);
    }
    param_1[0x26] = param_1[0x26] + param3;
    bVar7 = CARRY4(DAT_0007bc80,param3);
    DAT_0007bc80 = DAT_0007bc80 + param3;
    DAT_0007bc84 = DAT_0007bc84 + ((int)param3 >> 0x1f) + (uint)bVar7;
    DAT_0007b428 = DAT_0007b428 + dVar8;
    *(double *)(param_1 + 0x1e) = *(double *)(param_1 + 0x1e) + dVar8;
  }
  return;
}

