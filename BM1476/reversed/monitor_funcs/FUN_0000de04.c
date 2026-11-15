//==================== FUN_0000de04 @ 0000de04 ====================

int FUN_0000de04(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = FUN_0000c540();
  if (iVar1 != 0) {
    *(undefined4 *)(iVar1 + 0xc) = 4;
    uVar2 = FUN_0000c574(param_1);
    *(undefined4 *)(iVar1 + 0x10) = uVar2;
  }
  return iVar1;
}

