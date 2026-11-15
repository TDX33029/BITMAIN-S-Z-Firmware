//==================== FUN_00046c10 @ 00046c10 ====================

undefined4 FUN_00046c10(void)

{
  int iVar1;
  char *pcVar2;
  undefined4 uVar3;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  
  pcVar2 = &DAT_0009d4bb;
  iVar1 = 0;
  uVar3 = 1;
  local_3c = 0;
  local_38 = 0;
  local_34 = 0;
  local_30 = 0;
  local_2c = 0;
  local_28 = 0;
  local_24 = 0;
  local_20 = 0;
  local_1c = 0;
  do {
    pcVar2 = pcVar2 + 1;
    if (*pcVar2 == '\x01') {
      pthread_mutex_lock((pthread_mutex_t *)&DAT_000790d0);
      DAT_0007909c = iVar1;
      uVar3 = FUN_00041e7c((int)&local_3c + iVar1 * 9,0,9);
      FUN_0002a92c(100);
      pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790d0);
    }
    iVar1 = iVar1 + 1;
  } while (iVar1 != 4);
  return uVar3;
}

