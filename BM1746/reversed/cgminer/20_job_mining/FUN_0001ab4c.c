//==================== FUN_0001ab4c @ 0001ab4c ====================

undefined4
FUN_0001ab4c(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5
            ,undefined4 param_6)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = pthread_rwlock_rdlock((pthread_rwlock_t *)(param_1 + 0x140));
  if (iVar1 != 0) {
    FUN_0000b69c("clone_queued_work_bymidstate",0x22b0);
  }
  iVar1 = FUN_00016a80(*(undefined4 *)(param_1 + 0x160),param_2,param_3,param_4,param_5,param_6);
  uVar2 = 0;
  if (iVar1 != 0) {
    uVar2 = FUN_0001a44c(iVar1,0);
  }
  iVar1 = pthread_rwlock_unlock((pthread_rwlock_t *)(param_1 + 0x140));
  if (iVar1 != 0) {
    FUN_0000b630("clone_queued_work_bymidstate",0x22b4);
  }
  (*DAT_00075ed8)();
  return uVar2;
}

