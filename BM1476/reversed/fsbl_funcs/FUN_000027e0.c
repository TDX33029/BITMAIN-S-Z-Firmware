//==================== FUN_000027e0 @ 000027e0 ====================

void FUN_000027e0(undefined4 *param_1,undefined1 param_2,uint param_3)

{
  uint uVar1;
  undefined4 *puVar3;
  undefined4 *puVar2;
  undefined4 uVar4;
  
  if (((uint)param_1 & 3) != 0) {
    if (param_3 == 0) {
      return;
    }
    puVar3 = param_1;
    while( true ) {
      param_3 = param_3 - 1;
      param_1 = (undefined4 *)((int)puVar3 + 1);
      *(undefined1 *)puVar3 = param_2;
      if (((uint)param_1 & 3) == 0) break;
      puVar3 = param_1;
      if (param_3 == 0) {
        return;
      }
    }
  }
  if (param_3 < 4) {
    if (param_3 == 0) {
      return;
    }
    puVar3 = param_1;
    do {
      puVar2 = (undefined4 *)((int)puVar3 + 1);
      *(undefined1 *)puVar3 = param_2;
      puVar3 = puVar2;
    } while ((undefined4 *)((int)param_1 + param_3) != puVar2);
    return;
  }
  uVar4 = CONCAT22(CONCAT11(param_2,param_2),CONCAT11(param_2,param_2));
  if (0xf < param_3) {
    uVar1 = param_3 - 0x10 >> 4;
    puVar3 = param_1 + 4;
    do {
      puVar3[-4] = uVar4;
      puVar2 = puVar3 + 4;
      puVar3[-3] = uVar4;
      puVar3[-2] = uVar4;
      puVar3[-1] = uVar4;
      puVar3 = puVar2;
    } while (puVar2 != param_1 + uVar1 * 4 + 8);
    param_3 = param_3 & 0xf;
    param_1 = param_1 + (uVar1 + 1) * 4;
    if (param_3 < 4) goto LAB_000028a8;
  }
  uVar1 = param_3 - 4 >> 2;
  puVar3 = param_1;
  do {
    puVar2 = puVar3 + 1;
    *puVar3 = uVar4;
    puVar3 = puVar2;
  } while (puVar2 != param_1 + uVar1 + 1);
  param_3 = param_3 & 3;
  param_1 = param_1 + uVar1 + 1;
LAB_000028a8:
  if (param_3 == 0) {
    return;
  }
  puVar3 = param_1;
  do {
    puVar2 = (undefined4 *)((int)puVar3 + 1);
    *(undefined1 *)puVar3 = param_2;
    puVar3 = puVar2;
  } while ((undefined4 *)((int)param_1 + param_3) != puVar2);
  return;
}

