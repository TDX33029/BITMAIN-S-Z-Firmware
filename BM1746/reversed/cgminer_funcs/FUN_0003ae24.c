//==================== FUN_0003ae24 @ 0003ae24 ====================

undefined4 FUN_0003ae24(void)

{
  char acStack_1008 [4100];
  
  snprintf(acStack_1008,0x1000,"%s: i2c always busy, break\n","i2c_read");
  FUN_00038730(5,acStack_1008,0);
  return 0xff;
}

