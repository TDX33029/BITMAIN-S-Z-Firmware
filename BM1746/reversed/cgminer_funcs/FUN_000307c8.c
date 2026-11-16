//==================== FUN_000307c8 @ 000307c8 ====================

undefined4 FUN_000307c8(uint *param_1,char *param_2)

{
  size_t sVar1;
  void *pvVar2;
  uint uVar3;
  void *__dest;
  size_t __size;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  
  sVar1 = strlen(param_2);
  __dest = (void *)param_1[2];
  iVar4 = (int)__dest - (int)param_1[1];
  uVar3 = iVar4 + 0xc + sVar1;
  if (*param_1 < uVar3) {
    __size = *param_1 + 0x20000;
    if (__size < uVar3) {
      uVar5 = VectorFloatToUnsignedFixed(uVar3,0x20,0x10);
      iVar6 = VectorFloatToUnsigned(uVar5,3);
      __size = (iVar6 + 2) * 0x10000;
    }
    pvVar2 = realloc((void *)param_1[1],__size);
    *param_1 = __size;
    __dest = (void *)((int)pvVar2 + iVar4);
    param_1[1] = (uint)pvVar2;
    param_1[2] = (uint)__dest;
  }
  memcpy(__dest,param_2,sVar1 + 1);
  param_1[2] = param_1[2] + sVar1;
  return 1;
}

