//==================== FUN_000189a4 @ 000189a4 ====================

void FUN_000189a4(int param_1)

{
  int iVar1;
  char acStack_1008 [4096];
  
  if ((DAT_00078e09 != '\0') &&
     (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
    snprintf(acStack_1008,0x1000,"%s %d: invalid nonce - HW error",
             *(char **)(*(int *)(*(int *)(param_1 + 0x24) + 4) + 8),
             *(int *)(*(int *)(param_1 + 0x24) + 8));
    FUN_00038730(7,acStack_1008,1);
  }
  iVar1 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007ad4c);
  if (iVar1 != 0) {
    FUN_0000b414("inc_hw_errors",0x2068);
  }
  *(int *)(*(int *)(param_1 + 0x24) + 0x2c) = *(int *)(*(int *)(param_1 + 0x24) + 0x2c) + 1;
  DAT_0007b8a4 = DAT_0007b8a4 + 1;
  iVar1 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007ad4c);
  if (iVar1 != 0) {
    FUN_0000b5c4("inc_hw_errors",0x206b);
  }
  (*DAT_00075ed8)();
  (**(code **)(*(int *)(*(int *)(param_1 + 0x24) + 4) + 0x54))(param_1);
  return;
}

