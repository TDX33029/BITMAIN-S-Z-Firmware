//==================== FUN_00059d20 @ 00059d20 ====================

void * FUN_00059d20(int param_1,int param_2,int param_3)

{
  void *__dest;
  uint uVar1;
  void *__src;
  uint uVar2;
  
  uVar1 = *(uint *)(param_1 + 8);
  if (uVar1 < (uint)(param_2 + *(int *)(param_1 + 0xc))) {
    uVar2 = uVar1 << 1;
    if (uVar2 <= param_2 + uVar1) {
      uVar2 = param_2 + uVar1;
    }
    __src = *(void **)(param_1 + 0x10);
    __dest = (void *)FUN_0005956c(uVar2 << 2);
    if (__dest == (void *)0x0) {
      return (void *)0x0;
    }
    *(uint *)(param_1 + 8) = uVar2;
    *(void **)(param_1 + 0x10) = __dest;
    if (param_3 == 0) {
      return __src;
    }
    memcpy(__dest,__src,*(int *)(param_1 + 0xc) << 2);
    FUN_00059594(__src);
  }
  return *(void **)(param_1 + 0x10);
}

