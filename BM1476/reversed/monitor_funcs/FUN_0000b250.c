//==================== FUN_0000b250 @ 0000b250 ====================

uint FUN_0000b250(undefined4 *param_1,undefined4 *param_2,undefined4 *param_3)

{
  ushort uVar1;
  ushort **ppuVar2;
  char *pcVar3;
  uint uVar4;
  byte *pbVar5;
  byte *pbVar6;
  
  pbVar6 = (byte *)*param_1;
  if (pbVar6 != (byte *)0x0) {
    ppuVar2 = __ctype_b_loc();
    do {
      pbVar5 = pbVar6;
      pbVar6 = pbVar5 + 1;
    } while (((*ppuVar2)[*pbVar5] & 0x2000) != 0);
    if (*pbVar5 != 0) {
      *param_2 = pbVar5;
      do {
        pbVar5 = pbVar6;
        uVar4 = (uint)*pbVar5;
        if (uVar4 == 0) {
          return 0xffffffff;
        }
      } while ((((*ppuVar2)[uVar4] & 0x2000) == 0) && (pbVar6 = pbVar5 + 1, uVar4 != 0x3d));
      *pbVar5 = 0;
      while( true ) {
        uVar4 = (uint)pbVar5[1];
        if (uVar4 == 0) break;
        uVar1 = (*ppuVar2)[uVar4];
        if (((uVar1 & 0x2000) == 0) && (uVar4 != 0x3d)) {
          if (uVar4 != 0x22) {
            return 0xffffffff;
          }
          *param_3 = pbVar5 + 2;
          pcVar3 = strchr((char *)(pbVar5 + 2),0x22);
          if (pcVar3 == (char *)0x0) {
            return 0xffffffff;
          }
          *pcVar3 = '\0';
          return uVar1 & 0x2000;
        }
        pbVar5 = pbVar5 + 1;
      }
    }
  }
  return 0xffffffff;
}

