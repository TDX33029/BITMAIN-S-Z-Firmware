//==================== FUN_00027f10 @ 00027f10 ====================

void FUN_00027f10(int *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  param_1[8] = 0;
  iVar3 = pthread_rwlock_wrlock((pthread_rwlock_t *)&DAT_0007b700);
  if (iVar3 != 0) {
    FUN_0000b480("enable_device",0x2894);
  }
  iVar1 = DAT_0007b628;
  iVar3 = DAT_00078720;
  iVar2 = DAT_00078720 + 1;
  *param_1 = DAT_00078720;
  DAT_00078720 = iVar2;
  *(int **)(iVar1 + iVar3 * 4) = param_1;
  iVar3 = pthread_rwlock_unlock((pthread_rwlock_t *)&DAT_0007b700);
  if (iVar3 != 0) {
    FUN_0000b630("enable_device",0x2896);
  }
  (*DAT_00075ed8)();
  if (DAT_0007b7d0 == '\0') {
    DAT_0007bb6c = param_1[0x25] + DAT_0007bb6c;
  }
  else {
    DAT_00078724 = param_1[0x25] + DAT_00078724;
  }
  iVar3 = pthread_rwlock_init((pthread_rwlock_t *)(param_1 + 0x50),(pthread_rwlockattr_t *)0x0);
  if (iVar3 != 0) {
    FUN_0000b558("enable_device",0x289d);
  }
  param_1[0x58] = 0;
  return;
}

