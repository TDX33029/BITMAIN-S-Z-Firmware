//==================== FUN_0000e7c4 @ 0000e7c4 ====================

int FUN_0000e7c4(uint param_1,undefined4 param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int local_2c [3];
  undefined4 local_20;
  int local_1c;
  
  local_20 = 2;
  FUN_0000bb98(local_2c);
  if (local_2c[0] - 0x766eU < 0x53d1) {
    if (local_1c - 0x91ebU < 0x170b) {
      iVar1 = FUN_0000b6a4(param_1);
      if (iVar1 == 0) {
        iVar1 = FUN_0000e6c0(param_1,param_2);
      }
      iVar3 = param_1 + 0x87c + (param_1 & 0x7f) * -2;
      iVar2 = FUN_0000b6a4(iVar3);
      if (iVar2 == 0) {
        iVar2 = FUN_0000e6c0(iVar3,param_2);
        if (iVar2 != 0 && iVar1 != 0) {
          return iVar2;
        }
      }
      else if (iVar1 != 0) {
        return iVar2;
      }
      iVar1 = 0;
    }
    else {
      iVar1 = 6;
    }
  }
  else {
    iVar1 = 5;
  }
  return iVar1;
}

