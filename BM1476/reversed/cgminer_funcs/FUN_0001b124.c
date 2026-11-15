//==================== FUN_0001b124 @ 0001b124 ====================

undefined4 FUN_0001b124(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = pthread_rwlock_rdlock((pthread_rwlock_t *)&DAT_0007b650);
  if (iVar1 != 0) {
    FUN_0000b69c("get_thread",0x266);
  }
  uVar2 = *(undefined4 *)(DAT_0007b5e0 + param_1 * 4);
  iVar1 = pthread_rwlock_unlock((pthread_rwlock_t *)&DAT_0007b650);
  if (iVar1 != 0) {
    FUN_0000b630("get_thread",0x268);
  }
  (*DAT_00075ed8)();
  return uVar2;
}

