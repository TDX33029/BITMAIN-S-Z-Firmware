//==================== FUN_00029690 @ 00029690 ====================

char * FUN_00029690(int param_1)

{
  int *piVar1;
  int *piVar2;
  undefined *puVar3;
  
  if (param_1 == 0) {
    return "http:";
  }
  puVar3 = &UNK_00064e3c;
  do {
    piVar2 = (int *)(puVar3 + 8);
    if ((char *)*piVar2 == (char *)0x0) {
      return "invalid";
    }
    piVar1 = (int *)(puVar3 + 0xc);
    puVar3 = puVar3 + 8;
  } while (*piVar1 != param_1);
  return (char *)*piVar2;
}

