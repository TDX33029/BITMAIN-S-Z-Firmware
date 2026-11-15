//==================== FUN_00017f68 @ 00017f68 ====================

char * FUN_00017f68(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  if (DAT_00077010 != 0) {
    return "Use only user + pass or userpass, but not both";
  }
  DAT_0007700c = DAT_0007700c + 1;
  if (DAT_0007b198 < DAT_0007700c) {
    FUN_00017a3c();
  }
  FUN_0005b1d4(param_1,*(int *)(DAT_0007b438 + (DAT_0007700c + -1) * 4) + 0xd0,DAT_0007b438,
               DAT_0007700c + -1,param_4);
  return (char *)0x0;
}

