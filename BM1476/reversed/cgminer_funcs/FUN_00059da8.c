//==================== FUN_00059da8 @ 00059da8 ====================

undefined4 * FUN_00059da8(void)

{
  undefined4 *puVar1;
  int iVar2;
  
  puVar1 = (undefined4 *)FUN_0005956c(0x24);
  if (puVar1 == (undefined4 *)0x0) {
    return (undefined4 *)0x0;
  }
  puVar1[1] = 1;
  *puVar1 = 0;
  iVar2 = FUN_000579f4(puVar1 + 2);
  if (iVar2 == 0) {
    puVar1[7] = 0;
    puVar1[8] = 0;
    return puVar1;
  }
  FUN_00059594(puVar1);
  return (undefined4 *)0x0;
}

