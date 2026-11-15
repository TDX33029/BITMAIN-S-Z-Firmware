//==================== FUN_0001ac00 @ 0001ac00 ====================

undefined4 FUN_0001ac00(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  
  iVar1 = pthread_rwlock_rdlock((pthread_rwlock_t *)(param_1 + 0x140));
  if (iVar1 != 0) {
    FUN_0000b69c("clone_queued_work_byid",0x22dc);
  }
  iVar1 = *(int *)(param_1 + 0x160);
  if (iVar1 == 0) {
    uVar3 = 0;
  }
  else {
    iVar2 = *(int *)(iVar1 + 0x730);
    if (param_2 == *(int *)(iVar1 + 0x724)) {
      iVar2 = iVar1;
LAB_0001ac8c:
      uVar3 = FUN_0001a44c(iVar2,0);
    }
    else {
      while (iVar2 != 0) {
        if (param_2 == *(int *)(iVar2 + 0x724)) goto LAB_0001ac8c;
        iVar2 = *(int *)(iVar2 + 0x730);
      }
      uVar3 = 0;
    }
  }
  iVar1 = pthread_rwlock_unlock((pthread_rwlock_t *)(param_1 + 0x140));
  if (iVar1 != 0) {
    FUN_0000b630("clone_queued_work_byid",0x22e0);
  }
  (*DAT_00075ed8)();
  return uVar3;
}

