//==================== FUN_0000d9fc @ 0000d9fc ====================

int FUN_0000d9fc(int param_1)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = *(int **)(param_1 + 8);
  if (piVar2 == (int *)0x0) {
    return 0;
  }
  iVar1 = 0;
  do {
    piVar2 = (int *)*piVar2;
    iVar1 = iVar1 + 1;
  } while (piVar2 != (int *)0x0);
  return iVar1;
}

