//==================== FUN_04007de8 @ 04007de8 ====================

void FUN_04007de8(void)

{
  int *piVar1;
  uint *puVar2;
  uint *puVar3;
  uint *puVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  uint local_28;
  uint local_24 [4];
  
  puVar4 = DAT_04007e5c;
  puVar3 = DAT_04007e54;
  puVar2 = DAT_04007e50;
  if (((*DAT_04007e50 ^ *DAT_04007e54) & *DAT_04007e58) == 0) {
    *DAT_04007e5c = *DAT_04007e54;
  }
  else {
    uVar8 = *DAT_04007e60;
    if (uVar8 == 0) {
      func_0x03fc7fbc(DAT_04007e64);
      uVar8 = *puVar3;
    }
    *puVar4 = uVar8;
  }
  uVar8 = DAT_04007e68;
  piVar1 = DAT_04007ddc;
  uVar7 = *puVar4;
  uVar6 = *puVar2;
  local_24[0] = DAT_04007e68;
  iVar10 = *DAT_04007ddc;
  local_28 = uVar6;
  iVar5 = FUN_0400a41c(iVar10,DAT_04007de0,0x806);
  iVar9 = iVar10 + iVar5;
  *(undefined2 *)(iVar10 + iVar5) = 0x100;
  *(undefined2 *)(iVar9 + 2) = 8;
  *(undefined2 *)(iVar9 + 6) = 0x100;
  *(undefined2 *)(iVar9 + 4) = 0x406;
  FUN_04005ea4(iVar9 + 8,DAT_04007de4,6);
  local_24[0] = uVar6;
  FUN_04005ea4(iVar9 + 0xe,local_24,4);
  FUN_04005ea4(iVar9 + 0x12,uVar8,6);
  local_28 = uVar7;
  FUN_04005ea4(iVar9 + 0x18,&local_28,4);
  FUN_0400929c(*piVar1,iVar5 + 0x1c);
  return;
}

