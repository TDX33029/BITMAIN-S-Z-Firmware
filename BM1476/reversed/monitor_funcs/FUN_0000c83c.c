//==================== FUN_0000c83c @ 0000c83c ====================

char * FUN_0000c83c(int param_1,int param_2,int param_3)

{
  void *__s;
  undefined4 *puVar1;
  char *pcVar2;
  char *pcVar3;
  size_t sVar4;
  size_t sVar5;
  char *pcVar6;
  char extraout_r3;
  char cVar7;
  int iVar8;
  int iVar9;
  undefined4 uVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  undefined4 *puVar14;
  int *piVar15;
  char *pcVar16;
  undefined4 *puVar17;
  bool bVar18;
  uint in_fpscr;
  undefined4 uVar19;
  double __x;
  double dVar20;
  double dVar21;
  int *local_44;
  
  if (param_1 == 0) {
    return (char *)0x0;
  }
  switch(*(undefined1 *)(param_1 + 0xc)) {
  case 0:
    pcVar16 = (char *)FUN_0000c574("false");
    return pcVar16;
  case 1:
    pcVar16 = (char *)FUN_0000c574(&DAT_00011934);
    return pcVar16;
  case 2:
    pcVar16 = (char *)FUN_0000c574(&DAT_00011924);
    return pcVar16;
  case 3:
    dVar21 = (double)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x14),
                                         (byte)(in_fpscr >> 0x16) & 3);
    __x = *(double *)(param_1 + 0x18);
    if (((2.220446049250313e-16 < ABS(dVar21 - __x)) || (2147483647.0 < __x)) ||
       (__x < -2147483648.0)) {
      pcVar16 = (char *)(*DAT_0001b374)(0x40);
      if (pcVar16 != (char *)0x0) {
        dVar21 = floor(__x);
        dVar20 = ABS(__x);
        uVar19 = SUB84(__x,0);
        uVar10 = (undefined4)((ulonglong)__x >> 0x20);
        if ((ABS(dVar21 - __x) <= 2.220446049250313e-16) && (dVar20 < 1e+60)) {
          sprintf(pcVar16,"%.0f",uVar19,uVar10);
          return pcVar16;
        }
        if ((1e-06 <= dVar20) && (dVar20 <= 1000000000.0)) {
          sprintf(pcVar16,"%f",uVar19,uVar10);
          return pcVar16;
        }
        sprintf(pcVar16,"%e",uVar19,uVar10);
        return pcVar16;
      }
    }
    else {
      pcVar16 = (char *)(*DAT_0001b374)(0x15);
      if (pcVar16 != (char *)0x0) {
        sprintf(pcVar16,"%d",*(undefined4 *)(param_1 + 0x14));
        return pcVar16;
      }
    }
    break;
  case 4:
    pcVar16 = (char *)FUN_0000c69c(*(undefined4 *)(param_1 + 0x10));
    return pcVar16;
  case 5:
    piVar15 = *(int **)(param_1 + 8);
    if (piVar15 == (int *)0x0) {
      pcVar16 = (char *)(*DAT_0001b374)(3);
      if (pcVar16 != (char *)0x0) {
        pcVar16[0] = '[';
        pcVar16[1] = ']';
        pcVar16[2] = '\0';
        return pcVar16;
      }
    }
    else {
      iVar12 = 0;
      while( true ) {
        piVar15 = (int *)*piVar15;
        iVar11 = iVar12 + 1;
        if (piVar15 == (int *)0x0) break;
        iVar12 = iVar11;
      }
      puVar1 = (undefined4 *)(*DAT_0001b374)(iVar11 * 4);
      if (puVar1 != (undefined4 *)0x0) {
        memset(puVar1,0,iVar11 * 4);
        piVar15 = *(int **)(param_1 + 8);
        if (piVar15 == (int *)0x0) {
          iVar8 = 5;
        }
        else {
          iVar8 = 5;
          puVar14 = puVar1 + -1;
          do {
            pcVar16 = (char *)FUN_0000c83c(piVar15,param_2 + 1,param_3);
            puVar14 = puVar14 + 1;
            *puVar14 = pcVar16;
            if (pcVar16 == (char *)0x0) goto LAB_0000ca30;
            sVar4 = strlen(pcVar16);
            piVar15 = (int *)*piVar15;
            if (param_3 == 0) {
              iVar9 = 2;
            }
            else {
              iVar9 = 3;
            }
            iVar8 = iVar9 + iVar8 + sVar4;
          } while (piVar15 != (int *)0x0);
        }
        pcVar16 = (char *)(*DAT_0001b374)(iVar8);
        if (pcVar16 != (char *)0x0) {
          iVar8 = 0;
          pcVar2 = pcVar16 + 1;
          *pcVar16 = '[';
          pcVar16[1] = '\0';
          puVar14 = puVar1;
          do {
            strcpy(pcVar2,(char *)*puVar14);
            pcVar3 = (char *)*puVar14;
            sVar4 = strlen(pcVar3);
            pcVar2 = pcVar2 + sVar4;
            if (iVar12 != iVar8) {
              *pcVar2 = ',';
              if (param_3 == 0) {
                pcVar2 = pcVar2 + 1;
              }
              else {
                pcVar2[1] = ' ';
                pcVar2 = pcVar2 + 2;
              }
              *pcVar2 = '\0';
              pcVar3 = (char *)*puVar14;
            }
            iVar8 = iVar8 + 1;
            (*(code *)PTR_free_0001b378)(pcVar3);
            puVar14 = puVar14 + 1;
          } while (iVar8 < iVar11);
          (*(code *)PTR_free_0001b378)(puVar1);
          *pcVar2 = ']';
          pcVar2[1] = '\0';
          return pcVar16;
        }
LAB_0000ca30:
        local_44 = puVar1 + -1;
        iVar12 = 0;
        do {
          local_44 = local_44 + 1;
          iVar12 = iVar12 + 1;
          if (*local_44 != 0) {
            (*(code *)PTR_free_0001b378)();
          }
        } while (iVar12 < iVar11);
        (*(code *)PTR_free_0001b378)(puVar1);
        return (char *)0x0;
      }
    }
    break;
  case 6:
    piVar15 = *(int **)(param_1 + 8);
    if (piVar15 == (int *)0x0) {
      if (param_3 == 0) {
        iVar12 = 3;
      }
      else {
        iVar12 = param_2 + 4;
      }
      pcVar16 = (char *)(*DAT_0001b374)(iVar12);
      if (pcVar16 == (char *)0x0) {
        return (char *)0x0;
      }
      *pcVar16 = '{';
      if (param_3 == 0) {
        pcVar2 = pcVar16 + 1;
      }
      else {
        pcVar16[1] = '\n';
        pcVar2 = pcVar16 + 2;
        if (1 < param_2) {
          pcVar6 = pcVar2 + param_2 + -1;
          pcVar3 = pcVar2;
          do {
            pcVar2 = pcVar3 + 1;
            *pcVar3 = '\t';
            pcVar3 = pcVar2;
          } while (pcVar2 != pcVar6);
        }
      }
      *pcVar2 = '}';
      pcVar2[1] = '\0';
      return pcVar16;
    }
    iVar12 = 0;
    while( true ) {
      piVar15 = (int *)*piVar15;
      iVar11 = iVar12 + 1;
      if (piVar15 == (int *)0x0) break;
      iVar12 = iVar11;
    }
    sVar4 = iVar11 * 4;
    __s = (void *)(*DAT_0001b374)(sVar4);
    if (__s == (void *)0x0) {
      return (char *)0x0;
    }
    puVar1 = (undefined4 *)(*DAT_0001b374)(sVar4);
    if (puVar1 == (undefined4 *)0x0) {
      (*(code *)PTR_free_0001b378)(__s);
      return (char *)0x0;
    }
    iVar8 = param_2 + 1;
    memset(__s,0,sVar4);
    memset(puVar1,0,sVar4);
    piVar15 = *(int **)(param_1 + 8);
    if (param_3 == 0) {
      pcVar16 = (char *)0x7;
    }
    else {
      pcVar16 = (char *)(param_2 + 8);
    }
    if (piVar15 != (int *)0x0) {
      if (param_3 == 0) {
        iVar9 = 0;
      }
      else {
        iVar9 = param_2 + 3;
      }
      iVar13 = 0;
      bVar18 = false;
      do {
        pcVar2 = (char *)FUN_0000c69c(piVar15[8]);
        *(char **)((int)puVar1 + iVar13) = pcVar2;
        pcVar3 = (char *)FUN_0000c83c(piVar15,iVar8,param_3);
        *(char **)((int)__s + iVar13) = pcVar3;
        if (pcVar3 == (char *)0x0 || pcVar2 == (char *)0x0) {
          bVar18 = true;
        }
        else {
          sVar4 = strlen(pcVar3);
          sVar5 = strlen(pcVar2);
          pcVar16 = pcVar16 + sVar5 + sVar4 + iVar9 + 2;
        }
        piVar15 = (int *)*piVar15;
        iVar13 = iVar13 + 4;
      } while (piVar15 != (int *)0x0);
      if (bVar18) goto LAB_0000cbcc;
    }
    pcVar2 = (char *)(*DAT_0001b374)(pcVar16);
    if (pcVar2 != (char *)0x0) {
      if (param_3 == 0) {
        pcVar16 = pcVar2 + 1;
      }
      *pcVar2 = '{';
      if (param_3 != 0) {
        pcVar16 = pcVar2 + 2;
        pcVar2[1] = '\n';
      }
      iVar9 = 0;
      puVar17 = (undefined4 *)((int)__s + -4);
      *pcVar16 = '\0';
      puVar14 = puVar1;
      do {
        if (param_3 == 0) {
          strcpy(pcVar16,(char *)*puVar14);
          sVar4 = strlen((char *)*puVar14);
          pcVar16[sVar4] = ':';
          pcVar3 = pcVar16 + sVar4 + 1;
        }
        else {
          if (0 < iVar8) {
            pcVar6 = pcVar16 + iVar8;
            pcVar3 = pcVar16;
            do {
              pcVar16 = pcVar3 + 1;
              *pcVar3 = '\t';
              pcVar3 = pcVar16;
            } while (pcVar16 != pcVar6);
          }
          strcpy(pcVar16,(char *)*puVar14);
          sVar4 = strlen((char *)*puVar14);
          pcVar16[sVar4] = ':';
          pcVar3 = pcVar16 + sVar4 + 2;
          pcVar16[sVar4 + 1] = '\t';
        }
        strcpy(pcVar3,(char *)puVar17[1]);
        sVar4 = strlen((char *)puVar17[1]);
        bVar18 = iVar12 != iVar9;
        iVar9 = iVar9 + 1;
        cVar7 = extraout_r3;
        if (bVar18) {
          cVar7 = ',';
        }
        pcVar6 = pcVar3 + sVar4;
        if (bVar18) {
          pcVar3[sVar4] = cVar7;
          pcVar6 = pcVar6 + 1;
        }
        pcVar16 = pcVar6;
        if (param_3 != 0) {
          pcVar16 = pcVar6 + 1;
          *pcVar6 = '\n';
        }
        *pcVar16 = '\0';
        (*(code *)PTR_free_0001b378)(*puVar14);
        puVar17 = puVar17 + 1;
        (*(code *)PTR_free_0001b378)(*puVar17);
        puVar14 = puVar14 + 1;
      } while (iVar9 < iVar11);
      (*(code *)PTR_free_0001b378)(puVar1);
      (*(code *)PTR_free_0001b378)(__s);
      if ((param_3 != 0) && (0 < param_2)) {
        pcVar6 = pcVar16 + param_2;
        pcVar3 = pcVar16;
        do {
          pcVar16 = pcVar3 + 1;
          *pcVar3 = '\t';
          pcVar3 = pcVar16;
        } while (pcVar16 != pcVar6);
      }
      *pcVar16 = '}';
      pcVar16[1] = '\0';
      return pcVar2;
    }
LAB_0000cbcc:
    iVar12 = 0;
    iVar8 = 0;
    do {
      iVar8 = iVar8 + 1;
      if (*(int *)((int)puVar1 + iVar12) != 0) {
        (*(code *)PTR_free_0001b378)();
      }
      piVar15 = (int *)((int)__s + iVar12);
      iVar12 = iVar12 + 4;
      if (*piVar15 != 0) {
        (*(code *)PTR_free_0001b378)();
      }
    } while (iVar8 < iVar11);
    (*(code *)PTR_free_0001b378)(puVar1);
    (*(code *)PTR_free_0001b378)(__s);
    return (char *)0x0;
  }
  return (char *)0x0;
}

