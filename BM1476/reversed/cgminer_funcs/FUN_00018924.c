//==================== FUN_00018924 @ 00018924 ====================

void FUN_00018924(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  
  iVar1 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007ad4c);
  if (iVar1 != 0) {
    FUN_0000b414("inc_dev_status",0x2081);
  }
  DAT_0007b8a8 = param_1;
  DAT_0007bb20 = param_2;
  iVar1 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007ad4c);
  if (iVar1 != 0) {
    FUN_0000b5c4("inc_dev_status",0x2084);
  }
  (*DAT_00075ed8)();
  return;
}

