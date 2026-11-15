//==================== FUN_0002a240 @ 0002a240 ====================

bool FUN_0002a240(int param_1,undefined4 param_2)

{
  undefined4 *__ptr;
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  bool bVar4;
  
  __ptr = calloc(1,0xc);
  if (__ptr == (undefined4 *)0x0) {
    bVar4 = false;
  }
  else {
    puVar3 = __ptr + 1;
    *__ptr = param_2;
    __ptr[1] = puVar3;
    __ptr[2] = puVar3;
    iVar1 = pthread_mutex_lock((pthread_mutex_t *)(param_1 + 0xc));
    if (iVar1 != 0) {
      FUN_0000b708("tq_push",0x4f0);
    }
    bVar4 = *(char *)(param_1 + 8) != '\0';
    if (bVar4) {
      free(__ptr);
    }
    else {
      puVar2 = *(undefined4 **)(param_1 + 4);
      *(undefined4 **)(param_1 + 4) = puVar3;
      __ptr[1] = param_1;
      __ptr[2] = puVar2;
      *puVar2 = puVar3;
    }
    bVar4 = !bVar4;
    pthread_cond_signal((pthread_cond_t *)(param_1 + 0x28));
    FUN_00029388((pthread_mutex_t *)(param_1 + 0xc),"tq_push",0x4fb);
    (*DAT_00075ed8)();
  }
  return bVar4;
}

