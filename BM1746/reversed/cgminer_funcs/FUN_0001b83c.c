//==================== FUN_0001b83c @ 0001b83c ====================

undefined4 FUN_0001b83c(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = pthread_rwlock_rdlock((pthread_rwlock_t *)&DAT_0007b700);
  if (iVar1 != 0) {
    FUN_0000b69c("get_devices",0x278);
  }
  uVar2 = *(undefined4 *)(DAT_0007b628 + param_1 * 4);
  iVar1 = pthread_rwlock_unlock((pthread_rwlock_t *)&DAT_0007b700);
  if (iVar1 != 0) {
    FUN_0000b630("get_devices",0x27a);
  }
  (*DAT_00075ed8)();
  return uVar2;
}

