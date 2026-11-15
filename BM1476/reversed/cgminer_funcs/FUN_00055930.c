//==================== FUN_00055930 @ 00055930 ====================

void FUN_00055930(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined8 local_50;
  
  local_50 = 0;
  local_70 = 0x6a09e667;
  local_6c = 0xbb67ae85;
  local_68 = 0x3c6ef372;
  local_64 = 0xa54ff53a;
  local_60 = 0x510e527f;
  local_5c = 0x9b05688c;
  local_58 = 0x1f83d9ab;
  local_54 = 0x5be0cd19;
  FUN_0005576c(&local_70,param_1,param_2);
  FUN_000557d4(&local_70,param_3);
  return;
}

