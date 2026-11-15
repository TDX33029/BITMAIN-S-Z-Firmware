//==================== FUN_04005e0c @ 04005e0c ====================

void FUN_04005e0c(undefined4 *param_1,undefined1 param_2,uint param_3)

{
  uint uVar1;
  undefined4 *puVar3;
  undefined4 *puVar2;
  
  if (((uint)param_1 & 3) != 0) {
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
  if (3 < param_3) {
    uVar1 = param_3 - 4 >> 2;
    puVar3 = param_1;
    do {
      puVar2 = puVar3 + 1;
      *puVar3 = CONCAT31(CONCAT21(CONCAT11(param_2,param_2),param_2),param_2);
      puVar3 = puVar2;
    } while (param_1 + uVar1 + 1 != puVar2);
    param_3 = param_3 & 3;
    param_1 = param_1 + uVar1 + 1;
  }
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

