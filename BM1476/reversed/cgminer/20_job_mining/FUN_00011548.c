//==================== FUN_00011548 @ 00011548 ====================

void FUN_00011548(undefined4 param_1,int *param_2)

{
  uint in_fpscr;
  double dVar1;
  int iVar2;
  
  dVar1 = (double)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
  dVar1 = log10(dVar1);
  iVar2 = VectorFloatToSigned(dVar1 + 1.0,3);
  if (*param_2 < iVar2) {
    *param_2 = *param_2 + 1;
  }
  return;
}

