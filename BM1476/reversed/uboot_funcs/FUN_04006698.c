//==================== FUN_04006698 @ 04006698 ====================

void FUN_04006698(byte *param_1,byte *param_2,uint param_3,uint param_4,int param_5,int param_6,
                 int param_7,uint param_8)

{
  byte bVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  byte *pbVar5;
  byte *pbVar6;
  byte *pbVar7;
  int iVar8;
  byte bVar9;
  uint uVar10;
  uint uVar11;
  int iVar12;
  bool bVar13;
  byte local_64 [68];
  
  uVar10 = param_8 & 0x10;
  bVar9 = (byte)param_8 & 0x20;
  uVar2 = (uint)(param_5 != 10) & param_8 >> 6;
  if (uVar10 != 0) {
    param_8 = param_8 & 0xfffffffe;
  }
  if ((param_8 & 2) == 0) {
LAB_04006708:
    bVar1 = 0;
  }
  else if ((int)param_4 < 0) {
    bVar13 = param_3 != 0;
    param_3 = -param_3;
    param_4 = -(param_4 + bVar13);
    param_6 = param_6 + -1;
    bVar1 = 0x2d;
  }
  else if ((param_8 & 4) == 0) {
    if ((param_8 & 8) == 0) goto LAB_04006708;
    param_6 = param_6 + -1;
    bVar1 = 0x20;
  }
  else {
    param_6 = param_6 + -1;
    bVar1 = 0x2b;
  }
  if (uVar2 == 0) {
    if (param_3 != 0 || param_4 != 0) {
      if (param_5 == 10) {
        iVar4 = FUN_04006520(local_64);
        iVar4 = iVar4 - (int)local_64;
        iVar12 = iVar4 + -1;
        goto LAB_040067e4;
      }
LAB_04006738:
      if (param_5 == 0x10) {
        uVar3 = 0xf;
        iVar8 = 4;
      }
      else {
        uVar3 = param_5 - 1;
        iVar8 = 3;
      }
LAB_04006754:
      pbVar7 = local_64;
      iVar12 = 0;
      while( true ) {
        uVar11 = param_3 & 0xff;
        param_3 = param_3 >> iVar8 | param_4 << (0x20U - iVar8 & 0xff) |
                  param_4 >> (iVar8 - 0x20U & 0xff);
        *pbVar7 = bVar9 | *(byte *)(DAT_040069ec + (uVar11 & uVar3));
        iVar4 = iVar12 + 1;
        if (param_3 == 0 && param_4 >> iVar8 == 0) break;
        param_4 = param_4 >> iVar8;
        pbVar7 = pbVar7 + 1;
        iVar12 = iVar4;
      }
      goto LAB_040067e4;
    }
  }
  else if (param_5 == 0x10) {
    param_6 = param_6 + -2;
    if (param_3 != 0 || param_4 != 0) {
      iVar8 = 4;
      uVar3 = 0xf;
      goto LAB_04006754;
    }
  }
  else {
    param_6 = param_6 + -1;
    if (param_3 != 0 || param_4 != 0) goto LAB_04006738;
  }
  iVar12 = 0;
  iVar4 = 1;
  local_64[0] = 0x30;
LAB_040067e4:
  if (param_7 <= iVar4) {
    param_7 = iVar4;
  }
  param_6 = param_6 - param_7;
  iVar8 = param_6 + -1;
  if ((param_8 & 0x11) == 0) {
    if (iVar8 < 0) {
      iVar8 = param_6 + -2;
    }
    else {
      pbVar5 = param_1 + param_6;
      pbVar7 = param_1;
      do {
        param_1 = pbVar7 + 1;
        if (pbVar7 < param_2) {
          *pbVar7 = 0x20;
        }
        pbVar7 = param_1;
      } while (pbVar5 != param_1);
      iVar8 = -2;
    }
  }
  pbVar7 = param_1;
  if ((bVar1 != 0) && (pbVar7 = param_1 + 1, param_1 < param_2)) {
    *param_1 = bVar1;
  }
  pbVar5 = pbVar7;
  if (uVar2 != 0) {
    if (pbVar7 < param_2) {
      *pbVar7 = 0x30;
    }
    pbVar5 = pbVar7 + 1;
    if ((param_5 == 0x10) && (pbVar5 = pbVar7 + 2, pbVar7 + 1 < param_2)) {
      pbVar7[1] = bVar9 | 0x58;
    }
  }
  if (uVar10 == 0) {
    if ((param_8 & 1) == 0) {
      bVar9 = 0x20;
    }
    else {
      bVar9 = 0x30;
    }
    if (iVar8 < 0) {
      iVar8 = iVar8 + -1;
    }
    else {
      pbVar6 = pbVar5 + iVar8 + 1;
      pbVar7 = pbVar5;
      do {
        pbVar5 = pbVar7 + 1;
        if (pbVar7 < param_2) {
          *pbVar7 = bVar9;
        }
        pbVar7 = pbVar5;
      } while (pbVar6 != pbVar5);
      iVar8 = -2;
    }
  }
  if (iVar4 < param_7) {
    pbVar6 = pbVar5 + (param_7 - iVar4);
    pbVar7 = pbVar5;
    do {
      pbVar5 = pbVar7 + 1;
      if (pbVar7 < param_2) {
        *pbVar7 = 0x30;
      }
      pbVar7 = pbVar5;
    } while (pbVar6 != pbVar5);
  }
  if (-1 < iVar12) {
    iVar4 = iVar12;
    pbVar7 = pbVar5;
    do {
      if (pbVar7 < param_2) {
        *pbVar7 = local_64[iVar4];
      }
      bVar13 = iVar4 != 0;
      iVar4 = iVar4 + -1;
      pbVar7 = pbVar7 + 1;
    } while (bVar13);
    pbVar5 = pbVar5 + iVar12 + 1;
  }
  if (-1 < iVar8) {
    pbVar7 = pbVar5;
    do {
      pbVar6 = pbVar7 + 1;
      if (pbVar7 < param_2) {
        *pbVar7 = 0x20;
      }
      pbVar7 = pbVar6;
    } while (pbVar6 != pbVar5 + iVar8 + 1);
  }
  return;
}

