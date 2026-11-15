//==================== FUN_000598a8 @ 000598a8 ====================

char * FUN_000598a8(char *param_1,char *param_2)

{
  char *pcVar1;
  lconv *plVar2;
  char *pcVar3;
  char *pcVar4;
  char *__dest;
  double in_d0;
  
  pcVar1 = (char *)snprintf(param_1,(size_t)param_2,"%.17g",in_d0);
  if (((int)pcVar1 < 0) || (param_2 <= pcVar1)) {
    return (char *)0xffffffff;
  }
  plVar2 = localeconv();
  if (((byte)*plVar2->decimal_point != 0x2e) &&
     (pcVar3 = strchr(param_1,(uint)(byte)*plVar2->decimal_point), pcVar3 != (char *)0x0)) {
    *pcVar3 = '.';
  }
  pcVar4 = strchr(param_1,0x2e);
  pcVar3 = pcVar1;
  if (pcVar4 == (char *)0x0) {
    pcVar4 = strchr(param_1,0x65);
    if (pcVar4 != (char *)0x0) goto LAB_00059928;
    if (param_2 <= pcVar1 + 3) {
      return (char *)0xffffffff;
    }
    pcVar3 = pcVar1 + 2;
    param_1[(int)pcVar1] = '.';
    (param_1 + (int)pcVar1)[1] = '0';
    param_1[(int)pcVar3] = '\0';
  }
  pcVar4 = strchr(param_1,0x65);
  pcVar1 = pcVar3;
  if (pcVar4 == (char *)0x0) {
    return pcVar3;
  }
LAB_00059928:
  pcVar3 = pcVar4 + 2;
  if (pcVar4[1] == '-') {
    __dest = pcVar3;
  }
  else {
    __dest = pcVar4 + 1;
  }
  if (pcVar4[2] == '0') {
    pcVar4 = pcVar4 + 3;
    do {
      pcVar3 = pcVar4;
      pcVar4 = pcVar3 + 1;
    } while (*pcVar3 == '0');
  }
  if (pcVar3 != __dest) {
    memmove(__dest,pcVar3,(size_t)(param_1 + ((int)pcVar1 - (int)pcVar3)));
    pcVar1 = __dest + ((int)pcVar1 - (int)pcVar3);
  }
  return pcVar1;
}

