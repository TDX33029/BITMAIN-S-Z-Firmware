//==================== FUN_040030a8 @ 040030a8 ====================

void FUN_040030a8(int *param_1,int *param_2,undefined4 *param_3,int param_4)

{
  int iVar1;
  
  func_0x03fc8570(*param_3);
  func_0x03fc8570(param_3[1]);
  iVar1 = *param_1;
  param_3[2] = 0;
  param_3[3] = 0;
  *(undefined4 *)(iVar1 + param_4 * 0x14) = 0xffffffff;
  *param_2 = *param_2 + -1;
  return;
}

