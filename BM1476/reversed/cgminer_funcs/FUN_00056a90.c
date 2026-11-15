//==================== FUN_00056a90 @ 00056a90 ====================

void FUN_00056a90(void)

{
  int param3;
  pthread_t pStack_1030;
  pthread_attr_t pStack_102c;
  char acStack_1008 [4100];
  
  pthread_attr_init(&pStack_102c);
  pthread_attr_setstacksize(&pStack_102c,0x200000);
  param3 = pthread_create(&pStack_1030,&pStack_102c,FUN_0005664c,(void *)0x0);
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    snprintf(acStack_1008,0x1000,"httpListenThread start ret=%d",param3);
    FUN_00038730(5,acStack_1008,0);
  }
  return;
}

