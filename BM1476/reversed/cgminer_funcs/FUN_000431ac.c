//==================== FUN_000431ac @ 000431ac ====================

void FUN_000431ac(int param_1,uint param_2)

{
  undefined4 uStack_101c;
  undefined4 uStack_1018;
  undefined2 uStack_1014;
  undefined1 local_1012;
  char acStack_1010 [4096];
  
  uStack_1018 = 0;
  uStack_1014 = 0;
  uStack_101c = 0;
  local_1012 = 0;
  if ((DAT_00078e09 != '\0') &&
     (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
    snprintf(acStack_1010,0x1000,"chainid%d %s: nonce shift = %08x",param_1,"set_nonce_shift_chain",
             param_2);
    FUN_00038730(7,acStack_1010,0);
  }
  DAT_00076b50 = param_2 << 0x1b;
  FUN_0003cdcc(&uStack_101c,1,0,0x50,param_2 << 0x1b);
  FUN_0003c150(param_1,&uStack_101c,0xb);
  FUN_0002a92c(10);
  return;
}

