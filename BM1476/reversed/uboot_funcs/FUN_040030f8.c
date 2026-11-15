//==================== FUN_040030f8 @ 040030f8 ====================

undefined4 FUN_040030f8(undefined4 param_1,int param_2,int *param_3)

{
  int iVar1;
  int *piVar3;
  int *piVar2;
  undefined4 uVar4;
  
  if (param_2 < 1) {
    uVar4 = 0;
  }
  else {
    uVar4 = 0;
    piVar2 = param_3;
    do {
      piVar3 = piVar2 + 1;
      if ((*piVar2 != 0) && (iVar1 = FUN_04005a60(param_1), iVar1 == 0)) {
        uVar4 = 1;
        *piVar2 = 0;
      }
      piVar2 = piVar3;
    } while (piVar3 != param_3 + param_2);
  }
  return uVar4;
}

