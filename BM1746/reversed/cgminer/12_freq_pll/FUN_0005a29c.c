//==================== FUN_0005a29c @ 0005a29c ====================

undefined4 * FUN_0005a29c(void)

{
  int iVar1;
  undefined4 *puVar2;
  double in_d0;
  
  iVar1 = __isnan(in_d0);
  if (((iVar1 == 0) && (iVar1 = __isinf(in_d0), iVar1 == 0)) &&
     (puVar2 = (undefined4 *)FUN_0005956c(0x10), puVar2 != (undefined4 *)0x0)) {
    *(double *)(puVar2 + 2) = in_d0;
    *puVar2 = 4;
    puVar2[1] = 1;
  }
  else {
    puVar2 = (undefined4 *)0x0;
  }
  return puVar2;
}

