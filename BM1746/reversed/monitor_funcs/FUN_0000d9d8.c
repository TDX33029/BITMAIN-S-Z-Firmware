//==================== FUN_0000d9d8 @ 0000d9d8 ====================

/* WARNING: Removing unreachable block (ram,0x0000d970) */
/* WARNING: Removing unreachable block (ram,0x0000d984) */
/* WARNING: Removing unreachable block (ram,0x0000d98c) */
/* WARNING: Removing unreachable block (ram,0x0000d994) */
/* WARNING: Removing unreachable block (ram,0x0000d9b0) */
/* WARNING: Removing unreachable block (ram,0x0000d968) */
/* WARNING: Removing unreachable block (ram,0x0000d9a4) */

int FUN_0000d9d8(byte *param_1)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = FUN_0000c540();
  DAT_0001b3a4 = 0;
  if (iVar2 != 0) {
    if ((param_1 != (byte *)0x0) && (bVar1 = *param_1, bVar1 != 0)) {
      while (bVar1 < 0x21) {
        param_1 = param_1 + 1;
        if ((param_1 == (byte *)0x0) || (bVar1 = *param_1, bVar1 == 0)) break;
      }
    }
    iVar3 = FUN_0000d05c(iVar2,param_1);
    if (iVar3 == 0) {
      FUN_0000d864(iVar2);
      iVar2 = 0;
    }
  }
  return iVar2;
}

