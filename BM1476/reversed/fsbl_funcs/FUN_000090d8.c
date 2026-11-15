//==================== FUN_000090d8 @ 000090d8 ====================

void FUN_000090d8(undefined4 param_1,undefined4 *param_2,undefined4 param_3,int param_4,
                 undefined4 param_5)

{
  undefined1 auStack_234 [256];
  undefined4 local_134 [64];
  uint local_34;
  uint local_30;
  undefined4 local_2c;
  undefined4 local_28;
  int local_24;
  undefined4 local_20;
  undefined4 *local_1c;
  undefined4 local_18;
  int local_14;
  uint local_10;
  uint local_c;
  
  local_c = 0;
  local_28 = param_5;
  local_24 = param_4;
  local_20 = param_3;
  local_1c = param_2;
  local_18 = param_1;
  FUN_000027e0(local_134,0,0x100);
  local_134[0] = 1;
  local_2c = FUN_00008a00(*local_1c);
  FUN_00008a78(auStack_234,local_18,local_20,local_1c,local_2c);
  FUN_00008a78(local_28,local_20,local_134,local_1c,local_2c);
  for (local_14 = 0x3f; -1 < local_14; local_14 = local_14 + -1) {
    local_30 = *(uint *)(local_24 + local_14 * 4);
    for (local_10 = 0x80000000; local_10 != 0; local_10 = local_10 >> 1) {
      local_34 = local_30 & local_10;
      if (local_c != 0) {
        FUN_00008a78(local_28,local_28,local_28,local_1c,local_2c);
      }
      if (local_c == 0) {
        local_c = local_34;
      }
      if (local_34 != 0) {
        FUN_00008a78(local_28,local_28,auStack_234,local_1c,local_2c);
      }
    }
  }
  FUN_00008a78(local_28,local_28,local_134,local_1c,local_2c);
  return;
}

