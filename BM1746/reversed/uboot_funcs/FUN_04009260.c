//==================== FUN_04009260 @ 04009260 ====================

void FUN_04009260(void)

{
  int *piVar1;
  
  piVar1 = (int *)FUN_04008f68();
  if (piVar1 == (int *)0x0) {
    return;
  }
  if ((piVar1[0x12] & 1U) == 0) {
    return;
  }
  (**(code **)(*(int *)(*piVar1 + 0x3c) + 0x10))();
  *(undefined4 *)piVar1[10] = 1;
  return;
}

