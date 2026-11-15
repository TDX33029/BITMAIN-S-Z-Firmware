//==================== FUN_0005bdb8 @ 0005bdb8 ====================

void FUN_0005bdb8(int *param_1,int param_2,int param_3)

{
  memmove((void *)(param_2 + param_3 * 4),(void *)(param_2 + (param_3 + 1) * 4),
          (*param_1 - param_3) * 4);
  *param_1 = *param_1 + -1;
  return;
}

