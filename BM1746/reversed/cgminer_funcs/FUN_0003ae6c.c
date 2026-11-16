//==================== FUN_0003ae6c @ 0003ae6c ====================

undefined4 FUN_0003ae6c(void)

{
  char acStack_1008 [4100];
  
  snprintf(acStack_1008,0x1000,"%s input param error: str length = %u\n",
           "bm1740_makeup_set_config_cmd",0xd);
  FUN_00038730(3,acStack_1008,0);
  return 0xffffffff;
}

