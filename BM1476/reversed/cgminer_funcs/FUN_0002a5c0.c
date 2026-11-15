//==================== FUN_0002a5c0 @ 0002a5c0 ====================

void FUN_0002a5c0(undefined4 *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  lldiv_t local_18;
  
  lldiv(&local_18,CONCAT44(param_4,param_3),1000000);
  *param_1 = (undefined4)local_18.quot;
  param_1[1] = (int)local_18.rem * 1000;
  return;
}

