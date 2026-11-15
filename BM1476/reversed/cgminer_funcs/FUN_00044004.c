//==================== FUN_00044004 @ 00044004 ====================

void FUN_00044004(int param_1,uint param_2)

{
  undefined4 uStack_101c;
  undefined4 uStack_1018;
  undefined2 uStack_1014;
  undefined1 local_1012;
  char acStack_1010 [4100];
  
  uStack_1018 = 0;
  uStack_101c = 0;
  uStack_1014 = 0;
  local_1012 = 0;
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    snprintf(acStack_1010,0x1000,"%s chainID%d ticket_mask = 0x%08x","set_ticket_mask_chain",param_1
             ,param_2);
    FUN_00038730(5,acStack_1010,0);
  }
  DAT_00076b60 = param_2 & 0xff;
  FUN_0003cdcc(&uStack_101c,1,0,0x14,DAT_00076b60);
  FUN_0003c150(param_1,&uStack_101c,0xb);
  FUN_0002a92c(1);
  return;
}

