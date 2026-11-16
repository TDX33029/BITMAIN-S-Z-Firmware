//==================== FUN_000092c8 @ 000092c8 ====================

void FUN_000092c8(int param_1,int param_2,uint param_3)

{
  uint uVar1;
  uint local_64;
  int local_60;
  uint auStack_54 [16];
  uint local_14;
  uint local_10;
  uint local_c;
  
  local_10 = *(uint *)(param_1 + 0x60) & 0x3f;
  local_14 = 0x40 - local_10;
  if (param_3 != 0) {
    uVar1 = *(uint *)(param_1 + 0x60);
    *(uint *)(param_1 + 0x60) = param_3 + uVar1;
    *(uint *)(param_1 + 100) = *(int *)(param_1 + 100) + (uint)CARRY4(param_3,uVar1);
    local_64 = param_3;
    local_60 = param_2;
    if ((local_10 != 0) && (local_14 <= param_3)) {
      FUN_00002200(param_1 + local_10 + 0x20,param_2,local_14);
      for (local_c = 0; local_c < 0x10; local_c = local_c + 1) {
        auStack_54[local_c] =
             (uint)*(byte *)(param_1 + local_c * 4 + 0x20) << 0x18 |
             (uint)*(byte *)(param_1 + local_c * 4 + 0x21) << 0x10 |
             (uint)*(byte *)(param_1 + local_c * 4 + 0x22) << 8 |
             (uint)*(byte *)(param_1 + local_c * 4 + 0x23);
      }
      FUN_000099a0(param_1,auStack_54);
      local_64 = param_3 - local_14;
      local_60 = param_2 + local_14;
      local_10 = 0;
    }
    for (; 0x3f < local_64; local_64 = local_64 - 0x40) {
      for (local_c = 0; local_c < 0x10; local_c = local_c + 1) {
        auStack_54[local_c] =
             (uint)*(byte *)(local_60 + local_c * 4) << 0x18 |
             (uint)*(byte *)(local_60 + local_c * 4 + 1) << 0x10 |
             (uint)*(byte *)(local_60 + local_c * 4 + 2) << 8 |
             (uint)*(byte *)(local_60 + local_c * 4 + 3);
      }
      FUN_000099a0(param_1,auStack_54);
      local_60 = local_60 + 0x40;
    }
    if (local_64 != 0) {
      FUN_00002200(param_1 + local_10 + 0x20,local_60,local_64);
    }
  }
  return;
}

