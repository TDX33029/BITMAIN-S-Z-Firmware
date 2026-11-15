//==================== FUN_000499d4 @ 000499d4 ====================

undefined4 FUN_000499d4(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  char acStack_1010 [4100];
  
  DAT_0007c060 = calloc(1,0x40);
  iVar1 = FUN_0002f174(DAT_0007c060,0,FUN_00044b5c,param_1);
  if (iVar1 == 0) {
    pthread_detach(*(pthread_t *)((int)DAT_0007c060 + 0xc));
    uVar2 = FUN_0002a92c(500);
  }
  else {
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      snprintf(acStack_1010,0x1000,"%s: create thread for check miner_status",
               "create_bitmain_check_miner_status_pthread");
      FUN_00038730(7,acStack_1010,0);
    }
    uVar2 = 0xfffffffb;
  }
  return uVar2;
}

