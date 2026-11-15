//==================== FUN_000463c8 @ 000463c8 ====================

void FUN_000463c8(void)

{
  char cVar1;
  char cVar2;
  char cVar3;
  char *pcVar4;
  
  pcVar4 = &DAT_0009d4bb;
  cVar1 = '\0';
  do {
    pcVar4 = pcVar4 + 1;
    if (*pcVar4 == '\x01') {
      cVar2 = '\0';
      do {
        cVar3 = cVar2 + '\x01';
        FUN_0004633c(cVar1,cVar2,0x4c,9,4);
        cVar2 = cVar3;
      } while (cVar3 != '\x03');
    }
    cVar1 = cVar1 + '\x01';
  } while (cVar1 != '\x04');
  return;
}

