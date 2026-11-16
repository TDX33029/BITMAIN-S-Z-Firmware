//==================== FUN_00015798 @ 00015798 ====================

void FUN_00015798(double *param_1)

{
  double in_d0;
  double dVar1;
  double in_d1;
  double in_d2;
  
  if (0.0 < in_d1) {
    dVar1 = exp(in_d1 / in_d2);
    dVar1 = 1.0 - 1.0 / dVar1;
    *param_1 = (*param_1 + dVar1 * (in_d0 / in_d1)) / (dVar1 + 1.0);
  }
  return;
}

