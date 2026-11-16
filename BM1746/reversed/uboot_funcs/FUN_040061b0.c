//==================== FUN_040061b0 @ 040061b0 ====================

undefined8 FUN_040061b0(void)

{
  uint uVar1;
  int unaff_r9;
  
  uVar1 = ~uRamf8f00604;
  if (uVar1 < *(uint *)(unaff_r9 + 0x8c)) {
    *(int *)(unaff_r9 + 0x88) = *(int *)(unaff_r9 + 0x88) + 1;
  }
  *(uint *)(unaff_r9 + 0x8c) = uVar1;
  return CONCAT44(*(undefined4 *)(unaff_r9 + 0x88),*(undefined4 *)(unaff_r9 + 0x8c));
}

