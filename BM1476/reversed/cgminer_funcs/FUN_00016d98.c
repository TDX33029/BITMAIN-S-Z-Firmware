//==================== FUN_00016d98 @ 00016d98 ====================

char * FUN_00016d98(char *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int *piVar1;
  size_t sVar2;
  size_t sVar3;
  char *param0;
  undefined1 auStack_110c [92];
  char acStack_10b0 [160];
  char local_1010 [4];
  char acStack_100c [4];
  char acStack_1008 [4];
  char acStack_1004 [4];
  char local_1000 [4];
  char acStack_ffc [4];
  char acStack_ff8 [4];
  undefined1 local_ff4;
  undefined4 local_10;
  
  local_10 = param_4;
  if (DAT_00076f24 == 0) {
    DAT_00076f24 = __strdup();
  }
  DAT_00076f28 = DAT_00076f28 + 1;
  if (10 < DAT_00076f28) {
    return "Too many levels of JSON includes (limit 10) or a loop";
  }
  piVar1 = (int *)FUN_0005940c(param_1,0,auStack_110c,local_10);
  if ((piVar1 == (int *)0x0) || (*piVar1 != 0)) {
    sVar2 = strlen(param_1);
    sVar3 = strlen(acStack_10b0);
    sVar2 = sVar2 + sVar3 + 0x23;
    param0 = malloc(sVar2);
    if (param0 == (char *)0x0) {
      local_1010[0] = 'M';
      local_1010[1] = 'a';
      local_1010[2] = 'l';
      local_1010[3] = 'l';
      acStack_100c[0] = 'o';
      acStack_100c[1] = 'c';
      acStack_100c[2] = ' ';
      acStack_100c[3] = 'f';
      acStack_1008[0] = 'a';
      acStack_1008[1] = 'i';
      acStack_1008[2] = 'l';
      acStack_1008[3] = 'u';
      acStack_1004[0] = 'r';
      acStack_1004[1] = 'e';
      acStack_1004[2] = ' ';
      acStack_1004[3] = 'i';
      local_1000[0] = 'n';
      local_1000[1] = ' ';
      local_1000[2] = 'j';
      local_1000[3] = 's';
      acStack_ffc[0] = 'o';
      acStack_ffc[1] = 'n';
      acStack_ffc[2] = ' ';
      acStack_ffc[3] = 'e';
      acStack_ff8[0] = 'r';
      acStack_ff8[1] = 'r';
      acStack_ff8[2] = 'o';
      acStack_ff8[3] = 'r';
      local_ff4 = 0;
      FUN_00038730(3,local_1010,1);
      FUN_00016ca8(1);
    }
    snprintf(param0,sVar2,"JSON decode of file \'%s\' failed\n %s",param_1,acStack_10b0);
  }
  else {
    DAT_00076f20 = 1;
    param0 = (char *)FUN_00016dec(piVar1,1);
  }
  return param0;
}

