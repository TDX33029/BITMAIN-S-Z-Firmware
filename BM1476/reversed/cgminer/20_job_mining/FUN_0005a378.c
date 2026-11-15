//==================== FUN_0005a378 @ 0005a378 ====================

undefined4 FUN_0005a378(int *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  
  if (param_1 != (int *)0x0) {
    if (*param_1 == 3) {
      uVar1 = FUN_0005cb64(param_1[2],param_1[3],param_3,3,param_4);
      return uVar1;
    }
    if (*param_1 == 4) {
      return (int)*(undefined8 *)(param_1 + 2);
    }
  }
  return 0;
}

