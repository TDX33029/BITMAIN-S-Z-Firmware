//==================== FUN_0005b7c8 @ 0005b7c8 ====================

void FUN_0005b7c8(undefined4 *param_1)

{
  char cVar1;
  undefined4 *puVar2;
  uint uVar3;
  char *pcVar4;
  undefined4 uVar5;
  char *pcVar6;
  undefined4 uVar7;
  int iVar8;
  int extraout_r3;
  undefined4 extraout_r3_00;
  undefined4 extraout_r3_01;
  undefined4 uVar9;
  int *unaff_r5;
  undefined4 *unaff_r6;
  undefined4 *unaff_r7;
  undefined4 *unaff_r8;
  undefined4 unaff_r10;
  char *extraout_r12;
  undefined4 *puVar10;
  char **ppcVar11;
  bool bVar12;
  bool bVar13;
  undefined1 uVar14;
  bool bVar15;
  undefined8 uVar16;
  char *local_28 [3];
  int local_1c;
  
  uVar3 = param_1[1];
  iVar8 = uVar3 - 1;
  bVar15 = 3 < uVar3;
  bVar12 = uVar3 == 4;
  if (!bVar12) {
    bVar15 = iVar8 != 0;
  }
  bVar13 = iVar8 == 1;
  uVar14 = bVar12 || bVar13;
  ppcVar11 = local_28;
  if (!bVar15 || (bVar12 || bVar13)) {
    iVar8 = 0;
  }
  if (bVar15 && (!bVar12 && !bVar13)) {
    iVar8 = 1;
  }
  puVar2 = param_1;
  if (bVar15 && (!bVar12 && !bVar13)) {
LAB_0005b958:
    errx(1,"Option %s: unknown entry type %u",*puVar2,uVar3);
  }
  else {
    pcVar6 = (char *)*param_1;
    uVar14 = 1;
    if (param_1[6] == 0) goto LAB_0005b978;
    uVar14 = 0;
    if (*pcVar6 == '-') {
      cVar1 = pcVar6[1];
      pcVar6 = pcVar6 + 1;
      if (cVar1 == '\0') {
        iVar8 = 0;
      }
      else if (((cVar1 != '|') && (cVar1 != '=')) && (cVar1 != ' ')) {
        pcVar4 = pcVar6;
        while( true ) {
          pcVar4 = pcVar4 + 1;
          cVar1 = *pcVar4;
          iVar8 = iVar8 + 1;
          if ((cVar1 == '\0') || (cVar1 == '|')) break;
          if ((cVar1 == '=') || (cVar1 == ' ')) break;
        }
      }
      unaff_r8 = &DAT_0009d770;
      unaff_r6 = &DAT_0009d778;
      unaff_r7 = &DAT_0009d76c;
      unaff_r5 = &local_1c;
      local_1c = iVar8;
      while (*pcVar6 == '-') {
        uVar14 = 1;
        uVar5 = 1;
        if (local_1c == 1) goto LAB_0005b948;
        iVar8 = param_1[1];
        DAT_0009d76c = DAT_0009d76c + 1;
LAB_0005b8b0:
        if (iVar8 == 1) {
          uVar5 = 1;
          uVar14 = 1;
          iVar8 = local_1c;
          if (pcVar6[local_1c] == ' ' || pcVar6[local_1c] == '=') goto LAB_0005b934;
        }
        pcVar6 = (char *)FUN_0005b544(pcVar6,unaff_r5);
        if (pcVar6 == (char *)0x0) {
          return;
        }
      }
      uVar14 = local_1c == 1;
      if ((bool)uVar14) {
        iVar8 = param_1[1];
        DAT_0009d770 = DAT_0009d770 + 1;
        if (iVar8 == 2 || iVar8 == 4) {
          DAT_0009d778 = DAT_0009d778 + 1;
        }
        goto LAB_0005b8b0;
      }
      local_28[0] = pcVar6 + -1;
      uVar5 = errx(1,"Option %s: invalid short option \'%.*s\'",*param_1,local_1c + 1);
      iVar8 = extraout_r3;
      pcVar6 = extraout_r12;
LAB_0005b934:
      errx(uVar5,"Option %s: does not take arguments \'%s\'",*param_1,pcVar6 + iVar8 + 1);
      uVar5 = extraout_r3_00;
LAB_0005b948:
      uVar16 = errx(uVar5,"Option %s: invalid long option \'--\'",*param_1);
      uVar3 = (uint)((ulonglong)uVar16 >> 0x20);
      puVar2 = (undefined4 *)uVar16;
      goto LAB_0005b958;
    }
  }
  errx(1,"Option %s: does not begin with \'-\'");
LAB_0005b978:
  puVar2 = (undefined4 *)errx(1,"Option %s: description cannot be NULL");
  if ((bool)uVar14) {
    ppcVar11 = (char **)((uint)unaff_r6 & (int)param_1 << ((uint)puVar2 & 0xff));
  }
  *(undefined4 *)((int)ppcVar11 + -4) = 0x5b984;
  *(undefined4 *)((int)ppcVar11 + -8) = unaff_r10;
  *(undefined4 **)((int)ppcVar11 + -0xc) = unaff_r8;
  *(undefined4 **)((int)ppcVar11 + -0x10) = unaff_r7;
  *(undefined4 **)((int)ppcVar11 + -0x14) = unaff_r6;
  *(int **)((int)ppcVar11 + -0x18) = unaff_r5;
  *(undefined4 **)((int)ppcVar11 + -0x1c) = param_1;
  *(undefined4 *)((int)ppcVar11 + -0x20) = extraout_r3_01;
  DAT_0009d77c = realloc(DAT_0009d77c,(DAT_0009d774 + 1) * 0x1c);
  puVar10 = (undefined4 *)((int)DAT_0009d77c + DAT_0009d774 * 0x1c);
  uVar5 = puVar2[1];
  uVar7 = puVar2[2];
  uVar9 = puVar2[3];
  *puVar10 = *puVar2;
  puVar10[1] = uVar5;
  puVar10[2] = uVar7;
  puVar10[3] = uVar9;
  uVar5 = puVar2[5];
  uVar7 = puVar2[6];
  DAT_0009d774 = DAT_0009d774 + 1;
  puVar10[4] = puVar2[4];
  puVar10[5] = uVar5;
  puVar10[6] = uVar7;
  return;
}

