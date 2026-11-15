//==================== FUN_040061fc @ 040061fc ====================

void FUN_040061fc(void)

{
  uint uVar1;
  int iVar2;
  undefined8 uVar3;
  
  uVar3 = FUN_040061b0();
  iVar2 = (int)((ulonglong)uVar3 >> 0x20);
  uVar1 = (uint)uVar3;
  FUN_04006118(uVar1 * 1000,
               ((((iVar2 << 5 | uVar1 >> 0x1b) - (iVar2 + (uint)(uVar1 * 0x20 < uVar1))) * 4 |
                uVar1 * 0x1f >> 0x1e) + iVar2 + (uint)CARRY4(uVar1 * 0x7c,uVar1)) * 8 |
               uVar1 * 0x7d >> 0x1d);
  return;
}

