//==================== FUN_00003b44 @ 00003b44 ====================

undefined4 FUN_00003b44(int *param_1,int param_2)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  
  uVar4 = param_1[4] + 0x20;
  if (0x1fffff < uVar4) {
LAB_00003c54:
    param_1[6] = 0;
    return 4;
  }
  if (param_1[6] == 0) {
    return 4;
  }
  iVar5 = *param_1;
  if ((uVar4 & 0x1ff) == 0) {
    param_1[6] = param_1[6] + 1;
    if (param_1[5] == 0) {
      if ((uint)*(ushort *)(iVar5 + 8) <= uVar4 >> 5) goto LAB_00003c54;
    }
    else if ((*(ushort *)(iVar5 + 10) - 1 & uVar4 >> 9) == 0) {
      uVar1 = FUN_000036fc(iVar5);
      if (uVar1 < 2) {
        return 2;
      }
      if (uVar1 == 0xffffffff) {
        return 1;
      }
      uVar3 = *(uint *)(iVar5 + 0x14);
      if (uVar3 <= uVar1) {
        if (param_2 == 0) goto LAB_00003c54;
        uVar1 = FUN_00003868(param_1,param_1[5]);
        if (uVar1 == 0) {
          return 7;
        }
        if (uVar1 == 1) {
          return 2;
        }
        if (uVar1 == 0xffffffff) {
          return 1;
        }
        iVar2 = FUN_00002d5c(iVar5,uVar1);
        if (iVar2 != 0) {
          return 1;
        }
        uVar3 = *(uint *)(iVar5 + 0x14);
      }
      param_1[5] = uVar1;
      if (uVar1 - 2 < uVar3 - 2) {
        iVar2 = (uVar1 - 2) * (uint)*(ushort *)(iVar5 + 10) + *(int *)(iVar5 + 0x28);
      }
      else {
        iVar2 = 0;
      }
      param_1[6] = iVar2;
    }
  }
  param_1[4] = uVar4;
  param_1[7] = iVar5 + 0x40 + (uVar4 & 0x1ff);
  return 0;
}

