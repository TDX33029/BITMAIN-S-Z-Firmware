//==================== FUN_0000e54c @ 0000e54c ====================

bool FUN_0000e54c(int param_1,int param_2,int param_3,void *param_4)

{
  if (param_3 != param_2) {
    memcpy(param_4,(void *)(param_1 + param_2 * 0x100f),0x100f);
  }
  return param_3 != param_2;
}

