//==================== FUN_0005b35c @ 0005b35c ====================

void FUN_0005b35c(undefined4 param_1,int *param_2)

{
  int iVar1;
  int local_14 [2];
  
  iVar1 = FUN_0005b264(param_1,local_14);
  if (iVar1 == 0) {
    *param_2 = local_14[0];
    if (local_14[0] < 0) {
      FUN_0005b0c0("\'%s\' is negative",param_1);
    }
  }
  return;
}

