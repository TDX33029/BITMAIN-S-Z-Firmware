//==================== FUN_00019fc8 @ 00019fc8 ====================

int FUN_00019fc8(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  double in_d0;
  undefined4 extraout_s1;
  undefined1 auStack_28 [8];
  
  FUN_0002a84c(auStack_28);
  iVar2 = pthread_rwlock_wrlock((pthread_rwlock_t *)(param_1 + 0x140));
  if (iVar2 != 0) {
    FUN_0000b480("age_queued_work",0x22f6);
  }
  iVar2 = *(int *)(param_1 + 0x160);
  if (iVar2 == 0) {
    iVar4 = 0;
  }
  else {
    iVar4 = 0;
    iVar1 = *(int *)(iVar2 + 0x730);
    while( true ) {
      iVar3 = iVar1;
      uVar5 = FUN_0002aa24(auStack_28,iVar2 + 0x778);
      if (in_d0 < (double)CONCAT44(extraout_s1,uVar5)) {
        FUN_00016b40(param_1,iVar2);
        FUN_00015780(iVar2);
        iVar4 = iVar4 + 1;
      }
      if (iVar3 == 0) break;
      iVar1 = *(int *)(iVar3 + 0x730);
      iVar2 = iVar3;
    }
  }
  iVar2 = pthread_rwlock_unlock((pthread_rwlock_t *)(param_1 + 0x140));
  if (iVar2 != 0) {
    FUN_0000b630("age_queued_work",0x2300);
  }
  (*DAT_00075ed8)();
  return iVar4;
}

