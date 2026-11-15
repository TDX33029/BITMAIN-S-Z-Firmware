//==================== FUN_0001b074 @ 0001b074 ====================

int FUN_0001b074(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = pthread_rwlock_rdlock((pthread_rwlock_t *)(param_1 + 0x140));
  if (iVar1 != 0) {
    FUN_0000b69c("find_queued_work_byid",0x22d1);
  }
  iVar1 = *(int *)(param_1 + 0x160);
  if ((iVar1 == 0) || (iVar2 = *(int *)(iVar1 + 0x730), param_2 == *(int *)(iVar1 + 0x724))) {
    iVar2 = iVar1;
  }
  else {
    while ((iVar2 != 0 && (param_2 != *(int *)(iVar2 + 0x724)))) {
      iVar2 = *(int *)(iVar2 + 0x730);
    }
  }
  iVar1 = pthread_rwlock_unlock((pthread_rwlock_t *)(param_1 + 0x140));
  if (iVar1 != 0) {
    FUN_0000b630("find_queued_work_byid",0x22d3);
  }
  (*DAT_00075ed8)();
  return iVar2;
}

