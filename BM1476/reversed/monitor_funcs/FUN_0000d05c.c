//==================== FUN_0000d05c @ 0000d05c ====================

/* WARNING: Type propagation algorithm not settling */

byte * FUN_0000d05c(int param_1,byte *param_2)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  uint uVar4;
  int iVar5;
  byte bVar6;
  uint uVar7;
  byte *pbVar8;
  byte *pbVar9;
  uint in_fpscr;
  double dVar10;
  undefined4 uVar11;
  double dVar12;
  double dVar13;
  double dVar14;
  
  if (param_2 == (byte *)0x0) {
    return (byte *)0x0;
  }
  iVar1 = strncmp((char *)param_2,"null",4);
  if (iVar1 == 0) {
    *(undefined4 *)(param_1 + 0xc) = 2;
    return param_2 + 4;
  }
  iVar1 = strncmp((char *)param_2,"false",5);
  if (iVar1 == 0) {
    *(undefined4 *)(param_1 + 0xc) = 0;
    return param_2 + 5;
  }
  iVar1 = strncmp((char *)param_2,"true",4);
  if (iVar1 == 0) {
    *(undefined4 *)(param_1 + 0xc) = 1;
    *(undefined4 *)(param_1 + 0x14) = 1;
    return param_2 + 4;
  }
  uVar7 = (uint)*param_2;
  if (uVar7 == 0x22) {
    pbVar9 = (byte *)FUN_0000c244(param_1,param_2);
    return pbVar9;
  }
  if (uVar7 - 0x30 < 10 || uVar7 == 0x2d) {
    if (uVar7 == 0x2d) {
      uVar7 = (uint)param_2[1];
      dVar12 = -1.0;
      param_2 = param_2 + 1;
    }
    else {
      dVar12 = 1.0;
    }
    if (uVar7 == 0x30) {
      uVar7 = (uint)param_2[1];
      param_2 = param_2 + 1;
    }
    if (uVar7 - 0x31 < 9) {
      uVar4 = uVar7 - 0x30;
      dVar13 = 0.0;
      do {
        param_2 = param_2 + 1;
        uVar7 = (uint)*param_2;
        dVar14 = (double)VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
        uVar4 = uVar7 - 0x30;
        dVar13 = dVar14 + dVar13 * 10.0;
      } while ((uVar4 & 0xff) < 10);
    }
    else {
      dVar13 = 0.0;
    }
    if (uVar7 == 0x2e) {
      uVar7 = (uint)param_2[1];
      if ((uVar7 < 0x30) || (0x39 < uVar7)) {
        iVar1 = 0;
        dVar14 = 0.0;
        goto LAB_0000d280;
      }
      dVar14 = 0.0;
      pbVar9 = param_2 + 2;
      uVar4 = uVar7 - 0x30;
      do {
        pbVar8 = pbVar9;
        uVar7 = (uint)*pbVar8;
        dVar14 = dVar14 - 1.0;
        dVar10 = (double)VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
        dVar13 = dVar10 + dVar13 * 10.0;
        pbVar9 = pbVar8 + 1;
        uVar4 = uVar7 - 0x30;
      } while ((uVar7 - 0x30 & 0xff) < 10);
    }
    else {
      dVar14 = 0.0;
      pbVar8 = param_2;
    }
    param_2 = pbVar8;
    if ((uVar7 & 0xdf) == 0x45) {
      uVar7 = (uint)pbVar8[1];
      if (uVar7 == 0x2b) {
        param_2 = pbVar8 + 2;
        uVar7 = (uint)pbVar8[2];
        iVar1 = 1;
      }
      else if (uVar7 == 0x2d) {
        param_2 = pbVar8 + 2;
        uVar7 = (uint)pbVar8[2];
        iVar1 = -1;
      }
      else {
        param_2 = pbVar8 + 1;
        iVar1 = 1;
      }
      uVar7 = uVar7 - 0x30;
      if ((uVar7 & 0xff) < 10) {
        iVar5 = 0;
        do {
          param_2 = param_2 + 1;
          iVar5 = uVar7 + iVar5 * 10;
          uVar7 = *param_2 - 0x30;
        } while ((uVar7 & 0xff) < 10);
        iVar1 = iVar1 * iVar5;
        goto LAB_0000d280;
      }
    }
    iVar1 = 0;
LAB_0000d280:
    dVar10 = (double)VectorSignedToFloat(iVar1,(byte)(in_fpscr >> 0x16) & 3);
    dVar14 = pow(10.0,dVar10 + dVar14);
    *(undefined4 *)(param_1 + 0xc) = 3;
    dVar14 = dVar12 * dVar13 * dVar14;
    uVar11 = VectorFloatToSigned(dVar14,3);
    *(double *)(param_1 + 0x18) = dVar14;
    *(undefined4 *)(param_1 + 0x14) = uVar11;
    return param_2;
  }
  if (uVar7 == 0x5b) {
    pbVar9 = param_2 + 1;
    *(undefined4 *)(param_1 + 0xc) = 5;
    bVar6 = param_2[1];
    if (pbVar9 == (byte *)0x0) {
LAB_0000d638:
      if (bVar6 == 0x5d) goto LAB_0000d154;
    }
    else if (bVar6 != 0) {
      if (bVar6 < 0x21) {
        pbVar8 = param_2 + 2;
        do {
          pbVar9 = pbVar8;
          if (pbVar9 == (byte *)0x0) {
            bVar6 = bRam00000000;
            break;
          }
          bVar6 = *pbVar9;
          if (bVar6 == 0) goto LAB_0000d640;
          pbVar8 = pbVar9 + 1;
        } while (bVar6 < 0x21);
      }
      goto LAB_0000d638;
    }
LAB_0000d640:
    piVar2 = (int *)FUN_0000c540();
    *(int **)(param_1 + 8) = piVar2;
    if (piVar2 == (int *)0x0) {
      return (byte *)0x0;
    }
    do {
      pbVar8 = pbVar9 + 1;
      if ((*pbVar9 == 0) || (0x20 < *pbVar9)) break;
      pbVar9 = pbVar8;
    } while (pbVar8 != (byte *)0x0);
    pbVar9 = (byte *)FUN_0000d05c(piVar2,pbVar9);
    if (pbVar9 == (byte *)0x0) {
      return (byte *)0x0;
    }
    bVar6 = *pbVar9;
    while ((bVar6 != 0 && (bVar6 < 0x21))) {
      pbVar9 = pbVar9 + 1;
      if (pbVar9 == (byte *)0x0) {
        return (byte *)0x0;
      }
      bVar6 = *pbVar9;
    }
    while (bVar6 == 0x2c) {
      piVar3 = (int *)FUN_0000c540();
      if (piVar3 == (int *)0x0) {
        return (byte *)0x0;
      }
      pbVar8 = pbVar9 + 1;
      *piVar2 = (int)piVar3;
      piVar3[1] = (int)piVar2;
      if (((pbVar8 != (byte *)0x0) && (pbVar9[1] != 0)) && (pbVar9[1] < 0x21)) {
        pbVar9 = pbVar9 + 2;
        while (pbVar8 = pbVar9, pbVar8 != (byte *)0x0) {
          if ((*pbVar8 == 0) || (pbVar9 = pbVar8 + 1, 0x20 < *pbVar8)) break;
        }
      }
      pbVar9 = (byte *)FUN_0000d05c(piVar3,pbVar8);
      if (pbVar9 == (byte *)0x0) {
        return (byte *)0x0;
      }
      bVar6 = *pbVar9;
      while ((bVar6 != 0 && (bVar6 < 0x21))) {
        pbVar9 = pbVar9 + 1;
        if (pbVar9 == (byte *)0x0) {
          return (byte *)0x0;
        }
        bVar6 = *pbVar9;
      }
      piVar2 = piVar3;
    }
    if (bVar6 != 0x5d) {
      DAT_0001b3a4 = pbVar9;
      return (byte *)0x0;
    }
    goto LAB_0000d7bc;
  }
  if (uVar7 != 0x7b) {
    DAT_0001b3a4 = param_2;
    return (byte *)0x0;
  }
  pbVar9 = param_2 + 1;
  *(undefined4 *)(param_1 + 0xc) = 6;
  bVar6 = param_2[1];
  if (pbVar9 == (byte *)0x0) {
LAB_0000d14c:
    if (bVar6 == 0x7d) {
LAB_0000d154:
      return pbVar9 + 1;
    }
  }
  else if (bVar6 != 0) {
    if (bVar6 < 0x21) {
      pbVar8 = param_2 + 2;
      do {
        pbVar9 = pbVar8;
        if (pbVar9 == (byte *)0x0) {
          bVar6 = bRam00000000;
          break;
        }
        bVar6 = *pbVar9;
        if (bVar6 == 0) goto LAB_0000d2b0;
        pbVar8 = pbVar9 + 1;
      } while (bVar6 < 0x21);
    }
    goto LAB_0000d14c;
  }
