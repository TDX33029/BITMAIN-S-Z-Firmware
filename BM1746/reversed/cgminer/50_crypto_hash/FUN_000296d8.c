//==================== FUN_000296d8 @ 000296d8 ====================

void FUN_000296d8(char *param_1,byte *param_2,int param_3)

{
  char *pcVar1;
  
  if (param_3 < 1) {
    pcVar1 = param_1;
  }
  else {
    pcVar1 = param_1 + param_3 * 2;
    do {
      *param_1 = "0123456789abcdefbin2hex"[*param_2 >> 4];
      param_1[1] = "0123456789abcdefbin2hex"[*param_2 & 0xf];
      param_1 = param_1 + 2;
      param_2 = param_2 + 1;
    } while (pcVar1 != param_1);
  }
  *pcVar1 = '\0';
  return;
}

