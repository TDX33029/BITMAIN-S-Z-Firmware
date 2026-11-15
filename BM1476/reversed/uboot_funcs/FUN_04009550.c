//==================== FUN_04009550 @ 04009550 ====================

void FUN_04009550(char *param_1,char *param_2)

{
  char cVar1;
  char *pcVar2;
  char *local_14;
  
  pcVar2 = param_2 + -1;
  local_14 = param_2;
  do {
    if (param_1 == (char *)0x0) {
      cVar1 = '\0';
    }
    else {
      cVar1 = FUN_04007718(param_1,&local_14,0x10);
    }
    pcVar2 = pcVar2 + 1;
    *pcVar2 = cVar1;
    if ((param_1 != (char *)0x0) && (param_1 = local_14, *local_14 != '\0')) {
      param_1 = local_14 + 1;
    }
  } while (param_2 + 5 != pcVar2);
  return;
}

