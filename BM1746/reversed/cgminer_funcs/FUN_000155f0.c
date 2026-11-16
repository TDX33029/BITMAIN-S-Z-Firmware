//==================== FUN_000155f0 @ 000155f0 ====================

char * FUN_000155f0(char *param_1)

{
  uint uVar1;
  
  if ((param_1 == (char *)0x0) || (*param_1 == '\0')) {
    return "Invalid parameters for set temp cutoff";
  }
  uVar1 = strtol(param_1,(char **)0x0,10);
  if (uVar1 < 0xc9) {
    DAT_00076f00 = param_1;
    return (char *)0x0;
  }
  return "Invalid value passed to set temp cutoff";
}

