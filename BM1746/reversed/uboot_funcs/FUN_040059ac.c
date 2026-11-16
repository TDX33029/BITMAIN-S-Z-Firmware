//==================== FUN_040059ac @ 040059ac ====================

void FUN_040059ac(int param_1,char *param_2,int param_3)

{
  char cVar1;
  char *pcVar3;
  char *pcVar2;
  char *pcVar4;
  
  pcVar4 = (char *)(param_1 + -1);
  pcVar3 = param_2;
  if (param_2 == param_2 + param_3) {
    return;
  }
  do {
    pcVar2 = pcVar3 + 1;
    cVar1 = *pcVar3;
    pcVar4 = pcVar4 + 1;
    *pcVar4 = cVar1;
    if (cVar1 == '\0') {
      return;
    }
    pcVar3 = pcVar2;
  } while (pcVar2 != param_2 + param_3);
  return;
}

