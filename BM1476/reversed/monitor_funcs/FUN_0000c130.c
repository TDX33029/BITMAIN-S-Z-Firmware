//==================== FUN_0000c130 @ 0000c130 ====================

int FUN_0000c130(byte *param_1)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  
  uVar2 = (uint)*param_1;
  uVar1 = uVar2 - 0x30;
  if (9 < (uVar1 & 0xff)) {
    if (uVar2 - 0x41 < 6) {
      uVar1 = uVar2 - 0x37;
    }
    else {
      if (5 < uVar2 - 0x61) {
        return 0;
      }
      uVar1 = uVar2 - 0x57;
    }
  }
  uVar2 = (uint)param_1[1];
  iVar3 = uVar1 * 0x10;
  if ((uVar2 - 0x30 & 0xff) < 10) {
    iVar3 = (uVar2 - 0x30) + iVar3;
  }
  else if (uVar2 - 0x41 < 6) {
    iVar3 = (uVar2 - 0x37) + iVar3;
  }
  else {
    if (5 < uVar2 - 0x61) {
      return 0;
    }
    iVar3 = (uVar2 - 0x57) + iVar3;
  }
  uVar1 = (uint)param_1[2];
  uVar2 = uVar1 - 0x30;
  if (9 < (uVar2 & 0xff)) {
    if (uVar1 - 0x41 < 6) {
      uVar2 = uVar1 - 0x37;
    }
    else {
      if (5 < uVar1 - 0x61) {
        return 0;
      }
      uVar2 = uVar1 - 0x57;
    }
  }
  uVar1 = (uint)param_1[3];
  iVar3 = (uVar2 + iVar3 * 0x10) * 0x10;
  if ((uVar1 - 0x30 & 0xff) < 10) {
    return (uVar1 - 0x30) + iVar3;
  }
  if (uVar1 - 0x41 < 6) {
    return (uVar1 - 0x37) + iVar3;
  }
  if (uVar1 - 0x61 < 6) {
    return (uVar1 - 0x57) + iVar3;
  }
  return 0;
}

