//==================== FUN_0003e688 @ 0003e688 ====================

void FUN_0003e688(void)

{
  int iVar1;
  char *pcVar2;
  
  iVar1 = 0;
  pcVar2 = &DAT_0009d4bb;
  do {
    pcVar2 = pcVar2 + 1;
    if (*pcVar2 == '\x01') {
      pthread_mutex_lock((pthread_mutex_t *)&DAT_000790d0);
      DAT_0007909c = iVar1;
      FUN_0003e5a4(iVar1);
      pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790d0);
    }
    iVar1 = iVar1 + 1;
  } while (iVar1 != 4);
  return;
}

