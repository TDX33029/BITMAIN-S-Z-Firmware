//==================== FUN_0400583c @ 0400583c ====================

void FUN_0400583c(int param_1,undefined4 param_2,int param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  int local_24 [2];
  
  local_24[0] = 0;
  if (*(int *)(param_1 + 0x20c) != 0) {
    FUN_04004bd4(param_1,0,param_2,param_3,local_24,param_4);
    return;
  }
  if (0 < param_3) {
    local_24[0] = 0;
    do {
      iVar2 = local_24[0] + 1;
      iVar1 = FUN_04004bd4(param_1,0,param_2,param_3,local_24,param_4);
      local_24[0] = iVar2;
    } while (iVar1 == 0 && iVar2 < param_3);
  }
  return;
}

