//==================== FUN_0005cc10 @ 0005cc10 ====================

undefined8 FUN_0005cc10(undefined4 param_1,uint param_2,undefined4 param_3,undefined4 param_4)

{
  undefined8 uVar1;
  
  if ((double)CONCAT44(param_2,param_1) < 0.0) {
    uVar1 = FUN_0005cc48(param_1,param_2 ^ 0x80000000,param_1,param_2,param_4);
    return CONCAT44(-((int)((ulonglong)uVar1 >> 0x20) + (uint)((int)uVar1 != 0)),-(int)uVar1);
  }
  uVar1 = FUN_0005cc48();
  return uVar1;
}

