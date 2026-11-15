//==================== FUN_0001a914 @ 0001a914 ====================

undefined4 FUN_0001a914(undefined4 param_1,int param_2,undefined4 param_3,undefined4 *param_4)

{
  int iVar1;
  undefined4 uVar2;
  char local_1018 [4];
  char acStack_1014 [4];
  char acStack_1010 [4];
  char acStack_100c [4];
  undefined2 uStack_1008;
  undefined1 local_1006;
  
  if (param_4 != (undefined4 *)0x0) {
    *param_4 = 0;
  }
  iVar1 = FUN_0001654c(param_2,param_3);
  if (iVar1 == 0) {
    FUN_000189a4(param_1);
    uVar2 = 0;
  }
  else {
    FUN_0001a730(param_1,param_2);
    iVar1 = FUN_00029f54(param_2 + 0xc0,param_2 + 0xa0);
    if (iVar1 == 0) {
      if (param_4 != (undefined4 *)0x0) {
        *param_4 = 1;
      }
      uVar2 = 0;
      if ((DAT_00078e09 != '\0') &&
         (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (uVar2 = 0, 6 < DAT_000766c4)))) {
        local_1018[0] = 'S';
        local_1018[1] = 'h';
        local_1018[2] = 'a';
        local_1018[3] = 'r';
        acStack_1014[0] = 'e';
        acStack_1014[1] = ' ';
        acStack_1014[2] = 'a';
        acStack_1014[3] = 'b';
        acStack_1010[0] = 'o';
        acStack_1010[1] = 'v';
        acStack_1010[2] = 'e';
        acStack_1010[3] = ' ';
        acStack_100c[0] = 't';
        acStack_100c[1] = 'a';
        acStack_100c[2] = 'r';
        acStack_100c[3] = 'g';
        uStack_1008 = 0x7465;
        local_1006 = 0;
        FUN_00038730(7,local_1018,0);
        uVar2 = 0;
      }
    }
    else {
      uVar2 = 1;
    }
  }
  return uVar2;
}

