//==================== FUN_0000dba4 @ 0000dba4 ====================

int * FUN_0000dba4(int param_1,int param_2)

{
  int *piVar1;
  int *piVar2;
  int *piVar3;
  int iVar4;
  
  piVar2 = *(int **)(param_1 + 8);
  iVar4 = 0;
  if (piVar2 != (int *)0x0) {
    iVar4 = param_2;
  }
  piVar3 = piVar2;
  if ((piVar2 != (int *)0x0 && param_2 != 0) && -1 < iVar4) {
    do {
      piVar3 = (int *)*piVar3;
      param_2 = param_2 + -1;
      iVar4 = 0;
      if (piVar3 != (int *)0x0) {
        iVar4 = param_2;
      }
    } while ((piVar3 != (int *)0x0 && param_2 != 0) && -1 < iVar4);
  }
  if (piVar3 != (int *)0x0) {
    piVar1 = (int *)piVar3[1];
    if (piVar1 != (int *)0x0) {
      *piVar1 = *piVar3;
    }
    iVar4 = *piVar3;
    if (iVar4 != 0) {
      *(int **)(iVar4 + 4) = piVar1;
    }
    if (piVar3 == piVar2) {
      *(int *)(param_1 + 8) = iVar4;
    }
    *piVar3 = 0;
    piVar3[1] = 0;
  }
  return piVar3;
}

