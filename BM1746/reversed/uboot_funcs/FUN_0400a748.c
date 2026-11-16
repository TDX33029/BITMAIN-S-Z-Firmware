//==================== FUN_0400a748 @ 0400a748 ====================

void FUN_0400a748(char *param_1,char *param_2,int param_3)

{
  char cVar1;
  
  if (*param_2 == '\"') {
    param_2 = param_2 + 1;
    param_3 = param_3 + -1;
  }
  param_2 = param_2 + -1;
  while (param_3 = param_3 + -1, 0 < param_3) {
    param_2 = param_2 + 1;
    cVar1 = *param_2;
    if (cVar1 == '\"' || cVar1 == '\0') break;
    *param_1 = cVar1;
    param_1 = param_1 + 1;
  }
  *param_1 = '\0';
  return;
}

