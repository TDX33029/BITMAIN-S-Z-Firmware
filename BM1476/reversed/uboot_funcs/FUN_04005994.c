//==================== FUN_04005994 @ 04005994 ====================

void FUN_04005994(int param_1,char *param_2)

{
  char cVar1;
  char *pcVar2;
  
  pcVar2 = (char *)(param_1 + -1);
  do {
    cVar1 = *param_2;
    pcVar2 = pcVar2 + 1;
    *pcVar2 = cVar1;
    param_2 = param_2 + 1;
  } while (cVar1 != '\0');
  return;
}

