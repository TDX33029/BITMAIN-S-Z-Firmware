//==================== FUN_04005bc4 @ 04005bc4 ====================

int FUN_04005bc4(char *param_1,int param_2)

{
  char *pcVar1;
  
  if (param_2 == 0) {
    return 0;
  }
  if (*param_1 == '\0') {
    return 0;
  }
  pcVar1 = param_1;
  do {
    pcVar1 = pcVar1 + 1;
    if (param_1 + param_2 == pcVar1) {
      return (int)(param_1 + param_2) - (int)param_1;
    }
  } while (*pcVar1 != '\0');
  return (int)pcVar1 - (int)param_1;
}

