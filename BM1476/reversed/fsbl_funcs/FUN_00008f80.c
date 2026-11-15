//==================== FUN_00008f80 @ 00008f80 ====================

void FUN_00008f80(undefined4 param_1,undefined4 param_2,uint param_3,undefined4 *param_4,
                 undefined4 param_5)

{
  undefined1 auStack_218 [256];
  undefined4 local_118 [64];
  uint local_18;
  undefined4 local_14;
  uint local_10;
  uint local_c;
  
  local_c = 0;
  local_10 = 0x80000000;
  FUN_000027e0(local_118,0,0x100);
  local_118[0] = 1;
  local_14 = FUN_00008a00(*param_4);
  FUN_00008a78(auStack_218,param_2,param_5,param_4,local_14);
  FUN_00008a78(param_1,param_5,local_118,param_4,local_14);
  for (; local_10 != 0; local_10 = local_10 >> 1) {
    local_18 = param_3 & local_10;
    if (local_c != 0) {
      FUN_00008a78(param_1,param_1,param_1,param_4,local_14);
    }
    if (local_c == 0) {
      local_c = local_18;
    }
    if (local_18 != 0) {
      FUN_00008a78(param_1,param_1,auStack_218,param_4,local_14);
    }
  }
  FUN_00008a78(param_1,param_1,local_118,param_4,local_14);
  return;
}

