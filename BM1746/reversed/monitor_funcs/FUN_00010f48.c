//==================== FUN_00010f48 @ 00010f48 ====================

void FUN_00010f48(void *param_1)

{
  undefined4 *__ptr;
  undefined4 *puVar1;
  
  __ptr = *(undefined4 **)((int)param_1 + 4);
  *(undefined4 *)((int)param_1 + 4) = 0;
  while (__ptr != (undefined4 *)0x0) {
    puVar1 = (undefined4 *)__ptr[1];
    free((void *)*__ptr);
    free(__ptr);
    __ptr = puVar1;
  }
  free(param_1);
  return;
}

