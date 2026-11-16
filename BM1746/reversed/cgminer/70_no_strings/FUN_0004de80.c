//==================== FUN_0004de80 @ 0004de80 ====================

void FUN_0004de80(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 local_f8;
  undefined4 local_f4;
  undefined4 local_f0;
  undefined4 local_ec;
  undefined4 local_e8;
  undefined4 local_e4;
  undefined4 local_e0;
  undefined4 local_dc;
  undefined4 local_d8;
  undefined4 local_d4;
  
  local_f8 = 0;
  local_f4 = 0;
  local_f0 = 0x6a09e667;
  local_ec = 0xbb67ae85;
  local_e8 = 0x3c6ef372;
  local_e4 = 0xa54ff53a;
  local_e0 = 0x510e527f;
  local_dc = 0x9b05688c;
  local_d8 = 0x1f83d9ab;
  local_d4 = 0x5be0cd19;
  FUN_0004dbec(&local_f8,param_1,param_2);
  FUN_0004dccc(&local_f8,param_3);
  memset(&local_f8,0,0xe8);
  return;
}

