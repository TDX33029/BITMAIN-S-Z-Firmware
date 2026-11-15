//==================== FUN_04007884 @ 04007884 ====================

int FUN_04007884(char *param_1)

{
  int iVar1;
  
  if (*param_1 == '-') {
    iVar1 = FUN_04007718(param_1 + 1);
    return -iVar1;
  }
  iVar1 = FUN_04007718();
  return iVar1;
}

