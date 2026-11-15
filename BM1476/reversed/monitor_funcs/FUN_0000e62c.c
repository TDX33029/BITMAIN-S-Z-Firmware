//==================== FUN_0000e62c @ 0000e62c ====================

undefined4 FUN_0000e62c(int *param_1,void *param_2)

{
  if (param_1[1] == param_1[2]) {
    return 0;
  }
  memcpy(param_2,(void *)(*param_1 + param_1[1] * 0x100f),0x100f);
  param_1[1] = (param_1[1] + 1) % 9;
  return 1;
}

