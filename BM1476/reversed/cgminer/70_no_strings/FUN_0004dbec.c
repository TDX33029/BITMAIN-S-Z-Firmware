//==================== FUN_0004dbec @ 0004dbec ====================

void FUN_0004dbec(uint *param_1,void *param_2,uint param_3)

{
  uint uVar1;
  size_t __n;
  void *pvVar2;
  uint uVar3;
  
  if ((int)param_3 < 1) {
    return;
  }
  uVar3 = *param_1;
  uVar1 = param_3 + uVar3;
  *param_1 = uVar1;
  uVar3 = uVar3 & 0x3f;
  __n = 0x40 - uVar3;
  if (uVar1 < param_3) {
    param_1[1] = param_1[1] + 1;
  }
  if (uVar3 != 0 && (int)__n <= (int)param_3) {
    param_3 = param_3 - __n;
    memcpy((void *)((int)param_1 + uVar3 + 0x28),param_2,__n);
    param_2 = (void *)((int)param_2 + __n);
    FUN_0004b4f0(param_1,param_1 + 10);
    uVar3 = 0;
  }
  if (0x3f < (int)param_3) {
    pvVar2 = param_2;
    uVar1 = param_3;
    do {
      uVar1 = uVar1 - 0x40;
      FUN_0004b4f0(param_1,pvVar2);
      pvVar2 = (void *)((int)pvVar2 + 0x40);
    } while (0x3f < (int)uVar1);
    uVar1 = param_3 - 0x40 >> 6;
    param_3 = (param_3 - 0x40) + uVar1 * -0x40;
    param_2 = (void *)((int)param_2 + (uVar1 + 1) * 0x40);
  }
  if ((int)param_3 < 1) {
    return;
  }
  memcpy((void *)((int)param_1 + uVar3 + 0x28),param_2,param_3);
  return;
}

