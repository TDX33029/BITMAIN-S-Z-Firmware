//==================== FUN_00049ba0 @ 00049ba0 ====================

undefined4 FUN_00049ba0(int param_1)

{
  undefined4 uVar1;
  uint uVar2;
  
  uVar2 = param_1 << 0x10 | 0x1900000;
  pthread_mutex_lock((pthread_mutex_t *)&DAT_000790a0);
  uVar1 = FUN_0003c708(uVar2);
  FUN_0003c708(uVar2);
  FUN_0002a92c(100);
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790a0);
  return uVar1;
}

