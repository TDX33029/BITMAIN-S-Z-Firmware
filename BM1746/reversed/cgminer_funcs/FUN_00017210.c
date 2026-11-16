//==================== FUN_00017210 @ 00017210 ====================

void FUN_00017210(void)

{
  int iVar1;
  pthread_t apStack_c [2];
  
  FUN_0002a84c(&DAT_00076f04);
  iVar1 = pthread_create(apStack_c,(pthread_attr_t *)0x0,FUN_000231f0,(void *)0x0);
  if (iVar1 != 0) {
    FUN_0000b3c8();
  }
  return;
}

