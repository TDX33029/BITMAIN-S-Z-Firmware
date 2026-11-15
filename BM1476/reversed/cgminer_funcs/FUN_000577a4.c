//==================== FUN_000577a4 @ 000577a4 ====================

undefined4 FUN_000577a4(uint *param_1,undefined4 param_2,undefined4 param_3,uint param_4)

{
  undefined4 uVar1;
  
  if (((param_4 & 0x200) == 0) && ((param_1 == (uint *)0x0 || (1 < *param_1)))) {
    uVar1 = 0xffffffff;
  }
  else {
    uVar1 = FUN_00057178(param_1,param_4,0,param_2,param_3);
  }
  return uVar1;
}

