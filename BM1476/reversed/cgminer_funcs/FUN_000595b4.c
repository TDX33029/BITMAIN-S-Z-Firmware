//==================== FUN_000595b4 @ 000595b4 ====================

void * FUN_000595b4(char *param_1)

{
  size_t sVar1;
  void *__dest;
  
  sVar1 = strlen(param_1);
  if (((sVar1 == 0xffffffff) || (sVar1 = sVar1 + 1, sVar1 == 0)) ||
     (__dest = (void *)(*DAT_00076ba0)(sVar1), __dest == (void *)0x0)) {
    return (void *)0x0;
  }
  memcpy(__dest,param_1,sVar1);
  return __dest;
}

