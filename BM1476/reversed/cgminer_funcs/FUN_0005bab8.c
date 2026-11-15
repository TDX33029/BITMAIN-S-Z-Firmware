//==================== FUN_0005bab8 @ 0005bab8 ====================

char * FUN_0005bab8(undefined4 param_1)

{
  char *pcVar1;
  undefined4 local_14 [2];
  
  local_14[0] = 0;
  pcVar1 = (char *)FUN_0005b5ec(param_1,local_14);
  while( true ) {
    if (pcVar1 == (char *)0x0) {
      return (char *)0x0;
    }
    if (*pcVar1 != '-') break;
    pcVar1 = (char *)FUN_0005b6d4(pcVar1,param_1,local_14);
  }
  return pcVar1;
}

