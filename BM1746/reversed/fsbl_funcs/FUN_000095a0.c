//==================== FUN_000095a0 @ 000095a0 ====================

void FUN_000095a0(int param_1,int param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  undefined1 local_34;
  undefined1 local_33;
  undefined1 local_32;
  undefined1 local_31;
  undefined1 local_30;
  undefined1 local_2f;
  undefined1 local_2e;
  undefined1 local_2d;
  int local_2c;
  uint local_28;
  uint local_24;
  
  local_34 = (undefined1)((*(uint *)(param_1 + 100) & 0x1fffffff) >> 0x15);
  uVar1 = (*(uint *)(param_1 + 100) & 0x1fffffff) >> 0xd;
  local_33 = (undefined1)uVar1;
  uVar2 = (*(uint *)(param_1 + 100) & 0x1fffffff) >> 5;
  local_32 = (undefined1)uVar2;
  uVar6 = *(uint *)(param_1 + 0x60) >> 0x1d | *(int *)(param_1 + 100) << 3;
  local_31 = (undefined1)uVar6;
  uVar7 = *(uint *)(param_1 + 0x60);
  uVar3 = (uVar7 & 0x1fffffff) >> 0x15;
  local_30 = (undefined1)uVar3;
  uVar8 = *(uint *)(param_1 + 0x60);
  uVar4 = (uVar8 & 0x1fffffff) >> 0xd;
  local_2f = (undefined1)uVar4;
  uVar9 = *(uint *)(param_1 + 0x60);
  uVar5 = (uVar9 & 0x1fffffff) >> 5;
  local_2e = (undefined1)uVar5;
  local_2d = (undefined1)((*(uint *)(param_1 + 0x60) & 0xff) << 3);
  local_28 = *(uint *)(param_1 + 0x60) & 0x3f;
  if (local_28 < 0x38) {
    local_2c = 0x38 - local_28;
  }
  else {
    local_2c = 0x78 - local_28;
  }
  FUN_000092c8(param_1,&DAT_00019c10,local_2c,local_2c,uVar5,0,uVar9 << 3,
               uVar9 >> 0x1d | *(int *)(param_1 + 100) << 3,uVar4,0,uVar8 << 3,
               uVar8 >> 0x1d | *(int *)(param_1 + 100) << 3,uVar3,0,uVar7 << 3,
               uVar7 >> 0x1d | *(int *)(param_1 + 100) << 3,uVar6,0,*(uint *)(param_1 + 0x60) << 3,
               uVar6,uVar2,0,*(uint *)(param_1 + 0x60) << 3,
               *(uint *)(param_1 + 0x60) >> 0x1d | *(uint *)(param_1 + 100) << 3,uVar1,0);
  FUN_000092c8(param_1,&local_34,8);
  for (local_24 = 0; local_24 < 8; local_24 = local_24 + 1) {
    *(char *)(param_2 + local_24 * 4) =
         (char)((uint)*(undefined4 *)(param_1 + local_24 * 4) >> 0x18);
    *(char *)(param_2 + local_24 * 4 + 1) =
         (char)((uint)*(undefined4 *)(param_1 + local_24 * 4) >> 0x10);
    *(char *)(param_2 + local_24 * 4 + 2) =
         (char)((uint)*(undefined4 *)(param_1 + local_24 * 4) >> 8);
    *(char *)(param_2 + local_24 * 4 + 3) = (char)*(undefined4 *)(param_1 + local_24 * 4);
  }
  return;
}

