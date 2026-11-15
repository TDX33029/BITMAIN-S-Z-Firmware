//==================== FUN_00059c18 @ 00059c18 ====================

byte * FUN_00059c18(byte *param_1,uint *param_2)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint local_1c [2];
  
  uVar3 = (uint)*param_1;
  if (uVar3 == 0) {
    return param_1;
  }
  iVar1 = FUN_00059ac4(uVar3);
  if (iVar1 < 1) {
LAB_00059c84:
    param_1 = (byte *)0x0;
  }
  else {
    if (iVar1 == 1) {
      local_1c[0] = uVar3;
    }
    else {
      iVar2 = FUN_00059b40(param_1,iVar1,local_1c);
      if (iVar2 == 0) goto LAB_00059c84;
    }
    param_1 = param_1 + iVar1;
    if (param_2 != (uint *)0x0) {
      *param_2 = local_1c[0];
    }
  }
  return param_1;
}

