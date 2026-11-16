//==================== FUN_0001d9b4 @ 0001d9b4 ====================

undefined4
FUN_0001d9b4(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5
            ,undefined4 param_6)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = pthread_rwlock_rdlock((pthread_rwlock_t *)(param_1 + 0x140));
  if (iVar1 != 0) {
    FUN_0000b69c("find_queued_work_bymidstate",0x22a5);
  }
  uVar2 = FUN_00016a80(*(undefined4 *)(param_1 + 0x160),param_2,param_3,param_4,param_5,param_6);
  iVar1 = pthread_rwlock_unlock((pthread_rwlock_t *)(param_1 + 0x140));
  if (iVar1 != 0) {
    FUN_0000b630("find_queued_work_bymidstate",0x22a7);
  }
  (*DAT_00075ed8)();
  return uVar2;
}

