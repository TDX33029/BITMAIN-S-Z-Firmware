//==================== FUN_0400b9e0 @ 0400b9e0 ====================

void FUN_0400b9e0(void)

{
  int *piVar1;
  int iVar2;
  
  piVar1 = DAT_0400ba40;
  iVar2 = FUN_040061e4(*DAT_0400ba40);
  *piVar1 = iVar2;
  if (iVar2 != 0) {
    func_0x03fc7fbc(DAT_0400ba44);
    iVar2 = FUN_0400c66c(*DAT_0400ba48,*piVar1);
    FUN_040042ac(iVar2 * 1000,0,DAT_0400ba4c);
  }
  func_0x03fc7fbc(DAT_0400ba50);
  *DAT_0400ba54 = 2;
  return;
}

