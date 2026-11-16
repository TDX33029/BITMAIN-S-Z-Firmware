//==================== FUN_00045dec @ 00045dec ====================

float FUN_00045dec(int param_1)

{
  uint in_fpscr;
  float fVar1;
  
  if (param_1 - 0x23U < 7) {
    fVar1 = (float)VectorSignedToFloat(param_1 - 0x23U,(byte)(in_fpscr >> 0x16) & 3);
    return fVar1 * 1.4285715 + 40.0;
  }
  if (param_1 - 0x2aU < 7) {
    fVar1 = (float)VectorSignedToFloat(param_1 - 0x2aU,(byte)(in_fpscr >> 0x16) & 3);
    return fVar1 * 1.4285715 + 50.0;
  }
  if (param_1 - 0x31U < 6) {
    fVar1 = (float)VectorSignedToFloat(param_1 - 0x31U,(byte)(in_fpscr >> 0x16) & 3);
    return fVar1 * 3.3333333 + 60.0;
  }
  if (param_1 - 0x37U < 8) {
    fVar1 = (float)VectorSignedToFloat(param_1 - 0x37U,(byte)(in_fpscr >> 0x16) & 3);
    return fVar1 * 2.5 + 80.0;
  }
  if (param_1 < 0x23) {
    return 40.0;
  }
  fVar1 = 0.0;
  if (0x3e < param_1) {
    fVar1 = 100.0;
  }
  return fVar1;
}

