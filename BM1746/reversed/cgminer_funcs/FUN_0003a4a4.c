//==================== FUN_0003a4a4 @ 0003a4a4 ====================

undefined8 FUN_0003a4a4(void *param_1)

{
  pthread_t local_c;
  
  DAT_00078e18 = 0;
  pthread_create(&local_c,(pthread_attr_t *)0x0,FUN_0003af90,param_1);
  pthread_join(local_c,(void **)0x0);
  return DAT_00078e18;
}

