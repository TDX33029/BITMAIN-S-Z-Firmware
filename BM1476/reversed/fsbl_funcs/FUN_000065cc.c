//==================== FUN_000065cc @ 000065cc ====================

uint FUN_000065cc(int param_1)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  
  uVar1 = FUN_000064b8();
  if (((uVar1 & 2) != 0) || ((uVar1 & 1) == 0)) {
    return uVar1;
  }
  if (DAT_000219a4 != 0) {
    do {
    } while ((*(uint *)(DAT_000219a0 + 0x24) & 0x70000) != 0x70000);
  }
  iVar2 = FUN_000067e8(param_1);
  if (iVar2 != 0) {
    iVar3 = param_1 * 0x220 + 0x21560;
    iVar2 = FUN_00006804(iVar3,iVar2,*(undefined4 *)(iVar2 + 4));
    if ((iVar2 == 0) && (iVar2 = FUN_00007758(iVar3), iVar2 == 0)) {
      (&DAT_00019be8)[param_1] = (char)(uVar1 & 0xfe);
      return uVar1 & 0xfe;
    }
  }
  return uVar1 & 0xff | 1;
}

