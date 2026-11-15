//==================== FUN_0400ac64 @ 0400ac64 ====================

char * FUN_0400ac64(char *param_1)

{
  char *pcVar1;
  int iVar2;
  char *pcVar3;
  
  iVar2 = FUN_04005b98();
  pcVar1 = param_1 + iVar2 + -1;
  do {
    pcVar3 = pcVar1;
    if (pcVar3 < param_1) {
      return pcVar3;
    }
    pcVar1 = pcVar3 + -1;
  } while (*pcVar3 != '/');
  return pcVar3 + 1;
}

