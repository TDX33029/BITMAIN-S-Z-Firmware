//==================== FUN_00011514 @ 00011514 ====================

void FUN_00011514(float param_1,int *param_2)

{
  double dVar1;
  int iVar2;
  
  dVar1 = log10((double)param_1);
  iVar2 = VectorFloatToSigned(dVar1 + 1.0,3);
  if (*param_2 < iVar2) {
    *param_2 = *param_2 + 1;
  }
  return;
}

