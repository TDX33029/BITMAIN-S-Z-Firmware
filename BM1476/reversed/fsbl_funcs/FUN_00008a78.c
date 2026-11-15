//==================== FUN_00008a78 @ 00008a78 ====================

void FUN_00008a78(undefined4 param_1,uint *param_2,uint *param_3,uint *param_4,int param_5)

{
  ulonglong uVar1;
  longlong lVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  uint *puVar6;
  uint uVar7;
  uint *local_16c;
  uint local_15c [63];
  int local_60;
  uint local_5c;
  undefined8 local_58;
  undefined4 local_4c;
  int local_48;
  uint local_44;
  uint local_40;
  uint *local_3c;
  uint *local_38;
  uint *local_34;
  int local_30;
  uint local_2c;
  
  FUN_000027e0(local_15c,0,0x104);
  local_16c = param_3;
  for (local_2c = 0; local_2c < 0x40; local_2c = local_2c + 1) {
    local_40 = *local_16c;
    local_16c = local_16c + 1;
    uVar1 = (ulonglong)local_40 * (ulonglong)*param_2 + (ulonglong)local_15c[0];
    local_34 = param_2 + 1;
    local_3c = local_15c + 1;
    local_48 = (int)uVar1;
    local_44 = (uint)(uVar1 >> 0x20);
    uVar5 = local_44;
    uVar7 = param_5 * local_48;
    lVar2 = (ulonglong)uVar7 * (ulonglong)*param_4 + (uVar1 & 0xffffffff);
    local_38 = param_4 + 1;
    local_48 = (int)lVar2;
    local_4c = local_48;
    local_44 = (uint)((ulonglong)lVar2 >> 0x20);
    local_30 = 0x3f;
    do {
      uVar4 = *local_34;
      local_34 = local_34 + 1;
      uVar1 = (ulonglong)local_40 * (ulonglong)uVar4 + (ulonglong)uVar5 + (ulonglong)local_44;
      uVar5 = (uint)(uVar1 >> 0x20);
      lVar2 = (ulonglong)uVar7 * (ulonglong)*local_38 + (ulonglong)*local_3c;
      local_38 = local_38 + 1;
      local_58 = lVar2 + (uVar1 & 0xffffffff);
      puVar6 = local_3c + -1;
      local_3c = local_3c + 1;
      *puVar6 = (uint)(lVar2 + (uVar1 & 0xffffffff));
      local_30 = local_30 + -1;
      local_44 = local_58._4_4_;
    } while (local_30 != 0);
    local_44 = (uint)CARRY4(uVar5,local_5c);
    local_60 = local_58._4_4_ + uVar5 + local_5c;
    local_5c = local_44 + CARRY4(local_58._4_4_,uVar5 + local_5c);
    _local_48 = CONCAT44(local_5c,local_60);
    local_30 = 0;
  }
  if ((local_5c != 0) || (iVar3 = FUN_00008df8(local_15c,param_4), iVar3 != 0)) {
    FUN_00008eac(local_15c,param_4);
  }
  FUN_00002200(param_1,local_15c,0x100);
  return;
}

