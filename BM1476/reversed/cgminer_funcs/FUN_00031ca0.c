//==================== FUN_00031ca0 @ 00031ca0 ====================

undefined4 FUN_00031ca0(int param_1)

{
  pthread_t __th;
  
  __th = pthread_self();
  pthread_detach(__th);
  pthread_setcanceltype(1,(int *)0x0);
  FUN_0002f0b4("APIMcast");
  FUN_0003107c();
  *(undefined4 *)(param_1 + 0xc) = 0;
  return 0;
}

