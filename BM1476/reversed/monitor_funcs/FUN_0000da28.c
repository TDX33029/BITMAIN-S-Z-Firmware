//==================== FUN_0000da28 @ 0000da28 ====================

void FUN_0000da28(int param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = *(int **)(param_1 + 8);
  iVar1 = 0;
  if (piVar2 != (int *)0x0) {
    iVar1 = param_2;
  }
  if ((piVar2 == (int *)0x0 || param_2 == 0) || iVar1 < 0) {
    return;
  }
  do {
    piVar2 = (int *)*piVar2;
    param_2 = param_2 + -1;
    iVar1 = 0;
    if (piVar2 != (int *)0x0) {
      iVar1 = param_2;
    }
  } while ((piVar2 != (int *)0x0 && param_2 != 0) && -1 < iVar1);
  return;
}

