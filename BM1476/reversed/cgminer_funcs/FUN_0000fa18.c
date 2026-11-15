//==================== FUN_0000fa18 @ 0000fa18 ====================

undefined4 FUN_0000fa18(char *param_1)

{
  char *__fd;
  char *__format;
  char *pcStack_1014;
  char local_1010 [4];
  char acStack_100c [4];
  char acStack_1008 [4];
  char acStack_1004 [4];
  char local_1000 [4];
  char acStack_ffc [4];
  char acStack_ff8 [4];
  char acStack_ff4 [4];
  char acStack_ff0 [4];
  undefined2 local_fec;
  
  pcStack_1014 = "";
  __fd = (char *)strtol(param_1,&pcStack_1014,10);
  if ((*pcStack_1014 == '\0') && (-1 < (int)__fd)) {
    DAT_00076cf4 = fdopen((int)__fd,"a");
    if (DAT_00076cf4 != (FILE *)0x0) {
      return 0;
    }
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
      return 0;
    }
    __format = "Failed to open fd %u for share log";
    param_1 = __fd;
  }
  else {
    if ((*param_1 == '-') && (param_1[1] == '\0')) {
      DAT_00076cf4 = (FILE *)stdout;
      if (stdout != 0) {
        DAT_00076cf4 = (FILE *)stdout;
        return 0;
      }
      if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
        DAT_00076cf4 = (FILE *)stdout;
        return 0;
      }
      local_1010[0] = 'S';
      local_1010[1] = 't';
      local_1010[2] = 'a';
      local_1010[3] = 'n';
      acStack_100c[0] = 'd';
      acStack_100c[1] = 'a';
      acStack_100c[2] = 'r';
      acStack_100c[3] = 'd';
      acStack_1008[0] = ' ';
      acStack_1008[1] = 'o';
      acStack_1008[2] = 'u';
      acStack_1008[3] = 't';
      acStack_1004[0] = 'p';
      acStack_1004[1] = 'u';
      acStack_1004[2] = 't';
      acStack_1004[3] = ' ';
      local_1000[0] = 'm';
      local_1000[1] = 'i';
      local_1000[2] = 's';
      local_1000[3] = 's';
      acStack_ffc[0] = 'i';
      acStack_ffc[1] = 'n';
      acStack_ffc[2] = 'g';
      acStack_ffc[3] = ' ';
      acStack_ff8[0] = 'f';
      acStack_ff8[1] = 'o';
      acStack_ff8[2] = 'r';
      acStack_ff8[3] = ' ';
      acStack_ff4[0] = 's';
      acStack_ff4[1] = 'h';
      acStack_ff4[2] = 'a';
      acStack_ff4[3] = 'r';
      acStack_ff0[0] = 'e';
      acStack_ff0[1] = ' ';
      acStack_ff0[2] = 'l';
      acStack_ff0[3] = 'o';
      local_fec = 0x67;
      FUN_00038730(3,local_1010,0);
      return 0;
    }
    DAT_00076cf4 = fopen(param_1,"a");
    if (DAT_00076cf4 != (FILE *)0x0) {
      return 0;
    }
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
      return 0;
    }
    __format = "Failed to open %s for share log";
  }
  DAT_00076cf4 = (FILE *)0x0;
  snprintf(local_1010,0x1000,__format,param_1);
  FUN_00038730(3,local_1010,0);
  return 0;
}

