//==================== FUN_00042f7c @ 00042f7c ====================

void FUN_00042f7c(int param_1)

{
  uint uVar1;
  int iVar2;
  char acStack_1010 [4096];
  
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    snprintf(acStack_1010,0x1000,"%s chainID%d asics %d addrInterval %d",
             "software_set_address_chain",param_1,(uint)(byte)(&DAT_0009d6c5)[param_1],
             (uint)DAT_0009d6f3);
    FUN_00038730(5,acStack_1010,0);
  }
  iVar2 = 0;
  FUN_00042f04(param_1);
  FUN_0002a92c(0x32);
  do {
    uVar1 = iVar2 * (uint)DAT_0009d6f3;
    iVar2 = iVar2 + 1;
    FUN_00042f40(param_1,uVar1 & 0xff);
    FUN_0002a92c(0x32);
  } while (iVar2 != 3);
  return;
}

