//==================== FUN_0003bb44 @ 0003bb44 ====================

void FUN_0003bb44(void)

{
  uint uVar1;
  
  uVar1 = FUN_0003bb00();
  uVar1 = uVar1 & 0x3f;
  *(uint *)(DAT_00078e10 + 0x3c) = uVar1 | uVar1 << 8 | uVar1 << 0x10 | uVar1 << 0x18;
  return;
}

