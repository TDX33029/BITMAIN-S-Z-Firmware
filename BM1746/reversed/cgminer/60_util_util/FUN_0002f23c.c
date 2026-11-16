//==================== FUN_0002f23c @ 0002f23c ====================

undefined4 FUN_0002f23c(int param_1)

{
  pthread_setcanceltype(1,(int *)0x0);
  (**(code **)(param_1 + 0x10))(*(undefined4 *)(param_1 + 0x14));
  FUN_0002f1bc(param_1,"util.c","completion_thread",0xf7b);
  return 0;
}

