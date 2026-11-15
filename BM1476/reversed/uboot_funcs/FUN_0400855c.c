//==================== FUN_0400855c @ 0400855c ====================

void FUN_0400855c(char *param_1,int param_2,undefined4 param_3,int param_4,uint param_5)

{
  char cVar1;
  ulonglong uVar2;
  int *piVar3;
  undefined4 uVar4;
  uint uVar5;
  int iVar6;
  char *pcVar7;
  int iVar8;
  int iVar9;
  char *pcVar10;
  undefined4 local_28;
  int local_24;
  
  if (param_2 != 0x44 || (param_4 != 0x43 || param_5 < 0xec)) {
    return;
  }
  if (*param_1 != '\x02') {
    return;
  }
  if (param_1[1] != '\x01') {
    return;
  }
  if (param_1[2] != '\x06') {
    return;
  }
  pcVar7 = param_1;
  local_28 = param_3;
  local_24 = param_4;
  FUN_04005ea4(&local_24,param_1 + 4,4,6,param_1,param_2);
  iVar6 = 0;
  while( true ) {
    if (iVar6 == *DAT_040088d8) {
      return;
    }
    if (local_24 == *(int *)(DAT_040088dc + iVar6 * 4)) break;
    iVar6 = iVar6 + 1;
  }
  iVar6 = FUN_04005ff4(param_1 + 0x1c,DAT_0400890c,6);
  if (iVar6 != 0) {
    return;
  }
  pcVar10 = param_1 + 0x10;
  FUN_04005ea4(&local_24,pcVar10,4);
  piVar3 = DAT_04008910;
  if (local_24 == 0) {
    return;
  }
  iVar9 = *DAT_04008910;
  if (iVar9 != 4) {
    if (iVar9 == 6) {
      return;
    }
    if (iVar9 == 3) {
      FUN_040084d8(param_1);
      func_0x03ff78c8(param_1,param_5);
      uVar4 = DAT_04008914;
      *piVar3 = 4;
      FUN_0400a04c(5000,uVar4);
      iVar8 = *DAT_04008918;
      FUN_04005e0c(iVar8,0,0x5f2);
      iVar9 = FUN_0400a41c(iVar8,DAT_0400891c,0x800);
      iVar8 = iVar8 + iVar9;
      *(undefined4 *)(iVar8 + 0x1c) = DAT_04008920;
      uVar5 = FUN_040061e4(*DAT_04008900);
      uVar2 = (ulonglong)DAT_04008924 * (ulonglong)uVar5 >> 0x26;
      *(ushort *)(iVar8 + 0x24) =
           (ushort)DAT_04008928 & (ushort)((uint)uVar2 << 8) | (ushort)(uVar2 >> 8) & 0xff;
      local_24 = iVar6;
      FUN_04005ea4(iVar8 + 0x34,&local_24,4);
      FUN_04005ea4(iVar8 + 0x38,DAT_0400890c,6);
      FUN_0400a748(iVar8 + 0x88,DAT_040088f4,0x80);
      FUN_04005ea4(iVar8 + 0x20,param_1 + 4,4);
      FUN_04005ea4(&local_28,pcVar10,4);
      iVar6 = FUN_04008110(iVar8 + 0x108,3,*DAT_0400892c,local_28);
      FUN_0400a5c0(iVar8,0xffffffff,0x43,0x44,iVar6 + 0xec);
      FUN_0400929c(*DAT_04008918,iVar9 + 0x1c + iVar6 + 0xec);
      return;
    }
    func_0x03fc7fbc(DAT_04008908);
    return;
  }
  FUN_04005ea4(&local_24,param_1 + 0xec,4,DAT_04008910,pcVar7,param_2);
  if (local_24 != DAT_040088e0) {
    return;
  }
  pcVar7 = param_1 + 0xf0;
  while( true ) {
    cVar1 = *pcVar7;
    if (cVar1 == -1) {
      return;
    }
    if (cVar1 == '5') break;
    if (cVar1 == '\0') {
      pcVar7 = pcVar7 + 1;
    }
    else {
      pcVar7 = pcVar7 + (byte)pcVar7[1] + 2;
    }
  }
  if (pcVar7[2] != '\x05') {
    return;
  }
  FUN_040084d8(param_1);
  FUN_04005ea4(&local_24,param_1 + 0x14,4);
  if (local_24 != 0) {
    FUN_04005ea4(DAT_040088e4,param_1 + 0x14,4);
  }
  FUN_04005ea4(DAT_040088ec,*DAT_040088e8 + 6);
  if ((*DAT_040088f0 & 1) == 0) {
    iVar6 = FUN_04005b98(param_1 + 0x6c);
    if (iVar6 != 0) {
      FUN_0400a748(DAT_040088f4,param_1 + 0x6c,0x400);
    }
  }
  if (*DAT_040088f4 != '\0') {
    func_0x03fbeb7c(DAT_040088f8);
  }
  FUN_04005ea4(DAT_040088fc,pcVar10,4);
  *piVar3 = 6;
  uVar4 = FUN_040061e4(*DAT_04008900);
  FUN_04007548(DAT_04008904,DAT_040088fc,uVar4);
  FUN_0400a04c(0);
  func_0x03fbf43c(0xa8);
  FUN_04009a20();
  return;
}

