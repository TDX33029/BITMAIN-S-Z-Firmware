//==================== FUN_0000bb18 @ 0000bb18 ====================

undefined4 FUN_0000bb18(void)

{
  int iVar1;
  
  DAT_00028f4c = 0;
  iVar1 = FUN_0000c2f8(0);
  if (iVar1 == 0) {
    return 0xd;
  }
  iVar1 = FUN_0000c314(&DAT_00021be0,iVar1,*(undefined4 *)(iVar1 + 4));
  if (iVar1 != 0) {
    return 0xe;
  }
  iVar1 = FUN_0000c240(&DAT_00021be0);
  if (iVar1 == 0) {
    FUN_0000cdd4(&DAT_00021be0,0);
    return 0;
  }
  return 0xf;
}

