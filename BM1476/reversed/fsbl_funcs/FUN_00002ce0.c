//==================== FUN_00002ce0 @ 00002ce0 ====================

undefined4 FUN_00002ce0(int param_1)

{
  int iVar1;
  
  iVar1 = FUN_0000676c(*(undefined1 *)(param_1 + 1),param_1 + 0x40,*(undefined4 *)(param_1 + 0x2c),1
                      );
  if (iVar1 != 0) {
    return 1;
  }
  *(undefined1 *)(param_1 + 3) = 0;
  if (((uint)(*(int *)(param_1 + 0x2c) - *(int *)(param_1 + 0x20)) < *(uint *)(param_1 + 0x18)) &&
     (*(char *)(param_1 + 2) == '\x02')) {
    FUN_0000676c(*(undefined1 *)(param_1 + 1),param_1 + 0x40,
                 *(int *)(param_1 + 0x2c) + *(uint *)(param_1 + 0x18),1);
    return 0;
  }
  return 0;
}

