//==================== FUN_00014eb4 @ 00014eb4 ====================

char * FUN_00014eb4(char *param_1)

{
  int iVar1;
  
  iVar1 = sscanf(param_1,"%d:%d",&DAT_0007b880,&DAT_0007b87c);
  if (iVar1 != 2) {
    return "Invalid time set, should be HH:MM";
  }
  if ((((DAT_0007b880 < 0x18) && (DAT_0007b87c < 0x3c)) && (-1 < DAT_0007b880)) &&
     (-1 < DAT_0007b87c)) {
    DAT_0007b874 = 1;
    return (char *)0x0;
  }
  return "Invalid time set.";
}

