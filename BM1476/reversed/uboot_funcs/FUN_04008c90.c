//==================== FUN_04008c90 @ 04008c90 ====================

undefined4 FUN_04008c90(int param_1,int param_2,int param_3,undefined4 param_4)

{
  undefined4 uVar1;
  int iVar2;
  int local_14;
  int iStack_10;
  
  local_14 = param_2;
  iStack_10 = param_3;
  uVar1 = FUN_04007718(param_1 + 3,0,10,param_4,param_1);
  iVar2 = func_0x03fd0ed0(0x13,uVar1,0,&local_14);
  if (iVar2 == 0) {
    if (param_3 == 1) {
      FUN_04005e0c(*(int *)(local_14 + 8) + 4,0,6);
    }
    else if ((param_3 == 0) || (param_3 == 2)) {
      FUN_04009550(param_2,*(int *)(local_14 + 8) + 4);
    }
  }
  return 0;
}

