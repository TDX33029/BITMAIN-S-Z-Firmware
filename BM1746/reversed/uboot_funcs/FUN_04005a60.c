//==================== FUN_04005a60 @ 04005a60 ====================

int FUN_04005a60(int param_1,char *param_2)

{
  char *pcVar1;
  int iVar2;
  
  pcVar1 = (char *)(param_1 + -1);
  do {
    pcVar1 = pcVar1 + 1;
    iVar2 = (int)(char)(*pcVar1 - *param_2);
    if (iVar2 != 0) {
      return iVar2;
    }
    param_2 = param_2 + 1;
  } while (*pcVar1 != '\0');
  return 0;
}

