//==================== FUN_00008eac @ 00008eac ====================

void FUN_00008eac(uint *param_1,uint *param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint *local_28;
  uint *local_24;
  uint local_18;
  uint local_14;
  
  local_14 = 0;
  local_28 = param_2;
  local_24 = param_1;
  for (local_18 = 0; local_18 < 0x40; local_18 = local_18 + 1) {
    uVar1 = *local_24;
    uVar3 = *local_28;
    uVar2 = uVar1 - uVar3;
    local_28 = local_28 + 1;
    *local_24 = uVar2 - local_14;
    local_24 = local_24 + 1;
    local_14 = (uint)(-(uint)(uVar2 < local_14) != (uint)(uVar1 < uVar3));
  }
  return;
}

