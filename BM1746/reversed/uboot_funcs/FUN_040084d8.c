//==================== FUN_040084d8 @ 040084d8 ====================

void FUN_040084d8(int param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  byte *pbVar1;
  int local_14;
  
  local_14 = param_2;
  FUN_04005ea4(&local_14,param_1 + 0xec,4,param_4,param_1);
  pbVar1 = DAT_04008558;
  if (local_14 == DAT_04008554) {
    *DAT_04008558 = 0;
    FUN_040082e4(param_1 + 0xf0,param_1 + 0x314);
    if ((*pbVar1 & 1) != 0) {
      FUN_040082e4(param_1 + 0x6c,param_1 + 0xec);
    }
    if ((*pbVar1 & 2) != 0) {
      FUN_040082e4(param_1 + 0x2c,param_1 + 0x6c);
    }
  }
  return;
}

