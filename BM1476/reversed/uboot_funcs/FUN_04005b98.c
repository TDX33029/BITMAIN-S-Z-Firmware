//==================== FUN_04005b98 @ 04005b98 ====================

int FUN_04005b98(char *param_1)

{
  char *pcVar1;
  
  pcVar1 = param_1;
  if (*param_1 == '\0') {
    return 0;
  }
  do {
    pcVar1 = pcVar1 + 1;
  } while (*pcVar1 != '\0');
  return (int)pcVar1 - (int)param_1;
}

