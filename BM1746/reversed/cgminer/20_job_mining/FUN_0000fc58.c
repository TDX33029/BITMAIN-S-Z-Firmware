//==================== FUN_0000fc58 @ 0000fc58 ====================

void FUN_0000fc58(int param_1)

{
  time_t tVar1;
  int iVar2;
  
  *(undefined1 *)(param_1 + 0x3d) = 1;
  FUN_0002a84c(param_1 + 0x2c);
  *(undefined4 *)(*(int *)(param_1 + 0x24) + 0x60) = 0;
  iVar2 = *(int *)(param_1 + 0x24);
  tVar1 = time((time_t *)0x0);
  *(time_t *)(iVar2 + 0xf0) = tVar1;
  return;
}

