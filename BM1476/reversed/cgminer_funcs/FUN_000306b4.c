//==================== FUN_000306b4 @ 000306b4 ====================

void FUN_000306b4(int param_1,char *param_2)

{
  size_t param3;
  undefined4 *puVar1;
  void *__ptr;
  int iVar2;
  int param5;
  char acStack_1028 [4100];
  
  puVar1 = *(undefined4 **)(param_1 + 0xc);
  param3 = strlen(param_2);
  iVar2 = puVar1[1];
  __ptr = (void *)*puVar1;
  if ((uint)puVar1[2] < iVar2 + 1 + param3) {
    param5 = (param3 + 0x1001) - (param3 + 1 & 0xfff);
    __ptr = realloc(__ptr,param5 + puVar1[2]);
    *puVar1 = __ptr;
    if (__ptr == (void *)0x0) {
      snprintf(acStack_1028,0x1000,"OOM buf siz=%d tot=%d ext=%d in %s %s():%d",param3,
               *(int *)(*(int *)(param_1 + 0xc) + 8),param5,"api.c","add_item_buf",0x49b);
      FUN_00038730(3,acStack_1028,1);
      FUN_00016ca8(1);
    }
    *(int *)(*(int *)(param_1 + 0xc) + 8) = *(int *)(*(int *)(param_1 + 0xc) + 8) + param5;
  }
  memcpy((void *)((int)__ptr + iVar2),param_2,param3 + 1);
  *(size_t *)(*(int *)(param_1 + 0xc) + 4) = *(int *)(*(int *)(param_1 + 0xc) + 4) + param3;
  return;
}

