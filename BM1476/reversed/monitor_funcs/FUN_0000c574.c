//==================== FUN_0000c574 @ 0000c574 ====================

void * FUN_0000c574(char *param_1)

{
  size_t sVar1;
  void *__dest;
  
  sVar1 = strlen(param_1);
  __dest = (void *)(*DAT_0001b374)(sVar1 + 1);
  if (__dest != (void *)0x0) {
    memcpy(__dest,param_1,sVar1 + 1);
  }
  return __dest;
}

