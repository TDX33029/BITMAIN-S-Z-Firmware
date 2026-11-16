//==================== FUN_0002a3dc @ 0002a3dc ====================

void FUN_0002a3dc(int param_1)

{
  if (param_1 == 0) {
    return;
  }
  if (*(pthread_t *)(param_1 + 0xc) != 0) {
    pthread_join(*(pthread_t *)(param_1 + 0xc),(void **)0x0);
    *(undefined4 *)(param_1 + 0xc) = 0;
  }
  sem_destroy((sem_t *)(param_1 + 0x10));
  return;
}

