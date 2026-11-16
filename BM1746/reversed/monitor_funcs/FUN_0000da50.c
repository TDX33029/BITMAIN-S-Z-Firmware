//==================== FUN_0000da50 @ 0000da50 ====================

int FUN_0000da50(int param_1,undefined4 param_2)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = *(int **)(param_1 + 8);
  while ((piVar2 != (int *)0x0 && (iVar1 = FUN_0000c634(piVar2[8],param_2), iVar1 != 0))) {
    piVar2 = (int *)*piVar2;
  }
  return (int)piVar2;
}

