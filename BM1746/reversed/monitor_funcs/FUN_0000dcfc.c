//==================== FUN_0000dcfc @ 0000dcfc ====================

void FUN_0000dcfc(int param_1,undefined4 param_2,int *param_3)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  int iVar4;
  
  piVar3 = *(int **)(param_1 + 8);
  if (piVar3 == (int *)0x0) {
    return;
  }
  iVar4 = 0;
  while (iVar1 = FUN_0000c634(piVar3[8],param_2), iVar1 != 0) {
    piVar3 = (int *)*piVar3;
    iVar4 = iVar4 + 1;
    if (piVar3 == (int *)0x0) {
      return;
    }
  }
  iVar1 = FUN_0000c574(param_2);
  param_3[8] = iVar1;
  piVar3 = *(int **)(param_1 + 8);
  iVar1 = 0;
  if (piVar3 != (int *)0x0) {
    iVar1 = iVar4;
  }
  piVar2 = piVar3;
  if ((piVar3 != (int *)0x0 && iVar4 != 0) && -1 < iVar1) {
    do {
      piVar2 = (int *)*piVar2;
      iVar4 = iVar4 + -1;
      iVar1 = 0;
      if (piVar2 != (int *)0x0) {
        iVar1 = iVar4;
      }
    } while ((piVar2 != (int *)0x0 && iVar4 != 0) && -1 < iVar1);
  }
  if (piVar2 == (int *)0x0) {
    return;
  }
  iVar4 = *piVar2;
  iVar1 = piVar2[1];
  *param_3 = iVar4;
  param_3[1] = iVar1;
  if (iVar4 != 0) {
    *(int **)(iVar4 + 4) = param_3;
  }
  if (piVar2 == piVar3) {
    *(int **)(param_1 + 8) = param_3;
  }
  else {
    *(int **)param_3[1] = param_3;
  }
  piVar2[1] = 0;
  *piVar2 = 0;
  FUN_0000d864(piVar2);
  return;
}

