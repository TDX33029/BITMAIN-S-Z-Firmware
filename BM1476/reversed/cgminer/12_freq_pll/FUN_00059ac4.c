//==================== FUN_00059ac4 @ 00059ac4 ====================

undefined4 FUN_00059ac4(uint param_1)

{
  undefined4 uVar1;
  
  if ((param_1 & 0x80) == 0) {
    return 1;
  }
  if (((param_1 ^ 0x80) < 0x40) || ((param_1 + 0x40 & 0xff) < 2)) {
    return 0;
  }
  if ((param_1 + 0x3e & 0xff) < 0x1e) {
    return 2;
  }
  if ((param_1 + 0x20 & 0xff) < 0x10) {
    return 3;
  }
  if ((param_1 + 0x10 & 0xff) < 5) {
    uVar1 = 4;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}

