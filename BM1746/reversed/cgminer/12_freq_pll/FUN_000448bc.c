//==================== FUN_000448bc @ 000448bc ====================

void FUN_000448bc(undefined4 param_1)

{
  char cVar1;
  char cVar2;
  char local_1010 [4];
  char acStack_100c [4];
  undefined1 local_1008;
  
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    local_1010[0] = 't';
    local_1010[1] = 't';
    local_1010[2] = 'y';
    local_1010[3] = '_';
    acStack_100c[0] = 'i';
    acStack_100c[1] = 'n';
    acStack_100c[2] = 'i';
    acStack_100c[3] = 't';
    local_1008 = 0;
    FUN_00038730(5,local_1010,0);
  }
  cVar1 = '\0';
  do {
    cVar2 = cVar1 + '\x01';
    FUN_000445e8(cVar1,param_1);
    cVar1 = cVar2;
  } while (cVar2 != '\x04');
  FUN_0002a92c(10);
  return;
}

