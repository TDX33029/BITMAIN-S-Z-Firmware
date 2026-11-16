//==================== FUN_00049578 @ 00049578 ====================

void FUN_00049578(undefined4 param_1)

{
  char cVar1;
  char cVar2;
  char local_1010 [4];
  char acStack_100c [4];
  char acStack_1008 [4];
  char acStack_1004 [4];
  char local_1000 [4];
  char acStack_ffc [4];
  char acStack_ff8 [4];
  undefined2 local_ff4;
  
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    local_1010[0] = 'c';
    local_1010[1] = 'h';
    local_1010[2] = 'e';
    local_1010[3] = 'c';
    acStack_100c[0] = 'k';
    acStack_100c[1] = '_';
    acStack_100c[2] = 'e';
    acStack_100c[3] = 'v';
    acStack_1008[0] = 'e';
    acStack_1008[1] = 'r';
    acStack_1008[2] = 'y';
    acStack_1008[3] = '_';
    acStack_1004[0] = 'c';
    acStack_1004[1] = 'h';
    acStack_1004[2] = 'a';
    acStack_1004[3] = 'i';
    local_1000[0] = 'n';
    local_1000[1] = '_';
    local_1000[2] = 'a';
    local_1000[3] = 's';
    acStack_ffc[0] = 'i';
    acStack_ffc[1] = 'c';
    acStack_ffc[2] = '_';
    acStack_ffc[3] = 'n';
    acStack_ff8[0] = 'u';
    acStack_ff8[1] = 'm';
    acStack_ff8[2] = 'b';
    acStack_ff8[3] = 'e';
    local_ff4 = 0x72;
    FUN_00038730(5,local_1010,0);
  }
  cVar1 = '\0';
  do {
    cVar2 = cVar1 + '\x01';
    FUN_0004952c(cVar1,param_1);
    FUN_0002a92c(100);
    cVar1 = cVar2;
  } while (cVar2 != '\x04');
  return;
}

