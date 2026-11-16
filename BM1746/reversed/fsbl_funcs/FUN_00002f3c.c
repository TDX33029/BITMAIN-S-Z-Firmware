//==================== FUN_00002f3c @ 00002f3c ====================

uint FUN_00002f3c(int param_1,undefined4 param_2)

{
  uint uVar1;
  int iVar2;
  
  if ((*(char *)(param_1 + 3) != '\0') && (uVar1 = FUN_00002ce0(), uVar1 != 0)) {
    return uVar1;
  }
  iVar2 = FUN_0000667c(*(undefined1 *)(param_1 + 1),param_1 + 0x40,param_2,1);
  if (iVar2 != 0) {
    param_2 = 0xffffffff;
  }
  *(undefined4 *)(param_1 + 0x2c) = param_2;
  return (uint)(iVar2 != 0);
}

