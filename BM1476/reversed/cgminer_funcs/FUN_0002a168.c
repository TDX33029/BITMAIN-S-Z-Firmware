//==================== FUN_0002a168 @ 0002a168 ====================

void * FUN_0002a168(void)

{
  void *pvVar1;
  
  pvVar1 = calloc(1,0x58);
  if (pvVar1 != (void *)0x0) {
    *(void **)pvVar1 = pvVar1;
    *(void **)((int)pvVar1 + 4) = pvVar1;
    pthread_mutex_init((pthread_mutex_t *)((int)pvVar1 + 0xc),(pthread_mutexattr_t *)0x0);
    pthread_cond_init((pthread_cond_t *)((int)pvVar1 + 0x28),(pthread_condattr_t *)0x0);
  }
  return pvVar1;
}