LAB_0000d2b0:
  piVar2 = (int *)FUN_0000c540();
  *(int **)(param_1 + 8) = piVar2;
  if (piVar2 != (int *)0x0) {
    do {
      pbVar8 = pbVar9 + 1;
      if ((*pbVar9 == 0) || (0x20 < *pbVar9)) break;
      pbVar9 = pbVar8;
    } while (pbVar8 != (byte *)0x0);
    pbVar9 = (byte *)FUN_0000c244(piVar2,pbVar9);
    if (pbVar9 != (byte *)0x0) {
      bVar6 = *pbVar9;
      while ((bVar6 != 0 && (bVar6 < 0x21))) {
        pbVar9 = pbVar9 + 1;
        if (pbVar9 == (byte *)0x0) {
          return (byte *)0x0;
        }
        bVar6 = *pbVar9;
      }
      iVar1 = piVar2[4];
      piVar2[4] = 0;
      piVar2[8] = iVar1;
      if (*pbVar9 != 0x3a) {
        DAT_0001b3a4 = pbVar9;
        return (byte *)0x0;
      }
      pbVar8 = pbVar9 + 1;
      if (((pbVar8 != (byte *)0x0) && (pbVar9[1] != 0)) && (pbVar9[1] < 0x21)) {
        pbVar9 = pbVar9 + 2;
        while (pbVar8 = pbVar9, pbVar8 != (byte *)0x0) {
          if ((*pbVar8 == 0) || (pbVar9 = pbVar8 + 1, 0x20 < *pbVar8)) break;
        }
      }
      pbVar9 = (byte *)FUN_0000d05c(piVar2,pbVar8);
      if (pbVar9 != (byte *)0x0) {
        bVar6 = *pbVar9;
        while ((bVar6 != 0 && (bVar6 < 0x21))) {
          pbVar9 = pbVar9 + 1;
          if (pbVar9 == (byte *)0x0) {
            return (byte *)0x0;
          }
          bVar6 = *pbVar9;
        }
        while (bVar6 == 0x2c) {
          piVar3 = (int *)FUN_0000c540();
          if (piVar3 == (int *)0x0) {
            return (byte *)0x0;
          }
          pbVar8 = pbVar9 + 1;
          *piVar2 = (int)piVar3;
          piVar3[1] = (int)piVar2;
          if (((pbVar8 != (byte *)0x0) && (pbVar9[1] != 0)) && (pbVar9[1] < 0x21)) {
            pbVar9 = pbVar9 + 2;
            while (pbVar8 = pbVar9, pbVar8 != (byte *)0x0) {
              if ((*pbVar8 == 0) || (pbVar9 = pbVar8 + 1, 0x20 < *pbVar8)) break;
            }
          }
          pbVar9 = (byte *)FUN_0000c244(piVar3,pbVar8);
          if (pbVar9 == (byte *)0x0) {
            return (byte *)0x0;
          }
          bVar6 = *pbVar9;
          while ((bVar6 != 0 && (bVar6 < 0x21))) {
            pbVar9 = pbVar9 + 1;
            if (pbVar9 == (byte *)0x0) {
              return (byte *)0x0;
            }
            bVar6 = *pbVar9;
          }
          iVar1 = piVar3[4];
          piVar3[4] = 0;
          piVar3[8] = iVar1;
          if (*pbVar9 != 0x3a) {
            DAT_0001b3a4 = pbVar9;
            return (byte *)0x0;
          }
          pbVar8 = pbVar9 + 1;
          if (((pbVar8 != (byte *)0x0) && (pbVar9[1] != 0)) && (pbVar9[1] < 0x21)) {
            pbVar9 = pbVar9 + 2;
            while (pbVar8 = pbVar9, pbVar8 != (byte *)0x0) {
              if ((*pbVar8 == 0) || (pbVar9 = pbVar8 + 1, 0x20 < *pbVar8)) break;
            }
          }
          pbVar9 = (byte *)FUN_0000d05c(piVar3,pbVar8);
          if (pbVar9 == (byte *)0x0) {
            return (byte *)0x0;
          }
          bVar6 = *pbVar9;
          while ((bVar6 != 0 && (bVar6 < 0x21))) {
            pbVar9 = pbVar9 + 1;
            if (pbVar9 == (byte *)0x0) {
              return (byte *)0x0;
            }
            bVar6 = *pbVar9;
          }
          piVar2 = piVar3;
        }
        if (bVar6 != 0x7d) {
          DAT_0001b3a4 = pbVar9;
          return (byte *)0x0;
        }
LAB_0000d7bc:
        return pbVar9 + 1;
      }
    }
  }
  return (byte *)0x0;
}

