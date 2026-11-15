//==================== FUN_0000b3d4 @ 0000b3d4 ====================

undefined4 FUN_0000b3d4(undefined4 *param_1,byte *param_2)

{
  undefined4 uVar1;
  int iVar2;
  int local_2c [3];
  undefined4 local_20;
  int local_1c;
  
  local_20 = 2;
  FUN_0000bb98(local_2c);
  if (local_2c[0] - 0x766eU < 0x53d1) {
    if (local_1c - 0x91ebU < 0x170b) {
      iVar2 = FUN_0000b254(param_1);
      if (iVar2 == 0) {
        *param_2 = (byte)*param_1 & 1;
        uVar1 = 0;
      }
      else {
        uVar1 = 1;
      }
    }
    else {
      uVar1 = 3;
    }
  }
  else {
    uVar1 = 2;
  }
  return uVar1;
}

