//==================== FUN_00019f54 @ 00019f54 ====================

void FUN_00019f54(int param_1,undefined4 param_2)

{
  int iVar1;
  
  iVar1 = pthread_rwlock_wrlock((pthread_rwlock_t *)(param_1 + 0x140));
  if (iVar1 != 0) {
    FUN_0000b480("add_queued",0x2279);
  }
  FUN_00016624(param_1,param_2);
  iVar1 = pthread_rwlock_unlock((pthread_rwlock_t *)(param_1 + 0x140));
  if (iVar1 != 0) {
    FUN_0000b630("add_queued",0x227b);
  }
  (*DAT_00075ed8)();
  return;
}

