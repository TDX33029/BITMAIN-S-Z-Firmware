//==================== FUN_04003300 @ 04003300 ====================

bool FUN_04003300(uint param_1,int *param_2)

{
  uint extraout_r1;
  uint uVar1;
  uint uVar2;
  int iVar3;
  
  param_1 = param_1 | 1;
  do {
    uVar1 = param_1 * -0x55555555;
    uVar2 = param_1 % 3;
    if (param_1 < 10) {
LAB_04003364:
      if (uVar2 != 0) {
        param_2[1] = param_1;
        param_2[2] = 0;
        iVar3 = func_0x03fc8dc4(param_1 + 1,0x14);
        *param_2 = iVar3;
        return iVar3 != 0;
      }
    }
    else if (uVar2 != 0) {
      iVar3 = 3;
      do {
        iVar3 = iVar3 + 2;
        FUN_0400c7d4(param_1,iVar3,uVar1);
        uVar1 = iVar3 * iVar3;
        uVar2 = extraout_r1;
        if (param_1 <= uVar1) goto LAB_04003364;
      } while (extraout_r1 != 0);
    }
    param_1 = param_1 + 2;
  } while( true );
}

