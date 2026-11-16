//==================== FUN_00018834 @ 00018834 ====================

undefined1 FUN_00018834(int param_1,undefined1 *param_2)

{
  undefined1 uVar1;
  int iVar2;
  
  iVar2 = pthread_mutex_lock((pthread_mutex_t *)(param_1 + 0xdc));
  if (iVar2 != 0) {
    FUN_0000b414("pool_tset",0x304);
  }
  uVar1 = *param_2;
  *param_2 = 1;
  iVar2 = pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0xdc));
  if (iVar2 != 0) {
    FUN_0000b5c4("pool_tset",0x307);
  }
  (*DAT_00075ed8)();
  return uVar1;
}

