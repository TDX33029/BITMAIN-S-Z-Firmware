//==================== FUN_00001554 @ 00001554 ====================

ushort FUN_00001554(int param_1)

{
  ushort uVar1;
  
  func_0xfffff540(param_1,DAT_00001588,0xffffffff);
  if (*(int *)(param_1 + 0x10) == 0x10) {
    uVar1 = **(ushort **)(param_1 + 100);
  }
  else {
    uVar1 = (ushort)(byte)**(ushort **)(param_1 + 100);
  }
  return uVar1 & 0xff;
}

