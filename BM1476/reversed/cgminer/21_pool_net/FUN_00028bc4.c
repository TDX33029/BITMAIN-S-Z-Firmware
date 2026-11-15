//==================== FUN_00028bc4 @ 00028bc4 ====================

uint FUN_00028bc4(int param_1)

{
  uint uVar1;
  byte local_9 [5];
  
  uVar1 = recv(param_1,local_9,1,0);
  if (uVar1 != 0xffffffff) {
    uVar1 = (uint)local_9[0];
  }
  return uVar1;
}

