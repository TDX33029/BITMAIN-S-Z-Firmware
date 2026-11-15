//==================== FUN_00010d90 @ 00010d90 ====================

int * FUN_00010d90(int param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = *(int **)(param_2 + 4);
  if (piVar2 != (int *)0x0) {
    do {
      iVar1 = strcmp((char *)(*piVar2 + 0x1008),(char *)(param_1 + 0x1008));
      if (iVar1 == 0) {
        return piVar2;
      }
      piVar2 = (int *)piVar2[1];
    } while (piVar2 != (int *)0x0);
  }
  return (int *)0x0;
}

