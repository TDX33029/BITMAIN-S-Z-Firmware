//==================== FUN_000121b0 @ 000121b0 ====================

char * FUN_000121b0(undefined4 param_1,float *param_2)

{
  char *pcVar1;
  
  pcVar1 = (char *)FUN_0005b1e0();
  if (pcVar1 != (char *)0x0) {
    return pcVar1;
  }
  if (100.0 <= *param_2) {
    if (*param_2 <= 250.0) {
      return (char *)0x0;
    }
  }
  return "Value out of range";
}

