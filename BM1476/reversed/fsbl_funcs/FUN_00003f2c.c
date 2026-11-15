//==================== FUN_00003f2c @ 00003f2c ====================

int FUN_00003f2c(int *param_1,byte *param_2)

{
  byte bVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  byte *pbVar5;
  uint uVar6;
  undefined1 uVar7;
  byte bVar8;
  byte *pbVar9;
  byte *pbVar10;
  byte *pbVar11;
  byte *pbVar12;
  int iVar13;
  byte *pbVar14;
  char *pcVar15;
  uint uVar16;
  bool bVar17;
  
  pcVar15 = (char *)*param_1;
  if (*param_2 == 0x2f || *param_2 == 0x5c) {
    do {
      param_2 = param_2 + 1;
    } while (*param_2 == 0x5c || *param_2 == 0x2f);
  }
  param_1[2] = 0;
  if (*param_2 < 0x20) {
    *(undefined1 *)((int)param_1 + 0x2b) = 0x80;
    iVar13 = FUN_00003d50(param_1);
    return iVar13;
  }
  pbVar14 = (byte *)(param_1 + 8);
LAB_00003f78:
  pbVar12 = pbVar14;
  do {
    pbVar9 = pbVar12 + 1;
    *pbVar12 = 0x20;
    pbVar12 = pbVar9;
  } while (pbVar9 != (byte *)((int)param_1 + 0x2b));
  uVar4 = 0;
  uVar16 = 8;
  bVar8 = *param_2;
  iVar13 = 0;
  iVar3 = 1;
  while (uVar2 = (uint)bVar8, 0x20 < uVar2) {
    if (uVar2 == 0x5c || uVar2 == 0x2f) {
      pbVar12 = param_2 + iVar3;
      if (param_2[iVar3] == 0x2f || param_2[iVar3] == 0x5c) {
        pbVar9 = param_2 + iVar13 + 2;
        do {
          pbVar12 = pbVar9;
          pbVar9 = pbVar12 + 1;
        } while (*pbVar12 == 0x5c || *pbVar12 == 0x2f);
      }
      goto LAB_00003ffc;
    }
    bVar17 = 0x2d < uVar2;
    if (uVar2 != 0x2e) {
      bVar17 = uVar4 <= uVar16;
    }
    if (bVar17 && (uVar2 != 0x2e && uVar16 != uVar4)) {
      pbVar12 = DAT_00004250;
      if (uVar2 < 0x81) {
        if (uVar2 == 0x22) {
          return 6;
        }
        uVar6 = 0x2a;
      }
      else {
        if ((uVar2 < 0xa0) || ((uVar2 + 0x20 & 0xff) < 0x1d)) {
          bVar1 = param_2[iVar3];
          iVar3 = iVar13 + 2;
          if (bVar1 < 0x40) {
            return 6;
          }
          if ((0x7e < bVar1) && (0x7c < (bVar1 ^ 0x80))) {
            return 6;
          }
          if (uVar16 - 1 <= uVar4) {
            return 6;
          }
          pbVar14[uVar4] = bVar8;
          pbVar14[uVar4 + 1] = bVar1;
          uVar4 = uVar4 + 2;
          goto LAB_00003fe4;
        }
        uVar6 = 0x2a;
      }
      do {
        if (uVar2 == uVar6) {
          return 6;
        }
        uVar6 = (uint)pbVar12[1];
        pbVar12 = pbVar12 + 1;
      } while (uVar6 != 0);
      if (uVar2 - 0x61 < 0x1a) {
        bVar8 = bVar8 - 0x20;
      }
      pbVar14[uVar4] = bVar8;
      uVar4 = uVar4 + 1;
    }
    else {
      if (uVar16 == 0xb || uVar2 != 0x2e) {
        return 6;
      }
      uVar4 = 8;
      uVar16 = 0xb;
    }
LAB_00003fe4:
    bVar8 = param_2[iVar3];
    iVar13 = iVar3;
    iVar3 = iVar3 + 1;
  }
  pbVar12 = param_2 + iVar3;
LAB_00003ffc:
  if (uVar4 == 0) {
    return 6;
  }
  iVar13 = *param_1;
  if ((char)param_1[8] == -0x1b) {
    *(undefined1 *)(param_1 + 8) = 5;
  }
  if (uVar2 < 0x21) {
    uVar7 = 4;
  }
  else {
    uVar7 = 0;
  }
  *(undefined1 *)((int)param_1 + 0x2b) = uVar7;
  iVar3 = FUN_00003d50(param_1);
  if (iVar3 != 0) {
LAB_000040d4:
    if (iVar3 != 4) {
      return iVar3;
    }
LAB_000040dc:
    if ((*(byte *)((int)param_1 + 0x2b) & 4) == 0) {
      return 5;
    }
    return 4;
  }
  do {
    if ((param_1[6] != *(int *)(iVar13 + 0x2c)) && (iVar3 = FUN_00002f3c(iVar13), iVar3 != 0))
    goto LAB_000040d4;
    pbVar9 = (byte *)param_1[7];
    if (*pbVar9 == 0) goto LAB_000040dc;
    bVar8 = pbVar9[0xb];
    *(byte *)((int)param_1 + 6) = bVar8 & 0x3f;
    if ((pbVar9[0xb] & 8) == 0) {
      iVar3 = (uint)*pbVar9 - (uint)*(byte *)(param_1 + 8);
      pbVar5 = pbVar14;
      pbVar10 = pbVar9 + 1;
      do {
        if (iVar3 != 0) goto LAB_000040bc;
        pbVar11 = pbVar10 + 1;
        pbVar5 = pbVar5 + 1;
        iVar3 = (uint)*pbVar10 - (uint)*pbVar5;
        pbVar10 = pbVar11;
      } while (pbVar11 != pbVar9 + 0xb);
      if (iVar3 == 0) break;
    }
LAB_000040bc:
    iVar3 = FUN_00003b44(param_1,0);
    if (iVar3 != 0) goto LAB_000040d4;
  } while( true );
  if ((*(byte *)((int)param_1 + 0x2b) & 4) != 0) {
    return 0;
  }
  if ((bVar8 & 0x10) == 0) {
    return 5;
  }
  uVar4 = (uint)*(ushort *)(pcVar15 + (param_1[4] & 0x1ffU) + 0x5a);
  if (*pcVar15 == '\x03') {
    uVar4 = CONCAT22(*(undefined2 *)(pcVar15 + (param_1[4] & 0x1ffU) + 0x54),
                     *(ushort *)(pcVar15 + (param_1[4] & 0x1ffU) + 0x5a));
  }
  param_1[2] = uVar4;
  param_2 = pbVar12;
  goto LAB_00003f78;
}

