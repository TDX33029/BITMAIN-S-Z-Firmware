//==================== FUN_000416ec @ 000416ec ====================

int FUN_000416ec(void)

{
  int iVar1;
  char acStack_1008 [4096];
  
  DAT_0009d4ac = calloc(1,0x40);
  iVar1 = FUN_0002f174(DAT_0009d4ac,0,&LAB_00040034,DAT_0009d4ac);
  if (iVar1 == 0) {
    iVar1 = pthread_detach(*(pthread_t *)((int)DAT_0009d4ac + 0xc));
  }
  else {
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
      snprintf(acStack_1008,0x1000,"%s: create thread error for pic_heart_beat_func",
               "send_heart_beat_to_every_chain");
      FUN_00038730(3,acStack_1008,0);
    }
    iVar1 = -3;
  }
  return iVar1;
}

