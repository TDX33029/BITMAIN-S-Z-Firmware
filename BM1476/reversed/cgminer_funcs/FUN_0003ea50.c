//==================== FUN_0003ea50 @ 0003ea50 ====================

uint FUN_0003ea50(undefined4 param_1,int param_2)

{
  char cVar1;
  uint param4;
  undefined1 local_103c;
  undefined1 local_1038;
  int local_1034;
  undefined1 uStack_102d;
  undefined1 uStack_102c;
  undefined1 local_102b;
  ushort uStack_102a;
  char acStack_1028 [4100];
  
  if (param_2 - 700U < 0x65) {
    local_103c = (undefined1)param_2;
    local_1038 = (undefined1)((uint)param_2 >> 8);
    local_1034 = param_2 + 4;
  }
  else if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 5)) {
    local_1034 = 0x2c0;
    local_1038 = 2;
    local_103c = 0xbc;
    param_2 = 700;
  }
  else {
    param_2 = 700;
    snprintf(acStack_1028,0x1000,"illegal voltage [%d, %d], set default voltage %d\n",700,800,700);
    FUN_00038730(5,acStack_1028,0);
    local_1038 = 2;
    local_1034 = 0x2c0;
    local_103c = 0xbc;
  }
  cVar1 = '\x04';
  uStack_102a = 0;
  do {
    uStack_102d = 0xff;
    FUN_0003c7e0(param_1,0,&uStack_102d,1);
    FUN_0002a92c(100);
    local_102b = local_1038;
    uStack_102c = local_103c;
    FUN_0003c7e0(param_1,0x21,&uStack_102c,2);
    FUN_0002a92c(200);
    FUN_0003e564(param_1,&uStack_102a);
    param4 = (uint)uStack_102a;
    if (((int)param4 <= local_1034) && (param_2 + -4 <= (int)param4)) {
      return param4;
    }
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
      snprintf(acStack_1028,0x1000,"set vol %d, get vol %d, set again!\n",param_2,param4);
      FUN_00038730(5,acStack_1028,0);
    }
    cVar1 = cVar1 + -1;
    FUN_0002a92c(200);
  } while (cVar1 != '\0');
  return (uint)uStack_102a;
}

