//==================== FUN_0000af74 @ 0000af74 ====================

void FUN_0000af74(void)

{
  int iVar1;
  undefined1 *puVar2;
  uint uVar3;
  char *pcVar4;
  uint uVar5;
  undefined1 *puVar7;
  undefined1 *puVar6;
  uint uVar8;
  uint uVar9;
  char cVar10;
  char cVar11;
  uint uVar12;
  bool bVar13;
  
  puVar2 = &DAT_00028eac;
  do {
    puVar7 = puVar2;
    do {
      puVar6 = puVar7 + 5;
      *puVar7 = 0;
      puVar7 = puVar6;
    } while (puVar6 != puVar2 + 0x9b);
    puVar2 = puVar2 + 1;
  } while (puVar2 != &DAT_00028eb1);
  uVar12 = 0;
  do {
    uVar5 = 2;
    uVar3 = 0;
    while( true ) {
      uVar8 = (uVar5 & 0xff) - 1;
      uVar9 = uVar8 & 0xff;
      if (uVar5 == 0x20) break;
      uVar8 = uVar8 & 0xfb;
      bVar13 = 2 < uVar8;
      if (uVar8 != 3) {
        bVar13 = uVar9 != 0;
      }
      if (bVar13 && (uVar8 != 3 && uVar9 != 1)) {
        if ((uVar5 & 0xff) != 0x10) {
          iVar1 = uVar3 * 5;
          uVar3 = uVar3 + 1 & 0xff;
          if ((int)(uVar5 & 1 << (uVar12 & 0xff)) >> (uVar12 & 0xff) == 1) {
            (&DAT_00028eac)[uVar12 + iVar1] = 1;
          }
          else {
            (&DAT_00028eac)[uVar12 + iVar1] = 0;
          }
        }
      }
      uVar5 = uVar5 + 1;
    }
    uVar12 = uVar12 + 1;
  } while (uVar12 != 5);
  uVar12 = 0;
  do {
    uVar3 = 0;
    puVar2 = &DAT_00028eac + uVar12;
    do {
      uVar5 = uVar3 & 0xff;
      uVar3 = uVar3 + 1;
      if (uVar5 == (uVar12 & 0xff)) {
        puVar2[0x82] = 1;
      }
      else {
        puVar2[0x82] = 0;
      }
      puVar2 = puVar2 + 5;
    } while (uVar3 != 5);
    uVar12 = uVar12 + 1;
  } while (uVar12 != 5);
  uVar12 = 0;
  cVar10 = '\0';
  cVar11 = '\x1a';
  pcVar4 = &DAT_00028e8d;
  DAT_00028e8c = 0xff;
  do {
    uVar3 = uVar12 & 0xff;
    if (((uVar3 - 3 & 0xfb) == 0 || uVar3 < 2) || (uVar3 == 0xf)) {
      *pcVar4 = cVar11;
      cVar11 = cVar11 + '\x01';
    }
    else {
      *pcVar4 = cVar10;
      cVar10 = cVar10 + '\x01';
    }
    uVar12 = uVar12 + 1;
    pcVar4 = pcVar4 + 1;
  } while (uVar12 != 0x1f);
  return;
}

