//==================== FUN_0000b680 @ 0000b680 ====================

bool FUN_0000b680(undefined4 *param_1)

{
  bool bVar1;
  undefined8 uVar2;
  
  uVar2 = FUN_0000b254();
  bVar1 = (int)uVar2 == 0;
  if (bVar1) {
    *(byte *)((ulonglong)uVar2 >> 0x20) = (byte)*param_1 & 1;
  }
  return !bVar1;
}

