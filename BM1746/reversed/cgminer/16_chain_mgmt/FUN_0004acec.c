//==================== FUN_0004acec @ 0004acec ====================

void FUN_0004acec(int param_1,char param_2,int param_3)

{
  uint uVar1;
  char acStack_1010 [4096];
  
  uVar1 = FUN_0003cc6c(param_1 + 2,param_2 * '\b' + -0x15);
  if ((*(byte *)(param_1 + 8) & 0x1f) == uVar1) {
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      uVar1 = FUN_0005c648(*(undefined1 *)(param_1 + 7),DAT_0009d6f3);
      snprintf(acStack_1010,0x1000,"chain[%d] asic[%d] %02x %02x %02x %02x\n",param_3,uVar1 & 0xff,
               (uint)*(byte *)(param_1 + 3),(uint)*(byte *)(param_1 + 4),
               (uint)*(byte *)(param_1 + 5),(uint)*(byte *)(param_1 + 6));
      FUN_00038730(7,acStack_1010,0);
    }
  }
  else if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
    snprintf(acStack_1010,0x1000,"%s chain%d CRC error crc = %02x\n","pm_handle",param_3,uVar1);
    FUN_00038730(3,acStack_1010,0);
  }
  return;
}

