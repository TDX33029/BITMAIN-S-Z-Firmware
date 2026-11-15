//==================== FUN_0001a474 @ 0001a474 ====================

int FUN_0001a474(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = FUN_0001a44c(param_1,0);
  *(undefined1 *)(iVar1 + 0x6e1) = 1;
  FUN_0002a84c(iVar1 + 0x770);
  *(undefined1 *)(iVar1 + 0x6e8) = 0;
  *(int *)(iVar1 + 0x6d8) = *(int *)(iVar1 + 0x6d8) + -1;
  *(undefined1 *)(iVar1 + 0x6ea) = 0;
  return iVar1;
}

