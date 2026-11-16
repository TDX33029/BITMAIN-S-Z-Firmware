//==================== FUN_00022f88 @ 00022f88 ====================

void FUN_00022f88(void)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  bool bVar9;
  char acStack_1018 [4096];
  
  iVar2 = pthread_mutex_lock(DAT_0007704c);
  if (iVar2 != 0) {
    FUN_0000b414("discard_stale",0x145e);
  }
  if (DAT_00077050 == 0) {
    iVar2 = 0;
  }
  else {
    iVar2 = 0;
    iVar4 = DAT_00077050;
    iVar8 = *(int *)(DAT_00077050 + 0x730);
    while( true ) {
      iVar3 = FUN_00022b44(iVar4,0);
      iVar6 = DAT_00077050;
      if (iVar3 != 0) {
        iVar3 = *(int *)(iVar4 + 0x72c);
        if ((iVar3 == 0) && (*(int *)(iVar4 + 0x730) == 0)) {
          free((void *)**(undefined4 **)(DAT_00077050 + 0x728));
          free(*(void **)(DAT_00077050 + 0x728));
          DAT_00077050 = 0;
        }
        else {
          iVar7 = *(int *)(DAT_00077050 + 0x728);
          if (*(int *)(iVar7 + 0x10) - *(int *)(iVar7 + 0x14) == iVar4) {
            *(int *)(iVar7 + 0x10) = iVar3 + *(int *)(iVar7 + 0x14);
            iVar3 = *(int *)(iVar4 + 0x72c);
          }
          if (iVar3 == 0) {
            iVar3 = *(int *)(iVar4 + 0x730);
            iVar6 = iVar3;
            DAT_00077050 = iVar3;
          }
          else {
            *(undefined4 *)(iVar3 + *(int *)(*(int *)(iVar6 + 0x728) + 0x14) + 8) =
                 *(undefined4 *)(iVar4 + 0x730);
            iVar3 = *(int *)(iVar4 + 0x730);
          }
          if (iVar3 != 0) {
            *(undefined4 *)(iVar3 + *(int *)(*(int *)(iVar6 + 0x728) + 0x14) + 4) =
                 *(undefined4 *)(iVar4 + 0x72c);
          }
          uVar5 = (*(int **)(iVar6 + 0x728))[1] - 1U & *(uint *)(iVar4 + 0x744);
          iVar3 = **(int **)(iVar6 + 0x728) + uVar5 * 0xc;
          *(int *)(iVar3 + 4) = *(int *)(iVar3 + 4) + -1;
          puVar1 = (undefined4 *)(iVar6 + 0x728);
          bVar9 = *(int *)(*(int *)*puVar1 + uVar5 * 0xc) == iVar4 + 0x728;
          if (bVar9) {
            iVar6 = *(int *)(iVar4 + 0x738);
            *(int *)(*(int *)*puVar1 + uVar5 * 0xc) = iVar6;
          }
          if (bVar9) {
            iVar6 = DAT_00077050;
          }
          if (*(int *)(iVar4 + 0x734) != 0) {
            *(undefined4 *)(*(int *)(iVar4 + 0x734) + 0x10) = *(undefined4 *)(iVar4 + 0x738);
          }
          if (*(int *)(iVar4 + 0x738) != 0) {
            *(undefined4 *)(*(int *)(iVar4 + 0x738) + 0xc) = *(undefined4 *)(iVar4 + 0x734);
          }
          *(int *)(*(int *)(iVar6 + 0x728) + 0xc) = *(int *)(*(int *)(iVar6 + 0x728) + 0xc) + -1;
        }
        iVar2 = iVar2 + 1;
        FUN_000159b4(iVar4);
      }
      if (iVar8 == 0) break;
      iVar4 = iVar8;
      iVar8 = *(int *)(iVar8 + 0x730);
    }
  }
  pthread_cond_signal((pthread_cond_t *)&DAT_0007b1a0);
  iVar4 = pthread_mutex_unlock(DAT_0007704c);
  if (iVar4 != 0) {
    FUN_0000b5c4("discard_stale",0x1469);
  }
  (*DAT_00075ed8)();
  if (((iVar2 != 0) && (DAT_00078e09 != '\0')) &&
     ((DAT_0007ad48 != '\0' || ((DAT_00078e08 != '\0' || (6 < DAT_000766c4)))))) {
    snprintf(acStack_1018,0x1000,"Discarded %d stales that didn\'t match current hash",iVar2);
    FUN_00038730(7,acStack_1018,0);
  }
  return;
}

