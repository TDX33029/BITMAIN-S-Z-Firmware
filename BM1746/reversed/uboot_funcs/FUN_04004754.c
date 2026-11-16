//==================== FUN_04004754 @ 04004754 ====================

uint FUN_04004754(uint *param_1,uint param_2)

{
  byte bVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  bool bVar9;
  
  uVar5 = *param_1;
  uVar6 = param_1[1];
  uVar4 = 0;
  if (uVar6 < param_2) {
    uVar7 = 0;
  }
  else {
    uVar7 = FUN_0400c66c(uVar6);
    uVar6 = uVar6 - uVar7 * param_2;
  }
  uVar8 = 0;
  bVar9 = uVar6 == 0;
  uVar2 = 1;
  if (uVar6 == 0) {
    bVar9 = uVar5 <= param_2;
  }
  uVar3 = 0;
  if (param_2 != 0 && !bVar9) {
    do {
      bVar9 = CARRY4(param_2,param_2);
      param_2 = param_2 * 2;
      uVar4 = uVar4 * 2 + (uint)bVar9;
      bVar9 = CARRY4(uVar2,uVar2);
      uVar2 = uVar2 * 2;
      uVar3 = uVar3 * 2 + (uint)bVar9;
      bVar9 = uVar6 <= uVar4;
      if (uVar4 == uVar6) {
        bVar9 = uVar5 <= param_2;
      }
    } while (!bVar9 && (int)(uint)(param_2 == 0) <= (int)uVar4);
  }
  do {
    bVar9 = uVar4 <= uVar6;
    if (uVar6 == uVar4) {
      bVar9 = param_2 <= uVar5;
    }
    if (bVar9) {
      bVar9 = uVar5 < param_2;
      uVar5 = uVar5 - param_2;
      uVar6 = uVar6 - (uVar4 + bVar9);
      bVar9 = CARRY4(uVar8,uVar2);
      uVar8 = uVar8 + uVar2;
      uVar7 = uVar7 + uVar3 + (uint)bVar9;
    }
    bVar1 = (byte)uVar3;
    uVar3 = uVar3 >> 1;
    uVar2 = (uint)(bVar1 & 1) << 0x1f | uVar2 >> 1;
    bVar1 = (byte)uVar4;
    uVar4 = uVar4 >> 1;
    param_2 = (uint)(bVar1 & 1) << 0x1f | param_2 >> 1;
  } while (uVar2 != 0 || uVar3 != 0);
  *param_1 = uVar8;
  param_1[1] = uVar7;
  return uVar5;
}

