//==================== IRQ @ 00000018 ====================

void IRQ(void)

{
  undefined4 *unaff_r11;
  
  FUN_000014e4(&DAT_00017208,0x3b7);
  *unaff_r11 = 1;
  return;
}

