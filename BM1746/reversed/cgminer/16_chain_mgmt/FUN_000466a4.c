//==================== FUN_000466a4 @ 000466a4 ====================

int FUN_000466a4(int param_1)

{
  char acStack_1008 [4100];
  
  if ((param_1 != 1) && (param_1 != 2)) {
    if (param_1 - 3U < 2) {
      param_1 = 4;
    }
    else if (param_1 - 5U < 4) {
      param_1 = 8;
    }
    else if (param_1 - 9U < 8) {
      param_1 = 0x10;
    }
    else if (param_1 - 0x11U < 0x10) {
      param_1 = 0x20;
    }
    else if (param_1 - 0x21U < 0x20) {
      param_1 = 0x40;
    }
    else if (param_1 - 0x41U < 0x40) {
      param_1 = 0x80;
    }
    else if ((DAT_00078e09 == '\0') ||
            (((DAT_0007ad48 == '\0' && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7)))) {
      param_1 = -1;
    }
    else {
      snprintf(acStack_1008,0x1000,"actual_asic_number = %d, but it is error\n",param_1);
      FUN_00038730(7,acStack_1008,0);
      param_1 = -1;
    }
  }
  return param_1;
}

