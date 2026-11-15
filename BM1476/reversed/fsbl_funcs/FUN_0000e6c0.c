//==================== FUN_0000e6c0 @ 0000e6c0 ====================

int FUN_0000e6c0(uint *param_1,int param_2)

{
  int iVar1;
  int local_2c [3];
  undefined4 local_20;
  int local_1c;
  
  local_20 = 2;
  FUN_0000bb98(local_2c);
  if (local_2c[0] - 0x766eU < 0x53d1) {
    if (local_1c - 0x91ebU < 0x170b) {
      if (param_2 + 0xfeced300U < 0x2625a01) {
        iVar1 = FUN_0000aedc(0,param_2,1);
        if (iVar1 == 0) {
          iVar1 = FUN_0000b254(param_1);
          if (iVar1 == 0) {
            if (((*param_1 & 1) == 0) ||
               ((iVar1 = FUN_0000aedc(0,param_2,2), iVar1 == 0 &&
                (((*param_1 & 1) == 0 ||
                 ((iVar1 = FUN_0000aedc(0,param_2,3), iVar1 == 0 && ((*param_1 & 1) == 0)))))))) {
              iVar1 = 8;
            }
          }
          else {
            iVar1 = 1;
          }
        }
      }
      else {
        iVar1 = 0xb;
      }
    }
    else {
      iVar1 = 3;
    }
  }
  else {
    iVar1 = 2;
  }
  return iVar1;
}

