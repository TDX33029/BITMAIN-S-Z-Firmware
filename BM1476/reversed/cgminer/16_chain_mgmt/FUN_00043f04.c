//==================== FUN_00043f04 @ 00043f04 ====================

void FUN_00043f04(uint param_1)

{
  char cVar1;
  char *pcVar2;
  char acStack_1018 [4100];
  
  DAT_0009d6f5 = (undefined1)param_1;
  if ((DAT_00078e09 != '\0') &&
     (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
    snprintf(acStack_1018,0x1000,"%s: bt8d = %d","set_baud",param_1);
    FUN_00038730(7,acStack_1018,0);
  }
  pcVar2 = &DAT_0009d4bb;
  cVar1 = '\0';
  do {
    pcVar2 = pcVar2 + 1;
    if (*pcVar2 == '\x01') {
      FUN_00043e98(cVar1,param_1);
    }
    cVar1 = cVar1 + '\x01';
  } while (cVar1 != '\x04');
  sleep(1);
  FUN_0003bb44(param_1 & 0xff);
  FUN_0002a92c(10);
  return;
}

