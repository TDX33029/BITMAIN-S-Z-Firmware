//==================== FUN_00008df8 @ 00008df8 ====================

bool FUN_00008df8(int param_1,int param_2)

{
  int local_c;
  
  local_c = 0x40;
  do {
    local_c = local_c + -1;
    if (*(int *)(param_1 + local_c * 4) != *(int *)(param_2 + local_c * 4)) break;
  } while (0 < local_c);
  return *(uint *)(param_2 + local_c * 4) < *(uint *)(param_1 + local_c * 4);
}

