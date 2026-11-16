//==================== FUN_04006244 @ 04006244 ====================

void FUN_04006244(uint param_1)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  bool bVar6;
  undefined8 uVar7;
  uint local_30;
  uint local_28;
  int local_24;
  
  uVar7 = FUN_040061b0();
  local_24 = 0;
  local_28 = param_1;
  uVar1 = func_0x03fb2008();
  local_24 = uVar1 * local_24 + (int)((ulonglong)uVar1 * (ulonglong)local_28 >> 0x20);
  local_30 = (uint)((ulonglong)uVar1 * (ulonglong)local_28);
  local_28 = local_30;
  if (local_24 == 0) {
    iVar2 = 0;
    uVar1 = local_30 / 1000000;
  }
  else {
    FUN_04004754(&local_28,1000000);
    iVar2 = local_24;
    uVar1 = local_28;
  }
  uVar5 = (uint)uVar7 + 1;
  uVar4 = (int)((ulonglong)uVar7 >> 0x20) + (uint)(0xfffffffe < (uint)uVar7) + iVar2 +
          (uint)CARRY4(uVar5,uVar1);
  do {
    uVar7 = FUN_040061b0();
    uVar3 = (uint)((ulonglong)uVar7 >> 0x20);
    bVar6 = uVar4 <= uVar3;
    if (uVar3 == uVar4) {
      bVar6 = uVar5 + uVar1 <= (uint)uVar7;
    }
  } while (!bVar6);
  return;
}

