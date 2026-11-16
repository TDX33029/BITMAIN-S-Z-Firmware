//==================== FUN_00018c2c @ 00018c2c ====================

undefined1 FUN_00018c2c(int param_1,undefined1 *param_2)

{
  undefined1 uVar1;
  int iVar2;
  
  iVar2 = pthread_mutex_lock((pthread_mutex_t *)(param_1 + 0xdc));
  if (iVar2 != 0) {
    FUN_0000b414("pool_tclear",0x310);
  }
  uVar1 = *param_2;
  *param_2 = 0;
  iVar2 = pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0xdc));
  if (iVar2 != 0) {
    FUN_0000b5c4("pool_tclear",0x313);
  }
  (*DAT_00075ed8)();
  return uVar1;
}

