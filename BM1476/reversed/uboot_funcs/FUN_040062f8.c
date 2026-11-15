//==================== FUN_040062f8 @ 040062f8 ====================

void FUN_040062f8(uint param_1)

{
  uint uVar1;
  
  do {
    uVar1 = param_1;
    if (9999999 < param_1) {
      uVar1 = 10000000;
    }
    FUN_04006244(uVar1);
    param_1 = param_1 - uVar1;
  } while (param_1 != 0);
  return;
}

