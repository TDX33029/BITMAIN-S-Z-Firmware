//==================== FUN_00015648 @ 00015648 ====================

void FUN_00015648(void *param_1)

{
  free(*(void **)((int)param_1 + 0x6f0));
  free(*(void **)((int)param_1 + 0x704));
  free(*(void **)((int)param_1 + 0x718));
  free(*(void **)((int)param_1 + 0x710));
  memset(param_1,0,0x7b0);
  return;
}

