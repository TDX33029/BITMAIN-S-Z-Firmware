//==================== FUN_0002f0b4 @ 0002f0b4 ====================

void FUN_0002f0b4(char *param_1)

{
  char acStack_18 [20];
  
  snprintf(acStack_18,0x10,"cg@%s",param_1);
  prctl(0xf,acStack_18,0,0,0);
  return;
}

