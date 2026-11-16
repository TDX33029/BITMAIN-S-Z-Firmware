//==================== FUN_00002f8c @ 00002f8c ====================

int FUN_00002f8c(int *param_1)

{
  int iVar1;
  int iVar2;
  
  iVar2 = *param_1;
  if ((param_1[6] == *(int *)(iVar2 + 0x2c)) || (iVar1 = FUN_00002f3c(iVar2), iVar1 == 0)) {
    *(undefined1 *)param_1[7] = 0xe5;
    *(undefined1 *)(iVar2 + 3) = 1;
    return 0;
  }
  return iVar1;
}

