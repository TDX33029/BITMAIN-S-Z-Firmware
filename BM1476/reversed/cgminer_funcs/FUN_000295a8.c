//==================== FUN_000295a8 @ 000295a8 ====================

void FUN_000295a8(int param_1)

{
  int iVar1;
  ssize_t sVar2;
  
  iVar1 = pthread_mutex_lock((pthread_mutex_t *)(param_1 + 0x2d8));
  if (iVar1 != 0) {
    FUN_0000b708("clear_sock",0x7a3);
  }
  do {
    if (*(int *)(param_1 + 0x264) == 0) break;
    sVar2 = recv(*(int *)(param_1 + 0x264),*(void **)(param_1 + 0x268),0x1ffc,0);
  } while (0 < sVar2);
  FUN_00029388((pthread_mutex_t *)(param_1 + 0x2d8),"clear_sock",0x7ac);
  (*DAT_00075ed8)();
  FUN_00028ae8(param_1);
  return;
}

