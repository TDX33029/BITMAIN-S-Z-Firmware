//==================== FUN_0004791c @ 0004791c ====================

void FUN_0004791c(undefined4 *param_1)

{
  pthread_t __th;
  uint uVar1;
  char acStack_1018 [4096];
  
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    snprintf(acStack_1018,0x1000,"%s Begin\n","bitmain_ZCASH_reinit_chain");
    FUN_00038730(5,acStack_1018,0);
  }
  __th = pthread_self();
  pthread_detach(__th);
  pthread_mutex_lock((pthread_mutex_t *)&DAT_00079134);
  uVar1 = (uint)*(byte *)(param_1 + 1);
  FUN_00046434(*param_1,uVar1);
  FUN_00042f7c(uVar1);
  FUN_0002a92c(100);
  FUN_000435e0(uVar1,(&DAT_0009d705)[uVar1]);
  FUN_0002a92c(100);
  FUN_000430a0(uVar1,80000);
  FUN_0002a92c(100);
  FUN_00044004(uVar1,DAT_00076b60);
  FUN_0002a92c(100);
  FUN_0004601c(uVar1);
  (&DAT_000790ec)[uVar1] = 1;
  sleep(1);
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_00079134);
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    snprintf(acStack_1018,0x1000,"%s Done\n","bitmain_ZCASH_reinit_chain");
    FUN_00038730(5,acStack_1018,0);
  }
  return;
}

