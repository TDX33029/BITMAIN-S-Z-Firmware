//==================== FIQ @ 0000001c ====================

void FIQ(uint param_1)

{
  undefined4 *unaff_r11;
  
  FUN_000014e4(param_1 & 0xffff | 0x10000,0x3b7);
  *unaff_r11 = 1;
  return;
}

