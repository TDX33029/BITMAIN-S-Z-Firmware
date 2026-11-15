//==================== FUN_04005a90 @ 04005a90 ====================

int FUN_04005a90(char *param_1,char *param_2,int param_3)

{
  char *pcVar1;
  int iVar2;
  
  if (param_3 == 0) {
    return 0;
  }
  iVar2 = (int)(char)(*param_1 - *param_2);
  if (iVar2 != 0) {
    return iVar2;
  }
  if (*param_1 != '\0') {
    pcVar1 = param_2 + param_3;
    while (param_2 = param_2 + 1, pcVar1 != param_2) {
      param_1 = param_1 + 1;
      iVar2 = (int)(char)(*param_1 - *param_2);
      if (iVar2 != 0) {
        return iVar2;
      }
      if (*param_1 == '\0') {
        return 0;
      }
    }
  }
  return 0;
}

