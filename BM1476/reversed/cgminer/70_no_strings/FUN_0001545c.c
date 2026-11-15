//==================== FUN_0001545c @ 0001545c ====================

char * FUN_0001545c(undefined4 param_1,int *param_2,int param_3,int param_4)

{
  char *pcVar1;
  
  pcVar1 = (char *)FUN_0005b2e8();
  if (pcVar1 != (char *)0x0) {
    return pcVar1;
  }
  if ((*param_2 < param_3) || (param_4 < *param_2)) {
    return "Value out of range";
  }
  return (char *)0x0;
}

