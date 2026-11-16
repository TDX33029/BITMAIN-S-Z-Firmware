//==================== FUN_0000e49c @ 0000e49c ====================

undefined4 FUN_0000e49c(undefined4 *param_1)

{
  if ((void *)*param_1 != (void *)0x0) {
    free((void *)*param_1);
  }
  *param_1 = 0;
  param_1[2] = 0;
  param_1[1] = 0;
  return 1;
}

