//==================== FUN_00010ddc @ 00010ddc ====================

int * FUN_00010ddc(int param_1,int *param_2)

{
  int iVar1;
  int *piVar2;
  
  while ((piVar2 = (int *)param_2[1], piVar2 != (int *)0x0 &&
         (iVar1 = strcmp((char *)(*piVar2 + 0x1008),(char *)(param_1 + 0x1008)), iVar1 != 0))) {
    param_2 = piVar2;
  }
  return param_2;
}

