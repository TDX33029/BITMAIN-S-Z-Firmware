//==================== FUN_0005a328 @ 0005a328 ====================

undefined4 FUN_0005a328(int *param_1)

{
  int iVar1;
  undefined4 uVar2;
  double in_d0;
  
  if ((((param_1 == (int *)0x0) || (*param_1 != 4)) || (iVar1 = __isnan(in_d0), iVar1 != 0)) ||
     (iVar1 = __isinf(in_d0), iVar1 != 0)) {
    uVar2 = 0xffffffff;
  }
  else {
    *(double *)(param_1 + 2) = in_d0;
    uVar2 = 0;
  }
  return uVar2;
}

