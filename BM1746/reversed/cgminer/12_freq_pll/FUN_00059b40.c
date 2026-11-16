//==================== FUN_00059b40 @ 00059b40 ====================

undefined4 FUN_00059b40(byte *param_1,int param_2,uint *param_3)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  bool bVar4;
  
  uVar2 = (uint)*param_1;
  if (param_2 == 2) {
    uVar2 = uVar2 & 0x1f;
  }
  else if (param_2 == 3) {
    uVar2 = uVar2 & 0xf;
  }
  else {
    if (param_2 != 4) {
      return 0;
    }
    uVar2 = uVar2 & 7;
  }
  uVar3 = (uint)param_1[1];
  if ((uVar3 ^ 0x80) < 0x40) {
    iVar1 = 1;
    do {
      iVar1 = iVar1 + 1;
      uVar2 = (uVar3 & 0x3f) + uVar2 * 0x40;
      if (param_2 <= iVar1) {
        if (0x10ffff < (int)uVar2) {
          return 0;
        }
        if (uVar2 - 0xd800 < 0x800) {
          return 0;
        }
        bVar4 = uVar2 == 0x7f;
        if ((int)uVar2 < 0x80) {
          bVar4 = param_2 == 2;
        }
        if (bVar4) {
          return 0;
        }
        bVar4 = uVar2 == 0x7ff;
        if ((int)uVar2 < 0x800) {
          bVar4 = param_2 == 3;
        }
        if (bVar4) {
          return 0;
        }
        if (param_2 == 4 && (int)uVar2 < 0x10000) {
          return 0;
        }
        if (param_3 == (uint *)0x0) {
          return 1;
        }
        *param_3 = uVar2;
        return 1;
      }
      uVar3 = (uint)param_1[iVar1];
    } while ((uVar3 ^ 0x80) < 0x40);
  }
  return 0;
}

