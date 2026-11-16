//==================== FUN_0002aa24 @ 0002aa24 ====================

undefined4 FUN_0002aa24(int *param_1,int *param_2)

{
  uint in_fpscr;
  double dVar1;
  double dVar2;
  
  dVar2 = (double)VectorSignedToFloat(param_1[1] - param_2[1],(byte)(in_fpscr >> 0x16) & 3);
  dVar1 = (double)VectorSignedToFloat(*param_1 - *param_2,(byte)(in_fpscr >> 0x16) & 3);
  return SUB84(dVar1 + dVar2 / 1000000.0,0);
}

