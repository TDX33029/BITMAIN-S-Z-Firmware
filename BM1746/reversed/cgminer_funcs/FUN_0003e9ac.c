//==================== FUN_0003e9ac @ 0003e9ac ====================

void FUN_0003e9ac(undefined4 param_1)

{
  uint in_fpscr;
  double dVar1;
  undefined4 local_18;
  undefined4 local_14;
  
  local_18 = 0x20;
  FUN_0003e6e0(0,&local_18,1);
  sleep(1);
  dVar1 = (double)VectorUnsignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
  local_18 = CONCAT22(local_18._2_2_,0x4000);
  local_14 = VectorFloatToUnsigned((dVar1 / 1000.0 - 0.25) / 0.005 + 1.0,3);
  local_18 = CONCAT31(local_18._1_3_,(char)local_14);
  FUN_0003e6e0(0x13,&local_18,2);
  return;
}

