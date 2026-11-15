//==================== FUN_04005fa0 @ 04005fa0 ====================

void FUN_04005fa0(undefined1 *param_1,undefined1 *param_2,int param_3)

{
  undefined1 *puVar1;
  undefined1 *puVar2;
  
  if (param_2 == param_1) {
    return;
  }
  if (param_2 < param_1) {
    param_1 = param_1 + param_3;
    puVar2 = param_2 + param_3;
    if (param_3 == 0) {
      return;
    }
    do {
      puVar2 = puVar2 + -1;
      param_1 = param_1 + -1;
      *param_1 = *puVar2;
    } while (param_2 != puVar2);
    return;
  }
  if (param_3 == 0) {
    return;
  }
  param_1 = param_1 + -1;
  puVar2 = param_2;
  do {
    puVar1 = puVar2 + 1;
    param_1 = param_1 + 1;
    *param_1 = *puVar2;
    puVar2 = puVar1;
  } while (param_2 + param_3 != puVar1);
  return;
}

