//==================== FUN_0002a1a8 @ 0002a1a8 ====================

void FUN_0002a1a8(undefined4 *param_1)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  
  if (param_1 == (undefined4 *)0x0) {
    return;
  }
  puVar1 = (undefined4 *)*param_1;
  puVar2 = *(undefined4 **)*param_1;
  while (param_1 != puVar1) {
    puVar3 = (undefined4 *)puVar1[1];
    puVar2[1] = puVar3;
    *puVar3 = puVar2;
    *puVar1 = 0;
    puVar1[1] = 0;
    free(puVar1 + -1);
    puVar1 = puVar2;
    puVar2 = (undefined4 *)*puVar2;
  }
  pthread_cond_destroy((pthread_cond_t *)(param_1 + 10));
  pthread_mutex_destroy((pthread_mutex_t *)(param_1 + 3));
  memset(param_1,0,0x58);
  free(param_1);
  return;
}

