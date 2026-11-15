//==================== FUN_0000aedc @ 0000aedc ====================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_0000aedc(int param_1,int param_2,int param_3)

{
  if (param_1 == 1) {
    _DAT_f800d00c = _DAT_f800d00c | 0x10000;
  }
  else {
    _DAT_f800d00c = _DAT_f800d00c & 0xfffeffff;
  }
  _DAT_f800d00c = _DAT_f800d00c & 0xffffffcf;
  if (param_3 != 1) {
    if (param_3 == 2) {
      _DAT_f800d00c = _DAT_f800d00c | 0x10;
    }
    else if (param_3 == 3) {
      _DAT_f800d00c = _DAT_f800d00c | 0x20;
    }
  }
  _DAT_f800d018 = (uint)(param_2 * 0xc) / 1000000;
  _DAT_f800d01c = (uint)(param_2 * 0xf) / 100000000;
  return 0;
}

