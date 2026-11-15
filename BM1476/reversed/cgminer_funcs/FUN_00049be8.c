//==================== FUN_00049be8 @ 00049be8 ====================

undefined4 FUN_00049be8(void)

{
  int iVar1;
  undefined4 uVar2;
  char acStack_1008 [4096];
  
  DAT_0007c12c = calloc(1,0x40);
  iVar1 = FUN_0002f174(DAT_0007c12c,0,&LAB_00048a18,DAT_0007c12c);
  if (iVar1 == 0) {
    pthread_detach(*(pthread_t *)((int)DAT_0007c12c + 0xc));
    uVar2 = FUN_0002a92c(500);
  }
  else {
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      snprintf(acStack_1008,0x1000,"%s: create thread for read temp",
               "create_bitmain_read_temp_pthread");
      FUN_00038730(7,acStack_1008,0);
    }
    uVar2 = 0xfffffff9;
  }
  return uVar2;
}

