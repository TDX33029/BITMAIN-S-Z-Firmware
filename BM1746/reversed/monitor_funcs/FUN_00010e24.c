//==================== FUN_00010e24 @ 00010e24 ====================

void FUN_00010e24(void)

{
  int iVar1;
  int iVar2;
  int *__ptr;
  
  iVar1 = FUN_00010ddc();
  __ptr = *(int **)(iVar1 + 4);
  if (__ptr == (int *)0x0) {
    puts("there is no cmd for this response.");
    return;
  }
  iVar2 = *__ptr;
  *(int *)(iVar1 + 4) = __ptr[1];
  printf("delete one cmd:%s,nonce:%s.\n",(char *)(iVar2 + 3),(char *)(iVar2 + 0x1008));
  free((void *)*__ptr);
  free(__ptr);
  DAT_0001d440 = DAT_0001d440 + -1;
  return;
}

