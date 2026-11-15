//==================== FUN_00043e4c @ 00043e4c ====================

void FUN_00043e4c(undefined4 param_1)

{
  char cVar1;
  char *pcVar2;
  
  pcVar2 = &DAT_0009d4bb;
  cVar1 = '\0';
  do {
    pcVar2 = pcVar2 + 1;
    if (*pcVar2 == '\x01') {
      FUN_000435e0(cVar1,param_1);
    }
    cVar1 = cVar1 + '\x01';
  } while (cVar1 != '\x04');
  FUN_0002a92c(10);
  return;
}

