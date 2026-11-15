//==================== FUN_0000ba3c @ 0000ba3c ====================

undefined4 FUN_0000ba3c(int param_1,char *param_2)

{
  undefined4 uVar1;
  
  if ((param_1 - 0x30U & 0xff) < 10) {
    *param_2 = (char)(param_1 - 0x30U);
    return 0;
  }
  if (param_1 - 0x61U < 6) {
    *param_2 = (char)param_1 + -0x57;
    return 0;
  }
  if (param_1 - 0x41U < 6) {
    uVar1 = 0;
    *param_2 = (char)param_1 + -0x37;
  }
  else {
    uVar1 = 0x11;
  }
  return uVar1;
}

