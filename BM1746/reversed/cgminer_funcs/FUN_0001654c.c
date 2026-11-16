//==================== FUN_0001654c @ 0001654c ====================

bool FUN_0001654c(int param_1,undefined4 param_2)

{
  uint uVar1;
  
  *(undefined4 *)(param_1 + 0x4c) = param_2;
  if (DAT_0007b859 == '\0') {
    FUN_00011f60();
  }
  else {
    FUN_00014f30();
  }
  if (DAT_0007b859 == '\0') {
    uVar1 = 0;
  }
  else {
    uVar1 = 0xffff;
  }
  return *(uint *)(param_1 + 0xdc) <= uVar1;
}

