//==================== FUN_00028a40 @ 00028a40 ====================

void * FUN_00028a40(void *param_1)

{
  void *__dest;
  char *param4;
  char acStack_1010 [4096];
  
  __dest = malloc(0x80);
  if (__dest == (void *)0x0) {
    param4 = "original";
    if (*(char *)((int)param_1 + 100) != '\0') {
      param4 = "copy";
    }
    snprintf(acStack_1010,0x1000,"Failed to allocate device_drv copy of %s (%s)",
             *(char **)((int)param_1 + 8),param4);
    FUN_00038730(3,acStack_1010,1);
    FUN_00016ca8(1);
  }
  memcpy(__dest,param_1,0x80);
  *(undefined1 *)((int)__dest + 100) = 1;
  return __dest;
}

