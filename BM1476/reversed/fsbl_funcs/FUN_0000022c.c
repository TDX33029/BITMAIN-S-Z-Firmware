//==================== FUN_0000022c @ 0000022c ====================

bool FUN_0000022c(int param_1,undefined4 param_2,undefined4 *param_3)

{
  ushort uVar1;
  uint uVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  uint uVar6;
  
  if (param_3 == (undefined4 *)0x0) {
    FUN_000014e4(&DAT_00017208,0x506);
    DAT_00028e84 = 1;
    return false;
  }
  DAT_00028e84 = 0;
  if (*(char *)(param_1 + 0x6ecc) == '\x02') {
    iVar3 = *(int *)(param_1 + 8);
    do {
    } while ((*(uint *)(iVar3 + 0x400) & 0x40) != 0);
    *(uint *)(iVar3 + 0x404) = *(uint *)(iVar3 + 0x404) & 0xfffffff3;
  }
  uVar1 = *(ushort *)(param_1 + 0x1c);
  func_0xfffff540(param_1,DAT_0000034c,param_2,*(undefined4 *)(param_1 + 0x18));
  uVar6 = uVar1 - 4;
  uVar2 = uVar6 >> 2;
  if (uVar2 != 0) {
    puVar5 = param_3;
    do {
      puVar4 = puVar5 + 1;
      **(undefined4 **)(param_1 + 100) = *puVar5;
      puVar5 = puVar4;
    } while (param_3 + uVar2 != puVar4);
  }
  puVar5 = (undefined4 *)(*(uint *)(param_1 + 100) | 0x200000);
  *(undefined4 **)(param_1 + 100) = puVar5;
  *puVar5 = *(undefined4 *)((int)param_3 + uVar6);
  do {
  } while ((**(uint **)(param_1 + 8) & 0x40) == 0);
  (*(uint **)(param_1 + 8))[3] = 0x10;
  uVar2 = FUN_00001554(param_1);
  if ((uVar2 & 1) != 0) {
    return true;
  }
  if (*(char *)(param_1 + 0x6ecc) == '\x02') {
    iVar3 = func_0xffffee18(param_1);
    return iVar3 != 0;
  }
  return false;
}

