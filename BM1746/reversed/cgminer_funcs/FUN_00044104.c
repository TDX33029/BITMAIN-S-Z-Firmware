//==================== FUN_00044104 @ 00044104 ====================

void FUN_00044104(undefined4 param_1)

{
  char cVar1;
  char *pcVar2;
  
  pcVar2 = &DAT_0009d4bb;
  cVar1 = '\0';
  do {
    pcVar2 = pcVar2 + 1;
    if (*pcVar2 == '\x01') {
      FUN_00044004(cVar1,param_1);
    }
    cVar1 = cVar1 + '\x01';
  } while (cVar1 != '\x04');
  return;
}

