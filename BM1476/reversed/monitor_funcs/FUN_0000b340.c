//==================== FUN_0000b340 @ 0000b340 ====================

uint FUN_0000b340(undefined4 *param_1,undefined4 *param_2,undefined4 *param_3)

{
  ushort uVar1;
  ushort **ppuVar2;
  int iVar3;
  byte *pbVar4;
  char *pcVar5;
  uint uVar6;
  byte *pbVar7;
  ushort *puVar8;
  
  pbVar7 = (byte *)*param_1;
  if (pbVar7 != (byte *)0x0) {
    ppuVar2 = __ctype_b_loc();
    puVar8 = *ppuVar2;
    do {
      pbVar4 = pbVar7;
      pbVar7 = pbVar4 + 1;
    } while ((puVar8[*pbVar4] & 0x2000) != 0);
    if ((*pbVar4 != 0) && (iVar3 = strncmp((char *)pbVar4,"option",6), iVar3 == 0)) {
      uVar6 = (uint)pbVar4[6];
      pbVar7 = pbVar4 + 6;
      if ((puVar8[uVar6] & 0x2000) != 0) {
        pbVar4 = pbVar4 + 7;
        do {
          pbVar7 = pbVar4;
          uVar6 = (uint)*pbVar7;
          pbVar4 = pbVar7 + 1;
        } while ((puVar8[uVar6] & 0x2000) != 0);
      }
      if (uVar6 != 0) {
        *param_2 = pbVar7;
        do {
          pbVar4 = pbVar7;
          pbVar7 = pbVar4 + 1;
          if (*pbVar7 == 0) {
            return 0xffffffff;
          }
        } while (((*ppuVar2)[*pbVar7] & 0x2000) == 0);
        *pbVar7 = 0;
        pbVar7 = pbVar4 + 2;
        do {
          pbVar4 = pbVar7 + 1;
          uVar6 = (uint)*pbVar7;
          if (uVar6 == 0) {
            return 0xffffffff;
          }
          uVar1 = (*ppuVar2)[uVar6];
          pbVar7 = pbVar4;
        } while ((uVar1 & 0x2000) != 0);
        if (uVar6 == 0x27) {
          *param_3 = pbVar4;
          pcVar5 = strchr((char *)pbVar4,0x27);
          if (pcVar5 != (char *)0x0) {
            *pcVar5 = '\0';
            return uVar1 & 0x2000;
          }
        }
      }
    }
  }
  return 0xffffffff;
}

