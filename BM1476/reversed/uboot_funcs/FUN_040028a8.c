//==================== FUN_040028a8 @ 040028a8 ====================

void FUN_040028a8(uint *param_1,int param_2,uint param_3)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  
  uVar4 = *param_1;
  uVar5 = uVar4 & 0x3f;
  *param_1 = uVar4 + param_3;
  uVar2 = 0x40 - uVar5;
  if (CARRY4(uVar4,param_3)) {
    param_1[1] = param_1[1] + 1;
  }
  if (uVar5 != 0 && uVar2 <= param_3) {
    param_3 = param_3 - uVar2;
    FUN_04005ea4((int)(param_1 + 10) + uVar5,param_2,uVar2);
    param_2 = param_2 + uVar2;
    uVar5 = 0;
    FUN_04000090(param_1,param_1 + 10);
  }
  if (0x3f < param_3) {
    iVar3 = param_2 + (param_3 - 0x40 & 0xffffffc0) + 0x40;
    iVar1 = param_2;
    do {
      param_2 = iVar1 + 0x40;
      FUN_04000090(param_1,iVar1);
      iVar1 = param_2;
    } while (param_2 != iVar3);
    param_3 = param_3 & 0x3f;
  }
  if (param_3 == 0) {
    return;
  }
  FUN_04005ea4((int)param_1 + uVar5 + 0x28,param_2,param_3);
  return;
}

