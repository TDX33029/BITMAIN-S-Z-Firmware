//==================== FUN_0000bd28 @ 0000bd28 ====================

bool FUN_0000bd28(uint param_1,uint param_2)

{
  uint uVar1;
  bool bVar2;
  undefined8 uVar3;
  
  uVar3 = FUN_0000bc74();
  uVar1 = (uint)((ulonglong)uVar3 >> 0x20);
  bVar2 = param_2 <= uVar1;
  if (uVar1 == param_2) {
    bVar2 = param_1 <= (uint)uVar3;
  }
  return bVar2;
}

