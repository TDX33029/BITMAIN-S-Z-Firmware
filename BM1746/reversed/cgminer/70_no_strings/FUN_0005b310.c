//==================== FUN_0005b310 @ 0005b310 ====================

void FUN_0005b310(undefined4 param_1,int *param_2)

{
  int iVar1;
  int local_14 [2];
  
  iVar1 = FUN_0005b2e8(param_1,local_14);
  if (iVar1 == 0) {
    if (local_14[0] < 0) {
      FUN_0005b0c0("\'%s\' is negative",param_1);
    }
    else {
      *param_2 = local_14[0];
    }
  }
  return;
}

