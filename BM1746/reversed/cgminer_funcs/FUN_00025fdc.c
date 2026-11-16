//==================== FUN_00025fdc @ 00025fdc ====================

char * FUN_00025fdc(undefined4 param_1)

{
  char *pcVar1;
  char local_1018 [4];
  char acStack_1014 [4];
  char acStack_1010 [4];
  char acStack_100c [4];
  char acStack_1008 [4];
  undefined2 uStack_1004;
  undefined1 local_1002;
  
  echo();
  pcVar1 = malloc(0xff);
  if (pcVar1 == (char *)0x0) {
    local_1018[0] = 'F';
    local_1018[1] = 'a';
    local_1018[2] = 'i';
    local_1018[3] = 'l';
    acStack_1014[0] = 'e';
    acStack_1014[1] = 'd';
    acStack_1014[2] = ' ';
    acStack_1014[3] = 't';
    acStack_1010[0] = 'o';
    acStack_1010[1] = ' ';
    acStack_1010[2] = 'm';
    acStack_1010[3] = 'a';
    acStack_100c[0] = 'l';
    acStack_100c[1] = 'l';
    acStack_100c[2] = 'o';
    acStack_100c[3] = 'c';
    acStack_1008[0] = ' ';
    acStack_1008[1] = 'i';
    acStack_1008[2] = 'n';
    acStack_1008[3] = 'p';
    uStack_1004 = 0x7475;
    local_1002 = 0;
    FUN_00038730(3,local_1018,1);
    FUN_00016ca8(1);
  }
  leaveok(DAT_0007b430,0);
  snprintf(local_1018,0x1000,"%s:\n",param_1);
  FUN_00018bc0(local_1018);
  wgetnstr(DAT_0007b430,pcVar1,0xff);
  if (*pcVar1 == '\0') {
    pcVar1[0] = '-';
    pcVar1[1] = '1';
    pcVar1[2] = '\0';
  }
  leaveok(DAT_0007b430,1);
  noecho();
  return pcVar1;
}

