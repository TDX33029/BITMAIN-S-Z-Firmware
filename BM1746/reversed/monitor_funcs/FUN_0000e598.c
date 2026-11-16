//==================== FUN_0000e598 @ 0000e598 ====================

bool FUN_0000e598(int *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  bool bVar1;
  undefined4 local_c;
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  bVar1 = (param_1[2] + 1) % 9 != param_1[1];
  if (bVar1) {
    local_c = param_2;
    uStack_8 = param_3;
    uStack_4 = param_4;
    memcpy((void *)(*param_1 + param_1[2] * 0x100f),&local_c,0x100f);
    param_1[2] = (param_1[2] + 1) % 9;
  }
  return bVar1;
}

