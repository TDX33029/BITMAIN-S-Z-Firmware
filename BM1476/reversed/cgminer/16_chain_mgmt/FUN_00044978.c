//==================== FUN_00044978 @ 00044978 ====================

void FUN_00044978(void)

{
  char cVar1;
  int iVar2;
  uint param3;
  undefined *puVar3;
  char local_1028 [4];
  char acStack_1024 [4];
  char acStack_1020 [4092];
  
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    local_1028[0] = 'c';
    local_1028[1] = 'h';
    local_1028[2] = 'e';
    local_1028[3] = 'c';
    acStack_1024[0] = 'k';
    acStack_1024[1] = '_';
    acStack_1024[2] = 'c';
    acStack_1024[3] = 'h';
    acStack_1020[0] = 'a';
    acStack_1020[1] = 'i';
    acStack_1020[2] = 'n';
    acStack_1020[3] = '\0';
    FUN_00038730(5,local_1028,0);
  }
  param3 = 0;
  puVar3 = &DAT_0009d4b4;
  DAT_0009d6cb = '\0';
  iVar2 = *(int *)(DAT_00078e10 + 8);
  do {
    cVar1 = DAT_0007ad48;
    if ((iVar2 >> (param3 & 0xff) & 1U) == 0) {
      puVar3[8] = 0;
    }
    else {
      puVar3[8] = 1;
      DAT_0009d6cb = DAT_0009d6cb + '\x01';
      if (((cVar1 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
        snprintf(local_1028,0x1000,"Chain %d existed!",param3);
        FUN_00038730(5,local_1028,0);
      }
    }
    param3 = param3 + 1;
    puVar3 = puVar3 + 1;
  } while (param3 != 4);
  return;
}

