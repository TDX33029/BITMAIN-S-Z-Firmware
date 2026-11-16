//==================== FUN_0000d864 @ 0000d864 ====================

void FUN_0000d864(int *param_1)

{
  int *piVar1;
  
  if (param_1 == (int *)0x0) {
    return;
  }
  do {
    piVar1 = (int *)*param_1;
    if (((param_1[3] & 0x100U) == 0) &&
       (((param_1[2] == 0 || (FUN_0000d864(), (param_1[3] & 0x100U) == 0)) && (param_1[4] != 0)))) {
      (*(code *)PTR_free_0001b378)();
    }
    if (param_1[8] != 0) {
      (*(code *)PTR_free_0001b378)();
    }
    (*(code *)PTR_free_0001b378)(param_1);
    param_1 = piVar1;
  } while (piVar1 != (int *)0x0);
  return;
}

