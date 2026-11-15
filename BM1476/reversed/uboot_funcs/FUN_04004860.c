//==================== FUN_04004860 @ 04004860 ====================

undefined4 FUN_04004860(undefined4 param_1,uint param_2)

{
  undefined4 uVar1;
  
  uVar1 = func_0x03fc9318(0x20,param_1);
  if ((param_2 & 0x8000) != 0) {
    FUN_04005e0c(uVar1,0,param_1);
  }
  return uVar1;
}

