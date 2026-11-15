//==================== FUN_00059f80 @ 00059f80 ====================

undefined4 * FUN_00059f80(void)

{
  undefined4 *puVar1;
  int iVar2;
  
  puVar1 = (undefined4 *)FUN_0005956c(0x18);
  if (puVar1 == (undefined4 *)0x0) {
    return (undefined4 *)0x0;
  }
  *puVar1 = 1;
  puVar1[1] = 1;
  puVar1[3] = 0;
  puVar1[2] = 8;
  iVar2 = FUN_0005956c(0x20);
  puVar1[4] = iVar2;
  if (iVar2 == 0) {
    FUN_00059594(puVar1);
    return (undefined4 *)0x0;
  }
  puVar1[5] = 0;
  return puVar1;
}

