//==================== FUN_04005ea4 @ 04005ea4 ====================

void FUN_04005ea4(undefined4 *param_1,undefined4 *param_2,uint param_3)

{
  uint uVar1;
  undefined4 *puVar3;
  undefined4 *puVar2;
  int iVar4;
  undefined1 *puVar5;
  undefined4 *puVar6;
  
  if (param_1 == param_2) {
    return;
  }
  if (((((uint)param_2 | (uint)param_1) & 3) != 0) || (param_3 < 4)) {
    if (param_3 == 0) {
      return;
    }
    puVar5 = (undefined1 *)((int)param_1 + -1);
    puVar3 = param_2;
    do {
      puVar2 = (undefined4 *)((int)puVar3 + 1);
      puVar5 = puVar5 + 1;
      *puVar5 = *(undefined1 *)puVar3;
      puVar3 = puVar2;
    } while (puVar2 != (undefined4 *)((int)param_2 + param_3));
    return;
  }
  uVar1 = param_3 - 4 >> 2;
  puVar3 = param_1;
  puVar2 = param_2;
  do {
    puVar6 = puVar3 + 1;
    *puVar3 = *puVar2;
    puVar3 = puVar6;
    puVar2 = puVar2 + 1;
  } while (puVar6 != param_1 + uVar1 + 1);
  iVar4 = uVar1 + 1;
  if ((param_3 & 3) == 0) {
    return;
  }
  puVar5 = (undefined1 *)((int)param_1 + iVar4 * 4 + -1);
  puVar3 = param_2 + iVar4;
  do {
    puVar2 = (undefined4 *)((int)puVar3 + 1);
    puVar5 = puVar5 + 1;
    *puVar5 = *(undefined1 *)puVar3;
    puVar3 = puVar2;
  } while (puVar2 != (undefined4 *)((int)(param_2 + iVar4) + (param_3 & 3)));
  return;
}

