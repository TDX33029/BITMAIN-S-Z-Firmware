//==================== FUN_00016c88 @ 00016c88 ====================

void FUN_00016c88(int param_1)

{
  if (*(int *)(param_1 + 0x20) == 1) {
    return;
  }
  (**(code **)(*(int *)(param_1 + 4) + 0x10))();
  return;
}

