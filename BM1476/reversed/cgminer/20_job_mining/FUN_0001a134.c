//==================== FUN_0001a134 @ 0001a134 ====================

int FUN_0001a134(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                undefined4 param_5,undefined4 param_6)

{
  int iVar1;
  int iVar2;
  
  iVar1 = pthread_rwlock_wrlock((pthread_rwlock_t *)(param_1 + 0x140));
  if (iVar1 != 0) {
    FUN_0000b480("take_queued_work_bymidstate",0x2316);
  }
  iVar1 = FUN_00016a80(*(undefined4 *)(param_1 + 0x160),param_2,param_3,param_4,param_5,param_6);
  if (iVar1 != 0) {
    FUN_00016b40(param_1,iVar1);
  }
  iVar2 = pthread_rwlock_unlock((pthread_rwlock_t *)(param_1 + 0x140));
  if (iVar2 != 0) {
    FUN_0000b630("take_queued_work_bymidstate",0x231a);
  }
  (*DAT_00075ed8)();
  return iVar1;
}

