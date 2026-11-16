//==================== FUN_040096ec @ 040096ec ====================

bool FUN_040096ec(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 uVar2;
  int iStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined4 uStack_c;
  
  uVar2 = DAT_04009728;
  if (param_1 == 0) {
    uVar2 = DAT_04009724;
  }
  iStack_18 = param_1;
  uStack_14 = param_2;
  uStack_10 = param_3;
  uStack_c = param_4;
  FUN_04007514(&iStack_18,uVar2,param_1);
  iVar1 = func_0x03fbefb0(&iStack_18);
  return iVar1 != 0;
}

