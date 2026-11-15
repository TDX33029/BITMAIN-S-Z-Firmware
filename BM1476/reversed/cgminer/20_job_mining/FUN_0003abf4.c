//==================== FUN_0003abf4 @ 0003abf4 ====================

void FUN_0003abf4(void)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  double *pdVar6;
  int iVar7;
  uint *puVar8;
  double *pdVar9;
  char *pcVar10;
  int iVar11;
  bool bVar12;
  uint in_fpscr;
  double dVar13;
  double dVar14;
  double dVar15;
  double dVar16;
  double dVar17;
  int local_1c0;
  int local_1bc;
  int local_1b8;
  int local_1b4;
  int local_1b0 [4];
  double local_1a0 [40];
  
  memset(local_1a0,0,0x140);
  local_1b0[0] = 0;
  local_1b0[1] = 0;
  local_1b0[2] = 0;
  local_1b0[3] = 0;
  FUN_0002a84c(&local_1c0);
  FUN_0002a84c(&local_1b8);
  do {
    pdVar6 = (double *)&DAT_00078e40;
    FUN_0002a84c(&local_1b8);
    iVar7 = 0;
    iVar4 = local_1b4 - local_1bc;
    bVar12 = iVar4 < 0;
    if (bVar12) {
      iVar4 = iVar4 + 0xf4000;
    }
    pcVar10 = &DAT_0009d4bb;
    iVar1 = local_1b8 - local_1c0;
    if (bVar12) {
      iVar4 = iVar4 + 0x240;
    }
    puVar8 = &DAT_00078e60;
    iVar2 = iVar1;
    if (bVar12) {
      iVar2 = iVar1 + -1;
    }
    iVar5 = 0;
    if (bVar12) {
      iVar1 = iVar2;
    }
    dVar13 = 0.0;
    do {
      pcVar10 = pcVar10 + 1;
      if (*pcVar10 != '\0') {
        uVar3 = *puVar8;
        iVar11 = *(int *)((int)local_1b0 + iVar7);
        dVar16 = (double)FUN_0005cb50(uVar3 * 0xffff,
                                      (puVar8[1] << 0x10 | uVar3 >> 0x10) -
                                      (puVar8[1] + (uint)(uVar3 * 0x10000 < uVar3)));
        dVar15 = (double)VectorSignedToFloat(iVar4 + 1,(byte)(in_fpscr >> 0x16) & 3);
        puVar8[0] = 0;
        puVar8[1] = 0;
        pdVar9 = local_1a0 + iVar5 * 10;
        iVar2 = 10;
        *pdVar6 = 0.0;
        dVar14 = 0.0;
        dVar17 = (double)VectorSignedToFloat(iVar1,(byte)(in_fpscr >> 0x16) & 3);
        local_1a0[iVar11 + iVar5 * 10] = dVar16 / (dVar17 + dVar15 / 1000000.0);
        do {
          iVar2 = iVar2 + -1;
          dVar15 = *pdVar9;
          pdVar9 = pdVar9 + 1;
          dVar14 = dVar14 + dVar15;
        } while (iVar2 != 0);
        *pdVar6 = dVar14;
        iVar11 = iVar11 + 1;
        dVar14 = (dVar14 / 1000.0) / 10.0;
        sprintf(&DAT_0007c0a8 + iVar5 * 0x10,"%.2f",SUB84(dVar14,0),(int)((ulonglong)dVar14 >> 0x20)
               );
        dVar14 = *pdVar6;
        if (9 < iVar11) {
          iVar11 = 0;
        }
        *(int *)((int)local_1b0 + iVar7) = iVar11;
        dVar13 = dVar13 + dVar14 / 10.0;
      }
      iVar5 = iVar5 + 1;
      iVar7 = iVar7 + 4;
      pdVar6 = pdVar6 + 1;
      puVar8 = puVar8 + 2;
    } while (iVar5 != 4);
    sprintf(&DAT_00078b84,"%.2f",SUB84(dVar13,0),(int)((ulonglong)dVar13 >> 0x20));
    DAT_00078e80 = dVar13;
    FUN_0002a530(&local_1c0,&local_1b8);
    sleep(5);
  } while( true );
}

