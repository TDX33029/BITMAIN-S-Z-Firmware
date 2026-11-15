//==================== FUN_0002f438 @ 0002f438 ====================

bool FUN_0002f438(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  bool bVar1;
  void *__arg;
  int iVar2;
  pthread_t local_1c [2];
  
  __arg = malloc(0x18);
  if (__arg == (void *)0x0) {
    bVar1 = false;
  }
  else {
    FUN_0002f0f4(__arg,"util.c","cg_completion_timeout",0xf89);
    *(undefined4 *)((int)__arg + 0x10) = param_1;
    *(undefined4 *)((int)__arg + 0x14) = param_2;
    pthread_create(local_1c,(pthread_attr_t *)0x0,FUN_0002f23c,__arg);
    iVar2 = FUN_0002f304(__arg,param_3,"util.c","cg_completion_timeout",0xf8f);
    if (iVar2 != 0) {
      pthread_cancel(local_1c[0]);
    }
    else {
      pthread_join(local_1c[0],(void **)0x0);
      free(__arg);
    }
    bVar1 = iVar2 == 0;
  }
  return bVar1;
}

