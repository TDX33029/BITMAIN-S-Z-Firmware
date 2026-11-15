//==================== FUN_00042a60 @ 00042a60 ====================

undefined1 FUN_00042a60(undefined1 *param_1,int param_2)

{
  undefined1 uVar1;
  byte *pbVar3;
  byte *pbVar2;
  uint uVar4;
  char acStack_1010 [4096];
  
  if (param_2 == 0x92) {
    uVar4 = 0xffff;
    *param_1 = 0x55;
    param_1[2] = param_1[2] & 0x1f | 0x20;
    param_1[1] = 0xaa;
    pbVar3 = param_1 + 2;
    do {
      pbVar2 = pbVar3 + 1;
      uVar4 = ((uint)*(ushort *)(&DAT_00068aec + ((uint)*pbVar3 ^ uVar4 >> 8) * 2) ^ uVar4 << 8) &
              0xffff;
      pbVar3 = pbVar2;
    } while (pbVar2 != param_1 + 0x90);
    uVar1 = 0x92;
    *(ushort *)(param_1 + 0x90) = (ushort)(uVar4 >> 8) | (ushort)(uVar4 << 8);
  }
  else if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (uVar1 = 0, 3 < DAT_000766c4)) {
    snprintf(acStack_1010,0x1000,"%s len error\n","bm1740_makeup_work");
    FUN_00038730(4,acStack_1010,0);
    uVar1 = 0;
  }
  return uVar1;
}

