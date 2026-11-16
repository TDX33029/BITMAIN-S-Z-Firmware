//==================== FUN_00012164 @ 00012164 ====================

undefined4 FUN_00012164(int param_1)

{
  pthread_t __th;
  
  __th = pthread_self();
  pthread_detach(__th);
  pthread_setcanceltype(1,(int *)0x0);
  FUN_0002f0b4(&DAT_00060570);
  FUN_000120f4();
  FUN_00036764(DAT_00076ef8);
  *(undefined4 *)(param_1 + 0xc) = 0;
  return 0;
}

