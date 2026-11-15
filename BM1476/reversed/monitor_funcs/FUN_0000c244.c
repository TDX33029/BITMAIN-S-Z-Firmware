//==================== FUN_0000c244 @ 0000c244 ====================

char * FUN_0000c244(int param_1,char *param_2)

{
  int iVar1;
  byte *pbVar2;
  uint uVar3;
  uint uVar4;
  byte bVar5;
  char *pcVar6;
  char cVar7;
  int iVar8;
  char *pcVar9;
  byte *pbVar10;
  byte *pbVar11;
  
  pcVar9 = param_2 + 1;
  if (*param_2 == '\"') {
    iVar1 = 0;
    for (pcVar6 = pcVar9; cVar7 = *pcVar6, cVar7 != '\0' && cVar7 != '\"'; pcVar6 = pcVar6 + 1) {
      while (iVar1 = iVar1 + 1, cVar7 == '\\') {
        pcVar6 = pcVar6 + 2;
        cVar7 = *pcVar6;
        if (cVar7 == '\0' || cVar7 == '\"') goto LAB_0000c2a0;
      }
    }
LAB_0000c2a0:
    pbVar2 = (byte *)(*DAT_0001b374)(iVar1 + 1);
    if (pbVar2 == (byte *)0x0) {
      pcVar9 = (char *)0x0;
    }
    else {
      bVar5 = param_2[1];
      pbVar10 = pbVar2;
      while (bVar5 != 0 && bVar5 != 0x22) {
        if (bVar5 == 0x5c) {
          pcVar6 = pcVar9 + 1;
          switch(pcVar9[1]) {
          case 'b':
            pbVar11 = pbVar10 + 1;
            *pbVar10 = 8;
            bVar5 = pcVar9[2];
            break;
          default:
            *pbVar10 = pcVar9[1];
            bVar5 = pcVar9[2];
            pbVar11 = pbVar10 + 1;
            break;
          case 'f':
            pbVar11 = pbVar10 + 1;
            *pbVar10 = 0xc;
            bVar5 = pcVar9[2];
            break;
          case 'n':
            pbVar11 = pbVar10 + 1;
            *pbVar10 = 10;
            bVar5 = pcVar9[2];
            break;
          case 'r':
            pbVar11 = pbVar10 + 1;
            *pbVar10 = 0xd;
            bVar5 = pcVar9[2];
            break;
          case 't':
            pbVar11 = pbVar10 + 1;
            *pbVar10 = 9;
            bVar5 = pcVar9[2];
            break;
          case 'u':
            uVar3 = FUN_0000c130(pcVar9 + 2);
            pcVar6 = pcVar9 + 5;
            pbVar11 = pbVar10;
            if (uVar3 == 0 || uVar3 - 0xdc00 < 0x400) {
              bVar5 = pcVar9[6];
            }
            else if (uVar3 - 0xd800 < 0x400) {
              bVar5 = pcVar9[6];
              if ((bVar5 == 0x5c) && (pcVar9[7] == 'u')) {
                uVar4 = FUN_0000c130(pcVar9 + 8);
                pcVar6 = pcVar9 + 0xb;
                if (uVar4 - 0xdc00 < 0x400) {
                  uVar3 = (uVar4 & 0x3ff | (uVar3 & 0x3ff) << 10) + 0x10000;
                  goto LAB_0000c478;
                }
                bVar5 = pcVar9[0xc];
              }
            }
            else {
LAB_0000c478:
              if (uVar3 < 0x80) {
                iVar8 = 1;
                iVar1 = 1;
              }
              else {
                if (uVar3 < 0x800) {
                  iVar8 = 2;
                  iVar1 = 2;
                }
                else {
                  if (uVar3 < 0x10000) {
                    iVar8 = 3;
                    iVar1 = 3;
                  }
                  else {
                    bVar5 = (byte)uVar3;
                    iVar8 = 4;
                    uVar3 = uVar3 >> 6;
                    pbVar10[3] = bVar5 & 0x3f | 0x80;
                    iVar1 = 4;
                  }
                  bVar5 = (byte)uVar3;
                  uVar3 = uVar3 >> 6;
                  pbVar10[2] = bVar5 & 0x3f | 0x80;
                }
                bVar5 = (byte)uVar3;
                uVar3 = uVar3 >> 6;
                pbVar10[1] = bVar5 & 0x3f | 0x80;
              }
              *pbVar10 = (byte)uVar3 | (&UNK_0001190c)[iVar1];
              bVar5 = pcVar6[1];
              pbVar11 = pbVar10 + iVar8;
            }
          }
          pcVar9 = pcVar6 + 1;
          pbVar10 = pbVar11;
        }
        else {
          *pbVar10 = bVar5;
          bVar5 = pcVar9[1];
          pcVar9 = pcVar9 + 1;
          pbVar10 = pbVar10 + 1;
        }
      }
      *pbVar10 = 0;
      cVar7 = *pcVar9;
      *(byte **)(param_1 + 0x10) = pbVar2;
      if (cVar7 == '\"') {
        pcVar9 = pcVar9 + 1;
      }
      *(undefined4 *)(param_1 + 0xc) = 4;
    }
  }
  else {
    pcVar9 = (char *)0x0;
    DAT_0001b3a4 = param_2;
  }
  return pcVar9;
}

