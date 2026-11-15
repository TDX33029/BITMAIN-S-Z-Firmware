//==================== FUN_0003a354 @ 0003a354 ====================

void FUN_0003a354(void)

{
  char acStack_1008 [4100];
  
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    snprintf(acStack_1008,0x1000,"%s %d","reset_all_hash_board_low",0x1122);
    FUN_00038730(5,acStack_1008,0);
  }
  *(undefined4 *)(DAT_00078e10 + 0x34) = 0xffff;
  return;
}

