//==================== FUN_0005cc48 @ 0005cc48 ====================

undefined8 FUN_0005cc48(undefined4 param_1,undefined4 param_2)

{
  uint in_fpscr;
  undefined4 uVar1;
  undefined4 uVar2;
  double dVar3;
  
  uVar1 = VectorFloatToUnsigned((double)CONCAT44(param_2,param_1) * 2.3283064365386963e-10,3);
  dVar3 = (double)VectorUnsignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
  uVar2 = VectorFloatToUnsigned((double)CONCAT44(param_2,param_1) - dVar3 * 4294967296.0,3);
  return CONCAT44(uVar1,uVar2);
}

