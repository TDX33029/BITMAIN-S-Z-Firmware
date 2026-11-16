//==================== FUN_0001a0b8 @ 0001a0b8 ====================

void FUN_0001a0b8(int param_1,undefined4 param_2)

{
  int iVar1;
  
  iVar1 = pthread_rwlock_wrlock((pthread_rwlock_t *)(param_1 + 0x140));
  if (iVar1 != 0) {
    FUN_0000b480("work_completed",0x2309);
  }
  FUN_00016b40(param_1,param_2);
  iVar1 = pthread_rwlock_unlock((pthread_rwlock_t *)(param_1 + 0x140));
  if (iVar1 != 0) {
    FUN_0000b630("work_completed",0x230b);
  }
  (*DAT_00075ed8)();
  FUN_00015780(param_2);
  return;
}

