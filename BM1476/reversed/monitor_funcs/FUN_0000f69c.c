//==================== FUN_0000f69c @ 0000f69c ====================

void FUN_0000f69c(undefined4 param_1,char *param_2)

{
  char *__dest;
  char *pcVar1;
  
  __dest = calloc(0x3c,1);
  pcVar1 = (char *)FUN_0000e1f8(&DAT_0001e44c);
  pcVar1 = stpcpy(__dest,pcVar1);
  builtin_strncpy(pcVar1,"ID=",4);
  pcVar1 = stpcpy(pcVar1 + 3,(char *)&DAT_00026ab8);
  builtin_strncpy(pcVar1,",result=",8);
  pcVar1 = stpcpy(pcVar1 + 8,param_2);
  FUN_0000ecd0(param_1,(int)pcVar1 - (int)__dest);
  DAT_00026f90 = (*DAT_0001d43c)(DAT_00026f98,DAT_000258bc,__dest,(int)pcVar1 - (int)__dest);
  if (DAT_00026f90 == -1) {
    perror("send");
  }
  printf("send cgminer state :%s\n",__dest);
  free(__dest);
  return;
}

