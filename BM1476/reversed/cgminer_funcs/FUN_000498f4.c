//==================== FUN_000498f4 @ 000498f4 ====================

undefined4 FUN_000498f4(void)

{
  int iVar1;
  undefined4 uVar2;
  char acStack_1008 [4096];
  
  DAT_0007c284 = calloc(1,0x40);
  iVar1 = FUN_0002f174(DAT_0007c284,0,FUN_0003aa9c,0);
  if (iVar1 == 0) {
    pthread_detach(*(pthread_t *)((int)DAT_0007c284 + 0xc));
    uVar2 = FUN_0002a92c(500);
  }
  else {
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      snprintf(acStack_1008,0x1000,"%s: create thread for check miner_status",
               "create_bitmain_check_fan_pthread");
      FUN_00038730(7,acStack_1008,0);
    }
    uVar2 = 0xfffffff8;
  }
  return uVar2;
}

