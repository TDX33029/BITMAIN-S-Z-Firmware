//==================== FUN_000497cc @ 000497cc ====================

void FUN_000497cc(void)

{
  pthread_mutex_lock((pthread_mutex_t *)&DAT_00078e88);
  DAT_0007c28c = 0;
  DAT_0007c290 = 0;
  DAT_0007c294 = 0;
  FUN_0003c0a0(0);
  FUN_0003c0a0(1);
  FUN_0003c0a0(2);
  FUN_0003c0a0(3);
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_00078e88);
  return;
}

