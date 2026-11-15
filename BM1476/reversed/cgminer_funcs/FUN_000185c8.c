//==================== FUN_000185c8 @ 000185c8 ====================

void FUN_000185c8(int *param_1)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  bool bVar9;
  char acStack_1020 [4100];
  
  iVar2 = pthread_mutex_lock(DAT_0007704c);
  if (iVar2 != 0) {
    FUN_0000b414("clear_pool_work",0x1b68);
  }
  if (DAT_00077050 == 0) {
    iVar2 = 0;
  }
  else {
    iVar2 = 0;
    iVar3 = DAT_00077050;
    iVar8 = *(int *)(DAT_00077050 + 0x730);
    while( true ) {
      iVar5 = DAT_00077050;
      if (*(int **)(iVar3 + 0x6d4) == param_1) {
        iVar4 = *(int *)(iVar3 + 0x72c);
        if ((iVar4 == 0) && (*(int *)(iVar3 + 0x730) == 0)) {
          free((void *)**(undefined4 **)(DAT_00077050 + 0x728));
          free(*(void **)(DAT_00077050 + 0x728));
          DAT_00077050 = 0;
        }
        else {
          iVar6 = *(int *)(DAT_00077050 + 0x728);
          if (*(int *)(iVar6 + 0x10) - *(int *)(iVar6 + 0x14) == iVar3) {
            *(int *)(iVar6 + 0x10) = iVar4 + *(int *)(iVar6 + 0x14);
            iVar4 = *(int *)(iVar3 + 0x72c);
          }
          if (iVar4 == 0) {
            iVar4 = *(int *)(iVar3 + 0x730);
            iVar5 = iVar4;
            DAT_00077050 = iVar4;
          }
          else {
            *(undefined4 *)(iVar4 + *(int *)(*(int *)(iVar5 + 0x728) + 0x14) + 8) =
                 *(undefined4 *)(iVar3 + 0x730);
            iVar4 = *(int *)(iVar3 + 0x730);
          }
          if (iVar4 != 0) {
            *(undefined4 *)(iVar4 + *(int *)(*(int *)(iVar5 + 0x728) + 0x14) + 4) =
                 *(undefined4 *)(iVar3 + 0x72c);
          }
          uVar7 = (*(int **)(iVar5 + 0x728))[1] - 1U & *(uint *)(iVar3 + 0x744);
          iVar4 = **(int **)(iVar5 + 0x728) + uVar7 * 0xc;
          *(int *)(iVar4 + 4) = *(int *)(iVar4 + 4) + -1;
          puVar1 = (undefined4 *)(iVar5 + 0x728);
          bVar9 = *(int *)(*(int *)*puVar1 + uVar7 * 0xc) == iVar3 + 0x728;
          if (bVar9) {
            iVar5 = *(int *)(iVar3 + 0x738);
            *(int *)(*(int *)*puVar1 + uVar7 * 0xc) = iVar5;
          }
          if (bVar9) {
            iVar5 = DAT_00077050;
          }
          if (*(int *)(iVar3 + 0x734) != 0) {
            *(undefined4 *)(*(int *)(iVar3 + 0x734) + 0x10) = *(undefined4 *)(iVar3 + 0x738);
          }
          if (*(int *)(iVar3 + 0x738) != 0) {
            *(undefined4 *)(*(int *)(iVar3 + 0x738) + 0xc) = *(undefined4 *)(iVar3 + 0x734);
          }
          *(int *)(*(int *)(iVar5 + 0x728) + 0xc) = *(int *)(*(int *)(iVar5 + 0x728) + 0xc) + -1;
        }
        iVar2 = iVar2 + 1;
        FUN_00015780(iVar3);
      }
      if (iVar8 == 0) break;
      iVar3 = iVar8;
      iVar8 = *(int *)(iVar8 + 0x730);
    }
  }
  iVar3 = pthread_mutex_unlock(DAT_0007704c);
  if (iVar3 != 0) {
    FUN_0000b5c4("clear_pool_work",0x1b72);
  }
  (*DAT_00075ed8)();
  if (((iVar2 != 0) && (DAT_00078e09 != '\0')) &&
     ((DAT_0007ad48 != '\0' || ((DAT_00078e08 != '\0' || (6 < DAT_000766c4)))))) {
    snprintf(acStack_1020,0x1000,"Cleared %d work items due to stratum disconnect on pool %d",iVar2,
             *param_1);
    FUN_00038730(7,acStack_1020,0);
  }
  return;
}

