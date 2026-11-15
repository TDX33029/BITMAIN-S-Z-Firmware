//==================== FUN_0005a0ec @ 0005a0ec ====================

undefined4 * FUN_0005a0ec(int param_1)

{
  undefined4 *puVar1;
  int iVar2;
  
  if ((param_1 == 0) || (puVar1 = (undefined4 *)FUN_0005956c(0xc), puVar1 == (undefined4 *)0x0)) {
    return (undefined4 *)0x0;
  }
  *puVar1 = 2;
  puVar1[1] = 1;
  iVar2 = FUN_000595b4(param_1);
  puVar1[2] = iVar2;
  if (iVar2 == 0) {
    FUN_00059594(puVar1);
    return (undefined4 *)0x0;
  }
  return puVar1;
}

