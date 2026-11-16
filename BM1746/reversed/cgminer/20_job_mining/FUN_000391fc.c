//==================== FUN_000391fc @ 000391fc ====================

int FUN_000391fc(int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar2 = *(int *)(param_1 + 0xc);
  if (iVar2 == 0) {
    if (*(char *)(param_1 + 4) != '\0') {
      return 0;
    }
    FUN_00038b58();
    iVar2 = *(int *)(param_1 + 0xc);
    if (iVar2 == 0) {
      return 0;
    }
  }
  iVar1 = *(int *)(iVar2 + 8);
  *(int *)(param_1 + 0xc) = iVar1;
  if (iVar1 == 0) {
    if (*(char *)(param_1 + 0x2c) != '\0') {
      *(undefined4 *)(param_1 + 0x10) = 0;
    }
  }
  else {
    *(undefined4 *)(iVar1 + 4) = 0;
  }
  *(undefined4 *)(iVar2 + 8) = 0;
  *(undefined4 *)(iVar2 + 4) = 0;
  *(int *)(param_1 + 0x1c) = *(int *)(param_1 + 0x1c) + -1;
  return iVar2;
}

