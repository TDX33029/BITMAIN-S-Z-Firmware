//==================== FUN_0001ffec @ 0001ffec ====================

undefined1 FUN_0001ffec(int param_1)

{
  int iVar1;
  int iVar2;
  
  if (*(int *)(param_1 + 0x84) != 1) {
    return 0;
  }
  if (((((DAT_00076efc == 4) || (DAT_00076efc == 3)) ||
       ((*(char *)(param_1 + 0x298) != '\0' && (*(char *)(param_1 + 0x81) != '\0')))) ||
      ((iVar1 = FUN_0001da58(), param_1 == iVar1 || (*(int *)(param_1 + 0x2f4) != 0)))) ||
     ((DAT_00076efc == 0 && (iVar2 = *(int *)(param_1 + 4), iVar1 = FUN_0001fb7c(), iVar2 < iVar1)))
     ) {
    return 1;
  }
  return DAT_0007b4c8;
}

