//==================== FUN_00014e38 @ 00014e38 ====================

char * FUN_00014e38(char *param_1)

{
  int iVar1;
  
  iVar1 = sscanf(param_1,"%d:%d",&DAT_0007b5fc,&DAT_0007b5f8);
  if (iVar1 != 2) {
    return "Invalid time set, should be HH:MM";
  }
  if ((((DAT_0007b5fc < 0x18) && (DAT_0007b5f8 < 0x3c)) && (-1 < DAT_0007b5fc)) &&
     (-1 < DAT_0007b5f8)) {
    DAT_0007b5f0 = 1;
    return (char *)0x0;
  }
  return "Invalid time set.";
}

