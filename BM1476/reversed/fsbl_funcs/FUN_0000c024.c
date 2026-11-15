//==================== FUN_0000c024 @ 0000c024 ====================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_0000c024(void)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  
  DAT_00028f50 = 0;
  uVar2 = (_DAT_f8000100 & 0x7ffff) >> 0xc;
  uVar3 = (_DAT_f8000120 & 0x3fff) >> 8;
  iVar1 = FUN_00001a50(uVar3 * 0x27bc86bf,uVar2);
  uVar2 = FUN_00001a50(iVar1 * uVar2,uVar3);
  DAT_00028f50 = uVar2 / 20000000;
  return iVar1;
}

