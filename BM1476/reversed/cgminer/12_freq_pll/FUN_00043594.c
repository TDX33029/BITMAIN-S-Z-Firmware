//==================== FUN_00043594 @ 00043594 ====================

void FUN_00043594(undefined4 param_1)

{
  char cVar1;
  char *pcVar2;
  
  pcVar2 = &DAT_0009d4bb;
  cVar1 = '\0';
  do {
    pcVar2 = pcVar2 + 1;
    if (*pcVar2 == '\x01') {
      FUN_000434a0(cVar1,param_1);
    }
    cVar1 = cVar1 + '\x01';
  } while (cVar1 != '\x04');
  FUN_0002a92c(10);
  return;
}

