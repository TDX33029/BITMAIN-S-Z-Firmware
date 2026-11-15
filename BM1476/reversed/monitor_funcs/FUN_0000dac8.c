//==================== FUN_0000dac8 @ 0000dac8 ====================

void FUN_0000dac8(int param_1,undefined4 param_2,int param_3)

{
  undefined4 uVar1;
  int *piVar2;
  
  if (param_3 == 0) {
    return;
  }
  if (*(int *)(param_3 + 0x20) != 0) {
    (*(code *)PTR_free_0001b378)();
  }
  uVar1 = FUN_0000c574(param_2);
  piVar2 = *(int **)(param_1 + 8);
  if (piVar2 == (int *)0x0) {
    *(int *)(param_1 + 8) = param_3;
  }
  *(undefined4 *)(param_3 + 0x20) = uVar1;
  if (piVar2 == (int *)0x0) {
    return;
  }
  while ((int *)*piVar2 != (int *)0x0) {
    piVar2 = (int *)*piVar2;
  }
  *piVar2 = param_3;
  *(int **)(param_3 + 4) = piVar2;
  return;
}

