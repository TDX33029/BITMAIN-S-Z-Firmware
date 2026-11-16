//==================== FUN_04005c20 @ 04005c20 ====================

int FUN_04005c20(char *param_1)

{
  char cVar1;
  char *pcVar2;
  int iVar3;
  
  if (param_1 != (char *)0x0) {
    if (*param_1 == '\0') {
      pcVar2 = (char *)0x1;
    }
    else {
      pcVar2 = param_1;
      do {
        pcVar2 = pcVar2 + 1;
      } while (*pcVar2 != '\0');
      pcVar2 = pcVar2 + (1 - (int)param_1);
    }
    iVar3 = func_0x03fc87b0(pcVar2);
    if (iVar3 != 0) {
      pcVar2 = (char *)(iVar3 + -1);
      do {
        cVar1 = *param_1;
        pcVar2 = pcVar2 + 1;
        *pcVar2 = cVar1;
        param_1 = param_1 + 1;
      } while (cVar1 != '\0');
      return iVar3;
    }
  }
  return 0;
}

