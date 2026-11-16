//==================== FUN_00008a00 @ 00008a00 ====================

int FUN_00008a00(uint param_1)

{
  uint local_10;
  int local_c;
  
  local_10 = param_1 & 1;
  for (local_c = 2; local_c != 0; local_c = local_c * local_c) {
    local_10 = (2 - local_10 * param_1) * local_10;
  }
  return -local_10;
}

