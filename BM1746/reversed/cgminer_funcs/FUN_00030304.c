//==================== FUN_00030304 @ 00030304 ====================

void FUN_00030304(int *param_1,void *param_2,uint param_3,undefined4 param_4)

{
  uint uVar1;
  uint uVar2;
  uint __n;
  uint __n_00;
  
  __n = 0x40 - param_1[1];
  if (param_3 < __n) {
    __n = param_3;
  }
  memcpy((void *)((int)param_1 + param_1[1] + 8),param_2,__n);
  uVar2 = param_3 + param_1[1];
  if (uVar2 < 0x40) {
    param_1[1] = uVar2;
    return;
  }
  __n_00 = param_3 - __n & 0x3f;
  uVar1 = param_3 - __n >> 6;
  FUN_000300b0(param_1,param_1 + 2,1,uVar2,param_4);
  FUN_000300b0(param_1,(int)param_2 + __n,uVar1);
  memcpy(param_1 + 2,(void *)((int)param_2 + __n + uVar1 * 0x40),__n_00);
  param_1[1] = __n_00;
  *param_1 = *param_1 + (uVar1 + 1) * 0x40;
  return;
}

