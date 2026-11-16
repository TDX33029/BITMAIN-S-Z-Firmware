//==================== FUN_0005ba08 @ 0005ba08 ====================

char * FUN_0005ba08(undefined4 param_1,int *param_2,undefined4 param_3,undefined4 param_4)

{
  char *pcVar1;
  
  pcVar1 = (char *)FUN_0005b5ec();
  while( true ) {
    if (pcVar1 == (char *)0x0) {
      return (char *)0x0;
    }
    if (*pcVar1 == '-') break;
    pcVar1 = (char *)FUN_0005b6d4(pcVar1,param_1,param_2,*pcVar1,param_4);
  }
  *param_2 = *param_2 + -1;
  return pcVar1 + 1;
}

