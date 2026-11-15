//==================== FUN_00017db8 @ 00017db8 ====================

undefined4 FUN_00017db8(void)

{
  DAT_00077014 = DAT_00077014 + 1;
  if (DAT_0007b198 < DAT_00077014) {
    FUN_00017a3c();
  }
  return *(undefined4 *)(DAT_0007b438 + (DAT_00077014 + -1) * 4);
}

