//==================== FUN_04009b00 @ 04009b00 ====================

undefined4 FUN_04009b00(void)

{
  uint *puVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 *puVar7;
  
  iVar2 = func_0x03fbefb0(DAT_04009c2c);
  puVar1 = DAT_04009c30;
  if (iVar2 == 0) {
LAB_04009b18:
    uVar3 = 0;
  }
  else {
    iVar4 = FUN_04005a60(iVar2,DAT_04009c38);
    if (iVar4 == 0) goto LAB_04009b94;
    iVar4 = FUN_04005a60(iVar2,DAT_04009c3c);
    if (iVar4 == 0) goto LAB_04009b18;
    iVar4 = FUN_04005a60(iVar2,DAT_04009c40);
    if (iVar4 == 0) {
      uVar3 = 1;
    }
    else {
      uVar3 = FUN_04007718(iVar2,0);
    }
  }
  if (uVar3 <= *puVar1) {
    FUN_04009260();
    *DAT_04009c34 = 3;
    return 0xffffff92;
  }
LAB_04009b94:
  *puVar1 = *puVar1 + 1;
  FUN_04009260();
  FUN_040097e4(*DAT_04009c44 == 0);
  uVar5 = FUN_04009164();
  if (*DAT_04009c48 == 0) {
    uVar6 = 1;
    puVar7 = DAT_04009c34;
  }
  else {
    *DAT_04009c48 = 0;
    if (*DAT_04009c4c == 0) {
      uVar6 = 3;
      puVar7 = DAT_04009c34;
    }
    else {
      *DAT_04009c50 = DAT_04009c54;
      uVar6 = FUN_040061e4();
      *DAT_04009c58 = uVar6;
      *DAT_04009c5c = 10000;
      uVar6 = DAT_04009c64;
      puVar7 = DAT_04009c60;
    }
  }
  *puVar7 = uVar6;
  return uVar5;
}

