//==================== FUN_00045ec0 @ 00045ec0 ====================

void FUN_00045ec0(void)

{
  uint param4;
  float fVar1;
  char acStack_1010 [4096];
  
  DAT_00079128 = DAT_0009d6d6;
  if ((DAT_0009d6f7 == '\0') || (100 < (byte)DAT_0009d6f8)) {
    if (DAT_0009d6d6 == 0 || 0x3e < DAT_0009d6d6) {
      FUN_00045d88(100);
      DAT_0009d6f8._0_1_ = 100;
    }
    else if (DAT_0009d6d6 < 0x24) {
      FUN_00045d88(0x28);
      DAT_0009d6f8._0_1_ = 0x28;
    }
    else if (2 < (DAT_0009d6d6 - DAT_0007912c) + 1U) {
      fVar1 = (float)FUN_00045dec();
      param4 = (int)fVar1 & ~((int)fVar1 >> 0x1f);
      DAT_0009d6f8._0_1_ = (byte)param4;
      if ((DAT_00078e09 != '\0') &&
         (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
        snprintf(acStack_1010,0x1000,"%s: Set PWM percent : %d","set_PWM_according_to_temperature",
                 param4);
        FUN_00038730(7,acStack_1010,0);
      }
      FUN_00045d88(param4 & 0xff);
      DAT_0007912c = DAT_00079128;
    }
  }
  else {
    FUN_00045d88((byte)DAT_0009d6f8);
  }
  return;
}

