//==================== FUN_04005038 @ 04005038 ====================

void FUN_04005038(int param_1,int param_2,undefined4 param_3,undefined4 param_4,undefined4 *param_5)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  
  uVar3 = *param_5;
  while( true ) {
    iVar1 = FUN_04004bd4(param_1,param_2 + 2,param_3,param_4,param_5,0);
    if (iVar1 == 0) break;
    uVar2 = *param_5;
    iVar1 = FUN_04004bd4(param_1,(uint)*(byte *)(param_1 + param_2 + 1) + param_2,param_3,param_4,
                         param_5,0);
    *param_5 = uVar2;
    if (iVar1 != 0) {
      uVar3 = uVar2;
    }
  }
  *param_5 = uVar3;
  return;
}

