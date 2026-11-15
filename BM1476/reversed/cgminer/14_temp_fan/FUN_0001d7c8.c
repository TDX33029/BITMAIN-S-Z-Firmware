//==================== FUN_0001d7c8 @ 0001d7c8 ====================

void FUN_0001d7c8(void)

{
  char local_1010 [4];
  char acStack_100c [4];
  char acStack_1008 [4];
  char acStack_1004 [4];
  char local_1000 [4];
  char acStack_ffc [4];
  char acStack_ff8 [4];
  undefined2 local_ff4;
  
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
    snprintf(local_1010,0x1000,"Attempting to restart %s",&DAT_00076bf4);
    FUN_00038730(4,local_1010,0);
  }
  FUN_0002f438(&LAB_0001b4c0,0,5000);
  FUN_0002a84c(&DAT_00077148);
  FUN_00019adc();
  if (0 < DAT_0007728c) {
    kill(DAT_0007728c,0xf);
    DAT_0007728c = 0;
  }
  execv(*DAT_00077290,DAT_00077290);
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
    local_1010[0] = 'F';
    local_1010[1] = 'a';
    local_1010[2] = 'i';
    local_1010[3] = 'l';
    acStack_100c[0] = 'e';
    acStack_100c[1] = 'd';
    acStack_100c[2] = ' ';
    acStack_100c[3] = 't';
    acStack_1008[0] = 'o';
    acStack_1008[1] = ' ';
    acStack_1008[2] = 'r';
    acStack_1008[3] = 'e';
    acStack_1004[0] = 's';
    acStack_1004[1] = 't';
    acStack_1004[2] = 'a';
    acStack_1004[3] = 'r';
    local_1000[0] = 't';
    local_1000[1] = ' ';
    local_1000[2] = 'a';
    local_1000[3] = 'p';
    acStack_ffc[0] = 'p';
    acStack_ffc[1] = 'l';
    acStack_ffc[2] = 'i';
    acStack_ffc[3] = 'c';
    acStack_ff8[0] = 'a';
    acStack_ff8[1] = 't';
    acStack_ff8[2] = 'i';
    acStack_ff8[3] = 'o';
    local_ff4 = 0x6e;
    FUN_00038730(4,local_1010,0);
  }
  return;
}

