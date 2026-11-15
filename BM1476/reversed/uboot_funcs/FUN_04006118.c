//==================== FUN_04006118 @ 04006118 ====================

ulonglong FUN_04006118(uint param_1,int param_2)

{
  uint uVar1;
  uint uVar2;
  undefined4 uVar3;
  uint uVar4;
  bool bVar5;
  bool bVar6;
  uint local_18;
  uint local_14;
  
  local_18 = param_1;
  local_14 = param_2;
  uVar3 = func_0x03fb2008();
  uVar4 = local_18 >> 0x1b;
  bVar5 = local_18 * 0x20 < local_18;
  uVar1 = local_18 * 0x1f;
  bVar6 = CARRY4(local_18 * 0x7c,local_18);
  uVar2 = local_18 * 0x7d;
  local_18 = local_18 * 1000;
  local_14 = ((((local_14 << 5 | uVar4) - (local_14 + bVar5)) * 4 | uVar1 >> 0x1e) + local_14 +
             (uint)bVar6) * 8 | uVar2 >> 0x1d;
  if (local_14 == 0) {
    uVar4 = FUN_0400c66c(local_18,uVar3);
    return (ulonglong)uVar4;
  }
  FUN_04004754(&local_18,uVar3);
  return CONCAT44(local_14,local_18);
}

