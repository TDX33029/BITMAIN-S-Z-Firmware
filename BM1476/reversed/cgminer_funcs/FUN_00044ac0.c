//==================== FUN_00044ac0 @ 00044ac0 ====================

void FUN_00044ac0(int param_1)

{
  char acStack_6c [100];
  
  DAT_00076b68 = DAT_00076b68 ^ 1;
  if (param_1 == 0) {
    sprintf(acStack_6c,"echo %d > /sys/class/gpio/gpio%d/value",0,0x3ad);
    system(acStack_6c);
    sprintf(acStack_6c,"echo %d > /sys/class/gpio/gpio%d/value",(uint)DAT_00076b68,0x3ae);
    system(acStack_6c);
  }
  else {
    sprintf(acStack_6c,"echo %d > /sys/class/gpio/gpio%d/value",0,0x3ae);
    system(acStack_6c);
    sprintf(acStack_6c,"echo %d > /sys/class/gpio/gpio%d/value",(uint)DAT_00076b68,0x3ad);
    system(acStack_6c);
  }
  return;
}

