//==================== FUN_00046964 @ 00046964 ====================

undefined4 FUN_00046964(void)

{
  int iVar1;
  undefined4 uVar2;
  char acStack_1008 [4096];
  
  DAT_0009d4b0 = calloc(1,0x40);
  iVar1 = FUN_0002f174(DAT_0009d4b0,0,FUN_0004388c,0);
  if (iVar1 == 0) {
    pthread_detach(*(pthread_t *)((int)DAT_0009d4b0 + 0xc));
    FUN_0002a92c(500);
    uVar2 = 0;
  }
  else {
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      snprintf(acStack_1008,0x1000,"%s: create thread for check miner_status",
               "create_bitmain_turbo_mode_monitor_pthread");
      FUN_00038730(7,acStack_1008,0);
    }
    uVar2 = 0xfffffffb;
  }
  return uVar2;
}

