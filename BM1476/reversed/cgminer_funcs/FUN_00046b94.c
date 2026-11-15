//==================== FUN_00046b94 @ 00046b94 ====================

undefined4 FUN_00046b94(int param_1)

{
  int iVar1;
  char *pcVar2;
  undefined4 uVar3;
  
  pcVar2 = &DAT_0009d4bb;
  uVar3 = 1;
  iVar1 = 0;
  do {
    pcVar2 = pcVar2 + 1;
    if (*pcVar2 == '\x01') {
      pthread_mutex_lock((pthread_mutex_t *)&DAT_000790d0);
      DAT_0007909c = iVar1;
      uVar3 = FUN_00041e7c(param_1,0,9);
      FUN_0002a92c(100);
      pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790d0);
    }
    iVar1 = iVar1 + 1;
    param_1 = param_1 + 9;
  } while (iVar1 != 4);
  return uVar3;
}

