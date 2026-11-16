//==================== FUN_0000ddb0 @ 0000ddb0 ====================

void FUN_0000ddb0(int param_1)

{
  int iVar1;
  
  iVar1 = FUN_0000c540();
  if (iVar1 == 0) {
    return;
  }
  *(uint *)(iVar1 + 0xc) = (uint)(param_1 != 0);
  return;
}

